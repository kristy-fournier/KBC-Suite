#include "helpers.h"
#include <stdio.h>
#include <string.h>
unsigned char hexToNibble(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0; // Error handling usually goes here
}


int hexToCharArr(char hex[], unsigned char dataOut[], long length2) {
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