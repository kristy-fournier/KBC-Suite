#include "kh.h"

unsigned short kbc_hash(unsigned char bytes[],unsigned long long streamSize) {
    unsigned short hash = 0b01101011<<8 | 0b01010101;
    for(unsigned long long i=0;i<streamSize;i++) {
        hash = hash ^ bytes[i];
    }
    return hash;
}