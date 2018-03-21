// Mediant [1] is more bit-efficient than naive average when finding a rational
// number bounded by two rational numbers.
//
// Ideas from a HN discussion [2] on storing user-defined order scores in a SQL
// table [3]. This can be thought of as maintaining a tree in a sequential
// list. Another discussion: [4]. Interesting!
//
// [1]: https://en.wikipedia.org/wiki/Mediant_(mathematics)
// [2]: https://news.ycombinator.com/item?id=16635440
// [3]: https://begriffs.com/posts/2018-03-20-user-defined-order.html
// [4]: https://news.ycombinator.com/item?id=13517490

// Naive rational --------------------------------------------------------------

#include <ostream>
#include <utility>

struct rational
{
    int numerator = 0;
    int denominator = 1;

    explicit operator double() const
    {
        return static_cast<double>(numerator) / static_cast<double>(denominator);
    }
};

bool operator<(rational const& lhs, rational const& rhs)
{
    return lhs.numerator * rhs.denominator < rhs.numerator * lhs.denominator;
}

std::ostream& operator<<(std::ostream& os, rational const& r)
{
    return os << r.numerator << '/' << r.denominator;
}

int gcd(int x, int y)
{
    for (;;) {
        if (x == 0) {
            return y;
        }
        y %= x;

        std::swap(x, y);
    }
}

rational reduce(int numerator, int denominator)
{
    auto const div = gcd(numerator, denominator);
    return rational{numerator / div, denominator / div};
}

rational mediant(rational const& r, rational const& s)
{
    return rational{r.numerator + s.numerator, r.denominator + s.denominator};
}

rational average(rational const& r, rational const& s)
{
    auto const num = r.numerator * s.denominator + s.numerator * r.denominator;
    auto const den = 2 * r.denominator * s.denominator;
    return reduce(num, den);
}

//------------------------------------------------------------------------------

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <set>

void test(std::function<rational(rational, rational)> midpoint)
{
    int const initial_count = 3;
    int const insertion_count = 15;

    // Initial inventory
    std::set<rational> inventory;

    for (int i = 0; i < initial_count; ++i) {
        inventory.insert(rational{i});
    }

    // Insert items at random position
    std::default_random_engine random_engine;

    for (int i = 0; i < insertion_count; ++i) {
        std::uniform_int_distribution<std::size_t> index{0, inventory.size() - 2};
        auto const pre = std::next(std::begin(inventory), index(random_engine));
        auto const post = std::next(pre);
        auto const item = midpoint(*pre, *post);
        inventory.insert(item);
    }

    // Result
    for (auto const& item : inventory) {
        std::cout << item << '\t' << static_cast<double>(item) << '\n';
    }
}

int main()
{
    std::cout << "Average:\n";
    test(average);

    std::cout << "\nMediant:\n";
    test(mediant);
}
