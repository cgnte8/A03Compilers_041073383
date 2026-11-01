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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Tissue/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <ctype.h>
#include <string.h>

#ifndef BIOLOGY_H_
#include "../include/Biology.h"
#endif

#ifndef CELL_H_
#include "../include/Step1Cell.h"
#endif

#ifndef TISSUE_H_
#include "../include/Step2Tissue.h"
#endif

/*
***********************************************************
* Function name: tissueCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

TissuePointer tissueCreate(STEL_int size) {
	TissuePointer tissuePointer;
	STEL_int count = 0;
	/* Defensive programming */
	if (size == 0 || size<0 || size>TISSUE_MAX_SIZE)
		return NULL;

	tissuePointer = (TissuePointer)calloc(1, sizeof(Tissue));

	if (tissuePointer == NULL)
		return NULL;

	tissuePointer->content = (STEL_string)malloc(size);

	if (!tissuePointer->content) {
		free(tissuePointer);
		return NULL;
	}
	tissuePointer->content[0] = TISSUE_TERMINATOR;
	/* Initialize the histogram */
	for (count = 0; count < NCHAR; count++) {
		tissuePointer->histogram[count] = 0;
	}
	/* Initialize errors */
	tissuePointer->numReaderErrors = 0;
	/* Update the properties */
	tissuePointer->size = size;
	if (tissuePointer->size == 0 || tissuePointer->size < 0 || tissuePointer->size > TISSUE_MAX_SIZE) {
		free(tissuePointer);
		return NULL;
	}
	/* Update the properties */
	tissuePointer->position.read = 0;  /* Read position */
	tissuePointer->position.wrte = 0;  /* Write position */
	tissuePointer->position.mark = 0;  /* Mark position */
	/* Initialize flags */
	tissuePointer->flags.isEmpty = STEL_TRUE;  /* Tissue is empty */
	tissuePointer->flags.isFull = STEL_FALSE;  /* Tissue is not full */
	tissuePointer->flags.isRead = STEL_FALSE;  /* Tissue was not read */
	tissuePointer->flags.isMoved = STEL_FALSE; /* Tissue was not moved */
	/* The created flag must be signalized as EMP */
	return tissuePointer;
}


/*
***********************************************************
* Function name: tissueAddCell
* Purpose: Adds a char to buffer reader
* Parameters:
*   tissuePointer = pointer to Tissue Reader
*   ch = char to be added
* Return value:
*	tissuePointer (pointer to Tissue Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

TissuePointer tissueAddCell(TissuePointer const tissuePointer, STEL_char ch) {
	STEL_string tempReader = NULL;
	STEL_int newSize = 0;
	/* Defensive programming */
	if (!tissuePointer) {
		return NULL;
	}
	if (ch == CHARSEOF)
		return NULL;
	if ((ch >= NCHAR) || (ch < 0)) { // Count error if the char is out of range
		tissuePointer->numReaderErrors++;
		return NULL;
	}
	/* Reset Realocation */
	tissuePointer->flags.isMoved = STEL_FALSE;

	/* Test the inclusion of chars */
	if (tissuePointer->position.wrte * (STEL_int)sizeof(STEL_char) < tissuePointer->size) {
		tissuePointer->flags.isFull = STEL_FALSE; // Buffer is not full
	}
	else {
		tissuePointer->flags.isFull = STEL_TRUE; // Buffer is full
		newSize = tissuePointer->size * 2;
		/* Defensive programming */
		if (newSize < 0 || newSize <= tissuePointer->size) {
			return NULL;
		}
		if (DEBUG) {
		printf("%s%d%s", "\n................\n* New size: ",
			newSize, "\n................\n");
		}
		tempReader = (STEL_string)realloc(tissuePointer->content, newSize);
		if (!tempReader)
			return NULL;

		if (tempReader != tissuePointer->content) {
			tissuePointer->flags.isMoved = STEL_TRUE; // Buffer was moved
		}
		tissuePointer->content = tempReader;
		tissuePointer->size = newSize;
	}
	tissuePointer->flags.isEmpty = STEL_FALSE;
	STEL_int pos = tissuePointer->position.wrte++;
	tissuePointer->content[pos] = ch;
	
	/* Updates histogram */
	tissuePointer->histogram[(STEL_int)ch]++;
	return tissuePointer;
}

/*
***********************************************************
* Function name: tissueClear
* Purpose: Clears the buffer reader
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_bol tissueClear(TissuePointer const tissuePointer) {
	/* Defensive programming */
	if (!tissuePointer)
		return STEL_FALSE;
	tissuePointer->position.wrte = tissuePointer->position.mark = tissuePointer->position.read = 0;
	
	/* Adjust flags original */
	tissuePointer->flags.isEmpty = STEL_TRUE; // Tissue is empty
	return STEL_TRUE;
}

/*
***********************************************************
* Function name: tissueFree
* Purpose: Releases the buffer address
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_bol tissueFree(TissuePointer const tissuePointer) {
	/* Defensive programming */
	if (!tissuePointer)
		return STEL_FALSE;
	
	/* Free pointers */
	free(tissuePointer->content);
	free(tissuePointer);
	return STEL_TRUE;
}

/*
***********************************************************
* Function name: tissueIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_bol tissueIsFull(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer)
		return STEL_FALSE;
	
	/* Check flag if buffer is FUL */
	if (tissuePointer->position.wrte== tissuePointer->size)
		return STEL_TRUE;
	return STEL_FALSE;
}


/*
***********************************************************
* Function name: tissueIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_bol tissueIsEmpty(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer)
		return STEL_FALSE;
	
	/* Check flag if buffer is EMP */
	if (tissuePointer->position.wrte==0)
		return STEL_TRUE;
	return STEL_FALSE;
}

/*
***********************************************************
* Function name: tissueSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   tissuePointer = pointer to Tissue Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_bol tissueSetMark(TissuePointer const tissuePointer, STEL_int mark) {
	
	/* Defensive programming */
	if (!tissuePointer || mark<0 || mark > tissuePointer->position.wrte)
		return STEL_FALSE;
	
	/* Adjust mark */
	tissuePointer->position.mark = mark;
	return STEL_TRUE;
}


/*
***********************************************************
* Function name: tissuePrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_int tissuePrint(TissuePointer const tissuePointer) {
	STEL_int cont = 0;
	STEL_char c;
	
	/* Defensive programming (including invalid chars) */
	if (!tissuePointer || !(tissuePointer->content))
		return TISSUE_ERROR;
	c = tissueGetCell(tissuePointer);
	
	/* Check flag if buffer EOB has achieved */
	while (cont < tissuePointer->position.wrte) {
		cont++;
		printf("%c", c);
		c = tissueGetCell(tissuePointer);
	}
	return cont;
}

/*
***********************************************************
* Function name: tissueLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/ //xxx
STEL_int tissueLoad(TissuePointer const tissuePointer, 
						STEL_string fileName) {
	STEL_int size = 0;
	STEL_int i;
	
	/* Defensive programming */
	if (!fileName|| !tissuePointer)
		return TISSUE_ERROR;
	STEL_string output = cellTransformMem(fileName, STR_CompilerName, TRANSLATE);
	
	/* cellTransformMem returns NULL (INVALID) when the file cannot be
	 * opened or read; without this check, strlen(NULL) below would
	 * crash instead of reporting a clean TISSUE_ERROR. */
	if (!output)
		return TISSUE_ERROR;
	size = (STEL_int)strlen(output);
	for (i = 0; i < size; i++) {
		tissueAddCell(tissuePointer, output[i]);
	}
	return size;
}

/*
***********************************************************
* Function name: tissueLoadMem
* Purpose: Loads the buffer directly from an in-memory, already-plain
*	(not Cell-encoded) string, without touching the filesystem.
*	This is the in-memory counterpart of tissueLoad(): tissueLoad()
*	reads an encoded FILE and decodes it through cellTransformMem();
*	tissueLoadMem() instead takes plain text that is already sitting
*	in memory (e.g. a test fixture string) and copies it straight
*	into the Tissue buffer, cell by cell - no temporary files, no
*	encode/decode round-trip needed.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
*   content       = pointer to a NUL-terminated, already-plain string
* Return value:
*	Number of chars copied into the buffer, or TISSUE_ERROR.
*************************************************************
*/
STEL_int tissueLoadMem(TissuePointer const tissuePointer, const STEL_string content) {
	STEL_int size;
	STEL_int i;
	if (!content || !tissuePointer)
		return TISSUE_ERROR;
	size = (STEL_int)strlen(content);
	for (i = 0; i < size; i++) {
		tissueAddCell(tissuePointer, content[i]);
	}
	return size;
}

/*
***********************************************************
* Function name: tissueRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_bol tissueRecover(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer)
		return STEL_FALSE;
	
	/* Recover positions */
	tissuePointer->position.read = tissuePointer->position.mark = 0;
	return STEL_TRUE;
}


/*
***********************************************************
* Function name: tissueRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_bol tissueRetract(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer || tissuePointer->position.read == 0)
		return STEL_FALSE;
	
	/* Retract (return 1 pos read) */
	tissuePointer->position.read--;
	return STEL_TRUE;
}


/*
***********************************************************
* Function name: tissueRestore
* Purpose: Resets the buffer.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_bol tissueRestore(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer)
		return STEL_FALSE;
	
	/* Restore positions (read/mark) */
	tissuePointer->position.read = tissuePointer->position.mark;
	return STEL_TRUE;
}



/*
***********************************************************
* Function name: tissueGetCell
* Purpose: Returns the char in the getC position.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_char tissueGetCell(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer)
		return TISSUE_ERROR;
	
	/* Check condition to read/wrte */
	if (tissuePointer->position.read == tissuePointer->position.wrte) {
		
		/* Set EOB flag */
		tissuePointer->flags.isRead = STEL_TRUE; // EOB
		return TISSUE_TERMINATOR;
	}
	/* Reset EOB flag */
	tissuePointer->flags.isRead = STEL_FALSE; // !EOB
	return tissuePointer->content[tissuePointer->position.read++];
}


/*
***********************************************************
* Function name: tissueGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
*   pos = position to get the pointer
* Return value:
*	Locus of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_string tissueGetContent(TissuePointer const tissuePointer, STEL_int pos) {
	
	/* Defensive programming */
	if (!tissuePointer || pos < 0 || pos > tissuePointer->position.wrte)
		return NULL;
	
	/* Return content (string) */
	return tissuePointer->content + pos;
}



/*
***********************************************************
* Function name: tissueGetPosAccess
* Purpose: Returns the value of getCPosition.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_int tissueGetPosAccess(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer)
		return TISSUE_ERROR;
	
	/* Return read */
	return tissuePointer->position.read;
}


/*
***********************************************************
* Function name: tissueGetPosStore
* Purpose: Returns the position of char to be added
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_int tissueGetPosStore(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer)
		return TISSUE_ERROR;
	
	/* Return wrte */
	return tissuePointer->position.wrte;
}


/*
***********************************************************
* Function name: tissueGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_int tissueGetPosMark(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer)
		return TISSUE_ERROR;
	
	/* Return mark */
	return tissuePointer->position.mark;
}


/*
***********************************************************
* Function name: tissueGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_int tissueGetSize(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer)
		return TISSUE_ERROR;
	
	/* Return size */
	return tissuePointer->size;
}

/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Tissue.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Flags from Tissue.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
STEL_void tissuePrintVitals(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer)
		return;
	
	/* Return flags */
	printf("Vitals.isEmpty = %d\n", tissuePointer->flags.isEmpty);
	printf("Vitals.isFull = %d\n", tissuePointer->flags.isFull);
	printf("Vitals.isMoved = %d\n", tissuePointer->flags.isMoved);
	printf("Vitals.isRead = %d\n", tissuePointer->flags.isRead);
}
#else
#define bGetFlags(tissuePointer) ((tissuePointer)?(tissuePointer->flags):(RT_FAIL_1))
#endif

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_void tissuePrintStat(TissuePointer const tissuePointer) {
	STEL_int numChars = 0;
	STEL_int count = 0;
	STEL_int sep = 0;
	
	/* Defensive programming */
	if (tissuePointer) {
		
		/* Updates the histogram */
		for (count = 0; count < NCHAR; count++) {
			if (tissuePointer->histogram[count] > 0) {
				if (!isspace(count))
					printf("%s%c%s%d%s", "B[", count, "]=", tissuePointer->histogram[count], ", ");
				else
					printf("%s%d%s", "B[ ]=", tissuePointer->histogram[count], ", ");
				if (sep < 9) {
					sep++;
				}
				else {
					printf("%s", "\n");
					sep = 0;
				}
			}
		}
	}
	printf("%s", "\n");
}

/*
***********************************************************
* Function name: tissueNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
STEL_int tissueNumErrors(TissuePointer const tissuePointer) {
	
	/* Defensive programming */
	if (!tissuePointer)
		return TISSUE_ERROR;
	
	/* Updates the histogram */
	return tissuePointer->numReaderErrors;
}

/*
***********************************************************
* Function name: tissueChecksum
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   tissuePointer = pointer to Tissue Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

STEL_int tissueChecksum(TissuePointer tissuePointer) {
	STEL_char c;
	STEL_int sum = 0;
	STEL_int pos = 0;
	if (tissuePointer) {
		for (pos = 0; pos < tissuePointer->position.wrte; pos++) {
			c = tissuePointer->content[pos];
			sum += (STEL_char)c;
		}
		tissuePointer->checkSum = sum;
	}
	return sum;
}



















