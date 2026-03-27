#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define DIRECCION "127.0.0.1"
#define TAMANO 256

int main(int argc, char *argv[])
{
    int sock;
    char mensaje[TAMANO];
    char buffer[TAMANO];
    char *topic;

    struct sockaddr_in server_addr, my_addr;
    socklen_t addr_len;

    printf("SUBSCRIBER UDP -- AndeSports\n");

    if (argc < 2) {
        printf("Uso: %s <topic>\n", argv[0]);
        exit(1);
    }

    topic = argv[1];

    // 1. Crear socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Configurar dirección propia (para recibir)
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = 0; // puerto automático

    bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr));

    // 3. Configurar broker
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(DIRECCION);
    server_addr.sin_port = htons(PORT);

    // 4. Enviar SUBSCRIBE
    snprintf(mensaje, TAMANO, "SUBSCRIBE|%s|", topic);

    sendto(sock, mensaje, strlen(mensaje), 0,
           (struct sockaddr*)&server_addr,
           sizeof(server_addr));

    printf("Suscrito a %s\n", topic);
    printf("Esperando eventos...\n");

    while (1) {

        addr_len = sizeof(server_addr);

        int bytes = recvfrom(sock, buffer, TAMANO, 0,
                             (struct sockaddr*)&server_addr, &addr_len);

        if (bytes < 0) {
            perror("recvfrom");
            continue;
        }

        buffer[bytes] = '\0';

        printf("\nEvento: %s\n", buffer);
    }

    close(sock);
    return 0;
}