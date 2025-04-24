#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERVER1_PORT 9999
#define SERVER2_PORT 9998
#define SERVER1_IP "20.40.60.119"
#define SERVER2_IP "20.2.218.161"

char prime1[1024] = {0};
char prime2[1024] = {0};

void *connect_to_server(void *arg) {
    int port = *(int *)arg;
    free(arg);

    int sock;
    struct sockaddr_in server;
    char buffer[1024] = {0};
    int recv_size;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return NULL;
    }

    const char *ip;
    if (port == SERVER1_PORT) {
        ip = SERVER1_IP;
    } else if (port == SERVER2_PORT) {
        ip = SERVER2_IP;
    } else {
        fprintf(stderr, "Unknown port: %d\n", port);
        close(sock);
        return NULL;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address for %s\n", ip);
        close(sock);
        return NULL;
    }

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        close(sock);
        return NULL;
    }

    printf("Connected to server at %s:%d\n", ip, port);

    recv_size = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (recv_size < 0) {
        perror("Receive failed");
    } else {
        buffer[recv_size] = '\0';
        if (port == SERVER1_PORT) {
            snprintf(prime1, sizeof(prime1), "%s", buffer);
            printf("Received prime from Server 1: %s\n", prime1);
        } else if (port == SERVER2_PORT) {
            snprintf(prime2, sizeof(prime2), "%s", buffer);
            printf("Received prime from Server 2: %s\n", prime2);
        }
    }

    close(sock);
    return NULL;
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long long mod_inverse(long long e, long long phi) {
    long long t = 0, new_t = 1;
    long long r = phi, new_r = e;

    while (new_r != 0) {
        long long quotient = r / new_r;
        long long temp_t = t;
        t = new_t;
        new_t = temp_t - quotient * new_t;

        long long temp_r = r;
        r = new_r;
        new_r = temp_r - quotient * new_r;
    }

    if (r > 1) return -1;
    if (t < 0) t += phi;
    return t;
}

int main() {
    pthread_t thread1, thread2;
    int *port1 = malloc(sizeof(int));
    int *port2 = malloc(sizeof(int));
    *port1 = SERVER1_PORT;
    *port2 = SERVER2_PORT;

    pthread_create(&thread1, NULL, connect_to_server, port1);
    pthread_create(&thread2, NULL, connect_to_server, port2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("\nBoth primes received.\nPrime1: %s\nPrime2: %s\n", prime1, prime2);

    unsigned long long p = strtoull(prime1, NULL, 10);
    unsigned long long q = strtoull(prime2, NULL, 10);

    if (p == 0 || q == 0) {
        printf("Invalid prime values received.\n");
        return 1;
    }

    unsigned long long n = p * q;
    unsigned long long phi = (p - 1) * (q - 1);
    unsigned long long e = 65537;

    if (gcd(e, phi) != 1) {
        printf("e and phi(n) are not co-prime. Choose different primes.\n");
        return 1;
    }

    long long d = mod_inverse(e, phi);
    if (d == -1) {
        printf("Modular inverse for e does not exist.\n");
        return 1;
    }

    printf("\n--- RSA Key Pair ---\n");
    printf("Public Key: (e = %llu, n = %llu)\n", e, n);
    printf("Private Key: (d = %lld, n = %llu)\n", d, n);

    return 0;
}
