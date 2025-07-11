// main.c
#include <stdio.h> // functions for input and output and file handling
#include <stdlib.h> // functions for memory management, process control and conversions
#include <string.h> // functions for manipulating strings
#include <unistd.h> // functions to access the POSIX OS APIs like file descriptor operations and process control
#include <fcntl.h> // functions and macros for file controls like opening and reading
#include <netinet/in.h> // structs and functions for internet operations such as struct sockaddr_in and htons()
#include <sys/socket.h> // socket related functions and definitions socket(), bind(), listen(), accept()
#include <sys/stat.h> // functions and structure for file status information like metadata, size, permission

// macros to use throughout the code 
#define PORT 8080
#define BUFFER_SIZE 4096
#define WEBROOT "./www"

// this handle_client function process and respond to incoming client requests in the HTTP server
void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE]; // array of characters of size 4096
    read(client_fd, buffer, sizeof(buffer) - 1); // reads data from the client socket(client_fd) into the buffer array, only stores data till 4095 bytes as it leaves 1 byte for null terminator(\0) 

    // Simple GET parser
    char method[8], path[1024];
    sscanf(buffer, "%s %s", method, path);

    if (strcmp(method, "GET") != 0) {
        const char *msg = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
        write(client_fd, msg, strlen(msg));
        close(client_fd);
        return;
    }

    // Default file
    if (strcmp(path, "/") == 0) {
        strcpy(path, "/index.html");
    }

    char full_path[2048];
    snprintf(full_path, sizeof(full_path), "%s%s", WEBROOT, path);

    int file_fd = open(full_path, O_RDONLY);
    if (file_fd < 0) {
        const char *msg = "HTTP/1.1 404 Not Found\r\n\r\n404 Not Found";
        write(client_fd, msg, strlen(msg));
    } else {
        const char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        write(client_fd, header, strlen(header));

        char file_buffer[BUFFER_SIZE];
        ssize_t bytes;
        while ((bytes = read(file_fd, file_buffer, BUFFER_SIZE)) > 0) {
            write(client_fd, file_buffer, bytes);
        }
        close(file_fd);
    }

    close(client_fd);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);

    printf("Listening on port %d...\n", PORT);

    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        handle_client(client_fd);
    }

    close(server_fd);
    return 0;
}

