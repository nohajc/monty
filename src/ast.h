#ifndef __AST__
#define __AST__

typedef enum{
	ATOM, FINPUT, STMT_LIST, IF_STMT, IF_STMT_R, ELIF_STMT, ELIF_STMT_R, ELSE_STMT, WHILE_STMT,
	FOR_STMT, FOR_STMT_R, TARGET_LIST, TARGET, EXPR
} ntype_t;

#define TYPE(n) n->type
#define FIRST(n) n->a.lst.first
#define REST(n) n->a.lst.rest
#define VAL(n) n->a.val

typedef struct node_t{
	ntype_t type;
	union{
		struct{
			struct node_t * first;
			struct node_t * rest;
		} lst;
		void * val;
	} a;
} node_t;

node_t * ast_new_node(ntype_t t);

#endif
