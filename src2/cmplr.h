#ifndef CMPLR_H
#define CMPLR_H

#define MAXIDENT 256

#include <stdio.h>

typedef enum token_t {
    INTEGER, OP, ENDA,
    L_PAREN, R_PAREN, IDENT,
    KW_INT, SEMICOLON, ASSIGN_OP,
    KW_DEF, L_CURLY, R_CURLY, COMMA
} token_t;

typedef enum op_t { PLUS, MULTIPLY, MINUS, DIVIDE } op_t;

typedef union val_t {
    int int_val;
    op_t op;
    char ident[MAXIDENT];
} val_t;

typedef struct Token {
    token_t type;
    val_t val;
} Token;


void read_one_token(Token * tok, FILE * f);
void print_token(Token * tok);


#endif
