#pragma once

#include "Engine.hpp"
#include <string>

class Database
{
public:
    std::string name;

    Database();

private:
    Engine engine;
    int initializeState();
};