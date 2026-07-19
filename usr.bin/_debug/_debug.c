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

#define EXIST_FILE "/tmp/test_pledge_exist.tmp"
#define NEW_FILE   "/tmp/test_pledge_new.tmp"

static void test_open_read(const char *phase_name, bool should_succeed);
static void test_open_write_existing(const char *phase_name, bool should_succeed);
static void test_open_create_new(const char *phase_name, bool should_succeed);
static void test_socket_inet(const char *phase_name, bool should_succeed);
static void test_socket_unix(const char *phase_name, bool should_succeed);

/* Test 1: Reading an existing file (O_RDONLY) - Requires rpath */
static void
test_open_read(const char *phase_name, bool should_succeed)
{
    int fd = open(EXIST_FILE, O_RDONLY);
    if (fd < 0) {
        if (should_succeed) {
            printf("  [%s] FAIL: open(O_RDONLY) failed: %s\n", phase_name, strerror(errno));
        } else {
            printf("  [%s] SUCCESS: open(O_RDONLY) blocked safely (error: %s)\n", phase_name, strerror(errno));
        }
    } else {
        if (should_succeed) {
            printf("  [%s] SUCCESS: open(O_RDONLY) succeeded (fd: %d)\n", phase_name, fd);
            close(fd);
        } else {
            printf("  [%s] FAIL: open(O_RDONLY) succeeded but should be blocked (fd: %d)\n", phase_name, fd);
            close(fd);
        }
    }
}

/* Test 2: Writing to an existing file without creation (O_WRONLY) - Requires wpath */
static void
test_open_write_existing(const char *phase_name, bool should_succeed)
{
    int fd = open(EXIST_FILE, O_WRONLY);
    if (fd < 0) {
        if (should_succeed) {
            printf("  [%s] FAIL: open(O_WRONLY) existing failed: %s\n", phase_name, strerror(errno));
        } else {
            printf("  [%s] SUCCESS: open(O_WRONLY) existing blocked safely (error: %s)\n", phase_name, strerror(errno));
        }
    } else {
        if (should_succeed) {
            printf("  [%s] SUCCESS: open(O_WRONLY) existing succeeded (fd: %d)\n", phase_name, fd);
            close(fd);
        } else {
            printf("  [%s] FAIL: open(O_WRONLY) existing succeeded but should be blocked (fd: %d)\n", phase_name, fd);
            close(fd);
        }
    }
}

/* Test 3: Creating a new file (O_WRONLY | O_CREAT) - Requires wpath and cpath */
static void
test_open_create_new(const char *phase_name, bool should_succeed)
{
    /* Remove the file before testing to guarantee it is a new creation (O_CREAT) */
    (void)unlink(NEW_FILE);

    int fd = open(NEW_FILE, O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        if (should_succeed) {
            printf("  [%s] FAIL: open(O_CREAT) new failed: %s\n", phase_name, strerror(errno));
        } else {
            printf("  [%s] SUCCESS: open(O_CREAT) new blocked safely (error: %s)\n", phase_name, strerror(errno));
        }
    } else {
        if (should_succeed) {
            printf("  [%s] SUCCESS: open(O_CREAT) new succeeded (fd: %d)\n", phase_name, fd);
            close(fd);
            (void)unlink(NEW_FILE);
        } else {
            printf("  [%s] FAIL: open(O_CREAT) new succeeded but should be blocked (fd: %d)\n", phase_name, fd);
            close(fd);
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
    printf("=== Starting Refined Isolated Sandbox Tests ===\n\n");

    /* Pre-create a temporary file for reading/writing tests */
    int init_fd = open(EXIST_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (init_fd < 0) {
        perror("Failed to pre-create test file");
        return 1;
    }
    write(init_fd, "bunnybsd", 8);
    close(init_fd);

    /* Phase 1: Baseline */
    printf("[PHASE 1] Checking baseline (pre-pledge)...\n");
    test_open_read("PHASE 1", true);
    test_open_write_existing("PHASE 1", true);
    test_open_create_new("PHASE 1", true);
    test_socket_inet("PHASE 1", true);
    test_socket_unix("PHASE 1", true);

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

    /* Phase 2: Reading without write capabilities (stdio + rpath + error) */
    RUN_SANDBOX_TEST("PHASE 2", "stdio rpath error", {
        test_open_read("PHASE 2", true);             // Should work
        test_open_write_existing("PHASE 2", false);  // Blocked (EPERM)
        test_open_create_new("PHASE 2", false);      // Blocked (EPERM)
    });

    /* Phase 3: Writing without read or creation capabilities (stdio + wpath + error) */
    RUN_SANDBOX_TEST("PHASE 3", "stdio wpath error", {
        test_open_read("PHASE 3", false);            // Blocked (EPERM)
        test_open_write_existing("PHASE 3", true);   // Should work on existing files
        test_open_create_new("PHASE 3", false);      // Blocked (EPERM)
    });

    /* Phase 4: Attempting creation without write capabilities (stdio + cpath + error) */
    RUN_SANDBOX_TEST("PHASE 4", "stdio cpath error", {
        // Creation requires both cpath (for O_CREAT) and wpath (for O_WRONLY).
        // Without wpath, the creation attempt must be rejected!
        test_open_create_new("PHASE 4", false);      // Should return EPERM
    });

    /* Phase 5: Full write and creation capabilities without reading (stdio + wpath + cpath + error) */
    RUN_SANDBOX_TEST("PHASE 5", "stdio wpath cpath error", {
        test_open_read("PHASE 5", false);            // Blocked (EPERM)
        test_open_write_existing("PHASE 5", true);   // Should work
        test_open_create_new("PHASE 5", true);       // Creation should work now
    });

    /* Phase 6: Network INET sandbox with zero filesystem access (stdio + inet + error) */
    RUN_SANDBOX_TEST("PHASE 6", "stdio inet error", {
        // Since we removed open/openat from inet, a network process
        // without rpath is not allowed to call open() at all.
        test_open_read("PHASE 6", false);            // Blocked immediately (EPERM)
        test_socket_inet("PHASE 6", true);           // Should work
        test_socket_unix("PHASE 6", false);          // Blocked (EPERM)
    });

    /* Phase 7: Network UNIX sandbox (stdio + unix + error) */
    RUN_SANDBOX_TEST("PHASE 7", "stdio unix error", {
        test_socket_inet("PHASE 7", false);          // Blocked (EPERM)
        test_socket_unix("PHASE 7", true);           // Should work
    });

    /* Phase 8: Privilege escalation prevention check */
    RUN_SANDBOX_TEST("PHASE 8", "stdio error", {
        printf("  [PHASE 8] Trying to escalate to rpath...\n");
        if (pledge("stdio rpath error", NULL) == 0) {
            printf("  [PHASE 8] FAIL: Escalation allowed!\n");
        } else {
            printf("  [PHASE 8] SUCCESS: Escalation blocked cleanly (error: %s)\n", strerror(errno));
        }
    });

    /* Phase 9: Real fatal violation (No PLEDGE_ERROR interception mode) */
    RUN_SANDBOX_TEST("PHASE 9", "stdio", {
        printf("  [Child] Doing forbidden open() without PLEDGE_ERROR. I should die now...\n");
        fflush(stdout);
        
        // This call will trigger SIGABRT in the child on entry to the kernel
        int fd = open(EXIST_FILE, O_RDONLY);
        
        printf("  [Child] FAIL: I survived! fd = %d\n", fd);
        if (fd >= 0) close(fd);
    });

    /* Cleanup resources after all tests */
    (void)unlink(EXIST_FILE);
    printf("\n=== Sandbox Tests Completed successfully ===\n");
    return 0;
}