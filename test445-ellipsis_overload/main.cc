#include <iostream>

namespace sim
{
    struct basic_particle_parameters
    {
        double mass;
        double radius;
    };

    struct system
    {
        void add_particle(basic_particle_parameters const& params)
        {
            std::cout << "basic_particle_parameters:\n"
                      << "  mass   = " << params.mass  << '\n'
                      << "  radius = " << params.radius << '\n';
        }

        template<typename... Ts>
        void add_particle(Ts const&... params)
        {
            std::cout << "Ts (" << sizeof...(params) << ")\n";
        }
    };
}

int main()
{
    sim::system s;

    s.add_particle({
        .mass   = 1.0,
        .radius = 0.1,
    });

    s.add_particle("foo", 1.23);
}
