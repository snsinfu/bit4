#include <stdio.h>

long __declspec(naked) greatest_common_divisor(long m, long n)
{
    __asm {
        cmp  rsi, rdi
        jge  L_repeat
        xchg rsi, rdi

      L_repeat:
        test rdi, rdi
        jz   L_done

        xor  rdx, rdx
        mov  rax, rsi
        div  rdi
        mov  rsi, rdi
        mov  rdi, rdx

        jmp  L_repeat

      L_done:
        mov  rax, rsi
        ret
    }
}

int main()
{
    printf("%ld\n", greatest_common_divisor(1029, 1071));
    return 0;
}
