#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmplr.h"

/* Current Token */
Token *tok;

/* Source file */
FILE *source;

/* Number of symbols in symbol table */
int symcount = 0;

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

int check_symbol(char *nm)
{
    /* Check if a symbol is already
     * defined in the symbol table */
    int i;
    for (i = 0; i < symcount; i++) {
	if (!strcmp(symtab[i]->name, nm)) {
	    return 1;
	}
    }
    return 0;
}

Symbol *find_symbol(Node * n)
{
    /* Return a reference to a previously defined symbol
     * Print an error and exit if the symbol is not defined */
    int i;
    for (i = 0; i < symcount; i++) {
	if (!strcmp(symtab[i]->name, n->name)) {
	    return symtab[i];
	}
    }
    printf("Error: Variable %s undeclared\n", n->name);
    exit(-1);
}

/* Add a symbol to the symbol table. 
 * Print Error and exit if symbol is already defined */
void add_symbol(Node * var, Node * expr)
{

    if (check_symbol(var->name)) {
	printf("Symbol %s already declared. Terminating.\n", var->name);
	exit(-1);
    }
    Symbol *sym = malloc(sizeof(Symbol));
    strcpy(sym->name, var->name);
    sym->expr = expr;
    symtab[symcount] = sym;
    symcount++;
}

/* ABSTRACT SYNTAX TREE FUNCTIONS */

/* Negate a node
 * by multiplying by -1 */
Node *negate(Node * n)
{
    return times_node(integer_node(-1), n);
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

/* Abstract Syntax Tree Rewriting Functions */
Node *rewrite_minus(Node * ast)
{
    /* Rewrite a - b as a + -1*b */
    if (ast->type == INT || ast->type == VAR) {
	return ast;
    }

    if (ast->type == BIN_OP_MINUS) {
	return plus_node(rewrite_minus(ast->left),
			 times_node(integer_node(-1),
				    rewrite_minus(ast->right)));
    } else {
	ast->left = rewrite_minus(ast->left);
	ast->right = rewrite_minus(ast->right);
	return ast;
    }
}

Node *reorder_coeff(Node * ast)
{
    /* Reorders coefficients so that integers are on left */
    Node *tmp;
    if (ast->type == INT || ast->type == VAR) {
	return ast;
    }

    if (ast->type == BIN_OP_TIMES
	&& ast->right->type == INT && ast->left->type != INT) {
	tmp = reorder_coeff(ast->left);
	ast->left = ast->right;
	ast->right = tmp;
	return ast;
    }
    ast->left = reorder_coeff(ast->left);
    ast->right = reorder_coeff(ast->right);
    return ast;
}

Node *expand(Node * ast)
{

    Node *out, *al;

    /* if node is int or var, just return it */
    if (ast->type == VAR || ast->type == INT) {
	return ast;
    }

    /* If node is add, expand children */
    if (ast->type == BIN_OP_PLUS || ast->type == BIN_OP_MINUS) {
	ast->left = expand(ast->left);
	ast->right = expand(ast->right);
	return ast;
    }

    /* if node is multiply, rewrite */
    if (ast->type == BIN_OP_TIMES) {
	if (ast->right->type == BIN_OP_PLUS) {
	    al = expand(ast->left);
	    out =
		plus_node(expand(times_node(al, expand(ast->right->left))),
			  expand(times_node
				 (al, expand(ast->right->right))));
	    return out;
	}
	if (ast->left->type == BIN_OP_PLUS) {
	    al = expand(ast->right);
	    out =
		plus_node(expand(times_node(expand(ast->left->left), al)),
			  expand(times_node
				 (expand(ast->left->right), al)));
	    return out;
	}

	/* Expand Children */
	ast->left = expand(ast->left);
	ast->right = expand(ast->right);

	/* check to see if we can distribute further */
	if (ast->left->type == BIN_OP_PLUS
	    || ast->right->type == BIN_OP_PLUS) {
	    return expand(ast);
	}
	/* return expanded node */
	return ast;
    }

    /* fall through */
    return ast;
}

int evaluate(Node * ast)
{
    /* evaluate an expression */
    int v1;
    if (ast->type == VAR) {
	Symbol *sym = find_symbol(ast);
	v1 = evaluate(sym->expr);
    }

    if (ast->type == INT) {
	return ast->value;
    }

    switch (ast->type) {
    case BIN_OP_PLUS:
	v1 = evaluate(ast->left) + evaluate(ast->right);
	break;
    case BIN_OP_TIMES:
	v1 = evaluate(ast->left) * evaluate(ast->right);
	break;
    case BIN_OP_MINUS:
	v1 = evaluate(ast->left) - evaluate(ast->right);
	break;
    }
    return v1;
}

Node *attach_variables(Node * ast)
{
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
    source = fopen(argv[1], "r");
    Node *ast, *ast1;
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
