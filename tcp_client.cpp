/*
    Author: huyluongme
    TCP Multi-client - Client
*/

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int client_fd;
    const char* hello = "Hello from client";
    char buffer[1024] = {0};

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error");
        return -1;
    }

    printf("Client socket is created successful\n");

    sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    if(connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed");
        return -1;
    } 

    printf("Connect to Server successful\n");

    send(client_fd, hello, strlen(hello), 0);

    read(client_fd, buffer, 1024 - 1);

    printf("%s\n", buffer);

    while (true)
    {
        char mess[100];
        scanf("%[^\n]%*c", mess);
        send(client_fd, mess, strlen(mess), 0);
        int valread = read(client_fd, mess, 100);
        mess[valread] = '\0';
        if(strcmp(mess, "bye") == 0) break;
        printf("%s\n", mess);
    }
    

    close(client_fd);
    return 0;
}