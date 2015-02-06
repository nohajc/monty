#ifndef __DEBUG__
#define __DEBUG__

#include "lexer.h"

char tok_str[][MAXKW] = {
	"kwAND", "kwAS", "kwASSERT", "kwBREAK", "kwCLASS", "kwCONTINUE", "kwDEF", "kwDEL", "kwELIF", "kwELSE", "kwEXCEPT", "kwEXEC", "kwFINALLY", "kwFOR", "kwFROM",
	"kwGLOBAL",	"kwIF", "kwIMPORT", "kwIN", "kwIS", "kwLAMBDA", "kwNOT", "kwOR", "kwPASS", "kwRAISE", "kwRETURN", "kwTRY", "kwWHILE", "kwWITH", "kwYIELD",
	"IDENT", "OP", "BOOL", "NONE", "STRING", "INT", "FLOAT", "PAR", "CURLY", "BRAC", "COMMA", "SEMICOL", "EOI", "ERR"
};

#endif
