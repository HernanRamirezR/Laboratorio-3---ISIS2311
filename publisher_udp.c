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
    char contenido[TAMANO];
    char *topic;

    struct sockaddr_in server_addr;

    printf("PUBLISHER UDP -- AndeSports\n");

    if (argc < 2) {
        printf("Uso: %s <topic>\n", argv[0]);
        exit(1);
    }

    topic = argv[1];

    // 1. Crear socket UDP
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Configurar dirección del broker
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(DIRECCION);
    server_addr.sin_port = htons(PORT);

    while (1) {

        printf("\nIngrese evento: ");
        fgets(contenido, TAMANO, stdin);

        contenido[strcspn(contenido, "\n")] = 0;

        snprintf(mensaje, TAMANO, "PUBLISH|%s|%s", topic, contenido);

        sendto(sock, mensaje, strlen(mensaje), 0,
               (struct sockaddr*)&server_addr,
               sizeof(server_addr));

        printf("Mensaje enviado\n");
    }

    close(sock);
    return 0;
}