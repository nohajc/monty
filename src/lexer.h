#ifndef __LEXER__
#define __LEXER__

#include "reader.h"

#define MAXIDENT 64
#define MAXSTR 1024
#define MAXKW 16

typedef enum{
	LETTER, DIGIT, UNDERSC, WHITE, END, OTHER
} char_type_t;

typedef enum{
	kwAND, kwAS, kwASSERT, kwBREAK, kwCLASS, kwCONTINUE, kwDEF, kwDEL, kwELIF, kwELSE, kwEXCEPT, kwEXEC,
	kwFINALLY, kwFOR, kwFROM, kwGLOBAL, kwIF, kwIMPORT, kwIN, kwIS, kwLAMBDA, kwNOT, kwOR, kwPASS, kwRAISE,
	kwRETURN, kwTRY, kwWHILE, kwWITH, kwYIELD, IDENT, OP, BOOL, NONE, STRING, INT, FLOAT, PAR, CURLY, BRAC, 
	COMMA, SEMICOL, DOT, NEWLINE, EOI, ERR
} tok_type_t;

extern const char tok_str[][MAXKW];

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
		int64_t i_value;
		double f_value;
		operator_t op;
		par_t par;
		int b_val;
	} attr;
} tok_t;

typedef struct{
	char name[MAXKW];
	tok_type_t type;
} keyword_t; 

extern const keyword_t kw_table[];

#define STRP_SIZE 65536

typedef struct{
	reader_t r;
	char * stringpool;
	char * str_ptr;
	uint32_t str_size;
	uint32_t str_pos;
} lexer_t;

int lexer_init(lexer_t * lex, const char * fname);
tok_t lexer_next_token(lexer_t * lex);
void lexer_destroy(lexer_t * lex);

#endif
