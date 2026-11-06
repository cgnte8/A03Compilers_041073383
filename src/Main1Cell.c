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
*/

/*
************************************************************
* File name: MainCoder.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Tissue/Reader (A12)
* Function list: (...).
*************************************************************/

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
#include <ctype.h>

#ifndef STELLA_H_
#include "../include/Stella.h"
#endif

#ifndef CELL_H_
#include "../include/Step1Cell.h"
#endif

 /* Check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/*
 * TODO .......................................................................
 * Basically, change all datatypes to your language definitions
 * (see "Biology.h")
 */

/*
 * -------------------------------------------------------------
 *  Function declarations
 * -------------------------------------------------------------
 */

/*
************************************************************
* Coder starting method
* Params:
*	- Mode: Cypher/cellDecode
*	- Input: Filename
*   - Key: Encrypt word
*	- Output: Filename
************************************************************
*/

// Main function to handle command-line arguments
STEL_int main1Cell(STEL_int argc, STEL_string* argv) {
	if (argc < 5) {
		printf("Usage: %s [cellEncode=1|cellDecode=0] <input_file> <output_file>\n", argv[0]);
		return EXIT_FAILURE;
	}
	STEL_string operation = "";
	STEL_string inputFileName = "";
	STEL_string key = STR_CompilerName;
	STEL_string outputFileName = "";
	if (argc > 4) {
		operation = argv[2];
		inputFileName = argv[3];
		outputFileName = argv[4];
		// Call the appropriate function to file
		if (atoi(operation) == TRANSCRIBE)
			cellEncode(inputFileName, outputFileName, key);
		else if (atoi(operation) == TRANSLATE)
			cellDecode(inputFileName, outputFileName, key);
		else {
			errorPrint("%s%s%s", "Error: Unknown operation ", operation, ". Use 'cellEncode' or 'cellDecode'.\n");
			return EXIT_FAILURE;
		}
		printf("Operation '%s' completed successfully.\n", operation);
		// Preview the result by reading the output file straight back.
		STEL_string output = cellReadFile(outputFileName);
		printf("OUTPUT:\n%s\n", output);
		free(output);
	}
	return EXIT_SUCCESS;
}

