#define MSGPACK_USE_BOOST

#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include <msgpack.hpp>


int main()
{
    std::ofstream file{"example.msgpack"};
    msgpack::packer packer{file};

    packer.pack_map(2);
    packer.pack(std::string{"metadata"});
    packer.pack(std::map<std::string, msgpack::type::variant>{
        {"coord_multiplier", 0.0001},
        {"delta_coded", true},
    });
    packer.pack(std::string{"samples"});
    packer.pack_array(0);
}
