#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "crypto.h"
#include "connection.h"
#define PORT 2022
#define KEY "aabbccddeeffgghh"
#define INITIALIZATION_VECTOR "0011223344556677"

char* decryptFile(char* encryptionMode, char* cryptKey, char* cryptedFile) {
    char* decryptedFile;
    if (strcmp("ECB", encryptionMode)) {
        decryptedFile = cbc_decrypt(cryptKey, cryptedFile, INITIALIZATION_VECTOR);
    }
    else {
        decryptedFile = ecb_decrypt(cryptKey, cryptedFile);
    }
    return decryptedFile;
}

void* treatClient(void* args) {
    int client = *(int*)args;
    char* encryptionMode = readMessage(client);
    char* cryptedKey = readMessage(client);
    char* cryptKey = aes_decrypt(KEY, cryptedKey);
    sendMessage(client, "1");
    char* cryptedFile = readMessage(client);
    endConnection(client);
    char* decryptedFile = decryptFile(encryptionMode, cryptKey, cryptedFile);
    printf("%s\n", decryptedFile);
}

int main() {
    int socketDescriptor = initializeServer(PORT);
    while (1) {
        printf("Waiting at port %d\n", PORT);
        int client = acceptClient(socketDescriptor);
        printf("Received client at port %d.\n", PORT);
        fflush(stdout);
        pthread_t tid;
        pthread_create(&tid, NULL, &treatClient, &client);
    }
}