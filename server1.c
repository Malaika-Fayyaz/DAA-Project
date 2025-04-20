#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9999

bool miller_rabin(int n, int k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    int d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    for (int i = 0; i < k; i++) {
        int a = 2 + rand() % (n - 4);
        int x = 1;
        int temp = d;

        long long res = 1, base = a;
        while (temp > 0) {
            if (temp % 2 == 1)
                res = (res * base) % n;
            base = (base * base) % n;
            temp /= 2;
        }
        x = (int)res;

        if (x == 1 || x == n - 1) continue;

        bool continue_outer = false;
        for (int r = 1; r <= log2(n - 1); r++) {
            x = (int)(((long long)x * x) % n);
            if (x == n - 1) {
                continue_outer = true;
                break;
            }
        }
        if (continue_outer) continue;

        return false;
    }

    return true;
}

int generate_prime() {
    srand(time(NULL) ^ getpid());
    int prime;
    while (1) {
        prime = rand() % (999999 - 10000 + 1) + 10000;
        if (prime % 2 == 0) prime++;
        if (miller_rabin(prime, 5))
            break;
    }
    return prime;
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

    int prime = generate_prime();
    printf("Server 1 generated prime: %d\n", prime);

    char prime_str[12];
    sprintf(prime_str, "%d", prime);
    send(client_socket, prime_str, strlen(prime_str), 0);

    printf("Prime sent to client.\n");

    close(client_socket);
    close(server_fd);
    return 0;
}
