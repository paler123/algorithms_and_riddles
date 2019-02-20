#include "array2d.hpp"
#include "max_sum_solution.hpp"
#include <catch2/catch.hpp>

template <std::size_t rows, std::size_t cols>
using Problem = Array2d<int, rows, cols>;

TEST_CASE("Right max sum is found for problems", "[Numeric solver]") {

  SECTION("TRIVIAL") {
    constexpr auto trivial = Problem<1u, 1u>{{1}};
    STATIC_REQUIRE(MaxSum::solve(trivial) == 1);
  }
  SECTION("ILL FORMED") {
    constexpr auto ill_formed = Problem<5u, 3u>{};
    STATIC_REQUIRE(MaxSum::solve(ill_formed) == 0);
  }
  SECTION("NON TRIVIAL") {
    constexpr auto problem2x2_0 = Problem<2u, 2u>{{1, 0, 0, 1}};
    STATIC_REQUIRE(MaxSum::solve(problem2x2_0) == 2);

    constexpr auto problem2x2_1 = Problem<2u, 2u>{{10, 0, 9, 0}};
    STATIC_REQUIRE(MaxSum::solve(problem2x2_1) == 10);

    constexpr auto problem2x2_2 = Problem<2u, 2u>{{10, 2, 9, 0}};
    STATIC_REQUIRE(MaxSum::solve(problem2x2_2) == 11);

    constexpr auto problem1x5 = Problem<1u, 5u>{{10, 2, 9, 7, 6}};
    STATIC_REQUIRE(MaxSum::solve(problem1x5) == 10);

    constexpr auto problem1x7 = Problem<1u, 7u>{{10, 2, 9, 7, 6, 12, 11}};
    STATIC_REQUIRE(MaxSum::solve(problem1x7) == 12);

    constexpr auto problem3x3 = Problem<3u, 3u>{{1, 2, 3, 5, 6, 4, 3, 2, 4}};
    STATIC_REQUIRE(MaxSum::solve(problem3x3) == 13);

    constexpr auto problem4x4 =
        Problem<4u, 4u>{{1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 4, 2, 6, 3, 5, 7}};
    STATIC_REQUIRE(MaxSum::solve(problem4x4) == 27);
  }
}
