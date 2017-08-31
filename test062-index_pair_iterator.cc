#include <iostream>
#include <iterator>
#include <utility>

#include <cassert>
#include <cstddef>

namespace
{
    template<typename Iterator>
    struct iterator_range
    {
        iterator_range(Iterator begin, Iterator end);

        Iterator begin() const;
        Iterator end() const;

      private:
        Iterator begin_;
        Iterator end_;
    };

    template<typename Iterator>
    iterator_range<Iterator>::iterator_range(Iterator begin, Iterator end)
        : begin_{begin}, end_{end}
    {
    }

    template<typename Iterator>
    auto iterator_range<Iterator>::begin() const -> Iterator
    {
        return begin_;
    }

    template<typename Iterator>
    auto iterator_range<Iterator>::end() const -> Iterator
    {
        return end_;
    }
}

namespace
{
    template<typename Index>
    struct index_pair_iterator
    {
        using value_type = std::pair<Index, Index>;
        using reference = value_type const&;
        using pointer = value_type const*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        index_pair_iterator() = default;
        index_pair_iterator(Index i, Index j);

        bool operator==(index_pair_iterator const& other) const;
        bool operator!=(index_pair_iterator const& other) const;

        index_pair_iterator& operator++();
        index_pair_iterator operator++(int);
        reference operator*() const;
        pointer operator->() const;

      private:
        value_type indices_ = {0, 1};
    };

    template<typename Index>
    index_pair_iterator<Index>::index_pair_iterator(Index i, Index j)
        : indices_{i, j}
    {
        assert(i < j);
    }

    template<typename Index>
    bool index_pair_iterator<Index>::operator==(index_pair_iterator const& other) const
    {
        return indices_ == other.indices_;
    }

    template<typename Index>
    bool index_pair_iterator<Index>::operator!=(index_pair_iterator const& other) const
    {
        return !(*this == other);
    }

    template<typename Index>
    auto index_pair_iterator<Index>::operator++() -> index_pair_iterator&
    {
        ++indices_.first;

        if (indices_.first == indices_.second) {
            indices_.first = 0;
            ++indices_.second;
        }

        return *this;
    }

    template<typename Index>
    auto index_pair_iterator<Index>::operator++(int) -> index_pair_iterator
    {
        index_pair_iterator copy{*this};
        ++*this;
        return copy;
    }

    template<typename Index>
    auto index_pair_iterator<Index>::operator*() const -> reference
    {
        return indices_;
    }

    template<typename Index>
    auto index_pair_iterator<Index>::operator->() const -> pointer
    {
        return &indices_;
    }

    template<typename Index>
    iterator_range<index_pair_iterator<Index>> make_index_pair_range(Index n)
    {
        return {{0, 1}, {0, n}};
    }
}

double test(double const* r, std::size_t n)
{
    double sum = 0;
    for (auto ij : make_index_pair_range(n)) {
        sum += r[ij.first] * r[ij.second];
    }
    return sum;
}

int main()
{
    index_pair_iterator<int> beg{0, 1};
    index_pair_iterator<int> end{0, 5};

    for (index_pair_iterator<int> i = beg; i != end; ++i) {
        std::cout << i->first << '\t' << i->second << '\n';
    }

    for (auto ij : make_index_pair_range(4)) {
        std::cout << ij.first << '\t' << ij.second << '\n';
    }

    double r[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    std::cout << test(r, 10) << '\n';
}
