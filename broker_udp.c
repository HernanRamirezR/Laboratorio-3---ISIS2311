#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define TAMANO 256
#define MAX_SUBS 20

typedef struct {
    struct sockaddr_in addr;
    char topic[50];
} Subscriber;

Subscriber subs[MAX_SUBS];
int subs_count = 0;

int main() {

    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;

    char buffer[TAMANO];

    printf("BROKER UDP INICIADO --- AndeSports\n");

    // 1. Crear socket UDP
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    // 2. Configurar dirección
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 3. Bind
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    printf("Escuchando en puerto %d...\n", PORT);

    while (1) {

        addr_len = sizeof(client_addr);

        // 4. Recibir mensaje
        int bytes = recvfrom(sock, buffer, TAMANO, 0,
                             (struct sockaddr*)&client_addr, &addr_len);

        if (bytes < 0) {
            perror("recvfrom");
            continue;
        }

        buffer[bytes] = '\0';

        printf("\nMensaje recibido: %s\n", buffer);

        // ===== PARSING =====
        char *tipo = strtok(buffer, "|");
        char *topic = strtok(NULL, "|");
        char *contenido = strtok(NULL, "|");

        // ===== SUBSCRIBE =====
        if (strcmp(tipo, "SUBSCRIBE") == 0) {

            subs[subs_count].addr = client_addr;
            strcpy(subs[subs_count].topic, topic);
            subs_count++;

            printf("Subscriber agregado a %s\n", topic);
        }

        // ===== PUBLISH =====
        if (strcmp(tipo, "PUBLISH") == 0) {

            printf("Publicando en %s: %s\n", topic, contenido);

            for (int i = 0; i < subs_count; i++) {

                if (strcmp(subs[i].topic, topic) == 0) {

                    sendto(sock, contenido, strlen(contenido), 0,
                           (struct sockaddr*)&subs[i].addr,
                           sizeof(subs[i].addr));
                }
            }
        }
    }

    close(sock);
    return 0;
}