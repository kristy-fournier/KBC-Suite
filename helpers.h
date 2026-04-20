#ifndef KES_HELPERS_H
#define KES_HELPERS_H

// convert hex char (0-9,A-F) to equivelant byte (0-15)
unsigned char hexToNibble(char c);

// Convert an string of hex pairs to an array of bytes
int hexToCharArr(char hex[], unsigned char dataOut[], long length);

// Print an array of bytes as hex (each with proper leading 0s)
void printHex(unsigned char data[],long long length);

#endif