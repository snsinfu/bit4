#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <vector>

#include <highfive/H5Attribute.hpp>


namespace HighFive::details
{
    // Adapt std::vector<std::array<T, D>> as (*,D)-CArray.
    template<typename T, std::size_t D>
    struct data_converter<std::vector<std::array<T, D>>, void>
    {
        using row_type = std::array<T, D>;
        using array_type = std::vector<row_type>;

        static_assert(
            sizeof(row_type) == sizeof(T[D]),
            "This code assumes no padding in std::array"
        );

        data_converter(array_type& array, DataSpace& space, std::size_t dim = 0)
        {
            const auto dims = space.getDimensions();
            assert(dims.size() == 2);
            assert(dims[1] == D);
            (void) array;
            (void) dim;
            _rows = dims[0];
        }

        T* transform_read(array_type& array)
        {
            array.resize(_rows);
            return array[0].data();
        }

        T* transform_write(array_type& array)
        {
            return array[0].data();
        }

    private:
        std::size_t _rows;
    };
}
