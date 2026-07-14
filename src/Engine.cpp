#include <utility>
#include <unordered_map>

#include "Engine.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Database.hpp"
#include "Helpers.hpp"

Engine::Engine(Database &db)
    : db(db) {
      };

void Engine::query(std::string query)
{
    try
    {
        auto lexer = Lexer(query);

        auto tokens = lexer.getTokens();

        auto parser = Parser(std::move(tokens));

        auto statement = parser.getStatement();

        this->execute(statement);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
};

// Dispatches based on the requests
void Engine::execute(const Statement &statement)
{
    switch (statement.function)
    {
    case DBFunction::Create:
        this->create(statement.tableName, statement.cols);
        break;

    default:
        break;
    }

    this->db.persist();
}

bool Engine::tableExists(std::string_view name)
{
    for (const auto &table : this->db.getTables())
    {
        if (table.name == name)
            return true;
    }

    return false;
}

Table &Engine::getTable(std::string_view name)
{
    for (std::size_t i = 0; i < this->db.tables.size(); ++i)
    {
        auto &table = this->db.tables[i];

        if (table.name == name)
            return table;
    }

    throw std::invalid_argument("A table with the name " + std::string(name) + " does not exist.");
}

void Engine::create(std::string name, std::vector<ColumnRecord> cols)
{

    if (name.length() == 0)
        throw std::invalid_argument("Argument name cannot be of length 0, you need to provide atleast one letter.");

    if (cols.size() == 0)
        throw std::invalid_argument("Argument cols cannot be of length 0, you need to provide atleast one valid column.");

    if (this->tableExists(name))
        throw std::invalid_argument("A table with the name " + name + " already exists.");

    auto table = Table(name, cols);

    this->db.addTable(table);

    std::cout << "Succesfully created a table \n";
};

void Engine::insert(std::string_view name, std::vector<std::string> insertCols, std::vector<Row> rows)
{
    auto colSize = insertCols.size();

    auto &table = getTable(name);
    auto cols = table.getColumns();

    std::vector<std::size_t> colMap;

    // Check if every column exists in the table
    for (auto col : insertCols)
    {
        auto found = false;
        for (std::size_t i = 0; i < cols.size(); i++)
        {
            const auto tableCol = cols.at(i);

            if (tableCol.name == col)
            {
                found = true;
                break;
            };
        }
        if (found == false)
            throw std::invalid_argument("The Column " + std::string(name) + " does not exist at the table " + table.name + ".");
    }

    /*
        ? This assigns the offsets of the data to the columns
        Example input
        cols = id, user_id, age, first_name, last_name
        insertCols = first_name, id, last_name
        the resulting map vector would be 1, -1, -1, 0, 2
        -1 is for not present in insertCols vector and the numbers are offsets
        Explanation
        id maps to the second place in the insertCols vector -> 1
        user_id is not present in the insertCols vector -> -1
        age is not present in the insertCols vector -> -1
        first_name is in the first place in the insertCols vector -> 0
        last_name is in the third place in the insertCols vector -> 2
    */
    for (const auto &col : cols)
    {
        auto found = false;

        for (std::size_t i = 0; i < insertCols.size(); ++i)
        {
            if (col.name == insertCols[i])
            {
                found = true;
                colMap.push_back(i);
                break;
            };
        }

        if (!found)
        {
            if (!col.hasDefaultValue())
                throw std::invalid_argument("The column " + col.name + " does not have a default value, you have to provide one.");
        }
    }

    for (std::size_t rowIndex = 0; rowIndex < cols.size(); ++rowIndex)
    {
        auto &row = rows.at(rowIndex);
        Row normalizedRow;

        if (row.size() != cols.size())
            throw std::invalid_argument("The row count at row " + std::to_string(rowIndex) + " does not match.");

        for (std::size_t colIndex = 0; colIndex < cols.size(); ++colIndex)
        {
            auto &col = cols.at(colIndex);
            auto mapIndex = colMap.at(colIndex);

            ColType valueType;
            ColValue value;

            if (mapIndex != -1)
            {
                value = row.at(mapIndex);
                try
                {
                    valueType = colValueToColType(value).value();
                }
                catch (const std::bad_optional_access &e)
                {
                    throw std::invalid_argument("The value type for column " + col.name + " at row " + std::to_string(rowIndex) + " could not be resolved. ");
                }
                if (colValueToColType(value) != col.type)
                {
                    throw std::invalid_argument("The value type for column " + col.name + " at row " + std::to_string(rowIndex) + " does not match.\nReading " + colTypeToString(valueType) + " instead of " + colTypeToString(col.type) + ".");
                }
            }
            else
            {
                value = col.getDefaultValue();
            }

            normalizedRow.push_back(value);
        }

        table.writeRow(normalizedRow);
    }
}
