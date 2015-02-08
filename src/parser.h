#ifndef __PARSER__
#define __PARSER__

#include "ast.h"
#include "lexer.h"

typedef struct{
	lexer_t lex;
	tok_t token;
} parser_t;

int parser_init(parser_t * p, const char * fname);
node_t * parser_run(parser_t * p);

#endif
