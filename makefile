compile:
	g++ server.cpp ./src/DBTypes.cpp ./src/Helpers.cpp ./src/QueryBuilder.cpp ./src/Lexer.cpp ./src/Database.cpp ./src/Engine.cpp ./src/Parser.cpp -o server -std=c++23 -Wall -Wextra -Wpedantic -std=c++23 -Wall -Wextra -Wconversion -Wpedantic
	g++ client.cpp -o client -std=c++23 -Wall -Wextra -Wpedantic -std=c++23 -Wall -Wextra -Wconversion -Wpedantic
