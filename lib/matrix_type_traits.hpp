#pragma once
#include <type_traits>

namespace MatrixTypeTraits {
namespace Details {

template <typename T> struct cols_invoker {
  auto operator()(T &t) { return t.cols(); }
};

template <typename T> struct rows_invoker {
  auto operator()(T &t) { return t.rows(); }
};

template <typename T> struct index_op_invoker {
  auto operator()(T &t) { return t(0, 0); }
};

template <typename T, template <typename M> class invoker>
using invoked_type = std::remove_reference_t<
    std::remove_cv_t<std::result_of_t<invoker<T>(T &)>>>;

template <typename Res, typename T, template <typename M> class invoker>
constexpr bool right_type_on_invoke =
    std::is_same_v<Res, invoked_type<T, invoker>>;

template <typename T>
constexpr bool has_index_operator =
    std::is_arithmetic_v<std::remove_reference_t<
        std::remove_cv_t<std::result_of_t<index_op_invoker<T>(T &)>>>>;

template <typename T>
constexpr bool has_rows = right_type_on_invoke<std::size_t, T, rows_invoker>;

template <typename T>
constexpr bool has_cols = right_type_on_invoke<std::size_t, T, cols_invoker>;

} // namespace Details

template <typename T>
using is_matrix_of_arithmetic_types =
    std::enable_if_t<Details::has_index_operator<T> && Details::has_rows<T> &&
                     Details::has_cols<T>>;

} // namespace MatrixTypeTraits
