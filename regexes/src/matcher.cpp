#include "matcher.hpp"
#include "pattern_parser.hpp"
#include <queue>

namespace regexes
{

bool matches (std::string_view string, std::string_view pattern)
{
  return matches(string, tokenize(pattern));
}

struct MatchEnd
{
  std::string_view::const_iterator string_pos;
  Pattern::const_iterator pattern_pos;
  std::size_t times_matched;
};

bool matches (std::string_view string, Pattern const & pattern)
{
  std::queue<MatchEnd> match_branches;
  match_branches.push({string.cbegin(), pattern.cbegin(), 0u});
  while (!match_branches.empty())
  {
    MatchEnd branch = match_branches.front();
    auto [string_pos, pattern_pos, times_matched] = branch;
    match_branches.pop();
    if (string_pos == string.cend() && pattern_pos == pattern.cend())
    {
      return true;
    }
    if (pattern_pos == pattern.cend())
    { // kill branch if reached pattern end without reaching string end
      continue;
    }
    if (pattern_pos->matched(times_matched))
    {
      // if given pattern was matched sufficently many times, can branch
      // to matching next pattern at current position
      match_branches.push({string_pos, std::next(pattern_pos), 0u});
    }
    if (pattern_pos->exhausted(times_matched))
    { // if pattern was matched, then already in queue with next pattern
      continue;
    }
    if (string_pos != string.cend() && pattern_pos->accepts(*string_pos))
    {
      // pattern matches and was not exhausted, increase match count and go to next char
      match_branches.push({std::next(string_pos), pattern_pos, times_matched + 1});
    }
  }
  return false;
}

}
