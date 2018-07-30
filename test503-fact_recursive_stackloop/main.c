#include <stdio.h>

enum { max_stack_depth = 32 };

// Recursive implementation of the factorial.
static int fact_recursive(int n)
{
    if (n == 0) {
        return 1;
    }
    return n * fact_recursive(n - 1);
}

// Stack state-machine implementation of the factorial.
static int fact_stack(int n)
{
    struct stack
    {
        int n;
        int stage;
        int result;
    };
    typedef struct stack stack_t;

    stack_t stack[max_stack_depth] = {
        [0] = {
            .n = n,
            .stage = 0
        }
    };
    int depth = 0;

    while (depth >= 0) {
        switch (stack[depth].stage) {
        case 0:
            if (stack[depth].n == 0) {
                stack[depth].result = 1;
                depth--;
                continue; // return 1
            }

            stack[depth].stage++;
            stack[depth + 1] = (stack_t) {
                .n = stack[depth].n - 1
            };
            depth++;
            continue; // $1 = fact(n - 1)

        case 1:
            stack[depth].result = stack[depth].n * stack[depth + 1].result;
            depth--;
            continue; // return n * $1
        }
    }

    return stack[0].result;
}

int main(void)
{
    printf("%d\n", fact_recursive(10));
    printf("%d\n", fact_stack(10));
}
