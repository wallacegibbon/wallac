#ifndef __TOKEN_H__
#define __TOKEN_H__


struct token { struct token *l, *r; short type; void *value; };


void
initialize_token_list();

struct token *
new_token(short type, void *value);

void
join_token(short type, void *p);

void
print_token_list();

struct token *
slide_tk(struct token **p);

void
print_token(struct token *t);

char *
token_type_str(short type);


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

#define TK_EOF 11501


#endif

