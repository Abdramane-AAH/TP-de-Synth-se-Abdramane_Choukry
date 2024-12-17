#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int main(int argc, char *argv[]) {
    char host[20], file[20];
if (argc < 3) {
        fprintf(stderr, "Usage: %s <host> <file>\n", argv[0]);
        return 1;
    }
strncpy(host, argv[1], sizeof(host) - 1);
    host[sizeof(host) - 1] = '\0';

strncpy(file, argv[2], sizeof(file) - 1);
    file[sizeof(file) - 1] = '\0';

    struct addrinfo hints;
    struct addrinfo *res;

memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET;
    hints.ai_protocol = 0;

    int en = getaddrinfo(host, "1069", &hints, &res);
    if (en == -1) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }
 printf("Host : %s\n", host);
    printf("File : %s\n", file);
    printf("Adresse du serveur récupérée avec succès.\n");

    freeaddrinfo(res);

    return 0;
}