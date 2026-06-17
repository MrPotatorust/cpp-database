#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "Parser.hpp"

#pragma once

struct Table
{
    std::string_view name;
    std::vector<Column> columns;
};

using TablePtr = std::unique_ptr<Table>;

/*
MetaData Storage layout:

----------------------
Pointers to individual tables (byte offsets inside the file)
----------------------
Tables
**********
Table-Table-Table (Just layed out next to each other)
----------------------
Table
**********
table_name_len (uint8_t)
table_name (char *, max 255)
Pointers to individual cols (byte offsets inside the file)
col_type (uint8_t)
col_name_len (uint8_t)
col_name (char *, max 255)



*/

class Metadata
{
public:
    std::vector<TablePtr> tables;
    Metadata();
    void persist();

private:
    void loadFile();
    void persistToFile();
};

class Engine
{
public:
    Engine();

    void query(std::string query);

    std::unique_ptr<Metadata> dbMetaData;

    void insert(std::string_view name);

    void update(std::string_view name);

    void create(std::string_view name, std::vector<Column> cols);

    void select(std::string_view name);

private:
    void execute(const Statement &statement);

    bool tableExists(std::string_view name);
};