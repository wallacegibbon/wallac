#include <stdlib.h>
#include "libc.h"
#include "misc.h"
#include "linktbl.h"
#include "linklst.h"
#include "token.h"
#include "node.h"
#include "ctypes.h"
#include "limits.h"
#include "parser.h"
#include "cexprs.h"
#include "cstmts.h"

struct parser {
	struct token *tk;
	struct ast *ast;
};

struct stmt *get_one_statement(struct parser *psr, struct cfunc *fn);

int check_unsupported_keyword(int type)
{
	return type == KW_REGISTER || type == KW_AUTO || type == KW_CONST ||
	    type == KW_VOLATILE || type == KW_STATIC ||
	    type == KW_FLOAT || type == KW_DOUBLE ||
	    type == KW_ENUM || type == KW_UNION ||
	    type == KW_SWITCH ||
	    type == KW_CASE ||
	    type == KW_WHILE ||
	    type == KW_DO ||
	    type == KW_CONTINUE ||
	    type == KW_BREAK || type == KW_GOTO || type == KW_TYPEDEF;
}

int is_typetoken(int type)
{
	return type == KW_STRUCT || type == KW_INT || type == KW_CHAR ||
	    type == KW_UNSIGNED || type == KW_SIGNED ||
	    type == KW_SHORT || type == KW_LONG || type == KW_EXTERN;
}

int filter_unsupported_tk(struct token *tk)
{
	if (tk->type == TK_OPENBR || tk->type == TK_CLOSEBR)
		exit_with
		    ("%s:%d:[PARSER]Original C array is not supported by wcc\n",
		     tk->fname, tk->line);

	if (tk->type == TK_QUESTION)
		exit_with
		    ("%s:%d:[PARSER]Ternary operation(?:) is not supported\n",
		     tk->fname, tk->line);

	if (check_unsupported_keyword(tk->type))
		exit_with("%s:%d:[PARSER]Unsupported keyword: \"%s\"\n",
			  tk->fname, tk->line, token_type_str(tk->type));

	return 1;
}

struct token *nexttoken(struct parser *psr)
{
	struct token *tk;

	if (!psr->tk)
		return NULL;

	tk = psr->tk->next;

	if (tk)
		filter_unsupported_tk(tk);

	psr->tk = tk;
	return tk;
}

struct token *nexttoken_notend(struct parser *psr)
{
	struct token *tk;

	tk = nexttoken(psr);
	if (!tk)
		exit_with("[PARSER]Unexpected end of file\n");

	return tk;
}

int get_int_basic(struct parser *psr)
{
	struct token *tk;

	tk = psr->tk;
	if ((tk->type == KW_SHORT || tk->type == KW_LONG) &&
	    tk->next->type == KW_INT)
		nexttoken_notend(psr);

	nexttoken_notend(psr);

	if (tk->type == KW_CHAR)
		return CT_CHAR;

	if (tk->type == KW_SHORT)
		return CT_SHORT;

	if (tk->type == KW_INT)
		return CT_INT;

	if (tk->type == KW_LONG)
		return CT_LONG;

	exit_with("%s:%d:[PARSER]Invalid type token(%d)\n",
		  tk->fname, tk->line, tk->type);

	return 1;
}

struct ctype *get_unsigned_type(struct parser *psr)
{
	struct ctype *ct;
	int type;

	nexttoken_notend(psr);
	type = get_int_basic(psr);
	type |= 0x10;

	ct = new_ctype(type, 0, NULL, 0);
	return ct;
}

struct ctype *get_signed_type(struct parser *psr)
{
	struct ctype *ct;
	int type;

	nexttoken_notend(psr);
	type = get_int_basic(psr);

	ct = new_ctype(type, 0, NULL, 0);
	return ct;
}

struct ctype *get_void_type(struct parser *psr)
{
	struct ctype *ct;

	ct = new_ctype(CT_VOID, 0, NULL, 0);
	nexttoken_notend(psr);

	return ct;
}

struct ctype *get_struct_type(struct parser *psr)
{
	struct ctype *ct;
	struct token *tk;
	char *name;

	tk = nexttoken_notend(psr);
	if (tk->type != TK_IDENT)
		exit_with("%s:%d:[PARSER]Name for struct not specified\n",
			  tk->fname, tk->line);

	name = (void *)tk->value;

	if (!linktbl_keyexist(psr->ast->sdefs, name))
		exit_with("%s:%d:[PARSER]Undefined struct \"%s\"\n",
			  tk->fname, tk->line, name);

	nexttoken_notend(psr);

	ct = new_ctype(CT_STRUCT, 0, name, 0);

	return ct;
}

struct ctype *get_basic_type(struct parser *psr)
{
	int type;

	type = psr->tk->type;

	if (type == KW_STRUCT)
		return get_struct_type(psr);

	if (type == KW_VIOD)
		return get_void_type(psr);

	if (type == KW_UNSIGNED)
		return get_unsigned_type(psr);

	if (type == KW_SIGNED)
		return get_signed_type(psr);

	type = get_int_basic(psr);
	return new_ctype(type, 0, NULL, 0);
}

int p_space(int num)
{
	for (; num > 0; num--)
		pf(" ");
}

int ctype_print(struct ctype *ct)
{
	char *tstr;
	int i;

	if (ct->is_extern)
		pf("extern ");

	tstr = ctype_tostr(ct->type);

	if (ct->type == CT_STRUCT)
		pf("struct %s ", ct->struct_name);
	else
		pf("%s ", tstr);

	for (i = 0; i < ct->pdepth; i++)
		pf("*");
}

int cvar_print(struct cvar *cv, int offset)
{
	p_space(offset);
	pf("%s\t-> ", cv->name);
	ctype_print(cv->type);
	pf("\n");

	return 1;
}

int varlist_print(struct linktbl *vl, int offset)
{
	struct tblnode *n;;

	n = vl->chain;
	for (; n; n = n->next)
		cvar_print((struct cvar *)n->value, offset);

	return 1;
}

int struct_print(struct cstruct *cs)
{
	pf("\nSTRUCT %s:\n", cs->name);
	return varlist_print(cs->fields, 2);
}

int structlist_print(struct linktbl *sl)
{
	struct tblnode *n;

	n = sl->chain;
	for (; n; n = n->next)
		struct_print((struct cstruct *)n->value);

	return 1;
}

int func_print(struct cfunc *cf)
{
	pf("\nFUNCTION %s -> ", cf->name);
	ctype_print(cf->ret);

	pf("\nPARAMETERS:\n");
	if (cf->params)
		varlist_print(cf->params, 2);

	if (cf->var_arg)
		pf("  ...\n");

	pf("ARGUMENTS:\n");
	if (cf->vars)
		varlist_print(cf->vars, 2);

	pf("STATEMENTS:\n");
	pf("\n");
}

int funclist_print(struct linktbl *fl)
{
	struct tblnode *n;

	n = fl->chain;
	for (; n; n = n->next)
		func_print((struct cfunc *)n->value);

	return 1;
}

int fill_pdepth(struct parser *psr, struct ctype *ct)
{
	struct token *tk;
	int pdepth;

	pdepth = 0;

	for (; psr->tk->type == TK_ASTERISK && pdepth < INT_MAX;
	     nexttoken_notend(psr))
		pdepth++;

	tk = psr->tk;

	if (pdepth == INT_MAX)
		exit_with("%s:%d:[PARSER]Pointer depth overflow\n",
			  tk->fname, tk->line);

	if (tk->type != TK_IDENT)
		exit_with("%s:%d:[PARSER]Missing identifer after type\n",
			  tk->fname, tk->line);

	ct->pdepth = pdepth;

	return 1;
}

int get_varlist(struct parser *psr, struct linktbl *vars, struct ctype *ct)
{
	struct ctype *nct;
	struct token *tk;
	struct cvar *cv;
	char *name;
	int i;

	fill_pdepth(psr, ct);

	tk = psr->tk;
	name = (char *)tk->value;
	cv = new_cvar(name, ct);

	i = linktbl_add(vars, name, (void *)cv);
	if (!i)
		exit_with("%s:%d:[PARSER]variable \"%s\" redefine\n",
			  tk->fname, tk->line, name);

	nexttoken_notend(psr);
	tk = psr->tk;

	nexttoken(psr);

	if (tk->type == TK_SEMICOLON)
		return 1;

	if (tk->type != TK_COMMA)
		exit_with("%s:%d:[PARSER]Missing ';' after variables\n",
			  tk->fname, tk->line);

	nct = ctype_copy(ct);
	return get_varlist(psr, vars, nct);
}

int get_funcparam_normal(struct parser *psr, struct cfunc *fn)
{
	struct token *tk;
	struct ctype *ct;
	struct cvar *cv;
	char *name;
	int i;

	ct = get_basic_type(psr);
	fill_pdepth(psr, ct);

	tk = psr->tk;
	name = (char *)tk->value;
	cv = new_cvar(name, ct);

	i = linktbl_add(fn->params, name, (void *)cv);
	if (!i)
		exit_with("%s:%d:[PARSER]parameter \"%s\" redefine\n",
			  tk->fname, tk->line, name);

	nexttoken_notend(psr);

	return 1;
}

int get_funcparam_vararg(struct parser *psr, struct cfunc *fn)
{
	struct token *tk;

	tk = psr->tk;

	if (fn->params->size == 0)
		exit_with
		    ("%s:%d:[PARSER]\"...\" can not be the first parameter\n",
		     tk->fname, tk->line);

	if (tk->next->type != TK_CLOSEPA)
		exit_with
		    ("%s:%d:[PARSER]\"...\" should be the last parameter\n",
		     tk->fname, tk->line);

	fn->var_arg = 1;

	nexttoken_notend(psr);
	nexttoken_notend(psr);

	return 1;
}

int get_funcparams(struct parser *psr, struct cfunc *fn)
{
	struct token *tk;

	if (psr->tk->type == TK_ELLIPSIS)
		return get_funcparam_vararg(psr, fn);

	get_funcparam_normal(psr, fn);

	tk = psr->tk;
	nexttoken_notend(psr);

	if (tk->type == TK_CLOSEPA)
		return 1;

	if (tk->type == TK_SEMICOLON)
		exit_with("%s:%d:[PARSER]Use ',' to split parameters\n",
			  tk->fname, tk->line);

	if (tk->type != TK_COMMA)
		exit_with("%s:%d:[PARSER]Missing ')' after parameters\n",
			  tk->fname, tk->line);

	return get_funcparams(psr, fn);
}

char *check_name_conflict(struct linktbl *t1, struct linktbl *t2)
{
	struct tblnode *c;
	int r;

	for (c = t1->chain; c; c = c->next)
		if (linktbl_keyexist(t2, c->key))
			return c->key;

	return NULL;
}

int get_funcvars(struct parser *psr, struct cfunc *fn)
{
	struct token *tk;
	struct ctype *ct;
	char *n;

	tk = psr->tk;
	ct = get_basic_type(psr);
	get_varlist(psr, fn->vars, ct);

	n = check_name_conflict(fn->params, fn->vars);
	if (n)
		exit_with("%s:%d:[PARSER]Variable name (%s) conflict\n",
			  tk->fname, tk->line, n);

	if (is_typetoken(psr->tk->type))
		return get_funcvars(psr, fn);
	else
		return 1;
}

struct expr *get_expression(struct parser *psr)
{
	struct expr *exp;
	struct token *tk;

	tk = psr->tk;
	//if (tk->type == TK_OPENPA)

	return exp;
}

struct stmt *get_expr_stmt(struct parser *psr, struct cfunc *fn)
{
	struct stmt *stmt;
	struct expr *exp;
	struct token *tk;

	exp = get_expression(psr);
	stmt = new_stmt_expr(exp);

	tk = psr->tk;
	if (tk->type != TK_SEMICOLON)
		exit_with("%s:%d:[PARSER]Expression statement missing \";\"\n",
			  tk->fname, tk->line);

	nexttoken_notend(psr);

	return stmt;
}

struct stmt *get_ret_stmt(struct parser *psr, struct cfunc *fn)
{
	struct stmt *stmt;
	struct expr *exp;
	struct token *tk;

	nexttoken_notend(psr);
	get_expression(psr);

	exp = get_expression(psr);
	stmt = new_stmt_ret(exp);

	tk = psr->tk;
	if (tk->type != TK_SEMICOLON)
		exit_with("%s:%d:[PARSER]Return statement missing \";\"\n",
			  tk->fname, tk->line);

	nexttoken_notend(psr);

	return stmt;
}

struct stmt *get_else_part(struct parser *psr, struct cfunc *fn)
{
	struct stmt *stmt;
	struct token *tk;

	nexttoken_notend(psr);
	stmt = get_one_statement(psr, fn);

	return stmt;
}

struct stmt *get_if_stmt(struct parser *psr, struct cfunc *fn)
{
	struct stmt *stmt, *then_s, *else_s;
	struct expr *cond;
	struct token *tk;

	nexttoken_notend(psr);

	tk = psr->tk;
	if (tk->type != TK_OPENPA)
		exit_with("%s:%d:[PARSER]if statement missing \"(\"\n",
			  tk->fname, tk->line);

	nexttoken_notend(psr);
	cond = get_expression(psr);

	tk = psr->tk;
	if (tk->type != TK_CLOSEPA)
		exit_with("%s:%d:[PARSER]if statement missing \")\"\n",
			  tk->fname, tk->line);

	nexttoken_notend(psr);
	then_s = get_one_statement(psr, fn);

	if (tk->type == KW_ELSE)
		else_s = get_else_part(psr, fn);
	else
		else_s = NULL;

	stmt = new_stmt_if(cond, then_s, else_s);

	return stmt;
}

struct stmt *get_for_stmt(struct parser *psr, struct cfunc *fn)
{
	struct stmt *stmt, *body;
	struct expr *e1, *e2, *e3;
	struct token *tk;

	nexttoken_notend(psr);

	tk = psr->tk;
	if (tk->type != TK_OPENPA)
		exit_with("%s:%d:[PARSER]for statement missing \"(\"\n",
			  tk->fname, tk->line);

	nexttoken_notend(psr);
	e1 = get_expression(psr);

	tk = psr->tk;
	if (tk->type != TK_SEMICOLON)
		exit_with("%s:%d:[PARSER]for statement e1 missing \";\"\n",
			  tk->fname, tk->line);

	nexttoken_notend(psr);
	e2 = get_expression(psr);

	tk = psr->tk;
	if (tk->type != TK_SEMICOLON)
		exit_with("%s:%d:[PARSER]for statement e2 missing \";\"\n",
			  tk->fname, tk->line);

	nexttoken_notend(psr);
	e3 = get_expression(psr);

	tk = psr->tk;
	if (tk->type != TK_CLOSEPA)
		exit_with("%s:%d:[PARSER]for statement missing \")\"\n",
			  tk->fname, tk->line);

	nexttoken_notend(psr);

	body = get_one_statement(psr, fn);

	stmt = new_stmt_for(e1, e2, e3, body);

	return stmt;
}

struct stmt *get_one_statement(struct parser *psr, struct cfunc *fn)
{
	struct token *tk;

	tk = psr->tk;

	if (tk->type == KW_FOR)
		return get_for_stmt(psr, fn);

	if (tk->type == KW_IF)
		return get_if_stmt(psr, fn);

	if (tk->type == KW_RETURN)
		return get_ret_stmt(psr, fn);

	return get_expr_stmt(psr, fn);
}

int get_statements(struct parser *psr, struct cfunc *fn)
{
	for (; psr->tk->type != TK_END;)
		linklst_push(fn->stmts, (void *)get_one_statement(psr, fn));

	return 1;
}

int skip_until_end_token(struct parser *psr)
{
	for (; psr->tk->type != TK_END;)
		nexttoken_notend(psr);

	nexttoken(psr);

	return 1;
}

int get_funcbody(struct parser *psr, struct cfunc *fn)
{
	if (is_typetoken(psr->tk->type))
		get_funcvars(psr, fn);

	skip_until_end_token(psr);
	//get_statements(psr, fn);

	return 1;
}

int cmp_params(struct linktbl *oldparams, struct linktbl *params)
{
	struct tblnode *c1, *c2;
	int i;

	if (oldparams->size != params->size)
		return -1;

	c1 = oldparams->chain;
	c2 = params->chain;

	for (i = 1; c1 && c2; c1 = c1->next, c2 = c2->next, i++)
		if (!ctype_cmp(((struct cvar *)c1->value)->type,
			       ((struct cvar *)c2->value)->type))
			return i;

	return 0;
}

int free_params(struct linktbl *params)
{
	struct tblnode *p;

	p = params->chain;
	for (; p; p = p->next)
		cvar_free((struct cvar *)p->value);

	return 1;
}

int cmp_functions(struct cfunc *f1, struct cfunc *f2)
{
	int i;

	if (!ctype_cmp(f1->ret, f2->ret))
		return -1;

	i = cmp_params(f1->params, f2->params);
	if (i < 0)
		return -2;

	return i;
}

int update_function(struct parser *psr, struct cfunc *fn, int line)
{
	struct linktbl *funclist;
	struct cfunc *oldfn;
	char *name;

	funclist = psr->ast->funcs;
	name = fn->name;

	oldfn = linktbl_get(funclist, name);

	if (!oldfn->is_declare)
		exit_with("%s:%d:[PARSER]Function \"%s\" redefine\n",
			  psr->tk->fname, line, name);

	if (cmp_functions(oldfn, fn))
		exit_with("%s:%d:[PARSER]Function \"%s\" definition conflict\n",
			  psr->tk->fname, line, name);

	linktbl_set(psr->ast->funcs, name, (void *)fn);

	free_params(oldfn->params);
	ctype_free(oldfn->ret);
	free(oldfn);

	return 1;
}

int join_function(struct parser *psr, struct cfunc *fn, int line)
{
	struct linktbl *funclist;
	char *name;

	funclist = psr->ast->funcs;
	name = fn->name;

	if (linktbl_keyexist(funclist, name))
		update_function(psr, fn, line);
	else
		linktbl_add(funclist, name, (void *)fn);

	return 1;
}

int get_function(struct parser *psr, struct ctype *ret)
{
	struct cfunc *fn;
	struct token *tk;
	char *name;
	int line;

	fill_pdepth(psr, ret);

	tk = psr->tk;
	name = (char *)tk->value;
	line = tk->line;

	fn = new_cfunc(name, ret, NULL, NULL, NULL, 0);

	nexttoken_notend(psr);
	nexttoken_notend(psr);

	if (psr->tk->type != TK_CLOSEPA)
		get_funcparams(psr, fn);
	else
		nexttoken_notend(psr);

	join_function(psr, fn, line);

	tk = psr->tk;
	nexttoken_notend(psr);

	if (tk->type == TK_BEGIN)
		return get_funcbody(psr, fn);

	if (tk->type != TK_SEMICOLON)
		exit_with("%s:%d:[PARSER]Missing function body\n",
			  tk->fname, tk->line);

	fn->is_declare = 1;
	return 1;
}

int is_gvar_definition(struct parser *psr)
{
	struct token *tk;

	for (tk = psr->tk; tk && tk->type == TK_ASTERISK;)
		tk = tk->next;

	if (tk && tk->next && tk->next->type == TK_OPENPA)
		return 0;
	else
		return 1;
}

int is_struct_definition(struct parser *psr)
{
	struct token *tk;

	tk = psr->tk;
	if (tk->type == KW_STRUCT && tk->next && tk->next->next &&
	    tk->next->next->type == TK_BEGIN)
		return 1;
	else
		return 0;
}

int get_struct_fields(struct parser *psr, struct cstruct *cs)
{
	struct token *tk;
	struct ctype *ct;

	ct = get_basic_type(psr);
	get_varlist(psr, cs->fields, ct);

	if (psr->tk->type != TK_END)
		return get_struct_fields(psr, cs);
	else
		return 1;
}

int check_type_struct(struct ctype *ct, char *sname)
{
	if (ct->type == CT_STRUCT && !scmp(ct->struct_name, sname) &&
	    ct->pdepth == 0)
		return 1;
	else
		return 0;
}

int check_recursive_struct(struct cstruct *cs)
{
	struct tblnode *p;
	char *name;

	name = cs->name;
	p = cs->fields->chain;

	for (; p, p; p = p->next)
		if (check_type_struct(((struct cvar *)p->value)->type, name))
			return 1;

	return 0;
}

int get_struct_definition(struct parser *psr)
{
	struct cstruct *cs;
	struct token *tk;
	char *name;

	tk = nexttoken_notend(psr);
	name = (char *)tk->value;

	cs = new_cstruct(name, NULL);
	linktbl_add(psr->ast->sdefs, name, (void *)cs);

	nexttoken_notend(psr);
	nexttoken_notend(psr);

	get_struct_fields(psr, cs);

	if (check_recursive_struct(cs))
		exit_with("%s:%d:[PARSER]Recursive struct \"%s\"\n",
			  tk->fname, tk->line, name);

	tk = psr->tk;
	nexttoken_notend(psr);

	if (psr->tk->type != TK_SEMICOLON)
		exit_with("%s:%d:[PARSER]Missing ';' after struct definition\n",
			  tk->fname, tk->line);

	nexttoken_notend(psr);

	return 1;
}

int get_variables_or_func(struct parser *psr)
{
	struct ctype *ct;
	int is_extern;

	if (psr->tk->type == KW_EXTERN)
		is_extern = 1;
	else
		is_extern = 0;

	if (is_extern)
		nexttoken_notend(psr);

	ct = get_basic_type(psr);
	ct->is_extern = is_extern;

	if (is_gvar_definition(psr))
		return get_varlist(psr, psr->ast->gvars, ct);
	else
		return get_function(psr, ct);
}

struct ast *new_ast()
{
	struct ast *p;

	p = malloc(sizeof(struct ast));
	if (!p)
		exit_with("Failed alloc memory for ast\n");

	p->funcs = new_linktbl();
	p->gvars = new_linktbl();
	p->sdefs = new_linktbl();

	return p;
}

struct parser *new_parser(struct token *tks)
{
	struct parser *p;

	p = malloc(sizeof(struct parser));
	if (!p)
		exit_with("Failed alloc memory for parser\n");

	p->ast = new_ast();
	p->tk = tks;

	return p;
}

int free_parser(struct parser *psr)
{
	//free_token(psr->tk);
	free(psr);
}

int parse_recur(struct parser *psr)
{
	struct token *tk;

	for (; psr->tk && psr->tk->type == TK_SEMICOLON;)
		nexttoken(psr);

	if (!psr->tk)
		return 1;

	if (is_struct_definition(psr))
		get_struct_definition(psr);
	else
		get_variables_or_func(psr);

	return parse_recur(psr);
}

struct ast *parse(struct token *tks)
{
	struct parser *psr;
	struct ast *r;

	psr = new_parser(tks);

	parse_recur(psr);
	r = psr->ast;

	pf("Global variables:\n");
	varlist_print(r->gvars, 2);
	structlist_print(r->sdefs);
	funclist_print(r->funcs);

	free_parser(psr);

	return r;
}
