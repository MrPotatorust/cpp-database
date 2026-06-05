#include <vector>
#include <variant>
#include <string>
#include <ranges>
#include <string_view>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>

#pragma once

enum class LexerType
{
    String,

    Void
};

enum class TokenType : std::uint8_t
{
    Integer,
    Double,
    String,

    SpecialWord, // Minimum length of 3

    // Operators
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

class Parser
{
public:
    std::string_view command;

    Parser(const std::string command);
    std::vector<Token> getTokens();

private:
    std::vector<Token> tokens;

    std::string lexeme;

    std::size_t lexemeStart;
    std::size_t lexemeEnd;

    void clearLexeme();

    void tokenize(std::string_view source);
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
