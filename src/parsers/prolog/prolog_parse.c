#include "prolog_parse.h"
#include "prolog_lex.h"
#include "prolog_atom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
extern void *rt_plj_alloc(size_t);
#define IF_STACK_MAX 32
typedef struct {
    int active;
    int taken;
    int parent_active;
    int line;
} IfFrame;
typedef struct {
    Lexer      lx;
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
static tree_t *mk_atom(int atom_id) {
    if (atom_id == ATOM_CUT) return ast_node_new(TT_CUT);
    tree_t *e = ast_node_new(TT_FNC);
    const char *nm = prolog_atom_name(atom_id);
    e->v.sval = strdup(nm ? nm : "");
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *mk_call(int fid, tree_t **args, int arity) {
    if (fid == prolog_atom_intern("=") && arity == 2) {
        tree_t *e = ast_node_new(TT_UNIFY);
        ast_push(e, args[0]);
        ast_push(e, args[1]);
        return e;
    }
    if (arity == 2) {
        static const struct { const char *name; tree_e kind; } arith[] = {
            { "+", TT_ADD }, { "-", TT_SUB }, { "*", TT_MUL },
            { "/", TT_DIV }, { "//", TT_DIV }, { NULL, 0 }
        };
        const char *fn0 = prolog_atom_name(fid);
        for (int i = 0; fn0 && arith[i].name; i++) {
            if (strcmp(fn0, arith[i].name) == 0) {
                tree_t *e = ast_node_new(arith[i].kind);
                ast_push(e, args[0]);
                ast_push(e, args[1]);
                return e;
            }
        }
    }
    int comma_id = prolog_atom_intern(",");
    if (fid == comma_id && arity == 2) {
        tree_t *e = ast_node_new(TT_FNC); e->v.sval = strdup(",");
        ast_push(e, args[0]);
        tree_t *cur = args[1];
        while (cur && cur->t == TT_FNC && cur->v.sval && strcmp(cur->v.sval, ",") == 0 && cur->n == 2) {
            ast_push(e, cur->c[0]); cur = cur->c[1];
        }
        if (cur) ast_push(e, cur);
        return e;
    }
    int semi_id = prolog_atom_intern(";");
    if (fid == semi_id && arity == 2) {
        tree_t *e = ast_node_new(TT_FNC); e->v.sval = strdup(";");
        ast_push(e, args[0]);
        tree_t *cur = args[1];
        while (cur && cur->t == TT_FNC && cur->v.sval && strcmp(cur->v.sval, ";") == 0 && cur->n == 2) {
            ast_push(e, cur->c[0]); cur = cur->c[1];
        }
        if (cur) ast_push(e, cur);
        return e;
    }
    int arrow_id = prolog_atom_intern("->");
    if (fid == arrow_id && arity == 2) {
        tree_t *e = ast_node_new(TT_FNC); e->v.sval = strdup("->");
        ast_push(e, args[0]);
        tree_t *cur = args[1];
        while (cur && cur->t == TT_FNC && cur->v.sval && strcmp(cur->v.sval, ",") == 0 && cur->n == 2) {
            ast_push(e, cur->c[0]); cur = cur->c[1];
        }
        if (cur) ast_push(e, cur);
        return e;
    }
    tree_t *e = ast_node_new(TT_FNC);
    const char *fn = prolog_atom_name(fid);
    e->v.sval = strdup(fn ? fn : "");
    for (int i = 0; i < arity; i++) ast_push(e, args[i]);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *tls(tree_t *t) {
    if (!t) return mk_atom(ATOM_NIL);
    switch (t->t) {
        case TT_VAR: case TT_ILIT: case TT_FLIT: case TT_CUT:
            return t;
        case TT_QLIT:
            return mk_atom(prolog_atom_intern(t->v.sval ? t->v.sval : ""));
        case TT_MAKELIST: {
            int nelem = t->v.ival ? t->n - 1 : t->n;
            tree_t *result = t->v.ival ? tls(t->c[t->n - 1]) : mk_atom(ATOM_NIL);
            for (int i = nelem - 1; i >= 0; i--) {
                tree_t *dargs[2] = { tls(t->c[i]), result };
                result = mk_call(ATOM_DOT, dargs, 2);
            }
            return result;
        }
        default: {
            int fid = prolog_atom_intern(t->v.sval ? t->v.sval : "");
            int arity = t->n;
            tree_t **args = arity > 0 ? (tree_t **)rt_plj_alloc((size_t)arity * sizeof(tree_t *)) : NULL;
            for (int i = 0; i < arity; i++) args[i] = tls(t->c[i]);
            return mk_call(fid, args, arity);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *mk_raw(int fid, tree_t **args, int arity) {
    tree_t *e = ast_node_new(TT_FNC);
    const char *fn = prolog_atom_name(fid);
    e->v.sval = strdup(fn ? fn : "");
    for (int i = 0; i < arity; i++) ast_push(e, args[i]);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dcg_var_counter = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rls(tree_t *t) {
    if (!t) return mk_atom(ATOM_NIL);
    switch (t->t) {
        case TT_VAR:
            if (t->v.sval && strcmp(t->v.sval, "_") == 0) {
                tree_t *v = ast_node_new(TT_VAR);
                char buf[32];
                snprintf(buf, sizeof buf, "_S%d", dcg_var_counter++);
                v->v.sval = strdup(buf);
                return v;
            }
            return t;
        case TT_ILIT: case TT_FLIT: case TT_CUT:
            return t;
        case TT_QLIT:
            return mk_atom(prolog_atom_intern(t->v.sval ? t->v.sval : ""));
        case TT_MAKELIST: {
            int nelem = t->v.ival ? t->n - 1 : t->n;
            tree_t *result = t->v.ival ? rls(t->c[t->n - 1]) : mk_atom(ATOM_NIL);
            for (int i = nelem - 1; i >= 0; i--) {
                tree_t *dargs[2] = { rls(t->c[i]), result };
                result = mk_raw(ATOM_DOT, dargs, 2);
            }
            return result;
        }
        default: {
            int fid = prolog_atom_intern(t->v.sval ? t->v.sval : "");
            int arity = t->n;
            tree_t **args = arity > 0 ? (tree_t **)rt_plj_alloc((size_t)arity * sizeof(tree_t *)) : NULL;
            for (int i = 0; i < arity; i++) args[i] = rls(t->c[i]);
            return mk_raw(fid, args, arity);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dcg_count_conj(tree_t *t) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->v.sval && strcmp(t->v.sval, ",") == 0 && t->n == 2)
        return dcg_count_conj(t->c[0]) + dcg_count_conj(t->c[1]);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dcg_flatten_conj(tree_t *t, tree_t **buf, int idx) {
    if (!t) return idx;
    if (t->t == TT_FNC && t->v.sval && strcmp(t->v.sval, ",") == 0 && t->n == 2) {
        idx = dcg_flatten_conj(t->c[0], buf, idx);
        idx = dcg_flatten_conj(t->c[1], buf, idx);
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
static tree_t *dcg_fresh_var(TreeScope *ts) {
    char name[32];
    snprintf(name, sizeof(name), "_S%d", dcg_var_counter++);
    return ts_get(ts, name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *dcg_var_use(TreeScope *ts, tree_t *v) {
    if (v && v->t == TT_VAR && v->v.sval) return ts_get(ts, v->v.sval);
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *dcg_append_tail(TreeScope *ts, tree_t *list, tree_t *tail) {
    if (!list) return dcg_var_use(ts, tail);
    if (list->t == TT_FNC && list->v.sval && strcmp(list->v.sval, "[]") == 0 && list->n == 0)
        return dcg_var_use(ts, tail);
    if (list->t == TT_FNC && list->v.sval && strcmp(list->v.sval, ".") == 0 && list->n == 2) {
        tree_t *new_tail = dcg_append_tail(ts, list->c[1], tail);
        tree_t *args[2] = { list->c[0], new_tail };
        return mk_raw(ATOM_DOT, args, 2);
    }
    return dcg_var_use(ts, tail);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *dcg_make_unify(TreeScope *ts, tree_t *a, tree_t *b) {
    tree_t *args[2] = { dcg_var_use(ts, a), dcg_var_use(ts, b) };
    return mk_raw(prolog_atom_intern("="), args, 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *dcg_call_nt(TreeScope *ts, tree_t *nt, tree_t *s_in, tree_t *s_out) {
    if (nt && nt->t == TT_FNC) {
        int new_arity = nt->n + 2;
        tree_t **args = (tree_t **)rt_plj_alloc((size_t)new_arity * sizeof(tree_t *));
        for (int i = 0; i < nt->n; i++)
            args[i] = nt->c[i];
        args[new_arity-2] = dcg_var_use(ts, s_in);
        args[new_arity-1] = dcg_var_use(ts, s_out);
        return mk_raw(prolog_atom_intern(nt->v.sval ? nt->v.sval : ""), args, new_arity);
    }
    return mk_atom(prolog_atom_intern("true"));
}
static int dcg_expand_body(tree_t *body, tree_t *s_in, tree_t *s_out,
                           TreeScope *ts, tree_t **buf, int idx);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dcg_expand_body(tree_t *body, tree_t *s_in, tree_t *s_out,
                           TreeScope *ts, tree_t **buf, int idx) {
    if (!body) {
        buf[idx++] = dcg_make_unify(ts, s_in, s_out);
        return idx;
    }
    if (body->t == TT_FNC && body->v.sval && strcmp(body->v.sval, "{}") == 0
            && body->n == 1) {
        int n = dcg_count_conj(body->c[0]);
        tree_t **tmp = (tree_t **)rt_plj_alloc((size_t)(n+1) * sizeof(tree_t *));
        int nn = dcg_flatten_conj(body->c[0], tmp, 0);
        for (int i = 0; i < nn; i++) buf[idx++] = tmp[i];
        buf[idx++] = dcg_make_unify(ts, s_in, s_out);
        return idx;
    }
    if (body->t == TT_FNC && body->v.sval && strcmp(body->v.sval, "[]") == 0 && body->n == 0) {
        buf[idx++] = dcg_make_unify(ts, s_in, s_out);
        return idx;
    }
    if (body->t == TT_FNC && body->v.sval && strcmp(body->v.sval, ".") == 0 && body->n == 2) {
        tree_t *list_with_tail = dcg_append_tail(ts, body, s_out);
        buf[idx++] = dcg_make_unify(ts, s_in, list_with_tail);
        return idx;
    }
    if (body->t == TT_FNC && body->v.sval && strcmp(body->v.sval, ",") == 0
            && body->n == 2) {
        tree_t *s_mid = dcg_fresh_var(ts);
        idx = dcg_expand_body(body->c[0], s_in,  s_mid, ts, buf, idx);
        idx = dcg_expand_body(body->c[1], s_mid, s_out, ts, buf, idx);
        return idx;
    }
    if (body->t == TT_FNC && body->v.sval && strcmp(body->v.sval, ";") == 0
            && body->n == 2) {
        tree_t *buf_a[256]; int na = 0;
        tree_t *buf_b[256]; int nb = 0;
        na = dcg_expand_body(body->c[0], s_in, s_out, ts, buf_a, 0);
        nb = dcg_expand_body(body->c[1], s_in, s_out, ts, buf_b, 0);
        tree_t *conj_a = buf_a[0];
        for (int i = 1; i < na; i++) {
            tree_t *ca[2] = { conj_a, buf_a[i] };
            conj_a = mk_raw(prolog_atom_intern(","), ca, 2);
        }
        tree_t *conj_b = buf_b[0];
        for (int i = 1; i < nb; i++) {
            tree_t *cb[2] = { conj_b, buf_b[i] };
            conj_b = mk_raw(prolog_atom_intern(","), cb, 2);
        }
        tree_t *sargs[2] = { conj_a, conj_b };
        buf[idx++] = mk_raw(prolog_atom_intern(";"), sargs, 2);
        return idx;
    }
    if (body->t == TT_CUT) {
        buf[idx++] = body;
        buf[idx++] = dcg_make_unify(ts, s_in, s_out);
        return idx;
    }
    if (body->t == TT_FNC && body->v.sval && strcmp(body->v.sval, "true") == 0 && body->n == 0) {
        buf[idx++] = dcg_make_unify(ts, s_in, s_out);
        return idx;
    }
    buf[idx++] = dcg_call_nt(ts, body, s_in, s_out);
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dcg_expand_clause(PlClause *cl, tree_t *head_tr, tree_t *dcg_body, tree_t *pushback, TreeScope *ts) {
    cl->is_dcg = 1;
    tree_t *s0 = dcg_fresh_var(ts);
    tree_t *s  = dcg_fresh_var(ts);
    tree_t *new_head = ast_node_new(TT_FNC);
    new_head->v.sval = strdup(head_tr->v.sval ? head_tr->v.sval : "");
    for (int i = 0; i < head_tr->n; i++) ast_push(new_head, head_tr->c[i]);
    ast_push(new_head, dcg_var_use(ts, s0));
    ast_push(new_head, dcg_var_use(ts, s));
    tree_t *buf[1024];
    int n;
    if (pushback) {
        tree_t *s_mid = dcg_fresh_var(ts);
        n = dcg_expand_body(dcg_body, s0, s_mid, ts, buf, 0);
        tree_t *pushback_with_tail = dcg_append_tail(ts, pushback, s);
        buf[n++] = dcg_make_unify(ts, s_mid, pushback_with_tail);
    } else {
        n = dcg_expand_body(dcg_body, s0, s, ts, buf, 0);
    }
    tree_t *new_head_final = ast_node_new(TT_FNC);
    new_head_final->v.sval = strdup(new_head->v.sval ? new_head->v.sval : "");
    for (int i = 0; i < new_head->n; i++) ast_push(new_head_final, tls(new_head->c[i]));
    tree_t *body_prog = ast_node_new(TT_PROGRAM);
    for (int i = 0; i < n; i++) ast_push(body_prog, tls(buf[i]));
    tree_t *_cl = ast_node_new(TT_CLAUSE);
    ast_push(_cl, new_head_final);
    ast_push(_cl, body_prog);
    cl->tr = _cl;
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
    Token pk = lexer_peek(&p->lx);
    if (pk.kind == TK_EOF) return NULL;
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
        lexer_next(&p->lx);
        tree_t *dcg_body = rls(pt_term(p, &ts, 1200));
        tree_t *head_reshaped = rls(head_tr);
        tree_t *pushback = NULL;
        if (head_reshaped->t == TT_FNC && head_reshaped->v.sval &&
            strcmp(head_reshaped->v.sval, ",") == 0 && head_reshaped->n == 2) {
            pushback = head_reshaped->c[1];
            head_reshaped = head_reshaped->c[0];
        }
        dcg_expand_clause(cl, head_reshaped, dcg_body, pushback, &ts);
        Token dot = lexer_next(&p->lx);
        if (dot.kind != TK_DOT)
            perror_at(p, dot.line, "expected . at end of DCG clause");
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
    "'$lts_'([H|T],Seen,R):-(member(H,Seen)->R=R1;R=[H|R1]),'$lts_'(T,[H|Seen],R1).\n"
    /* maplist/2..5 + library(yall) '>>' — row prolog-missing-string-pairs-builtins. Prolog, not C: the
       adjacent include/3 and exclude/3 already ride call/N in exactly this shape, and call/N-on-a-user-
       predicate was the blocker seat05 split out (row prolog-call-n-user-predicate-segfault, now DONE). */
    "maplist(_,[]).\n"
    "maplist(G,[X|Xs]):-call(G,X),maplist(G,Xs).\n"
    "maplist(_,[],[]).\n"
    "maplist(G,[X|Xs],[Y|Ys]):-call(G,X,Y),maplist(G,Xs,Ys).\n"
    "maplist(_,[],[],[]).\n"
    "maplist(G,[X|Xs],[Y|Ys],[Z|Zs]):-call(G,X,Y,Z),maplist(G,Xs,Ys,Zs).\n"
    "maplist(_,[],[],[],[]).\n"
    "maplist(G,[W|Ws],[X|Xs],[Y|Ys],[Z|Zs]):-call(G,W,X,Y,Z),maplist(G,Ws,Xs,Ys,Zs).\n"
    /* ⛔ THE copy_term IS LOAD-BEARING, NOT TIDINESS: the lambda term is reused once per list element, so
       without a fresh copy the FIRST element binds the parameters and every later one fails to unify
       against them. MEASURED, not assumed — the same clauses with the copy_term removed answer
       one([6]) for a 1-element list and two_failed for a 2-element one, so a 1-element test PASSES a
       broken version and only a 2+-element one convicts it. Copying is also what SWI's yall does, which
       is why sharing an outer variable there needs the explicit '/'. */
    "'>>'(P,B,A1):-copy_term(P>>B,P1>>B1),P1=[A1],call(B1).\n"
    "'>>'(P,B,A1,A2):-copy_term(P>>B,P1>>B1),P1=[A1,A2],call(B1).\n"
    "'>>'(P,B,A1,A2,A3):-copy_term(P>>B,P1>>B1),P1=[A1,A2,A3],call(B1).\n"
    "'>>'(P,B,A1,A2,A3,A4):-copy_term(P>>B,P1>>B1),P1=[A1,A2,A3,A4],call(B1).\n";
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_clause_key(PlClause *cl, const char **name_out, int *ar_out) {
    if (!cl) return 0;
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
