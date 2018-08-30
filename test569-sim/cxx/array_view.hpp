// array_view - lightweight range view of array

// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_ARRAY_VIEW_HPP
#define INCLUDED_ARRAY_VIEW_HPP

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>


namespace cxx
{
    // data returns a pointer to the first element of a contiguous container or an array.
    template<typename Cont>
    constexpr auto data(Cont& cont) noexcept -> decltype(cont.data())
    {
        return cont.data();
    }

    template<typename T, std::size_t N>
    constexpr auto data(T (&arr)[N]) noexcept -> T*
    {
        return arr;
    }

    // size returns the number of elements in a container or an array.
    template<typename Cont>
    constexpr auto size(Cont& cont) noexcept -> decltype(cont.size())
    {
        return cont.size();
    }

    template<typename T, std::size_t N>
    constexpr auto size(T (&)[N]) noexcept -> std::size_t
    {
        return N;
    }

    // array_view is a lightweight range view of an array. It is similar to GSL's span but uses
    // size_t for indexing, for better compatibility with the standard library.
    template<typename T>
    class array_view
    {
    public:
        // value_type is the non-qualified type of the elements.
        using value_type = typename std::remove_cv<T>::type;

        // pointer is the type of a pointer to an element.
        using pointer = T*;

        // reference is the type of a reference to an element.
        using reference = T&;

        // size_type is the type of size and index values.
        using size_type = std::size_t;

        // iterator is the type of iterators. Guaranteed to be a random access iterator. Currently
        // this is implemented as an alias of T* but this may change in the future.
        using iterator = T*;

        // reverse_iterator is the type of reverse iterators. Guaranteed to be a random access
        // iterator.
        using reverse_iterator = std::reverse_iterator<iterator>;

        // const_array_view is the type of read-only array_view with the same value_type. This is
        // the same as array_view<T> if T is already const qualified.
        using const_array_view = array_view<T const>;

        // array_view is default constructible.
        array_view() = default;

        // array_view is copy constructible.
        array_view(array_view&) = default;

        // array_view is copy constructible.
        array_view(array_view const&) = default;

        // array_view is constructible from a raw memory span [data, data + size). The behavior is
        // undefined if the memory region is invalid.
        constexpr array_view(pointer data, size_type size)
            : data_{data}, size_{size}
        {
        }

        // array_view is implicitly convertible from a contiguous container like std::vector.
        template<
            typename Container,
            typename P = decltype(cxx::data(std::declval<Container&>())),
            typename S = decltype(cxx::size(std::declval<Container&>()))
        >
        constexpr array_view(Container& container) noexcept
            : array_view{cxx::data(container), cxx::size(container)}
        {
            static_assert(
                std::is_convertible<typename std::remove_pointer<P>::type(*)[], T(*)[]>::value,
                "incompatible element type"
            );
        }

        // empty returns true if the view is empty.
        constexpr bool empty() const noexcept
        {
            return size() == 0;
        }

        // size returns the number of elements.
        constexpr size_type size() const noexcept
        {
            return size_;
        }

        // data r eturns a pointer to the first element. The pointer may or may not be null if the
        // view is empty.
        constexpr pointer data() const noexcept
        {
            return data_;
        }

        // front returns a reference to the first element. The behavior is undefined if the view is
        // empty.
        constexpr reference front() const
        {
            return operator[](0);
        }

        // back returns a reference to the last element. The behavior is undefined if the view is
        // empty.
        constexpr reference back() const
        {
            return operator[](size() - 1);
        }

        // Indexing operator returns a reference to the idx-th element. The behavior is undefined
        // if the index is out of bounds.
        constexpr reference operator[](size_type idx) const
        {
            return data()[idx];
        }

        // at returns a reference to the idx-th element. It throws std::out_of_range if the index is
        // out of bounds.
        reference at(size_type idx) const
        {
            if (idx >= size()) {
                throw std::out_of_range("array_view access out-of-bounds");
            }
            return operator[](idx);
        }

        // begin returns an iterator to the beginning of the viewed array.
        constexpr iterator begin() const noexcept
        {
            return data();
        }

        // end returns an iterator to the end of the viewed array.
        constexpr iterator end() const noexcept
        {
            return data() + size();
        }

        // rbegin returns a reverse iterator to the reverse beginning.
        reverse_iterator rbegin() const noexcept
        {
            return reverse_iterator{end()};
        }

        // rend returns a reverse iterator to the reverse end.
        reverse_iterator rend() const noexcept
        {
            return reverse_iterator{begin()};
        }

        // as_const returns a read-only view of the same array.
        constexpr const_array_view as_const() const noexcept
        {
            return {data(), size()};
        }

        // swap swaps the viewed array with other.
        void swap(array_view& other) noexcept
        {
            auto const copy = *this;
            *this = other;
            other = copy;
        }

        // subview returns a view of the subarray with given region.
        constexpr array_view subview(size_type offset, size_type count) const
        {
            return {data() + offset, count};
        }

        // first returns a view of the first count elements.
        constexpr array_view first(size_type count) const
        {
            return subview(0, count);
        }

        /// last returns a view of the last count elements.
        constexpr array_view last(size_type count) const
        {
            return subview(size() - count, count);
        }

    private:
        pointer data_ = nullptr;
        size_type size_ = 0;
    };
} // namespace cxx

#endif // INCLUDED_ARRAY_VIEW_HPP
