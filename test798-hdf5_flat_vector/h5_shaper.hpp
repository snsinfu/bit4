#pragma once

#include <highfive/H5Attribute.hpp>
#include <highfive/bits/H5Utils.hpp>


// Attach 2D shape to flat array.
template<typename T>
class shaper_2d
{
public:
    shaper_2d(T* data, std::size_t rows, std::size_t cols)
        : _data(data), _shape{rows, cols}
    {
    }

    T* data() const
    {
        return _data;
    }

    std::size_t size() const
    {
        return _shape[0] * _shape[1];
    }

    std::vector<std::size_t> shape() const
    {
        return _shape;
    }

    void reshape(std::size_t rows, std::size_t cols)
    {
        assert(rows * cols <= size());
        _shape = {rows, cols};
    }

private:
    T* _data;
    std::vector<std::size_t> _shape;
};


namespace HighFive::details
{
    template<typename T>
    struct array_dims<shaper_2d<T>>
    {
        static constexpr std::size_t value = 2;
    };

    template<typename T>
    struct type_of_array<shaper_2d<T>>
    {
        using type = T;
    };

    template<typename T>
    struct data_converter<shaper_2d<T>, void>
    {
        using array_type = shaper_2d<T>;

        inline data_converter(array_type& array, DataSpace& space, size_t dim = 0)
            : _dims(space.getDimensions())
        {
            assert(_dims.size() == 2);
            (void) array;
            (void) dim;
        }

        T* transform_read(array_type& array)
        {
            array.reshape(_dims[0], _dims[1]);
            return array.data();
        }

        T* transform_write(array_type& array)
        {
            return array.data();
        }

        void process_result(array_type& array)
        {
            (void) array;
        }

    private:
        std::vector<std::size_t> _dims;
    };
}
