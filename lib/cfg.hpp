#pragma once

#include <strext.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

class Config
{
public:
    Config(const std::filesystem::path &file);
    std::string Get(std::string key);
private:
    std::unordered_map<std::string, std::string> cfg;
};
