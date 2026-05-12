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

        std::string hello = "Hello from client";
        send(sock, hello.c_str(), hello.size(), 0);
        std::cout << "Hello message sent" << std::endl;
        ssize_t valread = read(sock, buffer, BUFFER_SIZE);
        std::cout << "Received: " << buffer << std::endl;
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
        std::cout << 'Retrying connection' << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(retryTimeoutMs));
    }
}

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }
    // Connect to the server
    if (initializeConnection(sock, serv_addr) < 0)
    {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    eventLoop(sock, buffer);

    // Close the socket
    close(sock);
    return 0;
}