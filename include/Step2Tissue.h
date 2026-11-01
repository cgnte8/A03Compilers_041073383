/*
************************************************************
* BIOSYSTEMS COURSE - Algonquin College
* Code version: Fall, 2026
* Author: Connor Guenette - 04107333
* Professors: Paulo Sousa
************************************************************
#
#
# ECHO "=---------------------------------------="
# ECHO "|  BIOSYSTEMS - ALGONQUIN COLLEGE (26F) |"
# ECHO "=---------------------------------------="
# ECHO " ####################################### "
# ECHO " #                .:*-.                # "
# ECHO " #              :%#**#%#               # "
# ECHO " #              *#....+%+              # "
# ECHO " #              *#....=%+              # "
# ECHO " #               #+:.-%#               # "
# ECHO " #                +*.%+                # "
# ECHO " #                *%.*+                # "
# ECHO " #               ##...#*               # "
# ECHO " #               #.....#               # "
# ECHO " #                *+.%%-               # "
# ECHO " #                 #%#:                # "
# ECHO " #               +%#..#+               # "
# ECHO " #              =#*...*%:              # "
# ECHO " #               =.....%               # "
# ECHO " @             -###*#%%##:             @ "
# ECHO " @            %@%%%%%%%%%@%            @ "
# ECHO " ####################################### "
# ECHO " #####    B I O C O M P I L E R    ##### "
# ECHO " ####################################### "
*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef BIOLOGY_H_
#include "Biology.h"
#endif

#ifndef TISSUE_H_
#define TISSUE_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Constants about controls (not need to change) */
#define TISSUE_ERROR				(-1)	/* General error message */
#define TISSUE_TERMINATOR			'\0'	/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME .................................. */

/* You should add your own constant definitions here */
#define TISSUE_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define TISSUE_DEFAULT_SIZE			250		/* default initial buffer reader capacity */

/* TO_DO: Add your bit-masks constant definitions here */
// BITS                             (    76543210)
#define TISSUE_DEFAULT_FLAG 0x00 	// (0b00000000) = (0x00)_16 = (000)_10
// TO_DO: BIT 3: END = End of buffer flag
#define TISSUE_SET_FLAG_END 0x08	// (0b00001000) = (0x08)_16 = (008)_10
// TO_DO: BIT 2: REL = Rellocation memory flag
#define TISSUE_SET_FLAG_REL 0x04	// (0b00000100) = (0x04)_16 = (004)_10
// TO_DO: BIT 1: EMP = Tissue empty flag
#define TISSUE_SET_FLAG_FUL 0x02	// (0b00000010) = (0x02)_16 = (002)_10
// TO_DO: BIT 0: FUL = Tissue full flag
#define TISSUE_SET_FLAG_EMP 0x01	// (0b00000001) = (0x01)_16 = (001)_10

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	STEL_int wrte;			/* the offset to the add chars (in chars) */
	STEL_int read;			/* the offset to the get a char position (in chars) */
	STEL_int mark;			/* the offset to the mark position (in chars) */
} Locus;

/* Flags declaration */
typedef struct flag {
	STEL_bol isEmpty; /* checks if there is no content */
	STEL_bol isFull; /* the content is using all size */
	STEL_bol isRead; /* all content was read */
	STEL_bol isMoved; /* the content was moved in reallocation */
} Vitals;

/* Tissue structure */
typedef struct bufferReader {
	STEL_string		content;			/* pointer to the beginning of character array (character buffer) */
	STEL_int		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	Vitals			flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Locus		position;			/* Offset / position field */
	STEL_int		histogram[NCHAR];	/* Statistics of chars */
	STEL_int		numReaderErrors;	/* Number of errors from Reader */
	STEL_int		checkSum;			/* Sum of bytes(chars) */
} Tissue, *TissuePointer;

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
TissuePointer	tissueCreate		(STEL_int);
TissuePointer	tissueAddCell		(TissuePointer const, STEL_char);
STEL_bol		tissueClear		    (TissuePointer const);
STEL_bol		tissueFree		    (TissuePointer const);
STEL_bol		tissueIsFull		(TissuePointer const);
STEL_bol		tissueIsEmpty		(TissuePointer const);
STEL_bol		tissueSetMark		(TissuePointer const, STEL_int);
STEL_int		tissuePrint		    (TissuePointer const);
STEL_int		tissueLoad			(TissuePointer const, STEL_string);
STEL_int		tissueLoadMem		(TissuePointer const, const STEL_string);
STEL_bol		tissueRecover		(TissuePointer const);
STEL_bol		tissueRetract		(TissuePointer const);
STEL_bol		tissueRestore		(TissuePointer const);
STEL_int		tissueChecksum		(TissuePointer const);
/* Getters */
STEL_char		tissueGetCell		(TissuePointer const);
STEL_string		tissueGetContent	(TissuePointer const, STEL_int);
STEL_int		tissueGetPosAccess	(TissuePointer const);
STEL_int		tissueGetPosStore	(TissuePointer const);
STEL_int		tissueGetPosMark	(TissuePointer const);
STEL_int		tissueGetSize		(TissuePointer const);
STEL_void		tissuePrintVitals	(TissuePointer const);
STEL_void		tissuePrintStat		(TissuePointer const);
STEL_int		tissueNumErrors		(TissuePointer const);

#endif
