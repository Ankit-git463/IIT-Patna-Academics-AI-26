#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> // Windows-specific socket API
#include <ws2tcpip.h> // For IP parsing

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

#define BUFFER_SIZE 4096

void error_exit(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char server_ip[100];
    int port;
    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];
    int recv_size;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        error_exit("Failed to initialize Winsock.");
    }

    // Get server IP and port from user
    printf("Enter server IP: ");
    scanf("%s", server_ip);
    printf("Enter server port: ");
    scanf("%d", &port);

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        error_exit("Could not create socket.");
    }
    printf("Socket created.\n");

    // Setup server address struct
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        error_exit("Connection to server failed.");
    }
    printf("Connected to server.\n");

    // Main loop for sending and receiving data
    while (1) {
        printf("> ");
        fgets(send_buffer, BUFFER_SIZE, stdin);

        // If user types "quit", terminate the connection
        if (strncmp(send_buffer, "quit", 4) == 0) {
            printf("Closing connection...\n");
            break;
        }

        // Send user input to server
        if (send(sock, send_buffer, strlen(send_buffer), 0) < 0) {
            error_exit("Failed to send data.");
        }

        // Receive response from server
        if ((recv_size = recv(sock, recv_buffer, BUFFER_SIZE, 0)) == SOCKET_ERROR) {
            error_exit("Failed to receive data.");
        }
        recv_buffer[recv_size] = '\0'; // Null-terminate the received string

        // Print server's response
        printf("%s", recv_buffer);
    }

    // Close the socket and cleanup Winsock
    closesocket(sock);
    WSACleanup();

    return 0;
}
