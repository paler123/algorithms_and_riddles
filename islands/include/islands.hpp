#pragma once

#include "dynamic_matrix.hpp"
#include <queue>

namespace islands
{

namespace Details
{


using Index = std::pair<std::size_t, std::size_t>;

template<typename Matrix>
void add_to_queue_if_matches(Matrix const & input, std::queue<Index> & to_visit,
                             DynamicMatrix<bool> const & visited,
                             std::size_t row, std::size_t col,
                             std::size_t nrow, std::size_t ncol)
{
  if (!visited(nrow, ncol) && input(nrow, ncol) == input(row, col))
    to_visit.emplace(nrow, ncol);
}

template <typename Matrix>
void add_neighbours_to_queue(Matrix const & input, std::queue<Index> & to_visit,
                             DynamicMatrix<bool> const & visited,
                             std::size_t row, std::size_t col)
{
  if (row > 0) add_to_queue_if_matches(input, to_visit, visited, row, col, row - 1, col);
  if (col > 0) add_to_queue_if_matches(input, to_visit, visited, row, col, row, col - 1);
  if (row < input.rows() - 1)
    add_to_queue_if_matches(input, to_visit, visited, row, col, row + 1, col);
  if (col < input.cols() - 1)
    add_to_queue_if_matches(input, to_visit, visited, row, col, row, col + 1);
}

template <typename Matrix>
void visit(Matrix const & input, DynamicMatrix<bool> & visited,
           std::size_t row, std::size_t col)
{
  std::queue<Index> to_visit;
  to_visit.emplace(row, col);
  while (!to_visit.empty())
  {
    auto [row, col] = to_visit.front();
    visited(row, col) = true;
    add_neighbours_to_queue(input, to_visit, visited, row, col);
    to_visit.pop();
  }
}

} // namespace Details

template <typename Matrix>
int get_number_of_islands(Matrix const & input)
{
  int result = 0;
  DynamicMatrix<bool> visited{std::vector<bool>(input.rows() * input.cols(), false),
                              input.rows(), input.cols()};
  for (auto r = 0u; r < input.rows(); ++r)
  {
    for (auto c = 0u; c < input.cols(); ++c)
    {
      if (!visited(r, c))
      {
        ++result;
        Details::visit(input, visited, r, c);
      }
    }
  }

  return result;
}

} // namespace islands
