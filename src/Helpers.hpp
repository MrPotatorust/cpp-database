#pragma once

#include <optional>
#include <string>

#include "DBTypes.hpp"
#include "Lexer.hpp"

std::string tokenTypeToString(TokenType tokenType);
std::string tokenValueToString(const ColValue &tokenValue);
std::string colTypeToString(ColType colType);
std::optional<ColType> colValueToColType(const ColValue &value);
