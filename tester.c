#include "kbc.h"
#include "kh.h"
#include "helpers.h"
#include <stdio.h>
#include <string.h>

int old_main(int argc, char *argv[]) {
    char* msg = 0;
    char* keyin = 0;
    direction direction = MODE_ENC;
    mode cypherMode = CBC;
    
    // printf("args: %d\n\n",argc);
    if (argc >= 4) {
        msg = argv[1];
        keyin = argv[2];
        if(argv[3][0] == 'e') {
            direction = MODE_ENC;
        } else if(argv[3][0] == 'd'){
            direction = MODE_DEC;
        } else {
            printf("3rd argument (%s) invalid, must be \"enc\" or \"dec\"\n",argv[3]);
        }
    } else if (argc >= 3) {
        printf("Missing Args, run --help for more info\n");
        return 1;
    } else if (argc >= 2) {
        if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0) {
            printf("Usage:\n");
            printf("cipher.out {msg} {key} {enc|dec}\n");
            return 1;
        } else {
           printf("Missing Args, run --help for more info\n");
           return 1;
        }
    } else {
        printf("Missing Args, run --help for more info\n");
        return 1;
    }
    unsigned char keyBinOut[2];
    hexToCharArr(keyin,keyBinOut,4);
    unsigned short key = *(unsigned short*)keyBinOut;
    unsigned int length = strlen(msg);
    if (direction == MODE_DEC) {
        length = (length / 2);
    } else {
        length++;
    }
    // length at this point is equal to the size of the original array
    unsigned char outMsg[length];
    unsigned char data[length];
    unsigned short IV = 5678;
    if(direction == MODE_DEC) {
        hexToCharArr(msg,data,length*2);
        switch(cypherMode) {
            case ECB:
                ecb(data,outMsg,key,MODE_DEC,length);
                break;
            case CBC:
                cbc(data,outMsg,key,MODE_DEC,length,IV);
                break;
            case CTR:
                ctr(data,outMsg,key,MODE_DEC,length,IV);
                break;
        }
        outMsg[length] = '\0';
        printf("Decrypt: %s\n",outMsg);
    } else {
        switch(cypherMode) {
            case ECB:
                ecb((unsigned char*)msg,outMsg,key,MODE_ENC,length);
                break;
            case CBC:
                cbc((unsigned char*)msg,outMsg,key,MODE_ENC,length,IV);
                break;
            case CTR:
                ctr((unsigned char*)msg,outMsg,key,MODE_ENC,length,IV);
                break;
        }
        printf("Encrypted Data: ");
        printHex(outMsg,length);
    }
    return 0;
}
#include <stdlib.h>
int main() {
    FILE* x = fopen("README.md","rb");
    unsigned char* y = malloc(sizeof(char)*101);
    fread(y,1,100,x);
    unsigned short hash = kbc_hash(y,100);
    printHex((unsigned char*)&hash,2);
    // while(1) {
    //     unsigned short result = kbc_hash((unsigned char*)&i,8);
    //     if (result == 32000) {
    //         printf("Found hash %04X at i=%lld\n",result,i);
    //         return 0;
    //     }
    //     i++;
    //     if(i == 0) {
    //         return 1;
    //     }
    // }
    fclose(x);
    free(y);
    return 0;
}