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
	match_token(IDENTIFIER);
	match_token(RBRACK);
	if (lookahead->t_type == PERCENT) {
		var();
	} else {
		func();
	}
	match_token(LBRACK);
}

void var()
{
	match_token(PERCENT);
	match_token(IDENTIFIER);
}

void d_op()
{
	match_token(D_D);
	var();
}

void deriv_expr()
{
	d_op();
	match_token(RBRACK);
	if (lookahead->t_type == IDENTIFIER) {
		func();
		printf('')
	} else {
		expr();
	}
	match_token(LBRACK);
}

void get_next_token(struct token *t)
{
	
	char b;
	int i;
	b = getc(stdin);
	for(i=0; i<N_TOKENS; i++) {
		if (b == token_str[i]) {
			t->t_type = token_types[i];
		} else {
			t->t_type = IDENTIFIER;
		}
	}
	
}

int main()
{
	expr();
	putc('\n', stdout);
}