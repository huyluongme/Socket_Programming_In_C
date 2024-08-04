/*
    Author: huyluongme
    TCP Server with class
*/
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <iostream>
#include <memory>

using namespace std;

#define PORT 8080

class TCPServer {
private:
    int server_fd, opt, new_socket, val_read;
    sockaddr_in server_addr;
    socklen_t server_addr_len;
    const char* hello = "Hello from Server";
    const char* reply_to_client = "Server recieved message from client";
    char mess[1024] = {0};
public:
    bool start();
    void communicate();
    void stop();
};

bool TCPServer::start() {
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket failed\n");
        return false;
    }

    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        printf("setsockopt\n");
        return false;
    }

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    server_addr_len = sizeof(server_addr);


    if(bind(server_fd, (sockaddr*)&server_addr, server_addr_len) < 0) {
        printf("bind failed\n");
        return false;
    }

    if(listen(server_fd, 3) < 0) {
        printf("listen failed\n");
        return false;
    }

    if((new_socket = accept(server_fd, (sockaddr*)&server_addr, &server_addr_len)) < 0) {
        printf("accept failed\n");
        return false;
    }

    val_read = read(new_socket, mess, 1024);
    mess[val_read] = '\0';
    printf("%s\n", mess);
    send(new_socket, hello, strlen(mess), 0);
    return true;
}

void TCPServer::communicate() {
    while (true)
    {
        val_read = read(new_socket, mess, 1024);
        mess[val_read] = '\0';

        if(strcmp(mess, "bye") == 0) {
            send(new_socket, mess, strlen(mess), 0);
            break;
        }

        printf("Message from client: %s\n", mess);
        send(new_socket, reply_to_client, strlen(reply_to_client), 0);
    }
}

void TCPServer::stop() {
    close(new_socket);
    close(server_fd);
}

int main() {
    unique_ptr<TCPServer> server(new TCPServer());
    if(!server->start()) {
        printf("Server socket is failed\n");
        exit(EXIT_FAILURE);
    }

    server->communicate();

    server->stop();
    return 0;
}