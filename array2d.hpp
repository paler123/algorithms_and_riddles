#pragma once
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
