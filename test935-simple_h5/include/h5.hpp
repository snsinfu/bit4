#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "hdf5.h"


namespace h5
{
    class exception : public std::runtime_error
    {
    public:
        explicit exception(std::string const& message)
            : std::runtime_error(message)
        {
        }
    };


    // DATA TYPES

    using i32 = std::int32_t;
    using u32 = std::uint32_t;
    using i64 = std::int64_t;
    using u64 = std::uint64_t;

    // We want to map C++ scalar type to the corresponding HDF5 datatype. We
    // use function templates because datatype values are determined at run
    // time. (H5T_* macros are not constants!)

    template<typename T>
    hid_t storage_type() = delete;

    template<> inline hid_t storage_type<h5::i32>() { return H5T_STD_I32LE; }
    template<> inline hid_t storage_type<h5::i64>() { return H5T_STD_I64LE; }
    template<> inline hid_t storage_type<h5::u32>() { return H5T_STD_U32LE; }
    template<> inline hid_t storage_type<h5::u64>() { return H5T_STD_U64LE; }
    template<> inline hid_t storage_type<float  >() { return H5T_IEEE_F32LE; }
    template<> inline hid_t storage_type<double >() { return H5T_IEEE_F64LE; }

    template<typename T>
    hid_t memory_type() = delete;

    template<> inline hid_t memory_type<h5::i32>() { return H5T_NATIVE_INT32; }
    template<> inline hid_t memory_type<h5::i64>() { return H5T_NATIVE_INT64; }
    template<> inline hid_t memory_type<h5::u32>() { return H5T_NATIVE_UINT32; }
    template<> inline hid_t memory_type<h5::u64>() { return H5T_NATIVE_UINT64; }
    template<> inline hid_t memory_type<float  >() { return H5T_NATIVE_FLOAT; }
    template<> inline hid_t memory_type<double >() { return H5T_NATIVE_DOUBLE; }

    template<typename T>
    inline hid_t const storage_type_v = h5::storage_type<T>();

    template<typename T>
    inline hid_t const memory_type_v = h5::memory_type<T>();


    // BASIC HID WRAPPER

    namespace detail
    {
        // RAII wrapper for an hid_t.
        template<herr_t(& close_fn)(hid_t)>
        class unique_hid
        {
        public:
            // Takes the ownership of given hid. The hid can be negative (in
            // that case the created `unique_hid` instance does nothing).
            unique_hid(hid_t hid)
                : _hid{hid}
            {
            }

            ~unique_hid() noexcept
            {
                if (_hid >= 0) {
                    close_fn(_hid);
                }
            }

            unique_hid(unique_hid const&) = delete;

            unique_hid(unique_hid&& other) noexcept
                : _hid{other._hid}
            {
                other._hid = -1;
            }

            unique_hid& operator=(unique_hid&& other) noexcept
            {
                unique_hid tmp = std::move(other);
                swap(tmp);
                return *this;
            }

            void swap(unique_hid& other) noexcept
            {
                auto tmp = _hid;
                _hid = other._hid;
                other._hid = tmp;
            }

            operator hid_t() const noexcept
            {
                return _hid;
            }

        private:
            hid_t _hid;
        };
    }


    // FILE HANDLING

    namespace detail
    {
        // Opens an existing HDF5 file.
        inline
        detail::unique_hid<H5Fclose>
        do_open_file(std::string const& filename, bool readonly)
        {
            auto const file = H5Fopen(
                filename.c_str(),
                readonly ? H5F_ACC_RDONLY : H5F_ACC_RDWR,
                H5P_DEFAULT
            );
            if (file < 0) {
                throw h5::exception("cannot open file");
            }
            return file;
        }


        // Creates an empty HDF5 file.
        inline
        detail::unique_hid<H5Fclose>
        do_create_file(std::string const& filename, bool truncate)
        {
            auto const file = H5Fcreate(
                filename.c_str(),
                truncate ? H5F_ACC_TRUNC : H5F_ACC_EXCL,
                H5P_DEFAULT,
                H5P_DEFAULT
            );
            if (file < 0) {
                throw h5::exception("cannot create file");
            }
            return file;
        }


        // Opens an HDF5 file if exists, or creates an empty one otherwise.
        inline
        detail::unique_hid<H5Fclose>
        do_open_or_create_file(std::string const& filename)
        {
            return std::filesystem::exists(filename)
                ? do_open_file(filename, false)
                : do_create_file(filename, false);
        }


        // Opens or creates an HDF5 file based on given mode string.
        inline
        detail::unique_hid<H5Fclose>
        open_file(std::string const& filename, std::string const& mode)
        {
            if (mode == "r") {
                return detail::do_open_file(filename, true);
            }
            if (mode == "r+") {
                return detail::do_open_file(filename, false);
            }
            if (mode == "w") {
                return detail::do_create_file(filename, true);
            }
            if (mode == "w-") {
                return detail::do_create_file(filename, false);
            }
            if (mode == "a" || mode == "rw") {
                return detail::do_open_or_create_file(filename);
            }
            throw h5::exception("unrecognized file mode");
        }
    }


    // HDF5 PATH HANDLING

    namespace detail
    {
        inline
        std::string get_parent_path(std::string const& path)
        {
            auto const sep_pos = path.rfind('/');
            if (sep_pos == std::string::npos) {
                return "";
            }
            return path.substr(0, sep_pos);
        }


        // Checks if the last component of the path exists. It fails if ancestor
        // groups do not exist.
        inline
        bool check_path_exists(hid_t file, std::string const& path)
        {
            auto const exists = H5Lexists(file, path.c_str(), H5P_DEFAULT);
            if (exists < 0) {
                throw h5::exception("cannot check if a path exists");
            }
            return exists > 0;
        }


        // Checks if the path and its ancestors exist.
        inline
        bool check_path_exists_full(hid_t file, std::string const& path)
        {
            if (auto const parent = detail::get_parent_path(path); !parent.empty()) {
                if (!check_path_exists_full(file, parent)) {
                    return false;
                }
            }
            return detail::check_path_exists(file, path);
        }
    }


    // GROUP HANDLING

    namespace detail
    {
        // Creates a group at the path. This function fails if parent component
        // of the path does not exist.
        inline
        detail::unique_hid<H5Gclose>
        create_group(hid_t file, std::string const& path)
        {
            auto const group = H5Gcreate2(
                file, path.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT
            );
            if (group < 0) {
                throw h5::exception("cannot create group");
            }
            return group;
        }


        // Ensures a group and its ancestor groups exist at given path. Does
        // nothing if path is empty.
        inline
        void ensure_group_full(hid_t file, std::string const& path)
        {
            if (path.empty()) {
                return;
            }

            if (auto const parent = detail::get_parent_path(path); !parent.empty()) {
                ensure_group_full(file, parent);
            }

            if (!detail::check_path_exists(file, path)) {
                detail::create_group(file, path);
            }
        }
    }


    // DATASET HANDLING

    namespace detail
    {
        // FIXME: Datatype and rank validation?

        // Opens an existing dataset.
        inline
        detail::unique_hid<H5Dclose>
        do_open_dataset(hid_t file, std::string const& path)
        {
            auto const dataset = H5Dopen2(file, path.c_str(), H5P_DEFAULT);
            if (dataset < 0) {
                throw h5::exception("cannot open dataset");
            }
            return dataset;
        }


        // Creates an empty dataset at the given path. Missing ancestor groups
        // are created as needed.
        inline
        detail::unique_hid<H5Dclose>
        do_create_dataset(hid_t file, std::string const& path, hid_t datatype, int rank)
        {
            detail::ensure_group_full(file, detail::get_parent_path(path));

            // Create an empty dataset.
            detail::unique_hid<H5Sclose> dataspace = -1;

            if (rank == 0) {
                dataspace = H5Screate(H5S_SCALAR);
            } else {
                std::vector<hsize_t> dims(static_cast<std::size_t>(rank), 0);
                dataspace = H5Screate_simple(rank, dims.data(), dims.data());
            }

            auto const dataset = H5Dcreate2(
                file,
                path.c_str(),
                datatype,
                dataspace,
                H5P_DEFAULT,
                H5P_DEFAULT,
                H5P_DEFAULT
            );
            if (dataset < 0) {
                throw h5::exception("cannot create dataset");
            }
            return dataset;
        }


        inline
        bool check_dataset_type(hid_t dataset, hid_t datatype, int rank)
        {
            detail::unique_hid<H5Tclose> type = H5Dget_type(dataset);
            if (type < 0) {
                throw h5::exception("cannot get datatype for a dataset");
            }

            H5T_cdata_t* cdata = nullptr;
            if (H5Tfind(type, datatype, &cdata) == nullptr) {
                return false; // incompatible type
            }

            detail::unique_hid<H5Sclose> space = H5Dget_space(dataset);
            if (space < 0) {
                throw h5::exception("cannot get dataspace for a dataset");
            }

            if (H5Sget_simple_extent_ndims(space) != rank) {
                return false; // rank mismatch
            }

            return true;
        }


        inline
        detail::unique_hid<H5Dclose>
        open_or_create_dataset(
            hid_t file, std::string const& path, hid_t datatype, int rank
        )
        {
            auto dataset = detail::check_path_exists_full(file, path)
                ? detail::do_open_dataset(file, path)
                : detail::do_create_dataset(file, path, datatype, rank);

            if (!detail::check_dataset_type(dataset, datatype, rank)) {
                throw h5::exception("unexpected dataset type or rank");
            }

            return dataset;
        }
    }


    template<int rank>
    struct shape
    {
        std::size_t dims[rank];
    };


    struct dataset_options
    {
        int compression = -1;
        int scaleoffset = -1;
    };


    // Simple dataset
    template<typename D, int rank>
    class dataset
    {
    public:
        dataset(hid_t file, std::string const& path)
            : _file{file}
            , _path{path}
            , _dataset{detail::open_or_create_dataset(file, path, h5::storage_type_v<D>, rank)}
        {
        }

        h5::shape<rank> shape() const
        {
            detail::unique_hid<H5Sclose> space = H5Dget_space(_dataset);
            if (space < 0) {
                throw h5::exception("cannot determine dataspace");
            }

            int const ndims = H5Sget_simple_extent_ndims(space);
            if (ndims != rank) {
                throw h5::exception("rank mismatch");
            }

            hsize_t sizes[rank];
            if (H5Sget_simple_extent_dims(space, sizes, nullptr) < 0) {
                throw h5::exception("cannot determine extent");
            }

            h5::shape<rank> shape;
            for (int i = 0; i < rank; i++) {
                shape.dims[i] = static_cast<std::size_t>(sizes[i]);
            }
            return shape;
        }

        template<typename B>
        void read(B* buf, h5::shape<rank> const& buf_shape);

        template<typename B>
        void write(
            B const* buf,
            h5::shape<rank> const& buf_shape,
            h5::dataset_options const& options = {}
        );

    private:
        hid_t _file;
        std::string _path;
        detail::unique_hid<H5Dclose> _dataset;
    };


    class file
    {
    public:
        file(std::string const& filename, std::string const& mode = "a")
            : _file{detail::open_file(filename, mode)}
        {
        }

        template<typename D, int rank>
        h5::dataset<D, rank> dataset(std::string const& path)
        {
            return h5::dataset<D, rank>{_file, path};
        }

    private:
        detail::unique_hid<H5Fclose> _file;
    };
}
