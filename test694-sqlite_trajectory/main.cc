#include <iostream>

#include <msgpack.hpp>

#include "schema.hpp"
#include "simulator.hpp"


// Recording 1000-step trajectory of a coupled system of 10000 vars. Fixed-point
// delta coding with compression works the best.
//
// Encoding                                     DB size   Remark
// -------------------------------------------------------------
// Double                                       86 MB
// Float                                        48 MB
// Float + compression                          35 MB
// Float + delta                                48 MB
// Float + delta + compression                  27 MB
// Fixed-point                                  47 MB
// Fixed-point + compression                    27 MB
// Fixed-point delta                            18 MB
// Fixed-point delta + compression              14 MB     *BEST*
// Fixed-point double-delta                     20 MB
// Fixed-point double-delta + compression       15 MB
// -------------------------------------------------------------
//
// The delta-coded results above use in-frame delta coding. Inter-frame delta
// coding is more complex but really effective:
//
// Encoding                                     DB size   Remark
// -------------------------------------------------------------
// Fixed-point inter-frame delta                9.8 MB
// Fixed-point inter-frame delta + compression  7.9 MB    *BEST*
// -------------------------------------------------------------


int main()
{
    storage_type storage = make_storage("trajectory.sqlite3");
    simulator sim;
    std::vector<int> prev_coords;

    for (int step = 1; step <= 1000; step++) {
        sim.step();

        // Sample structure
        int const sample_id = storage.insert(sample_point{
            .step = step,
            .time = 0.01 * step
        });

        constexpr double coords_multiplier = 0.00001;

        structure_sample structure{
            .sample_id = sample_id,
            .coords_multiplier = coords_multiplier
        };

        msgpack::packer packer{structure.coords_msgpack};

        prev_coords.resize(sim.coords.size());

        for (std::size_t i = 0; i < sim.coords.size(); i++) {
            auto const val = static_cast<int>(std::round(sim.coords[i] / coords_multiplier));
            auto const delta = val - prev_coords[i];
            packer.pack(delta);
            prev_coords[i] = val;
        }

        // Without this only zlib header is written out!
        structure.coords_msgpack.flush();

        storage.insert(structure);
    }
}
