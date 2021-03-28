#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <exception>

namespace sb
{
	void stringReplace(std::string& s, const std::string& search, const std::string& replace)
	{
		std::size_t pos = 0;
		while ((pos = s.find(search, pos)) != std::string::npos) {
			s.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}

	std::vector<std::string> stringSplit(std::string s, const std::string& delimiter)
	{
		std::size_t pos = 0;
		std::vector<std::string> split;
		for (; (pos = s.find(delimiter)) != std::string::npos; s.erase(0, pos + delimiter.length()))
			split.emplace_back(s.substr(0, pos));
		split.emplace_back(s);
		return split;
	}

	void applyVariables(std::string& line, const std::unordered_map<std::string, std::string>& variables)
	{
		for (const std::pair<std::string, std::string>& e : variables)
			stringReplace(line, e.first, e.second);
	}

	std::string removePathQuotes(const std::string& s)
	{
		return s[0] == '"' && s[s.length() - 1] == '"' ? s.substr(1, s.length() - 2) : s;
	}
}
