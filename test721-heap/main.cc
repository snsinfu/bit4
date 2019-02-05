#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <vector>


using bench_clock = std::chrono::steady_clock;

constexpr int repeat_count = 100000;
constexpr int element_count = 100;
constexpr unsigned numgen_mult = 32939363;
constexpr unsigned numgen_mod = 16;


// O(n^2)
void benchmark_insertion()
{
    std::vector<unsigned> numbers;

    const auto start_time = bench_clock::now();

    for (int repeat = 0; repeat < repeat_count; repeat++) {
        numbers.clear();

        unsigned num = 1;
        for (int i = 0; i < element_count; i++) {
            num *= numgen_mult;
            num %= numgen_mod;

            // Binary search and insertion.
            numbers.push_back(num);

            std::greater<unsigned> order;
            std::rotate(
                numbers.rbegin(),
                numbers.rbegin() + 1,
                std::lower_bound(numbers.rbegin() + 1, numbers.rend(), num, order)
            );
        }
    }

    const auto finish_time = bench_clock::now();
    const auto elapsed_time = finish_time - start_time;
    const auto elapsed_time_sec =
        std::chrono::duration_cast<std::chrono::duration<double>>(
            elapsed_time
        ).count();

    const auto recip_throughput = 1e9 * elapsed_time_sec / repeat_count;
    std::cout << "insertion: " << recip_throughput << " ns/op\n";
}


// O(n log n)
void benchmark_set()
{
    std::set<unsigned> numbers;

    const auto start_time = bench_clock::now();

    for (int repeat = 0; repeat < repeat_count; repeat++) {
        numbers.clear();

        unsigned num = 1;
        for (int i = 0; i < element_count; i++) {
            num *= numgen_mult;
            num %= numgen_mod;

            numbers.insert(num);
        }
    }

    const auto finish_time = bench_clock::now();
    const auto elapsed_time = finish_time - start_time;
    const auto elapsed_time_sec =
        std::chrono::duration_cast<std::chrono::duration<double>>(
            elapsed_time
        ).count();

    const auto recip_throughput = 1e9 * elapsed_time_sec / repeat_count;
    std::cout << "set: " << recip_throughput << " ns/op\n";
}


// O(n log n)
void benchmark_prioqueue()
{
    std::priority_queue<unsigned> numbers;

    const auto start_time = bench_clock::now();

    for (int repeat = 0; repeat < repeat_count; repeat++) {
        while (!numbers.empty()) {
            numbers.pop();
        }

        unsigned num = 1;
        for (int i = 0; i < element_count; i++) {
            num *= numgen_mult;
            num %= numgen_mod;

            numbers.push(num);
        }
    }

    const auto finish_time = bench_clock::now();
    const auto elapsed_time = finish_time - start_time;
    const auto elapsed_time_sec =
        std::chrono::duration_cast<std::chrono::duration<double>>(
            elapsed_time
        ).count();

    const auto recip_throughput = 1e9 * elapsed_time_sec / repeat_count;
    std::cout << "prioqueue: " << recip_throughput << " ns/op\n";
}


// O(n log n)
void benchmark_heap()
{
    std::vector<unsigned> numbers;

    const auto start_time = bench_clock::now();

    for (int repeat = 0; repeat < repeat_count; repeat++) {
        numbers.clear();

        unsigned num = 1;
        for (int i = 0; i < element_count; i++) {
            num *= numgen_mult;
            num %= numgen_mod;

            numbers.push_back(num);
            std::push_heap(numbers.begin(), numbers.end());
        }
    }

    const auto finish_time = bench_clock::now();
    const auto elapsed_time = finish_time - start_time;
    const auto elapsed_time_sec =
        std::chrono::duration_cast<std::chrono::duration<double>>(
            elapsed_time
        ).count();

    const auto recip_throughput = 1e9 * elapsed_time_sec / repeat_count;
    std::cout << "heap: " << recip_throughput << " ns/op\n";
}


// O(n log n)
void benchmark_sort()
{
    std::vector<unsigned> numbers;

    const auto start_time = bench_clock::now();

    for (int repeat = 0; repeat < repeat_count; repeat++) {
        numbers.clear();

        unsigned num = 1;
        for (int i = 0; i < element_count; i++) {
            num *= numgen_mult;
            num %= numgen_mod;

            numbers.push_back(num);
        }

        std::sort(numbers.begin(), numbers.end());
    }

    const auto finish_time = bench_clock::now();
    const auto elapsed_time = finish_time - start_time;
    const auto elapsed_time_sec =
        std::chrono::duration_cast<std::chrono::duration<double>>(
            elapsed_time
        ).count();

    const auto recip_throughput = 1e9 * elapsed_time_sec / repeat_count;
    std::cout << "sort: " << recip_throughput << " ns/op\n";
}


// O(n)
void benchmark_raw()
{
    std::vector<unsigned> numbers;

    const auto start_time = bench_clock::now();

    for (int repeat = 0; repeat < repeat_count; repeat++) {
        numbers.clear();

        unsigned num = 1;
        for (int i = 0; i < element_count; i++) {
            num *= numgen_mult;
            num %= numgen_mod;

            numbers.push_back(num);
        }
    }

    const auto finish_time = bench_clock::now();
    const auto elapsed_time = finish_time - start_time;
    const auto elapsed_time_sec =
        std::chrono::duration_cast<std::chrono::duration<double>>(
            elapsed_time
        ).count();

    const auto recip_throughput = 1e9 * elapsed_time_sec / repeat_count;
    std::cout << "raw: " << recip_throughput << " ns/op\n";
}


int main()
{
    benchmark_insertion();
    benchmark_set();
    benchmark_prioqueue();
    benchmark_heap();
    benchmark_sort();
    benchmark_raw();
}
