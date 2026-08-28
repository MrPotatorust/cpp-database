#include <vector>
#include <memory>
#include <variant>
#include <string>
#include <cstdint>
#include <format>

#pragma once

enum class ColType : std::uint8_t
{
    UInt,
    Int,
    Float,
    Double,
    Bool,
    Varchar,
};

// Constraints, attributes
enum class ColAttribute : std::uint8_t
{
    Unique,
    Auto_increment,
    Not_null,
    Primary_key
};

struct ColumnRecord
{
    std::string name;
    ColType type;
    std::vector<ColAttribute> attrs;
};

enum class DBFunction
{
    Create,
    Insert,
    Select
};

using ColValue = std::variant<
    std::monostate,
    unsigned int,
    int,
    float,
    double,
    bool,
    std::string>;

using Row = std::vector<ColValue>;

class Column
{
public:
    std::string name;
    ColType type;
    std::vector<ColAttribute> attrs;

    Column(const ColumnRecord &record);

    bool hasDefaultValue() const;
    ColValue getDefaultValue() const;

    void dumpInfo() const;

private:
};

struct TableRecord
{
    std::string name;
    std::vector<ColumnRecord> columns;
};

using TableRecordPtr = std::unique_ptr<TableRecord>;

class Table
{
public:
    std::string name;
    std::vector<Column> columns;
    std::vector<Row> rows;
    std::size_t lastRowId;

    Table();
    Table(std::string name, std::vector<Column> cols);
    Table(std::string name, std::vector<ColumnRecord> cols);
    std::string getName();
    std::vector<Column> getColumns();
    void printRows(const std::size_t &limit = 0);
    void addColumn(const ColumnRecord &record);
    void writeRow(const Row &rowToAdd);
    void writeRows(const std::vector<Row> &rowsToAdd);
    void dumpInfo() const;
private:
    std::size_t incrementRowId();
};
