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
    int sock_fd;
    sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    char mess[1024] = {0};
    const char* hello = "Hello from Client";

    if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    sendto(sock_fd, (const char*)hello, strlen(hello), MSG_CONFIRM, (sockaddr*)&server_addr, server_addr_len);
    int val_read = recvfrom(sock_fd, (char*)mess, 1024, MSG_WAITALL, (sockaddr*)&server_addr, &server_addr_len);
    mess[val_read] = '\0';
    printf("Server: %s\n", mess);

    while(true) {
        printf("Enter your message: ");
        scanf("%[^\n]%*c", mess);
        sendto(sock_fd, (const char*)mess, strlen(mess), MSG_CONFIRM, (sockaddr*)&server_addr, server_addr_len);
        int val_read = recvfrom(sock_fd, (char*)mess, 1024, MSG_WAITALL, (sockaddr*)&server_addr, &server_addr_len);
        mess[val_read] = '\0';
        if(strcmp(mess, "bye") == 0) 
            break;
    }

    close(sock_fd);
    return 0;
}