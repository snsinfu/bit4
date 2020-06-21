#include <cstddef>
#include <iostream>
#include <random>
#include <utility>


char const color_red[] = "\x1b[31m";
char const color_gray[] = "\x1b[90m";
char const color_reset[] = "\x1b[m";


using std::size_t;

template<typename Random>
void seed(Random& random);

template<typename T, size_t N>
size_t size(T(&)[N]);

template<typename T, size_t N>
void show_swap(T const(& arr)[N], size_t i, size_t j);

template<typename T, size_t N>
void show_state(T const(& arr)[N], size_t start);


int main()
{
    std::mt19937_64 random;
    seed(random);

    int values[] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9,
    };
    auto const n_values = size(values);

    show_state(values, n_values);

    if (n_values != 0) {
        for (auto i = n_values - 1; i > 0; i--) {
            std::uniform_int_distribution<size_t> choice{0, i};
            auto j = choice(random);
            show_swap(values, i, j);
            std::swap(values[i], values[j]);
            show_state(values, i);
        }
    }
    show_state(values, 0);
}


template<typename Random>
void seed(Random& random)
{
    std::random_device random_source;
    std::seed_seq seed {
        random_source(), random_source(), random_source(), random_source()
    };
    random.seed(seed);
}


template<typename T, size_t N>
size_t size(T(&)[N])
{
    return N;
}


template<typename T, size_t N>
void show_swap(T const(& arr)[N], size_t i, size_t j)
{
    for (size_t k = 0; k < N; k++) {
        if (k > 0) {
            std::cout << ' ';
        }
        if (k < i) {
            std::cout << color_gray;
        }
        if (k == i || k == j) {
            std::cout << color_red;
        }
        std::cout << arr[k];
        std::cout << color_reset;
    }
    std::cout << '\n';
}


template<typename T, size_t N>
void show_state(T const(& arr)[N], size_t start)
{
    std::cout << color_gray;

    for (size_t i = 0; i < N; i++) {
        if (i == start) {
            std::cout << color_reset;
        }
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << arr[i];
    }
    std::cout << '\n';
}
