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
	char symbol;
	/* union of attributes */
};

#define N_TOKENS 8
char token_str[N_TOKENS] = {'+', '-', '[', ']', '(', ')', 'd', '%'};
token_type_t token_types[N_TOKENS] = {
	PLUS_OPERATOR,
	MINUS_OPERATOR,
	RBRACK,
	LBRACK,
	RPAREN,
	LPAREN,
	D_D,
	PERCENT
};

struct parse_tree_node {
	struct token tk;
}