#include <algorithm>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <iterator>
#include <random>
#include <vector>

std::uint64_t xor_encode(double prev, double value)
{
    std::uint64_t prev_repr;
    std::uint64_t value_repr;

    std::memcpy(&prev_repr, &prev, sizeof prev);
    std::memcpy(&value_repr, &value, sizeof value);

    return prev_repr ^ value_repr;
}

int main()
{
    std::mt19937 engine;
    std::vector<double> values;
    {
        std::normal_distribution<double> dist{0.0, 0.1};
        double cur = 0;
        for (int i = 0; i < 10; ++i) {
            cur += dist(engine);
            values.push_back(cur);
        }
    }

    double prev = values[0];
    for (double value : values) {
        std::printf("%016llx\n", static_cast<unsigned long long>(xor_encode(prev, value)));
        prev = value;
    }
}
