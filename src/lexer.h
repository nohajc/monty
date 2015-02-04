#ifndef __LEXER__
#define __LEXER__

#include "reader.h"

#define MAXIDENT 64
#define MAXSTR 1024

typedef enum{
	LETTER, DIGIT, WHITE, END, OTHER
} char_type_t;

typedef enum{
	kwAND, kwAS, kwASSERT, kwBREAK, kwCLASS, kwCONTINUE, kwDEF, kwDEL, kwELIF, kwELSE, kwEXCEPT, kwEXEC,
	kwFINALLY, kwFOR, kwFROM, kwGLOBAL, kwIF, kwIMPORT, kwIN, kwIS, kwLAMBDA, kwNOT, kwOR, kwPASS, kwRAISE,
	kwRETURN, kwTRY, kwWHILE, kwWITH, kwYIELD, IDENT, OP, BOOL, NONE, STRING, INT, FLOAT, PAR, CURLY, BRAC, 
	COMMA, SEMICOL, END
} tok_type_t;


#define IS_CMP(x) ((x) & 8)
#define IS_ASS(x) ((x) & 16)
#define IS_BIN(x) ((x) & 64)
#define IS_LOG(x) ((x) & 128)

typedef enum{
	PLUS, MINUS, MUL, DIV, MOD, EXP, FDIV,
	EQ = 8, NE, GT, LT, GE, LE,
	ASSIGN = 16, ADD, SUB, MULEQ, DIVEQ, MODEQ, EXPEQ, FDIVEQ,
	INC = 48, DEC,
	AND = 64, OR, XOR, INV, SHL, SHR,
	LAND = 128, LOR, NOT
} operator_t;

typedef enum{
	LEFT, RIGHT
} par_t;

typedef struct{
	tok_type_t type;
	union{
		char * ident;
		char * s_value;
		uint64_t i_value;
		double f_value;
		operator_t op;
		par_t par;
	} attr;
} tok_t;

const struct{
	char * name;
	tok_type_t type;
} kw_table[] = {
	{"and", kwAND},
	{"as", kwAS},
	{"assert", kwASSERT},
	{"break", kwBREAK},
	{"class", kwCLASS},
	{"continue", kwCONTINUE},
	{"def", kwDEF},
	{"del", kwDEL},
	{"elif", kwELIF},
	{"else", kwELSE},
	{"except", kwEXCEPT},
	{"exec", kwEXEC},
	{"finally", kwFINALLY},
	{"for", kwFOR},
	{"from", kwFROM},
	{"global", kwGLOBAL},
	{"if", kwIF},
	{"import", kwIMPORT},
	{"in", kwIN},
	{"is", kwIS},
	{"lambda", kwLAMBDA},
	{"not", kwNOT},
	{"or", kwOR},
	{"pass", kwPASS},
	{"raise", kwRAISE},
	{"return", kwRETURN},
	{"try", kwTRY},
	{"while", kwWHILE},
	{"with", kwWITH},
	{"yield", kwYIELD},
	{"True", BOOL},
	{"False", BOOL},
	{"None", NONE}
};

#endif

typedef struct{
	reader_t r;
	char * stringpool;
	char * str_ptr;
	uint32_t strp_size;
	uint32_t strp_left;
} lexer_t;

int lexer_init(lexer_t * lex, const char * fname);
tok_t lexer_next_token(lexer_t * lex);
