#include "kbc.h"
#include "helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    char* fileName;
    char* fileNameOut;
    unsigned short key;
    unsigned short IV;
    direction dir;
    mode encMode;
    if (argc >= 6) {
        fileName = argv[1];
        fileNameOut = argv[4];
        unsigned char* keyData = (unsigned char*) malloc(sizeof(char)*2);
        hexToCharArr(argv[5],keyData,4);
        key = *(unsigned short*)keyData;
        free(keyData);
        if(argv[2][0] == 'd') {
            dir = MODE_DEC;
        } else {
            dir = MODE_ENC;
        }
        if(strcmp(argv[3],"ecb")==0) {
            encMode = ECB;
        } else if (strcmp(argv[3],"cbc")==0) {
            encMode = CBC;
        } else if (strcmp(argv[3],"ctr")==0) {
            encMode = CTR;
        } else {
            return 1;
        }
        if(argc >=7) {
            IV = *(unsigned short*)argv[6];
        } else {
            IV = 0xB0F0;
        }
    } else {
        printf("Wrong Arguments\n");
        printf("{exe} {fileIn} {dec|enc} {cbc|ctr|ecb} {fileOut} {key} {IV - Optional}\n");
        return 1;
    }
    FILE *fptr = fopen(fileName, "rb");
    if (fptr == NULL) return 1;

    fseek(fptr, 0, SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr); // Move pointer back to the start of the file

    // 2. Allocate memory (+1 for the null terminator '\0')
    unsigned char* buffer = (unsigned char *)malloc(sizeof(char) * (fileSize));
    if (buffer == NULL) {
        fclose(fptr);
        return 1;
    }

    size_t result = fread(buffer, 1, fileSize, fptr);
    printf("Read in original file %s of size %ld\n",fileName,result);
    fclose(fptr);
    unsigned char* out = malloc(sizeof(char)*fileSize);
    switch(encMode) {
        case ECB:
            ecb(buffer,out,key,dir,fileSize);
            break;
        case CBC:
            cbc(buffer,out,key,dir,fileSize,IV);
            break;
        case CTR:
            ctr(buffer,out,key,dir,fileSize,IV);
            break;
    }
    free(buffer);
    
    
    FILE *fptrout = fopen(fileNameOut, "wb");
    if (fptrout == NULL) return 1;
    size_t lengthWrite = fwrite(out,1,fileSize,fptrout);
    fclose(fptrout);
    
    printf("Output to file %s of size %ld\n",fileNameOut,lengthWrite);
    free(out);

    return 0;
}