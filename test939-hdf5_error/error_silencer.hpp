#pragma once

#include <stdexcept>

#include <hdf5.h>


class error_silencer
{
public:
    error_silencer()
    {
        if (H5Eget_auto2(H5E_DEFAULT, &_func, &_data) < 0) {
            throw std::runtime_error("failed to get HDF5 error handler");
        }
        if (H5Eset_auto2(H5E_DEFAULT, nullptr, nullptr) < 0) {
            throw std::runtime_error("failed to set HDF5 error handler");
        }
    }

    ~error_silencer() noexcept
    {
        H5Eset_auto2(H5E_DEFAULT, _func, _data);
    }

private:
    H5E_auto2_t _func;
    void* _data;
};
