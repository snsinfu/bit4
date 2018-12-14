#define MSGPACK_USE_BOOST

#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>

#include <msgpack.hpp>


struct trajectory_writer_config
{
    using metadata_type = std::map<std::string, msgpack::type::variant>;

    std::uint32_t sample_count = 1;
    metadata_type metadata;
};

class trajectory_writer
{
public:
    using packer_type = msgpack::packer<std::ostream>;

    trajectory_writer(std::ostream& stream, trajectory_writer_config config)
        : stream_{stream}
        , packer_{stream}
    {
    }

    void put_sample(std::function<void(packer_type&)> put)
    {
        put(packer);
    }

private:
    std::ostream& stream_;
    packer_type   packer_;
};


int main()
{
    std::ofstream file{"example.msgpack"};

    trajectory_writer traj(file, {
        .sample_count = 100,
        .metadata     = {{"coord_multiplier", 0.0001}, {"delta_coded", true}}
    });

    traj.put_sample([&](auto& packer) {
        packer.pack_map(2);
        packer.pack(std::string{"coords"});
        packer.
    });
}
