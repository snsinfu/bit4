// RAII-fied Loan Pattern cache updating. It involves extra heap allocation,
// causes UB on exception inside update(), looks hackish and is non-intuitive.
// I should just use the Loan Pattern.

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
    struct deferred_task
    {
        virtual ~deferred_task() = default;
    };

    template<typename F>
    struct deferred_task_erasure : deferred_task
    {
        explicit deferred_task_erasure(F func)
            : func_{std::move(func)}
        {
        }

        ~deferred_task_erasure()
        {
            func_(); // Can't throw!
        }

      private:
        F func_;
    };

    template<typename F>
    std::unique_ptr<deferred_task> defer(F func)
    {
        return std::make_unique<deferred_task_erasure<F>>(std::move(func));
    }

    template<typename T>
    struct array_view
    {
        array_view(T* ptr, std::size_t size)
            : ptr_{ptr}
            , size_{size}
        {
        }

        template<typename F>
        array_view(T* ptr, std::size_t size, F debt)
            : ptr_{ptr}
            , size_{size}
            , debt_{defer(std::move(debt))}
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
        std::unique_ptr<deferred_task> debt_;
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

        array_view<int const> view() const
        {
            return array_view<int const>{values_.data(), values_.size()};
        }

        array_view<int> mutate()
        {
            return array_view<int>{values_.data(), values_.size(), [this] {
                update();
            }};
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

    // Ugly scope
    {
        auto values = sum.mutate();
        std::iota(values.begin(), values.end(), 1);

        assert(sum.value() == 0);  // Ugh, non-intuitive and surely bug prone
    }
    assert(sum.value() == 55);

    // Ugly scope
    {
        auto values = sum.view();
        std::cout << values.size() << " numbers: ";
        std::copy(values.begin(), values.end(), std::ostream_iterator<int>{std::cout, " "});
        std::cout << '\n';
    }
}
