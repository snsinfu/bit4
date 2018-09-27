#define BREW_USER "linuxbrew"
#define BREW_EXECUTABLE "/home/linuxbrew/.linuxbrew/bin/brew"

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    struct passwd *user = getpwnam(BREW_USER);
    if (user == NULL) {
        perror("cannot get user entry for " BREW_USER);
        exit(EXIT_FAILURE);
    }

    const uid_t uid = user->pw_uid;
    const uid_t gid = user->pw_gid;

    // Set user
    if (seteuid(uid) == -1) {
        perror("cannot set user");
        exit(EXIT_FAILURE);
    }

    if (setegid(gid) == -1) {
        perror("cannot set group");
        exit(EXIT_FAILURE);
    }

    // Forward to the brew command
    argv[0] = BREW_EXECUTABLE;

    if (execvp(argv[0], argv) == -1) {
        perror("cannot execute brew");
        exit(EXIT_FAILURE);
    }

    return 0;
}
