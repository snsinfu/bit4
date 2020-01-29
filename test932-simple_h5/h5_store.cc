#include <memory>
#include <stdexcept>
#include <string>

#include <hdf5.h>

#include "h5_store.hpp"


struct h5_store::context
{
    hid_t file;

    explicit context(std::string const& filename)
    {
        file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        if (file < 0) {
            throw std::runtime_error("failed to create hdf5 file");
        }
    }

    ~context()
    {
        H5Fclose(file);
    }
};


h5_store::h5_store(std::string const& filename)
    : _context{std::make_unique<context>(filename)}
{
}


h5_store::~h5_store() = default;


namespace
{
    std::vector<hsize_t> make_extent(std::vector<std::size_t> const& shape)
    {
        std::vector<hsize_t> extent;
        for (auto size : shape) {
            extent.push_back(static_cast<hsize_t>(size));
        }
        return extent;
    }

    hid_t h5_storage_type(h5_type type)
    {
        switch (type) {
        case h5_type::type_i32:
            return H5T_STD_I32LE;

        case h5_type::type_u32:
            return H5T_STD_U32LE;

        case h5_type::type_float:
            return H5T_IEEE_F32LE;
        }
    }

    hid_t h5_native_type(h5_type type)
    {
        switch (type) {
        case h5_type::type_i32:
            return H5T_NATIVE_INT32;

        case h5_type::type_u32:
            return H5T_NATIVE_UINT32;

        case h5_type::type_float:
            return H5T_NATIVE_FLOAT;
        }
    }
}


void h5_store::write_raw_array(
    std::string const& path,
    void const* data,
    h5_type type,
    std::vector<std::size_t> const& shape
)
{
    auto const dataspace = H5Screate(H5S_SIMPLE);
    auto const extent = make_extent(shape);
    auto const rank = static_cast<int>(extent.size());
    H5Sset_extent_simple(dataspace, rank, extent.data(), extent.data());

    auto const dataset = H5Dcreate2(
        _context->file,
        path.c_str(),
        h5_storage_type(type),
        dataspace,
        H5P_DEFAULT,
        H5P_DEFAULT,
        H5P_DEFAULT
    );

    H5Dwrite(
        dataset,
        h5_native_type(type),
        H5S_ALL,
        H5S_ALL,
        H5P_DEFAULT,
        data
    );

    H5Dclose(dataset);
    H5Sclose(dataspace);
}
