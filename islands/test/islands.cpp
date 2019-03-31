#include "islands.hpp"
#include <catch2/catch.hpp>

namespace
{
using namespace islands;

TEST_CASE("correctly calculated", "[islands counting]")
{
  DynamicMatrix<int> single{{
    1, 1,
    1, 1}, 2, 2};
  REQUIRE( 1 == get_number_of_islands(single));

  DynamicMatrix<int> diag{{
    1, 0,
    0, 1}, 2, 2};
  REQUIRE( 4 == get_number_of_islands(diag));

  DynamicMatrix<int> multiple {{
    1, 1, 0, 1,
    0, 1, 1, 1,
    0, 0, 3, 3,
    3, 3, 3, 3,
    4, 3, 4, 3}, 5, 4};
  REQUIRE(6 == get_number_of_islands(multiple));
}

}

