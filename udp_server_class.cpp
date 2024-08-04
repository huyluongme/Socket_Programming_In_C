/*
    Author: huyluongme
    UDP Server with class
*/
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <memory>

#define PORT 8080

class UDPServer {
private:
    int server_fd, val_read;
    sockaddr_in server_addr, client_addr;
    socklen_t server_addr_len, client_addr_len;
    const char* hello = "Hello from Server";
    char mess[1024];

public:
    bool start();
    void communicate();
    void stop();
};

bool UDPServer::start() {
    if((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket failed\n");
        return false;
    }

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    server_addr_len = sizeof(server_addr);
    client_addr_len = sizeof(client_addr);

    if(bind(server_fd, (sockaddr*)&server_addr, server_addr_len) < 0) {
        printf("bind falied\n");
        return false;
    }

    val_read = recvfrom(server_fd, mess, 1024, MSG_WAITALL, (sockaddr*)&client_addr, &client_addr_len);
    mess[val_read] = '\0';
    printf("Client: %s\n", mess);

    sendto(server_fd, hello, strlen(hello), MSG_CONFIRM, (sockaddr*)&client_addr, client_addr_len);
    return true;
}

void UDPServer::communicate() {
    while(true) {
        val_read = recvfrom(server_fd, mess, 1024, MSG_WAITALL, (sockaddr*)&client_addr, &client_addr_len);
        mess[val_read] = '\0';
        printf("Client: %s\n", mess);
        sendto(server_fd, mess, strlen(mess), MSG_CONFIRM, (sockaddr*)&client_addr, client_addr_len);
        if(strcmp(mess, "bye") == 0) break;
    }
}

void UDPServer::stop() {
    close(server_fd);
}

int main() {
    std::unique_ptr<UDPServer> server(new UDPServer());
    if(!server->start()) {
        printf("Server failed\n");
        return -1;
    }
    server->communicate();
    server->stop();
    return 0;
}