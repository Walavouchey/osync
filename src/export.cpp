#include <export.hpp>

#include <beatmapset.hpp>

#include <memory>

void exportBeatmap(Database &db, int OnlineID, const std::filesystem::path &lazerFolder, const std::filesystem::path &exportFolder)
{
    std::unique_ptr<Table> BeatmapSetInfo = db.Select("select ID, MetadataID from BeatmapSetInfo where OnlineBeatmapSetID = " + std::to_string(OnlineID));
    int ID = std::stoi(BeatmapSetInfo->rows[0][0]);
    int MetadataID = std::stoi(BeatmapSetInfo->rows[0][1]);
    std::unique_ptr<Table> BeatmapSetFileInfo = db.Select("select FileInfoID, Filename from BeatmapSetFileInfo where BeatmapSetInfoID = " + std::to_string(ID));
    std::unique_ptr<Table> BeatmapMetadata = db.Select("select Artist, Title from BeatmapMetadata where ID = " + std::to_string(MetadataID));
    std::string artist = BeatmapMetadata->rows[0][0];
    std::string title = BeatmapMetadata->rows[0][1];
    std::string foldername = std::to_string(OnlineID) + " " + artist + " - " + title;
    Beatmapset bm = Beatmapset(foldername);
    for (std::vector<std::string> row : BeatmapSetFileInfo->rows)
    {
        std::string filename = row[1];
        std::unique_ptr<Table> FileInfo = db.Select("select Hash from FileInfo where ID = " + row[0]);
        std::string hash = FileInfo->rows[0][0];
        bm.AddFile(filename, hash);
    }
    bm.Export(lazerFolder, exportFolder);
}
