#include <algorithm>
#include <chrono>
#include <iostream>
#include <set>
#include <vector>

#include <boost/container/flat_set.hpp>


using bench_clock = std::chrono::steady_clock;

constexpr int repeat_count = 100000;
constexpr int element_count = 30;
constexpr unsigned numgen_mult = 32939363;
constexpr unsigned numgen_mod = 16;


template<typename Op>
double benchmark(Op op)
{
    const auto start_time = bench_clock::now();

    for (int repeat = 0; repeat < repeat_count; repeat++) {
        op();
    }

    const auto finish_time = bench_clock::now();
    const auto elapsed_time = finish_time - start_time;
    const auto elapsed_time_sec =
        std::chrono::duration_cast<std::chrono::duration<double>>(
            elapsed_time
        ).count();

    return elapsed_time_sec / repeat_count;
}


// O(n)
void benchmark_raw()
{
    std::vector<unsigned> numbers;

    const auto recip_throughput = benchmark([&] {
        numbers.clear();

        unsigned num = 1;
        for (int i = 0; i < element_count; i++) {
            num *= numgen_mult;
            num %= numgen_mod;
            numbers.push_back(num);
        }
    });

    std::cout << "raw: " << recip_throughput * 1e9 << " ns/op\n";
}


// O(n log n)
void benchmark_set()
{
    std::set<unsigned> numbers;

    const auto recip_throughput = benchmark([&] {
        numbers.clear();

        unsigned num = 1;
        for (int i = 0; i < element_count; i++) {
            num *= numgen_mult;
            num %= numgen_mod;
            numbers.insert(num);
        }
    });

    std::cout << "set: " << recip_throughput * 1e9 << " ns/op\n";
}


// O(n log n)
void benchmark_flatset()
{
    boost::container::flat_set<unsigned> numbers;

    const auto recip_throughput = benchmark([&] {
        numbers.clear();

        unsigned num = 1;
        for (int i = 0; i < element_count; i++) {
            num *= numgen_mult;
            num %= numgen_mod;
            numbers.insert(num);
        }
    });

    std::cout << "flatset: " << recip_throughput * 1e9 << " ns/op\n";
}


// O(n log n)
void benchmark_sortuniq()
{
    std::vector<unsigned> numbers;

    const auto recip_throughput = benchmark([&] {
        numbers.clear();

        unsigned num = 1;
        for (int i = 0; i < element_count; i++) {
            num *= numgen_mult;
            num %= numgen_mod;
            numbers.push_back(num);
        }

        std::sort(numbers.begin(), numbers.end());
        numbers.erase(
            std::unique(numbers.begin(), numbers.end()),
            numbers.end()
        );
    });

    std::cout << "sortuniq: " << recip_throughput * 1e9 << " ns/op\n";
}


// O(n log n)
void benchmark_heap()
{
    std::vector<unsigned> numbers;

    const auto recip_throughput = benchmark([&] {
        numbers.clear();

        unsigned num = 1;
        for (int i = 0; i < element_count; i++) {
            num *= numgen_mult;
            num %= numgen_mod;
            numbers.push_back(num);
            std::push_heap(numbers.begin(), numbers.end());
        }

        std::sort_heap(numbers.begin(), numbers.end());
    });

    std::cout << "heap: " << recip_throughput * 1e9 << " ns/op\n";
}


int main()
{
    benchmark_raw();
    benchmark_set();
    benchmark_flatset();
    benchmark_sortuniq();
    benchmark_heap();
}
