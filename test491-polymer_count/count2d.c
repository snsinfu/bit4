#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct seen_list {
    int x, y;
    struct seen_list *next;
};

typedef struct seen_list seen_list_t;

static int count_paths(int n, int x, int y, seen_list_t *seen)
{
    if (x < 0 || y < 0 || x >= n || y >= n) {
        return 0;
    }

    int length = 1;

    for (seen_list_t *it = seen; it != NULL; it = it->next) {
        if (x == it->x && y == it->y) {
            return 0;
        }
        length++;
    }

    if (length == n * n) {
        return 1; // Found a square-filling path
    }

    seen_list_t here = {
        .x = x,
        .y = y,
        .next = seen
    };

    int sum = 0;

    sum += count_paths(n, x - 1, y, &here);
    sum += count_paths(n, x + 1, y, &here);
    sum += count_paths(n, x, y - 1, &here);
    sum += count_paths(n, x, y + 1, &here);

    return sum;
}

int main(int argc, char **argv)
{
    int n = 3;
    int x = 0;
    int y = 0;

    switch (argc) {
    case 1:
        break;

    case 2:
        n = atoi(argv[1]);
        break;

    case 4:
        n = atoi(argv[1]);
        x = atoi(argv[2]);
        y = atoi(argv[3]);
        break;

    default:
        fputs("invalid usage\n", stderr);
        return 1;
    }

    printf("%d\n", count_paths(n, x, y, NULL));

    return 0;
}
