#include "kbc.h"
#include "helpers.h"
#include "kh.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    char* fileName = NULL;
    char* fileNameOut = NULL;
    bool keySet = false;
    unsigned short key;
    bool IVSet = false;
    unsigned short IV;
    direction dir = 2;
    mode encMode = NONE;

    // replacing old args system
    static struct option long_options[] = {
        {"in",      required_argument, 0, 'i'},
        {"out",     required_argument, 0, 'o'},
        {"key",     required_argument, 0, 'k'},
        {"iv",      required_argument, 0, 'v'},
        {"mode",    required_argument, 0, 'm'}, // ecb,cbc,ctr
        {"encrypt",       no_argument, 0, 'e'},
        {"decrypt",       no_argument, 0, 'd'},
        {"help",          no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "i:o:k:v:m:edh", long_options, NULL)) != -1) {
        switch(opt){
            case 'i':
                fileName = optarg;
                break;
            case 'o':
                fileNameOut = optarg;
                break;
            case 'k':
                unsigned char keyData[2];
                hexToCharArr(optarg,keyData,4);
                key = keyData[0]<<8 | keyData[1];
                keySet = true;
                break;
            case 'v':
                IV = optarg[0]<<8 | optarg[1];
                IVSet = true;
                break;
            case 'm':
                if(strcmp(optarg,"ecb")==0) {
                    encMode = ECB;
                } else if (strcmp(optarg,"cbc")==0) {
                    encMode = CBC;
                } else if (strcmp(optarg,"ctr")==0) {
                    encMode = CTR;
                } else {
                    return 1;
                }
                break;
            case 'e':
                dir = MODE_ENC;
                break;
            case 'd':
                dir = MODE_DEC;
                break;
            case 'h':
                printf("{exe} {-e/-d} -k {keyInHex} -v {IV} -i {inFile} -o {outFile} -m {ecb|cbc|ctr}\n");
                printf("- - - - - - -\n");
                printf("Usage:\n");
                printf("--in | -i : (REQUIRED) Provided with a file to show the intended file to encrypt\n");
                printf("\n");
                return 0;
        }
    }

    // checking all data is set correctly from args

    // Basically anything directly wrong
    if (fileName == NULL || !keySet || dir == 2) {
        printf("Bad args, run --help for more info\n");
        return 1;
    }

    if (dir == MODE_ENC && encMode == NONE) {
        printf("Bad args, run --help for more info\n");
        return 1;
    }
    
    // Set a generic output file
    if (fileNameOut == NULL) {
        if(dir == MODE_ENC) {
            fileNameOut = "out.kbc";
        } else {
            // Otherwise we'll set it later
            // for now this is an error
            printf("Give an output file (with -o)\n");
            return 1;
        }
    }

    if(!IVSet) {
        IV = 0xB0F0;
    }

    // new args over (for now)

    FILE *fptr = fopen(fileName, "rb");
    if (fptr == NULL) {
        printf("File Not Found Error\n");
        return 1;
    }
    fseek(fptr, 0, SEEK_END);
    long fileSize = ftell(fptr);
    long paddedSize = fileSize%2==0 ? fileSize : fileSize + 1;
    rewind(fptr); // Move pointer back to the start of the file

    // 2. Allocate memory
    unsigned char* buffer = (unsigned char *)malloc(sizeof(char) * (paddedSize));
    buffer[paddedSize-1] = 0;
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
        unsigned short inHash;
        inHash = kbc_hash(buffer,header.fileSize);
        if(inHash!=header.hash) {
            printf("Hash Found: %04X\n",inHash);
            printf("Hash Seek:  %04X\n",header.hash);
            printf("\033[1;31mERROR\033[1;0m: Hashes do not match, Data may be corrupted\n");
            return 1;
        }
        if(strcmp(strrchr(fileNameOut, '.'),header.extension)!=0) {
            printf("\033[1;31mWARNING\033[1;0m: file extension in header does not match given output file, proceeding anyway\n");
        }
        fileSize = header.fileSize;
        paddedSize = (header.fileSize%2)==0 ? header.fileSize : header.fileSize +1;
        IV = header.iv;
        // fileNameOut = malloc(sizeof(char)*sizeof(argv[3]));
        encMode = header.cipherMode;
    }

    printf("Read in original file %s of size %ld\n",fileName,result);
    fclose(fptr);
    unsigned char* out = malloc(sizeof(char)*paddedSize);
    out[paddedSize-1] = 0;
    switch(encMode) {
        // unsigned char* bufferNew = buffer + sizeof(KBCHeader);
        // long fileSizeNew = fileSize - sizeof(KBCHeader);
        case ECB:
            ecb(buffer,out,key,dir,paddedSize);
            break;
        case CBC:
            cbc(buffer,out,key,dir,paddedSize,IV);
            break;
        case CTR:
            ctr(buffer,out,key,dir,paddedSize,IV);
            break;
        case NONE:
            printf("What?\n");
            return 1;
    }
    if(dir == MODE_DEC) {
        free(buffer-sizeof(KBCHeader));
    } else {
        free(buffer);
        header.hash = kbc_hash(out,fileSize);
    }
    if (dir == MODE_ENC) {
        kbc_headerPrint(&header);
    }
    FILE *fptrout = fopen(fileNameOut, "wb");
    if (fptrout == NULL) return 1;
    size_t lengthWrite = 0;
    if (dir == MODE_ENC) {
        lengthWrite += fwrite((unsigned char*)&header,1,sizeof(header),fptrout);
        lengthWrite += fwrite(out,1,paddedSize,fptrout);
    } else {
        lengthWrite += fwrite(out,1,header.fileSize,fptrout);
    }
    fclose(fptrout);
    
    printf("Output to file %s of size %ld\n",fileNameOut,lengthWrite);
    free(out);

    return 0;
}