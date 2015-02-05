#include "lexer.h"
#include <ctype.h>

#define ISHEX(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))
#define HEXVAL(c) ((c) >= 'a' ? ((c) - 'a' + 10) : ((c) >= 'A' ? ((c) - 'A' + 10) : (c) - '0'))

int lexer_init(lexer_t * lex, const char * fname){
	reader_t * r = &lex->r;

	int success = reader_open(r, const char * fname);
	if(!success) return 0;

	c = reader_next(r);
	return success;
}

char_type_t lexer_char_type(int c){
	if(c == EOF) return END;
	if(isspace(c)) return WHITE;
	if(isdigit(c)) return DIGIT;
	if(isalpha(c)) return LETTER;
	return OTHER;
}

tok_t lexer_next_token(lexer_t * lex){
	reader_t * r = &lex->r;
	int c, char_type_t t;
	int idlen = 0;
	tok_t token;

	c = reader_getchar(r);

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
			c = reader_next(r);
			goto q12;
		case '\'':
			c = reader_next(r);
			goto q13;
		case '0':
			c = reader_next(r);
			goto q14;
		case '_':
			c = reader_next(r);
			goto q15;
		default:;
	}

	t = lexer_char_type(c);
	switch(t){
		case LETTER:
			// TODO save first ident/keyword char
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
			token.type = END;
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

	q13: // got '\''

	q14: // got '0'
	t = lexer_char_type(c);
	if(c == 'x'){
		c = reader_next(r);
		goto q19;
	}
	else if(t = DIGIT){
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

	q21: // got hex digit
	if(ISHEX(c)){
		token.attr.i_value = token.attr.i_value * 16 + HEXVAL(c);
		c = reader_next(r);
		goto q21;
	}
	return token;
}
