#include <cmath>
#include <cstddef>
#include <vector>

template<int N, typename T>
T power_std(T num)
{
    return std::pow(num, N);
}

template<int N, typename T>
T power_naive(T num)
{
    if (N == 0) {
        return 1;
    }

    if (N < 0) {
        return 1 / power_naive<-N>(num);
    }

    T pow = num;
    for (int i = 1; i < N; ++i) {
        pow *= num;
    }

    return pow;
}

template double power_std<6, double>(double);
// pow() is optimized away with -O2 -mtune=haswell -mavx -ffast-math
//
// __Z9power_stdILi6EdET0_S0_:
//     push     rbp
//     mov      rbp, rsp
//     vmulsd   xmm1, xmm0, xmm0
//     vmulsd   xmm0, xmm1, xmm0
//     vmulsd   xmm0, xmm0, xmm0
//     pop      rbp
//     ret
//
// Not with -O2 -mtune=haswell -mavx -funsafe-math-optimizations
//
// __Z9power_stdILi6EdET0_S0_:
//     push     rbp
//     mov      rbp, rsp
//     vmovsd   xmm1, qword ptr [rip + LCPI0_0]
//     pop      rbp
//     jmp      _pow    # <-- function call!

template double power_naive<6, double>(double);
// Optimal with -O2 -mtune=haswell -mavx -ffast-math
//          and -O2 -mtune=haswell -mavx -funsafe-math-optimizations
//
// __Z11power_naiveILi6EdET0_S0_:
//     push	rbp
//     mov	rbp, rsp
//     vmulsd   xmm0, xmm0, xmm0
//     vmulsd   xmm1, xmm0, xmm0
//     vmulsd   xmm0, xmm0, xmm1
//     pop      rbp
//     ret
