#include "DBTypes.hpp"
#include "Helpers.hpp"
#include <iostream>

Column::Column(const ColumnRecord &record) : name(record.name), type(record.type), attrs(record.attrs) {};

//! This will throw an error in the future if it does not have a value
ColValue Column::getDefaultValue() const
{
    return std::monostate{};
}

bool Column::hasDefaultValue() const
{
    return true;
}

void Column::dumpInfo() const
{
    auto formatedName = this->name.size() > 12 ? this->name.substr(0, 9) + "..." : this->name;

    std::cout << std::format("| {:<12} | {:<13}", formatedName, colTypeToString(this->type));

    for (auto attr : this->attrs)
    {
        std::cout << std::format(" | {:<13}", colAttributeToString(attr));
    }

    std::cout << " | \n";
}

Table::Table()
{
}

Table::Table(std::string name, std::vector<Column> cols) : name(name), columns(cols)
{
}

Table::Table(std::string name, std::vector<ColumnRecord> cols) : name(name)
{
    for (auto col : cols)
    {
        this->columns.push_back(Column(col));
    }
}

void Table::addColumn(const ColumnRecord &record)
{
    this->columns.push_back(record);
};

std::string Table::getName()
{
    return this->name;
}

std::vector<Column> Table::getColumns()
{
    return this->columns;
};

void Table::writeRow(const Row &rowToAdd)
{
    this->rows.push_back(rowToAdd);
}

void Table::printRows(const std::size_t &limit)
{

    auto lim = limit == 0 ? this->rows.size() : limit;

    for (std::size_t i = 0; i < lim; i++)
    {
        auto &row = this->rows.at(i);

        for (const auto &value : row)
        {
            std::cout << colValueToString(value) << ' ';
        }

        std::cout << '\n';
    }
}

void Table::writeRows(const std::vector<Row> &rowsToAdd)
{
    (void)rowsToAdd;
}

void Table::dumpInfo() const
{
    std::cout << "Table: " << this->name << '\n';

    for (auto col : this->columns)
    {
        col.dumpInfo();
    }
}