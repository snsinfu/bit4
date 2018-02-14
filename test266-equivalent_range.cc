#include <cstddef>
#include <iostream>

using std::size_t;

// x = 3
//
// [-----------------)
// 1 2 3 3 3 3 4 4 5 .
//         ^ (== x)
//
//           [-------)
// 1 2 3 3 3 3 4 4 5 .
//               ^ (> x)
//
//           [---)
// 1 2 3 3 3 3 4 4 5 .
//             ^ (> x)
//
//           [-)
// 1 2 3 3 3 3 4 4 5 .
//           ^ (== x)
//
//             |
// 1 2 3 3 3 3 4 4 5 .

template<size_t size>
size_t upper_bound(double(& array)[size], double target)
{
    size_t begin = 0;
    size_t end = size;

    while (begin < end) {
        size_t index = (begin + end) / 2;

        if (array[index] > target) {
            end = index;
        } else {
            begin = index + 1;
        }
    }

    return begin;
}

template<size_t size>
size_t lower_bound(double(& array)[size], double target)
{
    size_t begin = 0;
    size_t end = size;

    while (begin < end) {
        size_t index = (begin + end) / 2;

        if (array[index] < target) {
            begin = index + 1;
        } else {
            end = index;
        }
    }

    return begin;
}

int main()
{
    double array[] = {1, 2, 3, 3, 3, 3, 4, 4, 5};
    double target = 3;
    size_t upper = upper_bound(array, target);
    size_t lower = lower_bound(array, target);

    for (double value : array) {
        if (lower-- == 0) {
            std::cout << "[ ";
        }
        if (upper-- == 0) {
            std::cout << "] ";
        }
        std::cout << value << ' ';
    }
    std::cout << '\n';
}
