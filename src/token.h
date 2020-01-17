#ifndef __TOKEN_H__
#define __TOKEN_H__

#define TOKEN_IS_KW(tk) (((tk) & 0xf000) == 0x5000)
#define TOKEN_IS_OPERATOR(tk) (((tk) & 0xf000) == 0)

enum TokenTypes {
	TK_ASTERISK = 0x0001,
	TK_DPLUS,
	TK_PLUS,
	TK_DMINUS,
	TK_MINUS,
	TK_POINTSTO,
	TK_DIVIDE,
	TK_MOD,
	TK_ASSIGN,
	TK_EQ,
	TK_EXCLAMATION,
	TK_NEQ,
	TK_LT,
	TK_LEQ,
	TK_GT,
	TK_GEQ,
	TK_DOT,
	TK_ELLIPSIS,
	TK_TILDE,
	TK_CARET,
	TK_AND,
	TK_OR,
	TK_DAND,
	TK_DOR,
	TK_OPENPA,
	TK_CLOSEPA,
	TK_OPENBR,
	TK_CLOSEBR,
	TK_BEGIN,
	TK_END,
	TK_QUESTION,
	TK_COLON,
	TK_SEMICOLON,
	TK_COMMA,

	TK_CINT = 0x2001,
	TK_CCHAR = 0x3001,

	TK_CSTR = 0x4001,
	TK_IDENT = 0x4002,

	KW_IF = 0x5001,
	KW_ELSE,
	KW_SWITCH,
	KW_CASE,
	KW_DEFAULT,
	KW_RETURN,
	KW_GOTO,
	KW_FOR,
	KW_WHILE,
	KW_DO,
	KW_CONTINUE,
	KW_BREAK,
	KW_VIOD,
	KW_CHAR,
	KW_INT,
	KW_FLOAT,
	KW_DOUBLE,
	KW_SHORT,
	KW_LONG,
	KW_SIGNED,
	KW_UNSIGNED,
	KW_STRUCT,
	KW_UNION,
	KW_ENUM,
	KW_TYPEDEF,
	KW_SIZEOF,
	KW_EXTERN,
	KW_VOLATILE,
	KW_CONST,
	KW_AUTO,
	KW_STATIC,
	KW_REGISTER
};

struct token {
	struct token *next, *prev;
	int type;
	int line;
	char *fname;
	void *value;
};

struct token *new_token(int type, void *value);

struct token *copy_token_chain(struct token *orig);

void print_token_list(struct token *start);

char *token_type_str(int type);

int try_get_keyword(char *identifier);

#endif
