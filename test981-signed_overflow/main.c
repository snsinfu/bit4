#include <inttypes.h>
#include <stdio.h>


static void case_1(void);
static void case_2(void);


int main(void)
{
    case_1();
    case_2();

    return 0;
}

static void case_1(void)
{
    puts("=== case 1 ===");

    int32_t x = 0x40000000;
    int32_t y = 7;

    printf("x = %" PRId32 "\n", x);

    for (int32_t y = 1; y < 10; y++) {
        int32_t z = x * y;
        printf("x * %" PRId32 " = %" PRId32 "\n", y, z);
    }
}

static void case_2(void)
{
    int32_t x = 0x7fffffff;
    int32_t y = 0x7fffffff;
    int32_t z = x * y;

    puts("=== case 2 ===");
    printf("x = %" PRId32 "\n", x);
    printf("y = %" PRId32 "\n", y);
    printf("x * y = %" PRId32 "\n", z);
}
