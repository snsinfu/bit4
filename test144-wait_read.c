#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

static ssize_t try_read(int fd, char *buf, size_t size, time_t timeout);
static int     wait_input(int fd, time_t timeout);

int main(void)
{
    char buf[1024];
    time_t timeout = 3;

    for (;;) {
        ssize_t nb_read = try_read(STDIN_FILENO, buf, sizeof buf, timeout);
        if (nb_read == -1 && errno != EWOULDBLOCK) {
            perror("reading from stdin");
            return 1;
        }
        if (nb_read == 0) {
            break;
        }
        if (nb_read != -1) {
            printf("%zd\n", nb_read);
        }
    }
    return 0;
}

// try_read attempts to read data from a blocking descriptor.
//
// Returns the number on bytes read on success, 0 on EOF, or -1 on timeout or
// error. errno is set to EWOULDBLOCK in case of timeout.
ssize_t try_read(int fd, char *buf, size_t size, time_t timeout)
{
    int ready = wait_input(fd, timeout);
    if (ready == -1) {
        return -1;
    }
    if (ready == 0) {
        errno = EWOULDBLOCK;
        return -1;
    }
    return read(fd, buf, size);
}

// wait_input waits for data available to read from a descriptor or timeout.
//
// Returns 1 on data arrival, 0 on timeout, or -1 on error.
int wait_input(int fd, time_t timeout)
{
    struct timeval timeout_tv = {
        .tv_sec  = timeout,
        .tv_usec = 0,
    };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    return select(1, &fds, NULL, NULL, &timeout_tv);
}
