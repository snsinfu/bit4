#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int return_value = 1;
    char *buf = NULL;

    size_t bufsize = confstr(_CS_V7_ENV, NULL, 0);
    if (bufsize == 0) {
        perror("confstr");
        goto cleanup;
    }

    buf = malloc(bufsize);
    if (buf == NULL) {
        perror("malloc");
        goto cleanup;
    }

    if (confstr(_CS_V7_ENV, buf, bufsize) == 0) {
        perror("confstr");
        goto cleanup;
    }

    puts(buf);

    return_value = 0;

  cleanup:
    free(buf);
    return return_value;
}
