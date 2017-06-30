#include <utility>
#include <cassert>

template<typename T>
void sort2(T& x, T& y) {
    bool const is_sorted = (x < y);
    T min = std::move(is_sorted ? x : y);
    T max = std::move(is_sorted ? y : x);
    x = std::move(min);
    y = std::move(max);
}

template<typename T>
T median(T x, T y, T z) {
    sort2(x, y);
    sort2(y, z);
    sort2(x, y);
    return y;
}

int main() {
    assert(median(1, 2, 3) == 2);
    assert(median(2, 3, 1) == 2);
    assert(median(3, 1, 2) == 2);
    assert(median(3, 2, 1) == 2);
    assert(median(2, 1, 3) == 2);
    assert(median(1, 3, 2) == 2);
}
