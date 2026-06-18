#include <iostream>
#include <stdexcept>
#include <fstream>
#include <stdexcept>

#include "Database.hpp"
#include "Engine.hpp"
#include "Parser.hpp"

Metadata::Metadata()
{
    this->loadFile();
};

void Metadata::loadFile()
{
    std::cout << "Loading file \n";
    std::ifstream file("./storage/metadata.dat", std::ios::binary);

    if (file.fail())
        return;

    std::vector<std::string>
        tables;

    std::size_t len;

    while (file.read(reinterpret_cast<char *>(&len), sizeof(len)))
    {
        std::string buffer(len, '\0');
        file.read(buffer.data(), len);
        tables.push_back(buffer);
    }

    std::cout << "Persisted tables \n";
    for (auto table : tables)
    {
        std::cout << table << '\n';
    }

    file.close();
};

void Metadata::persistToFile()
{
    std::ofstream file("./storage/metadata.dat", std::ios::binary);

    std::size_t offset = this->tables.size() * sizeof(char *);

    (void)offset;

    for (const auto &table : this->tables)
    {
        std::cout << "Saving table: " << table->name << '\n';
        std::size_t len = table->name.size();
        file.write(reinterpret_cast<const char *>(&len), sizeof(len));
        file.write(table->name.data(), len);
    }
};

void Metadata::persist()
{
    this->persistToFile();
};

Database::Database()
{
    this->metaData = std::make_unique<Metadata>();
    this->engine = std::make_unique<Engine>(*this);
};

Metadata &Database::getMetadata()
{
    return *this->metaData;
}

bool Database::query(std::string query)
{

    try
    {
        this->engine->query(query);
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Could not parse the query" << '\n';
        return false;
    }

    return true;
};