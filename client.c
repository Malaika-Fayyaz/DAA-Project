#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <gmp.h>

#define SERVER1_PORT 9999
#define SERVER2_PORT 9998
#define SERVER1_IP "20.40.60.119"
#define SERVER2_IP "20.2.218.161"

char prime1[2048] = {0};
char prime2[2048] = {0};

void *connect_to_server(void *arg) {
    int port = *(int *)arg;
    free(arg);

    int sock;
    struct sockaddr_in server;
    char buffer[2048] = {0};
    int recv_size;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return NULL;
    }

    const char *ip = (port == SERVER1_PORT) ? SERVER1_IP : SERVER2_IP;

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
            printf("Received prime from Server 1\n");
        } else if (port == SERVER2_PORT) {
            snprintf(prime2, sizeof(prime2), "%s", buffer);
            printf("Received prime from Server 2\n");
        }
    }

    close(sock);
    return NULL;
}

// Helper: Convert string -> number
void string_to_number(mpz_t result, const char *input) {
    mpz_import(result, strlen(input), 1, sizeof(input[0]), 0, 0, input);
}

// Helper: Convert number -> string
void number_to_string(char *output, mpz_t number) {
    size_t count;
    char *str = (char *)mpz_export(NULL, &count, 1, sizeof(char), 0, 0, number);
    memcpy(output, str, count);
    output[count] = '\0';
    free(str);
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

    printf("\nBoth primes received.\n");

    // Big Integer Setup
    mpz_t p, q, n, phi, e, d;
    mpz_inits(p, q, n, phi, e, d, NULL);

    mpz_set_str(p, prime1, 10);
    mpz_set_str(q, prime2, 10);

    // n = p * q
    mpz_mul(n, p, q);

    // phi = (p-1)*(q-1)
    mpz_t p1, q1;
    mpz_inits(p1, q1, NULL);
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi, p1, q1);

    // e = 65537
    mpz_set_ui(e, 65537);

    // check gcd(e, phi) == 1
    mpz_t gcd;
    mpz_init(gcd);
    mpz_gcd(gcd, e, phi);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        printf("e and phi(n) are not co-prime. Choose different primes.\n");
        return 1;
    }

    // d = e^(-1) mod phi
    if (!mpz_invert(d, e, phi)) {
        printf("Modular inverse for e does not exist.\n");
        return 1;
    }

    // Output the RSA keypair
    printf("\n--- RSA Key Pair ---\n");
    gmp_printf("Public Key: (e = %Zd, n = %Zd)\n", e, n);
    gmp_printf("Private Key: (d = %Zd, n = %Zd)\n", d, n);

    // Encrypt or Decrypt
    char choice[10];
    printf("\nDo you want to encrypt or decrypt? (enter 'encrypt' or 'decrypt') : ");
    scanf("%s", choice);

    if (strcmp(choice, "encrypt") == 0) {
        char input_string[1024];
        printf("Enter the string to encrypt: ");
        getchar(); // clear newline
        fgets(input_string, sizeof(input_string), stdin);

        // Remove trailing newline
        size_t len = strlen(input_string);
        if (input_string[len-1] == '\n') input_string[len-1] = '\0';

        mpz_t message, cipher;
        mpz_inits(message, cipher, NULL);

        string_to_number(message, input_string);

        // Check if message >= n
        if (mpz_cmp(message, n) >= 0) {
            printf("Error: message too large for the current RSA modulus.\n");
            return 1;
        }

        // cipher = message^e mod n
        mpz_powm(cipher, message, e, n);

        printf("\nEncrypted cipher text is:\n");
        gmp_printf("%Zd\n", cipher);

        mpz_clears(message, cipher, NULL);

    }
    else if (strcmp(choice, "decrypt") == 0) {
    char d_str[4096];
    char n_str[4096];
    char cipher_str[4096];

    printf("Enter private key 'd' : ");
    scanf("%s", d_str);

    printf("Enter modulus 'n' : ");
    scanf("%s", n_str);

    printf("Enter the cipher text to decrypt (as a number): ");
    scanf("%s", cipher_str);

    mpz_t d_input, n_input, cipher, decrypted_message;
    mpz_inits(d_input, n_input, cipher, decrypted_message, NULL);

    mpz_set_str(d_input, d_str, 10);
    mpz_set_str(n_input, n_str, 10);
    mpz_set_str(cipher, cipher_str, 10);

    // decrypted_message = (cipher ^ d) mod n
    mpz_powm(decrypted_message, cipher, d_input, n_input);

    char output_string[4096];
    number_to_string(output_string, decrypted_message);

    printf("\nDecrypted message is:\n");
    printf("%s\n", output_string);

    mpz_clears(d_input, n_input, cipher, decrypted_message, NULL);
    }
    else {
        printf("Invalid choice. Please enter 'encrypt' or 'decrypt'.\n");
    }

    mpz_clears(p, q, n, phi, e, d, p1, q1, gcd, NULL);

    return 0;
}
