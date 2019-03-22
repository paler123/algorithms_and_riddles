#pragma once
#include <string_view>
#include <vector>
#include <memory>

namespace regexes
{

struct Token;

bool matches (std::string_view string, std::string_view pattern);

bool matches (std::string_view string, std::vector<Token> const & pattern);

}
