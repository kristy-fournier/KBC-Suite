# Kristy Block Cipher (KBC)

## Usage and building

- Run `make` or `make build`\
### file-kbc
Encrypt a file using the mode and key you like
`./file-kbc.out [filein] [enc|dec] [key] [fileout] [cbc|ctr|ecb] [iv]`\
In the above the `key` is 4 hex digits including leading 0s. Ex: `0012` `F023` `aaaa` are valid keys\
The `iv` isn't super useful in the current version, it gets the raw bytes of the string and casts to a short or a char.

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