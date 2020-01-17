#ifndef __TOKEN_H__
#define __TOKEN_H__

#define TK_ASTERISK 10101

#define TK_DPLUS 10201
#define TK_PLUS 10202

#define TK_DMINUS 10301
#define TK_MINUS 10302
#define TK_POINTSTO 10303

#define TK_DIVIDE 10401
#define TK_MOD 10402

#define TK_ASSIGN 10501
#define TK_EQ 10502

#define TK_EXCLAMATION 10601
#define TK_NEQ 10602

#define TK_LT 10701
#define TK_LEQ 10702
#define TK_GT 10703
#define TK_GEQ 10704

#define TK_DOT 10801
#define TK_ELLIPSIS 10802

#define TK_TILDE 10901

#define TK_CARET 11001
#define TK_AND 11002
#define TK_OR 11003
#define TK_DAND 11004
#define TK_DOR 11005

#define TK_OPENPA 11101
#define TK_CLOSEPA 11102
#define TK_OPENBR 11103
#define TK_CLOSEBR 11104
#define TK_BEGIN 11105
#define TK_END 11106

#define TK_QUESTION 11201
#define TK_COLON 11202

#define TK_SEMICOLON 11301
#define TK_COMMA 11302

#define TK_CINT 11401
#define TK_CCHAR 11403
#define TK_CSTR 11404
#define TK_IDENT 11405

#define KW_IF 11501
#define KW_ELSE 11502
#define KW_WHILE 11503

#define KW_SWITCH 11504
#define KW_CASE 11505
#define KW_DEFAULT 11506
#define KW_FOR 11507
#define KW_DO 11508

#define KW_RETURN 11509
#define KW_GOTO 11510
#define KW_CONTINUE 11511
#define KW_BREAK 11512

#define KW_VIOD 11513
#define KW_CHAR 11514
#define KW_INT 11515
#define KW_FLOAT 11516
#define KW_DOUBLE 11517

#define KW_SHORT 11518
#define KW_LONG 11519
#define KW_SIGNED 11520
#define KW_UNSIGNED 11521

#define KW_STRUCT 11522
#define KW_UNION 11523
#define KW_ENUM 11524

#define KW_TYPEDEF 11525
#define KW_SIZEOF 11526

#define KW_EXTERN 11527
#define KW_VOLATILE 11528
#define KW_CONST 11529
#define KW_AUTO 11530
#define KW_STATIC 11531
#define KW_REGISTER 11532

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
