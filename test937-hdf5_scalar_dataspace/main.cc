#include <cassert>

#include "hdf5.h"
#include "unique_hid.hpp"


int main()
{
    // Scalar dataspace
    unique_hid<H5Sclose> scalar = H5Screate(H5S_SCALAR);
    if (scalar < 0) {
        return 1;
    }
    int const scalar_rank = H5Sget_simple_extent_dims(scalar, nullptr, nullptr);
    assert(scalar_rank == 0);

    // Simple (n-dimensional array) dataspace
    hsize_t const dims[] = {10};
    unique_hid<H5Sclose> vector = H5Screate_simple(1, dims, nullptr);
    if (vector < 0) {
        return 1;
    }
    int const vector_rank = H5Sget_simple_extent_dims(vector, nullptr, nullptr);
    assert(vector_rank == 1);

    return 0;
}
