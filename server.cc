#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <array>

using namespace std;

int main()
{
    // creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    if (!bind(serverSocket, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)))
    {
        perror("bind");
        return 1;
    };

    // listening to the assigned socket
    if (!listen(serverSocket, 5))
    {
        perror("listen");
        return 1;
    };

    // accepting connection request
    int clientSocket = accept(serverSocket, nullptr, nullptr);

    // recieving data
    array<char, 1024> buffer{};

    send(clientSocket, buffer, 1024);

    // cout << "Message from client: " << buffer
    //      << endl;
    // recv(clientSocket, buffer, sizeof(buffer), 0);

    // closing the socket.
    close(serverSocket);

    return 0;
}