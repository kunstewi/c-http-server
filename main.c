// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define WEBROOT "./www"

void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    read(client_fd, buffer, sizeof(buffer) - 1);

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

