#pragma once

#include "matrix_type_traits.hpp"
#include <cstddef>
#include <vector>

namespace MaxSum
{
namespace Details
{

template <typename T>
struct PathEnd
{
  T val;
  std::size_t index;
};

template <typename T>
struct TopTwo
{
  PathEnd<T> max;
  PathEnd<T> almost_max;
};

struct DiscardPaths
{
  constexpr void init(std::size_t, std::size_t, std::size_t) const
  {
    // intentionally discard the values
  }
  constexpr void adjust_ends(std::size_t, std::size_t) const
  {
    // intentionally discard the values
  }
};

struct StorePaths
{
  void init(std::size_t rows, std::size_t top, std::size_t almost_top)
  {
    top_path.reserve(rows);
    almost_top_path.reserve(rows);
    top_path.push_back(top);
    almost_top_path.push_back(almost_top);
  }
  void adjust_ends(std::size_t top_end, std::size_t almost_top_end)
  {
    // one of paths definetely goes through top
    if (path_covers_current_top_path(top_end))
    {
      if (path_covers_current_top_path(almost_top_end))
      {
        almost_top_path = top_path;
        top_path.push_back(top_end);
        almost_top_path.push_back(almost_top_end);
      }
      else
      {
        top_path.push_back(top_end);
        almost_top_path.push_back(almost_top_end);
      }
    }
    else
    {
      almost_top_path.push_back(top_end);
      top_path.push_back(almost_top_end);
      std::swap(top_path, almost_top_path);
    }
  }

  std::vector<std::size_t> recover_top()
  {
    return std::move(top_path);
  }

private:
  std::vector<std::size_t> top_path;
  std::vector<std::size_t> almost_top_path;
  bool path_covers_current_top_path(std::size_t path_end) const
  {
    return *top_path.crbegin() != path_end;
    // intentionally discard the values
  }
};

template <typename T>
constexpr TopTwo<T> sorted_first_two(T v1, T v2)
{
  auto m1 = PathEnd<T>{v1, 0};
  auto m2 = PathEnd<T>{v2, 1};
  return v1 >= v2 ? TopTwo<T>{m1, m2} : TopTwo<T>{m2, m1};
}

template <typename T>
constexpr void update_top(TopTwo<T>& top_paths, T path_value, std::size_t col)
{
  if (path_value > top_paths.max.val)
  {
    top_paths.almost_max = top_paths.max;
    top_paths.max = PathEnd<T>{path_value, col};
  }
  else if (path_value > top_paths.almost_max.val)
    top_paths.almost_max = PathEnd<T>{path_value, col};
}

template <typename Matrix>
constexpr auto find_top2_in_first_row(Matrix const& input)
{
  auto result = sorted_first_two(input(0, 0), input(0, 1));
  for (auto i = 2u; i < input.cols(); ++i)
    update_top(result, input(0, i), i);
  return result;
}

template <typename T>
constexpr T best_path_value_through_element(TopTwo<T> const& top_last_row, T val, std::size_t col)
{
  return top_last_row.max.index != col ? top_last_row.max.val + val : top_last_row.almost_max.val + val;
}

template <typename T, typename Matrix>
constexpr TopTwo<T> find_best_paths_for_row(TopTwo<T> const& top_last_row, std::size_t row, Matrix const& input)
{
  auto path_0 = best_path_value_through_element(top_last_row, input(row, 0), 0u);
  auto path_1 = best_path_value_through_element(top_last_row, input(row, 1), 1u);
  auto top_paths = sorted_first_two(path_0, path_1);
  for (auto i = 2u; i < input.cols(); ++i)
  {
    auto path_i = best_path_value_through_element(top_last_row, input(row, i), i);
    update_top<T>(top_paths, path_i, i);
  }
  return top_paths;
}

template <typename Matrix, typename PathsPolicy>
constexpr auto solve_non_trivial(Matrix const& input, PathsPolicy&& paths)
{
  auto top_paths = find_top2_in_first_row(input);
  paths.init(input.rows(), top_paths.max.index, top_paths.almost_max.index);
  for (auto i = 1u; i < input.rows(); ++i)
  {
    top_paths = find_best_paths_for_row(top_paths, i, input);
    paths.adjust_ends(top_paths.max.index, top_paths.almost_max.index);
  }
  // key observation: optimal path at row i is either best or second best at i -
  // 1
  return top_paths.max.val;
}

template <typename Matrix, typename PathsPolicy>
constexpr auto solve(Matrix const& input, PathsPolicy&& paths)
{
  auto result = input(0, 0);
  // special case for ill-shaped matrices
  if (input.rows() > 1 && input.cols() == 1)
  {
    result = 0;
  }
  else if (input.cols() > 1)
  {
    result = Details::solve_non_trivial(input, paths);
  }
  else if (input.cols() == 1)
    paths.init(0, 0, 0);
  return result;
}

} // namespace Details

/*
 * Finds max sum of elements of input Matrix, with following constraints:
 * Exactly one element from each row has to be included in the sum
 * If element at (i, j) has been selected, then (i + 1, j) can't be selected
 */
template <typename Matrix, typename = MatrixTypeTraits::is_matrix_of_arithmetic_types<Matrix>>
constexpr auto solve(Matrix const& input)
{
  return Details::solve(input, Details::DiscardPaths{});
}

/*
 * Same as solve, but additionally finds a path through the input matrix realising max.
 * Returns a pair of the max value and path.
 * Path is expressed as vector of column indices of length intput.rows(),
 * that is path.at(i) contains information about which element from ith row is taken.
 */
template <typename Matrix, typename = MatrixTypeTraits::is_matrix_of_arithmetic_types<Matrix>>
auto solve_with_path(Matrix const& input)
{
  Details::StorePaths paths;
  auto max_val = Details::solve(input, paths);
  return std::make_pair(max_val, paths.recover_top());
}

} // namespace MaxSum
