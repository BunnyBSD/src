// _debug app is meant for me to test things in userspace
// code of this app is highly volatile as well as app's product

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/sysctl.h>
#include <sys/pledge.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define EXIST_FILE "/tmp/test_pledge_exist.tmp"
#define NEW_FILE   "/tmp/test_pledge_new.tmp"

static int total_failed_tests = 0;

static int check_result(const char *phase, const char *test_name, int ret, int err, bool should_succeed);
static int test_open_read(const char *phase, bool should_succeed);
static int test_open_write_existing(const char *phase, bool should_succeed);
static int test_open_create_new(const char *phase, bool should_succeed);
static int test_socket_inet(const char *phase, bool should_succeed);
static int test_socket_unix(const char *phase, bool should_succeed);
static int test_connect_inet(const char *phase, bool should_succeed);
static int test_connect_unix(const char *phase, bool should_succeed);
static int test_fcntl_safe(const char *phase, bool should_succeed);
static int test_fcntl_lock(const char *phase, bool should_succeed);
static int test_sysctl_safe(const char *phase, bool should_succeed);
static int test_sysctl_unsafe(const char *phase, bool should_succeed);

/* Unified test evaluator. Returns 0 on expected behavior, 1 on test failure */
static int
check_result(const char *phase, const char *test_name, int ret, int err, bool should_succeed)
{
    bool allowed = (ret >= 0 || err != EPERM);

    if (should_succeed) {
        if (allowed) {
            printf("  [%s] SUCCESS: %s allowed (ret: %d, err: %s)\n", 
                   phase, test_name, ret, strerror(err));
            return 0;
        } else {
            printf("  [%s] FAIL: %s blocked unexpectedly: %s\n", 
                   phase, test_name, strerror(err));
            return 1;
        }
    } else {
        if (!allowed) {
            printf("  [%s] SUCCESS: %s blocked safely: %s\n", 
                   phase, test_name, strerror(err));
            return 0;
        } else {
            printf("  [%s] FAIL: %s allowed but should be blocked (ret: %d, err: %s)\n", 
                   phase, test_name, ret, strerror(err));
            return 1;
        }
    }
}

static int
test_open_read(const char *phase, bool should_succeed)
{
    int fd = open(EXIST_FILE, O_RDONLY);
    int err = errno;
    int ret = check_result(phase, "open(O_RDONLY)", fd, err, should_succeed);
    if (fd >= 0) close(fd);
    return ret;
}

static int
test_open_write_existing(const char *phase, bool should_succeed)
{
    int fd = open(EXIST_FILE, O_WRONLY);
    int err = errno;
    int ret = check_result(phase, "open(O_WRONLY) existing", fd, err, should_succeed);
    if (fd >= 0) close(fd);
    return ret;
}

static int
test_open_create_new(const char *phase, bool should_succeed)
{
    (void)unlink(NEW_FILE);
    int fd = open(NEW_FILE, O_WRONLY | O_CREAT, 0644);
    int err = errno;
    int ret = check_result(phase, "open(O_CREAT) new", fd, err, should_succeed);
    if (fd >= 0) {
        close(fd);
        (void)unlink(NEW_FILE);
    }
    return ret;
}

static int
test_socket_inet(const char *phase, bool should_succeed)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    int err = errno;
    int ret = check_result(phase, "socket(AF_INET)", s, err, should_succeed);
    if (s >= 0) close(s);
    return ret;
}

static int
test_socket_unix(const char *phase, bool should_succeed)
{
    int s = socket(AF_UNIX, SOCK_STREAM, 0);
    int err = errno;
    int ret = check_result(phase, "socket(AF_UNIX)", s, err, should_succeed);
    if (s >= 0) close(s);
    return ret;
}

static int
test_connect_inet(const char *phase, bool should_succeed)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) return 0; /* socket creation check is covered separately */
    
    struct sockaddr_in sin = { .sin_family = AF_INET, .sin_port = htons(80) };
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    int rc = connect(s, (struct sockaddr *)&sin, sizeof(sin));
    int err = errno;
    int ret = check_result(phase, "connect(AF_INET)", rc, err, should_succeed);
    close(s);
    return ret;
}

static int
test_connect_unix(const char *phase, bool should_succeed)
{
    int s = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s < 0) return 0; /* socket creation check is covered separately */

    struct sockaddr_un sun = { .sun_family = AF_UNIX };
    strncpy(sun.sun_path, "/tmp/nonexistent.sock", sizeof(sun.sun_path) - 1);

    int rc = connect(s, (struct sockaddr *)&sun, sizeof(sun));
    int err = errno;
    int ret = check_result(phase, "connect(AF_UNIX)", rc, err, should_succeed);
    close(s);
    return ret;
}

static int
test_fcntl_safe(const char *phase, bool should_succeed)
{
    int fd = open(EXIST_FILE, O_RDONLY);
    if (fd < 0) return 0;

    int rc = fcntl(fd, F_GETFL);
    int err = errno;
    int ret = check_result(phase, "fcntl(F_GETFL)", rc, err, should_succeed);
    close(fd);
    return ret;
}

static int
test_fcntl_lock(const char *phase, bool should_succeed)
{
    int fd = open(EXIST_FILE, O_RDWR);
    if (fd < 0) return 0;

    struct flock fl = { .l_type = F_WRLCK, .l_whence = SEEK_SET, .l_start = 0, .l_len = 0 };
    int rc = fcntl(fd, F_SETLK, &fl);
    int err = errno;
    int ret = check_result(phase, "fcntl(F_SETLK)", rc, err, should_succeed);
    close(fd);
    return ret;
}

static int
test_sysctl_safe(const char *phase, bool should_succeed)
{
    int mib[2] = { CTL_KERN, KERN_HOSTNAME };
    char hostname[128];
    size_t len = sizeof(hostname);

    int rc = sysctl(mib, 2, hostname, &len, NULL, 0);
    int err = errno;
    int ret = check_result(phase, "sysctl(KERN_HOSTNAME)", rc, err, should_succeed);
    return ret;
}

static int
test_sysctl_unsafe(const char *phase, bool should_succeed)
{
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
    size_t len = 0;

    int rc = sysctl(mib, 4, NULL, &len, NULL, 0);
    int err = errno;
    int ret = check_result(phase, "sysctl(KERN_PROC)", rc, err, should_succeed);
    return ret;
}

int
main(int argc, char *argv[])
{
    printf("=== Starting Complete Dynamic Isolated Sandbox Tests ===\n\n");

    /* Pre-create a temporary file for reading/writing tests */
    int init_fd = open(EXIST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (init_fd < 0) {
        perror("Failed to pre-create test file");
        return 1;
    }
    write(init_fd, "bunnybsd", 8);
    close(init_fd);

    /* Phase 1: Baseline (Unpledged checks) */
    printf("[PHASE 1] Checking baseline (pre-pledge)...\n");
    total_failed_tests += test_open_read("PHASE 1", true);
    total_failed_tests += test_open_write_existing("PHASE 1", true);
    total_failed_tests += test_open_create_new("PHASE 1", true);
    total_failed_tests += test_socket_inet("PHASE 1", true);
    total_failed_tests += test_socket_unix("PHASE 1", true);
    total_failed_tests += test_connect_inet("PHASE 1", true);
    total_failed_tests += test_connect_unix("PHASE 1", true);
    total_failed_tests += test_fcntl_safe("PHASE 1", true);
    total_failed_tests += test_fcntl_lock("PHASE 1", true);
    total_failed_tests += test_sysctl_safe("PHASE 1", true);
    total_failed_tests += test_sysctl_unsafe("PHASE 1", true);

    #define RUN_SANDBOX_TEST(phase_name, promises, expect_signal, code_block)  \
    do {                                                                       \
        printf("\n[%s] Testing with pledge(\"%s\")...\n", phase_name, promises); \
        fflush(stdout);                                                        \
        pid_t pid = fork();                                                    \
        if (pid < 0) {                                                         \
            perror("fork failed");                                             \
            return 1;                                                          \
        }                                                                      \
        if (pid == 0) {                                                        \
            if (pledge(promises, NULL) != 0) {                                 \
                printf("  [Child] pledge failed: %s\n", strerror(errno));      \
                exit(1);                                                       \
            }                                                                  \
            int child_fails = 0;                                               \
            code_block;                                                        \
            exit(child_fails);                                                 \
        } else {                                                               \
            int status;                                                        \
            waitpid(pid, &status, 0);                                          \
            if (WIFEXITED(status)) {                                           \
                int child_fails = WEXITSTATUS(status);                         \
                if (expect_signal) {                                           \
                    printf("  [Parent] FAIL: Expected child to be signaled, but it exited normally\n"); \
                    total_failed_tests++;                                      \
                } else {                                                       \
                    total_failed_tests += child_fails;                         \
                }                                                              \
            } else if (WIFSIGNALED(status)) {                                  \
                int sig = WTERMSIG(status);                                    \
                if (expect_signal) {                                           \
                    printf("  [Parent] SUCCESS: Child terminated by expected signal %d (%s)\n", \
                           sig, strsignal(sig));                               \
                } else {                                                       \
                    printf("  [Parent] FAIL: Child killed unexpectedly by signal %d (%s)\n", \
                           sig, strsignal(sig));                               \
                    total_failed_tests++;                                      \
                }                                                              \
            }                                                                      \
        }                                                                      \
    } while (0)

    /* Phase 2: Reading without write capabilities (stdio + rpath + error) */
    RUN_SANDBOX_TEST("PHASE 2", "stdio rpath error", false, {
        child_fails += test_open_read("PHASE 2", true);
        child_fails += test_open_write_existing("PHASE 2", false);
        child_fails += test_open_create_new("PHASE 2", false);
    });

    /* Phase 3: Writing without read or creation capabilities (stdio + wpath + error) */
    RUN_SANDBOX_TEST("PHASE 3", "stdio wpath error", false, {
        child_fails += test_open_read("PHASE 3", false);
        child_fails += test_open_write_existing("PHASE 3", true);
        child_fails += test_open_create_new("PHASE 3", false);
    });

    /* Phase 4: Attempting creation without write capabilities (stdio + cpath + error) */
    RUN_SANDBOX_TEST("PHASE 4", "stdio cpath error", false, {
        child_fails += test_open_create_new("PHASE 4", false);
    });

    /* Phase 5: Full write and creation capabilities without reading (stdio + wpath + cpath + error) */
    RUN_SANDBOX_TEST("PHASE 5", "stdio wpath cpath error", false, {
        child_fails += test_open_read("PHASE 5", false);
        child_fails += test_open_write_existing("PHASE 5", true);
        child_fails += test_open_create_new("PHASE 5", true);
    });

    /* Phase 6: Network INET sandbox with zero filesystem access (stdio + inet + error) */
    RUN_SANDBOX_TEST("PHASE 6", "stdio inet error", false, {
        child_fails += test_open_read("PHASE 6", false);
        child_fails += test_socket_inet("PHASE 6", true);
        child_fails += test_socket_unix("PHASE 6", false);
        child_fails += test_connect_inet("PHASE 6", true);
        child_fails += test_connect_unix("PHASE 6", false);
    });

    /* Phase 7: Network UNIX sandbox (stdio + unix + error) */
    RUN_SANDBOX_TEST("PHASE 7", "stdio unix error", false, {
        child_fails += test_socket_inet("PHASE 7", false);
        child_fails += test_socket_unix("PHASE 7", true);
        child_fails += test_connect_inet("PHASE 7", false);
        child_fails += test_connect_unix("PHASE 7", true);
    });

    /* Phase 8: Locking with flock allowed (stdio flock error) */
    RUN_SANDBOX_TEST("PHASE 8", "stdio flock error", false, {
        child_fails += test_fcntl_safe("PHASE 8", true);
        child_fails += test_fcntl_lock("PHASE 8", true);
    });

    /* Phase 9: Locking without flock blocked safely (stdio error) */
    RUN_SANDBOX_TEST("PHASE 9", "stdio error", false, {
        child_fails += test_fcntl_safe("PHASE 9", true);
        child_fails += test_fcntl_lock("PHASE 9", false);
    });

    /* Phase 10: Sysctl querying allowed vs process spying blocked safely (stdio sys_info error) */
    RUN_SANDBOX_TEST("PHASE 10", "stdio sys_info error", false, {
        child_fails += test_sysctl_safe("PHASE 10", true);
        child_fails += test_sysctl_unsafe("PHASE 10", false);
    });

    /* Phase 11: Privilege escalation prevention check */
    RUN_SANDBOX_TEST("PHASE 11", "stdio error", false, {
        printf("  [PHASE 11] Trying to escalate to rpath...\n");
        int rc = pledge("stdio rpath error", NULL);
        int err = errno;
        child_fails += check_result("PHASE 11", "pledge_escalate", rc, err, false);
    });

    /* Phase 12: Real fatal violation (Expect signal trap) */
    RUN_SANDBOX_TEST("PHASE 12", "stdio", true, {
        printf("  [Child] Doing forbidden open() without PLEDGE_ERROR. I should die now...\n");
        fflush(stdout);
        int fd = open(EXIST_FILE, O_RDONLY);
        printf("  [Child] FAIL: I survived! fd = %d\n", fd);
        if (fd >= 0) close(fd);
        child_fails++;
    });

    /* Cleanup resources */
    (void)unlink(EXIST_FILE);

    printf("\n=== Sandbox Tests Completed ===\n");
    if (total_failed_tests > 0) {
        printf(">>> FAILURE: %d test(s) failed in total.\n", total_failed_tests);
    } else {
        printf(">>> SUCCESS: All sandbox tests passed successfully!\n");
    }

    return total_failed_tests > 0 ? 1 : 0;
}
