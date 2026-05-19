#include <vector>
#include <variant>
#include <string>
#include <ranges>
#include <string_view>
#include <iomanip>
#include <iostream>
#include <memory>

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

    Default

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

struct ASTNode
{
    ASTNode *left;
    ASTNode *right;

    Token *token;
};

struct AST
{
    ASTNode *root;
};

class Parser
{
public:
    std::string_view command;

    Parser(const std::string command);

private:
    std::vector<std::unique_ptr<Token>> tokens;
    AST ast;

    std::size_t tokenPos;

    void nextToken();
    void prevToken();

    void tokenize(std::string_view source);
    std::unique_ptr<Token> assignToken(std::string_view word);

    void convertToAST();
};