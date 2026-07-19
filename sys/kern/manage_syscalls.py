#!/usr/bin/env python3
"""
BunnyBSD Syscall and Pledge Manager
"""

import json
import re
import sys
import os

MAP_FILE = "pledge_map.json"

def get_target_paths():
    """
    Dynamically resolves destination paths based on the current working directory,
    allowing the script to run seamlessly from either sys/kern or the sys root.
    """
    current_dir = os.path.basename(os.getcwd())
    
    # Resolve pledge.h path (should go to sys/sys/pledge.h)
    if current_dir == "kern" and os.path.exists("../sys"):
        header_path = "../sys/pledge.h"
    elif os.path.exists("sys/sys"):
        header_path = "sys/sys/pledge.h"
    else:
        header_path = "pledge.h"

    # Resolve sys_pledge.c path (should go to sys/kern/sys_pledge.c)
    if current_dir == "kern":
        c_path = "sys_pledge.c"
    elif os.path.exists("sys/kern"):
        c_path = "sys/kern/sys_pledge.c"
    else:
        c_path = "sys_pledge.c"

    return header_path, c_path

def load_pledge_map():
    """
    Loads reverse pledge mappings from pledge_map.json and returns
    a flattened lookup dictionary: { "syscall_name": ["pledge1", "pledge2", ...] }
    """
    if not os.path.exists(MAP_FILE):
        print(f"Error: Mapping database file '{MAP_FILE}' not found.", file=sys.stderr)
        print("Please ensure it is in the same directory as this script.", file=sys.stderr)
        sys.exit(1)
    
    with open(MAP_FILE, "r", encoding="utf-8") as f:
        try:
            reverse_map = json.load(f)
        except json.JSONDecodeError as e:
            print(f"Error parsing '{MAP_FILE}': {e}", file=sys.stderr)
            sys.exit(1)

    # Convert reverse map (pledge -> [syscalls]) to forward map (syscall -> [pledges])
    forward_map = {}
    for promise, syscalls in reverse_map.items():
        for syscall in syscalls:
            if syscall not in forward_map:
                forward_map[syscall] = []
            
            if promise == "bypass":
                # Maps bypass syscalls to "always" which generates PLEDGE_ALWAYS
                forward_map[syscall].append("always")
            elif promise == "forbidden":
                # Keeps the array empty but marks it as classified to avoid warnings
                pass
            else:
                forward_map[syscall].append(promise)

    return forward_map

def get_sys_const(entry):
    """
    Reconstructs the exact NetBSD C macro name for the system call constant.
    Handles compatibility prefixes, aliases, and triple-underscore (___) stripping.
    """
    sc_type = entry["type"]
    name = entry["name"]
    alias = entry.get("alias", "")
    compat = entry.get("compat", "")

    # Reconstruct the base name following makesyscalls.sh rules
    if alias:
        base = alias
    else:
        if compat:
            # If compat suffix exists, NetBSD's makesyscalls.sh always prepends "__"
            # due to triple-underscore '___' stripping (e.g. sys___socket30 -> __socket30)
            base = f"__{name}{compat}"
        else:
            base = name

    # Construct the prefix based on compatibility options
    if sc_type in ["STD", "NOARGS", "INDIR", "NOERR"]:
        return f"SYS_{base}"
    elif sc_type.startswith("COMPAT_"):
        ver = sc_type.lower()  # e.g., "compat_50"
        return f"SYS_{ver}_{base}"
    else:
        return f"SYS_{base}"

def action_import(master_path):
    """
    Parses a raw syscalls.master file and outputs a clean, structured syscalls.json.
    """
    if not os.path.exists(master_path):
        print(f"Error: Master file '{master_path}' not found.", file=sys.stderr)
        sys.exit(1)

    pledge_map = load_pledge_map()
    print(f"Importing and parsing '{master_path}'...")

    # Join backslash line continuations
    raw_lines = []
    with open(master_path, "r", encoding="utf-8") as f:
        current_line = ""
        for line in f:
            stripped = line.rstrip()
            if stripped.endswith("\\"):
                current_line += stripped[:-1].strip() + " "
            else:
                current_line += line
                raw_lines.append(current_line)
                current_line = ""

    parsed_data = []
    in_table = False
    unclassified_count = 0

    for line in raw_lines:
        line_stripped = line.strip()

        # Handle header section vs syscall table section
        if not in_table:
            if line_stripped == "%%":
                in_table = True
                parsed_data.append({"type": "raw", "text": "%%"})
                continue
            parsed_data.append({"type": "raw", "text": line_stripped})
            continue

        # Handle empty lines, comments, and preprocessor directives
        if not line_stripped:
            parsed_data.append({"type": "raw", "text": ""})
            continue
        if line_stripped.startswith(";") or line_stripped.startswith("#"):
            parsed_data.append({"type": "raw", "text": line_stripped})
            continue

        # Parse active syscalls with curly braces { ... }
        brace_match = re.match(r"^(\d+)\s+([A-Z0-9_]+)\s*(.*?)\{\s*(.*?)\s*;\s*\}\s*(.*)$", line_stripped)
        if brace_match:
            num = int(brace_match.group(1))
            sc_type = brace_match.group(2)
            opts_str = brace_match.group(3).strip()
            proto_str = brace_match.group(4).strip()
            alias = brace_match.group(5).strip()

            entry = {
                "num": num,
                "type": sc_type
            }

            # Parse modular/rump options and compatibility flags
            opts = opts_str.split()
            i = 0
            while i < len(opts):
                opt = opts[i]
                if opt == "MODULAR":
                    if i + 1 < len(opts):
                        entry["modular"] = opts[i+1]
                        i += 2
                    else:
                        entry["modular"] = True
                        i += 1
                elif opt == "RUMP":
                    entry["rump"] = True
                    i += 1
                else:
                    if "compat_flags" not in entry:
                        entry["compat_flags"] = []
                    entry["compat_flags"].append(opt)
                    i += 1

            if alias:
                entry["alias"] = alias

            # Parse and clean the prototype string
            parts = proto_str.split('|')
            if len(parts) >= 4:
                return_type = parts[0].strip()
                compat = parts[2].strip()
                rest = "|".join(parts[3:])

                match = re.match(r"^([a-zA-Z0-9_]+)\((.*)\)$", rest.strip(), re.DOTALL)
                if match:
                    name = match.group(1)
                    args_raw = match.group(2)
                    args_clean = re.sub(r'\s+', ' ', args_raw).strip()

                    entry["return"] = return_type
                    if compat:
                        entry["compat"] = compat
                    entry["name"] = name
                    entry["args"] = args_clean

                    # Strip ONLY NetBSD compatibility version suffixes (prevents wait4/pipe2 over-stripping)
                    base_name = re.sub(r'(09|10|12|13|14|16|20|30|40|43|50|60|90|100)$', '', name.lower())
                    
                    if base_name in pledge_map:
                        entry["pledge"] = pledge_map[base_name]
                    else:
                        # Syscall is not mapped in our dictionary -> Print a warning
                        print(f"WARNING: Syscall '{name}' (num {num}) is unclassified! Not found in '{MAP_FILE}'.", file=sys.stderr)
                        unclassified_count += 1
                        entry["pledge"] = []
            
            parsed_data.append(entry)
        else:
            # Parse obsolete, unimplemented, or ignored syscalls without prototypes
            no_brace_match = re.match(r"^(\d+)\s+([A-Z0-9_]+)\s*(.*)$", line_stripped)
            if no_brace_match:
                num = int(no_brace_match.group(1))
                sc_type = no_brace_match.group(2)
                rest = no_brace_match.group(3).strip()

                entry = {
                    "num": num,
                    "type": sc_type
                }
                if rest:
                    entry["comment"] = rest
                parsed_data.append(entry)
            else:
                # Fallback to raw line preservation
                parsed_data.append({"type": "raw", "text": line_stripped})

    # Save to a single clean syscalls.json
    with open("syscalls.json", "w", encoding="utf-8") as f_json:
        json.dump(parsed_data, f_json, indent=2, ensure_ascii=False)

    print("\nImport completed.")
    if unclassified_count > 0:
        print(f"Notice: Found {unclassified_count} unclassified active system call(s). Check your 'syscalls.json' output.", file=sys.stderr)
    else:
        print("All active system calls successfully classified.")

def action_export():
    """
    Generates syscalls.master, pledge.h, and sys_pledge.c from syscalls.json.
    """
    if not os.path.exists("syscalls.json"):
        print("Error: 'syscalls.json' not found. Run 'import' first.", file=sys.stderr)
        sys.exit(1)

    header_path, c_path = get_target_paths()
    print(f"Exporting database to build files...")

    # Load raw pledge map to find promises for pledge.h generation
    with open(MAP_FILE, "r", encoding="utf-8") as f:
        try:
            raw_map = json.load(f)
        except Exception as e:
            print(f"Error reading '{MAP_FILE}': {e}", file=sys.stderr)
            sys.exit(1)

    # Exclude system reserved keywords (bypass, forbidden, error) from dynamic bit generation
    promises = [k for k in raw_map.keys() if k not in ["bypass", "forbidden", "error"]]
    promises.sort()  # Keep defines sorted deterministically

    dynamic_defines = []
    for idx, p in enumerate(promises, start=1):
        macro = f"PLEDGE_{p.upper()}"
        dynamic_defines.append(f"#define {macro:<20} (1ULL << {idx})")
        
    dynamic_defines_str = "\n".join(dynamic_defines)

    # Construct the final pledge.h structure preserving license and code guards
    pledge_h_template = f"""/*-
 * Copyright (c) 2026 Karina Karter
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once
#include <sys/stdbool.h>

#if defined(_KERNEL) && !defined(_RUMPKERNEL) && !defined(_MODULE)
#include "opt_pledge.h"
#endif

#define PLEDGE_MAX_PROMISES_LEN 1024

#define PLEDGE_ALWAYS        (0xffffffffffffffffULL)
#define PLEDGE_ERROR         (1ULL << 0)
{dynamic_defines_str}


#ifdef _KERNEL
#ifdef PLEDGE

struct lwp;
bool pledge_check(struct lwp *, int);

int pledge_open_check(struct lwp *l, int flags);
int pledge_socket_check(struct lwp *l, int domain);
int pledge_ioctl_check(struct lwp *l, unsigned long com);
int pledge_sendit_check(struct lwp *l, const void *user_addr);
int pledge_fcntl_check(struct lwp *l, int cmd);
int pledge_sysctl_check(struct lwp *l, unsigned int *user_name, unsigned int namelen);

struct pledge_promise {{
    const char *name;
    uint64_t mask;
}};

extern const struct pledge_promise pledge_promises[];

#endif /* PLEDGE */
#else /* _KERNEL */

#include <sys/cdefs.h>
__BEGIN_DECLS
int pledge(const char *, const char *);
__END_DECLS

#endif
"""

    # Write pledge.h
    with open(header_path, "w", encoding="utf-8") as f_h:
        f_h.write(pledge_h_template)

    with open("syscalls.json", "r", encoding="utf-8") as f:
        data = json.load(f)

    # 2. Regenerate syscalls.master
    with open("syscalls.master", "w", encoding="utf-8") as f_master:
        for entry in data:
            if entry.get("type") == "raw":
                f_master.write(f"{entry['text']}\n")
                continue

            num = entry["num"]
            sc_type = entry["type"]

            # Format optional flags
            opts = []
            if entry.get("modular"):
                opts.append(f"MODULAR {entry['modular']}")
            if entry.get("rump"):
                opts.append("RUMP")
            if entry.get("compat_flags"):
                opts.extend(entry["compat_flags"])

            opt_str = "\t".join(opts)
            if opt_str:
                opt_str = "\t" + opt_str

            if "return" in entry:
                compat = entry.get("compat", "")
                proto_str = f"{entry['return']}|sys|{compat}|{entry['name']}({entry['args']})"
                alias = entry.get("alias", "")
                if alias:
                    alias = f" {alias}"
                f_master.write(f"{num}\t{sc_type}{opt_str}\t{{ {proto_str}; }}{alias}\n")
            else:
                comment = entry.get("comment", "")
                if comment:
                    f_master.write(f"{num}\t{sc_type}\t\t{comment}\n")
                else:
                    f_master.write(f"{num}\t{sc_type}\n")

    # 3. Regenerate sys_pledge.c
    with open(c_path, "w", encoding="utf-8") as f_pledge:
        f_pledge.write("/* Generated from syscalls.json -- DO NOT EDIT */\n\n")
        f_pledge.write("#include <sys/param.h>\n")  # Fixed NULL declaration
        f_pledge.write("#include <sys/types.h>\n")
        f_pledge.write("#include <sys/syscall.h>\n")
        f_pledge.write("#include <sys/pledge.h>\n\n")
        f_pledge.write("const uint64_t pledge_syscalls[SYS_NSYSENT] = {\n")

        for entry in data:
            if entry.get("type") == "raw" or "pledge" not in entry:
                continue

            pledge_list = entry["pledge"]
            if not pledge_list:
                continue

            macros = [f"PLEDGE_{p.upper()}" for p in pledge_list]
            mask_expr = " | ".join(macros)

            # Reconstruct the precise NetBSD syscall macro name (e.g. SYS_compat_43_oaccept)
            sys_const = get_sys_const(entry)
            
            f_pledge.write(f"#ifdef {sys_const}\n")
            f_pledge.write(f"    [{sys_const}] = {mask_expr},\n")
            f_pledge.write(f"#endif\n")

        f_pledge.write("};\n\n")
        
        # Write the dynamically generated mapping array of promises to masks
        f_pledge.write("const struct pledge_promise pledge_promises[] = {\n")
        f_pledge.write('    { "error", PLEDGE_ERROR },\n')
        for p in promises:
            f_pledge.write(f'    {{ "{p}", PLEDGE_{p.upper()} }},\n')
        f_pledge.write('    { NULL, 0 }\n')
        f_pledge.write("};\n")

    print(f"Success! Generated '{header_path}', 'syscalls.master' and '{c_path}'.")

if __name__ == "__main__":
    if len(sys.argv) < 2 or sys.argv[1] not in ["import", "export"]:
        print("Usage:", file=sys.stderr)
        print("  python3 manage_syscalls.py import <master_file>   - Parse .master to syscalls.json", file=sys.stderr)
        print("  python3 manage_syscalls.py export                 - Rebuild build files from syscalls.json", file=sys.stderr)
        sys.exit(1)

    command = sys.argv[1]
    if command == "import":
        if len(sys.argv) < 3:
            print("Error: Missing path to the .master file.", file=sys.stderr)
            sys.exit(1)
        action_import(sys.argv[2])
    elif command == "export":
        action_export()