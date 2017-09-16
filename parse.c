#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmplr.h"

Token *tok;

void match(token_t type) {
  if(tok->type == type) {
    read_one_token(tok);
  } else {
    printf("Syntax Error\n");
    exit(-1);
  }

}

Node *plus_node(Node *l, Node *r) {

  Node *out = malloc(sizeof(Node));
  out->type = BIN_OP_PLUS;
  out->value = 0;
  out->left = l;
  out->right = r;
  return out;
}

Node *minus_node(Node *l, Node *r) {

  Node *out = malloc(sizeof(Node));
  out->type = BIN_OP_MINUS;
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

Node *divide_node(Node *l, Node *r) {

  Node *out = malloc(sizeof(Node));
  out->type = BIN_OP_DIVIDE;
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

Node *var_node(char *var_name) {
  Node *out = malloc(sizeof(Node));
  out->type = VAR;
  strcpy(out->name, var_name);
  out->left = NULL;
  out->right = NULL;
  return out;
}

Node *factor() {
  // printf("factor\n");
  Node *out = NULL;
  if(tok->type == INTEGER) {
    out = integer_node(tok->val.int_val);
    match(INTEGER);
    return out;
  } else if(tok->type == L_PAREN) {
      match(L_PAREN);
      out = expression();
      match(R_PAREN);
      return out;
  } else if(tok->type == IDENT) {
    out = var_node(tok->val.ident);
    match(IDENT);
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
    match(OP);
    out = times_node(out, term());
    return out;
  }
  if(tok->type == OP && tok->val.op == DIVIDE) {
    match(OP);
    out = divide_node(out, term());
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
    match(OP);
    out = plus_node(ph, expression());
    return out;
  }
  if(tok->type == OP && tok->val.op == MINUS) {
    match(OP);
    out = minus_node(ph, expression());
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
    case BIN_OP_MINUS:
      printf("P Minus\n");
      break;
    case BIN_OP_DIVIDE:
      printf("P Divide\n");
      break;
    case INT:
      printf("P Integer: %d\n", n->value);
      break;
    case VAR:
      printf("P Varible: %s\n", n->name);
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
