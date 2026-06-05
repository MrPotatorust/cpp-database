#include "Engine.hpp"
#include "Parser.hpp"

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

    (void)offset;

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

void Engine::query(std::string query)
{
    auto parser = Parser(query);

    this->tokens = parser.getTokens();

    std::cout << "Lexemes: " << this->tokens.size() << '\n';
    for (auto token : tokens)
    {
        std::cout << token.lexeme << '\n';
    }

    // if (tokens[0].lexeme == "create")
    // {
    //     if (tokens[1].lexeme != "table")
    //         throw ParseError("The CREATE TABLE syntax is not correct, TABLE is missing", tokens[1].start, tokens[1].end);
    // }

    throw std::invalid_argument("Could not find the desired Engine function");
};

void Engine::create(std::string_view name, Column cols)
{

    (void)cols;

    if (name.length() == 0)
        throw std::invalid_argument("Argument name cannot be of length 0, you need to provide atleast one letter.");

    // if (cols.size() == 0)
    //     throw std::invalid_argument("Argument cols cannot be of length 0, you need to provide atleast one valid column.");

    auto table = std::make_unique<Table>(Table{name, {}});

    this->dbMetaData->tables.push_back(std::move(table));
    this->dbMetaData->persist();
};
