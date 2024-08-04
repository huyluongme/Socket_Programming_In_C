/*
    Author: huyluongme
    UDP Server
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include <cstring>

using namespace std;

#define PORT 8080

int main() {
    int server_fd;
    sockaddr_in server_addr, client_addr;
    socklen_t server_addr_len = sizeof(server_addr), client_addr_len = sizeof(client_addr);
    char mess[1024] = {0};
    const char* hello = "Hello from Server";

    if((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(bind(server_fd, (sockaddr*)&server_addr, server_addr_len) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    int val_read = recvfrom(server_fd, (char*)mess, 1024, MSG_WAITALL, (sockaddr*)&client_addr, &client_addr_len);
    mess[val_read] = '\0';
    printf("Client: %s\n", mess);
    sendto(server_fd, (const char*)hello, strlen(hello), MSG_CONFIRM, (sockaddr*)&client_addr, client_addr_len);

    while(true) {
        val_read = recvfrom(server_fd, (char*)mess, 1024, MSG_WAITALL, (sockaddr*)&client_addr, &client_addr_len);
        mess[val_read] = '\0';

        printf("Client: %s\n", mess);
        sendto(server_fd, mess, sizeof(mess), MSG_CONFIRM, (sockaddr*)&client_addr, client_addr_len);
        if(strcmp(mess, "bye") == 0) 
            break;
    }

    close(server_fd);
    return 0;
}