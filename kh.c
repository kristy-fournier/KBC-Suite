#include "kh.h"
#include "helpers.h"

unsigned short kbc_hash(unsigned char bytes[],unsigned long long streamSize) {
    unsigned short hash = 0b01101011<<8 | 0b01010101;
    for(unsigned long long j=0;j<streamSize;j++) {
        unsigned long long i = j%streamSize;
        hash = hash ^ bytes[i]<<8;
        hash = hash ^ bytes[i]>>8;
        hash = hash * 23;
        rotate_left(hash,3);
        hash = hash * (bytes[i]+17);
        rotate_right(hash,5);
        hash = hash * (bytes[i]+21);
        hash = hash * 17;
        rotate_left(hash,7);
        hash = hash * 101;
        hash += 1;
        if(i%9==0) {
            hash = hash>>14 | hash | hash<<14;
            hash = hash * hash;
        }
    }
    hash +=1;
    return hash;
}