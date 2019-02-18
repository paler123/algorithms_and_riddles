#include <array>

/*
 * Small access wrapper for an array.
 * Underlying storage kept puclic to allow efficient construction as an aggregate
 */
template <typename T, std::size_t rows_, std::size_t cols_>
struct Array2d {
    constexpr T& operator()(std::size_t row, std::size_t col) {
        return storage[index(row, col)];
    }

    constexpr T const & operator()(std::size_t row, std::size_t col) const {
        return storage[index(row, col)];
    }

    constexpr std::size_t rows() const { return rows_; }

    constexpr std::size_t cols() const { return cols_; }

    std::array<T, rows_ * cols_> storage;

private:
    constexpr std::size_t index (std::size_t row, std::size_t col) const {
        return rows_ * row + col;
    }
};

template <std::size_t rows, std::size_t cols>
using Problem = Array2d<int, rows, cols>;

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

/*
 *template <typename T1, typename T2, typename... Ts>
 *constexpr bool is_same_v = std::is_same_v<T1, T2> && is_same_v<T2, Ts...>;
 *
 *template <typename T1, typename T2>
 *constexpr bool is_same_v<T1, T2> = std::is_same_v<T1, T2>;
 */

template <typename T>
struct cols_invoker {
    auto operator()(T& t) {
         return t.cols();
    }
};

template <typename T>
struct rows_invoker {
    auto operator()(T& t) {
         return t.rows();
    }
};

template <typename T>
struct index_op_invoker {
    auto operator()(T& t) {
        return t(0, 0);
    }
};

template <typename T, template <typename M> class invoker>
using invoked_type = std::result_of_t<invoker<T>(T&)>;

template <typename Res, typename T, template<typename M> class invoker>
constexpr bool right_type_on_invoke = std::is_same_v<Res,
          invoked_type<T, invoker>>;

template <typename T>
constexpr bool has_index_operator =
    std::is_arithmetic_v<
        std::decay_t<
            std::result_of_t<index_op_invoker<T>(T&)>>>;

template <typename T>
constexpr bool has_rows = right_type_on_invoke<std::size_t, T, rows_invoker>;

template <typename T>
constexpr bool has_cols = right_type_on_invoke<std::size_t, T, cols_invoker>;


} // namespace Details


template <typename T>
using is_matrix_of_arithmetic_types = std::enable_if_t<
    Details::has_index_operator<T> &&
    Details::has_rows<T> &&
    Details::has_cols<T>
>;

/*
 * Finds max sum of elements of input Matrix, with following constraints:
 * Exactly one element from each row can be selected
 * If element at (i, j) has been selected, then (i + 1, j) can't be selected
 */
template<typename Matrix, typename = is_matrix_of_arithmetic_types<Matrix>>
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

struct S {};

int main() {
    constexpr auto trivial = Problem<1u, 1u>{{1}};
    static_assert(MaxSum::solve(trivial) == 1);

    constexpr auto problem2x2_0 = Problem<2u, 2u>{{1, 0, 0, 1}};
    static_assert(MaxSum::solve(problem2x2_0) == 2);

    constexpr auto problem2x2_1 = Problem<2u, 2u>{{10, 0, 9, 0}};
    static_assert(MaxSum::solve(problem2x2_1) == 10);

    constexpr auto problem2x2_2 = Problem<2u, 2u>{{10, 2, 9, 0}};
    static_assert(MaxSum::solve(problem2x2_2) == 11);

    constexpr auto problem1x5 = Problem<1u, 5u>{{10, 2, 9, 7, 6}};
    static_assert(MaxSum::solve(problem1x5) == 10);

    constexpr auto problem1x7 = Problem<1u, 7u>{{10, 2, 9, 7, 6, 12, 11}};
    static_assert(MaxSum::solve(problem1x7) == 12);

    constexpr auto problem3x3 = Problem<3u, 3u>{{1, 2, 3,
                                                 5, 6, 4,
                                                 3, 2, 4}};
    static_assert(MaxSum::solve(problem3x3) == 13);

    constexpr auto problem4x4 = Problem<4u, 4u>{{1, 2, 3, 4,
                                                 5, 6, 7, 8,
                                                 9, 1, 4, 2,
                                                 6, 3, 5, 7}};
    static_assert(MaxSum::solve(problem4x4) == 27);
}
