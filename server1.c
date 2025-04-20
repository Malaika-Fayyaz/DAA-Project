#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <winsock2.h>
#include <windows.h>
#include <math.h>

#define PORT 9999  // Port for the first server



// Miller-Rabin Primality Test (for 32-bit integers)
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

        // Compute a^d % n
        long long res = 1;
        long long base = a;
        while (temp > 0) {
            if (temp % 2 == 1)
                res = (res * base) % n;
            base = (base * base) % n;
            temp /= 2;
        }
        x = (int)res;

        if (x == 1 || x == n - 1)
            continue;

        bool continue_outer = false;
        for (int r = 1; r <= log2(n - 1); r++) {
            x = (int)(((long long)x * x) % n);
            if (x == n - 1) {
                continue_outer = true;
                break;
            }
        }

        if (continue_outer)
            continue;

        return false;
    }

    return true;
}


int generate_prime() {
    srand(time(NULL) ^ GetCurrentProcessId());
    int prime;
    while (1) {
        prime = rand() % (999999 - 10000 + 1) + 10000;
        if (prime % 2 == 0) prime++; // Ensure odd number
        if (miller_rabin(prime, 5)) // 5 rounds for reasonable certainty
            break;
    }
    return prime;
}




int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(client_addr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server 1 listening on port %d...\n", PORT);

    // Accept incoming connection
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) == INVALID_SOCKET) {
        printf("Accept failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Generate and send the prime number
    int prime = generate_prime();
    printf("Server 1 generated prime: %d\n", prime);

    char prime_str[12];
    sprintf(prime_str, "%d", prime);
    send(client_socket, prime_str, strlen(prime_str), 0);

    printf("Prime sent to client.\n");

    // Cleanup
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
