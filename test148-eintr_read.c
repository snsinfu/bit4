// BSD manpage of sigaction(2) says read(2) is not interruptable.

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
        char buf[10];
        if (read(STDIN_FILENO, buf, sizeof buf) == -1) {
            perror("");
            return 1;
        }
        puts("...");
    }

    return 0;
}

void on_signal(int sig)
{
    printf("signal %d\n", sig);
}
