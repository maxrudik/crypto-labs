#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define BLOCK_SIZE 1024

int main(int argc, char *argv[]) {

    FILE *input_file = fopen(argv[1], "rb");
    if (!input_file) {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }

    fseek(input_file, 0, SEEK_END);
    long filesize = ftell(input_file);
    long blocks_amount = filesize / BLOCK_SIZE;

    unsigned char block_sha[SHA256_DIGEST_LENGTH];
    unsigned char block_with_hash[BLOCK_SIZE + SHA256_DIGEST_LENGTH];

    fseek(input_file, blocks_amount * BLOCK_SIZE, SEEK_SET);
    fread(block_with_hash, 1, filesize - ftell(input_file), input_file);
    fseek(input_file, blocks_amount * BLOCK_SIZE, SEEK_SET);
    SHA256(block_with_hash, filesize - ftell(input_file), block_sha);
    fseek(input_file, (blocks_amount - 1) * BLOCK_SIZE, SEEK_SET);
    
    for (int i = 0; i < blocks_amount; i++) {
        fread(block_with_hash, 1, BLOCK_SIZE, input_file);
        memcpy(block_with_hash + BLOCK_SIZE, block_sha, SHA256_DIGEST_LENGTH);
        SHA256(block_with_hash, BLOCK_SIZE + SHA256_DIGEST_LENGTH, block_sha);
        fseek(input_file, -BLOCK_SIZE * 2, SEEK_CUR);
    }

    printf("File h0: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        printf("%02x", block_sha[i]);
    printf("\n");

    return 0;
}