#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

namespace
{
    template<typename Iter>
    class range
    {
      public:
        range(Iter begin, Iter end)
            : begin_{begin}, end_{end}
        {
        }

        Iter begin() const
        {
            return begin_;
        }

        Iter end() const
        {
            return end_;
        }

      private:
        Iter begin_;
        Iter end_;
    };

    template<typename Index, typename Base>
    class indexed_iterator
    {
      public:
        using value_type = std::pair<Index, typename std::iterator_traits<Base>::reference>;
        using reference = value_type;
        using pointer = void;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::input_iterator_tag;

        indexed_iterator(Index index, Base base)
            : index_{index}, base_{base}
        {
        }

        Base const& base() const
        {
            return base_;
        }

        reference operator*() const
        {
            return {index_, *base_};
        }

        indexed_iterator& operator++()
        {
            ++index_;
            ++base_;
            return *this;
        }

        indexed_iterator operator++(int)
        {
            auto copy = *this;
            ++*this;
            return copy;
        }

      private:
        Index index_;
        Base base_;
    };

    template<typename Index, typename Base>
    bool operator==(indexed_iterator<Index, Base> const& lhs,
                    indexed_iterator<Index, Base> const& rhs)
    {
        return lhs.base() == rhs.base();
    }

    template<typename Index, typename Base>
    bool operator!=(indexed_iterator<Index, Base> const& lhs,
                    indexed_iterator<Index, Base> const& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename R>
    auto index(R& rng)
    {
        using size_type = typename std::remove_cv_t<R>::size_type;
        return range{
            indexed_iterator{size_type{0}, rng.begin()},
            indexed_iterator{rng.size(), rng.end()}
        };
    }
}

#include <iostream>
#include <vector>

int main()
{
    std::vector<char> chars = {'c', 'h', 'a', 'r', 's'};

    for (auto [ idx, ch ] : index(chars)) {
        ch += static_cast<char>(idx);
    }

    for (auto ch : chars) {
        std::cout << ch << '\n';
    }
}
