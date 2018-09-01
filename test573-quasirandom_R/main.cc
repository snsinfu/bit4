// http://extremelearning.com.au/unreasonable-effectiveness-of-quasirandom-sequences/

#include <cmath>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <vector>


class kronecker_sequence {
public:
    explicit kronecker_sequence(double alpha, double init = 0)
        : value_{init}, alpha_{alpha}
    {
    }

    double operator()()
    {
        value_ += alpha_;
        value_ -= std::floor(value_);
        return value_;
    }

private:
    double value_;
    double alpha_;
};


struct point {
    double x;
    double y;
    double z;
};


int main(int argc, char** argv)
{
    std::size_t point_count = 100;

    if (argc == 2) {
        point_count = static_cast<std::size_t>(std::stoul(argv[1]));
    }

    // Generate points

    std::mt19937 engine;
    {
        std::random_device source;
        std::seed_seq seed{source(), source(), source(), source()};
        engine.seed(seed);
    }
    std::uniform_real_distribution<double> uniform;

    double const phi_3 = 1.220744084605759475361685349108831;
    double const alpha[3] = {
        1 / phi_3,
        1 / std::pow(phi_3, 2),
        1 / std::pow(phi_3, 3)
    };

    kronecker_sequence x_seq{alpha[0], uniform(engine)};
    kronecker_sequence y_seq{alpha[1], uniform(engine)};
    kronecker_sequence z_seq{alpha[2], uniform(engine)};

    std::vector<point> points;

    for (std::size_t i = 0; i < point_count; i++) {
        points.push_back({x_seq(), y_seq(), z_seq()});
    }

    // Compute distance between nearest points

    double const closepack_distance = std::cbrt(M_SQRT2 / point_count);
    double min_distance = std::sqrt(3.0);

    for (std::size_t i = 0; i < points.size(); i++) {
        for (std::size_t j = 0; j < i; j++) {
            auto const dx = points[i].x - points[j].x;
            auto const dy = points[i].y - points[j].y;
            auto const dz = points[i].z - points[j].z;
            auto const distance = std::sqrt(dx * dx + dy * dy + dz * dz);

            if (distance < min_distance) {
                min_distance = distance;
            }
        }
    }

    std::cout << (min_distance / closepack_distance) << '\n';
}
