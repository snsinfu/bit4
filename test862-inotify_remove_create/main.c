#define _POSIX_C_SOURCE 200109L

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/inotify.h>

#include <limits.h>
#include <poll.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "error: Invalid usage\n");
        return 1;
    }
    char *path = argv[1];

    int ino = inotify_init1(IN_CLOEXEC);
    if (ino == -1) {
        fprintf(stderr, "error: Cannot create inotify instance - %s\n", strerror(errno));
        return 1;
    }

    int wd = inotify_add_watch(ino, path, IN_ALL_EVENTS);
    if (wd == -1) {
        if (errno != ENOENT) {
            fprintf(stderr, "error: Cannot watch file %s - %s\n", path, strerror(errno));
            return 1;
        }
    }

    for (;;) {
        struct pollfd pfd = {
            .fd     = ino,
            .events = POLLIN
        };
        int npoll = poll(&pfd, 1, 1000);
        if (npoll == -1) {
            if (errno == EINTR) {
                continue;
            }
            fprintf(stderr, "error: poll failed - %s\n", strerror(errno));
            return 1;
        }
        if (npoll == 0) {
            if (wd == -1) {
                wd = inotify_add_watch(ino, path, IN_ALL_EVENTS);
                if (wd == -1) {
                    if (errno != ENOENT) {
                        fprintf(stderr, "error: Cannot watch file %s - %s\n", path, strerror(errno));
                        return 1;
                    }
                } else {
                    printf("ADD wd %d '%s'\n", wd, path);
                }
            }
            continue;
        }

        char evbuf[sizeof(struct inotify_event) + NAME_MAX + 1];
        ssize_t nread = read(ino, evbuf, sizeof evbuf);
        if (nread == -1) {
            if (errno == EINTR) {
                continue;
            }
            fprintf(stderr, "error: Cannot read inotify event - %s\n", strerror(errno));
            return 1;
        }

        time_t now = time(NULL);
        struct tm tm;
        localtime_r(&now, &tm);

        // Multiple events could have been read!
        for (ptrdiff_t off = 0; off < nread; ) {
            struct inotify_event *ev = (void *) (evbuf + off);

            printf(
                "wd %d [0x%08x] | %2d:%02d:%02d '%.*s'\n",
                ev->wd, ev->mask, tm.tm_hour, tm.tm_min, tm.tm_sec, ev->len, ev->name
            );
            if (ev->mask & IN_ACCESS)        puts("  ACCESS");
            if (ev->mask & IN_MODIFY)        puts("  MODIFY");
            if (ev->mask & IN_ATTRIB)        puts("  ATTRIB");
            if (ev->mask & IN_CLOSE_WRITE)   puts("  CLOSE_WRITE");
            if (ev->mask & IN_CLOSE_NOWRITE) puts("  CLOSE_NOWRITE");
            if (ev->mask & IN_OPEN)          puts("  OPEN");
            if (ev->mask & IN_MOVED_FROM)    puts("  MOVED_FROM");
            if (ev->mask & IN_MOVED_TO)      puts("  MOVED_TO");
            if (ev->mask & IN_CREATE)        puts("  CREATE");
            if (ev->mask & IN_DELETE)        puts("  DELETE");
            if (ev->mask & IN_DELETE_SELF)   puts("  DELETE_SELF");
            if (ev->mask & IN_MOVE_SELF)     puts("  MOVE_SELF");
            if (ev->mask & IN_UNMOUNT)       puts("  UNMOUNT");
            if (ev->mask & IN_IGNORED)       puts("  IGNORED");

            // Unwatch.
            if (ev->mask & IN_IGNORED) {
                wd = -1;

                // Try to rewatch.
                wd = inotify_add_watch(ino, path, IN_ALL_EVENTS);
                if (wd == -1) {
                    if (errno != ENOENT) {
                        fprintf(stderr, "error: Cannot watch file %s - %s\n", path, strerror(errno));
                        return 1;
                    }
                } else {
                    printf("ADD wd %d '%s'\n", wd, path);
                }
            }

            off += (ptrdiff_t) (sizeof(struct inotify_event) + ev->len);
        }
    }

    return 0;
}
