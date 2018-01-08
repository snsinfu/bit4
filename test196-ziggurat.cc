// https://en.wikipedia.org/wiki/Ziggurat_algorithm

#include <cstddef>
#include <cmath>
#include <cstdio>
#include <utility>

template<typename Density>
struct ziggurat_evaluator : private Density
{
    using density_type = Density;
    using number_type = typename Density::number_type;

    ziggurat_evaluator(int strip_count, number_type tail_start)
        : strip_count_{strip_count}
        , tail_start_{tail_start}
    {
    }

    // Ziggurat algorithm covers the graph of the probability density function
    // with strips of equal area. This function computes that area based on the
    // parameters passed to the constructor.
    number_type strip_area() const
    {
        return tail_start_ * density(tail_start_) + tail_mass(tail_start_);
    }

    // Depending on the parameters not all strips cannot have the equal area.
    // This function computes the maximum error. The parameters are good when
    // the error is close to zero.
    number_type error() const
    {
        return uppermost_strip_area() - strip_area();
    }

  private:
    using density_type::density;
    using density_type::inverse_density;
    using density_type::tail_mass;

    // Computes the area of the uppermost strip given the parameters.
    number_type uppermost_strip_area() const
    {
        number_type const end = uppermost_strip_end();
        return end * (density(0) - density(end));
    }

    // The area of the lowermost strip is fixed for given parameters. So start
    // with this area and compute the right ends of upper strips.
    number_type uppermost_strip_end() const
    {
        number_type const area = strip_area();
        number_type end = tail_start_;

        for (int i = 2; i < strip_count_; ++i) {
            end = inverse_density(density(end) + area / end);

            if (std::isnan(end)) {
                // Overshoot. The uppermost strip must be degenerate.
                return 0;
            }
        }
        return end;
    }

  private:
    int strip_count_;
    number_type tail_start_;
};

// Computes the right_end parameter that makes strip area error closest to zero.
template<typename Density>
typename Density::number_type solve_ziggurat(int strip_count)
{
    using number_type = typename Density::number_type;
    using evaluator = ziggurat_evaluator<Density>;

    // Determine the initial interval for bisection.
    number_type a = 1;
    while (evaluator{strip_count, a}.error() > 0) {
        a /= 2;
    }

    number_type b = 1;
    while (evaluator{strip_count, b}.error() < 0) {
        b *= 2;
    }

    for (;;) {
        number_type const r = (a + b) / 2;

        if (std::nextafter(a, b) == b) {
            return r;
        }

        if (evaluator{strip_count, r}.error() < 0) {
            a = r;
        } else {
            b = r;
        }
    }
}

template<typename T>
struct standard_normal_density
{
    using number_type = T;

    number_type density(number_type x) const
    {
        return std::sqrt(1 / (2 * M_PI)) * std::exp(-0.5 * x * x);
    }

    number_type inverse_density(number_type p) const
    {
        return std::sqrt(-std::log(2 * M_PI * p * p));
    }

    number_type tail_mass(number_type x) const
    {
        return 0.5 * std::erfc(M_SQRT1_2 * x);
    }
};

int main()
{
    for (int strip_count : {8, 16, 64, 128, 256, 512}) {
        std::printf(
            "%d\t%.16g\n",
            strip_count,
            solve_ziggurat<standard_normal_density<double>>(strip_count));
    }
}
