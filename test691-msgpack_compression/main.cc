#include <fstream>

#include <msgpack.hpp>
#include <msgpack/zbuffer.hpp>


int main()
{
    constexpr std::uint32_t count = 100000;

    msgpack::zbuffer buffer;
    msgpack::packer packer{buffer};

    packer.pack_array(count);
    for (std::uint32_t i = 0; i < count; i++) {
        packer.pack((i % 71) * (i % 93));
    }

    buffer.flush();

    std::ofstream file{"example.msgpack"};
    file.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
}
