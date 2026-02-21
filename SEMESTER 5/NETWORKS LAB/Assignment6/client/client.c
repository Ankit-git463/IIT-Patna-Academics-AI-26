#include <stdio.h>      // For standard input/output functions
#include <stdlib.h>     // For standard library functions
#include <string.h>     // For string manipulation functions
#include <winsock2.h>   // For Windows socket functions
#include <ws2tcpip.h>   // For address conversion functions
#include <direct.h>     // For _mkdir() to create directories
#include <fcntl.h>      // For file control options
#include <io.h>         // For _open() and _write() functions

// Client configuration
#define PORT 65432  // The port used by the server
#define HOST "127.0.0.1"  // Server's IP address (localhost)
#define DIRECTORY "./client_files/"  // Folder to save downloaded files

// Function to request the list of available files from the server
void request_files() {
    // Initialize Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a TCP socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // Define the server address and port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;  // IPv4
    server_address.sin_port = htons(PORT);  // Convert port number to network byte order
    
    // Convert IP address from text to binary format using inet_addr
    server_address.sin_addr.s_addr = inet_addr(HOST);  // Use inet_addr instead of inet_pton

    // Connect to the server
    connect(sock, (struct sockaddr *)&server_address, sizeof(server_address));

    // Send request to list all files
    send(sock, "listall", strlen("listall"), 0);
    
    // Buffer to hold the response from the server
    char buffer[1024] = {0};
    
    // Receive the list of files from the server
    recv(sock, buffer, sizeof(buffer), 0);
    
    // Print the available files
    printf("Available files:\n%s", buffer);

    // Close the socket
    closesocket(sock);
    WSACleanup();  // Clean up Winsock
}

// Function to request and download a specific file from the server
void download_file(const char *filename) {
    // Initialize Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a TCP socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // Define the server address and port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;  // IPv4
    server_address.sin_port = htons(PORT);  // Convert port number to network byte order
    
    // Convert IP address from text to binary format using inet_addr
    server_address.sin_addr.s_addr = inet_addr(HOST);  // Use inet_addr instead of inet_pton

    // Connect to the server
    connect(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    
    // Send the request for the specific file
    char request[256];
    snprintf(request, sizeof(request), "send %s", filename);
    send(sock, request, strlen(request), 0);
    
    // Buffer to hold the response from the server
    char buffer[1024] = {0};
    
    // Receive the file data from the server
    int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
    
    // Check if the server sent an error message (e.g., file not found)
    if (bytes_received > 0 && strstr(buffer, "ERROR") != NULL) {
        printf("File not found on the server.\n");
    } else {
        // If the file is found, save it to the client_files directory
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "%s%s", DIRECTORY, filename);
        
        // Open file in write-binary mode
        FILE *file = fopen(filepath, "wb");
        if (file != NULL) {
            fwrite(buffer, 1, bytes_received, file);  // Write the received data to the file
            printf("File '%s' downloaded successfully.\n", filename);
            fclose(file);  // Close the file
        } else {
            printf("Error creating file on the client.\n");
        }
    }

    // Close the socket
    closesocket(sock);
    WSACleanup();  // Clean up Winsock
}

// Main function to execute the client
int main() {
    // Ensure the directory to save files exists; if not, create it
    if (_mkdir(DIRECTORY) == -1) {
        printf("Directory already exists or error creating directory.\n");
    }

    // Loop to accept commands from the user
    while (1) {
        char command[256]; 
         // Buffer to hold user commands
        print();
        printf(">> ");  // Prompt for user input
        fgets(command, sizeof(command), stdin);  // Read user input

        // Remove the newline character from the input
        command[strcspn(command, "\n")] = 0;

        // Handle different commands from the user
        if (strcmp(command, "listall") == 0) {
            request_files();  // Request the list of files from the server
        } else if (strncmp(command, "send", 4) == 0) {
            // Extract the filename from the command
            const char *filename = command + 5;  // Skip "send " (5 characters)
            download_file(filename);  // Request to download the specified file
        } else {
            // Handle unrecognized commands
            printf("Unknown command\n");
        }
    }

    return 0;  // Return success
}
