#ifndef __DEBUG__
#define __DEBUG__

#include <inttypes.h>
#include "lexer.h"


void print_token(tok_t * tok){
	if(tok->type == NEWLINE){
		printf("NL\n");
		return;
	}
	/*if(tok->type == kwDEF || tok->type == kwFOR || tok->type == kwWHILE || tok->type == kwIF || tok->type == kwELIF || tok->type == kwELSE || tok->type == kwTRY
		|| tok->type == kwEXCEPT || tok->type == kwFINALLY || tok->type == kwRETURN){
		:printf("\n");
	}*/

	if(tok->type <= IDENT){
		if(tok->type != IDENT) printf("kw:");
		printf("%s", tok->attr.ident);
	}
	else if(tok->type == OP){
		switch(tok->attr.op){
			case PLUS:
				printf("+");
				break;
			case MINUS:
				printf("-");
				break;
			case MUL:
				printf("*");
				break;
			case DIV:
				printf("/");
				break;
			case MOD:
				printf("%%");
				break;
			case EXP:
				printf("**");
				break;
			case FDIV:
				printf("//");
				break;
			case EQ:
				printf("==");
				break;
			case NE:
				printf("!=");
				break;
			case GT:
				printf(">");
				break;
			case LT:
				printf("<");
				break;
			case GE:
				printf(">=");
				break;
			case LE:
				printf("<=");
				break;
			case ASSIGN:
				printf("=");
				break;
			case ADD:
				printf("+=");
				break;
			case SUB:
				printf("-=");
				break;
			case MULEQ:
				printf("*=");
				break;
			case DIVEQ:
				printf("/=");
				break;
			case MODEQ:
				printf("%%=");
				break;
			case EXPEQ:
				printf("**=");
				break;
			case FDIVEQ:
				printf("//=");
				break;
			case INC:
				printf("++");
				break;
			case DEC:
				printf("--");
				break;
			case AND:
				printf("&");
				break;
			case OR:
				printf("|");
				break;
			case XOR:
				printf("^");
				break;
			case INV:
				printf("~");
				break;
			case SHL:
				printf("<<");
				break;
			case SHR:
				printf(">>");
				break;
			case LAND:
				printf("&&");
				break;
			case LOR:
				printf("||");
				break;
			case NOT:
				printf("!");
				break;
			default:
				printf("Unknown operator\n");
		}
	}
	else if(tok->type == BOOL){
		printf("%s", (tok->attr.b_val == 1 ? "True" : "False"));
	}
	else if(tok->type == NONE){
		printf("None");
	}
	else if(tok->type == STRING){
		printf("\"%s\"", tok->attr.s_value);
	}
	else if(tok->type == INT){
		printf("%" PRId64, tok->attr.i_value);
	}
	else if(tok->type == FLOAT){
		printf("%f", tok->attr.f_value);
	}
	else{
		switch(tok->type){
			case PAR:
				printf("%c", (tok->attr.par == LEFT ? '(' : ')'));
				break;
			case CURLY:
				printf("%s", (tok->attr.par == LEFT ? "{" : "}"));
				break;
			case BRAC:
				printf("%c", (tok->attr.par == LEFT ? '[' : ']'));
				break;
			case COMMA:
				printf(",");
				break;
			case SEMICOL:
				printf(";");
				break;
			case EOI:
				break;
			case ERR:
				printf("Syntax error\n");
				break;
			default:
				printf("Unknown token\n");
		}
	}
	printf(" ");
}

#endif
