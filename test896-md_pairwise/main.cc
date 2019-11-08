#include <md.hpp>

#include "neighbor_pairs.hpp"
#include "pairwise_forcefield.hpp"


int main()
{
    md::system system;

    md::periodic_box box;
    md::neighbor_pairs<md::periodic_box> pairs;
    pairs.set_box(box);
    pairs.set_cutoff_distance(0.1);

    pairs.update(system.view_positions());

    system.add_forcefield(
        md::make_pairwise_forcefield(
            md::soft_wca_potential {
                .epsilon = 3.0,
                .sigma   = 0.1
            },
            pairs, box
        )
    );
}
