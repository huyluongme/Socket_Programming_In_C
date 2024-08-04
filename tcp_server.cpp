/*
    Author: huyluongmme
    TCP Server
*/
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main() {
    int server_fd, new_socket;
    ssize_t valread;
    sockaddr_in server_addr;
    int opt = 1;
    socklen_t server_addr_len = sizeof(server_addr);
    char buffer[1024] = {0};
    const char* hello = "Hello from server";
    const char* reply = "Server recieved mess";

    // create server socket file descriptor
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    printf("Server socket is created successful\n");

    // reuse address and port
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    if(bind(server_fd, (sockaddr*)&server_addr, server_addr_len) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("bind successful\n");

    if(listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Listening from clients\n");

    if((new_socket = accept(server_fd, (sockaddr*)&server_addr, &server_addr_len)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Accepted\n");
    

    read(new_socket, buffer, 1024 - 1);

    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);

    while (true)
    {   
        char mess[100];
        valread = read(new_socket, mess, 100);
        mess[valread] = '\0';

        if(strcmp(mess, "bye") == 0) {
            send(new_socket, mess, strlen(mess), 0);
            break;
        }

        printf("%s\n", mess);
        send(new_socket, reply, strlen(reply), 0);
    }
    

    close(new_socket);
    close(server_fd);
    return 0;
}