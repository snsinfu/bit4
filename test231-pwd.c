#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pwd.h>
#include <unistd.h>

static int prefix_prompt(char const *mark);
static int get_user_info(struct passwd *info);

int main(void)
{
    struct passwd info;

    if (get_user_info(&info) == -1) {
        return 1;
    }
    if (prefix_prompt("(sub) ") == -1) {
        return 1;
    }

    char *argv[] = {
        info.pw_shell, NULL
    };
    execv(info.pw_shell, argv);
    perror("execv");

    return 1;
}

// prefix_prompt prepends given marker string to the PS1 environment variable.
// Returns 0 on success or -1 on failure.
//
// XXX: Shell resets PS1 on startup so this does not work.
int prefix_prompt(char const *mark)
{
    if (mark == NULL) {
        return -1;
    }

    char const *orig = getenv("PS1");
    if (orig == NULL) {
        orig = "";
    }

    size_t const mark_len = strlen(mark);
    size_t const orig_len = strlen(orig);

    char *new_prompt = malloc(mark_len + orig_len + 1);
    if (new_prompt == NULL) {
        return -1;
    }

    new_prompt[0] = '\0';
    strncat(new_prompt, mark, mark_len);
    strncat(new_prompt, orig, orig_len);

    int retcode = setenv("PS1", new_prompt, 1);

    free(new_prompt);

    return retcode;
}

// get_user_info gets the passwd entry of the current user. Returns 0 on
// success or -1 on failure.
int get_user_info(struct passwd *info)
{
    char *name = getlogin();
    if (name == NULL) {
        perror("getlogin");
        return -1;
    }

    struct passwd *pw = getpwnam(name);
    if (pw == NULL) {
        perror("getpwnam");
        return -1;
    }

    if (info == NULL) {
        return -1;
    }
    *info = *pw;

    return 0;
}
