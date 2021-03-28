#pragma once

#include <Helpers.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <memory>
#include <exception>

namespace sb
{
    // simplified parser that only checks whether the file contains occurences of "Sprite", "Animation" and "Sample"
    bool hasStoryboardElements(std::ifstream& file)
    {
        std::unordered_map<std::string, std::string> variables;
        // check for utf-8 bom, which is present when exported through storybrew
        char buf[4];
        for (int i = 0; i < 3; i++)
            buf[i] = (file.seekg(i), file.peek());
        buf[3] = '\0';
        if (std::string(buf) == "\xEF\xBB\xBF") file.seekg(3);
        else file.seekg(0);

        std::string line;
        enum class Section
        {
            None,
            Events,
            Variables
        };
        Section section = Section::None;

        while (file.good())
        {
            constexpr int lineMaxReadLength = 10000;
            char lineTemp[lineMaxReadLength];
            file.getline(lineTemp, lineMaxReadLength);
            line = std::string(lineTemp);
            
            if (line.length() == 0) continue;
            if (line.rfind("//", 0) == 0) continue;

            if (line.find("[Events]") == 0)
            {
                section = Section::Events;
                continue;
            }
            else if (line.find("[Variables]") == 0)
            {
                section = Section::Variables;
                continue;
            }
            else if (line[0] == '[')
            {
                section = Section::None;
                continue;
            }

            switch (section)
            {
            case Section::None:
                continue;

            case Section::Events:
            {
                applyVariables(line, variables);
                if (line.find("Sprite") == 0 || line.find("Animation") == 0) return true;
            }
            break;
            case Section::Variables:
            {
                std::size_t splitPos = line.find('=');
                if (splitPos == std::string::npos || splitPos == line.length() - 1) continue;
                std::string key = line.substr(0, splitPos);
                std::string value = line.substr(splitPos + 1, line.length() - splitPos - 1);
                variables.emplace(key, value);
            }
            break;
            }
        }
        return false;
    }

    bool IsStoryboarded(const std::filesystem::path& directory, const std::string& osb, const std::string& diff)
    {
        bool isStoryboarded = false;

        std::ifstream diffFile(std::filesystem::path(directory) / diff);
        if (diffFile.is_open())
            isStoryboarded = hasStoryboardElements(diffFile);
        diffFile.close();

        if (isStoryboarded) return true;

        std::ifstream osbFile(std::filesystem::path(directory) / osb);

        if (osbFile.is_open())
            isStoryboarded = hasStoryboardElements(osbFile);
        osbFile.close();

        return isStoryboarded;
    }
}
