#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "Parser.hpp"

class Database;

#pragma once

struct Table
{
    std::string name;
    std::vector<Column> columns;
};

using TablePtr = std::unique_ptr<Table>;

class Engine
{
public:
    Engine(Database &db);

    void query(std::string query);

    void insert(std::string name);

    void update(std::string name);

    void create(std::string name, std::vector<Column> cols);

    void select(std::string name);

private:
    Database &db;

    void execute(const Statement &statement);

    bool tableExists(std::string_view name);
};