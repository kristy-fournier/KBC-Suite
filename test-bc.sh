#!/bin/bash
# KBC Test Script

# Set colors for better UX
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

make clean
make DEBUG=0
echo "This is a top secret message for KBC!" > original.txt
./file-kbc.out ./original.txt enc cbc ./middle.kbc "AAAA" "BBBB"
./file-kbc.out ./middle.kbc dec cbc ./decrypt.txt "AAAA" "BBBB"

ORIG_HASH=$(sha256sum original.txt | awk '{ print $1 }')
DECR_HASH=$(sha256sum decrypt.txt | awk '{ print $1 }')
if [ "$ORIG_HASH" == "$DECR_HASH" ]; then
    echo -e "Enc-Dec Test ${GREEN}Passed"
else
    echo -e "Enc-Dec Test ${RED}Failed"
    exit 1
fi
echo -en "${NC}"
make clean
rm -f original.txt decrypt.txt middle.kbc