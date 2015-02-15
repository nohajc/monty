#include <stdlib.h>
#include "parser.h"

int parser_init(parser_t * p, const char * fname){
	int success = lexer_init(&p->lex, fname);
	if(!success) return 0;
	p->token = lexer_next_token(&p->lex);

	return success;
}

#define EX 1
#define UN 0

void parser_error(tok_type_t t, int expected){
	fprintf(stderr, "Syntax error, %sexpected %s.\n", (expected ? "" : "un"), tok_str[t]);
	exit(1);
}

void parser_match(parser_t * p, tok_type_t t){
	if(p->token.type == t){
		p->token = lexer_next_token(&p->lex);
	}
	else{
		parser_error(t, EX);
	}
}

void parser_match_par(parser_t * p, tok_type_t t, par_t d){
	if(p->token.type == t && p->token.attr.par == d){
		p->token = lexer_next_token(&p->lex);
	}
	else{
		parser_error(t, EX);
	}

}

char * parser_match_ident(parser_t * p, tok_type_t t){
	char * id = NULL;
	if(p->token.type == t){
		id = p->token.attr.ident;
		p->token = lexer_next_token(&p->lex);
	}
	else{
		parser_error(t, EX);
	}
	return id;
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

node_t * nt_atom(parser_t * p){
	//TODO
	char * id;
	id = parser_match_ident(p, IDENT);
	return NULL;
}

node_t * nt_attributeref(parser_t * p){
	//TODO
	return NULL;
}

node_t * nt_subscript_or_slice(parser_t * p){
	//TODO
	return NULL;
}

node_t * nt_call(parser_t * p){
	//TODO
	return NULL;
}
node_t * nt_target(parser_t * p){
	node_t * nod, * tmp;
	int have_ident = 0;

	if(p->token.type == PAR && p->token.attr.par == LEFT){
		parser_match_par(p, PAR, LEFT);
		nod = nt_target_list(p, 1);
		parser_match_par(p, PAR, RIGHT);
	}
	else if(p->token.type == BRAC && p->token.attr.par == LEFT){
		parser_match_par(p, BRAC, LEFT);
		nod = nt_target_list(p, 1);
		parser_match_par(p, BRAC, RIGHT);
	}
	
	if(p->token.type == IDENT){
		have_ident = 1;
	}
	tmp = nt_atom(p);

	switch(p->token.type){
		case DOT:
			//attributeref
			nod = ast_new_node(ATTREF);
			FIRST(nod) = tmp;
			REST(nod) = nt_attributeref(p);
		   break;
		case BRAC:
			if(p->token.attr.par == LEFT){
		   	//subscription | slicing
				nod = ast_new_node(SUBSL);
				FIRST(nod) = tmp;
				REST(nod) = nt_subscript_or_slice(p);
				break;
			}
			else{
				parser_error(BRAC, UN); //TODO
			}
		case PAR:
			if(p->token.attr.par == LEFT){
		   	//call
				nod = ast_new_node(CALL);
				FIRST(nod) = tmp;
				REST(nod) = nt_call(p);
				break;
			}
			else{
				parser_error(PAR, UN); //TODO
			}
		default:
			if(have_ident){
				nod = tmp;
			}
			else{
				parser_error(IDENT, EX);
			}
	}
	
	return nod;
};

node_t * nt_target_list(parser_t * p, int first){
	node_t * nod;
	if(!first){
		switch(p->token.type){
			case kwIN:
			case SEMICOL:
			case NEWLINE:
				return NULL;
			case PAR:
			case BRAC:
				if(p->token.attr.par == RIGHT){
					return NULL;
				}
			case OP:
				if(p->token.attr.op == ASSIGN){
					return NULL;
				}
			default:;
		}
	}
	nod = ast_new_node(TARGET_LIST);
	FIRST(nod) = nt_target(p);
	REST(nod) = nt_target_list(p, 0);
	return nod;
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
	FIRST(nod) = nt_target_list(p, 1);
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
