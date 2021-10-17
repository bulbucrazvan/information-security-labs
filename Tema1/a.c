#include <stdio.h>
#include "crypto.h"
#include "connection.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#define KM_PORT 2021
#define B_PORT 2022
#define IP "127.0.0.1"
#define KEY "aabbccddeeffgghh"
#define INITIALIZATION_VECTOR "0011223344556677"

char* getFileContent(char* fileName) {
    FILE *file = fopen(fileName, "r");
    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    char* fileContent = malloc(fileSize + 1);
    rewind(file);
    fread(fileContent, sizeof(char), fileSize, file);
    fileContent[fileSize] = '\0';
    fclose(file);
    return fileContent;
}

char* encryptFile(char* encryptionMode, char* cryptKey, char* plainFile) {
    char* encryptedFile;
    if (strcmp("ECB", encryptionMode)) {
        encryptedFile = cbc_encrypt(cryptKey, plainFile, INITIALIZATION_VECTOR);
    }
    else {
        encryptedFile = ecb_encrypt(cryptKey, plainFile);
    }
    return encryptedFile;
}

int main() {
    printf("Type encryption mode: ");
    char* encryptionMode = malloc(20);
    scanf("%s", encryptionMode);
    int socketDescriptor = initializeConnection(IP, KM_PORT);
    char* receivedKey = readMessage(socketDescriptor);
    endConnection(socketDescriptor);
    socketDescriptor = initializeConnection(IP, B_PORT);
    sendMessage(socketDescriptor, encryptionMode);
    sendMessage(socketDescriptor, receivedKey);
    char* receivedMessage = readMessage(socketDescriptor);
    char* decryptedKey = aes_decrypt(KEY, receivedKey);
    char* cryptedMessage = encryptFile(encryptionMode, decryptedKey, getFileContent("text.txt"));
    sendMessage(socketDescriptor, cryptedMessage);
    endConnection(socketDescriptor);
    printf("Done");
    
}   