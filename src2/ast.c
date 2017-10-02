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

Node *paren_node(Node * expr, struct env *e) 
{
    Node *out = malloc(sizeof(Node));
    out->type = PAREN;
    out->value = 0;
    out->left = NULL;
    out->right = expr;
    out->e = e;
    return out;
}

Node *plus_node(Node * l, Node * r, struct env *e)
{
    Node *out = malloc(sizeof(Node));
    out->type = BIN_OP_PLUS;
    out->value = 0;
    out->left = l;
    out->right = r;
    out->e = e;
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

Node *minus_node(Node * l, Node * r, struct env *e)
{
    if (r->type == BIN_OP_PLUS || r->type == BIN_OP_MINUS ) {
        return plus_node(l, plus_node(negate(r->left), r->right, e), e);
    } else {
        return plus_node(l, negate(r), e);
    }
}

Node *times_node(Node * l, Node * r, struct env *e)
{
    Node *out = malloc(sizeof(Node));
    out->type = BIN_OP_TIMES;
    out->value = 0;
    out->left = l;
    out->right = r;
    out->e = e;
    return out;
}

Node *divide_node(Node * l, Node * r, struct env *e)
{
    Node *out = malloc(sizeof(Node));
    out->type = BIN_OP_DIVIDE;
    out->value = 0;
    out->left = l;
    out->right = r;
    out->e = e;
    return out;
}

Node *integer_node(int val, struct env *e)
{
    Node *out = malloc(sizeof(Node));
    out->type = INT;
    out->value = val;
    out->left = NULL;
    out->right = NULL;
    out->e = e;
    return out;
}

Node *var_node(char *var_name, struct env *e)
{
    Node *out = malloc(sizeof(Node));
    out->type = VAR;
    strcpy(out->name, var_name);
    out->left = NULL;
    out->right = NULL;
    out->e = e;
    return out;
}

Node *stmt_node(Node * l, Node * r, struct env *e)
{
    Node *out = malloc(sizeof(Node));
    out->type = STMT;
    out->left = l;
    out->right = r;
    out->e = e;
    return out;
}

Node *decl_node(Node * s, struct env *e)
{
    Node *out = malloc(sizeof(Node));
    out->type = DECL;
    out->left = s;
    out->right = NULL;
    out->e = e;
    return out;
}

Node *func_decl_node(Node * nm, Node * body, struct env *e)
{

    Node *out = malloc(sizeof(Node));
    out->type = FUNC_DECL;
    out->left = nm;
    out->right = body;
    out->e = e;
    return out;
}


