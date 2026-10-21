#include "prolog_parse.h"
#include "parsers/g_parser.h"
#include "ct_arena.h"
#include "ct_vec.h"
#include "prolog_lex.h"
#include "rt/prolog_atom.h"
#include "stage2.h"
#include "pl_aggregate_names.h"
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
    int         in_list;
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
typedef struct pl_op_entry { const char *name; int prec; Assoc assoc; Fixity fixity; } OpEntry;
static const OpEntry BIN_OPS[] = {
    { ":-",   1200, ASSOC_NONE  },
    { "-->",  1200, ASSOC_NONE  },
    { "=>",   1200, ASSOC_NONE  },
    { ",",    1000, ASSOC_RIGHT },
    { ";",    1100, ASSOC_RIGHT },
    { "|",    1105, ASSOC_RIGHT },
    { "|",    1105, ASSOC_RIGHT },
    { "->",   1050, ASSOC_RIGHT },
    { "*->",  1050, ASSOC_RIGHT },
    { ":=",    990, ASSOC_NONE  },
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
    { ":<",    700, ASSOC_NONE  },
    { ">:<",   700, ASSOC_NONE  },
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
    { "#<=>",  750, ASSOC_RIGHT },
    { "#\\<=>", 750, ASSOC_RIGHT },
    { "#==>",  740, ASSOC_RIGHT },
    { "#\\==>", 740, ASSOC_RIGHT },
    { "##",    730, ASSOC_RIGHT },
    { "#\\/",   730, ASSOC_LEFT  },
    { "#\\\\/",  730, ASSOC_LEFT  },
    { "#/\\",   720, ASSOC_LEFT  },
    { "#\\/\\",  720, ASSOC_LEFT  },
    { "#=",    700, ASSOC_NONE  },
    { "#\\=",   700, ASSOC_NONE  },
    { "#<",    700, ASSOC_NONE  },
    { "#=<",   700, ASSOC_NONE  },
    { "#>",    700, ASSOC_NONE  },
    { "#>=",   700, ASSOC_NONE  },
    { "#=#",   700, ASSOC_NONE  },
    { "#\\=#",  700, ASSOC_NONE  },
    { "#<#",   700, ASSOC_NONE  },
    { "#=<#",  700, ASSOC_NONE  },
    { "#>#",   700, ASSOC_NONE  },
    { "#>=#",  700, ASSOC_NONE  },
    { NULL,    0,   ASSOC_NONE  }
};
static const OpEntry PREFIX_OPS[] = {
    { ":-",   1200, ASSOC_NONE,  FIX_PREFIX },
    { "?-",   1200, ASSOC_NONE,  FIX_PREFIX },
    { "\\+",   900, ASSOC_RIGHT, FIX_PREFIX },
    { "-",     200, ASSOC_RIGHT, FIX_PREFIX },
    { "+",     200, ASSOC_RIGHT, FIX_PREFIX },
    { "\\",    200, ASSOC_RIGHT, FIX_PREFIX },
    { "#\\",   710, ASSOC_RIGHT, FIX_PREFIX },
    { "$",       1, ASSOC_NONE,  FIX_PREFIX },
    { NULL,    0,   ASSOC_NONE,  FIX_INFIX  }
};
static const char *op_type_unclassify(Assoc assoc, Fixity fix);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void user_op_add(const char *name, int prec, Assoc assoc, Fixity fixity) {
    if (prolog_op_cols_ready()) prolog_op_col_set(prolog_atom_intern(name), prec, op_type_unclassify(assoc, fixity));
    for (int i = 0; i < g_parser.pl.uinfix_n; i++) if (g_parser.pl.uinfix[i].fixity == fixity && strcmp(g_parser.pl.uinfix[i].name, name) == 0) { g_parser.pl.uinfix[i].prec = prec; g_parser.pl.uinfix[i].assoc = assoc; return; }
    if (g_parser.pl.uinfix_n >= g_parser.pl.uinfix_cap) { g_parser.pl.uinfix_cap = g_parser.pl.uinfix_cap ? g_parser.pl.uinfix_cap * 2 : 8; g_parser.pl.uinfix = (OpEntry *)ct_grow(g_parser.pl.uinfix, g_parser.pl.uinfix_cap * sizeof(OpEntry)); }
    g_parser.pl.uinfix[g_parser.pl.uinfix_n].name = ct_strdup(name); g_parser.pl.uinfix[g_parser.pl.uinfix_n].prec = prec; g_parser.pl.uinfix[g_parser.pl.uinfix_n].assoc = assoc; g_parser.pl.uinfix[g_parser.pl.uinfix_n].fixity = fixity; g_parser.pl.uinfix_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const OpEntry *find_user(const char *name, Fixity fix) {
    for (int i = 0; i < g_parser.pl.uinfix_n; i++) if (g_parser.pl.uinfix[i].fixity == fix && strcmp(g_parser.pl.uinfix[i].name, name) == 0) return &g_parser.pl.uinfix[i];
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
static const OpEntry *find_prefix(const char *name) {
    const OpEntry *u = find_user(name, FIX_PREFIX);
    if (u) return u->prec > 0 ? u : NULL;
    for (const OpEntry *op = PREFIX_OPS; op->name; op++) if (strcmp(op->name, name) == 0 && (op->name[0] == '#' || op->name[0] == '$')) return op;
    return NULL;
}
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
int prolog_op_table_count(void) { return bin_ops_count() + prefix_ops_count() + g_parser.pl.uinfix_n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_table_get(int idx, const char **name_out, int *prec_out, const char **type_out) {
    int nbin = bin_ops_count(), npre = prefix_ops_count();
    const OpEntry *e;
    if (idx < 0 || idx >= nbin + npre + g_parser.pl.uinfix_n) return 0;
    e = (idx < nbin) ? &BIN_OPS[idx] : (idx < nbin + npre) ? &PREFIX_OPS[idx - nbin] : &g_parser.pl.uinfix[idx - nbin - npre];
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
int prolog_op_user_count(void) { return g_parser.pl.uinfix_n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int prolog_op_user_get(int i, const char **name_out, int *prec_out, const char **type_out) {
    if (i < 0 || i >= g_parser.pl.uinfix_n) return 0;
    if (name_out) *name_out = g_parser.pl.uinfix[i].name;
    if (prec_out) *prec_out = g_parser.pl.uinfix[i].prec;
    if (type_out) *type_out = op_type_unclassify(g_parser.pl.uinfix[i].assoc, g_parser.pl.uinfix[i].fixity);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void register_op_one(int prec, const char *type, tree_t *namenode, int swi) {
    if (!namenode) return;
    if (namenode->t == TT_MAKELIST) { for (int i = 0; i < namenode->n; i++) register_op_one(prec, type, namenode->c[i], swi); return; }
    int blk = swi && namenode->t == TT_FNC && namenode->n == 0 && namenode->v.sval && !strcmp(namenode->v.sval, "{}");
    if ((namenode->t != TT_QLIT && !blk) || !namenode->v.sval || prec < 0 || prec > 1200 || (!blk && prolog_op_permission(namenode->v.sval, prec, type))) return;
    Assoc assoc; Fixity fix;
    if (op_type_classify(type, &assoc, &fix)) user_op_add(namenode->v.sval, prec, assoc, fix);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "prolog_prelude_libs.inc"
static void register_lib_ops(const char *lib) {
    for (int i = 0; PL_PRELUDE_LIBS[i].lib; i++) {
        if (strcmp(PL_PRELUDE_LIBS[i].lib, lib)) continue;
        for (const pl_prelude_lib_op_t *o = PL_PRELUDE_LIBS[i].ops; o && o->nm; o++) {
            Assoc assoc; Fixity fix;
            if (o->pri < 0 || o->pri > 1200 || prolog_op_permission(o->nm, o->pri, o->type)) continue;
            if (op_type_classify(o->type, &assoc, &fix)) user_op_add(o->nm, o->pri, assoc, fix);
        }
        return;
    }
}
static void register_op_directive(tree_t *goal, int swi) {
    if (!goal || goal->t != TT_FNC || !goal->v.sval) return;
    if (strcmp(goal->v.sval, ",") == 0 && goal->n == 2) { register_op_directive(goal->c[0], swi); register_op_directive(goal->c[1], swi); return; }
    if (strcmp(goal->v.sval, "module") == 0 && goal->n == 2) {
        tree_t *exports = goal->c[1];
        if (exports && exports->t == TT_MAKELIST)
            for (int i = 0; i < exports->n; i++) register_op_directive(exports->c[i], swi);
        return;
    }
    if (!strcmp(goal->v.sval, "use_module") && goal->n >= 1 && goal->c[0] && goal->c[0]->t == TT_FNC && goal->c[0]->v.sval && !strcmp(goal->c[0]->v.sval, "library") && goal->c[0]->n == 1 && goal->c[0]->c[0]
        && (goal->c[0]->c[0]->t == TT_QLIT || goal->c[0]->c[0]->t == TT_NAME) && goal->c[0]->c[0]->v.sval) { register_lib_ops(goal->c[0]->c[0]->v.sval); return; }
    if (strcmp(goal->v.sval, "op") != 0 || goal->n != 3) return;
    tree_t *pn = goal->c[0], *tn = goal->c[1], *nn = goal->c[2];
    if (!pn || pn->t != TT_ILIT || !tn || tn->t != TT_QLIT || !tn->v.sval) return;
    register_op_one((int)pn->v.ival, tn->v.sval, nn, swi);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lexer_functor_follows(Lexer *lx) {
    Lexer saved = *lx;
    lexer_next(lx);
    Token b = lexer_peek(lx);
    int r = b.kind == TK_LPAREN && b.adj;
    *lx = saved;
    return r;
}
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
    tree_t *e = ast_node_new(TT_QLIT);
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
static tree_t *mk_raw(int fid, tree_t **args, int arity);
static int tls_control(const tree_t *t) {
    const char *f = t->v.sval;
    return f && ((t->n == 2 && (!strcmp(f, ",") || !strcmp(f, ";") || !strcmp(f, "|") || !strcmp(f, "->") || !strcmp(f, "*->"))) || (t->n == 1 && !strcmp(f, "\\+")));
}
static tree_t *tls(tree_t *t, int goal) {
    if (!t) return mk_atom(ATOM_NIL);
    switch (t->t) {
        case TT_VAR: case TT_ILIT: case TT_FLIT: case TT_CUT: case TT_DQLIT:
            return t;
        case TT_QLIT:
            return pt_stamp(mk_atom(prolog_atom_intern(t->v.sval ? t->v.sval : "")), t->line);
        case TT_MAKELIST: {
            int nelem = t->v.ival ? t->n - 1 : t->n;
            tree_t *result = t->v.ival ? tls(t->c[t->n - 1], 0) : mk_atom(ATOM_NIL);
            for (int i = nelem - 1; i >= 0; i--) {
                tree_t *dargs[2] = { tls(t->c[i], 0), result };
                result = mk_raw(ATOM_DOT, dargs, 2);
            }
            return pt_stamp(result, t->line);
        }
        default: {
            int fid = prolog_atom_intern(t->v.sval ? t->v.sval : "");
            int arity = t->n;
            tree_t **args = arity > 0 ? (tree_t **)rt_wsb_alloc((size_t)arity * sizeof(tree_t *)) : NULL;
            for (int i = 0; i < arity; i++) args[i] = tls(t->c[i], goal && tls_control(t));
            return pt_stamp(goal ? mk_call(fid, args, arity) : mk_raw(fid, args, arity), t->line);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rls(tree_t *t) {
    if (!t) return mk_atom(ATOM_NIL);
    switch (t->t) {
        case TT_VAR:
            if (t->v.sval && strcmp(t->v.sval, "_") == 0) {
                tree_t *v = ast_node_new(TT_VAR);
                char buf[32];
                snprintf(buf, sizeof buf, "_S%d", g_parser.pl.dcg_var_counter++);
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
    int saved_list = p->in_list;
    p->in_args++;
    p->in_list = 1;
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
    p->in_list = saved_list;
    return lst;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pt_args(Parser *p, TreeScope *ts, tree_t *parent) {
    Token pk0 = lexer_peek(&p->lx);
    if (pk0.kind == TK_RPAREN) return 0;
    int n = 0;
    int saved_list = p->in_list;
    p->in_args++;
    p->in_list = 0;
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
    p->in_list = saved_list;
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
static tree_t *pt_primary0(Parser *p, TreeScope *ts) {
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
            for (const unsigned char *q = q0; q < q0 + qn;) { int cp; tree_t *e = ast_node_new(TT_ILIT); q += u8_decode(q, &cp); e->v.ival = (long long)cp; ast_push(n, e); } }
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
            if (pre_a && (prefix_arg_starts(pk) || ((pk.kind == TK_ATOM || pk.kind == TK_OP) && lexer_functor_follows(&p->lx)))) {
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
            Token pkc = lexer_peek(&p->lx);
            if (pkc.kind == TK_LPAREN && pkc.adj && !p->iso) {
                lexer_next(&p->lx);
                tree_t *fnc = ast_node_new(TT_FNC);
                fnc->v.sval = ct_strdup("!");
                pt_args(p, ts, fnc);
                Token rp = lexer_peek(&p->lx);
                if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
                else perror_at(p, rp.line, "expected ) to close argument list");
                return pt_stamp(fnc, ln);
            }
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
            int saved = p->in_args, saved_list = p->in_list;
            p->in_args = 0;
            p->in_list = 0;
            tree_t *inner = pt_term(p, ts, 1200);
            p->in_args = saved;
            p->in_list = saved_list;
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
            if ((strcmp(tk.text, "\\+") == 0 || strcmp(tk.text, "not") == 0) && (prefix_arg_starts(lexer_peek(&p->lx)) || lexer_functor_follows(&p->lx))) {
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
                if (pk3.kind == TK_ATOM || pk3.kind == TK_OP || prefix_arg_starts(pk3)) {
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
                if (pk3.kind == TK_ATOM || pk3.kind == TK_OP || prefix_arg_starts(pk3)) {
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
                if (pre_o && (prefix_arg_starts(pk3) || ((pk3.kind == TK_ATOM || pk3.kind == TK_OP) && lexer_functor_follows(&p->lx)))) {
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
            { int saved = p->in_args, saved_list = p->in_list; p->in_args = 0; p->in_list = 0; inner = pt_term(p, ts, 1200); p->in_args = saved; p->in_list = saved_list; }
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
static tree_t *pt_dict(Parser *p, TreeScope *ts, tree_t *tag, int ln) {
    lexer_next(&p->lx);
    tree_t *d = ast_node_new(TT_FNC);
    d->v.sval = ct_strdup("$dict_lit");
    ast_push(d, tag);
    if (lexer_peek(&p->lx).kind == TK_RBRACE) { lexer_next(&p->lx); return pt_stamp(d, ln); }
    p->in_args++;
    for (;;) {
        Token kt = lexer_next(&p->lx);
        tree_t *k = NULL;
        if (kt.kind == TK_ATOM) { k = ast_node_new(TT_QLIT); k->v.sval = ct_strdup(kt.text); }
        else if (kt.kind == TK_INT && !kt.big) { k = ast_node_new(TT_ILIT); k->v.ival = kt.ival; }
        else { perror_at(p, kt.line, "expected an atom or a small integer as a dict key"); break; }
        Token ct = lexer_next(&p->lx);
        if (ct.kind != TK_OP || strcmp(ct.text, ":")) { perror_at(p, ct.line, "expected : after a dict key"); break; }
        tree_t *v = pt_term(p, ts, 999);
        if (!v) break;
        ast_push(d, pt_stamp(k, kt.line));
        ast_push(d, v);
        Token sep = lexer_next(&p->lx);
        if (sep.kind == TK_COMMA) continue;
        if (sep.kind != TK_RBRACE) perror_at(p, sep.line, "expected , or } in a dict");
        break;
    }
    p->in_args--;
    return pt_stamp(d, ln);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pt_primary(Parser *p, TreeScope *ts) {
    tree_t *n = pt_primary0(p, ts);
    if (!n || p->prec != 0) return n;
    int ln = n->line;
    Token pk = lexer_peek(&p->lx);
    if (!p->iso && pk.kind == TK_LBRACE && pk.adj && (n->t == TT_VAR || n->t == TT_QLIT)) n = pt_dict(p, ts, n, ln);
    for (;;) {
        pk = lexer_peek(&p->lx);
        if (pk.kind != TK_OP || !pk.adj || strcmp(pk.text, ".")) break;
        lexer_next(&p->lx);
        Token ft = lexer_next(&p->lx);
        tree_t *f = NULL;
        if (ft.kind == TK_VAR) f = ts_get(ts, ft.text);
        else if (ft.kind == TK_ATOM && lexer_peek(&p->lx).kind == TK_LPAREN && lexer_peek(&p->lx).adj) {
            lexer_next(&p->lx);
            f = ast_node_new(TT_FNC);
            f->v.sval = ct_strdup(ft.text);
            pt_args(p, ts, f);
            Token rp = lexer_peek(&p->lx);
            if (rp.kind == TK_RPAREN) lexer_next(&p->lx);
            else perror_at(p, rp.line, "expected ) to close argument list");
        }
        else if (ft.kind == TK_ATOM) { f = ast_node_new(TT_QLIT); f->v.sval = ct_strdup(ft.text); }
        else { perror_at(p, ft.line, "expected a key or a function after a dict ."); break; }
        tree_t *dn = ast_node_new(TT_FNC);
        dn->v.sval = ct_strdup("$dot");
        ast_push(dn, n);
        ast_push(dn, pt_stamp(f, ft.line));
        n = pt_stamp(dn, ln);
    }
    p->prec = 0;
    return n;
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
        if (pk.kind == TK_LBRACE) {
            const OpEntry *bo = find_postfix("{}");
            if (!bo || bo->prec > max_prec || lp > (bo->assoc == ASSOC_LEFT ? bo->prec : bo->prec - 1)) break;
            tree_t *blk = pt_primary(p, ts);
            if (!blk) break;
            tree_t *bn = ast_node_new(TT_IDX); bn->v.sval = ct_strdup("{}"); ast_push(bn, lhs); ast_push(bn, blk); lhs = pt_stamp(bn, ln); lp = bo->prec; continue;
        }
        if      (pk.kind == TK_OP)                              optext = pk.text;
        else if (pk.kind == TK_ATOM)                            optext = pk.text;
        else if (pk.kind == TK_COMMA && p->in_args > 0)         break;
        else if (pk.kind == TK_COMMA && max_prec >= 1000)       optext = ",";
        else if (pk.kind == TK_SEMI  && max_prec >= 1100)       optext = ";";
        else if (pk.kind == TK_PIPE  && !p->in_list && max_prec >= 1105) optext = "|";
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
    snprintf(name, sizeof(name), "_S%d", g_parser.pl.dcg_var_counter++);
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
    int cps[n + 1], k = 0;
    for (size_t i = 0; i < n;) i += (size_t)u8_decode((const unsigned char *)b + i, &cps[k++]);
    while (k > 0) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = (long long)cps[--k]; tree_t *dargs[2] = { e, acc }; acc = mk_raw(ATOM_DOT, dargs, 2); }
    return acc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *dcg_append_tail(TreeScope *ts, tree_t *list, tree_t *tail) {
    if (!list) return dcg_var_use(ts, tail);
    if (list->t == TT_DQLIT) list = dcg_dq_codes(list);
    if ((list->t == TT_QLIT || list->t == TT_FNC) && list->v.sval && strcmp(list->v.sval, "[]") == 0 && list->n == 0)
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
    if (nt && (nt->t == TT_FNC || nt->t == TT_QLIT)) {
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
    if ((body->t == TT_QLIT || body->t == TT_FNC) && body->v.sval && strcmp(body->v.sval, "[]") == 0 && body->n == 0) {
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
    if (body->t == TT_FNC && body->v.sval && (strcmp(body->v.sval, ";") == 0 || strcmp(body->v.sval, "|") == 0)
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
    if (body->t == TT_QLIT && body->v.sval && strcmp(body->v.sval, "true") == 0) {
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
    for (int i = 0; i < new_head->n; i++) ast_push(new_head_final, tls(new_head->c[i], 0));
    tree_t *body_prog = ast_node_new(TT_PROGRAM);
    for (int i = 0; i < n; i++) ast_push(body_prog, tls(buf[i], 1));
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
typedef struct { const PlClause *head; const PlClause *stop; int swi; Parser *p; int lineno; } pl_ifcx_t;
static int pl_if_defines(const pl_ifcx_t *x, const char *nm, const tree_t *ar) {
    for (const PlClause *c = x->head; c && c != x->stop; c = c->next) {
        const tree_t *h = (c->tr && c->tr->t == TT_CLAUSE && c->tr->n >= 1) ? c->tr->c[0] : (const tree_t *)0;
        if (h && h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":") && h->n == 2) h = h->c[1];
        if (!h || !h->v.sval || strcmp(h->v.sval, nm) || (h->t != TT_FNC && h->t != TT_QLIT)) continue;
        int n = h->t == TT_FNC ? h->n : 0;
        if (ar->t == TT_VAR || (ar->t == TT_ILIT && ar->v.ival == n)) return 1;
    }
    return 0;
}
static int pl_if_flag(const tree_t *f, const tree_t *v, int swi) {
    extern int rt_pl_flag_at_read(const char *, int, const char **, long *);
    const char *av;
    long ver[3];
    if (!f || f->t != TT_QLIT || !f->v.sval) return 0;
    int k = rt_pl_flag_at_read(f->v.sval, swi, &av, ver);
    if (!k) return 0;
    if (!v || v->t == TT_VAR) return 1;
    if (k == 1 && v->t == TT_QLIT) return v->v.sval && !strcmp(v->v.sval, av);
    if (k == 1 && v->t == TT_ILIT) { char *e; long long n = strtoll(av, &e, 10); return *av && !*e && n == v->v.ival; }
    if (k != 2 || v->t != TT_FNC || v->n != 4 || !v->v.sval || strcmp(v->v.sval, av)) return 0;
    for (int i = 0; i < 3; i++) if (!v->c[i] || (v->c[i]->t != TT_VAR && !(v->c[i]->t == TT_ILIT && v->c[i]->v.ival == ver[i]))) return 0;
    const tree_t *t = v->c[3];
    return !t || t->t == TT_VAR || (t->t == TT_MAKELIST && t->n == 0) || (t->t == TT_QLIT && t->v.sval && !strcmp(t->v.sval, "[]"));
}
static int eval_if_condition_tree(const tree_t *cond, const pl_ifcx_t *x) {
    if (!cond) return 0;
    const char *fn = (cond->t == TT_QLIT || cond->t == TT_FNC) && cond->v.sval ? cond->v.sval : "";
    int arity = cond->t == TT_FNC ? cond->n : 0;
    if (cond->t == TT_QLIT && !strcmp(fn, "true")) return 1;
    if (cond->t == TT_QLIT && (!strcmp(fn, "fail") || !strcmp(fn, "false"))) return 0;
    if ((!strcmp(fn, "\\+") || !strcmp(fn, "not")) && arity == 1) return !eval_if_condition_tree(cond->c[0], x);
    if (!strcmp(fn, ",") && arity == 2) return eval_if_condition_tree(cond->c[0], x) && eval_if_condition_tree(cond->c[1], x);
    if (!strcmp(fn, "->") && arity == 2) return eval_if_condition_tree(cond->c[0], x) && eval_if_condition_tree(cond->c[1], x);
    if (!strcmp(fn, ";") && arity == 2) {
        const tree_t *l = cond->c[0];
        if (l && l->t == TT_FNC && l->v.sval && !strcmp(l->v.sval, "->") && l->n == 2) return eval_if_condition_tree(l->c[0], x) ? eval_if_condition_tree(l->c[1], x) : eval_if_condition_tree(cond->c[1], x);
        return eval_if_condition_tree(l, x) || eval_if_condition_tree(cond->c[1], x);
    }
    if (!strcmp(fn, "current_prolog_flag") && arity == 2) return pl_if_flag(cond->c[0], cond->c[1], x->swi);
    if (!strcmp(fn, "current_predicate") && arity == 1) {
        extern int pl_pi_is_builtin(const char *, int);
        extern int pl_prelude_defines(const char *, int);
        const tree_t *pi = cond->c[0];
        if (pi && pi->t == TT_FNC && pi->v.sval && !strcmp(pi->v.sval, ":") && pi->n == 2) pi = pi->c[1];
        if (pi && pi->t == TT_FNC && pi->v.sval && !strcmp(pi->v.sval, "/") && pi->n == 2 && pi->c[0] && pi->c[0]->t == TT_QLIT && pi->c[0]->v.sval && pi->c[1]) {
            const tree_t *ar = pi->c[1];
            if (ar->t != TT_VAR && ar->t != TT_ILIT) return 0;
            return pl_if_defines(x, pi->c[0]->v.sval, ar) || (ar->t == TT_ILIT && (pl_pi_is_builtin(pi->c[0]->v.sval, (int)ar->v.ival) || pl_prelude_defines(pi->c[0]->v.sval, (int)ar->v.ival)));
        }
    }
    if (!strcmp(fn, "exists_source") && arity == 1 && cond->c[0] && cond->c[0]->t == TT_FNC && cond->c[0]->v.sval && !strcmp(cond->c[0]->v.sval, "library") && cond->c[0]->n == 1) {
        const tree_t *l = cond->c[0]->c[0];
        const char *a = l && l->t == TT_QLIT ? l->v.sval : (const char *)0, *b = (const char *)0;
        if (l && l->t == TT_FNC && l->v.sval && !strcmp(l->v.sval, "/") && l->n == 2 && l->c[0] && l->c[0]->t == TT_QLIT && l->c[1] && l->c[1]->t == TT_QLIT) { a = l->c[0]->v.sval; b = l->c[1]->v.sval; }
        if (a) {
            size_t na = strlen(a);
            for (int i = 0; PL_PRELUDE_LIBS[i].lib; i++) {
                const char *e = PL_PRELUDE_LIBS[i].lib;
                if (b ? !strncmp(e, a, na) && e[na] == '_' && !strcmp(e + na + 1, b) : !strcmp(e, a)) return 1;
            }
            return 0;
        }
    }
    if (!x->p->quiet) fprintf(stderr, "%s:%d: warning: the :- if condition %s/%d has no read-time evaluator in SCRIP, so it reads as false\n", x->p->filename, x->lineno, fn, arity);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int try_handle_if_directive_tree(Parser *p, tree_t *goal, int lineno, pl_ifcx_t *x) {
    if (!goal) return 0;
    x->p = p;
    x->lineno = lineno;
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
        int verdict = parent_active ? eval_if_condition_tree(arg0, x) : 0;
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
            int verdict = eval_if_condition_tree(arg0, x);
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
    if (pk.kind == TK_NECK || pk.kind == TK_QUERY) {
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
            else if (fnc->n == 1) { register_op_directive(fnc->c[0], p->lx.swi); iso_directive(p, fnc->c[0]); cl->nbody = 0; ast_push(_fcl, ast_node_new(TT_NUL)); ast_push(_fcl, fnc->c[0]); }
            else ast_push(_fcl, fnc);
            cl->tr = _fcl;
            return cl;
        }
        tree_t *body_tr = pt_term(p, ts, 1200);
        Token dot = lexer_next(&p->lx);
        if (dot.kind != TK_DOT)
            perror_at(p, dot.line, "expected . after directive");
        register_op_directive(body_tr, p->lx.swi); iso_directive(p, body_tr);
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
    } else if (pk.kind == TK_OP && strcmp(pk.text, "=>") == 0) {
        Token arrow_tk = lexer_next(&p->lx);
        tree_t *body_tr = pt_term(p, ts, 1200);
        { tree_t *_cl = ast_node_new(TT_CLAUSE);
          ast_push(_cl, pt_stamp(pt_binop("=>", head_tr, body_tr), arrow_tk.line));
          cl->tr = _cl; }
        Token dot = lexer_next(&p->lx);
        if (dot.kind != TK_DOT)
            perror_at(p, dot.line, "expected . at end of SSU rule");
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
static const char * const PL_PRELUDE_SRC =
    "member(X,[X|_]).\n"
    "reset(G,B,C):-'$co_enter'(G,H,R,N),'$reset_k'(R,H,N,B,C).\n"
    "'$co_enter'(G,H,R,N):-nonvar(G),G='$cont'(H0,N0),!,H=H0,'$co_resume'(H0,1,N0,R,N).\n"
    "'$co_enter'(G,H,R,N):-'$co_start'(G,H,R,N).\n"
    "'$reset_k'(shift(X),H,N,B,C):-(\\+X=B->shift(X),'$co_resume'(H,1,N,R,N1),'$reset_k'(R,H,N1,B,C);(X=B,C='$cont'(H,N);'$reset_redo'(H,N,B,C))).\n"
    "'$reset_k'(done,H,N,B,C):-(C=0;'$reset_redo'(H,N,B,C)).\n"
    "'$reset_k'(throw(E),_,_,_,_):-throw(E).\n"
    "'$reset_redo'(H,N,B,C):-'$co_resume'(H,0,N,R,N1),'$reset_k'(R,H,N1,B,C).\n"
    "shift(X):-('$co_inside'->'$co_yield'(shift(X),N),(true;'$co_unwound'(unwound,N));throw(error(existence_error(reset,X),context(shift/1,'No matching reset/3 call')))).\n"
    "'$cont'(H,N):-'$co_resume'(H,1,N,R,N1),'$cont_k'(R,H,N1).\n"
    "'$cont_k'(done,H,N):-(true;'$co_resume'(H,0,N,R,N1),'$cont_k'(R,H,N1)).\n"
    "'$cont_k'(shift(X),H,N):-shift(X),'$co_resume'(H,1,N,R,N1),'$cont_k'(R,H,N1).\n"
    "'$cont_k'(throw(E),_,_):-throw(E).\n"
    "'$co_body'(G):-'$co_mark',(call(G),'$co_yield'(done,_),fail;'$co_finish'(0,fail)).\n"
    "'$scc_cut':-'$scc_bnew'(B),'$scc_cut_loop'(B).\n"
    "'$scc_cut_loop'(B):-'$scc_take'(B,C),!,(catch(C,_,true)->true;true),'$scc_cut_loop'(B).\n"
    "'$scc_cut_loop'(_).\n"
    "'$wakeup':-'$attv_take'(L),'$wake_run'(L).\n"
    "'$wake_run'([]).\n"
    "'$wake_run'(wakeup(A,V,R)):-'$wake_hooks'(A,V),'$wake_run'(R).\n"
    "'$wake_hooks'([],_).\n"
    "'$wake_hooks'(att(M,AV,R),V):-'$wake_uhook'(M,AV,V),'$wake_hooks'(R,V).\n"
    "'$wake_uhook'(freeze,G,Y):-!,(attvar(Y)->(get_attr(Y,freeze,G2)->put_attr(Y,freeze,'$and'(G2,G));put_attr(Y,freeze,G));'$wake_unfreeze'(G)).\n"
    "'$wake_uhook'(dif,A,Y):-!,'$dif_hook'(A,Y).\n"
    "'$wake_uhook'(when,A,Y):-!,'$when_hook'(A,Y).\n"
    "'$wake_uhook'(M,A,Y):-('$attr_hook_mod'(M)->'$attr_unify_hook'(M,A,Y);throw(error(existence_error(procedure,M:attr_unify_hook/2),M:attr_unify_hook/2))).\n"
    "'$wake_unfreeze'('$and'(A,B)):-!,'$wake_unfreeze'(A),'$wake_unfreeze'(B).\n"
    "'$wake_unfreeze'(G):-call(G).\n"
    "attvar(V):-'$attv_atts'(V,A),A\\==[].\n"
    "put_attr(V,M,X):-(var(V)->true;throw(error(uninstantiation_error(V),put_attr/3))),('$attv_atts'(V,A)->true;A=[]),'$att_put'(A,M,X,A2),'$attv_set'(V,A2).\n"
    "'$att_put'([],M,X,att(M,X,[])).\n"
    "'$att_put'(att(M0,X0,R),M,X,A):-(M0==M->A=att(M,X,R);A=att(M0,X0,R2),'$att_put'(R,M,X,R2)).\n"
    "get_attr(V,M,X):-'$attv_atts'(V,A),'$att_get'(A,M,X).\n"
    "'$att_get'(att(M0,X0,R),M,X):-(M0==M->X=X0;'$att_get'(R,M,X)).\n"
    "del_attr(V,M):-('$attv_atts'(V,A)->'$att_del'(A,M,A2),'$attv_set'(V,A2);true).\n"
    "'$att_del'([],_,[]).\n"
    "'$att_del'(att(M0,X0,R),M,A):-(M0==M->A=R;A=att(M0,X0,R2),'$att_del'(R,M,R2)).\n"
    "get_attrs(V,A):-'$attv_atts'(V,A),A\\==[].\n"
    "put_attrs(V,A):-(var(V)->true;throw(error(uninstantiation_error(V),put_attrs/2))),'$attv_set'(V,A).\n"
    "del_attrs(V):-('$attv_atts'(V,_)->'$attv_set'(V,[]);true).\n"
    "'$nb_getval_hooked'(K,V):-catch('$nb_getval_raw'(K,V),error(existence_error(variable,K),_),('$undefined_global_retry'(K)->'$nb_getval_raw'(K,V);throw(error(existence_error(variable,K),nb_getval/2)))).\n"
    "'$b_getval_hooked'(K,V):-catch('$b_getval_raw'(K,V),error(existence_error(variable,K),_),('$undefined_global_retry'(K)->'$b_getval_raw'(K,V);throw(error(existence_error(variable,K),b_getval/2)))).\n"
    "'$undefined_global_retry'(K):-exception(undefined_global_variable,K,A),A==retry.\n"
    "create_prolog_flag(F,V,O):-(memberchk(keep(true),O),catch(current_prolog_flag(F,_),_,fail)->true;'$create_prolog_flag'(F,V)).\n"
    "nb_current(K,V):-atom(K),catch(nb_getval(K,V0),error(existence_error(_,_),_),fail),V=V0.\n"
    "ttyflush:-flush_output(user_output).\n"
    "time(G):-statistics(cputime,T0),(call(G)*->'$time_report'(T0);'$time_report'(T0),fail).\n"
    "'$time_report'(T0):-statistics(cputime,T1),T is T1-T0,format(user_error,\"% ~3f CPU seconds~n\",[T]).\n"
    "nth_integer_root_and_remainder(N,I,R,M):-integer(N),integer(I),N>=1,(I>=0->'$nth_root_bs'(N,I,0,I,R);N mod 2=:=1,J is -I,'$nth_root_bs'(N,J,0,J,R0),R is -R0),M is I-R^N.\n"
    "'$nth_root_bs'(_,_,L,H,R):-H-L=<1,!,R=L.\n"
    "'$nth_root_bs'(N,I,L,H,R):-M is (L+H)//2,(M^N=<I->'$nth_root_bs'(N,I,M,H,R);'$nth_root_bs'(N,I,L,M,R)).\n"
    "call_residue_vars(G,Vs):-'$crv_enter'(M),'$crv_call'(G),'$crv_collect'(M,Vs).\n"
    "'$crv_call'(G):-(true;'$crv_adj'(-1),fail),catch(G,E,('$crv_adj'(-1),throw(E))),('$crv_adj'(-1);'$crv_adj'(1),fail).\n"
    "term_attvars(T,Vs):-term_variables(T,Vs0),'$attv_filter'(Vs0,Vs).\n"
    "'$attv_filter'([],[]).\n"
    "'$attv_filter'([V|T],R):-(attvar(V)->R=[V|R2];R=R2),'$attv_filter'(T,R2).\n"
    "freeze(V,G):-var(V),!,'$freeze_goal'(G,MG),(get_attr(V,freeze,G0)->put_attr(V,freeze,'$and'(G0,MG));put_attr(V,freeze,MG)).\n"
    "freeze(_,G):-call(G).\n"
    "'$freeze_goal'(G,G):-nonvar(G),G=_:_,!.\n"
    "'$freeze_goal'(G,user:G).\n"
    "frozen(V,G):-(var(V),get_attr(V,freeze,G0)->'$frozen_goals'(G0,V,G);G=true).\n"
    "'$frozen_goals'('$and'(A,B),V,(GA,GB)):-!,'$frozen_goals'(A,V,GA),'$frozen_goals'(B,V,GB).\n"
    "'$frozen_goals'(G,V,freeze(V,G)).\n"
    "?=(A,B):- \\+ unifiable(A,B,[_|_]).\n"
    "unifiable(X,Y,Us):-'$unifiable'(X,Y,Us).\n"
    "copy_term_nat(T,C):-'$copy_term_nat'(T,C).\n"
    "copy_term(Vs,T,Vs2,T2):-term_variables(Vs,CV),term_variables(T,TV),'$var_subtract'(TV,CV,K),'$copy_keep'(K,Vs-T,Vs2-T2,1).\n"
    "copy_term_nat(Vs,T,Vs2,T2):-term_variables(Vs,CV),term_variables(T,TV),'$var_subtract'(TV,CV,K),'$copy_keep'(K,Vs-T,Vs2-T2,0).\n"
    "'$var_subtract'([],_,[]).\n"
    "'$var_subtract'([V|Vs],S,R):-('$var_memberchk'(V,S)->R=R1;R=[V|R1]),'$var_subtract'(Vs,S,R1).\n"
    "'$var_memberchk'(V,[W|Ws]):-(V==W->true;'$var_memberchk'(V,Ws)).\n"
    "nonground(X,V):-term_variables(X,[V|_]).\n"
    "dif(X,Y):- ?=(X,Y),!,X\\==Y.\n"
    "dif(X,Y):-'$dif_c_c'(X,Y,_).\n"
    "'$dif_unifiable'(X,Y,Us):-unifiable(X,Y,Us).\n"
    "'$dif_c_c'(X,Y,OrNode):-('$dif_unifiable'(X,Y,Unifier)->(Unifier==[]->'$dif_or_one_fail'(OrNode);'$dif_c_c_l'(Unifier,OrNode,U),'$dif_subunifier'(U,OrNode));'$dif_or_succeed'(OrNode)).\n"
    "'$dif_subunifier'([],_).\n"
    "'$dif_subunifier'([X=Y|T],OrNode):-'$dif_c_c'(X,Y,OrNode),'$dif_subunifier'(T,OrNode).\n"
    "'$dif_c_c_l'(Unifier,OrNode,U):-'$dif_extend_ornode'(OrNode,List,Tail),'$dif_c_c_l_aux'(Unifier,OrNode,List0,Tail),('$dif_simplify'(List0,List,U)->true;List=List0,'$dif_or_succeed'(OrNode),U=[]).\n"
    "'$dif_extend_ornode'(OrNode,List,Vars):-(get_attr(OrNode,dif,node(Vars))->true;Vars=[]),put_attr(OrNode,dif,node(List)).\n"
    "'$dif_c_c_l_aux'([],_,List,List).\n"
    "'$dif_c_c_l_aux'([X=Y|Unifier],OrNode,List,Tail):-List=[X=Y|Rest],'$dif_add_ornode'(X,Y,OrNode),'$dif_c_c_l_aux'(Unifier,OrNode,Rest,Tail).\n"
    "'$dif_add_ornode'(X,Y,OrNode):-'$dif_add_ornode_var1'(X,Y,OrNode),(var(Y)->'$dif_add_ornode_var2'(X,Y,OrNode);true).\n"
    "'$dif_add_ornode_var1'(X,Y,OrNode):-(get_attr(X,dif,Attr)->Attr=vardif(V1,V2),put_attr(X,dif,vardif([OrNode-Y|V1],V2));put_attr(X,dif,vardif([OrNode-Y],[]))).\n"
    "'$dif_add_ornode_var2'(X,Y,OrNode):-(get_attr(Y,dif,Attr)->Attr=vardif(V1,V2),put_attr(Y,dif,vardif(V1,[OrNode-X|V2]));put_attr(Y,dif,vardif([],[OrNode-X]))).\n"
    "'$dif_simplify_ornode'(OrNode):-(get_attr(OrNode,dif,node(Pairs0))->'$dif_simplify'(Pairs0,Pairs,U),Pairs-U\\==[]-[],put_attr(OrNode,dif,node(Pairs)),'$dif_subunifier'(U,OrNode);true).\n"
    "'$dif_simplify'(List0,List,U):-sort(1,@=<,List0,Sorted),'$dif_simplify_'(Sorted,List,U).\n"
    "'$dif_simplify_'([],List,U):-!,List=[],U=[].\n"
    "'$dif_simplify_'([V1=V2|T],List,U):-V1==V2,!,'$dif_simplify_'(T,List,U).\n"
    "'$dif_simplify_'([V1=Val1,V2=Val2|T],List,U):-var(V1),V1==V2,!,(?=(Val1,Val2)->Val1==Val2,'$dif_simplify_'([V1=Val2|T],List,U);U=[Val1=Val2|UT],'$dif_simplify_'([V2=Val2|T],List,UT)).\n"
    "'$dif_simplify_'([H|T],List,U):-List=[H|Rest],'$dif_simplify_'(T,Rest,U).\n"
    "'$dif_hook'(vardif(V1,V2),Other):-(get_attr(Other,dif,vardif(OV1,OV2))->'$dif_reverse_lookups'(V1,Other,OrNodes1,NV1),'$dif_or_one_fails'(OrNodes1),'$dif_reverse_lookups'(OV1,Other,OrNodes2,NOV1),'$dif_or_one_fails'(OrNodes2),'$dif_remove_obsolete'(V2,Other,NV2),'$dif_remove_obsolete'(OV2,Other,NOV2),append(NV1,NOV1,CV1),append(NV2,NOV2,CV2),(CV1==[],CV2==[]->del_attr(Other,dif);put_attr(Other,dif,vardif(CV1,CV2)));var(Other)->put_attr(Other,dif,vardif(V1,V2));'$dif_verify_compounds'(V1,Other),'$dif_verify_compounds'(V2,Other)).\n"
    "'$dif_remove_obsolete'([],_,[]).\n"
    "'$dif_remove_obsolete'([N-Y|T],X,L):-(Y==X->'$dif_remove_obsolete'(T,X,L);L=[N-Y|RT],'$dif_remove_obsolete'(T,X,RT)).\n"
    "'$dif_reverse_lookups'([],_,[],[]).\n"
    "'$dif_reverse_lookups'([N-X|NXs],Value,Nodes,Rest):-(X==Value->Nodes=[N|RNodes],Rest=RRest;Nodes=RNodes,Rest=[N-X|RRest]),'$dif_reverse_lookups'(NXs,Value,RNodes,RRest).\n"
    "'$dif_verify_compounds'([],_).\n"
    "'$dif_verify_compounds'([OrNode-Y|Rest],X):-(var(Y)->true;OrNode==(-)->true;'$dif_c_c'(X,Y,OrNode)),'$dif_verify_compounds'(Rest,X).\n"
    "'$dif_or_succeed'(OrNode):-(get_attr(OrNode,dif,Attr)->Attr=node(Pairs),del_attr(OrNode,dif),OrNode=(-),'$dif_del_or_dif'(Pairs);true).\n"
    "'$dif_del_or_dif'([]).\n"
    "'$dif_del_or_dif'([X=Y|Xs]):-'$dif_cleanup_dead_nodes'(X),'$dif_cleanup_dead_nodes'(Y),'$dif_del_or_dif'(Xs).\n"
    "'$dif_cleanup_dead_nodes'(X):-(get_attr(X,dif,Attr)->Attr=vardif(V1,V2),'$dif_filter_dead_ors'(V1,NV1),'$dif_filter_dead_ors'(V2,NV2),(NV1==[],NV2==[]->del_attr(X,dif);put_attr(X,dif,vardif(NV1,NV2)));true).\n"
    "'$dif_filter_dead_ors'([],[]).\n"
    "'$dif_filter_dead_ors'([Or-Y|Rest],List):-(var(Or)->List=[Or-Y|NRest];List=NRest),'$dif_filter_dead_ors'(Rest,NRest).\n"
    "'$dif_or_one_fail'(OrNode):-'$dif_simplify_ornode'(OrNode).\n"
    "'$dif_or_one_fails'([]).\n"
    "'$dif_or_one_fails'([N|Ns]):-'$dif_or_one_fail'(N),'$dif_or_one_fails'(Ns).\n"
    "when(Cond,Goal):-'$when_cond'(Cond,Opt),'$when_first'(Opt,Goal).\n"
    "'$when_cond'(C,_):-var(C),!,throw(error(instantiation_error,when/2)).\n"
    "'$when_cond'(nonvar(X),O):-!,(nonvar(X)->O=true;O=nonvar(X)).\n"
    "'$when_cond'(ground(X),O):-!,(ground(X)->O=true;O=ground(X)).\n"
    "'$when_cond'(?=(X,Y),O):-!,(?=(X,Y)->O=true;O = ?=(X,Y)).\n"
    "'$when_cond'((A,B),O):-!,'$when_cond'(A,OA),'$when_cond'(B,OB),(OA==true->O=OB;OB==true->O=OA;O=(OA,OB)).\n"
    "'$when_cond'((A;B),O):-!,'$when_cond'(A,OA),'$when_cond'(B,OB),(OA==true->O=true;OB==true->O=true;'$when_or_list'(OA,LA),'$when_or_list'(OB,LB),append(LA,LB,L),O=or(L)).\n"
    "'$when_cond'(C,_):-throw(error(domain_error(when_condition,C),when/2)).\n"
    "'$when_or_list'(or(L),L):-!.\n"
    "'$when_or_list'(C,[C]).\n"
    "'$when_first'(true,Goal):-!,call(Goal).\n"
    "'$when_first'(nonvar(X),Goal):-!,'$when_suspend'(X,'$when_trigger_nonvar'(X,Goal)).\n"
    "'$when_first'(Cond,Goal):-'$when_trigger'(Cond,Goal).\n"
    "'$when_trigger'(nonvar(X),Goal):-'$when_trigger_nonvar'(X,Goal).\n"
    "'$when_trigger'(ground(X),Goal):-'$when_trigger_ground'(X,Goal).\n"
    "'$when_trigger'(?=(X,Y),Goal):-'$when_trigger_determined'(X,Y,Goal).\n"
    "'$when_trigger'((G1,G2),Goal):-'$when_trigger'(G1,'$when_trigger'(G2,Goal)).\n"
    "'$when_trigger'(or(GL),Goal):-'$when_trigger_disj'(GL,'$when_check_disj'(_,GL,Goal)).\n"
    "'$when_trigger_nonvar'(X,Goal):-(nonvar(X)->call(Goal);'$when_suspend'(X,'$when_trigger_nonvar'(X,Goal))).\n"
    "'$when_trigger_ground'(X,Goal):-(nonground(X,V)->'$when_suspend'(V,'$when_trigger_ground'(X,Goal));call(Goal)).\n"
    "'$when_trigger_determined'(X,Y,Goal):-unifiable(X,Y,Unifier),!,(Unifier==[]->call(Goal);put_attr(Det,when,det('$when_trigger_determined'(X,Y,Goal))),'$when_suspend_list'(Unifier,'$when_wake_det'(Det))).\n"
    "'$when_trigger_determined'(_,_,Goal):-call(Goal).\n"
    "'$when_wake_det'(Det):-(var(Det)->get_attr(Det,when,Attr),del_attr(Det,when),Det=(-),Attr=det(Goal),call(Goal);true).\n"
    "'$when_trigger_disj'([],_).\n"
    "'$when_trigger_disj'([H|T],G):-'$when_trigger'(H,G),'$when_trigger_disj'(T,G).\n"
    "'$when_check_disj'(Disj,_,Goal):-(Disj==(-)->true;Disj=(-),call(Goal)).\n"
    "'$when_suspend_list'([],_).\n"
    "'$when_suspend_list'([V=W|Unifier],Goal):-'$when_suspend'(V,Goal),(var(W)->'$when_suspend'(W,Goal);true),'$when_suspend_list'(Unifier,Goal).\n"
    "'$when_suspend'(V,Goal):-(get_attr(V,when,call(G0))->put_attr(V,when,call((G0,Goal)));put_attr(V,when,call(Goal))).\n"
    "'$when_hook'(call(Goal),Other):-(get_attr(Other,when,call(GOther))->del_attr(Other,when),call(Goal),call(GOther);call(Goal)).\n"
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
    "'$len_tail'(T,_,K,N):-N>=K,D is N-K,'$heap_guard'(D,48),'$len_mk'(D,T).\n"
    "'$len_gen'([],K,K).\n"
    "'$len_gen'([_|T],K,N):-K1 is K+1,'$len_gen'(T,K1,N).\n"
    "'$len_mk'(0,[]):-!.\n"
    "'$len_mk'(D,[_|T]):-D1 is D-1,'$len_mk'(D1,T).\n"
    "numlist(L,H,R):-'$nl_int'(L),'$nl_int'(H),L=<H,N is H-L+1,'$heap_guard'(N,48),'$numlist_'(L,H,R).\n"
    "'$nl_int'(X):-var(X),!,throw(error(instantiation_error,_)).\n"
    "'$nl_int'(X):-integer(X),!.\n"
    "'$nl_int'(X):-throw(error(type_error(integer,X),_)).\n"
    "'$numlist_'(L,H,[]):-L>H,!.\n"
    "'$numlist_'(L,H,[L|T]):-L=<H,L1 is L+1,'$numlist_'(L1,H,T).\n"
    "memberchk(X,L):-member(X,L),!.\n"
    "sub_string(S,B,L,A,Sub):-var(Sub),!,sub_atom(S,B,L,A,X),atom_string(X,Sub).\n"
    "sub_string(S,B,L,A,Sub):-string(Sub),!,sub_atom(S,B,L,A,Sub).\n"
    "sub_string(S,B,L,A,Sub):-atom_string(Sub,T),sub_atom(S,B,L,A,T).\n"
    "atomics_to_string(L,S):-atomic_list_concat(L,A),atom_string(A,S).\n"
    "atomics_to_string(L,Sep,S):-atomic_list_concat(L,Sep,A),atom_string(A,S).\n"
    "string_to_list(S,L):-string_codes(S,L).\n"
    "'$string_concat_split'(_,_,C):-var(C),!,throw(error(instantiation_error,context(system:string_concat/3,_))).\n"
    "'$string_concat_split'(A,B,C):-string_codes(C,Cs),(nonvar(A)->string_codes(A,As),append(As,Bs,Cs),string_codes(B,Bs);nonvar(B)->string_codes(B,Bs),append(As,Bs,Cs),string_codes(A,As);append(As,Bs,Cs),string_codes(A,As),string_codes(B,Bs)).\n"
    "string_code(_,S,_):-var(S),!,throw(error(instantiation_error,string_code/3)).\n"
    "string_code(I,S,C):-var(I),!,string_codes(S,Cs),'$sc_enum'(Cs,1,I,C).\n"
    "string_code(I,_,_):- \\+ integer(I),!,throw(error(type_error(integer,I),string_code/3)).\n"
    "string_code(I,_,_):-I<0,!,throw(error(domain_error(not_less_than_zero,I),string_code/3)).\n"
    "string_code(I,S,C):-I>0,string_codes(S,Cs),nth1(I,Cs,C0),!,C=C0.\n"
    "'$sc_enum'([X|Xs],N,I,C):-(I=N,C=X;N1 is N+1,'$sc_enum'(Xs,N1,I,C)).\n"
    "foreach(Gen,Goal):-term_variables(Gen,GV),term_variables(Goal,OV),'$fe_vars'(OV,GV,1,SV),'$fe_vars'(OV,GV,0,FV),Templ=..[v|SV],findall(Templ,Gen,L),'$fe_prove'(L,Templ,FV,Goal).\n"
    "'$fe_vars'([],_,_,[]).\n"
    "'$fe_vars'([V|Vs],GV,In,R):-('$fe_memq'(V,GV)->M=1;M=0),(M==In->R=[V|R1];R=R1),'$fe_vars'(Vs,GV,In,R1).\n"
    "'$fe_memq'(V,[W|Ws]):-(V==W->true;'$fe_memq'(V,Ws)).\n"
    "'$fe_prove'([],_,_,_).\n"
    "'$fe_prove'([H|T],Templ,FV,Goal):-copy_term(Templ/FV/Goal,H/FV/G1),call(G1),'$fe_prove'(T,Templ,FV,Goal).\n"
    "split_string(S,Sep,Pad,Subs):-string_codes(S,Cs0),string_codes(Sep,SC),string_codes(Pad,PC),'$ss_strip'(Cs0,PC,Cs),'$ss_fields'(Cs,SC,PC,Subs).\n"
    "'$ss_fields'(Cs,SC,PC,[S|Ss]):-'$ss_take'(Cs,SC,F,R),'$ss_strip'(F,PC,F1),string_codes(S,F1),(R=[]->Ss=[];R=[_|R1],'$ss_lstrip'(R1,PC,R2),'$ss_fields'(R2,SC,PC,Ss)).\n"
    "'$ss_take'([],_,[],[]).\n"
    "'$ss_take'([C|Cs],SC,[],[C|Cs]):-memberchk(C,SC),!.\n"
    "'$ss_take'([C|Cs],SC,[C|F],R):-'$ss_take'(Cs,SC,F,R).\n"
    "'$ss_strip'(F,PC,F2):-'$ss_lstrip'(F,PC,F1),reverse(F1,R1),'$ss_lstrip'(R1,PC,R2),reverse(R2,F2).\n"
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
    "'$expand_term1'((H-->B),T2,_):- current_prolog_flag('$dialect',gnu),!,'$gnu_dcg_rule'((H-->B),T2).\n"
    "'$expand_term1'((H-->B),T2,_):-!,dcg_translate_rule((H-->B),T2).\n"
    "'$expand_term1'(T,T,_).\n"
    "expand_term(T,X):-'$expand_term1'(T,X0,_),'$expand_bodies'(X0,X).\n"
    "'$expand_bodies'(X,X):-var(X),!.\n"
    "'$expand_bodies'([],[]):-!.\n"
    "'$expand_bodies'([C|Cs],[D|Ds]):-!,'$expand_bodies'(C,D),'$expand_bodies'(Cs,Ds).\n"
    "'$expand_bodies'((H:-B),(H:-B1)):-!,expand_goal(B,B1).\n"
    "'$expand_bodies'((:-B),(:-B1)):-!,expand_goal(B,B1).\n"
    "'$expand_bodies'(C,C).\n"
    "expand_goal(G,G):-var(G),!.\n"
    "expand_goal(G,X):-current_predicate(goal_expansion/2),call(goal_expansion(G,G1)),G1\\==G,!,expand_goal(G1,X0),('$expand_wrap'(G,X0)->X=(X0,true);X=X0).\n"
    "'$expand_wrap'(G,R):-R=(_->_),G\\=(_->_).\n"
    "'$expand_wrap'(G,R):-R=(_*->_),G\\=(_*->_).\n"
    "expand_goal(G,X):-'$expand_ctl'(G,X).\n"
    "'$expand_ctl'((A,B),(A1,B1)):-!,expand_goal(A,A1),expand_goal(B,B1).\n"
    "'$expand_ctl'((A;B),(A1;B1)):-!,expand_goal(A,A1),expand_goal(B,B1).\n"
    "'$expand_ctl'((A->B),(A1->B1)):-!,expand_goal(A,A1),expand_goal(B,B1).\n"
    "'$expand_ctl'((A*->B),(A1*->B1)):-!,expand_goal(A,A1),expand_goal(B,B1).\n"
    "'$expand_ctl'(\\+A,\\+A1):-!,expand_goal(A,A1).\n"
    "'$expand_ctl'(call(A),call(A1)):-!,expand_goal(A,A1).\n"
    "'$expand_ctl'(not(A),not(A1)):-!,expand_goal(A,A1).\n"
    "'$expand_ctl'(call_cleanup(A,B),call_cleanup(A1,B1)):-!,expand_goal(A,A1),expand_goal(B,B1).\n"
    "'$expand_ctl'(setup_call_cleanup(S,A,B),setup_call_cleanup(S1,A1,B1)):-!,expand_goal(S,S1),expand_goal(A,A1),expand_goal(B,B1).\n"
    "'$expand_ctl'(once(A),once(A1)):-!,expand_goal(A,A1).\n"
    "'$expand_ctl'(ignore(A),ignore(A1)):-!,expand_goal(A,A1).\n"
    "'$expand_ctl'(forall(A,B),forall(A1,B1)):-!,expand_goal(A,A1),expand_goal(B,B1).\n"
    "'$expand_ctl'(catch(A,C,B),catch(A1,C,B1)):-!,expand_goal(A,A1),expand_goal(B,B1).\n"
    "'$expand_ctl'(findall(T,A,L),findall(T,A1,L)):-!,expand_goal(A,A1).\n"
    "'$expand_ctl'(findall(T,A,L,M),findall(T,A1,L,M)):-!,expand_goal(A,A1).\n"
    "'$expand_ctl'(aggregate_all(S,A,R),aggregate_all(S,A1,R)):-!,expand_goal(A,A1).\n"
    "'$expand_ctl'(G,G1):-callable(G),G=..[F|As],As\\==[],functor(G,F,N),functor(S,F,N),catch('$meta_rt'(S),error(existence_error(_,_),_),fail),!,S=..[_|Ss],'$expand_margs'(As,Ss,As1),G1=..[F|As1].\n"
    "'$expand_ctl'(G,G).\n"
    "'$expand_margs'([],[],[]).\n"
    "'$expand_margs'([A|As],[S|Ss],[A1|As1]):-(S==0->expand_goal(A,A1);A1=A),'$expand_margs'(As,Ss,As1).\n"
    "dcg_translate_rule((H,P-->B),(H1:-B1,P1)):-!,'$dcgt_nt'(H,S0,S,H1),'$dcg_body'(B,S0,S1,B1),'$dcg_body'(P,S,S1,P1).\n"
    "dcg_translate_rule((H-->B),(H1:-B1)):-'$dcgt_nt'(H,S0,S,H1),'$dcg_body'(B,S0,S,B1).\n"
    "'$dcgt_nt'(NT,S0,S,G):-NT=..L,'$dcg_list'(L,[S0,S],L2),G=..L2.\n"
    "'$gnu_dcg_rule'((H-->B),(H1:-G)):-'$gnu_dcg_head'(H,H1,S0,S,E),'$gnu_dcg_body'(B,f,S0,S,E,t,G).\n"
    "'$gnu_dcg_head'(H,_,_,_,_):-var(H),!,throw(error(instantiation_error,_)).\n"
    "'$gnu_dcg_head'((H,P),H1,S0,S,P):-!,(var(P)->throw(error(instantiation_error,_));P==[]->true;P=[_|_]->true;throw(error(type_error(list,P),_))),'$gnu_dcg_nt'(H,S0,S,H1).\n"
    "'$gnu_dcg_head'(H,H1,S0,S,[]):-'$gnu_dcg_nt'(H,S0,S,H1).\n"
    "'$gnu_dcg_nt'(H,_,_,_):-var(H),!,throw(error(instantiation_error,_)).\n"
    "'$gnu_dcg_nt'(H,_,_,_):- \\+callable(H),!,throw(error(type_error(callable,H),_)).\n"
    "'$gnu_dcg_nt'(H,S0,S,H1):-H=..L,'$dcg_list'(L,[S0,S],L1),H1=..L1.\n"
    "'$gnu_dcg_body'(B,A,S0,S,E,O,G):-(E==[]->Q=f,(A==f->N=S;true);Q=t),'$gnu_dcg_on'(B,Q,S0,N,O,O1,Gs,Gs1),'$gnu_dcg_end'(E,A,S0,S,N,O1,Gs,Gs1,Gs2),'$gnu_dcg_conj'(Gs2,G).\n"
    "'$gnu_dcg_end'(E,_,_,S,N,O,Gs,Gs1,Gs):-E\\==[],!,'$gnu_dcg_tl'(E,S,N,O,_,Gs1,[]).\n"
    "'$gnu_dcg_end'(_,t,S0,S,N,_,Gs,[],Gs2):-!,(S0==N->Gs2=[N=S|Gs];N=S,Gs2=Gs).\n"
    "'$gnu_dcg_end'(_,_,_,_,_,_,Gs,[],Gs).\n"
    "'$gnu_dcg_conj'([],true).\n"
    "'$gnu_dcg_conj'([G|Gs],C):-(Gs==[]->C=G;C=(G,C1),'$gnu_dcg_conj'(Gs,C1)).\n"
    "'$gnu_dcg_on'(B,_,S0,S,O,O,[phrase(B,S0,S)|Gs],Gs):-var(B),!.\n"
    "'$gnu_dcg_on'([],_,S0,S,O,O,Gs,Gs):-!,S0=S.\n"
    "'$gnu_dcg_on'([H|T],_,S0,S,O0,O,Gs0,Gs):-!,'$gnu_dcg_tl'([H|T],S0,S,O0,O,Gs0,Gs).\n"
    "'$gnu_dcg_on'(B,_,_,_,_,_,_,_):- \\+callable(B),!,throw(error(type_error(callable,B),_)).\n"
    "'$gnu_dcg_on'((A,B),Q,S0,S,O0,O,Gs0,Gs):-!,'$gnu_dcg_on'(A,t,S0,W,O0,O1,Gs0,Gs1),'$gnu_dcg_on'(B,Q,W,S,O1,O,Gs1,Gs).\n"
    "'$gnu_dcg_on'((C->T),_,S0,S,_,f,[(GC->GT)|Gs],Gs):-!,'$gnu_dcg_body'(C,f,S0,W,[],f,GC),'$gnu_dcg_body'(T,f,W,S,[],f,GT).\n"
    "'$gnu_dcg_on'((C*->T),_,S0,S,_,f,[(GC*->GT)|Gs],Gs):-!,'$gnu_dcg_body'(C,f,S0,W,[],f,GC),'$gnu_dcg_body'(T,f,W,S,[],f,GT).\n"
    "'$gnu_dcg_on'((A;B),_,S0,S,_,f,[(GA;GB)|Gs],Gs):-!,'$gnu_dcg_body'(A,t,S0,S,[],f,GA),'$gnu_dcg_body'(B,t,S0,S,[],f,GB).\n"
    "'$gnu_dcg_on'('|'(A,B),_,S0,S,_,f,[(GA;GB)|Gs],Gs):-!,'$gnu_dcg_body'(A,t,S0,S,[],f,GA),'$gnu_dcg_body'(B,t,S0,S,[],f,GB).\n"
    "'$gnu_dcg_on'(\\+A,Q,S0,S,_,f,[\\+GA|Gs1],Gs):-!,'$gnu_dcg_body'(A,f,S0,_,[],f,GA),'$gnu_dcg_eq'(Q,S0,S,Gs1,Gs).\n"
    "'$gnu_dcg_on'(!,Q,S0,S,_,f,[!|Gs1],Gs):-!,'$gnu_dcg_eq'(Q,S0,S,Gs1,Gs).\n"
    "'$gnu_dcg_on'({}(G),Q,S0,S,_,f,[G|Gs1],Gs):-!,'$gnu_dcg_eq'(Q,S0,S,Gs1,Gs).\n"
    "'$gnu_dcg_on'(NT,_,S0,S,_,f,[G|Gs],Gs):-NT=..L,'$dcg_list'(L,[S0,S],L1),G=..L1.\n"
    "'$gnu_dcg_eq'(t,S0,S,Gs,Gs):-!,S0=S.\n"
    "'$gnu_dcg_eq'(f,S0,S,[S0=S|Gs],Gs).\n"
    "'$gnu_dcg_tl'(L,S0,S,O0,t,Gs0,Gs):-'$gnu_dcg_cp'(L,L,S,X),(O0==t->S0=X,Gs0=Gs;Gs0=[S0=X|Gs]).\n"
    "'$gnu_dcg_cp'([H|T],L,S,[H|R]):-(var(T)->throw(error(instantiation_error,_));T==[]->R=S;T=[_|_]->'$gnu_dcg_cp'(T,L,S,R);throw(error(type_error(list,L),_))).\n"
    "'$predicate_property1'(F,N,built_in):-'$gnu_builtin'(F,N),\\+current_predicate(F/N).\n"
    "'$pl_pp_pcheck'(P):-(var(P);\\+current_prolog_flag('$dialect',gnu)),!.\n"
    "'$pl_pp_pcheck'(P):-callable(P),functor(P,N,_),memberchk(N,[static,dynamic,private,public,monofile,multifile,user,built_in,built_in_fd,control_construct,native_code,"
        "prolog_file,prolog_line,meta_predicate,defined,discontiguous,exported,imported_from,foreign,iso,number_of_clauses,native,tabled,volatile,transparent,nodebug,visible,"
        "template,alias,declared_in,defined_in,scope,synchronized,file,line_count,notrace,thread_local,non_terminal,ssu,quasi_quotation_syntax]),!.\n"
    "'$pl_pp_pcheck'(P):-throw(error(domain_error(predicate_property,P),predicate_property/2)).\n"
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
    "is_relative_file_name(P):-var(P),!,throw(error(instantiation_error,is_relative_file_name/1)).\n"
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
    "nth0(N,L,E):-integer(N),!,'$nth_det'(N,L,E).\n"
    "nth0(N,L,E):-var(N),!,'$nth_'(L,0,N,E).\n"
    "nth0(N,_,_):-throw(error(type_error(integer,N),context(nth0/3,_))).\n"
    "nth1(N,L,E):-integer(N),!,N0 is N-1,'$nth_det'(N0,L,E).\n"
    "nth1(N,L,E):-var(N),!,'$nth_'(L,1,N,E).\n"
    "nth1(N,_,_):-throw(error(type_error(integer,N),context(nth1/3,_))).\n"
    "'$nth_det'(0,[X|_],E):-!,E=X.\n"
    "'$nth_det'(N,[_|T],E):-N>0,N1 is N-1,'$nth_det'(N1,T,E).\n"
    "'$nth_'([H|_],I,I,H).\n"
    "'$nth_'([_|T],I,N,E):-I1 is I+1,'$nth_'(T,I1,N,E).\n"
    "is_dict(D):-compound(D),functor(D,'$dict',A),A mod 2=:=1.\n"
    "is_dict(D,T):-is_dict(D),arg(1,D,T).\n"
    "rational(X,N,D):-rational(X),N0 is numerator(X),D0 is denominator(X),N=N0,D=D0.\n"
    "blob(X,T):-atom(X),T=text.\n"
    "'$is_char_list'(L,N):-is_list(L),'$is_char_list_'(L,0,N).\n"
    "'$is_char_list_'([],N,N).\n"
    "'$is_char_list_'([C|T],N0,N):-atom(C),atom_length(C,1),N1 is N0+1,'$is_char_list_'(T,N1,N).\n"
    "'$is_code_list'(L,N):-is_list(L),'$is_code_list_'(L,0,N).\n"
    "'$is_code_list_'([],N,N).\n"
    "'$is_code_list_'([C|T],N0,N):-integer(C),C>=0,C=<1114111,N1 is N0+1,'$is_code_list_'(T,N1,N).\n"
    "'$filled_array'(T,N,A,V):-functor(T,N,A),'$fill_array_'(A,T,V).\n"
    "'$fill_array_'(0,_,_):-!.\n"
    "'$fill_array_'(I,T,V):-arg(I,T,V),I1 is I-1,'$fill_array_'(I1,T,V).\n"
    "duplicate_term(T,C):-copy_term(T,C).\n"
    "on_signal(S,Old,New):-atomic(S),atomic_list_concat(['$signal_',S],K),(nb_current(K,H)->true;H=default),Old=H,(var(New)->New=H;nb_setval(K,New)).\n"
    "nb_linkval(K,V):-nb_setval(K,V).\n"
    "catch_with_backtrace(G,E,R):-catch(G,E,R).\n"
    "get_single_char(C):-get_code(user_input,C).\n"
    "ansi_format(_,F,A):-format(F,A).\n"
    "functor(T,N,A,Ty):-nonvar(T),!,'$f4_kind'(T,N,A,K),'$f4_accept'(Ty,K).\n"
    "functor(T,N,A,Ty):-'$f4_build'(T,N,A,Ty).\n"
    "'$f4_kind'(T,N,A,compound):-compound(T),!,compound_name_arity(T,N,A).\n"
    "'$f4_kind'(T,T,0,atom):-atom(T),!.\n"
    "'$f4_kind'(T,T,0,atomic).\n"
    "'$f4_accept'(Ty,K):-var(Ty),!,Ty=K.\n"
    "'$f4_accept'(K,K):-!.\n"
    "'$f4_accept'(callable,atom):-!.\n"
    "'$f4_accept'(callable,compound).\n"
    "'$f4_build'(_,N,_,_):-var(N),!,throw(error(instantiation_error,_)).\n"
    "'$f4_build'(_,_,A,_):-var(A),!,throw(error(instantiation_error,_)).\n"
    "'$f4_build'(_,_,A,_):- \\+ integer(A),!,throw(error(type_error(integer,A),_)).\n"
    "'$f4_build'(_,N,_,_):-compound(N),!,throw(error(type_error(atomic,N),_)).\n"
    "'$f4_build'(T,N,A,Ty):-var(Ty),!,'$f4_free'(T,N,A).\n"
    "'$f4_build'(T,N,A,compound):-!,'$f4_atom'(N),compound_name_arity(T,N,A).\n"
    "'$f4_build'(T,N,0,atom):-!,atom(N),T=N.\n"
    "'$f4_build'(T,N,A,callable):-!,'$f4_callable'(T,N,A).\n"
    "'$f4_build'(T,N,0,atomic):-!,atomic(N),\\+ atom(N),T=N.\n"
    "'$f4_free'(T,N,0):-!,(atom(N)->throw(error(instantiation_error,_));T=N).\n"
    "'$f4_free'(T,N,A):-'$f4_atom'(N),compound_name_arity(T,N,A).\n"
    "'$f4_atom'(N):-atom(N),!.\n"
    "'$f4_atom'(N):-throw(error(type_error(atom,N),_)).\n"
    "'$f4_callable'(T,N,0):-!,atom(N),T=N.\n"
    "'$f4_callable'(T,N,A):-A>0,'$f4_atom'(N),compound_name_arity(T,N,A).\n"
    "'$term_size'(T,Max,S):-'$tsz'(T,S),(var(Max)->true;S=<Max).\n"
    "'$tsz'(T,0):-var(T),!.\n"
    "'$tsz'(T,S):-compound(T),!,compound_name_arguments(T,_,As),length(As,A),A1 is A+1,'$tsz_l'(As,A1,S).\n"
    "'$tsz'(T,3):-float(T),!.\n"
    "'$tsz'(T,S):-string(T),!,string_length(T,L),S is (L+1)//8+3.\n"
    "'$tsz'(T,S):-integer(T),!,'$tsz_int'(T,S).\n"
    "'$tsz'(_,0).\n"
    "'$tsz_l'([],S,S).\n"
    "'$tsz_l'([X|Xs],S0,S):-'$tsz'(X,S1),S2 is S0+S1,'$tsz_l'(Xs,S2,S).\n"
    "'$tsz_int'(I,0):-I>= -1152921504606846976,I=<1152921504606846975,!.\n"
    "'$tsz_int'(I,3):-I>= -9223372036854775808,I=<9223372036854775807,!.\n"
    "'$tsz_int'(I,S):-J is abs(I),'$tsz_limbs'(J,0,L),S is 3+L.\n"
    "'$tsz_limbs'(0,L,L):-!.\n"
    "'$tsz_limbs'(J,L0,L):-L1 is L0+1,J1 is J>>64,'$tsz_limbs'(J1,L1,L).\n"
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
    "debug:-set_prolog_flag(debug,on).\n"
    "nodebug:-set_prolog_flag(debug,off),'$dbg_off_reset'.\n"
    "'$dbg_off_reset':- \\+current_prolog_flag('$dialect',gnu),!.\n"
    "'$dbg_off_reset':-nb_setval('$gnu_b_leash',31).\n"
    "debugging(T):-'$dbg_on'(T).\n"
    "'$dbg_on'(T):-'$dbg_topics'(L),member(X,L),\\+ \\+ X=T,!.\n"
    "'$dbg_topics'(L):-catch(nb_getval('$dbg_topics',L),error(existence_error(_,_),_),L=[]).\n"
    "'$dbg_rm'([],_,[]).\n"
    "'$dbg_rm'([X|Xs],T,R):-('$dbg_variant'(X,T)->R=R1;R=[X|R1]),'$dbg_rm'(Xs,T,R1).\n"
    "'$dbg_variant'(A,B):-copy_term(A,A1),numbervars(A1,0,_),copy_term(B,B1),numbervars(B1,0,_),A1==B1.\n"
    "':'(M,_):-var(M),!,throw(error(instantiation_error,(:)/2)).\n"
    "':'(M,G):-'$mod_call'(M,G).\n"
    "':'(M,G,A):-('$mod_cl'(M,G,1,G2)->call(G2,A);call(G,A)).\n"
    "':'(M,G,A,B):-('$mod_cl'(M,G,2,G2)->call(G2,A,B);call(G,A,B)).\n"
    "':'(M,G,A,B,C):-('$mod_cl'(M,G,3,G2)->call(G2,A,B,C);call(G,A,B,C)).\n"
    "':'(M,G,A,B,C,D):-('$mod_cl'(M,G,4,G2)->call(G2,A,B,C,D);call(G,A,B,C,D)).\n"
    "':'(M,G,A,B,C,D,E):-('$mod_cl'(M,G,5,G2)->call(G2,A,B,C,D,E);call(G,A,B,C,D,E)).\n"
    "':'(M,G,A,B,C,D,E,F):-('$mod_cl'(M,G,6,G2)->call(G2,A,B,C,D,E,F);call(G,A,B,C,D,E,F)).\n"
    "':'(M,G,A,B,C,D,E,F,H):-('$mod_cl'(M,G,7,G2)->call(G2,A,B,C,D,E,F,H);call(G,A,B,C,D,E,F,H)).\n"
    "':'(M,G,A,B,C,D,E,F,H,I):-('$mod_cl'(M,G,8,G2)->call(G2,A,B,C,D,E,F,H,I);call(G,A,B,C,D,E,F,H,I)).\n"
    "':'(M,G,A,B,C,D,E,F,H,I,J):-('$mod_cl'(M,G,9,G2)->call(G2,A,B,C,D,E,F,H,I,J);call(G,A,B,C,D,E,F,H,I,J)).\n"
    "'$cp_mq'(M,P):-var(M),!,current_predicate(P),M=user.\n"
    "'$cp_mq'(M,_):- \\+atom(M),!,throw(error(type_error(atom,M),current_predicate/1)).\n"
    "'$cp_mq'(M,P):-nonvar(P),\\+P=_/_,!,throw(error(type_error(predicate_indicator,M:P),current_predicate/1)).\n"
    "'$cp_mq'(user,P):-!,current_predicate(P).\n"
    "'$cp_mq'(M,N/A):-atom_concat(M,':',Pre),'$cp_mq1'(Pre,N,A).\n"
    "'$cp_mq1'(Pre,N,A):-atom(N),!,(atom_concat(Pre,N,QN),current_predicate(QN/A);current_predicate(N/A),atom_concat(Pre,N,QN),\\+current_predicate(QN/A)).\n"
    "'$cp_mq1'(Pre,N,A):-current_predicate(QN/A),atom_concat(Pre,N,QN).\n"
    "'$cp_mq1'(Pre,N,A):-current_predicate(N/A),\\+sub_atom(N,_,_,_,':'),atom_concat(Pre,N,QN),\\+current_predicate(QN/A).\n"
    "'$mod_head'(M,T,QT):-atom(M),M\\==user,nonvar(T),(T=(H:-B)->'$mod_q1'(M,H,QH),QT=(QH:-B);'$mod_q1'(M,T,QT)),!.\n"
    "'$mod_head'(_,T,T).\n"
    "'$mod_call'(M,G):-'$mod_qb'(M,G,Q),call(Q).\n"
    "translate_bindings(B0,B,_,_,residuals([],[])):-copy_term(B0,B1),'$tb_name'(B1),'$tb_join'(B1,B).\n"
    "get_prolog_backtrace(Max,Frames):-'$pl_backtrace'(Max,Frames).\n"
    "get_prolog_backtrace(Max,Frames,_):-'$pl_backtrace'(Max,Frames).\n"
    "'$tb_name'([]).\n"
    "'$tb_name'([N=V|T]):-(var(V)->V='$VAR'(N);true),'$tb_name'(T).\n"
    "'$tb_join'([],[]).\n"
    "'$tb_join'([N=V|T],R):-V=='$VAR'(N),!,'$tb_join'(T,R).\n"
    "'$tb_join'([N=V|T],[binding([N|Ns],V,[])|R]):-'$tb_same'(T,V,Ns,T1),'$tb_join'(T1,R).\n"
    "'$tb_same'([],_,[],[]).\n"
    "'$tb_same'([N=V1|T],V,[N|Ns],T1):-V1==V,!,'$tb_same'(T,V,Ns,T1).\n"
    "'$tb_same'([X|T],V,Ns,[X|T1]):-'$tb_same'(T,V,Ns,T1).\n"
    "'$mod_qb'(_,V,V):-var(V),!.\n"
    "'$mod_qb'(M,(A,B),(QA,QB)):-!,'$mod_qb'(M,A,QA),'$mod_qb'(M,B,QB).\n"
    "'$mod_qb'(M,(A;B),(QA;QB)):-!,'$mod_qb'(M,A,QA),'$mod_qb'(M,B,QB).\n"
    "'$mod_qb'(M,(A->B),(QA->QB)):-!,'$mod_qb'(M,A,QA),'$mod_qb'(M,B,QB).\n"
    "'$mod_qb'(M,(A*->B),(QA*->QB)):-!,'$mod_qb'(M,A,QA),'$mod_qb'(M,B,QB).\n"
    "'$mod_qb'(M,\\+A,\\+QA):-!,'$mod_qb'(M,A,QA).\n"
    "'$mod_qb'(_,Q:G,Q:G):-var(Q),!.\n"
    "'$mod_qb'(_,Q:G,QG):-!,'$mod_qb'(Q,G,QG).\n"
    "'$mod_qb'(M,G,QG):-compound(G),'$lfx_meta'(G,Sp),!,G=..[F|As],Sp=..[_|Ms],'$mod_qmargs'(Ms,As,M,As2),G2=..[F|As2],'$mod_head'(M,G2,QG).\n"
    "'$mod_qmargs'([],[],_,[]).\n"
    "'$mod_qmargs'([S|Ss],[A|As],M,[B|Bs]):-(S==0->'$mod_qb'(M,A,B);B=A),'$mod_qmargs'(Ss,As,M,Bs).\n"
    "'$mod_qb'(M,G,QG):-'$mod_head'(M,G,QG).\n"
    "'$mod_q1'(M,H,QH):-callable(H),functor(H,F,N),atom(F),atom_concat(M,':',P),atom_concat(P,F,QF),current_predicate(QF/N),H=..[_|As],QH=..[QF|As].\n"
    "'$mod_cl'(M,G,E,G2):-atom(M),M\\==user,callable(G),functor(G,F,N),atom(F),atom_concat(M,':',P),atom_concat(P,F,QF),N2 is N+E,current_predicate(QF/N2),G=..[_|As],"
        "G2=..[QF|As].\n"
    "'$tbl_call'(G,I):-'$tbl_get'(G,I,plain,T),'$tbl_answer'(T,G).\n"
    "'$tbl_mcall'(G,I,S):-G=..[F|As],I=..[Fi|_],S=..[_|Ms0],'$tbl_msplit'(Ms0,As,Gs,Xs,Cs,Vs,Ms),'$tbl_mcheck'(Cs),Gv=..[F|Gs],Iv=..[Fi|Gs],'$tbl_mterm'(Cs,Ma),'$tbl_mterm'(Vs,Mv),"
        "'$tbl_get'(Gv,Iv,moded(Ms,Xs,Mv),T),'$tbl_manswer'(T,Xs,Ma).\n"
    "'$tbl_get'(G,I,K,T1):-'$tbl_noattv'(G),'$tbl_vt'(VT),('$trie_lookup'(VT,G,T)->true;T=none),'$tbl_enter'(T,VT,G,I,K,T0),('$trie_lookup'(VT,G,T1)->true;T1=T0),'$trie_meta_get'(T1,0,S),(S==complete->true;nb_setval('$tbl_dep',true)).\n"
    "'$tbl_noattv'(G):-term_attvars(G,[]),!.\n"
    "'$tbl_noattv'(G):-throw(error(type_error(free_of_attvar,G),_)).\n"
    "'$tbl_cell'(Key,D,V):-catch(nb_getval(Key,V),error(existence_error(_,_),_),V=D).\n"
    "'$tbl_bcell'(Key,D,V):-catch(b_getval(Key,V),error(existence_error(_,_),_),V=D).\n"
    "'$tbl_msplit'([],[],[],[],[],[],[]).\n"
    "'$tbl_msplit'([M|Ms],[A|As],[A|Gs],[A|Xs],Cs,Vs,Mo):-'$tbl_mindex'(M),!,'$tbl_msplit'(Ms,As,Gs,Xs,Cs,Vs,Mo).\n"
    "'$tbl_msplit'([M|Ms],[A|As],[V|Gs],Xs,[A|Cs],[V|Vs],[M|Mo]):-'$tbl_msplit'(Ms,As,Gs,Xs,Cs,Vs,Mo).\n"
    "'$tbl_mindex'(M):-var(M),!.\n"
    "'$tbl_mindex'(index).\n"
    "'$tbl_mindex'('+').\n"
    "'$tbl_mcheck'([]).\n"
    "'$tbl_mcheck'([C|Cs]):-(var(C)->'$tbl_mcheck'(Cs);throw(error(uninstantiation_error(C),_))).\n"
    "'$tbl_mterm'([X],X):-!.\n"
    "'$tbl_mterm'(L,T):-T=..[s|L].\n"
    "'$tbl_enter'(none,VT,G,I,K,T):-!,'$trie_new'(T),'$trie_meta_set'(T,0,incomplete),'$trie_meta_set'(T,1,d(G,I,K,none,none)),'$trie_insert'(VT,0,G,T,_),'$tbl_inprog'(Ts),"
        "nb_setval('$tbl_inprog',[T|Ts]),('$tbl_leading'->'$tbl_eval1'(T);'$tbl_lead'(T)).\n"
    "'$tbl_enter'(T,_,_,_,_,T).\n"
    "'$tbl_leading':-'$tbl_cell'('$tbl_leader',false,L),L==true.\n"
    "'$tbl_inprog'(Ts):-'$tbl_cell'('$tbl_inprog',[],Ts).\n"
    "'$tbl_lead'(T):-nb_setval('$tbl_leader',true),nb_setval('$tbl_wfs',false),nb_setval('$tbl_ph',0),catch(('$tbl_eval1'(T),'$tbl_rounds','$tbl_wfsrun'),E,('$tbl_abort',throw(E))),'$tbl_inprog'(Ts),"
        "'$tbl_complete'(Ts),nb_setval('$tbl_inprog',[]),nb_setval('$tbl_ph',0),nb_setval('$tbl_wfs',false),nb_setval('$tbl_leader',false).\n"
    "'$tbl_eval1'(T):-'$tbl_cell'('$tbl_dep',false,D0),nb_setval('$tbl_dep',false),'$tbl_eval'(T,_),'$tbl_cell'('$tbl_dep',false,D),(D==true->true;'$tbl_done'(T)),nb_setval('$tbl_dep',D0).\n"
    "'$tbl_done'(T):-('$tbl_ph'(r)->'$tbl_final1'(T);true),'$trie_meta_set'(T,0,complete),'$tbl_inprog'(Ts),'$tbl_del'(Ts,T,Ts1),nb_setval('$tbl_inprog',Ts1).\n"
    "'$tbl_wfsrun':-'$tbl_cell'('$tbl_wfs',false,F),(F==true->'$tbl_alt'(0);true).\n"
    "'$tbl_alt'(N0):-'$tbl_inprog'(Ts),'$tbl_snapw'(Ts,u),nb_setval('$tbl_wnew',false),'$tbl_reset'(t),'$tbl_rounds','$tbl_cell'('$tbl_wnew',false,WN),'$tbl_alt2'(WN,N0).\n"
    "'$tbl_alt2'(true,N0):-!,'$tbl_reset'(u),'$tbl_rounds','$tbl_alt'(N0).\n"
    "'$tbl_alt2'(_,N0):-'$tbl_inprog'(Ts2),'$tbl_count'(Ts2,0,N1),'$tbl_snapw'(Ts2,t),(N1=:=N0->'$tbl_reset'(r),'$tbl_rounds','$tbl_inprog'(Ts3),'$tbl_final'(Ts3);'$tbl_reset'(u),'$tbl_rounds','$tbl_alt'(N1)).\n"
    "'$tbl_reset'(P):-nb_setval('$tbl_ph',P),'$tbl_inprog'(Ts),'$tbl_vt'(VT),'$tbl_renew'(Ts,VT,Ts2),nb_setval('$tbl_inprog',Ts2).\n"
    "'$tbl_renew'([],_,[]).\n"
    "'$tbl_renew'([T|Ts],VT,[T2|Ts2]):-'$trie_meta_get'(T,1,d(G,I,K,_,W)),'$trie_new'(T2),'$trie_meta_set'(T2,0,incomplete),'$trie_meta_set'(T2,1,d(G,I,K,none,W)),'$trie_delete'(VT,G,_),'$trie_insert'(VT,0,G,T2,_),'$trie_destroy'(T),'$tbl_renew'(Ts,VT,Ts2).\n"
    "'$tbl_count'([],N,N).\n"
    "'$tbl_count'([T|Ts],N0,N):-'$trie_entries'(T,L),length(L,K),N1 is N0+K,'$tbl_count'(Ts,N1,N).\n"
    "'$tbl_snapw'([],_).\n"
    "'$tbl_snapw'([T|Ts],Which):-'$trie_meta_get'(T,1,d(G,I,K,Sn,W0)),(W0==none->W1=w(none,none);W1=W0),W1=w(TS,US),(Which==t->Old=TS;Old=US),(Old==none->true;'$trie_destroy'(Old)),'$trie_new'(S),'$trie_entries'(T,L),'$tbl_snapins'(L,S),(Which==t->W2=w(S,US);W2=w(TS,S)),'$trie_meta_set'(T,1,d(G,I,K,Sn,W2)),'$tbl_snapw'(Ts,Which).\n"
    "'$tbl_snapins'([],_).\n"
    "'$tbl_snapins'([e(_,Sk,_)|L],S):-'$trie_insert'(S,0,Sk,true,_),'$tbl_snapins'(L,S).\n"
    "'$tbl_final'([]).\n"
    "'$tbl_final'([T|Ts]):-'$tbl_final1'(T),'$tbl_final'(Ts).\n"
    "'$tbl_final1'(T):-'$trie_entries'(T,L),'$tbl_finvals'(L,T),'$trie_meta_get'(T,1,d(G,I,K,Sn,W)),(W=w(TS,US)->'$tbl_wdrop'(TS),'$tbl_wdrop'(US);true),'$trie_meta_set'(T,1,d(G,I,K,Sn,none)).\n"
    "'$tbl_wdrop'(S):-(S==none->true;'$trie_destroy'(S)).\n"
    "'$tbl_finvals'([],_).\n"
    "'$tbl_finvals'([e(_,Sk,V)|L],T):-(V=r(P)->(member(_-[],P)->NV=true;NV=u(P)),'$trie_insert'(T,1,Sk,NV,_);true),'$tbl_finvals'(L,T).\n"
    "tnot(G):-'$tnot_split'(G,G1),'$tbl_vt'(VT),'$trie_lookup'(VT,G1,T),!,'$trie_meta_get'(T,0,S),'$tnot_st'(S,T,G1).\n"
    "tnot(G):-'$tnot_split'(G,G1),'$tnot_impl'(G1,I),!,'$tbl_get'(G1,I,plain,T),'$trie_meta_get'(T,0,S),'$tnot_st'(S,T,G1).\n"
    "tnot(G):- \\+ G.\n"
    "'$tnot_split'(G,_):-var(G),!,throw(error(instantiation_error,tnot/1)).\n"
    "'$tnot_split'(M:G,G1):-atom(M),!,('$mod_cl'(M,G,0,G1)->true;G1=G).\n"
    "'$tnot_split'(G,G).\n"
    "'$tnot_impl'(undefined,'$tbl undefined'):-!.\n"
    "'$tnot_impl'(G,I):-callable(G),G=..[F|As],atom(F),atom_concat('$tbl ',F,TF),length(As,N),current_predicate(TF/N),I=..[TF|As].\n"
    "'$tnot_st'(complete,T,G):-!,'$trie_entries'(T,L),(member(e(_,_,true),L)->fail;L==[]->true;'$tbl_ph'(P),'$tnot_u'(P,G)).\n"
    "'$tnot_st'(_,T,G):-'$tbl_raise','$tbl_ph'(P),'$tnot_i'(P,T,G).\n"
    "'$tnot_u'(t,_):-!,'$tbl_raise',fail.\n"
    "'$tnot_u'(r,G):-!,'$tbl_dlpush'(tnot(G)).\n"
    "'$tnot_u'(_,G):-('$tbl_leading'->'$tbl_raise';'$tbl_dlpush'(tnot(G))).\n"
    "'$tnot_i'(t,T,_):-!,('$tbl_w'(T,w(_,US)),US\\==none-> \\+ '$tbl_wany'(T,u);nb_setval('$tbl_wnew',true)).\n"
    "'$tnot_i'(r,T,G):-!,\\+ '$tbl_wany'(T,t),('$tbl_wany'(T,u)->'$tbl_dlpush'(tnot(G));true).\n"
    "'$tnot_i'(_,T,_):- \\+ '$tbl_wany'(T,t).\n"
    "undefined:-'$tbl_call'(undefined,'$tbl undefined').\n"
    "'$tbl undefined':-tnot(undefined).\n"
    "not_exists(G):-tnot(G).\n"
    "get_residual(G0,DL):-'$tnot_split'(G0,G),'$tbl_vt'(VT),'$trie_entries'(VT,L),member(e(_,V,T),L),'$trie_meta_get'(T,0,complete),\\+ \\+ V=G,copy_term(V,Vc),Vc=G,term_variables(Vc,Vs),Sk=..[ret|Vs],'$trie_entries'(T,As),member(e(_,Sk,Val),As),'$tbl_resid'(Val,Sk,D),'$tbl_unql'(D,DL).\n"
    "call_residual_program(G,P):-'$call_residual_m'(user,G,P).\n"
    "'$call_residual_m'(M,G,P):-'$call_delays_m'(M,G,_),'$tbl_rprog'([G],M,[],P).\n"
    "'$tbl_rprog'([],_,_,[]).\n"
    "'$tbl_rprog'([A|As],M,Seen,P):-member(S,Seen),S=@=A,!,'$tbl_rprog'(As,M,Seen,P).\n"
    "'$tbl_rprog'([A|As],M,Seen,P):-findall(A-D,('$tbl_resq'(A,D),D\\==[]),Rs),'$tbl_rcls'(Rs,M,Cs,New),append(As,New,As2),'$tbl_rprog'(As2,M,[A|Seen],P2),append(Cs,P2,P).\n"
    "'$tbl_rcls'([],_,[],[]).\n"
    "'$tbl_rcls'([A-D|Rs],M,[(H:-B)|Cs],New):-'$tbl_unqem'(A,M,H),'$tbl_unqlm'(D,M,Du),'$tbl_conj'(Du,B),'$tbl_ratoms'(D,N1),'$tbl_rcls'(Rs,M,Cs,N2),append(N1,N2,New).\n"
    "'$tbl_ratoms'([],[]).\n"
    "'$tbl_ratoms'([tnot(A)|Ls],[A|As]):-!,'$tbl_ratoms'(Ls,As).\n"
    "'$tbl_ratoms'([A|Ls],[A|As]):-'$tbl_ratoms'(Ls,As).\n"
    "'$tbl_resq'(G,D):-'$tbl_vt'(VT),'$trie_entries'(VT,L),member(e(_,V,T),L),'$trie_meta_get'(T,0,complete),\\+ \\+ V=G,copy_term(V,Vc),Vc=G,term_variables(Vc,Vs),Sk=..[ret|Vs],"
        "'$trie_entries'(T,As),member(e(_,Sk,Val),As),'$tbl_resid'(Val,Sk,D).\n"
    "'$tbl_resid'(true,_,[]).\n"
    "'$tbl_resid'(u(P),Sk,D):-member(Sk2-D,P),Sk2=Sk.\n"
    "sk_not(G):-not_exists(G).\n"
    "get_calls(MG,T,R):-'$tbl_strip'(MG,M,G),'$tbl_vt'(VT),'$trie_entries'(VT,L),member(e(_,_,I),L),T='$trie'(I),'$tbl_table_status'(T,_,M:G,R).\n"
    "get_returns(T,R):-'$trie_id'(T,I),'$trie_entries'(I,As),member(e(_,R,_),As).\n"
    "get_returns(T,R,N):-'$trie_id'(T,I),'$trie_entries'(I,As),'$gr_idx'(As,1,R,N).\n"
    "'$gr_idx'([e(_,R,_)|_],N,R,N).\n"
    "'$gr_idx'([_|As],N0,R,N):-N1 is N0+1,'$gr_idx'(As,N1,R,N).\n"
    "get_returns_for_call(MV,A):-current_table(MV,T),'$tbl_table_status'(T,_,_:A,Sk),'$trie_id'(T,I),'$trie_entries'(I,As),member(e(_,Sk,_),As).\n"
    "'$tbl_unql'([],[]).\n"
    "'$tbl_unql'([E|Es],[F|Fs]):-'$tbl_unqe'(E,F),'$tbl_unql'(Es,Fs).\n"
    "'$tbl_unqe'(tnot(G),tnot(G2)):-!,'$tbl_unqual'(G,_,G2).\n"
    "'$tbl_unqe'(G,G2):-'$tbl_unqual'(G,_,G2).\n"
    "call_delays(G,D):-'$call_delays_m'(user,G,D).\n"
    "'$call_delays_m'(M,G,D):-'$tbl_bcell'('$tbl_dl',[],D0),b_setval('$tbl_dl',[]),call(G),'$tbl_bcell'('$tbl_dl',[],DL),reverse(DL,DL1),'$tbl_unqlm'(DL1,M,DL2),'$tbl_conj'(DL2,D),"
        "append(DL,D0,D1),b_setval('$tbl_dl',D1).\n"
    "'$tbl_unqlm'([],_,[]).\n"
    "'$tbl_unqlm'([E|Es],M,[F|Fs]):-'$tbl_unqem'(E,M,F),'$tbl_unqlm'(Es,M,Fs).\n"
    "'$tbl_unqem'(tnot(G),M,tnot(G2)):-!,'$tbl_unqem'(G,M,G2).\n"
    "'$tbl_unqem'(G,M,G3):-'$tbl_unqual'(G,Mg,G2),(Mg==M->G3=G2;Mg==user->G3=G2;G3=Mg:G2).\n"
    "'$tbl_conj'([],true):-!.\n"
    "'$tbl_conj'([X],X):-!.\n"
    "'$tbl_conj'([X|Xs],(X,C)):-'$tbl_conj'(Xs,C).\n"
    "'$tbl_del'([],_,[]).\n"
    "'$tbl_del'([K|R],T,S):-(K==T->S=R;S=[K|S1],'$tbl_del'(R,T,S1)).\n"
    "'$tbl_rounds':-repeat,'$tbl_inprog'(Ks0),(Ks0==[]->!;length(Ks0,N0),'$tbl_round'(Ks0,0,C),'$tbl_inprog'(Ks1),length(Ks1,N1),(C+N1-N0>0->fail;!)).\n"
    "'$tbl_round'([],C,C).\n"
    "'$tbl_round'([K|T],C0,C):-'$tbl_eval'(K,N),C1 is C0+N,'$tbl_round'(T,C1,C).\n"
    "'$tbl_eval'(T,N):-'$trie_meta_get'(T,1,d(G,_,_,_,_)),ground(G),'$tbl_gshort'(T,N),!.\n"
    "'$tbl_gshort'(T,0):- \\+ '$tbl_ph'(r),'$trie_entries'(T,[_|_]),!.\n"
    "'$tbl_gshort'(T,N):-'$tbl_ph'(r),'$tbl_wtin'(T,ret),('$trie_entries'(T,[_|_])->N=0;'$trie_insert'(T,0,ret,r([ret-[]]),_),N=1).\n"
    "'$tbl_eval'(T,N):-'$trie_meta_get'(T,1,d(G,I,K,Sn,W)),'$tbl_bcell'('$tbl_ctx',all,X0),'$tbl_bcell'('$tbl_cpos',0,P0),'$tbl_cell'('$tbl_mx',0,M0),"
        "catch('$tbl_runs'(Sn,G,I,Fs,Mx),E,(nb_setval('$tbl_mx',M0),throw(E))),b_setval('$tbl_ctx',X0),b_setval('$tbl_cpos',P0),nb_setval('$tbl_mx',M0),"
        "'$tbl_inprog'(Ts),'$tbl_snap'(Ts,Ps),'$trie_meta_set'(T,1,d(G,I,K,s(Mx,Ps),W)),'$tbl_addk'(K,Fs,T,G,N).\n"
    "'$tbl_runs'(none,G,I,Fs,Mx):-!,b_setval('$tbl_ctx',all),b_setval('$tbl_cpos',0),nb_setval('$tbl_mx',0),'$tbl_fa'(G,I,Fs),'$tbl_cell'('$tbl_mx',0,Mx).\n"
    "'$tbl_runs'(s(Mx0,Ps),G,I,Fs,Mx):-nb_setval('$tbl_mx',Mx0),'$tbl_run'(1,Ps,G,I,Fs),'$tbl_cell'('$tbl_mx',0,Mx).\n"
    "'$tbl_run'(R,Ps,G,I,Fs):-'$tbl_cell'('$tbl_mx',0,M),(R>M->Fs=[];b_setval('$tbl_ctx',sn(R,Ps)),b_setval('$tbl_cpos',0),'$tbl_fa'(G,(I,b_getval('$tbl_cpos',J),J>=R),F1),"
        "R1 is R+1,'$tbl_run'(R1,Ps,G,I,F2),'$tbl_app'(F1,F2,Fs)).\n"
    "'$tbl_app'([],L,L).\n"
    "'$tbl_app'([X|Xs],L,[X|Ys]):-'$tbl_app'(Xs,L,Ys).\n"
    "'$tbl_snap'([],[]).\n"
    "'$tbl_snapat'([],_,0).\n"
    "'$tbl_snapat'([K-V|Ps],C,E):-(K==C->E=V;'$tbl_snapat'(Ps,C,E)).\n"
    "'$tbl_snap'([C|Cs],[C-E|Ps]):-'$trie_slice'(C,0,0,E-_),'$tbl_snap'(Cs,Ps).\n"
    "'$tbl_slice'(T,L):-'$trie_meta_get'(T,0,S),(S==complete->'$trie_entries'(T,L);'$tbl_range'(T,F,To),'$trie_slice'(T,F,To,_-L)).\n"
    "'$tbl_range'(C,F,To):-'$tbl_bcell'('$tbl_cpos',0,J0),J is J0+1,b_setval('$tbl_cpos',J),'$tbl_cell'('$tbl_mx',0,M),(J>M->nb_setval('$tbl_mx',J);true),"
        "'$tbl_bcell'('$tbl_ctx',all,X),(X=sn(R,Ps)->'$tbl_snapat'(Ps,C,E),(J<R->F=0,To=E;J=:=R->F=E,To= -1;F=0,To= -1);F=0,To= -1).\n"
    "'$tbl_addk'(plain,Fs,T,G,N):-'$tbl_ph'(r),!,'$tbl_radd'(Fs,T,G,0,N).\n"
    "'$tbl_addk'(plain,Fs,T,G,N):-'$tbl_add'(Fs,T,G,0,N).\n"
    "'$tbl_addk'(moded(Ms,Xs,Mv),Fs0,T,G,N):-('$tbl_ph'(r)->'$tbl_unpair'(Fs0,Fs);Fs=Fs0),'$tbl_madd'(Fs,T,G-Xs-Mv,Ms,0,N).\n"
    "'$tbl_unpair'([],[]).\n"
    "'$tbl_unpair'([A-_|R],[A|S]):-'$tbl_unpair'(R,S).\n"
    "'$tbl_radd'([],_,_,N,N).\n"
    "'$tbl_radd'([A-D|As],T,G,N0,N):-copy_term(G,Gc),term_variables(Gc,Vs),Gc=A,Sk=..[ret|Vs],reverse(D,D1),'$tbl_rput'(T,Sk,D1,N0,N1),'$tbl_radd'(As,T,G,N1,N).\n"
    "'$tbl_rput'(T,Sk,D,N0,N):-'$trie_lookup'(T,Sk,r(P0)),!,('$tbl_rhas'(P0,Sk-D)->N=N0;'$trie_insert'(T,1,Sk,r([Sk-D|P0]),_),N is N0+1).\n"
    "'$tbl_rput'(T,Sk,D,N0,N):-'$trie_insert'(T,0,Sk,r([Sk-D]),_),N is N0+1.\n"
    "'$tbl_rhas'(P,X):-member(Y,P),'$tbl_same'(Y,X),!.\n"
    "'$tbl_fa'(G,Goal,Fs):-'$tbl_ph'(r),!,'$tbl_bcell'('$tbl_dl',[],D0),findall(G-D,(b_setval('$tbl_dl',[]),Goal,'$tbl_bcell'('$tbl_dl',[],D)),Fs),b_setval('$tbl_dl',D0).\n"
    "'$tbl_fa'(G,Goal,Fs):-ground(G),!,(once(Goal)->Fs=[G];Fs=[]).\n"
    "'$tbl_fa'(G,Goal,Fs):-findall(G,Goal,Fs).\n"
    "'$tbl_ph'(P):-'$tbl_cell'('$tbl_ph',0,P0),P=P0.\n"
    "'$tbl_raise':-nb_setval('$tbl_wfs',true),nb_setval('$tbl_dep',true).\n"
    "'$tbl_dlpush'(E):-'$tbl_bcell'('$tbl_dl',[],L),b_setval('$tbl_dl',[E|L]).\n"
    "'$tbl_w'(T,W):-'$trie_meta_get'(T,1,d(_,_,_,_,W0)),(W0==none->W=w(none,none);W=W0).\n"
    "'$tbl_wany'(T,Which):-'$tbl_w'(T,w(TS,US)),(Which==t->S=TS;S=US),S\\==none,'$trie_entries'(S,[_|_]),!.\n"
    "'$tbl_wtin'(T,Sk):-'$tbl_w'(T,w(TS,_)),TS\\==none,'$trie_lookup'(TS,Sk,_),!.\n"
    "'$tbl_add'([],_,_,N,N).\n"
    "'$tbl_add'([A|As],T,G,N0,N):-copy_term(G,Gc),term_variables(Gc,Vs),Gc=A,Sk=..[ret|Vs],'$trie_insert'(T,0,Sk,true,R),(R>=0->N1 is N0+1;N1=N0),'$tbl_add'(As,T,G,N1,N).\n"
    "'$tbl_answer'(T,G):-term_variables(G,Vs),Sk=..[ret|Vs],'$tbl_slice'(T,L),member(e(_,Sk,V),L),'$tbl_aval'(V,T,G,Sk).\n"
    "'$tbl_aval'(true,_,_,_):-!.\n"
    "'$tbl_aval'(r(_),T,G,Sk):-!,('$tbl_wtin'(T,Sk)->true;'$tbl_dlpush'(G)).\n"
    "'$tbl_aval'(u(_),_,G,_):-'$tbl_ph'(P),'$tbl_ucons'(P,G).\n"
    "'$tbl_ucons'(t,_):-!,'$tbl_raise',fail.\n"
    "'$tbl_ucons'(r,G):-!,'$tbl_dlpush'(G).\n"
    "'$tbl_ucons'(_,G):-('$tbl_leading'->'$tbl_raise';'$tbl_dlpush'(G)).\n"
    "'$tbl_manswer'(T,Xs,Ma):-term_variables(Xs,Vs),Sk=..[ret|Vs],'$tbl_slice'(T,L),member(e(_,Sk,V),L),Ma=V.\n"
    "'$tbl_madd'([],_,_,_,N,N).\n"
    "'$tbl_madd'([A|As],T,P,Ms,N0,N):-copy_term(P,Gc-Xc-Mc),term_variables(Xc,Vs),Gc=A,Sk=..[ret|Vs],'$tbl_mput'(T,Sk,Ms,Mc,N0,N1),'$tbl_madd'(As,T,P,Ms,N1,N).\n"
    "'$tbl_mput'(T,Sk,Ms,M,N0,N):-'$trie_lookup'(T,Sk,Old),!,('$tbl_mupd'(Ms,Old,M,New)->true;New=Old),"
        "('$tbl_same'(New,Old)->N=N0;'$trie_delete'(T,Sk,_),'$trie_insert'(T,0,Sk,New,_),N is N0+1).\n"
    "'$tbl_mput'(T,Sk,_,M,N0,N):-'$trie_insert'(T,0,Sk,M,Q),(Q>=0->N is N0+1;N=N0).\n"
    "'$tbl_same'(A,B):-copy_term(A-B,A1-B1),numbervars(A1,0,N),numbervars(B1,0,N),A1==B1.\n"
    "'$tbl_mupd'([M],O,N,R):-!,'$tbl_mupd1'(M,O,N,R).\n"
    "'$tbl_mupd'(Ms,O,N,R):-O=..[s|Os],N=..[s|Ns],'$tbl_mupds'(Ms,Os,Ns,Rs),R=..[s|Rs].\n"
    "'$tbl_mupds'([],[],[],[]).\n"
    "'$tbl_mupds'([M|Ms],[O|Os],[N|Ns],[R|Rs]):-'$tbl_mupd1'(M,O,N,R),'$tbl_mupds'(Ms,Os,Ns,Rs).\n"
    "'$tbl_mupd1'(first,O,_,O):-!.\n"
    "'$tbl_mupd1'('-',O,_,O):-!.\n"
    "'$tbl_mupd1'(last,_,N,N):-!.\n"
    "'$tbl_mupd1'(min,O,N,R):-!,(O@<N->R=O;R=N).\n"
    "'$tbl_mupd1'(max,O,N,R):-!,(O@>N->R=O;R=N).\n"
    "'$tbl_mupd1'(sum,O,N,R):-!,R is O+N.\n"
    "'$tbl_mupd1'(lattice(P),O,N,R):-!,'$tbl_mclos'(P,C),call(C,O,N,R),!.\n"
    "'$tbl_mupd1'(po(P),O,N,R):-!,'$tbl_mclos'(P,C),(call(C,O,N)->R=O;R=N).\n"
    "'$tbl_mupd1'(M,_,_,_):-throw(error(domain_error(tabled_mode,M),_)).\n"
    "'$tbl_mclos'(M:P,M:C):-!,'$tbl_mclos'(P,C).\n"
    "'$tbl_mclos'(N/_,N):-!.\n"
    "'$tbl_mclos'(H,N):-compound(H),!,functor(H,N,_).\n"
    "'$tbl_mclos'(N,N).\n"
    "'$tbl_complete'([]).\n"
    "'$tbl_complete'([T|Ts]):-'$trie_meta_set'(T,0,complete),'$tbl_complete'(Ts).\n"
    "'$tbl_abort':-'$tbl_inprog'(Ts),'$tbl_vt'(VT),'$tbl_drop'(Ts,VT),nb_setval('$tbl_inprog',[]),nb_setval('$tbl_leader',false),nb_setval('$tbl_dep',false),nb_setval('$tbl_ph',0),nb_setval('$tbl_wfs',false).\n"
    "'$tbl_drop'([],_).\n"
    "'$tbl_drop'([T|Ts],VT):-('$tbl_dgoal'(T,G),'$trie_delete'(VT,G,_)->true;true),'$trie_destroy'(T),'$tbl_drop'(Ts,VT).\n"
    "'$tbl_dgoal'(T,G):-'$trie_meta_get'(T,1,d(G,_,_,_,_)).\n"
    "abolish_all_tables:-'$tbl_leading',!,throw(error(permission_error(abolish,table,all),abolish_all_tables/0)).\n"
    "abolish_all_tables:-'$tbl_vt'(VT),'$trie_entries'(VT,L),'$tbl_drop_all'(L),'$trie_destroy'(VT).\n"
    "'$tbl_drop_all'([]).\n"
    "'$tbl_drop_all'([e(_,_,T)|L]):-'$trie_destroy'(T),'$tbl_drop_all'(L).\n"
    "'$tbl_local_variant_table'('$trie'(VT)):-'$tbl_vt'(VT).\n"
    "current_table(MV,T):-'$tbl_strip'(MV,M,V),'$tbl_vt'(VT),(nonvar(V)->'$tbl_lookup'(VT,M,V,I);'$trie_entries'(VT,L),member(e(_,QV,I),L),'$tbl_unqual'(QV,M,V)),"
        "T='$trie'(I).\n"
    "'$tbl_strip'(MV,M,V):-nonvar(MV),MV=M0:V0,!,(var(M)->M=M0;true),'$tbl_strip'(V0,M,V).\n"
    "'$tbl_strip'(V,_,V).\n"
    "'$tbl_lookup'(VT,M,V,I):-atom(M),M\\==user,'$tbl_qual'(M,V,QV),'$trie_lookup'(VT,QV,I),!.\n"
    "'$tbl_lookup'(VT,_,V,I):-'$trie_lookup'(VT,V,I).\n"
    "'$tbl_qual'(M,V,QV):-callable(V),V=..[F|As],atom_concat(M,':',P),atom_concat(P,F,QF),QV=..[QF|As].\n"
    "'$tbl_unqual'(QV,M,V):-callable(QV),QV=..[QF|As],atom(QF),sub_atom(QF,B,1,A,':'),B>0,!,sub_atom(QF,0,B,_,M),B1 is B+1,sub_atom(QF,B1,A,0,F),V=..[F|As].\n"
    "'$tbl_unqual'(V,user,V).\n"
    "'$tbl_table_status'(T,S,M:W,Sk):-'$trie_id'(T,I),'$trie_meta_get'(I,0,S),'$tbl_dgoal'(I,G),'$tbl_unqual'(G,M,W),term_variables(W,Vs),Sk=..[ret|Vs].\n"
    "'$trie_id'(T,_):-var(T),!,throw(error(instantiation_error,_)).\n"
    "'$trie_id'('$trie'(I),I):-integer(I),'$trie_alive'(I),!.\n"
    "'$trie_id'(T,_):-nonvar(T),T='$trie'(I),integer(I),!,throw(error(existence_error(trie,T),_)).\n"
    "'$trie_id'(T,_):-throw(error(type_error(trie,T),_)).\n"
    "'$trie_key_ok'(K):-acyclic_term(K),!.\n"
    "'$trie_key_ok'(K):-throw(error(type_error(acyclic_term,K),_)).\n"
    "'$trie_ins_r'(R,_):-R>=0,!.\n"
    "'$trie_ins_r'(-3,K):-throw(error(permission_error(modify,trie_key,K),_)).\n"
    "'$trie_ins_r'(-4,K):-throw(error(type_error(free_of_attvar,K),_)).\n"
    "trie_new(T):-'$trie_new'(I),T='$trie'(I).\n"
    "is_trie(T):-nonvar(T),T='$trie'(I),integer(I),'$trie_alive'(I).\n"
    "trie_destroy(T):-'$trie_id'(T,I),'$trie_destroy'(I).\n"
    "trie_insert(T,K):-trie_insert(T,K,trienode).\n"
    "trie_insert(T,K,V):-'$trie_id'(T,I),'$trie_key_ok'(K),'$trie_insert'(I,0,K,V,R),'$trie_ins_r'(R,K).\n"
    "trie_insert(T,K,V,'$trie_node'(I,R)):-'$trie_id'(T,I),'$trie_key_ok'(K),'$trie_insert'(I,0,K,V,R),'$trie_ins_r'(R,K).\n"
    "trie_update(T,K,V):-'$trie_id'(T,I),'$trie_key_ok'(K),'$trie_insert'(I,1,K,V,_).\n"
    "trie_lookup(T,K,V):-'$trie_id'(T,I),'$trie_lookup'(I,K,V).\n"
    "trie_delete(T,K,V):-'$trie_id'(T,I),'$trie_delete'(I,K,V).\n"
    "trie_gen(T,K):-trie_gen(T,K,_).\n"
    "trie_gen(T,K,V):-'$trie_id'(T,I),'$trie_entries'(I,L),member(e(_,K,V),L).\n"
    "trie_gen_compiled(T,K):-trie_gen(T,K,_).\n"
    "trie_gen_compiled(T,K,V):-trie_gen(T,K,V).\n"
    "trie_term('$trie_node'(I,E),K):-'$trie_entry_key'(I,E,K).\n"
    "statistics(K,V):-'$stat_list_key'(K),nonvar(V),\\+'$stat_pair'(V),!,throw(error(domain_error(statistics_value,V),statistics/2)).\n"
    "statistics(K,[A,B]):-'$stat_list_key'(K),(\\+'$stat_int'(A);\\+'$stat_int'(B)),!,('$stat_int'(A)->E=B;E=A),throw(error(type_error(integer,E),statistics/2)).\n"
    "statistics(atoms,[N,0]):-!,'$gnu_os'(atoms,0,N).\n"
    "statistics(K,[U,F]):-'$stat_stack_key'(K),!,'$gnu_os'(stack_used,K,U),'$gnu_os'(stack_free,K,F).\n"
    "statistics(runtime,[T,D]):-!,'$cpu_ms'(T),'$stat_delta'('$stat_rt',T,D).\n"
    "statistics(cpu_time,[T,D]):-!,'$cpu_ms'(T),'$stat_delta'('$stat_ct',T,D).\n"
    "statistics(user_time,[T,D]):-!,'$cpu_ms'(T),'$stat_delta'('$stat_ut',T,D).\n"
    "statistics(system_time,[0,0]):-!.\n"
    "statistics(walltime,[T,D]):-!,wall_ms(W),'$stat_w0'(W0),T is W-W0,'$stat_delta'('$stat_wt',T,D).\n"
    "statistics(real_time,[T,D]):-!,wall_ms(W),T is W//1000,'$stat_delta'('$stat_re',T,D).\n"
    "statistics(cputime,S):-!,'$cpu_ms'(T),S is T/1000.\n"
    "statistics(process_cputime,S):-!,'$cpu_ms'(T),S is T/1000.\n"
    "statistics(agc,N):-!,catch(nb_getval('$stat_agc',N),_,N=0).\n"
    "statistics(global_shifts,N):- \\+current_prolog_flag('$dialect',gnu),!,'$gnu_os'(heap_grows,0,N).\n"
    "statistics(local_shifts,0):- \\+current_prolog_flag('$dialect',gnu),!.\n"
    "statistics(trail_shifts,0):- \\+current_prolog_flag('$dialect',gnu),!.\n"
    "statistics(K,_):-var(K),!,throw(error(instantiation_error,statistics/2)).\n"
    "statistics(K,_):-throw(error(domain_error(statistics_key,K),statistics/2)).\n"
    "'$stat_delta'(K,T,D):-catch(nb_getval(K,T0),_,T0=0),D is T-T0,nb_setval(K,T).\n"
    "'$stat_list_key'(K):-memberchk(K,[runtime,cpu_time,user_time,system_time,walltime,real_time,atoms,local_stack,global_stack,trail_stack,cstr_stack]).\n"
    "'$stat_stack_key'(K):-memberchk(K,[local_stack,global_stack,trail_stack,cstr_stack]).\n"
    "'$stat_pair'(V):-nonvar(V),V=[_,_].\n"
    "'$stat_int'(X):-(var(X);integer(X)),!.\n"
    "'$stat_w0'(W0):-catch(nb_getval('$stat_w0',W0),_,(wall_ms(W0),nb_setval('$stat_w0',W0))).\n"
    "prompt(O,N):-catch(nb_getval('$prompt',O0),_,O0='|: '),O=O0,(atom(N)->nb_setval('$prompt',N);throw(error(type_error(atom,N),prompt/2))).\n"
    "prompt1(_).\n"
    "'$display_swi'(T):-write_term(T,[quoted(true),ignore_ops(true)]).\n"
    "'$'.\n"
    "'$'(G):- $G.\n"
    "style_check(_).\n"
    "set_module(_).\n"
    "'$clausable'(_).\n"
    "recorda(K,T):-recorda(K,T,_).\n"
    "recordz(K,T):-recordz(K,T,_).\n"
    "recorda(K,T,R):-'$rec_key'(K,K1),'$rec_id'(I),asserta('$rec'(K1,I,T)),R='$record'(I).\n"
    "recordz(K,T,R):-'$rec_key'(K,K1),'$rec_id'(I),assertz('$rec'(K1,I,T)),R='$record'(I).\n"
    "recorded(K,T):-recorded(K,T,_).\n"
    "recorded(K,T,R):-'$rec_key'(K,K1),(nonvar(R)->R='$record'(I);true),'$rec'(K1,I,T),R='$record'(I).\n"
    "'$rec_key'(K,K):-var(K),!.\n"
    "'$rec_key'(K,N/A):-compound(K),!,functor(K,N,A).\n"
    "'$rec_key'(K,K).\n"
    "'$rec_id'(I):-catch(nb_getval('$rec_n',I0),_,I0=0),I is I0+1,nb_setval('$rec_n',I).\n"
    "'$rec_ref'(R):-nonvar(R),R='$record'(_).\n"
    "'$rec_erase'('$record'(I)):-retract('$rec'(_,I,_)),!.\n"
    "'$mq_arg'(A,Q):-(nonvar(A),A=_:_->Q=A;Q=user:A).\n"
    "print_message(_,format(F,A)):-!,catch(format(user_error,F,A),_,true),nl(user_error).\n"
    "print_message(_,_).\n"
    "variant(A,B):-copy_term(A,A1),copy_term(B,B1),numbervars(A1,0,N),numbervars(B1,0,N),A1==B1.\n"
    "memberchk_eq(X,[Y|Ys]):-(X==Y->true;memberchk_eq(X,Ys)).\n"
    "pi_head(PI,H):-var(PI),var(H),!,throw(error(instantiation_error,pi_head/2)).\n"
    "pi_head(M:PI,M:H):-!,pi_head(PI,H).\n"
    "pi_head(N/A,H):-!,functor(H,N,A).\n"
    "pi_head(N//A0,H):-!,(integer(A0)->A is A0+2,functor(H,N,A);functor(H,N,A),A0 is A-2).\n"
    "pi_head(PI,_):-throw(error(type_error(predicate_indicator,PI),pi_head/2)).\n"
    "list_difference_eq([],_,[]).\n"
    "list_difference_eq([X|Xs],Ys,L):-(memberchk_eq(X,Ys)->list_difference_eq(Xs,Ys,L);L=[X|T],list_difference_eq(Xs,Ys,T)).\n"
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
    "'$phrase'(G,S0,S):- current_prolog_flag('$dialect',gnu),!,'$gnu_dcg_body'(G,f,S1,S2,[],t,Goal),S0=S1,S=S2,call(Goal).\n"
    "'$phrase'(G,S0,S):-'$dcg_body'(G,S0,S,Goal),call(Goal).\n"
    "'$dcg_body'(V,S0,S,phrase(V,S0,S)):-var(V),!.\n"
    "'$dcg_body'([],S0,S,S0=S):-!.\n"
    "'$dcg_body'([H|T],S0,S,S0=L):-!,'$dcg_list'([H|T],S,L).\n"
    "'$dcg_body'(Str,S0,S,S0=L):-string(Str),!,string_codes(Str,Cs),'$dcg_list'(Cs,S,L).\n"
    "'$dcg_body'((A,B),S0,S,(GA,GB)):-!,'$dcg_body'(A,S0,S1,GA),'$dcg_body'(B,S1,S,GB).\n"
    "'$dcg_body'((C->T;E),S0,S,(GC->GT;GE)):-!,'$dcg_body'(C,S0,S1,GC),'$dcg_body'(T,S1,S,GT),'$dcg_body'(E,S0,S,GE).\n"
    "'$dcg_body'((C*->T;E),S0,S,(GC*->GT;GE)):-!,'$dcg_body'(C,S0,S1,GC),'$dcg_body'(T,S1,S,GT),'$dcg_body'(E,S0,S,GE).\n"
    "'$dcg_body'((A;B),S0,S,(GA;GB)):-!,'$dcg_body'(A,S0,S,GA),'$dcg_body'(B,S0,S,GB).\n"
    "'$dcg_body'('|'(A,B),S0,S,(GA;GB)):-!,'$dcg_body'(A,S0,S,GA),'$dcg_body'(B,S0,S,GB).\n"
    "'$dcg_body'((C->T),S0,S,(GC->GT)):-!,'$dcg_body'(C,S0,S1,GC),'$dcg_body'(T,S1,S,GT).\n"
    "'$dcg_body'(\\+A,S0,S,(\\+GA,S0=S)):-!,'$dcg_body'(A,S0,_,GA).\n"
    "'$dcg_body'({}(G),S0,S,(G,S0=S)):-!.\n"
    "'$dcg_body'(!,S0,S,(!,S0=S)):-!.\n"
    "'$dcg_body'(NT,_,_,_):- \\+callable(NT),!,throw(error(type_error(callable,NT),_)).\n"
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
    "prolog_load_context(module,M):-'$current_source_module'(M).\n"
    "'$current_source_module'(M):-catch(nb_getval('$source_module',M0),_,M0=user),M=M0.\n"
    "'$set_source_module'(M):-nb_setval('$source_module',M).\n"
    "'$set_source_module'(O,N):-'$current_source_module'(O),'$set_source_module'(N).\n"
    "'$flag_member'(X,[X|_]).\n"
    "'$flag_member'(X,[_|T]):-'$flag_member'(X,T).\n"
    "float_class(F,C):-(var(F)->throw(error(instantiation_error,float_class/2));float(F)->true;throw(error(type_error(float,F),float_class/2))),"
    "(F=\\=F->C0=nan;abs(F)=:=inf->C0=infinite;F=:=0.0->C0=zero;abs(F)<2.2250738585072014e-308->C0=subnormal;C0=normal),C=C0.\n"
    "bounded_number(L,H,N):-(var(N)->throw(error(instantiation_error,bounded_number/3));number(N)->true;throw(error(type_error(number,N),bounded_number/3))),"
    "'$bn_low'(L,N),'$bn_high'(H,N).\n"
    "'$bn_low'(L,N):-var(L),!,(integer(N)->L is N-1;F is float(N),F=:=F,abs(F)=\\=inf,L is nexttoward(F,-inf)).\n"
    "'$bn_low'(L,N):-L<N.\n"
    "'$bn_high'(H,N):-var(H),!,(integer(N)->H is N+1;F is float(N),F=:=F,abs(F)=\\=inf,H is nexttoward(F,inf)).\n"
    "'$bn_high'(H,N):-N<H.\n"
    "format_time(Out,Fmt,T):-format_time(Out,Fmt,T,posix).\n"
    "format_time(Out,Fmt,T,_):-'$ft_fmt'(Fmt,F),'$format_time_text'(F,T,A),'$ft_out'(Out,A).\n"
    "'$ft_fmt'(F,F):-atom(F),!.\n"
    "'$ft_fmt'(F,A):-string(F),!,atom_string(A,F).\n"
    "'$ft_fmt'(F,A):-catch(atom_codes(A,F),_,atom_chars(A,F)).\n"
    "'$ft_out'(O,_):-var(O),!,throw(error(instantiation_error,format_time/3)).\n"
    "'$ft_out'(atom(A),T):-!,A=T.\n"
    "'$ft_out'(string(S),T):-!,atom_string(T,S).\n"
    "'$ft_out'(codes(C),T):-!,atom_codes(T,C).\n"
    "'$ft_out'(codes(C,Tl),T):-!,atom_codes(T,C0),append(C0,Tl,C).\n"
    "'$ft_out'(chars(C),T):-!,atom_chars(T,C).\n"
    "'$ft_out'(chars(C,Tl),T):-!,atom_chars(T,C0),append(C0,Tl,C).\n"
    "'$ft_out'(S,T):-write(S,T).\n"
    "flag(K,Old,New):-'$flag_key'(K,NK),(nb_current(NK,V)->true;V=0),Old=V,(atom(New)->V1=New;V1 is New),nb_setval(NK,V1).\n"
    "get_flag(K,V):-'$flag_key'(K,NK),(nb_current(NK,V0)->V=V0;V=0).\n"
    "set_flag(K,V):-'$flag_key'(K,NK),nb_setval(NK,V).\n"
    "'$flag_key'(K,NK):-format(atom(A),'~q',[K]),atom_concat('$flag ',A,NK).\n"
    "with_mutex(_,G):-once(G).\n"
    "'$retract_target'(C,T):-var(C),!,T=(C:-true).\n"
    "'$retract_target'(M:C,T):-!,'$mod_head'(M,C,QC),'$retract_target'(QC,T).\n"
    "'$retract_target'((H:-B),(H1:-B)):-!,'$retract_head'(H,H1).\n"
    "'$retract_target'(C,(C:-true)).\n"
    "'$retract_head'(H,H):-var(H),!.\n"
    "'$retract_head'(M:H,H1):-!,'$mod_head'(M,H,QH),'$retract_head'(QH,H1).\n"
    "'$retract_head'(H,H).\n"
    "strip_module(T,M,P):-'$strip_module'(T,user,M,P).\n"
    "'$strip_module'(T,_,M,P):-nonvar(T),T=M1:T1,atom(M1),!,'$strip_module'(T1,M1,M,P).\n"
    "'$strip_module'(T,M,M,T).\n"
    "garbage_collect_atoms:-catch(nb_getval('$stat_agc',N0),_,N0=0),N is N0+1,nb_setval('$stat_agc',N).\n"
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
    "'$bagof_var'(T,G,R):-'$bag_goal_ok'(G),'$pl_list_guard'(R),'$bag_wit'(T,G,W,G1),(W=='$w'->findall(T,G1,R0),R0\\==[],R=R0;findall(W-T,G1,Ps0),Ps0\\==[],keysort(Ps0,Ps),'$bag_pick'(Ps,W,R)).\n"
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
    "exclude(P,L,R):-'$exclude_'(L,P,R).\n"
    "'$exclude_'([],_,[]).\n"
    "'$exclude_'([H|T],P,R):-(call(P,H)->R=R1;R=[H|R1]),'$exclude_'(T,P,R1).\n"
    "include(P,L,R):-'$include_'(L,P,R).\n"
    "'$include_'([],_,[]).\n"
    "'$include_'([H|T],P,R):-(call(P,H)->R=[H|R1];R=R1),'$include_'(T,P,R1).\n"
    "list_to_set(L,S):-'$lts_'(L,[],S).\n"
    "'$lts_'([],_,[]).\n"
    "'$lts_'([H|T],Seen,R):-(memberchk_eq(H,Seen)->R=R1;R=[H|R1]),'$lts_'(T,[H|Seen],R1).\n"
    "maplist(G,L):-'$maplist_'(L,G).\n"
    "'$maplist_'([],_).\n"
    "'$maplist_'([X|Xs],G):-call(G,X),'$maplist_'(Xs,G).\n"
    "maplist(G,L1,L2):-'$maplist_'(L1,L2,G).\n"
    "'$maplist_'([],[],_).\n"
    "'$maplist_'([X|Xs],[Y|Ys],G):-call(G,X,Y),'$maplist_'(Xs,Ys,G).\n"
    "maplist(G,L1,L2,L3):-'$maplist_'(L1,L2,L3,G).\n"
    "'$maplist_'([],[],[],_).\n"
    "'$maplist_'([X|Xs],[Y|Ys],[Z|Zs],G):-call(G,X,Y,Z),'$maplist_'(Xs,Ys,Zs,G).\n"
    "maplist(G,L1,L2,L3,L4):-'$maplist_'(L1,L2,L3,L4,G).\n"
    "'$maplist_'([],[],[],[],_).\n"
    "'$maplist_'([W|Ws],[X|Xs],[Y|Ys],[Z|Zs],G):-call(G,W,X,Y,Z),'$maplist_'(Ws,Xs,Ys,Zs,G).\n"
    "'>>'(P,B):-'$yall'(P,B,[]).\n"
    "'>>'(P,B,A1):-'$yall'(P,B,[A1]).\n"
    "'>>'(P,B,A1,A2):-'$yall'(P,B,[A1,A2]).\n"
    "'>>'(P,B,A1,A2,A3):-'$yall'(P,B,[A1,A2,A3]).\n"
    "'>>'(P,B,A1,A2,A3,A4):-'$yall'(P,B,[A1,A2,A3,A4]).\n"
    "'>>'(P,B,A1,A2,A3,A4,A5):-'$yall'(P,B,[A1,A2,A3,A4,A5]).\n"
    "'>>'(P,B,A1,A2,A3,A4,A5,A6):-'$yall'(P,B,[A1,A2,A3,A4,A5,A6]).\n"
    "'>>'(P,B,A1,A2,A3,A4,A5,A6,A7):-'$yall'(P,B,[A1,A2,A3,A4,A5,A6,A7]).\n"
    "'/'(F,L):-'$yall_slash'(F,L,[]).\n"
    "'/'(F,L,A1):-'$yall_slash'(F,L,[A1]).\n"
    "'/'(F,L,A1,A2):-'$yall_slash'(F,L,[A1,A2]).\n"
    "'/'(F,L,A1,A2,A3):-'$yall_slash'(F,L,[A1,A2,A3]).\n"
    "'/'(F,L,A1,A2,A3,A4):-'$yall_slash'(F,L,[A1,A2,A3,A4]).\n"
    "'/'(F,L,A1,A2,A3,A4,A5):-'$yall_slash'(F,L,[A1,A2,A3,A4,A5]).\n"
    "'/'(F,L,A1,A2,A3,A4,A5,A6):-'$yall_slash'(F,L,[A1,A2,A3,A4,A5,A6]).\n"
    "'/'(F,L,A1,A2,A3,A4,A5,A6,A7):-'$yall_slash'(F,L,[A1,A2,A3,A4,A5,A6,A7]).\n"
    "'$yall_slash'(F,L,As):-copy_term(F/L,F1/L1),F1=F,(nonvar(L1),L1=(Ps>>B)->'$yall_bind'(Ps,As,B,G);'$yall_ext'(As,L1,G)),call(G).\n"
    "'$yall'(P,B,As):-copy_term(P>>B,P1>>B1),'$yall_free'(P,P1,Ps),'$yall_bind'(Ps,As,B1,G),call(G).\n"
    "'$yall_free'(P,P1,Ps):-nonvar(P),P=F/_,!,P1=F1/Ps,F1=F.\n"
    "'$yall_free'(_,Ps,Ps).\n"
    "'$yall_bind'([],As,B,G):-!,'$yall_ext'(As,B,G).\n"
    "'$yall_bind'([P|Ps],[A|As],B,G):-!,P=A,'$yall_bind'(Ps,As,B,G).\n"
    "'$yall_bind'(_,[],B,B).\n"
    "'$yall_ext'([],B,B):-!.\n"
    "'$yall_ext'(As,B,G):-B=..L0,'$yall_app'(L0,As,L1),G=..L1.\n"
    "'$yall_app'([],L,L).\n"
    "'$yall_app'([H|T],L,[H|R]):-'$yall_app'(T,L,R).\n"
    "limit(C,G):-C==infinite,!,call(G).\n"
    "limit(C,G):-C>0,S=count(0),call(G),arg(1,S,N0),N is N0+1,(N=:=C->!;nb_setarg(1,S,N)).\n"
    "offset(C,G):-C>0,!,S=count(0),call(G),arg(1,S,N0),(N0>=C->true;N is N0+1,nb_setarg(1,S,N),fail).\n"
    "offset(C,G):-C=:=0,!,call(G).\n"
    "offset(C,_):-throw(error(domain_error(not_less_than_zero,C),offset/2)).\n"
    "'=@='(A,B):- \\+ \\+ '$variant_eq'(A,B).\n"
    "'\\\\=@='(A,B):- \\+ '$variant_eq'(A,B).\n"
    "'$variant_eq'(A,B):-copy_term_nat(A,A1),copy_term_nat(B,B1),term_variables(A1,V1),term_variables(B1,V2),'$variant_bind'(V1,0,N),'$variant_bind'(V2,0,N),A1==B1.\n"
    "'$variant_bind'([],N,N).\n"
    "'$variant_bind'([V|Vs],I,N):-V='$variant_var'(I),I1 is I+1,'$variant_bind'(Vs,I1,N).\n"
    "term_singletons(T,S):-term_variables(T,Vs),'$ts_filter'(Vs,T,S).\n"
    "'$ts_filter'([],_,[]).\n"
    "'$ts_filter'([V|Vs],T,R):-'$ts_occ'(T,V,0,N),(N=:=1->R=[V|R1];R=R1),'$ts_filter'(Vs,T,R1).\n"
    "'$ts_occ'(X,V,N0,N):-var(X),!,(X==V->N is N0+1;N=N0).\n"
    "'$ts_occ'(X,_,N,N):-atomic(X),!.\n"
    "'$ts_occ'(X,V,N0,N):-X=..[_|As],'$ts_occ_l'(As,V,N0,N).\n"
    "'$ts_occ_l'([],_,N,N).\n"
    "'$ts_occ_l'([A|As],V,N0,N):-'$ts_occ'(A,V,N0,N1),(N1>1->N=N1;'$ts_occ_l'(As,V,N1,N)).\n"
    "stream_pair(P,I,O):-nonvar(P),!,'$sp_split'(P,I,O).\n"
    "stream_pair(P,I,O):-('$sp_in'(I),'$sp_out'(O),P='$stream_pair'(I,O)).\n"
    "'$sp_in'(I):-(var(I)->throw(error(instantiation_error,stream_pair/3));stream_property(I,input)->true;throw(error(permission_error(input,stream,I),stream_pair/3))).\n"
    "'$sp_out'(O):-(var(O)->throw(error(instantiation_error,stream_pair/3));stream_property(O,output)->true;throw(error(permission_error(output,stream,O),stream_pair/3))).\n"
    "'$sp_split'('$stream_pair'(A,B),I,O):-!,(catch((stream_property(A,input),stream_property(B,output)),_,fail)->I=A,O=B;true).\n"
    "'$sp_split'(S,I,O):-(stream_property(S,input)->I=S;stream_property(S,output)->O=S;throw(error(existence_error(stream,S),stream_pair/3))).\n"
    "numbervars(T,S,E,O):-'$nv_opts'(O,Sg,At,Fn),'$nv4'(T,S,E,Sg,At,Fn).\n"
    "'$nv_opts'(O,_,_,_):-var(O),!,throw(error(instantiation_error,numbervars/4)).\n"
    "'$nv_opts'(O,Sg,At,Fn):-is_dict(O),!,O=..[_,_|VK],'$nv_dict'(VK,L),'$nv_list'(L,L,false,Sg,error,At,'$VAR',Fn).\n"
    "'$nv_opts'(O,Sg,At,Fn):-'$nv_list'(O,O,false,Sg,error,At,'$VAR',Fn).\n"
    "'$nv_dict'([],[]).\n"
    "'$nv_dict'([V,K|T],[O|R]):-O=..[K,V],'$nv_dict'(T,R).\n"
    "'$nv_list'(L,_,_,_,_,_,_,_):-var(L),!,throw(error(instantiation_error,numbervars/4)).\n"
    "'$nv_list'([],_,S,S,A,A,F,F):-!.\n"
    "'$nv_list'([H|T],All,S0,S,A0,A,F0,F):-!,'$nv_opt'(H,All,S0,S1,A0,A1,F0,F1),'$nv_list'(T,All,S1,S,A1,A,F1,F).\n"
    "'$nv_list'(L,_,_,_,_,_,_,_):-throw(error(type_error(list,L),numbervars/4)).\n"
    "'$nv_opt'(H,_,_,_,_,_,_,_):-var(H),!,throw(error(instantiation_error,numbervars/4)).\n"
    "'$nv_opt'(singletons(V),_,_,S,A,A,F,F):-!,'$nv_bool'(V,S).\n"
    "'$nv_opt'(singletons,_,_,true,A,A,F,F):-!.\n"
    "'$nv_opt'(attvar(V),All,S,S,_,A,F,F):-!,(memberchk(V,[skip,bind,error])->A=V;var(V)->throw(error(instantiation_error,numbervars/4));throw(error(domain_error(numbervar_option,All),numbervars/4))).\n"
    "'$nv_opt'(functor_name(V),_,S,S,A,A,_,F):-!,(var(V)->throw(error(instantiation_error,numbervars/4));atom(V)->F=V;throw(error(type_error(atom,V),numbervars/4))).\n"
    "'$nv_opt'(H,_,S,S,A,A,F,F):-atom(H),!.\n"
    "'$nv_opt'(H,_,S,S,A,A,F,F):-compound(H),functor(H,_,1),!.\n"
    "'$nv_opt'(H,_,_,_,_,_,_,_):-throw(error(type_error(option,H),numbervars/4)).\n"
    "'$nv_bool'(V,_):-var(V),!,throw(error(instantiation_error,numbervars/4)).\n"
    "'$nv_bool'(V,B):-memberchk(V,[true,on]),!,B=true.\n"
    "'$nv_bool'(V,B):-memberchk(V,[false,off]),!,B=false.\n"
    "'$nv_bool'(V,_):-throw(error(type_error(bool,V),numbervars/4)).\n"
    "'$nv4'(T,S,E,Sg,At,Fn):-(Sg==true,acyclic_term(T)->term_singletons(T,Ss),'$nv_single'(Ss,Fn);true),(Fn=='$VAR',At==error->numbervars(T,S,E);'$nv_walk'(T,S,E,At,Fn)).\n"
    "'$nv_single'([],_).\n"
    "'$nv_single'([V|Vs],F):-G=..[F,'_'],V=G,'$nv_single'(Vs,F).\n"
    "'$nv_walk'(T,S,E,At,Fn):-(integer(S)->true;var(S)->throw(error(instantiation_error,numbervars/4));throw(error(type_error(integer,S),numbervars/4))),term_variables(T,Vs),'$nv_bind'(Vs,S,E0,At,Fn),E=E0.\n"
    "'$nv_bind'([],E,E,_,_).\n"
    "'$nv_bind'([V|Vs],N,E,At,Fn):-(At==skip,attvar(V)->'$nv_bind'(Vs,N,E,At,Fn);At==error,attvar(V)->throw(error(type_error(free_of_attvar,V),numbervars/4));G=..[Fn,N],V=G,N1 is N+1,'$nv_bind'(Vs,N1,E,At,Fn)).\n"
    "trim_stacks.\n"
    "'$conj_nocut'(G):-nonvar(G),G=(A,B),!,call(A),'$conj_nocut'(B).\n"
    "'$conj_nocut'(G):-call(G).\n"
    "current_locale(L):-(nb_current('$cur_locale',L0)->L=L0;L=default).\n"
    "locale_property(L0,P):-'$loc_resolve'(L0,L),'$loc_props'(L,Ps),member(P,Ps).\n"
    "'$loc_resolve'(current_locale,L):-!,current_locale(L).\n"
    "'$loc_resolve'(L,L).\n"
    "'$loc_props'(default,[alias(default),decimal_point('.'),thousands_sep(','),grouping([repeat(3)])]):-!.\n"
    "'$loc_props'(L,Ps):-atom(L),atom_concat('$locale_',L,K),nb_current(K,Ps).\n"
    "locale_create(New,From,Opts):-'$loc_resolve'(From,F),'$loc_props'(F,Ps0),'$loc_merge'(Opts,Ps0,Ps),(memberchk(alias(A),Opts)->New=A;(nb_current('$loc_seq',N0)->true;N0=0),N is N0+1,nb_setval('$loc_seq',N),atomic_list_concat(['$loc',N],New)),atom_concat('$locale_',New,K),nb_setval(K,Ps).\n"
    "'$loc_merge'([],Ps,Ps).\n"
    "'$loc_merge'([O|Os],Ps0,Ps):-functor(O,Nm,1),functor(T,Nm,1),('$loc_del'(T,Ps0,Ps1)->true;Ps1=Ps0),'$loc_merge'(Os,[O|Ps1],Ps).\n"
    "'$loc_del'(T,[H|R],R):-H=T,!.\n"
    "'$loc_del'(T,[H|R],[H|R1]):-'$loc_del'(T,R,R1).\n"
    "set_locale(L):-'$loc_resolve'(L,L1),nb_setval('$cur_locale',L1),'$loc_props'(L1,Ps),(memberchk(decimal_point(Dp),Ps)->true;Dp='.'),(memberchk(thousands_sep(Ts),Ps)->true;Ts=','),(memberchk(grouping(G),Ps)->true;G=[repeat(3)]),'$loc_gl'(G,Cs),atom_codes(Gs,Cs),'$create_prolog_flag'('$loc_dp',Dp),'$create_prolog_flag'('$loc_ts',Ts),'$create_prolog_flag'('$loc_grp',Gs).\n"
    "'$loc_gl'([],[]).\n"
    "'$loc_gl'([repeat(N)|_],[114|Cs]):-!,number_codes(N,Cs).\n"
    "'$loc_gl'([N|T],Cs):-number_codes(N,C1),'$loc_gl'(T,C2),(C2==[]->Cs=C1;append(C1,[44|C2],Cs)).\n"
    "locale_destroy(_).\n"
    "set_random(state(S)):-!,_ is '$rand_set'(S).\n"
    "set_random(seed(random)):-!,_ is '$rand_seed'(-1).\n"
    "set_random(seed(N)):-integer(N),!,_ is '$rand_seed'(N).\n"
    "set_random(O):-throw(error(domain_error(set_random,O),set_random/1)).\n"
    "random_property(state(S)):-S is '$rand_state'.\n"
    "subseq([],[],[]).\n"
    "subseq([H|T],[H|S],C):-subseq(T,S,C).\n"
    "subseq([H|T],S,[H|C]):-subseq(T,S,C).\n"
    "random_subseq([],[],[]).\n"
    "random_subseq([H|T],S,C):-(random_float<0.5->S=[H|S1],C=C1;S=S1,C=[H|C1]),random_subseq(T,S1,C1).\n"
    "random_numlist(P,L,U,R):-numlist(L,U,All),'$rnl'(All,P,R).\n"
    "'$rnl'([],_,[]).\n"
    "'$rnl'([H|T],P,R):-(random_float<P->R=[H|R1];R=R1),'$rnl'(T,P,R1).\n"
    "subsumes(G,S):-subsumes_term(G,S),G=S.\n"
    "'$arg_swi'(N,T,A):-var(N),!,(compound(T)->functor(T,_,Ar),between(1,Ar,N),arg(N,T,A);arg(N,T,A)).\n"
    "'$arg_swi'(N,T,A):-arg(N,T,A).\n"
    "is_most_general_term(T):-atom(T),!.\n"
    "is_most_general_term(T):-nonvar(T),T=[_|_],!,is_list(T),'$mgt_vars'(T,[]).\n"
    "is_most_general_term(T):-compound(T),T=..[_|As],'$mgt_vars'(As,[]).\n"
    "'$clause_unmangle'(M,T,B):-'$body_unmangle'(T,M,B0),B=B0.\n"
    "'$body_unmangle'(V,_,V):-var(V),!.\n"
    "'$body_unmangle'((A,B),M,(A1,B1)):-!,'$body_unmangle'(A,M,A1),'$body_unmangle'(B,M,B1).\n"
    "'$body_unmangle'((A;B),M,(A1;B1)):-!,'$body_unmangle'(A,M,A1),'$body_unmangle'(B,M,B1).\n"
    "'$body_unmangle'((A->B),M,(A1->B1)):-!,'$body_unmangle'(A,M,A1),'$body_unmangle'(B,M,B1).\n"
    "'$body_unmangle'((A*->B),M,(A1*->B1)):-!,'$body_unmangle'(A,M,A1),'$body_unmangle'(B,M,B1).\n"
    "'$body_unmangle'(\\+A,M,\\+A1):-!,'$body_unmangle'(A,M,A1).\n"
    "'$body_unmangle'(Q:G,M,R):-atom(Q),callable(G),functor(G,N,_),'$mod_split'(N,Q,GN),!,G=..[_|As],G0=..[GN|As],(Q==M->R=G0;R=Q:G0).\n"
    "'$body_unmangle'(G,M,G1):-callable(G),functor(G,N,_),'$mod_split'(N,GM,GN),!,G=..[_|As],G0=..[GN|As],(GM==M->G1=G0;G1=GM:G0).\n"
    "'$body_unmangle'(G,_,G).\n"
    "'$mod_split'(N,M,R):-atom(N),sub_atom(N,Bf,1,Af,':'),!,Bf>0,Af>0,sub_atom(N,0,Bf,_,M),sub_atom(N,_,Af,0,R),atom_codes(M,[C|Cs]),C>=0'a,C=<0'z,'$mod_idc'(Cs).\n"
    "'$mod_idc'([]).\n"
    "'$mod_idc'([C|Cs]):-(C>=0'a,C=<0'z;C>=0'A,C=<0'Z;C>=0'0,C=<0'9;C=:=0'_),!,'$mod_idc'(Cs).\n"
    "findnsols(N,_,_,_):-var(N),!,throw(error(instantiation_error,_)).\n"
    "findnsols(N,_,_,_):- \\+integer(N),!,throw(error(type_error(integer,N),_)).\n"
    "findnsols(N,_,_,L):-N=<0,!,L=[].\n"
    "findnsols(N,T0,G0,L):-copy_term(T0-G0,T-G),S='$fns'(0,[],no),(call(G),arg(2,S,A0),nb_setarg(2,S,[T|A0]),arg(1,S,C0),C is C0+1,nb_setarg(1,S,C),C>=N,arg(2,S,A),nb_setarg(1,S,0),nb_setarg(2,S,[]),nb_setarg(3,S,yes),reverse(A,L);arg(2,S,A),arg(3,S,Y),(A==[]->Y==no;true),reverse(A,L)).\n"
    "divmod(A,B,Q,R):-Q is A div B,R is A mod B.\n"
    "'$wot_stream_sink'(S):-nonvar(S),S='$stream'(_),catch(stream_property(S,_),_,fail),!.\n"
    "is_stream(S):-nonvar(S),S='$stream'(_),catch(stream_property(S,_),_,fail),!.\n"
    "is_stream(S):-atom(S),catch(stream_property(_,alias(S)),_,fail),!.\n"
    "'$wot_stream'(S,G):-current_output(O),setup_call_cleanup(set_output(S),once(G),set_output(O)).\n"
    "'$between_test'(L,_,_):-var(L),!,throw(error(instantiation_error,context(between/3,_))).\n"
    "'$between_test'(L,_,_):- \\+integer(L),!,throw(error(type_error(integer,L),context(between/3,_))).\n"
    "'$between_test'(_,H,_):-var(H),!,throw(error(instantiation_error,context(between/3,_))).\n"
    "'$between_test'(_,H,_):- \\+integer(H),H\\==inf,H\\==infinite,!,throw(error(type_error(integer,H),context(between/3,_))).\n"
    "'$between_test'(_,_,X):- \\+integer(X),!,throw(error(type_error(integer,X),context(between/3,_))).\n"
    "'$between_test'(L,H,X):-X>=L,(integer(H)->X=<H;true).\n"
    "lazy_list(Next,S0,L):-freeze(L,'$lazy_list_next'(Next,S0,L)).\n"
    "'$lazy_list_next'(Next,S0,L):-(call(Next,S0,S1,H)->L=[H|T],lazy_list(Next,S1,T);L=[]).\n"
    "'$at_ctx'(V):-var(V),!,throw(error(instantiation_error,_)).\n"
    "'$at_ctx'(_).\n"
    "@(G,M):-'$at_ctx'(G),'$at_ctx'(M),(G=Q:_->'$at_ctx'(Q);true),('$meta_ctx'(G,M,G2)->call(G2);call(G)).\n"
    "'$meta_add'(S):-(catch('$meta_rt'(S),error(existence_error(_,_),_),fail)->true;assertz('$meta_rt'(S))).\n"
    "'$meta_ctx'(G,M,G2):-'$meta_strip'(G,Q,G1),callable(G1),(nonvar(Q),'$mod_cl'(Q,G1,0,R)->true;R=G1),functor(R,F,N),functor(S,F,N),catch('$meta_rt'(S),error(existence_error(_,_),_),fail),!,R=..[F|As],S=..[_|Ss],'$meta_qargs'(Ss,As,M,As2),G2=..[F|As2].\n"
    "'$meta_strip'(G,Q,G1):-nonvar(G),G=(Q0:G0),atom(Q0),!,'$meta_strip'(G0,Q1,G1),(var(Q1)->Q=Q0;Q=Q1).\n"
    "'$meta_strip'(G,_,G).\n"
    "'$meta_qargs'([],[],_,[]).\n"
    "'$meta_qargs'([S|Ss],[A|As],M,[B|Bs]):-('$meta_qspec'(S),\\+ (nonvar(A),A=(_:_))->B=(M:A);B=A),'$meta_qargs'(Ss,As,M,Bs).\n"
    "'$meta_qspec'(S):-integer(S),!.\n"
    "'$meta_qspec'(':').\n"
    "'$meta_qspec'('^').\n"
    "'$meta_qspec'('//').\n"
    "'$mgt_vars'([],_).\n"
    "'$mgt_vars'([V|Vs],Seen):-var(V),\\+ memberchk_eq(V,Seen),'$mgt_vars'(Vs,[V|Seen]).\n"
    "apply(G,_):-nonvar(G),\\+ callable(G),!,throw(error(type_error(callable,G),_)).\n"
    "apply(G,As):-G=..L0,'$yall_app'(L0,As,L1),G1=..L1,call(G1).\n"
    "collation_key(A,K):-(var(A)->throw(error(instantiation_error,context(collation_key/2,_)));atom(A)->atom_string(A,K);string(A)->K=A;throw(error(type_error(atom,A),context(collation_key/2,_)))).\n"
    "'$write_canonical_swi'(T):-current_output(S),'$write_canonical_swi'(S,T).\n"
    "'$write_canonical_swi'(S,T):-term_variables(T,Vs),term_singletons(T,Ss),'$wcs_names'(Vs,Ss,0,VN),write_term(S,T,[quoted(true),ignore_ops(true),quote_non_ascii(true),character_escapes_unicode(false),variable_names(VN)]).\n"
    "'$sha1_hex'(Bs, Hex) :- length(Bs, L), '$sha1_pad'(Bs, L, P), '$sha1_blocks'(P, 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0, H0, H1, H2, H3, H4), '$sha1_hex8'([H0, H1, H2, "
    "H3, H4], Cs), atom_codes(Hex, Cs).\n"
    "'$sha1_pad'(Bs, L, P) :- Z is (55 - L) mod 64, length(Zs, Z), '$sha1_zeros'(Zs), N is L * 8, '$sha1_be'(8, N, Ln), append(Bs, [0x80|Zs], P0), append(P0, Ln, P).\n"
    "'$sha1_zeros'([]).\n"
    "'$sha1_zeros'([0|Zs]) :- '$sha1_zeros'(Zs).\n"
    "'$sha1_be'(0, _, []) :- !.\n"
    "'$sha1_be'(K, N, Bs) :- K1 is K - 1, B is (N >> (8 * K1)) /\\ 0xFF, Bs = [B|Bt], '$sha1_be'(K1, N, Bt).\n"
    "'$sha1_blocks'([], A, B, C, D, E, A, B, C, D, E) :- !.\n"
    "'$sha1_blocks'(P, A0, B0, C0, D0, E0, A, B, C, D, E) :- length(Blk, 64), append(Blk, Rest, P), '$sha1_words'(Blk, W16), '$sha1_sched'(16, W16, W16, W80), '$sha1_rounds'(0, W80, A0, B0, C0, "
    "D0, E0, A1, B1, C1, D1, E1), A2 is (A0 + A1) /\\ 0xFFFFFFFF, B2 is (B0 + B1) /\\ 0xFFFFFFFF, C2 is (C0 + C1) /\\ 0xFFFFFFFF, D2 is (D0 + D1) /\\ 0xFFFFFFFF, E2 is (E0 + E1) /\\ 0xFFFFFFFF, "
    "'$sha1_blocks'(Rest, A2, B2, C2, D2, E2, A, B, C, D, E).\n"
    "'$sha1_words'([], []).\n"
    "'$sha1_words'([B0, B1, B2, B3|Bs], [W|Ws]) :- W is (B0 << 24) \\/ (B1 << 16) \\/ (B2 << 8) \\/ B3, '$sha1_words'(Bs, Ws).\n"
    "'$sha1_sched'(80, _, W, W) :- !.\n"
    "'$sha1_sched'(T, Win, W0, W) :- nth0(13, Win, X3), nth0(8, Win, X8), nth0(2, Win, X14), Win = [X16|Wt], N is xor(xor(X3, X8), xor(X14, X16)), '$sha1_rol'(N, 1, R), append(Wt, [R], Win1), "
    "append(W0, [R], W1), T1 is T + 1, '$sha1_sched'(T1, Win1, W1, W).\n"
    "'$sha1_rol'(X, N, R) :- R is ((X << N) \\/ (X >> (32 - N))) /\\ 0xFFFFFFFF.\n"
    "'$sha1_rounds'(80, _, A, B, C, D, E, A, B, C, D, E) :- !.\n"
    "'$sha1_rounds'(T, [W|Ws], A, B, C, D, E, A1, B1, C1, D1, E1) :- '$sha1_fk'(T, B, C, D, F, K), '$sha1_rol'(A, 5, R5), Tmp is (R5 + F + E + K + W) /\\ 0xFFFFFFFF, '$sha1_rol'(B, 30, B30), "
    "T1 is T + 1, '$sha1_rounds'(T1, Ws, Tmp, A, B30, C, D, A1, B1, C1, D1, E1).\n"
    "'$sha1_fk'(T, B, C, D, F, 0x5A827999) :- T < 20, !, F is (B /\\ C) \\/ ((\\B) /\\ D).\n"
    "'$sha1_fk'(T, B, C, D, F, 0x6ED9EBA1) :- T < 40, !, F is xor(xor(B, C), D).\n"
    "'$sha1_fk'(T, B, C, D, F, 0x8F1BBCDC) :- T < 60, !, F is (B /\\ C) \\/ (B /\\ D) \\/ (C /\\ D).\n"
    "'$sha1_fk'(_, B, C, D, F, 0xCA62C1D6) :- F is xor(xor(B, C), D).\n"
    "'$sha1_hex8'([], []).\n"
    "'$sha1_hex8'([H|Hs], Cs) :- '$sha1_nib'(7, H, Cs, Ct), '$sha1_hex8'(Hs, Ct).\n"
    "'$sha1_nib'(-1, _, Cs, Cs) :- !.\n"
    "'$sha1_nib'(I, H, [C|Cs], Ct) :- D is (H >> (4 * I)) /\\ 15, ( D < 10 -> C is 0'0 + D ; C is 0'a + D - 10 ), I1 is I - 1, '$sha1_nib'(I1, H, Cs, Ct).\n"
    "'$vh_number'([], _).\n"
    "'$vh_number'([V|Vs], N) :- V = '$vh_var'(N), N1 is N + 1, '$vh_number'(Vs, N1).\n"
    "'$variant_key'(T, K) :- copy_term_nat(T, K), term_variables(K, Vs), '$vh_number'(Vs, 0).\n"
    "variant_sha1(T, H) :- ( attvar(T) -> throw(error(instantiation_error, variant_sha1/2)) ; \\+ acyclic_term(T) -> throw(error(type_error(acyclic_term, T), variant_sha1/2)) ; term_attvars(T, "
    "[_|_]) -> throw(error(type_error(free_of_attvar, T), variant_sha1/2)) ; true ), '$variant_key'(T, K), format(atom(A), '~k', [K]), string_bytes(A, Bs, utf8), '$sha1_hex'(Bs, H).\n"
    "variant_hash(T, H) :- '$variant_key'(T, K), term_hash(K, H).\n"
    "'$notrace'(G) :- call(G).\n"
    "getenv(N, V) :- ( var(N) -> throw(error(instantiation_error, getenv/2)) ; true ), environ(N, V0), !, V = V0.\n"
    "fill_buffer(S) :- ( var(S) -> throw(error(instantiation_error, fill_buffer/1)) ; true ).\n"
    "read_pending_codes(S, Cs, T) :- get_code(S, C), ( C == -1 -> Cs = [], T = [] ; Cs = [C|R], '$rp_codes'(4095, S, R, T) ).\n"
    "'$rp_codes'(0, _, T, T) :- !.\n"
    "'$rp_codes'(N, S, Cs, T) :- peek_code(S, C), ( C == -1 -> Cs = T ; get_code(S, C), Cs = [C|R], N1 is N - 1, '$rp_codes'(N1, S, R, T) ).\n"
    "read_pending_chars(S, Cs, T) :- get_char(S, C), ( C == end_of_file -> Cs = [], T = [] ; Cs = [C|R], '$rp_chars'(4095, S, R, T) ).\n"
    "'$rp_chars'(0, _, T, T) :- !.\n"
    "'$rp_chars'(N, S, Cs, T) :- peek_char(S, C), ( C == end_of_file -> Cs = T ; get_char(S, C), Cs = [C|R], N1 is N - 1, '$rp_chars'(N1, S, R, T) ).\n"
    "'$fast_record'(T, '$fast_term'(C)) :- copy_term(T, C).\n"
    "'$fast_recorded'('$fast_term'(C), T) :- copy_term(C, T).\n"
    "tmp_file(Base, Name) :- ( var(Base) -> throw(error(instantiation_error, tmp_file/2)) ; true ), prolog_pid(P), ( nb_current('$tmp_file_n', N0) -> true ; N0 = 0 ), N is N0 + 1, "
    "nb_setval('$tmp_file_n', N), ( getenv('TMPDIR', D) -> true ; D = '/tmp' ), atomic_list_concat([D, '/swipl_', Base, '_', P, '_', N], Name).\n"
    "tmp_file_stream(A, B, C) :- ( var(A), is_list(C) -> File = A, Stream = B, Opts = C ; File = B, Stream = C, Opts = [encoding(A)] ), ( memberchk(extension(Ext), Opts) -> true ; Ext = '' ), "
    "tmp_file(tmp, F0), ( Ext == '' -> File = F0 ; file_name_extension(F0, Ext, File) ), ( memberchk(encoding(E), Opts) -> true ; E = utf8 ), ( E == binary -> open(File, write, Stream, "
    "[type(binary)]) ; open(File, write, Stream, [encoding(E)]) ).\n"
    "read_string(S, Len, Str) :- ( var(Len) -> '$rs_all'(S, Cs), length(Cs, Len) ; '$rs_nonneg'(Len), '$rs_n'(Len, S, Cs) ), string_codes(Str, Cs).\n"
    "'$rs_all'(S, Cs) :- get_code(S, C), ( C == -1 -> Cs = [] ; Cs = [C|T], '$rs_all'(S, T) ).\n"
    "'$rs_nonneg'(L) :- ( integer(L) -> ( L >= 0 -> true ; throw(error(type_error(nonneg, L), read_string/3)) ) ; throw(error(type_error(integer, L), read_string/3)) ).\n"
    "'$rs_n'(0, _, []) :- !.\n"
    "'$rs_n'(N, S, Cs) :- get_code(S, C), ( C == -1 -> Cs = [] ; Cs = [C|T], N1 is N - 1, '$rs_n'(N1, S, T) ).\n"
    "read_string(S, Seps, Pads, Sep, Str) :- '$rs_text_codes'(Seps, SC), '$rs_text_codes'(Pads, PC), '$rs_upto'(S, SC, Cs, Sep), '$rs_strip'(Cs, PC, C1), reverse(C1, R1), '$rs_strip'(R1, PC, "
    "R2), reverse(R2, C2), string_codes(Str, C2).\n"
    "'$rs_text_codes'(T, Cs) :- ( is_list(T) -> Cs = T ; atom(T) -> atom_codes(T, Cs) ; string(T) -> string_codes(T, Cs) ; var(T) -> throw(error(instantiation_error, read_string/5)) ; throw(error(type_error(text, T), read_string/5)) ).\n"
    "'$rs_upto'(S, SC, Cs, Sep) :- get_code(S, C), ( C == -1 -> Cs = [], Sep = -1 ; memberchk(C, SC) -> Cs = [], Sep = C ; Cs = [C|T], '$rs_upto'(S, SC, T, Sep) ).\n"
    "'$rs_strip'([C|Cs], PC, R) :- memberchk(C, PC), !, '$rs_strip'(Cs, PC, R).\n"
    "'$rs_strip'(Cs, _, Cs).\n"
    "access_file(F, M) :- ( var(M) -> throw(error(instantiation_error, access_file/2)) ; atom(M) -> true ; throw(error(type_error(atom, M), access_file/2)) ), ( var(F) -> throw(error(instantiation_error, access_file/2)) ; true ), '$af'(M, F).\n"
    "'$af'(none, _) :- !.\n"
    "'$af'(exist, F) :- !, '$gnu_os'(access, F, 0).\n"
    "'$af'(read, F) :- !, '$gnu_os'(access, F, 4).\n"
    "'$af'(execute, F) :- !, '$gnu_os'(access, F, 1).\n"
    "'$af'(search, F) :- !, '$gnu_os'(access, F, 1).\n"
    "'$af'(M, F) :- ( M == write ; M == append ), !, ( '$gnu_os'(access, F, 0) -> '$gnu_os'(access, F, 2) ; '$af_dir'(F, D), '$gnu_os'(access, D, 2) ).\n"
    "'$af'(M, _) :- throw(error(domain_error(io_mode, M), access_file/2)).\n"
    "'$af_dir'(F, D) :- atom_codes(F, Cs), ( append(H, [0'/|T], Cs), \\+ memberchk(0'/, T) -> ( H == [] -> D = '/' ; atom_codes(D, H) ) ; D = '.' ).\n"
    "absolute_file_name(Spec, A, B) :- ( is_list(A), \\+ is_list(B) -> Opts = A, Abs = B ; Abs = A, Opts = B ), ( is_list(Opts) -> true ; throw(error(type_error(list, Opts), absolute_file_name/3)) ), '$afn3'(Spec, Abs, Opts).\n"
    "'$afn3'(Spec, Abs, Opts) :- ( var(Spec) -> throw(error(instantiation_error, absolute_file_name/3)) ; true ), ( string(Spec) -> atom_string(SA, Spec) ; SA = Spec ), "
    "( atom(SA), atom_length(SA, SL), SL >= 4096 -> throw(error(representation_error(max_path_length), absolute_file_name/3)) ; true ), "
    "( memberchk(extensions(Exts0), Opts) -> true ; memberchk(file_type(FT), Opts) -> '$afn_ftype_exts'(FT, Exts0) ; Exts0 = [''] ), ( memberchk(access(Mode), Opts) -> true ; Mode = none ), "
    "( memberchk(file_errors(FE), Opts) -> true ; FE = error ), ( memberchk(solutions(Sol), Opts) -> true ; Sol = first ), ( memberchk(relative_to(Rel), Opts), atom(SA), \\+ sub_atom(SA, 0, 1, "
    "_, '/') -> atomic_list_concat([Rel, '/', SA], Base) ; Base = SA ), ( Sol == all -> '$afn_cand'(Base, Exts0, Mode, FT, Abs) ; '$afn_cand'(Base, Exts0, Mode, FT, "
    "Abs) -> true ; FE == fail -> fail ; throw(error(existence_error(source_sink, Spec), absolute_file_name/3)) ).\n"
    "'$afn_ftype_exts'(txt, ['']) :- !.\n"
    "'$afn_ftype_exts'(prolog, ['.pl', '']) :- !.\n"
    "'$afn_ftype_exts'(source, ['.pl', '']) :- !.\n"
    "'$afn_ftype_exts'(_, ['']).\n"
    "'$afn_cand'(Base, Exts, Mode, FT, Abs) :- member(E0, Exts), ( E0 == '' -> F = Base ; ( sub_atom(E0, 0, 1, _, '.') -> E = E0 ; atom_concat('.', E0, E) ), ( sub_atom(Base, _, _, 0, "
    "E) -> F = Base ; atom_concat(Base, E, F) ) ), absolute_file_name(F, Abs), ( FT == directory -> '$gnu_os'(access, Abs, 0), file_property(Abs, type(directory)) ; true ), ( Mode == none, "
    "FT \\== directory -> true ; '$af'(Mode, Abs), ( FT == directory -> true ; \\+ file_property(Abs, type(directory)) ; Mode == none ) ).\n"
    "file_base_name(P, B) :- '$fn_codes'(P, file_base_name/2, Cs), '$fn_split'(Cs, _, Bc), atom_codes(B0, Bc), '$fn_max'(B0, file_base_name/2), B = B0.\n"
    "file_directory_name(P, D) :- '$fn_codes'(P, file_directory_name/2, Cs), '$fn_split'(Cs, Dc, _), atom_codes(D0, Dc), '$fn_max'(D0, file_directory_name/2), D = D0.\n"
    "'$fn_max'(A, PI) :- atom_length(A, N), ( N >= 4096 -> throw(error(representation_error(max_path_length), PI)) ; true ).\n"
    "'$fn_codes'(P, PI, Cs) :- ( var(P) -> throw(error(instantiation_error, PI)) ; atom(P) -> atom_codes(P, Cs) ; string(P) -> string_codes(P, "
    "Cs) ; is_list(P) -> Cs = P ; throw(error(type_error(atom, P), PI)) ).\n"
    "'$fn_split'([], \".\", []) :- !.\n"
    "'$fn_split'(Cs, D, B) :- '$fn_rstrip'(Cs, T), ( T == [] -> D = \"/\", B = \"/\" ; '$fn_last_slash'(T, Pre, B0) -> B = B0, '$fn_rstrip'(Pre, D0), "
    "( D0 == [] -> D = \"/\" ; D = D0 ) ; D = \".\", B = T ).\n"
    "'$fn_rstrip'(Cs, T) :- reverse(Cs, R), '$fn_dropslash'(R, R1), reverse(R1, T).\n"
    "'$fn_dropslash'([0'/|R], R1) :- !, '$fn_dropslash'(R, R1).\n"
    "'$fn_dropslash'(R, R).\n"
    "'$fn_last_slash'(T, Pre, B) :- append(Pre0, [0'/|B], T), \\+ memberchk(0'/, B), !, append(Pre0, [0'/], Pre).\n"
    "file_name_extension(B, E, N) :- nonvar(N), !, '$fn_codes'(N, file_name_extension/3, Nc), ( append(Bc, [0'.|Ec], Nc), \\+ memberchk(0'., Ec), \\+ memberchk(0'/, Ec) -> true ; Bc = Nc, "
    "Ec = [] ), ( nonvar(E) -> '$fn_codes'(E, file_name_extension/3, E0), ( E0 = [0'.|Ew] -> true ; Ew = E0 ), ( Ew == Ec -> atom_codes(B1, Bc) ; fail ) ; atom_codes(E, Ec), atom_codes(B1, Bc) ), ( nonvar(B) -> '$fn_codes'(B, file_name_extension/3, Bk), atom_codes(B1, Bk) ; B = B1 ).\n"
    "file_name_extension(B, E, N) :- ( var(B) ; var(E) ), !, throw(error(instantiation_error, file_name_extension/3)).\n"
    "file_name_extension(B, E, N) :- '$fn_codes'(B, file_name_extension/3, Bc), '$fn_codes'(E, file_name_extension/3, E0), ( E0 == [] -> Nc = Bc ; ( E0 = [0'.|_] -> Ed = E0 ; Ed = [0'.|E0] ), "
    "( append(_, Ed, Bc) -> Nc = Bc ; append(Bc, Ed, Nc) ) ), atom_codes(N, Nc).\n"
    "'$wcs_names'([],_,_,[]).\n"
    "'$wcs_names'([V|Vs],Ss,I,[N=V|R]):-(memberchk_eq(V,Ss)->N='_',I1=I;'$wcs_name'(I,N),I1 is I+1),'$wcs_names'(Vs,Ss,I1,R).\n"
    "'$wcs_name'(I,N):-L is 65+I mod 26,J is I//26,(J=:=0->atom_codes(N,[L]);number_codes(J,Ds),atom_codes(N,[L|Ds])).\n"
    "string_bytes(S,B,E):-var(S),!,'$sb_dec'(E,B,Cs),string_codes(S,Cs).\n"
    "string_bytes(S,B,E):-string_codes(S,Cs),'$sb_enc'(E,Cs,B0),B=B0.\n"
    "'$sb_enc'(utf8,Cs,B):-!,'$sb_u8e'(Cs,B).\n"
    "'$sb_enc'(text,Cs,B):-!,'$sb_u8e'(Cs,B).\n"
    "'$sb_enc'(utf16le,Cs,B):-!,'$sb_16e'(Cs,le,B).\n"
    "'$sb_enc'(utf16be,Cs,B):-!,'$sb_16e'(Cs,be,B).\n"
    "'$sb_enc'(E,Cs,B):-'$sb_max'(E,M),!,'$sb_1e'(Cs,M,B).\n"
    "'$sb_enc'(E,_,_):-throw(error(domain_error(encoding,E),string_bytes/3)).\n"
    "'$sb_dec'(utf8,B,Cs):-!,'$sb_u8d'(B,Cs).\n"
    "'$sb_dec'(text,B,Cs):-!,'$sb_u8d'(B,Cs).\n"
    "'$sb_dec'(utf16le,B,Cs):-!,'$sb_16d'(B,le,Cs).\n"
    "'$sb_dec'(utf16be,B,Cs):-!,'$sb_16d'(B,be,Cs).\n"
    "'$sb_dec'(E,B,B):-'$sb_max'(E,_),!.\n"
    "'$sb_dec'(E,_,_):-throw(error(domain_error(encoding,E),string_bytes/3)).\n"
    "'$sb_max'(iso_latin_1,255).\n"
    "'$sb_max'(octet,255).\n"
    "'$sb_max'(ascii,127).\n"
    "'$sb_1e'([],_,[]).\n"
    "'$sb_1e'([C|Cs],M,[C|B]):-(C=<M->true;throw(error(representation_error(encoding),string_bytes/3))),'$sb_1e'(Cs,M,B).\n"
    "'$sb_u8e'([],[]).\n"
    "'$sb_u8e'([C|Cs],[C|B]):-C<128,!,'$sb_u8e'(Cs,B).\n"
    "'$sb_u8e'([C|Cs],[B0,B1|B]):-C<2048,!,B0 is 192\\/(C>>6),B1 is 128\\/(C/\\63),'$sb_u8e'(Cs,B).\n"
    "'$sb_u8e'([C|Cs],[B0,B1,B2|B]):-C<65536,!,B0 is 224\\/(C>>12),B1 is 128\\/((C>>6)/\\63),B2 is 128\\/(C/\\63),'$sb_u8e'(Cs,B).\n"
    "'$sb_u8e'([C|Cs],[B0,B1,B2,B3|B]):-B0 is 240\\/(C>>18),B1 is 128\\/((C>>12)/\\63),B2 is 128\\/((C>>6)/\\63),B3 is 128\\/(C/\\63),'$sb_u8e'(Cs,B).\n"
    "'$sb_u8d'([],[]).\n"
    "'$sb_u8d'([B|Bs],[B|Cs]):-B<128,!,'$sb_u8d'(Bs,Cs).\n"
    "'$sb_u8d'([B|Bs],[C|Cs]):-(B>=240->N=3,C0 is B/\\7;B>=224->N=2,C0 is B/\\15;N=1,C0 is B/\\31),'$sb_u8c'(N,Bs,C0,C,Bs1),'$sb_u8d'(Bs1,Cs).\n"
    "'$sb_u8c'(0,Bs,C,C,Bs):-!.\n"
    "'$sb_u8c'(N,[B|Bs],C0,C,R):-!,C1 is (C0<<6)\\/(B/\\63),N1 is N-1,'$sb_u8c'(N1,Bs,C1,C,R).\n"
    "'$sb_u8c'(_,[],_,_,_):-throw(error(syntax_error(incomplete_multibyte_sequence),string_bytes/3)).\n"
    "'$sb_16e'([],_,[]).\n"
    "'$sb_16e'([C|Cs],O,B):-C<65536,!,'$sb_16u'(C,O,B,B1),'$sb_16e'(Cs,O,B1).\n"
    "'$sb_16e'([C|Cs],O,B):-C1 is C-65536,H is 55296+(C1>>10),L is 56320+(C1/\\1023),'$sb_16u'(H,O,B,B1),'$sb_16u'(L,O,B1,B2),'$sb_16e'(Cs,O,B2).\n"
    "'$sb_16u'(U,le,[Lo,Hi|B],B):-Lo is U/\\255,Hi is U>>8.\n"
    "'$sb_16u'(U,be,[Hi,Lo|B],B):-Lo is U/\\255,Hi is U>>8.\n"
    "'$sb_16d'([],_,[]):-!.\n"
    "'$sb_16d'([X,Y|Bs],O,Cs):-!,(O==le->U is X\\/(Y<<8);U is (X<<8)\\/Y),(U>=55296,U<56320->'$sb_16lo'(Bs,O,U,Cs);Cs=[U|Cs1],'$sb_16d'(Bs,O,Cs1)).\n"
    "'$sb_16d'(_,_,_):-throw(error(syntax_error(incomplete_multibyte_sequence),string_bytes/3)).\n"
    "'$sb_16lo'([X,Y|Bs],O,H,[C|Cs]):-!,(O==le->L is X\\/(Y<<8);L is (X<<8)\\/Y),C is 65536+((H-55296)<<10)+(L-56320),'$sb_16d'(Bs,O,Cs).\n"
    "'$sb_16lo'(_,_,_,_):-throw(error(syntax_error(incomplete_multibyte_sequence),string_bytes/3)).\n"
    "compound_name_arity(T,N,A):-compound(T),!,'$compound0'(name,T,N),'$compound0'(arity,T,A).\n"
    "compound_name_arity(T,N,A):-var(T),!,(A==0->'$compound0'(build,N,T);functor(T,N,A)).\n"
    "compound_name_arity(T,_,_):-throw(error(type_error(compound,T),compound_name_arity/3)).\n"
    "compound_name_arguments(T,N,As):-compound(T),!,'$compound0'(arity,T,A),(A=:=0->'$compound0'(name,T,N),As=[];T=..[N|As]).\n"
    "compound_name_arguments(T,N,As):-var(T),!,(As==[]->'$compound0'(build,N,T);T=..[N|As]).\n"
    "compound_name_arguments(T,_,_):-throw(error(type_error(compound,T),compound_name_arguments/3)).\n"
    "call_nth(G,_):-var(G),!,throw(error(instantiation_error,call_nth/2)).\n"
    "call_nth(G,_):- \\+ callable(G),!,throw(error(type_error(callable,G),call_nth/2)).\n"
    "call_nth(_,N):-nonvar(N),\\+ integer(N),!,throw(error(type_error(integer,N),call_nth/2)).\n"
    "call_nth(_,N):-integer(N),N<0,!,throw(error(domain_error(not_less_than_zero,N),call_nth/2)).\n"
    "call_nth(_,N):-N==0,!,fail.\n"
    "call_nth(G,N):-(retract('$cn_seq'(K0))->K is K0+1;K=1),assertz('$cn_seq'(K)),assertz('$cn_cnt'(K,0)),call(G),retract('$cn_cnt'(K,C0)),C is C0+1,assertz('$cn_cnt'(K,C)),(integer(N)->C=:=N,! ; N=C).\n"
    "subsumes_term(G,S):- \\+ \\+ (term_variables(S,V1),unify_with_occurs_check(G,S),term_variables(V1,V2),V1==V2).\n"
    "'$btree_find_node'(K,T,P,N,C):-Kp is (P>>16)/\\255,Lp is (P>>8)/\\255,Rp is P/\\255,'$btree_find'(K,T,Kp,Lp,Rp,N,C).\n"
    "'$btree_find'(K,T,Kp,Lp,Rp,N,C):-arg(Kp,T,K0),compare(O,K,K0),'$btree_step'(O,K,T,Kp,Lp,Rp,N,C).\n"
    "'$btree_step'('=',_,T,_,_,_,T,'=').\n"
    "'$btree_step'('<',K,T,Kp,Lp,Rp,N,C):-arg(Lp,T,S),(compound(S)->'$btree_find'(K,S,Kp,Lp,Rp,N,C);N=T,C='<').\n"
    "'$btree_step'('>',K,T,Kp,Lp,Rp,N,C):-arg(Rp,T,S),(compound(S)->'$btree_find'(K,S,Kp,Lp,Rp,N,C);N=T,C='>').\n"
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
    "consult(F):- current_prolog_flag('$dialect',gnu),!,(atom(F),F\\==[]->'$gnu_consult'(F);'$gnu_atom_list'(F,consult/1),'$gnu_consult_list'(F)).\n"
    "consult(F):-'$consult_path'(F,P),open(P,read,S),catch('$consult_loop'(S,Is),E,(close(S),throw(E))),close(S),'$consult_inits'(Is).\n"
    "'$gnu_consult_list'([]).\n"
    "'$gnu_consult_list'([F|Fs]):-'$gnu_consult'(F),'$gnu_consult_list'(Fs).\n"
    "'$gnu_atom_list'(L,P):-var(L),!,throw(error(instantiation_error,P)).\n"
    "'$gnu_atom_list'([],_):-!.\n"
    "'$gnu_atom_list'([X|L],P):-!,(atom(X)->true;var(X)->throw(error(instantiation_error,P));throw(error(type_error(atom,X),P))),'$gnu_atom_list'(L,P).\n"
    "'$gnu_atom_list'(L,P):-throw(error(type_error(list,L),P)).\n"
    "'$gnu_consult'(F):-'$gnu_pl_file'(F,P),'$gnu_consult_open'(P,S,A),catch('$gnu_consult_loop'(S,A,[],Is),E,('$gnu_consult_close'(S),throw(E))),'$gnu_consult_close'(S),'$consult_inits'(Is).\n"
    "'$gnu_consult_open'(user,user_input,user):-!.\n"
    "'$gnu_consult_open'(P,S,A):-(file_exists(P)->absolute_file_name(P,A),open(P,read,S);throw(error(existence_error(source_sink,P),consult/1))).\n"
    "'$gnu_consult_close'(user_input):-!.\n"
    "'$gnu_consult_close'(S):-close(S).\n"
    "'$gnu_pl_file'(user,user):-!.\n"
    "'$gnu_pl_file'(F,F):-atom_codes(F,Cs),'$gnu_base'(Cs,Bs),memberchk(46,Bs),!.\n"
    "'$gnu_pl_file'(F,P):-member(X,['.pl','.pro','.prolog']),atom_concat(F,X,P),file_exists(P),!.\n"
    "'$gnu_pl_file'(F,P):-atom_concat(F,'.pl',P).\n"
    "'$gnu_base'(Cs,Bs):-(append(_,[47|R],Cs)->'$gnu_base'(R,Bs);Bs=Cs).\n"
    "'$gnu_consult_loop'(S,P,Sn,Is):-read_term(S,T,[]),(T==end_of_file->Is=[];expand_term(T,X),'$gnu_consult_terms'(X,P,Sn,Sn1,Is,R),'$gnu_consult_loop'(S,P,Sn1,R)).\n"
    "'$gnu_consult_terms'(X,P,Sn0,Sn,Is,R):-var(X),!,'$gnu_consult_term'(X,P,Sn0,Sn,Is,R).\n"
    "'$gnu_consult_terms'([],_,Sn,Sn,R,R):-!.\n"
    "'$gnu_consult_terms'([T|Ts],P,Sn0,Sn,Is,R):-!,'$gnu_consult_term'(T,P,Sn0,Sn1,Is,R1),'$gnu_consult_terms'(Ts,P,Sn1,Sn,R1,R).\n"
    "'$gnu_consult_terms'(T,P,Sn0,Sn,Is,R):-'$gnu_consult_term'(T,P,Sn0,Sn,Is,R).\n"
    "'$gnu_consult_term'(T,_,Sn,Sn,Is,R):-nonvar(T),T=(:-_),!,'$consult_term'(T,Is,R).\n"
    "'$gnu_consult_term'(C,P,Sn0,Sn,R,R):-('$gnu_clause_pi'(C,N,A)->(memberchk(N/A,Sn0)->Sn=Sn0;Sn=[N/A|Sn0],'$gnu_redefine'(N,A,P));Sn=Sn0),assertz(C).\n"
    "'$gnu_clause_pi'(C,N,A):-nonvar(C),(C=(H:-_)->true;H=C),callable(H),functor(H,N,A).\n"
    "'$gnu_redefine'(N,A,P):-functor(H,N,A),catch(retractall(H),_,true),'$gnu_pfile_seq'(K),catch(retractall('$gnu_pfile'(N,A,_,_)),_,true),assertz('$gnu_pfile'(N,A,P,K)).\n"
    "'$gnu_pfile_seq'(K):-findall(x,catch('$gnu_pfile'(_,_,_,_),error(existence_error(_,_),_),fail),L),length(L,K).\n"
    "'$gnu_pfile_of'(N,A,F,K):-catch('$gnu_pfile'(N,A,F,K),error(existence_error(_,_),_),fail),!.\n"
    "'$gnu_pfile_of'(_,_,user_input,0).\n"
    "listing(_):- \\+current_prolog_flag('$dialect',gnu),!,throw(error(existence_error(procedure,listing/1),listing/1)).\n"
    "listing(PI):-var(PI),!,throw(error(instantiation_error,listing/1)).\n"
    "listing(N):-atom(N),!,'$gnu_listing'(N/_).\n"
    "listing(PI):-'$gnu_pi_check'(PI),'$gnu_listing'(PI).\n"
    "'$gnu_pi_check'(N/A):-!,(var(N)->true;atom(N)->true;throw(error(type_error(atom,N),listing/1))),"
        "(var(A)->true;integer(A)->(A<0->throw(error(domain_error(not_less_than_zero,A),listing/1));true);throw(error(type_error(integer,A),listing/1))).\n"
    "'$gnu_pi_check'(PI):-throw(error(type_error(predicate_indicator,PI),listing/1)).\n"
    "'$gnu_listing'(PI):-findall(F-K-PI,'$gnu_listing_pi'(F,K,PI),L0),sort(L0,L),'$gnu_listing_files'(L).\n"
    "'$gnu_listing_pi'(F,K,N/A):-current_predicate(N/A),atom(N),\\+sub_atom(N,0,1,_,'$'),functor(H,N,A),predicate_property(H,dynamic),'$gnu_pfile_of'(N,A,F,K).\n"
    "'$gnu_listing_files'([]).\n"
    "'$gnu_listing_files'([F-K-PI|L]):-format('~n% file: ~w~n',[F]),'$gnu_listing_file'([F-K-PI|L],F,R),'$gnu_listing_files'(R).\n"
    "'$gnu_listing_file'([F-_-N/A|L],F,R):-!,'$gnu_listing_one'(N,A),'$gnu_listing_file'(L,F,R).\n"
    "'$gnu_listing_file'(R,_,R).\n"
    "'$gnu_listing_one'(N,A):-functor(H,N,A),nl,(clause(H,B),portray_clause((H:-B)),fail;true).\n"
    "load(_):- \\+current_prolog_flag('$dialect',gnu),!,throw(error(existence_error(procedure,load/1),load/1)).\n"
    "load(F):-(atom(F),F\\==[]->'$gnu_load'(F);'$gnu_atom_list'(F,load/1),'$gnu_load_all'(F)).\n"
    "'$gnu_load_all'([]).\n"
    "'$gnu_load_all'([F|Fs]):-'$gnu_load'(F),'$gnu_load_all'(Fs).\n"
    "'$gnu_load'(F):-atom_codes(F,Cs),'$gnu_base'(Cs,Bs),(memberchk(46,Bs)->P=F;atom_concat(F,'.wbc',P)),"
        "(file_exists(P)->throw(error(permission_error(load,byte_code,P),load/1));throw(error(existence_error(source_sink,P),load/1))).\n"
    "'$consult_path'(F,_):-var(F),!,throw(error(instantiation_error,consult/1)).\n"
    "'$consult_path'(F,F):-atom(F),file_exists(F),!.\n"
    "'$consult_path'(F,P):-atom(F),atom_concat(F,'.pl',P),file_exists(P),!.\n"
    "'$consult_path'(F,P):-atom(F),atom_concat(F,'.pro',P),file_exists(P),!.\n"
    "'$consult_path'(F,P):-atom(F),atom_concat(F,'.prolog',P),file_exists(P),!.\n"
    "'$consult_path'(F,P):-atom(F),atom_concat(F,'.plt',P),file_exists(P),!.\n"
    "'$consult_path'(F,_):-throw(error(existence_error(source_sink,F),consult/1)).\n"
    "'$consult_loop'(S,Is):-read_term(S,T,[]),(T==end_of_file->Is=[];expand_term(T,X),'$consult_terms'(X,Is,R),'$consult_loop'(S,R)).\n"
    "'$consult_terms'(X,Is,R):-var(X),!,'$consult_term'(X,Is,R).\n"
    "'$consult_terms'([],R,R):-!.\n"
    "'$consult_terms'([T|Ts],Is,R):-!,'$consult_term'(T,Is,R1),'$consult_terms'(Ts,R1,R).\n"
    "'$consult_terms'(T,Is,R):-'$consult_term'(T,Is,R).\n"
    "'$consult_term'((:-initialization(G)),[G|R],R):-!.\n"
    "'$consult_term'((:-multifile(_)),R,R):-!.\n"
    "'$consult_term'((:-discontiguous(_)),R,R):-!.\n"
    "'$consult_term'((:-D),R,R):-!,'$consult_goal'(D).\n"
    "'$consult_term'((H-->B),R,R):-!,dcg_translate_rule((H-->B),C),assertz(C).\n"
    "'$consult_term'(C,R,R):-assertz(C).\n"
    "'$consult_inits'([]).\n"
    "'$consult_inits'([G|Gs]):-'$consult_goal'(G),'$consult_inits'(Gs).\n"
    "'$consult_goal'(G):-(catch(G,E,(format(user_error,'Warning: directive raised: ~q~n',[E]),true))->true;format(user_error,'Warning: directive failed: ~q~n',[G])).\n"
    "load_files(S):-load_files(S,[]).\n"
    "load_files(S,O):-'$lf_check'(S,O),'$lf_mod'(S,M,F),'$lf_list'(F,Fs),'$lf_all'(Fs,M,O).\n"
    "'$lf_check'(S,_):-var(S),!,throw(error(instantiation_error,load_files/2)).\n"
    "'$lf_check'(_,O):-var(O),!,throw(error(instantiation_error,load_files/2)).\n"
    "'$lf_check'(_,O):- \\+ is_list(O),!,throw(error(type_error(list,O),load_files/2)).\n"
    "'$lf_check'(_,_).\n"
    "'$lf_mod'(M:F,M,F):-atom(M),!.\n"
    "'$lf_mod'(F,user,F).\n"
    "'$lf_list'(F,F):-is_list(F),!.\n"
    "'$lf_list'(F,[F]).\n"
    "'$lf_all'([],_,_).\n"
    "'$lf_all'([F|Fs],M,O):-'$lf_one'(F,M,O),'$lf_all'(Fs,M,O).\n"
    "'$lf_one'(F,_,_):-nonvar(F),F=library(_),!.\n"
    "'$lf_one'(F,M,O):-'$lf_path'(F,O,P),absolute_file_name(P,A),('$lf_skip'(A,M,O)->true;'$lf_load'(A,M,O)).\n"
    "'$lf_skip'(A,M,O):-memberchk(if(C),O),C\\==true,catch('$lf_loaded'(A,M),error(existence_error(_,_),_),fail),!.\n"
    "'$lf_path'(F,_,_):-var(F),!,throw(error(instantiation_error,load_files/2)).\n"
    "'$lf_path'(F,_,F):-atom(F),file_exists(F),!.\n"
    "'$lf_path'(F,O,P):-atom(F),'$lf_ext'(O,X),atom_concat(F,X,P),file_exists(P),!.\n"
    "'$lf_path'(F,_,_):-throw(error(existence_error(source_sink,F),load_files/2)).\n"
    "'$lf_ext'(O,X):-memberchk(dialect(xsb),O),!,member(X,['.P','.pl']).\n"
    "'$lf_ext'(_,X):-member(X,['.pl','.P','.pro','.prolog']).\n"
    "'$lf_load'(A,M,O):-(memberchk(dialect(xsb),O)->X=xsb;X=swi),'$lf_ops_on'(X,Sv),open(A,read,S),catch('$lf_read'(S,A,Ts1),E,(close(S),'$lf_ops_off'(X,Sv),throw(E))),close(S),catch('$lf_hdr'(X,A,Ts1,Ts),_,Ts=Ts1),'$lf_ops_off'(X,Sv),catch(retractall('$lf_loaded'(A,M)),_,true),assertz('$lf_loaded'(A,M)),'$lf_install'(Ts,M,A).\n"
    "'$lf_hdr'(xsb,A,Ts1,Ts):-atom_concat(B,'.P',A),atom_concat(B,'.H',H),file_exists(H),!,open(H,read,S),catch('$lf_read'(S,H,Hs),E,(close(S),throw(E))),close(S),append(Hs,Ts1,Ts).\n"
    "'$lf_hdr'(_,_,Ts,Ts).\n"
    "'$lf_read'(S,A,Ts):-'$lf_rdc'(S,A,[],Ts).\n"
    "'$lf_rdc'(S,A,C,Ts):-read_term(S,T,[]),'$lf_rdt'(T,S,A,C,Ts).\n"
    "'$lf_rdt'(T,_,_,_,[]):-T==end_of_file,!.\n"
    "'$lf_rdt'(T,S,A,C,Ts):-'$lf_cnd'(T,C,C1),!,'$lf_rdc'(S,A,C1,Ts).\n"
    "'$lf_rdt'(T,S,A,C,Ts):-'$lf_on'(C),!,'$lf_rop'(T),'$lf_rd1'(T,A,Ts,R),'$lf_rdc'(S,A,C,R).\n"
    "'$lf_rdt'(_,S,A,C,Ts):-'$lf_rdc'(S,A,C,Ts).\n"
    "'$lf_on'([]).\n"
    "'$lf_on'([t|_]).\n"
    "'$lf_cd'(T,D):-nonvar(T),T=(:-D0),nonvar(D0),D0=D.\n"
    "'$lf_cnd'(T,C,[X|C]):-'$lf_cd'(T,if(G)),!,('$lf_on'(C)->('$lf_if'(G)->X=t;X=f);X=s).\n"
    "'$lf_cnd'(T,[X|C],[Y|C]):-'$lf_cd'(T,elif(G)),!,(X==f->('$lf_if'(G)->Y=t;Y=f);X==t->Y=d;Y=X).\n"
    "'$lf_cnd'(T,[X|C],[Y|C]):-'$lf_cd'(T,else),!,(X==f->Y=t;X==t->Y=d;Y=X).\n"
    "'$lf_cnd'(T,[_|C],C):-'$lf_cd'(T,endif),!.\n"
    "'$lf_if'(G):-catch(G,_,fail),!.\n"
    "'$lf_rd1'(T,A,Ts,R):-nonvar(T),T=(:-D),nonvar(D),D=include(F),!,'$lf_rel'(A,F,P0),'$lf_path'(P0,[dialect(xsb)],P),open(P,read,S2),catch('$lf_read'(S2,P,Is),E,(close(S2),throw(E))),close(S2),append(Is,R,Ts).\n"
    "'$lf_rd1'(T,_,[T|R],R).\n"
    "'$lf_rel'(A,F,P):-atom(F),\\+ sub_atom(F,0,1,_,'/'),!,file_directory_name(A,D),atomic_list_concat([D,'/',F],P).\n"
    "'$lf_rel'(_,F,F).\n"
    "'$lf_rop'(T):-nonvar(T),T=(:-D),nonvar(D),D=op(P,Ty,N),!,catch(op(P,Ty,N),_,true).\n"
    "'$lf_rop'(_).\n"
    "'$lf_xops'([op(1050,fy,import),op(1100,fx,export),op(1040,xfx,from),op(1100,fy,index),op(1100,fy,ti),op(1100,fx,mode),op(1045,xfx,as),op(900,fy,tnot),op(900,fy,not),op(1100,fx,thread_shared)]).\n"
    "'$lf_ops_on'(swi,[]).\n"
    "'$lf_ops_on'(xsb,Sv):-'$lf_xops'(L),findall(op(P,T,N),(member(op(_,_,N),L),current_op(P,T,N)),Sv),'$lf_apply'(L).\n"
    "'$lf_ops_off'(swi,_).\n"
    "'$lf_ops_off'(xsb,Sv):-'$lf_xops'(L),'$lf_zero'(L),'$lf_apply'(Sv).\n"
    "'$lf_apply'([]).\n"
    "'$lf_apply'([op(P,T,N)|R]):-catch(op(P,T,N),_,true),'$lf_apply'(R).\n"
    "'$lf_zero'([]).\n"
    "'$lf_zero'([op(_,T,N)|R]):-catch(op(0,T,N),_,true),'$lf_zero'(R).\n"
    "'$lf_install'(Ts,M,A):-'$lfx_scan'(Ts,Ks0,[],Tb0,[]),sort(Ks0,Ks),sort(Tb0,Tb),'$lfx_srcs'(Ks,M,A),'$lfx_wraps'(Tb,M),'$lfx_terms'(Ts,M,Ks,Tb,A,Is),'$consult_inits'(Is).\n"
    "'$lfx_scan'([],Ks,Ks,Tb,Tb).\n"
    "'$lfx_scan'([T|Ts],Ks,K0,Tb,T0):-'$lfx_scan1'(T,Ks,K1,Tb,T1),'$lfx_scan'(Ts,K1,K0,T1,T0).\n"
    "'$lfx_scan1'(T,Ks,Ks,Tb,Tb):-var(T),!.\n"
    "'$lfx_scan1'((:-D),Ks,K0,Tb,T0):-!,'$lfx_sdir'(D,Ks,K0,Tb,T0).\n"
    "'$lfx_scan1'((?-_),Ks,Ks,Tb,Tb):-!.\n"
    "'$lfx_scan1'((H-->_),[F/N|K0],K0,Tb,Tb):-!,'$lfx_hkey'(H,F,N0),N is N0+2.\n"
    "'$lfx_scan1'((H:-_),[F/N|K0],K0,Tb,Tb):-!,'$lfx_hkey'(H,F,N).\n"
    "'$lfx_scan1'(H,[F/N|K0],K0,Tb,Tb):-'$lfx_hkey'(H,F,N).\n"
    "'$lfx_hkey'(H,F,N):-callable(H),H\\=(_:_),!,functor(H,F,N).\n"
    "'$lfx_hkey'(_,'$none',0).\n"
    "'$lfx_sdir'(D,Ks,Ks,Tb,Tb):-var(D),!.\n"
    "'$lfx_sdir'(table(S),Ks,K0,Tb,T0):-!,'$lfx_tspecs'(S,Ks,K0,Tb,T0).\n"
    "'$lfx_sdir'(dynamic(S),Ks,K0,Tb,Tb):-!,'$lfx_dspecs'(S,Ks,K0).\n"
    "'$lfx_sdir'(_,Ks,Ks,Tb,Tb).\n"
    "'$lfx_tspecs'(V,Ks,Ks,Tb,Tb):-var(V),!.\n"
    "'$lfx_tspecs'((A,B),Ks,K0,Tb,T0):-!,'$lfx_tspecs'(A,Ks,K1,Tb,T1),'$lfx_tspecs'(B,K1,K0,T1,T0).\n"
    "'$lfx_tspecs'([],Ks,Ks,Tb,Tb):-!.\n"
    "'$lfx_tspecs'([A|B],Ks,K0,Tb,T0):-!,'$lfx_tspecs'(A,Ks,K1,Tb,T1),'$lfx_tspecs'(B,K1,K0,T1,T0).\n"
    "'$lfx_tspecs'(as(S,_),Ks,K0,Tb,T0):-!,'$lfx_tspecs'(S,Ks,K0,Tb,T0).\n"
    "'$lfx_tspecs'(F/N,[F/N|K0],K0,[k(F/N,none)|T0],T0):-!.\n"
    "'$lfx_tspecs'(F//N,[F/N2|K0],K0,[k(F/N2,none)|T0],T0):-!,N2 is N+2.\n"
    "'$lfx_tspecs'(S,[F/N|K0],K0,[k(F/N,Md)|T0],T0):-callable(S),!,functor(S,F,N),('$lfx_moded'(S)->Md=S;Md=none).\n"
    "'$lfx_tspecs'(_,Ks,Ks,Tb,Tb).\n"
    "'$lfx_moded'(S):-S=..[_|As],member(A,As),\\+ '$tbl_mindex'(A),!.\n"
    "'$lfx_dspecs'(V,Ks,Ks):-var(V),!.\n"
    "'$lfx_dspecs'((A,B),Ks,K0):-!,'$lfx_dspecs'(A,Ks,K1),'$lfx_dspecs'(B,K1,K0).\n"
    "'$lfx_dspecs'([],Ks,Ks):-!.\n"
    "'$lfx_dspecs'([A|B],Ks,K0):-!,'$lfx_dspecs'(A,Ks,K1),'$lfx_dspecs'(B,K1,K0).\n"
    "'$lfx_dspecs'(as(S,_),Ks,K0):-!,'$lfx_dspecs'(S,Ks,K0).\n"
    "'$lfx_dspecs'(F/N,[F/N|K0],K0):-atom(F),!.\n"
    "'$lfx_dspecs'(_,Ks,Ks).\n"
    "'$lfx_q'(user,F,F):-!.\n"
    "'$lfx_q'(M,F,Q):-atom_concat(M,':',P),atom_concat(P,F,Q).\n"
    "'$lfx_t'(Q,T):-atom_concat('$tbl ',Q,T).\n"
    "'$lfx_srcs'([],_,_).\n"
    "'$lfx_srcs'([F/N|R],M,A):-'$lfx_q'(M,F,Q),'$src_file_add'(Q,N,A),'$lfx_srcs'(R,M,A).\n"
    "'$lfx_wraps'([],_).\n"
    "'$lfx_wraps'([k(F/N,Md)|R],M):-'$lfx_q'(M,F,Q),'$lfx_t'(Q,T),functor(H,Q,N),H=..[_|As],I=..[T|As],catch(dynamic(T/N),_,true),catch(dynamic(Q/N),_,true),(Md==none->B='$tbl_call'(H,I);'$lfx_mspec'(Md,Sp),B='$tbl_mcall'(H,I,Sp)),(catch(clause(H,_),_,fail)->true;assertz((H:-B))),'$lfx_wraps'(R,M).\n"
    "'$lfx_mspec'(S,C):-copy_term(S,C),term_variables(C,Vs),'$lfx_idx'(Vs).\n"
    "'$lfx_idx'([]).\n"
    "'$lfx_idx'([index|R]):-'$lfx_idx'(R).\n"
    "'$lfx_terms'([],_,_,_,_,[]).\n"
    "'$lfx_terms'([T|Ts],M,Ks,Tb,A,Is):-'$lfx_term'(T,M,Ks,Tb,A,Is,I0),'$lfx_terms'(Ts,M,Ks,Tb,A,I0).\n"
    "'$lfx_term'(T,_,_,_,_,I,I):-var(T),!.\n"
    "'$lfx_term'((:-initialization(G)),M,Ks,_,_,[G2|I0],I0):-!,'$lfx_body'(G,M,Ks,G2).\n"
    "'$lfx_term'((:-D),M,Ks,_,A,I,I):-!,'$lfx_dir'(D,M,Ks,A).\n"
    "'$lfx_term'((?-D),M,Ks,_,A,I,I):-!,'$lfx_dir'(D,M,Ks,A).\n"
    "'$lfx_term'((H-->B),M,Ks,Tb,A,I,I0):-!,dcg_translate_rule((H-->B),C),'$lfx_term'(C,M,Ks,Tb,A,I,I0).\n"
    "'$lfx_term'(C,M,Ks,Tb,_,I,I):-'$lfx_clause'(C,M,Ks,Tb,C2),catch(assertz(C2),E,(format(user_error,'Warning: load raised: ~q~n',[E]))).\n"
    "'$lfx_dir'(D,_,_,_):-var(D),!.\n"
    "'$lfx_dir'(table(_),_,_,_):-!.\n"
    "'$lfx_dir'(op(_,_,_),_,_,_):-!.\n"
    "'$lfx_dir'(ensure_loaded(F),M,_,A):-!,'$lf_list'(F,Fl),'$lfx_rels'(Fl,A,Ps),load_files(M:Ps,[dialect(xsb),if(not_loaded)]).\n"
    "'$lfx_dir'([F|Fs],M,_,A):-!,'$lfx_rels'([F|Fs],A,Ps),load_files(M:Ps,[dialect(xsb)]).\n"
    "'$lfx_dir'(dynamic(S),M,_,_):-!,'$lfx_dspecs'(S,Ks,[]),'$lfx_dyns'(Ks,M).\n"
    "'$lfx_dir'(import(S),M,_,A):-!,'$lfx_import'(S,M,A).\n"
    "'$lfx_dir'(D,_,_,_):-functor(D,F,N),memberchk(F/N,[import/1,export/1,index/1,index/2,ti/1,mode/1,hilog/1,compiler_options/1,module/1,module/2,use_module/1,use_module/2,thread_shared/1,discontiguous/1,multifile/1,document_export/1,local/1]),!.\n"
    "'$lfx_dir'(D,M,Ks,_):-'$lfx_body'(D,M,Ks,D2),'$consult_goal'(D2).\n"
    "'$lfx_rels'([],_,[]).\n"
    "'$lfx_rels'([F|Fs],A,[P|Ps]):-'$lf_rel'(A,F,P),'$lfx_rels'(Fs,A,Ps).\n"
    "tfindall(T,G,L):-findall(T,G,L).\n"
    "'$lfx_dyns'([],_).\n"
    "'$lfx_dyns'([F/N|R],M):-'$lfx_q'(M,F,Q),catch(dynamic(Q/N),_,true),'$lfx_dyns'(R,M).\n"
    "'$lfx_clause'((H:-B),M,Ks,Tb,(H2:-B2)):-!,'$lfx_head'(H,M,Tb,H2),'$lfx_body'(B,M,Ks,B2).\n"
    "'$lfx_clause'(H,M,_,Tb,H2):-'$lfx_head'(H,M,Tb,H2).\n"
    "'$lfx_head'(H,M,Tb,H2):-callable(H),H\\=(_:_),!,functor(H,F,N),'$lfx_q'(M,F,Q),(memberchk(k(F/N,_),Tb)->'$lfx_t'(Q,F2);F2=Q),H=..[_|As],H2=..[F2|As].\n"
    "'$lfx_head'(H,_,_,H).\n"
    "'$lfx_body'(G,M,_,G2):-var(G),!,(M==user->G2=G;G2='$lfx_mc'(M,G)).\n"
    "'$lfx_body'(M2:G,_,_,'$lfx_mc'(M2,G)):-atom(M2),!.\n"
    "'$lfx_body'(get_residual(G,R),M,_,'$lfx_gres'(M,G,R)):-var(G),M\\==user,!.\n"
    "'$lfx_body'(get_returns_for_call(C,R),M,_,get_returns_for_call(Q,R)):-M\\==user,!,'$lfx_tmq'(M,C,Q).\n"
    "'$lfx_body'(get_calls(C,T,R),M,_,get_calls(Q,T,R)):-M\\==user,!,'$lfx_tmq'(M,C,Q).\n"
    "'$lfx_body'(G,M,_,'$lfx_mca'(M,C,Xs)):-M\\==user,compound(G),G=..[call,C|Xs],Xs\\==[],var(C),!.\n"
    "'$lfx_body'(G,M,Ks,G2):-compound(G),G=..[F,C|Xs],Xs\\==[],'$lfx_clof'(F),!,length(Xs,E),'$lfx_clo'(C,E,M,Ks,C2),G2=..[F,C2|Xs].\n"
    "'$lfx_body'(G,M,_,'$lfx_mdb'(F,M,C)):-M\\==user,compound(G),G=..[F,C],var(C),memberchk(F,[assert,asserta,assertz,retract,retractall]),!.\n"
    "'$lfx_body'(call_delays(G,D),M,Ks,'$call_delays_m'(M,G2,D)):-M\\==user,!,'$lfx_body'(G,M,Ks,G2).\n"
    "'$lfx_body'(call_residual_program(G,P),M,Ks,'$call_residual_m'(M,G2,P)):-M\\==user,!,'$lfx_body'(G,M,Ks,G2).\n"
    "'$lfx_body'(G,M,Ks,G2):-'$lfx_meta'(G,Sp),!,G=..[F|As],Sp=..[_|Ms],'$lfx_margs'(Ms,As,M,Ks,As2),G2=..[F|As2].\n"
    "'$lfx_body'(G,M,_,G2):-'$lfx_impq'(G,M,G2),!.\n"
    "'$lfx_body'(G,M,Ks,G2):-callable(G),functor(G,F,N),'$lfx_def'(F,N,M,Ks),!,'$lfx_q'(M,F,Q),G=..[_|As],G2=..[Q|As].\n"
    "'$lfx_def'(F,N,_,Ks):-memberchk(F/N,Ks),!.\n"
    "'$lfx_def'(F,N,M,_):-M\\==user,'$lfx_q'(M,F,Q),catch('$src_file_rt'(Q,N,_),error(existence_error(_,_),_),fail),!.\n"
    "'$lfx_body'(G,M,_,'$lfx_mc'(M,G)):-M\\==user,'$lfx_unk'(G),!.\n"
    "'$lfx_body'(G,_,_,G).\n"
    "'$lfx_unk'(G):-callable(G),functor(G,F,N),\\+ current_predicate(F/N),\\+ predicate_property(G,built_in).\n"
    "'$lfx_impq'(G,M,G2):-callable(G),functor(G,F,N),catch('$lfx_imp'(M,F,N,Mod),_,fail),!,'$lfx_q'(Mod,F,Q),G=..[_|As],G2=..[Q|As].\n"
    "'$lfx_import'(from(Ps,Mod),M,A):-atom(Mod),'$lf_rel'(A,Mod,P0),catch('$lf_path'(P0,[dialect(xsb)],P),_,fail),!,load_files(Mod:P,[dialect(xsb),if(not_loaded)]),'$lfx_imps'(Ps,M,Mod).\n"
    "'$lfx_import'(_,_,_).\n"
    "'$lfx_imps'(V,_,_):-var(V),!.\n"
    "'$lfx_imps'((A,B),M,Mod):-!,'$lfx_imps'(A,M,Mod),'$lfx_imps'(B,M,Mod).\n"
    "'$lfx_imps'(F/N,M,Mod):-atom(F),integer(N),M\\==Mod,!,assertz('$lfx_imp'(M,F,N,Mod)).\n"
    "'$lfx_imps'(_,_,_).\n"
    "'$lfx_mdb'(F,M,C):-'$lfx_mq'(M,C,C2),G=..[F,C2],call(G).\n"
    "'$lfx_mq'(_,C,C):-var(C),!.\n"
    "'$lfx_mq'(_,M:C,C2):-atom(M),!,'$lfx_mq'(M,C,C2).\n"
    "'$lfx_mq'(M,(H:-B),(H2:-B2)):-!,'$lfx_mh'(M,H,H2),(M==user->B2=B;B2='$lfx_mc'(M,B)).\n"
    "'$lfx_mq'(M,H,H2):-'$lfx_mh'(M,H,H2).\n"
    "'$lfx_mh'(M,H,H2):-callable(H),functor(H,F,N),atom(F),\\+ predicate_property(H,built_in),!,'$lfx_q'(M,F,Q),'$lfx_mt'(Q,N,F2),H=..[_|As],H2=..[F2|As].\n"
    "'$lfx_mh'(_,H,H).\n"
    "'$lfx_mt'(Q,N,T):-'$lfx_t'(Q,T),current_predicate(T/N),!.\n"
    "'$lfx_mt'(Q,_,Q).\n"
    "'$lfx_mc'(M,G):-call(M:G).\n"
    "'$lfx_gres'(M,G,R):-'$mod_qb'(M,G,Q),get_residual(Q,R).\n"
    "'$lfx_tmq'(_,C,C):-nonvar(C),C=(_:_),!.\n"
    "'$lfx_tmq'(M,C,M:C).\n"
    "'$lfx_mca'(M,C,Xs):-nonvar(C),C=(M2:C2),atom(M2),!,'$lfx_mca'(M2,C2,Xs).\n"
    "'$lfx_mca'(M,C,Xs):-C=..L0,append(L0,Xs,L1),G=..L1,call(M:G).\n"
    "'$lfx_clof'(call).\n"
    "'$lfx_clof'(maplist).\n"
    "'$lfx_clo'(C,_,M,_,C2):-var(C),!,(M==user->C2=C;C2=M:C).\n"
    "'$lfx_clo'(C,E,M,Ks,C2):-callable(C),functor(C,F,N0),K is N0+E,memberchk(F/K,Ks),!,'$lfx_q'(M,F,Q),C=..[_|As],C2=..[Q|As].\n"
    "'$lfx_clo'(C,_,_,_,C).\n"
    "'$lfx_margs'([],[],_,_,[]).\n"
    "'$lfx_margs'([S|Ss],[A|As],M,Ks,[B|Bs]):-'$lfx_marg'(S,A,M,Ks,B),'$lfx_margs'(Ss,As,M,Ks,Bs).\n"
    "'$lfx_marg'(0,A,M,Ks,B):-!,'$lfx_body'(A,M,Ks,B).\n"
    "'$lfx_marg'(^,A,M,Ks,B):-!,'$lfx_hat'(A,M,Ks,B).\n"
    "'$lfx_marg'(c,A,M,Ks,B):-!,'$lfx_cl'(A,M,Ks,B).\n"
    "'$lfx_marg'(p,A,M,Ks,B):-!,'$lfx_pi'(A,M,Ks,B).\n"
    "'$lfx_marg'(_,A,_,_,A).\n"
    "'$lfx_hat'(A,M,Ks,B):-nonvar(A),A=(V^G),!,B=(V^G2),'$lfx_hat'(G,M,Ks,G2).\n"
    "'$lfx_hat'(A,M,Ks,B):-'$lfx_body'(A,M,Ks,B).\n"
    "'$lfx_cl'(C,_,_,C):-var(C),!.\n"
    "'$lfx_cl'((H:-B),M,Ks,(H2:-B2)):-!,'$lfx_clh'(H,M,H2),'$lfx_body'(B,M,Ks,B2).\n"
    "'$lfx_cl'(H,M,_,H2):-'$lfx_clh'(H,M,H2).\n"
    "'$lfx_clh'(H,_,H):-var(H),!.\n"
    "'$lfx_clh'(M:H,_,H2):-atom(M),!,'$lfx_mh'(M,H,H2).\n"
    "'$lfx_clh'(H,M,H2):-'$lfx_mh'(M,H,H2).\n"
    "'$lfx_pi'(P,_,_,P):-var(P),!.\n"
    "'$lfx_pi'(F/N,M,Ks,Q/N):-atom(F),memberchk(F/N,Ks),!,'$lfx_q'(M,F,Q).\n"
    "'$lfx_pi'(G,M,Ks,G2):-callable(G),G\\=(_/_),!,'$lfx_body'(G,M,Ks,G2).\n"
    "'$lfx_pi'(A,_,_,A).\n"
    "'$lfx_meta'((_,_),','(0,0)).\n"
    "'$lfx_meta'((_;_),';'(0,0)).\n"
    "'$lfx_meta'((_->_),'->'(0,0)).\n"
    "'$lfx_meta'((_*->_),'*->'(0,0)).\n"
    "'$lfx_meta'(\\+(_),\\+(0)).\n"
    "'$lfx_meta'(call(_),call(0)).\n"
    "'$lfx_meta'(findall(_,_,_),findall(-,0,-)).\n"
    "'$lfx_meta'(findall(_,_,_,_),findall(-,0,-,-)).\n"
    "'$lfx_meta'(forall(_,_),forall(0,0)).\n"
    "'$lfx_meta'(bagof(_,_,_),bagof(-,^,-)).\n"
    "'$lfx_meta'(setof(_,_,_),setof(-,^,-)).\n"
    "'$lfx_meta'(aggregate_all(_,_,_),aggregate_all(-,0,-)).\n"
    "'$lfx_meta'(tnot(_),tnot(0)).\n"
    "'$lfx_meta'(not(_),not(0)).\n"
    "'$lfx_meta'(once(_),once(0)).\n"
    "'$lfx_meta'(ignore(_),ignore(0)).\n"
    "'$lfx_meta'(catch(_,_,_),catch(0,-,0)).\n"
    "'$lfx_meta'(tfindall(_,_,_),tfindall(-,0,-)).\n"
    "'$lfx_meta'(with_output_to(_,_),with_output_to(-,0)).\n"
    "'$lfx_meta'(get_residual(_,_),get_residual(0,-)).\n"
    "'$lfx_meta'(call_delays(_,_),call_delays(0,-)).\n"
    "'$lfx_meta'(call_residual_program(_,_),call_residual_program(0,-)).\n"
    "'$lfx_meta'(sk_not(_),sk_not(0)).\n"
    "'$lfx_meta'(u_not(_),u_not(0)).\n"
    "'$lfx_meta'(not_exists(_),not_exists(0)).\n"
    "'$lfx_meta'(call_cleanup(_,_),call_cleanup(0,0)).\n"
    "'$lfx_meta'(setup_call_cleanup(_,_,_),setup_call_cleanup(0,0,0)).\n"
    "'$lfx_meta'(assert(_),assert(c)).\n"
    "'$lfx_meta'(asserta(_),asserta(c)).\n"
    "'$lfx_meta'(assertz(_),assertz(c)).\n"
    "'$lfx_meta'(retract(_),retract(c)).\n"
    "'$lfx_meta'(retractall(_),retractall(c)).\n"
    "'$lfx_meta'(abolish(_),abolish(p)).\n"
    "'$lfx_meta'(abolish_table_pred(_),abolish_table_pred(p)).\n"
    "'$lfx_meta'(abolish_table_subgoals(_),abolish_table_subgoals(0)).\n"
    "'$lfx_meta'(abolish_table_call(_),abolish_table_call(0)).\n"
    "'$lfx_meta'(get_calls(_,_,_),get_calls(0,-,-)).\n"
    "'$lfx_meta'(get_call(_,_,_),get_call(0,-,-)).\n"
    "'$lfx_meta'(current_table(_,_),current_table(0,-)).\n"
    "'$lfx_meta'(time(_),time(0)).\n"
    "source_file(H,_):-var(H),!,fail.\n"
    "source_file(M:H,F):-atom(M),!,source_file(H,F).\n"
    "source_file(H,F):-callable(H),functor(H,N,A),catch('$src_file_rt'(Q,A,F0),error(existence_error(_,_),_),fail),'$sf_name'(Q,N),!,F=F0.\n"
    "'$sf_name'(N,N):-!.\n"
    "'$sf_name'(Q,N):-atom_concat(P,N,Q),sub_atom(P,_,1,0,':').\n"
    "'$src_file_add'(N,A,F):-(catch('$src_file_rt'(N,A,F),error(existence_error(_,_),_),fail)->true;assertz('$src_file_rt'(N,A,F))).\n"
    "'$load_item'(T,L,U,E,M):-(catch('$load_item1'(T,U,E,M),X,(format(user_error,'Warning: line ~w: load raised: ~q~n',[L,X]),true))->true;format(user_error,'Warning: line ~w: load failed: ~q~n',[L,T])).\n"
    "'$load_item1'(T,U,0,_):-!,'$load_one'(T,U).\n"
    "'$load_item1'(T,U,_,_):-U\\==0,'$load_unq'(T,U,M,T1),!,expand_term(T1,X),(X==T1->'$load_one'(T,U);'$load_mq'(X,M,Y),'$load_items'(Y,U)).\n"
    "'$load_item1'(T,U,_,M):-M\\==0,'$load_munq'(T,M,T1),!,'$expand_term1'(T1,X0,_),(X0==T1->'$expand_bodies'(T,X);'$expand_bodies'(X0,X)),'$load_items'(X,U).\n"
    "'$load_item1'(T,U,_,_):-expand_term(T,X),'$load_items'(X,U).\n"
    "'$load_munq'((H:-B),M,(H1:-B)):-!,'$load_munq'(H,M,H1).\n"
    "'$load_munq'(H,M,H1):-callable(H),H=..[F|As],atom_concat(M,':',P),atom_concat(P,F1,F),H1=..[F1|As].\n"
    "'$load_unq'((H:-B),U,M,(H1:-B)):-!,'$load_unq'(H,U,M,H1).\n"
    "'$load_unq'(H,U,M,H1):-callable(H),atom_concat(plunit_,U,M),atom_concat(M,':',P),H=..[F|As],atom_concat(P,F1,F),H1=..[F1|As].\n"
    "'$load_mq'(X,_,X):-var(X),!.\n"
    "'$load_mq'([],_,[]):-!.\n"
    "'$load_mq'([C|Cs],M,[D|Ds]):-!,'$load_mq1'(C,M,D),'$load_mq'(Cs,M,Ds).\n"
    "'$load_mq'(C,M,D):-'$load_mq1'(C,M,D).\n"
    "'$load_mq1'(C,_,C):-var(C),!.\n"
    "'$load_mq1'((:-D),_,(:-D)):-!.\n"
    "'$load_mq1'((test(N):-B),M,(test(N):-M:B)):-!.\n"
    "'$load_mq1'((test(N,O):-B),M,(test(N,O):-M:B)):-!.\n"
    "'$load_mq1'(test(N),_,test(N)):-!.\n"
    "'$load_mq1'(test(N,O),_,test(N,O)):-!.\n"
    "'$load_mq1'((H:-B),M,(H1:-M:B)):-!,'$load_mq1'(H,M,H1).\n"
    "'$load_mq1'(H,M,H1):-callable(H),!,H=..[F|As],atom_concat(M,':',P),atom_concat(P,F,F1),H1=..[F1|As].\n"
    "'$load_mq1'(C,_,C).\n"
    "'$load_items'(X,U):-var(X),!,'$load_one'(X,U).\n"
    "'$load_items'([],_):-!.\n"
    "'$load_items'([T|Ts],U):-!,'$load_one'(T,U),'$load_items'(Ts,U).\n"
    "'$load_items'(T,U):-'$load_one'(T,U).\n"
    "'$load_one'((:-D),_):-!,'$consult_goal'(D).\n"
    "'$load_one'((H-->B),U):-!,dcg_translate_rule((H-->B),C),'$load_one'(C,U).\n"
    "'$load_one'(C,U):-U\\==0,'$load_test'(C,N,O,B),!,assertz(pj_test(U,N,O,B)),assertz(C).\n"
    "'$load_one'(C,_):-assertz(C).\n"
    "'$load_test'((test(N):-B),N,[],B):-!.\n"
    "'$load_test'((test(N,O):-B),N,Os,B):-!,'$load_test_opts'(O,Os).\n"
    "'$load_test'(test(N),N,[],true):-!.\n"
    "'$load_test'(test(N,O),N,Os,true):-'$load_test_opts'(O,Os).\n"
    "'$load_test_opts'(O,O):-is_list(O),!.\n"
    "'$load_test_opts'(O,[O]).\n"
    "'$rand_seed'(S):-catch(nb_getval('$rand_seed',S),error(existence_error(_,_),_),S=12345).\n"
    "'$rand_next'(R):-'$rand_seed'(S0),R is (S0*1103515245+12345) mod 2147483648,nb_setval('$rand_seed',R).\n"
    "'$rand_out'(X,P):-nonvar(X),!,throw(error(uninstantiation_error(X),P)).\n"
    "'$rand_out'(_,_).\n"
    "'$rand_num'(V,P):-var(V),!,throw(error(instantiation_error,P)).\n"
    "'$rand_num'(V,P):-number(V),!.\n"
    "'$rand_num'(V,P):-throw(error(type_error(number,V),P)).\n"
    "random(X):-'$rand_out'(X,random/1),'$rand_next'(R),X is (R+0.5)/2147483648.0.\n"
    "random_permutation(A,B):-is_list(A),!,'$rperm'(A,B).\n"
    "random_permutation(A,B):-is_list(B),!,'$rperm'(B,A).\n"
    "random_permutation(A,B):-var(A),var(B),!,throw(error(instantiation_error,random_permutation/2)).\n"
    "random_permutation(A,_):-throw(error(type_error(list,A),random_permutation/2)).\n"
    "'$rperm'(L,P):-'$rperm_k'(L,K),keysort(K,S),'$rperm_v'(S,P0),P=P0.\n"
    "'$rperm_k'([],[]).\n"
    "'$rperm_k'([H|T],[R-H|K]):-random(R),'$rperm_k'(T,K).\n"
    "'$rperm_v'([],[]).\n"
    "'$rperm_v'([_-V|T],[V|P]):-'$rperm_v'(T,P).\n"
    "random(L,H,X):-'$rand_num'(L,random/3),'$rand_num'(H,random/3),'$rand_out'(X,random/3),H>L,'$rand_next'(R),"
        "(integer(L),integer(H)->X is L+R mod (H-L);X is L+(H-L)*((R+0.5)/2147483648.0)).\n"
    "set_seed(S):-var(S),!,throw(error(instantiation_error,set_seed/1)).\n"
    "set_seed(S):- \\+integer(S),!,throw(error(type_error(integer,S),set_seed/1)).\n"
    "set_seed(S):-S<0,!,throw(error(domain_error(not_less_than_zero,S),set_seed/1)).\n"
    "set_seed(S):-S1 is S mod 2147483648,nb_setval('$rand_seed',S1).\n"
    "get_seed(S):-nonvar(S),\\+ integer(S),!,throw(error(type_error(integer,S),get_seed/1)).\n"
    "get_seed(S):-'$rand_seed'(S).\n"
    "randomize:-'$rand_next'(_).\n"
    "random_between(L,H,X):-'$rand_int'(L),'$rand_int'(H),H>=L,'$rand_next'(R),X is L+R mod (H-L+1).\n"
    "'$rand_int'(V):-var(V),!,throw(error(instantiation_error,random_between/3)).\n"
    "'$rand_int'(V):-integer(V),!.\n"
    "'$rand_int'(V):-throw(error(type_error(integer,V),random_between/3)).\n";
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pl_clause_dcg(const PlClause *cl) {
    tree_t *r;
    if (!cl || !cl->tr || cl->tr->n != 1 || !(r = cl->tr->c[0]) || r->t != TT_FNC || r->n != 2 || !r->v.sval || strcmp(r->v.sval, "-->")) return (tree_t *)0;
    return r;
}
static tree_t *pl_clause_ssu(PlClause *cl) {
    tree_t *r;
    if (!cl || !cl->tr || cl->tr->n != 1 || !(r = cl->tr->c[0]) || r->t != TT_FNC || r->n != 2 || !r->v.sval || strcmp(r->v.sval, "=>")) return (tree_t *)0;
    return r;
}
static int pl_clause_key(PlClause *cl, const char **name_out, int *ar_out) {
    tree_t *dcg = pl_clause_dcg(cl);
    tree_t *ssu = pl_clause_ssu(cl);
    if (!cl) return 0;
    if (ssu) {
        tree_t *hd = ssu->c[0];
        if (hd && hd->t == TT_FNC && hd->n == 2 && hd->v.sval && !strcmp(hd->v.sval, ",")) hd = hd->c[0];
        if (hd && hd->t == TT_QLIT && hd->v.sval) { *name_out = hd->v.sval; *ar_out = 0;      return 1; }
        if (hd && hd->t == TT_FNC  && hd->v.sval) { *name_out = hd->v.sval; *ar_out = hd->n;  return 1; }
        return 0;
    }
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
    tree_t *ssu = pl_clause_ssu(cl);
    if (dcg) return dcg->c[1];
    if (ssu) return ssu;
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
static int pl_agg_native_calls(const tree_t *t) {
    if (!t) return 0;
    int n = (t->t == TT_FNC && t->n == 3 && t->v.sval && !strcmp(t->v.sval, "aggregate_all") && pl_aggregate_all_native_kind(t->c[0])) ? 1 : 0;
    for (int i = 0; i < t->n; i++) n += pl_agg_native_calls(t->c[i]);
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_word_count(const char *src, const char *w) {
    size_t wl = strlen(w);
    int n = 0;
    for (const char *p = src; (p = strstr(p, w)) != NULL; p += wl) {
        char before = (p == src) ? ' ' : p[-1], after = p[wl];
        if (!(before == '_' || isalnum((unsigned char)before)) && !(after == '_' || isalnum((unsigned char)after))) n++;
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_dcg_ctrl(const tree_t *b, int n, const char *op) { return b->t == TT_FNC && b->v.sval && b->n == n && !strcmp(b->v.sval, op); }
static void pl_tree_collect_dcg_keys(const tree_t *b, cv_t *keys) {
    if (!b) return;
    if (pl_dcg_ctrl(b, 2, ",") || pl_dcg_ctrl(b, 2, ";") || pl_dcg_ctrl(b, 2, "->") || pl_dcg_ctrl(b, 2, "|")) { pl_tree_collect_dcg_keys(b->c[0], keys); pl_tree_collect_dcg_keys(b->c[1], keys); return; }
    if (pl_dcg_ctrl(b, 1, "\\+")) { pl_tree_collect_dcg_keys(b->c[0], keys); return; }
    if (!((b->t == TT_FNC || b->t == TT_QLIT || b->t == TT_NAME) && b->v.sval)) return;
    if (!strcmp(b->v.sval, "{}") || !strcmp(b->v.sval, ".") || !strcmp(b->v.sval, "[]") || !strcmp(b->v.sval, "!") || !strcmp(b->v.sval, "call")) return;
    char k[strlen(b->v.sval) + 16];
    snprintf(k, sizeof k, "%s/%d", b->v.sval, (b->t == TT_FNC ? b->n : 0) + 2);
    pl_cv_add(keys, k);
}
static void pl_tree_collect_keys(const tree_t *t, cv_t *keys) {
    if (!t) return;
    if ((t->t == TT_FNC || t->t == TT_QLIT || t->t == TT_NAME) && t->v.sval) { char k[strlen(t->v.sval) + 16]; snprintf(k, sizeof k, "%s/%d", t->v.sval, t->t == TT_FNC ? t->n : 0); pl_cv_add(keys, k); }
    if (pl_dcg_ctrl(t, 2, "-->")) pl_tree_collect_dcg_keys(t->c[1], keys);
    if (pl_dcg_ctrl(t, 2, "phrase") || pl_dcg_ctrl(t, 3, "phrase")) pl_tree_collect_dcg_keys(t->c[0], keys);
    for (int i = 0; i < t->n; i++) pl_tree_collect_keys(t->c[i], keys);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cv_has_key(const cv_t *v, const char *nm, int ar);
static int pl_base_defines(PlClause *first, PlClause *last, const char *nm, int ar) {
    for (PlClause *cl = first; cl; cl = cl->next) {
        const char *cn; int car;
        if (pl_clause_key(cl, &cn, &car) && cn && car == ar && !strcmp(cn, nm)) return 1;
        if (cl == last) break;
    }
    return 0;
}
typedef struct { const char *s; int n; int ar; } pl_pk_t;
static uint32_t pl_pk_hname(const char *s, int n) {
    uint32_t h = 2166136261u;
    for (int i = 0; i < n; i++) h = (h ^ (unsigned char)s[i]) * 16777619u;
    return h;
}
static uint32_t pl_pk_hash(const char *s, int n, int ar) { return pl_pk_hname(s, n) ^ ((uint32_t)ar * 2654435761u); }
static uint32_t pl_pk_cap(uint32_t n) { uint32_t c = 16; while (c < 2 * n + 2) c <<= 1; return c; }
static void pl_pk_put(pl_pk_t *t, uint32_t cap, const char *s, int n, int ar) {
    for (uint32_t i = pl_pk_hash(s, n, ar) & (cap - 1);; i = (i + 1) & (cap - 1)) {
        if (!t[i].s) { t[i].s = s; t[i].n = n; t[i].ar = ar; return; }
        if (t[i].n == n && t[i].ar == ar && !memcmp(t[i].s, s, (size_t)n)) return;
    }
}
static int pl_pk_has_h(const pl_pk_t *t, uint32_t cap, const char *s, int n, uint32_t hn, int ar) {
    for (uint32_t i = (hn ^ ((uint32_t)ar * 2654435761u)) & (cap - 1);; i = (i + 1) & (cap - 1)) {
        if (!t[i].s) return 0;
        if (t[i].n == n && t[i].ar == ar && !memcmp(t[i].s, s, (size_t)n)) return 1;
    }
}
static int pl_pk_has(const pl_pk_t *t, uint32_t cap, const char *s, int ar) { int n = (int)strlen(s); return pl_pk_has_h(t, cap, s, n, pl_pk_hname(s, n), ar); }
typedef struct { pl_pk_t *t; uint32_t cap, n; } pl_pk_set_t;
static void pl_pk_set_add(pl_pk_set_t *h, const char *s, int n, int ar) {
    if (2 * (h->n + 1) > h->cap) {
        uint32_t nc = h->cap ? h->cap * 2 : 256;
        pl_pk_t *nt = (pl_pk_t *)ct_zalloc(nc, sizeof(pl_pk_t));
        for (uint32_t i = 0; i < h->cap; i++) if (h->t[i].s) pl_pk_put(nt, nc, h->t[i].s, h->t[i].n, h->t[i].ar);
        if (h->t) ct_drop(h->t);
        h->t = nt;
        h->cap = nc;
    }
    for (uint32_t i = pl_pk_hash(s, n, ar) & (h->cap - 1);; i = (i + 1) & (h->cap - 1)) {
        if (!h->t[i].s) { h->t[i].s = s; h->t[i].n = n; h->t[i].ar = ar; h->n++; return; }
        if (h->t[i].n == n && h->t[i].ar == ar && !memcmp(h->t[i].s, s, (size_t)n)) return;
    }
}
static void pl_tree_collect_dcg_pk(const tree_t *b, pl_pk_set_t *h) {
    if (!b) return;
    if (pl_dcg_ctrl(b, 2, ",") || pl_dcg_ctrl(b, 2, ";") || pl_dcg_ctrl(b, 2, "->") || pl_dcg_ctrl(b, 2, "|")) { pl_tree_collect_dcg_pk(b->c[0], h); pl_tree_collect_dcg_pk(b->c[1], h); return; }
    if (pl_dcg_ctrl(b, 1, "\\+")) { pl_tree_collect_dcg_pk(b->c[0], h); return; }
    if (!((b->t == TT_FNC || b->t == TT_QLIT || b->t == TT_NAME) && b->v.sval)) return;
    if (!strcmp(b->v.sval, "{}") || !strcmp(b->v.sval, ".") || !strcmp(b->v.sval, "[]") || !strcmp(b->v.sval, "!") || !strcmp(b->v.sval, "call")) return;
    pl_pk_set_add(h, b->v.sval, (int)strlen(b->v.sval), (b->t == TT_FNC ? b->n : 0) + 2);
}
static void pl_tree_collect_pk(const tree_t *t, pl_pk_set_t *h) {
    if (!t) return;
    if ((t->t == TT_FNC || t->t == TT_QLIT || t->t == TT_NAME) && t->v.sval) pl_pk_set_add(h, t->v.sval, (int)strlen(t->v.sval), t->t == TT_FNC ? t->n : 0);
    if (pl_dcg_ctrl(t, 2, "-->")) pl_tree_collect_dcg_pk(t->c[1], h);
    if (pl_dcg_ctrl(t, 2, "phrase") || pl_dcg_ctrl(t, 3, "phrase")) pl_tree_collect_dcg_pk(t->c[0], h);
    for (int i = 0; i < t->n; i++) pl_tree_collect_pk(t->c[i], h);
}
static void pl_pk_fill_cv(pl_pk_t *t, uint32_t cap, const cv_t *v) {
    for (uint32_t i = 0; v && i < v->len; i++) {
        const char *e = CV_AT(*v, char *, i), *sl = strrchr(e, '/');
        if (sl) pl_pk_put(t, cap, e, (int)(sl - e), (int)strtol(sl + 1, (char **)0, 10));
    }
}
static int pl_lib_quiet(const pl_prelude_lib_t *lib, const char *nm, int ar) {
    for (const pl_prelude_lib_pi_t *q = lib->quiet; q && q->nm; q++) if (q->ar == ar && !strcmp(q->nm, nm)) return 1;
    return 0;
}
static int pl_lib_named(const pl_prelude_lib_t *lib, const pl_pk_t *rk, uint32_t rkc, const pl_pk_t *bk, uint32_t bkc, PlClause *base, PlClause *base_last, const char *native_only, const pl_pk_t *ud,
    uint32_t udc, int imported) {
    for (const pl_prelude_lib_pi_t *e = lib->exports; e->nm; e++) {
        int n = (int)strlen(e->nm);
        uint32_t hn = pl_pk_hname(e->nm, n);
        if (native_only && !strcmp(e->nm, native_only)) continue;
        if (ud && pl_pk_has_h(ud, udc, e->nm, n, hn, e->ar)) continue;
        if (!pl_pk_has_h(rk, rkc, e->nm, n, hn, e->ar) && !(e->ar && pl_pk_has_h(rk, rkc, e->nm, n, hn, 0) && (imported || !pl_lib_quiet(lib, e->nm, 0)))) continue;
        if (pl_lib_quiet(lib, e->nm, e->ar)) continue;
        if (!(bk ? pl_pk_has_h(bk, bkc, e->nm, n, hn, e->ar) : pl_base_defines(base, base_last, e->nm, e->ar))) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_preprocess_list(PlProgram *prog, int depth);
static int pl_clause_is_attv_hook(PlClause *cl) {
    const tree_t *h = (cl && cl->tr && cl->tr->t == TT_CLAUSE && cl->tr->n >= 1) ? cl->tr->c[0] : (const tree_t *)0;
    if (!h || h->t != TT_FNC || !h->v.sval || strcmp(h->v.sval, ":") || h->n != 2 || !h->c[1] || h->c[1]->t != TT_FNC || !h->c[1]->v.sval) return 0;
    return (!strcmp(h->c[1]->v.sval, "attr_unify_hook") && h->c[1]->n == 2) || (!strcmp(h->c[1]->v.sval, "attribute_goals") && h->c[1]->n == 3);
}
static int pl_clause_is_directive(PlClause *cl) { return cl && cl->tr && cl->tr->t == TT_CLAUSE && cl->tr->n >= 2 && cl->tr->c[0] && cl->tr->c[0]->t == TT_NUL; }
static PlProgram *pl_prelude_parse(const char *src, int lib, const char *vis, int nlibs) {
    OpEntry *uo = g_parser.pl.uinfix;
    int un = g_parser.pl.uinfix_n, uc = g_parser.pl.uinfix_cap;
    g_parser.pl.uinfix = NULL;
    g_parser.pl.uinfix_n = 0;
    g_parser.pl.uinfix_cap = 0;
    for (int j = 0; j < nlibs; j++) if (vis[j] || j == lib) register_lib_ops(PL_PRELUDE_LIBS[j].lib);
    PlProgram *p = prolog_parse(src, "<prelude>");
    g_parser.pl.uinfix = uo;
    g_parser.pl.uinfix_n = un;
    g_parser.pl.uinfix_cap = uc;
    if (lib >= 0) register_lib_ops(PL_PRELUDE_LIBS[lib].lib);
    if (p && lib >= 0) pl_preprocess_list(p, 1);
    return p;
}
static void pl_libs_append(PlProgram *pre, cv_t *referenced, const cv_t *refkeys, const char *native_only, const cv_t *user_defined, const cv_t *imported) {
    int nlibs = 0;
    while (PL_PRELUDE_LIBS[nlibs].lib) nlibs++;
    PlProgram *lp[nlibs + 1];
    char seen[nlibs + 1], imp[nlibs + 1], vis[nlibs + 1];
    PlClause *base_last = pre->tail;
    int any = 0;
    uint32_t rkc = pl_pk_cap(refkeys ? refkeys->len : 0), udc = pl_pk_cap(user_defined ? user_defined->len : 0), nb = 0;
    pl_pk_t rk[rkc], ud[udc];
    memset(rk, 0, sizeof rk);
    memset(ud, 0, sizeof ud);
    pl_pk_fill_cv(rk, rkc, refkeys);
    pl_pk_fill_cv(ud, udc, user_defined);
    for (int i = 0; i < nlibs; i++) imp[i] = (char)(imported && pl_cv_has(imported, PL_PRELUDE_LIBS[i].lib));
    for (int i = 0; i < nlibs; i++) vis[i] = imp[i];
    for (int i = 0; i < nlibs; i++) { lp[i] = NULL; seen[i] = 0; if (pl_lib_named(&PL_PRELUDE_LIBS[i], rk, rkc, NULL, 0, pre->head, base_last, native_only, user_defined ? ud : NULL, udc, imp[i])) { lp[i] = pl_prelude_parse(PL_PRELUDE_LIBS[i].src, i, vis, nlibs); vis[i] = 1; any = 1; } }
    if (!any) return;
    for (PlClause *cl = pre->head; cl; cl = cl->next) { nb++; if (cl == base_last) break; }
    uint32_t bkc = pl_pk_cap(nb);
    pl_pk_t bk[bkc];
    memset(bk, 0, sizeof bk);
    for (PlClause *cl = pre->head; cl; cl = cl->next) {
        const char *cn; int car;
        if (pl_clause_key(cl, &cn, &car) && cn) pl_pk_put(bk, bkc, cn, (int)strlen(cn), car);
        if (cl == base_last) break;
    }
    for (int changed = 1; changed;) {
        changed = 0;
        for (int i = 0; i < nlibs; i++) {
            pl_pk_set_t lh = { 0 };
            if (!lp[i] || seen[i]) continue;
            for (PlClause *cl = lp[i]->head; cl; cl = cl->next) pl_tree_collect_pk(cl->tr, &lh);
            seen[i] = 1;
            if (!lh.t) { lh.cap = 16; lh.t = (pl_pk_t *)ct_zalloc(lh.cap, sizeof(pl_pk_t)); }
            for (int j = 0; j < nlibs; j++) if (!lp[j] && pl_lib_named(&PL_PRELUDE_LIBS[j], lh.t, lh.cap, bk, bkc, pre->head, base_last, NULL, user_defined ? ud : NULL, udc, imp[j])) { lp[j] = pl_prelude_parse(PL_PRELUDE_LIBS[j].src, j, vis, nlibs); vis[j] = 1; changed = 1; }
            ct_drop(lh.t);
        }
    }
    for (int i = 0; i < nlibs; i++) {
        if (!lp[i]) continue;
        for (int j = 0; j < nlibs; j++) {
            if (!lp[j] || j == i) continue;
            for (const pl_prelude_lib_pi_t *sh = PL_PRELUDE_LIBS[j].shadows; sh && sh->nm; sh++) {
                PlClause *prev = NULL, *nx = NULL;
                for (PlClause *cl = lp[i]->head; cl; cl = nx) {
                    const char *cn; int car;
                    nx = cl->next;
                    if (!pl_clause_key(cl, &cn, &car) || !cn || car != sh->ar || strcmp(cn, sh->nm)) { prev = cl; continue; }
                    if (prev) prev->next = nx; else lp[i]->head = nx;
                    if (lp[i]->tail == cl) lp[i]->tail = prev;
                    lp[i]->nclauses--;
                }
            }
        }
        if (pl_word_referenced(PL_PRELUDE_LIBS[i].src, "phrase")) pl_cv_add(referenced, "$phrase");
        if (pl_word_referenced(PL_PRELUDE_LIBS[i].src, "predicate_property")) pl_cv_add(referenced, "$pl_pp_pcheck");
        if (strstr(PL_PRELUDE_LIBS[i].src, "=>")) pl_cv_add(referenced, "subsumes_term");
        if (pl_word_referenced(PL_PRELUDE_LIBS[i].src, "bagof") || pl_word_referenced(PL_PRELUDE_LIBS[i].src, "setof")) { pl_cv_add(referenced, "$bagof_var"); pl_cv_add(referenced, "$setof_var"); }
        if (pl_word_referenced(PL_PRELUDE_LIBS[i].src, "write_canonical")) pl_cv_add(referenced, "$write_canonical_swi");
        if (pl_word_referenced(PL_PRELUDE_LIBS[i].src, "display")) pl_cv_add(referenced, "$display_swi");
        if (imp[i] && (pl_word_referenced(PL_PRELUDE_LIBS[i].src, "put_attr") || pl_word_referenced(PL_PRELUDE_LIBS[i].src, "freeze"))) pl_cv_add(referenced, "$wakeup");
        PlClause *nextc = NULL;
        for (PlClause *cl = lp[i]->head; cl; cl = nextc) {
            nextc = cl->next;
            const char *nm; int ar;
            if (!pl_clause_is_directive(cl) && !pl_clause_is_attv_hook(cl) && (!pl_clause_key(cl, &nm, &ar) || !nm || (nm[0] != '$' && pl_pk_has(bk, bkc, nm, ar)))) continue;
            cl->next = NULL;
            cl->dq_forced = 3 + 1;
            if (!pre->head) pre->head = cl; else pre->tail->next = cl;
            pre->tail = cl;
            pre->nclauses++;
        }
        ct_drop(lp[i]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * const pl_attv_api[] = { "put_attr/3", "get_attr/3", "del_attr/2", "put_attrs/2", "get_attrs/2", "del_attrs/1", "attvar/1", "freeze/2", "frozen/2", "dif/2", "when/2",
    "term_attvars/2", "call_residue_vars/2", "copy_term/3", "attr_unify_hook/2", "clpfd/0", "fd_domain/3", "fd_domain/2", "fd_labeling/1", "fd_labeling/2", "fd_labelingff/1",
    "fd_all_different/1", "fd_element/3", "fd_relation/2", NULL };
static void pl_attv_term_arms(const tree_t *t, unsigned char *seen) {
    if (!t) return;
    if ((t->t == TT_FNC || t->t == TT_QLIT || t->t == TT_NAME) && t->v.sval) {
        int ar = t->t == TT_FNC ? t->n : 0;
        for (int i = 0; pl_attv_api[i]; i++) {
            const char *sl = strchr(pl_attv_api[i], '/');
            size_t nl = (size_t)(sl - pl_attv_api[i]);
            if (!seen[i] && pl_attv_api[i][0] == t->v.sval[0] && strlen(t->v.sval) == nl && !memcmp(t->v.sval, pl_attv_api[i], nl) && atoi(sl + 1) == ar) seen[i] = 1;
        }
    }
    for (int i = 0; i < t->n; i++) pl_attv_term_arms(t->c[i], seen);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_tree_collect_declared(const tree_t *t, cv_t *keys) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "/") && t->n == 2 && t->c[0] && (t->c[0]->t == TT_QLIT || t->c[0]->t == TT_NAME) && t->c[0]->v.sval && t->c[1] && t->c[1]->t == TT_ILIT) {
        pl_cv_add(keys, pl_pred_key(t->c[0]->v.sval, (int)t->c[1]->v.ival));
        return;
    }
    for (int i = 0; i < t->n; i++) pl_tree_collect_declared(t->c[i], keys);
}
static int pl_tree_has_fn(const tree_t *t, const char *fn) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, fn)) return 1;
    for (int i = 0; i < t->n; i++) if (pl_tree_has_fn(t->c[i], fn)) return 1;
    return 0;
}
static int pl_tree_var_clause_goal(const tree_t *t) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->v.sval && t->n >= 1 && t->c[0] && t->c[0]->t == TT_VAR) {
        const char *f = t->v.sval;
        if (((t->n == 1 || t->n == 2) && (!strcmp(f, "assert") || !strcmp(f, "asserta") || !strcmp(f, "assertz"))) || (t->n == 1 && (!strcmp(f, "retract") || !strcmp(f, "retractall"))) || ((t->n == 2 || t->n == 3) && !strcmp(f, "clause"))) return 1;
    }
    for (int i = 0; i < t->n; i++) if (pl_tree_var_clause_goal(t->c[i])) return 1;
    return 0;
}
static size_t pl_lib_path_name(const tree_t *t, char *out) {
    size_t a, b;
    if (!t) return 0;
    if ((t->t == TT_NAME || t->t == TT_QLIT) && t->v.sval) { a = strlen(t->v.sval); if (out) memcpy(out, t->v.sval, a + 1); return a; }
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "/") || t->n != 2 || !(a = pl_lib_path_name(t->c[0], out))) return 0;
    if (out) out[a] = '_';
    if (!(b = pl_lib_path_name(t->c[1], out ? out + a + 1 : out))) return 0;
    return a + 1 + b;
}
static void pl_collect_imported_libs(PlProgram *prog, cv_t *imported) {
    for (PlClause *cl = prog->head; cl; cl = cl->next) {
        const tree_t *g = pl_clause_is_directive(cl) ? cl->tr->c[1] : (const tree_t *)0;
        size_t nl;
        if (!g || g->t != TT_FNC || !g->v.sval || strcmp(g->v.sval, "use_module") || (g->n != 1 && g->n != 2) || !g->c[0] || g->c[0]->t != TT_FNC || !g->c[0]->v.sval) continue;
        if (strcmp(g->c[0]->v.sval, "library") || g->c[0]->n != 1 || !(nl = pl_lib_path_name(g->c[0]->c[0], (char *)0))) continue;
        char nm[nl + 1];
        pl_lib_path_name(g->c[0]->c[0], nm);
        pl_cv_add(imported, nm);
        if (!strcmp(nm, "pio")) pl_cv_add(imported, "pure_input");
    }
}
static int pl_body_has_list_goal(const tree_t *b) {
    if (!b) return 0;
    if (b->t == TT_MAKELIST && b->n > 0) return 1;
    if (b->t != TT_FNC || !b->v.sval) return 0;
    if (b->n == 2 && (!strcmp(b->v.sval, ",") || !strcmp(b->v.sval, ";") || !strcmp(b->v.sval, "->") || !strcmp(b->v.sval, "*->") || !strcmp(b->v.sval, "forall")))
        return pl_body_has_list_goal(b->c[0]) || pl_body_has_list_goal(b->c[1]);
    if (b->n == 3 && !strcmp(b->v.sval, "catch")) return pl_body_has_list_goal(b->c[0]) || pl_body_has_list_goal(b->c[2]);
    if ((b->n == 3 || b->n == 4) && (!strcmp(b->v.sval, "findall") || !strcmp(b->v.sval, "bagof") || !strcmp(b->v.sval, "setof"))) return pl_body_has_list_goal(b->c[1]);
    if (b->n >= 1 && !strcmp(b->v.sval, "call")) return pl_body_has_list_goal(b->c[0]);
    return b->n == 1 && (!strcmp(b->v.sval, "\\+") || !strcmp(b->v.sval, "once") || !strcmp(b->v.sval, "ignore") || !strcmp(b->v.sval, "not")) && pl_body_has_list_goal(b->c[0]);
}
static int pl_dcg_body_var_goal(const tree_t *b) {
    if (!b) return 0;
    if (b->t == TT_VAR) return 1;
    if (b->t != TT_FNC || !b->v.sval) return 0;
    if (b->n == 2 && (!strcmp(b->v.sval, ",") || !strcmp(b->v.sval, ";") || !strcmp(b->v.sval, "|") || !strcmp(b->v.sval, "->") || !strcmp(b->v.sval, "*->"))) return pl_dcg_body_var_goal(b->c[0]) || pl_dcg_body_var_goal(b->c[1]);
    return b->n == 1 && !strcmp(b->v.sval, "\\+") && pl_dcg_body_var_goal(b->c[0]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *nm; int i; } pl_ckey_t;
static int pl_ckey_cmp(const void *a, const void *b) { const pl_ckey_t *x = (const pl_ckey_t *)a, *y = (const pl_ckey_t *)b; int c = strcmp(x->nm, y->nm); return c ? c : (x->i > y->i) - (x->i < y->i); }
static int pl_ckey_lower(const pl_ckey_t *so, int nk, const char *s) { int lo = 0, hi = nk; while (lo < hi) { int m = (lo + hi) / 2; if (strcmp(so[m].nm, s) < 0) lo = m + 1; else hi = m; } return lo; }
void prolog_inject_prelude(PlProgram *prog, const char *user_src) {
    if (!prog || !user_src) return;
    cv_t user_defined = { 0 }, referenced = { 0 }, refkeys = { 0 }, wanted = { 0 }, attv_off = { 0 };
    for (PlClause *cl = prog->head; cl; cl = cl->next) {
        const char *nm; int ar;
        if (pl_clause_key(cl, &nm, &ar) && nm) pl_cv_add(&user_defined, pl_pred_key(nm, ar));
        if (pl_clause_is_directive(cl) && cl->tr->c[1] && cl->tr->c[1]->t == TT_FNC && cl->tr->c[1]->v.sval && (!strcmp(cl->tr->c[1]->v.sval, "dynamic") || !strcmp(cl->tr->c[1]->v.sval, "discontiguous")))
            pl_tree_collect_declared(cl->tr->c[1], &user_defined);
        if (cl->tr) { pl_tree_collect_calls(cl->tr, &referenced); pl_tree_collect_keys(cl->tr, &refkeys); }
        if (!pl_clause_dcg(cl) && pl_body_has_list_goal(pl_clause_body(cl))) pl_cv_add(&referenced, "consult");
        if (pl_clause_dcg(cl) && pl_dcg_body_var_goal(pl_clause_body(cl))) pl_cv_add(&referenced, "phrase");
    }
    if (pl_cv_has(&referenced, "phrase")) pl_cv_add(&referenced, "$phrase");
    if (pl_cv_has(&referenced, "predicate_property") || pl_cv_has(&referenced, "listing") || pl_word_referenced(user_src, "listing")) pl_cv_add(&referenced, "$pl_pp_pcheck");
    if (pl_cv_has(&referenced, "listing") || pl_word_referenced(user_src, "listing")) pl_cv_add(&refkeys, pl_pred_key("portray_clause", 1));
    if (pl_cv_has(&referenced, "string_concat")) pl_cv_add(&referenced, "$string_concat_split");
    if (pl_cv_has(&referenced, "display")) pl_cv_add(&referenced, "$display_swi");
    if (pl_cv_has(&referenced, "write_canonical")) pl_cv_add(&referenced, "$write_canonical_swi");
    pl_cv_add(&referenced, "$arg_swi");
    pl_cv_add(&referenced, "$conj_nocut");
    pl_cv_add(&referenced, "$retract_target");
    if (pl_cv_has(&referenced, "current_prolog_flag")) pl_cv_add(&referenced, "$flag_member");
    if (pl_cv_has(&referenced, "current_predicate")) pl_cv_add(&referenced, "$cp_mq");
    if (pl_cv_has(&referenced, "erase")) { pl_cv_add(&referenced, "$rec_ref"); pl_cv_add(&referenced, "$rec_erase"); }
    if (pl_cv_has(&referenced, "table")) { pl_cv_add(&referenced, "$tbl_call"); pl_cv_add(&referenced, "$tbl_mcall"); }
    if (pl_cv_has(&referenced, "source_file")) pl_cv_add(&referenced, "$src_file_add");
    if (pl_cv_has(&referenced, "load_files")) {
        pl_cv_add(&referenced, "tfindall");
        pl_cv_add(&referenced, "$tbl_call");
        pl_cv_add(&referenced, "$tbl_mcall");
        pl_cv_add(&referenced, "abolish_all_tables");
        pl_cv_add(&referenced, "current_table");
        pl_cv_add(&referenced, ":");
        pl_cv_add(&referenced, "@");
        pl_cv_add(&referenced, "$lfx_mc");
        pl_cv_add(&referenced, "$lfx_mca");
        pl_cv_add(&referenced, "$lfx_gres");
        pl_cv_add(&referenced, "$lfx_mdb");
        pl_cv_add(&referenced, "abolish_table_pred");
        pl_cv_add(&refkeys, pl_pred_key("abolish_table_pred", 1));
        pl_cv_add(&referenced, "tnot");
        pl_cv_add(&referenced, "get_residual");
        pl_cv_add(&referenced, "call_delays");
        pl_cv_add(&referenced, "undefined");
        pl_cv_add(&referenced, "not_exists");
        pl_cv_add(&referenced, "sk_not");
        pl_cv_add(&referenced, "get_calls");
        pl_cv_add(&referenced, "get_returns");
        pl_cv_add(&referenced, "get_returns_for_call");
        pl_cv_add(&referenced, "$src_file_add");
        pl_cv_add(&referenced, "file_name_extension");
        pl_cv_add(&referenced, "variant");
        pl_cv_add(&refkeys, pl_pred_key("variant", 2));
        pl_cv_add(&referenced, "ord_setproduct");
        pl_cv_add(&refkeys, pl_pred_key("ord_setproduct", 3));
        pl_cv_add(&referenced, "gc_tables");
        pl_cv_add(&refkeys, pl_pred_key("gc_tables", 1));
        pl_cv_add(&referenced, "abolish_table_subgoals");
        pl_cv_add(&refkeys, pl_pred_key("abolish_table_subgoals", 1));
        pl_cv_add(&refkeys, pl_pred_key("abolish_table_subgoals", 2));
        pl_cv_add(&referenced, "abolish_table_call");
        pl_cv_add(&refkeys, pl_pred_key("abolish_table_call", 1));
        pl_cv_add(&refkeys, pl_pred_key("abolish_table_call", 2));
        for (int li = 0; PL_PRELUDE_LIBS[li].lib; li++) {
            if (strcmp(PL_PRELUDE_LIBS[li].lib, "lists") && strcmp(PL_PRELUDE_LIBS[li].lib, "apply") && strcmp(PL_PRELUDE_LIBS[li].lib, "ordsets")) continue;
            for (const pl_prelude_lib_pi_t *le = PL_PRELUDE_LIBS[li].exports; le->nm; le++) { pl_cv_add(&referenced, le->nm); pl_cv_add(&refkeys, pl_pred_key(le->nm, le->ar)); }
        }
    }
    if (pl_cv_has(&referenced, "setup_call_cleanup") || pl_cv_has(&referenced, "setup_call_catcher_cleanup") || pl_cv_has(&referenced, "call_cleanup")) pl_cv_add(&referenced, "$scc_cut");
    if (pl_cv_has(&referenced, "meta_predicate")) pl_cv_add(&referenced, "$mq_arg");
    if (pl_cv_has(&referenced, "clause")) pl_cv_add(&referenced, "$clause_unmangle");
    int mq_hit = 0;
    if (pl_cv_has(&referenced, "module") || pl_cv_has(&referenced, "begin_tests")) for (PlClause *cl = prog->head; cl && !mq_hit; cl = cl->next) mq_hit = cl->tr && pl_tree_var_clause_goal(cl->tr);
    if (pl_cv_has(&referenced, "begin_tests")) { pl_cv_add(&referenced, "$mctx_call"); pl_cv_add(&refkeys, pl_pred_key("$mctx_call", 2)); }
    if (mq_hit) {
        static const struct { const char * nm; int ar; } mq[] = { { "$mctx_assert", 3 }, { "$mctx_asserta", 3 }, { "$mctx_assertz", 3 }, { "$mctx_assert", 4 }, { "$mctx_asserta", 4 }, { "$mctx_assertz", 4 },
            { "$mctx_retract", 3 }, { "$mctx_retractall", 3 }, { "$mctx_clause", 4 }, { "$mctx_clause", 5 }, { 0, 0 } };
        for (int mi = 0; mq[mi].nm; mi++) { pl_cv_add(&referenced, mq[mi].nm); pl_cv_add(&refkeys, pl_pred_key(mq[mi].nm, mq[mi].ar)); }
    }
    if (pl_cv_has(&referenced, "@")) { pl_cv_add(&referenced, "$at_ctx"); pl_cv_add(&referenced, "$meta_add"); }
    if (pl_cv_has(&referenced, "lazy_list")) pl_cv_add(&referenced, "$wakeup");
    if (pl_cv_has(&referenced, "between")) pl_cv_add(&referenced, "$between_test");
    if (pl_cv_has(&referenced, "with_output_to")) { pl_cv_add(&referenced, "$wot_stream_sink"); pl_cv_add(&referenced, "$wot_stream"); }
    {
        int napi = 0;
        while (pl_attv_api[napi]) napi++;
        unsigned char seen[napi + 1];
        int armed = 0;
        memset(seen, 0, (size_t)napi + 1);
        for (PlClause *cl = prog->head; cl; cl = cl->next) if (cl->tr) pl_attv_term_arms(cl->tr, seen);
        for (int i = 0; pl_attv_api[i]; i++) if (seen[i]) armed = 1;
        if (armed) pl_cv_add(&referenced, "$wakeup");
        for (int i = 0; pl_attv_api[i]; i++) if (!seen[i]) { const char *sl = strchr(pl_attv_api[i], '/'); char nm[sl - pl_attv_api[i] + 1]; memcpy(nm, pl_attv_api[i], (size_t)(sl - pl_attv_api[i])); nm[sl - pl_attv_api[i]] = 0; pl_cv_add(&attv_off, ct_strdup(nm)); }
    }
    if (pl_cv_has(&referenced, ":")) { pl_cv_add(&referenced, "$mod_head"); pl_cv_add(&referenced, "$mod_call"); pl_cv_add(&referenced, "$mod_qb"); }
    for (PlClause *cl = prog->head; cl; cl = cl->next) if (cl->tr && pl_tree_has_fn(cl->tr, "$dot")) { pl_cv_add(&referenced, "$dict_eval"); pl_cv_add(&refkeys, pl_pred_key("$dict_eval", 3)); break; }
    if (pl_cv_has(&user_defined, "term_expansion/2") || pl_cv_has(&user_defined, "goal_expansion/2")) pl_cv_add(&referenced, "$load_item");
    if (pl_cv_has(&user_defined, "goal_expansion/2")) pl_cv_add(&referenced, "$meta_add");
    if (pl_word_referenced(user_src, "reset") || pl_word_referenced(user_src, "shift")) pl_cv_add(&referenced, "$co_body");
    if (pl_word_referenced(user_src, "bagof") || pl_word_referenced(user_src, "setof")) { pl_cv_add(&referenced, "$bagof_var"); pl_cv_add(&referenced, "$setof_var"); }
    for (PlClause *cl = prog->head; cl; cl = cl->next) if (pl_clause_ssu(cl)) { pl_cv_add(&referenced, "subsumes_term"); break; }
    PlProgram *pre = pl_prelude_parse(PL_PRELUDE_SRC, -1, "", 0);
    if (!pre || !pre->head) { if (pre) ct_drop(pre); return; }
    int agg_native = 0;
    for (PlClause *cl = prog->head; cl; cl = cl->next) agg_native += pl_agg_native_calls(cl->tr);
    const char *native_only = (agg_native && agg_native == pl_word_count(user_src, "aggregate_all")) ? "aggregate_all" : NULL;
    {
        cv_t imported = { 0 };
        pl_collect_imported_libs(prog, &imported);
        pl_libs_append(pre, &referenced, &refkeys, native_only, &user_defined, &imported);
    }
    {
        int exc = pl_cv_has(&user_defined, "exception/3");
        for (PlClause *cl = pre->head; cl && !exc; cl = cl->next) { const char *nm; int ar; if (pl_clause_key(cl, &nm, &ar) && nm && ar == 3 && !strcmp(nm, "exception")) exc = 1; }
        if (exc) { pl_cv_add(&referenced, "$nb_getval_hooked"); pl_cv_add(&referenced, "$b_getval_hooked"); }
    }
    for (PlClause *cl = pre->head; cl; cl = cl->next) {
        const char *nm; int ar;
        if (!pl_clause_key(cl, &nm, &ar) || !nm) continue;
        if (native_only && ar == 3 && !strcmp(nm, native_only)) continue;
        if (pl_cv_has(&attv_off, nm) && !pl_cv_has(&referenced, "$wakeup")) continue;
        if (!strcmp(nm, "/") && !pl_cv_has(&referenced, nm) && !strstr(user_src, "}/")) continue;
        if (!pl_cv_has(&referenced, nm) && (nm[0] == '$' || !pl_word_referenced(user_src, nm))) continue;
        char *key = pl_pred_key(nm, ar);
        if (!pl_cv_has(&user_defined, key)) pl_cv_add(&wanted, key);
    }
    int n = 0, nk = 0, ng = 0, changed = 1;
    for (PlClause *cl = pre->head; cl; cl = cl->next) n++;
    PlClause *pc[n];
    const char *pn[n];
    int pa[n], gid[n], root[n];
    pl_ckey_t so[n];
    cv_t calls[n];
    unsigned char cdone[n], wf[n], uf[n];
    char *gkey[n];
    {
        int i = 0;
        for (PlClause *cl = pre->head; cl; cl = cl->next, i++) {
            pc[i] = cl;
            gid[i] = -1;
            cdone[i] = 0;
            calls[i] = (cv_t){ 0 };
            root[i] = pl_clause_is_directive(cl) || (pl_clause_is_attv_hook(cl) && cl->dq_forced == 3 + 1);
            if (pl_clause_key(cl, &pn[i], &pa[i]) && pn[i]) so[nk++] = (pl_ckey_t){ pn[i], i };
        }
    }
    qsort(so, (size_t)nk, sizeof so[0], pl_ckey_cmp);
    for (int a = 0, b; a < nk; a = b) {
        for (b = a; b < nk && !strcmp(so[b].nm, so[a].nm); b++) {
            for (int k = a; k < b && gid[so[b].i] < 0; k++) if (pa[so[k].i] == pa[so[b].i]) gid[so[b].i] = gid[so[k].i];
            if (gid[so[b].i] >= 0) continue;
            gkey[ng] = pl_pred_key(so[b].nm, pa[so[b].i]);
            wf[ng] = (unsigned char)pl_cv_has(&wanted, gkey[ng]);
            uf[ng] = (unsigned char)pl_cv_has(&user_defined, gkey[ng]);
            gid[so[b].i] = ng++;
        }
    }
    while (changed) {
        changed = 0;
        for (int i = 0; i < n; i++) {
            if (!root[i] && (gid[i] < 0 || !wf[gid[i]])) continue;
            if (!cdone[i]) {
                pl_tree_collect_calls(pl_clause_body(pc[i]), &calls[i]);
                if (pl_cv_has(&calls[i], "call_cleanup") || pl_cv_has(&calls[i], "setup_call_cleanup") || pl_cv_has(&calls[i], "setup_call_catcher_cleanup")) pl_cv_add(&calls[i], "$scc_cut");
                if (pl_cv_has(&calls[i], "string_concat")) pl_cv_add(&calls[i], "$string_concat_split");
                if (pl_cv_has(&calls[i], "between")) pl_cv_add(&calls[i], "$between_test");
                if (pl_cv_has(&calls[i], "erase")) { pl_cv_add(&calls[i], "$rec_ref"); pl_cv_add(&calls[i], "$rec_erase"); }
                if (pl_cv_has(&calls[i], "current_predicate")) pl_cv_add(&calls[i], "$cp_mq");
                if (pl_cv_has(&calls[i], "with_output_to")) { pl_cv_add(&calls[i], "$wot_stream_sink"); pl_cv_add(&calls[i], "$wot_stream"); }
                cdone[i] = 1;
            }
            for (uint32_t ci = 0; ci < calls[i].len; ci++) {
                const char *cn = CV_AT(calls[i], char *, ci);
                for (int j = pl_ckey_lower(so, nk, cn); j < nk && !strcmp(so[j].nm, cn); j++) {
                    int g = gid[so[j].i];
                    if (!wf[g] && !uf[g]) { wf[g] = 1; CV_PUSH(wanted, char *) = gkey[g]; changed = 1; }
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        PlClause *cl = pc[i];
        int keep = (gid[i] >= 0 && wf[gid[i]]) || ((pl_clause_is_directive(cl) || pl_clause_is_attv_hook(cl)) && cl->dq_forced == 3 + 1);
        if (keep) { cl->lineno = 0; cl->next = NULL; if (!prog->head) prog->head = cl; else prog->tail->next = cl; prog->tail = cl; prog->nclauses++; }
    }
    ct_drop(pre);
    (void)pl_prelude_defines("", -1);
    for (uint32_t i = 0; i < wanted.len; i++) if (!pl_cv_has(&g_stage2.pl_prelude_keys, CV_AT(wanted, char *, i))) CV_PUSH(g_stage2.pl_prelude_keys, char *) = CV_AT(wanted, char *, i);
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
        PlProgram *pre = pl_prelude_parse(PL_PRELUDE_SRC, -1, "", 0);
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
    const char *const sfx[] = { "", ".pl", ".pro", ".prolog", ".plt" };
    char *cand = (char *)ct_alloc(dl + sl + sizeof ".prolog"); FILE *f = (FILE *)0; char *src; long n;
    if (!cand) return (char *)0;
    for (int k = 0; k < 10 && !f; k++) {
        size_t o = (k < 5 && spec[0] != '/') ? dl : 0;
        if (o) memcpy(cand, pp->filename, dl);
        memcpy(cand + o, spec, sl); memcpy(cand + o + sl, sfx[k % 5], strlen(sfx[k % 5]) + 1);
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
    if (!g || g->t != TT_FNC || !g->v.sval || !g->c || !g->c[0] || (g->c[0]->t != TT_QLIT && g->c[0]->t != TT_NAME) || !g->c[0]->v.sval) return (PlProgram *)0;
    if (!strcmp(g->v.sval, "use_module") && (g->n == 1 || g->n == 2)) {
        char *probe = (char *)0; char *psrc = pl_include_read(pp, g->c[0]->v.sval, &probe);
        if (!psrc) return (PlProgram *)0;
        ct_drop(psrc); ct_drop(probe);
    } else if (strcmp(g->v.sval, "include") || g->n != 1) return (PlProgram *)0;
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
static const char *pl_module_directive_name(const tree_t *t) {
    const tree_t *g;
    if (!t || t->t != TT_CLAUSE || t->n != 2 || !t->c[0] || t->c[0]->t != TT_NUL) return (const char *)0;
    g = t->c[1];
    if (!g || g->t != TT_FNC || !g->v.sval || strcmp(g->v.sval, "module") || g->n != 2 || !g->c[0] || (g->c[0]->t != TT_QLIT && g->c[0]->t != TT_NAME)) return (const char *)0;
    return g->c[0]->v.sval;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static PlClause *pl_module_restore_clause(const char *mod, int lineno) {
    PlClause *c = ct_zalloc(1, sizeof(PlClause));
    tree_t *cl = ast_node_new(TT_CLAUSE), *g = ast_node_new(TT_FNC), *m = ast_node_new(TT_QLIT);
    m->v.sval = ct_strdup(mod);
    g->v.sval = ct_strdup("$module_restore");
    ast_push(g, m);
    ast_push(cl, ast_node_new(TT_NUL));
    ast_push(cl, g);
    c->lineno = lineno;
    c->tr = cl;
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_clause_is_eof(const tree_t *t) {
    const tree_t *h = (t && t->t == TT_CLAUSE && t->n == 1) ? t->c[0] : t;
    return h && h->t == TT_QLIT && h->v.sval && !strcmp(h->v.sval, "end_of_file");
}
static int pl_if_ctx_at(pl_ifcx_t *x, PlProgram *prog, PlClause *cl) {
    extern int prolog_clause_marks_swi(const PlClause *);
    extern int prolog_dialect_gnu_selected(void);
    x->head = prog->head;
    x->stop = cl;
    if (x->swi < 0) {
        x->swi = 0;
        for (const PlClause *c = prog->head; c && !prolog_dialect_gnu_selected(); c = c->next) if (prolog_clause_marks_swi(c)) { x->swi = 1; break; }
    }
    return 1;
}
static void pl_preprocess_list(PlProgram *prog, int depth) {
    Parser q; PlClause *prev = (PlClause *)0, *cl; const char *cur_mod = "user"; pl_ifcx_t ix;
    if (!prog) return;
    memset(&q, 0, sizeof q); q.filename = prog->filename ? prog->filename : "<stdin>"; q.quiet = prog->quiet; q.incl_depth = depth;
    memset(&ix, 0, sizeof ix); ix.swi = -1;
    cl = prog->head;
    while (cl) {
        PlClause *next = cl->next; int drop = 0, is_include = 0; const tree_t *t = cl->tr;
        prolog_fold_pieces(cl->tr);
        { const char *mn = pl_module_directive_name(t); if (mn) cur_mod = mn; }
        if (t && t->t == TT_CLAUSE && t->n == 2 && t->c[0] && t->c[0]->t == TT_NUL && t->c[1] && pl_if_ctx_at(&ix, prog, cl) && try_handle_if_directive_tree(&q, t->c[1], cl->lineno, &ix)) drop = 1;
        else if (!if_currently_active(&q)) drop = 1;
        else if (pl_clause_is_eof(t)) {
            for (PlClause *r = cl; r; r = r->next) prog->nclauses--;
            if (prev) prev->next = (PlClause *)0; else prog->head = (PlClause *)0;
            break;
        } else {
            PlProgram *sub = pl_include_expand(&q, cl, depth, &is_include);
            if (is_include) {
                drop = 1;
                if (sub && sub->head) {
                    for (PlClause *sc = sub->head; sc; sc = sc->next) if (!sc->file) sc->file = sub->filename;
                    PlClause *mr = pl_module_restore_clause(cur_mod, cl->lineno);
                    sub->tail->next = mr;
                    sub->tail = mr;
                    sub->nclauses++;
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
    p.in_list  = 0;
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
