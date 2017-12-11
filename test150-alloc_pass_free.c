// Allocate resource, pass it to another function, and free. This idiom
// simplifies resource management. Sort of RAII for C.

#include <stdio.h>
#include <stdlib.h>

static int read_write(FILE *src, FILE *dest);
static int do_read_write(FILE *src, FILE *dest, char *buf, size_t bufsize);

int main()
{
    if (read_write(stdin, stdout) == -1) {
        return 1;
    }
    return 0;
}

int read_write(FILE *src, FILE *dest)
{
    size_t bufsize = 4096;
    char *buf = malloc(bufsize);
    if (buf == NULL) {
        return -1;
    }

    int result = do_read_write(src, dest, buf, bufsize);
    free(buf);
    return result;
}

int do_read_write(FILE *src, FILE *dest, char *buf, size_t bufsize)
{
    for (;;) {
        size_t nb_read = fread(buf, 1, bufsize, src);
        if (nb_read == 0) {
            if (feof(src)) {
                break;
            }
            return -1;
        }
        if (fwrite(buf, 1, nb_read, dest) < nb_read) {
            return -1;
        }
    }
    return 0;
}
