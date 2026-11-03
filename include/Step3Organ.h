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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: Jan 01 2025
* Purpose: Scanner (Organ) header — structures, enums, prototypes only.
*           Keywords and DFA data live in compiler_scanner_dfa.csv.
* Function list: (...).
*************************************************************/

#ifndef BIOLOGY_H_
#include "Biology.h"
#endif

#ifndef TISSUE_H_
#include "Step2Tissue.h"
#endif

#ifndef ORGAN_H_
#define ORGAN_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20   /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1   /* Value for run-time error */
#define BIO_INT_MAX 32767   /* largest integer literal/value (2 bytes)            */
#define MAX_LEXEME  1024    /* longest lexeme accepted (e.g. a string literal)    */
#define CHAR_CLASSES 12     /* character classes = transition columns of the DFA  */

/* TO_DO: Define the number of tokens */
#define NUM_SIGNALS 24

/* Default data-file paths (DEFAULT_DFA_FILE, DEFAULT_GRAMMAR_FILE,
 * DEFAULT_RUNNER_CONFIG) are defined once, in Biology.h, and reused
 * here - do not redefine them per-stage. */

/* TO_DO: Define Signal codes - Create your token classes */
enum SIGNALS {
	ERR_S,		/* 0: Error token */
	IVID_S,		/* 1: integer variable identifier - ends with '$' */
	FVID_S,		/* 2: float variable identifier - ends with '%' */
	SVID_S,		/* 3: string variable identifier - ends with '@' */
	MNID_S,		/* 4: method name identifier - ends with '&' */
	INL_S,		/* 5: Integer literal token */
	FPL_S,		/* 6: Floating point literal token */
	STR_S,		/* 7: String literal token */
	SCC_OP_S,	/* 8: String concatenation operator token: (++) */
	ASS_OP_S,	/* 9: Assignment operator token */
	ART_OP_S,	/* 10: Arithmetic operator token */
	REL_OP_S,	/* 11: Relational operator token */
	LOG_OP_S,	/* 12: Logical operator token */
	LPR_S,		/* 13: Left parenthesis token */
	RPR_S,		/* 14: Right parenthesis token */
	LBR_S,		/* 15: Left brace token */
	RBR_S,		/* 16: Right brace token */
	KW_S,		/* 17: Keyword token */
	COM_S,		/* 18: Comma token */
	EOS_S,		/* 19: End of statement (semicolon) */
	RTE_S,		/* 20: Run-time error token */
	CMT_S,		/* 21: Comment token */
	SEOF_S,		/* 22: Source end-of-file token */
	DICE_S		/* 23: Dice token */
};

/* TO_DO: Define the list of keywords */
extern STEL_string signalStrTable[NUM_SIGNALS];   /* names of the Signals (Step3Organ.c) */

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_POW } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT, OP_LE, OP_GE } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union SignalAttribute {
	STEL_int codeType;				/* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	STEL_int intValue;				/* integer literal attribute (value) */
	STEL_int keywordIndex;			/* keyword index in the keyword table */
	STEL_int contentString;			/* string literal offset (stringLiteralTable->content) */
	STEL_float floatValue;				/* floating-point literal attribute (value) */
	STEL_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	STEL_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} SignalAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	STEL_byte flags;				/* Flags information */
	union {
		STEL_int intValue;		/* Integer value */
		STEL_float floatValue;		/* Float value */
		STEL_void* stringContent;	/* String value */
	} values;
} IdAttibutes;

/* Signal declaration */
typedef struct Signal {
	STEL_int code;				/* token code */
	SignalAttribute attribute;		/* token attribute */
	IdAttibutes idAttribute;		/* for further use */
} Signal;

/* Scanner */
typedef struct scannerData {
	STEL_int organHistogram[NUM_SIGNALS];	/* Statistics of chars */
} OrganData, * pOrganData;

///////////////////////////////////////////////////////////////////////////////////////////////////////



/* TO_DO: Error states and illegal state */
#define ESNR	16		/* Error state with no retract */
#define ESWR	17		/* Error state with retract */
#define FS		18		/* Final state */


/* TO_DO: State transition table definition */
#define SPEC_CHARS	27

#define EOS_CHR '\0'	// CH00
#define EOF_CHR (-1)	// CH01
#define DOL_CHR '$'		// CH02
#define PTG_CHR '%'		// CH03
#define ATT_CHR '@'		// CH04
#define AMP_CHR '&'		// CH05
#define HST_CHR '#'		// CH06
#define UND_CHR '_'		// CH07
#define DOT_CHR '.'		// CH08
#define COM_CHR ','		// CH09
#define SCL_CHR ';'		// CH10
#define QUT_CHR '"'		// CH11
#define SPC_CHR ' '		// CH12
#define TAB_CHR '\t'	// CH13
#define NWL_CHR '\n'	// CH14
#define FRF_CHR '\f'	// CH15
#define LTO_CHR '<'		// CH16
#define GTO_CHR '>'		// CH17
#define EQO_CHR '='		// CH18
#define LPR_CHR '('		// CH19
#define RPR_CHR ')'		// CH20
#define LBR_CHR '{'		// CH21
#define RBR_CHR '}'		// CH22
#define ADD_CHR '+'		// CH23
#define SUB_CHR '-'		// CH24
#define MUL_CHR '*'		// CH25
#define DIV_CHR '/'		// CH26
#define POW_CHR '^'		// CH27

/* Boundary characters (statement / file terminators) — fixed structural constants */
#define BOUNDARY_TABSIZE 2
extern STEL_char boundaryTable[BOUNDARY_TABSIZE];

/* Keywords are loaded at runtime from compiler_scanner_dfa.csv [KEYWORDS].
 * KWT_SIZE is the live count after loadScannerDFA(); MAX_KEYWORDS is the cap. */
#define MAX_KEYWORDS 64
extern STEL_int KWT_SIZE;
extern STEL_string keywordTable[MAX_KEYWORDS];

/* About indentation (useful for positional languages (ex: Python, Cobol) */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */
#define INDENT '\t'  /* Tabulation */

 /* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	STEL_char indentationCharType;
	STEL_int indentationCurrentPos;
} LanguageAttributes;

/* Number of errors */
extern STEL_int numOrganErrors;          /* lexical errors (Step3Organ.c) */

/* Scanner data */
extern OrganData organData;              /* statistics (Step3Organ.c)     */
extern TissuePointer stringLiteralTable; /* string literals (Step3Organ.c) */
extern STEL_int errorNumber;             /* run-time error (Step3Organ.c)  */
extern STEL_int lexemeLine;              /* line where the last Signal began */

/* ====================================================================
 * Organ  declarations (CSV-driven scanner)
 * ==================================================================== */
#define MAX_STATES_  64
#define MAX_CLASSES_ 16
#define NOFS_  0
#define FSNR_  1
#define FSWR_  2

typedef enum {
    ACT_NULL = 0,
    ACT_IDENTIFIER,
    ACT_KEYWORD,
    ACT_INTEGER,
    ACT_FLOAT,
    ACT_STRING,
    ACT_COMMENT,
    ACT_ERROR,
	ACT_DICE
} DFAAction;

/* Accepting-state functions - called by the  scanner action dispatch */
Signal signalIdentifier    (STEL_string lexeme);
Signal signalIntegerLiteral(STEL_string lexeme);
Signal signalFloatLiteral  (STEL_string lexeme);
Signal signalStringLiteral (STEL_string lexeme);
Signal signalKeyword       (STEL_string lexeme);
Signal signalError         (STEL_string lexeme);
Signal signalComment       (STEL_string lexeme);
Signal signalDice		   (STEL_string lexeme);

STEL_int loadScannerDFA  (const STEL_string csvFile);
STEL_int startOrgan    (TissuePointer psc_buf);
Signal   organScan     (STEL_void);
STEL_void printOrganData(STEL_void);

#endif
