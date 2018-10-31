#include <cmath>
#include <cstdint>
#include <string>

#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/stream.hpp>

#include <msgpack.hpp>


int main()
{
    std::string const output_filename = "trajectory.pack.gz";

    boost::iostreams::file_sink file(output_filename);
    boost::iostreams::filtering_ostream output;
    output.push(boost::iostreams::gzip_compressor());
    output.push(file);

    // {
    //   metadata: {
    //     title: "string",
    //     id: 123
    //   },
    //   frames: [
    //     [1, 2, 3, ...],
    //     ...
    //   ]
    // }
    msgpack::packer<std::ostream> pack(output);

    std::string const key_metadata = "metadata";
    std::string const key_title = "title";
    std::string const key_id = "id";
    std::string const key_frames = "frames";
    std::string const key_multiplier = "multiplier";

    std::string const trajectory_title = "A simulation trajectory";
    int const trajectory_id = 42;
    int const trajectory_steps = 10;
    int const point_count = 10000;

    bool const use_fixed_point = true;
    float const fixed_point_multiplier = 1 << 16;

    pack.pack_map(2);
    {
        pack.pack(key_metadata);
        pack.pack_map(3);
        {
            pack.pack(key_title);
            pack.pack(trajectory_title);

            pack.pack(key_id);
            pack.pack(trajectory_id);

            pack.pack(key_multiplier);
            pack.pack(fixed_point_multiplier);
        }

        pack.pack(key_frames);
        pack.pack_array(trajectory_steps);

        // Kronecker sequence: x(m) = (m alpha) mod 1.
        float x = 0;
        float alpha = 1.61803398875F;

        for (int step = 0; step < trajectory_steps; step++) {
            pack.pack_array(point_count);

            for (int i = 0; i < point_count; i++) {
                x += alpha;
                x -= std::floor(x);

                // Low-precision fixed-point numbers are way more compressible
                // than floating-point numbers. (easily ~20x difference)

                if (use_fixed_point) {
                    pack.pack(std::int32_t(std::nearbyint(fixed_point_multiplier * x)));
                } else {
                    pack.pack(x);
                }
            }
        }
    }
}
