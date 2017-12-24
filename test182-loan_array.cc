// Using Loan Pattern to ensure updating an internal cache while providing raw
// access to an internal data structure.

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <utility>
#include <vector>

namespace ext
{
    // array_view is a non-owning view into an array of Ts.
    template<typename T>
    struct array_view
    {
        explicit array_view(T* ptr, std::size_t size)
            : ptr_{ptr}, size_{size}
        {
        }

        std::size_t size() const noexcept
        {
            return size_;
        }

        T& operator[](std::size_t index)
        {
            return ptr_[index];
        }

        T const& operator[](std::size_t index) const
        {
            return ptr_[index];
        }

        T* begin() const noexcept
        {
            return ptr_;
        }

        T* end() const noexcept
        {
            return ptr_ + size_;
        }

      private:
        T* ptr_;
        std::size_t size_;
    };

    // sum_device is a class for computing the sum of a bunch of numbers with
    // caching. It provides raw memory access for efficiently filling up the
    // numbers.
    struct sum_device
    {
        explicit sum_device(std::size_t size)
            : values_(size)
        {
        }

        int value() const
        {
            return sum_;
        }

        template<typename F>
        void view(F func) const
        {
            func(array_view<int const>{values_.data(), values_.size()});
        }

        template<typename F>
        void mutate(F func)
        {
            func(array_view<int>{values_.data(), values_.size()});
            update();
        }

      private:
        void update()
        {
            sum_ = std::accumulate(values_.begin(), values_.end(), 0);
        }

      private:
        int sum_ = 0;
        std::vector<int> values_;
    };
}

int main()
{
    ext::sum_device sum{10};

    sum.mutate([](auto values) {
        std::iota(values.begin(), values.end(), 1);
    });
    assert(sum.value() == 55);

    sum.view([](auto values) {
        std::cout << values.size() << " numbers: ";
        std::copy(values.begin(), values.end(), std::ostream_iterator<int>{std::cout, " "});
        std::cout << '\n';
    });
}
