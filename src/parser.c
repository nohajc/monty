#include <stdlib.h>
#include "parser.h"

int parser_init(parser_t * p, const char * fname){
	int success = lexer_init(&p->lex, fname);
	if(!success) return 0;
	p->token = lexer_next_token(&p->lex);

	return success;
}

void parser_error(tok_type_t t){
	fprintf(stderr, "Syntax error, expected %s.\n", tok_str[t]);
	exit(1);
}

void parser_match(parser_t * p, tok_type_t t){
	if(p->token.type == t){
		p->token = lexer_next_token(&p->lex);
	}
	else{
		parser_error(t);
	}
}

void parser_match_par(parser_t * p, tok_type_t t, par_t d){
	if(p->token.type == t && p->token.attr.par == d){
		p->token = lexer_next_token(&p->lex);
	}
	else{
		parser_error(t);
	}

}

node_t * nt_simple_stmt(parser_t * p){
	//TODO
	return NULL;
}


node_t * nt_expression(parser_t * p){
	//TODO
	return NULL;
}

node_t * nt_expression_list(parser_t * p){
	//TODO
	return NULL;
}

node_t * nt_target_list(parser_t * p){
	return NULL;
}

node_t * nt_suite(parser_t * p){
	node_t * nod;
	if(p->token.type == CURLY && p->token.attr.par == LEFT){
		parser_match_par(p, CURLY, LEFT);
		nod = nt_file_input(p);
		parser_match_par(p, CURLY, RIGHT);
	}
	else{
		nod = nt_stmt_list(p, 1);
		parser_match(p, NEWLINE);
	}
	return nod;
}

node_t * nt_elif_stmt_rest(parser_t * p){
	node_t * nod = ast_new_node(ELIF_STMT_R);

	FIRST(nod) = nt_suite(p);

	if(p->token.type == kwELIF){
		REST(nod) = nt_elif_stmt(p);
	}
	else if(p->token.type == kwELSE){
		REST(nod) = nt_else_stmt(p);
	}
	else{
		REST(nod) = NULL;
	}

	return nod;
}

node_t * nt_elif_stmt(parser_t * p){
	node_t * nod;
	parser_match(p, kwELIF);
	nod = ast_new_node(ELIF_STMT);
	FIRST(nod) = nt_expression(p);
	REST(nod) = nt_elif_stmt_rest(p);
	return nod;
}

node_t * nt_else_stmt(parser_t * p){
	node_t * nod;
	parser_match(p, kwELSE);
	nod = ast_new_node(ELSE_STMT);
	FIRST(nod) = nt_suite(p);
	REST(nod) = NULL;
	return nod;
}

node_t * nt_if_stmt_rest(parser_t * p){
	node_t * nod = ast_new_node(IF_STMT_R);

	FIRST(nod) = nt_suite(p);

	if(p->token.type == kwELIF){
		REST(nod) = nt_elif_stmt(p);
	}
	else if(p->token.type == kwELSE){
		REST(nod) = nt_else_stmt(p);
	}
	else{
		REST(nod) = NULL;
	}

	return nod;
}

node_t * nt_if_stmt(parser_t * p){
	node_t * nod;
	parser_match(p, kwIF);
	nod = ast_new_node(IF_STMT);
	FIRST(nod) = nt_expression(p);
	REST(nod) = nt_if_stmt_rest(p);
	return nod;
}

node_t * nt_while_stmt(parser_t * p){
	node_t * nod;
	parser_match(p, kwWHILE);
	nod = ast_new_node(WHILE_STMT);
	FIRST(nod) = nt_expression(p);
	REST(nod) = nt_suite(p);
	// else is not implemented
	return nod;
}

node_t * nt_for_stmt_rest(parser_t * p){
	node_t * nod;
	nod = ast_new_node(FOR_STMT_R);
	FIRST(nod) = nt_expression_list(p);
	REST(nod) = nt_suite(p);
	// else is not implemented
	return nod;
}

node_t * nt_for_stmt(parser_t * p){
	node_t * nod;
	parser_match(p, kwFOR);
	nod = ast_new_node(FOR_STMT);
	FIRST(nod) = nt_target_list(p);
	parser_match(p, kwIN);
	REST(nod) = nt_for_stmt_rest(p);
	return nod;
}

node_t * nt_try_stmt(parser_t * p){
	//TODO
	return NULL;
}

node_t * nt_with_stmt(parser_t * p){
	//TODO
	return NULL;
}

node_t * nt_funcdef(parser_t * p){
	//TODO
	return NULL;
}

node_t * nt_classdef(parser_t * p){
	//TODO
	return NULL;
}

node_t * nt_stmt_list(parser_t * p, int first){
	node_t * nod;
	
	if(!first && p->token.type == NEWLINE){
		return NULL;
	}

	nod = ast_new_node(STMT_LIST);
	FIRST(nod) = nt_simple_stmt(p);

	if(p->token.type == NEWLINE){
		REST(nod) = NULL;
	}
	else{ 
		parser_match(p, SEMICOL);
		REST(nod) = nt_stmt_list(p, 0);
	}
	
	return nod;
}

node_t * nt_statement(parser_t * p){
	node_t * nod;

	switch(p->token.type){
		case kwIF:
			return nt_if_stmt(p);
		case kwWHILE:
			return nt_while_stmt(p);
		case kwFOR:
			return nt_for_stmt(p);
		case kwTRY:
			return nt_try_stmt(p);
		case kwWITH:
			return nt_with_stmt(p);
		case kwDEF:
			return nt_funcdef(p);
		case kwCLASS:
			return nt_classdef(p);
		default:
			nod = nt_stmt_list(p, 1);
			parser_match(p, NEWLINE);
	}
	return nod;
}

node_t * nt_file_input(parser_t * p){
	if(p->token.type == EOI){
		return NULL;
	}
	node_t * nod = ast_new_node(FINPUT);

	while(p->token.type == NEWLINE){
		p->token = lexer_next_token(&p->lex);
	}
	FIRST(nod) = nt_statement(p);
	REST(nod) = nt_file_input(p);
	
	return nod;
}

node_t * parser_run(parser_t * p){
	return nt_file_input(p);
}
