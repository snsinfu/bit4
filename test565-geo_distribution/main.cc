#include <iostream>
#include <random>

#include <geo.hpp>


namespace geo
{
    template<typename T>
    class uniform_distribution
    {
    public:
        using value_type = T;
        using scalar_type = typename T::scalar_type;

        uniform_distribution()
        {
            for (scalar_type& q : min_) {
                q = scalar_type{0};
            }
            for (scalar_type& q : max_) {
                q = scalar_type{1};
            }
        }

        uniform_distribution(value_type min, value_type max)
            : min_{min}, max_{max}
        {
        }

        template<typename RNG>
        value_type operator()(RNG& engine)
        {
            value_type value;
            for (geo::index i = 0; i < value.dimension; i++) {
                std::uniform_real_distribution<scalar_type> uniform{min_[i], max_[i]};
                value[i] = uniform(engine);
            }
            return value;
        }

    private:
        value_type min_;
        value_type max_;
    };
}

int main()
{
    using point_t = geo::point<double, 3>;
    using vector_t = geo::vector<double, 3>;

    std::mt19937 engine;

    std::cout << "Uniformly distributed points\n";
    geo::uniform_distribution<point_t> uniform_point;
    for (int i = 0; i < 5; i++) {
        std::cout << uniform_point(engine) << '\n';
    }

    std::cout << "\nUniformly distributed vectors\n";
    geo::uniform_distribution<vector_t> uniform_vector{{-1, -1, -1}, {1, 1, 1}};
    for (int i = 0; i < 5; i++) {
        std::cout << uniform_vector(engine) << '\n';
    }
}
