#include <iostream>

#include <md.hpp>


namespace md
{
    template<typename P1, typename P2>
    struct sum_potential
    {
        P1 potential_1;
        P2 potential_2;

        md::scalar evaluate_energy(md::vector r) const
        {
            return potential_1.evaluate_energy(r) + potential_2.evaluate_energy(r);
        }

        md::vector evaluate_force(md::vector r) const
        {
            return potential_1.evaluate_force(r) + potential_2.evaluate_force(r);
        }
    };


    template<typename P>
    struct scaled_potential
    {
        P          potential;
        md::scalar scale;

        md::scalar evaluate_energy(md::vector r) const
        {
            return scale * potential.evaluate_energy(r);
        }

        md::vector evaluate_force(md::vector r) const
        {
            return scale * potential.evaluate_force(r);
        }
    };


    template<int P, int Q>
    inline polybell_potential<P, Q> operator*(
        md::scalar scale,
        polybell_potential<P, Q> const& pot
    )
    {
        polybell_potential<P, Q> scaled = pot;
        scaled.overlap_energy *= scale;
        return scaled;
    }


    template<
        typename P,
        md::scalar(P::*)(md::vector) const = &P::evaluate_energy,
        md::vector(P::*)(md::vector) const = &P::evaluate_force
    >
    P check_binary_potential();

    template<typename P>
    using binary_potential_t = decltype(check_binary_potential<P>());


    template<typename P1, typename P2>
    inline sum_potential<binary_potential_t<P1>, binary_potential_t<P2>>
    operator+(P1 const& pot1, P2 const& pot2)
    {
        sum_potential<P1, P2> sum;
        sum.potential_1 = pot1;
        sum.potential_2 = pot2;
        return sum;
    }
}


struct my_potential
{
    md::scalar evaluate_energy(md::vector r) const
    {
        return r.squared_norm() / 2;
    }

    md::vector evaluate_force(md::vector r) const
    {
        return r;
    }
};



int main()
{
    md::polybell_potential<2, 3> soft { .overlap_energy = 2, .cutoff_distance = 0.1 };
    md::polybell_potential<8, 3> hard { .overlap_energy = 2, .cutoff_distance = 0.1 };
    md::spring_potential spring;

    md::vector r = { 0.01, 0.02, 0.03 };
    auto pot = 0.4 * soft + 0.6 * hard + spring;

    std::cout << pot.evaluate_energy(r) << '\n';

    // error!
    my_potential mp;
    mp + mp;
}
