# Kristy Block Cipher (KBC)

## Usage and building

- Run `make` or `make build`
### file-kbc
Encrypt a file using the mode and key you like\
`./file-kbc.out [-e/-d] -i [filein] -k [hexKey] -o [fileout] -m [cbc|ctr|ecb] -v [ivstring]`\
In the above the `key` is 4 hex digits including leading 0s. Ex: `0012` `F023` `aaaa` are valid keys\
The `iv` isn't super useful in the current version, it gets the raw bytes of the string and casts to a short or a char. IV must be at least 4 digits 

### file-kh
Hash a file
`./file-kh.out [filein]`
outputs 4 hex digits representing the 2 short hash bytes

## kbc.c
A toy-cipher with a 16-bit block size and 16-bit key (unsigned short)
Has implementations for ECB, CBC, and CTR modes of use. These modes take in a bytestream (represented as an unsigned char array) and encrypt it to another unsigned char array.
The input and output arrays should be the same size, and divisible by 2.

## kh.c (Unfinished)
Hash function, not yet complete

## Table for understanding header versions

KBC binary headers use a format to identify themselves in the first 4 bytes. In chars, the first 4 bytes should always be in the format of `KBCx`. If for some reason at any point more than 9 numbers are needed, `KCxx` might be used. I will attempt to explain it here

|Header | Version |
|-------|---------|
|`KBC1` | v3.x.x (expected v4.x.x)  |