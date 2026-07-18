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
/*
 * Helper to check (obviosly)
 * TODO: when building good pledge (Not POC) rewrite it completely and nicely
 */

bool
pledge_check(struct lwp *l, int code) {
    uint64_t mask = l->l_proc->p_pledge;

    if (code == SYS_exit || code == SYS_pledge)
        return true;

    if (mask & PLEDGE_STDIO) {
        if (code == SYS_read || code == SYS_write || code == SYS_close)
			return true;
    }

   	if (mask & PLEDGE_RPATH) {
		if (code == SYS_open ||
		    code == SYS___stat50 ||
		    code == SYS___fstat50 ||
		    code == SYS___lstat50)
			return true;
	}

    return false;
}

/*
 * pledge(2)
 */
int
sys_pledge(struct lwp *l, const struct sys_pledge_args *uap, register_t *retval) {
    char buf[128];
    size_t done;
    int error;

    error = copyinstr(SCARG(uap, promises), buf, sizeof(buf), &done);
    if (error)
        return error;

    /*
     * TODO: rewrite parser after POC
     */
    uint64_t new_mask = 0;
    if (strstr(buf, "stdio") != NULL)
        new_mask |= PLEDGE_STDIO;

    if (strstr(buf, "rpath") != NULL)
        new_mask |= PLEDGE_RPATH;

    if (l->l_proc->p_pledged) {
        if ((new_mask & ~l->l_proc->p_pledge) != 0) {
            return EPERM;
        }
    }

    l->l_proc->p_pledge = new_mask;
    l->l_proc->p_pledged = true;

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
