#include "prolog_parse.h"
#include "ct_arena.h"
#include "ct_vec.h"
#include "prolog_lex.h"
#include "rt/prolog_atom.h"
#include "stage2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
extern void *rt_wsb_alloc(size_t);
extern int rt_pl_double_quotes_mode(void);
extern void rt_pl_double_quotes_set(const char *);
typedef struct {
    int active;
    int taken;
    int parent_active;
    int line;
} IfFrame;
typedef struct { char *name; int idx; } TSEntry;
typedef struct { cv_t e; } TreeScope;
typedef struct {
    Lexer      lx;
    const char *filename;
    int         nerrors;
    int         clause_errs;
    int         in_args;
    int         quiet;
    int         dq;
    int         prec;
    cv_t        ifst;
    TreeScope   ts;
    int         incl_depth;
    int         iso;
} Parser;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int if_currently_active(const Parser *p) {
    if (p->ifst.len == 0) return 1;
    return CV_AT(p->ifst, IfFrame, p->ifst.len - 1).active;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void resync_past_clause_end(Parser *p) {
    for (;;) {
        Token t = lexer_peek(&p->lx);
        if (t.kind == TK_EOF) return;
        lexer_next(&p->lx);
        if (t.kind == TK_DOT) return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void perror_at(Parser *p, int line, const char *msg) {
    if (p->clause_errs == 0) {
        if (!p->quiet) fprintf(stderr, "%s:%d: parse error: %s\n", p->filename, line, msg);
        p->nerrors++;
        p->clause_errs++;
        if (p->lx.last_kind != TK_DOT) resync_past_clause_end(p);
        p->lx.fenced = 1;
        return;
    }
    p->clause_errs++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    { "**",    200, ASSOC_NONE  },
    { "^",     200, ASSOC_RIGHT },
    { ":",     600, ASSOC_RIGHT },
    { "as",    700, ASSOC_NONE  },
    { NULL,    0,   ASSOC_NONE  }
};
static const OpEntry PREFIX_OPS[] = {
    { ":-",   1200, ASSOC_NONE,  FIX_PREFIX },
    { "?-",   1200, ASSOC_NONE,  FIX_PREFIX },
    { "\\+",   900, ASSOC_RIGHT, FIX_PREFIX },
    { "-",     200, ASSOC_RIGHT, FIX_PREFIX },
    { "+",     200, ASSOC_RIGHT, FIX_PREFIX },
    { "\\",    200, ASSOC_RIGHT, FIX_PREFIX },
    { NULL,    0,   ASSOC_NONE,  FIX_INFIX  }
};
static OpEntry *g_uinfix = NULL;
static int g_uinfix_n = 0, g_uinfix_cap = 0;
static const char *op_type_unclassify(Assoc assoc, Fixity fix);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void user_op_add(const char *name, int prec, Assoc assoc, Fixity fixity) {
    if (prolog_op_cols_ready()) prolog_op_col_set(prolog_atom_intern(name), prec, op_type_unclassify(assoc, fixity));
    for (int i = 0; i < g_uinfix_n; i++) if (g_uinfix[i].fixity == fixity && strcmp(g_uinfix[i].name, name) == 0) { g_uinfix[i].prec = prec; g_uinfix[i].assoc = assoc; return; }
    if (g_uinfix_n >= g_uinfix_cap) { g_uinfix_cap = g_uinfix_cap ? g_uinfix_cap * 2 : 8; g_uinfix = (OpEntry *)ct_grow(g_uinfix, g_uinfix_cap * sizeof(OpEntry)); }
    g_uinfix[g_uinfix_n].name = ct_strdup(name); g_uinfix[g_uinfix_n].prec = prec; g_uinfix[g_uinfix_n].assoc = assoc; g_uinfix[g_uinfix_n].fixity = fixity; g_uinfix_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const OpEntry *find_user(const char *name, Fixity fix) {
    for (int i = 0; i < g_uinfix_n; i++) if (g_uinfix[i].fixity == fix && strcmp(g_uinfix[i].name, name) == 0) return &g_uinfix[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const OpEntry *find_binop(const char *name) {
    const OpEntry *u = find_user(name, FIX_INFIX);
    if (u) return u->prec > 0 ? u : NULL;
    for (const OpEntry *op = BIN_OPS; op->name; op++)
        if (strcmp(op->name, name) == 0) return op;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const OpEntry *find_prefix(const char *name) { const OpEntry *u = find_user(name, FIX_PREFIX); return (u && u->prec > 0) ? u : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const OpEntry *find_postfix(const char *name) { const OpEntry *u = find_user(name, FIX_POSTFIX); return (u && u->prec > 0) ? u : NULL; }
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
static int prefix_ops_count(void) { int n = 0; for (const OpEntry *op = PREFIX_OPS; op->name; op++) n++; return n; }
int prolog_op_table_count(void) { return bin_ops_count() + prefix_ops_count() + g_uinfix_n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_table_get(int idx, const char **name_out, int *prec_out, const char **type_out) {
    int nbin = bin_ops_count(), npre = prefix_ops_count();
    const OpEntry *e;
    if (idx < 0 || idx >= nbin + npre + g_uinfix_n) return 0;
    e = (idx < nbin) ? &BIN_OPS[idx] : (idx < nbin + npre) ? &PREFIX_OPS[idx - nbin] : &g_uinfix[idx - nbin - npre];
    if (e->prec <= 0 || (idx < nbin + npre && find_user(e->name, e->fixity))) return 0;
    if (name_out) *name_out = e->name;
    if (prec_out) *prec_out = e->prec;
    if (type_out) *type_out = op_type_unclassify(e->assoc, e->fixity);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_permission(const char *name, int prec, const char *type) {
    Assoc assoc; Fixity fix;
    if (!name || !type || !op_type_classify(type, &assoc, &fix)) return 0;
    if (!strcmp(name, ",")) return 1;
    if (!strcmp(name, "[]") || !strcmp(name, "{}")) return 2;
    if (!strcmp(name, "|") && prec != 0 && (fix != FIX_INFIX || prec < 1001)) return 2;
    if (prec > 0 && fix == FIX_INFIX && find_postfix(name)) return 2;
    if (prec > 0 && fix == FIX_POSTFIX && find_binop(name)) return 2;
    return 0;
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
    if (namenode->t != TT_QLIT || !namenode->v.sval || prec < 0 || prec > 1200 || prolog_op_permission(namenode->v.sval, prec, type)) return;
    Assoc assoc; Fixity fix;
    if (op_type_classify(type, &assoc, &fix)) user_op_add(namenode->v.sval, prec, assoc, fix);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void register_op_directive(tree_t *goal) {
    if (!goal || goal->t != TT_FNC || !goal->v.sval) return;
    if (strcmp(goal->v.sval, ",") == 0 && goal->n == 2) { register_op_directive(goal->c[0]); register_op_directive(goal->c[1]); return; }
    if (strcmp(goal->v.sval, "module") == 0 && goal->n == 2) {
        tree_t *exports = goal->c[1];
        if (exports && exports->t == TT_MAKELIST)
            for (int i = 0; i < exports->n; i++) register_op_directive(exports->c[i]);
        return;
    }
    if (strcmp(goal->v.sval, "op") != 0 || goal->n != 3) return;
    tree_t *pn = goal->c[0], *tn = goal->c[1], *nn = goal->c[2];
    if (!pn || pn->t != TT_ILIT || !tn || tn->t != TT_QLIT || !tn->v.sval) return;
    register_op_one((int)pn->v.ival, tn->v.sval, nn);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int prefix_arg_starts(Token pk) {
    switch (pk.kind) {
        case TK_VAR: case TK_ANON: case TK_INT: case TK_FLOAT: case TK_STRING: case TK_BQSTRING: case TK_LPAREN: case TK_LBRACKET: case TK_LBRACE: case TK_CUT: return 1;
        case TK_ATOM: case TK_OP: return (find_prefix(pk.text) != NULL) || (find_binop(pk.text) == NULL);
        default: return 0;
    }
}
static tree_t *pt_stamp(tree_t *t, int ln) { if (t && t->line <= 0 && ln > 0) t->line = ln; return t; }
static tree_t *mk_atom(int atom_id) {
    if (atom_id == ATOM_CUT) return ast_node_new(TT_CUT);
    tree_t *e = ast_node_new(TT_FNC);
    const char *nm = prolog_atom_name(atom_id);
    e->v.sval = ct_strdup(nm ? nm : "");
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
    tree_t *e = ast_node_new(TT_FNC);
    const char *fn = prolog_atom_name(fid);
    e->v.sval = ct_strdup(fn ? fn : "");
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
            return pt_stamp(mk_atom(prolog_atom_intern(t->v.sval ? t->v.sval : "")), t->line);
        case TT_MAKELIST: {
            int nelem = t->v.ival ? t->n - 1 : t->n;
            tree_t *result = t->v.ival ? tls(t->c[t->n - 1]) : mk_atom(ATOM_NIL);
            for (int i = nelem - 1; i >= 0; i--) {
                tree_t *dargs[2] = { tls(t->c[i]), result };
                result = mk_call(ATOM_DOT, dargs, 2);
            }
            return pt_stamp(result, t->line);
        }
        default: {
            int fid = prolog_atom_intern(t->v.sval ? t->v.sval : "");
            int arity = t->n;
            tree_t **args = arity > 0 ? (tree_t **)rt_wsb_alloc((size_t)arity * sizeof(tree_t *)) : NULL;
            for (int i = 0; i < arity; i++) args[i] = tls(t->c[i]);
            return pt_stamp(mk_call(fid, args, arity), t->line);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *mk_raw(int fid, tree_t **args, int arity) {
    tree_t *e = ast_node_new(TT_FNC);
    const char *fn = prolog_atom_name(fid);
    e->v.sval = ct_strdup(fn ? fn : "");
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
                v->v.sval = ct_strdup(buf);
                return v;
            }
            return t;
        case TT_ILIT: case TT_FLIT: case TT_CUT:
            return t;
        case TT_QLIT:
            return pt_stamp(mk_atom(prolog_atom_intern(t->v.sval ? t->v.sval : "")), t->line);
        case TT_MAKELIST: {
            int nelem = t->v.ival ? t->n - 1 : t->n;
            tree_t *result = t->v.ival ? rls(t->c[t->n - 1]) : mk_atom(ATOM_NIL);
            for (int i = nelem - 1; i >= 0; i--) {
                tree_t *dargs[2] = { rls(t->c[i]), result };
                result = mk_raw(ATOM_DOT, dargs, 2);
            }
            return pt_stamp(result, t->line);
        }
        default: {
            int fid = prolog_atom_intern(t->v.sval ? t->v.sval : "");
            int arity = t->n;
            tree_t **args = arity > 0 ? (tree_t **)rt_wsb_alloc((size_t)arity * sizeof(tree_t *)) : NULL;
            for (int i = 0; i < arity; i++) args[i] = rls(t->c[i]);
            return pt_stamp(mk_raw(fid, args, arity), t->line);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *ts_get(TreeScope *ts, const char *name) {
    for (uint32_t i = 0; i < ts->e.len; i++)
        if (strcmp(CV_AT(ts->e, TSEntry, i).name, name) == 0) {
            tree_t *v = ast_node_new(TT_VAR);
            v->v.sval = CV_AT(ts->e, TSEntry, i).name;
            return v;
        }
    char *interned = ct_strdup(name);
    { TSEntry te; te.name = interned; te.idx = (int) ts->e.len; CV_PUSH(ts->e, TSEntry) = te; }
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
        tree_t *elem = pt_term(p, ts, p->iso ? 999 : 1200);
        if (elem) ast_push(lst, elem);
        Token pk = lexer_peek(&p->lx);
        if (pk.kind == TK_COMMA) { lexer_next(&p->lx); continue; }
        if (pk.kind == TK_PIPE) {
            lexer_next(&p->lx);
            tree_t *tail = pt_term(p, ts, p->iso ? 999 : 1200);
            if (tail) ast_push(lst, tail);
            lst->v.ival = 1;
            pk = lexer_peek(&p->lx);
            if (pk.kind == TK_RBRACKET) lexer_next(&p->lx);
            else perror_at(p, pk.line, "expected ] to close list");
        } else if (pk.kind == TK_RBRACKET) {
            lexer_next(&p->lx);
        } else {
            perror_at(p, pk.line, "expected , | or ] in list");
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
        tree_t *a = pt_term(p, ts, p->iso ? 999 : 1200);
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
static tree_t *pt_big_lit(const char *digits) {
    tree_t *n = ast_node_new(TT_FNC);
    tree_t *d = ast_node_new(TT_QLIT);
    n->v.sval = ct_strdup("$pl_big");
    d->v.sval = ct_strdup(digits);
    ast_push(n, d);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pt_binop(const char *op, tree_t *lhs, tree_t *rhs) {
    tree_t *n = ast_node_new(TT_FNC);
    n->v.sval = ct_strdup(op);
    ast_push(n, lhs);
    ast_push(n, rhs);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pt_primary(Parser *p, TreeScope *ts) {
    Token tk = lexer_next(&p->lx); int ln = tk.line; p->prec = 0;
    switch (tk.kind) {
        case TK_VAR:
            return pt_stamp(ts_get(ts, tk.text), ln);
        case TK_ANON: {
            tree_t *v = ast_node_new(TT_VAR);
            v->v.sval = ct_strdup("_");
            return pt_stamp(v, ln);
        }
        case TK_INT: {
            if (tk.big && tk.text) return pt_stamp(pt_big_lit(tk.text), ln);
            tree_t *n = ast_node_new(TT_ILIT);
            n->v.ival = tk.ival;
            return pt_stamp(n, ln);
        }
        case TK_FLOAT: {
            tree_t *n = ast_node_new(TT_FLIT);
            n->v.dval = tk.fval;
            return pt_stamp(n, ln);
        }
        case TK_BQSTRING: {
            tree_t *n = ast_node_new(TT_MAKELIST);
            n->v.ival = 0;
            { const unsigned char *q0 = (const unsigned char *)tk.text; size_t qn = tk.len >= 0 ? (size_t)tk.len : strlen(tk.text);
            for (const unsigned char *q = q0; q < q0 + qn; q++) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = (long long)*q; ast_push(n, e); } }
            return pt_stamp(n, ln);
        }
        case TK_STRING: {
            int dqm = p->dq;
            if (dqm == 0) {
                if (tk.pc) return pt_stamp(tk.pc, ln);
                tree_t *n = ast_node_new(TT_QLIT);
                n->v.sval = ct_strdup(tk.text);
                return pt_stamp(n, ln);
            }
            tree_t *n = ast_node_new(TT_MAKELIST);
            n->v.ival = 0;
            { const unsigned char *q0 = (const unsigned char *)tk.text; size_t qn = tk.len >= 0 ? (size_t)tk.len : strlen(tk.text);
            for (const unsigned char *q = q0; q < q0 + qn; q++) {
                tree_t *e;
                if (dqm == 2) { e = ast_node_new(TT_ILIT); e->v.ival = (long long)*q; }
                else { char one[2]; one[0] = (char)*q; one[1] = 0; e = ast_node_new(TT_QLIT); e->v.sval = ct_strdup(one); }
                ast_push(n, e);
            } }
            return pt_stamp(n, ln);
        }
        case TK_ATOM: {
            Token pk = lexer_peek(&p->lx);
            if (pk.kind == TK_LPAREN && pk.adj) {
                lexer_next(&p->lx);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = ct_strdup(tk.text);
                pt_args(p, ts, fnc);
                Token rp = lexer_peek(&p->lx);
                if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                else perror_at(p, rp.line, "expected ) to close argument list");
                return pt_stamp(fnc, ln);
            }
            if (strcmp(tk.text, "dynamic") == 0 ||
                strcmp(tk.text, "discontiguous") == 0 ||
                strcmp(tk.text, "multifile") == 0 ||
                strcmp(tk.text, "module_transparent") == 0 ||
                strcmp(tk.text, "meta_predicate") == 0 ||
                strcmp(tk.text, "use_module") == 0 ||
                strcmp(tk.text, "ensure_loaded") == 0 ||
                strcmp(tk.text, "table") == 0 ||
                strcmp(tk.text, "thread_local") == 0 ||
                strcmp(tk.text, "public") == 0 ||
                strcmp(tk.text, "record") == 0 ||
                strcmp(tk.text, "mode") == 0) {
                if (pk.kind == TK_ATOM || pk.kind == TK_VAR || pk.kind == TK_INT ||
                    pk.kind == TK_FLOAT || pk.kind == TK_LPAREN || pk.kind == TK_LBRACKET ||
                    pk.kind == TK_OP) {
                    tree_t *fnc = ast_node_new(TT_FNC);
                    fnc->v.sval = ct_strdup(tk.text);
                    tree_t *arg = pt_term(p, ts, 1150);
                    if (arg) ast_push(fnc, arg);
                    p->prec = 1150;
                    return pt_stamp(fnc, ln);
                }
            }
            if (strcmp(tk.text, "[]") == 0)
                return pt_stamp(ast_node_new(TT_MAKELIST), ln);
            const OpEntry *pre_a = find_prefix(tk.text);
            if (pre_a && prefix_arg_starts(pk)) {
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = ct_strdup(tk.text);
                tree_t *arg = pt_term(p, ts, (pre_a->assoc == ASSOC_RIGHT) ? pre_a->prec : pre_a->prec - 1);
                if (arg) ast_push(fnc, arg);
                p->prec = pre_a->prec;
                return pt_stamp(fnc, ln);
            }
            if (tk.pc) return pt_stamp(tk.pc, ln);
            tree_t *n = ast_node_new(TT_QLIT);
            n->v.sval = ct_strdup(tk.text);
            return pt_stamp(n, ln);
        }
        case TK_CUT: {
            return pt_stamp(ast_node_new(TT_CUT), ln);
        }
        case TK_NECK: {
            Token pkn = lexer_peek(&p->lx);
            if (prefix_arg_starts(pkn)) {
                tree_t *arg = pt_term(p, ts, 1199);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = ct_strdup(":-");
                if (arg) ast_push(fnc, arg);
                p->prec = 1200;
                return pt_stamp(fnc, ln);
            }
            tree_t *n = ast_node_new(TT_QLIT);
            n->v.sval = ct_strdup(":-");
            return pt_stamp(n, ln);
        }
        case TK_QUERY: {
            Token pkq = lexer_peek(&p->lx);
            if (prefix_arg_starts(pkq)) {
                tree_t *arg = pt_term(p, ts, 1199);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = ct_strdup("?-");
                if (arg) ast_push(fnc, arg);
                p->prec = 1200;
                return pt_stamp(fnc, ln);
            }
            tree_t *n = ast_node_new(TT_QLIT);
            n->v.sval = ct_strdup("?-");
            return pt_stamp(n, ln);
        }
        case TK_LPAREN: {
            int saved = p->in_args;
            p->in_args = 0;
            tree_t *inner = pt_term(p, ts, 1200);
            p->in_args = saved;
            Token rp = lexer_peek(&p->lx);
            if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
            else perror_at(p, rp.line, "expected )");
            return pt_stamp(inner, ln);
        }
        case TK_LBRACKET:
            return pt_stamp(pt_list(p, ts), ln);
        case TK_COMMA:
        case TK_SEMI: {
            const char *opname = (tk.kind == TK_COMMA) ? "," : ";";
            Token pk2 = lexer_peek(&p->lx);
            if (pk2.kind == TK_LPAREN) {
                lexer_next(&p->lx);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = ct_strdup(opname);
                pt_args(p, ts, fnc);
                Token rp = lexer_peek(&p->lx);
                if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                else perror_at(p, rp.line, "expected ) to close argument list");
                return pt_stamp(fnc, ln);
            }
            if (tk.kind == TK_SEMI) { tree_t *n = ast_node_new(TT_QLIT); n->v.sval = ct_strdup(opname); return pt_stamp(n, ln); }
            perror_at(p, tk.line, "unexpected , (a bare comma is not a term)");
            return pt_stamp(NULL, ln);
        }
        case TK_OP: {
            { Token pk0 = lexer_peek(&p->lx);
              if (pk0.kind == TK_LPAREN && pk0.adj) {
                  lexer_next(&p->lx);
                  tree_t *fnc = ast_node_new(TT_FNC);
                  fnc->v.sval = ct_strdup(tk.text);
                  pt_args(p, ts, fnc);
                  Token rp = lexer_peek(&p->lx);
                  if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                  else perror_at(p, rp.line, "expected ) to close argument list");
                  return pt_stamp(fnc, ln); } }
            if ((strcmp(tk.text, "\\+") == 0 || strcmp(tk.text, "not") == 0) && prefix_arg_starts(lexer_peek(&p->lx))) {
                tree_t *arg = pt_term(p, ts, 900);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = ct_strdup(tk.text);
                if (arg) ast_push(fnc, arg);
                p->prec = 900;
                return pt_stamp(fnc, ln);
            }
            if (strcmp(tk.text, "\\") == 0 && prefix_arg_starts(lexer_peek(&p->lx))) {
                tree_t *arg = pt_term(p, ts, 200);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = ct_strdup("\\");
                if (arg) ast_push(fnc, arg);
                p->prec = 200;
                return pt_stamp(fnc, ln);
            }
            if (strcmp(tk.text, "-") == 0) {
                Token pk3 = lexer_peek(&p->lx);
                if (pk3.kind == TK_INT) {
                    Token num = lexer_next(&p->lx);
                    if (num.big && num.text) {
                        size_t dn = strlen(num.text);
                        char *nb = (char *) ct_alloc(dn + 2);
                        tree_t *n;
                        if (!nb) return pt_stamp(ast_node_new(TT_ILIT), ln);
                        nb[0] = '-';
                        memcpy(nb + 1, num.text, dn + 1);
                        n = pt_big_lit(nb);
                        ct_drop(nb);
                        return pt_stamp(n, ln);
                    }
                    tree_t *n = ast_node_new(TT_ILIT);
                    n->v.ival = -num.ival;
                    return pt_stamp(n, ln);
                }
                if (pk3.kind == TK_FLOAT) {
                    Token num = lexer_next(&p->lx);
                    tree_t *n = ast_node_new(TT_FLIT);
                    n->v.dval = -num.fval;
                    return pt_stamp(n, ln);
                }
                if (pk3.kind == TK_ATOM || pk3.kind == TK_OP || pk3.kind == TK_VAR || pk3.kind == TK_LPAREN) {
                    tree_t *arg = pt_term(p, ts, 200);
                    tree_t *fnc = ast_node_new(TT_FNC);
                    fnc->v.sval = ct_strdup("-");
                    if (arg) ast_push(fnc, arg);
                    p->prec = 200;
                    return pt_stamp(fnc, ln);
                }
            }
            if (strcmp(tk.text, "+") == 0) {
                Token pk3 = lexer_peek(&p->lx);
                if (pk3.kind == TK_ATOM || pk3.kind == TK_OP || pk3.kind == TK_VAR ||
                    pk3.kind == TK_LPAREN || pk3.kind == TK_INT || pk3.kind == TK_FLOAT) {
                    tree_t *arg = pt_term(p, ts, 200);
                    tree_t *fnc = ast_node_new(TT_FNC);
                    fnc->v.sval = ct_strdup("+");
                    if (arg) ast_push(fnc, arg);
                    p->prec = 200;
                    return pt_stamp(fnc, ln);
                }
            }
            {
                Token pk3 = lexer_peek(&p->lx);
                if (pk3.kind == TK_LPAREN && pk3.adj) {
                    lexer_next(&p->lx);
                    tree_t *fnc = ast_node_new(TT_FNC);
                    fnc->v.sval = ct_strdup(tk.text);
                    pt_args(p, ts, fnc);
                    Token rp = lexer_peek(&p->lx);
                    if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                    else perror_at(p, rp.line, "expected ) to close argument list");
                    return pt_stamp(fnc, ln);
                }
                const OpEntry *pre_o = find_prefix(tk.text);
                if (pre_o && prefix_arg_starts(pk3)) {
                    tree_t *fnc = ast_node_new(TT_FNC);
                    fnc->v.sval = ct_strdup(tk.text);
                    tree_t *arg = pt_term(p, ts, (pre_o->assoc == ASSOC_RIGHT) ? pre_o->prec : pre_o->prec - 1);
                    if (arg) ast_push(fnc, arg);
                    p->prec = pre_o->prec;
                    return pt_stamp(fnc, ln);
                }
                tree_t *n = ast_node_new(TT_QLIT);
                n->v.sval = ct_strdup(tk.text);
                return pt_stamp(n, ln);
            }
        }
        case TK_LBRACE: {
            Token pk2 = lexer_peek(&p->lx);
            if (pk2.kind == TK_RBRACE) {
                lexer_next(&p->lx);
                tree_t *n = ast_node_new(TT_FNC);
                n->v.sval = ct_strdup("{}");
                if (lexer_peek(&p->lx).kind == TK_LPAREN) {
                    lexer_next(&p->lx);
                    pt_args(p, ts, n);
                    Token rp = lexer_peek(&p->lx);
                    if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                    else perror_at(p, rp.line, "expected ) to close argument list");
                }
                return pt_stamp(n, ln);
            }
            tree_t *inner;
            { int saved = p->in_args; p->in_args = 0; inner = pt_term(p, ts, 1200); p->in_args = saved; }
            Token rb = lexer_peek(&p->lx);
            if (rb.kind == TK_RBRACE) lexer_next(&p->lx);
            else perror_at(p, rb.line, "expected } to close term");
            tree_t *fnc = ast_node_new(TT_FNC);
            fnc->v.sval = ct_strdup("{}");
            if (inner) ast_push(fnc, inner);
            return pt_stamp(fnc, ln);
        }
        default:
            perror_at(p, tk.line, "unexpected token, expected a term");
            return pt_stamp(NULL, ln);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pt_term(Parser *p, TreeScope *ts, int max_prec) {
    int ln = lexer_peek(&p->lx).line;
    tree_t *lhs = pt_primary(p, ts);
    if (!lhs) return NULL;
    int lp = p->prec;
    if (lp > max_prec) perror_at(p, ln, "operator priority clash");
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
            if (po && po->prec <= max_prec && lp <= (po->assoc == ASSOC_LEFT ? po->prec : po->prec - 1)) {
                lexer_next(&p->lx); tree_t *pf = ast_node_new(TT_FNC); pf->v.sval = ct_strdup(po->name); ast_push(pf, lhs); lhs = pt_stamp(pf, ln); lp = po->prec; continue; }
            break;
        }
        if (lp > (op->assoc == ASSOC_LEFT ? op->prec : op->prec - 1)) break;
        lexer_next(&p->lx);
        int rprec = (op->assoc == ASSOC_RIGHT) ? op->prec : op->prec - 1;
        tree_t *rhs = pt_term(p, ts, rprec);
        if (!rhs) break;
        tree_t *node = pt_stamp(pt_binop(op->name, lhs, rhs), ln);
        lhs = node; lp = op->prec;
    }
    p->prec = 0;
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
        tree_t **args = (tree_t **)rt_wsb_alloc((size_t)new_arity * sizeof(tree_t *));
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
        tree_t **tmp = (tree_t **)rt_wsb_alloc((size_t)(n+1) * sizeof(tree_t *));
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
    if (body->t == TT_FNC && body->v.sval && (strcmp(body->v.sval, "->") == 0 || strcmp(body->v.sval, "*->") == 0) && body->n == 2) {
        tree_t *buf_c[256]; int nc = 0; tree_t *buf_t[256]; int nt = 0; tree_t *s_mid = dcg_fresh_var(ts);
        nc = dcg_expand_body(body->c[0], s_in, s_mid, ts, buf_c, 0);
        nt = dcg_expand_body(body->c[1], s_mid, s_out, ts, buf_t, 0);
        tree_t *conj_c = buf_c[nc - 1];
        for (int i = nc - 2; i >= 0; i--) { tree_t *cc[2] = { buf_c[i], conj_c }; conj_c = mk_raw(prolog_atom_intern(","), cc, 2); }
        tree_t *conj_t = buf_t[nt - 1];
        for (int i = nt - 2; i >= 0; i--) { tree_t *ct[2] = { buf_t[i], conj_t }; conj_t = mk_raw(prolog_atom_intern(","), ct, 2); }
        tree_t *iargs[2] = { conj_c, conj_t };
        buf[idx++] = mk_raw(prolog_atom_intern(body->v.sval), iargs, 2);
        return idx;
    }
    if (body->t == TT_FNC && body->v.sval && strcmp(body->v.sval, "\\+") == 0 && body->n == 1) {
        tree_t *buf_g[256]; int ng = 0; tree_t *s_void = dcg_fresh_var(ts);
        ng = dcg_expand_body(body->c[0], s_in, s_void, ts, buf_g, 0);
        tree_t *conj_g = buf_g[ng - 1];
        for (int i = ng - 2; i >= 0; i--) { tree_t *cg[2] = { buf_g[i], conj_g }; conj_g = mk_raw(prolog_atom_intern(","), cg, 2); }
        tree_t *nargs[1] = { conj_g };
        buf[idx++] = mk_raw(prolog_atom_intern("\\+"), nargs, 1);
        buf[idx++] = dcg_make_unify(ts, s_in, s_out);
        return idx;
    }
    if (body->t == TT_VAR) {
        tree_t *cargs[3] = { dcg_var_use(ts, body), dcg_var_use(ts, s_in), dcg_var_use(ts, s_out) };
        buf[idx++] = mk_raw(prolog_atom_intern("phrase"), cargs, 3);
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
    new_head->v.sval = ct_strdup(head_tr->v.sval ? head_tr->v.sval : "");
    for (int i = 0; i < head_tr->n; i++) ast_push(new_head, head_tr->c[i]);
    ast_push(new_head, dcg_var_use(ts, s0));
    ast_push(new_head, dcg_var_use(ts, s));
    tree_t *buf[1024];
    int n;
    if (pushback) {
        tree_t *s_mid = dcg_fresh_var(ts);
        n = dcg_expand_body(dcg_body, s0, s_mid, ts, buf, 0);
        tree_t *pushback_with_tail = dcg_append_tail(ts, pushback, s_mid);
        buf[n++] = dcg_make_unify(ts, s, pushback_with_tail);
    } else {
        n = dcg_expand_body(dcg_body, s0, s, ts, buf, 0);
    }
    tree_t *new_head_final = ast_node_new(TT_FNC);
    new_head_final->v.sval = ct_strdup(new_head->v.sval ? new_head->v.sval : "");
    for (int i = 0; i < new_head->n; i++) ast_push(new_head_final, tls(new_head->c[i]));
    tree_t *body_prog = ast_node_new(TT_PROGRAM);
    for (int i = 0; i < n; i++) ast_push(body_prog, tls(buf[i]));
    tree_t *_cl = ast_node_new(TT_CLAUSE);
    ast_push(_cl, new_head_final);
    ast_push(_cl, body_prog);
    cl->tr = _cl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void prolog_dcg_expand(PlClause *cl) {
    if (!cl || !cl->tr || cl->tr->t != TT_CLAUSE || cl->tr->n != 1) return;
    tree_t *arrow = cl->tr->c[0];
    if (!arrow || arrow->t != TT_FNC || !arrow->v.sval || strcmp(arrow->v.sval, "-->") != 0 || arrow->n != 2) return;
    TreeScope ts; memset(&ts, 0, sizeof ts);
    tree_t *dcg_body = rls(arrow->c[1]);
    tree_t *head_reshaped = rls(arrow->c[0]);
    tree_t *pushback = NULL;
    if (head_reshaped->t == TT_FNC && head_reshaped->v.sval && strcmp(head_reshaped->v.sval, ",") == 0 && head_reshaped->n == 2) { pushback = head_reshaped->c[1]; head_reshaped = head_reshaped->c[0]; }
    dcg_expand_clause(cl, head_reshaped, dcg_body, pushback, &ts);
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
            if (strcmp(val, "true")  == 0) return 0;
            if (strcmp(val, "false") == 0) return 1;
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
        int parent_active = if_currently_active(p);
        int verdict = parent_active ? eval_if_condition_tree(arg0) : 0;
        int active = parent_active && (verdict != 0);
        IfFrame nf; nf.active = 0; nf.taken = 0; nf.parent_active = 0; nf.line = 0; CV_PUSH(p->ifst, IfFrame) = nf;
        IfFrame *f = &CV_AT(p->ifst, IfFrame, p->ifst.len - 1);
        f->active = active;
        f->taken  = active;
        f->parent_active = parent_active;
        f->line   = lineno;
        return 1;
    }
    if (strcmp(fn, "elif") == 0 && arity == 1) {
        if (p->ifst.len == 0) {
            perror_at(p, lineno, ":- elif without matching :- if");
            return 1;
        }
        IfFrame *f = &CV_AT(p->ifst, IfFrame, p->ifst.len - 1);
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
        if (p->ifst.len == 0) {
            perror_at(p, lineno, ":- else without matching :- if");
            return 1;
        }
        IfFrame *f = &CV_AT(p->ifst, IfFrame, p->ifst.len - 1);
        if (!f->parent_active || f->taken) {
            f->active = 0;
        } else {
            f->active = 1;
            f->taken  = 1;
        }
        return 1;
    }
    if (strcmp(fn, "endif") == 0 && arity == 0) {
        if (p->ifst.len == 0) {
            perror_at(p, lineno, ":- endif without matching :- if");
            return 1;
        }
        p->ifst.len--;
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dq_directive(Parser *p, const tree_t *goal) {
    if (!goal || goal->t != TT_FNC || !goal->v.sval || strcmp(goal->v.sval, "set_prolog_flag") || goal->n != 2) return;
    const tree_t *f = goal->c[0], *v = goal->c[1];
    if (!f || !v || f->t != TT_QLIT || v->t != TT_QLIT || !f->v.sval || !v->v.sval || strcmp(f->v.sval, "double_quotes")) return;
    if (!strcmp(v->v.sval, "atom") || !strcmp(v->v.sval, "string")) p->dq = 0; else if (!strcmp(v->v.sval, "chars")) p->dq = 1; else if (!strcmp(v->v.sval, "codes")) p->dq = 2; else return;
    rt_pl_double_quotes_set(v->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void iso_directive(Parser *p, const tree_t *goal) {
    if (!goal || goal->t != TT_FNC || !goal->v.sval || strcmp(goal->v.sval, "set_prolog_flag") || goal->n != 2) return;
    const tree_t *f = goal->c[0], *v = goal->c[1];
    if (!f || !v || f->t != TT_QLIT || v->t != TT_QLIT || !f->v.sval || !v->v.sval || strcmp(f->v.sval, "iso")) return;
    if (!strcmp(v->v.sval, "true")) p->iso = 1; else if (!strcmp(v->v.sval, "false")) p->iso = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static PlClause *parse_clause(Parser *p) {
    Token pk = lexer_peek(&p->lx);
    if (pk.kind == TK_EOF) return NULL;
    TreeScope *ts = &p->ts; ts->e.len = 0;
    PlClause *cl = ct_zalloc(1, sizeof(PlClause));
    cl->lineno = pk.line;
    if (pk.kind == TK_NECK) {
        lexer_next(&p->lx);
        tree_t *body_tr = pt_term(p, ts, 1200);
        Token dot = lexer_next(&p->lx);
        if (dot.kind != TK_DOT)
            perror_at(p, dot.line, "expected . after directive");
        register_op_directive(body_tr); dq_directive(p, body_tr); iso_directive(p, body_tr);
        cl->nbody = 0;
        { tree_t *_cl = ast_node_new(TT_CLAUSE);
          ast_push(_cl, ast_node_new(TT_NUL));
          if (body_tr) ast_push(_cl, body_tr);
          cl->tr = _cl; }
        return cl;
    }
    tree_t *head_tr = pt_term(p, ts, 1199);
    pk = lexer_peek(&p->lx);
    if (pk.kind == TK_NECK) {
        lexer_next(&p->lx);
        tree_t *body_tr = pt_term(p, ts, 1200);
        { tree_t *_cl = ast_node_new(TT_CLAUSE);
          ast_push(_cl, head_tr);
          if (body_tr) ast_push(_cl, body_tr);
          cl->tr = _cl; }
        Token dot = lexer_next(&p->lx);
        if (dot.kind != TK_DOT)
            perror_at(p, dot.line, "expected . at end of clause");
    } else if (pk.kind == TK_OP && strcmp(pk.text, "-->") == 0) {
        Token arrow_tk = lexer_next(&p->lx);
        tree_t *body_tr = pt_term(p, ts, 1200);
        { tree_t *_cl = ast_node_new(TT_CLAUSE);
          ast_push(_cl, pt_stamp(pt_binop("-->", head_tr, body_tr), arrow_tk.line));
          cl->tr = _cl; }
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
    "length(L,N):-'$len_skel'(L,0,K,T),'$len_tail'(T,L,K,N).\n"
    "'$len_skel'(L,A,A,L):-var(L),!.\n"
    "'$len_skel'([_|T],A,K,R):-!,A1 is A+1,'$len_skel'(T,A1,K,R).\n"
    "'$len_skel'(L,A,A,L).\n"
    "'$len_tail'(T,L,_,_):-nonvar(T),T\\==[],!,throw(error(type_error(list,L),length/2)).\n"
    "'$len_tail'(_,_,_,N):-nonvar(N),\\+ integer(N),!,throw(error(type_error(integer,N),length/2)).\n"
    "'$len_tail'(_,_,_,N):-integer(N),N<0,!,throw(error(domain_error(not_less_than_zero,N),length/2)).\n"
    "'$len_tail'(_,_,_,N):-integer(N),N>=4294967296,!,throw(error(resource_error(stack),length/2)).\n"
    "'$len_tail'(T,_,K,N):-T==[],!,N=K.\n"
    "'$len_tail'(T,_,K,N):-var(N),!,'$len_gen'(T,K,N).\n"
    "'$len_tail'(T,_,K,N):-N>=K,D is N-K,'$len_mk'(D,T).\n"
    "'$len_gen'([],K,K).\n"
    "'$len_gen'([_|T],K,N):-K1 is K+1,'$len_gen'(T,K1,N).\n"
    "'$len_mk'(0,[]):-!.\n"
    "'$len_mk'(D,[_|T]):-D1 is D-1,'$len_mk'(D1,T).\n"
    "numlist(L,H,R):-'$numlist_'(L,H,R).\n"
    "'$numlist_'(L,H,[]):-L>H,!.\n"
    "'$numlist_'(L,H,[L|T]):-L=<H,L1 is L+1,'$numlist_'(L1,H,T).\n"
    "memberchk(X,L):-member(X,L),!.\n"
    "list(L):-is_list(L).\n"
    "g_assign(K,V):-retractall('$g_var'(K,_)),assertz('$g_var'(K,V)).\n"
    "g_read(K,V):-catch('$g_var'(K,V0),error(existence_error(procedure,_),_),fail),!,V=V0.\n"
    "g_read(_,0).\n"
    "'$catch'(G,C,R,_,_,_):-catch(G,C,R).\n"
    "'$expand_term1'(T1,T2,_):-var(T1),!,T2=T1.\n"
    "'$expand_term1'(T1,T2,true):-current_predicate(term_expansion/2),call(term_expansion(T1,T2)),!.\n"
    "'$expand_term1'((H-->B),T2,_):-!,dcg_translate_rule((H-->B),T2).\n"
    "'$expand_term1'(T,T,_).\n"
    "'$predicate_property1'(F,N,built_in):-'$gnu_builtin'(F,N),\\+current_predicate(F/N).\n"
    "'$aux_name'(N):-'$gnu_aux_split'(N,_,_).\n"
    "'$pred_without_aux'(N,A,N1,A1):-('$gnu_aux_split'(N,F,FA)->N1=F,A1=FA;N1=N,A1=A).\n"
    "'$make_aux_name'(N,A,K,X):-'$pred_without_aux'(N,A,N1,A1),atom_codes(N1,NC),number_codes(A1,AC),number_codes(K,KC),"
        "append(AC,[0'_,0'$,0'a,0'u,0'x|KC],T),append([0'$|NC],[0'/|T],All),atom_codes(X,All).\n"
    "'$gnu_aux_split'(N,F,A):-atom(N),atom_codes(N,[0'$|Cs]),append(Pre,[0'_,0'$,0'a,0'u,0'x,D|Ds],Cs),!,'$gnu_digits'([D|Ds]),"
        "reverse(Pre,R),'$gnu_digit_run'(R,RD,[0'/|RF]),reverse(RD,AC),reverse(RF,FC),atom_codes(F,FC),(AC==[]->A=0;number_codes(A,AC)).\n"
    "'$gnu_digits'([]).\n"
    "'$gnu_digits'([C|Cs]):-C>=0'0,C=<0'9,'$gnu_digits'(Cs).\n"
    "'$gnu_digit_run'([C|Cs],[C|Ds],R):-C>=0'0,C=<0'9,!,'$gnu_digit_run'(Cs,Ds,R).\n"
    "'$gnu_digit_run'(R,[],R).\n"
    "is_relative_file_name(P):-atom(P),\\+sub_atom(P,0,1,_,'/').\n"
    "decompose_file_name(P,D,B,S):-atom_codes(P,Cs),('$gnu_last_sep'(Cs,0'/,DP,BC)->append(DP,[0'/],DC);DC=[],BC=Cs),"
        "('$gnu_last_sep'(BC,0'.,PC,SP)->SC=[0'.|SP];PC=BC,SC=[]),atom_codes(D,DC),atom_codes(B,PC),atom_codes(S,SC).\n"
    "'$gnu_last_sep'(Cs,Ch,Pre,Post):-reverse(Cs,R),'$gnu_upto'(R,Ch,RPost,RPre),reverse(RPost,Post),reverse(RPre,Pre).\n"
    "'$gnu_upto'([C|Cs],C,[],Cs):-!.\n"
    "'$gnu_upto'([C|Cs],Ch,[C|P],R):-'$gnu_upto'(Cs,Ch,P,R).\n"
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
    "maplist(_,[]).\n"
    "maplist(G,[X|Xs]):-call(G,X),maplist(G,Xs).\n"
    "maplist(_,[],[]).\n"
    "maplist(G,[X|Xs],[Y|Ys]):-call(G,X,Y),maplist(G,Xs,Ys).\n"
    "maplist(_,[],[],[]).\n"
    "maplist(G,[X|Xs],[Y|Ys],[Z|Zs]):-call(G,X,Y,Z),maplist(G,Xs,Ys,Zs).\n"
    "maplist(_,[],[],[],[]).\n"
    "maplist(G,[W|Ws],[X|Xs],[Y|Ys],[Z|Zs]):-call(G,W,X,Y,Z),maplist(G,Ws,Xs,Ys,Zs).\n"
    "'>>'(P,B,A1):-copy_term(P>>B,P1>>B1),P1=[A1],call(B1).\n"
    "'>>'(P,B,A1,A2):-copy_term(P>>B,P1>>B1),P1=[A1,A2],call(B1).\n"
    "'>>'(P,B,A1,A2,A3):-copy_term(P>>B,P1>>B1),P1=[A1,A2,A3],call(B1).\n"
    "'>>'(P,B,A1,A2,A3,A4):-copy_term(P>>B,P1>>B1),P1=[A1,A2,A3,A4],call(B1).\n"
    "call_nth(G,_):-var(G),!,throw(error(instantiation_error,call_nth/2)).\n"
    "call_nth(G,_):- \\+ callable(G),!,throw(error(type_error(callable,G),call_nth/2)).\n"
    "call_nth(_,N):-nonvar(N),\\+ integer(N),!,throw(error(type_error(integer,N),call_nth/2)).\n"
    "call_nth(_,N):-integer(N),N<0,!,throw(error(domain_error(not_less_than_zero,N),call_nth/2)).\n"
    "call_nth(_,N):-N==0,!,fail.\n"
    "call_nth(G,N):-(retract('$cn_seq'(K0))->K is K0+1;K=1),assertz('$cn_seq'(K)),assertz('$cn_cnt'(K,0)),call(G),retract('$cn_cnt'(K,C0)),C is C0+1,assertz('$cn_cnt'(K,C)),(integer(N)->C=:=N,! ; N=C).\n"
    "subsumes_term(G,S):- \\+ \\+ (term_variables(S,V1),unify_with_occurs_check(G,S),term_variables(V1,V2),V1==V2).\n"
    "evaluable_property(E,_):-var(E),!,throw(error(instantiation_error,evaluable_property/2)).\n"
    "evaluable_property(E,_):- \\+ callable(E),!,throw(error(type_error(callable,E),evaluable_property/2)).\n"
    "evaluable_property(_,P):-nonvar(P),\\+ '$ev_dom'(P),!,throw(error(domain_error(evaluable_property,P),evaluable_property/2)).\n"
    "evaluable_property(E,P):-functor(E,F,A),functor(T,F,A),'$ev'(T,R),'$ev_is'(T,R,P).\n"
    "'$ev_dom'(built_in).\n"
    "'$ev_dom'(static).\n"
    "'$ev_dom'(dynamic).\n"
    "'$ev_dom'(foreign).\n"
    "'$ev_dom'(iso).\n"
    "'$ev_dom'(template(_,_)).\n"
    "'$ev_is'(_,_,built_in).\n"
    "'$ev_is'(_,_,static).\n"
    "'$ev_is'(T,R,template(T,R)).\n"
    "'$ev'(abs(number),number).\n"
    "'$ev'(acosh(number),float).\n"
    "'$ev'(acos(number),float).\n"
    "'$ev'(asinh(number),float).\n"
    "'$ev'(asin(number),float).\n"
    "'$ev'(atan2(number,number),float).\n"
    "'$ev'(copysign(number,number),number).\n"
    "'$ev'(nexttoward(number,number),float).\n"
    "'$ev'(atanh(number),float).\n"
    "'$ev'(atan(number),float).\n"
    "'$ev'(ceiling(float),integer).\n"
    "'$ev'(cosh(number),float).\n"
    "'$ev'(cos(number),float).\n"
    "'$ev'(e,float).\n"
    "'$ev'(epsilon,float).\n"
    "'$ev'(exp(number),float).\n"
    "'$ev'(float_fractional_part(float),float).\n"
    "'$ev'(float_integer_part(float),float).\n"
    "'$ev'(float(number),float).\n"
    "'$ev'(floor(float),integer).\n"
    "'$ev'(gcd(integer,integer),integer).\n"
    "'$ev'((integer div integer),integer).\n"
    "'$ev'((\\(integer)),integer).\n"
    "'$ev'((integer//integer),integer).\n"
    "'$ev'((integer/\\integer),integer).\n"
    "'$ev'((integer<<integer),integer).\n"
    "'$ev'((integer>>integer),integer).\n"
    "'$ev'((integer\\/integer),integer).\n"
    "'$ev'((integer mod integer),integer).\n"
    "'$ev'((integer rem integer),integer).\n"
    "'$ev'(log10(number),float).\n"
    "'$ev'(log(number),float).\n"
    "'$ev'(log(number,number),float).\n"
    "'$ev'(lsb(integer),integer).\n"
    "'$ev'(max(number,number),number).\n"
    "'$ev'(min(number,number),number).\n"
    "'$ev'(msb(integer),integer).\n"
    "'$ev'((+(number)),number).\n"
    "'$ev'((-(number)),number).\n"
    "'$ev'((number**number),float).\n"
    "'$ev'((number/number),float).\n"
    "'$ev'((number*number),number).\n"
    "'$ev'((number+number),number).\n"
    "'$ev'((number-number),number).\n"
    "'$ev'((number^number),number).\n"
    "'$ev'(pi,float).\n"
    "'$ev'(popcount(integer),integer).\n"
    "'$ev'(round(float),integer).\n"
    "'$ev'(sign(number),number).\n"
    "'$ev'(sinh(number),float).\n"
    "'$ev'(sin(number),float).\n"
    "'$ev'(sqrt(number),float).\n"
    "'$ev'(tanh(number),float).\n"
    "'$ev'(tan(number),float).\n"
    "'$ev'(truncate(float),integer).\n"
    "'$ev'(xor(integer,integer),integer).\n"
    "assertion(G):-(\\+ \\+ call(G)->true;throw(error(assertion_failed(G),assertion/1))).\n"
    "cyclic_term(T):- \\+ acyclic_term(T).\n"
    "context_module(user).\n"
    "utf8_codes([H|T]) --> utf8_code(H), !, utf8_codes(T).\n"
    "utf8_codes([]) --> [].\n"
    "utf8_code(C) --> [C0], { nonvar(C0) }, !, ( {C0 < 0x80} -> {C = C0} ; {C0/\\0xe0 =:= 0xc0} -> utf8_cont(C1, 0), {C is (C0/\\0x1f)<<6\\/C1} ; {C0/\\0xf0 =:= 0xe0} -> utf8_cont(C1, 6), utf8_cont(C2, 0), {C is ((C0/\\0xf)<<12)\\/C1\\/C2} ; {C0/\\0xf8 =:= 0xf0} -> utf8_cont(C1, 12), utf8_cont(C2, 6), utf8_cont(C3, 0), {C is ((C0/\\0x7)<<18)\\/C1\\/C2\\/C3} ; {C0/\\0xfc =:= 0xf8} -> utf8_cont(C1, 18), utf8_cont(C2, 12), utf8_cont(C3, 6), utf8_cont(C4, 0), {C is ((C0/\\0x3)<<24)\\/C1\\/C2\\/C3\\/C4} ; {C0/\\0xfe =:= 0xfc} -> utf8_cont(C1, 24), utf8_cont(C2, 18), utf8_cont(C3, 12), utf8_cont(C4, 6), utf8_cont(C5, 0), {C is ((C0/\\0x1)<<30)\\/C1\\/C2\\/C3\\/C4\\/C5} ).\n"
    "utf8_code(C) --> { nonvar(C) }, !, ( { C < 0x80 } -> [C] ; { C < 0x800 } -> { C0 is 0xc0\\/((C>>6)/\\0x1f), C1 is 0x80\\/(C/\\0x3f) }, [C0,C1] ; { C < 0x10000 } -> { C0 is 0xe0\\/((C>>12)/\\0x0f), C1 is 0x80\\/((C>>6)/\\0x3f), C2 is 0x80\\/(C/\\0x3f) }, [C0,C1,C2] ; { C < 0x200000 } -> { C0 is 0xf0\\/((C>>18)/\\0x07), C1 is 0x80\\/((C>>12)/\\0x3f), C2 is 0x80\\/((C>>6)/\\0x3f), C3 is 0x80\\/(C/\\0x3f) }, [C0,C1,C2,C3] ; { C < 0x4000000 } -> { C0 is 0xf8\\/((C>>24)/\\0x03), C1 is 0x80\\/((C>>18)/\\0x3f), C2 is 0x80\\/((C>>12)/\\0x3f), C3 is 0x80\\/((C>>6)/\\0x3f), C4 is 0x80\\/(C/\\0x3f) }, [C0,C1,C2,C3,C4] ; { C < 0x80000000 } -> { C0 is 0xfc\\/((C>>30)/\\0x01), C1 is 0x80\\/((C>>24)/\\0x3f), C2 is 0x80\\/((C>>18)/\\0x3f), C3 is 0x80\\/((C>>12)/\\0x3f), C4 is 0x80\\/((C>>6)/\\0x3f), C5 is 0x80\\/(C/\\0x3f) }, [C0,C1,C2,C3,C4,C5] ).\n"
    "utf8_cont(Val, Shift) --> [C], { C/\\0xc0 =:= 0x80, Val is (C/\\0x3f)<<Shift }.\n"
    "sequence(OnElem,List)-->'$seq_'(List,OnElem).\n"
    "'$seq_'([H|T],P)-->call(P,H),'$seq_'(T,P).\n"
    "'$seq_'([],_)-->[].\n"
    "sequence(OnElem,OnSep,List)-->'$seq_'(List,OnElem,OnSep).\n"
    "sequence(Start,OnElem,OnSep,End,List)-->Start,'$seq_'(List,OnElem,OnSep),End,!.\n"
    "'$seq_'(List,OnElem,OnSep)-->{var(List)},!,(call(OnElem,H)*->(OnSep->!,{List=[H|T]},'$seq_as'(T,OnElem,OnSep);{List=[H]});{List=[]}).\n"
    "'$seq_'([H|T],OnElem,OnSep)-->call(OnElem,H),({T==[]}->[];OnSep,'$seq_'(T,OnElem,OnSep)).\n"
    "'$seq_'([],_,_)-->[].\n"
    "'$seq_as'([H|T],OnElem,OnSep)-->call(OnElem,H),(OnSep->!,'$seq_as'(T,OnElem,OnSep);{T=[]}).\n"
    "optional(Match,_)-->Match,!.\n"
    "optional(_,Default)-->Default,!.\n"
    "foreach(Generator,Rule)-->foreach(Generator,Rule,[]).\n"
    "foreach(Generator,Rule,Sep)-->{term_variables(Generator,GV0),sort(GV0,GV),term_variables((Rule,Sep),RV0),sort(RV0,RV),subtract(RV,GV,SGV),intersection(GV,RV,SV),Templ=..[v|SV],STempl=..[v|SGV],findall(Templ,Generator,List)},'$emit_list'(List,Templ,STempl,Rule,Sep).\n"
    "'$emit_list'([],_,_,_,_)-->[].\n"
    "'$emit_list'([H|T],Templ,STempl,OnElem,OnSep)-->{copy_term(t(Templ,STempl,OnElem,OnSep),t(H,STempl,OnElemC,OnSepC))},phrase(OnElemC),({T==[]}->[];phrase(OnSepC),'$emit_list'(T,Templ,STempl,OnElem,OnSep)).\n";
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pl_clause_dcg(const PlClause *cl) {
    tree_t *r;
    if (!cl || !cl->tr || cl->tr->n != 1 || !(r = cl->tr->c[0]) || r->t != TT_FNC || r->n != 2 || !r->v.sval || strcmp(r->v.sval, "-->")) return (tree_t *)0;
    return r;
}
static int pl_clause_key(PlClause *cl, const char **name_out, int *ar_out) {
    tree_t *dcg = pl_clause_dcg(cl);
    if (!cl) return 0;
    if (dcg) {
        tree_t *hd = dcg->c[0];
        if (hd && hd->t == TT_FNC && hd->n == 2 && hd->v.sval && !strcmp(hd->v.sval, ",")) hd = hd->c[0];
        if (hd && hd->t == TT_QLIT && hd->v.sval) { *name_out = hd->v.sval; *ar_out = 2;          return 1; }
        if (hd && hd->t == TT_FNC  && hd->v.sval) { *name_out = hd->v.sval; *ar_out = hd->n + 2;  return 1; }
        return 0;
    }
    if (cl->tr && cl->tr->n > 0 && cl->tr->c[0] && cl->tr->c[0]->t != TT_NUL) {
        tree_t *hd = cl->tr->c[0];
        if (hd->t == TT_QLIT && hd->v.sval) { *name_out = hd->v.sval; *ar_out = 0;      return 1; }
        if (hd->t == TT_FNC  && hd->v.sval) { *name_out = hd->v.sval; *ar_out = hd->n;  return 1; }
    }
    return 0;
}
static tree_t *pl_clause_body(PlClause *cl) {
    tree_t *dcg = pl_clause_dcg(cl);
    if (dcg) return dcg->c[1];
    return (cl && cl->tr && cl->tr->n > 1) ? cl->tr->c[1] : (tree_t *)0;
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
static int pl_cv_has(const cv_t *v, const char *s) {
    for (uint32_t i = 0; i < v->len; i++) if (!strcmp(CV_AT(*v, char *, i), s)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_cv_add(cv_t *v, const char *s) { if (!pl_cv_has(v, s)) CV_PUSH(*v, char *) = ct_strdup(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pl_pred_key(const char *nm, int ar) { size_t l = strlen(nm) + 16; char *k = (char *)ct_alloc(l); snprintf(k, l, "%s/%d", nm, ar); return k; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_tree_collect_calls(const tree_t *t, cv_t *names) {
    if (!t) return;
    if ((t->t == TT_FNC || t->t == TT_QLIT || t->t == TT_NAME) && t->v.sval) pl_cv_add(names, t->v.sval);
    for (int i = 0; i < t->n; i++) pl_tree_collect_calls(t->c[i], names);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void prolog_inject_prelude(PlProgram *prog, const char *user_src) {
    if (!prog || !user_src) return;
    cv_t user_defined = { 0 }, referenced = { 0 }, wanted = { 0 };
    for (PlClause *cl = prog->head; cl; cl = cl->next) {
        const char *nm; int ar;
        if (pl_clause_key(cl, &nm, &ar) && nm) pl_cv_add(&user_defined, pl_pred_key(nm, ar));
        if (cl->tr) pl_tree_collect_calls(cl->tr, &referenced);
    }
    PlProgram *pre = prolog_parse(PL_PRELUDE_SRC, "<prelude>");
    if (!pre || !pre->head) { if (pre) ct_drop(pre); return; }
    for (PlClause *cl = pre->head; cl; cl = cl->next) {
        const char *nm; int ar;
        if (!pl_clause_key(cl, &nm, &ar) || !nm) continue;
        if (!pl_cv_has(&referenced, nm) && (nm[0] == '$' || !pl_word_referenced(user_src, nm))) continue;
        char *key = pl_pred_key(nm, ar);
        if (!pl_cv_has(&user_defined, key)) pl_cv_add(&wanted, key);
    }
    int changed = 1;
    while (changed) {
        changed = 0;
        for (PlClause *cl = pre->head; cl; cl = cl->next) {
            const char *nm; int ar;
            if (!pl_clause_key(cl, &nm, &ar) || !nm) continue;
            if (!pl_cv_has(&wanted, pl_pred_key(nm, ar))) continue;
            cv_t calls = { 0 };
            pl_tree_collect_calls(pl_clause_body(cl), &calls);
            for (uint32_t ci = 0; ci < calls.len; ci++) {
                for (PlClause *d = pre->head; d; d = d->next) {
                    const char *dn; int dar;
                    if (!pl_clause_key(d, &dn, &dar) || !dn) continue;
                    if (strcmp(dn, CV_AT(calls, char *, ci))) continue;
                    char *dk = pl_pred_key(dn, dar);
                    if (!pl_cv_has(&wanted, dk) && !pl_cv_has(&user_defined, dk)) { pl_cv_add(&wanted, dk); changed = 1; }
                }
            }
        }
    }
    PlClause *nextc = NULL;
    for (PlClause *cl = pre->head; cl; cl = nextc) {
        nextc = cl->next;
        const char *nm; int ar;
        int keep = pl_clause_key(cl, &nm, &ar) && nm && pl_cv_has(&wanted, pl_pred_key(nm, ar));
        if (keep) { cl->next = NULL; if (!prog->head) prog->head = cl; else prog->tail->next = cl; prog->tail = cl; prog->nclauses++; }
    }
    ct_drop(pre);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cv_has_key(const cv_t *v, const char *nm, int ar) {
    size_t n = strlen(nm);
    for (uint32_t i = 0; i < v->len; i++) {
        const char *e = CV_AT(*v, char *, i);
        if (!strncmp(e, nm, n) && e[n] == '/' && (int)strtol(e + n + 1, (char **)0, 10) == ar) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_prelude_defines(const char *nm, int ar) {
    if (!nm) return 0;
    if (!g_stage2.pl_prelude_keys.len) {
        PlProgram *pre = prolog_parse(PL_PRELUDE_SRC, "<prelude>");
        if (!pre) return 0;
        for (PlClause *cl = pre->head; cl; cl = cl->next) {
            const char *cn; int car;
            if (pl_clause_key(cl, &cn, &car) && cn && !pl_cv_has_key(&g_stage2.pl_prelude_keys, cn, car)) CV_PUSH(g_stage2.pl_prelude_keys, char *) = pl_pred_key(cn, car);
        }
        ct_drop(pre);
    }
    return pl_cv_has_key(&g_stage2.pl_prelude_keys, nm, ar);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_parse_loop(Parser *pp, PlProgram *prog);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pl_include_read(const Parser *pp, const char *spec, char **path_out) {
    const char *slash = strrchr(pp->filename, '/'); size_t dl = slash ? (size_t)(slash - pp->filename) + 1 : 0; size_t sl = strlen(spec);
    char *cand = (char *)ct_alloc(dl + sl + 4); FILE *f = (FILE *)0; char *src; long n;
    if (!cand) return (char *)0;
    for (int k = 0; k < 4 && !f; k++) {
        size_t o = (k < 2 && spec[0] != '/') ? dl : 0;
        if (o) memcpy(cand, pp->filename, dl);
        memcpy(cand + o, spec, sl); cand[o + sl] = 0;
        if (k % 2) memcpy(cand + o + sl, ".pl", 4);
        f = fopen(cand, "r"); }
    if (!f) { ct_drop(cand); return (char *)0; }
    fseek(f, 0, SEEK_END); n = ftell(f); rewind(f);
    src = (char *)ct_alloc((size_t)n + 1); if (!src) { fclose(f); ct_drop(cand); return (char *)0; }
    if (fread(src, 1, (size_t)n, f) != (size_t)n) src[0] = src[0];
    src[n] = 0; fclose(f); *path_out = cand; return src;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_preprocess_list(PlProgram *prog, int depth);
static PlProgram *pl_include_expand(Parser *pp, const PlClause *cl, int depth, int *is_include) {
    const tree_t *t = cl ? cl->tr : (const tree_t *)0; const tree_t *g; const char *spec; char *path = (char *)0; char *src; PlProgram *sub;
    *is_include = 0;
    if (!t || t->t != TT_CLAUSE || t->n < 2 || !t->c[0] || t->c[0]->t != TT_NUL) return (PlProgram *)0;
    g = t->c[1];
    if (!g || g->t != TT_FNC || !g->v.sval || strcmp(g->v.sval, "include") || g->n != 1 || !g->c[0] || (g->c[0]->t != TT_QLIT && g->c[0]->t != TT_NAME) || !g->c[0]->v.sval) return (PlProgram *)0;
    *is_include = 1;
    spec = g->c[0]->v.sval;
    if (depth >= 16 || !(src = pl_include_read(pp, spec, &path))) {
        if (!pp->quiet) fprintf(stderr, "%s:%d: include: cannot read '%s'\n", pp->filename, cl->lineno, spec);
        pp->nerrors++; return (PlProgram *)0; }
    sub = prolog_parse_ex(src, path, pp->quiet);
    if (!sub) { pp->nerrors++; return (PlProgram *)0; }
    pl_preprocess_list(sub, depth + 1);
    pp->nerrors += sub->nerrors;
    return sub;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_preprocess_list(PlProgram *prog, int depth) {
    Parser q; PlClause *prev = (PlClause *)0, *cl;
    if (!prog) return;
    memset(&q, 0, sizeof q); q.filename = prog->filename ? prog->filename : "<stdin>"; q.quiet = prog->quiet; q.incl_depth = depth;
    cl = prog->head;
    while (cl) {
        PlClause *next = cl->next; int drop = 0, is_include = 0; const tree_t *t = cl->tr;
        prolog_fold_pieces(cl->tr);
        if (t && t->t == TT_CLAUSE && t->n == 2 && t->c[0] && t->c[0]->t == TT_NUL && t->c[1] && try_handle_if_directive_tree(&q, t->c[1], cl->lineno)) drop = 1;
        else if (!if_currently_active(&q)) drop = 1;
        else {
            PlProgram *sub = pl_include_expand(&q, cl, depth, &is_include);
            if (is_include) {
                drop = 1;
                if (sub && sub->head) {
                    if (prev) prev->next = sub->head; else prog->head = sub->head;
                    sub->tail->next = next; prev = sub->tail; prog->nclauses += sub->nclauses;
                }
            }
        }
        if (drop) {
            if (!is_include || !prev || prev->next != next) { if (prev) prev->next = next; else prog->head = next; }
            prog->nclauses--;
        } else prev = cl;
        cl = next;
    }
    prog->tail = prev;
    if (q.ifst.len != 0) { fprintf(stderr, "%s: parse error: unmatched :- if (opened at line %d)\n", q.filename, CV_AT(q.ifst, IfFrame, q.ifst.len - 1).line); q.nerrors++; }
    prog->nerrors += q.nerrors;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void prolog_preprocess(PlProgram *prog) { pl_preprocess_list(prog, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_parse_loop(Parser *pp, PlProgram *prog) {
    for (;;) {
        Token pk = lexer_peek(&pp->lx);
        if (pk.kind == TK_EOF) break;
        if (pk.kind == TK_ERROR) {
            if (!pp->quiet) fprintf(stderr, "%s:%d: lex error: %s\n",
                    pp->filename, pk.line, pk.text);
            pp->nerrors++;
            lexer_next(&pp->lx);
            if (pp->lx.last_kind != TK_DOT) resync_past_clause_end(pp);
            continue;
        }
        pp->clause_errs = 0;
        PlClause *cl = parse_clause(pp);
        if (pp->clause_errs > 0) {
            if (cl) ct_drop(cl);
            pp->lx.fenced = 0;
            pp->lx.has_peek = 0;
            continue;
        }
        if (!cl) break;
        if (cl->nbody == 0 && cl->tr == NULL) {
            ct_drop(cl);
            continue;
        }
        if (!prog->head) prog->head = cl;
        else             prog->tail->next = cl;
        prog->tail = cl;
        prog->nclauses++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PlProgram *prolog_parse_ex(const char *src, const char *filename, int quiet) {
    prolog_atom_init();
    Parser p;
    lexer_init(&p.lx, src);
    p.filename = filename ? filename : "<input>";
    p.nerrors  = 0;
    p.clause_errs = 0;
    p.in_args  = 0;
    p.quiet    = quiet;
    p.dq       = rt_pl_double_quotes_mode();
    p.prec     = 0;
    p.incl_depth = 0;
    { extern int rt_pl_iso_mode(void); p.iso = rt_pl_iso_mode(); }
    memset(&p.ts, 0, sizeof p.ts);
    PlProgram *prog = ct_zalloc(1, sizeof(PlProgram));
    pl_parse_loop(&p, prog);
    prog->nerrors = p.nerrors;
    prog->src = src;
    prog->filename = filename;
    prog->quiet = quiet;
    return prog;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
PlProgram *prolog_parse(const char *src, const char *filename) { return prolog_parse_ex(src, filename, 0); }
