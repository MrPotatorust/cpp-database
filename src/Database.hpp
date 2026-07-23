#pragma once

#include "DBTypes.hpp"
#include <string>
#include <vector>

struct QueryResult
{
    bool success;
    std::string message;
};

/*
Metadata Storage layout:

----------------------
Pointers to individual tables (byte offsets inside the file)
----------------------
Tables
**********
Table-Table-Table (Just layed out next to each other)
----------------------
Table
**********
cols_count (uint_8)
table_name_len (uint64_t)
table_name (char *, max as much as can fit in uint64_t)
col_name_len (uint64_t)
col_name (char *, max as much as can fit in uint64_t)
col_type (uint8_t)


*/

class Database
{
public:
    std::string name;
    std::vector<Table> tables;

    std::vector<Row> data;

    Database();
    QueryResult query(std::string query);
    void persist();
    std::vector<Table> &getTables();
    const std::vector<Table> &getTables() const;
    void addTable(const Table &table);

private:
    void loadMetada();
    void persistMetadata();
};
