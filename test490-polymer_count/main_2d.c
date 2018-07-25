#include <stddef.h>
#include <stdio.h>

static const int N = 3;

struct seen_list {
    int x, y;
    struct seen_list *next;
};

typedef struct seen_list seen_list_t;

static int count(int x, int y, seen_list_t *seen)
{
    if (x < 0 || y < 0 || x >= N || y >= N) {
        return 0;
    }

    int length = 1;

    for (seen_list_t *it = seen; it != NULL; it = it->next) {
        if (x == it->x && y == it->y) {
            return 0;
        }
        length++;
    }

    if (length == N * N) {
        return 1;
    }

    seen_list_t here = {
        .x = x,
        .y = y,
        .next = seen
    };

    int sum = 0;

    sum += count(x - 1, y, &here);
    sum += count(x + 1, y, &here);
    sum += count(x, y - 1, &here);
    sum += count(x, y + 1, &here);

    return sum;
}

int main(void)
{
    printf("%d\n", count(0, 0, NULL));

    return 0;
}
