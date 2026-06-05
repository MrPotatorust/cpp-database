#include <vector>
#include <variant>
#include <string>
#include <ranges>
#include <string_view>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <cwctype>

enum class TokenType
{
    Integer,
    Double,
    String,

    SpecialWord, // Minimum length of 3

    // Operators
    Plus,
    Minus,
    Multiplication,
    Division,
    Modulus,

    End,
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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Passed incorrent argument ./main command" << '\n';
    }

    std::string command = argv[1];

    std::cout << argv[1] << '\n';

    std::vector<std::string> lexemes;

    std::string curLexeme;
    bool inString = false;
    for (char c : command)
    {

        bool isAlnum = isalnum(c) != 0;

        if (isAlnum)
        {
            curLexeme += c;
            continue;
        }

        // This handles special non alnum chars
        switch (c)
        {

        //? Space breaking
        case ' ':
            if (!inString)
            {
                if (!curLexeme.empty())
                    lexemes.push_back(curLexeme);
                curLexeme.clear();
                break;
            }

            curLexeme += c;
            break;
        case '\n':
            if (!inString)
            {
                if (!curLexeme.empty())
                    lexemes.push_back(curLexeme);
                curLexeme.clear();
                break;
            }

            curLexeme += c;
            break;

        //? String identifier
        case '\'':
            inString = !inString;
            if (!curLexeme.empty())
                lexemes.push_back(curLexeme);
            curLexeme.clear();
            break;
        case '\"':
            inString = !inString;
            if (!curLexeme.empty())
                lexemes.push_back(curLexeme);
            curLexeme.clear();
            break;
        case ',':
            if (!inString)
            {
                if (!curLexeme.empty())
                    lexemes.push_back(curLexeme);
                curLexeme.clear();
                lexemes.push_back(",");
            }
            break;

        //? Parentheses
        case '(':
            if (!inString)
            {
                if (!curLexeme.empty())
                    lexemes.push_back(curLexeme);
                curLexeme.clear();
                lexemes.push_back("(");
            }
            break;
        case ')':
            if (!inString)
            {
                if (!curLexeme.empty())
                    lexemes.push_back(curLexeme);
                curLexeme.clear();
                lexemes.push_back(")");
            }
            break;
        default:
            if (inString)
                curLexeme += c;
            break;
        }
    }

    if (!curLexeme.empty())
        lexemes.push_back(curLexeme);

    std::cout << "Lexemes: " << lexemes.size() << '\n';
    for (auto lexeme : lexemes)
    {
        std::cout << '"' << lexeme << '"' << '\n';
    }

    return 0;
}