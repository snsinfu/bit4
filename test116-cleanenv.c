// In the POSIX standard issue 6, setting environ makes subsequent getenv(),
// setenv() and unsetenv() calls undefined behaviors[1]. This restriction is
// relaxed in the issue 7 and now applications can assign a new environment to
// environ and still can call functions like setenv(). The system would copy the
// environment to a new array and sets new environemnt variables thereafter.
//
// [1]: http://pubs.opengroup.org/onlinepubs/009695399/functions/getenv.html
//      RATIONALE section
//
// [2]: http://pubs.opengroup.org/onlinepubs/9699919799/functions/getenv.html
//      RATIONALE section

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int clear_env_dummy(void);
int clear_env_posix6(void);
int clear_env_posix7(void);
int reset_path(void);

int main(int argc, char **argv)
{
    int(*clear_env)(void) = clear_env_dummy;

    for (int opt; (opt = getopt(argc, argv, "67")) != -1; ) {
        switch (opt) {
          case '6':
            clear_env = clear_env_posix6;
            break;

          case '7':
            clear_env = clear_env_posix7;
            break;

          default:
            return 1;
        }
    }

    if (clear_env() == -1) {
        return 1;
    }
    if (reset_path() == -1) {
        return 1;
    }

    execlp("env", "env", NULL);
    perror("Failed to execute env");
    return 1;
}

// clear_env_dummy does nothing and returns 0.
int clear_env_dummy(void)
{
    return 0;
}

// clear_env_posix6 unsets all environment variables. Works with POSIX.1-2001.
//
// Returns 0 on success. Returns -1 on encountering any error.
int clear_env_posix6(void)
{
    int return_value = -1;
    char *keybuf = NULL;
    size_t bufsize = 0;

    while (environ && environ[0]) {
        size_t keylen = strcspn(environ[0], "=");
        if (keylen == 0) {
            fprintf(stderr, "Invalid entry in the environment\n");
            goto cleanup;
        }

        if (keylen + 1 > bufsize) {
            keybuf = realloc(keybuf, keylen + 1);
            if (keybuf == NULL) {
                perror("Faield to allocate memory");
                goto cleanup;
            }
            bufsize = keylen + 1;
        }
        memcpy(keybuf, environ[0], keylen);
        keybuf[keylen] = '\0';

        if (unsetenv(keybuf) == -1) {
            perror("Failed to unset an environment variable");
            goto cleanup;
        }
    }
    return_value = 0;

  cleanup:
    free(keybuf);
    return return_value;
}

// clear_env_posix7 unsets all environment variables. Requires POSIX.1-2008.
//
// This function always succeeds and returns 0.
int clear_env_posix7(void)
{
    static char *empty[] = { NULL };
    environ = empty;
    return 0;
}

// reset_path sets the PATH environment variable to the system default value.
// Returns 0 on success. Returns -1 on encountering any error.
int reset_path(void)
{
    int return_value = -1;
    char *path = NULL;

    size_t size = confstr(_CS_PATH, NULL, 0);
    path = malloc(size);
    if (path == NULL) {
        perror("Failed to allocate memory");
        goto cleanup;
    }

    if (confstr(_CS_PATH, path, size) == 0) {
        perror("Failed to get the system PATH");
        goto cleanup;
    }

    if (setenv("PATH", path, 1) == -1) {
        perror("Failed to set PATH");
        goto cleanup;
    }
    return_value = 0;

  cleanup:
    free(path);
    return return_value;
}
