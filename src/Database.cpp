#include "Database.hpp"
#include "Parser.hpp"
#include <iostream>
#include <stdexcept>

Database::Database() : engine()
{
    this->initializeState();
};

int Database::initializeState()
{

    return 0;
};

bool Database::query(std::string query)
{

    std::vector<Token> tokens;

    try
    {
        auto parser = Parser(query);
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Could not parse the query" << '\n';
        return false;
    }

    return true;
};