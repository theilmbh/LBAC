#ifndef CMPLR_H
#define CMPLR_H

typedef enum token_t {INTEGER, OP, ENDA} token_t;
typedef enum op_t {PLUS, MULTIPLY} op_t;

typedef union val_t {
  int int_val;
  op_t op;
} val_t;

typedef struct Token {

  token_t type;
  val_t val;
} Token;

void read_one_token(Token *tok);
void print_token(Token *tok);

#endif
