#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <stdio.h>
#include <time.h>

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

static ssize_t try_read(int fd, char *buf, size_t size, const struct timeval *timeout);
static int     wait_input(int fd, const struct timeval *timeout);
static int     monoclock(struct timeval *clk);
static void    time_sub(const struct timeval *t1, const struct timeval *t2, struct timeval *diff);

int main(void)
{
    time_t max_delay = 3;

    struct timeval deadline;
    if (monoclock(&deadline) == -1) {
        perror("");
        return 1;
    }
    deadline.tv_sec += max_delay;

    char buf[10];
    size_t avail = 0;

    for (;;) {
        struct timeval now;
        if (monoclock(&now) == -1) {
            perror("");
            return 1;
        }

        struct timeval timeout;
        time_sub(&deadline, &now, &timeout);
        printf("Setting timeout in %lu.%0.6lu sec\n",
               (unsigned long) timeout.tv_sec,
               (unsigned long) timeout.tv_usec);

        ssize_t nb_read = try_read(STDIN_FILENO, buf, sizeof buf, &timeout);
        if (nb_read == 0) {
            puts("EOF");
            break;
        }
        if (nb_read == -1) {
            if (errno != EWOULDBLOCK) {
                perror("");
                return 1;
            }
            puts("Timed out");
            nb_read = 0;
        } else {
            printf("Got %zd\n", nb_read);
        }

        avail += nb_read;

        if (avail == 16 || nb_read == 0) {
            printf("Flush %zu\n", avail);
            avail = 0;

            if (monoclock(&deadline) == -1) {
                perror("");
                return 1;
            }
            deadline.tv_sec += max_delay;
        }
    }

    return 0;
}

// try_read attempts to read data from a blocking descriptor.
//
// Returns the number of bytes read on success, 0 on EOF, or -1 on timeout or
// error. errno is set to EWOULDBLOCK in case of timeout.
ssize_t try_read(int fd, char *buf, size_t size, const struct timeval *timeout)
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

// wait_input waits for any data available for read from given descriptor or
// timeout.
//
// Returns 1 on receiving data, 0 on timeout, or -1 on any error.
int wait_input(int fd, const struct timeval *timeout)
{
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    if (timeout) {
        struct timeval timeout_var = *timeout;
        return select(fd + 1, &fds, NULL, NULL, &timeout_var);
    }
    return select(fd + 1, &fds, NULL, NULL, NULL);
}

// monoclock gets the current time point from a monotonic clock.
//
// Returns 0 on success or -1 on error.
int monoclock(struct timeval *clk)
{
    struct timespec time;
    if (clock_gettime(CLOCK_MONOTONIC, &time) == -1) {
        return -1;
    }
    clk->tv_sec = time.tv_sec;
    clk->tv_usec = (suseconds_t) (time.tv_nsec / 1000);
    return 0;
}

// time_sub calculates the time difference t1 - t2.
void time_sub(const struct timeval *t1, const struct timeval *t2,
              struct timeval *diff)
{
    static const suseconds_t second_usec = 1000000;

    diff->tv_sec = t1->tv_sec - t2->tv_sec;
    diff->tv_usec = t1->tv_usec - t2->tv_usec;

    while (diff->tv_usec >= second_usec) {
        diff->tv_sec++;
        diff->tv_usec -= second_usec;
    }

    while (diff->tv_usec < 0) {
        diff->tv_sec--;
        diff->tv_usec += second_usec;
    }
}
