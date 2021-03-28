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

std::string mode2str(uint8_t mode)
{
    switch (mode)
    {
    case 0: return "osu";
    case 1: return "taiko";
    case 2: return "fruits";
    case 3: return "mania";
    default: return "osu";
    }
}

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

std::string reprMarkdown(StableDB::Beatmap bm)
{
    return mdLink(bm.Artist + " - " + bm.Title + " [" + bm.Difficulty + "]", "https://osu.ppy.sh/beatmapsets/" + std::to_string(bm.BeatmapSetID) + "#" + mode2str(bm.GameMode) + "/" + std::to_string(bm.BeatmapID));
}

std::string reprMarkdownUnicode(StableDB::Beatmap bm)
{
    return mdLink(bm.ArtistUnicode + " - " + bm.TitleUnicode + " [" + bm.Difficulty + "]", "https://osu.ppy.sh/beatmapsets/" + std::to_string(bm.BeatmapSetID) + "#" + mode2str(bm.GameMode) + "/" + std::to_string(bm.BeatmapID));
}

int main(int argc, char **argv)
{
    try
    {
        Config cfg = Config("config.cfg");
        std::filesystem::path stableFolder = cfg.Get("stable-folder");
        StableDB sdb = StableDB(stableFolder / "osu!.db", stableFolder / "collection.db");
        auto beatmaps = sdb.GetBeatmapsByHash();
        auto collections = sdb.GetCollections();
        std::unordered_map<std::string, std::vector<StableDB::Beatmap>> populatedCollections;
        for (auto collection : collections)
            for (auto hash : collection.Hashes)
            {
                auto k = beatmaps.find(hash);
                if (k != beatmaps.end()) populatedCollections[collection.Name].push_back(k->second);
            }
        for(auto collection : populatedCollections)
        {
            std::cout << "- " << collection.first << std::endl;
            for (auto beatmap : collection.second)
                std::cout << " - " << reprMarkdown(beatmap) << std::endl;
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
