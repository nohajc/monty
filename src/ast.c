#include <stdlib.h>
#include "ast.h"
#include "common.h"

node_t * ast_new_node(ntype_t t){
	node_t * nod = malloc(sizeof(node_t));
	if(!nod) BADALLOC();

	TYPE(nod) = t;
	return nod;
}
