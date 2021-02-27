#pragma once

#include <string>
#include <vector>

namespace strext
{
    std::string& stringTrim(std::string &s);
    std::vector<std::string> stringSplit(std::string s, const std::string& delimiter, int limit);
}
