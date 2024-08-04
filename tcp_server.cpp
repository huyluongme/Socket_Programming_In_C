#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

using namespace std;

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0), newSocket;
    int opt = 1;
    ssize_t valread;
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";

    if(serverSocket < 0) {
        perror("create socket failed");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("socketopt");
        exit(EXIT_FAILURE);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    if(bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(serverSocket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if((newSocket = accept(serverSocket, (sockaddr*)&serverAddress, (socklen_t *)(sizeof(serverAddress)))) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while(true) {
        valread = read(newSocket, buffer, 1024 - 1);
        printf("%s\n", buffer);
        send(newSocket, hello, strlen(hello), 0);
    }

    close(newSocket);
    close(serverSocket);
    return 0;
}