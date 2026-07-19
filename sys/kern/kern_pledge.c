/*-
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

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$BunnyBSD$");
#include "opt_pledge.h"

#include <sys/proc.h>
#include <sys/systm.h>
#include <sys/syscall.h>
#include <sys/syscallargs.h>
#include <sys/pledge.h>
#include <sys/stdbool.h>

#ifdef PLEDGE

extern const uint64_t pledge_syscalls[SYS_NSYSENT];

/*
 * Helper to check
 */

bool
pledge_check(struct lwp *l, int code)
{
    if (code < 0 || code >= SYS_NSYSENT)
        return false;

    uint64_t pledge = pledge_syscalls[code];
    uint64_t mask = l->l_proc->p_pledge;

    if (pledge == PLEDGE_ALWAYS)
        return true;

    if (mask == 0)
        return false;

    if ((mask & pledge) != 0)
        return true;

    return false;
}

/*
 * parses pledges 0 on success EINVAL if unknown promise
 */
static int
pledge_parse(const char *promises_str, uint64_t *out_mask)
{
    char buf[128];
    char *p, *token;
    uint64_t mask = 0;
    bool found;

    strlcpy(buf, promises_str, sizeof(buf));
    p = buf;

    while ((token = strsep(&p, " \t\r\n")) != NULL) {
        if (*token == '\0')
            continue;

        found = false;
        for (size_t i = 0; pledge_promises[i].name != NULL; i++) {
            if (strcmp(token, pledge_promises[i].name) == 0) {
                mask |= pledge_promises[i].mask;
                found = true;
                break;
            }
        }

        if (!found) {
            return EINVAL;
        }
    }

    *out_mask = mask;
    return 0;
}

/*
 * pledge(2)
 */
int
sys_pledge(struct lwp *l, const struct sys_pledge_args *uap, register_t *retval)
{
    char buf[128];
    size_t done;
    int error;
    uint64_t new_mask = 0;

    if (SCARG(uap, execpromises) != NULL) return ENOSYS; /* execpromises not implemented yet*/

    if (SCARG(uap, promises) != NULL) {
        error = copyinstr(SCARG(uap, promises), buf, sizeof(buf), &done);
        if (error)
            return error;

        error = pledge_parse(buf, &new_mask);
        if (error)
            return error;

        /* proc can only drop pledges */
        if (l->l_proc->p_pledged) {
            if ((new_mask & ~l->l_proc->p_pledge) != 0) {
                return EPERM;
            }
        }

        l->l_proc->p_pledge = new_mask;
        l->l_proc->p_pledged = true;
    }

    return 0;
}

#else /* !PLEDGE */

/*
 * stub for kernels with no pledge(2)
 */
int
sys_pledge(struct lwp *l, const struct sys_pledge_args *uap, register_t *retval)
{
	return ENOSYS;
}

#endif /* PLEDGE */
