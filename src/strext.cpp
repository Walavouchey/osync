#include <strext.hpp>

namespace strext
{
    std::string& stringTrim(std::string &s)
    {
        while (std::isspace(s[0])) s.erase(0, 1);
        while (std::isspace(*(s.end() - 1))) s.erase(s.end() - 1);
        return s;
    }
    std::vector<std::string> stringSplit(std::string s, const std::string& delimiter, int limit = 0)
    {
        std::size_t pos = 0;
        std::vector<std::string> split;
        for (int i = 0; (pos = s.find(delimiter)) != std::string::npos; s.erase(0, pos + delimiter.length()), i++)
            split.emplace_back(s.substr(0, limit > 0 && i >= limit ? pos = s.length() : pos));
        split.emplace_back(s);
        return split;
    } 
}
