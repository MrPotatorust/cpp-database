#include <vector>
#include <variant>
#include <string>
#include <ranges>
#include <string_view>
#include <iomanip>
#include <iostream>
#include <memory>

#pragma once

enum class TokenType
{
    Integer,
    Double,
    String,

    Function,

    // Operators
    Plus,
    Minus,
    Multiplication,
    Division,
    Modulus,

    End,

    Void
};

using TokenValue = std::variant<
    std::monostate, int, double, std::string_view>;

struct Token
{
    TokenType type;
    TokenValue value;
    std::size_t start;
    std::size_t end;
    std::string lexeme;
};

using TokenPtr = std::unique_ptr<Token>;

struct ASTNode
{
    virtual ~ASTNode() = default;

    virtual void print();

    TokenPtr token;
};

using ASTPtr = std::unique_ptr<ASTNode>;

struct IntNode : ASTNode
{
    int value;

    explicit IntNode(int v) : value(v) {};
};

struct DoubleNode : ASTNode
{
    double value;

    explicit DoubleNode(double v) : value(v) {};
};

struct FunctionNode : ASTNode
{
    std::string_view value;

    ASTPtr left;
    ASTPtr right;

    explicit FunctionNode(std::string_view v, ASTPtr l, ASTPtr r) : value(v), left(std::move(l)), right(std::move(r)) {};
};

class Parser
{
public:
    std::string_view command;

    Parser(const std::string command);

private:
    std::vector<TokenPtr> tokens;

    std::size_t tokenPos;

    void nextToken();
    void prevToken();

    void tokenize(std::string_view source);
    TokenPtr assignToken(std::string_view word);

    void convertToAST();

    TokenPtr peekToken();
};