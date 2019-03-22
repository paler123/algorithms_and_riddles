#include "matcher.hpp"
#include <catch2/catch.hpp>

namespace
{
using namespace regexes;

TEST_CASE("Pattern matching works", "[Matcher]")
{
  REQUIRE(matches("", ""));
  REQUIRE(matches("a", "a"));
  REQUIRE(matches("ab", "a."));
  REQUIRE(matches("ab", ".."));
  REQUIRE(matches("", "a*"));
  REQUIRE(matches("aaaaa", ".*"));
  REQUIRE(matches("aaabbaaabaaaaaaa", "aa*aab*aaab.a*"));
  REQUIRE(matches("ba", "[ab][ab]"));
  REQUIRE(matches("b", "[ab][c]*"));

  REQUIRE(!matches("", "a*a"));
  REQUIRE(!matches("ab", "aba"));
  REQUIRE(!matches("ab", "abaa"));
  REQUIRE(!matches("aaa", "...."));
  REQUIRE(!matches("a", "b"));
  REQUIRE(!matches("a", "ab"));
  REQUIRE(!matches("ab", "a"));
}

}
