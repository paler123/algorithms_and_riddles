#include "islands.hpp"
#include <catch2/catch.hpp>

namespace
{
using namespace islands;

TEST_CASE("correctly calculated", "[islands counting]")
{
  DynamicMatrix<int> single{{1, 1, 1, 1}, 2, 2};
  REQUIRE( 1 == islands::get_number_of_islands(single));
}

}
