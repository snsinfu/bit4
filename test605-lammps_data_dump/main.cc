#include <cstddef>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <utility>
#include <vector>


int main()
{
    using std::size_t;

    const size_t atom_count = 90;

    const int atom_type_count = 1;
    const int atom_type = 1;
    const int bond_type_count = 1;
    const int bond_type = 1;

    std::vector<double> xs(atom_count);
    std::vector<double> ys(atom_count);
    std::vector<double> zs(atom_count);

    std::mt19937_64 engine;
    std::uniform_real_distribution<double> initial_coord{-0.5, 0.5};

    for (size_t i = 0; i < atom_count; i++) {
        xs.at(i) = initial_coord(engine);
        ys.at(i) = initial_coord(engine);
        zs.at(i) = initial_coord(engine);
    }

    const std::vector<std::pair<size_t, size_t>> bonded_segments = {
        {0, 29},
        {30, 59},
        {60, 89},
    };

    const size_t bond_count = std::accumulate(
        bonded_segments.begin(),
        bonded_segments.end(),
        size_t{0},
        [](size_t sum, const std::pair<size_t, size_t>& segment) {
            return sum + segment.second - segment.first;
        }
    );

    std::ofstream data{"simulation.data"};

    data << "Simulation data\n";
    data << '\n';
    data << atom_count << " atoms\n";
    data << bond_count << " bonds\n";
    data << '\n';
    data << atom_type_count << " atom types\n";
    data << bond_type_count << " bond types\n";
    data << '\n';
    data << "-1 1 xlo xhi\n";
    data << "-1 1 ylo yhi\n";
    data << "-1 1 zlo zhi\n";
    data << '\n';
    data << "Atoms\n";
    data << '\n';

    for (size_t i = 0; i < atom_count; i++) {
        data
            << i + 1
            << '\t'
            << atom_type
            << '\t'
            << xs.at(i)
            << '\t'
            << ys.at(i)
            << '\t'
            << zs.at(i)
            << '\n';
    }

    data << '\n';
    data << "Bonds\n";
    data << '\n';

    int bond_id = 1;

    for (const std::pair<size_t, size_t>& segment : bonded_segments) {
        for (size_t atom_idx = segment.first; atom_idx < segment.second; atom_idx++) {
            data
                << bond_id
                << '\t'
                << bond_type
                << '\t'
                << atom_idx + 1
                << '\t'
                << atom_idx + 2
                << '\n';

            bond_id++;
        }
    }

    std::ofstream dump{"simulation.dump"};

    const int step_count = 5;
    const std::string atom_label = "atom";
    std::normal_distribution<double> random_walk(0, 0.01);

    for (int step = 0; step < step_count; step++) {
        dump << "ITEM: TIMESTEP\n" << step << '\n';
        dump << "ITEM: NUMBER OF ATOMS\n" << atom_count << '\n';
        dump << "ITEM: ATOMS type x y z\n";

        for (size_t i = 0; i < atom_count; i++) {
            dump << atom_label
                 << '\t'
                 << xs.at(i)
                 << '\t'
                 << ys.at(i)
                 << '\t'
                 << zs.at(i)
                 << '\n';

            xs.at(i) += random_walk(engine);
            ys.at(i) += random_walk(engine);
            zs.at(i) += random_walk(engine);
        }
    }
}
