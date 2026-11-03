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
 * Test3Organ.c
 * ---------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Biology.h"
#include "../include/Step1Cell.h"
#include "../include/Step2Tissue.h"
#include "../include/Step3Organ.h"	

static int failures = 0;
static int checks = 0;

/*
 * Function: check()
 * Compares the actual test result with the expected result.
 * Prints the outcome of the test and updates the counters.
 */
static void check(const char* label, int condition) {
    ++checks;

    printf("[%s] %s\n", condition ? " OK " : "FAIL", label);

    if (!condition)
        ++failures;
}


/*
 * Function: testFirstToken()
 * Creates a temporary buffer, scans its first token and compares it with the expected signal code.
 */
static void testFirstToken(const char* label, const char* input, STEL_int expected) {
    TissuePointer buffer;
    Signal result;
    STEL_int i;
    STEL_int success = 1;

    // Create a temporary Tissue buffer
    buffer = tissueCreate(128);

    if (buffer == NULL) {
        check(label, 0);
        return;
    }

    // Copy the test input into the buffer, character by character
    for (i = 0; input[i] != '\0'; i++) {
        if (tissueAddCell(buffer, input[i]) == NULL) {
            success = 0;
            break;
        }
    }

    // Add the end-of-source character
    if (success && tissueAddCell(buffer, TISSUE_TERMINATOR) == NULL) {
        success = 0;
    }

    // Initialize the scanner with the temporary buffer
    if (success && startOrgan(buffer) == 0) {

        // Scan the first token
        result = organScan();

        // Compare the recognized signal code
        success = (result.code == expected);

        if (!success) {
            printf("Expected: %d, Received: %d\n", expected, result.code);
        }
    }
    else {
        success = 0;
    }

    // Record the test result
    check(label, success);

    // Free the temporary buffer
    tissueFree(buffer);
}


/*
 * Function: testOrgan()
 * Performs automated tests on the Stella lexical scanner.
 * Tests keywords, identifiers, literals, operators and errors.
 * Returns EXIT_SUCCESS or EXIT_FAILURE based on the test results.
 */
STEL_int testOrgan(STEL_int argc, STEL_string* argv) {
    (void)argc;
    (void)argv;

    failures = 0;
    checks = 0;

    printf("=== Test3Organ: Stella DFA scanner ===\n\n");

    check("Load Stella DFA", loadScannerDFA("txtFiles/compiler_scanner_dfa.txt") > 0);

    if (failures)
        return EXIT_FAILURE;

    stringLiteralTable = tissueCreate(TISSUE_DEFAULT_SIZE);

    check("Create string literal table",
        stringLiteralTable != NULL);

    if (!stringLiteralTable)
        return EXIT_FAILURE;

    testFirstToken("Keyword: STEL_int", "STEL_int ", KW_S);
    testFirstToken("Game keyword: damage", "damage ", KW_S);

    testFirstToken("Integer identifier: score$", "score$ ", IVID_S);
    testFirstToken("Float identifier: health%", "health% ", FVID_S);
    testFirstToken("String identifier: player@", "player@ ", SVID_S);
    testFirstToken("Method identifier: main&", "main& ", MNID_S);

    testFirstToken("Integer literal: 123", "123;", INL_S);
    testFirstToken("Float literal: 12.75", "12.75;", FPL_S);
    testFirstToken("String literal", "\"Hello World!\";", STR_S);
    testFirstToken("Comment", "# comment # ", CMT_S);

    testFirstToken("Assignment operator", "= ", ASS_OP_S);
    testFirstToken("Equality operator", "== ", REL_OP_S);
    testFirstToken("Left brace", "{ ", LBR_S);

    testFirstToken("Dice literal: 2d6", "2d6;", DICE_S);
    testFirstToken("Dice literal: 1d20", "1d20;", DICE_S);

    testFirstToken("Invalid unsuffixed word", "unknownWord ", ERR_S);
    testFirstToken("Invalid leading underscore", "_invalid$ ", ERR_S);
    testFirstToken("Invalid dice separator", "2x6;", ERR_S);
    testFirstToken("Integer overflow", "999999;", ERR_S);
    testFirstToken("Invalid decimal: 12.", "12.;", ERR_S);

    tissueFree(stringLiteralTable);
    stringLiteralTable = NULL;

    printf("\n=== %s (%d/%d checks passed) ===\n",
        failures == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED",
        checks - failures, checks);

    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}