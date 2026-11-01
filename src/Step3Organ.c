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
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: Jan 01 2025
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef STELLOGY_H_
#include "../include/Biology.h"
#endif

#ifndef TISSUE_H_
#include "../include/Step2Tissue.h"
#endif

#ifndef ORGAN_H_
#include "../include/Step3Organ.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - traits */
/* This buffer is used as a repository for string literals. */
TissuePointer stringLiteralTable = NULL;	/* String literal table */
STEL_int line;								/* Current line number of the source code */
STEL_int lexemeLine = 1;					/* Line where the last Signal started   */
STEL_int errorNumber = 0;					/* Run-time error number */
STEL_int numOrganErrors = 0;				/* Number of lexical errors */
OrganData organData;						/* Scanner statistics */
STEL_string signalStrTable[NUM_SIGNALS] = {	/* same order as enum SIGNALS */
	"ERR_S", "IVID_S", "FVID_S", "SVID_S", "MNID_S", "INL_S", "FPL_S", "STR_S",
	"SCC_OP_S", "ASS_OP_S", "ART_OP_S", "REL_OP_S", "LOG_OP_S", "LPR_S", "RPR_S",
	"LBR_S", "RBR_S", "KW_S", "COM_S", "EOS_S", "RTE_S", "CMT_S", "SEOF_S"
};

/* Language data (keywords loaded from CSV; boundary is fixed) */
STEL_int KWT_SIZE = 0;
STEL_string keywordTable[MAX_KEYWORDS];
STEL_char boundaryTable[BOUNDARY_TABSIZE] = { EOS_CHR, EOF_CHR };

/* Local(file) global objects - traits */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */


/* ====================================================================
 * V1 hardcoded scanner (startOrgan / organScan / organClassify /
 * organTransition / printOrganData) has been removed.
 * The scanner is now driven entirely by the CSV-DFA file loaded
 * via loadScannerDFA()  - see the section below.
 * ==================================================================== */

/* ====================================================================
 * Accepting-state functions  (signal*)
 * Called by the scanner action dispatch in organScan().
 * These were previously interleaved inside the V1 scanner code;
 * they are kept here as standalone functions.
 * ==================================================================== */

Signal signalIdentifier(STEL_string lexeme) {
    Signal t;
    STEL_int i;
    STEL_char last;
    memset(&t, 0, sizeof(t));
    if (!lexeme || !lexeme[0]) return signalError("");
    last = lexeme[strlen(lexeme) - 1];
    /* Check for keyword match first (FSWR states retract, so last char is NOT the suffix) */
    for (i = 0; i < KWT_SIZE; i++) {
        if (keywordTable[i] && strcmp(lexeme, keywordTable[i]) == 0) {
            t.code = KW_S;
            t.attribute.keywordIndex = i;
            return t;
        }
    }
    /* Classify by variable-suffix character, and store lexeme for grammar lookahead */
    switch (last) {
    case DOL_CHR: t.code = IVID_S; break;
    case PTG_CHR: t.code = FVID_S; break;
    case ATT_CHR: t.code = SVID_S; break;
    case AMP_CHR: t.code = MNID_S; break;
    default:
        return signalError(lexeme);            /* no valid type suffix */
    }
    if ((STEL_int)strlen(lexeme) > VID_LEN)    /* identifiers have at most VID_LEN chars */
        return signalError(lexeme);
    /* Store the full lexeme so grammar rules like "MNID:main&" can match */
    strncpy_s(t.attribute.idLexeme, sizeof(t.attribute.idLexeme),
              lexeme, sizeof(t.attribute.idLexeme) - 1);
    return t;
}

Signal signalKeyword(STEL_string lexeme) {
    Signal t;
    STEL_int i;
    memset(&t, 0, sizeof(t));
    for (i = 0; i < KWT_SIZE; i++) {
        if (keywordTable[i] && strcmp(lexeme, keywordTable[i]) == 0) {
            t.code = KW_S;
            t.attribute.keywordIndex = i;
            return t;
        }
    }
    /* a word without type suffix that is not a keyword (e.g. "Count" for "Count$") */
    return signalError(lexeme);
}

Signal signalIntegerLiteral(STEL_string lexeme) {
    Signal t;
    STEL_long value;
    memset(&t, 0, sizeof(t));
    if (!lexeme || strlen(lexeme) > NUM_LEN)          /* too many digits */
        return signalError(lexeme ? lexeme : "");
    value = strtol(lexeme, NULL, 10);
    if (value < 0 || value > BIO_INT_MAX)            /* outside the 2-byte range */
        return signalError(lexeme);
    t.code = INL_S;
    t.attribute.intValue = (STEL_int)value;
    return t;
}

Signal signalFloatLiteral(STEL_string lexeme) {
    Signal t;
    STEL_double value;
    memset(&t, 0, sizeof(t));
    if (!lexeme) return signalError("");
    value = strtod(lexeme, NULL);
    if (value > FLT_MAX || (value > 0.0 && value < FLT_MIN))   /* not a 4-byte float */
        return signalError(lexeme);
    t.code = FPL_S;
    t.attribute.floatValue = (STEL_float)value;
    return t;
}

Signal signalStringLiteral(STEL_string lexeme) {
    Signal t;
    STEL_int j, len;
    memset(&t, 0, sizeof(t));
    t.code = STR_S;
    if (!lexeme || !stringLiteralTable) return t;
    len = (STEL_int)strlen(lexeme);
    t.attribute.contentString = tissueGetPosStore(stringLiteralTable);
    /* store the content WITHOUT the quotes; count the lines it spans */
    for (j = 1; j <= len - 1; j++) {
        STEL_char ch = (j == len - 1) ? '\0' : lexeme[j];
        if (ch == '\n') line++;
        if (!tissueAddCell(stringLiteralTable, ch)) {      /* table cannot grow */
            errorNumber = RTE_CODE;
            memset(&t, 0, sizeof(t));
            t.code = RTE_S;
            strcpy_s(t.attribute.errLexeme, sizeof(t.attribute.errLexeme), "Run Time Error:");
            return t;
        }
    }
    return t;
}

Signal signalComment(STEL_string lexeme) {
    Signal t;
    (STEL_void)lexeme;
    memset(&t, 0, sizeof(t));
    t.code = CMT_S;
    return t;
}

Signal signalError(STEL_string lexeme) {
    Signal t;
    STEL_int len = lexeme ? (STEL_int)strlen(lexeme) : 0;
    memset(&t, 0, sizeof(t));
    t.code = ERR_S;
    if (len > ERR_LEN) {                       /* keep ERR_LEN-3 chars + "..." */
        strncpy_s(t.attribute.errLexeme, sizeof(t.attribute.errLexeme), lexeme, ERR_LEN - 3);
        strcat_s(t.attribute.errLexeme, sizeof(t.attribute.errLexeme), "...");
    } else if (len > 0) {
        strcpy_s(t.attribute.errLexeme, sizeof(t.attribute.errLexeme), lexeme);
    }
    {   /* keep messages on one line: newlines/tabs of the lexeme become spaces */
        STEL_char *q;
        for (q = t.attribute.errLexeme; *q; q++) if (*q == '\n' || *q == '\r' || *q == '\t') *q = ' ';
    }
    numOrganErrors++;
    return t;
}

/* ====================================================================
 * Organ  -  CSV-DFA-driven scanner (only version)
 * ==================================================================== */
/* ----------------------------------------------------------------
 * Runtime DFA tables (filled by loadScannerDFA)
 * ---------------------------------------------------------------- */
static STEL_int organTransitionTable[MAX_STATES_][MAX_CLASSES_];
static STEL_int organStateType[MAX_STATES_];
static DFAAction actionTable[MAX_STATES_];
static STEL_int numStates = 0;
static STEL_int numClasses = 0;

/* Private scanner state (mirrors Step3Organ internals) */
static TissuePointer organBuffer = NULL;
/* Note: 'line' (the shared line counter) is the global, non-static
 * variable declared above -- Step4System.h declares it 'extern' so
 * other translation units (Step4System.c, Test3Organ.c, ...) can see
 * the same counter, so it must not be redeclared 'static' here. */

/* Accepted signal statistics (variant) */
static OrganData scData;

/* ----------------------------------------------------------------
 * Name-to-code lookup helpers for CSV parsing
 * ---------------------------------------------------------------- */
static STEL_int parseStateType(const STEL_string s) {
    if (strcmp(s, "FSNR") == 0) return FSNR_;
    if (strcmp(s, "FSWR") == 0) return FSWR_;
    return NOFS_;
}

static DFAAction parseAction(const STEL_string s) {
    if (strcmp(s, "IDENTIFIER") == 0) return ACT_IDENTIFIER;
    if (strcmp(s, "KEYWORD")    == 0) return ACT_KEYWORD;
    if (strcmp(s, "INTEGER")    == 0) return ACT_INTEGER;
    if (strcmp(s, "FLOAT")      == 0) return ACT_FLOAT;
    if (strcmp(s, "STRING")     == 0) return ACT_STRING;
    if (strcmp(s, "COMMENT")    == 0) return ACT_COMMENT;
    if (strcmp(s, "ERROR")      == 0) return ACT_ERROR;
    return ACT_NULL;
}

/* Strip leading and trailing whitespace in-place (safe for fixed arrays) */
static void trimInPlace(STEL_string s) {
    STEL_int start = 0, end;
    end = (STEL_int)strlen(s);
    while (start < end && isspace((unsigned char)s[start])) start++;
    while (end > start && isspace((unsigned char)s[end - 1])) end--;
    memmove(s, s + start, (size_t)(end - start));
    s[end - start] = '\0';
}

/* ----------------------------------------------------------------
 * loadScannerDFA()
 *   Parses the CSV file.  Pipe-separated fields.  Lines beginning
 *   with '#' or blank are skipped.  Returns number of states loaded.
 * ---------------------------------------------------------------- */
STEL_int loadScannerDFA(const STEL_string csvFile) {
    FILE *fp;
    STEL_char lineBuf[512];
    STEL_int loaded = 0;
    STEL_int section = 0; /* 0=none, 1=CHAR_CLASSES, 2=KEYWORDS, 3=DFA */
    STEL_int i;

    numStates = 0;
    numClasses = 0;
    KWT_SIZE = 0;
    for (i = 0; i < MAX_KEYWORDS; i++) {           /* free keywords of a previous load */
        free(keywordTable[i]);
        keywordTable[i] = NULL;
    }
    memset(organTransitionTable, 0, sizeof(organTransitionTable));
    memset(organStateType, 0, sizeof(organStateType));
    memset(actionTable, 0, sizeof(actionTable));

    fp = fopen(csvFile, "r");
    if (!fp) {
        fprintf(stderr, "Organ : cannot open DFA file '%s'\n", csvFile);
        return 0;
    }

    while (fgets(lineBuf, sizeof(lineBuf), fp)) {
        STEL_char *p = lineBuf;
        while (*p && isspace((unsigned char)*p)) p++;
        if (*p == '\0' || *p == '\n') continue;

        /* Section headers */
        if (*p == '[') {
            if (strncmp(p, "[CHAR_CLASSES]", 14) == 0) section = 1;
            else if (strncmp(p, "[KEYWORDS]", 10) == 0) section = 2;
            else if (strncmp(p, "[DFA]", 5) == 0) section = 3;
            else section = 0;
            continue;
        }
        if (*p == '#') continue; /* comments */

        if (section == 1) {      /* column names: their number must be CHAR_CLASSES */
            STEL_int cols = 1;
            STEL_char *q;
            for (q = p; *q; q++) if (*q == '|') cols++;
            if (cols != CHAR_CLASSES) {
                fprintf(stderr, "Organ : [CHAR_CLASSES] has %d columns but CHAR_CLASSES is %d\n", cols, CHAR_CLASSES);
                fclose(fp);
                return 0;
            }
            continue;
        }

        if (section == 2) {
            /* Keyword line: single word, optional trailing comment after # */
            STEL_char word[64];
            STEL_int n = 0;
            while (*p && !isspace((unsigned char)*p) && *p != '#' && n < (STEL_int)sizeof(word) - 1)
                word[n++] = *p++;
            word[n] = '\0';
            if (n > 0 && KWT_SIZE < MAX_KEYWORDS) {
                /* allocate a durable copy */
                STEL_string copy = (STEL_string)malloc((size_t)n + 1);
                if (copy) {
                    strcpy(copy, word);
                    keywordTable[KWT_SIZE++] = copy;
                }
            }
            continue;
        }

        if (section != 3 && section != 0) continue;
        /* Accept DFA rows also in section 0 for backward compatibility with
         * older CSV files that had no section markers. */
        if (section == 1) continue;

        /* DFA row: STATE|...|STATE_TYPE|ACTION */
        {
            STEL_char fields[CHAR_CLASSES + 4][64];
            STEL_int nf = 0;
            STEL_int state, col;
            STEL_char *start = p;
            STEL_char *endf;

            while (nf < CHAR_CLASSES + 4) {
                endf = strchr(start, '|');
                if (!endf) {
                    strncpy_s(fields[nf], sizeof(fields[nf]), start, sizeof(fields[nf]) - 1);
                    trimInPlace(fields[nf]);
                    nf++;
                    break;
                }
                *endf = '\0';
                strncpy_s(fields[nf], sizeof(fields[nf]), start, sizeof(fields[nf]) - 1);
                trimInPlace(fields[nf]);
                nf++;
                start = endf + 1;
            }
            if (nf < CHAR_CLASSES + 3) continue;   /* STATE + classes + TYPE + ACTION */
            /* skip header-like rows */
            if (!isdigit((unsigned char)fields[0][0])) continue;

            state = atoi(fields[0]);
            if (state < 0 || state >= MAX_STATES_) continue;

            for (col = 0; col < CHAR_CLASSES && (col + 1) < nf; col++) {
                organTransitionTable[state][col] = atoi(fields[col + 1]);
            }
            organStateType[state] = parseStateType(fields[CHAR_CLASSES + 1]);
            actionTable[state]    = parseAction(fields[CHAR_CLASSES + 2]);

            if (col > numClasses) numClasses = col;
            if (state + 1 > numStates) numStates = state + 1;
            loaded++;
        }
    }
    fclose(fp);

    if (loaded == 0) {
        fprintf(stderr, "Organ : no valid states loaded from '%s'\n", csvFile);
    } else {
        printf("[Organ ] DFA loaded: %d states x %d classes, %d keywords from '%s'\n",
               numStates, numClasses, KWT_SIZE, csvFile);
    }
    return loaded;
}

static STEL_int charClass(STEL_char c) {
    if (isalpha((unsigned char)c)) return 0;   /* L */
    if (isdigit((unsigned char)c)) return 1;   /* D */
    switch (c) {
    case '_':  return 2;  /* U */
    case '.':  return 3;  /* P */
    case '$':  return 4;  /* I */
    case '%':  return 5;  /* F */
    case '@':  return 6;  /* S */
    case '&':  return 7;  /* M */
    case '"':  return 8;  /* Q */
    case '#':  return 9;  /* C */
    case '\0': return 10; /* E */
    default:
        if ((STEL_int)(unsigned char)c == 255 || c == EOF) return 10; /* E */
        return 11; /* O */
    }
}

static STEL_int nextState(STEL_int state, STEL_char c) {
    STEL_int cls = charClass(c);
    if (state < 0 || state >= numStates || cls < 0 || cls >= numClasses)
        return ESNR;
    return organTransitionTable[state][cls];
}

/* ----------------------------------------------------------------
 * startOrgan()
 *   Initialise the  scanner on a loaded Tissue buffer.
 * ---------------------------------------------------------------- */
STEL_int startOrgan(TissuePointer psc_buf) {
    if (!psc_buf || !numStates) {
        fprintf(stderr, "Organ : startOrgan called with no buffer or unloaded DFA\n");
        return 1;
    }
    organBuffer = psc_buf;
    tissueRecover(psc_buf);   /* reset read position to start of buffer */
    line = 1;
    memset(&scData, 0, sizeof(scData));
    return 0;
}

/* ----------------------------------------------------------------
 * organScan()
 *   Produce one Signal from the input Tissue buffer using the
 *   runtime DFA tables.  Mirrors the logic of tokenizer() / organScan()
 *   in Step3Organ.c exactly - only the table lookup changes.
 * ---------------------------------------------------------------- */
Signal organScan(STEL_void) {
    static STEL_char lexeme[MAX_LEXEME + 2];
    STEL_int overflow = 0;     /* lexeme longer than MAX_LEXEME */
    STEL_int state = 0;
    STEL_int lexLen = 0;
    STEL_char c;
    Signal result;

    memset(&result, 0, sizeof(result));
    memset(lexeme, 0, sizeof(lexeme));

    /* Skip whitespace, count newlines */
    do {
        c = tissueGetCell(organBuffer);
        if (c == '\n') line++;
    } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');
    lexemeLine = line;          /* a multi-line lexeme is reported where it starts */

    /* EOF check */
    if (c == TISSUE_TERMINATOR || (STEL_int)(unsigned char)c == 255) {
        result.code = SEOF_S;
        result.attribute.seofType = SEOF_0;
        scData.organHistogram[SEOF_S]++;
        return result;
    }

    /* ----------------------------------------------------------------
     * Part 1: single-character and simple multi-character tokens.
     * These are handled identically to V1's Part 1 switch block -
     * they never need to enter the DFA at all.
     * ---------------------------------------------------------------- */
    switch (c) {
    case ';':
        result.code = EOS_S;
        scData.organHistogram[EOS_S]++;
        return result;
    case '(':
        result.code = LPR_S;
        scData.organHistogram[LPR_S]++;
        return result;
    case ')':
        result.code = RPR_S;
        scData.organHistogram[RPR_S]++;
        return result;
    case '{':
        result.code = LBR_S;
        scData.organHistogram[LBR_S]++;
        return result;
    case '}':
        result.code = RBR_S;
        scData.organHistogram[RBR_S]++;
        return result;
    case '+': {
        STEL_char nx = tissueGetCell(organBuffer);
        if (nx == '+') {
            result.code = SCC_OP_S;
        } else {
            tissueRetract(organBuffer);
            result.code = ART_OP_S;
            result.attribute.codeType = OP_ADD;
        }
        scData.organHistogram[result.code]++;
        return result;
    }
    case '-':
        result.code = ART_OP_S;
        result.attribute.codeType = OP_SUB;
        scData.organHistogram[ART_OP_S]++;
        return result;
    case '*':
        result.code = ART_OP_S;
        result.attribute.codeType = OP_MUL;
        scData.organHistogram[ART_OP_S]++;
        return result;
    case '/':
        result.code = ART_OP_S;
        result.attribute.codeType = OP_DIV;
        scData.organHistogram[ART_OP_S]++;
        return result;
    case '^':
        result.code = ART_OP_S;
        result.attribute.codeType = OP_POW;
        scData.organHistogram[ART_OP_S]++;
        return result;
    case '>': {                                  /* >  >= */
        STEL_char nx = tissueGetCell(organBuffer);
        result.code = REL_OP_S;
        if (nx == '=') result.attribute.codeType = OP_GE;
        else { tissueRetract(organBuffer); result.attribute.codeType = OP_GT; }
        scData.organHistogram[REL_OP_S]++;
        return result;
    }
    case '<': {
        STEL_char nx = tissueGetCell(organBuffer);
        result.code = REL_OP_S;
        if (nx == '>') {
            result.attribute.codeType = OP_NE;
        } else if (nx == '=') {
            result.attribute.codeType = OP_LE;
        } else {
            tissueRetract(organBuffer);
            result.attribute.codeType = OP_LT;
        }
        scData.organHistogram[REL_OP_S]++;
        return result;
    }
    case '=': {
        STEL_char nx = tissueGetCell(organBuffer);
        if (nx == '=') {
            result.code = REL_OP_S;
            result.attribute.codeType = OP_EQ;
        } else {
            tissueRetract(organBuffer);
            result.code = ASS_OP_S;
        }
        scData.organHistogram[result.code]++;
        return result;
    }
    case ',':
        result.code = COM_S;
        scData.organHistogram[COM_S]++;
        return result;
    case '&':                                    /* &&  (a single & is an error) */
    case '|': {                                  /* || */
        STEL_char nx = tissueGetCell(organBuffer);
        if (nx == c) {
            result.code = LOG_OP_S;
            result.attribute.codeType = (c == '&') ? OP_AND : OP_OR;
            scData.organHistogram[LOG_OP_S]++;
            return result;
        }
        tissueRetract(organBuffer);
        lexeme[0] = c; lexeme[1] = '\0';
        result = signalError(lexeme);
        scData.organHistogram[ERR_S]++;
        return result;
    }
    case '!':                                    /* logical NOT */
        result.code = LOG_OP_S;
        result.attribute.codeType = OP_NOT;
        scData.organHistogram[LOG_OP_S]++;
        return result;
    default:
        break;   /* fall through to DFA (Part 2) */
    }
    lexeme[lexLen++] = c;
    state = nextState(0, c);

    while (organStateType[state] == NOFS_) {
        c = tissueGetCell(organBuffer);
        if (c == '\n') line++;
        if (lexLen < MAX_LEXEME) lexeme[lexLen++] = c;
        else overflow = 1;
        state = nextState(state, c);
    }

    /* Retract if FSWR */
    if (organStateType[state] == FSWR_) {
        tissueRetract(organBuffer);
        if (!overflow && lexLen > 0) {      /* the retracted char was stored */
            lexLen--;
            if (lexeme[lexLen] == '\n') line--;
        }
        lexeme[lexLen] = '\0';
    } else {
        lexeme[lexLen] = '\0';
    }

    /* A lexeme longer than MAX_LEXEME is a lexical error */
    if (overflow) {
        result = signalError(lexeme);
        scData.organHistogram[ERR_S]++;
        return result;
    }

    /* Call the accepting function based on action code */
    switch (actionTable[state]) {
    case ACT_IDENTIFIER:
        result = signalIdentifier(lexeme);
        break;
    case ACT_KEYWORD:
        result = signalKeyword(lexeme);
        break;
    case ACT_INTEGER:
        result = signalIntegerLiteral(lexeme);
        break;
    case ACT_FLOAT:
        result = signalFloatLiteral(lexeme);
        break;
    case ACT_STRING:
        result = signalStringLiteral(lexeme);
        break;
    case ACT_COMMENT:
        result = signalComment(lexeme);
        break;
    case ACT_ERROR:
        result = signalError(lexeme);
        break;
    default:
        result = signalError(lexeme);
        break;
    }

    if (result.code >= 0 && result.code < NUM_SIGNALS)
        scData.organHistogram[result.code]++;

    return result;
}

/* ----------------------------------------------------------------
 * printOrganData() - mirror of printOrganData
 * ---------------------------------------------------------------- */
STEL_void printOrganData(STEL_void) {
    STEL_int i;
    printf("Statistics (Organ ):\n");
    printf("----------------------------------\n");
    for (i = 0; i < NUM_SIGNALS; i++) {
        if (scData.organHistogram[i] > 0)
            printf("Signal[%s]=%d\n", signalStrTable[i], scData.organHistogram[i]);
    }
    printf("----------------------------------\n");
}
