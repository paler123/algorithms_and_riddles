#include "array2d.hpp"
#include "max_sum_solution.hpp"
#include <catch2/catch.hpp>

template <std::size_t rows, std::size_t cols>
using Problem = Array2d<int, rows, cols>;

TEST_CASE("Right paths found for problems", "[Numeric solver]")
{

  SECTION("TRIVIAL")
  {
    constexpr auto trivial = Problem<1u, 1u>{{1}};
    auto expected_result = std::make_pair(1, std::vector<std::size_t>{0});
    REQUIRE(MaxSum::solve_with_path(trivial) == expected_result);
  }
  SECTION("ILL FORMED")
  {
    constexpr auto ill_formed = Problem<5u, 1u>{};
    auto expected_result = std::make_pair(0, std::vector<std::size_t>{});
    REQUIRE(MaxSum::solve_with_path(ill_formed) == expected_result);
  }
  SECTION("NON TRIVIAL")
  {
    constexpr auto problem2x2_0 = Problem<2u, 2u>{{1, 0, 0, 1}};
    auto expected_result = std::make_pair(2, std::vector<std::size_t>{0, 1});
    REQUIRE(MaxSum::solve_with_path(problem2x2_0) == expected_result);

    constexpr auto problem2x2_1 = Problem<2u, 2u>{{10, 0, 9, 0}};
    expected_result = {10, std::vector<std::size_t>{0, 1}};
    auto actual = MaxSum::solve_with_path(problem2x2_1);
    REQUIRE(MaxSum::solve_with_path(problem2x2_1) == expected_result);

    constexpr auto problem2x2_2 = Problem<2u, 2u>{{10, 2, 9, 0}};
    expected_result = {11, std::vector<std::size_t>{1, 0}};
    actual = MaxSum::solve_with_path(problem2x2_2);
    REQUIRE(MaxSum::solve_with_path(problem2x2_2) == expected_result);

    constexpr auto problem1x5 = Problem<1u, 5u>{{10, 2, 9, 7, 6}};
    expected_result = {10, std::vector<std::size_t>{0}};
    REQUIRE(MaxSum::solve_with_path(problem1x5) == expected_result);

    constexpr auto problem1x7 = Problem<1u, 7u>{{10, 2, 9, 7, 6, 12, 11}};
    expected_result = {12, std::vector<std::size_t>{5}};
    REQUIRE(MaxSum::solve_with_path(problem1x7) == expected_result);

    constexpr auto problem3x3 = Problem<3u, 3u>{{1, 2, 3, 5, 6, 4, 3, 2, 4}};
    expected_result = {13, std::vector<std::size_t>{2, 1, 2}};
    REQUIRE(MaxSum::solve_with_path(problem3x3) == expected_result);
  }
  SECTION("MULTIPLE PATHS")
  {
    constexpr auto problem4x4 = Problem<4u, 4u>{{1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 4, 2, 6, 3, 5, 7}};
    auto possible_result_1 = std::make_pair(27, std::vector<std::size_t>{2, 3, 0, 3});
    auto possible_result_2 = std::make_pair(27, std::vector<std::size_t>{3, 2, 0, 3});
    auto actual = MaxSum::solve_with_path(problem4x4);
    REQUIRE((actual == possible_result_1 || actual == possible_result_2));
  }
  SECTION("LESS COLUMNS THAN ROWS")
  {
    constexpr auto problem3x2 = Problem<3u, 2u>{{1, 10, 2, 5, 6, 7}};
    auto expected_result = std::make_pair(19, std::vector<std::size_t>{1, 0, 1});
    REQUIRE(MaxSum::solve_with_path(problem3x2) == expected_result);

    constexpr auto problem4x3 = Problem<4u, 3u>{{1, 1, 4, 2, 3, 6, 4, 7, 8, 8, 3, 1}};
    expected_result = {23, std::vector<std::size_t>{2, 1, 2, 0}};
    REQUIRE(MaxSum::solve_with_path(problem4x3) == expected_result);
  }
}
