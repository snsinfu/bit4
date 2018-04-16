#ifndef INCLUDED_TESTUTIL_HPP
#define INCLUDED_TESTUTIL_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <vector>

#include <xtensor/xeval.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>


// Transforms N-tensor to a vector of (N-1)-tensors.
template<typename T, std::size_t N>
std::vector<xt::xtensor<T, N - 1>> extract_rows(xt::xtensor<T, N> const& tensor)
{
    std::size_t const row_count = tensor.shape()[0];

    std::vector<xt::xtensor<T, N - 1>> rows;
    rows.reserve(row_count);

    for (std::size_t row_index = 0; row_index < row_count; ++row_index) {
        rows.push_back(xt::view(tensor, row_index));
    }

    return rows;
}

// Prevents template type deduction.
template<typename T>
struct nodeduce
{
    using type = T;
};

template<typename T>
using nodeduce_t = typename nodeduce<T>::type;

// Reorders vector to match items to reference ones. This function uses an
// eager algorithm and does not guarantee the optimal result.
template<typename T, typename U>
double maxmin_reorder(std::vector<T>& items,
                      std::vector<U> const& reference,
                      std::function<double(nodeduce_t<T> const&, nodeduce_t<U> const&)> score)
{
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

template<typename T, typename U>
double maxmin_reorder(std::vector<T>&& items,
                      std::vector<U> const& reference,
                      std::function<double(nodeduce_t<T> const&, nodeduce_t<U> const&)> score)
{
    return maxmin_reorder(items, reference, score);
}

#endif
