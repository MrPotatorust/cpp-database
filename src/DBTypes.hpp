#include <vector>
#include <memory>
#include <variant>

#pragma once

enum class ColType : std::uint8_t
{
    UInt,
    Int,
    Float,
    Double,
    Bool,
    Varchar,
};

// Constraints, attributes
enum class ColAttribute
{
    Unique,
    Auto_increment,
    Not_null,
    Primary_key
};

struct Column
{
    std::string name;
    ColType type;
    std::vector<ColAttribute> colAttrs;
};

enum class DBFunction
{
    Create,

};