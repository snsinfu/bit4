#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    const int pipe_rd = pipefd[0];
    const int pipe_wr = pipefd[1];

    const pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Child process.

        // Note: Seemingly we MUST close the write-end of the pipe on mac, or
        //       the child process blocks forever. (Why?)
        if (close(pipe_wr) == -1) {
            exit(1);
        }

        if (dup2(pipe_rd, STDIN_FILENO) == -1) {
            exit(1);
        }

        char *argv[] = {"wc", "-l", NULL};
        execvp(argv[0], argv);
        exit(1);
    }

    // Parent process.
    if (close(pipe_rd) == -1) {
        perror("close");
        return 1;
    }

    const char *text =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc eu\n"
        "volutpat nisl. Curabitur vitae risus tortor. Duis sed dolor odio.\n"
        "Vestibulum a dignissim purus. Nullam sit amet egestas dui.\n";
    {
        const char *buf = text;
        size_t remain = strlen(text);

        while (remain > 0) {
            ssize_t written = write(pipe_wr, buf, remain);
            if (written == -1) {
                if (errno == EINTR) {
                    continue;
                }
                perror("write");
                return 1;
            }
            remain -= (size_t) written;
            buf += written;
        }
    }

    if (close(pipe_wr) == -1) {
        perror("close");
        return 1;
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return 1;
    }

    // Simulate exit-code checking.
    if (WIFEXITED(status) && WEXITSTATUS(status) == 255) {
        puts("255");
    }

    return 0;
}
