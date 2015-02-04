#include "lexer.h"
#include <ctype.h>

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
			c = reader_next(r);
			goto q15;
		case DIGIT: // nonzero
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

	q2: // got '-'

	q3: // got '*'

	q4: // got '/'

	q5: // got '%'

	q6: // got '='

	q7: // got '!'

	q8: // got '<'

	q9: // got '>'

	q10: // got '&'

	q11: // got '|'

	q12: // got '\"'

	q13: // got '\''

	q14: // got '0'

	q15: // got '_' or LETTER

	q16: // got nonzero digit
}