#include <stdexcept>
#include <algorithm>
#include <cctype>

#include "Parser.hpp"
#include "Helpers.hpp"

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
            std::string{"Expected "} + tokenTypeToString(comparedType) + " but got " + tokenTypeToString(token.type), token.start, token.end);

    if (!isAtEnd())
        tokIndex++;

    return previous();
}

Token Parser::consume(ColValue comparedValue)
{

    auto token = peek();

    if (comparedValue != token.value)
        throw ParseError(
            std::string{"Expected "} + tokenValueToString(comparedValue) + " but got " + tokenValueToString(token.value), token.start, token.end);

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

    auto function = toLowerCase(firstToken.lexeme);

    if (function == "create")
    {
        this->parseCreate();
        return;
    }
    if (function == "select")
    {
        this->parseSelect();
        return;
    }
    if (function == "insert")
    {
        this->parseInsert();
        return;
    }

    throw ParseError("Could not locate any valid function", firstToken.start, firstToken.end);
}

const Statement &Parser::getStatement()
{
    return this->statement.value();
}

std::string Parser::parseNameToken()
{
    auto nameToken = this->advance();

    return this->normalizeForName(nameToken);
}

std::string
Parser::normalizeForName(const Token &token)
{

    std::string str(token.lexeme);

    if (str.length() == 0)
    {
        throw ParseError("Name cannot be of length 0", token.start, token.end);
    }

    if (!containsOnlyLetters(token.lexeme) || token.lexeme.length() > maxNameLength)
        throw ParseError("Cant normalize token according to naming standards", token.start, token.end);

    toLowerCaseRef(str);

    return str;
}

void Parser::parseCreate()
{
    auto createToken = this->advance();

    if (createToken.type != TokenType::SpecialWord || toLowerCase(createToken.lexeme) != "create")
    {
        throw ParseError("The first word in a create statement has to be 'create'", createToken.start, createToken.end);
    }
    auto tableToken = this->advance();
    if (tableToken.type != TokenType::SpecialWord || toLowerCase(tableToken.lexeme) != "table")
    {
        throw ParseError("The second word in a create statement has to be 'table'", tableToken.start, tableToken.end);
    }
    auto tableName = this->parseNameToken();
    auto tableCols = parseList<ColumnRecord>([this]()
                                             { return this->parseColumnRecord(); });

    this->statement = CreateStatement{
        tableName,
        std::move(tableCols)};
}

void Parser::parseSelect()
{
}

void Parser::parseInsert()
{

    auto insertToken = this->advance();
    if (insertToken.type != TokenType::SpecialWord || toLowerCase(insertToken.lexeme) != "insert")
    {
        throw ParseError("The first word in a insert statement has to be insert''", insertToken.start, insertToken.end);
    }

    auto intoToken = this->advance();
    if (intoToken.type != TokenType::SpecialWord || toLowerCase(intoToken.lexeme) != "into")
    {
        throw ParseError("The second word in a insert statement has to be 'into'", intoToken.start, intoToken.end);
    }

    auto tableName = this->parseNameToken();

    std::vector<std::string> columns;
    if (this->peek().lexeme == "(")
    {
        columns = parseList<std::string>();
    }

    auto valuesToken = this->advance();
    if (valuesToken.type != TokenType::SpecialWord || toLowerCase(valuesToken.lexeme) != "values")
    {
        throw ParseError("The word before values list in a insert statement has to be 'values'", valuesToken.start, valuesToken.end);
    }

    //? Parses each columns value, because the insert can be a list of list values ((123, "fun"), (123, "hihih"))
    std::vector<Row> rows = parseList<Row>([this]()
                                           { return this->parseList<ColValue>([this]()
                                                                              { return this->advance().value; }); }, false);

    this->statement = InsertStatement{
        tableName,
        columns,
        rows};
}

ColAttribute Parser::parseColAttribute()
{
    auto token = this->advance();

    auto colAttr = stringToColAttribute(token.lexeme);

    if (!colAttr.has_value())
        throw ParseError("Could not resolve column attribute", token.start, token.end);

    return colAttr.value();
}

std::string Parser::parseItem()
{
    auto token = this->advance();

    //? Alternative safer implementation
    // auto tokenString = tokenValueToString(token.value);

    // if (tokenString == std::string{UNKNOWN_TOKEN_TYPE})
    // {clear
    //     throw ParseError("Could not convert token to string", token.start, token.end);
    // }
    return token.lexeme;
}

template <typename T>
std::vector<T> Parser::parseList(bool inParentheses)
{
    return this->parseList<T>([this]()
                              { return this->parseItem(); }, inParentheses);
}

template <typename T, typename ParseItemFn>
std::vector<T> Parser::parseList(ParseItemFn parseItemFn, bool inParentheses)
{
    std::vector<T> list;

    if (inParentheses)
        this->consume("(");

    while (this->peek().lexeme != ")")
    {
        list.push_back(parseItemFn());

        try
        {
            this->consume(",");
        }
        catch (const std::exception &e)
        {
            if (inParentheses)
                this->consume(")");
            break;
        }
    }

    return list;
}

ColumnRecord Parser::parseColumnRecord()
{
    ColumnRecord columnRecord;

    auto nameToken = this->advance();
    try
    {
        columnRecord.name = this->normalizeForName(nameToken);
    }
    catch (const std::exception &e)
    {
        throw ParseError("Expected a column name", nameToken.start, nameToken.end);
    }

    auto columnTypeToken = this->advance();


    auto colType = stringToColType(columnTypeToken.lexeme);

    if(!colType.has_value())
        throw ParseError("Expected a valid column type", columnTypeToken.start, columnTypeToken.end);

    columnRecord.type = colType.value();



    columnRecord.attrs = this->parseList<ColAttribute>([this]()
                                                       { return this->parseColAttribute(); }, false);

    return columnRecord;
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
