/*
 * =====================================================================================
 *
 *       Filename:  parse.h
 *
 *    Description:  Declarations for parsing the language
 *
 *        Version:  1.0
 *        Created:  09/22/2017 02:10:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Brad Theilman (BHT), bradtheilman@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef PARSE_H
#define PARSE_H

#include "ast.h"

Node *factor();
Node *expression();
Node *term();

#endif
