#include <msgpack.hpp>

#include "delta_coder.hpp"
#include "schema.hpp"
#include "simulator.hpp"


int main()
{
    storage_type storage = make_storage("trajectory.sqlite3");
    structure_sample structure;
    simulator sim;
    delta_coder coder;

    for (int step = 1; step <= 100; step++) {
        sim.step();

        // Sample
        int const sample_id = storage.insert(sample_point{
            .step = step,
            .time = 0.01 * step
        });

        // Sample structure with fixed-point delta coding
        structure.sample_id = sample_id;
        structure.coords_msgpack.clear();
        msgpack::pack(structure.coords_msgpack, coder(sim.coords));

        storage.insert(structure);
    }
}
