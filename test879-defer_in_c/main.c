// https://twitter.com/saleemrash1d/status/993069519336824833
#define defer _d0(__COUNTER__)
#define _d0(X) _d1(X)
#define _d1(X) _d2(S##X, F##X)
#define _d2(S, F) auto void F(void *); int S __attribute__((cleanup(F))); void F(void *_)


#include <stdio.h>

int main(void)
{
    puts("1st");
    defer { puts("4th"); }
    defer { puts("3rd"); }
    puts("2nd");

    return 0;
}

// Does not compile with clang. On macOS with brew-installed gcc use this
// command: make CC=gcc-9
