// clang++ -std=c++17 -fcoroutines-ts
//
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4680.pdf
// https://kirit.com/How%20C%2B%2B%20coroutines%20work/My%20first%20coroutine

#include <cstddef>
#include <cstdio>
#include <utility>

#include "generator.hpp"

generator<std::pair<std::size_t, std::size_t>> index_combinations(std::size_t num)
{
    for (std::size_t i = 0; i < num; ++i) {
        for (std::size_t j = 0; j < i; ++j) {
            co_yield std::make_pair(i, j);
        }
    }
}

void run_range(std::size_t num)
{
    for (auto [i, j] : index_combinations(num)) {
        printf("%zu\t%zu\n", i, j);
    }
}

void run_generator(std::size_t num)
{
    auto gen = index_combinations(num);

    for (; !gen.done(); gen.advance()) {
        auto [i, j] = gen.value();
        printf("%zu\t%zu\n", i, j);
    }
}

int main()
{
    run_range(10);
}

// clang++ 5.0.0 with -O2 emits good inlined code.
//
//__Z9run_rangem:
//    push  rbp
//    mov   rbp, rsp
//    push  r15
//    push  r14
//    push  r12
//    push  rbx
//    mov   r15, rdi
//    cmp   r15, 2
//    jb    LBB2_6
//    lea   r14, [rip + L_.str]
//    mov   r12d, 1
//    mov   esi, 1
//    xor   edx, edx
//    xor   eax, eax
//    mov   rdi, r14
//    call  _printf
//    xor   ebx, ebx
//    jmp   LBB2_2
//LBB2_5:
//    xor   eax, eax
//    mov   rdi, r14
//    mov   rsi, r12
//    mov   rdx, rbx
//    call  _printf
//LBB2_2:
//    inc   rbx
//    cmp   rbx, r12
//    jb    LBB2_5
//    inc   r12
//    cmp   r12, r15
//    jae   LBB2_6
//    xor   ebx, ebx
//    jmp   LBB2_5
//LBB2_6:
//    pop   rbx
//    pop   r12
//    pop   r14
//    pop   r15
//    pop   rbp
//    ret
