#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <msgpack.hpp>


struct trajectory_config
{
    std::string  title;
    std::int32_t steps      = 0;
    double       resolution = 1;
};

class trajectory_recorder
{
    using stream_type = boost::iostreams::filtering_ostream;

public:
    template<typename Device>
    explicit trajectory_recorder(Device& device, trajectory_config config = {})
        : packer_{stream_}
        , resolution_{config.resolution}
    {
        stream_.push(boost::iostreams::gzip_compressor());
        stream_.push(device);

        packer_.pack_map(2);

        packer_.pack(std::string{"metadata"});
        packer_.pack_map(2);
        packer_.pack(std::string{"title"});
        packer_.pack(config.title);
        packer_.pack(std::string{"sample_multiplier"});
        packer_.pack(config.resolution);

        packer_.pack(std::string{"trajectory"});
        packer_.pack_array(std::uint32_t(config.steps));
    }

    template<typename Range>
    void put(Range const& values)
    {
        prev_.resize(values.size());

        packer_.pack_map(1);

        packer_.pack(std::string{"sample"});
        packer_.pack_array(std::uint32_t(values.size()));

        for (std::size_t i = 0; i < values.size(); i++) {
            auto const v = round(values[i]);
            packer_.pack(v - prev_[i]);
            prev_[i] = v;
        }
    }

private:
    std::int32_t round(double v) const
    {
        return std::int32_t(std::nearbyint(v / resolution_));
    }

private:
    stream_type                  stream_;
    msgpack::packer<stream_type> packer_;
    std::vector<std::int32_t>    prev_;
    double                       resolution_;
};


int main()
{
    auto file = boost::iostreams::file_sink{"trajectory.msgpack.gz"};
    auto traj = trajectory_recorder(file, {
        .title      = "example trajectory",
        .steps      = 100,
        .resolution = 0.001,
    });

    std::mt19937 random;
    std::uniform_real_distribution<double> uniform;
    std::normal_distribution<double> normal;

    std::vector<double> sample(10000);
    std::generate(sample.begin(), sample.end(), [&] { return uniform(random); });

    for (int step = 0; step < 100; step++) {
        for (double& v : sample) {
            v += 0.01 * normal(random);
        }
        traj.put(sample);
    }
}
