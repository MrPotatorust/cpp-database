#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <array>
#include <string>

using namespace std;

int main()
{
    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        perror("socket");
        return 1;
    }

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    int connected = connect(clientSocket, (struct sockaddr *)&serverAddress,
                            sizeof(serverAddress));

    if (connected < 0)
    {
        perror("connect");
        close(clientSocket);
        return 1;
    }

    string serverResponse;
    array<char, 4096> buffer{};

    for (;;)
    {
        ssize_t bytesReceived = recv(clientSocket, buffer.data(), buffer.size(), 0);

        if (bytesReceived > 0)
        {
            serverResponse.append(buffer.data(), static_cast<size_t>(bytesReceived));

            // Process line-delimitedmessages from the stream.
            size_t pos = 0;
            while ((pos = serverResponse.find('\n')) != string::npos)
            {
                string oneMessage = serverResponse.substr(0, pos);
                cout << "Server: " << oneMessage << '\n';
                serverResponse.erase(0, pos + 1);
            }
        }
        else if (bytesReceived == 0)
        {
            cout << "Server disconnected\n";
            break;
        }
        else
        {
            if (errno == EINTR)
            {
                continue;
            }
            perror("recv");
            break;
        }

        // send(clientSocket, message, strlen(message), 0);
    }

    // closing socket
    close(clientSocket);

    return 0;
}