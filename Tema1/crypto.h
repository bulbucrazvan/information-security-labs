#define KEY_BIT_SIZE 128
#define AES_BLOCK_SIZE 16

char* aes_encrypt(char *userKey, char *plainText);

char* aes_decrypt(char *userKey, char *cipherText);

char* ecb_encrypt(char* userKey, char* plainText);

char* ecb_decrypt(char* userKey, char* cipherText);

char* performXOR(char* blockOne, char* blockTwo);

char* cbc_encrypt(char* userKey, char* plainText, char* initializationVector);

char* cbc_decrypt(char* userKey, char* cipherText, char* initializationVector);

char* generateKey();