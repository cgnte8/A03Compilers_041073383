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

*
************************************************************
* File name: MainScanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: Jan 01 2025
* Purpose: This file is the main code for Scanner (A22)
* Function list: (...).
************************************************************
*/

/*
 *.............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: Preprocessor directives
 * The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define directive does not have any effect on other compiler projects
 * (Gcc, VSCode, Codeblocks, etc.).
 *.............................................................................
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef BIOLOGY_H_
#include "../include/Biology.h"
#endif

#ifndef TISSUE_H_
#include "../include/Step2Tissue.h"
#endif

#ifndef ORGAN_H_
#include "../include/Step3Organ.h"
#endif

 /*check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/*
 * -------------------------------------------------------------
 *  Global vars and External vars
 * -------------------------------------------------------------
 */

 /* Global objects - traits (used in other codes as external) */
/* stringLiteralTable and errorNumber are defined in Step3Organ.c */

/* External objects */
extern STEL_int line; /* Source code line numbers - defined in scanner.c */
extern Signal organScan(STEL_void);
extern OrganData organData;

/*
 * -------------------------------------------------------------
 *  Function declarations
 * -------------------------------------------------------------
 */
STEL_void displayOrgan(TissuePointer ptrBuffer);
STEL_long organFileSize(STEL_string fname);
static STEL_void printSignal(Signal t);  /* forward declaration */

/*
************************************************************
 *  Scanner Main function
 * Parameters:
 *   argc / argv = Parameters from command prompt
 * Return value:
 *	Success operation.
 ***********************************************************
 */


/* V1 main3Organ and displayOrgan removed - use main3Organ. */

STEL_int main3Organ(STEL_int argc, STEL_string *argv) {
    TissuePointer sourceBuffer;
    Signal currentSignal;
    STEL_int loadSize = 0;
    STEL_string sourceFile;
    STEL_string dfaFile;

    numOrganErrors = 0;

    if (argc <= 2) {
        errorPrint("Usage: STELlogy A <source_file> [dfa_csv]\n");
        return EXIT_FAILURE;
    }

    sourceFile = argv[2];
    dfaFile    = (argc > 3) ? argv[3] : DEFAULT_DFA_FILE;

    /* Load DFA from CSV */
    if (!loadScannerDFA((STEL_string)dfaFile)) {
        errorPrint("Organ : DFA load failed from '%s'\n", dfaFile);
        return EXIT_FAILURE;
    }

    /* Load and decode the source file through Cell */
    sourceBuffer = tissueCreate(TISSUE_DEFAULT_SIZE);
    if (!sourceBuffer) {
        errorPrint("Organ : cannot allocate tissue buffer\n");
        return EXIT_FAILURE;
    }

    printf("Reading file %s ....Please wait\n", sourceFile);
    loadSize = tissueLoad(sourceBuffer, (STEL_string)sourceFile);
    if (loadSize == TISSUE_ERROR) {
        printf("The input file %s is not completely loaded.\n", sourceFile);
    }
    if (loadSize > 0) {
        tissueAddCell(sourceBuffer, TISSUE_TERMINATOR);
        tissueRecover(sourceBuffer);
    }

    /* Create string-literal repository */
    stringLiteralTable = tissueCreate(TISSUE_DEFAULT_SIZE);
    if (!stringLiteralTable) {
        errorPrint("Organ : cannot allocate string-literal buffer\n");
        tissueFree(sourceBuffer);
        return EXIT_FAILURE;
    }

    /* Start  scanner */
    if (startOrgan(sourceBuffer)) {
        errorPrint("Organ : startOrgan failed\n");
        tissueFree(sourceBuffer);
        tissueFree(stringLiteralTable);
        return EXIT_FAILURE;
    }

    printf("\nScanning source file (Organ  - CSV DFA)...\n\n");
    printf("Signal\t\tAttribute\n");
    printf("----------------------------------\n");

    do {
        currentSignal = organScan();
        printSignal(currentSignal);
    } while (currentSignal.code != SEOF_S);

    printf("\nPrinting string table...\n");
    printf("----------------------------------\n");
    if (tissueGetPosStore(stringLiteralTable))
        tissuePrint(stringLiteralTable);
    printf("\n----------------------------------\n");
    printf("Number of scanner errors: %d\n", numOrganErrors);
    printf("----------------------------------\n");

    printOrganData();

    tissueFree(sourceBuffer);
    tissueFree(stringLiteralTable);
    /* Propagate scanner errors to the process exit code, so scripted /
     * batch callers can detect a failed scan without parsing stdout. */
    return numOrganErrors ? EXIT_FAILURE : EXIT_SUCCESS;
}

/* Mirror of printToken / printSignal from Main3Organ.c */
static STEL_void printSignal(Signal t) {
    static const STEL_char *ari[] = { "+", "-", "*", "/", "^" };
    static const STEL_char *rel[] = { "==", "<>", ">", "<", "<=", ">=" };
    static const STEL_char *lgc[] = { "&&", "||", "!" };
    printf("%-12s\t", signalStrTable[t.code]);
    switch (t.code) {
    case ERR_S: case RTE_S:
        printf("%s\n", t.attribute.errLexeme); break;
    case IVID_S: case FVID_S: case SVID_S: case MNID_S:
        printf("%s\n", t.attribute.idLexeme); break;
    case KW_S:
        printf("%s\n", keywordTable[t.attribute.keywordIndex]); break;
    case INL_S:
        printf("%d\n", t.attribute.intValue); break;
    case FPL_S:
        printf("%f\n", t.attribute.floatValue); break;
    case STR_S:
        printf("%d\t\"%s\"\n", t.attribute.contentString,
               tissueGetContent(stringLiteralTable, t.attribute.contentString)); break;
    case SCC_OP_S: printf("++\n"); break;
    case ART_OP_S: printf("%s\n", ari[t.attribute.arithmeticOperator]); break;
    case REL_OP_S: printf("%s\n", rel[t.attribute.relationalOperator]); break;
    case LOG_OP_S: printf("%s\n", lgc[t.attribute.logicalOperator]); break;
    case SEOF_S:   printf("%d\n", t.attribute.seofType); break;
    default:       printf("\n");
    }
}
