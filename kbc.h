#ifndef BLOCKCIPHER_H
#define BLOCKCIPHER_H

// Encrypt a single block
unsigned short enc(unsigned short m, unsigned short k);

// Decrypt a single block
unsigned short dec(unsigned short c, unsigned short k);

// Encode a byte array in ECB Mode
int ecb(unsigned char messageIn[], unsigned char messageOut[], unsigned short k, unsigned char mode, long long byteCount);

// Encode a byte array in CBC Mode
int cbc(unsigned char messageIn[], unsigned char messageOut[], unsigned short k, unsigned char mode, long long byteCount, unsigned short IV);

// Encode a byte array in CTR Mode
int ctr(unsigned char messageIn[], unsigned char messageOut[], unsigned short k, unsigned char mode, long long byteCount, unsigned char nonce);


#endif