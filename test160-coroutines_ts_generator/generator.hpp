#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <cassert>
#include <cstdlib>
#include <experimental/coroutine>
#include <iterator>
#include <memory>
#include <utility>

// N4680 [dcl.fct.def.coroutine]
template<typename T>
struct generator
{
    struct promise_type;

    using handle_type = std::experimental::coroutine_handle<promise_type>;

    struct promise_type
    {
        T current_value;

        auto get_return_object()
        {
            return generator{handle_type::from_promise(*this)};
        }

        auto initial_suspend()
        {
            return std::experimental::suspend_never{};
        }

        auto final_suspend()
        {
            return std::experimental::suspend_always{};
        }

        void unhandled_exception()
        {
            std::terminate();
        }

        void return_void()
        {
        }

        auto yield_value(T value)
        {
            current_value = std::move(value);
            return std::experimental::suspend_always{};
        }
    };

    struct sentinel
    {
    };

    struct iterator
    {
        using value_type = T;
        using reference = T const&;
        using pointer = T const*;
        using difference_type = long;
        using iterator_category = std::input_iterator_tag;

        explicit iterator(generator& gen)
            : gen_{gen}
        {
        }

        bool operator==(sentinel const&) const
        {
            return gen_.done();
        }

        bool operator!=(sentinel const& sen) const
        {
            return !(*this == sen);
        }

        reference operator*() const
        {
            return gen_.value();
        }

        pointer operator->() const
        {
            return std::addressof(**this);
        }

        iterator& operator++()
        {
            gen_.advance();
            return *this;
        }

        iterator operator++(int)
        {
            auto copy = *this;
            ++*this;
            return copy;
        }

      private:
        generator& gen_;
    };

    generator(generator&& other)
        : coro_{std::move(other.coro_)}
    {
        other.coro_ = nullptr;
    }

    generator(generator const&) = delete;
    generator& operator=(generator const&) = delete;
    generator& operator=(generator&&) = delete;

    ~generator()
    {
        if (coro_) {
            coro_.destroy();
        }
    }

    void advance()
    {
        assert(coro_);
        assert(!coro_.done());
        coro_.resume();
    }

    bool done()
    {
        return coro_.done();
    }

    T const& value()
    {
        return coro_.promise().current_value;
    }

    iterator begin()
    {
        return iterator{*this};
    }

    sentinel end()
    {
        return {};
    }

  private:
    explicit generator(handle_type coro)
        : coro_{coro}
    {
    }

    handle_type coro_;
};

#endif
