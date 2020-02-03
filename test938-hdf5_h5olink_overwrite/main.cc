#include <string>
#include <vector>

#include "hdf5.h"
#include "unique_hid.hpp"


int main()
{
    unique_hid<H5Fclose> file = H5Fcreate("test.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file < 0) {
        return 1;
    }

    std::string const path = "/dataset";

    // Original dataset
    {
        std::vector<int> values;
        for (int i = 0; i < 100; i++) {
            values.push_back(i);
        }

        hsize_t const dims[] = {
            static_cast<hsize_t>(values.size())
        };
        unique_hid<H5Sclose> dataspace = H5Screate_simple(1, dims, nullptr);
        if (dataspace < 0) {
            return 1;
        }

        unique_hid<H5Dclose> dataset = H5Dcreate2(
            file,
            path.c_str(),
            H5T_STD_I32LE,
            dataspace,
            H5P_DEFAULT,
            H5P_DEFAULT,
            H5P_DEFAULT
        );
        if (dataset < 0) {
            return 1;
        }

        if (H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, values.data()) < 0) {
            return 1;
        }
    }

    // New dataset at the same path
    {
        std::vector<float> values;
        for (int i = 0; i < 32; i++) {
            values.push_back(i / (i + 1.0F));
        }

        hsize_t const dims[] = {
            static_cast<hsize_t>(values.size())
        };
        unique_hid<H5Sclose> dataspace = H5Screate_simple(1, dims, nullptr);
        if (dataspace < 0) {
            return 1;
        }

        unique_hid<H5Dclose> dataset = H5Dcreate_anon(
            file,
            H5T_IEEE_F32LE,
            dataspace,
            H5P_DEFAULT,
            H5P_DEFAULT
        );
        if (dataset < 0) {
            return 1;
        }

        if (H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, values.data()) < 0) {
            return 1;
        }

        // H5Olink won't replace existing link (no link creation properties
        // neither). Need to remove the old link if one exists.

        if (H5Ldelete(file, path.c_str(), H5P_DEFAULT) < 0) {
            return 1;
        }

        if (H5Olink(dataset, file, path.c_str(), H5P_DEFAULT, H5P_DEFAULT) < 0) {
            return 1;
        }
    }


    return 0;
}
