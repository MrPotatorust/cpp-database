#include "Engine.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Database.hpp"

Engine::Engine(Database &db)
    : db(db) {
      };

void Engine::query(std::string query)
{
    try
    {
        auto lexer = Lexer(query);

        auto tokens = lexer.getTokens();

        auto parser = Parser(tokens);

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

    this->db.getMetadata().persist();
}

bool Engine::tableExists(std::string_view name)
{
    for (const auto &table : this->db.getMetadata().tables)
    {
        if (table && table->name == name)
            return true;
    }

    return false;
}

void Engine::create(std::string name, std::vector<Column> cols)
{

    if (name.length() == 0)
        throw std::invalid_argument("Argument name cannot be of length 0, you need to provide atleast one letter.");

    if (cols.size() == 0)
        throw std::invalid_argument("Argument cols cannot be of length 0, you need to provide atleast one valid column.");

    if (this->tableExists(name))
        throw std::invalid_argument(std::string("A table with the name ") + std::string(name) + " already exists.");

    auto table = std::make_unique<Table>(Table{name, cols});

    this->db.getMetadata().tables.push_back(std::move(table));

    std::cout << "Succesfully created a table \n";
};
