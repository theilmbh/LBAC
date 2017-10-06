/*
 * =====================================================================================
 *
 *       Filename:  ast.h
 *
 *    Description:  Abstract syntax tree definitions/declarations
 *
 *        Version:  1.0
 *        Created:  09/22/2017 01:51:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Brad Theilman (BHT), bradtheilman@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef AST_H
#define AST_H

#include "cmplr.h"
#include "symbol.h"

extern const char* node_types[];

typedef enum ast_node_t {
    BIN_OP_PLUS,
    BIN_OP_TIMES,
    BIN_OP_MINUS,
    BIN_OP_DIVIDE,
    INT,
    VAR,
    PAREN,
    STMT,
    DECL,
    FUNC_DECL,
    ARGS
} ast_node_t;

typedef struct Node {
    ast_node_t type;
    int value;
    char name[MAXIDENT];
    struct env *e;
    struct Node *left;
    struct Node *right;
} Node;

Node *paren_node(Node * expr, struct env *e);
Node *var_node(char *var_name, struct env *e);
Node *plus_node(Node * l, Node * r, struct env *e);
Node *times_node(Node * l, Node * r, struct env *e);
Node *minus_node(Node * l, Node * r, struct env *e);
Node *integer_node(int val, struct env *e);
Node *divide_node(Node * l, Node * r, struct env *e);
Node *stmt_node(Node * l, Node * r, struct env *e);
Node *decl_node(Node * s, struct env *e);
Node *func_decl_node(Node * nm, Node * body, struct env *e);
Node *func_call_node(Node * nm, Node * args_list, struct env *e);
Node *args_node(Node * l, Node * r, struct env *e);

#endif
