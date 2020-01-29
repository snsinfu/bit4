#pragma once

#include <functional>


namespace defer
{
    struct defer_t
    {
        template<typename F>
        defer_t(F proc) : _proc{proc}
        {
        }

        ~defer_t()
        {
            _proc();
        }

    private:
        std::function<void()> _proc;
    };
}

#define CONCAT_TOKEN1(x, y) x ## y
#define CONCAT_TOKEN2(x, y) CONCAT_TOKEN1(x, y)
#define DEFER defer::defer_t CONCAT_TOKEN2(_defer_, __COUNTER__) = [&]
