#include <type_traits>

#include "Helpers.hpp"

namespace
{
    constexpr auto UKNOWN_TYPE = "Unknown";
}

std::string tokenTypeToString(TokenType tokenType)
{
    switch (tokenType)
    {
    case TokenType::Integer:
        return "Integer";
    case TokenType::Double:
        return "Double";
    case TokenType::String:
        return "String";
    case TokenType::SpecialWord:
        return "SpecialWord";
    case TokenType::Create:
        return "Create";
    case TokenType::Drop:
        return "Drop";
    case TokenType::Delete:
        return "Delete";
    case TokenType::Update:
        return "Update";
    case TokenType::Select:
        return "Select";
    case TokenType::EndF:
        return "EndF";
    case TokenType::Void:
        return "Void";
    }

    return UKNOWN_TYPE;
}

std::string tokenValueToString(const ColValue &tokenValue)
{
    return std::visit(
        [](const auto &value) -> std::string
        {
            using ValueType = std::remove_cvref_t<decltype(value)>;

            if constexpr (std::is_same_v<ValueType, std::monostate>)
                return "Empty";
            else if constexpr (std::is_same_v<ValueType, std::string>)
                return value;
            else if constexpr (std::is_same_v<ValueType, bool>)
                return value ? "true" : "false";
            else
                return std::to_string(value);
        },
        tokenValue);
}

std::string colTypeToString(ColType colType)
{
    switch (colType)
    {
    case ColType::UInt:
        return "UInt";
        break;
    case ColType::Int:
        return "Int";
    case ColType::Float:
        return "Float";
    case ColType::Double:
        return "Double";
    case ColType::Bool:
        return "Bool";
    case ColType::Varchar:
        return "Varchar";
    }
    return UKNOWN_TYPE;
}

std::optional<ColType> colValueToColType(const ColValue &value)
{
    return std::visit(
        [](const auto &storedValue) -> std::optional<ColType>
        {
            using ValueType = std::remove_cvref_t<decltype(storedValue)>;

            if constexpr (std::is_same_v<ValueType, std::monostate>)
                return std::nullopt;
            else if constexpr (std::is_same_v<ValueType, unsigned int>)
                return ColType::UInt;
            else if constexpr (std::is_same_v<ValueType, int>)
                return ColType::Int;
            else if constexpr (std::is_same_v<ValueType, float>)
                return ColType::Float;
            else if constexpr (std::is_same_v<ValueType, double>)
                return ColType::Double;
            else if constexpr (std::is_same_v<ValueType, bool>)
                return ColType::Bool;
            else
                return ColType::Varchar;
        },
        value);
}
