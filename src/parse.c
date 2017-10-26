#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmplr.h"
#include "algebra.h"
#include "symbol.h"

/* Current Token */
Token *tok;

/* Source file */
FILE *source;

void match(token_t type)
{
    /* Accept a token matching a specific type */
    if (tok->type == type) {
	read_one_token(tok, source);
    } else {
	printf("Syntax Error\n");
	exit(-1);
    }
}

/* ABSTRACT SYNTAX TREE FUNCTIONS */

/* Negate a node
 * by multiplying by -1 */
Node *negate(Node * n)
{
    return times_node(integer_node(-1), n);
}

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
    return plus_node(l, times_node(integer_node(-1), r));
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

/* Grammar functions corresponding to Nonterminals */
Node *factor()
{
    // printf("factor\n");
    Node *out = NULL;
    /* Just a plain integer */
    if (tok->type == INTEGER) {
	out = integer_node(tok->val.int_val);
	match(INTEGER);
	return out;
    } else if (tok->type == L_PAREN) {
	/* parenthetical expression */
	match(L_PAREN);
	out = expression();
	match(R_PAREN);
	return out;
    } else if (tok->type == IDENT) {
	/* Variable */
	out = var_node(tok->val.ident);
	match(IDENT);
	return out;
    } else {
	/* IDK what we got */
	printf("Syntax Error\n");
	return NULL;
    }
}

Node *term()
{
    /* parse a term */
    Node *out = NULL;

    /* First, parse any factors */
    out = factor();

    /* Times operation */
    if (tok->type == OP && tok->val.op == MULTIPLY) {
	match(OP);
	out = times_node(out, term());
	return out;
    }

    /* Divide operation */
    if (tok->type == OP && tok->val.op == DIVIDE) {
	match(OP);
	out = divide_node(out, term());
	return out;
    }
    return out;
}

Node *expression()
{

    /* Parse an expression */
    Node *out = NULL;
    Node *ph = NULL;
    ph = term();

    /* Plus operation */
    if (tok->type == OP && tok->val.op == PLUS) {
	match(OP);
	out = plus_node(ph, expression());
	return out;
    }

    /* Minus Operation */
    if (tok->type == OP && tok->val.op == MINUS) {
	match(OP);
	out = minus_node(ph, expression());
	return out;
    }
    return ph;
}

void assignment()
{
    Node *var, *expr;
    match(KW_INT);
    if (tok->type == IDENT) {
	var = var_node(tok->val.ident);
	match(IDENT);
	match(ASSIGN_OP);
	expr = expression();
	match(SEMICOLON);
	add_symbol(var, expr);
    }
}

Node *statement()
{

    Node *out = NULL;
    if (tok->type == KW_INT) {
	assignment();
	out = statement();
    } else {
	out = expression();
    }
    if (tok->type == ENDA) {
        return out;
    }
    return out;
}

/* Main Parsing Routine */
Node *parse()
{
    Node *ast;
    Token mytok;
    tok = &mytok;

    /* Load the lookahead with the first token */
    read_one_token(tok, source);
    // print_token(tok);

    /* Let's go! */
    ast = statement();
    return ast;
}

Node *attach_variables(Node * ast)
{
    if (ast == NULL) {
        return NULL;
    }

    if (ast->type == VAR) {
	Symbol *sym = find_symbol(ast);
	return attach_variables(sym->expr);
    }

    if (ast->type == INT) {
	return ast;
    }

    ast->left = attach_variables(ast->left);
    ast->right = attach_variables(ast->right);
    return ast;
}

void print_expression(Node * ast)
{

    /* If it's an operator, print the left side,
       then the operator, then the right side */
    if (ast->type != INT && ast->type != VAR) {
	print_expression(ast->left);
	switch (ast->type) {
	case BIN_OP_TIMES:
	    printf("*");
	    break;
	case BIN_OP_PLUS:
	    printf(" + ");
	    break;
	case BIN_OP_MINUS:
	    printf(" - ");
	    break;
	}
	print_expression(ast->right);
    }

    /* Otherwise just print the value */
    if (ast->type == INT) {
	printf("%d", ast->value);
    }
    if (ast->type == VAR) {
	printf("%s", ast->name);
    }

}

void print_node(FILE * out, Node * n, int indent)
{
    // for(int i=0; i<indent; i++) {
    //   printf(" ");
    // }
    switch (n->type) {
    case BIN_OP_TIMES:
	fprintf(out, "P Multiply\n");
	break;
    case BIN_OP_PLUS:
	fprintf(out, "P Add\n");
	break;
    case BIN_OP_MINUS:
	fprintf(out, "P Minus\n");
	break;
    case BIN_OP_DIVIDE:
	fprintf(out, "P Divide\n");
	break;
    case INT:
	fprintf(out, "P Integer: %d\n", n->value);
	break;
    case VAR:
	fprintf(out, "P Varible: %s\n", n->name);
	break;
    case PAREN:
        print_node(out, n->right, indent);
        break;
    }
}

void print_ast(FILE * out, Node * n, int indent, int levelmin, int levelmax)
{
    int i,j;
    int iskip = 4;
    print_node(out, n, indent);
    if (n->left != NULL) {
        for (i = 0; i < levelmax; i++) {
            if (i>levelmin) {
                fprintf(out, "|");
                iskip -=1;
            }
            for (j = 0; j < iskip; j++) {
                fprintf(out, " ");
            }
        }
	fprintf(out, "|-> ");
	print_ast(out, n->left, indent + 4, levelmin, levelmax+1);
    }
    iskip=4;
    if (n->right != NULL) {
        for (i = 0; i < levelmax; i++) {
            if (i>levelmin) {
                fprintf(out, "|");
                iskip -=1;
            }
            for (j = 0; j < iskip; j++) {
                fprintf(out, " ");
            }
        }
	fprintf(out, "|-> ");
	print_ast(out, n->right, indent + 4, levelmin+1, levelmax+1);
    }
}

#define PRINT_AST

int main(int argc, char **argv)
{
    //source = fopen(argv[1], "r");
    source = stdin;
    Node *ast, *ast1;
    int eval_num = 1;
    printf(" ******** WELCOME TO ********\n");
    printf(" ********   CASSIE   ********\n\n");
    printf(" Copyright (c) 2017 Brad Theilman\n");
    while (1) {
        printf("In  [%d] :=> ", eval_num);
        ast = parse();
        //ast1 = attach_variables(ast);
        //FILE *out = fopen("./ast.tree", "w");
#ifdef PRINT_AST
        print_ast(stdout, rewrite_minus(ast), 0, -1, 0);
#endif

        int v = evaluate(rewrite_minus(ast));
        printf("\nOut [%d] :=> %d\n\n", eval_num, v);
        eval_num++;
    }


    //
    // printf("Rewrite Minus: \n");
    // ast = rewrite_minus(ast);
    // print_ast(ast, 0);
    //
    // printf("Expanded: \n");
    // ast = expand(ast);
    // print_ast(ast, 0);
    //
    // printf("Reorder coeff: \n");
    // ast = reorder_coeff(ast);
    // print_ast(ast, 0);
    //
    // print_expression(ast);
    // printf("\n");
    //fclose(out);
    //fclose(source);
    return 0;
}
