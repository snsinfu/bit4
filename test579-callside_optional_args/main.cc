#include <cmath>
#include <cstddef>
#include <iostream>


namespace
{
    struct context
    {
        std::size_t particle_count;
    };

    namespace detail
    {
        template<typename P>
        auto invoke(P& potential, context& ctx, std::size_t i, std::size_t j)
            -> decltype(potential(ctx, i, j))
        {
            return potential(ctx, i, j);
        }

        template<typename P>
        auto invoke(P& potential, context&, std::size_t i, std::size_t j)
            -> decltype(potential(i, j))
        {
            return potential(i, j);
        }

        template<typename P>
        auto invoke(P& potential, context& ctx, std::size_t, std::size_t)
            -> decltype(potential(ctx))
        {
            return potential(ctx);
        }

        template<typename P>
        auto invoke(P& potential, context&, std::size_t, std::size_t)
            -> decltype(potential())
        {
            return potential();
        }
    }

    template<typename P>
    double compute_pairwise_energy(context& ctx, P potential)
    {
        double sum = 0;
        for (std::size_t j = 0; j < ctx.particle_count; j++) {
            for (std::size_t i = 0; i < j; i++) {
                sum += detail::invoke(potential, ctx, i, j);
            }
        }
        return sum;
    }
}

int main()
{
    context ctx;
    ctx.particle_count = 10;

    std::cout << compute_pairwise_energy(ctx, [](context& ctx, std::size_t i, std::size_t j) {
        return ctx.particle_count / std::fabs(j - i);
    }) << '\n';

    std::cout << compute_pairwise_energy(ctx, [](std::size_t i, std::size_t j) {
        return 1 / std::fabs(j - i);
    }) << '\n';

    std::cout << compute_pairwise_energy(ctx, [](context& ctx) {
        return ctx.particle_count;
    }) << '\n';

    std::cout << compute_pairwise_energy(ctx, []() {
        return 1;
    }) << '\n';
}
