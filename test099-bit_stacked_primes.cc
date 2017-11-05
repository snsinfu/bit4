#include <cstdint>
#include <iostream>

bool is_prime(std::uint64_t number)
{
    if (number <= 1) {
        return false;
    }
    if (number == 2 || number == 3) {
        return true;
    }
    if (number % 6 != 1 && number % 6 != 5) {
        return false;
    }

    for (std::uint64_t pivot = 5; pivot * pivot <= number; pivot += 6) {
        if (number % pivot == 0) {
            return false;
        }
        if (number % (pivot + 2) == 0) {
            return false;
        }
    }

    return true;
}

std::uint64_t next_prime(std::uint64_t number)
{
    do {
        number++;
    } while (!is_prime(number));
    return number;
}

int main()
{
    std::uint64_t const min = 2;
    std::uint64_t const max = 1'000'000'000;

    for (std::uint64_t number = min; number <= max; number = next_prime(number)) {
        for (std::uint64_t modulo = 4; modulo < number; modulo <<= 1) {
            std::cout << (is_prime(number % modulo) ? '|' : '.');
        }
        std::cout << ' ' << number << '\n';
    }
}
