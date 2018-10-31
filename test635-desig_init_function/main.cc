#include <functional>
#include <iostream>


struct config
{
    int steps = 0;
    std::function<void(int)> callback;
};

void run_long_computation(config const& c)
{
    std::function<void(int)> callback = c.callback ? c.callback : [](int) {};

    for (int step = 0; step < c.steps; step++) {
        callback(step);
    }
}

int main()
{
    run_long_computation({
        .steps = 10,
        .callback = [](int step) { std::cout << step << '\n'; }
    });
}
