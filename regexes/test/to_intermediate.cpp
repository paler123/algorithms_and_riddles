#include "to_intermediate.hpp"
#include <catch2/catch.hpp>
#include <string>

namespace
{

using namespace regexes;

TEST_CASE("Token extraction works", "[Intermediate representation]")
{
  std::string pattern {"aba.cg*.*[abc-f][ab]*"};
  std::string_view pattern_view = pattern;
  SECTION("Char extraction")
  {
    REQUIRE( TokenDescription{TokenType::Char, Modifier::None, 1} ==
        get_next_token(pattern_view.begin(), pattern_view.end()));
  }
  SECTION("Wildcard extraction")
  {
    REQUIRE( TokenDescription{TokenType::Wildcard, Modifier::None, 1} ==
        get_next_token(pattern_view.begin() + 3, pattern_view.end()));
  }
  SECTION("Char with star extraction")
  {
    REQUIRE( TokenDescription{TokenType::Char, Modifier::Star, 2} ==
        get_next_token(pattern_view.begin() + 5, pattern_view.end()));
  }
  SECTION("Wildcard with star extraction")
  {
    REQUIRE( TokenDescription{TokenType::Wildcard, Modifier::Star, 2} ==
        get_next_token(pattern_view.begin() + 7, pattern_view.end()));
  }
  SECTION("Charset extraction")
  {
    REQUIRE( TokenDescription{TokenType::Charset, Modifier::None, 7} ==
        get_next_token(pattern_view.begin() + 9, pattern_view.end()));
  }
  SECTION("Charset with star extraction")
  {
    REQUIRE( TokenDescription{TokenType::Charset, Modifier::Star, 5} ==
        get_next_token(pattern_view.begin() + 16, pattern_view.end()));
  }
}

}
