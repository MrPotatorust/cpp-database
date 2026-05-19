#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <thread>

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

constexpr std::size_t retryCount = 3;
constexpr std::size_t retryTimeoutMs = 2000;

int eventLoop(int sock, char buffer[])
{
    while (true)
    {

        std::string message;

        std::getline(std::cin, message);

        send(sock, message.c_str(), message.size(), 0);
        ssize_t valread = read(sock, buffer, BUFFER_SIZE);

        if (valread <= 0)
        {
            std::cout << "Client closed connection \n";
            break;
        }

        std::cout << "Received: " << buffer << '\n';
    }

    return 0;
}

int initializeConnection(int sock, struct sockaddr_in serv_addr)
{
    for (std::size_t i = 0; i < retryCount; i++)
    {
        int connectionStatus = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        if (connectionStatus == 0)
        {
            return 0;
        }
        std::cout << "Retrying connection" << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(retryTimeoutMs));
    }

    return -1;
}

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error" << '\n';
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << '\n';
        return -1;
    }
    // Connect to the server
    if (initializeConnection(sock, serv_addr) < 0)
    {
        std::cerr << "Connection Failed" << '\n';
        return -1;
    }
    std::cout << "Connection initalized" << '\n';

    eventLoop(sock, buffer);

    // Close the socket
    close(sock);
    return 0;
}