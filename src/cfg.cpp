#include <cfg.hpp>

#include <exc.hpp>

#include <fstream>
#include <iostream>
#include <vector>

Config::Config(const std::filesystem::path &file)
{
    std::ifstream cfgFile(file);
    if (!cfgFile.is_open()) throw E("Failed to open config file");
    while (cfgFile.good())
    {
        constexpr int lineMaxReadLength = 1000;
        char lineTemp[lineMaxReadLength];
        cfgFile.getline(lineTemp, lineMaxReadLength);
        std::string line = std::string(lineTemp);
        std::vector<std::string> split = strext::stringSplit(line, ":", 1);
        strext::stringTrim(split[0]);
        if (!split[0].empty()) cfg.emplace(split[0], strext::stringTrim(split[1]));
    }
}

std::string Config::Get(std::string key)
{
    auto k = cfg.find(key);
    if (k == cfg.end()) throw E(("Could not find config: " + key).c_str());
    return k->second;
}
