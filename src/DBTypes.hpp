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

struct Column
{
    ColType type;
    std::string_view name;
};
