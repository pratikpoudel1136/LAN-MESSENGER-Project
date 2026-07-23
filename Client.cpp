#define _WIN32_WINNT 0x0601

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;
    std::string message;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed!\n";
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cout << "Socket creation error\n";
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cout << "Connection failed! Make sure server is running first.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "=== Connected to Server ===\n";
    std::cout << "Type your messages (type 'quit' to exit):\n\n";

    while (true) {
        std::cout << "You: ";
        std::getline(std::cin, message);

        if (message.empty()) continue;

        send(sock, message.c_str(), message.length(), 0);

        if (message == "quit" || message == "Quit") {
            std::cout << "Disconnecting...\n";
            break;
        }
    }

    closesocket(sock);
    WSACleanup();
    std::cout << "Client closed.\n";
    return 0;
}