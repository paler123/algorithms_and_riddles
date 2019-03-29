#pragma once
#include <vector>

/*
 * Small access wrapper for an array.
 * Underlying storage kept puclic to allow efficient construction as an
 * aggregate
 */
template <typename T>
struct DynamicMatrix
{
  constexpr typename std::vector<T>::reference operator()(std::size_t row, std::size_t col)
  {
    return storage[index(row, col)];
  }

  constexpr typename std::vector<T>::const_reference operator()(std::size_t row, std::size_t col) const
  {
    return storage[index(row, col)];
  }

  constexpr std::size_t rows() const
  {
    return rows_;
  }

  constexpr std::size_t cols() const
  {
    return cols_;
  }

  std::vector<T> storage;
  std::size_t rows_;
  std::size_t cols_;

private:
  constexpr std::size_t index(std::size_t row, std::size_t col) const
  {
    return cols_ * row + col;
  }
};
