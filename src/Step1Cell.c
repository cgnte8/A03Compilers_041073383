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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef STELLA_H_
#include "../include/Stella.h"
#endif

#ifndef CELL_H_
#include "../include/Step1Cell.h"
#endif

// Function to perform the Vigen�re cipher (encoding or decoding)
STEL_void cellTransformFile(const STEL_string inputFileName, const STEL_string outputFileName, const STEL_string key, STEL_int encode) {
    FILE* inputFile = fopen(inputFileName, "r");
    FILE* outputFile = fopen(outputFileName, "w");
    if (!inputFile || !outputFile) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    STEL_int keyLength = (int)strlen(key);
    STEL_int keyIndex = 0;
    STEL_char inputChar, keyChar, outputChar;
    // Process each character in the file
    while ((inputChar = fgetc(inputFile)) != EOF) {
        if (inputChar >= ASCII_START && inputChar <= ASCII_END) {
            keyChar = key[keyIndex % keyLength] - ASCII_START; // Convert key char to range 0-(ASCII_RANGE-1)
            if (encode) {
                // Encode the character
                outputChar = ASCII_START + (inputChar - ASCII_START + keyChar) % ASCII_RANGE;
            }
            else {
                // Decode the character
                outputChar = ASCII_START + (inputChar - ASCII_START - keyChar + ASCII_RANGE) % ASCII_RANGE;
            }
            keyIndex++;
        }
        else {
            // Non-visible ASCII characters remain unchanged
            outputChar = inputChar;
        }
        fputc(outputChar, outputFile);
    }
    fclose(inputFile);
    fclose(outputFile);
}

// Function to perform the Vigen�re cipher (encoding or decoding)
STEL_string cellTransformMem(const STEL_string inputFileName, const STEL_string key, STEL_int encode) {
    FILE* inputFile = fopen(inputFileName, "r");
	STEL_int size = cellFileSize(inputFileName);
	STEL_string output = (STEL_string)malloc(size + 1);
	if (!output) {
		perror("Error allocating memory");
		return INVALID;
	}
    STEL_int pos = 0;
    if (!inputFile) {
        perror("Error opening file");
        return INVALID;
    }
    STEL_int keyLength = (int)strlen(key);
    STEL_int keyIndex = 0;
    STEL_char inputChar, keyChar, outputChar;
    // Process each character in the file
    while ((inputChar = fgetc(inputFile)) != EOF) {
        if (inputChar >= ASCII_START && inputChar <= ASCII_END) {
            keyChar = key[keyIndex % keyLength] - ASCII_START; // Convert key char to range 0-(ASCII_RANGE-1)
            if (encode) {
                // Encode the character
                outputChar = ASCII_START + (inputChar - ASCII_START + keyChar) % ASCII_RANGE;
            }
            else {
                // Decode the character
                outputChar = ASCII_START + (inputChar - ASCII_START - keyChar + ASCII_RANGE) % ASCII_RANGE;
            }
            keyIndex++;
        }
        else {
            // Non-visible ASCII characters remain unchanged
            outputChar = inputChar;
        }
		output[pos++] = outputChar;
    }
    fclose(inputFile);
    output[pos] = EOF_CHAR;
	return output;
}

// Function to encode (cellEncode)
STEL_void cellEncode(const STEL_string inputFileName, const STEL_string outputFileName, const STEL_string key) {
    cellTransformFile(inputFileName, outputFileName, key, TRANSCRIBE);
}

// Function to decode (cellDecode)
STEL_void cellDecode(const STEL_string inputFileName, const STEL_string outputFileName, const STEL_string key) {
    cellTransformFile(inputFileName, outputFileName, key, TRANSLATE);
}

STEL_string cellReadFile(const STEL_char* filename) {
    FILE* file = fopen(filename, "r");
    STEL_string buffer;
    STEL_int length;
    size_t bytesRead;
    if (file == NULL) {
        return NULL; // File couldn't be opened
    }
    // Seek to end to get file size
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    rewind(file);
    // Allocate memory for file content plus null terminator. length is
    // the RAW on-disk byte count, which is only an UPPER BOUND on how
    // many characters fread() will actually deliver: opened in text
    // mode ("r"), platforms that perform "\r\n" -> "\n" translation
    // (e.g. Windows) can return fewer bytes than length whenever the
    // file has any CRLF line endings -- as Cell-encoded files do,
    // since the cipher passes '\r'/'\n' through unchanged.
    buffer = (STEL_char*)malloc(length + 1);
    if (buffer == NULL) {
        fclose(file);
        return NULL; // Memory allocation failed
    }
    // Read file into buffer, then null-terminate at the ACTUAL number
    // of bytes fread() delivered (bytesRead), not at the pre-computed
    // length -- otherwise, whenever bytesRead < length (see above),
    // the buffer's unfilled tail is leftover uninitialized heap memory,
    // and the terminator lands past the end of the real string, so
    // printing it shows the real content followed by stray garbage.
    bytesRead = fread(buffer, 1, length, file);
    buffer[bytesRead] = '\0'; // Null-terminate string
    fclose(file);
    return buffer;
}

// Get file size
STEL_int cellFileSize(const STEL_string filename) {
	STEL_int size = 0;
    FILE* file = fopen(filename, "rb"); // Open file in binary mode
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return -1;
    }
    fseek(file, 0, SEEK_END);  // Move to end of file
    size = ftell(file);   // Get current position (file size)
    fclose(file);
    return size;
}
