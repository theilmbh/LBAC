#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmplr.h"

void print_token(Token *tok)
{
  switch(tok->type) {

    case OP:
      printf("Operator\n");
      break;
    case INTEGER:
      printf("Integer: %d\n", tok->val.int_val);
      break;
    case ENDA:
      printf("END\n");
      break;
  }
}

void read_one_token(Token *tok)
{
  int ch;
  ch = getc(stdin);
  while(isspace(ch)) {
    ch = getc(stdin);
  }

  /* Handle integer literal */
  if(isdigit(ch)) {
    tok->type = INTEGER;
    tok->val.int_val = ch - '0';
    while(isdigit(ch = getc(stdin))) {
      tok->val.int_val = tok->val.int_val*10 + ch - '0';
    }
    ungetc(ch, stdin);
    return;
  }

  if(ch == '+') {
    tok->type = OP;
    tok->val.op = PLUS;
    return;
  }

  if(ch == '*') {
    tok->type = OP;
    tok->val.op = MULTIPLY;
    return;
  }

  if(ch == EOF) {
    printf("End of file?\n");
    tok->type = ENDA;
    return;
  }

  printf("Lexing Error!\n");
}
