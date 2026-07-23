#include <type_traits>
#include <algorithm>

#include "Helpers.hpp"

bool containsOnlyLetters(const std::string &s)
{
    return all_of(s.begin(), s.end(), [](char ch)
                  { return isalpha(static_cast<unsigned char>(ch)); });
}

void toLowerCaseRef(std::string &upperCaseStr)
{
    std::transform(upperCaseStr.begin(), upperCaseStr.end(), upperCaseStr.begin(), [](unsigned char c)
                   { return static_cast<char>(std::tolower(c)); });
}

std::string toLowerCase(std::string_view upperCaseStr)
{
    std::string str(upperCaseStr);
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                   { return static_cast<char>(std::tolower(c)); });

    return str;
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

    return std::string{UNKNOWN_TOKEN_TYPE};
}

std::string tokenValueToString(const ColValue &tokenValue)
{
    return std::visit(
        [](const auto &value) -> std::string
        {
            using ValueType = std::remove_cvref_t<decltype(value)>;

            if constexpr (std::is_same_v<ValueType, std::monostate>)
                return std::string{EMPTY_VALUE};
            else if constexpr (std::is_same_v<ValueType, std::string>)
                return value;
            else if constexpr (std::is_same_v<ValueType, bool>)
                return value ? "true" : "false";
            else
                return std::to_string(value);
        },
        tokenValue);
}

std::string colValueToString(const ColValue &value)
{
    return std::visit([](const auto &value) -> std::string
                      {
        using ValueType = std::remove_cvref_t<decltype(value)>;
                    
        if constexpr (std::is_same_v<ValueType, std::monostate>){
            return std::string{EMPTY_VALUE};
        }            
        else if constexpr(std::is_same_v<ValueType, std::string>){
            return value;
        }
        else return 
            std::to_string(value); },
                      value);
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
    return std::string{UNKNOWN_TYPE};
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

std::optional<ColAttribute> stringToColAttribute(std::string_view value)
{
    const auto normalizedValue = toLowerCase(value);

    if (normalizedValue == "unique")
        return ColAttribute::Unique;
    if (normalizedValue == "auto_increment")
        return ColAttribute::Auto_increment;
    if (normalizedValue == "not_null")
        return ColAttribute::Not_null;
    if (normalizedValue == "primary_key")
        return ColAttribute::Primary_key;

    return std::nullopt;
}

std::string colAttributeToString(ColAttribute attribute)
{
    switch (attribute)
    {
    case ColAttribute::Unique:
        return "unique";
    case ColAttribute::Auto_increment:
        return "auto_increment";
    case ColAttribute::Not_null:
        return "not_null";
    case ColAttribute::Primary_key:
        return "primary_key";
    }

    return std::string{UNKNOWN_TYPE};
}
