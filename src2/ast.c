/*
 * =====================================================================================
 *
 *       Filename:  ast.c
 *
 *    Description:  Abstract Syntax Tree Functions
 *
 *        Version:  1.0
 *        Created:  09/22/2017 01:54:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Brad Theilman (BHT), bradtheilman@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <string.h>
#include "ast.h"
#include "algebra.h"
#include <stdlib.h>

Node *paren_node(Node * expr) 
{
    Node *out = malloc(sizeof(Node));
    out->type = PAREN;
    out->value = 0;
    out->left = NULL;
    out->right = expr;
    return out;
}

Node *plus_node(Node * l, Node * r)
{
    Node *out = malloc(sizeof(Node));
    out->type = BIN_OP_PLUS;
    out->value = 0;
    out->left = l;
    out->right = r;
    return out;
}

/* Node *minus_node(Node * l, Node * r)
{
    Node *out = malloc(sizeof(Node));
    out->type = BIN_OP_MINUS;
    out->value = 0;
    out->left = l;
    out->right = r;
    return out;
}*/

Node *minus_node(Node * l, Node * r)
{
    if (r->type == BIN_OP_PLUS || r->type == BIN_OP_MINUS ) {
        return plus_node(l, plus_node(negate(r->left), r->right));
    } else {
        return plus_node(l, negate(r));
    }
}

Node *times_node(Node * l, Node * r)
{
    Node *out = malloc(sizeof(Node));
    out->type = BIN_OP_TIMES;
    out->value = 0;
    out->left = l;
    out->right = r;
    return out;
}

Node *divide_node(Node * l, Node * r)
{
    Node *out = malloc(sizeof(Node));
    out->type = BIN_OP_DIVIDE;
    out->value = 0;
    out->left = l;
    out->right = r;
    return out;
}

Node *integer_node(int val)
{
    Node *out = malloc(sizeof(Node));
    out->type = INT;
    out->value = val;
    out->left = NULL;
    out->right = NULL;
    return out;
}

Node *var_node(char *var_name)
{
    Node *out = malloc(sizeof(Node));
    out->type = VAR;
    strcpy(out->name, var_name);
    out->left = NULL;
    out->right = NULL;
    return out;
}
