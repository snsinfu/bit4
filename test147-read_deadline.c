#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <stdio.h>
#include <time.h>

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

static ssize_t try_read(int fd, char *buf, size_t size, const struct timeval *deadline);
static int     wait_input(int fd, const struct timeval *deadline);
static int     monoclock(struct timeval *time);
static void    sub(const struct timeval *t1, const struct timeval *t2, struct timeval *diff);
static void    normalize(struct timeval *time);

int main(void)
{
    const time_t max_delay = 3;
    const size_t flush_size = 16;

    struct timeval deadline;
    struct timeval *active_deadline = NULL;

    char buf[10];
    size_t avail = 0;

    for (;;) {
        ssize_t nb_read = try_read(STDIN_FILENO, buf, sizeof buf, active_deadline);
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
        }
        if (avail == 0 && nb_read > 0) {
            if (monoclock(&deadline) == -1) {
                perror("");
                return 1;
            }
            deadline.tv_sec += max_delay;
            active_deadline = &deadline;
        }
        printf("Got %zd\n", nb_read);

        avail += nb_read;

        if (avail >= flush_size || nb_read == 0) {
            printf("Flush %zu\n", avail);
            avail = 0;
            active_deadline = NULL;
        }
    }

    return 0;
}

// try_read attempts to read data from a blocking descriptor.
//
// Returns the number of bytes read on success, 0 on EOF, or -1 on timeout or
// error. errno is set to EWOULDBLOCK in case of timeout.
ssize_t try_read(int fd, char *buf, size_t size, const struct timeval *deadline)
{
    int ready = wait_input(fd, deadline);
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
// passing deadline.
//
// Returns 1 on receiving data, 0 on timeout, or -1 on any error.
int wait_input(int fd, const struct timeval *deadline)
{
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    if (deadline) {
        struct timeval now;
        if (monoclock(&now) == -1) {
            return -1;
        }
        struct timeval timeout;
        sub(deadline, &now, &timeout);
        return select(fd + 1, &fds, NULL, NULL, &timeout);
    }
    return select(fd + 1, &fds, NULL, NULL, NULL);
}

// monoclock gets the current time point from a monotonic clock.
//
// Returns 0 on success or -1 on error.
int monoclock(struct timeval *time)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1) {
        return -1;
    }
    time->tv_sec = ts.tv_sec;
    time->tv_usec = (suseconds_t) (ts.tv_nsec / 1000);
    return 0;
}

// sub calculates the time difference t1 - t2.
void sub(const struct timeval *t1, const struct timeval *t2, struct timeval *diff)
{
    diff->tv_sec = t1->tv_sec - t2->tv_sec;
    diff->tv_usec = t1->tv_usec - t2->tv_usec;
    normalize(diff);
}

// normalize adjusts tv_usec of given time within 0 to 999999 with carries and
// borrows from tv_sec.
void normalize(struct timeval *time)
{
    const suseconds_t second_usec = 1000000;

    while (time->tv_usec >= second_usec) {
        time->tv_sec++;
        time->tv_usec -= second_usec;
    }

    while (time->tv_usec < 0) {
        time->tv_sec--;
        time->tv_usec += second_usec;
    }
}
