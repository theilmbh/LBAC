/*
 * =====================================================================================
 *
 *       Filename:  symbol.h
 *
 *    Description:  Symbol Table Management declarations
 *
 *        Version:  1.0
 *        Created:  09/20/2017 03:33:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Brad Theilman (BHT), bradtheilman@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include "cmplr.h"
#include "ast.h"

#define MAXSYM 256

enum symbol_type { 
    VAR_LOCAL, /* Locally defined variable (e.g function param) */
    VAR_GLOBAL,       /* Global Variables */ 
    FUNC      /* Function Declarations */ 
};

typedef struct Symbol {
    char name[MAXIDENT];
    enum symbol_type type;
    unsigned int addr;  /* If this is a local variable, gives position 
                           on stack.  If global, gives absolute address */
} Symbol;

struct env {
    struct env *prev;
    int symcount;
    Symbol *symtab[MAXSYM];
};

/* Create a new environment (variable scope)
 * Chain the new environment to the environment prev */
struct env *create_env(struct env *prev);

/* Check to see if a symbol is already defined
 * in the environment  */
int check_symbol(struct env * e, char * nm);

/*  Return the Symbol table entry for a specific node */
Symbol *find_symbol(Node * n, struct env *e);

/* Add a symbol to the current environment if possible */
void add_symbol(struct env * e, char * nm, enum symbol_type type);

#endif
