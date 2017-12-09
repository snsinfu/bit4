#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

static void on_signal(int sig);

int main(void)
{
    struct sigaction sa = {
        .sa_handler = on_signal,
        .sa_flags   = SA_RESTART,
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);

    struct itimerval interval = {
        .it_interval = {2, 0},
        .it_value    = {2, 0},
    };
    setitimer(ITIMER_REAL, &interval, NULL);

    for (;;) {
        struct timeval timeout = {
            .tv_sec  = 1,
            .tv_usec = 0,
        };
        if (select(0, NULL, NULL, NULL, &timeout) == -1) {
            perror("select"); // Expect EINTR here.
        } else {
            puts("select: Timeout");
        }
    }

    return 0;
}

void on_signal(int sig)
{
    (void) sig;
}
