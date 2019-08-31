#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <linux/limits.h>
#include <sys/inotify.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    int ino = inotify_init1(IN_CLOEXEC);
    if (ino == -1) {
        fprintf(stderr, "error: Cannot create inotify instance - %s\n", strerror(errno));
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        int wd = inotify_add_watch(ino, argv[i], IN_ALL_EVENTS);
        if (wd == -1) {
            fprintf(stderr, "error: Cannot watch file %s - %s\n", argv[i], strerror(errno));
            return 1;
        }
    }

    char evbuf[sizeof(struct inotify_event) + NAME_MAX + 1];

    for (;;) {
        ssize_t nread = read(ino, evbuf, sizeof evbuf);
        if (nread == -1) {
            if (errno == EINTR) {
                continue;
            }
            fprintf(stderr, "error: Cannot read inotify event - %s\n", strerror(errno));
            return 1;
        }

        struct inotify_event *ev = (void *) evbuf;
        printf("!");

        // Interpret events:
        if (ev->mask & IN_ACCESS)        printf(" access");
        if (ev->mask & IN_ATTRIB)        printf(" attrib");
        if (ev->mask & IN_CLOSE_WRITE)   printf(" close_write");
        if (ev->mask & IN_CLOSE_NOWRITE) printf(" close_nowrite");
        if (ev->mask & IN_CREATE)        printf(" create");
        if (ev->mask & IN_DELETE)        printf(" delete");
        if (ev->mask & IN_DELETE_SELF)   printf(" delete_self");
        if (ev->mask & IN_MODIFY)        printf(" modify");
        if (ev->mask & IN_MOVE_SELF)     printf(" move_self");
        if (ev->mask & IN_MOVED_FROM)    printf(" moved_from");
        if (ev->mask & IN_MOVED_TO)      printf(" moved_to");
        if (ev->mask & IN_OPEN)          printf(" in_open");
        if (ev->mask & IN_IGNORED)       printf(" ignored");
        if (ev->mask & IN_ISDIR)         printf(" dir");
        if (ev->mask & IN_UNMOUNT)       printf(" unmount");

        // Changed file:
        if (ev->len != 0) {
            printf(" > %s", ev->name);
        }

        printf("\n");
        fflush(stderr);
    }

    return 0;
}
