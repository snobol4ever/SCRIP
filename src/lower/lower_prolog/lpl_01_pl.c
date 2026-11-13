#include "lower/lower_prolog/lpl_head.h"
#include <stdio.h>
#include "runtime/core/g_runtime.h"
#include <stdlib.h>
#include "ir/ir_graph.h"
#include "ir/ir_construct.h"
#include "ir/stage2.h"
#include "ir/stage2_pl.h"
#include "lower/lower_prolog.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "planner/frame_layout.h"
#include "lower/pl_control_names.h"
#include "lower/lower_prolog/lpl_01_pl.h"
#include "lower/lower_prolog/lpl_02_pl.h"
#include "lower/lower_prolog/lpl_03_pl.h"
#include "lower/lower_prolog/lpl_04_pl.h"
#include "lower/lower_prolog/lpl_05_pl.h"
#include "lower/lower_prolog/lpl_06_pl.h"
#include "lower/lower_prolog/lpl_08_pl.h"
#include "lower/lower_prolog/lpl_09_sh.h"
#include "lower/g_lower_pl.h"
static int pl_trace_wanted(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned resolve_pred_hash(const char *s) { unsigned h = 5381; while (*s) h = h * 33 ^ (unsigned char)*s++; return h % STAGE2_PL_PRED_TABLE_SIZE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
Resolve_PredEntry *resolve_pred_table_insert(Resolve_PredTable *pt, const char *key, tree_t *choice) {
    unsigned h = resolve_pred_hash(key);
    Resolve_PredEntry *e = (Resolve_PredEntry *) ct_alloc(sizeof(Resolve_PredEntry));
    e->key = key;
    e->choice = choice;
    e->entry_pc = -1;
    e->file = NULL;
    e->next = pt->buckets[h];
    pt->buckets[h] = e;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *resolve_pred_table_lookup(Resolve_PredTable *pt, const char *key) {
    for (Resolve_PredEntry *e = pt->buckets[resolve_pred_hash(key)]; e; e = e->next) if (strcmp(e->key, key) == 0) return e->choice;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_bb_ent_t * pl_bb_lookup(const char * name, int arity) {
    if (!name) return NULL;
    for (uint32_t i = 0; i < g_lower_pl.bb_cv.len; i++) { pl_bb_ent_t * e = &CV_AT(g_lower_pl.bb_cv, pl_bb_ent_t, i); if (e->arity == arity && e->name && strcmp(e->name, name) == 0) return e; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_bb_ent_t * pl_bb_register(const char * name, int arity, int bb_idx) {
    if (!name) return NULL;
    pl_bb_ent_t * existing = pl_bb_lookup(name, arity);
    if (existing) { existing->bb_idx = bb_idx; return existing; }
    pl_bb_ent_t * e = &CV_PUSH(g_lower_pl.bb_cv, pl_bb_ent_t);
    e->name = ct_strdup(name);
    e->arity = arity;
    e->bb_idx = bb_idx;
    return e;
}
void pl_dyn_mark(const char *name, int arity) {
    if (!name) return;
    if (pl_pi_is_static_builtin(name, arity) && !pl_swi_user_redefines(name, arity)) return;
    for (int i = 0; i < g_stage2_pl.pl_dyn_n; i++) if (g_stage2_pl.pl_dyn_name[i] && !strcmp(g_stage2_pl.pl_dyn_name[i], name) && g_stage2_pl.pl_dyn_arity[i] == arity) return;
    if (g_stage2_pl.pl_dyn_n >= g_stage2_pl.pl_dyn_cap) {
        int nc = g_stage2_pl.pl_dyn_cap > 0 ? g_stage2_pl.pl_dyn_cap * 2 : 64;
        g_stage2_pl.pl_dyn_name = (const char **) ct_grow((void *) g_stage2_pl.pl_dyn_name, (size_t) nc * sizeof(const char *));
        g_stage2_pl.pl_dyn_arity = (int *) ct_grow((void *) g_stage2_pl.pl_dyn_arity, (size_t) nc * sizeof(int));
        g_stage2_pl.pl_dyn_cap = nc;
    }
    g_stage2_pl.pl_dyn_name[g_stage2_pl.pl_dyn_n] = name;
    g_stage2_pl.pl_dyn_arity[g_stage2_pl.pl_dyn_n] = arity;
    g_stage2_pl.pl_dyn_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_dyn_is_marked(const char *name, int arity) {
    if (!name) return 0;
    for (int i = 0; i < g_stage2_pl.pl_dyn_n; i++) if (g_stage2_pl.pl_dyn_name[i] && !strcmp(g_stage2_pl.pl_dyn_name[i], name) && g_stage2_pl.pl_dyn_arity[i] == arity) return 1;
    return 0;
}
void pl_decl_dyn_mark(const char * name, int arity) {
    if (!name) return;
    for (int i = 0; i < g_lower_pl.decl_dyn_n; i++) if (g_lower_pl.decl_dyn_name[i] && !strcmp(g_lower_pl.decl_dyn_name[i], name) && g_lower_pl.decl_dyn_arity[i] == arity) return;
    if (g_lower_pl.decl_dyn_n >= g_lower_pl.decl_dyn_cap) {
        int nc = g_lower_pl.decl_dyn_cap > 0 ? g_lower_pl.decl_dyn_cap * 2 : 64;
        g_lower_pl.decl_dyn_name = (const char **) ct_grow((void *) g_lower_pl.decl_dyn_name, (size_t) nc * sizeof(const char *));
        g_lower_pl.decl_dyn_arity = (int *) ct_grow((void *) g_lower_pl.decl_dyn_arity, (size_t) nc * sizeof(int));
        g_lower_pl.decl_dyn_cap = nc;
    }
    g_lower_pl.decl_dyn_name[g_lower_pl.decl_dyn_n] = ct_strdup(name);
    g_lower_pl.decl_dyn_arity[g_lower_pl.decl_dyn_n] = arity;
    g_lower_pl.decl_dyn_n++;
}
int pl_decl_dyn_is(const char * name, int arity) {
    if (!name) return 0;
    for (int i = 0; i < g_lower_pl.decl_dyn_n; i++) if (g_lower_pl.decl_dyn_name[i] && !strcmp(g_lower_pl.decl_dyn_name[i], name) && g_lower_pl.decl_dyn_arity[i] == arity) return 1;
    return 0;
}
void pl_decl_other_record(const tree_t * spec, int mf) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_decl_other_record(spec->c[0], mf); pl_decl_other_record(spec->c[1], mf); return; }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_decl_other_record(spec->c[i], mf); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "/") && spec->n == 2 && spec->c[0] && (spec->c[0]->t == TT_QLIT || spec->c[0]->t == TT_NAME) && spec->c[0]->v.sval && spec->c[1] &&
        spec->c[1]->t == TT_ILIT) {
        if (mf) CV_PUSH(g_stage2_pl.pl_decl_multifile, const tree_t *) = spec;
        if (g_lower_pl.decl_other_n >= g_lower_pl.decl_other_cap) {
            int nc = g_lower_pl.decl_other_cap > 0 ? g_lower_pl.decl_other_cap * 2 : 256;
            g_lower_pl.decl_other_name = (const char **) ct_grow((void *) g_lower_pl.decl_other_name, (size_t) nc * sizeof(const char *));
            g_lower_pl.decl_other_arity = (int *) ct_grow((void *) g_lower_pl.decl_other_arity, (size_t) nc * sizeof(int));
            g_lower_pl.decl_other_cap = nc;
        }
        g_lower_pl.decl_other_name[g_lower_pl.decl_other_n] = spec->c[0]->v.sval;
        g_lower_pl.decl_other_arity[g_lower_pl.decl_other_n] = (int) spec->c[1]->v.ival;
        g_lower_pl.decl_other_n++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_var_name(int slot) {
    if (slot >= 0 && slot < 1024) { if (!g_lower_pl.var_name_cache[slot]) g_lower_pl.var_name_cache[slot] = ct_fmt("G%d", slot); return g_lower_pl.var_name_cache[slot]; }
    return ct_fmt("G%d", slot);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_pi_name(const char * nm, int ar) { if (!nm) return ct_strdup("?"); return ct_fmt("%s/%d", nm, ar); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * term_lval_e(lcx_t * cx, const tree_t * t, IR_t ** entry_out) {
    if (entry_out) *entry_out = NULL;
    if (t && t->t == TT_VAR) { IR_t * nd = lpl_build(cx, IR_VAR_REF, NULL, cx->tω); IR_LIT(nd).sval = pl_vname(cx, (int) t->v.ival); return nd; }
    return term_e(cx, t, entry_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * pl_param_name(int i) {
    if (i < 0) return ct_fmt("A%d", i);
    cv_reserve(&g_lower_pl.param_name_cache, (uint32_t) sizeof(char *), (uint64_t) i + 1, "pl_param_name");
    if (!CV_AT(g_lower_pl.param_name_cache, char *, i)) CV_AT(g_lower_pl.param_name_cache, char *, i) = ct_fmt("A%d", i);
    return CV_AT(g_lower_pl.param_name_cache, char *, i);
}
const char * pl_vname(const lcx_t * cx, int slot) { if (cx && slot >= 0 && slot < cx->nvalias && cx->valias[slot]) return pl_param_name(cx->valias[slot] - 1); return pl_var_name(slot); }
int pl_seal_branch_tail(int tail, int pop, IR_t * nd) {
    if (!tail || !nd || nd->op != IR_CALL_PROC_STAGED || pl_trace_wanted()) return 0;
    nd->seal = pop ? PL_SEAL_TAIL_POP : PL_SEAL_TAIL;
    return pop;
}
static void pl_register_releases(lcx_t * cx, int from) {
    int to = cx->g->n;
    for (int k = from; k < to; k++) {
        IR_t * x = cx->g->all[k];
        if (!x) continue;
        if (x->op == IR_BOUND) ir_construct_add(cx->g, IR_CK_BOUND, x, (IR_t *) 0, k, to);
        if (x->op == IR_CUT && IR_LIT(x).ival == 0) ir_construct_add(cx->g, IR_CK_CUT, x, (IR_t *) 0, from, to);
    }
}
int pl_scc_armed(void) { extern int rt_proc_jmp_entry(const char *); return (pl_file_defines("$scc_cut", 0) || rt_proc_jmp_entry("$scc_cut/0")); }
int emit_pl_scc_armed(void) { return pl_scc_armed(); }
int pl_xsb_attv_armed(const char * cur_key) {
    extern int rt_proc_jmp_entry(const char *);
    return g_stage2_pl.pl_dialect == PL_DIALECT_XSB && cur_key && cur_key[0] != '$' && (pl_file_defines("$xsb_attv_clause", 2) || rt_proc_jmp_entry("$xsb_attv_clause/2"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_trace_wanted(void) { return g_runtime.abi.trace_budget != 0; }
static int pl_trace_is_control(const tree_t * t, int n, const char * f) { return t && t->t == TT_FNC && t->n == n && t->v.sval && !strcmp(t->v.sval, f); }
static void pl_trace_number_goals(const tree_t * t) {
    if (!t) return;
    if (pl_trace_is_control(t, 2, ",") || pl_trace_is_control(t, 2, ";") || pl_trace_is_control(t, 2, "->") || pl_trace_is_control(t, 2, "*->")) {
        pl_trace_number_goals(t->c[0]);
        pl_trace_number_goals(t->c[1]);
        return;
    }
    if (pl_trace_is_control(t, 1, "\\+") || pl_trace_is_control(t, 1, "call") || pl_trace_is_control(t, 1, "once")) { pl_trace_number_goals(t->c[0]); return; }
    if ((t->t == TT_FNC || t->t == TT_QLIT || t->t == TT_NAME || t->t == TT_CUT) && t->line <= 0) ((tree_t *) t)->line = (int) ++g_lower_pl.trace_stmtno;
}
static IR_t * pl_trace_stmt_wrap(lcx_t * cx, long line, IR_t * entry, IR_t * ωfail) {
    if (line <= 0 || !pl_trace_wanted()) return entry;
    IR_t * call = lpl_build(cx, IR_CALL, entry, ωfail);
    IR_LIT(call).sval = "__trace_stmt";
    IR_t * lit = lpl_build(cx, IR_LIT_INTEGER, call, ωfail);
    IR_LIT(lit).ival = line;
    ir_operand_push(call, lit);
    return lit;
}
static IR_t * pl_trace_named_wrap(lcx_t * cx, const char * hook, const char * name, IR_t * entry, IR_t * ωfail) {
    if (!name || !*name || !pl_trace_wanted()) return entry;
    IR_t * call = lpl_build(cx, IR_CALL, entry, ωfail);
    IR_LIT(call).sval = (char *) hook;
    IR_t * nm = lpl_build(cx, IR_LIT_STRING, call, ωfail);
    IR_LIT(nm).sval = (char *) name;
    ir_operand_push(call, nm);
    return nm;
}
IR_t * pl_leaf(lcx_t * cx, const char * sym, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = sym;
    IR_t * prev = NULL;
    IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL;
        IR_t * a = term_e(cx, t->c[i], &ae);
        IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en);
        else first = en;
        lc_ω_to(a, ωfail);
        prev = a;
        ir_operand_push(nd, a);
    }
    if (prev) lc_γ_to(prev, nd);
    { IR_t * fe = first ? first : nd; if (entry_out) *entry_out = fe; }
    return nd;
}
const pl_det_leaf_t pl_det_leaves[] = { { "var", 1, "$var" }, { "nonvar", 1, "$nonvar" }, { "atom", 1, "$atom" }, { "number", 1, "$number" }, { "integer", 1, "$integer" }, { "float", 1,
    "$float" }, { "atomic", 1, "$atomic" }, { "string", 1, "$string" }, { "compound", 1, "$compound" }, { "callable", 1, "$callable" }, { "ground", 1, "$ground" }, { "is_list", 1, "$is_list" },
    { "acyclic_term", 1, "$acyclic_term" }, { "==", 2, "$atop_eq" }, { "\\==", 2, "$atop_ne" }, { "@<", 2, "$atop_lt" }, { "@=<", 2, "$atop_le" }, { "@>", 2, "$atop_gt" }, { "@>=", 2, "$atop_ge" },
    { "compare", 3, "$compare" }, { "functor", 3, "$functor" }, { "arg", 3, "$arg" }, { "=..", 2, "$univ" }, { "copy_term", 2, "$copy_term" }, { "$copy_term_nat", 2, "$copy_term_nat" },
    { "term_variables", 2, "$term_variables" }, { "numbervars", 3, "$numbervars3" }, { "numbervars", 1, "$numbervars1" }, { "succ", 2, "$succ" }, { "$skip_list", 3, "$skip_list" }, { "$seek_list", 4,
    "$seek_list" }, { "$between_special", 2, "$between_special" }, { "$argv", 1, "$argv" }, { "$scc_bnew", 1, "$scc_bnew" }, { "$scc_take", 2, "$scc_take" }, { "$attv_set", 2, "$attv_set" },
    { "setarg", 3, "$setarg" }, { "nb_setarg", 3, "$nb_setarg" }, { "nb_linkarg", 3, "$nb_linkarg" }, { "stamp_date_time", 3, "$stamp_date_time" }, { "date_time_stamp", 2, "$date_time_stamp" },
    { "$format_time_text", 3, "$format_time_text" }, { "$create_prolog_flag", 4, "$create_prolog_flag" }, { "$flag_meta", 3, "$flag_meta" }, { "$attv_atts", 2, "$attv_atts" }, { "$attv_take", 1,
    "$attv_take" }, { "$unifiable", 3, "$unifiable" }, { "plus", 3, "$plus" }, { "sort", 2, "$sort" }, { "msort", 2, "$msort" }, { "char_type", 2, "$char_type" }, { "$char_type_d", 2, "$char_type" },
    { "term_string", 2, "$term_string" }, { "term_to_atom", 2, "$term_to_atom" }, { "atom_length", 2, "$atom_length" }, { "atom_concat", 3, "$atom_concat" }, { "atom_chars", 2, "$atom_chars" },
    { "atom_codes", 2, "$atom_codes" }, { "atom_number", 2, "$atom_number" }, { "atom_string", 2, "$atom_string" }, { "upcase_atom", 2, "$upcase_atom" }, { "downcase_atom", 2, "$downcase_atom" },
    { "string_concat", 3, "$string_concat" }, { "$string_concat_det", 3, "$string_concat" }, { "string_length", 2, "$string_length" }, { "string_lower", 2, "$string_lower" }, { "string_upper", 2,
    "$string_upper" }, { "string_to_atom", 2, "$string_to_atom" }, { "number_string", 2, "$number_string" }, { "string_chars", 2, "$string_chars" }, { "string_codes", 2, "$string_codes" },
    { "atomic_concat", 3, "$atomic_concat" }, { "atomic_list_concat", 2, "$atomic_list_concat" }, { "atomic_list_concat", 3, "$atomic_list_concat" }, { "concat_atom", 2, "$concat_atom" },
    { "concat_atom", 3, "$concat_atom" }, { "char_code", 2, "$char_code" }, { "number_codes", 2, "$number_codes" }, { "number_chars", 2, "$number_chars" }, { "name", 2, "$name" }, { "get_char", 1,
    "$get_char" }, { "peek_char", 1, "$peek_char" }, { "get_code", 1, "$get_code" }, { "peek_code", 1, "$peek_code" }, { "get_byte", 1, "$get_byte" }, { "peek_byte", 1, "$peek_byte" }, { "put_code",
    1, "$put_code" }, { "put_byte", 1, "$put_byte" }, { "unget_char", 1, "$unget_char" }, { "at_end_of_stream", 0, "$at_end_of_stream" }, { "current_prolog_flag", 2, "$current_prolog_flag" },
    { "set_prolog_flag", 2, "$set_prolog_flag" }, { "telling", 1, "$telling" }, { "seeing", 1, "$seeing" }, { "tell", 1, "$tell" }, { "append", 1, "$append1" }, { "see", 1, "$see" }, { "told", 0,
    "$told" }, { "seen", 0, "$seen" }, { "at_end_of_stream", 1, "$at_end_of_stream_s" }, { "put", 1, "$put_edin" }, { "get0", 1, "$get_code" }, { "get", 1, "$get_edin" }, { "skip", 1, "$skip" },
    { "unget_code", 1, "$unget_code" }, { "unget_byte", 1, "$unget_byte" }, { "get_code", 2, "$get_code_s" }, { "peek_code", 2, "$peek_code_s" }, { "get_byte", 2, "$get_byte_s" }, { "peek_byte", 2,
    "$peek_byte_s" }, { "put_code", 2, "$put_code_s" }, { "put_byte", 2, "$put_byte_s" }, { "unget_char", 2, "$unget_char_s" }, { "unget_code", 2, "$unget_code_s" }, { "unget_byte", 2,
    "$unget_byte_s" }, { "read", 1, "$read" }, { "atom_to_term", 3, "$atom_to_term" }, { "read_term_from_atom", 3, "$read_term_from_atom" }, { "read_term_from_chars", 3, "$read_term_from_chars" },
    { "read_term_from_codes", 3, "$read_term_from_codes" }, { "writeq", 1, "$writeq" }, { "print", 1, "$print" }, { "write_term", 2, "$write_term" }, { "write_term", 3, "$write_term_s" },
    { "write_canonical", 1, "$write_canonical" }, { "writeln", 1, "$writeln" }, { "display", 1, "$display" }, { "display", 2, "$display_s" }, { "unify_with_occurs_check", 2, "$unify_oc" },
    { "$aggregate_reduce", 3, "$aggregate_reduce" }, { "$wot_open", 3, "$wot_open" }, { "$wot_capture", 4, "$wot_capture" }, { "$wot_discard", 3, "$wot_discard" }, { "$set_prolog_flag_declare", 2,
    "$set_prolog_flag_declare" }, { "put_char", 1, "$put_char" }, { "$db_t_guard", 2, "$db_t_guard" }, { "$db_assertz_t", 3, "$db_assertz_t" }, { "$db_consult_t", 3, "$db_consult_t" },
    { "$db_consult_done", 1, "$db_consult_done" }, { "$db_asserta_t", 1, "$db_asserta_t" }, { "$db_abolish_t", 1, "$db_abolish_t" }, { "$db_retractall_t", 1, "$db_retractall_t" }, { "$db_seed_once",
    5, "$db_seed_once" }, { "$db_asserta_r", 2, "$db_asserta_r" }, { "$db_assertz_r", 2, "$db_assertz_r" }, { "$db_erase_ref", 1, "$db_erase_ref" }, { "$db_n_r", 2, "$db_n_r" }, { "$db_at_r", 3,
    "$db_at_r" }, { "$db_ref_r", 3, "$db_ref_r" }, { "$pl_declared", 2, "$pl_declared" }, { "$pl_dynamic", 1, "$pl_dynamic" }, { "$pl_list_guard", 1, "$pl_list_guard" }, { "$pl_op_check", 3,
    "$pl_op_check" }, { "$heap_guard", 2, "$heap_guard" }, { "$cutcall", 2, "$cutcall" }, { "$shc", 1, "$shc" }, { "$db_clause_src", 3, "$db_clause_src" }, { "$db_unload_file", 1, "$db_unload_file" },
    { "$pl_ioarg", 2, "$pl_ioarg" }, { "$alarm_push", 2, "$alarm_push" }, { "$alarm_pop", 1, "$alarm_pop" }, { "$os_sleep", 1, "$os_sleep" }, { "$sk_socket", 2, "$sk_socket" }, { "$sk_close", 2,
    "$sk_close" }, { "$sk_bind", 3, "$sk_bind" }, { "$sk_listen", 3, "$sk_listen" }, { "$sk_accept", 2, "$sk_accept" }, { "$sk_connect", 3, "$sk_connect" }, { "$sk_open", 2, "$sk_open" },
    { "$sk_setopt", 4, "$sk_setopt" }, { "$sk_fileno", 2, "$sk_fileno" }, { "$sk_hostname", 1, "$sk_hostname" }, { "$sk_resolve", 2, "$sk_resolve" }, { "$proc_create", 7, "$proc_create" },
    { "$proc_wait", 3, "$proc_wait" }, { "$proc_kill", 3, "$proc_kill" }, { "$fx_stat", 2, "$fx_stat" }, { "$fx_utime", 4, "$fx_utime" }, { "$fx_symlink", 3, "$fx_symlink" }, { "$fx_link", 3,
    "$fx_link" }, { "$fx_chmod", 3, "$fx_chmod" }, { "$fx_readlink", 2, "$fx_readlink" }, { "$put_code", 1, "$put_code" }, { "$put_code_s", 2, "$put_code_s" }, { "$put_char", 1, "$put_char" },
    { "$put_char_c_s", 2, "$put_char_c_s" }, { "$get_code", 1, "$get_code" }, { "$get_code_s", 2, "$get_code_s" }, { "$peek_code", 1, "$peek_code" }, { "$peek_code_s", 2, "$peek_code_s" },
    { "$get_char", 1, "$get_char" }, { "$get_char_s", 2, "$get_char_s" }, { "$peek_char", 1, "$peek_char" }, { "$peek_char_s", 2, "$peek_char_s" }, { "$get_byte", 1, "$get_byte" }, { "$get_byte_s", 2,
    "$get_byte_s" }, { "$peek_byte", 1, "$peek_byte" }, { "$peek_byte_s", 2, "$peek_byte_s" }, { "$get_edin", 1, "$get_edin" }, { "$put_edin", 1, "$put_edin" }, { "$skip", 1, "$skip" },
    { "$current_prolog_flag", 2, "$current_prolog_flag" }, { "$pl_sp_check", 2, "$pl_sp_check" }, { "$pl_goal_guard", 1, "$pl_goal_guard" }, { "$pl_cp_count", 1, "$pl_cp_count" }, { "$mod_get", 3,
    "$mod_get" }, { "$mod_set", 3, "$mod_set" }, { "$co_start", 4, "$co_start" }, { "$co_resume", 5, "$co_resume" }, { "$co_yield", 2, "$co_yield" }, { "$co_unwound", 2, "$co_unwound" },
    { "$co_finish", 2, "$co_finish" }, { "$co_mark", 0, "$co_mark" }, { "$co_b0", 0, "$co_b0" }, { "$co_inside", 0, "$co_inside" }, { "$engine_new", 4, "$engine_new" }, { "$engine_next", 2,
    "$engine_next" }, { "$engine_yield", 1, "$engine_yield" }, { "$engine_destroy", 1, "$engine_destroy" }, { "$engine_self", 1, "$engine_self" }, { "$engine_id", 2, "$engine_id" }, { "$engine_list",
    1, "$engine_list" }, { "$engine_reaped", 1, "$engine_reaped" }, { "$engine_stats", 4, "$engine_stats" }, { "$gc_now", 0, "$gc_now" }, { "$trie_new", 1, "$trie_new" }, { "$trie_alive", 1,
    "$trie_alive" }, { "$trie_destroy", 1, "$trie_destroy" }, { "$trie_insert", 5, "$trie_insert" }, { "$trie_lookup", 3, "$trie_lookup" }, { "$trie_delete", 3, "$trie_delete" }, { "$trie_entries", 2,
    "$trie_entries" }, { "$trie_entry_key", 3, "$trie_entry_key" }, { "$trie_count", 2, "$trie_count" }, { "$trie_meta_set", 3, "$trie_meta_set" }, { "$trie_meta_get", 3, "$trie_meta_get" },
    { "$tbl_vt", 1, "$tbl_vt" }, { "$tbh_close", 0, "$tbh_close" }, { "$ssu_begin", 1, "$ssu_begin" }, { "$ssu_check", 1, "$ssu_check" }, { "$tbl_park", 1, "$tbl_park" }, { "$tbl_sl", 3, "$tbl_sl" },
    { "$tbl_resume_all", 1, "$tbl_resume_all" }, { "$tbh_leading", 0, "$tbh_leading" }, { "$tbh_get", 2, "$tbh_get" }, { "$tbh_own", 2, "$tbh_own" }, { "$tbh_set", 2, "$tbh_set" }, { "$tbh_bset", 2,
    "$tbh_bset" }, { "$tbl_dlget", 1, "$tbl_dlget" }, { "$tbl_dlset", 1, "$tbl_dlset" }, { "$tx_db", 2, "$tx_db" }, { "$mod_ext", 4, "$mod_ext" }, { "$pl_cp_nth", 3, "$pl_cp_nth" }, { "$pl_pp_guard",
    1, "$pl_pp_guard" }, { "$pl_pp_count", 2, "$pl_pp_count" }, { "$pl_pp_nth", 3, "$pl_pp_nth" }, { "$pl_cp_guard", 1, "$pl_cp_guard" }, { "halt", 0, "$halt" }, { "halt", 1, "$halt" },
    { "flush_output", 0, "$flush_output" }, { "format", 1, "$format" }, { "format", 2, "$format" }, { "write", 2, "$write_s" }, { "writeq", 2, "$writeq_s" }, { "print", 2, "$print_s" },
    { "write_canonical", 2, "$write_canonical_s" }, { "writeln", 2, "$writeln_s" }, { "nl", 1, "$nl_s" }, { "put_char", 2, "$put_char_c_s" }, { "flush_output", 1, "$flush_output_s" }, { "format", 3,
    "$format3" }, { "read", 2, "$read_s" }, { "get_char", 2, "$get_char_s" }, { "peek_char", 2, "$peek_char_s" }, { "open", 3, "$open" }, { "open", 4, "$open4" }, { "close", 1, "$close" }, { "close",
    2, "$close" }, { "current_output", 1, "$current_output" }, { "current_input", 1, "$current_input" }, { "set_output", 1, "$set_output" }, { "set_input", 1, "$set_input" }, { "keysort", 2,
    "$keysort" }, { "set_stream_position", 2, "$set_stream_position" }, { "op", 3, "$op" }, { "$pl_op_count", 1, "$pl_op_count" }, { "$pl_op_nth", 4, "$pl_op_nth" }, { "$pl_sp_count", 1,
    "$pl_sp_count" }, { "$pl_sp_nth", 3, "$pl_sp_nth" }, { "$pl_cs_count", 1, "$pl_cs_count" }, { "$pl_cs_nth", 4, "$pl_cs_nth" }, { "wall_us", 1, "$wall_us" }, { "wall_ms", 1, "$wall_ms" }, { "sort",
    1, "$gnu_sort1" }, { "msort", 1, "$gnu_msort1" }, { "keysort", 1, "$gnu_keysort1" }, { "line_count", 2, "$gnu_line_count" }, { "line_position", 2, "$gnu_line_position" }, { "$put_token", 2,
    "$put_token" }, { "$sha_hash", 3, "$sha_hash" }, { "character_count", 2, "$gnu_character_count" }, { "stream_line_column", 3, "$gnu_stream_line_column" }, { "last_read_start_line_column", 2,
    "$gnu_last_read_start" }, { "absolute_file_name", 2, "$gnu_absolute_file_name" }, { "prolog_file_name", 2, "$gnu_prolog_file_name" }, { "$gnu_builtin", 2, "$gnu_builtin" }, { "working_directory",
    1, "$gnu_working_directory" }, { "change_directory", 1, "$gnu_change_directory" }, { "make_directory", 1, "$gnu_make_directory" }, { "$gnu_os", 3, "$gnu_os" }, { "$compound0", 3, "$compound0" },
    { "set_stream", 2, "$set_stream" }, { "delete_file", 1, "$gnu_delete_file" }, { "file_exists", 1, "$gnu_file_exists" }, { "directory_files", 2, "$gnu_directory_files" }, { "term_hash", 2,
    "$gnu_term_hash" }, { "$gnu_term_hash4", 4, "$gnu_term_hash4" }, { "prolog_pid", 1, "$gnu_prolog_pid" }, { "pipe", 2, "$pipe" }, { "open_null_stream", 1, "$open_null_stream" },
    { "$gnu_environ_list", 1, "$gnu_environ_list" }, { "$gnu_file_props", 2, "$gnu_file_props" }, { "$trie_slice", 4, "$trie_slice" }, { "$cpu_ms", 1, "$cpu_ms" }, { "$crv_enter", 1, "$crv_enter" },
    { "$crv_adj", 1, "$crv_adj" }, { "$crv_collect", 2, "$crv_collect" }, { "$copy_keep", 4, "$copy_keep" }, { "$pl_backtrace", 2, "$pl_backtrace" }, { "$factorize_term", 3, "$factorize_term" },
    { "rational", 1, "$rational" }, { "$undo_push", 1, "$undo_push" }, { "$undo_take", 1, "$undo_take" }, { "", 0, "" } };
int pl_det_leaf_name_wired(const char * nm) { for (int i = 0; pl_det_leaves[i].nm[0]; i++) if (!strcmp(nm, pl_det_leaves[i].nm)) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * pl_det_leaf_sym(const char * nm, int ar) {
    for (int i = 0; pl_det_leaves[i].nm[0]; i++) if (pl_det_leaves[i].ar == ar && !strcmp(nm, pl_det_leaves[i].nm)) return pl_det_leaves[i].sym;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const struct {
    const char * nm;
    int ar;
    const char * gsym;
} pl_anum_guards[] = { { "atom_length", 2, "$pl_anum_guard2" }, { "atom_chars", 2, "$pl_anum_guard2" }, { "atom_codes", 2, "$pl_anum_guard2" }, { "char_code", 2, "$pl_anum_guard2" }, { "number_chars",
    2, "$pl_anum_guard2" }, { "number_codes", 2, "$pl_anum_guard2" }, { "number_string", 2, "$pl_anum_guard2" }, { "atom_concat", 3, "$pl_anum_guard3" }, { "atomic_concat", 3, "$pl_anum_guard3" },
    { "atomic_list_concat", 2, "$pl_anum_guard2" }, { "atomic_list_concat", 3, "$pl_anum_guard3" }, { "arg", 3, "$pl_anum_guard3" }, { "functor", 3, "$pl_anum_guard3" }, { 0, 0, 0 } };
const char * pl_anum_guard_sym(const char * nm, int ar) {
    for (int i = 0; pl_anum_guards[i].nm; i++) if (pl_anum_guards[i].ar == ar && !strcmp(nm, pl_anum_guards[i].nm)) return pl_anum_guards[i].gsym;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_leaf_lv_guarded(lcx_t * cx, const char * sym, const char * gsym, const char * gname, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    IR_t * nd = lpl_build(cx, IR_CALL, γnext, ωfail);
    IR_LIT(nd).sval = sym;
    IR_t * chk = lpl_build(cx, IR_CALL, nd, ωfail);
    IR_LIT(chk).sval = gsym;
    IR_t * nl = lpl_build(cx, IR_LIT_STRING, chk, ωfail);
    IR_LIT(nl).sval = (char *) gname;
    ir_operand_push(chk, nl);
    IR_t * prev = NULL;
    IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL;
        IR_t * a = term_lval_e(cx, t->c[i], &ae);
        IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en);
        else first = en;
        lc_ω_to(a, ωfail);
        prev = a;
        ir_operand_push(nd, a);
        ir_operand_push(chk, a);
    }
    if (prev) lc_γ_to(prev, nl);
    if (entry_out) *entry_out = first ? first : nl;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_file_defines(const char * nm, int ar) {
    char key[fmt_len("%s/%d", nm, ar)];
    snprintf(key, sizeof key, "%s/%d", nm, ar);
    const tree_t * ch = resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
    return (ch && ch->t == TT_CHOICE && ch->n > 0) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * pl_pred_file(const char * nm, int ar) {
    char key[fmt_len("%s/%d", nm, ar)];
    snprintf(key, sizeof key, "%s/%d", nm, ar);
    for (Resolve_PredEntry * e = g_stage2.resolve_pred_table.buckets[resolve_pred_hash(key)]; e; e = e->next) {
        if (strcmp(e->key, key)) continue;
        return (e->choice && e->choice->t == TT_CHOICE && e->choice->n > 0 && e->choice->c[0] && e->choice->c[0]->line > 0 && e->file && e->file[0] != '<') ? e->file : (const char *) 0;
    }
    return (const char *) 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_rt_registered_static(const char * nm, int ar) {
    extern int rt_proc_is_registered(const char *);
    extern int rt_pl_db_key_is_dynamic(void *, const char *);
    char key[fmt_len("%s/%d", nm, ar)];
    snprintf(key, sizeof key, "%s/%d", nm, ar);
    return g_stage2.rt_lowering && g_stage2.rt_root && rt_proc_is_registered(key) && !rt_pl_db_key_is_dynamic(g_stage2.rt_root, key);
}
int pl_user_file_defines(const char * nm, int ar) {
    extern int pl_prelude_defines(const char *, int);
    return pl_pred_file(nm, ar) || (pl_rt_registered_static(nm, ar) && !pl_prelude_defines(nm, ar));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_rt_creatable(const char * nm, int ar) {
    extern int rt_pl_db_key_is_static(void *, const char *);
    char key[fmt_len("%s/%d", nm, ar)];
    if (!g_stage2.rt_lowering || !g_stage2.rt_root || pl_file_defines(nm, ar)) return 0;
    snprintf(key, sizeof key, "%s/%d", nm, ar);
    return !rt_pl_db_key_is_static(g_stage2.rt_root, key);
}
int pl_db_owned(const char * nm, int ar) {
    if (pl_rt_is_dynamic(nm, ar)) return 1;
    if (pl_decl_dyn_is(nm, ar)) return 1;
    if (pl_dyn_index(nm, ar) < 0) return 0;
    return pl_file_defines(nm, ar) ? 0 : 1;
}
int pl_db_writable(const char * nm, int ar) { return pl_db_owned(nm, ar) || pl_rt_creatable(nm, ar); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_pi_is_static_builtin(const char * pn, int ar) {
    if (!pn) return 0;
    if (pl_decl_dyn_is(pn, ar)) return 0;
    if (pl_pi_is_control(pn, ar)) return 1;
    if (pl_det_leaf_sym(pn, ar)) return 1;
    if (pl_rung_of(pn) && strcmp(pn, "for")) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_pi_is_builtin(const char * pn, int ar) { return pl_pi_is_static_builtin(pn, ar); }
int pl_tree_has_var(const tree_t * t) { if (!t) return 0; if (t->t == TT_VAR) return 1; for (int i = 0; i < t->n; i++) if (pl_tree_has_var(t->c[i])) return 1; return 0; }
int pl_phrase_rt_checked(void) { extern int rt_proc_is_registered(const char *); return g_stage2.rt_lowering && rt_proc_is_registered(pl_pi_name("$phrase_rt", 3)); }
static int pl_callee_static(lcx_t * cx, const char * nm, int ar, const char * key, int db_live, int defined) {
    if (g_stage2.rt_lowering || db_live || pl_decl_dyn_is(nm, ar)) return 0;
    if (defined) return 1;
    return (cx && cx->cur_key && strcmp(cx->cur_key, key) == 0) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * pl_user_call(lcx_t * cx, const char * nm, const tree_t * t, int nargs, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    int pl_db_live = pl_db_owned(nm, nargs);
    int st = 0;
    if (pl_db_live && g_stage2.rt_lowering) {
        extern int rt_proc_is_registered(const char *);
        if (!rt_proc_is_registered(pl_pi_name(nm, nargs))) return pl_meta_call_dyn(cx, t, NULL, 0, γnext, ωfail, entry_out);
    }
    {
        char key[fmt_len("%s/%d", nm, nargs)];
        snprintf(key, sizeof key, "%s/%d", nm, nargs);
        const tree_t * ch = pl_db_live ? (const tree_t *) 0 : resolve_pred_table_lookup(&g_stage2.resolve_pred_table, key);
        int file_def = ch && ch->t == TT_CHOICE && ch->n > 0;
        int bbhit = (pl_db_live || file_def) ? 0 : (pl_bb_lookup(key, nargs) != 0);
        if (!ch && !pl_db_live && !bbhit) {
            const char * ck = cx ? cx->cur_key : (const char *) 0;
            const char * colon = (ck && !strncmp(ck, "plunit_", 7)) ? strchr(ck, ':') : (const char *) 0;
            if (colon && (g_stage2_pl.pl_dialect == PL_DIALECT_SWI) && !(cx->umod && !cx->umod[0]) && strcmp(nm, "$mctx_call")) {
                tree_t * mq = ast_node_new(TT_QLIT);
                mq->v.sval = cx->umod ? (char *) cx->umod : ct_strndup(ck, (size_t) (colon - ck));
                return goal(cx, pl_cc_fnc2("$mctx_call", mq, (tree_t *) t), γnext, ωfail, entry_out);
            }
            if (cx && cx->umod && cx->umod[0] && g_stage2_pl.pl_dialect == PL_DIALECT_SWI && strcmp(nm, "$mod_callq")) {
                const char * sv = cx->umod;
                IR_t * qr;
                cx->umod = "";
                qr = goal(cx, pl_cc_fnc2("$mod_callq", (tree_t *) pl_atom_goal(sv), (tree_t *) t), γnext, ωfail, entry_out);
                cx->umod = sv;
                return qr;
            }
            return pl_meta_call_dyn(cx, t, NULL, 0, γnext, ωfail, entry_out);
        }
        if (ch && ch->t == TT_FNC && !g_stage2.rt_lowering) { IR_t * nd = lpl_build(cx, IR_GOTO, ωfail, ωfail); if (entry_out) *entry_out = nd; return nd; }
        st = pl_callee_static(cx, nm, nargs, key, pl_db_live, file_def || bbhit);
    }
    IR_t * nd = lpl_build(cx, IR_CALL_PROC_STAGED, γnext, ωfail);
    IR_LIT(nd).sval = pl_pi_name(nm, nargs);
    nd->strict = st ? PL_CALL_STATIC : 0;
    IR_t * prev = NULL;
    IR_t * first = NULL;
    for (int i = 0; i < nargs; i++) {
        IR_t * ae = NULL;
        IR_t * a = term_lval_e(cx, t->c[i], &ae);
        IR_t * en = ae ? ae : a;
        if (prev) lc_γ_to(prev, en);
        else first = en;
        lc_ω_to(a, ωfail);
        prev = a;
        ir_operand_push(nd, a);
    }
    if (prev) lc_γ_to(prev, nd);
    IR_t * body_entry = first ? first : nd;
    if (pl_db_live) {
        tree_t * pit = ast_node_new(TT_QLIT);
        pit->v.sval = (char *) pl_pi_name(nm, nargs);
        IR_t * ne = NULL;
        pl_db_leaf1(cx, "$db_nonempty", pl_dyn_index_or_add(nm, nargs), body_entry, ωfail, &ne);
        IR_t * guard_entry = NULL;
        pl_db_leaf2(cx, "$db_alive", pl_dyn_index_or_add(nm, nargs), pit, ne, ωfail, &guard_entry);
        if (g_stage2.rt_lowering) {
            if (entry_out) *entry_out = guard_entry;
        } else {
            IR_t * se = NULL;
            pl_db_seed_file(cx, nm, nargs, guard_entry, ωfail, &se);
            if (entry_out) *entry_out = se ? se : guard_entry;
        }
        return nd;
    }
    if (entry_out) *entry_out = body_entry;
    return nd;
}
static int pl_trace_stmt_control(const tree_t * t) {
    if (!t) return 1;
    if (t->t == TT_PROGRAM || t->t == TT_MAKELIST || t->t == TT_IF) return 1;
    if (t->t != TT_FNC || t->n != 2 || !t->v.sval) return 0;
    return !strcmp(t->v.sval, ",") || !strcmp(t->v.sval, ";") || !strcmp(t->v.sval, "|") || !strcmp(t->v.sval, "->") || !strcmp(t->v.sval, "*->");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_dl_prelude_key(const lcx_t * cx) {
    extern int pl_prelude_defines(const char *, int);
    const char * sl = cx->cur_key ? strrchr(cx->cur_key, '/') : (const char *) 0;
    if (!sl || sl == cx->cur_key) return 0;
    char kn[sl - cx->cur_key + 1];
    memcpy(kn, cx->cur_key, (size_t)(sl - cx->cur_key));
    kn[sl - cx->cur_key] = 0;
    return pl_prelude_defines(kn, atoi(sl + 1)) && !pl_file_defines(kn, atoi(sl + 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_dl_ctl_key(const char * key) {
    static const char * const pre[] = { "$cc", "$mc:,/", "$mc:;/", "$mc:->/", "$mc:*->/", "$mc:\\+/", "$mc:|/", ",/2", ";/2", "->/2", "*->/2", "$conj_nocut/1", "$fc/3", NULL };
    if (!key) return 0;
    for (int i = 0; pre[i]; i++) if (!strncmp(key, pre[i], strlen(pre[i]))) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_dl_wants_tick(const lcx_t * cx, const tree_t * t) {
    extern int pl_prelude_defines(const char *, int);
    static const char * const ctl[] = { ",", ";", "->", "*->", "\\+", "not", "call", "findall", "forall", "once", "ignore", "catch", "setup_call_cleanup", "call_cleanup", "true", "fail", ":", "bagof",
        "setof", NULL };
    const char * nm;
    int n;
    if (cx->cur_key && (!strncmp(cx->cur_key, "$dl_", 4) || !strncmp(cx->cur_key, "$mc:", 4) || !strcmp(cx->cur_key, "call_with_depth_limit/3"))) return 0;
    if (pl_dl_ctl_key(cx->cur_key) || pl_dl_prelude_key(cx)) return 0;
    if (t->t == TT_UNIFY) return !(t->n == 2 && t->c[0] && t->c[1] && (t->c[0]->t == TT_VAR || t->c[1]->t == TT_VAR));
    if (t->t != TT_FNC && t->t != TT_QLIT && t->t != TT_NAME) return 0;
    nm = t->v.sval;
    n = (t->t == TT_FNC) ? t->n : 0;
    if (!nm || nm[0] == '$') return 0;
    if (cx->cur_key) { char sk[fmt_len("%s/%d", nm, n)]; snprintf(sk, sizeof sk, "%s/%d", nm, n); if (!strcmp(cx->cur_key, sk)) return 0; }
    for (int i = 0; ctl[i]; i++) if (!strcmp(nm, ctl[i])) return 0;
    if (t->t == TT_FNC && n == 2 && (!strcmp(nm, "=") || !strcmp(nm, "==") || !strcmp(nm, "\\==")) && t->c[0] && t->c[1] && (t->c[0]->t == TT_VAR || t->c[1]->t == TT_VAR)) return 0;
    return pl_user_defines(nm, n) || pl_prelude_defines(nm, n) || pl_db_owned(nm, n) ? 0 : 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_dl_ctl_leaf_scope(const lcx_t * cx) { return g_stage2.depth_counted && pl_dl_ctl_key(cx->cur_key); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * goal(lcx_t * cx, const tree_t * t, IR_t * γnext, IR_t * ωfail, IR_t ** entry_out) {
    if (t && g_stage2.depth_counted && cx->dl_skip != t && pl_dl_wants_tick(cx, t)) {
        cx->dl_skip = t;
        return goal(cx, pl_cc_fnc2(",", pl_cc_fnc1("$dl_tick", pl_cc_ilit(0)), (tree_t *) t), γnext, ωfail, entry_out);
    }
    if (cx->dl_skip == t) cx->dl_skip = NULL;
    if (!t || !pl_trace_wanted() || cx->stmt_depth > 0 || pl_trace_stmt_control(t)) return goal_inner(cx, t, γnext, ωfail, entry_out);
    IR_t * e = NULL;
    cx->stmt_depth++;
    IR_t * nd = goal_inner(cx, t, γnext, ωfail, &e);
    cx->stmt_depth--;
    IR_t * te = pl_trace_stmt_wrap(cx, (t->line > 0) ? (long) t->line : ++g_lower_pl.trace_stmtno, e ? e : nd, ωfail);
    if (entry_out) *entry_out = te;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_new_proc(const char * name, int nparams, int bb_idx) {
    int pi = stage2_proc_grow(&g_stage2);
    g_stage2.proc_table[pi].name = ct_strdup(name);
    g_stage2.proc_table[pi].proc = NULL;
    g_stage2.proc_table[pi].entry_pc = -1;
    g_stage2.proc_table[pi].bb_idx = bb_idx;
    g_stage2.proc_table[pi].nparams = nparams;
    g_stage2.proc_table[pi].is_generator = 1;
    {
        int mi = g_stage2.module_registry.nmod - 1;
        if (mi >= 0) { g_stage2.module_registry.mods[mi].nprocs++; if (!strcmp(name, "main") && g_stage2.module_registry.main_mod < 0) g_stage2.module_registry.main_mod = mi; }
    }
    return pi;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_graph_stamp(IR_graph_t * g, int arity, int maxlocal, const unsigned char * aliased, int naliased) {
    g->body_root = NULL;
    {
        extern int dop_direct_leaf_known(const char *, int);
        for (int i = 0; i < g->n; i++) {
            IR_t * nd = g->all[i];
            if (nd && nd->op == IR_CALL && nd->sval && nd->sval[0] == '$' && dop_direct_leaf_known(nd->sval, nd->n_operands)) nd->seal = IR_SEAL_CALL_DET_LEAF;
        }
    }
    g->nparams = arity;
    if (arity > 0) { g->pnames = (const char **) ct_zalloc((size_t) arity, sizeof(const char *)); for (int i = 0; i < arity; i++) g->pnames[i] = pl_param_name(i); }
    int nl = 0;
    if (maxlocal >= 0) {
        g->lnames = (const char **) ct_zalloc((size_t)(maxlocal + 1), sizeof(const char *));
        for (int k = 0; k <= maxlocal; k++) {
            const char * nm = pl_var_name(k);
            int used = !(aliased && k < naliased && aliased[k]);
            for (int i = 0; i < g->n && !used; i++) {
                const IR_t * nd = g->all[i];
                if (nd && (nd->op == IR_VAR || nd->op == IR_VAR_REF || nd->op == IR_UNIFY_FIRST || nd->op == IR_UNIFY_VALUE) && IR_LIT(nd).sval && !strcmp(IR_LIT(nd).sval, nm)) used = 1;
            }
            if (used) g->lnames[nl++] = nm;
        }
        g->nlocals = nl;
    }
    g->nslots = arity + nl + 8;
    g->resumable_callable = 1;
    g->unbound_serial = 1;
    g->depth_counted = g_stage2.depth_counted;
    g->deterministic = 1;
    g->home_frame = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_alt_alloc(IR_graph_t * g, int nc) {
    g->alt_entry = (IR_t **) ct_zalloc((size_t) nc, sizeof(IR_t *));
    g->alt_ret = (IR_t **) ct_zalloc((size_t) nc, sizeof(IR_t *));
    g->alt_redo = (IR_t **) ct_zalloc((size_t) nc, sizeof(IR_t *));
    g->alt_zhi = nc > 1 ? (int *) ct_zalloc((size_t)(nc + 1), sizeof(int)) : (int *) 0;
    g->n_alts = nc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * pl_body_graph(const tree_t * const * gl, int ng) {
    IR_graph_t * g = IR_alloc(4096);
    lcx_t cx;
    memset(&cx, 0, sizeof cx);
    cx.g = g;
    cx.tω = NULL;
    cx.cutω = NULL;
    cx.clause_cutω = NULL;
    cx.cut_scope = 0;
    cx.scope_seq = 0;
    cx.meta_redo = NULL;
    cx.meta_redo_set = 0;
    cx.tail = 0;
    cx.stmt_depth = 1;
    IR_t * succeed = lpl_build(&cx, IR_SUCCEED, NULL, NULL);
    IR_t * fail = lpl_build(&cx, IR_FAIL, NULL, NULL);
    IR_t * step = lpl_build(&cx, IR_FAIL, NULL, NULL);
    cx.cutω = fail;
    cx.clause_cutω = fail;
    IR_t * entry = NULL;
    IR_t * redo = NULL;
    IR_t * tnode = NULL;
    int maxlocal = -1;
    for (int i = 0; i < ng; i++) maxlocal = max_var_slot(gl[i], maxlocal);
    int fresh_saved = g_lower_pl.fresh_next;
    g_lower_pl.fresh_next = maxlocal + 1;
    if (pl_trace_wanted()) for (int i = 0; i < ng; i++) pl_trace_number_goals(gl[i]);
    cx.tail = !pl_trace_wanted();
    int rel_from = cx.g->n;
    IR_t * first = pl_lower_conj(&cx, gl, ng, succeed, step, &entry, &redo, &tnode);
    pl_register_releases(&cx, rel_from);
    if (tnode && tnode->op == IR_CALL_PROC_STAGED && !pl_trace_wanted()) tnode->seal = PL_SEAL_TAIL;
    maxlocal = g_lower_pl.fresh_next - 1;
    g_lower_pl.fresh_next = fresh_saved;
    g->entry = entry ? entry : (first ? first : succeed);
    pl_alt_alloc(g, 1);
    g->alt_entry[0] = g->entry;
    g->alt_ret[0] = succeed;
    g->alt_redo[0] = redo;
    g->alt_fail = step;
    pl_graph_stamp(g, 0, maxlocal, (const unsigned char *)0, 0);
    (void) fail;
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_key_dynamic(const char * key) {
    const char * slash = key ? strrchr(key, '/') : (const char *) 0;
    if (!slash || slash == key) return 0;
    size_t nlen = (size_t)(slash - key);
    char nm[nlen + 1];
    memcpy(nm, key, nlen);
    nm[nlen] = 0;
    return pl_dyn_is_marked(nm, atoi(slash + 1));
}
static int max_var_slot_below(const tree_t * t, int lim, int mx) {
    if (!t) return mx;
    if (t->t == TT_VAR && (int) t->v.ival < lim && (int) t->v.ival > mx) mx = (int) t->v.ival;
    for (int i = 0; i < t->n; i++) mx = max_var_slot_below(t->c[i], lim, mx);
    return mx;
}
static void pl_compact_fresh_slots(tree_t * t, long long * keys, int * n, int cap, int lim, int base) {
    if (!t) return;
    if (t->t == TT_VAR) {
        long long key = t->v.ival;
        if (key < lim) return;
        int slot = -1;
        for (int i = 0; i < *n; i++) if (keys[i] == key) { slot = i; break; }
        if (slot < 0 && *n < cap) { keys[*n] = key; slot = *n; (*n)++; }
        t->v.ival = base + (slot < 0 ? cap - 1 : slot);
        return;
    }
    for (int i = 0; i < t->n; i++) pl_compact_fresh_slots(t->c[i], keys, n, cap, lim, base);
}
static IR_graph_t * pl_pred_graph(const tree_t * ch, const char * key) {
    int nc = (ch->t == TT_CHOICE) ? ch->n : 1;
    int pop_on = !pl_trace_wanted() && !g_stage2.rt_lowering && !pl_key_dynamic(key);
    if (nc < 1) nc = 1;
    IR_graph_t * g = IR_alloc(1024 + 1024 * nc);
    lcx_t cx;
    memset(&cx, 0, sizeof cx);
    cx.g = g;
    cx.cur_key = key;
    cx.tω = NULL;
    cx.cutω = NULL;
    cx.clause_cutω = NULL;
    cx.cut_scope = 0;
    cx.scope_seq = 0;
    cx.meta_redo = NULL;
    cx.meta_redo_set = 0;
    cx.tail = 0;
    const char * trace_key = key ? ct_strdup(key) : NULL;
    {
        extern int pl_prelude_defines(const char *, int);
        const char * slash = key ? strrchr(key, '/') : NULL;
        if (slash) {
            size_t nl = (size_t)(slash - key);
            char nmbuf[nl + 1];
            memcpy(nmbuf, key, nl);
            nmbuf[nl] = 0;
            {
                int ar = atoi(slash + 1);
                if (pl_prelude_defines(nmbuf, ar) || pl_db_owned(nmbuf, ar) || (pl_pi_is_static_builtin(nmbuf, ar) && !pl_file_defines(nmbuf, ar))) { cx.stmt_depth = 1; trace_key = NULL; }
            }
        }
    }
    IR_t * step = lpl_build(&cx, IR_FAIL, NULL, NULL);
    cx.cutω = lpl_build(&cx, IR_FAIL, NULL, NULL);
    cx.clause_cutω = cx.cutω;
    pl_alt_alloc(g, nc);
    g->alt_fail = step;
    int arity = -1;
    int maxlocal = -1;
    int fresh_saved = g_lower_pl.fresh_next;
    for (int k = 0; k < nc; k++) { const tree_t * cl = (ch->t == TT_CHOICE) ? ch->c[k] : ch; if (cl) { int ml = max_var_slot(cl, -1); if (ml > maxlocal) maxlocal = ml; } }
    if (maxlocal >= 900000) {
        tree_t * cc = pl_tree_copy(ch);
        int real = -1;
        for (int k = 0; k < nc; k++) real = max_var_slot_below((cc->t == TT_CHOICE) ? cc->c[k] : cc, 900000, real);
        maxlocal = -1;
        for (int k = 0; k < nc; k++) {
            tree_t * cl = (cc->t == TT_CHOICE) ? cc->c[k] : cc;
            int occ = pl_tree_var_occ(cl) + 1;
            long long keys[occ];
            int nk = 0;
            pl_compact_fresh_slots(cl, keys, &nk, occ, 900000, real + 1);
            int ml = max_var_slot(cl, -1);
            if (ml > maxlocal) maxlocal = ml;
        }
        ch = cc;
    }
    int nva = maxlocal + 1 > 0 ? maxlocal + 1 : 1;
    int valias[nva];
    unsigned char aliased[nva];
    memset(aliased, 0, sizeof aliased);
    cx.valias = valias;
    cx.nvalias = nva;
    g_lower_pl.fresh_next = maxlocal + 1;
    for (int k = 0; k < nc; k++) {
        int clause_lo = g->n;
        const tree_t * cl = (ch->t == TT_CHOICE) ? ch->c[k] : ch;
        if (!cl || cl->t != TT_CLAUSE) pl_refuse("clause shape in", key, 2);
        int ar = (int) cl->v.dval;
        if (ar < 0) ar = 0;
        if (ar > cl->n) ar = cl->n;
        if (arity < 0) arity = ar;
        if (ar != arity) pl_refuse("clauses of differing arity in", key, 2);
        memset(valias, 0, sizeof valias);
        for (int i = 0; i < ar; i++) {
            const tree_t * a = cl->c[i];
            if (!a || a->t != TT_VAR) continue;
            int sl = (int) a->v.ival;
            if (sl < 0 || sl >= nva) continue;
            int seen = 0;
            for (int j = 0; j < i && !seen; j++) {
                int nvs = pl_var_nodes(cl->c[j]) + 1;
                int vs[nvs], nv = 0;
                pl_collect_vars(cl->c[j], vs, &nv, nvs);
                for (int q = 0; q < nv; q++) if (vs[q] == sl) seen = 1;
            }
            if (!seen) { valias[sl] = i + 1; aliased[sl] = 1; }
        }
        IR_t * succeed = lpl_build(&cx, IR_SUCCEED, NULL, NULL);
        IR_t * tret = pl_trace_named_wrap(&cx, "__trace_return", trace_key, succeed, step);
        IR_t * bentry = NULL;
        IR_t * redo = NULL;
        IR_t * tnode = NULL;
        if (pl_trace_wanted()) for (int i = ar; i < cl->n; i++) pl_trace_number_goals(cl->c[i]);
        int nbg = cl->n - ar + 1;
        const tree_t * bg[nbg > 0 ? nbg : 1];
        int nb = pl_attv_on() ? pl_attv_body(cl, ar, bg, nbg) : 0;
        cx.tail = !pl_trace_wanted();
        lc_vec pop_ok;
        lc_vec_init(&pop_ok, (int) sizeof(IR_t *));
        int pop_from = cx.g->n;
        cx.tail_pop = pop_on;
        cx.pop_ok = pop_on ? &pop_ok : (lc_vec *) 0;
        int kfirst[nva], klast[nva], kco[nva];
        if (!pl_attv_on() && !pl_trace_wanted()) {
            for (int v = 0; v < nva; v++) { kfirst[v] = -2; klast[v] = -2; kco[v] = cl->n; }
            for (int i = 0; i < ar; i++) pl_kill_scan(cl->c[i], -1, kfirst, klast, nva);
            for (int i = ar; i < cl->n; i++) pl_kill_scan(cl->c[i], i - ar, kfirst, klast, nva);
            pl_kill_co_goal((const tree_t * const *) cl->c, ar, -1, kco, nva);
            for (int i = ar; i < cl->n; i++) pl_kill_co_goal((const tree_t * const *)(cl->c + i), 1, i - ar, kco, nva);
            cx.kill_co = kco;
            cx.kill_gl = (const tree_t * const *)(cl->c + ar);
            cx.kill_ng = cl->n - ar;
            cx.kill_first = kfirst;
            cx.kill_last = klast;
            cx.kill_nv = nva;
        }
        IR_t * first = pl_attv_on() ? pl_lower_conj(&cx, bg, nb, tret, step, &bentry, &redo, &tnode) :
            pl_lower_conj(&cx, (const tree_t * const *)(cl->c + ar), cl->n - ar, tret, step, &bentry, &redo, &tnode);
        cx.kill_gl = (const tree_t * const *) 0;
        if (tnode && tnode->op == IR_CALL_PROC_STAGED && !pl_trace_wanted()) tnode->seal = PL_SEAL_TAIL;
        cx.tail_pop = 0;
        if (pop_on) pl_pop_vet(&cx, pop_from);
        pl_register_releases(&cx, pop_from);
        cx.pop_ok = (lc_vec *) 0;
        IR_t * next = bentry ? bentry : (first ? first : succeed);
        for (int i = ar - 1; i >= 0; i--) {
            if (cl->c[i] && cl->c[i]->t == TT_VAR && (int) cl->c[i]->v.ival >= 0 && (int) cl->c[i]->v.ival < nva && valias[(int) cl->c[i]->v.ival] == i + 1) continue;
            { IR_t * klit = pl_const_lit(&cx, cl->c[i]); if (klit) { next = pl_unify_const_node(&cx, pl_param_name(i), klit, next, step); continue; } }
            if (pl_head_boxable(cl->c[i])) {
                IR_t * srcv = lpl_build(&cx, IR_VAR_REF, NULL, NULL);
                IR_LIT(srcv).sval = pl_param_name(i);
                next = pl_head_term(&cx, cl, ar, srcv, 0, cl->c[i], next, step);
                continue;
            }
            IR_t * u = lpl_build(&cx, IR_CALL, next, step);
            IR_LIT(u).sval = "$unify";
            IR_t * lhs = lpl_build(&cx, IR_VAR_REF, NULL, NULL);
            IR_LIT(lhs).sval = pl_param_name(i);
            IR_t * he = NULL;
            IR_t * rhs = term_lval_e(&cx, cl->c[i], &he);
            lc_γ_to(lhs, he ? he : rhs);
            lc_ω_to(lhs, step);
            lc_γ_to(rhs, u);
            lc_ω_to(rhs, step);
            ir_operand_push(u, lhs);
            ir_operand_push(u, rhs);
            next = lhs;
        }
        g->alt_entry[k] = next;
        g->alt_ret[k] = succeed;
        g->alt_redo[k] = redo;
        if (nc > 1) ir_construct_add(g, IR_CK_CLAUSE, next, succeed, clause_lo, g->n);
    }
    maxlocal = g_lower_pl.fresh_next - 1;
    g_lower_pl.fresh_next = fresh_saved;
    if (arity < 0) arity = 0;
    g->entry = pl_trace_named_wrap(&cx, "__trace_call", trace_key, g->alt_entry[0], step);
    g->alt_entry[0] = g->entry;
    pl_graph_stamp(g, arity, maxlocal, aliased, nva);
    g->deterministic = (nc == 1);
    g->dl_latent = g_stage2.depth_counted && pl_dl_ctl_key(key);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_pl_pred_graph(const char * key, const tree_t * ch) {
    if (ch->t == TT_CHOICE) { if (ch->n < 1) return -1; if (!ch->c[0] || ch->c[0]->t != TT_CLAUSE) return -1; } else if (ch->t != TT_CLAUSE) return -1;
    {
        extern void zls_frame_resident_mark(const IR_t *);
        IR_graph_t * g = pl_pred_graph(ch, key);
        if (g && g_stage2.depth_counted && key && !strcmp(key, "call_with_depth_limit/3")) g->dl_pair = 1;
        if (g && g->home_frame) for (int i = 0; i < g->n; i++) if (g->all[i] && g->all[i]->op == IR_TABLE_HDR) zls_frame_resident_mark(g->all[i]);
        return bb_program_add(&g_stage2.bbp, g);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_register_program(stage2_t * s2, const tree_t * prog) {
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
            resolve_pred_table_insert(&s2->resolve_pred_table, sub->v.sval, sub)->file = stmt_attr_str(stmt_attr_find(s, ":file"));
            if (strcmp(sub->v.sval, "main/0") == 0 && s2->module_registry.main_mod < 0) s2->module_registry.main_mod = mod_idx;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_attv_user(const char * nm, int ar) { return pl_file_defines(nm, ar) || pl_db_owned(nm, ar); }
