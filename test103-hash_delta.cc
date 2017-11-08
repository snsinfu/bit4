#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using std::uint32_t;
using std::uint64_t;

struct Vec
{
    uint32_t x;
    uint32_t y;
};

class Hasher
{
  public:
    Hasher(Vec const& coefficients, uint32_t modulo)
        : coefficients_{coefficients}
        , modulo_{modulo}
    {
    }

    uint32_t hash(Vec const& point) const
    {
        // Use double-width uint for intermediate computations, or the implicit
        // modulo 2^32 breaks the result.
        uint64_t hash_value = 0;
        hash_value += uint64_t(coefficients_.x) * point.x;
        hash_value += uint64_t(coefficients_.y) * point.y;
        return static_cast<uint32_t>(hash_value % modulo_);
    }

    uint32_t modulo() const
    {
        return modulo_;
    }

  private:
    Vec coefficients_;
    uint32_t modulo_;
};

uint32_t compute_delta(Hasher const& hasher, Vec const& center, int dx, int dy)
{
    Vec const target = {
        center.x + dx,
        center.y + dy
    };
    uint32_t const center_hash = hasher.hash(center);
    uint32_t const target_hash = hasher.hash(target);
    uint32_t const modulo = hasher.modulo();

    return (target_hash + modulo - center_hash) % modulo;
}

std::vector<uint32_t> compute_neighbor_deltas(Hasher const& hasher, Vec const& center)
{
    std::vector<uint32_t> deltas;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            deltas.push_back(compute_delta(hasher, center, dx, dy));
        }
    }
    return deltas;
}

void print_deltas(std::vector<uint32_t> const& deltas)
{
    std::string delim;
    for (uint32_t delta : deltas) {
        std::cout << delim << delta;
        delim = " ";
    }
    std::cout << '\n';
}

int main()
{
    Hasher hasher{{73856093, 19349669}, 1023};
    print_deltas(compute_neighbor_deltas(hasher, {10000, 10000}));
    print_deltas(compute_neighbor_deltas(hasher, {10001, 10000}));
    print_deltas(compute_neighbor_deltas(hasher, {10001, 10002}));
    print_deltas(compute_neighbor_deltas(hasher, {10004, 10003}));
}
