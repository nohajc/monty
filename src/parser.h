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

node_t * nt_suite(parser_t * p);
node_t * nt_target(parser_t * p);
node_t * nt_target_list(parser_t * p, int first);
node_t * nt_else_stmt(parser_t * p);
node_t * nt_elif_stmt_rest(parser_t * p);
node_t * nt_elif_stmt(parser_t * p);
node_t * nt_simple_stmt(parser_t * p);
node_t * nt_expression(parser_t * p);
node_t * nt_if_stmt_rest(parser_t * p);
node_t * nt_if_stmt(parser_t * p);
node_t * nt_while_stmt(parser_t * p);
node_t * nt_for_stmt(parser_t * p);
node_t * nt_try_stmt(parser_t * p);
node_t * nt_with_stmt(parser_t * p);
node_t * nt_funcdef(parser_t * p);
node_t * nt_classdef(parser_t * p);
node_t * nt_stmt_list(parser_t * p, int first);
node_t * nt_statement(parser_t * p);
node_t * nt_file_input(parser_t * p);

#endif
