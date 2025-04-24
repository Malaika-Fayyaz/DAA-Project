#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <gmp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9999
#define PRIME_DIGITS 100

void generate_prime(mpz_t prime) {
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL) ^ getpid());

    do {
        mpz_urandomb(prime, state, PRIME_DIGITS * 3.32193); // bits ≈ digits * log2(10)
        mpz_nextprime(prime, prime);
    } while (mpz_sizeinbase(prime, 10) != PRIME_DIGITS);  // Ensure 100-digit length

    gmp_randclear(state);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server 1 listening on port %d...\n", PORT);

    if ((client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    mpz_t prime;
    mpz_init(prime);
    generate_prime(prime);

    char *prime_str = mpz_get_str(NULL, 10, prime);
    printf("Generated 100-digit prime:\n%s\n", prime_str);

    send(client_socket, prime_str, strlen(prime_str), 0);
    printf("Prime sent to client.\n");

    free(prime_str);
    mpz_clear(prime);
    close(client_socket);
    close(server_fd);

    return 0;
}
