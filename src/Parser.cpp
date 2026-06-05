#include <stdexcept>
#include <charconv>
#include <ranges>
#include <algorithm>
#include <cctype>

#include "Parser.hpp"

Parser::Parser(std::string command)
    : lexemeStart(0), lexemeEnd(0)
{
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

    if (command.length() == 0)
        throw std::invalid_argument("You have to pass a command");

    this->tokenize(command);
}

// Part of "Lexer"
void Parser::assignToken(LexerType type)
{
    if (this->lexeme.empty())
        return;

    auto token = Token{};
    token.type = TokenType::Integer;
    token.start = lexemeStart;
    token.end = lexemeEnd;

    token.value = std::monostate{};
    token.lexeme = this->lexeme;

    if (type == LexerType::String)
    {
        token.type = TokenType::String;
        token.value = this->lexeme;
    }
    else
    {
        // Number parsing

        int intVal = 0;
        double doubleVal = 0.0;

        auto [ptr, ec] = std::from_chars(this->lexeme.data(), this->lexeme.data() + this->lexeme.size(), intVal);

        if (ec == std::errc())
        {
            auto [ptr2, ec2] = std::from_chars(this->lexeme.data(), this->lexeme.data() + this->lexeme.size(), intVal);

            if (ec2 != std::errc())
            {
                throw ec2;
            }

            if (intVal != doubleVal)
            {
                token.type = TokenType::Double;
                token.value = doubleVal;
            }
            else
            {
                token.type = TokenType::Integer;
                token.value = intVal;
            }
        }
        else if (ec == std::errc::result_out_of_range)
        {
            throw ParseError("The passed integer is too big", token.start, token.end);
        }
        else
        {
            token.type = TokenType::SpecialWord;
            token.value = this->lexeme;
        }
    }

    tokens.push_back(token);

    this->clearLexeme();
}

// Part of "Lexer"
void Parser::tokenize(std::string_view source)
{

    bool inString = false;

    for (std::size_t i{0}; i < source.length(); i++)
    {
        char c = source[i];

        if (inString && c != '\'' && c != '\"')
        {
            if (this->lexeme.empty())
                this->lexemeStart = i;

            this->lexeme += c;
            this->lexemeEnd = i + 1;
            continue;
        }

        bool isAlnum = std::isalnum(static_cast<unsigned char>(c)) != 0;

        if (isAlnum)
        {
            if (this->lexeme.empty())
                this->lexemeStart = i;

            this->lexeme += c;
            this->lexemeEnd = i + 1;
            continue;
        }

        // This handles special non alnum chars
        switch (c)
        {

        //? Space breaking
        case ' ':
            if (!inString)
            {
                this->assignToken();
                break;
            }

            this->lexeme += c;
            break;
        case '\n':
            if (!inString)
            {
                this->assignToken();
                break;
            }

            this->lexeme += c;
            break;

        //? String identifier
        case '\'':
            if (inString)
            {
                this->assignToken(LexerType::String);
            }
            else
            {
                this->assignToken();
                this->lexemeStart = i + 1;
                this->lexemeEnd = i + 1;
            }
            inString = !inString;
            break;
        case '\"':
            if (inString)
            {
                this->assignToken(LexerType::String);
            }
            else
            {
                this->assignToken();
                this->lexemeStart = i + 1;
                this->lexemeEnd = i + 1;
            }
            inString = !inString;
            break;
        case ',':
            if (!inString)
            {
                this->assignToken();
                this->lexeme = ",";
                this->lexemeStart = i;
                this->lexemeEnd = i + 1;
                this->assignToken();
            }
            break;

        //? Parentheses
        case '(':
            if (!inString)
            {
                this->assignToken();
                this->lexeme = "(";
                this->lexemeStart = i;
                this->lexemeEnd = i + 1;
                this->assignToken();
            }
            break;
        case ')':
            if (!inString)
            {
                this->assignToken();
                this->lexeme = ")";
                this->lexemeStart = i;
                this->lexemeEnd = i + 1;
                this->assignToken();
            }
            break;
        default:
            if (inString)
                this->lexeme += c;
            break;
        }
    }

    this->assignToken();

    // for (const auto &&element : std::views::split(source, ' '))
    // {
    //     auto word = std::string_view(element);

    //     auto beginIt = element.begin();
    //     auto endIt = element.end();

    //     auto token = assignToken(word);
    //     token->start = beginIt - source.begin();
    //     token->end = endIt - source.begin();

    //     this->tokens.push_back(std::move(token));
    // }

    // if (tokens[0]->type != TokenType::SpecialWord)
    // {
    //     throw ParseError("The first token is not a Special Word type, so a function", tokens[0]->start, tokens[0]->end);
    // }

    // std::cout << "Token count: " << this->tokens.size() << '\n';
};

void Parser::clearLexeme()
{
    this->lexeme.clear();
    this->lexemeStart = 0;
    this->lexemeEnd = 0;
}

std::vector<Token> Parser::getTokens()
{
    return std::move(this->tokens);
}
