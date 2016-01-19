/* Simple translator to convert expressions to postfix form
 * Based off of Ch. 2 of Compilers: Principles, Techniques, and Tools
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

char lookahead;

void expr();
void term();
void match(char t);
void syntax_error();

void expr()
{
	term();
	while(1) {
		if (lookahead == '+') {
			match('+');
			term();
			putc('+', stdout);
		} else if (lookahead == '-') {
			match('-');
			term();
			putc('-', stdout);
		} else {
			break;
		}
	}
}

void term()
{
	if (isdigit((int) lookahead)) {
		putc(lookahead, stdout);
		match(lookahead);
	} else if (lookahead == '(') {
		match('(');
		expr();
		match(')');
	} else {
		syntax_error();
	}
}

void match(char t)
{
	if (lookahead == t) {
		lookahead = getc(stdin);
	} else {
		syntax_error();
	}
}

void syntax_error()
{
	printf("Syntax error\n");
	exit(1);
}

int main()
{
	lookahead = getc(stdin);
	expr();
	putc('\n', stdout);
	return 0;
}