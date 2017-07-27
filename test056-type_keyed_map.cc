#include <iterator>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <cassert>
#include <cstddef>

namespace detail
{
    template<typename T, typename U>
    using copy_const_t = std::conditional_t<
        std::is_const<T>::value, std::add_const_t<U>, std::remove_const_t<U>
    >;

    template<typename T>
    struct type_keyed_map
    {
        template<typename Map>
        struct basic_iterator;

        using iterator = basic_iterator<type_keyed_map>;
        using const_iterator = basic_iterator<type_keyed_map const>;

        template<typename K, typename... Args>
        void emplace(Args&&... args)
        {
            map_.emplace(typeid(K), T(std::forward(args)...));
        }

        template<typename K>
        T& get()
        {
            return map_.at(typeid(K));
        }

        template<typename K>
        T const& get() const
        {
            return map_.at(typeid(K));
        }

        template<typename K>
        bool has() const noexcept
        {
            return map_.find(typeid(K)) != map_.end();
        }

        iterator begin() noexcept
        {
            return iterator{map_.begin()};
        }

        iterator end() noexcept
        {
            return iterator{map_.end()};
        }

        const_iterator begin() const noexcept
        {
            return const_iterator{map_.begin()};
        }

        const_iterator end() const noexcept
        {
            return const_iterator{map_.end()};
        }

      private:
        std::unordered_map<std::type_index, T> map_;
    };

    template<typename T>
    template<typename Map>
    struct type_keyed_map<T>::basic_iterator
    {
        using underlying_iterator = std::conditional_t<
            std::is_const<Map>::value,
            typename std::unordered_map<std::type_index, T>::const_iterator,
            typename std::unordered_map<std::type_index, T>::iterator
        >;

        using value_type = T;
        using access_type = detail::copy_const_t<Map, T>;
        using pointer = access_type*;
        using reference = access_type&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        basic_iterator() = default;

        explicit
        basic_iterator(underlying_iterator iter)
            : iter_{iter}
        {
        }

        // Support iterator -> const_iterator conversion
        basic_iterator(basic_iterator<std::remove_const_t<Map>> const& other)
            : iter_{other.base()}
        {
        }

        bool operator==(basic_iterator const& other) const
        {
            return iter_ == other.iter_;
        }

        bool operator!=(basic_iterator const& other) const
        {
            return !(*this == other);
        }

        reference operator*() const
        {
            return iter_->second;
        }

        pointer operator->() const
        {
            return std::addressof(iter_->second);
        }

        basic_iterator& operator++()
        {
            ++iter_;
            return *this;
        }

        basic_iterator operator++(int)
        {
            basic_iterator copy{*this};
            ++*this;
            return copy;
        }

        underlying_iterator base() const
        {
            return iter_;
        }

      private:
        underlying_iterator iter_;
    };
}

int main()
{
    detail::type_keyed_map<int> tkm;

    for (int& x : tkm) {
        x = 42;
    }
}
