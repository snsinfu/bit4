#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

std::uint32_t const sample_rate = 44100;

void write_word(std::ostream& out, std::uint32_t word)
{
    std::array<unsigned char, 4> bytes = {{
        static_cast<unsigned char>(word >> 24),
        static_cast<unsigned char>(word >> 16),
        static_cast<unsigned char>(word >>  8),
        static_cast<unsigned char>(word)
    }};

    // unsigned-to-signed conversion can be undefined
    std::array<char, 4> chars;
    std::memcpy(chars.data(), bytes.data(), bytes.size());

    out.write(chars.data(), chars.size());
}

void dump(std::string filename, std::vector<float> const& data)
{
    std::ofstream outfile{filename, std::ios::binary};

    std::uint32_t const magic = 0x2e736e64;
    std::uint32_t const header_size = 24;
    std::uint32_t const data_format = 6; // 32-bit IEEE floating point
    std::uint32_t const channels = 2;

    write_word(outfile, magic);
    write_word(outfile, header_size);
    write_word(outfile, static_cast<std::uint32_t>(4 * data.size()));
    write_word(outfile, data_format);
    write_word(outfile, sample_rate);
    write_word(outfile, channels);

    static_assert(sizeof(float) == sizeof(std::uint32_t), "");

    for (float sample : data) {
        std::uint32_t word;
        std::memcpy(&word, &sample, sizeof word);
        write_word(outfile, word);
    }
}

struct noise_distribution
{
    template<typename URNG>
    float operator()(URNG& engine) const
    {
        std::uniform_real_distribution<float> unif{-5.F, 0.F};
        return std::powf(10, unif(engine));
    }
};

std::vector<float> generate_noise()
{
    std::mt19937 random_engine;
    {
        std::random_device random;
        std::seed_seq seed{random(), random(), random(), random()};
        random_engine.seed(seed);
    }

    std::vector<float> samples;
    noise_distribution dist;
    float echo = 1;
    float gain = 10;

    for (std::size_t count = 0; count < sample_rate * 120; ++count) {
        float const left = dist(random_engine);
        float const right = dist(random_engine);
        samples.push_back(left * echo * gain);
        samples.push_back(right * echo * gain);
        echo *= std::sqrtf(left * right);
        echo = std::min(1.F, std::powf(echo, 0.92F) + 0.1F);
    }
    return samples;
}

int main()
{
    dump("noise.au", generate_noise());
}
