#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

using std::size_t;


class subview_iterator
{
public:
    using value_type = double;
    using reference = const double&;
    using pointer = const double*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    subview_iterator(const double* data, const size_t* index)
        : _data(data), _index(index)
    {
    }

    bool operator==(const subview_iterator& other) const
    {
        return _index == other._index;
    }

    bool operator!=(const subview_iterator& other) const
    {
        return !(*this == other);
    }

    reference operator*() const
    {
        return _data[*_index];
    }

    subview_iterator& operator++()
    {
        _index++;
        return *this;
    }

    subview_iterator operator++(int)
    {
        auto copy = *this;
        ++*this;
        return copy;
    }

private:
    const double* _data;
    const size_t* _index;
};


// Array view with holes and overlaps.
class subview
{
public:
    subview(const double* data, size_t size)
        : _data(data), _size(size)
    {
    }

    explicit subview(const std::vector<double>& vec)
        : subview(vec.data(), vec.size())
    {
    }

    size_t size() const
    {
        return _pos.size();
    }

    void add_view(size_t begin, size_t end)
    {
        assert(end <= _size);

        for (size_t i = begin; i < end; i++) {
            _pos.push_back(i);
        }
    }

    const double& operator[](size_t i) const
    {
        return _data[_pos[i]];
    }

    subview_iterator begin() const
    {
        return subview_iterator{_data, _pos.data()};
    }

    subview_iterator end() const
    {
        return subview_iterator{_data, _pos.data() + _pos.size()};
    }

private:
    std::vector<size_t> _pos;
    const double* _data;
    size_t _size;
};


int main()
{
    std::vector<double> coords = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    };

    subview view(coords);
    view.add_view(1, 5);
    view.add_view(6, 8);
    view.add_view(2, 9);

    for (auto x : view) {
        std::cout << x << '\n';
    }
}
