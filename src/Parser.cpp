#include <stdexcept>

#include "Parser.hpp"

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

    return "Unknown";
}

Parser::Parser(const std::vector<Token> tokens)
{
    if (tokens.size() == 0)
        throw std::invalid_argument("You have to pass tokens to parser");

    this->tokens = tokens;
    this->tokIndex = 0;

    this->parse();
}

const Token &Parser::peek()
{
    return this->tokens[tokIndex];
}

Token Parser::advance()
{
    if (!isAtEnd())
        tokIndex++;

    return previous();
}

Token Parser::consume(TokenType tokenType)
{

    if (peek().type != tokenType)
    {
        throw std::invalid_argument(
            std::string("Expected ") + tokenTypeToString(tokenType) + " but got " + tokenTypeToString(peek().type));
    }

    if (!isAtEnd())
        tokIndex++;

    return previous();
}

Token Parser::previous()
{
    return tokens[tokIndex - 1];
}

bool Parser::isAtEnd()
{
    return peek().type == TokenType::EndF;
}

void Parser::parse()
{

    auto firstToken = this->peek();

    if (firstToken.lexeme == "create")
    {
        this->parseCreate();
        return;
    }

    throw ParseError("Could not locate any valid function", firstToken.start, firstToken.end);
}

const Statement &Parser::getStatement()
{
    return this->statement;
}

void Parser::parseCreate()
{
    auto createToken = this->advance();

    if (createToken.type != TokenType::SpecialWord || createToken.lexeme != "create")
    {
        throw ParseError("The first word in a create statement has to be 'create'", createToken.start, createToken.end);
    }
    auto tableToken = this->advance();
    if (tableToken.type != TokenType::SpecialWord || tableToken.lexeme != "table")
    {
        throw ParseError("The second word in a create statement has to be 'table'", tableToken.start, tableToken.end);
    }
    auto tableNameToken = this->advance();
    if (tableNameToken.lexeme.length() <= 0)
    {
        throw ParseError("Did not provide a table name", tableNameToken.start, tableNameToken.end);
    }
    auto tableCols = parseCols();

    Statement statement{};

    statement.function = DBFunction::Create;
    statement.tableName = tableNameToken.lexeme;
    statement.cols = tableCols;

    this->statement = statement;
}

std::vector<Column> Parser::parseCols()
{
    std::vector<Column> columns;

    auto openParen = this->advance();
    if (openParen.lexeme != "(")
    {
        throw ParseError("Expected '(' after table name", openParen.start, openParen.end);
    }

    while (!isAtEnd() && peek().lexeme != ")")
    {
        auto columnNameToken = this->advance();
        if (columnNameToken.lexeme.empty() || columnNameToken.lexeme == "," || columnNameToken.lexeme == "(" || columnNameToken.lexeme == ")")
        {
            throw ParseError("Expected a column name", columnNameToken.start, columnNameToken.end);
        }

        auto columnTypeToken = this->advance();
        Column column{};
        column.name = columnNameToken.lexeme;

        if (columnTypeToken.lexeme == "uint")
            column.type = ColType::UInt;
        else if (columnTypeToken.lexeme == "int")
            column.type = ColType::Int;
        else if (columnTypeToken.lexeme == "float")
            column.type = ColType::Float;
        else if (columnTypeToken.lexeme == "double")
            column.type = ColType::Double;
        else if (columnTypeToken.lexeme == "bool")
            column.type = ColType::Bool;
        else if (columnTypeToken.lexeme == "varchar")
            column.type = ColType::Varchar;
        else
        {
            throw ParseError("Expected a valid column type", columnTypeToken.start, columnTypeToken.end);
        }

        columns.push_back(column);

        if (peek().lexeme == ",")
        {
            this->advance();
        }
        else if (peek().lexeme != ")")
        {
            throw ParseError("Expected ',' or ')' after column definition", peek().start, peek().end);
        }
    }

    if (peek().lexeme != ")")
    {
        throw ParseError("Expected ')' after column definitions", peek().start, peek().end);
    }

    this->advance();

    return columns;
}

// void Parser::parseDrop()
// {
// }
// void Parser::parseDelete()
// {
// }
// void Parser::parseUpdate()
// {
// }
// void Parser::parseSelect()
// {
// }