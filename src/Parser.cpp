#include <stdexcept>

#include "Parser.hpp"
#include "Helpers.hpp"

//? WithAttrs means types like int, long or constraints
ColumnRecord parseColumn(std::vector<Token>, bool withAttrs)
{
}

Parser::Parser(std::vector<Token> tokens)
{
    if (tokens.size() == 0)
        throw std::invalid_argument("You have to pass tokens to parser");

    this->tokens = std::move(tokens);
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

Token Parser::consume(TokenType comparedType)
{

    auto token = peek();

    if (comparedType != token.type)
        throw ParseError(
            std::string("Expected ") + tokenTypeToString(comparedType) + " but got " + tokenTypeToString(token.type), token.start, token.end);

    if (!isAtEnd())
        tokIndex++;

    return previous();
}

Token Parser::consume(ColValue comparedValue)
{

    auto token = peek();

    if (comparedValue != token.value)
        throw ParseError(
            std::string("Expected ") + tokenValueToString(comparedValue) + " but got " + tokenValueToString(token.value), token.start, token.end);

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
    if (firstToken.lexeme == "select")
    {
        this->parseSelect();
        return;
    }
    if (firstToken.lexeme == "insert")
    {
        this->parseInsert();
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

void Parser::parseSelect()
{
}

void Parser::parseInsert()
{
    std::vector<ColumnRecord> columns;
}

template <typename T, typename ParseItemFn>
std::vector<T> Parser::parseList(ParseItemFn parseItemFn)
{
    std::vector<T> list;

    this->consume("(");

    while (this->peek().lexeme != ")")
    {
        list.push_back(parseItemFn(this->tokens));

        try
        {
            this->consume(",");
        }
        catch (const std::exception &e)
        {

            this->consume(")");
            break;
        }
    }

    return list;
}

std::vector<ColumnRecord> Parser::parseCols()
{
    std::vector<ColumnRecord> columns;

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
        ColumnRecord column{};
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
