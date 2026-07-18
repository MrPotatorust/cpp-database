#pragma once

#include <optional>
#include <string>

#include "constants.hpp"
#include "DBTypes.hpp"
#include "Lexer.hpp"

// This function exists because we need to normalize names for columns, tables...
bool containsOnlyLetters(const std::string &s);
// Changes in place
void toLowerCaseRef(std::string &upperCaseStr);
// Returns a value
std::string toLowerCase(std::string_view upperCaseStr);
std::string colValueToString(const ColValue &value);
std::string tokenTypeToString(TokenType tokenType);
std::string tokenValueToString(const ColValue &tokenValue);
std::string colTypeToString(ColType colType);
std::optional<ColType> colValueToColType(const ColValue &value);
