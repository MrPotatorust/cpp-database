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

struct CreateStmt
{
    CreateStmtOpts create;
    std::string name;
    std::vector<Column> cols;
};

struct DropStmt
{
    std::string name;
};

struct TruncateStmt
{
    std::string name;
};

struct DeleteStmt
{
    std::string name;
    std::optional<Expression>
        where;
};

struct UpdateStmt
{
    std::string name;
};

struct SelectStmt
{
    std::string name;
};

class Parser
{
public:
    std::string_view command;

    Parser(const std::vector<Token> tokens);

private:
    std::vector<Token> tokens;

    void convertToAST();
};
