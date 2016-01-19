/* autodiff.h */
/* Token types, data structures, etc */

/* token types */
#define PLUS_OPERATOR 1
#define MINUS_OPERATOR 2
#define IDENTIFIER 3
#define D_D 4
#define PERCENT 5
#define LBRACK 6
#define RBRACK 7
#define LPAREN 8
#define RPAREN 9

typedef int token_type_t;

struct token {
	token_type_t t_type;
	/* union of attributes */
}