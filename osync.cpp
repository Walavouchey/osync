#include <cfg.hpp>
#include <exc.hpp>
#include <export.hpp>
#include <lazerdb.hpp>
#include <stabledb.hpp>

#include <exception>
#include <filesystem>
#include <iostream>
#include <unordered_set>

int main(int argc, char **argv)
{
    try
    {
        Config cfg = Config("config.cfg");
        std::filesystem::path lazerFolder = cfg.Get("lazer-folder");
        std::filesystem::path stableFolder = cfg.Get("stable-folder");
        std::filesystem::path exportFolder = cfg.Get("export-folder");
        Database db = Database(lazerFolder / "client.db");
        std::unique_ptr<Table> lazerSets = db.Select("select OnlineBeatmapSetID from BeatmapSetInfo");
        StableDB sdb = StableDB(stableFolder / "osu!.db");
        std::unordered_set<uint32_t> stableSets = sdb.GetBeatmapSetIDs();
        std::vector<uint32_t> setsToExport;
        for (std::vector<std::string> &row : lazerSets->rows)
        {
            if (row[0].empty()) continue;
            uint32_t ID = std::stoi(row[0]);
            if (stableSets.find(ID) == stableSets.end())
                setsToExport.push_back(ID);
        }
        std::cout << setsToExport.size() << " beatmapsets found on lazer that are not on stable" << std::endl;
        std::cout << "Exporting to " << exportFolder.string() << std::endl;
        for (uint32_t ID : setsToExport)
            exportBeatmap(db, ID, lazerFolder, exportFolder);
        std::cout << "Done" << std::endl;
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
