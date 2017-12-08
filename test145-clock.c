#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
    struct timespec prev_time;
    if (clock_gettime(CLOCK_MONOTONIC, &prev_time) == -1) {
        perror("failed to get time");
        return 1;
    }

    usleep(2500000);

    struct timespec curr_time;
    if (clock_gettime(CLOCK_MONOTONIC, &curr_time) == -1) {
        perror("failed to get time");
        return 1;
    }

    static const long second_nsec = 1000000000;

    struct timespec dur = {
        .tv_sec = curr_time.tv_sec - prev_time.tv_sec,
        .tv_nsec = curr_time.tv_nsec - prev_time.tv_nsec,
    };

    while (dur.tv_nsec >= second_nsec) {
        dur.tv_sec++;
        dur.tv_nsec -= second_nsec;
    }

    while (dur.tv_nsec < 0) {
        dur.tv_sec--;
        dur.tv_nsec += second_nsec;
    }

    printf("%lu\t%ld\n", (unsigned long) dur.tv_sec, dur.tv_nsec);;

    return 0;
}
