#define _WIN32_WINNT 0x0601

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed!\n";
        return 1;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cout << "Socket creation failed!\n";
        WSACleanup();
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cout << "Bind failed!\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cout << "Listen failed!\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "=== Server Started ===\n";
    std::cout << "Waiting for client on port 8080...\n";

    client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (client_socket == INVALID_SOCKET) {
        std::cout << "Accept failed!\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected successfully!\n\n";

    while (true) {
        int bytes = recv(client_socket, buffer, 1024, 0);
        if (bytes <= 0) {
            std::cout << "\nClient disconnected.\n";
            break;
        }

        buffer[bytes] = '\0';
        std::cout << "Client: " << buffer << std::endl;

        if (strcmp(buffer, "quit") == 0 || strcmp(buffer, "Quit") == 0) {
            std::cout << "Client requested to quit.\n";
            break;
        }
    }

    closesocket(client_socket);
    closesocket(server_fd);
    WSACleanup();
    std::cout << "Server closed.\n";
    return 0;
}