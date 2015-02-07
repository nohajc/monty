#include "common.h"
#include "lexer.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ISHEX(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))
#define HEXVAL(c) ((c) >= 'a' ? ((c) - 'a' + 10) : ((c) >= 'A' ? ((c) - 'A' + 10) : (c) - '0'))

const keyword_t kw_table[] = {
	{"False", BOOL},
	{"None", NONE},
	{"True", BOOL},
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

int lexer_init(lexer_t * lex, const char * fname){
	reader_t * r = &lex->r;

	int success = reader_open(r, fname);
	if(!success) return 0;
	
	lex->stringpool = malloc(STRP_SIZE * sizeof(char));
	lex->str_size = STRP_SIZE;
	lex->str_pos = 0;
	lex->str_ptr = lex->stringpool;

	(void)reader_next(r);
	return success;
}

void lexer_destroy(lexer_t * lex){
	reader_close(&lex->r);
	free(lex->stringpool);
}

void lexer_str_append(lexer_t * lex, char c){
	REALLOC_CHECK(char, lex->stringpool, lex->str_pos, lex->str_size, 1);
	*lex->str_ptr++ = c;
	++lex->str_pos;
}

void lexer_str_terminate(lexer_t * lex){
	lexer_str_append(lex, 0);
}

int lexer_kw_cmp(const void * a, const void * b){
	const char * ka = a;
	const keyword_t * kb = b;

	return strcmp(ka, kb->name);
}

void lexer_kw_check(tok_t * token){
	keyword_t * kw = bsearch(token->attr.ident, kw_table, NELEMS(kw_table), sizeof(*kw_table), lexer_kw_cmp);
	if(!kw) return;
	if(kw->type == BOOL){
		token->type = BOOL;
		token->attr.b_val = strcmp(token->attr.ident, "True") ? 0 : 1;
		return;
	}
	if(kw->type == kwAND){
		token->type = OP;
		token->attr.op = LAND;
		return;
	}
	if(kw->type == kwOR){
		token->type = OP;
		token->attr.op = LOR;
		return;
	}
	if(kw->type == kwNOT){
		token->type = OP;
		token->attr.op = NOT;
		return;
	}
	token->type = kw->type;
}

char_type_t lexer_char_type(int c){
	if(c == EOF) return END;
	if(isspace(c)) return WHITE;
	if(c == '_') return UNDERSC;
	if(isdigit(c)) return DIGIT;
	if(isalpha(c)) return LETTER;
	return OTHER;
}


tok_t lexer_next_token(lexer_t * lex){
	reader_t * r = &lex->r;
	int c;
	char_type_t t;
	tok_t token;

	c = reader_getchar(r);
	//printf("got char: '%c' = %d\n", c, c);

	q0: // first char
	switch(c){
		case '(':
			token.type = PAR;
			token.attr.par = LEFT;
			c = reader_next(r);
			return token;
		case ')':
			token.type = PAR;
			token.attr.par = RIGHT;
			c = reader_next(r);
			return token;
		case '{':
			token.type = CURLY;
			token.attr.par = LEFT;
			c = reader_next(r);
			return token;
		case '}':
			token.type = CURLY;
			token.attr.par = RIGHT;
			c = reader_next(r);
			return token;
		case '[':
			token.type = BRAC;
			token.attr.par = LEFT;
			c = reader_next(r);
			return token;
		case ']':
			token.type = BRAC;
			token.attr.par = RIGHT;
			c = reader_next(r);
			return token;
		case ',':
			token.type = COMMA;
			c = reader_next(r);
			return token;
		case ';':
			token.type = SEMICOL;
			c = reader_next(r);
			return token;
		case '+':
			c = reader_next(r);
			goto q1;
		case '-':
			c = reader_next(r);
			goto q2;
		case '*':
			c = reader_next(r);
			goto q3;
		case '/':
			c = reader_next(r);
			goto q4;
		case '%':
			c = reader_next(r);
			goto q5;
		case '=':
			c = reader_next(r);
			goto q6;
		case '!':
			c = reader_next(r);
			goto q7;
		case '<':
			c = reader_next(r);
			goto q8;
		case '>':
			c = reader_next(r);
			goto q9;
		case '&':
			c = reader_next(r);
			goto q10;
		case '|':
			c = reader_next(r);
			goto q11;
		case '^':
			token.type = OP;
			token.attr.op = XOR;
			c = reader_next(r);
			return token;
		case '~':
			token.type = OP;
			token.attr.op = INV;
			c = reader_next(r);
			return token;
		case '\"':
			token.type = STRING;
			token.attr.s_value = lex->str_ptr;
			c = reader_next(r);
			goto q12;
		case '\'':
			token.type = STRING;
			token.attr.s_value = lex->str_ptr;
			c = reader_next(r);
			goto q13;
		case '0':
			c = reader_next(r);
			goto q14;
		case '#':
			c = reader_next(r);
			goto q22;
		default:;
	}

	t = lexer_char_type(c);
	switch(t){
		case UNDERSC:
		case LETTER:
			token.type = IDENT;
			token.attr.ident = lex->str_ptr;
			lexer_str_append(lex, c);
			c = reader_next(r);
			goto q15;
		case DIGIT: // nonzero
			token.type = INT;
			token.attr.i_value = c - '0';
			c = reader_next(r);
			goto q16;
		case WHITE:
			c = reader_next(r);
			goto q0;
		case END:
			token.type = EOI;
			return token;
		default:
			token.type = ERR;
			return token;
	}

	q1: // got '+'
	switch(c){
		case '+':
			token.type = OP;
			token.attr.op = INC;
			c = reader_next(r);
			return token;
		case '=':
			token.type = OP;
			token.attr.op = ADD;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = PLUS;
			return token;
	}

	q2: // got '-'
	switch(c){
		case '-':
			token.type = OP;
			token.attr.op = DEC;
			c = reader_next(r);
			return token;
		case '=':
			token.type = OP;
			token.attr.op = SUB;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = MINUS;
			return token;
	}

	q3: // got '*'
	switch(c){
		case '*':
			c = reader_next(r);
			goto q17;
		case '=':
			token.type = OP;
			token.attr.op = MULEQ;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = MUL;
			return token;
	}

	q4: // got '/'
	switch(c){
		case '/':
			c = reader_next(r);
			goto q18;
		case '=':
			token.type = OP;
			token.attr.op = DIVEQ;
			c = reader_next(r);
			return token;
		case '*':
			c = reader_next(r);
			goto q23;
		default:
			token.type = OP;
			token.attr.op = DIV;
			return token;
	}

	q5: // got '%'
	switch(c){
		case '=':
			token.type = OP;
			token.attr.op = MODEQ;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = MOD;
			return token;
	}

	q6: // got '='
	switch(c){
		case '=':
			token.type = OP;
			token.attr.op = EQ;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = ASSIGN;
			return token;
	}


	q7: // got '!'
	switch(c){
		case '=':
			token.type = OP;
			token.attr.op = NE;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = NOT;
			return token;
	}

	q8: // got '<'
	switch(c){
		case '<':
			token.type = OP;
			token.attr.op = SHL;
			c = reader_next(r);
			return token;
		case '>':
			token.type = OP;
			token.attr.op = NE;
			c = reader_next(r);
			return token;
		case '=':
			token.type = OP;
			token.attr.op = LE;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = LT;
			return token;
	}

	q9: // got '>'
	switch(c){
		case '>':
			token.type = OP;
			token.attr.op = SHR;
			c = reader_next(r);
			return token;
		case '=':
			token.type = OP;
			token.attr.op = GE;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = GT;
			return token;

	}

	q10: // got '&'
	switch(c){
		case '&':
			token.type = OP;
			token.attr.op = LAND;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = AND;
			return token;
	}

	q11: // got '|'
	switch(c){
		case '|':
			token.type = OP;
			token.attr.op = LOR;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = OR;
			return token;
	}

	q12: // got '\"'
	switch(c){
		case '\"':
			lexer_str_terminate(lex);
			c = reader_next(r);
			return token;
		case '\\':
			c = reader_next(r);
		default:
			lexer_str_append(lex, c);
			c = reader_next(r);
			goto q12;
	}

	q13: // got '\''
	switch(c){
		case '\'':
			lexer_str_terminate(lex);
			c = reader_next(r);
			return token;
		case '\\':
			c = reader_next(r);
		default:
			lexer_str_append(lex, c);
			c = reader_next(r);
			goto q13;
	}

	q14: // got '0'
	t = lexer_char_type(c);
	if(c == 'x'){
		c = reader_next(r);
		goto q19;
	}
	else if(t == DIGIT){
		if(c > '7'){
			token.type = ERR;
			return token;
		}
		token.type = INT;
		token.attr.i_value = c - '0';
		c = reader_next(r);
		goto q20;
	}
	else{
		token.type = INT;
		token.attr.i_value = 0;
		return token;
	}

	q15: // got '_' or LETTER
	t = lexer_char_type(c);
	switch(t){
		case LETTER:
		case DIGIT:
		case UNDERSC:
			lexer_str_append(lex, c);
			c = reader_next(r);
			goto q15;
		default:
			lexer_str_terminate(lex);
			lexer_kw_check(&token);
			return token;
	}

	q16: // got decimal digit
	t = lexer_char_type(c);
	switch(t){
		case DIGIT:
			token.attr.i_value = token.attr.i_value * 10 + (c - '0');
			c = reader_next(r);
			goto q16;
		default:
			return token;
	}

	q17: // got '**'
	switch(c){
		case '=':
			token.type = OP;
			token.attr.op = EXPEQ;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = EXP;
			return token;
	}

	q18: // got '//'
	switch(c){
		case '=':
			token.type = OP;
			token.attr.op = FDIVEQ;
			c = reader_next(r);
			return token;
		default:
			token.type = OP;
			token.attr.op = FDIV;
			return token;
	}

	q19: // got '0x'
	if(ISHEX(c)){
		token.type = INT;
		token.attr.i_value = HEXVAL(c);
		c = reader_next(r);
		goto q21;
	}
	else{
		token.type = ERR;
		return token;
	}

	q20: // got octal digit
	t = lexer_char_type(c);
	switch(t){
		case DIGIT:
			if(c > '7'){
				token.type = ERR;
				return token;
			}
			token.attr.i_value = token.attr.i_value * 8 + (c - '0');
			c = reader_next(r);
			goto q20;
		default:
			return token;
	}

	q22: // short comment
	if(c != '\n'){
		c = reader_next(r);
		goto q22;
	}
	c = reader_next(r);
	goto q0;

	q23: // long comment
	if(c == '*'){
		c = reader_next(r);
		if(c == '/'){
			c = reader_next(r);
			goto q0;
		}
	}
	c = reader_next(r);
	goto q23;

	q21: // got hex digit
	if(ISHEX(c)){
		token.attr.i_value = token.attr.i_value * 16 + HEXVAL(c);
		c = reader_next(r);
		goto q21;
	}
	return token;
}
