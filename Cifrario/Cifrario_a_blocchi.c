#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 11  // Dimensioni blocco
#define NUM_ROUNDS 4  // Numero iterazioni

// box per permutazioni (mappa posizione 0 in pos 2, 1 in 4, etc)
unsigned char p_box[BLOCK_SIZE] = {2, 4, 6, 0, 3, 7, 1, 5, 9, 8, 10};

// INVERSO box per permutazioni
unsigned char inv_p_box[BLOCK_SIZE] = {3, 6, 0, 4, 1, 7, 2, 5, 8, 9, 10};

// chiavi (1 per round)
unsigned char keys[NUM_ROUNDS][BLOCK_SIZE] = {
    {0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18},
    {0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10},
    {0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08},
    {0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00}
};

// Permutazioni
void permute(unsigned char *block, unsigned char *box) {
    unsigned char temp[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++) {
        // temp è un appoggio per il blocco permutato
        temp[i] = block[box[i]];          
    }
    // memcpy(destinazione, oggetto, dimensione), scambio circolare veloce
    memcpy(block, temp, BLOCK_SIZE);
}

// XOR tra blocco e chiave
void add_round_key(unsigned char *block, unsigned char *key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        block[i] = block[i] ^ key[i];
    }
}

// Cripta (XOR e permuta)x4
void encrypt_block(unsigned char *block) {
    for (int round = 0; round < NUM_ROUNDS; round++) {
        add_round_key(block, keys[round]);
        permute(block, p_box);
    }
}

// Decripta (permuta, poi XOR)x4
void decrypt_block(unsigned char *block) {
    for (int round = NUM_ROUNDS - 1; round >= 0; round--) {
        permute(block, inv_p_box);
        add_round_key(block, keys[round]);
    }
}

int main() {
    unsigned char plaintext[BLOCK_SIZE] = "Hello world";
    unsigned char ciphertext[BLOCK_SIZE];
    unsigned char decryptedtext[BLOCK_SIZE];

    printf("Plaintext: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%c", plaintext[i]);
    }
    printf("\n");

    // Semplicemente per comodità sposto plaintext in ciphertext, così encrypt restituisce
    // L'output in ciphertext
    memcpy(ciphertext, plaintext, BLOCK_SIZE);
    encrypt_block(ciphertext); 

    printf("Ciphertext: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    // Sposto ciphertext in decryptedtext, dove verrà inserito l'output decifrato
    memcpy(decryptedtext, ciphertext, BLOCK_SIZE);
    decrypt_block(decryptedtext);

    printf("Testo decifrato: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%c", decryptedtext[i]);
    }
    printf("\n");

    return 0;
}