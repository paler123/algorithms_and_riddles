#pragma once

#include <cstddef>
#include "matrix_type_traits.hpp"

namespace MaxSum {
namespace Details {

template <typename T>
struct IndexedRowValue {
    T val;
    std::size_t index;
};

template <typename T>
struct TopTwo {
    IndexedRowValue<T> max;
    IndexedRowValue<T> almost_max;
};

template <typename T>
constexpr TopTwo<T> sorted_first_two (T v1, T v2) {
    auto m1 = IndexedRowValue<T>{v1, 0};
    auto m2 = IndexedRowValue<T>{v2, 1};
    return v1 >= v2
        ? TopTwo<T>{m1, m2}
        : TopTwo<T>{m2, m1};
}

template <typename T>
constexpr void update_top (TopTwo<T> & top_paths, T path_value,
                           std::size_t col) {
    if (path_value > top_paths.max.val) {
        top_paths.almost_max = top_paths.max;
        top_paths.max = IndexedRowValue<T>{path_value, col};
    }
    else if (path_value > top_paths.almost_max.val)
        top_paths.almost_max = IndexedRowValue<T>{path_value, col};
}

template<typename Matrix>
constexpr auto find_top2_in_first_row (Matrix const &input) {
    auto result = sorted_first_two(input(0, 0), input(0, 1));
    for (auto i = 2u; i < input.cols(); ++i)
        update_top(result,input(0, i), i);
    return result;
}

template <typename T>
constexpr T best_path_value_through_element(TopTwo<T> const & top_last_row,
                                              T val,
                                              std::size_t col){
    return top_last_row.max.index != col
        ? top_last_row.max.val + val
        : top_last_row.almost_max.val + val;
}


template<typename T, typename Matrix>
constexpr TopTwo<T> find_best_paths_for_row(TopTwo<T> const & top_last_row,
                                                 std::size_t row,
                                                 Matrix const & input) {
    auto path_0 = best_path_value_through_element(top_last_row, input(row, 0), 0u);
    auto path_1 = best_path_value_through_element(top_last_row, input(row, 1), 1u);
    auto top_paths = sorted_first_two(path_0, path_1);
    for (auto i = 2u; i < input.cols(); ++i) {
        auto path_i = best_path_value_through_element(top_last_row, input(row, i), i);
        update_top<T>(top_paths, path_i, i);
    }
    return top_paths;
}

template<typename Matrix>
constexpr auto solve_non_trivial(Matrix const & input) {
    auto top_paths = find_top2_in_first_row(input);
    for (auto i = 1u; i < input.rows(); ++i)
        top_paths = find_best_paths_for_row(top_paths, i, input);
    // key observation: optimal path at row i is either best or second best at i - 1
    return top_paths.max.val;
}

} // namespace Details


/*
 * Finds max sum of elements of input Matrix, with following constraints:
 * Exactly one element from each row can be selected
 * If element at (i, j) has been selected, then (i + 1, j) can't be selected
 */
template<typename Matrix,
         typename = MatrixTypeTraits::is_matrix_of_arithmetic_types<Matrix>>
constexpr auto solve (Matrix const & input){
    auto result = input(0, 0);
    // special case for ill-shaped matrices
    if (input.rows() > input.cols())
        result = 0;
    else if (input.rows() <=  input.cols() && input.cols() > 1){
        result = Details::solve_non_trivial(input);
    }
    return result;
}

} // namespace MaxSum
