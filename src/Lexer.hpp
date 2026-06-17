#include <variant>
#include <memory>
#include <vector>
#include <iostream>

#include "DBTypes.hpp"

#pragma once

enum class LexerType
{
    String,

    Void
};

enum class TokenType
{
    Integer,
    Double,
    String,

    SpecialWord, // TEMP PLACEMENT FOR FUNCTIONS
    Create,
    Drop,
    Delete,
    Update,
    Select,

    EndF,
    Void
};

using TokenValue = std::variant<
    std::monostate, int, double, std::string>;

struct Token
{
    TokenType type;
    TokenValue value;
    std::size_t start;
    std::size_t end;
    std::string lexeme;
};

class Lexer
{
public:
    std::string_view command;
    std::vector<Token> tokens;

    Lexer(const std::string command);

    std::vector<Token> getTokens();

private:
    std::string lexeme;

    std::size_t lexemeStart;
    std::size_t lexemeEnd;

    void clearLexeme();

    void tokenize();
    void assignToken(LexerType type = LexerType::Void);
};

class ParseError : public std::runtime_error
{
public:
    ParseError(
        std::string msg,
        std::size_t start,
        std::size_t end)
        : std::runtime_error(std::move(msg)), start(start), end(end)
    {
    }

    std::size_t start;
    std::size_t end;
};
