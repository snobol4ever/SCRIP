#include "lower/lower_pascal_tree/lpt_head.h"
#include "lower/g_lower_pas.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include "ir/ast.h"
#include "lower/lower_pascal_tree.h"
#include "lower/lower_pascal_class.h"
#include "lower/lower_pascal_tree/lpt_01_pas.h"
#include "lower/lower_pascal_tree/lpt_02_pas.h"
#include "lower/lower_pascal_tree/lpt_03_pas.h"
#include "lower/lower_pascal_tree/lpt_04_sh.h"
#include "lower/lower_pascal_tree/lpt_05_pas.h"
#include "lower/lower_pascal_tree/lpt_06_pas.h"
#include "lower/lower_pascal_tree/lpt_07_sh.h"
#include "lower/lower_pascal_tree/lpt_11_sh.h"
#include "lower/lower_pascal_tree/lpt_12_pas.h"
static pval E_decl_part(const tree_t *n);
static pval E_body(const tree_t *n);
static pval E_body(const tree_t *n);
static pval E_const_decl_list(const tree_t *n, int from, int to);
static pval E_decl_part(const tree_t *n);
static pval E_goto_statement(const tree_t *n);
static pval E_if_statement(const tree_t *n);
static pval E_label_list(const tree_t *n, int from, int to);
static pval E_parameter_decl_list(const tree_t *n, int from, int to);
static pval E_pf_section(const tree_t *n);
static pval E_pf_sections(const tree_t *n, int from, int to);
static pval E_repeat_statement(const tree_t *n);
static pval E_statement(const tree_t *n);
static pval E_statement_no_label(const tree_t *n);
static pval E_var_decl_list(const tree_t *n, int from, int to);
static pval E_while_statement(const tree_t *n);
static pval E_with_open(const tree_t *n, int from, int to);
static pval E_with_statement(const tree_t *n);
static void pas_const_mark_enum(const char *name) {
    for (int i = g_lower_pas.sem.pas_nconst - 1; name && i >= 0; i--) if (g_lower_pas.sem.pas_consts[i].name && !strcmp(g_lower_pas.sem.pas_consts[i].name, name)) {
        g_lower_pas.sem.pas_consts[i].isenum = 1;
        return;
    }
}
int pas_const_isenum(const char *name) {
    for (int i = g_lower_pas.sem.pas_nconst - 1; name && i >= 0; i--) if (g_lower_pas.sem.pas_consts[i].name && !strcmp(g_lower_pas.sem.pas_consts[i].name, name)) return g_lower_pas.sem.pas_consts[i]
        .isenum;
    return 0;
}
int pas_const_get(const char *name, long long *out) {
    if (!name) return 0;
    int u = pas_scope_uid(name);
    for (int i = 0; i < g_lower_pas.sem.pas_nconst; i++)
        if (g_lower_pas.sem.pas_consts[i].name && !strcmp(g_lower_pas.sem.pas_consts[i].name, name) && (g_lower_pas.sem.pas_consts[i].uid == 0 || g_lower_pas.sem.pas_consts[i].uid == u)) {
        *out = g_lower_pas.sem.pas_consts[i].val;
        return 1;
    }
    return 0;
}
static void pas_scope_applied(const char *name) {
    for (int i = g_lower_pas.sem.pas_scope.n - 1; name && i >= 0; i--) if (g_lower_pas.sem.pas_scope.defs[i].name && !strcmp(g_lower_pas.sem.pas_scope.defs[i].name, name)) {
        if (g_lower_pas.sem.pas_scope.defs[i].depth < g_lower_pas.sem.pas_scope.depth)
            pas_scope_push(&g_lower_pas.sem.pas_scope.apl, &g_lower_pas.sem.pas_scope.napl, &g_lower_pas.sem.pas_scope.capl, name);
        return;
    }
}
void pas_scope_defined_after_use(const char *name) {
    if (g_lower_pas.sem.pas_recbody_depth > 0) return;
    for (int i = g_lower_pas.sem.pas_scope.napl - 1; i >= 0 && g_lower_pas.sem.pas_scope.apl[i].depth == g_lower_pas.sem.pas_scope.depth; i--) if (!strcmp(g_lower_pas.sem.pas_scope.apl[i].name, name))
        {
        fprintf(stderr, "pascal: ISO 7185 6.2.2.9 violation: '%s' is used in this block before its defining-point in this block, which shall precede every applied occurrence\n", name);
        g_lower_pas.sem.pas_iso_errors++;
        return;
    }
}
void pas_scope_require(const char *name) {
    pas_scope_applied(name);
    if (pas_scope_has(name)) return;
    fprintf(stderr, "pascal: ISO 7185 6.2.2.1 violation: the identifier '%s' has no defining-point -- it is declared nowhere in a region enclosing this use\n", name);
    g_lower_pas.sem.pas_iso_errors++;
}
void pas_note_error(void) { g_lower_pas.sem.pas_iso_errors++; }
int pas_is_rel(tree_t *e) { if (!e) return 0; switch (e->t) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: return 1; default: return 0; } }
void pas_not_a_word_symbol(const char *n) {
    const char *ws[] = { "and", "array", "begin", "case", "const", "div", "do", "downto", "else", "end", "file", "for", "function", "goto", "if", "in", "label", "mod", "nil", "not", "of", "or",
        "packed", "procedure", "program", "record", "repeat", "set", "then", "to", "type", "until", "var", "while", "with" };
    for (size_t i = 0; n && i < sizeof ws / sizeof ws[0]; i++) if (!strcmp(n, ws[i])) {
        fprintf(stderr, "pascal: ISO 7185 6.1.2 violation: '%s' is a word-symbol and cannot be defined as an identifier\n", n);
        g_lower_pas.sem.pas_iso_errors++;
        return;
    }
}
static long long pas_label_key(const tree_t *c) {
    if (c && c->t == TT_VAR && c->v.sval) return 100000000LL + (long long)((unsigned long long)pas_hash_stable(c->v.sval) % 1000000000ULL);
    return tok_i(c);
}
static void pas_label_ident_check(const tree_t *c) {
    if (!c || c->t != TT_VAR || !c->v.sval || (g_lower_pas.sem.pas_seen_mode_directive && !g_lower_pas.sem.pas_mode_iso)) return;
    fprintf(stderr, "pascal: ISO 7185 6.1.6 violation: the label '%s' is not a digit-sequence -- an identifier label is fpc's, not ISO's\n", c->v.sval);
    g_lower_pas.sem.pas_iso_errors++;
}
static const char *pas_label_text(const tree_t *c, long long key) {
    if (c && c->t == TT_VAR && c->v.sval) return ct_strdup(c->v.sval);
    char *b = (char *)ct_zalloc(24, 1);
    snprintf(b, 24, "%lld", key);
    return b;
}
static void pas_label_in_range(long long v) {
    if (v < 0 || v > 9999) { fprintf(stderr, "pascal: ISO 7185 6.1.6 violation: the label %lld is not in the closed interval 0 to 9999\n", v); g_lower_pas.sem.pas_iso_errors++; }
}
static void pas_label_declare(long long v) {
    char b[24];
    snprintf(b, sizeof b, "%lld", v);
    pas_scope_push(&g_lower_pas.sem.pas_scope.lab, &g_lower_pas.sem.pas_scope.nlab, &g_lower_pas.sem.pas_scope.clab, ct_strdup(b));
}
static PasDef *pas_label_find(long long v) {
    char b[24];
    snprintf(b, sizeof b, "%lld", v);
    for (int i = g_lower_pas.sem.pas_scope.nlab - 1; i >= 0; i--) if (!strcmp(g_lower_pas.sem.pas_scope.lab[i].name, b)) return &g_lower_pas.sem.pas_scope.lab[i];
    fprintf(stderr, "pascal: ISO 7185 6.2.2.1 violation: the label %lld has no defining-point -- no label-declaration-part of this block or of an enclosing block declares it\n", v);
    g_lower_pas.sem.pas_iso_errors++;
    return NULL;
}
static void pas_label_prefix(long long v) { PasDef *d = pas_label_find(v); if (d && d->depth == g_lower_pas.sem.pas_scope.depth) d->formal++; }
static void pas_labels_close(void) {
    while (g_lower_pas.sem.pas_scope.nlab > 0 && g_lower_pas.sem.pas_scope.lab[g_lower_pas.sem.pas_scope.nlab - 1].depth >= g_lower_pas.sem.pas_scope.depth) {
        PasDef *d = &g_lower_pas.sem.pas_scope.lab[--g_lower_pas.sem.pas_scope.nlab];
        if (d->formal == 1) continue;
        fprintf(stderr, "pascal: ISO 7185 6.2.1 violation: the label %s prefixes %d statements of the block that declares it, which shall closest-contain exactly one\n", d->name, d->formal);
        g_lower_pas.sem.pas_iso_errors++;
    }
}
static void pas_real_is_not_ordinal(double v) {
    if (g_lower_pas.sem.pas_case_depth > 0) fprintf(stderr, "pascal: ISO 7185 6.8.3.5 violation: the case-constant %g is of type real, which is not an ordinal-type\n", v);
    else fprintf(stderr, "pascal: ISO 7185 6.4.2.4 violation: the constant %g is of type real, and a subrange bound or a variant's case-constant (6.4.3.3) shall be of an ordinal-type\n", v);
    g_lower_pas.sem.pas_iso_errors++;
}
static void pas_program_params_distinct(PNodeList *ids) {
    for (int i = 0; ids && i < ids->count; i++) for (int j = 0; j < i; j++)
        if (ids->items[i] && ids->items[j] && ids->items[i]->v.sval && ids->items[j]->v.sval && !strcmp(ids->items[i]->v.sval, ids->items[j]->v.sval)) {
        fprintf(stderr, "pascal: ISO 7185 6.10 violation: the program-parameter '%s' appears more than once in the program-heading\n", ids->items[i]->v.sval);
        g_lower_pas.sem.pas_iso_errors++;
        break;
    }
}
static PNodeList *pas_var_names_add(PNodeList *seen, PNodeList *ids) {
    if (ids) for (int i = 0; i < ids->count; i++) {
        const char *n = ids->items[i] ? ids->items[i]->v.sval : NULL;
        if (!n) continue;
        for (int j = 0; j < seen->count; j++) if (seen->items[j] && seen->items[j]->v.sval && !strcmp(seen->items[j]->v.sval, n)) {
            fprintf(stderr, "pascal: ISO 7185 6.2.2.7 violation: the identifier '%s' has two defining-points in one variable-declaration-part\n", n);
            g_lower_pas.sem.pas_iso_errors++;
            break;
        }
        pnl_push(seen, ids->items[i]);
    }
    return seen;
}
tree_t *pas_for_once(tree_t *e) {
    if (!e || e->t != TT_FOR || e->n < 4 || !e->c[1] || !e->c[2]) return e;
    int kfrom = e->c[1]->t == TT_ILIT || e->c[1]->t == TT_QLIT, kto = e->c[2]->t == TT_ILIT || e->c[2]->t == TT_QLIT;
    if (kto) return e;
    tree_t *q = ast_node_new(TT_SEQ_EXPR);
    if (!kfrom) {
        char _b[24];
        snprintf(_b, sizeof _b, "__pas_fl%d", g_lower_pas.sem.fln++);
        const char *_n = ct_strdup(_b);
        pas_scope_define(_n);
        pas_local_add(_n);
        ast_push(q, mk_assign(leaf_s(TT_VAR, _n), e->c[1]));
        e->c[1] = leaf_s(TT_VAR, _n);
    }
    char _b2[24];
    snprintf(_b2, sizeof _b2, "__pas_fl%d", g_lower_pas.sem.fln++);
    const char *_n2 = ct_strdup(_b2);
    pas_scope_define(_n2);
    pas_local_add(_n2);
    ast_push(q, mk_assign(leaf_s(TT_VAR, _n2), e->c[2]));
    e->c[2] = leaf_s(TT_VAR, _n2);
    ast_push(q, e);
    return q;
}
void tok_events(const tree_t *x) {
    if (!x) return;
    for (int i = 0; i < x->n; i++) {
        tree_t *a = x->c[i];
        if (a && a->t == TT_ATTR && a->v.sval && !strcmp(a->v.sval, ":pragma") && a->slen == 0 && a->n > 0 && a->c[0] && a->c[0]->v.sval) { a->slen = 1; pas_apply_event(a->c[0]->v.sval); }
    }
}
static const char *pas_spelling(const tree_t *x) {
    for (int i = 0; x && i < x->n; i++) if (IS(x->c[i], TT_ATTR) && x->c[i]->v.sval && !strcmp(x->c[i]->v.sval, ":spelling") && x->c[i]->n > 0 && x->c[i]->c[0]->v.sval) return x->c[i]->c[0]->v.sval;
    return x ? x->v.sval : NULL;
}
long long tok_i(const tree_t *x) { tok_events(x); return x->v.ival; }
double tok_d(const tree_t *x) { tok_events(x); return x->v.dval; }
pval E_block(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    v[1].ival = 0;
    for (int i = 0; i < n->n - 1; i++) { v[2] = E_decl_part(n->c[i]); v[1].ival = pas_decl_part_order(v[1].ival, v[2].ival); }
    v[2] = E_body(n->c[n->n - 1]);
    pas_labels_close();
    out.node = v[2].node;
    return out;
}
static pval E_body(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_SEQ_EXPR)) { v[2] = E_statement_list(n, 0, n->n); { out.node = prog_of(v[2].list); } return out; }
    elab_bad("body", n);
    return out;
}
pval E_body_stmt(const tree_t *n) {
    pval v[5];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (n != NULL) { { } v[2] = E_statement(n); { int _ln = n->line; if (v[2].node && !v[2].node->line) v[2].node->line = _ln; out.node = v[2].node; } return out; }
    elab_bad("body_stmt", n);
    return out;
}
static pval E_const_decl_list(const tree_t *n, int from, int to) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) { const tree_t *ce = n->c[i]; if (i == from) { v[1] = E_const_decl(ce); } else { v[1] = out; v[2] = E_const_decl(ce); } }
    return out;
}
static pval E_decl_part(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_PART) && !strcmp(n->v.sval, "label")) { v[2] = E_label_list(n, 0, n->n); { out.ival = 1; } return out; }
    if (IS(n, TT_PART) && !strcmp(n->v.sval, "const")) { v[2] = E_const_decl_list(n, 0, n->n); { out.ival = 2; } return out; }
    if (IS(n, TT_PART) && !strcmp(n->v.sval, "type")) { v[2] = E_type_decl_list(n, 0, n->n); { out.ival = 3; } return out; }
    if (IS(n, TT_PART) && !strcmp(n->v.sval, "var")) { v[2] = E_var_decl_list(n, 0, n->n); { out.ival = 4; } return out; }
    if (IS(n, TT_PROCEDURE) || IS(n, TT_FUNCTION)) { v[1] = E_procedure_decl(n); { out.ival = 5; } return out; }
    elab_bad("decl_part", n);
    return out;
}
pval E_expression_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1] = E_expression(ce);
            { PNodeList *l = pnl_new(); pnl_push(l, v[1].node); out.list = l; }
        } else {
            v[1] = out;
            v[3] = E_expression(ce);
            { pnl_push(v[1].list, v[3].node); out.list = v[1].list; }
        }
    }
    return out;
}
pval E_file_id_list_opt(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (n == NULL) { return out; }
    if (n != NULL) {
        v[2] = E_id_list(n, 0, n->n);
        {
            pas_scope_define_list(v[2].list);
            pas_program_params_distinct(v[2].list);
            if (v[2].list) for (int i = 0; i < v[2].list->count; i++) {
                tree_t *id = v[2].list->items[i];
                if (id && id->v.sval && strcmp(id->v.sval, "input") && strcmp(id->v.sval, "output")) {
                    if (g_lower_pas.sem.pas_nhdrfile >= g_lower_pas.sem.pas_hdrfile_cap) {
                        g_lower_pas.sem.pas_hdrfile_cap = g_lower_pas.sem.pas_hdrfile_cap ? g_lower_pas.sem.pas_hdrfile_cap * 2 : 16;
                        g_lower_pas.sem.pas_hdrfiles = ct_grow(g_lower_pas.sem.pas_hdrfiles, (size_t)g_lower_pas.sem.pas_hdrfile_cap * sizeof *g_lower_pas.sem.pas_hdrfiles);
                    }
                    g_lower_pas.sem.pas_hdrfiles[g_lower_pas.sem.pas_nhdrfile++] = ct_strdup(id->v.sval);
                }
            }
        }
        return out;
    }
    elab_bad("file_id_list_opt", n);
    return out;
}
static pval E_goto_statement(const tree_t *n) {
    pval v[5];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_GOTO_U)) {
        v[2].ival = pas_label_key(n->c[0]);
        { pas_label_find(v[2].ival); tree_t *G = ast_node_new(TT_GOTO_U); G->v.sval = (char *)pas_label_text(n->c[0], v[2].ival); G->line = (n->line); out.node = G; }
        return out;
    }
    elab_bad("goto_statement", n);
    return out;
}
pval E_id_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1].str = tok_s(ce);
            { pas_not_a_word_symbol(v[1].str); PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, v[1].str)); out.list = l; }
        } else {
            v[1] = out;
            v[3].str = tok_s(ce);
            { pas_not_a_word_symbol(v[3].str); pnl_push(v[1].list, leaf_s(TT_VAR, v[3].str)); out.list = v[1].list; }
        }
    }
    return out;
}
static pval E_if_statement(const tree_t *n) {
    pval v[9];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_IF) && n->n == 2) {
        v[2] = E_expression(n->c[0]);
        v[4] = E_body_stmt(n->c[1]);
        { out.node = bin(TT_IF, pas_cond_bool(v[2].node, "an if-statement", "6.8.3.4"), v[4].node); }
        return out;
    }
    if (IS(n, TT_IF)) {
        v[2] = E_expression(n->c[0]);
        v[4] = E_body_stmt(n->c[1]);
        v[6] = E_body_stmt(n->c[2]);
        { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond_bool(v[2].node, "an if-statement", "6.8.3.4")); ast_push(e, v[4].node); ast_push(e, v[6].node); out.node = e; }
        return out;
    }
    elab_bad("if_statement", n);
    return out;
}
static pval E_label_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1].ival = pas_label_key(ce);
            { pas_label_ident_check(ce); if (!IS(ce, TT_VAR)) pas_label_in_range(v[1].ival); pas_label_declare(v[1].ival); }
        } else {
            v[1] = out;
            v[3].ival = pas_label_key(ce);
            { pas_label_ident_check(ce); if (!IS(ce, TT_VAR)) pas_label_in_range(v[3].ival); pas_label_declare(v[3].ival); }
        }
    }
    return out;
}
static pval E_parameter_decl_list(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) { v[1] = E_parameter_decl(ce); { out.list = v[1].list; } } else { v[1] = out; v[3] = E_parameter_decl(ce); { out.list = pnl_concat(v[1].list, v[3].list); } }
    }
    return out;
}
pval E_parameter_list_opt(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_PARAMS)) { v[2] = E_parameter_decl_list(n, 0, n->n); { out.list = v[2].list; } return out; }
    if (n == NULL) { { out.list = pnl_new(); } return out; }
    elab_bad("parameter_list_opt", n);
    return out;
}
pval E_pf_params(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_PARAMS)) { v[2] = E_pf_sections(n, 0, n->n); { out.str = v[2].str; } return out; }
    if (n == NULL) { { out.str = ""; } return out; }
    elab_bad("pf_params", n);
    return out;
}
static pval E_pf_section(const tree_t *n) {
    pval v[8];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    int hasp = n->n > 1 && n->c[1] && n->c[1]->t == TT_PARAMS;
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "value")) { v[1] = E_id_list(n->c[0], 0, n->c[0]->n); v[3].str = tok_s(n->c[1]); { out.str = pas_pf_sect('v', v[1].list->count, v[3].str); } return out; }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "var")) { v[2] = E_id_list(n->c[0], 0, n->c[0]->n); v[4].str = tok_s(n->c[1]); { out.str = pas_pf_sect('r', v[2].list->count, v[4].str); } return out; }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "procedure")) { v[2].str = tok_s(n->c[0]); v[3] = E_pf_params(hasp ? n->c[1] : NULL); { out.str = pas_pf_sig('P', v[3].str, NULL); } return out; }
    if (IS(n, TT_DECL) && !strcmp(n->v.sval, "function")) {
        v[2].str = tok_s(n->c[0]);
        v[3] = E_pf_params(hasp ? n->c[1] : NULL);
        v[5].str = tok_s(n->c[n->n - 1]);
        { out.str = pas_pf_sig('F', v[3].str, v[5].str); }
        return out;
    }
    elab_bad("pf_section", n);
    return out;
}
static pval E_pf_sections(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) { v[1] = E_pf_section(ce); { out.str = v[1].str; } } else { v[1] = out; v[3] = E_pf_section(ce); { out.str = pas_pf_cat3(v[1].str, ";", v[3].str); } }
    }
    return out;
}
static pval E_repeat_statement(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_REPEAT)) {
        v[2] = E_statement_list(n->c[0], 0, n->c[0]->n);
        v[4] = E_expression(n->c[1]);
        { out.node = bin(TT_REPEAT, seq_of(v[2].list), pas_cond_bool(v[4].node, "a repeat-statement", "6.8.3.7")); }
        return out;
    }
    elab_bad("repeat_statement", n);
    return out;
}
pval E_scalar_constant(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_VAR)) {
        v[1].str = tok_s(n);
        {
            if (n->slen != PAS_UNIT_QUAL_MARK) pas_scope_applied(v[1].str);
            long long cv = 0;
            if (v[1].str && !strcmp(v[1].str, "true")) cv = 1;
            else if (v[1].str && !strcmp(v[1].str, "false")) cv = 0;
            else if (!pas_const_get(v[1].str, &cv) && v[1].str && !strcmp(v[1].str, "maxint")) cv = g_lower_pas.sem.pas_int16 ? 32767 : 2147483647;
            g_lower_pas.sem.pas_last_const_enum = pas_const_isenum(v[1].str);
            out.ival = cv;
        }
        return out;
    }
    if (IS(n, TT_ILIT)) { v[1].ival = tok_i(n); { g_lower_pas.sem.pas_last_const_enum = 0; out.ival = v[1].ival; } return out; }
    if (IS(n, TT_FLIT)) { v[1].dval = tok_d(n); { g_lower_pas.sem.pas_last_const_enum = 0; pas_real_is_not_ordinal(v[1].dval); out.ival = (long long)v[1].dval; } return out; }
    if (IS(n, TT_QLIT)) { v[1].str = tok_s(n); { g_lower_pas.sem.pas_last_const_enum = 0; out.ival = (v[1].str && strlen(v[1].str) == 1) ? (long long)(unsigned char)v[1].str[0] : 0; } return out; }
    if (IS(n, TT_CHRLIT)) { v[1].ival = tok_i(n); { g_lower_pas.sem.pas_last_const_enum = 0; out.ival = v[1].ival; } return out; }
    elab_bad("scalar_constant", n);
    return out;
}
pval E_simple_type(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_VAR)) {
        v[1].str = tok_s(n);
        if (v[1].str && !strcmp(v[1].str, "ansichar") && !pas_scope_uid("ansichar")) v[1].str = "char";
        {
            if (n->slen != PAS_UNIT_QUAL_MARK) pas_scope_applied(v[1].str);
            long long _esz;
            if (!pas_sizeof_lookup(v[1].str, &_esz)) _esz = -1;
            g_lower_pas.sem.pas_pend_typename = ct_strdup(v[1].str);
            g_lower_pas.sem.pas_pend_isbool = pas_is_booltype(v[1].str);
            g_lower_pas.sem.pas_pend_istfile = pas_is_tfiletype(v[1].str);
            g_lower_pas.sem.pas_pend_ischar = !strcmp(v[1].str, "char") || !strcmp(v[1].str, "widechar") || pas_typealias_is_char(v[1].str);
            const char *_pt = pas_ptrtype_target(v[1].str);
            if (_pt) {
                g_lower_pas.sem.pas_pend_ptrtarget = ct_strdup(_pt);
                out.ival = -3;
            } else {
                if (!strcmp(v[1].str, "char") || !strcmp(v[1].str, "widechar")) {
                    out.ival = 255;
                } else if (pas_is_settype(v[1].str)) {
                    out.ival = -2;
                } else {
                    long long _eh = pas_enumtype_high(v[1].str);
                    long long _sh = pas_subtype_high(v[1].str);
                    long long _ah = pas_arrtype_high(v[1].str);
                    if (_eh >= 0) {
                        out.ival = _eh;
                    } else if (_sh >= 0) {
                        out.ival = _sh;
                    } else if (_ah >= 0) {
                        if (g_lower_pas.sem.pas_recbody_depth == 0 && pas_rectype_nf(v[1].str) > 0) { pas_rectype_to_pend(v[1].str); g_lower_pas.sem.pas_pend_typename = ct_strdup(v[1].str); }
                        out.ival = _ah;
                    } else {
                        if (g_lower_pas.sem.pas_recbody_depth == 0) pas_rectype_to_pend(v[1].str);
                        g_lower_pas.sem.pas_pend_typename = ct_strdup(v[1].str);
                        out.ival = -1;
                    }
                }
            }
            g_lower_pas.sem.pas_pend_esz = _esz;
        }
        return out;
    }
    if (IS(n, TT_ENUM_TYPE)) {
        v[2] = E_id_list(n, 0, n->n);
        {
            pas_scope_define_list(v[2].list);
            int _eo = 0;
            size_t _en = 1;
            if (v[2].list) for (int i = 0; i < v[2].list->count; i++) { tree_t *_id = v[2].list->items[i]; if (_id && _id->v.sval) _en += strlen(i < n->n ? pas_spelling(n->c[i]) : _id->v.sval) + 1; }
            g_lower_pas.sem.pas_pend_enum_names = ct_zalloc(_en, 1);
            if (v[2].list) for (int i = 0; i < v[2].list->count; i++) {
                tree_t *_id = v[2].list->items[i];
                if (_id && _id->v.sval) {
                    if (_eo > 0) strcat(g_lower_pas.sem.pas_pend_enum_names, ",");
                    strcat(g_lower_pas.sem.pas_pend_enum_names, i < n->n ? pas_spelling(n->c[i]) : _id->v.sval);
                    pas_const_add(_id->v.sval, (long long)(_eo));
                    pas_const_mark_enum(_id->v.sval);
                    _eo++;
                }
            }
            g_lower_pas.sem.pas_pend_enum_max = (long long)(_eo - 1);
            g_lower_pas.sem.pas_pend_esz = -1;
            out.ival = _eo > 0 ? (long long)(_eo - 1) : -1;
        }
        return out;
    }
    if (IS(n, TT_SUBRANGE) && IS(n->c[0], TT_QLIT)) {
        v[1].str = tok_s(n->c[0]);
        v[3] = E_constant(n->c[1]);
        {
            long long _l = (v[1].str && strlen(v[1].str) == 1) ? (long long)(unsigned char)v[1].str[0] : 0;
            g_lower_pas.sem.pas_pend_sub_low = _l;
            g_lower_pas.sem.pas_pend_sub_high = v[3].ival;
            g_lower_pas.sem.pas_pend_ischar = 1;
            g_lower_pas.sem.pas_pend_esz = 1;
            out.ival = v[3].ival;
        }
        return out;
    }
    if (IS(n, TT_SUBRANGE) && IS(n->c[0], TT_CHRLIT)) {
        v[1].ival = tok_i(n->c[0]);
        v[3] = E_constant(n->c[1]);
        { g_lower_pas.sem.pas_pend_sub_low = v[1].ival; g_lower_pas.sem.pas_pend_sub_high = v[3].ival; g_lower_pas.sem.pas_pend_ischar = 1; g_lower_pas.sem.pas_pend_esz = 1; out.ival = v[3].ival; }
        return out;
    }
    if (IS(n, TT_SUBRANGE)) {
        v[1] = E_constant(n->c[0]);
        v[3] = E_constant(n->c[1]);
        {
            g_lower_pas.sem.pas_pend_sub_low = v[1].ival;
            g_lower_pas.sem.pas_pend_sub_high = v[3].ival;
            g_lower_pas.sem.pas_pend_ischar = 0;
            g_lower_pas.sem.pas_pend_esz = (v[3].ival >= v[1].ival) ? pas_range_fit_size(v[1].ival, v[3].ival) : -1;
            out.ival = v[3].ival;
        }
        return out;
    }
    elab_bad("simple_type", n);
    return out;
}
static pval E_statement(const tree_t *n) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_LABEL_DEF)) {
        v[1].ival = pas_label_key(n->c[0]);
        v[3] = E_statement_no_label(n->c[1]);
        {
            if (!IS(n->c[0], TT_VAR)) pas_label_in_range(v[1].ival);
            pas_label_prefix(v[1].ival);
            tree_t *L = ast_node_new(TT_LABEL_DEF);
            L->v.sval = (char *)pas_label_text(n->c[0], v[1].ival);
            ast_push(L, v[3].node);
            out.node = L;
        }
        return out;
    }
    if (n != NULL) { v[1] = E_statement_no_label(n); { out.node = v[1].node; } return out; }
    elab_bad("statement", n);
    return out;
}
pval E_statement_list(const tree_t *n, int from, int to) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            { }
            v[2] = E_statement(ce);
            {
                int _ln = ce->line;
                PNodeList *l = pnl_new();
                if (v[2].node && !v[2].node->line) v[2].node->line = _ln;
                if (v[2].node) { if (pas_trace_enabled() && v[2].node->t != TT_SUCCEED) pnl_push(l, mk_fnc1("__trace_stmt", ilit(_ln))); pnl_push(l, v[2].node); }
                out.list = l;
            }
        } else {
            v[1] = out;
            { }
            v[4] = E_statement(ce);
            {
                int _ln = ce->line;
                if (v[4].node && !v[4].node->line) v[4].node->line = _ln;
                if (v[4].node) { if (pas_trace_enabled() && v[4].node->t != TT_SUCCEED) pnl_push(v[1].list, mk_fnc1("__trace_stmt", ilit(_ln))); pnl_push(v[1].list, v[4].node); }
                out.list = v[1].list;
            }
        }
    }
    return out;
}
static pval E_statement_no_label(const tree_t *n) {
    pval v[4];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_ASSIGN)) { v[1] = E_assignment(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_FNC)) { v[1] = E_call(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_SEQ_EXPR)) { v[1] = E_compound_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_GOTO_U)) { v[1] = E_goto_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_IF)) { v[1] = E_if_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_CASE)) { v[1] = E_case_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_WHILE)) { v[1] = E_while_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_REPEAT)) { v[1] = E_repeat_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_FOR)) { v[1] = E_for_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_WITH)) { v[1] = E_with_statement(n); { out.node = v[1].node; } return out; }
    if (IS(n, TT_SUCCEED)) { { out.node = ast_node_new(TT_SUCCEED); } return out; }
    if (IS(n, TT_DIE)) { out.node = ast_node_new(TT_DIE); out.node->line = n->line; return out; }
    if (IS(n, TT_TRY) && n->n >= 2) {
        tree_t *T = ast_node_new(TT_TRY);
        T->v.sval = n->v.sval;
        T->line = n->line;
        ast_push(T, E_statement_no_label(n->c[0]).node);
        ast_push(T, E_statement_no_label(n->c[1]).node);
        if (n->n > 2 && IS(n->c[2], TT_VAR)) ast_push(T, leaf_s(TT_VAR, n->c[2]->v.sval));
        out.node = T;
        return out;
    }
    elab_bad("statement_no_label", n);
    return out;
}
static pval E_var_decl_list(const tree_t *n, int from, int to) {
    pval v[5];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1] = E_var_decl(ce);
            { out.list = pas_var_names_add(pnl_new(), v[1].list); }
        } else {
            v[1] = out;
            v[2] = E_var_decl(ce);
            { out.list = pas_var_names_add(v[1].list, v[2].list); }
        }
    }
    return out;
}
static pval E_while_statement(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_WHILE)) { v[2] = E_expression(n->c[0]); v[4] = E_body_stmt(n->c[1]); { out.node = bin(TT_WHILE, pas_cond_bool(v[2].node, "a while-statement", "6.8.3.8"), v[4].node); } return out; }
    elab_bad("while_statement", n);
    return out;
}
static pval E_with_open(const tree_t *n, int from, int to) {
    pval v[6];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    for (int i = from; i < to; i++) {
        const tree_t *ce = n->c[i];
        if (i == from) {
            v[1] = E_selector(ce);
            { pas_with_push_once(v[1].node); out.ival = 1; }
        } else {
            v[1] = out;
            v[3] = E_selector(ce);
            { pas_with_push_once(v[3].node); out.ival = v[1].ival + 1; }
        }
    }
    return out;
}
static pval E_with_statement(const tree_t *n) {
    pval v[7];
    pval out;
    memset(v, 0, sizeof v);
    memset(&out, 0, sizeof out);
    if (IS(n, TT_WITH)) { v[2] = E_with_open(n, 0, n->n - 1); v[4] = E_body_stmt(n->c[n->n - 1]); { out.node = pas_with_finish(v[2].ival, v[4].node); } return out; }
    elab_bad("with_statement", n);
    return out;
}
