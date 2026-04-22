#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "kbc.h"
#include "helpers.h"

unsigned short enc(unsigned short m, unsigned short k) {
    unsigned short multiplier = (k * 9 ^ 0b10110100) % 16;
    for (int i = (multiplier)-1;i>=0;i--) {
        m = rotate_left(m,i+9);
        m = m ^ k;
        m = rotate_right(m,i-12);
        m = m ^ rotate_right(k,i);
    }
    m = m ^ 0b1010101010101010;
    return m;
}

unsigned short dec(unsigned short c, unsigned short k) {
    unsigned short multiplier = (k * 9 ^ 0b10110100) % 16;
    c = c ^ 0b1010101010101010;
    for (int i = 0;i<(multiplier);i++) {
        c = c ^ rotate_right(k,i);
        c = rotate_left(c,i-12);
        c = c ^ k;
        c = rotate_right(c,i+9);
    }
    return c;
   
}

int ecb(unsigned char messageIn[], unsigned char messageOut[], unsigned short k, unsigned char mode, long long byteCount) {
    for(long long i = 0; i<byteCount;i+=2) {
        unsigned short temp = (messageIn[i]<<8);
        if(i!=byteCount-1) {
            temp = temp | messageIn[i+1];
        }
        unsigned short result;
        if (mode == MODE_DEC) {
            result = dec(temp,k);
        } else if (mode == MODE_ENC) {
            result = enc(temp,k);
        } else {
            return 1;
        }
        messageOut[i] = result>>8;
        if(i!=byteCount){
            messageOut[i+1] = result & 0b11111111;
        }
    }
    // messageOut[byteCount-1] = '\0';
    return 0;
}

int cbc(unsigned char messageIn[], unsigned char messageOut[], unsigned short k, unsigned char mode, long long byteCount, unsigned short IV) {
    // unsigned short IV = 1234;
    for(long long i = 0; i<byteCount;i+=2) {
        unsigned short temp = (messageIn[i]<<8);
        if(i!=byteCount-1) {
            temp = temp | messageIn[i+1];
        }
        if (mode == MODE_ENC) {
            if (i == 0) {
                temp = temp ^ IV;
            } else {
                temp = temp ^ (messageOut[i-2]<<8 | messageOut[i-1]);
            }
        }
        unsigned short result;
        if (mode == MODE_DEC) {
            result = dec(temp,k);
        } else if (mode == MODE_ENC) {
            result = enc(temp,k);
        } else {
            return 1;
        }
        if (mode == MODE_DEC) {
            if (i == 0) {
                result = result ^ IV;
            } else {
                result = result ^ (messageIn[i-2]<<8 | messageIn[i-1]);
            }
        }
        messageOut[i] = result>>8;
        if(i!=byteCount){
            messageOut[i+1] = result & 0b11111111;
        }
    }
    // messageOut[byteCount-1] = '\0';
    return 0;
}

int ctr(unsigned char messageIn[], unsigned char messageOut[], unsigned short k, unsigned char mode, long long byteCount, unsigned char nonce) {
    (void)mode; // Avoids a compiler warning about not using.
    unsigned short counter = nonce << 8;
    for(long long i = 0; i<byteCount;i+=2) {
        unsigned short temp = (messageIn[i]<<8);
        if(i!=byteCount-1) {
            temp = temp | messageIn[i+1];
        }
        unsigned short result;
        result = enc(counter,k);
        result = result ^ temp;
        messageOut[i] = result>>8;
        if(i<byteCount){
            messageOut[i+1] = result & 0b11111111;
        }
        counter++;
    }
    return 0;
}

// Should be strlen(msg) == 2;
void testSingleBlock(char* msg) {
    unsigned short* msgShort = (unsigned short*) msg;
    const unsigned short plaintext = *msgShort;
    // const unsigned short plaintext = 0xf149;
    const unsigned short key = 0x0110;

    unsigned short enc_out = enc(plaintext,key);

    unsigned short plain2 = dec(enc_out,key);

    printf("Original Plaintext: %X\n",plaintext);
    printf("Encrypted: %X\n",enc_out);
    printf("New Plain: %X\n",plain2);
}