#include "Database.hpp"
#include "Engine.hpp"
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
    auto engine = Engine();

    // auto columns = std::vector<Column>();

    // auto testCol = Column(ColType::UInt, "id");

    // columns.push_back(testCol);

    // engine.createTable("test_table", columns);

    // std::cout << engine.dbMetaData->tables.at(0)->name << '\n';

    try
    {
        engine.query(query);
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Could not parse the query" << '\n';
        return false;
    }

    return true;
};