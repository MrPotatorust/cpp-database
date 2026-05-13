#include "Database.hpp"
#include <iostream>

Database::Database() : engine()
{
    this->initializeState();
}

int Database::initializeState()
{
    this->name = engine.getDbName();
    return 0;
}