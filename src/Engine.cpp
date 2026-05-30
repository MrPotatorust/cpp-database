#include "Engine.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>

constexpr int strLength = 255;

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

    size_t len;
    file.read(reinterpret_cast<char *>(&len), sizeof(len));

    std::string buffer(len, '\0');
    file.read(buffer.data(), len);

    std::cout << "Buffer: ";
    std::cout << buffer << '\n';
};

void Metadata::persistToFile()
{
    std::ofstream file("./storage/metadata.dat", std::ios::binary);

    std::size_t offset = this->tables.size() * sizeof(char *);

    for (const auto &table : this->tables)
    {
        size_t len = table->name.size();
        file.write(reinterpret_cast<const char *>(&len), sizeof(len));
        file.write(table->name.data(), len);
    }
};

void Metadata::persist()
{
    this->persistToFile();
};

Engine::Engine()
{
    this->dbMetaData = std::make_unique<Metadata>();
};

void Engine::createTable(std::string_view name, std::vector<Column> cols)
{
    if (name.length() == 0)
        throw std::invalid_argument("Argument name cannot be of length 0, you need to provide atleast one letter.");

    if (cols.size() == 0)
        throw std::invalid_argument("Argument cols cannot be of length 0, you need to provide atleast one valid column.");

    auto table = std::make_unique<Table>(name, cols);

    this->dbMetaData->tables.push_back(std::move(table));
    this->dbMetaData->persist();
};