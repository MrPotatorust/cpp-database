#include <string>
#include <vector>
#include <memory>

#pragma once

struct EngineResponse
{

    bool success;
    std::string message;
};

enum class ColType : std::uint8_t
{
    UInt,
    Int,
    Float,
    Double,
    Bool,
    Varchar,
};

struct Column
{
    ColType type;
    std::string_view name;
};

// using ColPtr = std::unique_ptr<Col>;

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

    std::unique_ptr<Metadata> dbMetaData;

    void createTable(std::string_view name, std::vector<Column> cols);

private:
};