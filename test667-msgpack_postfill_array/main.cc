#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include <msgpack.hpp>


int main()
{
    std::mt19937 random;
    {
        std::random_device source;
        std::seed_seq seed{source(), source(), source(), source()};
        random.seed(seed);
    }

    std::ofstream file{"example.msgpack"};
    msgpack::packer<std::ofstream> pack{file};

    pack.pack_map(2);
    pack.pack(std::string("metadata"));
    pack.pack_map(0);

    pack.pack(std::string("samples"));

    auto const length_pos = file.tellp();
    pack.pack_array(0xFFFFFFFF); // placeholder

    std::uint32_t count = 0;
    std::uniform_int_distribution<int> value{0, 100};

    for (;;) {
        auto const v = value(random);
        if (v == 0) {
            break;
        }

        pack.pack(v);
        count++;
    }

    // Overwrite the array length.
    file.seekp(length_pos);
    file.seekp(1, std::ios::cur);

    std::uint8_t count_code[4] = {
        std::uint8_t(count >> 24),
        std::uint8_t(count >> 16),
        std::uint8_t(count >> 8),
        std::uint8_t(count),
    };
    file.write(reinterpret_cast<char*>(count_code), sizeof count_code);
}
