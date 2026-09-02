#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lower.h"
#include "emit.h"
#include "stage2.h"
#include "../parsers/snobol4/scrip_cc.h"
#include "bb_program.h"
#define PL_BB_TABLE_MAX 256
typedef struct { const char * name; int arity; int bb_idx; } pl_bb_ent_t;
static pl_bb_ent_t * pl_bb_tab = NULL;
static int           pl_bb_n   = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned resolve_pred_hash(const char *s) {
    unsigned h = 5381;
    while (*s) h = h * 33 ^ (unsigned char)*s++;
    return h % STAGE2_PL_PRED_TABLE_SIZE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void resolve_pred_table_insert(Resolve_PredTable *pt, const char *key, tree_t *choice) {
    unsigned h = resolve_pred_hash(key);
    Resolve_PredEntry *e = (Resolve_PredEntry *) malloc(sizeof(Resolve_PredEntry));
    e->key = key; e->choice = choice; e->entry_pc = -1; e->next = pt->buckets[h]; pt->buckets[h] = e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key) {
    for (Resolve_PredEntry *e = pt->buckets[resolve_pred_hash(key)]; e; e = e->next)
        if (strcmp(e->key, key) == 0) return e->choice;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_bb_ent_t * pl_bb_lookup(const char * name, int arity) {
    if (!name || !pl_bb_tab) return NULL;
    for (int i = 0; i < pl_bb_n; i++) if (pl_bb_tab[i].arity == arity && pl_bb_tab[i].name && strcmp(pl_bb_tab[i].name, name) == 0) return &pl_bb_tab[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_bb_ent_t * pl_bb_register(const char * name, int arity, int bb_idx) {
    if (!name) return NULL;
    pl_bb_ent_t * existing = pl_bb_lookup(name, arity);
    if (existing) { existing->bb_idx = bb_idx; return existing; }
    if (pl_bb_n >= PL_BB_TABLE_MAX) return NULL;
    if (!pl_bb_tab) { pl_bb_tab = (pl_bb_ent_t *)calloc(PL_BB_TABLE_MAX, sizeof *pl_bb_tab); if (!pl_bb_tab) return NULL; }
    pl_bb_ent_t * e = &pl_bb_tab[pl_bb_n++];
    e->name = strdup(name); e->arity = arity; e->bb_idx = bb_idx;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_dyn_mark(const char *name, int arity) {
    if (!name) return;
    for (int i = 0; i < g_stage2.pl_dyn_n; i++) if (g_stage2.pl_dyn_name[i] && !strcmp(g_stage2.pl_dyn_name[i], name) && g_stage2.pl_dyn_arity[i] == arity) return;
    if (g_stage2.pl_dyn_n >= 64) return;
    g_stage2.pl_dyn_name[g_stage2.pl_dyn_n] = name; g_stage2.pl_dyn_arity[g_stage2.pl_dyn_n] = arity; g_stage2.pl_dyn_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_dyn_is_marked(const char *name, int arity) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2.pl_dyn_n; i++) if (g_stage2.pl_dyn_name[i] && !strcmp(g_stage2.pl_dyn_name[i], name) && g_stage2.pl_dyn_arity[i] == arity) return 1;
    return 0;
}
typedef struct { IR_graph_t * g; IR_t * tω; } lcx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * build(lcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) { return lc_build(cx->g, op, γ, ω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_refuse(const char * what, const char * detail, int rung) {
    fprintf(stderr, "scrip: prolog: %s%s%s is not on the ladder yet -- rung %d lands it (ARCH-PROLOG-BYRD-BOX-TRANSLATION.md sec E; rung 0 is hello world)\n",
            what, detail ? " " : "", detail ? detail : "", rung);
    exit(2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_var_name(int slot) {
    static char * cache[1024]; static char buf[24];
    if (slot >= 0 && slot < 1024) { if (!cache[slot]) { snprintf(buf, sizeof buf, "G%d", slot); cache[slot] = strdup(buf); } return cache[slot]; }
    snprintf(buf, sizeof buf, "G%d", slot); return strdup(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_pi_name(const char * nm, int ar) {
    static char buf[264];
    if (!nm) return strdup("?");
    snprintf(buf, sizeof buf, "%s/%d", nm, ar); return strdup(buf);
}
static IR_t * term_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out);
static IR_t * term_lval_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * mkc_node(lcx_t * cx, const char * fname, int nkids, IR_t ** kids, IR_t ** kid_entries, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, NULL, cx->tω); IR_LIT(nd).sval = "$mkc";
    IR_t * fn = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(fn).sval = strdup(fname);
    ir_operand_push(nd, fn);
    IR_t * prev = fn; IR_t * first = fn;
    for (int i = 0; i < nkids; i++) {
        IR_t * ke = kid_entries[i] ? kid_entries[i] : kids[i];
        lc_γ_to(prev, ke); if (cx->tω) lc_ω_to(kids[i], cx->tω);
        prev = kids[i];
        ir_operand_push(nd, kids[i]);
    }
    lc_γ_to(prev, nd);
    if (entry_out) *entry_out = first;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (!t) return NULL;
    switch (t->t) {
    case TT_QLIT: { IR_t * nd = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval; return nd; }
    case TT_ILIT: { IR_t * nd = build(cx, IR_LIT_INTEGER, NULL, cx->tω); IR_LIT(nd).ival = t->v.ival; return nd; }
    case TT_FLIT: { IR_t * nd = build(cx, IR_LIT_REAL, NULL, cx->tω); IR_LIT(nd).dval = t->v.dval; return nd; }
    case TT_VAR:  { IR_t * nd = build(cx, IR_VAR, NULL, cx->tω); IR_LIT(nd).sval = pl_var_name((int) t->v.ival); return nd; }
    case TT_MAKELIST: {
        int bar = (t->v.ival == 1 && t->n > 0);
        IR_t * prev; IR_t * prev_e = NULL;
        if (bar) prev = term_lval_e(cx, t->c[t->n - 1], &prev_e);
        else { prev = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(prev).sval = "[]"; }
        for (int i = (bar ? t->n - 2 : t->n - 1); i >= 0; i--) {
            IR_t * ee = NULL; IR_t * e = term_lval_e(cx, t->c[i], &ee);
            IR_t * kids[2] = { e, prev }; IR_t * kes[2] = { ee, prev_e };
            prev = mkc_node(cx, ".", 2, kids, kes, &prev_e);
        }
        if (entry_out) *entry_out = prev_e;
        return prev;
    }
    case TT_FNC: {
        int nk = t->n;
        if (nk == 0) { IR_t * nd = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(nd).sval = t->v.sval ? t->v.sval : "?"; return nd; }
        IR_t ** kids = (IR_t **) calloc((size_t)(nk > 0 ? nk : 1), sizeof(IR_t *));
        IR_t ** kes  = (IR_t **) calloc((size_t)(nk > 0 ? nk : 1), sizeof(IR_t *));
        for (int i = 0; i < nk; i++) kids[i] = term_lval_e(cx, t->c[i], &kes[i]);
        IR_t * nd = mkc_node(cx, t->v.sval ? t->v.sval : "?", nk, kids, kes, entry_out);
        free(kids); free(kes);
        return nd;
    }
    default: { IR_t * nd = build(cx, IR_LIT_STRING, NULL, cx->tω); IR_LIT(nd).sval = "?"; return nd; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * term_lval_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (t && t->t == TT_VAR) { IR_t * nd = build(cx, IR_VAR_REF, NULL, cx->tω); IR_LIT(nd).sval = pl_var_name((int) t->v.ival); return nd; }
    return term_e(cx, t, entry_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_param_name(int i) {
    static char * cache[64]; static char buf[16];
    if (i >= 0 && i < 64) { if (!cache[i]) { snprintf(buf, sizeof buf, "A%d", i); cache[i] = strdup(buf); } return cache[i]; }
    snprintf(buf, sizeof buf, "A%d", i); return strdup(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int max_var_slot(const tree_t * t, int mx) {
    if (!t) return mx;
    if (t->t == TT_VAR && (int) t->v.ival > mx) mx = (int) t->v.ival;
    for (int i = 0; i < t->n; i++) mx = max_var_slot(t->c[i], mx);
    return mx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_same_functor(const tree_t * a, const tree_t * b) {
    return a && b && a->t == TT_FNC && b->t == TT_FNC && a->n == b->n && a->n > 0 && a->v.sval && b->v.sval && !strcmp(a->v.sval, b->v.sval);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * unify_pair(lcx_t * cx, const tree_t * lt, const tree_t * rt, IR_t * γ, IR_t * ω, IR_t ** entry_out) {
    if (pl_same_functor(lt, rt)) {
        IR_t * next = γ; IR_t * first_entry = γ; IR_t * head = NULL;
        for (int i = lt->n - 1; i >= 0; i--) { IR_t * e = NULL; IR_t * u = unify_pair(cx, lt->c[i], rt->c[i], next, ω, &e); next = e ? e : u; head = u; if (i == 0) first_entry = next; }
        if (entry_out) *entry_out = first_entry;
        return head;
    }
    IR_t * nd = build(cx, IR_CALL, γ, ω); IR_LIT(nd).sval = "$unify";
    IR_t * e0 = NULL; IR_t * e1 = NULL;
    IR_t * a0 = term_lval_e(cx, lt, &e0); IR_t * a1 = term_lval_e(cx, rt, &e1);
    lc_γ_to(a0, e1 ? e1 : a1); lc_ω_to(a0, ω);
    lc_γ_to(a1, nd); lc_ω_to(a1, ω);
    ir_operand_push(nd, a0); ir_operand_push(nd, a1);
    if (entry_out) *entry_out = e0 ? e0 : a0;
    return nd;
}
static const char * pl_rung6_builtins[] = { "=..", "==", "@<", "@=<", "@>", "@>=", "\\==", "acyclic_term", "arg", "atom", "atom_chars", "atom_codes",
    "atom_concat", "atom_length", "atom_number", "atom_string", "atomic", "atomic_list_concat", "callable", "char_type", "compound", "concat_atom", "copy_term", "downcase_atom", "float", "format",
    "functor", "ground", "integer", "is_list", "msort", "name", "nonvar", "number", "number_chars", "number_codes", "number_string", "numbervars", "plus", "print", "sort", "string_chars",
    "string_codes", "string_concat", "string_length", "string_lower", "string_to_atom", "string_upper", "succ", "tab", "term_string", "term_to_atom", "upcase_atom", "var", "write_canonical",
    "writeln", "writeq", "put_char", "halt", "flush_output", "read", "read_term", "get_char", "peek_char", "nl", "write", NULL };
static const char * pl_rung7_builtins[] = { "between", "repeat", "clause", "retract", "sub_atom", "for", "current_op", "current_predicate", "predicate_property",
    "current_prolog_flag", "current_stream", "stream_property", NULL };
static const char * pl_rung8_builtins[] = { "findall", "bagof", "setof", "aggregate_all", "forall", NULL };
static const char * pl_rung9_builtins[] = { "catch", "throw", NULL };
static const char * pl_rung10_builtins[] = { "call", "assert", "asserta", "assertz", "retractall", "abolish", "dynamic", "nb_setval", "nb_getval", "b_setval", "b_getval", "phrase",
    "with_output_to", "setup_call_cleanup", "use_module", "ensure_loaded", "module", "set_prolog_flag", NULL };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_name_in(const char * nm, const char * const * lst) { if (!nm) return 0; for (int i = 0; lst[i]; i++) if (!strcmp(nm, lst[i])) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_rung_of(const char * nm) {
    if (pl_name_in(nm, pl_rung10_builtins)) return 10;
    if (pl_name_in(nm, pl_rung9_builtins)) return 9;
    if (pl_name_in(nm, pl_rung8_builtins)) return 8;
    if (pl_name_in(nm, pl_rung7_builtins)) return 7;
    if (pl_name_in(nm, pl_rung6_builtins)) return 6;
    return 0;
}
static IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_cmp_op_suffix(const char * s) {
    if (!s) return NULL;
    if (!strcmp(s, "<")) return "lt"; if (!strcmp(s, ">")) return "gt"; if (!strcmp(s, "=<")) return "le";
    if (!strcmp(s, ">=")) return "ge"; if (!strcmp(s, "=:=")) return "eq"; if (!strcmp(s, "=\\=")) return "ne";
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_ax_suffix(const char * s, int ar) {
    if (!s) return NULL;
    if (ar == 2) {
        if (!strcmp(s, "+")) return "add"; if (!strcmp(s, "-")) return "sub"; if (!strcmp(s, "*")) return "mul";
        if (!strcmp(s, "/")) return "div"; if (!strcmp(s, "//")) return "idiv"; if (!strcmp(s, "div")) return "idiv";
        if (!strcmp(s, "mod")) return "mod"; if (!strcmp(s, "rem")) return "rem"; if (!strcmp(s, "**")) return "fpow"; if (!strcmp(s, "^")) return "pow";
        if (!strcmp(s, "min")) return "min"; if (!strcmp(s, "max")) return "max"; if (!strcmp(s, "gcd")) return "gcd"; if (!strcmp(s, "xor")) return "xor";
        if (!strcmp(s, ">>")) return "shr"; if (!strcmp(s, "<<")) return "shl"; if (!strcmp(s, "/\\")) return "band"; if (!strcmp(s, "\\/")) return "bor";
        return NULL;
    }
    if (ar == 1) {
        if (!strcmp(s, "-")) return "neg"; if (!strcmp(s, "+")) return "pos"; if (!strcmp(s, "abs")) return "abs"; if (!strcmp(s, "sign")) return "sign";
        if (!strcmp(s, "truncate")) return "trunc"; if (!strcmp(s, "integer")) return "intg"; if (!strcmp(s, "float")) return "flt";
        if (!strcmp(s, "floor")) return "floor"; if (!strcmp(s, "ceiling")) return "ceil"; if (!strcmp(s, "round")) return "round";
        if (!strcmp(s, "sqrt")) return "sqrt"; if (!strcmp(s, "msb")) return "msb"; if (!strcmp(s, "\\")) return "bnot"; if (!strcmp(s, "sin")) return "sin"; if (!strcmp(s, "cos")) return "cos";
        if (!strcmp(s, "atan")) return "atan"; if (!strcmp(s, "log")) return "log"; if (!strcmp(s, "exp")) return "exp";
        if (!strcmp(s, "float_integer_part")) return "fip"; if (!strcmp(s, "float_fractional_part")) return "ffp";
        return NULL;
    }
    if (ar == 0) { if (!strcmp(s, "pi")) return "pi"; return NULL; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lower_arith_val(lcx_t * cx, const tree_t * t, IR_t * ωfail, IR_t ** entry_out) {
    if (t && (t->t == TT_QLIT || t->t == TT_NAME) && t->v.sval && pl_ax_suffix(t->v.sval, 0)) {
        char nb[24]; snprintf(nb, sizeof nb, "$ax_%s", pl_ax_suffix(t->v.sval, 0));
        IR_t * nd = build(cx, IR_CALL, NULL, ωfail); IR_LIT(nd).sval = strdup(nb);
        if (entry_out) *entry_out = nd;
        return nd;
    }
    if (t && t->t == TT_FNC && t->v.sval && (t->n == 1 || t->n == 2) && pl_ax_suffix(t->v.sval, t->n)) {
        char nb[24]; snprintf(nb, sizeof nb, "$ax_%s", pl_ax_suffix(t->v.sval, t->n));
        IR_t * nd = build(cx, IR_CALL, NULL, ωfail); IR_LIT(nd).sval = strdup(nb);
        IR_t * prev = NULL; IR_t * first = NULL;
        for (int i = 0; i < t->n; i++) {
            IR_t * ke = NULL; IR_t * k = lower_arith_val(cx, t->c[i], ωfail, &ke); IR_t * en = ke ? ke : k;
            if (prev) lc_γ_to(prev, en); else first = en;
            lc_ω_to(k, ωfail);
            prev = k; ir_operand_push(nd, k);
        }
        if (prev) lc_γ_to(prev, nd);
        if (entry_out) *entry_out = first ? first : nd;
        return nd;
    }
    return term_e(cx, t, entry_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_conj(const tree_t * t, lc_vec * out) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, ",")) { for (int i = 0; i < t->n; i++) collect_conj(t->c[i], out); return; }
    if (t->t == TT_PROGRAM) { for (int i = 0; i < t->n; i++) collect_conj(t->c[i], out); return; }
    lc_vec_push(out, &t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_lower_conj(lcx_t * cx, const tree_t * const * gl, int ng, IR_t * γtail, IR_t * ωbase, IR_t ** entry_out, IR_t ** redo_out) {
    IR_t ** gn = (IR_t **) calloc((size_t)(ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t ** en = (IR_t **) calloc((size_t)(ng > 0 ? ng : 1), sizeof(IR_t *));
    IR_t * next = γtail;
    for (int i = ng - 1; i >= 0; i--) {
        IR_t * e = NULL;
        IR_t * nd = goal(cx, gl[i], next, ωbase, &e);
        gn[i] = nd; en[i] = e ? e : nd;
        next = en[i];
    }
    IR_t * last_res = ωbase; int last_res_beta = 0;
    for (int i = 0; i < ng; i++) {
        if (gn[i] && gn[i]->op == IR_GOTO) { if (last_res_beta) lc_γ_to_β(gn[i], last_res); else lc_γ_to(gn[i], last_res); }
        else if (last_res_beta) lc_ω_to_β(gn[i], last_res);
        else lc_ω_to(gn[i], last_res);
        if (gn[i] && gn[i]->op == IR_CALL_PROC_STAGED) { last_res = gn[i]; last_res_beta = 1; }
    }
    if (redo_out) *redo_out = last_res_beta ? last_res : NULL;
    if (entry_out) *entry_out = (ng > 0) ? en[0] : γtail;
    IR_t * first = (ng > 0) ? gn[0] : NULL;
    free(gn); free(en);
    return first;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_leaf(lcx_t * cx, const char * sym, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = sym;
    IR_t * prev = NULL; IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL; IR_t * a = term_e(cx, t->c[i], &ae); IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en); else first = en;
        lc_ω_to(a, ωfail);
        prev = a; ir_operand_push(nd, a);
    }
    if (prev) lc_γ_to(prev, nd);
    if (entry_out) *entry_out = first ? first : nd;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char * nm; int ar; const char * sym; } pl_det_leaf_t;
static const pl_det_leaf_t pl_det_leaves[] = {
    { "var", 1, "$var" }, { "nonvar", 1, "$nonvar" }, { "atom", 1, "$atom" }, { "number", 1, "$number" }, { "integer", 1, "$integer" }, { "float", 1, "$float" }, { "atomic", 1, "$atomic" },
    { "compound", 1, "$compound" }, { "callable", 1, "$callable" }, { "ground", 1, "$ground" }, { "is_list", 1, "$is_list" }, { "acyclic_term", 1, "$acyclic_term" }, { "==", 2, "$atop_eq" },
    { "\\==", 2, "$atop_ne" }, { "@<", 2, "$atop_lt" }, { "@=<", 2, "$atop_le" }, { "@>", 2, "$atop_gt" }, { "@>=", 2, "$atop_ge" }, { "compare", 3, "$compare" }, { "functor", 3, "$functor" },
    { "arg", 3, "$arg" }, { "=..", 2, "$univ" }, { "copy_term", 2, "$copy_term" }, { "numbervars", 3, "$numbervars3" }, { "numbervars", 1, "$numbervars1" }, { "succ", 2, "$succ" },
    { "plus", 3, "$plus" }, { "sort", 2, "$sort" }, { "msort", 2, "$msort" }, { "char_type", 2, "$char_type" }, { "term_string", 2, "$term_string" }, { "term_to_atom", 2, "$term_string" },
    { "atom_length", 2, "$atom_length" }, { "atom_concat", 3, "$atom_concat" }, { "atom_chars", 2, "$atom_chars" }, { "atom_codes", 2, "$atom_codes" }, { "atom_number", 2, "$atom_number" },
    { "atom_string", 2, "$atom_string" }, { "upcase_atom", 2, "$upcase_atom" }, { "downcase_atom", 2, "$downcase_atom" }, { "string_concat", 3, "$string_concat" },
    { "string_length", 2, "$string_length" }, { "string_lower", 2, "$string_lower" }, { "string_upper", 2, "$string_upper" }, { "string_to_atom", 2, "$string_to_atom" },
    { "number_string", 2, "$number_string" }, { "string_chars", 2, "$atom_chars" }, { "string_codes", 2, "$atom_codes" }, { "atomic_list_concat", 2, "$atomic_list_concat" },
    { "atomic_list_concat", 3, "$atomic_list_concat" }, { "concat_atom", 2, "$concat_atom" }, { "concat_atom", 3, "$concat_atom" }, { "char_code", 2, "$char_code" },
    { "number_codes", 2, "$number_codes" }, { "number_chars", 2, "$number_chars" }, { "name", 2, "$name" }, { "get_char", 1, "$get_char" }, { "peek_char", 1, "$peek_char" },
    { "read", 1, "$read" },
    { "atom_to_term", 3, "$atom_to_term" }, { "writeq", 1, "$writeq" }, { "print", 1, "$writeq" }, { "write_canonical", 1, "$write_canonical" }, { "writeln", 1, "$writeln" },
    { "put_char", 1, "$put_char" },
    { "halt", 0, "$halt" }, { "halt", 1, "$halt" }, { "flush_output", 0, "$flush_output" }, { "format", 1, "$format" }, { "format", 2, "$format" },
    { 0, 0, 0 } };
static const struct { const char * nm; int ar; } pl_stream_arity[] = {
    { "write", 2 }, { "writeq", 2 }, { "print", 2 }, { "write_canonical", 2 }, { "nl", 1 }, { "format", 3 },
    { "read", 2 }, { "get_char", 2 }, { "peek_char", 2 }, { "put_char", 2 }, { "tab", 2 }, { "flush_output", 1 }, { 0, 0 } };
static const char * pl_det_leaf_sym(const char * nm, int ar) {
    for (int i = 0; pl_det_leaves[i].nm; i++) if (pl_det_leaves[i].ar == ar && !strcmp(nm, pl_det_leaves[i].nm)) return pl_det_leaves[i].sym;
    return NULL;
}
static int pl_stream_arity_of(const char * nm, int ar) { for (int i = 0; pl_stream_arity[i].nm; i++) if (pl_stream_arity[i].ar == ar && !strcmp(nm, pl_stream_arity[i].nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_leaf_lv(lcx_t * cx, const char * sym, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = sym;
    IR_t * prev = NULL; IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL; IR_t * a = term_lval_e(cx, t->c[i], &ae); IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en); else first = en;
        lc_ω_to(a, ωfail);
        prev = a; ir_operand_push(nd, a);
    }
    if (prev) lc_γ_to(prev, nd);
    if (entry_out) *entry_out = first ? first : nd;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_user_call(lcx_t * cx, const char * nm, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    { char key[264]; snprintf(key, sizeof key, "%s/%d", nm, nargs);
      if (!resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key)) pl_refuse("existence error for unknown procedure", key, 9); }
    IR_t * nd = build(cx, IR_CALL_PROC_STAGED, γnext, ωfail); IR_LIT(nd).sval = pl_pi_name(nm, nargs);
    IR_t * prev = NULL; IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL; IR_t * a = term_lval_e(cx, t->c[i], &ae); IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en); else first = en;
        lc_ω_to(a, ωfail);
        prev = a; ir_operand_push(nd, a);
    }
    if (prev) lc_γ_to(prev, nd);
    if (entry_out) *entry_out = first ? first : nd;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (!t) return build(cx, IR_SUCCEED, γnext, ωfail);
    switch (t->t) {
    case TT_FNC: {
        const char * nm = t->v.sval ? t->v.sval : "?";
        if (!strcmp(nm, ",")) {
            lc_vec glv; lc_vec_init(&glv, (int) sizeof(const tree_t *));
            collect_conj(t, &glv);
            return pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, γnext, ωfail, entry_out, NULL);
        }
        if (!strcmp(nm, "write") && t->n == 1) return pl_leaf(cx, "$write", t, 1, γnext, ωfail, entry_out);
        if (!strcmp(nm, ";") || !strcmp(nm, "|")) { int ite = (t->n == 2 && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "->"));
            pl_refuse(ite ? "if-then-else" : "disjunction", ite ? "->" : ";", ite ? 5 : 3); }
        if (!strcmp(nm, "->")) pl_refuse("if-then", "->", 5);
        if (!strcmp(nm, "\\+") || !strcmp(nm, "not") || !strcmp(nm, "once") || !strcmp(nm, "ignore")) pl_refuse("control construct", nm, 5);
        if (!strcmp(nm, "=") && t->n == 2) { IR_t * e = NULL; IR_t * nd = unify_pair(cx, t->c[0], t->c[1], γnext, ωfail, &e); if (entry_out) *entry_out = e ? e : nd; return nd; }
        if (!strcmp(nm, "\\=")) pl_refuse("control construct", nm, 5);
        if (!strcmp(nm, "is") && t->n == 2) {
            IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = "$is_v";
            IR_t * xe = NULL; IR_t * xl = term_lval_e(cx, t->c[0], &xe);
            IR_t * ve = NULL; IR_t * v = lower_arith_val(cx, t->c[1], ωfail, &ve);
            lc_γ_to(xl, ve ? ve : v); lc_ω_to(xl, ωfail); lc_γ_to(v, nd); lc_ω_to(v, ωfail);
            ir_operand_push(nd, xl); ir_operand_push(nd, v);
            if (entry_out) *entry_out = xe ? xe : xl;
            return nd;
        }
        { const char * csuf = (t->n == 2) ? pl_cmp_op_suffix(nm) : NULL;
          if (csuf) {
            char nb[16]; snprintf(nb, sizeof nb, "$cmp_%s", csuf);
            IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = strdup(nb);
            IR_t * ea = NULL; IR_t * eb = NULL;
            IR_t * a = lower_arith_val(cx, t->c[0], ωfail, &ea); IR_t * b = lower_arith_val(cx, t->c[1], ωfail, &eb);
            lc_γ_to(a, eb ? eb : b); lc_ω_to(a, ωfail); lc_γ_to(b, nd); lc_ω_to(b, ωfail);
            ir_operand_push(nd, a); ir_operand_push(nd, b);
            if (entry_out) *entry_out = ea ? ea : a;
            return nd;
          } }
        if (!strcmp(nm, "tab") && t->n == 1) {
            IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = "$tab";
            IR_t * ve = NULL; IR_t * v = lower_arith_val(cx, t->c[0], ωfail, &ve);
            lc_γ_to(v, nd); lc_ω_to(v, ωfail); ir_operand_push(nd, v);
            if (entry_out) *entry_out = ve ? ve : v;
            return nd;
        }
        if (pl_stream_arity_of(nm, t->n)) pl_refuse("stream builtin", nm, 9);
        { const char * ls = pl_det_leaf_sym(nm, t->n); if (ls) return pl_leaf_lv(cx, ls, t, t->n, γnext, ωfail, entry_out); }
        { int r = pl_rung_of(nm); if (r) pl_refuse(r == 6 ? "builtin arity not wired" : "builtin", nm, r); }
        return pl_user_call(cx, nm, t, t->n, γnext, ωfail, entry_out);
    }
    case TT_QLIT: case TT_NAME: {
        const char * nm = t->v.sval ? t->v.sval : "?";
        if (!strcmp(nm, "true")) return build(cx, IR_SUCCEED, γnext, ωfail);
        if (!strcmp(nm, "fail") || !strcmp(nm, "false")) return build(cx, IR_GOTO, ωfail, ωfail);
        if (!strcmp(nm, "nl")) return pl_leaf(cx, "$nl", t, 0, γnext, ωfail, entry_out);
        if (!strcmp(nm, "!")) pl_refuse("cut", "!", 4);
        { const char * ls = pl_det_leaf_sym(nm, 0); if (ls) return pl_leaf_lv(cx, ls, t, 0, γnext, ωfail, entry_out); }
        { int r = pl_rung_of(nm); if (r) pl_refuse(r == 6 ? "builtin arity not wired" : "builtin", nm, r); }
        return pl_user_call(cx, nm, t, 0, γnext, ωfail, entry_out);
    }
    case TT_CUT: pl_refuse("cut", "!", 4); return NULL;
    case TT_UNIFY: { IR_t * e = NULL; IR_t * nd = unify_pair(cx, t->c[0], t->c[1], γnext, ωfail, &e); if (entry_out) *entry_out = e ? e : nd; return nd; }
    case TT_IF: pl_refuse("if-then-else", "->", 5); return NULL;
    case TT_PROGRAM: return pl_lower_conj(cx, (const tree_t * const *) t->c, t->n, γnext, ωfail, entry_out, NULL);
    case TT_VAR: pl_refuse("variable goal", "call/1", 10); return NULL;
    default: return build(cx, IR_SUCCEED, γnext, ωfail);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_new_proc(const char * name, int nparams, int bb_idx) {
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name         = strdup(name);
    g_stage2.proc_table[pi].proc         = NULL;
    g_stage2.proc_table[pi].entry_pc     = -1;
    g_stage2.proc_table[pi].bb_idx       = bb_idx;
    g_stage2.proc_table[pi].nparams      = nparams;
    g_stage2.proc_table[pi].is_generator = 1;
    { int mi = g_stage2.module_registry.nmod - 1; if (mi >= 0) { g_stage2.module_registry.mods[mi].nprocs++;
        if (!strcmp(name, "main") && g_stage2.module_registry.main_mod < 0) g_stage2.module_registry.main_mod = mi; } }
    return pi;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_graph_stamp(IR_graph_t * g, int arity, int maxlocal) {
    g->body_root = NULL;
    g->nparams = arity;
    if (arity > 0) { g->pnames = (const char **) calloc((size_t) arity, sizeof(const char *)); for (int i = 0; i < arity; i++) g->pnames[i] = pl_param_name(i); }
    if (maxlocal >= 0) { g->nlocals = maxlocal + 1;
        g->lnames = (const char **) calloc((size_t)(maxlocal + 1), sizeof(const char *));
        for (int k = 0; k <= maxlocal; k++) g->lnames[k] = pl_var_name(k); }
    g->nslots = arity + (maxlocal + 1) + 8;
    g->zframe_graph = 1;
    g->zframe_pinned_base = 1;
    g->deterministic = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_alt_alloc(IR_graph_t * g, int nc) {
    g->alt_entry = (IR_t **) calloc((size_t) nc, sizeof(IR_t *));
    g->alt_ret   = (IR_t **) calloc((size_t) nc, sizeof(IR_t *));
    g->alt_redo  = (IR_t **) calloc((size_t) nc, sizeof(IR_t *));
    g->n_alts    = nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * pl_body_graph(const tree_t * const * gl, int ng) {
    IR_graph_t * g = IR_alloc(4096);
    lcx_t cx; cx.g = g; cx.tω = NULL;
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * step    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * entry = NULL; IR_t * redo = NULL;
    IR_t * first = pl_lower_conj(&cx, gl, ng, succeed, step, &entry, &redo);
    int maxlocal = -1; for (int i = 0; i < ng; i++) maxlocal = max_var_slot(gl[i], maxlocal);
    g->entry = entry ? entry : (first ? first : succeed);
    pl_alt_alloc(g, 1);
    g->alt_entry[0] = g->entry; g->alt_ret[0] = succeed; g->alt_redo[0] = redo; g->alt_fail = step;
    pl_graph_stamp(g, 0, maxlocal);
    (void) fail;
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t * pl_pred_graph(const tree_t * ch, const char * key) {
    int nc = (ch->t == TT_CHOICE) ? ch->n : 1;
    if (nc < 1) nc = 1;
    IR_graph_t * g = IR_alloc(1024 + 1024 * nc);
    lcx_t cx; cx.g = g; cx.tω = NULL;
    IR_t * step = build(&cx, IR_FAIL, NULL, NULL);
    pl_alt_alloc(g, nc);
    g->alt_fail = step;
    int arity = -1; int maxlocal = -1;
    for (int k = 0; k < nc; k++) {
        const tree_t * cl = (ch->t == TT_CHOICE) ? ch->c[k] : ch;
        if (!cl || cl->t != TT_CLAUSE) pl_refuse("clause shape in", key, 2);
        int ar = (int) cl->v.dval; if (ar < 0) ar = 0; if (ar > cl->n) ar = cl->n;
        if (arity < 0) arity = ar;
        if (ar != arity) pl_refuse("clauses of differing arity in", key, 2);
        IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
        IR_t * bentry = NULL; IR_t * redo = NULL;
        IR_t * first = pl_lower_conj(&cx, (const tree_t * const *)(cl->c + ar), cl->n - ar, succeed, step, &bentry, &redo);
        IR_t * next = bentry ? bentry : (first ? first : succeed);
        for (int i = ar - 1; i >= 0; i--) {
            IR_t * u = build(&cx, IR_CALL, next, step); IR_LIT(u).sval = "$unify";
            IR_t * lhs = build(&cx, IR_VAR_REF, NULL, NULL); IR_LIT(lhs).sval = pl_param_name(i);
            IR_t * he = NULL; IR_t * rhs = term_lval_e(&cx, cl->c[i], &he);
            lc_γ_to(lhs, he ? he : rhs); lc_ω_to(lhs, step);
            lc_γ_to(rhs, u); lc_ω_to(rhs, step);
            ir_operand_push(u, lhs); ir_operand_push(u, rhs);
            next = lhs;
        }
        g->alt_entry[k] = next; g->alt_ret[k] = succeed; g->alt_redo[k] = redo;
        { int ml = max_var_slot(cl, -1); if (ml > maxlocal) maxlocal = ml; }
    }
    if (arity < 0) arity = 0;
    g->entry = g->alt_entry[0];
    pl_graph_stamp(g, arity, maxlocal);
    g->deterministic = (nc == 1);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lower_pl_pred_graph(const char * key, const tree_t * ch) {
    if (ch->t == TT_CHOICE) { if (ch->n < 1) return -1; if (!ch->c[0] || ch->c[0]->t != TT_CLAUSE) return -1; }
    else if (ch->t != TT_CLAUSE) return -1;
    { IR_graph_t * g = pl_pred_graph(ch, key);
      return bb_program_add(&g_stage2.bbp, g); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_register_program(stage2_t * s2, const tree_t * prog) {
    extern int polyglot_module_open(stage2_t * s2, const tree_t * s);
    extern void polyglot_module_extend(stage2_t * s2, int mod_idx, const tree_t * s);
    int mod_idx = -1;
    for (int _ci = 0; _ci < prog->n; _ci++) {
        const tree_t * s = prog->c[_ci];
        if (!s || (s->t != TT_STMT && s->t != TT_END)) continue;
        if (mod_idx < 0) mod_idx = polyglot_module_open(s2, s);
        polyglot_module_extend(s2, mod_idx, s);
        tree_t * sub = lp_s_expr(s, ":subj");
        if (!sub) continue;
        if ((sub->t == TT_CHOICE || sub->t == TT_CLAUSE) && sub->v.sval) {
            resolve_pred_table_insert(&s2->resolve_pred_table, sub->v.sval, sub);
            if (strcmp(sub->v.sval, "main/0") == 0 && s2->module_registry.main_mod < 0) s2->module_registry.main_mod = mod_idx;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_pl_stage2(const tree_t *prog) {
    int _pl_bb0 = g_stage2.bbp.count;
    pl_register_program(&g_stage2, prog);
    enum { PL_INIT_GOALS_MAX = 256 };
    const tree_t * init_goals[PL_INIT_GOALS_MAX]; int ninit = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t *s = prog->c[i];
        if (!s || s->t != TT_STMT) continue;
        const tree_t *subj = lp_s_expr(s, ":subj");
        if (!subj || subj->t == TT_CHOICE || subj->t == TT_CLAUSE) continue;
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "initialization") && subj->n >= 1) {
            const tree_t *gt = subj->c[0];
            if (gt && ((gt->t == TT_QLIT || gt->t == TT_NAME || gt->t == TT_FNC) && gt->v.sval)) { if (ninit < PL_INIT_GOALS_MAX) init_goals[ninit++] = gt; continue; }
        }
        pl_refuse("directive", (subj->v.sval ? subj->v.sval : "?"), 10);
    }
    tree_t * main_goal = NULL;
    if (ninit == 0 && resolve_pred_table_lookup(&g_stage2.resolve_pred_table, "main/0")) { main_goal = ast_node_new(TT_QLIT); main_goal->v.sval = (char *) "main"; init_goals[ninit++] = main_goal; }
    IR_graph_t * top = pl_body_graph(init_goals, ninit);
    top->root_graph = 1;
    int top_idx = bb_program_add(&g_stage2.bbp, top);
    pl_new_proc("main", 0, top_idx);
    for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++) {
        for (Resolve_PredEntry *pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
            if (!pe->key || !pe->choice) continue;
            const char *slash = strrchr(pe->key, '/');
            int ar = slash ? atoi(slash + 1) : 0;
            if (pl_bb_lookup(pe->key, ar)) continue;
            int bb_idx = lower_pl_pred_graph(pe->key, pe->choice);
            if (bb_idx < 0) continue;
            pl_bb_register(pe->key, ar, bb_idx);
            pl_new_proc(pe->key, ar, bb_idx);
        }
    }
    for (int _gi = _pl_bb0; _gi < g_stage2.bbp.count; _gi++) if (g_stage2.bbp.table[_gi]) { g_stage2.bbp.table[_gi]->zframe_graph = 1; g_stage2.bbp.table[_gi]->zframe_pinned_base = 1; }
    return &g_stage2;
}
