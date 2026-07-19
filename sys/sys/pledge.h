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

#pragma once
#include <sys/stdbool.h>

#if defined(_KERNEL) && !defined(_RUMPKERNEL) && !defined(_MODULE)
#include "opt_pledge.h"
#endif

#define PLEDGE_MAX_PROMISES_LEN 1024

#define PLEDGE_ALWAYS        (0xffffffffffffffffULL)
#define PLEDGE_ERROR         (1ULL << 0)
#define PLEDGE_AIO           (1ULL << 1)
#define PLEDGE_AUDIO         (1ULL << 2)
#define PLEDGE_CHOWN         (1ULL << 3)
#define PLEDGE_CPATH         (1ULL << 4)
#define PLEDGE_ENTROPY       (1ULL << 5)
#define PLEDGE_EXEC          (1ULL << 6)
#define PLEDGE_FATTR         (1ULL << 7)
#define PLEDGE_FILE_METADATA_READ (1ULL << 8)
#define PLEDGE_FLOCK         (1ULL << 9)
#define PLEDGE_FS_SYNC       (1ULL << 10)
#define PLEDGE_IDENTITY_GET  (1ULL << 11)
#define PLEDGE_IDENTITY_SET  (1ULL << 12)
#define PLEDGE_INET          (1ULL << 13)
#define PLEDGE_MEMORY_MGMT   (1ULL << 14)
#define PLEDGE_MULTIPLEX     (1ULL << 15)
#define PLEDGE_PROCESS_LIFECYCLE (1ULL << 16)
#define PLEDGE_PROCESS_META  (1ULL << 17)
#define PLEDGE_RPATH         (1ULL << 18)
#define PLEDGE_SIGNAL        (1ULL << 19)
#define PLEDGE_STDIO         (1ULL << 20)
#define PLEDGE_SYNCHRONIZATION (1ULL << 21)
#define PLEDGE_SYS_INFO      (1ULL << 22)
#define PLEDGE_SYSV_IPC      (1ULL << 23)
#define PLEDGE_THREAD_LWP    (1ULL << 24)
#define PLEDGE_TIME_READ     (1ULL << 25)
#define PLEDGE_TIME_SET      (1ULL << 26)
#define PLEDGE_TIMERS_AND_SLEEP (1ULL << 27)
#define PLEDGE_TTY           (1ULL << 28)
#define PLEDGE_UNIX          (1ULL << 29)
#define PLEDGE_WPATH         (1ULL << 30)


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

struct pledge_promise {
    const char *name;
    uint64_t mask;
};

extern const struct pledge_promise pledge_promises[];

#endif /* PLEDGE */
#else /* _KERNEL */

#include <sys/cdefs.h>
__BEGIN_DECLS
int pledge(const char *, const char *);
__END_DECLS

#endif
