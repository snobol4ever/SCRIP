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
    { "|",    1105, ASSOC_RIGHT },
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
        case TK_ATOM: case TK_OP: return (find_prefix(pk.text) != NULL) || (find_binop(pk.text) == NULL) || !strcmp(pk.text, "-") || !strcmp(pk.text, "+");
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
        case TT_VAR: case TT_ILIT: case TT_FLIT: case TT_CUT: case TT_DQLIT:
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
        case TT_ILIT: case TT_FLIT: case TT_CUT: case TT_DQLIT:
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
            tree_t *n = ast_node_new(TT_DQLIT);
            if (tk.pc) ast_push(n, tk.pc);
            else { tree_t *e = ast_node_new(TT_QLIT); e->v.sval = ct_strdup(tk.text); ast_push(n, e); }
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
                if (prefix_arg_starts(pk)) {
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
            if (pkn.kind == TK_LPAREN && pkn.adj) {
                lexer_next(&p->lx);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = ct_strdup(":-");
                pt_args(p, ts, fnc);
                Token rp = lexer_peek(&p->lx);
                if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                else perror_at(p, rp.line, "expected ) to close argument list");
                return pt_stamp(fnc, ln);
            }
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
            if (pkq.kind == TK_LPAREN && pkq.adj) {
                lexer_next(&p->lx);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = ct_strdup("?-");
                pt_args(p, ts, fnc);
                Token rp = lexer_peek(&p->lx);
                if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                else perror_at(p, rp.line, "expected ) to close argument list");
                return pt_stamp(fnc, ln);
            }
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
static tree_t *dcg_dq_codes(const tree_t *dq) {
    const char *b = (dq->n > 0 && dq->c[0] && dq->c[0]->v.sval) ? dq->c[0]->v.sval : ""; size_t n = strlen(b);
    tree_t *acc = mk_atom(ATOM_NIL);
    for (size_t i = n; i > 0; i--) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = (long long)(unsigned char)b[i - 1]; tree_t *dargs[2] = { e, acc }; acc = mk_raw(ATOM_DOT, dargs, 2); }
    return acc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *dcg_append_tail(TreeScope *ts, tree_t *list, tree_t *tail) {
    if (!list) return dcg_var_use(ts, tail);
    if (list->t == TT_DQLIT) list = dcg_dq_codes(list);
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
static void dcg_fill_lines(tree_t *t, int ln) {
    if (!t) return;
    pt_stamp(t, ln);
    for (int i = 0; i < t->n; i++) dcg_fill_lines(t->c[i], t->line);
}
static int dcg_expand_body_at(tree_t *body, tree_t *s_in, tree_t *s_out, TreeScope *ts, tree_t **buf, int idx);
static int dcg_need(tree_t *b) {
    if (!b) return 1;
    if (b->t == TT_FNC && b->v.sval && b->n == 1 && strcmp(b->v.sval, "{}") == 0) return dcg_count_conj(b->c[0]) + 1;
    if (b->t == TT_FNC && b->v.sval && b->n == 2 && strcmp(b->v.sval, ",") == 0) return dcg_need(b->c[0]) + dcg_need(b->c[1]);
    if (b->t == TT_FNC && b->v.sval && b->n == 1 && strcmp(b->v.sval, "\\+") == 0) return 2;
    if (b->t == TT_CUT) return 2;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dcg_expand_body(tree_t *body, tree_t *s_in, tree_t *s_out,
                           TreeScope *ts, tree_t **buf, int idx) {
    int i0 = idx;
    idx = dcg_expand_body_at(body, s_in, s_out, ts, buf, idx);
    for (int i = i0; i < idx && body; i++) pt_stamp(buf[i], body->line);
    return idx;
}
static int dcg_expand_body_at(tree_t *body, tree_t *s_in, tree_t *s_out, TreeScope *ts, tree_t **buf, int idx) {
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
    if (body->t == TT_DQLIT || (body->t == TT_FNC && body->v.sval && strcmp(body->v.sval, ".") == 0 && body->n == 2)) {
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
        tree_t *buf_a[dcg_need(body->c[0])]; int na = 0;
        tree_t *buf_b[dcg_need(body->c[1])]; int nb = 0;
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
        tree_t *buf_c[dcg_need(body->c[0])]; int nc = 0; tree_t *buf_t[dcg_need(body->c[1])]; int nt = 0; tree_t *s_mid = dcg_fresh_var(ts);
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
        tree_t *buf_g[dcg_need(body->c[0])]; int ng = 0; tree_t *s_void = dcg_fresh_var(ts);
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
    tree_t *buf[dcg_need(dcg_body) + (pushback ? 1 : 0)];
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
    dcg_fill_lines(_cl, head_tr->line);
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
    pt_stamp(head_reshaped, arrow->line > 0 ? arrow->line : cl->tr->line);
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
        Token pa = lexer_peek(&p->lx);
        if (pa.kind == TK_LPAREN && pa.adj) {
            lexer_next(&p->lx);
            tree_t *fnc = ast_node_new(TT_FNC);
            fnc->v.sval = ct_strdup(":-");
            pt_args(p, ts, fnc);
            Token rp = lexer_peek(&p->lx);
            if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
            else perror_at(p, rp.line, "expected ) to close argument list");
            Token fdot = lexer_next(&p->lx);
            if (fdot.kind != TK_DOT) perror_at(p, fdot.line, "expected . at end of clause");
            tree_t *_fcl = ast_node_new(TT_CLAUSE);
            if (fnc->n == 2) { ast_push(_fcl, fnc->c[0]); ast_push(_fcl, fnc->c[1]); }
            else if (fnc->n == 1) { register_op_directive(fnc->c[0]); iso_directive(p, fnc->c[0]); cl->nbody = 0; ast_push(_fcl, ast_node_new(TT_NUL)); ast_push(_fcl, fnc->c[0]); }
            else ast_push(_fcl, fnc);
            cl->tr = _fcl;
            return cl;
        }
        tree_t *body_tr = pt_term(p, ts, 1200);
        Token dot = lexer_next(&p->lx);
        if (dot.kind != TK_DOT)
            perror_at(p, dot.line, "expected . after directive");
        register_op_directive(body_tr); iso_directive(p, body_tr);
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
    "length(L,N):-'$skip_list'(K,L,T),'$len_tail'(T,L,K,N).\n"
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
    "sub_string(S,B,L,A,Sub):-var(Sub),!,sub_atom(S,B,L,A,X),atom_string(X,Sub).\n"
    "sub_string(S,B,L,A,Sub):-string(Sub),!,sub_atom(S,B,L,A,Sub).\n"
    "sub_string(S,B,L,A,Sub):-atom_string(Sub,T),sub_atom(S,B,L,A,T).\n"
    "string_code(I,_,_):-var(I),!,throw(error(instantiation_error,string_code/3)).\n"
    "atomics_to_string(L,S):-atomic_list_concat(L,A),atom_string(A,S).\n"
    "string_code(I,S,C):-integer(I),I>0,string_codes(S,Cs),nth1(I,Cs,C0),!,C=C0.\n"
    "split_string(S,Sep,Pad,Subs):-string_codes(S,Cs),string_codes(Sep,SC),string_codes(Pad,PC),'$ss_fields'(Cs,SC,Fs),'$ss_strip_all'(Fs,PC,Subs).\n"
    "'$ss_fields'(Cs,SC,[F|Fs]):-'$ss_take'(Cs,SC,F,R),(R=[]->Fs=[];R=[_|R1],'$ss_fields'(R1,SC,Fs)).\n"
    "'$ss_take'([],_,[],[]).\n"
    "'$ss_take'([C|Cs],SC,[],[C|Cs]):-memberchk(C,SC),!.\n"
    "'$ss_take'([C|Cs],SC,[C|F],R):-'$ss_take'(Cs,SC,F,R).\n"
    "'$ss_strip_all'([],_,[]).\n"
    "'$ss_strip_all'([F|Fs],PC,[S|Ss]):-'$ss_lstrip'(F,PC,F1),reverse(F1,R1),'$ss_lstrip'(R1,PC,R2),reverse(R2,F2),string_codes(S,F2),'$ss_strip_all'(Fs,PC,Ss).\n"
    "'$ss_lstrip'([C|Cs],PC,R):-memberchk(C,PC),!,'$ss_lstrip'(Cs,PC,R).\n"
    "'$ss_lstrip'(L,_,L).\n"
    "list(L):-is_list(L).\n"
    "g_assign(N,V):-'$gv_asg'(N,V,nb,c).\n"
    "g_assignb(N,V):-'$gv_asg'(N,V,b,c).\n"
    "g_link(N,V):-'$gv_asg'(N,V,b,l).\n"
    "g_read(N,V):-'$gv_tgt'(N,K,P,Ss),'$gv_get'(K,S),'$gv_at'(S,P,C),(Ss==[]->'$gv_rd'(C,V);'$gv_term'(C,T),'$gv_sub'(T,Ss,X),copy_term(X,V)).\n"
    "g_array_size(N,Z):-'$gv_tgt'(N,K,P,Ss),'$gv_uint'(y,Z),Ss==[],'$gv_get'(K,S),'$gv_at'(S,P,C),C='$ga'(_,Es),length(Es,Z).\n"
    "g_inc(N):-'$gv_incdec'(N,1,_,_,n,n).\n"
    "g_inco(N,O):-'$gv_incdec'(N,1,O,_,y,n).\n"
    "g_inc(N,W):-'$gv_incdec'(N,1,_,W,n,y).\n"
    "g_inc(N,O,W):-'$gv_incdec'(N,1,O,W,y,y).\n"
    "g_dec(N):-'$gv_incdec'(N,-1,_,_,n,n).\n"
    "g_deco(N,O):-'$gv_incdec'(N,-1,O,_,y,n).\n"
    "g_dec(N,W):-'$gv_incdec'(N,-1,_,W,n,y).\n"
    "g_dec(N,O,W):-'$gv_incdec'(N,-1,O,W,y,y).\n"
    "g_set_bit(N,B):-'$gv_bit'(B,M),'$gv_bitop'(N,s,M).\n"
    "g_reset_bit(N,B):-'$gv_bit'(B,M),'$gv_bitop'(N,r,M).\n"
    "g_test_set_bit(N,B):-'$gv_bit'(B,M),'$gv_int'(N,I),I/\\M=\\=0.\n"
    "g_test_reset_bit(N,B):-'$gv_bit'(B,M),'$gv_int'(N,I),I/\\M=:=0.\n"
    "'$gv_err'(E):-throw(error(E,_)).\n"
    "'$gv_key'(A,K):-atom_concat('$gv:',A,K),!.\n"
    "'$gv_get'(K,S):-catch(nb_getval(K,S0),error(existence_error(_,_),_),S0='$gl'(0)),!,S=S0.\n"
    "'$gv_set'(nb,K,S):-!,nb_setval(K,S).\n"
    "'$gv_set'(b,K,S):-catch(nb_getval(K,_),error(existence_error(_,_),_),nb_setval(K,'$gl'(0))),!,b_setval(K,S).\n"
    "'$gv_callable'(N):-var(N),!,'$gv_err'(instantiation_error).\n"
    "'$gv_callable'(N):-callable(N),!.\n"
    "'$gv_callable'(N):-'$gv_err'(type_error(callable,N)).\n"
    "'$gv_tgt'(N,K,P,Ss):-'$gv_callable'(N),N=B-I0,!,'$gv_tgt'(B,K,P,Ss0),'$gv_idx'(I0,I),'$gv_get'(K,S),'$gv_at'(S,P,C),('$gv_term'(C,T0),'$gv_sub'(T0,Ss0,T1),'$gv_sel'(T1,I,_)->true;'$gv_err'(domain_error(g_argument_selector,N))),'$gv_app'(Ss0,[I],Ss).\n"
    "'$gv_tgt'(N,K,P,[]):-N=..[A|Is],'$gv_key'(A,K),(Is==[]->P=[];'$gv_get'(K,S),(S='$ga'(_,_)->true;'$gv_err'(domain_error(g_array_index,N))),'$gv_path'(Is,N,K,[],P)).\n"
    "'$gv_path'([],_,_,P,P):-!.\n"
    "'$gv_path'([I0|Is],N,K,P0,P):-'$gv_idx'(I0,I),'$gv_get'(K,S),'$gv_at'(S,P0,C),(C='$ga'(St,Es),I>=0->true;'$gv_err'(domain_error(g_array_index,N))),length(Es,Sz),(I<Sz->true;St=='$none'->'$gv_err'(domain_error(g_array_index,N));I>1048576->'$gv_err'(domain_error(g_array_index,N));'$gv_pow2'(1,I,NSz),Ad is NSz-Sz,'$gv_rep'(Ad,St,New),'$gv_app'(Es,New,Es2),'$gv_put'(S,P0,'$ga'(St,Es2),S2),nb_setval(K,S2)),'$gv_app'(P0,[I],P1),'$gv_path'(Is,N,K,P1,P).\n"
    "'$gv_idx'(I0,I):-integer(I0),!,I=I0.\n"
    "'$gv_idx'(I0,I):-'$gv_int'(I0,I).\n"
    "'$gv_int'(N,I):-'$gv_tgt'(N,K,P,Ss),'$gv_get'(K,S),'$gv_at'(S,P,C),'$gv_intval'(C,Ss,I).\n"
    "'$gv_intval'('$ga'(_,_),_,_):-!,'$gv_err'(type_error(integer,g_array)).\n"
    "'$gv_intval'(C,Ss,I):-'$gv_term'(C,T),'$gv_sub'(T,Ss,X),(integer(X)->I=X;'$gv_err'(type_error(integer,X))).\n"
    "'$gv_intput'(C,Ss,I,C2):-'$gv_term'(C,T),(Ss==[]->T2=I;'$gv_subput'(T,Ss,I,T2)),functor(C,F,1),C2=..[F,T2].\n"
    "'$gv_pow2'(S,I,N):-S>I,!,N=S.\n"
    "'$gv_pow2'(S,I,N):-S2 is S*2,'$gv_pow2'(S2,I,N).\n"
    "'$gv_rep'(0,_,[]):-!.\n"
    "'$gv_rep'(N,X,[X|T]):-N1 is N-1,'$gv_rep'(N1,X,T).\n"
    "'$gv_app'([],L,L):-!.\n"
    "'$gv_app'([H|T],L,[H|R]):-'$gv_app'(T,L,R).\n"
    "'$gv_nth0'(0,[X|_],X):-!.\n"
    "'$gv_nth0'(I,[_|T],X):-I1 is I-1,'$gv_nth0'(I1,T,X).\n"
    "'$gv_setnth0'(0,[_|T],X,[X|T]):-!.\n"
    "'$gv_setnth0'(I,[H|T],X,[H|T2]):-I1 is I-1,'$gv_setnth0'(I1,T,X,T2).\n"
    "'$gv_at'(C,[],C):-!.\n"
    "'$gv_at'('$ga'(_,Es),[I|Is],C):-'$gv_nth0'(I,Es,C0),'$gv_at'(C0,Is,C).\n"
    "'$gv_put'(_,[],New,New):-!.\n"
    "'$gv_put'('$ga'(St,Es),[I|Is],New,'$ga'(St,Es2)):-'$gv_nth0'(I,Es,C0),'$gv_put'(C0,Is,New,C1),'$gv_setnth0'(I,Es,C1,Es2).\n"
    "'$gv_term'('$gl'(T),T):-!.\n"
    "'$gv_term'('$gc'(T),T).\n"
    "'$gv_sub'(T,[],T):-!.\n"
    "'$gv_sub'(T,[I|Is],X):-'$gv_sel'(T,I,A),'$gv_sub'(A,Is,X).\n"
    "'$gv_sel'(T,I,A):-nonvar(T),T=[_|_],!,I>=1,I0 is I-1,'$gv_nth0'(I0,T,A).\n"
    "'$gv_sel'(T,I,A):-compound(T),functor(T,_,Ar),I>=1,I=<Ar,arg(I,T,A).\n"
    "'$gv_selput'(T,I,A,T2):-T=[_|_],!,I0 is I-1,'$gv_setnth0'(I0,T,A,T2).\n"
    "'$gv_selput'(T,I,A,T2):-T=..[F|As],I0 is I-1,'$gv_setnth0'(I0,As,A,As2),T2=..[F|As2].\n"
    "'$gv_subput'(T,[I|Is],V,T2):-'$gv_sel'(T,I,A),(Is==[]->A2=V;'$gv_subput'(A,Is,V,A2)),'$gv_selput'(T,I,A2,T2).\n"
    "'$gv_simple'(X):-var(X),!.\n"
    "'$gv_simple'(X):-atom(X),!.\n"
    "'$gv_simple'(X):-integer(X).\n"
    "'$gv_asg'(N,V,M,Cp):-'$gv_tgt'(N,K,P,Ss),'$gv_get'(K,S),'$gv_at'(S,P,C0),'$gv_asg1'(Ss,N,V,M,Cp,C0,C1),'$gv_put'(S,P,C1,S1),'$gv_set'(M,K,S1).\n"
    "'$gv_asg1'([],_,V,M,Cp,C0,C1):-!,'$gv_elem'(V,C0,M,Cp,C1).\n"
    "'$gv_asg1'(_,N,_,b,_,_,_):-!,'$gv_err'(domain_error(g_argument_selector,N)).\n"
    "'$gv_asg1'(Ss,_,V,_,_,C0,C1):-'$gv_term'(C0,T0),'$gv_sub'(T0,Ss,A0),'$gv_subput'(T0,Ss,V,T1),('$gv_simple'(A0),'$gv_simple'(V)->functor(C0,F,1),C1=..[F,T1];C1='$gc'(T1)).\n"
    "'$gv_elem'(V,C0,M,Cp,C):-compound(V),functor(V,F,_),'$gv_arrop'(F,Op),!,'$gv_arr'(Op,V,C0,M,Cp,C).\n"
    "'$gv_elem'(V,_,_,_,'$gl'(V)):-atom(V),!.\n"
    "'$gv_elem'(V,_,_,_,'$gl'(V)):-integer(V),!.\n"
    "'$gv_elem'(V,_,_,l,'$gl'(V)):-!.\n"
    "'$gv_elem'(V,_,b,c,'$gc'(V2)):-!,copy_term(V,V2).\n"
    "'$gv_elem'(V,_,nb,c,'$gc'(V)).\n"
    "'$gv_arrop'(g_array,a):-!.\n"
    "'$gv_arrop'(g_array_auto,u):-!.\n"
    "'$gv_arrop'(g_array_extend,x).\n"
    "'$gv_arr'(Op,V,C0,M,Cp,'$ga'(St,Es)):-functor(V,_,Ar),arg(1,V,Z),('$gv_arrsz'(Z,Ar,V,Sz,L,Init)->true;'$gv_err'(domain_error(g_array_index,V))),(Op==x,C0='$ga'(St0,Es0)->length(Es0,Old),(Old>=Sz->'$gv_take'(Sz,Es0,Es);'$gv_src'(L,Init,Old,Sz,Src),'$gv_elems'(Src,M,Cp,New),'$gv_app'(Es0,New,Es)),St=St0;'$gv_src'(L,Init,0,Sz,Src),'$gv_elems'(Src,M,Cp,Es),(Op==u->(L=='$none'->I1=Init;I1=0),'$gv_elem'(I1,'$gl'(0),M,Cp,St);M==b,C0='$ga'(St0,_)->St=St0;St='$none')).\n"
    "'$gv_arrsz'(Z,Ar,V,Z,'$none',Init):-integer(Z),!,Z>0,Ar=<2,(Ar=:=2->arg(2,V,Init);Init=0).\n"
    "'$gv_arrsz'(Z,1,_,Sz,Z,0):-is_list(Z),length(Z,Sz),Sz>0.\n"
    "'$gv_src'('$none',Init,From,To,Src):-!,N is To-From,'$gv_rep'(N,Init,Src).\n"
    "'$gv_src'(L,_,From,_,Src):-'$gv_drop'(From,L,Src).\n"
    "'$gv_drop'(0,L,L):-!.\n"
    "'$gv_drop'(N,[_|T],L):-N1 is N-1,'$gv_drop'(N1,T,L).\n"
    "'$gv_take'(0,_,[]):-!.\n"
    "'$gv_take'(N,[H|T],[H|R]):-N1 is N-1,'$gv_take'(N1,T,R).\n"
    "'$gv_elems'([],_,_,[]):-!.\n"
    "'$gv_elems'([X|Xs],M,Cp,[C|Cs]):-'$gv_elem'(X,'$gl'(0),M,Cp,C),'$gv_elems'(Xs,M,Cp,Cs).\n"
    "'$gv_rd'('$gl'(T),V):-!,V=T.\n"
    "'$gv_rd'('$gc'(T),V):-!,copy_term(T,V).\n"
    "'$gv_rd'('$ga'(_,Es),g_array(L)):-'$gv_rdl'(Es,L).\n"
    "'$gv_rdl'([],[]):-!.\n"
    "'$gv_rdl'([C|Cs],[X|Xs]):-'$gv_rd'(C,X),'$gv_rdl'(Cs,Xs).\n"
    "'$gv_uint'(n,_):-!.\n"
    "'$gv_uint'(y,X):-var(X),!.\n"
    "'$gv_uint'(y,X):-integer(X),!.\n"
    "'$gv_uint'(y,X):-'$gv_err'(type_error(integer,X)).\n"
    "'$gv_incdec'(N,D,O,W,Uo,Uw):-'$gv_uint'(Uo,O),'$gv_uint'(Uw,W),'$gv_tgt'(N,K,P,Ss),'$gv_get'(K,S),'$gv_at'(S,P,C),'$gv_intval'(C,Ss,I),I2 is I+D,(Uo==y->O=I;true),'$gv_intput'(C,Ss,I2,C2),'$gv_put'(S,P,C2,S2),nb_setval(K,S2),(Uw==y->W=I2;true).\n"
    "'$gv_bit'(B,_):-var(B),!,'$gv_err'(instantiation_error).\n"
    "'$gv_bit'(B,_):- \\+integer(B),!,'$gv_err'(type_error(integer,B)).\n"
    "'$gv_bit'(B,_):-B<0,!,'$gv_err'(domain_error(not_less_than_zero,B)).\n"
    "'$gv_bit'(B,M):-M is 1<<(B mod 61).\n"
    "'$gv_bitop'(N,Op,M):-'$gv_tgt'(N,K,P,Ss),'$gv_get'(K,S),'$gv_at'(S,P,C),'$gv_intval'(C,Ss,I),(Op==s->I1 is I\\/M;I1 is I/\\ \\M),I2 is ((I1+1152921504606846976) mod 2305843009213693952)-1152921504606846976,'$gv_intput'(C,Ss,I2,C2),'$gv_put'(S,P,C2,S2),nb_setval(K,S2).\n"
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
    "environ(N,_):-nonvar(N),\\+atom(N),!,throw(error(type_error(atom,N),environ/2)).\n"
    "environ(_,V):-nonvar(V),\\+atom(V),!,throw(error(type_error(atom,V),environ/2)).\n"
    "environ(N,V):-atom(N),!,'$gnu_environ_list'(L),memberchk(N=V0,L),V=V0.\n"
    "environ(N,V):-'$gnu_environ_list'(L),member(N=V,L).\n"
    "file_property(_,P):-nonvar(P),\\+'$gnu_fprop'(P),!,throw(error(domain_error(os_file_property,P),file_property/2)).\n"
    "file_property(F,P):-'$gnu_file_props'(F,L),member(P,L).\n"
    "'$gnu_fprop'(absolute_file_name(_)).\n"
    "'$gnu_fprop'(real_file_name(_)).\n"
    "'$gnu_fprop'(type(_)).\n"
    "'$gnu_fprop'(size(_)).\n"
    "'$gnu_fprop'(permission(_)).\n"
    "'$gnu_fprop'(creation(_)).\n"
    "'$gnu_fprop'(last_access(_)).\n"
    "'$gnu_fprop'(last_modification(_)).\n"
    "decompose_file_name(P,D,B,S):-atom_codes(P,Cs),('$gnu_last_sep'(Cs,0'/,DP,BC)->append(DP,[0'/],DC);DC=[],BC=Cs),"
        "('$gnu_last_sep'(BC,0'.,PC,SP)->SC=[0'.|SP];PC=BC,SC=[]),atom_codes(D,DC),atom_codes(B,PC),atom_codes(S,SC).\n"
    "'$gnu_last_sep'(Cs,Ch,Pre,Post):-reverse(Cs,R),'$gnu_upto'(R,Ch,RPost,RPre),reverse(RPost,Post),reverse(RPre,Pre).\n"
    "'$gnu_upto'([C|Cs],C,[],Cs):-!.\n"
    "'$gnu_upto'([C|Cs],Ch,[C|P],R):-'$gnu_upto'(Cs,Ch,P,R).\n"
    "last([X],X):- !.\n"
    "last([_|T],X):-last(T,X).\n"
    "argument_counter(N):-nonvar(N),\\+ integer(N),!,throw(error(type_error(integer,N),argument_counter/1)).\n"
    "argument_counter(N):-'$argv'(L),length(L,N).\n"
    "argument_list(L):-nonvar(L),L\\=[],L\\=[_|_],!,throw(error(type_error(list,L),argument_list/1)).\n"
    "argument_list(L):-'$argv'([_|L]).\n"
    "argument_value(I,_):-var(I),!,throw(error(instantiation_error,argument_value/2)).\n"
    "argument_value(I,_):- \\+ integer(I),!,throw(error(type_error(integer,I),argument_value/2)).\n"
    "argument_value(I,_):-I<0,!,throw(error(domain_error(not_less_than_zero,I),argument_value/2)).\n"
    "argument_value(_,A):-nonvar(A),\\+ atom(A),!,throw(error(type_error(atom,A),argument_value/2)).\n"
    "argument_value(I,A):-'$argv'(L),nth0(I,L,A0),!,A=A0.\n"
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
    "sort(K,O,L,S):-'$sort4_kp'(K,P),'$sort4_oc'(O),'$sort4_list'(L,L),'$sort4_keyed'(L,P,Ps),'$sort4_ord'(O,D,U),'$sort4_dir'(D,Ps,Q),'$sort4_out'(U,Q,S0),S=S0.\n"
    "'$sort4_kp'(K,_):-var(K),!,throw(error(instantiation_error,sort/4)).\n"
    "'$sort4_kp'(0,0):-!.\n"
    "'$sort4_kp'(K,_):-integer(K),K<0,!,throw(error(domain_error(not_less_than_one,K),sort/4)).\n"
    "'$sort4_kp'(K,[K]):-integer(K),!.\n"
    "'$sort4_kp'(K,[K]):-atom(K),!.\n"
    "'$sort4_kp'(K,K):-'$sort4_plist'(K),!,'$sort4_pathok'(K).\n"
    "'$sort4_kp'(K,_):-throw(error(type_error(sort_key,K),sort/4)).\n"
    "'$sort4_plist'(L):-var(L),!,fail.\n"
    "'$sort4_plist'([]):-!.\n"
    "'$sort4_plist'([_|T]):-'$sort4_plist'(T).\n"
    "'$sort4_pathok'([]).\n"
    "'$sort4_pathok'([I|R]):-'$sort4_idx'(I),'$sort4_pathok'(R).\n"
    "'$sort4_idx'(I):-integer(I),!,(I<1->throw(error(domain_error(not_less_than_one,I),sort/4));true).\n"
    "'$sort4_idx'(I):-atom(I).\n"
    "'$sort4_oc'(O):-var(O),!,throw(error(instantiation_error,sort/4)).\n"
    "'$sort4_oc'(O):- \\+atom(O),!,throw(error(type_error(atom,O),sort/4)).\n"
    "'$sort4_oc'(O):- \\+'$sort4_ord'(O,_,_),!,throw(error(domain_error(order,O),sort/4)).\n"
    "'$sort4_oc'(_).\n"
    "'$sort4_list'(L,_):-var(L),!,throw(error(instantiation_error,sort/4)).\n"
    "'$sort4_list'([],_):-!.\n"
    "'$sort4_list'([_|T],L0):-!,'$sort4_list'(T,L0).\n"
    "'$sort4_list'(_,L0):-throw(error(type_error(list,L0),sort/4)).\n"
    "'$sort4_ord'('@<',a,u).\n"
    "'$sort4_ord'('@=<',a,d).\n"
    "'$sort4_ord'('@>',d,u).\n"
    "'$sort4_ord'('@>=',d,d).\n"
    "'$sort4_keyed'([],_,[]).\n"
    "'$sort4_keyed'([E|T],P,[Y-E|R]):-'$sort4_key'(P,E,Y),'$sort4_keyed'(T,P,R).\n"
    "'$sort4_key'(0,E,E):-!.\n"
    "'$sort4_key'(P,E,Y):-'$sort4_cmp'(E),'$sort4_steps'(P,E,Y).\n"
    "'$sort4_cmp'(E):-var(E),!,throw(error(instantiation_error,sort/4)).\n"
    "'$sort4_cmp'(E):- \\+compound(E),!,throw(error(type_error(compound,E),sort/4)).\n"
    "'$sort4_cmp'(_).\n"
    "'$sort4_steps'([],E,E).\n"
    "'$sort4_steps'([I|R],E,Y):-'$sort4_step'(I,E,Z),'$sort4_more'(R,Z,Y).\n"
    "'$sort4_more'([],Z,Z):-!.\n"
    "'$sort4_more'(R,Z,Y):-'$sort4_cmp'(Z),'$sort4_steps'(R,Z,Y).\n"
    "'$sort4_step'(I,E,_):-atom(I),!,throw(error(type_error(dict,E),sort/4)).\n"
    "'$sort4_step'(I,E,_):-functor(E,_,A),A<I,!,throw(error(existence_error(argument,I,E),sort/4)).\n"
    "'$sort4_step'(I,E,Z):-arg(I,E,Z).\n"
    "'$sort4_dir'(a,P,Q):-keysort(P,Q).\n"
    "'$sort4_dir'(d,P,Q):-'$sort4_rev'(P,[],P1),keysort(P1,Q1),'$sort4_rev'(Q1,[],Q).\n"
    "'$sort4_rev'([],A,A).\n"
    "'$sort4_rev'([X|T],A,R):-'$sort4_rev'(T,[X|A],R).\n"
    "'$sort4_out'(d,[],[]).\n"
    "'$sort4_out'(d,[_-E|T],[E|R]):-'$sort4_out'(d,T,R).\n"
    "'$sort4_out'(u,[],[]).\n"
    "'$sort4_out'(u,[Y-E|T],[E|R]):-'$sort4_skip'(T,Y,T1),'$sort4_out'(u,T1,R).\n"
    "'$sort4_skip'([Y1-_|T],Y,T1):-Y1==Y,!,'$sort4_skip'(T,Y,T1).\n"
    "'$sort4_skip'(T,_,T).\n"
    "debug(T,F,A):-'$dbg_on'(T),!,format(user_error,'% ',[]),format(user_error,F,A),nl(user_error).\n"
    "debug(_,_,_).\n"
    "debug(T):-'$dbg_topics'(L),nb_setval('$dbg_topics',[T|L]).\n"
    "nodebug(T):-'$dbg_topics'(L),'$dbg_rm'(L,T,L1),nb_setval('$dbg_topics',L1).\n"
    "nodebug.\n"
    "debugging(T):-'$dbg_on'(T).\n"
    "'$dbg_on'(T):-'$dbg_topics'(L),member(X,L),\\+ \\+ X=T,!.\n"
    "'$dbg_topics'(L):-catch(nb_getval('$dbg_topics',L),error(existence_error(_,_),_),L=[]).\n"
    "'$dbg_rm'([],_,[]).\n"
    "'$dbg_rm'([X|Xs],T,R):-('$dbg_variant'(X,T)->R=R1;R=[X|R1]),'$dbg_rm'(Xs,T,R1).\n"
    "'$dbg_variant'(A,B):-copy_term(A,A1),numbervars(A1,0,_),copy_term(B,B1),numbervars(B1,0,_),A1==B1.\n"
    "':'(M,_):-var(M),!,throw(error(instantiation_error,(:)/2)).\n"
    "':'(_,G):-call(G).\n"
    "':'(_,G,A):-call(G,A).\n"
    "':'(_,G,A,B):-call(G,A,B).\n"
    "':'(_,G,A,B,C):-call(G,A,B,C).\n"
    "':'(_,G,A,B,C,D):-call(G,A,B,C,D).\n"
    "':'(_,G,A,B,C,D,E):-call(G,A,B,C,D,E).\n"
    "':'(_,G,A,B,C,D,E,F):-call(G,A,B,C,D,E,F).\n"
    "':'(_,G,A,B,C,D,E,F,H):-call(G,A,B,C,D,E,F,H).\n"
    "'$tbl_call'(G,I):-'$tbl_key'(G,Key),'$tbl_get'(Key,T),'$tbl_enter'(T,Key,G,I),'$tbl_get'(Key,t(S,As,_)),(S==complete->true;nb_setval('$tbl_dep',true)),"
        "'$tbl_answer'(As,G).\n"
    "'$tbl_key'(G,Key):-'$tbl_vkey'(G,K),term_to_atom(K,A),atom_concat('$tbl:',A,Key).\n"
    "'$tbl_vkey'(G,K):-copy_term(G,K),numbervars(K,0,_).\n"
    "'$tbl_get'(Key,T):-catch(nb_getval(Key,T),error(existence_error(_,_),_),T=none).\n"
    "'$tbl_cell'(Key,D,V):-catch(nb_getval(Key,V),error(existence_error(_,_),_),V=D).\n"
    "'$tbl_enter'(none,Key,G,I):-!,copy_term(G-I,P),nb_setval(Key,t(inprog,[],P)),'$tbl_inprog'(Ks),nb_setval('$tbl_inprog',[Key|Ks]),'$tbl_all'(As),"
        "nb_setval('$tbl_all',[Key|As]),('$tbl_leading'->'$tbl_eval1'(Key);'$tbl_lead'(Key)).\n"
    "'$tbl_enter'(_,_,_,_).\n"
    "'$tbl_leading':-'$tbl_cell'('$tbl_leader',false,L),L==true.\n"
    "'$tbl_inprog'(Ks):-'$tbl_cell'('$tbl_inprog',[],Ks).\n"
    "'$tbl_all'(Ks):-'$tbl_cell'('$tbl_all',[],Ks).\n"
    "'$tbl_lead'(Key):-nb_setval('$tbl_leader',true),catch(('$tbl_eval1'(Key),'$tbl_rounds'),E,('$tbl_abort',throw(E))),'$tbl_inprog'(Ks),'$tbl_complete'(Ks),"
        "nb_setval('$tbl_inprog',[]),nb_setval('$tbl_leader',false).\n"
    "'$tbl_eval1'(Key):-'$tbl_cell'('$tbl_dep',false,D0),nb_setval('$tbl_dep',false),'$tbl_eval'(Key,_),'$tbl_cell'('$tbl_dep',false,D),"
        "(D==true->true;'$tbl_done'(Key)),nb_setval('$tbl_dep',D0).\n"
    "'$tbl_done'(Key):-nb_getval(Key,t(_,As,P)),nb_setval(Key,t(complete,As,P)),'$tbl_inprog'(Ks),'$tbl_del'(Ks,Key,Ks1),nb_setval('$tbl_inprog',Ks1).\n"
    "'$tbl_del'([],_,[]).\n"
    "'$tbl_del'([K|T],Key,R):-(K==Key->R=T;R=[K|R1],'$tbl_del'(T,Key,R1)).\n"
    "'$tbl_rounds':-'$tbl_inprog'(Ks0),(Ks0==[]->true;length(Ks0,N0),'$tbl_round'(Ks0,0,C),'$tbl_inprog'(Ks1),length(Ks1,N1),(C+N1-N0>0->'$tbl_rounds';true)).\n"
    "'$tbl_round'([],C,C).\n"
    "'$tbl_round'([K|T],C0,C):-'$tbl_eval'(K,N),C1 is C0+N,'$tbl_round'(T,C1,C).\n"
    "'$tbl_eval'(Key,N):-nb_getval(Key,t(_,_,P)),copy_term(P,G-I),findall(G,I,Fs),nb_getval(Key,t(S,As0,P1)),'$tbl_add'(Fs,As0,As,0,N),"
        "(N>0->nb_setval(Key,t(S,As,P1));true).\n"
    "'$tbl_add'([],As,As,N,N).\n"
    "'$tbl_add'([F|T],As0,As,N0,N):-'$tbl_vkey'(F,FK),(memberchk(FK-_,As0)->'$tbl_add'(T,As0,As,N0,N);"
        "N1 is N0+1,'$tbl_app'(As0,FK-F,As1),'$tbl_add'(T,As1,As,N1,N)).\n"
    "'$tbl_app'([],X,[X]).\n"
    "'$tbl_app'([H|T],X,[H|R]):-'$tbl_app'(T,X,R).\n"
    "'$tbl_answer'(As,G):-member(_-A,As),copy_term(A,G0),G=G0.\n"
    "'$tbl_complete'([]).\n"
    "'$tbl_complete'([K|T]):-nb_getval(K,t(_,As,P)),nb_setval(K,t(complete,As,P)),'$tbl_complete'(T).\n"
    "'$tbl_abort':-'$tbl_inprog'(Ks),'$tbl_drop'(Ks),nb_setval('$tbl_inprog',[]),nb_setval('$tbl_leader',false),nb_setval('$tbl_dep',false).\n"
    "abolish_all_tables:-'$tbl_leading',!,throw(error(permission_error(abolish,table,all),abolish_all_tables/0)).\n"
    "abolish_all_tables:-'$tbl_all'(Ks),'$tbl_drop'(Ks),nb_setval('$tbl_all',[]).\n"
    "'$tbl_drop'([]).\n"
    "'$tbl_drop'([K|T]):-nb_setval(K,none),'$tbl_drop'(T).\n"
    "predsort(P,L,S):-length(L,N),'$predsort_'(P,N,L,_,S1),!,S=S1.\n"
    "'$predsort_'(P,2,[X1,X2|L],L,R):-!,call(P,D,X1,X2),'$predsort_2'(D,X1,X2,R).\n"
    "'$predsort_'(_,1,[X|L],L,[X]):-!.\n"
    "'$predsort_'(_,0,L,L,[]):-!.\n"
    "'$predsort_'(P,N,L1,L3,R):-N1 is N//2,N2 is N-N1,'$predsort_'(P,N1,L1,L2,R1),'$predsort_'(P,N2,L2,L3,R2),'$predmerge_'(P,R1,R2,R).\n"
    "'$predsort_2'(<,X1,X2,[X1,X2]).\n"
    "'$predsort_2'(=,X1,_,[X1]).\n"
    "'$predsort_2'(>,X1,X2,[X2,X1]).\n"
    "'$predmerge_'(_,[],R,R):-!.\n"
    "'$predmerge_'(_,R,[],R):-!.\n"
    "'$predmerge_'(P,[H1|T1],[H2|T2],R):-call(P,D,H1,H2),!,'$predmerge_d'(D,P,H1,H2,T1,T2,R).\n"
    "'$predmerge_d'(<,P,H1,H2,T1,T2,[H1|R]):-'$predmerge_'(P,T1,[H2|T2],R).\n"
    "'$predmerge_d'(=,P,H1,_,T1,T2,[H1|R]):-'$predmerge_'(P,T1,T2,R).\n"
    "'$predmerge_d'(>,P,H1,H2,T1,T2,[H2|R]):-'$predmerge_'(P,[H1|T1],T2,R).\n"
    "read_line_to_string(S,L):-get_code(S,C),'$rl_first'(C,S,L).\n"
    "'$rl_first'(-1,_,end_of_file):-!.\n"
    "'$rl_first'(C,S,L):-'$rl_codes'(C,S,Cs),string_codes(L,Cs).\n"
    "'$rl_codes'(-1,_,[]):-!.\n"
    "'$rl_codes'(10,_,[]):-!.\n"
    "'$rl_codes'(13,S,[]):-peek_code(S,10),!,get_code(S,_).\n"
    "'$rl_codes'(C,S,[C|Cs]):-get_code(S,C1),'$rl_codes'(C1,S,Cs).\n"
    "'$phrase'(G,_,_):-var(G),!,throw(error(instantiation_error,_)).\n"
    "'$phrase'(G,S0,S):-'$dcg_body'(G,S0,S,Goal),call(Goal).\n"
    "'$dcg_body'(V,S0,S,phrase(V,S0,S)):-var(V),!.\n"
    "'$dcg_body'([],S0,S,S0=S):-!.\n"
    "'$dcg_body'([H|T],S0,S,S0=L):-!,'$dcg_list'([H|T],S,L).\n"
    "'$dcg_body'((A,B),S0,S,(GA,GB)):-!,'$dcg_body'(A,S0,S1,GA),'$dcg_body'(B,S1,S,GB).\n"
    "'$dcg_body'((C->T;E),S0,S,(GC->GT;GE)):-!,'$dcg_body'(C,S0,S1,GC),'$dcg_body'(T,S1,S,GT),'$dcg_body'(E,S0,S,GE).\n"
    "'$dcg_body'((C*->T;E),S0,S,(GC*->GT;GE)):-!,'$dcg_body'(C,S0,S1,GC),'$dcg_body'(T,S1,S,GT),'$dcg_body'(E,S0,S,GE).\n"
    "'$dcg_body'((A;B),S0,S,(GA;GB)):-!,'$dcg_body'(A,S0,S,GA),'$dcg_body'(B,S0,S,GB).\n"
    "'$dcg_body'((C->T),S0,S,(GC->GT)):-!,'$dcg_body'(C,S0,S1,GC),'$dcg_body'(T,S1,S,GT).\n"
    "'$dcg_body'(\\+A,S0,S,(\\+GA,S0=S)):-!,'$dcg_body'(A,S0,_,GA).\n"
    "'$dcg_body'({}(G),S0,S,(G,S0=S)):-!.\n"
    "'$dcg_body'(!,S0,S,(!,S0=S)):-!.\n"
    "'$dcg_body'(NT,S0,S,G):-NT=..L0,'$dcg_list'(L0,[S0,S],L1),G=..L1.\n"
    "'$dcg_list'([],S,S).\n"
    "'$dcg_list'([H|T],S,[H|R]):-'$dcg_list'(T,S,R).\n"
    "code_type(C,T):-integer(C),!,C>=0,char_code(Ch,C),'$code_type'(Ch,T).\n"
    "code_type(C,T):-char_type(C,T).\n"
    "'$code_type'(Ch,to_lower(X)):-!,(integer(X)->char_code(Y,X);true),char_type(Ch,to_lower(Y)),char_code(Y,X).\n"
    "'$code_type'(Ch,to_upper(X)):-!,(integer(X)->char_code(Y,X);true),char_type(Ch,to_upper(Y)),char_code(Y,X).\n"
    "'$code_type'(Ch,upper(X)):-!,char_type(Ch,upper(Y)),char_code(Y,X).\n"
    "'$code_type'(Ch,lower(X)):-!,char_type(Ch,lower(Y)),char_code(Y,X).\n"
    "'$code_type'(Ch,T):-char_type(Ch,T).\n"
    "select(X,[X|T],T).\n"
    "select(X,[H|T],[H|R]):-select(X,T,R).\n"
    "nth(N,L,E):-nth1(N,L,E).\n"
    "delete([],_,[]).\n"
    "delete([H|T],X,R):-(\\+ H\\=X->R=R1;R=[H|R1]),delete(T,X,R1).\n"
    "permutation(Xs,Ys):-'$skip_list'(Xl,Xs,Xt),'$skip_list'(Yl,Ys,Yt),(Xt==[],Yt==[]->Xl==Yl;var(Xt),Yt==[]->length(Xs,Yl);Xt==[],var(Yt)->length(Ys,Xl);var(Xt),var(Yt)->length(Xs,N),length(Ys,N);'$must_be_list_'(Xs),'$must_be_list_'(Ys)),'$perm_'(Xs,Ys).\n"
    "'$must_be_list_'(L):-'$skip_list'(_,L,T),(T==[]->true;var(T)->throw(error(instantiation_error,_));throw(error(type_error(list,L),_))).\n"
    "'$perm_'([],[]).\n"
    "'$perm_'(L,[H|T]):-select(H,L,R),'$perm_'(R,T).\n"
    "prefix([],_).\n"
    "prefix([X|T],[X|T1]):-prefix(T,T1).\n"
    "suffix(L,L).\n"
    "suffix(X,[_|T]):-suffix(X,T).\n"
    "sublist(L,L).\n"
    "sublist(S,[H|T]):-'$sublist1_'(T,H,S).\n"
    "'$sublist1_'(S,_,S).\n"
    "'$sublist1_'([H|T],_,S):-'$sublist1_'(T,H,S).\n"
    "'$sublist1_'([H|T],X,[X|S]):-'$sublist1_'(T,H,S).\n"
    "flatten(L,F):-'$flatten_'(L,[],F0),!,F=F0.\n"
    "'$flatten_'(V,T,[V|T]):-var(V),!.\n"
    "'$flatten_'([],T,T):-!.\n"
    "'$flatten_'([H|T],Tl,L):-!,'$flatten_'(H,FT,L),'$flatten_'(T,Tl,FT).\n"
    "'$flatten_'(X,T,[X|T]).\n"
    "term_string(T,S,_):-var(S),!,term_string(T,S).\n"
    "term_string(T,S,O):-read_term_from_atom(S,T,O).\n"
    "garbage_collect.\n"
    "wildcard_match(P,S):-wildcard_match(P,S,[]).\n"
    "wildcard_match(P,S,O):-'$wc_ign'(O,I),'$wc_codes'(P,Pc),'$wc_codes'(S,Sc0),'$wc_comp'(Pc,I,top,Ops,_),'$wc_lowl'(I,Sc0,Sc),('$wc_m'(Ops,Sc)->true).\n"
    "'$wc_ign'(O,I):-(memberchk(case_sensitive(B),O)->(B==false->I=1;I=0);I=0).\n"
    "'$wc_codes'(T,_):-var(T),!,throw(error(instantiation_error,_)).\n"
    "'$wc_codes'([],[]):-!.\n"
    "'$wc_codes'([H|T],Cs):-!,'$wc_lcodes'([H|T],Cs).\n"
    "'$wc_codes'(T,Cs):-atom(T),!,atom_codes(T,Cs).\n"
    "'$wc_codes'(T,Cs):-string(T),!,string_codes(T,Cs).\n"
    "'$wc_codes'(T,Cs):-number(T),!,number_codes(T,Cs).\n"
    "'$wc_codes'(T,_):-throw(error(type_error(text,T),_)).\n"
    "'$wc_lcodes'([],[]):-!.\n"
    "'$wc_lcodes'([H|T],[C|Cs]):-!,'$wc_code'(H,C),'$wc_lcodes'(T,Cs).\n"
    "'$wc_lcodes'(L,_):-throw(error(type_error(text,L),_)).\n"
    "'$wc_code'(H,_):-var(H),!,throw(error(instantiation_error,_)).\n"
    "'$wc_code'(H,H):-integer(H),!,(H>=0,H=<1114111->true;throw(error(type_error(character_code,H),_))).\n"
    "'$wc_code'(H,C):-atom(H),atom_length(H,1),!,char_code(H,C).\n"
    "'$wc_code'(H,_):-throw(error(type_error(character_code,H),_)).\n"
    "'$wc_low'(0,C,C):-!.\n"
    "'$wc_low'(_,C,L):-char_code(Ch,C),downcase_atom(Ch,Lc),char_code(Lc,L).\n"
    "'$wc_lowl'(0,L,L):-!.\n"
    "'$wc_lowl'(_,[],[]).\n"
    "'$wc_lowl'(I,[C|T],[L|U]):-'$wc_low'(I,C,L),'$wc_lowl'(I,T,U).\n"
    "'$wc_comp'([],_,_,[],[]):-!.\n"
    "'$wc_comp'([92],_,_,[c(92)],[]):-!.\n"
    "'$wc_comp'([92,C|T],I,M,[c(C)|O],R):-!,'$wc_comp'(T,I,M,O,R).\n"
    "'$wc_comp'([63|T],I,M,[any|O],R):-!,'$wc_comp'(T,I,M,O,R).\n"
    "'$wc_comp'([42|T],I,M,[star|O],R):-!,'$wc_comp'(T,I,M,O,R).\n"
    "'$wc_comp'([91|T],I,M,[set(S)|O],R):-!,'$wc_set'(T,S,T1),'$wc_comp'(T1,I,M,O,R).\n"
    "'$wc_comp'([123|T],I,M,[alt(A)|O],R):-!,'$wc_alts'(T,I,A,T1),'$wc_comp'(T1,I,M,O,R).\n"
    "'$wc_comp'([C|T],_,curl,[],[C|T]):-(C==125;C==44),!.\n"
    "'$wc_comp'([C|T],I,M,[c(L)|O],R):-'$wc_low'(I,C,L),'$wc_comp'(T,I,M,O,R).\n"
    "'$wc_set'([],_,_):-!,throw(error(syntax_error('Unmatched ''['''),_)).\n"
    "'$wc_set'([92],_,_):-!,throw(error(syntax_error('Unmatched ''['''),_)).\n"
    "'$wc_set'([92,C|T],[C|S],R):-!,'$wc_set'(T,S,R).\n"
    "'$wc_set'([93|T],[],T):-!.\n"
    "'$wc_set'([C,45,E|T],[C-E|S],R):-E=\\=93,!,'$wc_set'(T,S,R).\n"
    "'$wc_set'([C|T],[C|S],R):-'$wc_set'(T,S,R).\n"
    "'$wc_alts'(T,I,[A|As],R):-'$wc_comp'(T,I,curl,A,T1),'$wc_alt_more'(T1,I,As,R).\n"
    "'$wc_alt_more'([44|T],I,As,R):-!,'$wc_alts'(T,I,As,R).\n"
    "'$wc_alt_more'([125|T],_,[],T):-!.\n"
    "'$wc_alt_more'(_,_,_,_):-throw(error(syntax_error('Unmatched ''{'''),_)).\n"
    "'$wc_m'([],[]).\n"
    "'$wc_m'([c(C)|O],[C|T]):-'$wc_m'(O,T).\n"
    "'$wc_m'([any|O],[_|T]):-'$wc_m'(O,T).\n"
    "'$wc_m'([star|O],S):-'$wc_m'(O,S).\n"
    "'$wc_m'([star|O],[_|T]):-'$wc_m'([star|O],T).\n"
    "'$wc_m'([set(I)|O],[C|T]):-'$wc_in'(I,C),!,'$wc_m'(O,T).\n"
    "'$wc_m'([alt(As)|O],S):-member(A,As),append(A,O,AO),'$wc_m'(AO,S).\n"
    "'$wc_in'([X|_],C):-(X=L-H->C>=L,C=<H;X==C),!.\n"
    "'$wc_in'([_|T],C):-'$wc_in'(T,C).\n"
    "'$bagof_var'(T,G,R):-'$bag_goal_ok'(G),'$pl_list_guard'(R),'$bag_wit'(T,G,W,G1),(W=='$w'->findall(T,G1,R0),R0\\==[],R=R0;findall(W-T,G1,Ps),Ps\\==[],'$bag_pick'(Ps,W,R)).\n"
    "'$setof_var'(T,G,R):-'$bag_goal_ok'(G),'$pl_list_guard'(R),'$bag_wit'(T,G,W,G1),(W=='$w'->findall(T,G1,R0),R0\\==[],sort(R0,R);findall(W-T,G1,Ps0),Ps0\\==[],keysort(Ps0,Ps),'$bag_pick'(Ps,W,R1),sort(R1,R)).\n"
    "'$bag_wit'(T,G,W,G1):-'$bag_strip'(G,[T],Bs,G1),term_variables(Bs,BV),term_variables(G1,GV),'$bag_sub'(GV,BV,FV),W=..['$w'|FV].\n"
    "'$bag_goal_ok'(G):-'$bag_strip'(G,[],_,G0),(var(G0)->throw(error(instantiation_error,_));callable(G0)->true;throw(error(type_error(callable,G0),_))).\n"
    "'$bag_strip'(G,B,B,G):-var(G),!.\n"
    "'$bag_strip'(V^G,B0,B,G1):-!,'$bag_strip'(G,[V|B0],B,G1).\n"
    "'$bag_strip'(G,B,B,G).\n"
    "'$bag_sub'([],_,[]).\n"
    "'$bag_sub'([V|Vs],B,R):-('$bag_memq'(V,B)->R=R1;R=[V|R1]),'$bag_sub'(Vs,B,R1).\n"
    "'$bag_memq'(V,[W|Ws]):-(V==W->true;'$bag_memq'(V,Ws)).\n"
    "'$bag_pick'([W0-T0|Ps],W,R):-'$bag_vars'(Ps,W0,Ts,Rest),(W=W0,R=[T0|Ts];Rest\\==[],'$bag_pick'(Rest,W,R)).\n"
    "'$bag_vars'([],_,[],[]).\n"
    "'$bag_vars'([W1-T1|Ps],W0,Ts,Rest):-('$bag_variant'(W1,W0)->W1=W0,Ts=[T1|Ts1],Rest=Rest1;Ts=Ts1,Rest=[W1-T1|Rest1]),'$bag_vars'(Ps,W0,Ts1,Rest1).\n"
    "'$bag_variant'(A,B):- \\+ \\+ (copy_term(A,A1),copy_term(B,B1),numbervars(A1,0,N),numbervars(B1,0,N),A1==B1).\n"
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
    "'$emit_list'([H|T],Templ,STempl,OnElem,OnSep)-->{copy_term(t(Templ,STempl,OnElem,OnSep),t(H,STempl,OnElemC,OnSepC))},phrase(OnElemC),({T==[]}->[];phrase(OnSepC),'$emit_list'(T,Templ,STempl,OnElem,OnSep)).\n"
    "consult(F):-'$consult_path'(F,P),open(P,read,S),catch('$consult_loop'(S,Is),E,(close(S),throw(E))),close(S),'$consult_inits'(Is).\n"
    "'$consult_path'(F,_):-var(F),!,throw(error(instantiation_error,consult/1)).\n"
    "'$consult_path'(F,F):-atom(F),file_exists(F),!.\n"
    "'$consult_path'(F,P):-atom(F),atom_concat(F,'.pl',P),file_exists(P),!.\n"
    "'$consult_path'(F,_):-throw(error(existence_error(source_sink,F),consult/1)).\n"
    "'$consult_loop'(S,Is):-read_term(S,T,[]),(T==end_of_file->Is=[];'$consult_term'(T,Is,R),'$consult_loop'(S,R)).\n"
    "'$consult_term'((:-initialization(G)),[G|R],R):-!.\n"
    "'$consult_term'((:-multifile(_)),R,R):-!.\n"
    "'$consult_term'((:-discontiguous(_)),R,R):-!.\n"
    "'$consult_term'((:-D),R,R):-!,'$consult_goal'(D).\n"
    "'$consult_term'((H-->B),R,R):-!,dcg_translate_rule((H-->B),C),assertz(C).\n"
    "'$consult_term'(C,R,R):-assertz(C).\n"
    "'$consult_inits'([]).\n"
    "'$consult_inits'([G|Gs]):-'$consult_goal'(G),'$consult_inits'(Gs).\n"
    "'$consult_goal'(G):-(catch(G,E,(format(user_error,'Warning: directive raised: ~q~n',[E]),true))->true;format(user_error,'Warning: directive failed: ~q~n',[G])).\n";
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
    if (pl_cv_has(&referenced, "phrase")) pl_cv_add(&referenced, "$phrase");
    if (pl_cv_has(&referenced, "table")) pl_cv_add(&referenced, "$tbl_call");
    if (pl_word_referenced(user_src, "bagof") || pl_word_referenced(user_src, "setof")) { pl_cv_add(&referenced, "$bagof_var"); pl_cv_add(&referenced, "$setof_var"); }
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
    p.prec     = 0;
    p.incl_depth = 0;
    { extern int rt_pl_iso_mode(void); p.iso = rt_pl_iso_mode(); p.lx.iso = p.iso; }
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
