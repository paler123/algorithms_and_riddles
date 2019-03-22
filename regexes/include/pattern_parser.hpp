#include <vector>
#include <memory>

namespace regexes
{

struct Matcher;
struct SatisfiedPolicy;

struct Token
{
  ~Token();
  Token(Token&&) noexcept = default;
  Token& operator=(Token&&) noexcept = default;
  Token(Token const &) = delete;
  Token& operator=(Token const&) = delete;
  Token(std::unique_ptr<Matcher>&& matcher,
        std::unique_ptr<SatisfiedPolicy>&& matched_cryterium,
        std::unique_ptr<SatisfiedPolicy>&& exhausted_cryterium);

  bool accepts(char c) const;
  bool matched(std::size_t times_matched) const;
  bool exhausted(std::size_t times_matched) const;
private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
};

using Pattern = std::vector<Token>;
Pattern tokenize (std::string_view pattern);

}
