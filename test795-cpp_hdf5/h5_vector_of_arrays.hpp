#pragma once

#include <array>
#include <cassert>
#include <vector>

#include <highfive/bits/H5Utils.hpp>


namespace HighFive::details
{
    template<typename T, size_t N>
    struct data_converter<std::vector<std::array<T, N>>, void>
    {
        using Matrix = std::vector<std::array<T, N>>;

        inline data_converter(Matrix& array, DataSpace& space, size_t dim = 0)
            : _rows(space.getDimensions()[0])
        {
            auto dims = space.getDimensions();
            assert(dims.size() == 2);
            assert(dims[1] == N);
            (void) dim;
            (void) array;
        }

        typename type_of_array<T>::type* transform_read(Matrix& array)
        {
            array.resize(_rows);
            return array[0].data();
        }

        typename type_of_array<T>::type* transform_write(Matrix& array)
        {
            return array[0].data();
        }

        void process_result(Matrix& array)
        {
            (void) array;
        }

    private:
        size_t _rows;
    };
}
