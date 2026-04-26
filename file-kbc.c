#include "kbc.h"
#include "helpers.h"
#include "kh.h"
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
    if (argc >= 6 && argv[2][0] == 'e') {
        fileName = argv[1];
        fileNameOut = argv[4];
        unsigned char* keyData = (unsigned char*) malloc(sizeof(char)*2);
        hexToCharArr(argv[3],keyData,4);
        key = keyData[0]<<8 | keyData[1];
        free(keyData);
        if(argv[2][0] == 'd') {
            dir = MODE_DEC;
        } else {
            dir = MODE_ENC;
        }
        if(strcmp(argv[5],"ecb")==0) {
            encMode = ECB;
        } else if (strcmp(argv[5],"cbc")==0) {
            encMode = CBC;
        } else if (strcmp(argv[5],"ctr")==0) {
            encMode = CTR;
        } else {
            return 1;
        }
        if(argc >=7) {
            IV = *(unsigned short*)argv[6];
        } else {
            IV = 0xB0F0;
        }
    } else if (argc==5) {
        fileName = argv[1];
        fileNameOut = argv[4];
        dir = MODE_DEC;
        unsigned char keyData[2] = {0};
        hexToCharArr(argv[3],keyData,4);
        key = keyData[0]<<8 | keyData[1];
    } else if (argc > 2){
        if ((argv[2][0] == 'd')) {
            printf("Wrong Arguments\n");
            printf("{exe} {fileIn} {dec} {key} {fileOut}\n");
        } else {
            printf("Wrong Arguments\n");
            printf("{exe} {fileIn} {enc} {key} {fileOut} {cbc|ctr|ecb} {IV - Optional}\n");
        }
        return 1;
    } else {
        printf("Wrong Arguments, for more help run:\n");
        printf("{exe} {fileIn} {dec|enc}\n");
        return 1;
    }
    FILE *fptr = fopen(fileName, "rb");
    if (fptr == NULL) return 1;

    fseek(fptr, 0, SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr); // Move pointer back to the start of the file

    // 2. Allocate memory
    unsigned char* buffer;
    if (fileSize%2==0) {
        buffer = (unsigned char *)malloc(sizeof(char) * (fileSize));
    } else {
        buffer = (unsigned char *)malloc(sizeof(char) * (fileSize+1));
        buffer[fileSize] = 0;
    }
    if (buffer == NULL) {
        fclose(fptr);
        return 1;
    }

    size_t result = fread(buffer, 1, fileSize, fptr);

    KBCHeader header={.tag="KBC1"};
    if (dir == MODE_ENC) {
        // header creation
        char* ext = strrchr(fileName, '.');
        memset(header.extension,0,sizeof(header.extension));
        strncpy(header.extension,ext,8);
        header.iv = IV;
        header.fileSize = fileSize;
        header.cipherMode = (unsigned char)encMode;
        header.hash = 0;
        kbc_headerPrint(&header);
    } else {
        memcpy(&header, buffer, sizeof(KBCHeader));
        buffer = buffer + sizeof(KBCHeader);
        kbc_headerPrint(&header);
        if(memcmp(header.tag,"KBC1",4)!= 0) {
            printf("\033[1;31mERROR\033[1;0m:Bad Block Cipher Version Number, try again\n");
            return 1;
        }
        if(kbc_hash(buffer,(header.fileSize%2==1) ? header.fileSize +1 : header.fileSize)!=header.hash) {
            printf("\033[1;31mERROR\033[1;0m: Hashes do not match, Data may be corrupted\n");
            return 1;
        }
        if(strcmp(strrchr(fileNameOut, '.'),header.extension)!=0) {
            printf("\033[1;31mWARNING\033[1;0m: file extension in header does not match given output file, proceeding anyway\n");
        }
        fileSize = header.fileSize;
        IV = header.iv;
        // fileNameOut = malloc(sizeof(char)*sizeof(argv[3]));
        encMode = header.cipherMode;

    }

    printf("Read in original file %s of size %ld\n",fileName,result);
    fclose(fptr);
    unsigned char* out;
    if(dir == MODE_DEC || fileSize%2==0) {
        out = malloc(sizeof(char)*fileSize);
    } else {
        out = malloc(sizeof(char)*fileSize+1);
        out[fileSize] = 0;
    }
    switch(encMode) {
        // unsigned char* bufferNew = buffer + sizeof(KBCHeader);
        // long fileSizeNew = fileSize - sizeof(KBCHeader);
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
    if(dir == MODE_DEC) {
        free(buffer-sizeof(KBCHeader));
    } else {
        free(buffer);
        header.hash = kbc_hash(out,fileSize);
    }
    
    
    FILE *fptrout = fopen(fileNameOut, "wb");
    if (fptrout == NULL) return 1;
    size_t lengthWrite = 0;
    if (dir == MODE_ENC) {
        lengthWrite += fwrite((unsigned char*)&header,1,sizeof(header),fptrout);
    }
    lengthWrite += fwrite(out,1,fileSize,fptrout);
    fclose(fptrout);
    
    printf("Output to file %s of size %ld\n",fileNameOut,lengthWrite);
    free(out);

    return 0;
}