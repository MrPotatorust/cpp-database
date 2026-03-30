compile:
	g++ server.cc -o server -std=c++23 -Wall -Wextra -Wpedantic -O2 -g
	g++ client.cc -o client -std=c++23 -Wall -Wextra -Wpedantic -O2 -g