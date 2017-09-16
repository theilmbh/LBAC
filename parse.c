#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmplr.h"

typedef enum ast_node_t {BIN_OP_PLUS, BIN_OP_TIMES, INT} ast_node_t;

typedef struct Node {
  ast_node_t type;
  int value;
  struct Node *left;
  struct Node *right;
} Node;

Token *tok;

void match(Token *tok) {
  read_one_token(tok);
}

Node *plus_node(Node *l, Node *r) {

  Node *out = malloc(sizeof(Node));
  out->type = BIN_OP_PLUS;
  out->value = 0;
  out->left = l;
  out->right = r;
  return out;
}

Node *times_node(Node *l, Node *r) {

  Node *out = malloc(sizeof(Node));
  out->type = BIN_OP_TIMES;
  out->value = 0;
  out->left = l;
  out->right = r;
  return out;
}

Node *integer_node(int val) {

  Node *out = malloc(sizeof(Node));
  out->type = INT;
  out->value = val;
  out->left = NULL;
  out->right = NULL;
  return out;
}

Node *factor() {
  // printf("factor\n");
  Node *out = NULL;
  if(tok->type == INTEGER) {
    out = integer_node(tok->val.int_val);
    match(tok);
    return out;
  } else {
    printf("Syntax Error\n");
    return NULL;
  }
}

Node *term() {
  // printf("Term\n");
  Node *out = NULL;
  out = factor();
  if(tok->type == OP && tok->val.op == MULTIPLY) {
    match(tok);
    out = times_node(out, term());
    return out;
  }
  return out;
}

Node *expression() {
  // printf("Expression\n");
  Node *out = NULL;
  Node *ph = NULL;
  ph = term();
  if(tok->type == OP && tok->val.op == PLUS) {
    match(tok);
    out = plus_node(ph, expression());
    return out;
  }
  return ph;
}

Node *parse()
{
  Node *ast;
  Token mytok;
  tok = &mytok;
  read_one_token(tok);
  ast = expression();
  return ast;
}

void print_node(Node *n, int indent) {
  // for(int i=0; i<indent; i++) {
  //   printf(" ");
  // }
  switch(n->type) {
    case BIN_OP_TIMES:
      printf("P Multiply\n");
      break;
    case BIN_OP_PLUS:
      printf("P Add\n");
      break;
    case INT:
      printf("P Integer: %d\n", n->value);
      break;
  }
}

void print_ast(Node *n, int indent) {
    int i;
    print_node(n, indent);
    if(n->left != NULL) {
      for(i=0; i<indent; i++) {
        printf(" ");
      }
      printf("|-> ");
      print_ast(n->left, indent+4);
    }
    if(n->right != NULL) {
      for(i=0; i<indent; i++) {
        printf(" ");
      }
      printf("|-> ");
      print_ast(n->right, indent+4);
    }
}

int main() {

  print_ast(parse(), 0);
  return 0;
}
