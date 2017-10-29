#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ostream>
#include <random>
#include <string>
#include <vector>

#include <dak.hpp>
#include <msgpack.hpp>
#include <range.hpp>

namespace units
{
    using point = dak::point<double, dak::dim::length, 3>;
    using displacement = dak::vector<double, dak::dim::length, 3>;
}

class random_walk_simulator
{
    double const sigma = 0.01;

  public:
    explicit random_walk_simulator(std::size_t num_points, std::uint32_t seed)
        : points_(num_points), engine_{seed}
    {
        initialize_points();
    }

    range::range<units::point const*> points() const
    {
        return {points_.data(), points_.data() + points_.size()};
    }

    void step_forward()
    {
        std::normal_distribution<double> move{0, sigma};
        for (units::point& point : points_) {
            point += units::displacement{move(engine_), move(engine_), move(engine_)};
        }
    }

  private:
    void initialize_points()
    {
        std::uniform_real_distribution<double> coord{-1, 1};
        for (units::point& point : points_) {
            point = units::point{coord(engine_), coord(engine_), coord(engine_)};
        }
    }

  private:
    std::vector<units::point> points_;
    std::mt19937_64 engine_;
};

int main()
{
    std::size_t const num_points = 60000;
    std::uint32_t const seed = 1234;
    int const num_steps = 50;

    std::ofstream output{"a.msg"};
    msgpack::packer<std::ostream> packer{&output};

    packer.pack_map(3);
    {
        packer.pack(std::string{"name"});
        packer.pack(std::string{"Lorem ipsum dolor sit amet"});

        packer.pack(std::string{"num_points"});
        packer.pack(std::uint32_t(num_points));

        packer.pack(std::string{"dimension"});
        packer.pack(std::uint32_t(units::point::dimension));
    }

    random_walk_simulator simulator{num_points, seed};

    packer.pack_array(std::uint32_t(num_steps));

    for (int step = 0; step < num_steps; ++step) {
        simulator.step_forward();

        packer.pack_bin(num_points * sizeof(float) * units::point::dimension);
        for (units::point const& point : simulator.points()) {
            float coords[units::point::dimension];
            for (unsigned axis = 0; axis < units::point::dimension; ++axis) {
                coords[axis] = static_cast<float>(point[axis].value());
            }
            // XXX: Raw binary serialization is bad!
            packer.pack_bin_body(reinterpret_cast<char const*>(coords), sizeof coords);
        }
    }
}
