#ifndef KES_HELPERS_H
#define KES_HELPERS_H

// Const Defenitions
#define MODE_ENC 0
#define MODE_DEC 1

typedef unsigned char direction;

extern const char* modesLookup[];

typedef enum mode: unsigned char {
    ECB,CBC,CTR,NONE
} mode;

// safely bit-rotate a short by count to the left
static inline unsigned short rotate_left(unsigned short value, unsigned int count) {
    count &= 0xF; // Faster way to do % 16
    return (value << count) | (value >> (16 - count));
}
// safely bit-rotate a short by count to the right
static inline unsigned short rotate_right(unsigned short value, unsigned int count) {
    count &= 0xF; // Faster way to do % 16
    return (value >> count) | (value << (16 - count));
}

#pragma pack(push,1)
typedef struct {
    char tag[4];                 // "KBCx" where x is version num
    unsigned char cipherMode;    // matching the above mode enum (ecb,cbc,ctr = 0,1,2)
    unsigned short hash;         // hash of the data
    char extension[8];           // ".xyz\0\0\0\0" | Original extension including '.' | allows up to 7 chars of extension
    unsigned short iv;           // The IV used for CBC, or first byte for CTR
    unsigned long long fileSize; // Original size before padding
} KBCHeader;
#pragma pack(pop)

void kbc_headerPrint(KBCHeader* headerIn);

// convert hex char (0-9,A-F) to equivelant byte (0-15)
unsigned char hexToNibble(char c);

// Convert an string of hex pairs to an array of bytes
int hexToCharArr(char hex[], unsigned char dataOut[], long length);

// Print an array of bytes as hex (each with proper leading 0s)
void printHex(unsigned char data[],long long length);

#endif