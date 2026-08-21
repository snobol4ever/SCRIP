#include "prolog_parse.h"
#include "prolog_lex.h"
#include "prolog_atom.h"
#include "term.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_VARS 256
typedef struct {
    char *name;
    Term *term;
} VarEntry;
typedef struct {
    VarEntry entries[MAX_VARS];
    int      count;
} VarScope;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void scope_reset(VarScope *sc) { sc->count = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *scope_get(VarScope *sc, const char *name) {
    for (int i = 0; i < sc->count; i++)
        if (strcmp(sc->entries[i].name, name) == 0)
            return sc->entries[i].term;
    if (sc->count >= MAX_VARS) {
        fprintf(stderr, "too many variables in one clause\n");
        return term_new_var(-1);
    }
    Term *v = term_new_var(-1);
    sc->entries[sc->count].name = strdup(name);
    sc->entries[sc->count].term = v;
    sc->count++;
    return v;
}
#define IF_STACK_MAX 32
typedef struct {
    int active;
    int taken;
    int parent_active;
    int line;
} IfFrame;
typedef struct {
    Lexer      lx;
    VarScope   sc;
    const char *filename;
    int         nerrors;
    int         in_args;
    IfFrame     ifst[IF_STACK_MAX];
    int         ifst_top;
} Parser;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int if_currently_active(const Parser *p) {
    if (p->ifst_top == 0) return 1;
    return p->ifst[p->ifst_top - 1].active;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void perror_at(Parser *p, int line, const char *msg) {
    fprintf(stderr, "%s:%d: parse error: %s\n", p->filename, line, msg);
    p->nerrors++;
}
typedef enum { ASSOC_NONE, ASSOC_LEFT, ASSOC_RIGHT } Assoc;
typedef enum { FIX_INFIX, FIX_PREFIX, FIX_POSTFIX } Fixity;
typedef struct { const char *name; int prec; Assoc assoc; Fixity fixity; } OpEntry;
static const OpEntry BIN_OPS[] = {
    { ":-",   1200, ASSOC_NONE  },
    { "-->",  1200, ASSOC_NONE  },
    { ",",    1000, ASSOC_RIGHT },
    { ";",    1100, ASSOC_RIGHT },
    { "->",   1050, ASSOC_RIGHT },
    { "*->",  1050, ASSOC_RIGHT },
    { "@",     900, ASSOC_NONE  },
    { "=",     700, ASSOC_NONE  },
    { "\\=",   700, ASSOC_NONE  },
    { "==",    700, ASSOC_NONE  },
    { "\\==",  700, ASSOC_NONE  },
    { "is",    700, ASSOC_NONE  },
    { "<",     700, ASSOC_NONE  },
    { ">",     700, ASSOC_NONE  },
    { "=<",    700, ASSOC_NONE  },
    { ">=",    700, ASSOC_NONE  },
    { "=:=",   700, ASSOC_NONE  },
    { "=\\=",  700, ASSOC_NONE  },
    { "=..",   700, ASSOC_NONE  },
    { "=@=",   700, ASSOC_NONE  },
    { "\\=@=", 700, ASSOC_NONE  },
    { "?=",    700, ASSOC_NONE  },
    { "@<",    700, ASSOC_NONE  },
    { "@>",    700, ASSOC_NONE  },
    { "@=<",   700, ASSOC_NONE  },
    { "@>=",   700, ASSOC_NONE  },
    { "+",     500, ASSOC_LEFT  },
    { "-",     500, ASSOC_LEFT  },
    { "*",     400, ASSOC_LEFT  },
    { "/",     400, ASSOC_LEFT  },
    { "//",   400, ASSOC_LEFT  },
    { "mod",   400, ASSOC_LEFT  },
    { "div",   400, ASSOC_LEFT  },
    { "rem",   400, ASSOC_LEFT  },
    { "rdiv",  400, ASSOC_LEFT  },
    { ">>",    400, ASSOC_LEFT  },
    { "<<",    400, ASSOC_LEFT  },
    { "xor",   400, ASSOC_LEFT  },
    { "/\\",   500, ASSOC_LEFT  },
    { "\\/",   500, ASSOC_LEFT  },
    { "**",    200, ASSOC_RIGHT },
    { "^",     200, ASSOC_RIGHT },
    { ":",     200, ASSOC_RIGHT },
    { NULL,    0,   ASSOC_NONE  }
};
static OpEntry *g_uinfix = NULL;
static int g_uinfix_n = 0, g_uinfix_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void user_op_add(const char *name, int prec, Assoc assoc, Fixity fixity) {
    for (int i = 0; i < g_uinfix_n; i++) if (g_uinfix[i].fixity == fixity && strcmp(g_uinfix[i].name, name) == 0) { g_uinfix[i].prec = prec; g_uinfix[i].assoc = assoc; return; }
    if (g_uinfix_n >= g_uinfix_cap) { g_uinfix_cap = g_uinfix_cap ? g_uinfix_cap * 2 : 8; g_uinfix = (OpEntry *)realloc(g_uinfix, g_uinfix_cap * sizeof(OpEntry)); }
    g_uinfix[g_uinfix_n].name = strdup(name); g_uinfix[g_uinfix_n].prec = prec; g_uinfix[g_uinfix_n].assoc = assoc; g_uinfix[g_uinfix_n].fixity = fixity; g_uinfix_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const OpEntry *find_binop(const char *name) {
    for (const OpEntry *op = BIN_OPS; op->name; op++)
        if (strcmp(op->name, name) == 0) return op;
    for (int i = 0; i < g_uinfix_n; i++)
        if (g_uinfix[i].fixity == FIX_INFIX && strcmp(g_uinfix[i].name, name) == 0) return &g_uinfix[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const OpEntry *find_prefix(const char *name) {
    for (int i = 0; i < g_uinfix_n; i++)
        if (g_uinfix[i].fixity == FIX_PREFIX && strcmp(g_uinfix[i].name, name) == 0) return &g_uinfix[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const OpEntry *find_postfix(const char *name) {
    for (int i = 0; i < g_uinfix_n; i++)
        if (g_uinfix[i].fixity == FIX_POSTFIX && strcmp(g_uinfix[i].name, name) == 0) return &g_uinfix[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int op_type_classify(const char *type, Assoc *assoc_out, Fixity *fix_out) {
    if (strcmp(type, "xfx") == 0) { *fix_out = FIX_INFIX;   *assoc_out = ASSOC_NONE;  return 1; }
    if (strcmp(type, "xfy") == 0) { *fix_out = FIX_INFIX;   *assoc_out = ASSOC_RIGHT; return 1; }
    if (strcmp(type, "yfx") == 0) { *fix_out = FIX_INFIX;   *assoc_out = ASSOC_LEFT;  return 1; }
    if (strcmp(type, "fy")  == 0) { *fix_out = FIX_PREFIX;  *assoc_out = ASSOC_RIGHT; return 1; }
    if (strcmp(type, "fx")  == 0) { *fix_out = FIX_PREFIX;  *assoc_out = ASSOC_NONE;  return 1; }
    if (strcmp(type, "yf")  == 0) { *fix_out = FIX_POSTFIX; *assoc_out = ASSOC_LEFT;  return 1; }
    if (strcmp(type, "xf")  == 0) { *fix_out = FIX_POSTFIX; *assoc_out = ASSOC_NONE;  return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *op_type_unclassify(Assoc assoc, Fixity fix) {
    if (fix == FIX_INFIX)   return (assoc == ASSOC_NONE) ? "xfx" : (assoc == ASSOC_RIGHT) ? "xfy" : "yfx";
    if (fix == FIX_PREFIX)  return (assoc == ASSOC_RIGHT) ? "fy" : "fx";
    return (assoc == ASSOC_LEFT) ? "yf" : "xf";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bin_ops_count(void) { int n = 0; for (const OpEntry *op = BIN_OPS; op->name; op++) n++; return n; }
int prolog_op_table_count(void) { return bin_ops_count() + g_uinfix_n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_table_get(int idx, const char **name_out, int *prec_out, const char **type_out) {
    int nbin = bin_ops_count();
    const OpEntry *e;
    if (idx < 0 || idx >= nbin + g_uinfix_n) return 0;
    e = (idx < nbin) ? &BIN_OPS[idx] : &g_uinfix[idx - nbin];
    if (name_out) *name_out = e->name;
    if (prec_out) *prec_out = e->prec;
    if (type_out) *type_out = op_type_unclassify(e->assoc, e->fixity);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_table_add(const char *name, int prec, const char *type) {
    Assoc assoc; Fixity fix;
    if (!name || !type) return 0;
    if (!op_type_classify(type, &assoc, &fix)) return 0;
    user_op_add(name, prec, assoc, fix);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_user_count(void) { return g_uinfix_n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_user_get(int i, const char **name_out, int *prec_out, const char **type_out) {
    if (i < 0 || i >= g_uinfix_n) return 0;
    if (name_out) *name_out = g_uinfix[i].name;
    if (prec_out) *prec_out = g_uinfix[i].prec;
    if (type_out) *type_out = op_type_unclassify(g_uinfix[i].assoc, g_uinfix[i].fixity);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void register_op_one(int prec, const char *type, tree_t *namenode) {
    if (!namenode) return;
    if (namenode->t == TT_MAKELIST) { for (int i = 0; i < namenode->n; i++) register_op_one(prec, type, namenode->c[i]); return; }
    if (namenode->t != TT_QLIT || !namenode->v.sval) return;
    Assoc assoc; Fixity fix;
    if (op_type_classify(type, &assoc, &fix)) user_op_add(namenode->v.sval, prec, assoc, fix);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void register_op_directive(tree_t *goal) {
    if (!goal || goal->t != TT_FNC || !goal->v.sval) return;
    if (strcmp(goal->v.sval, ",") == 0 && goal->n == 2) { register_op_directive(goal->c[0]); register_op_directive(goal->c[1]); return; }
    if (strcmp(goal->v.sval, "op") != 0 || goal->n != 3) return;
    tree_t *pn = goal->c[0], *tn = goal->c[1], *nn = goal->c[2];
    if (!pn || pn->t != TT_ILIT || !tn || tn->t != TT_QLIT || !tn->v.sval) return;
    register_op_one((int)pn->v.ival, tn->v.sval, nn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int prefix_arg_starts(Token pk) {
    switch (pk.kind) {
        case TK_VAR: case TK_ANON: case TK_INT: case TK_FLOAT: case TK_STRING: case TK_LPAREN: case TK_LBRACKET: case TK_LBRACE: case TK_CUT: return 1;
        case TK_ATOM: case TK_OP: return (find_prefix(pk.text) != NULL) || (find_binop(pk.text) == NULL);
        default: return 0;
    }
}
static Term *parse_term(Parser *p, int max_prec);
static Term *parse_primary(Parser *p);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *parse_list(Parser *p) {
    Token tk = lexer_peek(&p->lx);
    if (tk.kind == TK_RBRACKET) {
        lexer_next(&p->lx);
        return term_new_atom(ATOM_NIL);
    }
    p->in_args++;
    Term *head = parse_term(p, 1200);
    p->in_args--;
    if (!head) return term_new_atom(ATOM_NIL);
    tk = lexer_peek(&p->lx);
    Term *tail = NULL;
    if (tk.kind == TK_COMMA) {
        lexer_next(&p->lx);
        tail = parse_list(p);
    } else if (tk.kind == TK_PIPE) {
        lexer_next(&p->lx);
        p->in_args++;
        tail = parse_term(p, 1200);
        p->in_args--;
        tk = lexer_peek(&p->lx);
        if (tk.kind == TK_RBRACKET) lexer_next(&p->lx);
        else perror_at(p, tk.line, "expected ] after list tail");
    } else if (tk.kind == TK_RBRACKET) {
        lexer_next(&p->lx);
        tail = term_new_atom(ATOM_NIL);
    } else {
        perror_at(p, tk.line, "expected , | ] in list");
        tail = term_new_atom(ATOM_NIL);
    }
    Term *args[2] = { head, tail };
    return term_new_compound(ATOM_DOT, 2, args);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int parse_args(Parser *p, Term ***args_out) {
    int cap = 8, n = 0;
    Term **args = malloc(cap * sizeof(Term *));
    Token pk0 = lexer_peek(&p->lx);
    if (pk0.kind == TK_RPAREN) {
        *args_out = args;
        return 0;
    }
    p->in_args++;
    for (;;) {
        if (n >= cap) { cap *= 2; args = realloc(args, cap * sizeof(Term *)); }
        Term *t = parse_term(p, 1200);
        if (!t) break;
        args[n++] = t;
        Token tk = lexer_peek(&p->lx);
        if (tk.kind != TK_COMMA) break;
        lexer_next(&p->lx);
    }
    p->in_args--;
    *args_out = args;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *parse_primary(Parser *p) {
    Token tk = lexer_next(&p->lx);
    switch (tk.kind) {
        case TK_VAR:
            return scope_get(&p->sc, tk.text);
        case TK_ANON:
            return term_new_var(-1);
        case TK_INT: {
            Term *t = term_new_int(tk.ival);
            return t;
        }
        case TK_FLOAT: {
            Term *t = term_new_float(tk.fval);
            return t;
        }
        case TK_STRING: {
            int id = prolog_atom_intern(tk.text);
            return term_new_atom(id);
        }
        case TK_ATOM: {
            Token pk = lexer_peek(&p->lx);
            if (pk.kind == TK_LPAREN) {
                lexer_next(&p->lx);
                Term **args = NULL;
                int nargs = parse_args(p, &args);
                Token rp = lexer_peek(&p->lx);
                if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                else perror_at(p, rp.line, "expected ) after args");
                int fid = prolog_atom_intern(tk.text);
                Term *t = term_new_compound(fid, nargs, args);
                free(args);
                return t;
            }
            if (strcmp(tk.text, "dynamic") == 0 ||
                    strcmp(tk.text, "discontiguous") == 0 ||
                    strcmp(tk.text, "multifile") == 0 ||
                    strcmp(tk.text, "module_transparent") == 0 ||
                    strcmp(tk.text, "meta_predicate") == 0 ||
                    strcmp(tk.text, "use_module") == 0 ||
                    strcmp(tk.text, "ensure_loaded") == 0 ||
                    strcmp(tk.text, "mode") == 0) {
                if (pk.kind == TK_ATOM || pk.kind == TK_VAR || pk.kind == TK_INT ||
                    pk.kind == TK_FLOAT || pk.kind == TK_LPAREN || pk.kind == TK_LBRACKET ||
                    pk.kind == TK_OP) {
                    int fid = prolog_atom_intern(tk.text);
                    Term *arg = parse_term(p, 1150);
                    Term *args1[1] = { arg };
                    return term_new_compound(fid, 1, args1);
                }
            }
            const OpEntry *pre_a = find_prefix(tk.text);
            if (pre_a && prefix_arg_starts(pk)) {
                int fid = prolog_atom_intern(tk.text);
                int argmax = (pre_a->assoc == ASSOC_RIGHT) ? pre_a->prec : pre_a->prec - 1;
                Term *arg = parse_term(p, argmax);
                Term *pargs[1] = { arg };
                return term_new_compound(fid, 1, pargs);
            }
            int id = prolog_atom_intern(tk.text);
            return term_new_atom(id);
        }
        case TK_CUT:
            return term_new_atom(ATOM_CUT);
        case TK_LPAREN: {
            int saved_in_args = p->in_args;
            p->in_args = 0;
            Term *t = parse_term(p, 1200);
            p->in_args = saved_in_args;
            Token rp = lexer_peek(&p->lx);
            if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
            else perror_at(p, rp.line, "expected )");
            return t;
        }
        case TK_LBRACKET:
            return parse_list(p);
        case TK_COMMA:
        case TK_SEMI: {
            const char *opname = (tk.kind == TK_COMMA) ? "," : ";";
            Token pk = lexer_peek(&p->lx);
            if (pk.kind == TK_LPAREN) {
                lexer_next(&p->lx);
                Term **args = NULL;
                int nargs = parse_args(p, &args);
                Token rp = lexer_peek(&p->lx);
                if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                else perror_at(p, rp.line, "expected ) after args");
                int fid = prolog_atom_intern(opname);
                Term *t = term_new_compound(fid, nargs, args);
                free(args);
                return t;
            }
            perror_at(p, tk.line, "unexpected token in term");
            return NULL;
        }
        case TK_OP: {
            if (strcmp(tk.text, "\\+") == 0 || strcmp(tk.text, "not") == 0) {
                Term *arg = parse_term(p, 900);
                int fid = prolog_atom_intern(tk.text);
                Term *args[1] = { arg };
                return term_new_compound(fid, 1, args);
            }
            if (strcmp(tk.text, "\\") == 0) {
                Term *arg = parse_term(p, 200);
                int fid = prolog_atom_intern("\\");
                Term *args[1] = { arg };
                return term_new_compound(fid, 1, args);
            }
            if (strcmp(tk.text, "-") == 0) {
                Token pk = lexer_peek(&p->lx);
                if (pk.kind == TK_INT || pk.kind == TK_FLOAT) {
                    Token num = lexer_next(&p->lx);
                    if (num.kind == TK_INT)   return term_new_int(-num.ival);
                    if (num.kind == TK_FLOAT) return term_new_float(-num.fval);
                }
                if (pk.kind == TK_ATOM || pk.kind == TK_OP || pk.kind == TK_VAR || pk.kind == TK_LPAREN) {
                    Term *arg = parse_term(p, 200);
                    int fid = prolog_atom_intern("-");
                    Term *args[1] = { arg };
                    return term_new_compound(fid, 1, args);
                }
            }
            if (strcmp(tk.text, "+") == 0) {
                Token pk = lexer_peek(&p->lx);
                if (pk.kind == TK_ATOM || pk.kind == TK_OP || pk.kind == TK_VAR ||
                    pk.kind == TK_LPAREN || pk.kind == TK_INT || pk.kind == TK_FLOAT) {
                    Term *arg = parse_term(p, 200);
                    int fid = prolog_atom_intern("+");
                    Term *args[1] = { arg };
                    return term_new_compound(fid, 1, args);
                }
            }
            {
                int id = prolog_atom_intern(tk.text);
                Token pk2 = lexer_peek(&p->lx);
                if (pk2.kind == TK_LPAREN) {
                    lexer_next(&p->lx);
                    Term **args = NULL;
                    int nargs = parse_args(p, &args);
                    Token rp = lexer_peek(&p->lx);
                    if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                    else perror_at(p, rp.line, "expected ) after args");
                    Term *t = term_new_compound(id, nargs, args);
                    free(args);
                    return t;
                }
                const OpEntry *pre_o = find_prefix(tk.text);
                if (pre_o && prefix_arg_starts(pk2)) {
                    int argmax = (pre_o->assoc == ASSOC_RIGHT) ? pre_o->prec : pre_o->prec - 1;
                    Term *arg = parse_term(p, argmax);
                    Term *pargs[1] = { arg };
                    return term_new_compound(id, 1, pargs);
                }
                return term_new_atom(id);
            }
        }
        case TK_NECK:
        case TK_QUERY: {
            perror_at(p, tk.line, "unexpected :- in term position");
            return NULL;
        }
        case TK_LBRACE: {
            Token pk2 = lexer_peek(&p->lx);
            if (pk2.kind == TK_RBRACE) {
                lexer_next(&p->lx);
                return term_new_atom(prolog_atom_intern("{}"));
            }
            Term *inner;
            { int saved_in_args = p->in_args; p->in_args = 0; inner = parse_term(p, 1200); p->in_args = saved_in_args; }
            Token rb = lexer_next(&p->lx);
            if (rb.kind != TK_RBRACE)
                perror_at(p, rb.line, "expected } after term");
            int curl_id = prolog_atom_intern("{}");
            Term *args[1] = { inner };
            return term_new_compound(curl_id, 1, args);
        }
        default:
            perror_at(p, tk.line, "unexpected token in term");
            return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *parse_term(Parser *p, int max_prec) {
    Term *lhs = parse_primary(p);
    if (!lhs) return NULL;
    for (;;) {
        Token pk = lexer_peek(&p->lx);
        const char *optext = NULL;
        if (pk.kind == TK_OP)   optext = pk.text;
        else if (pk.kind == TK_ATOM) optext = pk.text;
        else if (pk.kind == TK_COMMA && p->in_args > 0) break;
        else if (pk.kind == TK_COMMA && max_prec >= 1000) optext = ",";
        else if (pk.kind == TK_SEMI  && max_prec >= 1100) optext = ";";
        else if (pk.kind == TK_NECK  && max_prec >= 1200) optext = ":-";
        else break;
        const OpEntry *op = optext ? find_binop(optext) : NULL;
        if (!op || op->prec > max_prec) {
            const OpEntry *po = optext ? find_postfix(optext) : NULL;
            if (po && po->prec <= max_prec) { lexer_next(&p->lx); int pid = prolog_atom_intern(po->name); Term *pa[1] = { lhs }; lhs = term_new_compound(pid, 1, pa); continue; }
            break;
        }
        lexer_next(&p->lx);
        int rprec = (op->assoc == ASSOC_LEFT) ? op->prec - 1 : op->prec;
        Term *rhs = parse_term(p, rprec);
        if (!rhs) break;
        int fid = prolog_atom_intern(op->name);
        Term *args[2] = { lhs, rhs };
        lhs = term_new_compound(fid, 2, args);
    }
    return lhs;
}
static int dcg_var_counter = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dcg_count_conj(Term *t) {
    t = term_deref(t);
    if (!t) return 0;
    int comma_id = prolog_atom_intern(",");
    if (t->tag == TERM_COMPOUND && t->compound.functor == comma_id && t->compound.arity == 2)
        return dcg_count_conj(t->compound.args[0]) + dcg_count_conj(t->compound.args[1]);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dcg_flatten_conj(Term *t, Term **buf, int idx) {
    t = term_deref(t);
    if (!t) return idx;
    int comma_id = prolog_atom_intern(",");
    if (t->tag == TERM_COMPOUND && t->compound.functor == comma_id && t->compound.arity == 2) {
        idx = dcg_flatten_conj(t->compound.args[0], buf, idx);
        idx = dcg_flatten_conj(t->compound.args[1], buf, idx);
        return idx;
    }
    buf[idx++] = t;
    return idx;
}
#define TS_MAX_VARS 256
typedef struct { char *name; int idx; } TSEntry;
typedef struct { TSEntry e[TS_MAX_VARS]; int n; } TreeScope;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ts_reset(TreeScope *ts) { ts->n = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *ts_get(TreeScope *ts, const char *name) {
    for (int i = 0; i < ts->n; i++)
        if (strcmp(ts->e[i].name, name) == 0) {
            tree_t *v = ast_node_new(TT_VAR);
            v->v.sval = ts->e[i].name;
            return v;
        }
    if (ts->n >= TS_MAX_VARS) {
        tree_t *v = ast_node_new(TT_VAR);
        v->v.sval = strdup("_OVERFLOW");
        return v;
    }
    char *interned = strdup(name);
    ts->e[ts->n].name = interned;
    ts->e[ts->n].idx  = ts->n;
    ts->n++;
    tree_t *v = ast_node_new(TT_VAR);
    v->v.sval = interned;
    return v;
}
static tree_t *pt_term(Parser *p, TreeScope *ts, int max_prec);
static tree_t *pt_primary(Parser *p, TreeScope *ts);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pt_list(Parser *p, TreeScope *ts) {
    Token tk = lexer_peek(&p->lx);
    if (tk.kind == TK_RBRACKET) {
        lexer_next(&p->lx);
        tree_t *n = ast_node_new(TT_MAKELIST);
        return n;
    }
    tree_t *lst = ast_node_new(TT_MAKELIST);
    lst->v.ival = 0;
    p->in_args++;
    for (;;) {
        tree_t *elem = pt_term(p, ts, 1200);
        if (elem) ast_push(lst, elem);
        Token pk = lexer_peek(&p->lx);
        if (pk.kind == TK_COMMA) { lexer_next(&p->lx); continue; }
        if (pk.kind == TK_PIPE) {
            lexer_next(&p->lx);
            tree_t *tail = pt_term(p, ts, 1200);
            if (tail) ast_push(lst, tail);
            lst->v.ival = 1;
            pk = lexer_peek(&p->lx);
            if (pk.kind == TK_RBRACKET) lexer_next(&p->lx);
        } else if (pk.kind == TK_RBRACKET) {
            lexer_next(&p->lx);
        }
        break;
    }
    p->in_args--;
    return lst;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pt_args(Parser *p, TreeScope *ts, tree_t *parent) {
    Token pk0 = lexer_peek(&p->lx);
    if (pk0.kind == TK_RPAREN) return 0;
    int n = 0;
    p->in_args++;
    for (;;) {
        tree_t *a = pt_term(p, ts, 1200);
        if (!a) break;
        ast_push(parent, a);
        n++;
        Token tk = lexer_peek(&p->lx);
        if (tk.kind != TK_COMMA) break;
        lexer_next(&p->lx);
    }
    p->in_args--;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pt_binop(const char *op, tree_t *lhs, tree_t *rhs) {
    tree_t *n = ast_node_new(TT_FNC);
    n->v.sval = strdup(op);
    ast_push(n, lhs);
    ast_push(n, rhs);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pt_primary(Parser *p, TreeScope *ts) {
    Token tk = lexer_next(&p->lx);
    switch (tk.kind) {
        case TK_VAR:
            return ts_get(ts, tk.text);
        case TK_ANON: {
            tree_t *v = ast_node_new(TT_VAR);
            v->v.sval = strdup("_");
            return v;
        }
        case TK_INT: {
            tree_t *n = ast_node_new(TT_ILIT);
            n->v.ival = tk.ival;
            return n;
        }
        case TK_FLOAT: {
            tree_t *n = ast_node_new(TT_FLIT);
            n->v.dval = tk.fval;
            return n;
        }
        case TK_STRING: {
            tree_t *n = ast_node_new(TT_QLIT);
            n->v.sval = strdup(tk.text);
            return n;
        }
        case TK_ATOM: {
            Token pk = lexer_peek(&p->lx);
            if (pk.kind == TK_LPAREN) {
                lexer_next(&p->lx);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = strdup(tk.text);
                pt_args(p, ts, fnc);
                Token rp = lexer_peek(&p->lx);
                if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                return fnc;
            }
            if (strcmp(tk.text, "dynamic") == 0 ||
                strcmp(tk.text, "discontiguous") == 0 ||
                strcmp(tk.text, "multifile") == 0 ||
                strcmp(tk.text, "module_transparent") == 0 ||
                strcmp(tk.text, "meta_predicate") == 0 ||
                strcmp(tk.text, "use_module") == 0 ||
                strcmp(tk.text, "ensure_loaded") == 0 ||
                strcmp(tk.text, "mode") == 0) {
                if (pk.kind == TK_ATOM || pk.kind == TK_VAR || pk.kind == TK_INT ||
                    pk.kind == TK_FLOAT || pk.kind == TK_LPAREN || pk.kind == TK_LBRACKET ||
                    pk.kind == TK_OP) {
                    tree_t *fnc = ast_node_new(TT_FNC);
                    fnc->v.sval = strdup(tk.text);
                    tree_t *arg = pt_term(p, ts, 1150);
                    if (arg) ast_push(fnc, arg);
                    return fnc;
                }
            }
            if (strcmp(tk.text, "[]") == 0)
                return ast_node_new(TT_MAKELIST);
            const OpEntry *pre_a = find_prefix(tk.text);
            if (pre_a && prefix_arg_starts(pk)) {
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = strdup(tk.text);
                tree_t *arg = pt_term(p, ts, (pre_a->assoc == ASSOC_RIGHT) ? pre_a->prec : pre_a->prec - 1);
                if (arg) ast_push(fnc, arg);
                return fnc;
            }
            tree_t *n = ast_node_new(TT_QLIT);
            n->v.sval = strdup(tk.text);
            return n;
        }
        case TK_CUT: {
            return ast_node_new(TT_CUT);
        }
        case TK_LPAREN: {
            int saved = p->in_args;
            p->in_args = 0;
            tree_t *inner = pt_term(p, ts, 1200);
            p->in_args = saved;
            Token rp = lexer_peek(&p->lx);
            if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
            return inner;
        }
        case TK_LBRACKET:
            return pt_list(p, ts);
        case TK_COMMA:
        case TK_SEMI: {
            const char *opname = (tk.kind == TK_COMMA) ? "," : ";";
            Token pk2 = lexer_peek(&p->lx);
            if (pk2.kind == TK_LPAREN) {
                lexer_next(&p->lx);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = strdup(opname);
                pt_args(p, ts, fnc);
                Token rp = lexer_peek(&p->lx);
                if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                return fnc;
            }
            return NULL;
        }
        case TK_OP: {
            if (strcmp(tk.text, "\\+") == 0 || strcmp(tk.text, "not") == 0) {
                tree_t *arg = pt_term(p, ts, 900);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = strdup(tk.text);
                if (arg) ast_push(fnc, arg);
                return fnc;
            }
            if (strcmp(tk.text, "\\") == 0) {
                tree_t *arg = pt_term(p, ts, 200);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = strdup("\\");
                if (arg) ast_push(fnc, arg);
                return fnc;
            }
            if (strcmp(tk.text, "-") == 0) {
                Token pk3 = lexer_peek(&p->lx);
                if (pk3.kind == TK_INT) {
                    Token num = lexer_next(&p->lx);
                    tree_t *n = ast_node_new(TT_ILIT);
                    n->v.ival = -num.ival;
                    return n;
                }
                if (pk3.kind == TK_FLOAT) {
                    Token num = lexer_next(&p->lx);
                    tree_t *n = ast_node_new(TT_FLIT);
                    n->v.dval = -num.fval;
                    return n;
                }
                if (pk3.kind == TK_ATOM || pk3.kind == TK_OP || pk3.kind == TK_VAR || pk3.kind == TK_LPAREN) {
                    tree_t *arg = pt_term(p, ts, 200);
                    tree_t *fnc = ast_node_new(TT_FNC);
                    fnc->v.sval = strdup("-");
                    if (arg) ast_push(fnc, arg);
                    return fnc;
                }
            }
            if (strcmp(tk.text, "+") == 0) {
                Token pk3 = lexer_peek(&p->lx);
                if (pk3.kind == TK_ATOM || pk3.kind == TK_OP || pk3.kind == TK_VAR ||
                    pk3.kind == TK_LPAREN || pk3.kind == TK_INT || pk3.kind == TK_FLOAT) {
                    tree_t *arg = pt_term(p, ts, 200);
                    tree_t *fnc = ast_node_new(TT_FNC);
                    fnc->v.sval = strdup("+");
                    if (arg) ast_push(fnc, arg);
                    return fnc;
                }
            }
            {
                Token pk3 = lexer_peek(&p->lx);
                if (pk3.kind == TK_LPAREN) {
                    lexer_next(&p->lx);
                    tree_t *fnc = ast_node_new(TT_FNC);
                    fnc->v.sval = strdup(tk.text);
                    pt_args(p, ts, fnc);
                    Token rp = lexer_peek(&p->lx);
                    if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                    return fnc;
                }
                const OpEntry *pre_o = find_prefix(tk.text);
                if (pre_o && prefix_arg_starts(pk3)) {
                    tree_t *fnc = ast_node_new(TT_FNC);
                    fnc->v.sval = strdup(tk.text);
                    tree_t *arg = pt_term(p, ts, (pre_o->assoc == ASSOC_RIGHT) ? pre_o->prec : pre_o->prec - 1);
                    if (arg) ast_push(fnc, arg);
                    return fnc;
                }
                tree_t *n = ast_node_new(TT_QLIT);
                n->v.sval = strdup(tk.text);
                return n;
            }
        }
        case TK_LBRACE: {
            Token pk2 = lexer_peek(&p->lx);
            if (pk2.kind == TK_RBRACE) {
                lexer_next(&p->lx);
                tree_t *n = ast_node_new(TT_FNC);
                n->v.sval = strdup("{}");
                return n;
            }
            tree_t *inner;
            { int saved = p->in_args; p->in_args = 0; inner = pt_term(p, ts, 1200); p->in_args = saved; }
            Token rb = lexer_next(&p->lx);
            (void)rb;
            tree_t *fnc = ast_node_new(TT_FNC);
            fnc->v.sval = strdup("{}");
            if (inner) ast_push(fnc, inner);
            return fnc;
        }
        default:
            return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pt_term(Parser *p, TreeScope *ts, int max_prec) {
    tree_t *lhs = pt_primary(p, ts);
    if (!lhs) return NULL;
    for (;;) {
        Token pk = lexer_peek(&p->lx);
        const char *optext = NULL;
        if      (pk.kind == TK_OP)                              optext = pk.text;
        else if (pk.kind == TK_ATOM)                            optext = pk.text;
        else if (pk.kind == TK_COMMA && p->in_args > 0)         break;
        else if (pk.kind == TK_COMMA && max_prec >= 1000)       optext = ",";
        else if (pk.kind == TK_SEMI  && max_prec >= 1100)       optext = ";";
        else if (pk.kind == TK_NECK  && max_prec >= 1200)       optext = ":-";
        else break;
        const OpEntry *op = optext ? find_binop(optext) : NULL;
        if (!op || op->prec > max_prec) {
            const OpEntry *po = optext ? find_postfix(optext) : NULL;
            if (po && po->prec <= max_prec) { lexer_next(&p->lx); tree_t *pf = ast_node_new(TT_FNC); pf->v.sval = strdup(po->name); ast_push(pf, lhs); lhs = pf; continue; }
            break;
        }
        lexer_next(&p->lx);
        int rprec = (op->assoc == ASSOC_LEFT) ? op->prec - 1 : op->prec;
        tree_t *rhs = pt_term(p, ts, rprec);
        if (!rhs) break;
        tree_t *node = pt_binop(op->name, lhs, rhs);
        lhs = node;
    }
    return lhs;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *dcg_fresh_var(VarScope *sc) {
    char name[32];
    snprintf(name, sizeof(name), "_S%d", dcg_var_counter++);
    return scope_get(sc, name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *dcg_append_tail(Term *list, Term *tail) {
    list = term_deref(list);
    if (!list) return tail;
    if (list->tag == TERM_ATOM && list->atom_id == ATOM_NIL)
        return tail;
    if (list->tag == TERM_COMPOUND && list->compound.arity == 2) {
        Term *new_tail = dcg_append_tail(list->compound.args[1], tail);
        Term **args = malloc(2 * sizeof(Term *));
        args[0] = list->compound.args[0];
        args[1] = new_tail;
        return term_new_compound(list->compound.functor, 2, args);
    }
    return tail;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *dcg_make_unify(Term *a, Term *b) {
    int eq_id = prolog_atom_intern("=");
    Term **args = malloc(2 * sizeof(Term *));
    args[0] = a; args[1] = b;
    return term_new_compound(eq_id, 2, args);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static Term *dcg_call_nt(Term *nt, Term *s_in, Term *s_out) {
    nt = term_deref(nt);
    if (nt->tag == TERM_ATOM) {
        Term **args = malloc(2 * sizeof(Term *));
        args[0] = s_in; args[1] = s_out;
        return term_new_compound(nt->atom_id, 2, args);
    } else if (nt->tag == TERM_COMPOUND) {
        int new_arity = nt->compound.arity + 2;
        Term **args = malloc(new_arity * sizeof(Term *));
        for (int i = 0; i < nt->compound.arity; i++)
            args[i] = nt->compound.args[i];
        args[new_arity-2] = s_in;
        args[new_arity-1] = s_out;
        return term_new_compound(nt->compound.functor, new_arity, args);
    }
    return term_new_atom(prolog_atom_intern("true"));
}
static int dcg_expand_body(Term *body, Term *s_in, Term *s_out,
                           VarScope *sc, Term **buf, int idx);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dcg_expand_body(Term *body, Term *s_in, Term *s_out,
                           VarScope *sc, Term **buf, int idx) {
    body = term_deref(body);
    if (!body) {
        buf[idx++] = dcg_make_unify(s_in, s_out);
        return idx;
    }
    int comma_id = prolog_atom_intern(",");
    int semi_id  = prolog_atom_intern(";");
    int curl_id  = prolog_atom_intern("{}");
    if (body->tag == TERM_COMPOUND && body->compound.functor == curl_id
            && body->compound.arity == 1) {
        int n = dcg_count_conj(body->compound.args[0]);
        int old = idx;
        Term **tmp = malloc((n+1) * sizeof(Term *));
        int nn = dcg_flatten_conj(body->compound.args[0], tmp, 0);
        for (int i = 0; i < nn; i++) buf[idx++] = tmp[i];
        free(tmp);
        (void)old;
        buf[idx++] = dcg_make_unify(s_in, s_out);
        return idx;
    }
    if (body->tag == TERM_ATOM && body->atom_id == ATOM_NIL) {
        buf[idx++] = dcg_make_unify(s_in, s_out);
        return idx;
    }
    if (body->tag == TERM_COMPOUND && body->compound.functor == ATOM_DOT) {
        Term *list_with_tail = dcg_append_tail(body, s_out);
        buf[idx++] = dcg_make_unify(s_in, list_with_tail);
        return idx;
    }
    if (body->tag == TERM_COMPOUND && body->compound.functor == comma_id
            && body->compound.arity == 2) {
        Term *s_mid = dcg_fresh_var(sc);
        idx = dcg_expand_body(body->compound.args[0], s_in,  s_mid, sc, buf, idx);
        idx = dcg_expand_body(body->compound.args[1], s_mid, s_out, sc, buf, idx);
        return idx;
    }
    if (body->tag == TERM_COMPOUND && body->compound.functor == semi_id
            && body->compound.arity == 2) {
        Term *buf_a[256]; int na = 0;
        Term *buf_b[256]; int nb = 0;
        na = dcg_expand_body(body->compound.args[0], s_in, s_out, sc, buf_a, 0);
        nb = dcg_expand_body(body->compound.args[1], s_in, s_out, sc, buf_b, 0);
        Term *conj_a = buf_a[0];
        for (int i = 1; i < na; i++) {
            Term **ca = malloc(2 * sizeof(Term *));
            ca[0] = conj_a; ca[1] = buf_a[i];
            conj_a = term_new_compound(comma_id, 2, ca);
        }
        Term *conj_b = buf_b[0];
        for (int i = 1; i < nb; i++) {
            Term **cb = malloc(2 * sizeof(Term *));
            cb[0] = conj_b; cb[1] = buf_b[i];
            conj_b = term_new_compound(comma_id, 2, cb);
        }
        Term **sargs = malloc(2 * sizeof(Term *));
        sargs[0] = conj_a; sargs[1] = conj_b;
        buf[idx++] = term_new_compound(semi_id, 2, sargs);
        return idx;
    }
    if (body->tag == TERM_ATOM && body->atom_id == prolog_atom_intern("!")) {
        buf[idx++] = body;
        buf[idx++] = dcg_make_unify(s_in, s_out);
        return idx;
    }
    if (body->tag == TERM_ATOM && body->atom_id == prolog_atom_intern("true")) {
        buf[idx++] = dcg_make_unify(s_in, s_out);
        return idx;
    }
    buf[idx++] = dcg_call_nt(body, s_in, s_out);
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dcg_expand_clause(PlClause *cl, Term *dcg_body, Term *pushback, VarScope *sc) {
    dcg_var_counter = 0;
    Term *s0 = dcg_fresh_var(sc);
    Term *s  = dcg_fresh_var(sc);
    Term *head = term_deref(cl->head);
    if (head->tag == TERM_ATOM) {
        Term **args = malloc(2 * sizeof(Term *));
        args[0] = s0; args[1] = s;
        cl->head = term_new_compound(head->atom_id, 2, args);
    } else if (head->tag == TERM_COMPOUND) {
        int new_arity = head->compound.arity + 2;
        Term **args = malloc(new_arity * sizeof(Term *));
        for (int i = 0; i < head->compound.arity; i++)
            args[i] = head->compound.args[i];
        args[new_arity-2] = s0;
        args[new_arity-1] = s;
        cl->head = term_new_compound(head->compound.functor, new_arity, args);
    }
    Term *buf[1024];
    int n;
    if (pushback) {
        Term *s_mid = dcg_fresh_var(sc);
        n = dcg_expand_body(dcg_body, s0, s_mid, sc, buf, 0);
        Term *pushback_with_tail = dcg_append_tail(pushback, s);
        buf[n++] = dcg_make_unify(s_mid, pushback_with_tail);
    } else {
        n = dcg_expand_body(dcg_body, s0, s, sc, buf, 0);
    }
    cl->body  = malloc(n * sizeof(Term *));
    cl->nbody = n;
    for (int i = 0; i < n; i++) cl->body[i] = buf[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int eval_if_condition_tree(tree_t *cond) {
    if (!cond) return -1;
    if (cond->t == TT_QLIT) {
        const char *a = cond->v.sval ? cond->v.sval : "";
        if (strcmp(a, "true")  == 0) return 1;
        if (strcmp(a, "fail")  == 0 || strcmp(a, "false") == 0) return 0;
        return -1;
    }
    if (cond->t != TT_FNC) return -1;
    const char *fn = cond->v.sval ? cond->v.sval : "";
    int arity = cond->n;
    if ((strcmp(fn, "\\+") == 0 || strcmp(fn, "not") == 0) && arity == 1) {
        int v = eval_if_condition_tree(cond->c[0]);
        if (v < 0) return -1;
        return v ? 0 : 1;
    }
    if (strcmp(fn, "current_prolog_flag") == 0 && arity == 2) {
        tree_t *flag_t = cond->c[0];
        tree_t *val_t  = cond->c[1];
        if (!flag_t || !val_t) return -1;
        const char *flag = (flag_t->t == TT_QLIT) ? flag_t->v.sval : NULL;
        const char *val  = (val_t->t  == TT_QLIT) ? val_t->v.sval  : NULL;
        if (!flag || !val) return -1;
        if (strcmp(flag, "bounded") == 0) {
            if (strcmp(val, "true")  == 0) return 1;
            if (strcmp(val, "false") == 0) return 0;
            return -1;
        }
        if (strcmp(flag, "prefer_rationals") == 0) {
            if (strcmp(val, "true")  == 0) return 0;
            if (strcmp(val, "false") == 0) return 1;
            return -1;
        }
        return -1;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int try_handle_if_directive_tree(Parser *p, tree_t *goal, int lineno) {
    if (!goal) return 0;
    const char *fn = NULL;
    int arity = 0;
    tree_t *arg0 = NULL;
    if (goal->t == TT_QLIT) {
        fn = goal->v.sval;
        arity = 0;
    } else if (goal->t == TT_FNC) {
        fn = goal->v.sval;
        arity = goal->n;
        if (arity > 0) arg0 = goal->c[0];
    } else {
        return 0;
    }
    if (!fn) return 0;
    if (strcmp(fn, "if") == 0 && arity == 1) {
        if (p->ifst_top >= IF_STACK_MAX) {
            perror_at(p, lineno, ":- if/1 nesting too deep");
            return 1;
        }
        int parent_active = if_currently_active(p);
        int verdict = parent_active ? eval_if_condition_tree(arg0) : 0;
        int active = parent_active && (verdict != 0);
        IfFrame *f = &p->ifst[p->ifst_top++];
        f->active = active;
        f->taken  = active;
        f->parent_active = parent_active;
        f->line   = lineno;
        return 1;
    }
    if (strcmp(fn, "elif") == 0 && arity == 1) {
        if (p->ifst_top == 0) {
            perror_at(p, lineno, ":- elif without matching :- if");
            return 1;
        }
        IfFrame *f = &p->ifst[p->ifst_top - 1];
        if (!f->parent_active || f->taken) {
            f->active = 0;
        } else {
            int verdict = eval_if_condition_tree(arg0);
            int active  = (verdict != 0);
            f->active = active;
            if (active) f->taken = 1;
        }
        return 1;
    }
    if (strcmp(fn, "else") == 0 && arity == 0) {
        if (p->ifst_top == 0) {
            perror_at(p, lineno, ":- else without matching :- if");
            return 1;
        }
        IfFrame *f = &p->ifst[p->ifst_top - 1];
        if (!f->parent_active || f->taken) {
            f->active = 0;
        } else {
            f->active = 1;
            f->taken  = 1;
        }
        return 1;
    }
    if (strcmp(fn, "endif") == 0 && arity == 0) {
        if (p->ifst_top == 0) {
            perror_at(p, lineno, ":- endif without matching :- if");
            return 1;
        }
        p->ifst_top--;
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static PlClause *parse_clause(Parser *p) {
    scope_reset(&p->sc);
    Token pk = lexer_peek(&p->lx);
    if (pk.kind == TK_EOF) return NULL;
    Lexer saved_lx     = p->lx;
    int   saved_ifst   = p->ifst_top;
    TreeScope ts; ts_reset(&ts);
    PlClause *cl = calloc(1, sizeof(PlClause));
    cl->lineno = pk.line;
    if (pk.kind == TK_NECK) {
        lexer_next(&p->lx);
        tree_t *body_tr = pt_term(p, &ts, 1200);
        Token dot = lexer_next(&p->lx);
        if (dot.kind != TK_DOT)
            perror_at(p, dot.line, "expected . after directive");
        if (if_currently_active(p)) register_op_directive(body_tr);
        if (try_handle_if_directive_tree(p, body_tr, cl->lineno)) {
            cl->head = NULL; cl->body = NULL; cl->nbody = 0; cl->tr = NULL;
            return cl;
        }
        cl->head  = NULL;
        cl->body  = NULL;
        cl->nbody = 0;
        { tree_t *_cl = ast_node_new(TT_CLAUSE);
          ast_push(_cl, ast_node_new(TT_NUL));
          if (body_tr) ast_push(_cl, body_tr);
          cl->tr = _cl; }
        return cl;
    }
    tree_t *head_tr = pt_term(p, &ts, 1199);
    pk = lexer_peek(&p->lx);
    if (pk.kind == TK_NECK) {
        lexer_next(&p->lx);
        tree_t *body_tr = pt_term(p, &ts, 1200);
        { tree_t *_cl = ast_node_new(TT_CLAUSE);
          ast_push(_cl, head_tr);
          if (body_tr) ast_push(_cl, body_tr);
          cl->tr = _cl; }
        Token dot = lexer_next(&p->lx);
        if (dot.kind != TK_DOT)
            perror_at(p, dot.line, "expected . at end of clause");
    } else if (pk.kind == TK_OP && strcmp(pk.text, "-->") == 0) {
        p->lx       = saved_lx;
        p->ifst_top = saved_ifst;
        scope_reset(&p->sc);
        Term *head = parse_term(p, 1199);
        cl->head = head;
        pk = lexer_peek(&p->lx);
        lexer_next(&p->lx);
        Term *dcg_body = parse_term(p, 1200);
        Term *pushback = NULL;
        Term *hd = term_deref(cl->head);
        int comma_id = prolog_atom_intern(",");
        if (hd && hd->tag == TERM_COMPOUND &&
            hd->compound.functor == comma_id && hd->compound.arity == 2) {
            cl->head = hd->compound.args[0];
            pushback = hd->compound.args[1];
        }
        dcg_expand_clause(cl, dcg_body, pushback, &p->sc);
        cl->tr = NULL;
        Token dot = lexer_next(&p->lx);
        if (dot.kind != TK_DOT)
            perror_at(p, dot.line, "expected . at end of DCG clause");
        if (p->sc.count > 0) {
            cl->nvar      = p->sc.count;
            cl->var_names = malloc((size_t)p->sc.count * sizeof(char *));
            cl->var_terms = malloc((size_t)p->sc.count * sizeof(Term *));
            for (int _vi = 0; _vi < p->sc.count; _vi++) {
                cl->var_names[_vi] = p->sc.entries[_vi].name;
                cl->var_terms[_vi] = p->sc.entries[_vi].term;
            }
        }
    } else {
        { tree_t *_cl = ast_node_new(TT_CLAUSE);
          ast_push(_cl, head_tr);
          cl->tr = _cl; }
        Token dot = lexer_next(&p->lx);
        if (dot.kind != TK_DOT)
            perror_at(p, dot.line, "expected . at end of fact");
    }
    return cl;
}
static const char *PL_PRELUDE_SRC =
    "member(X,[X|_]).\n"
    "member(X,[_|T]):-member(X,T).\n"
    "append([],L,L).\n"
    "append([H|T],L,[H|R]):-append(T,L,R).\n"
    "reverse(L,R):-'$reverse_'(L,[],R).\n"
    "'$reverse_'([],A,A).\n"
    "'$reverse_'([H|T],A,R):-'$reverse_'(T,[H|A],R).\n"
    "length(L,N):-'$length_'(L,0,N).\n"
    "'$length_'([],N,N).\n"
    "'$length_'([_|T],A,N):-A1 is A+1,'$length_'(T,A1,N).\n"
    "numlist(L,H,R):-'$numlist_'(L,H,R).\n"
    "'$numlist_'(L,H,[]):-L>H,!.\n"
    "'$numlist_'(L,H,[L|T]):-L=<H,L1 is L+1,'$numlist_'(L1,H,T).\n"
    "last([X],X):- !.\n"
    "last([_|T],X):-last(T,X).\n"
    "nth0(N,L,E):-'$nth_'(L,0,N,E).\n"
    "nth1(N,L,E):-'$nth_'(L,1,N,E).\n"
    "'$nth_'([H|_],I,I,H).\n"
    "'$nth_'([_|T],I,N,E):-I1 is I+1,'$nth_'(T,I1,N,E).\n"
    "sum_list(L,S):-'$sum_list_'(L,0,S).\n"
    "sumlist(L,S):-'$sum_list_'(L,0,S).\n"
    "'$sum_list_'([],S,S).\n"
    "'$sum_list_'([H|T],A,S):-A1 is A+H,'$sum_list_'(T,A1,S).\n"
    "max_list([H|T],M):-'$max_list_'(T,H,M).\n"
    "'$max_list_'([],M,M).\n"
    "'$max_list_'([H|T],A,M):-(H>A->A1=H;A1=A),'$max_list_'(T,A1,M).\n"
    "min_list([H|T],M):-'$min_list_'(T,H,M).\n"
    "'$min_list_'([],M,M).\n"
    "'$min_list_'([H|T],A,M):-(H<A->A1=H;A1=A),'$min_list_'(T,A1,M).\n"
    "select(X,[X|T],T).\n"
    "select(X,[H|T],[H|R]):-select(X,T,R).\n"
    "subtract([],_,[]).\n"
    "subtract([H|T],L,R):-(member(H,L)->R=R1;R=[H|R1]),subtract(T,L,R1).\n"
    "intersection([],_,[]).\n"
    "intersection([H|T],L,R):-(member(H,L)->R=[H|R1];R=R1),intersection(T,L,R1).\n"
    "union([],L,L).\n"
    "union([H|T],L,R):-(member(H,L)->R=R1;R=[H|R1]),union(T,L,R1).\n"
    "exclude(_,[],[]).\n"
    "exclude(P,[H|T],R):-(call(P,H)->R=R1;R=[H|R1]),exclude(P,T,R1).\n"
    "include(_,[],[]).\n"
    "include(P,[H|T],R):-(call(P,H)->R=[H|R1];R=R1),include(P,T,R1).\n"
    "list_to_set(L,S):-'$lts_'(L,[],S).\n"
    "'$lts_'([],_,[]).\n"
    "'$lts_'([H|T],Seen,R):-(member(H,Seen)->R=R1;R=[H|R1]),'$lts_'(T,[H|Seen],R1).\n";
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_clause_key(PlClause *cl, const char **name_out, int *ar_out) {
    if (!cl) return 0;
    if (cl->head) {
        Term *h = term_deref(cl->head);
        if (h && h->tag == TERM_ATOM)     { *name_out = prolog_atom_name(h->atom_id);          *ar_out = 0;                 return 1; }
        if (h && h->tag == TERM_COMPOUND) { *name_out = prolog_atom_name(h->compound.functor); *ar_out = h->compound.arity; return 1; }
    }
    if (cl->tr && cl->tr->n > 0 && cl->tr->c[0] && cl->tr->c[0]->t != TT_NUL) {
        tree_t *hd = cl->tr->c[0];
        if (hd->t == TT_QLIT && hd->v.sval) { *name_out = hd->v.sval; *ar_out = 0;      return 1; }
        if (hd->t == TT_FNC  && hd->v.sval) { *name_out = hd->v.sval; *ar_out = hd->n;  return 1; }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_word_referenced(const char *src, const char *w) {
    size_t wl = strlen(w);
    for (const char *p = src; (p = strstr(p, w)) != NULL; p += wl) {
        char before = (p == src) ? ' ' : p[-1];
        char after  = p[wl];
        int bok = !(before == '_' || isalnum((unsigned char)before));
        int aok = !(after  == '_' || isalnum((unsigned char)after));
        if (bok && aok) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_tree_collect_calls(tree_t *t, char names[][96], int *nn, int cap) {
    if (!t) return;
    if ((t->t == TT_FNC || t->t == TT_QLIT) && t->v.sval) {
        int seen = 0;
        for (int i = 0; i < *nn; i++) if (!strcmp(names[i], t->v.sval)) { seen = 1; break; }
        if (!seen && *nn < cap) { snprintf(names[*nn], 96, "%s", t->v.sval); (*nn)++; }
    }
    for (int i = 0; i < t->n; i++) pl_tree_collect_calls(t->c[i], names, nn, cap);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void prolog_inject_prelude(PlProgram *prog, const char *user_src) {
    if (!prog || !user_src) return;
    char user_defined[512][96];
    int nud = 0;
    for (PlClause *cl = prog->head; cl; cl = cl->next) {
        const char *nm; int ar;
        if (pl_clause_key(cl, &nm, &ar) && nm && nud < 512) { snprintf(user_defined[nud], 96, "%s/%d", nm, ar); nud++; }
    }
    PlProgram *pre = prolog_parse(PL_PRELUDE_SRC, "<prelude>");
    if (!pre || !pre->head) { if (pre) free(pre); return; }
    char wanted[256][96];
    int nwant = 0;
    for (PlClause *cl = pre->head; cl; cl = cl->next) {
        const char *nm; int ar;
        if (!pl_clause_key(cl, &nm, &ar) || !nm) continue;
        if (nm[0] == '$') continue;
        if (!pl_word_referenced(user_src, nm)) continue;
        char key[96]; snprintf(key, 96, "%s/%d", nm, ar);
        int is_user = 0;
        for (int i = 0; i < nud; i++) if (!strcmp(user_defined[i], key)) { is_user = 1; break; }
        if (is_user) continue;
        int seen = 0;
        for (int i = 0; i < nwant; i++) if (!strcmp(wanted[i], key)) { seen = 1; break; }
        if (!seen && nwant < 256) { snprintf(wanted[nwant], 96, "%s/%d", nm, ar); nwant++; }
    }
    int changed = 1;
    while (changed) {
        changed = 0;
        for (PlClause *cl = pre->head; cl; cl = cl->next) {
            const char *nm; int ar;
            if (!pl_clause_key(cl, &nm, &ar) || !nm) continue;
            char key[96]; snprintf(key, 96, "%s/%d", nm, ar);
            int in_want = 0;
            for (int i = 0; i < nwant; i++) if (!strcmp(wanted[i], key)) { in_want = 1; break; }
            if (!in_want) continue;
            char calls[64][96]; int ncalls = 0;
            if (cl->tr && cl->tr->n > 1 && cl->tr->c[1]) pl_tree_collect_calls(cl->tr->c[1], calls, &ncalls, 64);
            for (int ci = 0; ci < ncalls; ci++) {
                for (PlClause *d = pre->head; d; d = d->next) {
                    const char *dn; int dar;
                    if (!pl_clause_key(d, &dn, &dar) || !dn) continue;
                    if (strcmp(dn, calls[ci])) continue;
                    char dk[96]; snprintf(dk, 96, "%s/%d", dn, dar);
                    int have = 0;
                    for (int i = 0; i < nwant; i++) if (!strcmp(wanted[i], dk)) { have = 1; break; }
                    int du = 0;
                    for (int i = 0; i < nud; i++) if (!strcmp(user_defined[i], dk)) { du = 1; break; }
                    if (!have && !du && nwant < 256) { snprintf(wanted[nwant], 96, "%s/%d", dn, dar); nwant++; changed = 1; }
                }
            }
        }
    }
    PlClause *nextc = NULL;
    for (PlClause *cl = pre->head; cl; cl = nextc) {
        nextc = cl->next;
        const char *nm; int ar;
        int keep = 0;
        if (pl_clause_key(cl, &nm, &ar) && nm) {
            char key[96]; snprintf(key, 96, "%s/%d", nm, ar);
            for (int i = 0; i < nwant; i++) if (!strcmp(wanted[i], key)) { keep = 1; break; }
        }
        if (keep) { cl->next = NULL; if (!prog->head) prog->head = cl; else prog->tail->next = cl; prog->tail = cl; prog->nclauses++; }
    }
    free(pre);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PlProgram *prolog_parse(const char *src, const char *filename) {
    prolog_atom_init();
    Parser p;
    lexer_init(&p.lx, src);
    p.filename = filename ? filename : "<input>";
    p.nerrors  = 0;
    p.ifst_top = 0;
    p.in_args  = 0;
    scope_reset(&p.sc);
    PlProgram *prog = calloc(1, sizeof(PlProgram));
    for (;;) {
        Token pk = lexer_peek(&p.lx);
        if (pk.kind == TK_EOF) break;
        if (pk.kind == TK_ERROR) {
            fprintf(stderr, "%s:%d: lex error: %s\n",
                    p.filename, pk.line, pk.text);
            p.nerrors++;
            lexer_next(&p.lx);
            continue;
        }
        PlClause *cl = parse_clause(&p);
        if (!cl) break;
        if (cl->head == NULL && cl->body == NULL && cl->nbody == 0 && cl->tr == NULL) {
            free(cl);
            continue;
        }
        if (!if_currently_active(&p)) {
            if (cl->body) free(cl->body);
            free(cl);
            continue;
        }
        if (!prog->head) prog->head = cl;
        else             prog->tail->next = cl;
        prog->tail = cl;
        prog->nclauses++;
    }
    if (p.ifst_top != 0) {
        fprintf(stderr, "%s: parse error: unmatched :- if (opened at line %d)\n",
                p.filename, p.ifst[p.ifst_top - 1].line);
        p.nerrors++;
    }
    prog->nerrors = p.nerrors;
    if (!filename || strcmp(filename, "<prelude>") != 0) prolog_inject_prelude(prog, src);
    return prog;
}
