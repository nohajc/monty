#include "reader.h"

int reader_open(reader_t * r, const char * fname){
	r->f = fopen(fname, "r");
	if(!f) return 0;

	fseek(r->f, 0, SEEK_END);
	r->fsize = ftell(r->f);
	fseek(r->f, 0, SEEK_SET);

	r->bufpos = MAXBUF;

	return 1;
}

int reader_getchar(reader_t * r){
	return (int)*r->ptr;
}

int reader_next(reader_t * r){
	if(r->bufpos < r->bufsize){
		r->ptr++;
		r->bufpos++;
	}
	else{
		r->bufsize = fread(r->buf, sizeof(char), MAXBUF, r->f);
		if(r->bufsize == 0) return EOF;
		r->ptr = r->buf;
		r->bufpos = 0;
	}

	return (int)*r->ptr;
}

void reader_close(reader_t * r){
	fclose(r->f);
}