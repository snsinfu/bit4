#ifndef INCLUDED_COPY_HPP
#define INCLUDED_COPY_HPP

namespace ext
{
    // Creates a copy of given object.
    template<typename T>
    T copy(T const& obj);
}

namespace ext
{
    template<typename T>
    T copy(T const& obj)
    {
        return obj;
    }
}

#endif
