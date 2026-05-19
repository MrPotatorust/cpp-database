#pragma once

#include "Engine.hpp"
#include <string>
#include <vector>

class Database
{
public:
    std::string name;
    Database();
    bool query(std::string query);

private:
    Engine engine;
    int initializeState();
};