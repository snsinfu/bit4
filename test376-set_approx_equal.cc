#include <algorithm>
#include <cstddef>
#include <functional>
#include <vector>


template<typename T>
struct nodeduce
{
    using type = T;
};

template<typename T>
using nodeduce_t = typename nodeduce<T>::type;

template<typename T, typename U>
double maxmin_shuffle(std::vector<T>& items,
                      std::vector<U> const& reference,
                      std::function<double(nodeduce_t<T> const&, nodeduce_t<U> const&)> score)
{
    // Eager algorithm. No optimal guarantee.

    auto min_score = 0.0;
    auto remaining_begin = items.begin();

    for (std::size_t ref_index = 0; ref_index < reference.size(); ++ref_index) {
        auto best_score = 0.0;
        auto best_iter = remaining_begin;

        U const& ref_item = reference[ref_index];

        for (auto iter = remaining_begin; iter != items.end(); ++iter) {
            auto const cur_score = score(*iter, ref_item);

            if (iter == remaining_begin || cur_score > best_score) {
                best_score = cur_score;
                best_iter = iter;
            }
        }

        if (ref_index == 0 || best_score < min_score) {
            min_score = best_score;
        }

        std::iter_swap(remaining_begin, best_iter);
        ++remaining_begin;
    }

    return min_score;
}


//------------------------------------------------------------------------------

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main()
{
    std::vector<std::string> const ref_items = {
        "alice", "bob", "charlie"
    };
    std::vector<std::string> items = {
        "billing", "alibaba", "channel"
    };

    // We use the matching prefix length as a score.
    auto score = [](std::string const& s1, std::string const& s2) {
        auto const mismatch = std::mismatch(s1.begin(), s1.end(),
                                            s2.begin(), s2.end());
        return std::distance(s1.begin(), mismatch.first);
    };

    std::cout << maxmin_shuffle(items, ref_items, score) << '\n';

    for (auto const& item : items) {
        std::cout << item << '\n';
    }
}
