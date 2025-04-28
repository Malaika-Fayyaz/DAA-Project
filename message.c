#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "20.244.3.123"  // Localhost IP address
#define SERVER_PORT 99       // Same port for both client and server

// Function to encrypt the message
void encrypt_message(mpz_t result, mpz_t message, mpz_t e, mpz_t n) {
    mpz_powm(result, message, e, n);  // result = message^e mod n
}

// Function to decrypt the message
void decrypt_message(mpz_t result, mpz_t cipher, mpz_t d, mpz_t n) {
    mpz_powm(result, cipher, d, n);  // result = cipher^d mod n
}

// Function to convert a string to an integer (ASCII sum representation)
void string_to_mpz(mpz_t result, const char *str) {
    size_t len = strlen(str);
    char *ascii_str = malloc(len * 3 + 1);  // To store the ASCII values
    for (size_t i = 0; i < len; i++) {
        sprintf(ascii_str + i * 3, "%03d", str[i]);  // Convert each char to 3 digits
    }
    mpz_set_str(result, ascii_str, 10);
    free(ascii_str);
}

// Function to convert an integer (ASCII sum representation) to a string
void mpz_to_string(char *str, mpz_t num) {
    char *num_str = mpz_get_str(NULL, 10, num);
    size_t len = strlen(num_str);
    for (size_t i = 0; i < len; i += 3) {
        int ascii_value;
        sscanf(num_str + i, "%3d", &ascii_value);
        str[i / 3] = (char)ascii_value;
    }
    str[len / 3] = '\0';
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[2048];

    // RSA key values (public and private keys)
    mpz_t e, d, n, message, cipher, decrypted_message;
    mpz_inits(e, d, n, message, cipher, decrypted_message, NULL);

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        return 1;
    }

    // Connect to the server (same machine, so localhost)
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Receive public key (e, n)
    recv(sockfd, buffer, sizeof(buffer), 0);
    mpz_set_str(e, buffer, 10);
    recv(sockfd, buffer, sizeof(buffer), 0);
    mpz_set_str(n, buffer, 10);

    // Receive private key (d, n)
    recv(sockfd, buffer, sizeof(buffer), 0);
    mpz_set_str(d, buffer, 10);
    
    printf("Received Public Key: e = ");
    gmp_printf("%Zd\n", e);
    printf("Received n = ");
    gmp_printf("%Zd\n", n);
    printf("Received Private Key: d = ");
    gmp_printf("%Zd\n", d);

    // Get message from user
    printf("Enter the message to encrypt: ");
    char msg[1024];
    fgets(msg, sizeof(msg), stdin);

    // Convert message to mpz
    string_to_mpz(message, msg);

    // Encrypt the message using the public key
    encrypt_message(cipher, message, e, n);
    printf("Encrypted Message: ");
    gmp_printf("%Zd\n", cipher);

    // Decrypt the message using the private key
    decrypt_message(decrypted_message, cipher, d, n);
    char decrypted_str[1024];
    mpz_to_string(decrypted_str, decrypted_message);
    printf("Decrypted Message: %s\n", decrypted_str);

    // Close the socket
    close(sockfd);

    mpz_clears(e, d, n, message, cipher, decrypted_message, NULL);
    return 0;
}

