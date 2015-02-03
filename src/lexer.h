#ifndef __LEXER__
#define __LEXER__

#define MAXIDENT 64
#define MAXSTR 1024

typedef enum{
	kwAND, kwAS, kwASSERT, kwBREAK, kwCLASS, kwCONTINUE, kwDEF, kwDEL, kwELIF, kwELSE, kwEXCEPT, kwEXEC,
	kwFINALLY, kwFOR, kwFROM, kwGLOBAL, kwIF, kwIMPORT, kwIN, kwIS, kwLAMBDA, kwNOT, kwOR, kwPASS, kwRAISE,
	kwRETURN, kwTRY, kwWHILE, kwWITH, kwYIELD, IDENT, OP, BOOL, NONE, STRING, INT, FLOAT, PAR, CURLY, BRAC, 
	COMMA, SEMICOL
} tok_type_t;

typedef struct{
	tok_type_t type;
	union{
		char ident[MAXIDENT];
		char s_value[MAXSTR];
		uint64_t i_value;
		double f_value;
		operator_t op;
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
	{"yield", kwYIELD}
};

#endif

int lexer_init(reader_t * r, const char * fname);