#include <vector>
#include <variant>
#include <string>
#include <ranges>
#include <string_view>
#include <iomanip>
#include <iostream>

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

class Parser
{
public:
    std::string parseCommand(std::string command);
    Parser(const std::string_view command);

private:
    std::vector<Token> tokens;

    std::vector<Token> tokenize(std::string_view source);
    Token assignToken(std::string_view word);
};