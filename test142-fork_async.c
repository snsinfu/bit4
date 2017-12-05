// Run async tasks with fork().

#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int wait_all();
static void async_collatz(int seed);
static void collatz(int seed);

int main(void)
{
    for (int i = 0; i < 10; i++) {
        async_collatz(rand());
        sleep(1);
    }
    if (wait_all() == -1) {
        return 1;
    }
    return 0;
}

// wait_all blocks until all child processes exit. Returns 0 on success. Returns
// -1 on any failure.
int wait_all()
{
    while (wait(NULL) > 0) {
        continue;
    }
    if (errno != ECHILD) {
        perror("wait");
        return -1;
    }
    return 0;
}

// async_cllatz launches a child process that just executes collatz(seed).
void async_collatz(int seed)
{
    if (fork() == 0) {
        collatz(seed);
        exit(0);
    }
    printf("🚀 Launch %d\n", seed);
}

// collatz computes the number of collatz ops for given integer.
void collatz(int seed)
{
    if (seed < 1) {
        printf("Bad input: %d\n", seed);
        return;
    }

    int count = 0;

    for (int num = seed; num != 1; ) {
        if (num % 2 == 0) {
            num /= 2;
        } else {
            num = num * 3 + 1;
        }
        count++;

        // Slow down a bit.
        if (count % 50 == 0) {
            sleep(1);
        }
    }

    printf("✨  Result %d -> %d\n", seed, count);
}
