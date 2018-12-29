#include <iostream>

#include <msgpack.hpp>

#include "schema.hpp"
#include "simulator.hpp"


// Recording 1000-step trajectory of a coupled system of 10000 vars. Fixed-point
// delta coding with compression works the best.
//
// Encoding                                 DB size   Remark
// ---------------------------------------------------------
// Double                                   86 MB
// Float                                    48 MB
// Float + compression                      35 MB
// Float + delta                            48 MB
// Float + delta + compression              27 MB
// Fixed-point                              47 MB
// Fixed-point + compression                27 MB
// Fixed-point delta                        18 MB
// Fixed-point delta + compression          14 MB     *BEST*
// Fixed-point double-delta                 20 MB
// Fixed-point double-delta + compression   15 MB


int main()
{
    storage_type storage = make_storage("trajectory.sqlite3");
    simulator sim;

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
        int prev_val = 0;

        for (double coord : sim.coords) {
            auto const val = static_cast<int>(std::round(coord / coords_multiplier));
            auto const delta = val - prev_val;
            packer.pack(delta);
            prev_val = val;
        }

        // Without this only zlib header is written out!
        structure.coords_msgpack.flush();

        storage.insert(structure);
    }
}
