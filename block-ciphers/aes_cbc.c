#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>

#define BLOCK_SIZE 16
#define KEY_SIZE 16
#define CIPHERTEXT_SIZE 64

int main(int argc, char *argv[]) {
    unsigned char cbc_key[KEY_SIZE] = "\x14\x0b\x41\xb2\x2a\x29\xbe\xb4\x06\x1b\xda\x66\xb6\x74\x7e\x14";
    unsigned char ciphertext[CIPHERTEXT_SIZE] = "\x5b\x68\x62\x9f\xeb\x86\x06\xf9\xa6\x66\x76\x70\xb7\x5b\x38\xa5\xb4\x83\x2d\x0f\x26\xe1\xab\x7d\xa3\x32\x49\xde\x7d\x4a\xfc\x48\xe7\x13\xac\x64\x6a\xce\x36\xe8\x72\xad\x5f\xb8\xa5\x12\x42\x8a\x6e\x21\x36\x4b\x0c\x37\x4d\xf4\x55\x03\x47\x3c\x52\x42\xa2\x53";

    int blocks_amount = sizeof(ciphertext) / BLOCK_SIZE;
    char plaintext_padded[BLOCK_SIZE * (blocks_amount - 1)];

    AES_KEY decrypt_key;
    AES_set_decrypt_key(cbc_key, BLOCK_SIZE * 8, &decrypt_key);
    
    for (int i = BLOCK_SIZE * (blocks_amount - 1); i >= BLOCK_SIZE; i -= BLOCK_SIZE) {

        AES_decrypt(ciphertext + i, plaintext_padded + i, &decrypt_key);

        for (int j = 0; j < BLOCK_SIZE; ++j)
            plaintext_padded[i + j] = plaintext_padded[i + j] ^ ciphertext[i - BLOCK_SIZE + j];
    }

    int padding_size = plaintext_padded[CIPHERTEXT_SIZE - 1];
    plaintext_padded[CIPHERTEXT_SIZE - padding_size] = '\0';

    printf("CBC message: %s\n", plaintext_padded + BLOCK_SIZE);
    return 0;
}