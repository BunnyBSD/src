/* Generated from syscalls.json -- DO NOT EDIT */

#include <sys/param.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/pledge.h>

#ifdef PLEDGE

const uint64_t pledge_syscalls[SYS_NSYSENT] = {
#ifdef SYS_exit
    [SYS_exit] = PLEDGE_ALWAYS,
#endif
#ifdef SYS_fork
    [SYS_fork] = PLEDGE_PROCESS_LIFECYCLE,
#endif
#ifdef SYS_read
    [SYS_read] = PLEDGE_STDIO,
#endif
#ifdef SYS_write
    [SYS_write] = PLEDGE_STDIO,
#endif
#ifdef SYS_open
    [SYS_open] = PLEDGE_RPATH | PLEDGE_WPATH | PLEDGE_CPATH,
#endif
#ifdef SYS_close
    [SYS_close] = PLEDGE_STDIO,
#endif
#ifdef SYS_compat_50_wait4
    [SYS_compat_50_wait4] = PLEDGE_PROCESS_LIFECYCLE,
#endif
#ifdef SYS_compat_43_ocreat
    [SYS_compat_43_ocreat] = PLEDGE_FATTR | PLEDGE_WPATH | PLEDGE_CPATH,
#endif
#ifdef SYS_link
    [SYS_link] = PLEDGE_CPATH,
#endif
#ifdef SYS_unlink
    [SYS_unlink] = PLEDGE_CPATH,
#endif
#ifdef SYS_chdir
    [SYS_chdir] = PLEDGE_RPATH,
#endif
#ifdef SYS_fchdir
    [SYS_fchdir] = PLEDGE_RPATH,
#endif
#ifdef SYS_compat_50_mknod
    [SYS_compat_50_mknod] = PLEDGE_CPATH,
#endif
#ifdef SYS_chmod
    [SYS_chmod] = PLEDGE_FATTR,
#endif
#ifdef SYS_chown
    [SYS_chown] = PLEDGE_CHOWN,
#endif
#ifdef SYS_compat_20_getfsstat
    [SYS_compat_20_getfsstat] = PLEDGE_RPATH,
#endif
#ifdef SYS_compat_43_olseek
    [SYS_compat_43_olseek] = PLEDGE_STDIO,
#endif
#ifdef SYS_getpid
    [SYS_getpid] = PLEDGE_IDENTITY_GET,
#endif
#ifdef SYS_setuid
    [SYS_setuid] = PLEDGE_IDENTITY_SET,
#endif
#ifdef SYS_getuid
    [SYS_getuid] = PLEDGE_IDENTITY_GET,
#endif
#ifdef SYS_geteuid
    [SYS_geteuid] = PLEDGE_IDENTITY_GET,
#endif
#ifdef SYS_recvmsg
    [SYS_recvmsg] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_sendmsg
    [SYS_sendmsg] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_recvfrom
    [SYS_recvfrom] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_accept
    [SYS_accept] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_getpeername
    [SYS_getpeername] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_getsockname
    [SYS_getsockname] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_access
    [SYS_access] = PLEDGE_RPATH,
#endif
#ifdef SYS_chflags
    [SYS_chflags] = PLEDGE_FATTR,
#endif
#ifdef SYS_fchflags
    [SYS_fchflags] = PLEDGE_FATTR,
#endif
#ifdef SYS_sync
    [SYS_sync] = PLEDGE_FS_SYNC,
#endif
#ifdef SYS_kill
    [SYS_kill] = PLEDGE_PROCESS_LIFECYCLE,
#endif
#ifdef SYS_compat_43_stat43
    [SYS_compat_43_stat43] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS_getppid
    [SYS_getppid] = PLEDGE_IDENTITY_GET,
#endif
#ifdef SYS_compat_43_lstat43
    [SYS_compat_43_lstat43] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS_dup
    [SYS_dup] = PLEDGE_STDIO,
#endif
#ifdef SYS_pipe
    [SYS_pipe] = PLEDGE_STDIO,
#endif
#ifdef SYS_getegid
    [SYS_getegid] = PLEDGE_IDENTITY_GET,
#endif
#ifdef SYS_compat_13_sigaction13
    [SYS_compat_13_sigaction13] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_getgid
    [SYS_getgid] = PLEDGE_IDENTITY_GET,
#endif
#ifdef SYS_compat_13_sigprocmask13
    [SYS_compat_13_sigprocmask13] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_compat_13_sigpending13
    [SYS_compat_13_sigpending13] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_compat_13_sigaltstack13
    [SYS_compat_13_sigaltstack13] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_ioctl
    [SYS_ioctl] = PLEDGE_TTY | PLEDGE_INET | PLEDGE_AUDIO,
#endif
#ifdef SYS_symlink
    [SYS_symlink] = PLEDGE_CPATH,
#endif
#ifdef SYS_readlink
    [SYS_readlink] = PLEDGE_RPATH,
#endif
#ifdef SYS_execve
    [SYS_execve] = PLEDGE_EXEC,
#endif
#ifdef SYS_umask
    [SYS_umask] = PLEDGE_FATTR,
#endif
#ifdef SYS_compat_43_fstat43
    [SYS_compat_43_fstat43] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS_compat_43_ogetpagesize
    [SYS_compat_43_ogetpagesize] = PLEDGE_SYS_INFO,
#endif
#ifdef SYS_compat_12_msync
    [SYS_compat_12_msync] = PLEDGE_MEMORY_MGMT,
#endif
#ifdef SYS_vfork
    [SYS_vfork] = PLEDGE_PROCESS_LIFECYCLE,
#endif
#ifdef SYS_compat_43_ommap
    [SYS_compat_43_ommap] = PLEDGE_MEMORY_MGMT,
#endif
#ifdef SYS_munmap
    [SYS_munmap] = PLEDGE_MEMORY_MGMT,
#endif
#ifdef SYS_mprotect
    [SYS_mprotect] = PLEDGE_MEMORY_MGMT,
#endif
#ifdef SYS_madvise
    [SYS_madvise] = PLEDGE_MEMORY_MGMT,
#endif
#ifdef SYS_mincore
    [SYS_mincore] = PLEDGE_MEMORY_MGMT,
#endif
#ifdef SYS_getgroups
    [SYS_getgroups] = PLEDGE_IDENTITY_GET,
#endif
#ifdef SYS_setgroups
    [SYS_setgroups] = PLEDGE_IDENTITY_SET,
#endif
#ifdef SYS_getpgrp
    [SYS_getpgrp] = PLEDGE_IDENTITY_GET,
#endif
#ifdef SYS_setpgid
    [SYS_setpgid] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS_compat_50_setitimer
    [SYS_compat_50_setitimer] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_compat_43_owait
    [SYS_compat_43_owait] = PLEDGE_PROCESS_LIFECYCLE,
#endif
#ifdef SYS_compat_50_getitimer
    [SYS_compat_50_getitimer] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_compat_43_ogethostname
    [SYS_compat_43_ogethostname] = PLEDGE_SYS_INFO,
#endif
#ifdef SYS_compat_43_ogetdtablesize
    [SYS_compat_43_ogetdtablesize] = PLEDGE_SYS_INFO,
#endif
#ifdef SYS_dup2
    [SYS_dup2] = PLEDGE_STDIO,
#endif
#ifdef SYS_getrandom
    [SYS_getrandom] = PLEDGE_ENTROPY,
#endif
#ifdef SYS_fcntl
    [SYS_fcntl] = PLEDGE_STDIO | PLEDGE_FLOCK,
#endif
#ifdef SYS_compat_50_select
    [SYS_compat_50_select] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS_fsync
    [SYS_fsync] = PLEDGE_STDIO,
#endif
#ifdef SYS_setpriority
    [SYS_setpriority] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS_compat_30_socket
    [SYS_compat_30_socket] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_connect
    [SYS_connect] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_compat_43_oaccept
    [SYS_compat_43_oaccept] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_getpriority
    [SYS_getpriority] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS_compat_43_osend
    [SYS_compat_43_osend] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_compat_43_orecv
    [SYS_compat_43_orecv] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_compat_13_sigreturn13
    [SYS_compat_13_sigreturn13] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_bind
    [SYS_bind] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_setsockopt
    [SYS_setsockopt] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_listen
    [SYS_listen] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_compat_43_osigvec
    [SYS_compat_43_osigvec] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_compat_43_osigblock
    [SYS_compat_43_osigblock] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_compat_43_osigsetmask
    [SYS_compat_43_osigsetmask] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_compat_13_sigsuspend13
    [SYS_compat_13_sigsuspend13] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_compat_43_osigstack
    [SYS_compat_43_osigstack] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_compat_43_orecvmsg
    [SYS_compat_43_orecvmsg] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_compat_43_osendmsg
    [SYS_compat_43_osendmsg] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_compat_50_gettimeofday
    [SYS_compat_50_gettimeofday] = PLEDGE_TIME_READ,
#endif
#ifdef SYS_compat_50_getrusage
    [SYS_compat_50_getrusage] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS_getsockopt
    [SYS_getsockopt] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_readv
    [SYS_readv] = PLEDGE_STDIO,
#endif
#ifdef SYS_writev
    [SYS_writev] = PLEDGE_WPATH,
#endif
#ifdef SYS_compat_50_settimeofday
    [SYS_compat_50_settimeofday] = PLEDGE_TIME_SET,
#endif
#ifdef SYS_fchown
    [SYS_fchown] = PLEDGE_CHOWN,
#endif
#ifdef SYS_fchmod
    [SYS_fchmod] = PLEDGE_FATTR,
#endif
#ifdef SYS_compat_43_orecvfrom
    [SYS_compat_43_orecvfrom] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_setreuid
    [SYS_setreuid] = PLEDGE_IDENTITY_SET,
#endif
#ifdef SYS_setregid
    [SYS_setregid] = PLEDGE_IDENTITY_SET,
#endif
#ifdef SYS_rename
    [SYS_rename] = PLEDGE_CPATH,
#endif
#ifdef SYS_compat_43_otruncate
    [SYS_compat_43_otruncate] = PLEDGE_WPATH,
#endif
#ifdef SYS_compat_43_oftruncate
    [SYS_compat_43_oftruncate] = PLEDGE_WPATH,
#endif
#ifdef SYS_flock
    [SYS_flock] = PLEDGE_FLOCK,
#endif
#ifdef SYS_mkfifo
    [SYS_mkfifo] = PLEDGE_CPATH,
#endif
#ifdef SYS_sendto
    [SYS_sendto] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_shutdown
    [SYS_shutdown] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_socketpair
    [SYS_socketpair] = PLEDGE_UNIX,
#endif
#ifdef SYS_mkdir
    [SYS_mkdir] = PLEDGE_CPATH,
#endif
#ifdef SYS_rmdir
    [SYS_rmdir] = PLEDGE_CPATH,
#endif
#ifdef SYS_compat_50_utimes
    [SYS_compat_50_utimes] = PLEDGE_FATTR,
#endif
#ifdef SYS_compat_50_adjtime
    [SYS_compat_50_adjtime] = PLEDGE_TIME_SET,
#endif
#ifdef SYS_compat_43_ogetpeername
    [SYS_compat_43_ogetpeername] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_compat_43_ogethostid
    [SYS_compat_43_ogethostid] = PLEDGE_SYS_INFO,
#endif
#ifdef SYS_compat_43_ogetrlimit
    [SYS_compat_43_ogetrlimit] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS_compat_43_osetrlimit
    [SYS_compat_43_osetrlimit] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS_compat_43_okillpg
    [SYS_compat_43_okillpg] = PLEDGE_PROCESS_LIFECYCLE,
#endif
#ifdef SYS_setsid
    [SYS_setsid] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS_compat_43_ogetsockname
    [SYS_compat_43_ogetsockname] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_compat_43_ogetdirentries
    [SYS_compat_43_ogetdirentries] = PLEDGE_RPATH,
#endif
#ifdef SYS_compat_20_statfs
    [SYS_compat_20_statfs] = PLEDGE_RPATH,
#endif
#ifdef SYS_compat_20_fstatfs
    [SYS_compat_20_fstatfs] = PLEDGE_RPATH,
#endif
#ifdef SYS_compat_09_ouname
    [SYS_compat_09_ouname] = PLEDGE_SYS_INFO,
#endif
#ifdef SYS_sysarch
    [SYS_sysarch] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS___futex
    [SYS___futex] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS___futex_set_robust_list
    [SYS___futex_set_robust_list] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS___futex_get_robust_list
    [SYS___futex_get_robust_list] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS_pread
    [SYS_pread] = PLEDGE_STDIO,
#endif
#ifdef SYS_pwrite
    [SYS_pwrite] = PLEDGE_WPATH,
#endif
#ifdef SYS_timerfd_create
    [SYS_timerfd_create] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_timerfd_settime
    [SYS_timerfd_settime] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_timerfd_gettime
    [SYS_timerfd_gettime] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_setgid
    [SYS_setgid] = PLEDGE_IDENTITY_SET,
#endif
#ifdef SYS_setegid
    [SYS_setegid] = PLEDGE_IDENTITY_SET,
#endif
#ifdef SYS_seteuid
    [SYS_seteuid] = PLEDGE_IDENTITY_SET,
#endif
#ifdef SYS_compat_12_stat12
    [SYS_compat_12_stat12] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS_compat_12_fstat12
    [SYS_compat_12_fstat12] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS_compat_12_lstat12
    [SYS_compat_12_lstat12] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS_pathconf
    [SYS_pathconf] = PLEDGE_RPATH,
#endif
#ifdef SYS_fpathconf
    [SYS_fpathconf] = PLEDGE_SYS_INFO,
#endif
#ifdef SYS_getsockopt2
    [SYS_getsockopt2] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_getrlimit
    [SYS_getrlimit] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS_setrlimit
    [SYS_setrlimit] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS_compat_12_getdirentries
    [SYS_compat_12_getdirentries] = PLEDGE_RPATH,
#endif
#ifdef SYS_mmap
    [SYS_mmap] = PLEDGE_MEMORY_MGMT,
#endif
#ifdef SYS_lseek
    [SYS_lseek] = PLEDGE_STDIO,
#endif
#ifdef SYS_truncate
    [SYS_truncate] = PLEDGE_WPATH,
#endif
#ifdef SYS_ftruncate
    [SYS_ftruncate] = PLEDGE_WPATH,
#endif
#ifdef SYS___sysctl
    [SYS___sysctl] = PLEDGE_SYS_INFO,
#endif
#ifdef SYS_undelete
    [SYS_undelete] = PLEDGE_CPATH,
#endif
#ifdef SYS_compat_50_futimes
    [SYS_compat_50_futimes] = PLEDGE_FATTR,
#endif
#ifdef SYS_getpgid
    [SYS_getpgid] = PLEDGE_IDENTITY_GET,
#endif
#ifdef SYS_poll
    [SYS_poll] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS_semget
    [SYS_semget] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_semop
    [SYS_semop] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_semconfig
    [SYS_semconfig] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_compat_14_msgctl
    [SYS_compat_14_msgctl] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_msgget
    [SYS_msgget] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_msgsnd
    [SYS_msgsnd] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_msgrcv
    [SYS_msgrcv] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_shmat
    [SYS_shmat] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_compat_14_shmctl
    [SYS_compat_14_shmctl] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_shmdt
    [SYS_shmdt] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_shmget
    [SYS_shmget] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_compat_50_clock_gettime
    [SYS_compat_50_clock_gettime] = PLEDGE_TIME_READ,
#endif
#ifdef SYS_compat_50_clock_settime
    [SYS_compat_50_clock_settime] = PLEDGE_TIME_SET,
#endif
#ifdef SYS_compat_50_clock_getres
    [SYS_compat_50_clock_getres] = PLEDGE_TIME_READ,
#endif
#ifdef SYS_timer_create
    [SYS_timer_create] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_timer_delete
    [SYS_timer_delete] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_compat_50_timer_settime
    [SYS_compat_50_timer_settime] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_compat_50_timer_gettime
    [SYS_compat_50_timer_gettime] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_timer_getoverrun
    [SYS_timer_getoverrun] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_compat_50_nanosleep
    [SYS_compat_50_nanosleep] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_fdatasync
    [SYS_fdatasync] = PLEDGE_STDIO,
#endif
#ifdef SYS_compat_50___sigtimedwait
    [SYS_compat_50___sigtimedwait] = PLEDGE_SIGNAL,
#endif
#ifdef SYS__ksem_init
    [SYS__ksem_init] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS__ksem_open
    [SYS__ksem_open] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS__ksem_unlink
    [SYS__ksem_unlink] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS__ksem_close
    [SYS__ksem_close] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS__ksem_post
    [SYS__ksem_post] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS__ksem_wait
    [SYS__ksem_wait] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS__ksem_trywait
    [SYS__ksem_trywait] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS__ksem_getvalue
    [SYS__ksem_getvalue] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS__ksem_destroy
    [SYS__ksem_destroy] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS__ksem_timedwait
    [SYS__ksem_timedwait] = PLEDGE_SYNCHRONIZATION,
#endif
#ifdef SYS_eventfd
    [SYS_eventfd] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS___posix_rename
    [SYS___posix_rename] = PLEDGE_CPATH,
#endif
#ifdef SYS_compat_30_getdents
    [SYS_compat_30_getdents] = PLEDGE_RPATH,
#endif
#ifdef SYS_lchmod
    [SYS_lchmod] = PLEDGE_FATTR,
#endif
#ifdef SYS_lchown
    [SYS_lchown] = PLEDGE_CHOWN,
#endif
#ifdef SYS_compat_50_lutimes
    [SYS_compat_50_lutimes] = PLEDGE_FATTR,
#endif
#ifdef SYS___msync13
    [SYS___msync13] = PLEDGE_MEMORY_MGMT,
#endif
#ifdef SYS_compat_30___stat13
    [SYS_compat_30___stat13] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS_compat_30___fstat13
    [SYS_compat_30___fstat13] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS_compat_30___lstat13
    [SYS_compat_30___lstat13] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS___sigaltstack14
    [SYS___sigaltstack14] = PLEDGE_SIGNAL,
#endif
#ifdef SYS___vfork14
    [SYS___vfork14] = PLEDGE_PROCESS_LIFECYCLE,
#endif
#ifdef SYS___posix_chown
    [SYS___posix_chown] = PLEDGE_CHOWN,
#endif
#ifdef SYS___posix_fchown
    [SYS___posix_fchown] = PLEDGE_CHOWN,
#endif
#ifdef SYS___posix_lchown
    [SYS___posix_lchown] = PLEDGE_CHOWN,
#endif
#ifdef SYS_getsid
    [SYS_getsid] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS___clone
    [SYS___clone] = PLEDGE_PROCESS_LIFECYCLE,
#endif
#ifdef SYS_preadv
    [SYS_preadv] = PLEDGE_STDIO,
#endif
#ifdef SYS_pwritev
    [SYS_pwritev] = PLEDGE_WPATH,
#endif
#ifdef SYS_compat_16___sigaction14
    [SYS_compat_16___sigaction14] = PLEDGE_SIGNAL,
#endif
#ifdef SYS___sigpending14
    [SYS___sigpending14] = PLEDGE_SIGNAL,
#endif
#ifdef SYS___sigprocmask14
    [SYS___sigprocmask14] = PLEDGE_SIGNAL,
#endif
#ifdef SYS___sigsuspend14
    [SYS___sigsuspend14] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_compat_16___sigreturn14
    [SYS_compat_16___sigreturn14] = PLEDGE_SIGNAL,
#endif
#ifdef SYS___getcwd
    [SYS___getcwd] = PLEDGE_RPATH,
#endif
#ifdef SYS_compat_50___msgctl13
    [SYS_compat_50___msgctl13] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_compat_50___shmctl13
    [SYS_compat_50___shmctl13] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_lchflags
    [SYS_lchflags] = PLEDGE_FATTR,
#endif
#ifdef SYS_issetugid
    [SYS_issetugid] = PLEDGE_IDENTITY_GET,
#endif
#ifdef SYS_getcontext
    [SYS_getcontext] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_setcontext
    [SYS_setcontext] = PLEDGE_SIGNAL,
#endif
#ifdef SYS__lwp_create
    [SYS__lwp_create] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_exit
    [SYS__lwp_exit] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_self
    [SYS__lwp_self] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_wait
    [SYS__lwp_wait] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_suspend
    [SYS__lwp_suspend] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_continue
    [SYS__lwp_continue] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_wakeup
    [SYS__lwp_wakeup] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_getprivate
    [SYS__lwp_getprivate] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_setprivate
    [SYS__lwp_setprivate] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_kill
    [SYS__lwp_kill] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_detach
    [SYS__lwp_detach] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS_compat_50__lwp_park
    [SYS_compat_50__lwp_park] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_unpark
    [SYS__lwp_unpark] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_unpark_all
    [SYS__lwp_unpark_all] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_setname
    [SYS__lwp_setname] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_getname
    [SYS__lwp_getname] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS__lwp_ctl
    [SYS__lwp_ctl] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS___sigaction_sigtramp
    [SYS___sigaction_sigtramp] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_kqueue
    [SYS_kqueue] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS_compat_50_kevent
    [SYS_compat_50_kevent] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS__sched_setparam
    [SYS__sched_setparam] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS__sched_getparam
    [SYS__sched_getparam] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS__sched_setaffinity
    [SYS__sched_setaffinity] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS__sched_getaffinity
    [SYS__sched_getaffinity] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS_sched_yield
    [SYS_sched_yield] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS__sched_protect
    [SYS__sched_protect] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS_fsync_range
    [SYS_fsync_range] = PLEDGE_STDIO,
#endif
#ifdef SYS_uuidgen
    [SYS_uuidgen] = PLEDGE_SYS_INFO,
#endif
#ifdef SYS_compat_90_getvfsstat
    [SYS_compat_90_getvfsstat] = PLEDGE_RPATH,
#endif
#ifdef SYS_compat_90_statvfs1
    [SYS_compat_90_statvfs1] = PLEDGE_RPATH,
#endif
#ifdef SYS_compat_90_fstatvfs1
    [SYS_compat_90_fstatvfs1] = PLEDGE_RPATH,
#endif
#ifdef SYS_compat_30_fhstatvfs1
    [SYS_compat_30_fhstatvfs1] = PLEDGE_RPATH,
#endif
#ifdef SYS_extattr_set_file
    [SYS_extattr_set_file] = PLEDGE_FATTR,
#endif
#ifdef SYS_extattr_get_file
    [SYS_extattr_get_file] = PLEDGE_RPATH,
#endif
#ifdef SYS_extattr_delete_file
    [SYS_extattr_delete_file] = PLEDGE_FATTR,
#endif
#ifdef SYS_extattr_set_fd
    [SYS_extattr_set_fd] = PLEDGE_FATTR,
#endif
#ifdef SYS_extattr_get_fd
    [SYS_extattr_get_fd] = PLEDGE_RPATH,
#endif
#ifdef SYS_extattr_delete_fd
    [SYS_extattr_delete_fd] = PLEDGE_FATTR,
#endif
#ifdef SYS_extattr_set_link
    [SYS_extattr_set_link] = PLEDGE_FATTR,
#endif
#ifdef SYS_extattr_get_link
    [SYS_extattr_get_link] = PLEDGE_RPATH,
#endif
#ifdef SYS_extattr_delete_link
    [SYS_extattr_delete_link] = PLEDGE_FATTR,
#endif
#ifdef SYS_extattr_list_fd
    [SYS_extattr_list_fd] = PLEDGE_RPATH,
#endif
#ifdef SYS_extattr_list_file
    [SYS_extattr_list_file] = PLEDGE_RPATH,
#endif
#ifdef SYS_extattr_list_link
    [SYS_extattr_list_link] = PLEDGE_RPATH,
#endif
#ifdef SYS_compat_50_pselect
    [SYS_compat_50_pselect] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS_compat_50_pollts
    [SYS_compat_50_pollts] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS_setxattr
    [SYS_setxattr] = PLEDGE_FATTR,
#endif
#ifdef SYS_lsetxattr
    [SYS_lsetxattr] = PLEDGE_FATTR,
#endif
#ifdef SYS_fsetxattr
    [SYS_fsetxattr] = PLEDGE_FATTR,
#endif
#ifdef SYS_getxattr
    [SYS_getxattr] = PLEDGE_RPATH,
#endif
#ifdef SYS_lgetxattr
    [SYS_lgetxattr] = PLEDGE_RPATH,
#endif
#ifdef SYS_fgetxattr
    [SYS_fgetxattr] = PLEDGE_RPATH,
#endif
#ifdef SYS_listxattr
    [SYS_listxattr] = PLEDGE_RPATH,
#endif
#ifdef SYS_llistxattr
    [SYS_llistxattr] = PLEDGE_RPATH,
#endif
#ifdef SYS_flistxattr
    [SYS_flistxattr] = PLEDGE_RPATH,
#endif
#ifdef SYS_removexattr
    [SYS_removexattr] = PLEDGE_FATTR,
#endif
#ifdef SYS_lremovexattr
    [SYS_lremovexattr] = PLEDGE_FATTR,
#endif
#ifdef SYS_fremovexattr
    [SYS_fremovexattr] = PLEDGE_FATTR,
#endif
#ifdef SYS_compat_50___stat30
    [SYS_compat_50___stat30] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS_compat_50___fstat30
    [SYS_compat_50___fstat30] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS_compat_50___lstat30
    [SYS_compat_50___lstat30] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS___getdents30
    [SYS___getdents30] = PLEDGE_RPATH,
#endif
#ifdef SYS___socket30
    [SYS___socket30] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_compat_90_fhstatvfs1
    [SYS_compat_90_fhstatvfs1] = PLEDGE_RPATH,
#endif
#ifdef SYS_aio_cancel
    [SYS_aio_cancel] = PLEDGE_AIO,
#endif
#ifdef SYS_aio_error
    [SYS_aio_error] = PLEDGE_AIO,
#endif
#ifdef SYS_aio_fsync
    [SYS_aio_fsync] = PLEDGE_AIO,
#endif
#ifdef SYS_aio_read
    [SYS_aio_read] = PLEDGE_AIO,
#endif
#ifdef SYS_aio_return
    [SYS_aio_return] = PLEDGE_AIO,
#endif
#ifdef SYS_compat_50_aio_suspend
    [SYS_compat_50_aio_suspend] = PLEDGE_AIO,
#endif
#ifdef SYS_aio_write
    [SYS_aio_write] = PLEDGE_AIO,
#endif
#ifdef SYS_lio_listio
    [SYS_lio_listio] = PLEDGE_AIO,
#endif
#ifdef SYS_mremap
    [SYS_mremap] = PLEDGE_MEMORY_MGMT,
#endif
#ifdef SYS___posix_fadvise50
    [SYS___posix_fadvise50] = PLEDGE_STDIO,
#endif
#ifdef SYS___select50
    [SYS___select50] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS___gettimeofday50
    [SYS___gettimeofday50] = PLEDGE_TIME_READ,
#endif
#ifdef SYS___settimeofday50
    [SYS___settimeofday50] = PLEDGE_TIME_SET,
#endif
#ifdef SYS___utimes50
    [SYS___utimes50] = PLEDGE_FATTR,
#endif
#ifdef SYS___adjtime50
    [SYS___adjtime50] = PLEDGE_TIME_SET,
#endif
#ifdef SYS___futimes50
    [SYS___futimes50] = PLEDGE_FATTR,
#endif
#ifdef SYS___lutimes50
    [SYS___lutimes50] = PLEDGE_FATTR,
#endif
#ifdef SYS___setitimer50
    [SYS___setitimer50] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS___getitimer50
    [SYS___getitimer50] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS___clock_gettime50
    [SYS___clock_gettime50] = PLEDGE_TIME_READ,
#endif
#ifdef SYS___clock_settime50
    [SYS___clock_settime50] = PLEDGE_TIME_SET,
#endif
#ifdef SYS___clock_getres50
    [SYS___clock_getres50] = PLEDGE_TIME_READ,
#endif
#ifdef SYS___nanosleep50
    [SYS___nanosleep50] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS_____sigtimedwait50
    [SYS_____sigtimedwait50] = PLEDGE_SIGNAL,
#endif
#ifdef SYS_compat_60__lwp_park
    [SYS_compat_60__lwp_park] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS_compat_100___kevent50
    [SYS_compat_100___kevent50] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS___pselect50
    [SYS___pselect50] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS___pollts50
    [SYS___pollts50] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS___aio_suspend50
    [SYS___aio_suspend50] = PLEDGE_AIO,
#endif
#ifdef SYS___stat50
    [SYS___stat50] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS___fstat50
    [SYS___fstat50] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS___lstat50
    [SYS___lstat50] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS___shmctl50
    [SYS___shmctl50] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS___msgctl50
    [SYS___msgctl50] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS___getrusage50
    [SYS___getrusage50] = PLEDGE_PROCESS_META,
#endif
#ifdef SYS___timer_settime50
    [SYS___timer_settime50] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS___timer_gettime50
    [SYS___timer_gettime50] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS___wait450
    [SYS___wait450] = PLEDGE_PROCESS_LIFECYCLE,
#endif
#ifdef SYS___mknod50
    [SYS___mknod50] = PLEDGE_CPATH,
#endif
#ifdef SYS_pipe2
    [SYS_pipe2] = PLEDGE_STDIO,
#endif
#ifdef SYS_compat_100_dup3
    [SYS_compat_100_dup3] = PLEDGE_STDIO,
#endif
#ifdef SYS_kqueue1
    [SYS_kqueue1] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS_paccept
    [SYS_paccept] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_linkat
    [SYS_linkat] = PLEDGE_CPATH,
#endif
#ifdef SYS_renameat
    [SYS_renameat] = PLEDGE_CPATH,
#endif
#ifdef SYS_mkfifoat
    [SYS_mkfifoat] = PLEDGE_CPATH,
#endif
#ifdef SYS_mknodat
    [SYS_mknodat] = PLEDGE_CPATH,
#endif
#ifdef SYS_mkdirat
    [SYS_mkdirat] = PLEDGE_CPATH,
#endif
#ifdef SYS_faccessat
    [SYS_faccessat] = PLEDGE_RPATH,
#endif
#ifdef SYS_fchmodat
    [SYS_fchmodat] = PLEDGE_FATTR,
#endif
#ifdef SYS_fchownat
    [SYS_fchownat] = PLEDGE_CHOWN,
#endif
#ifdef SYS_fexecve
    [SYS_fexecve] = PLEDGE_EXEC,
#endif
#ifdef SYS_fstatat
    [SYS_fstatat] = PLEDGE_FILE_METADATA_READ,
#endif
#ifdef SYS_utimensat
    [SYS_utimensat] = PLEDGE_FATTR,
#endif
#ifdef SYS_openat
    [SYS_openat] = PLEDGE_RPATH | PLEDGE_WPATH | PLEDGE_CPATH,
#endif
#ifdef SYS_readlinkat
    [SYS_readlinkat] = PLEDGE_RPATH,
#endif
#ifdef SYS_symlinkat
    [SYS_symlinkat] = PLEDGE_CPATH,
#endif
#ifdef SYS_unlinkat
    [SYS_unlinkat] = PLEDGE_CPATH,
#endif
#ifdef SYS_futimens
    [SYS_futimens] = PLEDGE_FATTR,
#endif
#ifdef SYS_posix_spawn
    [SYS_posix_spawn] = PLEDGE_EXEC,
#endif
#ifdef SYS_recvmmsg
    [SYS_recvmmsg] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_sendmmsg
    [SYS_sendmmsg] = PLEDGE_INET | PLEDGE_UNIX,
#endif
#ifdef SYS_clock_nanosleep
    [SYS_clock_nanosleep] = PLEDGE_TIMERS_AND_SLEEP,
#endif
#ifdef SYS____lwp_park60
    [SYS____lwp_park60] = PLEDGE_THREAD_LWP,
#endif
#ifdef SYS_posix_fallocate
    [SYS_posix_fallocate] = PLEDGE_WPATH,
#endif
#ifdef SYS_fdiscard
    [SYS_fdiscard] = PLEDGE_WPATH,
#endif
#ifdef SYS_wait6
    [SYS_wait6] = PLEDGE_PROCESS_LIFECYCLE,
#endif
#ifdef SYS_clock_getcpuclockid2
    [SYS_clock_getcpuclockid2] = PLEDGE_TIME_READ,
#endif
#ifdef SYS___getvfsstat90
    [SYS___getvfsstat90] = PLEDGE_RPATH,
#endif
#ifdef SYS___statvfs190
    [SYS___statvfs190] = PLEDGE_RPATH,
#endif
#ifdef SYS___fstatvfs190
    [SYS___fstatvfs190] = PLEDGE_RPATH,
#endif
#ifdef SYS___fhstatvfs190
    [SYS___fhstatvfs190] = PLEDGE_RPATH,
#endif
#ifdef SYS___acl_get_link
    [SYS___acl_get_link] = PLEDGE_RPATH,
#endif
#ifdef SYS___acl_set_link
    [SYS___acl_set_link] = PLEDGE_FATTR,
#endif
#ifdef SYS___acl_delete_link
    [SYS___acl_delete_link] = PLEDGE_FATTR,
#endif
#ifdef SYS___acl_aclcheck_link
    [SYS___acl_aclcheck_link] = PLEDGE_RPATH,
#endif
#ifdef SYS___acl_get_file
    [SYS___acl_get_file] = PLEDGE_RPATH,
#endif
#ifdef SYS___acl_set_file
    [SYS___acl_set_file] = PLEDGE_FATTR,
#endif
#ifdef SYS___acl_get_fd
    [SYS___acl_get_fd] = PLEDGE_RPATH,
#endif
#ifdef SYS___acl_set_fd
    [SYS___acl_set_fd] = PLEDGE_FATTR,
#endif
#ifdef SYS___acl_delete_file
    [SYS___acl_delete_file] = PLEDGE_FATTR,
#endif
#ifdef SYS___acl_delete_fd
    [SYS___acl_delete_fd] = PLEDGE_FATTR,
#endif
#ifdef SYS___acl_aclcheck_file
    [SYS___acl_aclcheck_file] = PLEDGE_RPATH,
#endif
#ifdef SYS___acl_aclcheck_fd
    [SYS___acl_aclcheck_fd] = PLEDGE_RPATH,
#endif
#ifdef SYS_lpathconf
    [SYS_lpathconf] = PLEDGE_RPATH,
#endif
#ifdef SYS_memfd_create
    [SYS_memfd_create] = PLEDGE_MEMORY_MGMT,
#endif
#ifdef SYS___kevent100
    [SYS___kevent100] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS_epoll_create1
    [SYS_epoll_create1] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS_epoll_ctl
    [SYS_epoll_ctl] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS_epoll_pwait2
    [SYS_epoll_pwait2] = PLEDGE_MULTIPLEX,
#endif
#ifdef SYS___dup3100
    [SYS___dup3100] = PLEDGE_STDIO,
#endif
#ifdef SYS_semtimedop
    [SYS_semtimedop] = PLEDGE_SYSV_IPC,
#endif
#ifdef SYS_pledge
    [SYS_pledge] = PLEDGE_ALWAYS,
#endif
};

const struct pledge_promise pledge_promises[] = {
    { "error", PLEDGE_ERROR },
    { "aio", PLEDGE_AIO },
    { "audio", PLEDGE_AUDIO },
    { "chown", PLEDGE_CHOWN },
    { "cpath", PLEDGE_CPATH },
    { "entropy", PLEDGE_ENTROPY },
    { "exec", PLEDGE_EXEC },
    { "fattr", PLEDGE_FATTR },
    { "file_metadata_read", PLEDGE_FILE_METADATA_READ },
    { "flock", PLEDGE_FLOCK },
    { "fs_sync", PLEDGE_FS_SYNC },
    { "identity_get", PLEDGE_IDENTITY_GET },
    { "identity_set", PLEDGE_IDENTITY_SET },
    { "inet", PLEDGE_INET },
    { "memory_mgmt", PLEDGE_MEMORY_MGMT },
    { "multiplex", PLEDGE_MULTIPLEX },
    { "process_lifecycle", PLEDGE_PROCESS_LIFECYCLE },
    { "process_meta", PLEDGE_PROCESS_META },
    { "rpath", PLEDGE_RPATH },
    { "signal", PLEDGE_SIGNAL },
    { "stdio", PLEDGE_STDIO },
    { "synchronization", PLEDGE_SYNCHRONIZATION },
    { "sys_info", PLEDGE_SYS_INFO },
    { "sysv_ipc", PLEDGE_SYSV_IPC },
    { "thread_lwp", PLEDGE_THREAD_LWP },
    { "time_read", PLEDGE_TIME_READ },
    { "time_set", PLEDGE_TIME_SET },
    { "timers_and_sleep", PLEDGE_TIMERS_AND_SLEEP },
    { "tty", PLEDGE_TTY },
    { "unix", PLEDGE_UNIX },
    { "wpath", PLEDGE_WPATH },
    { NULL, 0 }
};

#endif /* PLEDGE */
