#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE_DATA 516 // En-tête (4o) + données (512o max)

int main(int argc, char *argv[]) {
    // Vérification du nombre d'arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <host> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char host[20], file[20];
    strncpy(host, argv[1], sizeof(host) - 1); // Copie sécurisée
    host[sizeof(host) - 1] = '\0';
    strncpy(file, argv[3], sizeof(file) - 1); // Copie sécurisée
    file[sizeof(file) - 1] = '\0';

    printf("Host : %s\n", host);
    printf("File : %s\n", file);

    int n_bytes;
    struct addrinfo hints;
    struct addrinfo *res;

    // Initialisation de la structure hints
    memset(&hints, 0, sizeof(struct addrinfo)); // Initialise tout à 0
    hints.ai_socktype = SOCK_DGRAM; // Utilisation de sockets UDP
    hints.ai_family = AF_INET;      // IPv4 uniquement
    hints.ai_protocol = IPPROTO_UDP; // Protocole UDP

    // Résolution de l'adresse
    int en = getaddrinfo(host, "1069", &hints, &res);
    if (en != 0) { // Remplacé -1 par != 0 pour être conforme à la documentation
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(en));
        exit(EXIT_FAILURE);
    }

    // Création de la socket
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        freeaddrinfo(res); // Libération de la mémoire allouée par getaddrinfo
        exit(EXIT_FAILURE);
    }

    // Création de la requête RRQ
    char buf[BUF_SIZE_DATA];
    memset(buf, 0, BUF_SIZE_DATA);
    buf[0] = 0; // Opcode pour RRQ
    buf[1] = 1;
    strcpy(buf + 2, file); // Nom du fichier
    size_t padding = strlen(file);
    strcpy(buf + 2 + padding + 1, "octet"); // Mode de transfert "octet"
    size_t padding2 = strlen("octet");

    // Envoi de la requête RRQ
    if (sendto(sock, buf, 2 + padding + 1 + padding2 + 1, 0, res->ai_addr, res->ai_addrlen) == -1) {
        perror("send RRQ");
        freeaddrinfo(res);
        close(sock);
        exit(EXIT_FAILURE);
    }

    n_bytes = BUF_SIZE_DATA; // Taille initiale pour la réception

    while (n_bytes == BUF_SIZE_DATA) {
        // Réception des données
        if ((n_bytes = recvfrom(sock, buf, BUF_SIZE_DATA, 0, res->ai_addr, &res->ai_addrlen)) == -1) {
            perror("receive DATA");
            freeaddrinfo(res);
            close(sock);
            exit(EXIT_FAILURE);
        }

        // Affichage de l'en-tête
        printf("En-tête : ");
        for (int i = 0; i < 4; i++) {
            printf("%x", buf[i]);
        }
        printf("\nContenu du fichier : ");
        for (int i = 4; i < n_bytes; i++) {
            printf("%c", buf[i]); // Affichage des données en tant que caractères
        }
        printf("\n");

        // Préparation de l'ACK
        buf[0] = 0;
        buf[1] = 4; // Opcode pour ACK

        // Envoi de l'ACK
        if (sendto(sock, buf, 4, 0, res->ai_addr, res->ai_addrlen) == -1) {
            perror("Send ACK");
            freeaddrinfo(res);
            close(sock);
            exit(EXIT_FAILURE);
        }
        printf("ACK envoyé\n");
    }

    // Fermeture de la socket et libération de la mémoire
    close(sock);
    freeaddrinfo(res);

    return EXIT_SUCCESS;
}


