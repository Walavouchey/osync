#include <cfg.hpp>
#include <exc.hpp>
#include <export.hpp>
#include <lazerdb.hpp>
#include <Parser.hpp>
#include <progressbar.hpp>
#include <stabledb.hpp>

#include <exception>
#include <filesystem>
#include <iostream>
#include <unordered_set>

void sanitizePath(std::string &s)
{
    constexpr char illegalChars[] = { '/', '\\', ':', '*', '?', '"', '<' , '>', '|' };
    for (char &c : s)
        for (const char &ic : illegalChars) 
            if (c == ic) { c = '_'; break; }
}

int main(int argc, char **argv)
{
    try
    {
        std::filesystem::path exportLocation;
        if (argc > 1) exportLocation = std::filesystem::path(argv[1]);
        else throw E("Must specify an export folder");
        Config cfg = Config("config.cfg");
        std::filesystem::path stableFolder = cfg.Get("stable-folder");
        StableDB sdb = StableDB(stableFolder / "osu!.db");
        auto sets = sdb.GetBeatmapSets();
        ProgressBar progress("Files copied: ", sets.size());
        for (auto set : sets)
        {
            progress.update();
            auto beatmap = set.second.Beatmaps[0];
            std::filesystem::path AudioFileName = beatmap.AudioFileName;
            std::filesystem::path FolderName = beatmap.FolderName;
            std::string newFile = beatmap.Artist + " - " + beatmap.Title + AudioFileName.extension().string();
            sanitizePath(newFile);
            std::filesystem::create_directories(exportLocation);
            try { std::filesystem::copy(stableFolder / "Songs" / FolderName / AudioFileName, exportLocation / newFile); }
            catch (...) { /* swallow */ }
        }
    }
    catch (exc e)
    {
        std::cerr << e.file() << ", line " << e.line() << ": " << e.what() << std::endl;
        return 1;
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "An unknown error has occured." << std::endl;
        return 1;
    }
    return 0;
}
