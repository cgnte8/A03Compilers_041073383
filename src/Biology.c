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
* File name: compilers.c
* Compiler: MS Visual Studio 2022 / GCC (C89 -ansi -pedantic)
* Course: CST 8152 - Compilers
* Assignment: A1.
* Purpose: This file is the main program of the Compilers Project
*          -- STUDENT MODEL FOLDER for Assignment 1, trimmed to
*          include only the Cell implemented so far. Self-test
*          suites and other pipeline stages are intentionally left out
*          of this folder; each later assignment's model folder adds
*          the next stage on top of this one.
* Function list: main().
************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef BIOLOGY_H_
#include "../include/Biology.h"
#endif

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/*
***********************************************************
* Function name: main
* Purpose: Main function
* Author: Paulo Sousa
* History/Versions: Ver F22
* Called functions: main1Cell()
* Parameters: Command line arguments - argc, argv
* Return value: Status
* Algorithm: -
*************************************************************
*/

/* ================================================================
 * biologyDispatch  --  routes one option to its stage.
 * Extracted from main so that interactiveMenu can reuse it.
 * ================================================================ */
static STEL_int helpOption(STEL_void);  /* forward declaration */
static STEL_int STELlogyDispatch(STEL_int argc, STEL_string *argv) {
	STEL_char option = argv[1][0];
	switch (option) {
	/* ---- Pipeline stages ---- */
	case PGM_CELL:
		printf("\n[%c] Cell  -- encode / decode\n\n", PGM_CELL);
		return main1Cell(argc, argv);

	/* ---- Tissue ---- */
	case PGM_TISSUE:
		printf("\n[%c] Tissue  -- buffered file reader\n\n", PGM_TISSUE);
		return main2Tissue(argc, argv);

	/* ---- Help ---- */
	case PGM_HELP:
	case PGM_HELP_LONG:
		return helpOption();
	default:
		printf("Unknown option '%c'. Use '?' or 'H' for help.\n", option);
		break;
	}
	return EXIT_SUCCESS;
}

/* ================================================================
 * helpOption  --  reads README.md and prints it to stdout.
 * Falls back to a compact built-in summary when the file is absent.
 * ================================================================ */
static STEL_int helpOption(STEL_void) {
	const STEL_char *path = "README.md";
	FILE *fp = fopen(path, "r");
	STEL_char line[256];
	if (fp) {
		printf("\n");
		while (fgets(line, (STEL_int)sizeof(line), fp))
			fputs(line, stdout);
		fclose(fp);
	} else {
		/* Built-in fallback */
		printf("\n  STELCompiler  --  Stella Language Pipeline (Assignment 1 model)\n\n");
		printf("  Usage: STELlogy <option> [args...]\n\n");
		printf("  Pipeline stages implemented in this folder:\n");
		printf("  1  Cell\n");
		printf("\n  Help:        ?  or  H\n\n");
		printf("  (README.md not found - place it beside the binary for full help)\n\n");
	}
	return EXIT_SUCCESS;
}

/* ================================================================
 * showMenu  --  prints the interactive option table.
 * ================================================================ */
static STEL_void showMenu(STEL_void) {
	printf("\n");
	printf(" +================================================+\n");
	printf(" |   STELCompiler  --  Genoma Language Pipeline    |\n");
	printf(" |   Algonquin College -- Assignment 1 model     |\n");
	printf(" +================================================+\n");
	printf("\n");
	printf("  PIPELINE STAGES\n");
	printf("  [1] Cell   - encode / decode a source file\n");
	printf("\n");
	printf("  HELP\n");
	printf("  [?] or [H]  -- display README.md inline help\n");
	printf("\n");
	printf("  Enter option: ");
	fflush(stdout);
}

/* ================================================================
 * promptFile  --  reads a file path from stdin.
 * Shows [defaultVal] as hint; uses defaultVal when user presses Enter.
 * Returns 1 if a value is available, 0 if empty with no default.
 * ================================================================ */
static STEL_int promptFile(const STEL_char *prompt,
                            STEL_char *buf, STEL_int bufSize,
                            const STEL_char *defaultVal) {
	printf("  %s", prompt);
	if (defaultVal && defaultVal[0])
		printf(" [default: %s]", defaultVal);
	printf(": ");
	fflush(stdout);
	if (!fgets(buf, bufSize, stdin)) { buf[0] = '\0'; return 0; }
	buf[strcspn(buf, "\r\n")] = '\0';
	if (buf[0] == '\0' && defaultVal && defaultVal[0])
		strncpy_s(buf, (size_t)bufSize, defaultVal, (size_t)(bufSize - 1));
	return buf[0] != '\0';
}

/* ================================================================
 * interactiveMenu  --  called when STELlogy is run with no argument.
 *
 * Displays all available options, reads the user's choice, prompts
 * for any file arguments required by that option (showing defaults
 * from the config-file constants), then delegates to STELlogyDispatch.
 * ================================================================ */
static STEL_int interactiveMenu(STEL_void) {
	STEL_char optBuf[4];
	/* Buffers for up to 4 extra file arguments */
	STEL_char arg2[512], arg3[512], arg4[512], arg5[512];
	STEL_string menuArgv[8];
	STEL_int menuArgc;
	STEL_char option;

	showMenu();

	if (!fgets(optBuf, (STEL_int)sizeof(optBuf), stdin)) return EXIT_SUCCESS;
	optBuf[strcspn(optBuf, "\r\n")] = '\0';
	if (optBuf[0] == '\0') return EXIT_SUCCESS;

	option = optBuf[0];

	/* Initialise buffers and base argv */
	memset(arg2, 0, sizeof(arg2));
	memset(arg3, 0, sizeof(arg3));
	memset(arg4, 0, sizeof(arg4));
	memset(arg5, 0, sizeof(arg5));
	menuArgv[0] = (STEL_string)"STELlogy";
	menuArgv[1] = optBuf;
	menuArgc    = 2;

	printf("\n");

	/* Prompt for required / optional arguments based on chosen option */
	switch (option) {

	/* ---- Cell: encode_mode  source_file  output_file ---- */
	case PGM_CELL:
		promptFile("Encode mode  (1=encode, 0=decode)", arg2, (STEL_int)sizeof(arg2), "1");
		if (!promptFile("Source file", arg3, (STEL_int)sizeof(arg3), "")) {
			printf("  Source file is required.\n"); return EXIT_FAILURE;
		}
		if (!promptFile("Output file", arg4, (STEL_int)sizeof(arg4), "")) {
			printf("  Output file is required.\n"); return EXIT_FAILURE;
		}
		menuArgv[2] = arg2; menuArgv[3] = arg3; menuArgv[4] = arg4;
		menuArgc = 5;
		break;

	/* ---- Help ---- */
	case PGM_HELP:
	case PGM_HELP_LONG:
		return helpOption();

	default:
		printf("  Unknown option '%c'.\n", option);
		printf("  Use '?' or 'H' for help.\n");
		return EXIT_FAILURE;
	}

	printf("\n");
	return STELlogyDispatch(menuArgc, menuArgv);
}

STEL_int main(int argc, char** argv) {

	STEL_int i;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	/* No argument: show interactive menu instead of just printing usage */
	if (argc < 2) {
		return interactiveMenu();
	}
	return STELlogyDispatch(argc, argv);
}

/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
* Author: Paulo Sousa
* History/Versions: Ver S23
* Called functions: fopen(), fgets(), fclose()
* Parameters: -
* Return value: (Null)
* Algorithm: Prints the content of DEFAULT_LOGO_FILE ("logo.txt") when
*            that file exists next to the executable; otherwise falls
*            back to the built-in ASCII-art logo below, so the program
*            still shows a logo even if the file is missing.
*************************************************************
*/

STEL_void printLogo() {
	FILE *fp = fopen(DEFAULT_LOGO_FILE, "r");
	STEL_char line[256];

	if (fp) {
		while (fgets(line, (STEL_int)sizeof(line), fp))
			fputs(line, stdout);
		fclose(fp);
		return;
	}

	/* Built-in fallback, used only when DEFAULT_LOGO_FILE is not found */
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"\t###########################################\n",
		"\t#                                         #\n",
		"\t#                  .+*+.                  #\n",
		"\t#                .*#####*.                #\n",
		"\t#               *#########*               #\n",
		"\t#              +###########*              #\n",
		"\t#              *####*+*####*              #\n",
		"\t#               *###. .###*               #\n",
		"\t#                +##. .##+                #\n",
		"\t#                 *#####*                 #\n",
		"\t#               .+#######*.               #\n",
		"\t#              *###########*              #\n",
		"\t#             +#############+             #\n",
		"\t#              *###########*              #\n",
		"\t#               *#########*               #\n",
		"\t#                 +#####*                 #\n",
		"\t#                   *+                    #\n",
		"\t#                                         #\n",
		"\t#####     S T E L C O M P I L E R     #####\n",
		"\t#                                         #\n",
		"\t########        S T E L L A        ########\n",
		"\t#                                         #\n",
		"\t###########################################\n"
	);
}

/*
************************************************************
* Error printing function with variable number of arguments
* Params: Trait arguments, using formats from C language.
*	- Internal vars use list of arguments and types from stdarg.h
*   - NOTE: The format is using signature from C Language
************************************************************
*/

STEL_void errorPrint(STEL_string fmt, ...) {
	/* Initialize variable list */
	va_list ap;
	va_start(ap, fmt);

	(STEL_void)vfprintf(stderr, fmt, ap);
	va_end(ap);

	/* Move to new line */
	if (strchr(fmt, '\n') == NULL)
		fprintf(stderr, "\n");
}

