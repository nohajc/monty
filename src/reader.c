#include "reader.h"
#include <string.h>

int reader_open(reader_t * r, const char * fname){
	r->f = fopen(fname, "r");
	if(!r->f) return 0;

	fseek(r->f, 0, SEEK_END);
	r->fleft = ftell(r->f);
	fseek(r->f, 0, SEEK_SET);
	
	memset(r->buf, 0, MAXBUF);
	r->bufpos = r->bufsize = MAXBUF;

	return 1;
}

int reader_getchar(reader_t * r){
	if(!r->fleft) return EOF;
	return (int)*r->ptr;
}

int reader_next(reader_t * r){
	if(!r->fleft) return EOF;
	if(r->bufpos < r->bufsize){
		++r->ptr;
		++r->bufpos;
		--r->fleft;
	}
	else{
		r->bufsize = fread(r->buf, sizeof(char), MAXBUF, r->f);
		if(r->bufsize == 0) return EOF;
		r->ptr = r->buf;
		r->bufpos = 0;
		--r->fleft;
	}

	//printf("reader_next: %c\n", *r->ptr);

	return (int)*r->ptr;
}

void reader_close(reader_t * r){
	fclose(r->f);
}
