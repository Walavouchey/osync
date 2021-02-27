#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_set>

class StableDB
{
public:
    StableDB(const std::filesystem::path &stableFolder);
    std::unordered_set<uint32_t> GetBeatmapSetIDs() const;
private:
    struct IntDoublePair { int8_t type1; int32_t value1; int8_t type2; double value2; };
    struct TimingPoint { double BPM; double Offset; bool Uninherited; };
    struct Beatmap
    {
        Beatmap(std::ifstream &stream);
        // uint32_t Size;
        std::string Artist;
        std::string ArtistUnicode;
        std::string Title;
        std::string TitleUnicode;
        std::string Creator;
        std::string Difficulty;
        std::string AudioFileName;
        std::string Hash;
        std::string DifficultyFile;
        uint8_t RankedStatus;
        uint16_t HitcircleCount;
        uint16_t SliderCount;
        uint16_t SpinnerCount;
        uint64_t LastModificationTime;
        float AR;
        float CS;
        float HP;
        float OD;
        double SV;
        std::vector<IntDoublePair> SRSTD;
        std::vector<IntDoublePair> SRTAIKO;
        std::vector<IntDoublePair> SRCATCH;
        std::vector<IntDoublePair> SRMANIA;
        uint32_t DrainTime;
        uint32_t Duration;
        uint32_t PreviewTime;
        std::vector<TimingPoint> TimingPoints;
        uint32_t BeatmapID;
        uint32_t BeatmapSetID;
        uint32_t ThreadID;
        uint8_t GradeSTD;
        uint8_t GradeTAIKO;
        uint8_t GradeCATCH;
        uint8_t GradeMANIA;
        uint16_t LocalOffset;
        float StackLeniency;
        uint8_t GameMode;
        std::string Source;
        std::string Tags;
        uint16_t OnlineOffset;
        std::string TitleFont;
        bool Unplayed;
        uint64_t LastPlayedTime;
        bool OSZ2;
        std::string FolderName;
        uint64_t LastUpdatedTime;
        bool IgnoreHitSound;
        bool IgnoreSkin;
        bool DisableStoryboard;
        bool DisableVideo;
        bool VisualOverride;
        //uint16_t Unknown;
        uint32_t LastModificationTime2;
        uint8_t SSMANIA;
    };
    static uint64_t ReadULEB128(std::ifstream &stream);
    static std::string ReadString(std::ifstream &stream);
    static void ReadBeatmap(std::ifstream &stream);
    static void Populate(std::vector<IntDoublePair> &vector, std::ifstream &stream);
    static void Populate(std::vector<TimingPoint> &vector, std::ifstream &stream);
private:
    struct Database
    {
        uint32_t Version;
        uint32_t FolderCount;
        bool AccountUnlocked;
        int64_t TimeUntilUnlock;
        std::string PlayerName;
        uint32_t BeatmapCount;
        std::vector<Beatmap> Beatmaps;
        uint32_t PermissionFlags;
    };
    std::unique_ptr<Database> db;
};
