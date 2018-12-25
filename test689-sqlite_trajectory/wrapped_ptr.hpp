#ifndef WRAPPED_PTR_HPP
#define WRAPPED_PTR_HPP

#include <memory>


template<typename T, int(* close_fun)(T*)>
struct wrapped_ptr : std::unique_ptr<T, int(*)(T*)>
{
    using unique_ptr = std::unique_ptr<T, int(*)(T*)>;

    wrapped_ptr()
        : unique_ptr{nullptr, close_fun}
    {
    }

    operator T*() const
    {
        return unique_ptr::get();
    }
};

#endif
