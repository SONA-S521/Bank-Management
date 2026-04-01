#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    double balance = 1000.0; 
    char *welcome = "Welcome to TCP Online Banking System\n";
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    printf("Server listening on port %d...\n", PORT);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    send(new_socket, welcome, strlen(welcome), 0);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        read(new_socket, buffer, 1024);

        if (strncmp(buffer, "BAL", 3) == 0) {
            char msg[100];
            sprintf(msg, "Your balance: %.2f\n", balance);
            send(new_socket, msg, strlen(msg), 0);
        } 
        else if (strncmp(buffer, "DEP", 3) == 0) {
            double amt = atof(buffer + 4);
            balance += amt;
            send(new_socket, "Deposit successful\n", 20, 0);
        } 
        else if (strncmp(buffer, "WIT", 3) == 0) {
            double amt = atof(buffer + 4);
            if (amt > balance) {
                send(new_socket, "Insufficient funds\n", 20, 0);
            } else {
                balance -= amt;
                send(new_socket, "Withdrawal successful\n", 22, 0);
            }
        } 
        else if (strncmp(buffer, "EXIT", 4) == 0) {
            send(new_socket, "Goodbye!\n", 9, 0);
            break;
        } 
        else {
            send(new_socket, "Invalid command\n", 16, 0);
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

