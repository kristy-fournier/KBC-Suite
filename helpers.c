#include "helpers.h"
#include <stdio.h>
#include <string.h>

// gemini wrote
unsigned char hexToNibble(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0; // Error handling usually goes here
}

// unsigned short rotate_left(unsigned short value, unsigned int count) {
//     // Mask count to avoid undefined behavior if count >= 16
//     count %= 16; 
//     return (value << count) | (value >> (16 - count));
// }

// unsigned short rotate_right(unsigned short value, unsigned int count) {
//     count %= 16;
//     return (value >> count) | (value << (16 - count));
// }

// end gemini wrote

int hexToCharArr(char hex[], unsigned char dataOut[], long length2) {
    (void)length2; // will remove this at some point, just avoiding a warning for now
    long length = strlen(hex);
    if(length%2 != 0) {
        return -1;
    }
    for(long i = 0;i<length;i+=2) {
        // printf("loop is running\n");
        char temp = 0;
        temp = (hexToNibble(hex[i])<<4) | hexToNibble(hex[i+1]);
        dataOut[i/2] = temp;
    }
    return 0;
}

void printHex(unsigned char data[],long long length) {
    for(long long i = 0;i<length;i++) {
        printf("%02X",data[i]);
    }
    printf("\n");
}

void kbc_headerPrint(KBCHeader* headerIn) {
    printf("HeaderType: %.*s\n",4,headerIn->magic);
    printf("FileExt:    %.*s\n",8,headerIn->extension);
    printf("IV:         %04X\n",headerIn->iv);
    printf("Length:     %lld\n",headerIn->fileSize);
}