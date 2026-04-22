#!/bin/bash
# KH Test Script

# Set colors for better UX
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

make clean
make DEBUG=0
echo "This is Hash TEST!" > hashTest1.txt
echo "This is Hash TEST!" > hashTest2.txt

HASH_1=$(./file-kh.out hashTest1.txt | awk '{ print $1 }')
HASH_2=$(./file-kh.out hashTest2.txt | awk '{ print $1 }')
if [ "$HASH_1" == "$HASH_2" ]; then
    echo -e "Hash Test ${GREEN}Passed"
else
    echo -e "Hash Test ${RED}Failed"
    exit 1
fi
echo "hash1:${HASH_1}"
echo "hash2:${HASH_2}"
echo -en "${NC}"
make clean
rm -f hashTest1.txt hashTest2.txt