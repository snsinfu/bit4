#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>

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


    namespace detail
    {
        using std::int32_t;
        using std::uint32_t;
        using std::int64_t;
        using std::uint64_t;

        // Datatype values are determined at run time.
        template<typename T>
        hid_t storage_type() = delete;

        template<> inline hid_t storage_type<int32_t >() { return H5T_STD_I32LE; }
        template<> inline hid_t storage_type<int64_t >() { return H5T_STD_I64LE; }
        template<> inline hid_t storage_type<uint32_t>() { return H5T_STD_U32LE; }
        template<> inline hid_t storage_type<uint64_t>() { return H5T_STD_U64LE; }
        template<> inline hid_t storage_type<float   >() { return H5T_IEEE_F32LE; }
        template<> inline hid_t storage_type<double  >() { return H5T_IEEE_F64LE; }

        template<typename T>
        hid_t memory_type() = delete;

        template<> inline hid_t memory_type<int32_t >() { return H5T_NATIVE_INT32; }
        template<> inline hid_t memory_type<int64_t >() { return H5T_NATIVE_INT64; }
        template<> inline hid_t memory_type<uint32_t>() { return H5T_NATIVE_UINT32; }
        template<> inline hid_t memory_type<uint64_t>() { return H5T_NATIVE_UINT64; }
        template<> inline hid_t memory_type<float   >() { return H5T_NATIVE_FLOAT; }
        template<> inline hid_t memory_type<double  >() { return H5T_NATIVE_DOUBLE; }

        template<typename T>
        inline hid_t const storage_type_v = detail::storage_type<T>();

        template<typename T>
        inline hid_t const memory_type_v = detail::memory_type<T>();


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

            operator hid_t() noexcept
            {
                return _hid;
            }

        private:
            hid_t _hid;
        };


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


        inline
        detail::unique_hid<H5Gclose>
        do_create_group(hid_t file, std::string const& path)
        {
            auto const group = H5Gcreate2(
                file, path.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT
            );
            if (group < 0) {
                throw h5::exception("cannot create group");
            }
            return group;
        }


        inline
        bool do_check_path_exists(hid_t file, std::string const& path)
        {
            auto const exists = H5Lexists(file, path.c_str(), H5P_DEFAULT);
            if (exists < 0) {
                throw h5::exception("cannot check if a path exists");
            }
            return exists > 0;
        }


        inline
        std::string group_path(std::string const& path)
        {
            auto const sep_pos = path.rfind('/');
            if (sep_pos == std::string::npos) {
                return "";
            }
            return path.substr(0, sep_pos);
        }


        // Ensures a group and its ancestor groups exist at given path. Does
        // nothing if path is empty.
        inline
        void ensure_group(hid_t file, std::string const& path)
        {
            if (path.empty()) {
                return;
            }

            if (auto const parent = detail::group_path(path); !parent.empty()) {
                ensure_group(file, parent);
            }

            if (!detail::do_check_path_exists(file, path)) {
                detail::do_create_group(file, path);
            }
        }


        // Checks if the path and its ancestors exist.
        inline
        bool check_path_exists(hid_t file, std::string const& path)
        {
            if (auto const parent = detail::group_path(path); !parent.empty()) {
                if (!check_path_exists(file, parent)) {
                    return false;
                }
            }
            return do_check_path_exists(file, path);
        }


        inline
        hid_t do_open_dataset(hid_t file, std::string const& path)
        {
            auto const dataset = H5Dopen2(file, path.c_str(), H5P_DEFAULT);
            if (dataset < 0) {
                throw h5::exception("cannot open dataset");
            }
            return dataset;
        }


        inline
        hid_t do_create_dataset(hid_t file, std::string const& path)
        {
            detail::ensure_group(file, group_path(path));
            // TODO
        }


        inline
        unique_hid<H5Dclose>
        open_or_create_dataset(hid_t file, std::string const& path)
        {
            return detail::check_path_exists(file, path)
                ? detail::do_open_dataset(file, path)
                : detail::do_create_dataset(file, path);
        }
    }


    template<int rank>
    struct shape
    {
        std::size_t dims[rank];
    };


    struct dataset_options
    {
        int compression = 0;
        int scaleoffset = -1;
    };


    template<typename T, int rank>
    class dataset
    {
    public:
        dataset(hid_t file, std::string const& path)
            : _file{file}
            , _path{path}
            , _dataset{detail::open_or_create_dataset(file, path)}
        {
        }

        h5::shape<rank> shape() const
        {
            detail::unique_hid<H5Sclose> space = H5Dget_space(_dataset);
            if (space < 0) {
                throw h5::exception("cannot determine dataspace");
            }

            auto const ndims = H5Sget_simple_extent_ndims(space);
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

        template<typename T, int rank>
        h5::dataset<T, rank> dataset(std::string const& path)
        {
            return h5::dataset<T, rank>{_file, path};
        }

    private:
        detail::unique_hid<H5Fclose> _file;
    };
}
