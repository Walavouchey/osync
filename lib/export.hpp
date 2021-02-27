#pragma once

#include <lazerdb.hpp>

#include <filesystem>

void exportBeatmap(Database &db, int OnlineID, const std::filesystem::path &lazerFolder, const std::filesystem::path &exportFolder);
