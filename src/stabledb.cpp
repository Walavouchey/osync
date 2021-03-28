#include <stabledb.hpp>

#include <exc.hpp>

#include <iostream>

uint64_t StableDB::ReadULEB128(std::ifstream &stream)
{
    uint64_t result = 0;
    uint32_t shift = 0;
    while (true)
    {
        uint8_t byte = stream.get();
        result |= (byte & 0x7F) << shift;
        if ((byte & 0x80) >> 7 == 0) break;
        shift += 7;
    }
    return result;
}

std::string StableDB::ReadString(std::ifstream &stream)
{
    std::string result = "";
    if (stream.get() != 0x0B) return result;
    uint64_t length = ReadULEB128(stream);
    for (int i = 0; i < length; i++)
        result += (char) stream.get();
    return result;
}

void StableDB::Populate(std::vector<IntDoublePair> &vector, std::ifstream &stream)
{
    #define READ(out) stream.read(reinterpret_cast<char*>(&(out)), sizeof(out))
    uint32_t len;
    READ(len);
    for (int i = 0; i < len; i++)
    {
        IntDoublePair data;
        READ(data.type1);
        READ(data.value1);
        READ(data.type2);
        READ(data.value2);
        vector.push_back(data);
    }
    #undef READ
}

void StableDB::Populate(std::vector<TimingPoint> &vector, std::ifstream &stream)
{
    #define READ(out) stream.read(reinterpret_cast<char*>(&(out)), sizeof(out))
    uint32_t len;
    READ(len);
    for (int i = 0; i < len; i++)
    {
        TimingPoint data;
        READ(data.BPM);
        READ(data.Offset);
        READ(data.Uninherited);
        vector.push_back(data);
    }
    #undef READ
}

StableDB::Beatmap::Beatmap(std::ifstream &stream)
{
    #define READ(out) stream.read(reinterpret_cast<char*>(&(out)), sizeof(out))
    #define READ_STRING(out) out = ReadString(stream)
    // READ(Size);
    READ_STRING(Artist);
    READ_STRING(ArtistUnicode);
    READ_STRING(Title);
    READ_STRING(TitleUnicode);
    READ_STRING(Creator);
    READ_STRING(Difficulty);
    READ_STRING(AudioFileName);
    READ_STRING(Hash);
    READ_STRING(DifficultyFile);
    READ(RankedStatus);
    READ(HitcircleCount);
    READ(SliderCount);
    READ(SpinnerCount);
    READ(LastModificationTime);
    READ(AR);
    READ(CS);
    READ(HP);
    READ(OD);
    READ(SV);
    Populate(SRSTD, stream);
    Populate(SRTAIKO, stream);
    Populate(SRCATCH, stream);
    Populate(SRMANIA, stream);
    READ(DrainTime);
    READ(Duration);
    READ(PreviewTime);
    Populate(TimingPoints, stream);
    READ(BeatmapID);
    READ(BeatmapSetID);
    READ(ThreadID);
    READ(GradeSTD);
    READ(GradeTAIKO);
    READ(GradeCATCH);
    READ(GradeMANIA);
    READ(LocalOffset);
    READ(StackLeniency);
    READ(GameMode);
    READ_STRING(Source);
    READ_STRING(Tags);
    READ(OnlineOffset);
    READ_STRING(TitleFont);
    READ(Unplayed);
    READ(LastPlayedTime);
    READ(OSZ2);
    READ_STRING(FolderName);
    READ(LastUpdatedTime);
    READ(IgnoreHitSound);
    READ(IgnoreSkin);
    READ(DisableStoryboard);
    READ(DisableVideo);
    READ(VisualOverride);
    //READ(Unknown);
    READ(LastModificationTime2);
    READ(SSMANIA);
    #undef READ
    #undef READ_STRING
}

StableDB::StableDB(const std::filesystem::path &stableDBFile)
{
    db = std::make_unique<Database>();
    std::ifstream file(stableDBFile, std::ios::binary);
    if (!file.is_open()) throw E("Unable to open stable database file: " + stableDBFile.string());
    #define READ(out) file.read(reinterpret_cast<char*>(&(out)), sizeof(out))
    #define READ_STRING(out) out = ReadString(file)
    READ(db->Version);
    READ(db->FolderCount);
    READ(db->AccountUnlocked);
    READ(db->TimeUntilUnlock);
    READ_STRING(db->PlayerName);
    READ(db->BeatmapCount);
    for (int i = 0; i < db->BeatmapCount; i++)
        db->Beatmaps.emplace_back(Beatmap(file));
    READ(db->PermissionFlags);
    #undef READ
    #undef READ_STRING
}

std::unordered_set<uint32_t> StableDB::GetBeatmapSetIDs() const
{
    std::unordered_set<uint32_t> map;
    for (const Beatmap &bm : db->Beatmaps)
        map.insert(bm.BeatmapSetID);
    return map;
}

std::unordered_map<uint32_t, StableDB::BeatmapSet> StableDB::GetBeatmapSets() const
{
    std::unordered_map<uint32_t, BeatmapSet> map;
    for (const Beatmap &bm : db->Beatmaps)
    {
        auto insertion = map.insert({ bm.BeatmapSetID, {
            bm.BeatmapSetID, bm.Creator, bm.FolderName,
            bm.Artist, bm.ArtistUnicode, bm.Title, bm.TitleUnicode,
            {} } });
        insertion.first->second.Beatmaps.push_back(bm);
    }
    return map;
}
