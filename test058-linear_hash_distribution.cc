#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <cmath>
#include <cstddef>

struct Point
{
    double x;
    double y;
};

std::vector<Point> generate_random_walk(std::size_t n, double sigma, std::mt19937_64& engine)
{
    std::vector<Point> points;

    std::normal_distribution<double> step{0, M_SQRT2 * sigma};
    double x = 0;
    double y = 0;

    for (std::size_t i = 0; i < n; ++i) {
        points.push_back(Point{x, y});
        x += step(engine);
        y += step(engine);
    }

    return points;
}

double examine_hash_distribution(std::vector<Point> const& points, double sigma, bool prime_modulo)
{
    std::size_t const p = 73856093;
    std::size_t const q = 19349669;
    std::size_t const r = 83492791;
    std::size_t const m = 100;

    auto const hash = [&](std::size_t x, std::size_t y) {
        if (prime_modulo) {
            return (p * x + q * y) % r % m;
        } else {
            return (p * x + q * y) % m;
        }
    };

    double const bin_freq = 1 / sigma;

    auto const to_bin = [&](double u) {
        int const offset = 1000000;
        return static_cast<std::size_t>(static_cast<int>(std::nearbyint(bin_freq * u)) + offset);
    };

    std::vector<int> histogram(m);

    for (Point const& point : points) {
        std::size_t const x = to_bin(point.x);
        std::size_t const y = to_bin(point.y);
        std::size_t const h = hash(x, y);
        histogram[h] += 1;
    }

    std::unordered_map<int, int> count_distribution;
    int max_count = histogram[0];

    double mean_count = 0;
    double mean_sq_count = 0;

    for (int count : histogram) {
        count_distribution[count] += 1;
        max_count = std::max(max_count, count);

        double const count_val = static_cast<double>(count);
        mean_count += count_val;
        mean_sq_count += count_val * count_val;
    }

    mean_count /= static_cast<double>(m);
    mean_sq_count /= static_cast<double>(m);

    double const std_count = std::sqrt(mean_sq_count - mean_count * mean_count);
    return std_count;

    for (int count = 0; count <= max_count; ++count) {
        std::cout << count
                  << '\t'
                  << count_distribution[count]
                  << '\t'
                  << std::string(static_cast<std::size_t>(count_distribution[count]), '.')
                  << '\n';
    }
}

int main()
{
    std::mt19937_64 engine;
    {
        std::random_device source;
        std::seed_seq seed{source(), source(), source(), source()};
        engine.seed(seed);
        engine.discard(1000000);
    }

    std::size_t const count = 10000;
    double const sigma = 0.1;

    int const trial_count = 100;

    auto const examine = [&](bool prime_modulo) {
        double mean_std_count = 0;

        for (int i = 0; i < trial_count; ++i) {
            std::vector<Point> points = generate_random_walk(count, sigma, engine);
            double const std_count = examine_hash_distribution(points, sigma, prime_modulo);
            mean_std_count += std_count;
        }

        mean_std_count /= trial_count;

        return mean_std_count;
    };

    std::cout << "stddev of bin occupation number\n";

    std::cout << "w/ prime modulo: " << examine(true) << '\n';
    std::cout << "w/o prime modulo: " << examine(false) << '\n';
}
