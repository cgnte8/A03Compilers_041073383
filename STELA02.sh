#
# Run this script to build and test the biology program for encoding and decoding text files.
# This will only run on WSL
#

set -e
cd "$(dirname "$0")"
echo "Building Stella Compiler ..."
gcc -std=gnu99 -fcommon -w -o biology \
    src/Biology.c \
    src/Main1Cell.c \
    src/Step1Cell.c \
    src/Step2Tissue.c \
    src/Main2Tissue.c \
    -lm

echo
echo "=== [1] Cell: Encode STEL_CODE1.txt -> CODED.TXT ==="
./biology 1 1 txtFiles/STEL_CODE1.txt txtFiles/CODED.txt

echo
echo "=== [2] Tissue: Decode A02_CODED.txt -> A02_DECODED.txt ==="
./biology 2 txtFiles/CODED.txt txtFiles/A02_DECODED.txt 2> txtFiles/A02_ERRORS.err

echo
