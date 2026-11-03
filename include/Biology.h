/*
************************************************************
* BIOSYSTEMS COURSE - Algonquin College
* Code version: Fall, 2026
* Author: Connor Guenette - 04107333
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: Compilers.h
* Compiler: MS Visual Studio 2022 / GCC (C89 -ansi -pedantic)
* Course: CST 8152 - Compilers
* Assignment: A1.
* Purpose: This file defines the functions called by main function.
*          STUDENT MODEL FOLDER for Assignment 1 -- prototypes
*          are trimmed to match only the stage(s) present in this
*          folder; self-test prototypes are omitted entirely (no
*          Test*.c files are included in these model folders).
* Function list: main1Cell().
*************************************************************/

#ifndef BIOLOGY_H_
#define BIOLOGY_H_

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/* TO_DO: Adjust your language (cannot be "Sofia") */
/* Language name (also used as the Cell cipher key throughout the pipeline) */
#define STR_CompilerName	"Stella"

/*
------------------------------------------------------------
Default data-file names (single source of truth).
Every stage (Organ/System/Body) that needs one of these paths
includes Biology.h and reuses the constant below instead of
redefining it locally.
------------------------------------------------------------
*/
#define DEFAULT_DFA_FILE      "compiler_scanner_dfa.txt"
#define DEFAULT_GRAMMAR_FILE  "compiler_parser_grammar.txt"
#define DEFAULT_RUNNER_CONFIG "compiler_runner_config.txt"

/* Default logo file (Req: printLogo prints this file's content, if present) */
#define DEFAULT_LOGO_FILE     "../txtFiles/logo.txt"

/* Logical constants - adapt for your language */
#define STEL_TRUE  1
#define STEL_FALSE 0

#define INVALID NULL
#define EOF_CHAR '\0'

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/

/* TO_DO: Define your typedefs */
typedef char			STEL_char;
typedef char*			STEL_string;
typedef int				STEL_int;
typedef float			STEL_float;
typedef void			STEL_void;

typedef unsigned char	STEL_bol;
typedef unsigned char	STEL_byte;

typedef long			STEL_long;
typedef double			STEL_double;

/*
------------------------------------------------------------
Programs (Assignment 1 model folder: only stage(s) 1-1 apply;
higher-numbered stage constants are kept for source compatibility
with later assignments' grammar/DFA files, but nothing in this
folder ever dispatches to them):
1: Cell   - encode / decode a source file
2: Tissue - buffered file reader
3: Organ  - CSV-DFA driven scanner (compiler_scanner_dfa.csv)
4: System - grammar-file driven parser (compiler_parser_grammar.txt)
5: Body   - config-file driven runner (compiler_runner_config.txt)
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_CELL     = '1', // Coder:   encode / decode a source file
	PGM_TISSUE   = '2', // Reader:  buffered file reader
	PGM_ORGAN    = '3', // Scanner: CSV-DFA driven (compiler_scanner_dfa.csv)
	PGM_SYSTEM   = '4', // Parser:  grammar-file driven (compiler_parser_grammar.txt)
	PGM_BODY     = '5', // Runner:  config-file driven  (compiler_runner_config.txt)

	/* Help: print README.md as inline help */
	PGM_HELP      = '?',
	PGM_HELP_LONG = 'H',
};

/*
------------------------------------------------------------
Main functions signatures (Assignment 1 model: only the
stage(s) actually included in this folder are declared)
------------------------------------------------------------
*/
STEL_int main1Cell      (STEL_int argc, STEL_string* argv); /* Coder - invokes Main1Cell code */
STEL_int main2Tissue    (STEL_int argc, STEL_string* argv); /* Reader - invokes Main2Tissue code */
STEL_int main3Organ		(STEL_int argc, STEL_string* argv);	/* Scanner - invokes Main3Organ code */

STEL_int testCell(STEL_int argc, STEL_string* argv);
STEL_int testTissue(STEL_int argc, STEL_string* argv);
STEL_int testOrgan(STEL_int argc, STEL_string* argv);

STEL_void printLogo();
STEL_void errorPrint(STEL_string fmt, ...);


/* Cross-platform safe-string compatibility ------------------------------------
 * MSVC provides strcpy_s, strncpy_s, strcat_s, strncat_s, _strdup natively.
 * On gcc/POSIX those are not standard C, so we define wrappers here.
 * This replaces the need for a separate PosixShims.h.
 * --------------------------------------------------------------------------- */
#ifndef _MSC_VER
#  include <string.h>
#  include <stdlib.h>
#  ifndef strcpy_s
static int _STEL_strcpy_s(char *d, size_t n, const char *s) {
    if(!d || !n) return 1;
    strncpy(d, s, n-1); d[n-1] = '\0'; return 0; }
#    define strcpy_s(d,n,s)    _STEL_strcpy_s((d),(n),(s))
#  endif
#  ifndef strncpy_s
static int _STEL_strncpy_s(char *d, size_t n, const char *s, size_t c) {
    size_t m = (c < n-1) ? c : n-1;
    if(!d || !n) return 1;
    strncpy(d, s, m); d[m] = '\0'; return 0; }
#    define strncpy_s(d,n,s,c) _STEL_strncpy_s((d),(n),(s),(size_t)(c))
#  endif
#  ifndef strcat_s
#    define strcat_s(d,n,s)    (strncat((d),(s),(n)-strlen(d)-1), 0)
#  endif
#  ifndef strncat_s
#    define strncat_s(d,n,s,c) (strncat((d),(s),(size_t)(c)), 0)
#  endif
#  ifndef _strdup
#    define _strdup strdup
#  endif
#endif

#endif

/*
 * Sample debugger "args" array (e.g. for a VS Code launch.json) for
 * quickly testing Cell directly against the bundled InputExample1.txt,
 * without going through the interactive menu:
 *
 *   "args": [1, 1, "InputExample1.txt", "CODED.TXT"]   // Cell: encode -> CODED.TXT
 *
 * IMPORTANT: CODED.TXT (if you create one) is a STATIC file, Cell-
 * encoded once with whatever STR_CompilerName was in effect at the
 * time -- it is NOT re-encoded automatically just because you edit
 * STR_CompilerName above. If you rename the compiler and then test a
 * later stage directly against a stale CODED.TXT, decoding it with the
 * NEW key produces garbage. The batch file in this folder always
 * re-encodes InputExample1.txt fresh with whatever key the binary was
 * just built with, so it never goes stale.
 */

