#include <stdexcept>
#include <charconv>
#include <ranges>
#include <algorithm>

#include "Parser.hpp"

Parser::Parser(std::string command)
{
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

    if (command.length() == 0)
        throw std::invalid_argument("You have to pass a command");

    this->tokenPos = 0;

    this->tokenize(command);

    this->convertToAST();
}

// Part of "Lexer"
TokenPtr Parser::assignToken(const std::string_view word)
{

    auto token = std::make_unique<Token>();
    token->type = TokenType::Integer;
    token->value = std::monostate{};
    token->start = 0;
    token->end = 0;
    token->lexeme = word;

    // Check for operators
    if (word == "+")
    {
        token->type = TokenType::Plus;
        token->value = std::monostate{};
        return token;
    }
    if (word == "-")
    {
        token->type = TokenType::Minus;
        token->value = std::monostate{};
        return token;
    }
    if (word == "*")
    {
        token->type = TokenType::Multiplication;
        token->value = std::monostate{};
        return token;
    }
    if (word == "/")
    {
        token->type = TokenType::Division;
        token->value = std::monostate{};
        return token;
    }
    if (word == "%")
    {
        token->type = TokenType::Modulus;
        token->value = std::monostate{};
        return token;
    }
    if (word.starts_with('"') && word.ends_with('"'))
    {
        std::string_view wordNoQuotes = word.substr(1, word.length() - 1);
        token->type = TokenType::String;
        token->value = wordNoQuotes;
        return token;
    }

    // Number parsing

    int intVal = 0;
    double doubleVal = 0.0;

    auto [ptr, ec] = std::from_chars(word.data(), word.data() + word.size(), intVal);

    if (ec == std::errc())
    {
        auto [ptr2, ec2] = std::from_chars(word.data(), word.data() + word.size(), intVal);

        if (ec2 != std::errc())
        {
            throw ec2;
        }

        if (intVal != doubleVal)
        {
            token->type = TokenType::Double;
            token->value = doubleVal;
            return token;
        }

        token->type = TokenType::Integer;
        token->value = intVal;
        return token;
    }
    else if (ec == std::errc::result_out_of_range)
    {
        throw std::out_of_range("The passed integer is too big");
    }

    // Temp function parsing
    if (word == "select")
    {
        token->type = TokenType::Function;
        token->value = std::string_view("select");
        return token;
    }
    else if (word == "from")
    {
        token->type = TokenType::Function;
        token->value = std::string_view("from");
        return token;
    }
    else if (word == "where")
    {
        token->type = TokenType::Function;
        token->value = std::string_view("where");
        return token;
    }

    throw std::invalid_argument("Passed word is not declared in TokenType enum");
}

// Part of "Lexer"
void Parser::tokenize(std::string_view source)
{
    for (const auto &&element : std::views::split(source, ' '))
    {
        auto word = std::string_view(element);

        auto beginIt = element.begin();
        auto endIt = element.end();

        auto token = assignToken(word);
        token->start = beginIt - source.begin();
        token->end = endIt - source.begin();

        this->tokens.push_back(std::move(token));
    }

    std::cout << "Token count: " << this->tokens.size() << '\n';
};

TokenPtr Parser::peekToken()
{
}

void Parser::nextToken()
{
    if (this->tokens.empty())
        return;

    if (this->tokenPos + 1 < this->tokens.size())
        ++this->tokenPos;
}

void Parser::prevToken()
{
    if (this->tokens.empty())
        return;

    if (this->tokenPos > 0)
        --this->tokenPos;
}

void Parser::convertToAST()
{
    if (this->tokens.empty())
        throw std::invalid_argument("Cant convert to Abstract Syntax tree, no tokens are available");

    for (std::size_t i = 0; i < tokens.size(); i++)
    {
        auto &&token = this->tokens.at(this->tokenPos);

        std::cout << this->tokens.at(this->tokenPos)->lexeme << '\n';
        this->nextToken();
    }
}