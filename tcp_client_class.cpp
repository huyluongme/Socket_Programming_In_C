/*
    Author: huyluongme
    TCP Client with class
*/
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

using namespace std;

#define PORT 8080

class TCPClient {
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

bool TCPClient::start() {
    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket is failed\n");
        return false;
    }

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    server_addr_len = sizeof(server_addr);

    if(connect(client_fd, (sockaddr*)&server_addr, server_addr_len) < 0) {
        printf("connect is failed\n");
        return false;
    }

    send(client_fd, hello, strlen(hello), 0);
    val_read = read(client_fd, mess, 1024);
    mess[val_read] = '\0';
    printf("%s\n", mess);
    return true;
}

void TCPClient::communicate() {
    while(true) {
        printf("Enter your message: ");
        scanf("%[^\n]%*c", mess);
        send(client_fd, mess, strlen(mess), 0);
        val_read = read(client_fd, mess, 1024);
        mess[val_read] = '\0';
        if(strcmp(mess, "bye") == 0) break;

        printf("%s\n", mess);
    }
}

void TCPClient::stop() {
    close(client_fd);
}

int main() {
    std::unique_ptr<TCPClient> client(new TCPClient());
    if(!client->start()) {
        printf("Client socket is failed\n");
        exit(EXIT_FAILURE);
    }
    client->communicate();
    client->stop();
    return 0;
}