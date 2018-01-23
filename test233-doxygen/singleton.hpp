#ifndef SINGLETON_HPP
#define SINGLETON_HPP

// So. Doxygen supports the compromise[1] where declaration and definition both
// have doxygen comments. I assumed that Doxygen used declaration's comment as
// a brief description and definition's comment as a detailed description but
// the assumption seemed to be wrong. Well, the output documentation had two
// brief descriptions! The first one was from declaration and the second one
// from the first sentense of definition's comment.
//
// [1]: https://www.stack.nl/~dimitri/doxygen/manual/docblocks.html
//
// So essentially we have two choices. a) Concise header with no documentation
// comment and dense implementation with all the documentation, or b) dense
// header with all the documentation. b) is the standard.

namespace ext
{
    /// Single-value container.
    template<typename T>
    class singleton
    {
      public:

        using value_type = T;
        using reference = T&;
        using const_reference = T const&;
        using iterator = T*;
        using const_iterator = T const*;

        /// Default-initializes the contained value.
        singleton();

        /// Initializes the contained value to a copy of given one.
        explicit singleton(T const& other);

        /// Initializes the contained value to given one.
        explicit singleton(T&& other);

        /// Returns an iterator pointing to the contained value.
        iterator begin();

        /// Returns an iterator pointing to past the contained value.
        iterator end();

        /// Returns an iterator pointing to the contained value.
        const_iterator begin() const;

        /// Returns an iterator pointing to past the contained value.
        const_iterator end() const;

      private:
        T value_;
    };
}

//------------------------------------------------------------------------------

#include <utility>

namespace ext
{
    /// A singleton object always contains a single object of type T. This
    /// class has no actual use -- it is just for fiddling with Doxygen anyway.
    template<typename T>
    class singleton;

    /// The default constructor value-initializes the contained object. This
    /// constructor is not available if T is not default constructible.
    template<typename T>
    singleton<T>::singleton()
        : value_{}
    {
    }

    /// This constructor copy constructs the contained object with given one.
    template<typename T>
    singleton<T>::singleton(T const& other)
        : value_{other}
    {
    }

    // This constructor move constructs the contained object with given one.
    template<typename T>
    singleton<T>::singleton(T&& other)
        : value_{std::move(other)}
    {
    }

    template<typename T>
    auto singleton<T>::begin() -> iterator
    {
        return &value_;
    }

    template<typename T>
    auto singleton<T>::end() -> iterator
    {
        return begin() + 1;
    }

    template<typename T>
    auto singleton<T>::begin() const -> const_iterator
    {
        return &value_;
    }

    template<typename T>
    auto singleton<T>::end() const -> const_iterator
    {
        return begin() + 1;
    }
}

#endif
