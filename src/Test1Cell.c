/*
 ************************************************************
 * BIOSYSTEMS COURSE - Algonquin College
 * Code version: Fall, 2026
 * Author: Connor Guenette - 04107333
 * Professors: Paulo Sousa
 ************************************************************
 #
 # ECHO "###########################################"
 # ECHO "#                                         #"
 # ECHO "#                  .+*+.                  #"
 # ECHO "#                .*#####*.                #"
 # ECHO "#               *#########*               #"
 # ECHO "#              +###########*              #"
 # ECHO "#              *####*+*####*              #"
 # ECHO "#               *###. .###*               #"
 # ECHO "#                +##. .##+                #"
 # ECHO "#                 *#####*                 #"
 # ECHO "#               .+#######*.               #"
 # ECHO "#              *###########*              #"
 # ECHO "#             +#############+             #"
 # ECHO "#              *###########*              #"
 # ECHO "#               *#########*               #"
 # ECHO "#                 +#####*                 #"
 # ECHO "#                   *+                    #"
 # ECHO "#                                         #"
 # ECHO "#####     S T E L C O M P I L E R     #####"
 # ECHO "#                                         #"
 # ECHO "########        S T E L L A        ########"
 # ECHO "#                                         #"
 # ECHO "###########################################"
 * ---------------------------------------------------------------------
 * Test1Cell.c
 * ---------------------------------------------------------------------
 * Functional self-test for the Cell module (Step1Cell.c/.h),
 * the biological analog of the original Coder/Decoder stage.
 *
 * Biological role: Cell = Coder/Decoder -> "Convert data into usable
 * information". Here that's modeled as a Vigenere-style genetic cipher:
 * cellEncode() "transcribes" plain data using a key, cellDecode()
 * "translates" it back.
 *
 * NOTE: this is now called as testXxx(argc, argv) from Biology.c's
 * main() dispatcher (options 6-0), not run as its own program --
 * having a main() in every file was the whole problem this fixes
 * (Visual Studio reports "multiple main" if every Test*.c keeps its
 * own). Build/run it as part of the whole project, e.g.:
 *   gcc -std=gnu99 -fcommon -o biology \
 *       Biology.c Main1Cell.c Main2Tissue.c Main3Organ.c Main4System.c \
 *       Main5Body.c Step1Cell.c Step2Tissue.c Step3Organ.c \
 *       Step4System.c Step5Body.c Test1Cell.c Test2Tissue.c \
 *       Test3Organ.c Test4System.c Test5Body.c -lm
 *   ./biology 6   (Cell)   ./biology 7 (Tissue)  ./biology 8 (Organ)
 *   ./biology 9   (System) ./biology 0 (Body)
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Biology.h"
#include "../include/Step1Cell.h"

static int failures = 0;

static void check(const char *label, int condition) {
    printf("[%s] %s\n", condition ? " OK " : "FAIL", label);
    if (!condition) failures++;
}

STEL_int testCell(STEL_int argc, STEL_string* argv) {
    const char *plain_text = "STELCOMPILER sequence ACGT-ACGT-TTAG, 2026 test run!";
    const char *key = "STELCompiler";
    FILE *f;
    STEL_string roundtrip;
    STEL_int plainSize, encodedSize;

    printf("=== Test1Cell: Cell (Coder/Decoder) module ===\n\n");

    /* Write a plain-text sample "cell" to disk */
    f = fopen("t1_plain.txt", "w");
    fputs(plain_text, f);
    fclose(f);

    plainSize = cellFileSize("t1_plain.txt");
    check("cellFileSize reports correct plain file size",
          plainSize == (STEL_int)strlen(plain_text));

    /* Encode ("transcribe") it to disk, then decode ("translate") back */
    cellEncode("t1_plain.txt", "t1_encoded.txt", (STEL_string)key);
    cellDecode("t1_encoded.txt", "t1_decoded.txt", (STEL_string)key);

    encodedSize = cellFileSize("t1_encoded.txt");
    check("encoded file exists and is non-empty", encodedSize > 0);

    /* The encoded bytes should differ from the plain bytes for a
     * non-trivial message (Vigenere transform is not the identity) */
    {
        STEL_string encodedContent = cellReadFile("t1_encoded.txt");
        check("encoded content differs from plain text",
              encodedContent != NULL && strcmp(encodedContent, plain_text) != 0);
        free(encodedContent);
    }

    /* Round trip via files: decode(encode(x)) == x */
    {
        STEL_string decodedContent = cellReadFile("t1_decoded.txt");
        check("file round-trip cellDecode(cellEncode(x)) == x",
              decodedContent != NULL && strcmp(decodedContent, plain_text) == 0);
        free(decodedContent);
    }

    /* Round trip entirely in memory via cellTransformMem, matching how
     * Tissue/Organ/System/Body load their source files */
    roundtrip = cellTransformMem("t1_encoded.txt", (STEL_string)key, TRANSLATE);
    check("in-memory cellTransformMem(..., TRANSLATE) matches original",
          roundtrip != NULL && strcmp(roundtrip, plain_text) == 0);
    free(roundtrip);

    /* A file that doesn't exist should report a clean error, not crash */
    check("cellFileSize on a missing file returns -1",
          cellFileSize("t1_does_not_exist.txt") == -1);

    printf("\n=== %s (%d failure%s) ===\n",
           failures == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED",
           failures, failures == 1 ? "" : "s");

    remove("t1_plain.txt");
    remove("t1_encoded.txt");
    remove("t1_decoded.txt");

    return failures == 0 ? 0 : 1;
}
