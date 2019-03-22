#include "to_intermediate.hpp"

#include <algorithm>

namespace regexes
{

using TokenDescription = std::tuple<TokenType, Modifier, std::size_t>;
TokenDescription get_next_token(
    std::string_view::const_iterator token_begin,
    std::string_view::const_iterator pattern_end)
{
  TokenType tt;
  Modifier mod = Modifier::None;
  std::size_t length = 0;
  switch (*token_begin)
  {
  case '.':
  {
    tt = TokenType::Wildcard;
    ++length;
    break;
  }
  case '[':
  {
    tt = TokenType::Charset;
    auto token_end = std::find(token_begin, pattern_end, ']');
    length = static_cast<std::size_t>(std::distance(token_begin, token_end)) + 1;
    break;
  }
  default:
  {
    tt = TokenType::Char;
    ++length;
    break;
  }
  }

  std::advance(token_begin, length);
  if (token_begin != pattern_end && *token_begin == '*')
  {
    ++length;
    mod = Modifier::Star;
  }

  return {tt, mod, length};
}

}
