#ifndef OBSERVER_PTR_HPP
#define OBSERVER_PTR_HPP

#include <cstddef>  // nullptr_t

namespace ext
{
    template<typename T>
    class observer_ptr
    {
      public:
        using element_type = T;
        using pointer = T*;
        using reference = T&;

        observer_ptr() noexcept;
        observer_ptr(std::nullptr_t) noexcept;
        explicit observer_ptr(pointer ptr) noexcept;

        explicit operator bool() const noexcept;
        reference operator*() const;
        pointer operator->() const;

      private:
        pointer ptr_ = nullptr;
    };

    /// observer_ptr is a non-owning pointer to a single object.
    template<typename T>
    class observer_ptr;

    /// Constructs an observer_ptr pointing to nothing.
    template<typename T>
    observer_ptr<T>::observer_ptr() noexcept = default;

    /// Constructs an observer_ptr pointing to nothing.
    template<typename T>
    observer_ptr<T>::observer_ptr(std::nullptr_t) noexcept
    {
    }

    /// Constructs an observer_ptr pointing to the same object as given raw
    /// pointer.
    template<typename T>
    observer_ptr<T>::observer_ptr(pointer ptr) noexcept
        : ptr_{ptr}
    {
    }

    /// operator bool checks whether an object is pointed to by *this.
    ///
    /// Returns true if pointer is non-null, or false otherwise.
    template<typename T>
    observer_ptr<T>::operator bool() const noexcept
    {
        return ptr_ != nullptr;
    }

    /// operator* provides access to the pointed object by reference.
    template<typename T>
    auto observer_ptr<T>::operator*() const -> reference
    {
        return *ptr_;
    }

    /// operator-> provides access to the pointed object as a raw pointer.
    template<typename T>
    auto observer_ptr<T>::operator->() const -> pointer
    {
        return ptr_;
    }
}

#endif
