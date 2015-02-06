# Make a highlight file for types.  Requires Exuberant ctags and awk
types: types.vim
types.vim: *.[ch]
	ctags --c-kinds=gstu -o- *.[ch] |\
	awk 'BEGIN{printf("syntax keyword Type\t")}\
	{printf("%s ", $$1)}END{print ""}' > $@
