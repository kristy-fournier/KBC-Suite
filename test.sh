make DEBUG=0
echo "This is a top secret message for KBC!" > original.txt
./file.out ./original.txt enc cbc ./middle.kbc "AAAA" "BBBB"
./file.out ./middle.kbc dec cbc ./decrypt.txt "AAAA" "BBBB"

ORIG_HASH=$(sha256sum original.txt | awk '{ print $1 }')
DECR_HASH=$(sha256sum decrypt.txt | awk '{ print $1 }')
if [ "$ORIG_HASH" == "$DECR_HASH" ]; then
    echo "Enc-Dec Test Passed"
else
    echo "Enc-Dec Test Failed"
    exit 1
fi
make clean
rm -f original.txt decrypt.txt middle.kbc