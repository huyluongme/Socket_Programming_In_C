#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

struct ClientData {
    int client_socket;
    int client_id;
};

void* handle_client(void* arg) {
    ClientData* client_data = (ClientData*)arg;
    int sock = client_data->client_socket;
    int client_id = client_data->client_id;
    int valread;
    char buffer[BUFFER_SIZE];

    while (true) {
        valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            std::cout << "Client " << client_id << " disconnected" << std::endl;
            close(sock);
            free(client_data);
            pthread_exit(NULL);
        }
        buffer[valread] = '\0';
        std::cout << "Client " << client_id << ": " << buffer << std::endl;
        send(sock, buffer, strlen(buffer), 0);
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int client_id = 0;
    int opt = 1;

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        client_id++;
        std::cout << "New connection, client ID " << client_id << ", socket fd is " << new_socket << ", ip is: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << std::endl;

        pthread_t thread_id;
        ClientData* client_data = (ClientData*)malloc(sizeof(ClientData));
        client_data->client_socket = new_socket;
        client_data->client_id = client_id;

        if (pthread_create(&thread_id, NULL, handle_client, (void*)client_data) < 0) {
            perror("could not create thread");
            free(client_data);
            close(new_socket);
        }
    }

    return 0;
}
