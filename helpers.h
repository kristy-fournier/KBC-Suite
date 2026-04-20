#ifndef KES_HELPERS_H
#define KES_HELPERS_H

// safely bit-rotate a short by count to the left
unsigned short rotate_left(unsigned short value, unsigned int count);

// safely bit-rotate a short by count to the right
unsigned short rotate_right(unsigned short value, unsigned int count);

// convert hex char (0-9,A-F) to equivelant byte (0-15)
unsigned char hexToNibble(char c);

// Convert an string of hex pairs to an array of bytes
int hexToCharArr(char hex[], unsigned char dataOut[], long length);

// Print an array of bytes as hex (each with proper leading 0s)
void printHex(unsigned char data[],long long length);

#endif