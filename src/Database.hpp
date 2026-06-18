#pragma once

#include "Engine.hpp"
#include <string>
#include <vector>

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

class Database
{
public:
    std::string name;
    Database();
    bool query(std::string query);
    Metadata &getMetadata();

private:
    std::unique_ptr<Metadata> metaData;
    std::unique_ptr<Engine> engine;
};