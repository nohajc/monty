#include "debug.h"
#include "lexer.h"

void print_usage(void){
}

int main(int argc, char ** argv){
	lexer_t lex;
	tok_t tok;
	
	if(argc != 2){
		print_usage();
		return 0;
	}

	lexer_init(&lex, argv[1]);

	do{
		tok = lexer_next_token(&lex);
		//printf("%s\n", tok_str[tok.type]);
		print_token(&tok);
	} while(tok.type != EOI);

	lexer_destroy(&lex);

	return 0;
}
