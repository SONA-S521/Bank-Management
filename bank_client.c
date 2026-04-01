#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char input[100];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    read(sock, buffer, 1024);
    printf("%s", buffer);
    while (1) {
        printf("\nEnter command (BAL, DEP <amt>, WIT <amt>, EXIT): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; 
        send(sock, input, strlen(input), 0);

        if (strncmp(input, "EXIT", 4) == 0)
            break;

        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, 1024);
        printf("Server: %s\n", buffer);
    }

    close(sock);
    return 0;
}

