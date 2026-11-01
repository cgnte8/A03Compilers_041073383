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
* File name: MainReader.c
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

#ifndef BIOLOGY_H_
#include "../include/Biology.h"
#endif

#ifndef CELL_H_
#include "../include/Step1Cell.h"
#endif

#ifndef TISSUE_H_
#include "../include/Step2Tissue.h"
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
STEL_void displayTissue(TissuePointer ptr_Buffer);
STEL_long tissueFileSizeCheck(STEL_string fname);
STEL_int isNumber(const STEL_string ns);
STEL_void startTissue(STEL_string, STEL_string);

/*
************************************************************
* Main function from Tissue
* Parameters:
*   argc / argv = Parameters from command prompt
* Return value:
*	Success operation.
************************************************************
*/

STEL_int main2Tissue(STEL_int argc, STEL_string* argv) {

	/* Create source input buffer */
	STEL_string program = argv[0];
	STEL_string input = argv[2];
	STEL_int size = 0;
	///STEL_string output = (char*)malloc(strlen(input) + strlen(SUFFIX_READER) + 1);
    ///if (output == NULL) {  
    ///   errorPrint("Memory allocation failed for 'output'.");  
    ///   exit(EXIT_FAILURE);  
    ///}
    ///strcpy(output, input);
	///strcat(output, SUFFIX_READER);
	/* Missing file name or/and mode parameter */
	if (argc <= 2) {
		errorPrint("\nDate: %s  Time: %s", __DATE__, __TIME__);
		errorPrint("\nRuntime error at line %d in file %s\n", __LINE__, __FILE__);
		errorPrint("%s\b\b\b\b%s%s", argv[0], ": ", "Missing parameters.");
		errorPrint("Usage: <Option=2> <SourceFile>");
		exit(EXIT_FAILURE);
	}
	///cellDecode(input, output, STR_CompilerName);
	///startTissue(program, output);
	startTissue(program, input);

	/* Return success */
	return (EXIT_SUCCESS);
}

/*
************************************************************
* Tissue starting method
* Params:
*	- Program: Name of the program
*	- Input: Filename
*	- Mode: Operational mode
*	- Size: Tissue capacity
*	- Increment: buffer increment.
************************************************************
*/
STEL_void startTissue(STEL_string program, STEL_string input) {

	TissuePointer bufferp;		/* pointer to Tissue structure */
	STEL_int loadSize = 0;	/* the size of the file loaded in the buffer */

	/* Create buffer */
	bufferp = tissueCreate(TISSUE_DEFAULT_SIZE);

	if (bufferp == NULL) {
		errorPrint("%s%s", program, ": Cannot allocate buffer - Use: buffer <input>.");
		errorPrint("Filename: %s \n", input);
		exit(1);
	}

	/* Load source file into input buffer  */
	printf("Reading file %s ....Please wait\n", input);
	loadSize = tissueLoad(bufferp, input);

	/* If the input file has not been completely loaded, find the file size and print the last symbol loaded */
	if (loadSize == TISSUE_ERROR) {
		printf("The input file %s %s\n", input, "has not been completely loaded.");
		printf("Current size of buffer: %d.\n", tissueGetSize(bufferp));
		printf("Input file size: %ld\n", tissueFileSizeCheck(input));
	}

	/* Finishes the buffer: add end of file character (EOF) to the buffer display again */
	if ((loadSize != TISSUE_ERROR) && (loadSize != 0)) {
		if (!tissueAddCell(bufferp, TISSUE_TERMINATOR)) {
			errorPrint("%s%s%s", program, ": ", "Error in compacting buffer.");
		}
	}

	/* Prints the buffer property and content */
	displayTissue(bufferp);

	/* Free the dynamic memory used by the buffer */
	tissueFree(bufferp);
	bufferp = NULL;
}

/*
************************************************************
* Get buffer size
* Params:
*	- Filename: Name of the file
************************************************************
*/

STEL_long tissueFileSizeCheck(STEL_string fname) {
	FILE* input;
	STEL_long flength;
	input = fopen(fname, "r");
	if (input == NULL) {
		errorPrint("%s%s", "Cannot open file: ", fname);
		return 0;
	}
	fseek(input, 0L, SEEK_END);
	flength = ftell(input);
	fclose(input);
	return flength;
}

/*
************************************************************
 * Tests for decimal-digit character string
 * Params:
 *		- String to be evaluated as numeric
 * Return:
 *		- Number value: Returns nonzero (true) if ns is a number; 0 (False) otherwise
************************************************************
*/

STEL_int isNumber(const STEL_string ns) {
	STEL_char c; STEL_int i = 0;
	if (ns == NULL) return 0;
	while ((c = ns[i++]) == 0) {
		if (!isdigit(c)) return 0;
	}
	return 1;
}


/*
************************************************************
* Print function
*	- Params: buffer to print all properties.
************************************************************
*/

STEL_void displayTissue(TissuePointer ptr_Buffer) {
	printf("\nPrinting buffer parameters:\n\n");
	printf("The size of the buffer is:  %d\n",
		tissueGetSize(ptr_Buffer));
	printf("The current size of the buffer is:  %d\n",
		tissueGetPosStore(ptr_Buffer));
	printf("The first symbol in the buffer is:  %c\n",
		tissueGetPosStore(ptr_Buffer) ? *tissueGetContent(ptr_Buffer, 0) : ' ');
	printf("The value of the flags:\n");
		tissuePrintVitals(ptr_Buffer);
	printf("Checksum: %d\n", tissueChecksum(ptr_Buffer));
	printf("%s", "Reader statistics : \n");
	tissuePrintStat(ptr_Buffer);
	printf("Number of errors: %d\n",
		tissueNumErrors(ptr_Buffer));
	printf("\nPrinting buffer contents:\n\n");
	tissueRecover(ptr_Buffer);
	if (!tissuePrint(ptr_Buffer))
		printf("Empty buffer\n");
}
