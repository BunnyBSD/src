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
#include <sys/socket.h>
#include <sys/pledge.h>
#include <sys/stdbool.h>
#include <sys/fcntl.h>
#include <sys/sysctl.h>

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
    struct proc *p = l->l_proc;

    mutex_enter(p->p_lock);
    uint64_t mask = p->p_pledge;
    mutex_exit(p->p_lock);

    if (pledge == PLEDGE_ALWAYS)
        return true;

    if (mask == 0)
        return false;

    if ((mask & pledge) != 0)
        return true;

    return false;
}

/* gatekeeper for open(2) with pledge */
int 
pledge_open_check(struct lwp *l, int flags) 
{
    struct proc *p = l->l_proc;
    bool pledged;
    uint64_t mask;

    mutex_enter(p->p_lock);
    pledged = p->p_pledged;
    mask = p->p_pledge;
    mutex_exit(p->p_lock);

    if (!pledged)
        return 0;

    /* O_CREATE require cpath */
    if (flags & O_CREAT) {
        if ((mask & PLEDGE_CPATH) == 0) {
            return EPERM;
        }
    }

    /* O_WRONLY, O_RDWR, O_TRUNC require wpath */
    if ((flags & (O_WRONLY | O_RDWR | O_TRUNC)) != 0) {
        if ((mask & PLEDGE_WPATH) == 0) {
            return EPERM;
        }
    }

    /* any read require RPATH and check that its not O_WRONLY */
    if ((flags & O_ACCMODE) != O_WRONLY) {
        if ((mask & PLEDGE_RPATH) == 0) {
            return EPERM;
        }
    }

    return 0;
}

/* gatekeeper for socket(2) with pledge */
int
pledge_socket_check(struct lwp *l, int domain)
{
    struct proc *p = l->l_proc;
    bool pledged;
    uint64_t mask;

    mutex_enter(p->p_lock);
    pledged = p->p_pledged;
    mask = p->p_pledge;
    mutex_exit(p->p_lock);

    if (!pledged)
        return 0;

    if (domain == AF_INET || domain == AF_INET6) {
        if ((mask & PLEDGE_INET) == 0) {
            return EPERM;
        }
    }

    else if (domain == AF_UNIX) {
        if ((mask & PLEDGE_UNIX) == 0) {
            return EPERM;
        }
    }

    else {
        return EPERM;
    }

    return 0;
}

/* gatekeeper for ioctl(2) syscall*/
int
pledge_ioctl_check(struct lwp *l, unsigned long com) 
{
    struct proc *p = l->l_proc;
    bool pledged;
    uint64_t mask;

    mutex_enter(p->p_lock);
    pledged = p->p_pledged;
    mask = p->p_pledge;
    mutex_exit(p->p_lock);

    if (!pledged)
        return 0;

    unsigned char group = (com >> 8) & 0xff;

    switch (group) {
        case 't': /* tty ioctl commands (TIOC...) */
            if ((mask & PLEDGE_TTY) == 0) {
                return EPERM;
            }
            break;
        case 'i': /* socket/network ioctl commands (SIOC...) */
            if ((mask & (PLEDGE_INET | PLEDGE_UNIX)) == 0) {
                return EPERM;
            }
            break;
        case 'A': /* audio device ioctl commands (AUDIO_) */
        case 'a':
            if ((mask & PLEDGE_AUDIO) == 0) {
                return EPERM;
            }
            break;
        case 'f': /* generic file/descriptor ops (FIONBIO, FIONREAD, etc..) */
            /* always allowed if proc has any basic IO privs*/
            break;
        default: /* block unknown if pledged*/
            return EPERM;
    }

    return 0;
}

/* gatekeeper for sendit(2) syscall*/
int 
pledge_sendit_check(struct lwp *l, const struct sockaddr *sa)
{
    struct proc *p = l->l_proc;
    bool pledged;
    uint64_t mask;

    mutex_enter(p->p_lock);
    pledged = p->p_pledged;
    mask = p->p_pledge;
    mutex_exit(p->p_lock);

    if (!pledged)
        return 0;

    if (sa == NULL)
        return 0;

    uint8_t family = sa->sa_family;

    if (family == AF_INET || family == AF_INET6) {
        if ((mask & PLEDGE_INET) == 0) {
            return EPERM;
        }
    }
    
    else if (family == AF_UNIX) {
        if ((mask & PLEDGE_UNIX) == 0) {
            return EPERM;
        }
    }

    else {
        return EPERM;
    }

    return 0;
}

/* gatekeeper for fcntl(2) syscall*/
int
pledge_fcntl_check(struct lwp *l, int cmd)
{
    struct proc *p = l->l_proc;
    bool pledged;
    uint64_t mask;

    mutex_enter(p->p_lock);
    pledged = p->p_pledged;
    mask = p->p_pledge;
    mutex_exit(p->p_lock);

    if (!pledged)
        return 0;

    switch (cmd) {
        case F_GETFD:
        case F_SETFD:
        case F_GETFL:
        case F_SETFL:
        case F_DUPFD:
        case F_DUPFD_CLOEXEC:
        case F_DUPFD_CLOFORK:
        case F_DUPFD_CLOBOTH:
            return 0; /* basic desciptors allowed for stdio */
        case F_GETLK:
        case F_SETLK:
        case F_SETLKW:
            /* require PLEDGE_FLOCK for locking files*/
            if ((mask & PLEDGE_FLOCK) == 0) {
                return EPERM;
            }
            return 0;
        default:
            return EPERM;
    }
}

/* gatekeeper for sysctl(2) syscall*/
int
pledge_sysctl_check(struct lwp *l, const int *user_name, unsigned int namelen)
{
    struct proc *p = l->l_proc;
    bool pledged;
    uint64_t mask;

    mutex_enter(p->p_lock);
    pledged = p->p_pledged;
    mask = p->p_pledge;
    mutex_exit(p->p_lock);

    if (!pledged)
        return 0;

    if (mask & PLEDGE_SYS_INFO) {
        if (namelen == 0 || namelen > CTL_MAXNAME) {
            return EPERM;
        }

        if (namelen >= 2 && user_name[0] == CTL_KERN) {
            int second = user_name[1];
            if (second == KERN_HOSTNAME || second == KERN_DOMAINNAME ||
                second == KERN_BOOTTIME || second == KERN_OSTYPE ||
                second == KERN_OSRELEASE || second == KERN_VERSION) {
                    return 0;
                }
        }

        if (namelen >= 1 && user_name[0] == CTL_HW) {
            return 0;
        }
    }

    return EPERM;
}

/*
 * parses pledges 0 on success EINVAL if unknown promise
 */
static int
pledge_parse(const char *promises_str, uint64_t *out_mask)
{
    uint64_t mask = 0;
    const char *p = promises_str;

    while (*p != '\0') {
        if (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
            p++;
            continue;
        }

        const char *token_start = p;
        while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '\r' && *p != '\n') {
            p++;
        }
        size_t token_len = p - token_start;

        if (token_len == 0) {
            continue;
        }

        /* searching tok in promises array*/
        bool found = false;
        for (size_t i = 0; pledge_promises[i].name != NULL; i++) {
            /* 
             * checking len and containments here to exclude collisions 
             */
            if (strncmp(token_start, pledge_promises[i].name, token_len) == 0 &&
                pledge_promises[i].name[token_len] == '\0') {
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
    char buf[PLEDGE_MAX_PROMISES_LEN];
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


        struct proc *p = l->l_proc;
        mutex_enter(p->p_lock);

        /* proc can only drop pledges */
        if (p->p_pledged) {
            if ((new_mask & ~p->p_pledge) != 0) {
                mutex_exit(p->p_lock);
                return EPERM;
            }
        }

        p->p_pledge = new_mask;
        p->p_pledged = true;

        mutex_exit(p->p_lock);
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
