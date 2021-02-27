#pragma once

#include <vector>
#include <string>
#include <filesystem>

class Beatmapset
{
public:
    Beatmapset(const std::string &foldername);
    void AddFile(const std::filesystem::path &filename, const std::string &hash);
    void Export(const std::filesystem::path &lazerPath, const std::filesystem::path &location);
    //void ExportArchive(const std::filesystem::path &lazerPath, const std::filesystem::path &location);
private:
    void SanitizePath(std::string &s);
    struct file
    {
        file(const std::filesystem::path &filename, const std::string &hash)
            :
            filename(filename),
            hash(hash)
        {}
        std::filesystem::path filename;
        std::string hash;
    };
    std::vector<file> files;
    std::string foldername;
};
