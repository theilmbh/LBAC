/* Implement a recursive parser to parse differentiation expressions
 * Build a parse tree from the input
 * Brad Theilman 01/2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctypes.h>

#include "autodiff.h"

struct token *lookahead;

void match_token(token_type_t type)
{
	if (lookahead->t_type == type) {
		get_next_token(lookahead);
	} else {
		syntax_error();
	}

}

void expr()
{
	term();
	while(1) {
		if (lookahead->t_type == PLUS_OPERATOR) {
			/* add appropriate structure to parse tree */
			match_token(PLUS_OPERATOR);
			term();
		} else if (lookahead->t_type == MINUS_OPERATOR) {
			match_token(MINUS_OPERATOR);
			term();
		} else {
			break;
		}
	}
}

void term()
{

}

void func()
{

}

void var()
{

}

void d_op()
{

}
