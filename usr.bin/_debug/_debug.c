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
#include <sys/pledge.h>

#define TEST_FILE "/tmp/test_pledge_write.tmp"

static void test_open_file(const char *phase_name, bool should_succeed);
static void test_write_file(const char *phase_name, bool should_succeed);
static void test_socket_inet(const char *phase_name, bool should_succeed);
static void test_socket_unix(const char *phase_name, bool should_succeed);

static void
test_open_file(const char *phase_name, bool should_succeed)
{
    int fd = open("/etc/passwd", O_RDONLY);
    if (fd < 0) {
        if (should_succeed) {
            printf("  [%s] FAIL: open(rpath) failed: %s\n", phase_name, strerror(errno));
        } else {
            printf("  [%s] SUCCESS: open(rpath) blocked safely (error: %s)\n", phase_name, strerror(errno));
        }
    } else {
        if (should_succeed) {
            printf("  [%s] SUCCESS: open(rpath) succeeded (fd: %d)\n", phase_name, fd);
            close(fd);
        } else {
            printf("  [%s] FAIL: open(rpath) succeeded but should be blocked (fd: %d)\n", phase_name, fd);
            close(fd);
        }
    }
}

static void
test_write_file(const char *phase_name, bool should_succeed)
{
    int fd = open(TEST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        if (should_succeed) {
            printf("  [%s] FAIL: open(wpath/cpath) failed: %s\n", phase_name, strerror(errno));
        } else {
            printf("  [%s] SUCCESS: open(wpath/cpath) blocked safely (error: %s)\n", phase_name, strerror(errno));
        }
        return;
    }

    ssize_t nw = write(fd, "test", 4);
    if (nw < 0 && should_succeed) {
        printf("  [%s] write failed: %s\n", phase_name, strerror(errno));
    }
    close(fd);

    if (unlink(TEST_FILE) != 0 && should_succeed) {
        printf("  [%s] unlink failed: %s\n", phase_name, strerror(errno));
    } else {
        if (should_succeed) {
            printf("  [%s] SUCCESS: file created, written, and deleted successfully\n", phase_name);
        }
    }
}

static void
test_socket_inet(const char *phase_name, bool should_succeed)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        if (should_succeed) {
            printf("  [%s] FAIL: socket(inet) failed: %s\n", phase_name, strerror(errno));
        } else {
            printf("  [%s] SUCCESS: socket(inet) blocked safely (error: %s)\n", phase_name, strerror(errno));
        }
    } else {
        if (should_succeed) {
            printf("  [%s] SUCCESS: socket(inet) created (fd: %d)\n", phase_name, s);
            close(s);
        } else {
            printf("  [%s] FAIL: socket(inet) succeeded but should be blocked (fd: %d)\n", phase_name, s);
            close(s);
        }
    }
}

static void
test_socket_unix(const char *phase_name, bool should_succeed)
{
    int s = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s < 0) {
        if (should_succeed) {
            printf("  [%s] FAIL: socket(unix) failed: %s\n", phase_name, strerror(errno));
        } else {
            printf("  [%s] SUCCESS: socket(unix) blocked safely (error: %s)\n", phase_name, strerror(errno));
        }
    } else {
        if (should_succeed) {
            printf("  [%s] SUCCESS: socket(unix) created (fd: %d)\n", phase_name, s);
            close(s);
        } else {
            printf("  [%s] FAIL: socket(unix) succeeded but should be blocked (fd: %d)\n", phase_name, s);
            close(s);
        }
    }
}

int
main(int argc, char *argv[])
{
    printf("=== Starting Complete Isolated Sandbox Tests ===\n\n");

    /* Phase 1: Baseline */
    printf("[PHASE 1] Checking baseline (pre-pledge)...\n");
    test_open_file("PHASE 1", true);
    test_write_file("PHASE 1", true);
    test_socket_inet("PHASE 1", true);
    test_socket_unix("PHASE 1", true);

    /* Helper macro to fork and run a specific sandbox test cleanly */
    #define RUN_SANDBOX_TEST(phase_name, promises, code_block)                 \
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
            code_block;                                                        \
            exit(0);                                                           \
        } else {                                                               \
            int status;                                                        \
            waitpid(pid, &status, 0);                                          \
            if (WIFSIGNALED(status)) {                                         \
                int sig = WTERMSIG(status);                                    \
                printf("  [Parent] Violation! Child killed by signal %d (%s)\n", \
                       sig, strsignal(sig));                                   \
            }                                                                  \
        }                                                                      \
    } while (0)

    /* Phase 2: Strict stdio + error */
    RUN_SANDBOX_TEST("PHASE 2", "stdio error", {
        test_open_file("PHASE 2", false);
        test_write_file("PHASE 2", false);
        test_socket_inet("PHASE 2", false);
        test_socket_unix("PHASE 2", false);
    });

    /* Phase 3: Filesystem Write sandbox (stdio + wpath + cpath + error) */
    RUN_SANDBOX_TEST("PHASE 3", "stdio wpath cpath error", {
        test_open_file("PHASE 3", false);   // Reading still blocked
        test_write_file("PHASE 3", true);    // Creating and writing should work!
        test_socket_inet("PHASE 3", false);
        test_socket_unix("PHASE 3", false);
    });

    /* Phase 4: Network INET sandbox (stdio + inet + error) */
    RUN_SANDBOX_TEST("PHASE 4", "stdio inet error", {
        test_write_file("PHASE 4", false);  // Filesystem blocked
        test_socket_inet("PHASE 4", true);   // INET sockets should work!
        test_socket_unix("PHASE 4", false);  // UNIX sockets blocked
    });

    /* Phase 5: Network UNIX sandbox (stdio + unix + error) */
    RUN_SANDBOX_TEST("PHASE 5", "stdio unix error", {
        test_socket_inet("PHASE 5", false);  // INET sockets blocked
        test_socket_unix("PHASE 5", true);   // UNIX sockets should work!
    });

    /* Phase 6: Privilege escalation prevention check */
    RUN_SANDBOX_TEST("PHASE 6", "stdio error", {
        printf("  [PHASE 6] Trying to escalate to rpath...\n");
        if (pledge("stdio rpath error", NULL) == 0) {
            printf("  [PHASE 6] FAIL: Escalation allowed!\n");
        } else {
            printf("  [PHASE 6] SUCCESS: Escalation blocked cleanly (error: %s)\n", strerror(errno));
        }
    });

    /* Phase 7: Real FATAL violation (No error mode) */
    RUN_SANDBOX_TEST("PHASE 7", "stdio", {
        printf("  [Child] Doing forbidden open() without PLEDGE_ERROR. I should die now...\n");
        fflush(stdout);
        
        // This will violate the sandbox and immediately trigger SIGABRT in the child
        int fd = open("/etc/passwd", O_RDONLY);
        
        printf("  [Child] FAIL: I survived! fd = %d\n", fd);
        if (fd >= 0) close(fd);
    });

    printf("\n=== Sandbox Tests Completed successfully ===\n");
    return 0;
}