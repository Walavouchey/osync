//#include <ZipFile.h>

#include <beatmapset.hpp>

#include <exc.hpp>
#include <progressbar.hpp>

#include <fstream>
#include <iostream>

Beatmapset::Beatmapset(const std::string &foldername)
    :
    foldername(foldername)
{
    SanitizePath(this->foldername);
}

void Beatmapset::AddFile(const std::filesystem::path &filename, const std::string &hash)
{
    files.emplace_back(filename, hash);
}

void Beatmapset::Export(const std::filesystem::path &lazerPath, const std::filesystem::path &location)
{
    if (!std::filesystem::exists(location))
        throw E("The export path " + location.string() + " doesn't exist");
    ProgressBar progress = ProgressBar(foldername + ": ", files.size(), 0, -1, false);
    for (file f : files)
    {
        progress.update();
        if (!f.filename.has_filename()) continue; // could be a directory, which might be a bug in lazer
        std::filesystem::path sourcePath = lazerPath
            / "files"
            / std::filesystem::path(f.hash.substr(0, 1))
            / std::filesystem::path(f.hash.substr(0, 2))
            / f.hash;
        std::filesystem::create_directories(location / foldername / f.filename.parent_path());
        std::filesystem::copy(sourcePath, location / foldername / f.filename);
    }
    progress.finish();
}

/*
void Beatmapset::ExportArchive(const std::filesystem::path &lazerPath, const std::filesystem::path &location)
{
    std::string osz = (location / foldername).string() + ".osz";
    ZipArchive::Ptr archive = ZipFile::Open(osz);
    std::vector<std::ifstream> contentStreams;
    ProgressBar progress = ProgressBar(foldername + ": ", files.size());
    for (file f : files)
    {
        progress.print();
        std::cout << f.filename.string() << '\t' << f.hash << std::endl;
        ZipArchiveEntry::Ptr entry = archive->CreateEntry(f.filename.string());
        std::filesystem::path sourcePath = lazerPath
            / "files"
            / std::filesystem::path(f.hash.substr(0, 1))
            / std::filesystem::path(f.hash.substr(0, 2))
            / f.hash;
        contentStreams.emplace_back(sourcePath.string(), std::ios::binary);
        entry->SetCompressionStream(contentStreams.back());
    }
    progress.finish();
    ZipFile::SaveAndClose(archive, osz);
}
*/

void Beatmapset::SanitizePath(std::string &s)
{
    constexpr char illegalChars[] = { '/', '\\', ':', '*', '?', '"', '<' , '>', '|' };
    for (char &c : s)
        for (const char &ic : illegalChars) 
            if (c == ic) { c = '_'; break; }
}
