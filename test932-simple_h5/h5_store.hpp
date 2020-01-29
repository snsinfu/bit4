#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>


enum class h5_type
{
    type_i32,
    type_u32,
    type_float,
};


template<typename T>
struct h5_type_of;

template<>
struct h5_type_of<std::int32_t> { static constexpr auto value = h5_type::type_i32; };

template<>
struct h5_type_of<std::uint32_t> { static constexpr auto value = h5_type::type_u32; };

template<>
struct h5_type_of<float> { static constexpr auto value = h5_type::type_float; };


class h5_store
{
public:
    explicit h5_store(std::string const& filename);
    ~h5_store();

    void write_string(std::string const& path, std::string const& data);

    template<typename T>
    void write_array(std::string const& path, T const* data, std::size_t len)
    {
        write_raw_array(path, data, h5_type_of<T>::value, { len });
    }

    template<typename T>
    void write_array(std::string const& path, T const* data, std::size_t rows, std::size_t cols)
    {
        write_raw_array(path, data, h5_type_of<T>::value, { rows, cols });
    }

private:
    void write_raw_array(
        std::string const& path,
        void const* data,
        h5_type type,
        std::vector<std::size_t> const& shape
    );

private:
    struct context;
    std::unique_ptr<context> _context;
};
