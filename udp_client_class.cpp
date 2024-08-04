/*
    Author: huyluongme
    UDP Client with class
*/
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080

class UDPClient {
private:
    int client_fd, val_read;
    sockaddr_in server_addr;
    socklen_t server_addr_len;
    const char* hello = "Hello from Client";
    char mess[1024] = {0};

public:
    bool start();
    void communicate();
    void stop();
};

bool UDPClient::start() {
    if((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket failed\n");
        return false;
    }

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    server_addr_len = sizeof(server_addr);
    sendto(client_fd, hello, strlen(hello), MSG_CONFIRM, (sockaddr*)&server_addr, server_addr_len);
    val_read = recvfrom(client_fd, mess, 1024, MSG_WAITALL, (sockaddr*)&server_addr, &server_addr_len);
    mess[val_read] = '\0';
    printf("Server: %s\n", mess);
    return true;
}

void UDPClient::communicate() {
    while(true) {
        printf("Enter your message: ");
        scanf("%[^\n]%*c", mess);
        sendto(client_fd, mess, strlen(mess), MSG_CONFIRM, (sockaddr*)&server_addr, server_addr_len);
        val_read = recvfrom(client_fd, mess, 1024, MSG_WAITALL, (sockaddr*)&server_addr, &server_addr_len);
        mess[val_read] = '\0';
        if(strcmp(mess, "bye") == 0) {
            break;
        }
    }
}

void UDPClient::stop() {
    close(client_fd);
}

int main() {
    std::unique_ptr<UDPClient> client(new UDPClient());
    if(!client->start()) {
        printf("Client failed\n");
        return -1;
    }
    client->communicate();
    client->stop();
    return 0;
}