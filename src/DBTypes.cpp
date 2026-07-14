#include "DBTypes.hpp"

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

void Table::writeRows(const std::vector<Row> &rowsToAdd)
{
    this->rows.insert(this->rows.end(), rows.begin(), rows.end());
}