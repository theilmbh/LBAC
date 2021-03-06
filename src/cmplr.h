#ifndef CMPLR_H
#define CMPLR_H

#define MAXIDENT 256

#include <stdio.h>

typedef enum token_t {
    INTEGER, OP, ENDA,
    L_PAREN, R_PAREN, IDENT,
    KW_INT, SEMICOLON, ASSIGN_OP
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

typedef enum ast_node_t {
    BIN_OP_PLUS,
    BIN_OP_TIMES,
    BIN_OP_MINUS,
    BIN_OP_DIVIDE,
    INT,
    VAR,
    PAREN
} ast_node_t;

typedef struct Node {
    ast_node_t type;
    int value;
    char name[MAXIDENT];
    struct Node *left;
    struct Node *right;
} Node;

void read_one_token(Token * tok, FILE * f);
void print_token(Token * tok);

Node *plus_node(Node * l, Node * r);
Node *times_node(Node * l, Node * r);
Node *minus_node(Node * l, Node * r);
Node *integer_node(int val);

Node *factor();
Node *expression();
Node *term();
Node *parse();
Node *statement();

#endif
