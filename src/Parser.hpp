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

struct Statement
{
    DBFunction function;
    std::string tableName;
    std::vector<ColumnRecord> cols;
};

class Parser
{
public:
    std::string_view command;

    Parser(std::vector<Token> tokens);

    const Statement &getStatement();

private:
    std::vector<Token> tokens;
    Statement statement;

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

    template <typename T, typename ParseItem>
    std::vector<T> parseList(ParseItem parseItem);

    std::vector<ColumnRecord> parseCols();
};
