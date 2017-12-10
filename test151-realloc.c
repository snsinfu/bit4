#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

static int     read_write(void);
static int     do_read_write(char **buf, size_t bufsize);
static ssize_t find_last(char *buf, size_t size, char ch);

int main()
{
    if (read_write() == -1) {
        return 1;
    }
    return 0;
}

int read_write()
{
    size_t bufsize = 4;
    char *buf = malloc(bufsize);
    if (buf == NULL) {
        return -1;
    }

    int result = do_read_write(&buf, bufsize);
    free(buf);
    return result;
}

int do_read_write(char **buf, size_t bufsize)
{
    size_t avail = 0;

    for (;;) {
        if (avail == bufsize) {
            char *newbuf = realloc(*buf, bufsize + 4);
            if (newbuf == NULL) {
                perror("allocation failed");
                return -1;
            }
            *buf = newbuf;
            bufsize += 4;
        }

        ssize_t nb_read = read(STDIN_FILENO, *buf + avail, bufsize - avail);
        if (nb_read == -1) {
            perror("read failed");
            return -1;
        }
        if (nb_read == 0) {
            break;
        }
        avail += (size_t) nb_read;

        ssize_t end = find_last(*buf, avail, '\n');
        if (end == -1) {
            continue;
        }
        size_t chunk_size = (size_t) end + 1;

        ssize_t nb_written = write(STDOUT_FILENO, *buf, chunk_size);
        if (nb_written < (ssize_t) chunk_size) {
            perror("write failed");
            return -1;
        }

        memmove(*buf, *buf + chunk_size, avail - chunk_size);
        avail -= chunk_size;
    }

    ssize_t nb_written = write(STDOUT_FILENO, *buf, avail);
    if (nb_written < (ssize_t) avail) {
        perror("write failed");
        return -1;
    }

    return 0;
}

ssize_t find_last(char *buf, size_t size, char ch)
{
    ssize_t end = (ssize_t) size - 1;
    for (; end >= 0; end--) {
        if (buf[end] == ch) {
            break;
        }
    }
    return end;
}
