#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string_view>
#include <vector>


int main()
{
    std::string_view document = "I'm sure those are not the right words";
    std::vector<std::size_t> suffix_array(document.size());

    std::iota(std::begin(suffix_array), std::end(suffix_array), 0);
    std::sort(std::begin(suffix_array), std::end(suffix_array), [&](std::size_t i, std::size_t j) {
        return document.substr(i) < document.substr(j);
    });

    std::string_view word = "re";

    std::size_t lower_bound = 0;
    std::size_t upper_bound = suffix_array.size();
    std::size_t pos;
    int steps = 0;

    while (lower_bound + 1 < upper_bound) {
        steps += 1;

        auto index = (lower_bound + upper_bound) / 2;
        pos = suffix_array[index];
        auto prefix = document.substr(pos, word.size());
        auto cmp = prefix.compare(word);

        if (cmp < 0) {
            lower_bound = index;
        } else if (cmp > 0) {
            upper_bound = index;
        } else {
            break;
        }
    }

    std::cout << steps << " steps\n";

    if (lower_bound + 1 < upper_bound) {
        std::cout << "'"
                  << word
                  << "' found at "
                  << pos
                  << ": "
                  << document.substr(pos)
                  << '\n';
    } else {
        std::cout << "'"
                  << word
                  << "' not found\n";
    }

    // TODO: Show all hits.
}
