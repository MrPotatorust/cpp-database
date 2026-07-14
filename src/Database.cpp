#include <iostream>
#include <stdexcept>
#include <fstream>
#include <stdexcept>

#include "Database.hpp"
#include "Engine.hpp"
#include "Parser.hpp"
#include "DBTypes.hpp"

namespace
{
    // Memory layout: strLen(8B), string
    void writeString(std::ofstream &file, const std::string &val)
    {

        std::uint64_t len = static_cast<std::uint64_t>(val.size());

        file.write(reinterpret_cast<const char *>(&len), sizeof(len));
        file.write(val.data(), len);
    };

    // Memory layout: strLen(8B or nBytes), string
    // nBytes is the number of bytes to read into strLen, default is 8
    std::string readString(std::ifstream &file, std::size_t nBytes = 0)
    {
        std::uint64_t strLen{0};
        std::string str;

        std::size_t bytesToRead = nBytes == 0 ? sizeof(strLen) : nBytes;

        if (!file.read(reinterpret_cast<char *>(&strLen), bytesToRead))
            throw std::out_of_range("Could not read the string length from the file.");

        str.resize(strLen);

        if (!file.read(&str[0], strLen))
            throw std::out_of_range("Could not read the string from the file.");

        return str;
    }

};

Database::Database()
{
    this->loadFile();
};

std::vector<Table> &Database::getTables()
{
    return this->tables;
};

const std::vector<Table> &Database::getTables() const
{
    return this->tables;
};

void Database::addTable(const Table &table)
{
    this->tables.push_back(table);
}

void Database::loadFile()
{
    std::cout << "Loading file \n";
    std::ifstream file("./storage/metadata.dat", std::ios::binary);

    if (file.fail())
        return;

    std::size_t colsCount{0};
    std::size_t colNameLen{0};

    while (true)
    {
        if (!file.read(reinterpret_cast<char *>(&colsCount), sizeof(std::uint8_t)))
            break;

        auto table = Table();
        table.name = readString(file);

        bool readOk = true;

        for (std::size_t i = 0; i < colsCount; ++i)
        {
            ColumnRecord col;

            if (!file.read(reinterpret_cast<char *>(&colNameLen), sizeof(std::uint64_t)))
            {
                readOk = false;
                break;
            }

            col.name.resize(colNameLen);
            if (!file.read(&col.name[0], colNameLen))
            {
                readOk = false;
                break;
            }

            std::uint8_t type = 0;
            if (!file.read(reinterpret_cast<char *>(&type), sizeof(type)))
            {
                readOk = false;
                break;
            }
            col.type = static_cast<ColType>(type);

            table.columns.push_back(std::move(col));
        }

        if (!readOk)
            break;

        this->tables.push_back(std::move(table));
    }

    for (auto &table : this->tables)
    {

        std::cout << "Table " << table.name << '\n';
        for (auto col : table.columns)
        {
            std::cout << col.name << '\n';
            std::cout << static_cast<int>(col.type) << '\n';
        }
    }

    file.close();
};

void Database::persistToFile()
{
    std::ofstream file("./storage/metadata.dat", std::ios::binary);

    for (const auto &table : this->tables)
    {

        std::uint8_t colsCount = static_cast<std::uint8_t>(table.columns.size());

        file.write(reinterpret_cast<const char *>(&colsCount), sizeof(colsCount));

        writeString(file, table.name);

        for (const auto &col : table.columns)
        {
            writeString(file, col.name);
            std::uint8_t type = static_cast<std::uint8_t>(col.type);
            file.write(reinterpret_cast<const char *>(&type), sizeof(type));
        }
    }
};

void Database::persist()
{
    this->persistToFile();
};

bool Database::query(std::string query)
{

    try
    {
        Engine engine(*this);
        engine.query(query);
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Could not parse the query" << '\n';
        return false;
    }

    return true;
};
