#include <iostream>
#include <string>

#include "hdf5.h"
#include "unique_hid.hpp"


int main()
{
    unique_hid<H5Fclose> file = H5Fcreate(
        "_test.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT
    );
    if (file < 0) {
        return 1;
    }

    // Variable-length string dataset

    unique_hid<H5Tclose> type = H5Tcopy(H5T_C_S1);
    if (type < 0) {
        return 1;
    }
    if (H5Tset_cset(type, H5T_CSET_UTF8) < 0) {
        return 1;
    }
    if (H5Tset_size(type, H5T_VARIABLE) < 0) {
        return 1;
    }

    unique_hid<H5Sclose> space = H5Screate(H5S_SCALAR);
    if (space < 0) {
        return 1;
    }

    unique_hid<H5Dclose> dataset = H5Dcreate2(
        file, "/metadata", type, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT
    );
    if (dataset < 0) {
        return 1;
    }

    // Write out
    {
        std::string const text = "The quick brown fox jumps over the lazy dog.";

        unique_hid<H5Tclose> mem_type = H5Tcopy(H5T_C_S1);
        if (mem_type < 0) {
            return 1;
        }
        if (H5Tset_cset(mem_type, H5T_CSET_UTF8) < 0) {
            return 1;
        }
        if (H5Tset_size(mem_type, H5T_VARIABLE) < 0) {
            return 1;
        }

        char const* data = text.c_str();
        if (H5Dwrite(dataset, mem_type, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data) < 0) {
            return 1;
        }

        if (H5Fflush(file, H5F_SCOPE_GLOBAL) < 0) {
            return 1;
        }
    }

    // Read in
    {
        char const* data = nullptr;

        unique_hid<H5Tclose> mem_type = H5Tcopy(H5T_C_S1);
        if (mem_type < 0) {
            return 1;
        }
        if (H5Tset_cset(mem_type, H5T_CSET_UTF8) < 0) {
            return 1;
        }
        if (H5Tset_size(mem_type, H5T_VARIABLE) < 0) {
            return 1;
        }

        if (H5Dread(dataset, mem_type, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data) < 0) {
            return 1;
        }

        // Now `data` points to a NUL-terminated string.

        std::string text{data};
        std::cout << "Text: \"" << text << "\"\n";
    }

    return 0;
}
