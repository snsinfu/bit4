#include <iostream>

#include "hdf5.h"
#include "unique_hid.hpp"
#include "error_silencer.hpp"


int main()
{
    unique_hid<H5Fclose> file = H5Fcreate("test.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file < 0) {
        return 1;
    }

    {
        // H5Tfind triggers auto error reporting when there is no conversion
        // path from src to dst. This behavior is unfavorable when one is only
        // checking if the pair of the types are compatible. So silence.
        error_silencer silencer;

        auto const src = H5T_C_S1;
        auto const dst = H5T_IEEE_F32LE;

        H5T_cdata_t* cdata = nullptr;
        if (H5Tfind(src, dst, &cdata) == nullptr) {
            std::clog << "Not convertible\n";
        } else {
            std::clog << "Convertible\n";
        }
    }
}
