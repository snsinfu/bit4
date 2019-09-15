#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


static void handle_signal_noop(int sig);


int main(void)
{
    struct sigaction sa;
    sa.sa_handler = handle_signal_noop;
    sa.sa_flags = SA_NOCLDSTOP;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGCHLD);
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        return 1;
    }

    if (kill(getpid(), SIGHUP) == -1) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}

void handle_signal_noop(int sig)
{
    (void) sig;
}
