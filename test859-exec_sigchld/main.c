// Launch child process, do some work, and exit with the same status as the
// child process.

#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <unistd.h>


static void handle_sigchld(int);

static const int meta_error_exec = 111;
static const int meta_error_wait = 112;


int main(int argc, char **argv)
{
    (void) argc;

    struct sigaction sa;
    memset(&sa, 1, sizeof sa);
    sa.sa_handler = handle_sigchld;
    sigaction(SIGCHLD, &sa, NULL);

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

    // This process.
    // Do something...
    sleep(10000);

    return 0;
}

void handle_sigchld(int sig)
{
    (void) sig;

    for (;;) {
        int status;
        pid_t pid = wait(&status);
        if (pid == -1) {
            if (errno == EINTR) {
                continue;
            }
            exit(meta_error_wait);
        }
        exit(WEXITSTATUS(status));
    }
}
