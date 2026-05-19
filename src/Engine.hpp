#include <string>

struct engineR
{

    bool success;
    std::string message;
};

class Engine
{
public:
    Engine();
    struct engineR get(std::string tableName);

private:
};