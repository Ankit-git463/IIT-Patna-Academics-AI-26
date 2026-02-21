// client.c (Windows version)
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock\n");
        return 1;
    }

    // Create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (server_addr.sin_addr.s_addr == INADDR_NONE) {
        printf("Invalid address or Address not supported\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Receive data from the server
    int recv_len = recv(sock, buffer, BUFFER_SIZE, 0);
    if (recv_len == SOCKET_ERROR) {
        printf("Receive failed\n");
    } else {
        buffer[recv_len] = '\0'; // Null-terminate the received data
        printf("Received date and time from server: %s", buffer);
    }

    // Close the socket
    closesocket(sock);

    // Cleanup Winsock
    WSACleanup();

    return 0;
}
