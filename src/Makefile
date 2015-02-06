CC=gcc
CFLAGS=-Wall -Wno-long-long -g

SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)

main: $(OBJS) types
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJS) : %.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

# Make a highlight file for types.  Requires Exuberant ctags and awk
types: types.vim
types.vim: *.[ch]
	ctags --c-kinds=gstu -o- *.[ch] |\
	awk 'BEGIN{printf("syntax keyword Type\t")}\
	{printf("%s ", $$1)}END{print ""}' > $@
