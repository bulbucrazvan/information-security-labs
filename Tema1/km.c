#include <stdio.h>
#include "crypto.h"
#include "connection.h"
#include <pthread.h>
#define PORT 2021
#define KEY "aabbccddeeffgghh"

void* treatClient(void* args);

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

void* treatClient(void* args) {
    int client = *(int*)args;
    char* generatedKey = generateKey();
    char* encryptedKey = aes_encrypt(KEY, generatedKey);
    sendMessage(client, encryptedKey);
    endConnection(client);
}