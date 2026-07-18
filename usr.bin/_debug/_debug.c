// _debug app is meant for me to test things in userspace
// code of this app is highly volatile as well as app's product

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <sys/pledge.h>

int
main(int argc, char *argv[])
{
    printf("Trying to set pledge(\"stdio error\")\n");
    if (pledge("stdio error", NULL) != 0) {
        printf("pledge err: %s\n", strerror(errno));
        return 1;
    }
    printf("pledged! trying to open /etc/passwd\n");
    int fd = open("/etc/passwd", O_RDONLY);

    if (fd < 0) {
        printf("Error: %s (success)\n", strerror(errno));
    } else {
        printf("Success: %d (fail)\n", fd);
        close(fd);
    }

    return 0;
}
