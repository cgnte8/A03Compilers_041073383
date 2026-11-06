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

#ifndef STELLA_H_
#include "Stella.h"
#endif

#ifndef CELL_H_
#define CELL_H_

 // ASCII range for visible characters
#define ASCII_START 32
#define ASCII_END 126
#define ASCII_RANGE (ASCII_END - ASCII_START + 1)

#define TRANSCRIBE 1
#define TRANSLATE 0

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
STEL_void cellTransformFile(const STEL_string inputFileName, const STEL_string outputFileName, const STEL_string key, STEL_int encode);
STEL_string cellTransformMem(const STEL_string inputFileName, const STEL_string key, STEL_int encode);

STEL_void cellEncode(const STEL_string inputFileName, const STEL_string outputFileName, const STEL_string key);
STEL_void cellDecode(const STEL_string inputFileName, const STEL_string outputFileName, const STEL_string key);
STEL_int cellFileSize(const STEL_string filename);	
STEL_string cellReadFile(const STEL_char* filename);

#endif

