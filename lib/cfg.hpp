#pragma once


#include <filesystem>
#include <string>
#include <unordered_map>

class Config
{
public:
    Config(const std::filesystem::path &file);
    std::string Get(std::string key);
private:
    std::unordered_map<std::string, std::string> cfg;
};
