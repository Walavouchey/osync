#include <lazerdb.hpp>

#include <exc.hpp>

#include <iostream>

#define EXC(expr) if (expr != SQLITE_OK) HandleError();

void Table::Print()
{
    for (std::string text : head)
        std::cout << text << "\t";
    std::cout << std::endl;
    for (auto rowIt = rows.begin(); rowIt < rows.end(); rowIt++, std::cout << std::endl)
        for (std::string text : *rowIt)
            std::cout << text << "\t";
}

Database::Database(const std::filesystem::path &file)
{
    EXC(sqlite3_open_v2(file.string().c_str(), &db, SQLITE_OPEN_READONLY, nullptr));
}

std::unique_ptr<Table> Database::Select(const std::string &cmd)
{
    std::unique_ptr<Table> data = std::make_unique<Table>();
    EXC(sqlite3_exec(db, cmd.c_str(), ReceiveData, (void*) data.get(), nullptr));
    return data;
}

void Database::Close()
{
    sqlite3_close_v2(db);
}

Database::~Database()
{
    Close();
}

int Database::ReceiveData(void *data, int cols, char **row, char **head)
{
    
    try
    {
        if (((Table*) data)->head.empty())
            for (int i = 0; i < cols; i++)
                ((Table*) data)->head.emplace_back(head[i] == nullptr ? "" : head[i]);
        ((Table*) data)->rows.push_back(std::vector<std::string>());
        for (int i = 0; i < cols; i++)
            ((Table*) data)->rows.back().emplace_back(row[i] == nullptr ? "" : row[i]);
    }
    catch (...)
    {
        return 1;
    }
    return 0;
}

void Database::HandleError()
{
    std::string s = sqlite3_errmsg(db);
    Close();
    throw E(s);
}

