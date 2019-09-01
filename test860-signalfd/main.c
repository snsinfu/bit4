#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/signalfd.h>
#include <sys/wait.h>

#include <poll.h>
#include <unistd.h>


static const int meta_error_exec = 111;
static const int meta_error_wait = 112;

static void handle_signal_noop(int sig);


int main(int argc, char **argv)
{
    if (argc <= 1) {
        fprintf(stderr, "error: command is not given\n");
        exit(meta_error_exec);
    }

    // A signal handler needs to be set to detect signal with signalfd.
    struct sigaction sa;
    memset(&sa, 1, sizeof sa);
    sa.sa_handler = handle_signal_noop;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        fprintf(stderr, "error: cannot handle SIGCHLD - %s\nn", strerror(errno));
        exit(meta_error_wait);
    }

    // We will wait for child process using poll(2) via this signalfd.
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    int sigfd = signalfd(-1, &mask, SFD_CLOEXEC);
    if (sigfd == -1) {
        fprintf(stderr, "error: cannot monitor SIGCHLD - %s\nn", strerror(errno));
        exit(meta_error_wait);
    }

    // Spawn a child process.
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "error: cannot fork - %s\n", strerror(errno));
        exit(meta_error_exec);
    }

    if (pid == 0) {
        // Child process.
        execvp(argv[1], argv + 1);
        fprintf(stderr, "error: cannot exec - %s\n", strerror(errno));
        exit(meta_error_exec);
    }

    // This process. Wait for the child to exit using poll(2). We wait for the
    // signalfd only, but we can asynchronously wait for more fds here!
    enum {
        poll_sigfd,
        poll_count
    };
    struct pollfd polls[poll_count];

    polls[poll_sigfd] = (struct pollfd) {
        .fd     = sigfd,
        .events = POLLIN
    };

    for (;;) {
        int npoll = poll(polls, poll_count, -1);
        if (npoll == -1) {
            if (errno == EINTR) {
                continue;
            }
            fprintf(stderr, "error: poll failed - %s\n", strerror(errno));
            exit(meta_error_wait);
        }

        if (polls[poll_sigfd].revents & POLLIN) {
            int status = 0;
            if (waitpid(pid, &status, 0) == -1) {
                fprintf(stderr, "error: waitpid failed - %s\n", strerror(errno));
                exit(meta_error_wait);
            }

            // Exit with the same code as the child.
            if (WIFEXITED(status)) {
                exit(WEXITSTATUS(status));
            }

            // Die with the same signal as the child.
            if (WIFSIGNALED(status)) {
                kill(getpid(), WTERMSIG(status));
            }

            exit(meta_error_wait);
        }
    }

    return 0;
}


void handle_signal_noop(int sig)
{
    (void) sig;
}
