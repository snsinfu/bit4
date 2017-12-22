#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <execution>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

constexpr long long mixing_length = 1'000'000;
constexpr std::uint32_t difficulty_mask = 0xffc00000;

std::uint32_t mangle(std::uint32_t message, std::uint32_t salt)
{
    std::mt19937 engine{message ^ salt};
    engine.discard(mixing_length);
    return engine();
}

std::uint32_t hard_task(std::uint32_t message)
{
    for (std::uint32_t counter = 0; ; ++counter) {
        if ((mangle(message, counter) & difficulty_mask) == 0) {
            return counter;
        }
    }
}

int main()
{
    std::size_t const num_tasks = 10;
    std::mt19937 engine;

    std::vector<std::uint32_t> messages;
    {
        std::uniform_int_distribution<std::uint32_t> message_distr;
        std::generate_n(std::back_inserter(messages), num_tasks, [&]() {
            return message_distr(engine);
        });
    }

    std::for_each(std::par_unseq, std::begin(messages), std::end(messages), [](std::uint32_t message) {
        std::cout << hard_task(message) << '\n';
    });
}
