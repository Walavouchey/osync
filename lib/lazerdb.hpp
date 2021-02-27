#pragma once

#include <sqlite3.h>

#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct Table
{
    std::vector<std::string> head;
    std::vector<std::vector<std::string>> rows;
    void Print();
};

class Database
{
public:
    Database(const std::filesystem::path &file);
    std::unique_ptr<Table> Select(const std::string &cmd);
    void Close();
    ~Database();
private:
    static int ReceiveData(void *data, int cols, char **row, char **head);
    void HandleError();
private:
    sqlite3* db;
};
