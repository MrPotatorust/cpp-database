#include "Engine.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

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

    try
    {
        auto lexer = Lexer(query);

        auto tokens = lexer.getTokens();

        auto parser = Parser(tokens);

        auto statement = parser.getStatement();
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
}

bool Engine::tableExists(std::string_view name)
{
    for (const auto &table : this->dbMetaData->tables)
    {
        if (table && table->name == name)
            return true;
    }

    return false;
}

void Engine::create(std::string_view name, std::vector<Column> cols)
{

    (void)cols;

    if (name.length() == 0)
        throw std::invalid_argument("Argument name cannot be of length 0, you need to provide atleast one letter.");

    if (cols.size() == 0)
        throw std::invalid_argument("Argument cols cannot be of length 0, you need to provide atleast one valid column.");

    if (this->tableExists(name))
        throw std::invalid_argument(std::string("A table with the name ") + std::string(name) + " already exists.");

    auto table = std::make_unique<Table>(Table{name, cols});

    this->dbMetaData->tables.push_back(std::move(table));
    this->dbMetaData->persist();
};
