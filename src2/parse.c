#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmplr.h"
#include "algebra.h"
#include "symbol.h"
#include "ast.h"
#include "parse.h"

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

int expect(token_t type)
{
    /* check if token is what we expect it to be */
    if (tok->type == type) {
        return 1;
    } else {
        printf("Syntax Error!\n");
        exit(-1);
    }
}

/* Grammar functions corresponding to Nonterminals */
Node *function_declaration(struct env * e)
{
    Node *out = NULL;
    if (expect(IDENT)) {
        struct env *new_e = create_env(e);
        Node *func_name = var_node(tok->val.ident);
        match(IDENT);
        match(L_PAREN);
        Node *func_arglist = args(new_e);
        match(R_PAREN);
        match(L_CURLY);
        Node *func_body = expression(new_e);
        match(R_CURLY);
        return func_decl_node(func_name, func_arglist, func_body, new_e);
    }
}

Node *args(struct env *e)
{
    Node *out = NULL;
    if (expect(IDENT)) {
        Node *x = var_node(tok->val.ident);
        match(IDENT);
        add_symbol(e, x->name, LOCAL_VAR);

        if (tok->type == COMMA) {
            match(COMMA);
            Node *y = args(e);
            out = args_node(x, y);
        } else {
            out = args_node(x, NULL);
        }
    }
    return out;
}

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
	return paren_node(out);
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

void assignment(struct env * e)
{
    Node *var, *expr;
    match(KW_INT);
    if (tok->type == IDENT) {
	add_symbol(e, tok->val.ident, VAR_LOCAL);
	var = var_node(tok->val.ident);
	match(IDENT);
	match(ASSIGN_OP);
	expr = expression();
	match(SEMICOLON);
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
    }
}

void print_ast(FILE * out, Node * n, int indent)
{
    int i;
    print_node(out, n, indent);
    if (n->left != NULL) {
	for (i = 0; i < indent; i++) {
	    fprintf(out, " ");
	}
	fprintf(out, "|-> ");
	print_ast(out, n->left, indent + 4);
    }
    if (n->right != NULL) {
	for (i = 0; i < indent; i++) {
	    fprintf(out, " ");
	}
	fprintf(out, "|-> ");
	print_ast(out, n->right, indent + 4);
    }
}

int main(int argc, char **argv)
{
    Node *ast, *ast1;
    source = fopen(argv[1], "r");
    if (source == NULL) {
        printf("Cannot open source file!\n");
        return -1;
    }

    ast = parse();
    printf("Attaching variables...\n");
    ast1 = attach_variables(ast);
    FILE *out = fopen("./ast.tree", "w");
    print_ast(out, rewrite_minus(ast1), 0);

    int v = evaluate(rewrite_minus(ast));
    printf("Expression evaluated to: %d\n", v);


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
    fclose(out);
    fclose(source);
    return 0;
}
