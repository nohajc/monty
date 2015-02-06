#ifndef __COMMON__
#define __COMMON__

#include <stdio.h>

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

#define BADALLOC() do{ \
	fprintf(stderr, "%s\n", "Malloc failed"); \
	exit(255); \
} while(0)

#define REALLOC_CHECK(type, ptr, pos, size, left) do{ \
	if(pos + left > size){ \
		uint32_t new_size = 2 * size; \
		type * new_ptr = realloc(ptr, new_size); \
		if(!new_ptr) BADALLOC(); \
		ptr = new_ptr; \
		size = new_size; \
	} \
} while(0)

#endif
