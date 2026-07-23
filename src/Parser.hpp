#include <vector>
#include <variant>
#include <string>
#include <ranges>
#include <string_view>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include "DBTypes.hpp"
#include "Lexer.hpp"

#pragma once

enum class ExpressionOp
{
    Equal,
};

enum class CreateStmtOpts
{
    Table,
    Database
};

struct ASTNode
{
    virtual ~ASTNode() = default;
};

struct Identifier : ASTNode
{
    std::string value;
};

// Just for the WHERE expressions now
struct Expression : ASTNode
{
    ExpressionOp operation;
    std::unique_ptr<Identifier> left;
    std::unique_ptr<Identifier> right;
};

struct CreateStatement
{
    std::string tableName;
    std::vector<ColumnRecord> columns;
};

struct InsertStatement
{
    std::string tableName;
    std::vector<std::string> columns;
    std::vector<Row> rows;
};

struct SelectStatement
{
    std::string tableName;
    std::vector<std::string> columns;
};

using Statement = std::variant<CreateStatement, InsertStatement, SelectStatement>;

class Parser
{
public:
    std::string_view command;

    Parser(std::vector<Token> tokens);

    const Statement &getStatement();
    std::string parseNameToken();

private:
    std::vector<Token> tokens;
    std::optional<Statement> statement;

    std::size_t tokIndex;

    const Token &peek();
    Token advance();

    Token consume(TokenType comparedType);
    Token consume(ColValue comparedValue);

    Token previous();

    bool isAtEnd();

    void parse();
    void parseCreate();
    void parseSelect();
    void parseInsert();

    std::string normalizeForName(const Token &token);

    std::string parseItem(); //? The default parser function, just parses strings
    ColumnRecord parseColumnRecord();
    ColAttribute parseColAttribute();

    //? This is an overload function with a default parser function
    template <typename T>
    std::vector<T> parseList(bool inParentheses = true);

    template <typename T, typename ParseItemFn>
    std::vector<T> parseList(ParseItemFn parseItemFn, bool inParentheses = true);
};
