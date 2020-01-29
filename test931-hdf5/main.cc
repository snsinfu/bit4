#include <algorithm>
#include <cstdio>
#include <exception>
#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include <hdf5.h>

#include "defer.hpp"


namespace
{
    void run();
}


int main()
{
    try {
        run();
    } catch (std::exception const& ex) {
        std::cerr << "error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}


namespace
{
    struct vec
    {
        double x = 0;
        double y = 0;
        double z = 0;
    };

    void run()
    {
        // Generate some data
        std::mt19937_64 random;
        std::vector<vec> points(1000);
        std::generate(
            points.begin(),
            points.end(),
            [&] {
                std::normal_distribution<double> dist{0, 1};
                return vec { dist(random), dist(random), dist(random) };
            }
        );

        // Write to HDF5 file
        std::string const filename = "_test.h5";
        std::string const dataname = "dataset";

        hid_t const store = H5Fcreate(
            filename.c_str(),
            H5F_ACC_TRUNC,
            H5P_DEFAULT,
            H5P_DEFAULT
        );
        if (store < 0) {
            H5Eprint(H5Eget_current_stack(), stderr);
            throw std::runtime_error("falied to create hdf5 file");
        }
        DEFER { H5Fclose(store); };

        hid_t const dataspace = H5Screate(H5S_SIMPLE);
        if (dataspace < 0) {
            H5Eprint(H5Eget_current_stack(), stderr);
            throw std::runtime_error("falied to create a dataspace");
        }
        DEFER { H5Sclose(dataspace); };

        hsize_t const extent[] = { points.size(), 3 };
        H5Sset_extent_simple(dataspace, 2, extent, extent);

        hid_t const create_props = H5Pcreate(H5P_DATASET_CREATE);
        hsize_t const chunk_dim[] = { 200, 3 };
        H5Pset_chunk(create_props, 2, chunk_dim);
        H5Pset_shuffle(create_props);
        H5Pset_deflate(create_props, 5);

        hid_t const dataset = H5Dcreate2(
            store,
            dataname.c_str(),
            H5T_IEEE_F32LE,
            dataspace,
            H5P_DEFAULT,
            create_props,
            H5P_DEFAULT
        );
        if (dataset < 0) {
            H5Eprint(H5Eget_current_stack(), stderr);
            throw std::runtime_error("falied to create a dataset");
        }
        DEFER { H5Dclose(dataset); };

        H5Dwrite(
            dataset,
            H5T_NATIVE_DOUBLE,
            H5S_ALL,
            H5S_ALL,
            H5P_DEFAULT,
            points.data()
        );
        H5Dflush(dataset);
    }
}
