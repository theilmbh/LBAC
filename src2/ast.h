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

Node *paren_node(Node * expr);
Node *var_node(char *var_name);
Node *plus_node(Node * l, Node * r);
Node *times_node(Node * l, Node * r);
Node *minus_node(Node * l, Node * r);
Node *integer_node(int val);
Node *divide_node(Node * l, Node * r);

#endif
