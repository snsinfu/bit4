#ifndef INCLUDED_INDEX_HPP
#define INCLUDED_INDEX_HPP

#include <xtensor/xstrided_view.hpp>


namespace ij
{
    template<typename E>
    auto i(E&& vector)
    {
        return xt::view(vector, xt::all(), xt::newaxis());
    }
}

namespace ijk
{
    template<typename E>
    auto ij(E&& matrix)
    {
        return xt::view(matrix, xt::all(), xt::all(), xt::newaxis());
    }

    template<typename E>
    auto ik(E&& matrix)
    {
        return xt::view(matrix, xt::all(), xt::newaxis(), xt::all());
    }

    template<typename E>
    auto kj(E&& matrix)
    {
        return xt::view(xt::transpose(matrix), xt::newaxis(), xt::all(), xt::all());
    }
}

#endif
