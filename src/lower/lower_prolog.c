#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lower.h"
#include "emit.h"
#include "stage2.h"
#include "../parsers/snobol4/scrip_cc.h"
#include "bb_program.h"
#include "ir_query.h"
#define PL_BB_TABLE_MAX 256
typedef struct { const char * name; int arity; int bb_idx; } pl_bb_ent_t;
static pl_bb_ent_t * pl_bb_tab = NULL;
static int           pl_bb_n   = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_SEAL_TAIL 1
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
typedef struct { IR_graph_t * g; IR_t * tω; IR_t * cutω; IR_t * clause_cutω; } lcx_t;
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
static const char * pl_rung8_builtins[] = { "findall", "bagof", "setof", "aggregate_all", NULL };
static const char * pl_rung9_builtins[] = { NULL };
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
static int pl_tree_is_nil(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_MAKELIST) return t->n == 0;
    return (t->t == TT_QLIT || t->t == TT_NAME) && t->v.sval && !strcmp(t->v.sval, "[]");
}
static const char * pl_decl_directives[] = { "multifile", "discontiguous", "ensure_loaded", "use_module", "module", "meta_predicate", "dynamic", NULL };
static void pl_decl_dynamic_record(stage2_t * s2, tree_t * spec, tree_t * marker) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_decl_dynamic_record(s2, spec->c[0], marker); pl_decl_dynamic_record(s2, spec->c[1], marker); return; }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_decl_dynamic_record(s2, spec->c[i], marker); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "/") && spec->n == 2 && spec->c[0] && (spec->c[0]->t == TT_QLIT || spec->c[0]->t == TT_NAME)
        && spec->c[0]->v.sval && spec->c[1] && spec->c[1]->t == TT_ILIT) {
        char key[264]; snprintf(key, sizeof key, "%s/%d", spec->c[0]->v.sval, (int) spec->c[1]->v.ival);
        if (!resolve_pred_table_lookup(&s2->resolve_pred_table, key)) resolve_pred_table_insert(&s2->resolve_pred_table, strdup(key), marker);
        return;
    }
    pl_refuse("dynamic declaration shape", spec->v.sval ? spec->v.sval : "?", 10);
}
static int pl_flag_directive_is_default(const tree_t * subj) {
    const tree_t * f = subj->c[0]; const tree_t * v = subj->c[1];
    if (!f || !v || !(f->t == TT_QLIT || f->t == TT_NAME) || !(v->t == TT_QLIT || v->t == TT_NAME) || !f->v.sval || !v->v.sval) return 0;
    if (!strcmp(f->v.sval, "double_quotes")) return !strcmp(v->v.sval, "atom") || !strcmp(v->v.sval, "string");
    return 0;
}
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
static int pl_ax_divides(const char * sfx) {
    if (!sfx) return 0;
    return !strcmp(sfx, "div") || !strcmp(sfx, "idiv") || !strcmp(sfx, "mod") || !strcmp(sfx, "rem");
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
        IR_t * prev = NULL; IR_t * first = NULL; IR_t * dvsr = NULL;
        for (int i = 0; i < t->n; i++) {
            IR_t * ke = NULL; IR_t * k = lower_arith_val(cx, t->c[i], ωfail, &ke); IR_t * en = ke ? ke : k;
            if (prev) lc_γ_to(prev, en); else first = en;
            lc_ω_to(k, ωfail);
            prev = k; ir_operand_push(nd, k);
            if (i == 1) dvsr = k;
        }
        if (dvsr && t->n == 2 && pl_ax_divides(pl_ax_suffix(t->v.sval, 2))) {
            IR_t * zg = build(cx, IR_CALL, nd, ωfail); IR_LIT(zg).sval = "$ax_zguard";
            IR_t * opn = build(cx, IR_LIT_STRING, zg, ωfail); IR_LIT(opn).sval = strdup(t->v.sval);
            ir_operand_push(zg, dvsr); ir_operand_push(zg, opn);
            if (prev) lc_γ_to(prev, opn);
            prev = zg;
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
static IR_t * pl_lower_conj(lcx_t * cx, const tree_t * const * gl, int ng, IR_t * γtail, IR_t * ωbase, IR_t ** entry_out, IR_t ** redo_out, IR_t ** tail_out) {
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
        if (gn[i] && gn[i]->op == IR_CUT) { lc_ω_to(gn[i], cx->cutω); last_res = cx->cutω; last_res_beta = 0; continue; }
        if (gn[i] && gn[i]->op == IR_GOTO) { if (last_res_beta) lc_γ_to_β(gn[i], last_res); else lc_γ_to(gn[i], last_res); }
        else if (last_res_beta) lc_ω_to_β(gn[i], last_res);
        else lc_ω_to(gn[i], last_res);
        if (gn[i] && (gn[i]->op == IR_CALL_PROC_STAGED || gn[i]->op == IR_DISJUNCTION || gn[i]->op == IR_INDIRECT_GOTO || ir_is_generator_kind(gn[i]->op))) { last_res = gn[i]; last_res_beta = 1; }
    }
    if (redo_out) *redo_out = last_res_beta ? last_res : NULL;
    if (entry_out) *entry_out = (ng > 0) ? en[0] : γtail;
    if (tail_out) *tail_out = (ng > 0) ? gn[ng - 1] : NULL;
    IR_t * first = (ng > 0) ? gn[0] : NULL;
    free(gn); free(en);
    return first;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_has_caret(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "^")) return 1;
    for (int i = 0; i < t->n; i++) if (pl_tree_has_caret(t->c[i])) return 1;
    return 0;
}
static int pl_is_ite(const tree_t * t) { return t && t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|")) && t->n == 2 && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "->"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_disj(const tree_t * t) { return t && t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|")) && t->n == 2 && !pl_is_ite(t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void collect_disj(const tree_t * t, lc_vec * out) {
    if (pl_is_disj(t)) { collect_disj(t->c[0], out); collect_disj(t->c[1], out); return; }
    lc_vec_push(out, &t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_disj_entry(lcx_t * cx, IR_t * e, IR_t * dj) {
    int guard = 0;
    while (e && e->op == IR_SUCCEED && e->γ.node && e->γ.node != dj && guard++ < 4096) e = e->γ.node;
    if (e && e->op != IR_SUCCEED) return e;
    { IR_t * g = build(cx, IR_GOTO, dj, dj);
      memcpy(g->γ.sz, "σ", 3); g->γ.sz[3] = 0;
      memcpy(g->ω.sz, "φ", 3); g->ω.sz[3] = 0;
      return g; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_lower_disj(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    lc_vec bv; lc_vec_init(&bv, (int) sizeof(const tree_t *));
    collect_disj(t, &bv);
    const tree_t * const * br = (const tree_t * const *) bv.data;
    int nb = bv.n;
    if (nb > 32) pl_refuse("disjunction wider than 32 branches", ";", 3);
    IR_t * dj = build(cx, IR_DISJUNCTION, γnext, ωfail);
    for (int j = 0; j < nb; j++) {
        int before = cx->g->n;
        lc_vec glv; lc_vec_init(&glv, (int) sizeof(const tree_t *));
        collect_conj(br[j], &glv);
        IR_t * bentry = NULL; IR_t * redo = NULL;
        IR_t * first = pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, dj, dj, &bentry, &redo, NULL);
        for (int k = before; k < cx->g->n; k++) {
            IR_t * x = cx->g->all[k];
            if (!x) continue;
            if (x->γ.node == dj) { if (x->op == IR_GOTO && x->ω.node == dj) memcpy(x->γ.sz, "φ", 3); else memcpy(x->γ.sz, "σ", 3); x->γ.sz[3] = 0; }
            if (x->ω.node == dj) { memcpy(x->ω.sz, "φ", 3); x->ω.sz[3] = 0; }
        }
        int cut_in_branch = 0;
        for (int k = before; k < cx->g->n; k++) if (cx->g->all[k] && cx->g->all[k]->op == IR_CUT) { cut_in_branch = 1; break; }
        ir_operand_push(dj, pl_disj_entry(cx, bentry ? bentry : (first ? first : dj), dj));
        ir_operand_push(dj, cut_in_branch ? cx->cutω : (redo ? redo : dj));
    }
    for (int j = 0; j < nb; j++) ir_operand_push(dj, NULL);
    IR_LIT(dj).ival = (long) nb;
    if (entry_out) *entry_out = dj;
    return dj;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_mark_into(lcx_t * cx, int before, IR_t * tgt, const char * gsz, const char * osz) {
    for (int k = before; k < cx->g->n; k++) {
        IR_t * x = cx->g->all[k];
        if (!x) continue;
        if (gsz && x->γ.node == tgt) { memcpy(x->γ.sz, gsz, strlen(gsz) + 1); }
        if (osz && x->ω.node == tgt) { memcpy(x->ω.sz, osz, strlen(osz) + 1); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_atom_goal(const char * nm) {
    tree_t * n = ast_node_new(TT_QLIT); n->v.sval = (char *) nm; return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_lower_ite(lcx_t * cx, const tree_t * C, const tree_t * T, const tree_t * E, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * ig = build(cx, IR_INDIRECT_GOTO, γnext, ωfail);
    IR_t * mark = build(cx, IR_BOUND, NULL, ig);
    IR_t * unmk_c = build(cx, IR_UNMARK, ig, ωfail); ir_operand_push(unmk_c, mark); IR_LIT(unmk_c).ival = 1;
    IR_t * unmk_f = build(cx, IR_UNMARK, ig, ωfail); ir_operand_push(unmk_f, mark); IR_LIT(unmk_f).ival = 1;
    IR_t * arm_entry[2] = { NULL, NULL };
    const tree_t * arms[2]; int nb = 0;
    arms[nb++] = T; if (E) arms[nb++] = E;
    for (int j = 0; j < nb; j++) {
        IR_t * ml = build(cx, IR_MOVE_LABEL, NULL, ωfail);
        int before = cx->g->n;
        lc_vec av; lc_vec_init(&av, (int) sizeof(const tree_t *));
        collect_conj(arms[j], &av);
        IR_t * ae = NULL; IR_t * redo = NULL;
        IR_t * first = pl_lower_conj(cx, (const tree_t * const *) av.data, av.n, ml, unmk_c, &ae, &redo, NULL);
        ir_operand_push(ml, redo ? redo : ig);
        ir_operand_push(ml, ig);
        IR_LIT(ml).ival = redo ? 1 : 0;
        arm_entry[j] = ae ? ae : (first ? first : ml);
    }
    { int before = cx->g->n;
      lc_vec cv; lc_vec_init(&cv, (int) sizeof(const tree_t *));
      collect_conj(C, &cv);
      IR_t * ce = NULL;
      lc_γ_to(unmk_f, (nb > 1) ? arm_entry[1] : ig);
      IR_t * saveω = cx->cutω; cx->cutω = unmk_f;
      IR_t * savecutω = cx->cutω; cx->cutω = unmk_f;
      IR_t * cfirst = pl_lower_conj(cx, (const tree_t * const *) cv.data, cv.n, arm_entry[0], unmk_f, &ce, NULL, NULL);
      cx->cutω = savecutω;
      cx->cutω = saveω;
      lc_γ_to(mark, ce ? ce : (cfirst ? cfirst : arm_entry[0]));
      if (entry_out) *entry_out = mark; }
    return ig;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_leaf_lv(lcx_t * cx, const char * sym, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out);
static IR_t * pl_lower_catch(lcx_t * cx, const tree_t * G, const tree_t * C, const tree_t * R, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * mark = build(cx, IR_BOUND, NULL, ωfail);
    IR_t * unmk = build(cx, IR_UNMARK, NULL, ωfail); ir_operand_push(unmk, mark);
    tree_t * ct = ast_node_new(TT_FNC); ct->v.sval = (char *) "$catch_handle"; ast_push(ct, (tree_t *) C);
    IR_t * he = NULL;
    IR_t * hc = pl_leaf_lv(cx, "$catch_handle", ct, 1, NULL, ωfail, &he);
    lc_γ_to(unmk, he ? he : hc);
    { lc_vec rv; lc_vec_init(&rv, (int) sizeof(const tree_t *));
      collect_conj(R, &rv);
      IR_t * re = NULL;
      IR_t * rfirst = pl_lower_conj(cx, (const tree_t * const *) rv.data, rv.n, γnext, ωfail, &re, NULL, NULL);
      lc_γ_to(hc, re ? re : (rfirst ? rfirst : γnext)); }
    { lc_vec gv; lc_vec_init(&gv, (int) sizeof(const tree_t *));
      collect_conj(G, &gv);
      IR_t * ge = NULL;
      IR_t * gfirst = pl_lower_conj(cx, (const tree_t * const *) gv.data, gv.n, γnext, unmk, &ge, NULL, NULL);
      lc_γ_to(mark, ge ? ge : (gfirst ? gfirst : unmk)); }
    if (entry_out) *entry_out = mark;
    return hc;
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
    { "atom_to_term", 3, "$atom_to_term" }, { "read_term_from_atom", 3, "$read_term_from_atom" }, { "read_term_from_chars", 3, "$read_term_from_chars" }, { "read_term_from_codes", 3, "$read_term_from_codes" },
    { "writeq", 1, "$writeq" }, { "print", 1, "$writeq" }, { "write_canonical", 1, "$write_canonical" }, { "writeln", 1, "$writeln" },
    { "put_char", 1, "$put_char" },
    { "halt", 0, "$halt" }, { "halt", 1, "$halt" }, { "flush_output", 0, "$flush_output" }, { "format", 1, "$format" }, { "format", 2, "$format" },
    { "write", 2, "$write_s" }, { "writeq", 2, "$writeq_s" }, { "print", 2, "$writeq_s" }, { "write_canonical", 2, "$write_canonical_s" }, { "writeln", 2, "$writeln_s" }, { "nl", 1, "$nl_s" },
    { "put_char", 2, "$put_char_s" }, { "flush_output", 1, "$flush_output_s" }, { "format", 3, "$format3" }, { "read", 2, "$read_s" }, { "get_char", 2, "$get_char_s" }, { "peek_char", 2, "$peek_char_s" },
    { "open", 3, "$open" }, { "open", 4, "$open4" }, { "close", 1, "$close" }, { "close", 2, "$close" }, { "current_output", 1, "$current_output" }, { "current_input", 1, "$current_input" },
    { "set_output", 1, "$set_output" }, { "set_input", 1, "$set_input" }, { "keysort", 2, "$keysort" }, { "op", 3, "$op" },
    { 0, 0, 0 } };
static const char * pl_det_leaf_sym(const char * nm, int ar) {
    for (int i = 0; pl_det_leaves[i].nm; i++) if (pl_det_leaves[i].ar == ar && !strcmp(nm, pl_det_leaves[i].nm)) return pl_det_leaves[i].sym;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const struct { const char * nm; int ar; const char * gsym; } pl_anum_guards[] = {
    { "atom_length", 2, "$pl_anum_guard2" }, { "atom_chars", 2, "$pl_anum_guard2" }, { "atom_codes", 2, "$pl_anum_guard2" }, { "char_code", 2, "$pl_anum_guard2" },
    { "number_chars", 2, "$pl_anum_guard2" }, { "number_codes", 2, "$pl_anum_guard2" }, { "atom_concat", 3, "$pl_anum_guard3" }, { 0, 0, 0 } };
static const char * pl_anum_guard_sym(const char * nm, int ar) {
    for (int i = 0; pl_anum_guards[i].nm; i++) if (pl_anum_guards[i].ar == ar && !strcmp(nm, pl_anum_guards[i].nm)) return pl_anum_guards[i].gsym;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_leaf_lv_guarded(lcx_t * cx, const char * sym, const char * gsym, const char * gname, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = sym;
    IR_t * chk = build(cx, IR_CALL, nd, ωfail); IR_LIT(chk).sval = gsym;
    IR_t * nl = build(cx, IR_LIT_STRING, chk, ωfail); IR_LIT(nl).sval = (char *) gname;
    ir_operand_push(chk, nl);
    IR_t * prev = NULL; IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL; IR_t * a = term_lval_e(cx, t->c[i], &ae); IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en); else first = en;
        lc_ω_to(a, ωfail);
        prev = a; ir_operand_push(nd, a); ir_operand_push(chk, a);
    }
    if (prev) lc_γ_to(prev, nl);
    if (entry_out) *entry_out = first ? first : nl;
    return nd;
}
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
static IR_t * pl_leaf_guarded1(lcx_t * cx, const char * sym, const char * guard_sym, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = sym;
    IR_t * ve = NULL; IR_t * v = term_lval_e(cx, t->c[0], &ve);
    lc_ω_to(v, ωfail);
    IR_t * chk = build(cx, IR_CALL, nd, ωfail); IR_LIT(chk).sval = guard_sym;
    ir_operand_push(chk, v);
    ir_operand_push(nd, v);
    lc_γ_to(v, chk);
    if (entry_out) *entry_out = ve ? ve : v;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_leaf_guarded1_dir(lcx_t * cx, const char * sym, const char * guard_sym, const char * dir, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = sym;
    IR_t * ve = NULL; IR_t * v = term_lval_e(cx, t->c[0], &ve);
    lc_ω_to(v, ωfail);
    IR_t * chk = build(cx, IR_CALL, nd, ωfail); IR_LIT(chk).sval = guard_sym;
    IR_t * dl = build(cx, IR_LIT_STRING, chk, ωfail); IR_LIT(dl).sval = dir;
    ir_operand_push(chk, v); ir_operand_push(chk, dl);
    ir_operand_push(nd, v);
    lc_γ_to(v, dl);
    if (entry_out) *entry_out = ve ? ve : v;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_dyn_index(const char * name, int arity) {
    if (!name) return -1;
    for (int i = 0; i < g_stage2.pl_dyn_n; i++) if (g_stage2.pl_dyn_name[i] && !strcmp(g_stage2.pl_dyn_name[i], name) && g_stage2.pl_dyn_arity[i] == arity) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_dyn_index_or_add(const char * name, int arity) {
    int k = pl_dyn_index(name, arity);
    if (k >= 0) return k;
    pl_dyn_mark(strdup(name), arity);
    return pl_dyn_index(name, arity);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_db_leaf1(lcx_t * cx, const char * sym, int k, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = (char *) sym;
    IR_t * kn = build(cx, IR_LIT_INTEGER, NULL, ωfail); IR_LIT(kn).ival = k;
    lc_γ_to(kn, nd); lc_ω_to(kn, ωfail);
    ir_operand_push(nd, kn);
    if (entry_out) *entry_out = kn;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_owned(const char * nm, int ar) {
    if (pl_dyn_index(nm, ar) < 0) return 0;
    { char key[264]; snprintf(key, sizeof key, "%s/%d", nm, ar);
      const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
      if (ch && ch->t == TT_CHOICE && ch->n > 0) return 0;
      return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_head_key(const tree_t * a, int * ar_out) {
    const tree_t * h = a;
    if (!h) return NULL;
    if (h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":-") && h->n == 2) h = h->c[0];
    if (!h) return NULL;
    if (h->t == TT_FNC && h->v.sval) { if (ar_out) *ar_out = h->n; return h->v.sval; }
    if ((h->t == TT_QLIT || h->t == TT_NAME) && h->v.sval) { if (ar_out) *ar_out = 0; return h->v.sval; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_spec_key(const tree_t * s, int * ar_out) {
    if (!s || s->t != TT_FNC || !s->v.sval || strcmp(s->v.sval, "/") || s->n != 2) return NULL;
    if (!s->c[0] || !(s->c[0]->t == TT_QLIT || s->c[0]->t == TT_NAME) || !s->c[0]->v.sval) return NULL;
    if (!s->c[1] || s->c[1]->t != TT_ILIT) return NULL;
    if (ar_out) *ar_out = (int) s->c[1]->v.ival;
    return s->c[0]->v.sval;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_db_leaf2(lcx_t * cx, const char * sym, int k, const tree_t * arg, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = (char *) sym;
    IR_t * kn = build(cx, IR_LIT_INTEGER, NULL, ωfail); IR_LIT(kn).ival = k;
    IR_t * te = NULL; IR_t * tv = term_e(cx, arg, &te);
    lc_γ_to(kn, te ? te : tv); lc_ω_to(kn, ωfail);
    lc_γ_to(tv, nd); lc_ω_to(tv, ωfail);
    ir_operand_push(nd, kn); ir_operand_push(nd, tv);
    if (entry_out) *entry_out = kn;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_user_call(lcx_t * cx, const char * nm, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    int pl_db_live = pl_db_owned(nm, nargs);
    { char key[264]; snprintf(key, sizeof key, "%s/%d", nm, nargs);
      const tree_t * ch = pl_db_live ? (const tree_t *) 0 : resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
      if (!ch && !pl_db_live) { tree_t * kt = ast_node_new(TT_QLIT); kt->v.sval = strdup(key);
                 tree_t * ct = ast_node_new(TT_FNC); ct->v.sval = (char *) "$existence_error"; ast_push(ct, kt);
                 return pl_leaf(cx, "$existence_error", ct, 1, ωfail, ωfail, entry_out); }
      if (ch && ch->t == TT_FNC) { IR_t * nd = build(cx, IR_GOTO, ωfail, ωfail); if (entry_out) *entry_out = nd; return nd; } }
    IR_t * nd = build(cx, IR_CALL_PROC_STAGED, γnext, ωfail); IR_LIT(nd).sval = pl_pi_name(nm, nargs);
    IR_t * prev = NULL; IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL; IR_t * a = term_lval_e(cx, t->c[i], &ae); IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en); else first = en;
        lc_ω_to(a, ωfail);
        prev = a; ir_operand_push(nd, a);
    }
    if (prev) lc_γ_to(prev, nd);
    IR_t * body_entry = first ? first : nd;
    if (pl_db_live) { tree_t * pit = ast_node_new(TT_QLIT); pit->v.sval = (char *) pl_pi_name(nm, nargs);
        IR_t * guard_entry = NULL; pl_db_leaf2(cx, "$db_alive", pl_dyn_index_or_add(nm, nargs), pit, body_entry, ωfail, &guard_entry);
        if (entry_out) *entry_out = guard_entry; return nd; }
    if (entry_out) *entry_out = body_entry;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_nil_term(void) { return ast_node_new(TT_MAKELIST); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * pl_meta_call_dyn(lcx_t * cx, const tree_t * g, const tree_t * const * extra, int nextra, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = build(cx, IR_CALL_VALUE, γnext, ωfail); IR_LIT(nd).sval = "goal";
    IR_t * ge = NULL; IR_t * gv = term_e(cx, g, &ge); lc_ω_to(gv, ωfail);
    ir_operand_push(nd, gv);
    IR_t * prev = gv; IR_t * first = ge ? ge : gv;
    for (int i = 0; i < nextra; i++) {
        IR_t * ae = NULL; IR_t * a = term_lval_e(cx, extra[i], &ae);
        lc_γ_to(prev, ae ? ae : a); lc_ω_to(a, ωfail);
        prev = a; ir_operand_push(nd, a);
    }
    lc_γ_to(prev, nd);
    if (entry_out) *entry_out = first;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_is_callable(const tree_t * g) {
    if (!g) return 0;
    switch (g->t) { case TT_FNC: case TT_QLIT: case TT_NAME: case TT_CUT: case TT_UNIFY: case TT_IF: case TT_PROGRAM: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const tree_t * pl_meta_goal(const tree_t * g, const tree_t * const * extra, int nextra) {
    if (!g) return NULL;
    if (nextra <= 0) return pl_tree_is_callable(g) ? g : NULL;
    if (g->t != TT_FNC && g->t != TT_NAME && g->t != TT_QLIT) return NULL;
    tree_t * e = ast_node_new(TT_FNC); e->v.sval = g->v.sval; e->line = g->line;
    for (int i = 0; i < g->n; i++) ast_push(e, g->c[i]);
    for (int i = 0; i < nextra; i++) ast_push(e, (tree_t *) extra[i]);
    return e;
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
            return pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, γnext, ωfail, entry_out, NULL, NULL);
        }
        if (!strcmp(nm, "write") && t->n == 1) return pl_leaf(cx, "$write", t, 1, γnext, ωfail, entry_out);
        if (!strcmp(nm, ";") || !strcmp(nm, "|")) { if (pl_is_ite(t)) return pl_lower_ite(cx, t->c[0]->c[0], t->c[0]->c[1], t->c[1], γnext, ωfail, entry_out);
            return pl_lower_disj(cx, t, γnext, ωfail, entry_out); }
        if (!strcmp(nm, "->") && t->n == 2) return pl_lower_ite(cx, t->c[0], t->c[1], NULL, γnext, ωfail, entry_out);
        if ((!strcmp(nm, "\\+") || !strcmp(nm, "not")) && t->n == 1) return pl_lower_ite(cx, t->c[0], pl_atom_goal("fail"), pl_atom_goal("true"), γnext, ωfail, entry_out);
        if (!strcmp(nm, "once") && t->n == 1) return pl_lower_ite(cx, t->c[0], pl_atom_goal("true"), pl_atom_goal("fail"), γnext, ωfail, entry_out);
        if (!strcmp(nm, "ignore") && t->n == 1) return pl_lower_ite(cx, t->c[0], pl_atom_goal("true"), pl_atom_goal("true"), γnext, ωfail, entry_out);
        if (!strcmp(nm, "forall") && t->n == 2) {
            tree_t * inner = ast_node_new(TT_FNC); inner->v.sval = (char *) "\\+"; ast_push(inner, (tree_t *) t->c[1]);
            tree_t * conj = ast_node_new(TT_FNC); conj->v.sval = (char *) ","; ast_push(conj, (tree_t *) t->c[0]); ast_push(conj, inner);
            return pl_lower_ite(cx, conj, pl_atom_goal("fail"), pl_atom_goal("true"), γnext, ωfail, entry_out); }
        if ((!strcmp(nm, "call") && t->n >= 1) || (!strcmp(nm, "phrase") && (t->n == 2 || t->n == 3))) {
            const tree_t * xs[2]; const tree_t * const * extra; int nextra;
            if (!strcmp(nm, "phrase")) { xs[0] = t->c[1]; xs[1] = (t->n == 3) ? t->c[2] : pl_nil_term(); extra = xs; nextra = 2; }
            else { extra = (const tree_t * const *) &t->c[1]; nextra = t->n - 1; }
            if (t->c[0] && t->c[0]->t == TT_VAR) return pl_meta_call_dyn(cx, t->c[0], extra, nextra, γnext, ωfail, entry_out);
            const tree_t * ext = pl_meta_goal(t->c[0], extra, nextra);
            if (!ext) pl_refuse("meta-call whose goal is not a callable term known at compile time --", nm, 10);
            IR_t * saveω = cx->cutω; cx->cutω = ωfail;
            IR_t * r = goal(cx, ext, γnext, ωfail, entry_out);
            cx->cutω = saveω;
            return r;
        }
        if (!strcmp(nm, "throw") && t->n == 1) return pl_leaf(cx, "$throw", t, 1, ωfail, ωfail, entry_out);
        if (!strcmp(nm, "catch") && t->n == 3) return pl_lower_catch(cx, t->c[0], t->c[1], t->c[2], γnext, ωfail, entry_out);
        if (!strcmp(nm, "=") && t->n == 2) { IR_t * e = NULL; IR_t * nd = unify_pair(cx, t->c[0], t->c[1], γnext, ωfail, &e); if (entry_out) *entry_out = e ? e : nd; return nd; }
        if (!strcmp(nm, "\\=") && t->n == 2) {
            tree_t * u = ast_node_new(TT_UNIFY); ast_push(u, (tree_t *) t->c[0]); ast_push(u, (tree_t *) t->c[1]);
            return pl_lower_ite(cx, u, pl_atom_goal("fail"), pl_atom_goal("true"), γnext, ωfail, entry_out); }
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
        if (!strcmp(nm, "tab") && t->n == 2) {
            IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = "$tab_s";
            IR_t * se = NULL; IR_t * sv = term_lval_e(cx, t->c[0], &se);
            IR_t * ve = NULL; IR_t * v = lower_arith_val(cx, t->c[1], ωfail, &ve);
            lc_γ_to(sv, ve ? ve : v); lc_ω_to(sv, ωfail); lc_γ_to(v, nd); lc_ω_to(v, ωfail);
            ir_operand_push(nd, sv); ir_operand_push(nd, v);
            if (entry_out) *entry_out = se ? se : sv;
            return nd;
        }
        if (!strcmp(nm, "sub_atom") && t->n == 5) {
            IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = "$sub_atom_at";
            IR_t * to = build(cx, IR_TO, nd, ωfail); IR_LIT(to).sval = (char *) "ag";
            IR_t * ae = NULL; IR_t * av = term_e(cx, t->c[0], &ae);
            IR_t * cnt = build(cx, IR_CALL, to, ωfail); IR_LIT(cnt).sval = "$sub_atom_n";
            IR_t * av2e = NULL; IR_t * av2 = term_e(cx, t->c[0], &av2e);
            IR_t * lo = build(cx, IR_LIT_INTEGER, NULL, ωfail); IR_LIT(lo).ival = 0;
            IR_t * bl = NULL, * ll = NULL, * al = NULL, * sl = NULL; IR_t * be = NULL, * le = NULL, * ale = NULL, * se = NULL;
            bl = term_lval_e(cx, t->c[1], &be); ll = term_lval_e(cx, t->c[2], &le);
            al = term_lval_e(cx, t->c[3], &ale); sl = term_lval_e(cx, t->c[4], &se);
            IR_t * gnl = build(cx, IR_LIT_STRING, NULL, ωfail); IR_LIT(gnl).sval = (char *) "sub_atom";
            IR_t * gchk = build(cx, IR_CALL, lo, ωfail); IR_LIT(gchk).sval = "$pl_anum_guard5";
            lc_γ_to(gnl, gchk); lc_ω_to(gnl, ωfail);
            ir_operand_push(gchk, gnl); ir_operand_push(gchk, av); ir_operand_push(gchk, bl); ir_operand_push(gchk, ll); ir_operand_push(gchk, al); ir_operand_push(gchk, sl);
            lc_γ_to(bl, le ? le : ll);   lc_ω_to(bl, ωfail);
            lc_γ_to(ll, ale ? ale : al); lc_ω_to(ll, ωfail);
            lc_γ_to(al, se ? se : sl);   lc_ω_to(al, ωfail);
            lc_γ_to(sl, ae ? ae : av);   lc_ω_to(sl, ωfail);
            lc_γ_to(av, gnl); lc_ω_to(av, ωfail);
            lc_γ_to(lo, av2e ? av2e : av2); lc_ω_to(lo, ωfail);
            lc_γ_to(av2, cnt); lc_ω_to(av2, ωfail);
            ir_operand_push(cnt, av2);
            ir_operand_push(to, lo); ir_operand_push(to, cnt);
            ir_operand_push(nd, av); ir_operand_push(nd, to);
            ir_operand_push(nd, bl); ir_operand_push(nd, ll); ir_operand_push(nd, al); ir_operand_push(nd, sl);
            lc_ω_to_β(nd, to);
            if (entry_out) *entry_out = be ? be : bl;
            return to;
        }
        if ((!strcmp(nm, "findall") || !strcmp(nm, "bagof") || !strcmp(nm, "setof")) && t->n == 3) {
            const char * fin = !strcmp(nm, "findall") ? "$findall_result" : (!strcmp(nm, "bagof") ? "$bagof_result" : "$setof_result");
            if (pl_tree_has_caret(t->c[1])) pl_refuse("bagof/setof free-variable grouping (^)", nm, 8);
            IR_t * nd  = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = (char *) fin;
            IR_t * acc = build(cx, IR_CALL, NULL, ωfail);  IR_LIT(acc).sval = "$findall_new";
            IR_t * add = build(cx, IR_CALL, NULL, ωfail);  IR_LIT(add).sval = "$findall_add";
            IR_t * gentry = NULL; IR_t * gredo = NULL;
            lc_vec glv; lc_vec_init(&glv, (int) sizeof(const tree_t *));
            collect_conj(t->c[1], &glv);
            IR_t * te = NULL; IR_t * tv = term_e(cx, t->c[0], &te);
            IR_t * first = pl_lower_conj(cx, (const tree_t * const *) glv.data, glv.n, te ? te : tv, nd, &gentry, &gredo, NULL);
            lc_γ_to(acc, gentry ? gentry : (first ? first : nd));
            lc_γ_to(tv, add); lc_ω_to(tv, nd);
            ir_operand_push(add, acc); ir_operand_push(add, tv);
            if (gredo) lc_γ_to_β(add, gredo); else lc_γ_to(add, nd);
            lc_ω_to(add, nd);
            IR_t * re = NULL; IR_t * rl = term_lval_e(cx, t->c[2], &re);
            lc_γ_to(rl, nd); lc_ω_to(rl, ωfail);
            ir_operand_push(nd, acc); ir_operand_push(nd, rl);
            if (entry_out) *entry_out = re ? re : rl;
            lc_γ_to(rl, acc);
            return nd;
        }
        if ((!strcmp(nm, "between") || !strcmp(nm, "for")) && t->n == 3) {
            int forarg = !strcmp(nm, "for");
            const tree_t * lo_t = forarg ? t->c[1] : t->c[0];
            const tree_t * hi_t = forarg ? t->c[2] : t->c[1];
            const tree_t * vr_t = forarg ? t->c[0] : t->c[2];
            IR_t * nd = build(cx, IR_CALL, γnext, ωfail); IR_LIT(nd).sval = "$is_v";
            IR_t * to = build(cx, IR_TO, nd, ωfail); IR_LIT(to).sval = (char *) "ag";
            IR_t * loe = NULL; IR_t * lo = lower_arith_val(cx, lo_t, ωfail, &loe);
            IR_t * hie = NULL; IR_t * hi = lower_arith_val(cx, hi_t, ωfail, &hie);
            IR_t * xe = NULL; IR_t * xl = term_lval_e(cx, vr_t, &xe);
            lc_γ_to(xl, loe ? loe : lo); lc_ω_to(xl, ωfail);
            lc_γ_to(lo, hie ? hie : hi); lc_ω_to(lo, ωfail);
            if (!forarg) {
                IR_t * chk = build(cx, IR_CALL, to, ωfail); IR_LIT(chk).sval = "$pl_between_guard";
                ir_operand_push(chk, lo); ir_operand_push(chk, hi); ir_operand_push(chk, xl);
                lc_γ_to(hi, chk); lc_ω_to(hi, ωfail);
            } else {
                lc_γ_to(hi, to); lc_ω_to(hi, ωfail);
            }
            ir_operand_push(to, lo); ir_operand_push(to, hi);
            ir_operand_push(nd, xl); ir_operand_push(nd, to);
            lc_ω_to_β(nd, to);
            if (entry_out) *entry_out = xe ? xe : xl;
            return to;
        }
        if (!strcmp(nm, "put_char") && t->n == 1) return pl_leaf_guarded1(cx, "$put_char", "$pl_char_guard", t, γnext, ωfail, entry_out);
        if (!strcmp(nm, "current_output") && t->n == 1) return pl_leaf_guarded1(cx, "$current_output", "$pl_curstream_guard", t, γnext, ωfail, entry_out);
        if (!strcmp(nm, "current_input") && t->n == 1) return pl_leaf_guarded1(cx, "$current_input", "$pl_curstream_guard", t, γnext, ωfail, entry_out);
        if (!strcmp(nm, "set_output") && t->n == 1) return pl_leaf_guarded1_dir(cx, "$set_output", "$pl_stream_guard", "output", t, γnext, ωfail, entry_out);
        if (!strcmp(nm, "set_input") && t->n == 1) return pl_leaf_guarded1_dir(cx, "$set_input", "$pl_stream_guard", "input", t, γnext, ωfail, entry_out);
        if (!strcmp(nm, "read_term") && t->n == 2 && pl_tree_is_nil(t->c[1])) return pl_leaf_lv(cx, "$read", t, 1, γnext, ωfail, entry_out);
        if ((!strcmp(nm, "assertz") || !strcmp(nm, "assert") || !strcmp(nm, "asserta")) && t->n == 1) {
            int ar = 0; const char * pn = pl_head_key(t->c[0], &ar);
            if (!pn) pl_refuse("assert of a clause whose head is not a callable term known at compile time --", nm, 10);
            if (!pl_db_owned(pn, ar)) pl_refuse("assert on a predicate that ALSO has clauses in the file -- rung 10b seeds a file-defined predicate into the database as its follow-up; today the two would disagree silently --", pn, 10);
            return pl_db_leaf2(cx, !strcmp(nm, "asserta") ? "$db_asserta" : "$db_assertz", pl_dyn_index_or_add(pn, ar), t->c[0], γnext, ωfail, entry_out); }
        if (!strcmp(nm, "retract") && t->n == 1) {
            int ar = 0; const char * pn = pl_head_key(t->c[0], &ar);
            if (!pn) pl_refuse("retract of a clause whose head is not a callable term known at compile time --", nm, 10);
            if (!pl_db_owned(pn, ar)) pl_refuse("retract on a predicate that has clauses in the file (they are wired boxes, not database rows) --", pn, 10);
            pl_refuse("retract/1 on a dynamic predicate -- the clause-list INTERPRETER that served it is DELETED (Lon 2026-09-03: the clauses are compiled to Byrd boxes, assert adds code and retract removes it); the compiled-clause path lands it --", pn, 10); }
        if (!strcmp(nm, "retractall") && t->n == 1) {
            int ar = 0; const char * pn = pl_head_key(t->c[0], &ar);
            if (!pn) pl_refuse("retractall whose argument is not a callable term known at compile time --", nm, 10);
            if (!pl_db_owned(pn, ar)) pl_refuse("retractall on a predicate that has clauses in the file --", pn, 10);
            return pl_db_leaf2(cx, "$db_retractall", pl_dyn_index_or_add(pn, ar), t->c[0], γnext, ωfail, entry_out); }
        if (!strcmp(nm, "abolish") && t->n == 1) {
            int ar = 0; const char * pn = pl_spec_key(t->c[0], &ar);
            if (!pn) pl_refuse("abolish whose argument is not a Name/Arity known at compile time --", nm, 10);
            if (!pl_db_owned(pn, ar)) pl_refuse("abolish on a predicate that has clauses in the file --", pn, 10);
            return pl_db_leaf1(cx, "$db_abolish", pl_dyn_index_or_add(pn, ar), γnext, ωfail, entry_out); }
        if (!strcmp(nm, "clause") && t->n == 2) {
            int ar = 0; const char * pn = pl_head_key(t->c[0], &ar);
            if (!pn) pl_refuse("clause/2 whose head is not a callable term known at compile time --", nm, 7);
            if (!pl_db_owned(pn, ar)) pl_refuse("clause/2 on a predicate that has clauses in the file (reflecting a wired box needs the proc table, rung 10b follow-up) --", pn, 7);
            pl_refuse("clause/2 on a dynamic predicate -- the clause-list INTERPRETER that served it is DELETED (Lon 2026-09-03: it is all code, not data); the compiled-clause path lands it --", pn, 10); }
        if (!strcmp(nm, "dynamic") && t->n >= 1) return build(cx, IR_SUCCEED, γnext, ωfail);
        { const char * ls = pl_det_leaf_sym(nm, t->n); if (ls) { const char * gs = pl_anum_guard_sym(nm, t->n);
            if (gs) return pl_leaf_lv_guarded(cx, ls, gs, nm, t, t->n, γnext, ωfail, entry_out);
            return pl_leaf_lv(cx, ls, t, t->n, γnext, ωfail, entry_out); } }
        { int r = pl_rung_of(nm); if (r) pl_refuse(r == 6 ? "builtin arity not wired" : "builtin", nm, r); }
        return pl_user_call(cx, nm, t, t->n, γnext, ωfail, entry_out);
    }
    case TT_QLIT: case TT_NAME: {
        const char * nm = t->v.sval ? t->v.sval : "?";
        if (!strcmp(nm, "true")) return build(cx, IR_SUCCEED, γnext, ωfail);
        if (!strcmp(nm, "fail") || !strcmp(nm, "false")) return build(cx, IR_GOTO, ωfail, ωfail);
        if (!strcmp(nm, "nl")) return pl_leaf(cx, "$nl", t, 0, γnext, ωfail, entry_out);
        if (!strcmp(nm, "!")) { IR_t * cn = build(cx, IR_CUT, γnext, cx->cutω); if (cx->cutω != cx->clause_cutω) IR_LIT(cn).ival = 1; return cn; }
        { const char * ls = pl_det_leaf_sym(nm, 0); if (ls) return pl_leaf_lv(cx, ls, t, 0, γnext, ωfail, entry_out); }
        { int r = pl_rung_of(nm); if (r) pl_refuse(r == 6 ? "builtin arity not wired" : "builtin", nm, r); }
        return pl_user_call(cx, nm, t, 0, γnext, ωfail, entry_out);
    }
    case TT_CUT: { IR_t * cn = build(cx, IR_CUT, γnext, cx->cutω); if (cx->cutω != cx->clause_cutω) IR_LIT(cn).ival = 1; return cn; }
    case TT_UNIFY: { IR_t * e = NULL; IR_t * nd = unify_pair(cx, t->c[0], t->c[1], γnext, ωfail, &e); if (entry_out) *entry_out = e ? e : nd; return nd; }
    case TT_IF: return pl_lower_ite(cx, t->c[0], (t->n > 1) ? t->c[1] : NULL, (t->n > 2) ? t->c[2] : NULL, γnext, ωfail, entry_out);
    case TT_PROGRAM: return pl_lower_conj(cx, (const tree_t * const *) t->c, t->n, γnext, ωfail, entry_out, NULL, NULL);
    case TT_VAR: return pl_meta_call_dyn(cx, t, NULL, 0, γnext, ωfail, entry_out);
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
    lcx_t cx; cx.g = g; cx.tω = NULL; cx.cutω = NULL; cx.clause_cutω = NULL;
    IR_t * succeed = build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail    = build(&cx, IR_FAIL, NULL, NULL);
    IR_t * step    = build(&cx, IR_FAIL, NULL, NULL);
    cx.cutω = fail; cx.clause_cutω = fail;
    IR_t * entry = NULL; IR_t * redo = NULL; IR_t * tnode = NULL;
    IR_t * first = pl_lower_conj(&cx, gl, ng, succeed, step, &entry, &redo, &tnode);
    if (tnode && tnode->op == IR_CALL_PROC_STAGED) tnode->seal = PL_SEAL_TAIL;
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
    lcx_t cx; cx.g = g; cx.tω = NULL; cx.cutω = NULL; cx.clause_cutω = NULL;
    IR_t * step = build(&cx, IR_FAIL, NULL, NULL);
    cx.cutω = build(&cx, IR_FAIL, NULL, NULL); cx.clause_cutω = cx.cutω;
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
        IR_t * bentry = NULL; IR_t * redo = NULL; IR_t * tnode = NULL;
        IR_t * first = pl_lower_conj(&cx, (const tree_t * const *)(cl->c + ar), cl->n - ar, succeed, step, &bentry, &redo, &tnode);
        if (tnode && tnode->op == IR_CALL_PROC_STAGED) tnode->seal = PL_SEAL_TAIL;
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
void * pl_runtime_define_pred(const char * key, const tree_t * choice, int arity) {
    extern IR_graph_t * g_emit_cfg; extern int g_frame_active; extern int g_rt_fragment_emit; extern int g_gen_proc_active;
    extern int emit_jmp_entry_for_proc(const char *, int, int, IR_graph_t *); extern void emit_jmp_entry_clear(void);
    extern void zls_graph_name(const IR_graph_t *, const char *);
    extern void rt_proc_set_fn(const char *, bb_box_fn); extern void rt_proc_set_frame_bytes(const char *, int);
    extern void rt_proc_set_dyn_scope(const char *, int); extern void emit_patzeta_register(const char *, int, int, int);
    extern void rt_proc_set_generator(const char *, int); extern void rt_proc_set_jmpentry(const char *, int); extern void rt_proc_set_zstatic(const char *, int);
    extern void bb_ab_seal_entry_cells(const char *, void *, int);
    extern int g_last_flat_frame_bytes; extern int g_last_flat_fp; extern int g_last_flat_uniform; extern int g_last_flat_zstatic;
    int idx; IR_graph_t * g; bb_box_fn fn; IR_graph_t * cfg_sv; int fa; int rfe_sv; int gpa_sv;
    if (!key || !choice) return (void *)0;
    { extern void bb_pool_init(void); bb_pool_init(); }
    idx = lower_pl_pred_graph(key, choice);
    if (idx < 0) return (void *)0;
    g = g_stage2.bbp.table[idx];
    if (!g) return (void *)0;
    g->zframe_graph = 1; g->zframe_pinned_base = 1;
    { extern void zls_forget_graph_nodes(const IR_graph_t *); zls_forget_graph_nodes(g); }
    { extern void ir_drive_slot_assign(IR_graph_t *); ir_drive_slot_assign(g); }
    cfg_sv = g_emit_cfg; g_emit_cfg = g;
    fa = g_frame_active; g_frame_active = 1;
    rfe_sv = g_rt_fragment_emit; g_rt_fragment_emit = 1;
    rt_proc_set_generator(key, 1); rt_proc_set_jmpentry(key, 1); rt_proc_set_dyn_scope(key, 0);
    gpa_sv = g_gen_proc_active; g_gen_proc_active = 1;
    { extern int g_flat_frame_floor; extern int zls_g_region(const IR_graph_t *); g_flat_frame_floor = 0;
      if (g->entry && ((g->entry->op == IR_DEFINE && IR_LIT(g->entry).ival == 3) || g->entry->op == IR_GOTO_DEFERRED)) {
          for (int _mi = 0; _mi < g_stage2.proc_count; _mi++) if (g_stage2.proc_table[_mi].name && !strcmp(g_stage2.proc_table[_mi].name, "main")) {
              int _mx = g_stage2.proc_table[_mi].bb_idx; if (_mx >= 0 && _mx < g_stage2.bbp.count && g_stage2.bbp.table[_mx]) g_flat_frame_floor = zls_g_region(g_stage2.bbp.table[_mx]); break; }
          if (g_flat_frame_floor <= 0) g_flat_frame_floor = zls_g_region(g); } }
    emit_jmp_entry_for_proc(key, 0, 1, g);
    { extern int g_flat_dc_np; extern int rt_pl_dc_ok(const char *, int); g_flat_dc_np = rt_pl_dc_ok(key, g->nparams) ? g->nparams : -1; }
    zls_graph_name(g, key);
    { char pfx[300]; snprintf(pfx, sizeof pfx, "proc_%s", key);
      if (getenv("SCRIP_PL_RTASM")) { extern int zls_g_region(const IR_graph_t *); extern int zls_off(const IR_t *); int _mx = -1; for (int _i = 0; _i < g->n; _i++) if (g->all[_i]) { int _o = zls_off(g->all[_i]); if (_o > _mx) _mx = _o; }
        fprintf(stderr, "[RTASM] %s: nodes=%d nparams=%d zls_region=%d max_granted_cell=%d %s\n", key, g->n, g->nparams, zls_g_region(g), _mx, (_mx >= 0 && _mx + 16 > zls_g_region(g)) ? "*** CELL OUTSIDE REGION ***" : "(in region)");
        { extern int zls_scope_of(const IR_t *); extern int zls_g_first_scope(const IR_graph_t *); int _rs = zls_g_first_scope(g);
          for (int _i = 0; _i < g->n; _i++) if (g->all[_i]) { int _o = zls_off(g->all[_i]); int _sc = zls_scope_of(g->all[_i]); if (_o < 0) continue;
            fprintf(stderr, "[RTASM]   node[%d] op=%d off=%d scope=%d root_scope=%d %s\n", _i, (int)g->all[_i]->op, _o, _sc, _rs, (_sc >= 0 && _sc != _rs) ? "*** STALE ENTRY FROM ANOTHER GRAPH ***" : ""); } }
        fprintf(stderr, "[RTASM] ---- runtime fragment for %s ----\n", key); emit_chain(g->entry, stderr, pfx); fprintf(stderr, "[RTASM] ---- end %s ----\n", key); }
      fn = emit_chain(g->entry, (FILE *)0, pfx); }
    emit_jmp_entry_clear();
    g_gen_proc_active = gpa_sv;
    g_rt_fragment_emit = rfe_sv; g_frame_active = fa; g_emit_cfg = cfg_sv;
    if (!fn) return (void *)0;
    rt_proc_set_frame_bytes(key, g_last_flat_frame_bytes);
    rt_proc_set_fn(key, fn);
    bb_ab_seal_entry_cells(key, (void *)fn, 1);
    rt_proc_set_zstatic(key, g_last_flat_zstatic);
    emit_patzeta_register(key, g_last_flat_frame_bytes, g_last_flat_fp, g_last_flat_uniform);
    { extern long g_last_dc_off; extern void rt_proc_set_dcfn(const char *, void *); rt_proc_set_dcfn(key, (g_last_dc_off >= 0) ? (void *)((char *)fn + g_last_dc_off) : (void *)0); }
    return (void *)fn;
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
enum { PL_DIR_VARS_MAX = 1024 };
static void pl_dir_number_vars(tree_t * t, const char ** names, int * n, int scope0) {
    if (!t) return;
    if (t->t == TT_VAR) {
        const char * nm = t->v.sval;
        int slot = *n;
        if (nm && strcmp(nm, "_") != 0) { for (int i = scope0; i < *n; i++) if (names[i] && !strcmp(names[i], nm)) { slot = i; break; } }
        if (slot == *n && *n < PL_DIR_VARS_MAX) { names[*n] = nm; (*n)++; }
        t->v.ival = slot;
        return;
    }
    for (int i = 0; i < t->n; i++) pl_dir_number_vars(t->c[i], names, n, scope0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_pl_stage2(const tree_t *prog) {
    int _pl_bb0 = g_stage2.bbp.count;
    pl_register_program(&g_stage2, prog);
    enum { PL_INIT_GOALS_MAX = 256 };
    const tree_t * init_goals[PL_INIT_GOALS_MAX]; int ninit = 0;
    const tree_t * dir_goals[PL_INIT_GOALS_MAX]; int ndir = 0;
    const char * dvn[PL_DIR_VARS_MAX]; int dvc = 0;
    for (int i = 0; i < prog->n; i++) {
        const tree_t *s = prog->c[i];
        if (!s || s->t != TT_STMT) continue;
        const tree_t *subj = lp_s_expr(s, ":subj");
        if (!subj || subj->t == TT_CHOICE || subj->t == TT_CLAUSE) continue;
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "initialization") && subj->n >= 1) {
            const tree_t *gt = subj->c[0];
            if (gt && ((gt->t == TT_QLIT || gt->t == TT_NAME || gt->t == TT_FNC) && gt->v.sval)) { if (ninit < PL_INIT_GOALS_MAX) init_goals[ninit++] = gt; continue; }
        }
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "dynamic") && subj->n >= 1) {
            for (int k = 0; k < subj->n; k++) pl_decl_dynamic_record(&g_stage2, subj->c[k], (tree_t *) subj); continue; }
        if (subj->t == TT_FNC && subj->v.sval && pl_name_in(subj->v.sval, pl_decl_directives)) continue;
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "op") && subj->n == 3) { if (ninit < PL_INIT_GOALS_MAX) init_goals[ninit++] = subj; continue; }
        if (subj->t == TT_FNC && subj->v.sval && !strcmp(subj->v.sval, "set_prolog_flag") && subj->n == 2) {
            if (pl_flag_directive_is_default(subj)) continue;
            pl_refuse("directive set_prolog_flag", subj->c[0] && subj->c[0]->v.sval ? subj->c[0]->v.sval : "?", 10); }
        { int dv_scope0 = dvc; pl_dir_number_vars((tree_t *) subj, dvn, &dvc, dv_scope0);
          tree_t * ig = ast_node_new(TT_FNC); ig->v.sval = (char *) "ignore"; ast_push(ig, (tree_t *) subj);
          if (ndir < PL_INIT_GOALS_MAX) dir_goals[ndir++] = ig; continue; }
    }
    { const tree_t * all_goals[PL_INIT_GOALS_MAX * 2]; int nall = 0;
      for (int i = 0; i < ndir && nall < PL_INIT_GOALS_MAX * 2; i++) all_goals[nall++] = dir_goals[i];
      for (int i = 0; i < ninit && nall < PL_INIT_GOALS_MAX * 2; i++) all_goals[nall++] = init_goals[i];
      ninit = nall; for (int i = 0; i < nall; i++) init_goals[i < PL_INIT_GOALS_MAX ? i : PL_INIT_GOALS_MAX - 1] = all_goals[i]; }
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
    for (int di = 0; di < g_stage2.pl_dyn_n; di++) {
        const char * dn = g_stage2.pl_dyn_name[di]; int da = g_stage2.pl_dyn_arity[di];
        if (!dn) continue;
        { char key[264]; snprintf(key, sizeof key, "%s/%d", dn, da);
          if (pl_bb_lookup(key, da)) continue;
          { extern tree_t * pl_runtime_clause_tree(tree_t *);
            tree_t * hd; tree_t * raw; tree_t * cl; tree_t * ch;
            if (da > 0) { hd = ast_node_new(TT_FNC); hd->v.sval = strdup(dn);
                          for (int i = 0; i < da; i++) { tree_t * v = ast_node_new(TT_VAR); v->v.sval = (char *) "_"; ast_push(hd, v); } }
            else { hd = ast_node_new(TT_QLIT); hd->v.sval = strdup(dn); }
            { tree_t * fb = ast_node_new(TT_QLIT); fb->v.sval = (char *) "fail";
              raw = ast_node_new(TT_FNC); raw->v.sval = (char *) ":-"; ast_push(raw, hd); ast_push(raw, fb); }
            cl = pl_runtime_clause_tree(raw);
            if (!cl) continue;
            ch = ast_node_new(TT_CHOICE); ch->v.sval = strdup(key); ast_push(ch, cl);
            { int bb_idx = lower_pl_pred_graph(key, ch);
              if (bb_idx < 0) continue;
              pl_bb_register(key, da, bb_idx);
              pl_new_proc(key, da, bb_idx); } } }
    }
    for (int _gi = _pl_bb0; _gi < g_stage2.bbp.count; _gi++) if (g_stage2.bbp.table[_gi]) { g_stage2.bbp.table[_gi]->zframe_graph = 1; g_stage2.bbp.table[_gi]->zframe_pinned_base = 1; }
    top->standing_cells = g_stage2.pl_dyn_n;
    return &g_stage2;
}
