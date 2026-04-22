#include "kh.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Missing file arg:\n{exe} {file to hash}\n");
        return 1;
    }
    char* fileName = argv[1];
    FILE *fptr = fopen(fileName, "rb");
    if (fptr == NULL) {
        printf("Not a file\n");
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr); // Move pointer back to the start of the file

    unsigned char* buffer = (unsigned char *)malloc(sizeof(char) * (fileSize));
    if (buffer == NULL) {
        printf("Could not allocate memory\n");
        fclose(fptr);
        return 1;
    }

    fread(buffer, 1, fileSize, fptr);
    unsigned short hash = kbc_hash(buffer,fileSize);
    printHex((unsigned char*)&hash,2);
    
    free(buffer);
    fclose(fptr);
    return 0;
}