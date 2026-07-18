#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "Parser.hpp"

class Database;

#pragma once

class Engine
{
public:
    Engine(Database &db);

    std::string query(std::string query);

    void insert(std::string_view name, std::vector<std::string> insertCols, std::vector<Row> rows);

    void update(std::string name);

    void create(std::string name, std::vector<ColumnRecord> cols);

    void select(std::string name);

private:
    Database &db;

    std::string execute(const Statement &statement);

    bool tableExists(std::string_view name);

    Table &getTable(std::string_view name);
};
