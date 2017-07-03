#include <stdio.h>

int main(void) {

    const char code[] = {
        0x8b, 0xc6, // mov    eax, esi
        0xf7, 0xef, // imul/5 edi       ; edx:eax = eax * edi
        0xc3,       // ret
    };
    typedef int(*func_t)(int, int) __attribute__((sysv_abi));

    func_t func = (func_t) code;

    enum { n = 4 };
    int xs[n] = { 1, 2, 3, -1 };
    int ys[n] = { 6, 5, 4, -2 };

    for (int i = 0; i < n; i++) {
        int x = xs[i];
        int y = ys[i];
        printf("%d, %d -> %d\n", x, y, func(x, y));
    }
}
