#include <stdio.h>      // For standard input/output functions
#include <stdlib.h>     // For standard library functions
#include <string.h>     // For string manipulation functions
#include <winsock2.h>   // For Windows socket functions
#include <ws2tcpip.h>   // For address conversion functions
#include <direct.h>     // For _mkdir() to create directories
#include <io.h>         // For _open() and _write() functions
#include <fcntl.h>      // For file control options
#include <dirent.h>     // For directory operations

// Server configuration
#define PORT 65432  // Port on which the server will listen
#define DIRECTORY "./server_files/"  // Folder containing files to be shared

// Function to list all files in the server's directory
void list_files(SOCKET new_socket) {
    // Open the directory specified in the DIRECTORY macro
    struct _finddata_t fileinfo;
    long handle;
    char file_list[1024] = "";  // Buffer to hold the list of files

    // Prepare the search pattern to find all files in the directory
    handle = _findfirst(DIRECTORY "*.*", &fileinfo);

    // Check if the directory was opened successfully
    if (handle == -1) {
        perror("Could not open directory");
        return;
    }

    // Loop through each entry in the directory
    do {
        // Exclude the current (.) and parent (..) directory entries
        if (fileinfo.name[0] != '.') {
            // Concatenate the file name to the file list
            strcat(file_list, fileinfo.name);
            strcat(file_list, "\n");  // New line after each file name
        }
    } while (_findnext(handle, &fileinfo) == 0);  // Find the next file

    // Send the list of files to the client
    send(new_socket, file_list, strlen(file_list), 0);
    _findclose(handle);  // Close the directory search handle
}

// Function to send a specific file to the client
void send_file(SOCKET new_socket, const char *filename) {
    // Construct the full path of the file
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s%s", DIRECTORY, filename);

    // Open the requested file in read-binary mode
    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        // Send an error message if the file is not found
        const char *error_msg = "ERROR: File not found\n";
        send(new_socket, error_msg, strlen(error_msg), 0);
        return;
    }

    // Read and send the file content in chunks
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(new_socket, buffer, bytes_read, 0);  // Send the chunk to the client
    }

    fclose(file);  // Close the file
}

// Main function to start the server
int main() {
    // Initialize Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a TCP socket
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Define the server address and port
    struct sockaddr_in address;
    address.sin_family = AF_INET;  // IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    address.sin_port = htons(PORT);  // Convert port number to network byte order

    // Bind the socket to the specified address and port
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Start listening for incoming connections
    listen(server_fd, 3);
    printf("Server started. Listening on port %d\n", PORT);

    while (1) {
        // Accept an incoming connection from a client
        SOCKET new_socket = accept(server_fd, NULL, NULL);
        char buffer[1024] = {0};  // Buffer to hold received messages

        // Receive a request from the client
        recv(new_socket, buffer, sizeof(buffer), 0);

        // Handle the client's request based on the received message
        if (strcmp(buffer, "listall") == 0) {
            list_files(new_socket);  // List files if requested
        } else if (strncmp(buffer, "send", 4) == 0) {
            // Extract the filename from the client's request
            const char *filename = buffer + 5;  // Skip "send " (5 characters)
            send_file(new_socket, filename);  // Send the requested file
        }

        // Close the socket for this client
        closesocket(new_socket);
    }

    // Close the server socket
    closesocket(server_fd);
    WSACleanup();  // Clean up Winsock
    return 0;
}
