#pragma once
#include <string_view>
#include <tuple>

namespace regexes
{

enum class TokenType
{
  Char = 0,
  Charset = 1,
  Wildcard = 2
};

enum class Modifier
{
  None = 0,
  Star = 1
};

using TokenDescription = std::tuple<TokenType, Modifier, std::size_t>;

TokenDescription get_next_token(
    std::string_view::const_iterator token_begin,
    std::string_view::const_iterator pattern_end
);

}
