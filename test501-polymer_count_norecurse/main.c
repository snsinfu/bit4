#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// count counts the number of paths that fill n-by-n-by-n cubic grid starting
// from given coordinates.
static int count(int n, int xstart, int ystart, int zstart)
{
    // ... without recursion. This implementation devices a stack-based state
    // machine to emulate recursion (for fun). Ugly, but not slow.

    struct stack
    {
        int x;
        int y;
        int z;
        int stage;
    };

    typedef struct stack stack_t;

    stack_t stack[n * n * n + 1];
    int occupied[n][n][n];
    int depth = 0;
    int sum = 0;

    stack[0] = (stack_t) {
        .x = xstart,
        .y = ystart,
        .z = zstart,
        .stage = 0
    };
    memset(occupied, 0, sizeof occupied);

    while (depth >= 0) {
        int x = stack[depth].x;
        int y = stack[depth].y;
        int z = stack[depth].z;

        switch (stack[depth].stage) {
        case 0:
            if (x < 0 || y < 0 || z < 0 || x >= n || y >= n || z >= n || occupied[x][y][z]) {
                depth--;
                continue;
            }

            if (depth + 1 == n * n * n) {
                sum++;
                depth--;
                continue;
            }

            occupied[x][y][z] = 1;
            stack[depth].stage++;

        case 1:
            stack[depth + 1] = (stack_t) { .x = x + 1, .y = y, .z = z };
            stack[depth].stage++;
            depth++;
            continue;

        case 2:
            stack[depth + 1] = (stack_t) { .x = x - 1, .y = y, .z = z };
            stack[depth].stage++;
            depth++;
            continue;

        case 3:
            stack[depth + 1] = (stack_t) { .x = x, .y = y + 1, .z = z };
            stack[depth].stage++;
            depth++;
            continue;

        case 4:
            stack[depth + 1] = (stack_t) { .x = x, .y = y - 1, .z = z };
            stack[depth].stage++;
            depth++;
            continue;

        case 5:
            stack[depth + 1] = (stack_t) { .x = x, .y = y, .z = z + 1 };
            stack[depth].stage++;
            depth++;
            continue;

        case 6:
            stack[depth + 1] = (stack_t) { .x = x, .y = y, .z = z - 1 };
            stack[depth].stage++;
            depth++;
            continue;

        case 7:
            occupied[x][y][z] = 0;
            depth--;
        }
    }

    return sum;
}

int main(int argc, char **argv)
{
    int n = 3;
    int x = 0;
    int y = 0;
    int z = 0;

    switch (argc - 1) {
    case 1:
        n = atoi(argv[1]);
        break;

    case 4:
        n = atoi(argv[1]);
        x = atoi(argv[2]);
        y = atoi(argv[3]);
        z = atoi(argv[4]);
        break;
    }

    printf("%d : %d %d %d => %d\n", n, x, y, z, count(n, x, y, z));
}
