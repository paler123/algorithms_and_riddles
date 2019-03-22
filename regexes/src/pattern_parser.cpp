#include "pattern_parser.hpp"
#include "to_intermediate.hpp"

#include <set>

namespace regexes
{

struct Matcher
{
  virtual ~Matcher() = default;
  virtual bool accepts(char c) const = 0;
};

struct SatisfiedPolicy
{
  virtual ~SatisfiedPolicy() = default;
  virtual bool satisfied(std::size_t times_matched) const = 0;
};

struct Token::Impl {
  Impl(std::unique_ptr<Matcher>&& matcher,
       std::unique_ptr<SatisfiedPolicy>&& matched_cryterium,
       std::unique_ptr<SatisfiedPolicy>&& exhausted_cryterium)
  : matcher{std::move(matcher)}
  , matched_cryterium{std::move(matched_cryterium)}
  , exhausted_cryterium{std::move(exhausted_cryterium)}
  {}
  bool accepts(char c) const
  {
    return matcher->accepts(c);
  }
  bool matched(std::size_t times_matched) const
  {
    return matched_cryterium->satisfied(times_matched);
  }
  bool exhausted(std::size_t times_matched) const
  {
    return exhausted_cryterium->satisfied(times_matched);
  }
private:
  std::unique_ptr<Matcher> matcher;
  std::unique_ptr<SatisfiedPolicy> matched_cryterium;
  std::unique_ptr<SatisfiedPolicy> exhausted_cryterium;
};

Token::~Token() = default;

Token::Token(std::unique_ptr<Matcher>&& matcher,
             std::unique_ptr<SatisfiedPolicy>&& matched_cryterium,
             std::unique_ptr<SatisfiedPolicy>&& exhausted_cryterium)
: pImpl{std::make_unique<Impl>(std::move(matcher),
                               std::move(matched_cryterium),
                               std::move(exhausted_cryterium))}
{}


bool Token::accepts(char c) const
{
  return pImpl->accepts(c);
}

bool Token::matched(std::size_t times_matched) const
{
  return pImpl->matched(times_matched);
}

bool Token::exhausted(std::size_t times_matched) const
{
  return pImpl->exhausted(times_matched);
}

struct AlwaysSatisfied : SatisfiedPolicy
{
  bool satisfied(std::size_t) const { return true; }
};

struct NeverSatisfied : SatisfiedPolicy
{
  bool satisfied(std::size_t) const { return false; }
};

struct SatisfiedAfterMatch : SatisfiedPolicy
{
  bool satisfied(std::size_t times_matched) const { return times_matched > 0; }
};

struct CharMatcher : Matcher
{
public:
  CharMatcher(char value) : value(value) {}
  bool accepts(char c) const override { return c == value; }
private:
  char value;
};

struct Charset : Matcher
{
  Charset(std::set<char>&& accepted_chars)
  : accepted_chars(std::make_move_iterator(accepted_chars.begin()),
                   std::make_move_iterator(accepted_chars.end()))
  {}
  bool accepts(char c) const override
  {
    return c == *std::lower_bound(accepted_chars.cbegin(), accepted_chars.cend(), c);
  }
private:
  std::vector<char> accepted_chars;
};

struct Wildcard : Matcher
{
  bool accepts(char) const override { return true; }
};

Token to_object (TokenType type, Modifier mod,
                 std::string_view::const_iterator pattern_begin)
{
  std::unique_ptr<Matcher> matcher;
  switch (type)
  {
  case TokenType::Char:
  {
    matcher = std::make_unique<CharMatcher>(*pattern_begin);
    break;
  }
  case TokenType::Wildcard:
  {
    matcher = std::make_unique<Wildcard>();
    break;
  }
  case TokenType::Charset:
  {
    std::set<char> accepted_chars;
    for (auto it = ++pattern_begin; *it != ']'; ++it)
      accepted_chars.emplace(*it);
    matcher = std::make_unique<Charset>(std::move(accepted_chars));
    break;
  }
  }
  std::unique_ptr<SatisfiedPolicy> matched_cryterium;
  std::unique_ptr<SatisfiedPolicy> exhausted_cryterium;
  switch (mod)
  {
  case Modifier::None:
  {
    matched_cryterium = std::make_unique<SatisfiedAfterMatch>();
    exhausted_cryterium = std::make_unique<SatisfiedAfterMatch>();
    break;
  }
  case Modifier::Star:
  {
    matched_cryterium = std::make_unique<AlwaysSatisfied>();
    exhausted_cryterium = std::make_unique<NeverSatisfied>();
    break;
  }
  }
  return {std::move(matcher), std::move(matched_cryterium), std::move(exhausted_cryterium)};
}

Pattern tokenize (std::string_view pattern)
{
  Pattern result;
  auto pattern_it = pattern.cbegin();
  while (pattern_it != pattern.cend())
  {
    auto next_token = get_next_token(pattern_it, pattern.cend());
    result.emplace_back(to_object(std::get<0>(next_token),
                        std::get<1>(next_token), pattern_it));
    std::advance(pattern_it, std::get<2>(next_token));
  }
  return result;
}

}
