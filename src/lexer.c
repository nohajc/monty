#include "lexer.h"

int lexer_init(reader_t * r, const char * fname){
	int success = reader_open(r, const char * fname);
	if(!success) return 0;

	reader_next(r);
	return success;
}

