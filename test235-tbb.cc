#include <iostream>
#include <thread>

#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>

int main()
{
    tbb::task_scheduler_init tbb_init;

    int count = 1000;
    std::mutex mutex;

    tbb::parallel_for(0, count, [&](int index) {
        std::lock_guard<std::mutex> lock{mutex};
        std::cout << index << '\t' << std::this_thread::get_id() << '\n';
    });
}

// c++ test235-tbb.cc -ltbb
