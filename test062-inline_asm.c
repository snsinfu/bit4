#include <stdio.h>

int greatest_common_divisor(int m, int n)
{
    int result;

    __asm { // -fms-extensions
        mov  esi, m
        mov  edi, n

        cmp  esi, edi
        jge  L_go
        mov  ecx, esi
        mov  esi, edi
        mov  edi, ecx

      L_go:
        test edi, edi
        jz   L_done

        xor  edx, edx
        mov  eax, esi
        div  edi
        mov  esi, edi
        mov  edi, edx

        jmp  L_go

      L_done:
        mov  result, esi
    }

    return result;
}

int main(void)
{
    printf("%d\n", greatest_common_divisor(1029, 1071));
    return 0;
}
