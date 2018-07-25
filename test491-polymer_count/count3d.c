#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct seen_list {
    int x, y, z;
    struct seen_list *next;
};

typedef struct seen_list seen_list_t;

static int count_paths(int n, int x, int y, int z, seen_list_t *seen)
{
    if (x < 0 || y < 0 || z < 0 || x >= n || y >= n || z >= n) {
        return 0;
    }

    int length = 1;

    for (seen_list_t *it = seen; it != NULL; it = it->next) {
        if (x == it->x && y == it->y && z == it->z) {
            return 0;
        }
        length++;
    }

    if (length == n * n * n) {
        return 1; // Found a cube-filling path
    }

    seen_list_t here = {
        .x = x,
        .y = y,
        .z = z,
        .next = seen
    };

    int sum = 0;

    sum += count_paths(n, x - 1, y, z, &here);
    sum += count_paths(n, x + 1, y, z, &here);
    sum += count_paths(n, x, y - 1, z, &here);
    sum += count_paths(n, x, y + 1, z, &here);
    sum += count_paths(n, x, y, z - 1, &here);
    sum += count_paths(n, x, y, z + 1, &here);

    return sum;
}

int main(int argc, char **argv)
{
    int n = 3;
    int x = 0;
    int y = 0;
    int z = 0;

    switch (argc) {
    case 1:
        break;

    case 2:
        n = atoi(argv[1]);
        break;

    case 5:
        n = atoi(argv[1]);
        x = atoi(argv[2]);
        y = atoi(argv[3]);
        z = atoi(argv[4]);
        break;

    default:
        fputs("invalid usage\n", stderr);
        return 1;
    }

    printf("%d\n", count_paths(n, x, y, z, NULL));

    return 0;
}
