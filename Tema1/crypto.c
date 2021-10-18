#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/aes.h>
#include "crypto.h"

char* aes_encrypt(char *userKey, char *plainText) {
    AES_KEY encryptionKey;
    AES_set_encrypt_key(userKey, KEY_BIT_SIZE, &encryptionKey);
    char* cipherText = malloc(AES_BLOCK_SIZE);
    AES_encrypt(plainText, cipherText, &encryptionKey);
    return cipherText;
}

char* aes_decrypt(char *userKey, char *cipherText) {
    AES_KEY decryptionKey;
    AES_set_decrypt_key(userKey, KEY_BIT_SIZE, &decryptionKey);
    char* decryptedText = malloc(AES_BLOCK_SIZE);
    AES_decrypt(cipherText, decryptedText, &decryptionKey);
    return decryptedText;
}

char* ecb_encrypt(char* userKey, char* plainText) {
    int textLength = strlen(plainText);
    char* cipherText = malloc(textLength + 1);
    int fullBlocks = textLength / AES_BLOCK_SIZE, lastBlock = (textLength % AES_BLOCK_SIZE > 0);
    char* cipherBlock = malloc(AES_BLOCK_SIZE);
    for (int i = 0; i < (fullBlocks + lastBlock); i++) {
        cipherBlock = aes_encrypt(userKey, (plainText + AES_BLOCK_SIZE * i));
        strcat(cipherText, cipherBlock);
    }
    return cipherText;
}

char* ecb_decrypt(char* userKey, char* cipherText) {
    int textLength = strlen(cipherText);
    char* decryptedText = malloc(textLength + 1);
    int fullBlocks = textLength / AES_BLOCK_SIZE, lastBlock = (textLength % AES_BLOCK_SIZE > 0);
    char* decryptedBlock = malloc(AES_BLOCK_SIZE);
    for (int i = 0; i < (fullBlocks + lastBlock); i++) {
        decryptedBlock = aes_decrypt(userKey, (cipherText + AES_BLOCK_SIZE * i));
        strcat(decryptedText, decryptedBlock);
    } 
    return decryptedText;
}


char* performXOR(char* blockOne, char* blockTwo) {
    char *result = malloc(AES_BLOCK_SIZE);
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        result[i] = blockOne[i] ^ blockTwo[i];
    }
    return result;
}

char* cbc_encrypt(char* userKey, char* plainText, char* initializationVector){
    int textLength = strlen(plainText);
    char* cipherText = malloc(textLength + 1);
    int fullBlocks = textLength / AES_BLOCK_SIZE, lastBlock = (textLength % AES_BLOCK_SIZE > 0);
    char *cipherBlock = malloc(AES_BLOCK_SIZE), *XORBlock = malloc(AES_BLOCK_SIZE), *cryptBlock;
    strncpy(XORBlock, initializationVector, AES_BLOCK_SIZE);
    for (int i = 0; i < (fullBlocks + lastBlock); i++) {
        cryptBlock = performXOR((plainText + AES_BLOCK_SIZE * i), XORBlock);
        cipherBlock = aes_encrypt(userKey, cryptBlock);
        strcat(cipherText, cipherBlock);
        strncpy(XORBlock, cipherBlock, AES_BLOCK_SIZE);
    }
    return cipherText;
}

char* cbc_decrypt(char* userKey, char* cipherText, char* initializationVector) {
    int textLength = strlen(cipherText);
    char* decryptedText = malloc(textLength + 1);
    int fullBlocks = textLength / AES_BLOCK_SIZE, lastBlock = (textLength % AES_BLOCK_SIZE > 0);
    char* decryptedBlock, *XORBlock = malloc(AES_BLOCK_SIZE), *cryptBlock = malloc(AES_BLOCK_SIZE);
    strncpy(XORBlock, initializationVector, AES_BLOCK_SIZE);
    for (int i = 0; i < (fullBlocks + lastBlock); i++) {
        cryptBlock = aes_decrypt(userKey, (cipherText + AES_BLOCK_SIZE * i));
        decryptedBlock = performXOR(cryptBlock, XORBlock);
        strncpy(XORBlock, (cipherText + AES_BLOCK_SIZE * i), AES_BLOCK_SIZE);
        strcat(decryptedText, decryptedBlock);
    } 
    return decryptedText;
}

char* generateKey() {
    char* generatedKey = malloc(AES_BLOCK_SIZE);
    RAND_bytes(generatedKey, AES_BLOCK_SIZE);
    return generatedKey;
}
