#ifndef __READER__
#define __READER__

#include <stdint.h>
#include <stdio.h>

#define MAXBUF 16384

typedef struct{
	FILE * f;
	uint64_t fsize;
	uint32_t bufsize;
	uint32_t bufpos;
	char buf[MAXBUF];
	char * ptr;
} reader_t;

int reader_open(reader_t * r, const char * fname);
int reader_getchar(reader_t * r);
int reader_next(reader_t * r);
void reader_close(reader_t * r);

#endif