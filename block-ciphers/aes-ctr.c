#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>

#define BLOCK_SIZE 16
#define CIPHERTEXT_SIZE 46

void inc_block(unsigned char *block) {
    for (int i = BLOCK_SIZE - 1; i >= 0; ++i) {
        if (block[i] == '\xff') {
            block[i] = '\x00';
        } else {
            block[i] += 1;
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    unsigned char ctr_key[BLOCK_SIZE] = "\x36\xf1\x83\x57\xbe\x4d\xbd\x77\xf0\x50\x51\x5c\x73\xfc\xf9\xf2";
    unsigned char ciphertext[CIPHERTEXT_SIZE] = "\x77\x0b\x80\x25\x9e\xc3\x3b\xeb\x25\x61\x35\x8a\x9f\x2d\xc6\x17\xe4\x62\x18\xc0\xa5\x3c\xbe\xca\x69\x5a\xe4\x5f\xaa\x89\x52\xaa\x0e\x31\x1b\xde\x9d\x4e\x01\x72\x6d\x31\x84\xc3\x44\x51";

    int blocks_amount = sizeof(ciphertext) / BLOCK_SIZE;
    blocks_amount += sizeof(ciphertext) % BLOCK_SIZE ? 1 : 0;
    char plaintext[CIPHERTEXT_SIZE + 1];

    AES_KEY encrypt_key;
    AES_set_encrypt_key(ctr_key, BLOCK_SIZE * 8, &encrypt_key);
    unsigned char decrypted_iv[BLOCK_SIZE];
    
    for (int i = BLOCK_SIZE; i < BLOCK_SIZE * blocks_amount; i += BLOCK_SIZE) {

        AES_encrypt(ciphertext, decrypted_iv, &encrypt_key);

        for (int j = 0; j < BLOCK_SIZE && (i + j) <= CIPHERTEXT_SIZE; ++j)
            plaintext[i + j] = ciphertext[i + j] ^ decrypted_iv[j];
        
        inc_block(ciphertext);
    }

    plaintext[CIPHERTEXT_SIZE] = '\0';
    printf("CTR message: %s\n", plaintext + BLOCK_SIZE);
    return 0;
}