#include <cfg.hpp>
#include <exc.hpp>
#include <export.hpp>
#include <lazerdb.hpp>
#include <Parser.hpp>
#include <stabledb.hpp>

#include <exception>
#include <filesystem>
#include <iostream>
#include <unordered_set>

std::string mdLink(std::string text, std::string link)
{
    return "[" + text + "](" + link + ")";
}

std::string reprMarkdown(StableDB::BeatmapSet set)
{
    return mdLink(set.Artist + " - " + set.Title, "https://osu.ppy.sh/beatmapsets/" + std::to_string(set.BeatmapSetID));
}

std::string reprMarkdownUnicode(StableDB::BeatmapSet set)
{
    return mdLink(set.ArtistUnicode + " - " + set.TitleUnicode, "https://osu.ppy.sh/beatmapsets/" + std::to_string(set.BeatmapSetID));
}

int main(int argc, char **argv)
{
    try
    {
        Config cfg = Config("config.cfg");
        std::filesystem::path stableFolder = cfg.Get("stable-folder");
        StableDB sdb = StableDB(stableFolder / "osu!.db");
        auto sets = sdb.GetBeatmapSets();
        unsigned int counter = 0;
        unsigned int counter2 = 0;
        for (auto set : sets)
        {
            bool hasStoryboard = false;
            for (auto beatmap : set.second.Beatmaps)
                if (sb::IsStoryboarded(
                    stableFolder / "Songs" / set.second.FolderName,
                    beatmap.DifficultyFile.substr(0, beatmap.DifficultyFile.rfind("[") - 1) + ".osb",
                    beatmap.DifficultyFile
                ))
                {
                    hasStoryboard = true;
                    counter++;
                    break;
                }
            if (hasStoryboard)
                std::cout << reprMarkdown(set.second) << std::endl << std::endl;
            counter2++;
        }
        std::cout << counter << " storyboards found out of " << counter2 << " beatmapsets" << std::endl;
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
