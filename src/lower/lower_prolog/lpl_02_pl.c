#include "lower/lower_prolog/lpl_head.h"
#include <stdio.h>
#include "runtime/core/g_runtime_eval.h"
#include <stdlib.h>
#include <limits.h>
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "ir/stage2_pl.h"
#include "ir/bb_box.h"
#include "lower/lower_common.h"
#include "emitter/emit_state.h"
#include "emitter/emit_regime.h"
#include "planner/frame_layout.h"
#include "runtime/rt/prolog_atom.h"
#include "lower/lower_prolog/lpl_01_pl.h"
#include "lower/lower_prolog/lpl_02_pl.h"
#include "lower/lower_prolog/lpl_04_pl.h"
#include "lower/lower_prolog/lpl_05_pl.h"
#include "lower/g_lower_pl.h"
static tree_t * pl_dot_take(const tree_t * t, lc_vec * ev, int * next);
static tree_t * pl_dot_goal(const tree_t * g, int * next);
int pl_decl_mf_is(const char * name, int arity) {
    for (uint32_t i = 0; name && i < g_stage2_pl.pl_decl_multifile.len; i++) {
        const tree_t * m = CV_AT(g_stage2_pl.pl_decl_multifile, const tree_t *, i);
        if (m->c[1]->v.ival == arity && !strcmp(m->c[0]->v.sval, name)) return 1;
    }
    return 0;
}
static const char * pl_meta_tok_text(const tree_t * a) {
    static const char * const dg[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
    if (a && a->t == TT_ILIT && a->v.ival >= 0 && a->v.ival <= 9) return dg[a->v.ival];
    if (a && (a->t == TT_NAME || a->t == TT_QLIT) && a->v.sval && strcmp(a->v.sval, "*")) return a->v.sval;
    return "?";
}
void pl_decl_meta_record(const tree_t * spec) {
    if (!spec) return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_decl_meta_record(spec->c[0]); pl_decl_meta_record(spec->c[1]); return; }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_decl_meta_record(spec->c[i]); return; }
    if (spec->t == TT_FNC && spec->v.sval && spec->n > 0) {
        CV_PUSH(g_stage2_pl.pl_decl_meta, const tree_t *) = spec;
        if (g_stage2.rt_lowering && g_stage2.rt_root) {
            extern void rt_pl_db_meta_decl(void *, const char *, int64_t, const char * const *);
            const char * toks[spec->n];
            for (int i = 0; i < spec->n; i++) toks[i] = pl_meta_tok_text(spec->c[i]);
            rt_pl_db_meta_decl(g_stage2.rt_root, spec->v.sval, spec->n, toks);
        }
    }
}
const tree_t * pl_decl_meta_of(const char * name, int arity) {
    for (uint32_t i = 0; name && i < g_stage2_pl.pl_decl_meta.len; i++) { const tree_t * m = CV_AT(g_stage2_pl.pl_decl_meta, const tree_t *, i); if (m->n == arity && !strcmp(m->v.sval, name)) return m; }
    return (const tree_t *) 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_iso_builtin(const char * nm, int ar) {
    static const struct {
        const char * nm;
        int ar;
    } iso[] = { { "!", 0 }, { ",", 2 }, { "->", 2 }, { ";", 2 }, { "<", 2 }, { "=", 2 }, { "=..", 2 }, { "=:=", 2 }, { "=<", 2 }, { "==", 2 }, { "=\\=", 2 }, { ">", 2 }, { ">=", 2 }, { "@<", 2 },
        { "@=<", 2 }, { "@>", 2 }, { "@>=", 2 }, { "\\+", 1 }, { "\\=", 2 }, { "\\==", 2 }, { "abolish", 1 }, { "acyclic_term", 1 }, { "arg", 3 }, { "asserta", 1 }, { "assertz", 1 },
        { "at_end_of_stream", 0 }, { "at_end_of_stream", 1 }, { "atom", 1 }, { "atom_chars", 2 }, { "atom_codes", 2 }, { "atom_concat", 3 }, { "atom_length", 2 }, { "atomic", 1 }, { "bagof", 3 },
        { "call", 1 }, { "call", 2 }, { "call", 3 }, { "call", 4 }, { "call", 5 }, { "call", 6 }, { "call", 7 }, { "call", 8 }, { "callable", 1 }, { "catch", 3 }, { "char_code", 2 },
        { "char_conversion", 2 }, { "clause", 2 }, { "close", 1 }, { "close", 2 }, { "compare", 3 }, { "compound", 1 }, { "copy_term", 2 }, { "current_char_conversion", 2 }, { "current_input", 1 },
        { "current_op", 3 }, { "current_output", 1 }, { "current_predicate", 1 }, { "current_prolog_flag", 2 }, { "discontiguous", 1 }, { "dynamic", 1 }, { "fail", 0 }, { "false", 0 }, { "findall",
        3 }, { "float", 1 }, { "flush_output", 0 }, { "flush_output", 1 }, { "functor", 3 }, { "get_byte", 1 }, { "get_byte", 2 }, { "get_char", 1 }, { "get_char", 2 }, { "get_code", 1 },
        { "get_code", 2 }, { "ground", 1 }, { "halt", 0 }, { "halt", 1 }, { "initialization", 1 }, { "integer", 1 }, { "is", 2 }, { "keysort", 2 }, { "length", 2 }, { "message_queue_create", 2 },
        { "message_queue_destroy", 1 }, { "message_queue_property", 2 }, { "multifile", 1 }, { "mutex_create", 2 }, { "mutex_destroy", 1 }, { "mutex_lock", 1 }, { "mutex_property", 2 },
        { "mutex_trylock", 1 }, { "mutex_unlock", 1 }, { "nl", 0 }, { "nl", 1 }, { "nonvar", 1 }, { "number", 1 }, { "number_chars", 2 }, { "number_codes", 2 }, { "numbervars", 3 }, { "once", 1 },
        { "op", 3 }, { "open", 3 }, { "open", 4 }, { "peek_byte", 1 }, { "peek_byte", 2 }, { "peek_char", 1 }, { "peek_char", 2 }, { "peek_code", 1 }, { "peek_code", 2 }, { "phrase", 2 }, { "phrase",
        3 }, { "predicate_property", 2 }, { "put_byte", 1 }, { "put_byte", 2 }, { "put_char", 1 }, { "put_char", 2 }, { "put_code", 1 }, { "put_code", 2 }, { "read", 1 }, { "read", 2 }, { "read_term",
        2 }, { "read_term", 3 }, { "repeat", 0 }, { "retract", 1 }, { "retractall", 1 }, { "set_input", 1 }, { "set_output", 1 }, { "set_prolog_flag", 2 }, { "set_stream_position", 2 }, { "setof",
        3 }, { "sort", 2 }, { "stream_property", 2 }, { "sub_atom", 5 }, { "subsumes_term", 2 }, { "term_variables", 2 }, { "thread_create", 3 }, { "thread_detach", 1 }, { "thread_get_message", 1 },
        { "thread_get_message", 2 }, { "thread_get_message", 3 }, { "thread_peek_message", 1 }, { "thread_peek_message", 2 }, { "thread_property", 2 }, { "thread_self", 1 }, { "thread_send_message",
        2 }, { "thread_signal", 2 }, { "throw", 1 }, { "true", 0 }, { "unify_with_occurs_check", 2 }, { "var", 1 }, { "with_mutex", 2 }, { "write", 1 }, { "write", 2 }, { "write_canonical", 1 },
        { "write_canonical", 2 }, { "write_term", 2 }, { "write_term", 3 }, { "writeq", 1 }, { "writeq", 2 }, { 0, 0 } };
    for (int i = 0; iso[i].nm; i++) if (iso[i].ar == ar && !strcmp(iso[i].nm, nm)) return 1;
    return 0;
}
int pl_xsb_sysmod(const char * m) { extern int prolog_xsb_sysmod(const char *); return prolog_xsb_sysmod(m); }
static int pl_xsb_library_replaces(const char * nm, int ar) { return g_stage2_pl.pl_dialect == PL_DIALECT_XSB && ar == 2 && !strcmp(nm, "term_to_atom") && pl_file_defines(nm, ar); }
int pl_swi_user_redefines(const char * nm, int ar) {
    return (g_stage2_pl.pl_dialect == PL_DIALECT_SWI || g_stage2_pl.pl_dialect == PL_DIALECT_XSB) && nm[0] != '$' && !pl_iso_builtin(nm, ar) &&
        (pl_user_file_defines(nm, ar) || pl_xsb_library_replaces(nm, ar));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_is_callable(const tree_t * g) {
    if (!g) return 0;
    switch (g->t) { case TT_FNC: case TT_QLIT: case TT_NAME: case TT_CUT: case TT_UNIFY: case TT_IF: case TT_PROGRAM: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_phrase_plain_nt(const tree_t * g) {
    static const char * const ctl[] = { ",", ";", "->", "*->", "\\+", "{}", ".", "[]", "[|]", "!", "|", "call", ":", 0 };
    if (!g || (g->t != TT_FNC && g->t != TT_NAME && g->t != TT_QLIT) || !g->v.sval) return 0;
    for (int i = 0; ctl[i]; i++) if (!strcmp(g->v.sval, ctl[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * pl_meta_goal(const tree_t * g, const tree_t * const * extra, int nextra) {
    if (!g) return NULL;
    if (nextra <= 0) return pl_tree_is_callable(g) ? g : NULL;
    if (g->t != TT_FNC && g->t != TT_NAME && g->t != TT_QLIT) return NULL;
    if (g_stage2_pl.pl_dialect == PL_DIALECT_XSB && g->t == TT_FNC && g->v.sval && !strcmp(g->v.sval, ":") && g->n == 2 && g->c[0] && (g->c[0]->t == TT_QLIT || g->c[0]->t == TT_NAME)) {
        const tree_t * in = pl_meta_goal(g->c[1], extra, nextra);
        return in ? pl_cc_fnc2(":", (tree_t *) g->c[0], (tree_t *) in) : NULL;
    }
    tree_t * e = ast_node_new(TT_FNC);
    e->v.sval = g->v.sval;
    e->line = g->line;
    for (int i = 0; i < g->n; i++) ast_push(e, g->c[i]);
    for (int i = 0; i < nextra; i++) ast_push(e, (tree_t *) extra[i]);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * pl_runtime_define_pred_x(const char * key, const tree_t * choice, int arity, IR_graph_t ** gout, int pkt_cell, int pkt_slot, int * chain_off_out, void * rt_root) {
    if (choice && (choice->t == TT_CHOICE || choice->t == TT_CLAUSE) && pl_tree_calls(choice, "prolog_cut_to", 1)) choice = pl_cutto_subj(choice, arity, key && !strncmp(key, "$cc", 3));
    extern IR_graph_t * g_emit_cfg;
    extern int g_frame_active;
    extern int g_gen_proc_active;
    extern void planner_road_named(const char *, int, int, const IR_graph_t *, planner_road_t *);
    extern void emit_jmp_entry_clear(void);
    extern void zls_graph_name(const IR_graph_t *, const char *);
    extern void rt_proc_set_fn(const char *, bb_box_fn);
    extern bb_box_fn compile_chain(IR_graph_t *, IR_t *, FILE *, const char *);
    extern void rt_proc_set_frame_bytes(const char *, int);
    extern void rt_proc_set_dyn_scope(const char *, int);
    extern void rt_proc_set_generator(const char *, int);
    extern void rt_proc_set_jmpentry(const char *, int);
    extern void rt_proc_set_zstatic(const char *, int);
    extern void bb_ab_seal_entry_cells(const char *, void *, int);
    extern int rt_pl_dialect_swi(void);
    extern int rt_pl_dialect_xsb(void);
    extern int g_last_flat_frame_bytes;
    extern int g_last_flat_zstatic;
    int idx;
    IR_graph_t * g;
    bb_box_fn fn;
    IR_graph_t * cfg_sv;
    int fa;
    int rfe_sv;
    int gpa_sv;
    if (!key || !choice) return (void *)0;
    {
        extern int rt_proc_is_registered(const char *);
        extern void rt_proc_register(const char *, const char **, int);
        if (!g_stage2.depth_counted && rt_proc_is_registered("call_with_depth_limit/3")) g_stage2.depth_counted = 1;
        if (!rt_proc_is_registered(key)) rt_proc_register(ct_strdup(key), (const char **) 0, arity);
    }
    { extern void bb_pool_init(void); bb_pool_init(); }
    { extern void zls_reset(void); zls_reset(); extern void fc_tables_reset(void); fc_tables_reset(); }
    {
        void * rsv = g_stage2.rt_root;
        int lsv = g_stage2.rt_lowering;
        g_stage2.rt_root = rt_root;
        g_stage2.rt_lowering = 1;
        if (g_stage2_pl.pl_dialect != PL_DIALECT_SWI && rt_pl_dialect_swi()) g_stage2_pl.pl_dialect = PL_DIALECT_SWI;
        if (g_stage2_pl.pl_dialect != PL_DIALECT_XSB && rt_pl_dialect_xsb()) g_stage2_pl.pl_dialect = PL_DIALECT_XSB;
        idx = lower_pl_pred_graph(key, choice);
        g_stage2.rt_root = rsv;
        g_stage2.rt_lowering = lsv;
    }
    if (idx < 0) return (void *)0;
    g = g_stage2.bbp.table[idx];
    if (!g) return (void *)0;
    if (gout) *gout = g;
    g->resumable_callable = 1;
    g->unbound_serial = 1;
    g->poll_alpha = 1;
    if (pkt_slot >= 0) { g->pkt_fragment = 1; g->pkt_cell = pkt_cell; g->pkt_slot = pkt_slot; }
    { extern void optimizer_run(IR_graph_t *); optimizer_run(g); }
    { extern void zls_forget_graph_nodes(const IR_graph_t *); zls_forget_graph_nodes(g); }
    { extern void fl_derive_tier(IR_graph_t *); fl_derive_tier(g); }
    { extern void lower_call_forms_graph(IR_graph_t *); lower_call_forms_graph(g); }
    { extern void zls_home_pass(const IR_graph_t *); zls_home_pass(g); }
    { extern void planner_cells_graph(IR_graph_t *); planner_cells_graph(g); }
    cfg_sv = g_emit_cfg;
    g_emit_cfg = g;
    fa = g_frame_active;
    g_frame_active = 1;
    rfe_sv = g_rt_eval.rt_fragment_emit;
    g_rt_eval.rt_fragment_emit = 1;
    rt_proc_set_generator(key, 1);
    rt_proc_set_jmpentry(key, 1);
    rt_proc_set_dyn_scope(key, 0);
    { extern void rt_proc_set_pinned(const char *, int); extern int zls_g_resumable_frame(const IR_graph_t *); rt_proc_set_pinned(key, zls_g_resumable_frame(g)); }
    gpa_sv = g_gen_proc_active;
    g_gen_proc_active = 1;
    {
        extern int g_flat_frame_floor;
        extern int zls_g_region(const IR_graph_t *);
        g_flat_frame_floor = 0;
        if (g->entry && ((g->entry->op == IR_DEFINE_ADOPT) || g->entry->op == IR_GOTO_DEFERRED)) {
            for (int _mi = 0; _mi < g_stage2.proc_count; _mi++) if (g_stage2.proc_table[_mi].name && !strcmp(g_stage2.proc_table[_mi].name, "main")) {
                int _mx = g_stage2.proc_table[_mi].bb_idx;
                if (_mx >= 0 && _mx < g_stage2.bbp.count && g_stage2.bbp.table[_mx]) g_flat_frame_floor = zls_g_region(g_stage2.bbp.table[_mx]);
                break;
            }
            if (g_flat_frame_floor <= 0) g_flat_frame_floor = zls_g_region(g);
        }
    }
    { planner_road_t _rd; planner_road_named(key, 0, 1, g, &_rd); emit_jmp_entry_for_road(&_rd, g, key); }
    { extern int g_flat_dc_np; extern int rt_pl_dc_ok(const char *, int); g_flat_dc_np = (pkt_slot < 0 && rt_pl_dc_ok(key, g->nparams)) ? g->nparams : -1; }
    zls_graph_name(g, key);
    {
        char pfx[fmt_len("proc_%s", key)];
        snprintf(pfx, sizeof pfx, "proc_%s", key);
        if (getenv("SCRIP_PL_RTASM")) {
            extern int zls_g_region(const IR_graph_t *);
            extern int zls_off(const IR_t *);
            int _mx = -1;
            for (int _i = 0; _i < g->n; _i++) if (g->all[_i]) { int _o = zls_off(g->all[_i]); if (_o > _mx) _mx = _o; }
            fprintf(stderr, "[RTASM] %s: nodes=%d nparams=%d zls_region=%d max_granted_cell=%d %s\n", key, g->n, g->nparams, zls_g_region(g), _mx,
                (_mx >= 0 && _mx + 16 > zls_g_region(g)) ? "*** CELL OUTSIDE REGION ***" : "(in region)");
            {
                extern int zls_scope_of(const IR_t *);
                extern int zls_g_first_scope(const IR_graph_t *);
                int _rs = zls_g_first_scope(g);
                for (int _i = 0; _i < g->n; _i++) if (g->all[_i]) {
                    int _o = zls_off(g->all[_i]);
                    int _sc = zls_scope_of(g->all[_i]);
                    if (_o < 0) continue;
                    fprintf(stderr, "[RTASM]   node[%d] op=%d off=%d scope=%d root_scope=%d %s\n", _i, (int)g->all[_i]->op, _o, _sc, _rs,
                        (_sc >= 0 && _sc != _rs) ? "*** STALE ENTRY FROM ANOTHER GRAPH ***" : "");
                }
            }
            fprintf(stderr, "[RTASM] ---- runtime fragment for %s ----\n", key);
            compile_chain(g, g->entry, stderr, pfx);
            fprintf(stderr, "[RTASM] ---- end %s ----\n", key);
        }
        fn = compile_chain(g, g->entry, (FILE *)0, pfx);
    }
    { extern void emit_gc_tables_register(const void *); emit_gc_tables_register((const void *) fn); }
    emit_jmp_entry_clear();
    g_gen_proc_active = gpa_sv;
    g_rt_eval.rt_fragment_emit = rfe_sv;
    g_frame_active = fa;
    g_emit_cfg = cfg_sv;
    if (!fn) { extern void rt_code_pool_check(void); rt_code_pool_check(); return (void *)0; }
    if (chain_off_out) { extern long emit_last_pkt_chain_off(void); *chain_off_out = (int)emit_last_pkt_chain_off(); }
    rt_proc_set_frame_bytes(key, g_last_flat_frame_bytes);
    rt_proc_set_fn(key, fn);
    bb_ab_seal_entry_cells(key, (void *)fn, 1);
    rt_proc_set_zstatic(key, g_last_flat_zstatic);
    { extern long g_last_dc_off; extern void rt_proc_set_dcfn(const char *, void *); rt_proc_set_dcfn(key, (g_last_dc_off >= 0) ? (void *)((char *)fn + g_last_dc_off) : (void *)0); }
    if (!gout) {
        extern void zls_reset(void);
        extern void IR_free(IR_graph_t *);
        zls_reset();
        g_stage2.bbp.table[idx] = (IR_graph_t *)0;
        if (idx == g_stage2.bbp.count - 1) g_stage2.bbp.count--;
        IR_free(g);
    }
    return (void *)fn;
}
void * pl_runtime_define_pred(const char * key, const tree_t * choice, int arity, void * rt_root) { return pl_runtime_define_pred_x(key, choice, arity, NULL, -1, -1, (int *)0, rt_root); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_tree_count(const tree_t * t) { int n; if (!t) return 0; n = 1; for (int i = 0; i < t->n; i++) n += pl_tree_count(t->c[i]); return n; }
static void pl_tree_gather(tree_t * t, tree_t ** v, int * k) { if (!t) return; v[(*k)++] = t; for (int i = 0; i < t->n; i++) pl_tree_gather(t->c[i], v, k); }
static int pl_tree_ptr_cmp(const void * a, const void * b) { uintptr_t x = (uintptr_t) *(tree_t * const *) a, y = (uintptr_t) *(tree_t * const *) b; return (x > y) - (x < y); }
static void pl_trees_drop(tree_t * a, tree_t * b) {
    int n = pl_tree_count(a) + pl_tree_count(b);
    int k = 0;
    tree_t ** v;
    if (n <= 0) return;
    v = (tree_t **) ct_alloc((size_t) n * sizeof(tree_t *));
    pl_tree_gather(a, v, &k);
    pl_tree_gather(b, v, &k);
    qsort(v, (size_t) k, sizeof(tree_t *), pl_tree_ptr_cmp);
    for (int i = 0; i < k; i++) if (!i || v[i] != v[i - 1]) { if (v[i]->c) ct_drop((char *) v[i]->c - sizeof(size_t)); ct_drop(v[i]); }
    ct_drop(v);
}
void * pl_runtime_define_fragment(const char * fkey, void * clause_cell, int arity, int cell_k, int slot, int * ridx_out, int * chain_off_out, void * rt_root) {
    extern void * rt_pl_clause_tree(void *);
    extern tree_t * pl_runtime_clause_tree(tree_t *);
    extern void * rt_pl_choice_new(const char *);
    extern void rt_pl_choice_add(void *, void *);
    extern int rt_proc_index_of(const char *);
    void * raw = clause_cell ? rt_pl_clause_tree(clause_cell) : (void *)0;
    void * cl = raw ? (void *) pl_runtime_clause_tree((tree_t *) raw) : (void *)0;
    void * ch;
    void * fn;
    int off = -1;
    if (!fkey || !cl) return (void *)0;
    ch = rt_pl_choice_new(fkey);
    rt_pl_choice_add(ch, cl);
    fn = pl_runtime_define_pred_x(ct_strdup(fkey), (const tree_t *) ch, arity, NULL, cell_k, slot, &off, rt_root);
    pl_trees_drop((tree_t *) raw, (tree_t *) ch);
    if (!fn || off < 0) return (void *)0;
    if (ridx_out) *ridx_out = rt_proc_index_of(fkey);
    if (chain_off_out) *chain_off_out = off;
    return fn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_runtime_install_packet_entry(const char * key, int arity, const char * head_fkey) {
    extern int rt_proc_is_registered(const char *);
    extern void rt_proc_register(const char *, const char **, int);
    extern void * rt_proc_fn(const char *);
    extern void rt_proc_set_generator(const char *, int);
    extern void rt_proc_set_jmpentry(const char *, int);
    extern void rt_proc_set_dyn_scope(const char *, int);
    extern void rt_proc_set_pinned(const char *, int);
    extern void rt_proc_set_fn(const char *, bb_box_fn);
    extern void bb_ab_seal_entry_cells(const char *, void *, int);
    void * fn = head_fkey ? rt_proc_fn(head_fkey) : (void *)0;
    if (!key || !fn) return 0;
    if (!rt_proc_is_registered(key)) rt_proc_register(ct_strdup(key), (const char **) 0, arity);
    rt_proc_set_generator(key, 1);
    rt_proc_set_jmpentry(key, 1);
    rt_proc_set_dyn_scope(key, 0);
    rt_proc_set_pinned(key, 1);
    rt_proc_set_fn(key, (bb_box_fn) fn);
    bb_ab_seal_entry_cells(key, fn, 1);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_runtime_register_key(const char * key, int arity) {
    extern int rt_proc_is_registered(const char *);
    extern void rt_proc_register(const char *, const char **, int);
    extern void rt_proc_set_generator(const char *, int);
    extern void rt_proc_set_jmpentry(const char *, int);
    extern void rt_proc_set_dyn_scope(const char *, int);
    extern void rt_proc_set_pinned(const char *, int);
    if (!key || rt_proc_is_registered(key)) return;
    rt_proc_register(ct_strdup(key), (const char **) 0, arity);
    rt_proc_set_generator(key, 1);
    rt_proc_set_jmpentry(key, 1);
    rt_proc_set_dyn_scope(key, 0);
    rt_proc_set_pinned(key, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * pl_runtime_define_static(const char * key, const char * wkey, const tree_t * choice, int arity, void * rt_root) {
    void * fn;
    pl_runtime_register_key(key, arity);
    fn = pl_runtime_define_pred_x(ct_strdup(wkey), choice, arity, NULL, -1, -1, (int *)0, rt_root);
    return (fn && pl_runtime_install_packet_entry(key, arity, wkey)) ? fn : (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_mkc_functor_is(const IR_t * f, const char * nm) {
    extern int prolog_functor_name(int);
    extern const char * prolog_atom_name(int);
    if (!f || !nm) return 0;
    if (f->op == IR_LIT_INTEGER) { const char * s = prolog_atom_name(prolog_functor_name((int)IR_LIT(f).ival)); return s && !strcmp(s, nm); }
    return (f->op == IR_LIT_STRING || f->op == IR_LIT_ATOM) && IR_LIT(f).sval && !strcmp(IR_LIT(f).sval, nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_name_is_scrip_own(const char * nm, int ar) {
    extern int pl_prelude_defines(const char *, int);
    if (!nm) return 1;
    if (!strcmp(nm, "$pl_cconv") && ar == 2) return 1;
    return ar >= 0 && pl_prelude_defines(nm, ar);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_dyn_marker(const tree_t * t) { return t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "dynamic"); }
void pl_dyn_spec_file(const tree_t * spec, const char * file) {
    if (!spec || !file || file[0] == '<') return;
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ",") && spec->n == 2) { pl_dyn_spec_file(spec->c[0], file); pl_dyn_spec_file(spec->c[1], file); return; }
    if (spec->t == TT_MAKELIST) { for (int i = 0; i < spec->n; i++) pl_dyn_spec_file(spec->c[i], file); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, ":") && spec->n == 2) { pl_dyn_spec_file(spec->c[1], file); return; }
    if (spec->t == TT_FNC && spec->v.sval && !strcmp(spec->v.sval, "/") && spec->n == 2 && spec->c[0] && (spec->c[0]->t == TT_QLIT || spec->c[0]->t == TT_NAME) && spec->c[0]->v.sval && spec->c[1] &&
        spec->c[1]->t == TT_ILIT) {
        char key[fmt_len("%s/%d", spec->c[0]->v.sval, (int) spec->c[1]->v.ival)];
        snprintf(key, sizeof key, "%s/%d", spec->c[0]->v.sval, (int) spec->c[1]->v.ival);
        for (Resolve_PredEntry * e = g_stage2.resolve_pred_table.buckets[resolve_pred_hash(key)]; e; e = e->next) if (!strcmp(e->key, key) && !e->file) e->file = file;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_db_decls_capture(void) {
    extern int prolog_atom_intern(const char *);
    extern int pl_prelude_lib_exports(const char *, int);
    extern void * ct_grow(void *, size_t);
    long long * w = (long long *) 0;
    int n = 0, cap = 0;
#define PL_DW(v) do { if (n >= cap) { cap = cap ? cap * 2 : 64; w = (long long *) ct_grow((void *) w, (size_t) cap * sizeof(long long)); } w[n++] = (long long) (v); } while (0)
    PL_DW(0);
    PL_DW(g_stage2_pl.pl_dyn_n > 0 ? g_stage2_pl.pl_dyn_n : 1);
    for (int di = 0; di < g_stage2_pl.pl_dyn_n; di++) {
        const char * dn = g_stage2_pl.pl_dyn_name[di];
        int da = g_stage2_pl.pl_dyn_arity[di];
        int kind = 0;
        if (!dn || (pl_name_is_scrip_own(dn, da) && strncmp(dn, "$pl_", 4))) continue;
        if (da >= 0) { if (pl_decl_dyn_is(dn, da)) kind = 2; else if (pl_file_defines(dn, da)) kind = 3; }
        PL_DW(prolog_atom_intern(dn));
        PL_DW(da);
        PL_DW(kind);
        PL_DW(di);
    }
    for (int bi = 0; bi < STAGE2_PL_PRED_TABLE_SIZE; bi++) for (Resolve_PredEntry * pe = g_stage2.resolve_pred_table.buckets[bi]; pe; pe = pe->next) {
        const char * slash;
        int ar;
        size_t nl;
        if (!pe->key || !pe->choice || (pe->choice->t != TT_CHOICE && !pl_dyn_marker(pe->choice)) || (pe->choice->t == TT_CHOICE && pe->choice->n < 1)) continue;
        slash = strrchr(pe->key, '/');
        if (!slash) continue;
        ar = atoi(slash + 1);
        nl = (size_t) (slash - pe->key);
        {
            char nm[nl + 1];
            memcpy(nm, pe->key, nl);
            nm[nl] = 0;
            if (pl_name_is_scrip_own(nm, ar)) {
                if (pl_prelude_lib_exports(nm, ar) && !pl_iso_builtin(nm, ar) && pl_dyn_index(nm, ar) < 0) {
                    PL_DW(prolog_atom_intern(nm));
                    PL_DW(ar);
                    PL_DW(7);
                    PL_DW(-1);
                } else if (g_stage2_pl.pl_dialect == PL_DIALECT_GNU) {
                    PL_DW(prolog_atom_intern(nm));
                    PL_DW(ar);
                    PL_DW(7);
                    PL_DW(-1);
                }
                continue;
            }
            if (pl_decl_dyn_is(nm, ar) && pe->file && pe->file[0] != '<') {
                char rp[PATH_MAX];
                if (pl_file_key_path(pe->file, rp)) { PL_DW(prolog_atom_intern(nm)); PL_DW(ar); PL_DW(4); PL_DW(prolog_atom_intern(rp)); }
            }
            if (pl_dyn_marker(pe->choice)) { if (pe->file && pe->file[0] != '<' && pe->choice->line > 0) { PL_DW(prolog_atom_intern(nm)); PL_DW(ar); PL_DW(5); PL_DW(pe->choice->line); } continue; }
            if (pl_dyn_index(nm, ar) < 0 && !pl_decl_dyn_is(nm, ar)) {
                PL_DW(prolog_atom_intern(nm));
                PL_DW(ar);
                PL_DW(1);
                PL_DW(-1);
                {
                    const char * pf = pl_pred_file(nm, ar);
                    char rp[PATH_MAX];
                    if (pf && pl_file_key_path(pf, rp)) { PL_DW(prolog_atom_intern(nm)); PL_DW(ar); PL_DW(4); PL_DW(prolog_atom_intern(rp)); }
                }
            }
            if (pe->file && pe->file[0] != '<' && pe->choice->c[0] && pe->choice->c[0]->line > 0) { PL_DW(prolog_atom_intern(nm)); PL_DW(ar); PL_DW(5); PL_DW(pe->choice->c[0]->line); }
            if (pe->choice->t == TT_CHOICE) { PL_DW(prolog_atom_intern(nm)); PL_DW(ar); PL_DW(9); PL_DW(pe->choice->n); }
        }
    }
    for (int i = 0; i < g_lower_pl.decl_other_n; i++) if (!pl_name_is_scrip_own(g_lower_pl.decl_other_name[i], g_lower_pl.decl_other_arity[i])) {
        PL_DW(prolog_atom_intern(g_lower_pl.decl_other_name[i]));
        PL_DW(g_lower_pl.decl_other_arity[i]);
        PL_DW(2);
        PL_DW(-1);
    }
    for (uint32_t i = 0; i < g_stage2_pl.pl_decl_meta.len; i++) {
        const tree_t * m = CV_AT(g_stage2_pl.pl_decl_meta, const tree_t *, i);
        for (int k = 0; m && m->v.sval && k < m->n; k++) { PL_DW(prolog_atom_intern(m->v.sval)); PL_DW(m->n); PL_DW(8); PL_DW(prolog_atom_intern(pl_meta_tok_text(m->c[k]))); }
    }
    for (uint32_t i = 0; i < g_stage2_pl.pl_decl_multifile.len; i++) {
        const tree_t * m = CV_AT(g_stage2_pl.pl_decl_multifile, const tree_t *, i);
        if (!m || !m->c[0] || !m->c[0]->v.sval || !m->c[1] || pl_name_is_scrip_own(m->c[0]->v.sval, (int) m->c[1]->v.ival)) continue;
        PL_DW(prolog_atom_intern(m->c[0]->v.sval));
        PL_DW(m->c[1]->v.ival);
        PL_DW(6);
        PL_DW(0);
    }
#undef PL_DW
    w[0] = n - 2;
    g_stage2_pl.db_decls_words = w;
    g_stage2_pl.db_decls_n = n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_dict_any(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->v.sval && (!strcmp(t->v.sval, "$dict_lit") || !strcmp(t->v.sval, "$dot"))) return 1;
    for (int i = 0; i < t->n; i++) if (pl_dict_any(t->c[i])) return 1;
    return 0;
}
static int pl_dict_key_cmp(const tree_t * a, const tree_t * b) {
    int ai = a->t == TT_ILIT, bi = b->t == TT_ILIT;
    if (ai != bi) return ai ? -1 : 1;
    if (ai) return (a->v.ival > b->v.ival) - (a->v.ival < b->v.ival);
    return strcmp(a->v.sval ? a->v.sval : "", b->v.sval ? b->v.sval : "");
}
static tree_t * pl_dict_canon(const tree_t * t) {
    if (!t) return NULL;
    tree_t * c = ast_node_new(t->t);
    c->v = t->v;
    c->line = t->line;
    c->slen = t->slen;
    for (int i = 0; i < t->n; i++) ast_push(c, pl_dict_canon(t->c[i]));
    if (c->t != TT_FNC || !c->v.sval || strcmp(c->v.sval, "$dict_lit") || !(c->n & 1)) return c;
    int np = (c->n - 1) / 2;
    int ord[np > 0 ? np : 1];
    for (int i = 0; i < np; i++) ord[i] = i;
    for (int i = 1; i < np; i++) for (int j = i; j > 0 && pl_dict_key_cmp(c->c[1 + 2 * ord[j]], c->c[1 + 2 * ord[j - 1]]) < 0; j--) { int x = ord[j]; ord[j] = ord[j - 1]; ord[j - 1] = x; }
    tree_t * d = ast_node_new(TT_FNC);
    d->v.sval = (char *) "$dict";
    d->line = c->line;
    ast_push(d, c->c[0]);
    for (int i = 0; i < np; i++) { ast_push(d, c->c[2 + 2 * ord[i]]); ast_push(d, c->c[1 + 2 * ord[i]]); }
    return d;
}
const tree_t * pl_dict_place(const tree_t * t) { return pl_dict_any(t) ? pl_dict_canon(t) : t; }
static tree_t * pl_dot_var(int k, int named) {
    if (!named) return pl_ssu_var(k);
    char b[32];
    snprintf(b, sizeof b, "_$dot%d", k);
    tree_t * v = ast_node_new(TT_VAR);
    v->v.sval = ct_strdup(b);
    return v;
}
static tree_t * pl_dot_fn(const tree_t * f, lc_vec * ev, int * next) {
    if (f && f->t == TT_FNC && f->n == 0 && f->v.sval) return pl_cc_fnc1("$dict_m0", (tree_t *) pl_atom_goal(f->v.sval));
    return pl_dot_take(f, ev, next);
}
static tree_t * pl_dot_take(const tree_t * t, lc_vec * ev, int * next) {
    if (!t) return NULL;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "$dot") && t->n == 2) {
        tree_t * o = pl_dot_take(t->c[0], ev, next);
        tree_t * f = pl_dot_fn(t->c[1], ev, next);
        int named = *next < 0, k = named ? -((*next)--) : (*next)++;
        tree_t * g = pl_cc_fnc3("$dict_eval", o, f, pl_dot_var(k, named));
        lc_vec_push(ev, &g);
        return pl_dot_var(k, named);
    }
    tree_t * c = ast_node_new(t->t);
    c->v = t->v;
    c->line = t->line;
    c->slen = t->slen;
    for (int i = 0; i < t->n; i++) ast_push(c, pl_dot_take(t->c[i], ev, next));
    return c;
}
static int pl_dot_is(const tree_t * t) { return t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "$dot") && t->n == 2; }
static tree_t * pl_dot_goal_arg(const tree_t * a, char spec, lc_vec * ev, int * next) {
    if (spec == '0') return pl_dot_goal(a, next);
    if (spec != '^') return pl_dot_take(a, ev, next);
    if (a && a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, "^") && a->n == 2) return pl_cc_fnc2("^", pl_dot_take(a->c[0], ev, next), pl_dot_goal_arg(a->c[1], '^', ev, next));
    return pl_dot_goal(a, next);
}
static const char * pl_dot_meta(const tree_t * g) {
    if (!g || g->t != TT_FNC || !g->v.sval) return (const char *) 0;
    if (!strcmp(g->v.sval, "initialization") && (g->n == 1 || g->n == 2)) return g->n == 1 ? "0" : "0?";
    {
        extern int rt_pl_meta_builtin(const char *, int, int, const char * const **);
        const char * const * toks;
        char m[g->n + 1];
        if (g->n <= 0 || !rt_pl_meta_builtin(g->v.sval, g->n, (g_stage2_pl.pl_dialect == PL_DIALECT_GNU), &toks)) return (const char *) 0;
        for (int i = 0; i < g->n; i++) m[i] = (toks[i][0] >= '0' && toks[i][0] <= '9' && !toks[i][1]) ? (toks[i][0] == '0' ? '0' : '1') : toks[i][0] == '^' ? '^' : '?';
        m[g->n] = 0;
        return lp_strdup(m);
    }
}
static tree_t * pl_dot_goal(const tree_t * g, int * next) {
    if (!g) return NULL;
    lc_vec ev;
    lc_vec_init(&ev, (int) sizeof(tree_t *));
    const char * tmpl = pl_dot_meta(g);
    tree_t * c;
    if (g->t == TT_IF || g->t == TT_PROGRAM || tmpl) {
        c = ast_node_new(g->t);
        c->v = g->v;
        c->line = g->line;
        for (int i = 0; i < g->n; i++) ast_push(c, tmpl ? pl_dot_goal_arg(g->c[i], tmpl[i], &ev, next) : pl_dot_goal(g->c[i], next));
    } else if ((g->t == TT_UNIFY || (g->t == TT_FNC && g->v.sval && !strcmp(g->v.sval, "="))) && g->n == 2 && (pl_dot_is(g->c[0]) != pl_dot_is(g->c[1]))) {
        const tree_t * d = pl_dot_is(g->c[0]) ? g->c[0] : g->c[1];
        tree_t * o = pl_dot_take(d->c[0], &ev, next);
        tree_t * f = pl_dot_fn(d->c[1], &ev, next);
        c = pl_cc_fnc3("$dict_eval", o, f, pl_dot_take(d == g->c[0] ? g->c[1] : g->c[0], &ev, next));
    } else c = pl_dot_take(g, &ev, next);
    for (int i = ev.n - 1; i >= 0; i--) c = pl_cc_fnc2(",", ((tree_t **) ev.data)[i], c);
    return c;
}
static tree_t * pl_dict_clause(const tree_t * cl, int goal_arg) {
    if (!cl || cl->t != TT_CLAUSE || !pl_dict_any(cl)) return (tree_t *) cl;
    tree_t * k = pl_dict_canon(cl);
    int ar = (int) cl->v.dval;
    if (ar < 0) ar = 0;
    if (ar > k->n) ar = k->n;
    int next = max_var_slot(cl, -1) + 1;
    tree_t * nc = ast_node_new(TT_CLAUSE);
    nc->v = cl->v;
    nc->line = cl->line;
    lc_vec hev;
    lc_vec_init(&hev, (int) sizeof(tree_t *));
    for (int i = 0; i < ar; i++) ast_push(nc, i == goal_arg ? pl_dot_goal(k->c[i], &next) : pl_dot_take(k->c[i], &hev, &next));
    for (int i = 0; i < hev.n; i++) ast_push(nc, ((tree_t **) hev.data)[i]);
    for (int i = ar; i < k->n; i++) {
        const tree_t * g = k->c[i];
        if (g && (g->t == TT_IF || g->t == TT_PROGRAM || pl_dot_meta(g))) { ast_push(nc, pl_dot_goal(g, &next)); continue; }
        lc_vec ev;
        lc_vec_init(&ev, (int) sizeof(tree_t *));
        tree_t * g2 = pl_dot_take(g, &ev, &next);
        for (int j = 0; j < ev.n; j++) ast_push(nc, ((tree_t **) ev.data)[j]);
        ast_push(nc, g2);
    }
    return nc;
}
static const char * pl_dict_def_mod(const tree_t * subj) {
    const char * k = subj && subj->t == TT_CHOICE ? subj->v.sval : NULL;
    size_t n = k ? strlen(k) : 0;
    if (n < 4 || strcmp(k + n - 4, ":=/2")) return NULL;
    if (n == 4) return "user";
    if (n < 6 || k[n - 5] != ':') return NULL;
    return ct_strndup(k, n - 5);
}
static tree_t * pl_dict_method_clause(const tree_t * cl, const char * mod) {
    if (!cl || cl->t != TT_CLAUSE || cl->n < 2) return NULL;
    const tree_t * h = cl->c[0];
    if (h && h->t == TT_FNC && h->v.sval && !strcmp(h->v.sval, ":") && h->n == 2 && h->c[0] && (h->c[0]->t == TT_QLIT || h->c[0]->t == TT_NAME)) { mod = h->c[0]->v.sval; h = h->c[1]; }
    if (!pl_dot_is(h)) return NULL;
    tree_t * f = h->c[1] && h->c[1]->t == TT_FNC && h->c[1]->n == 0 && h->c[1]->v.sval ? pl_cc_fnc1("$dict_m0", (tree_t *) pl_atom_goal(h->c[1]->v.sval)) : (tree_t *) h->c[1];
    tree_t * nc = ast_node_new(TT_CLAUSE);
    nc->v = cl->v;
    nc->v.dval = 4;
    nc->line = cl->line;
    ast_push(nc, (tree_t *) pl_atom_goal(mod));
    ast_push(nc, f);
    ast_push(nc, (tree_t *) h->c[0]);
    for (int i = 1; i < cl->n; i++) ast_push(nc, (tree_t *) cl->c[i]);
    return pl_dict_clause(nc, -1);
}
const tree_t * pl_dict_rewrite(const tree_t * prog) {
    int any = 0;
    for (int i = 0; i < prog->n && !any; i++) { const tree_t * s = prog->c[i]; any = pl_dict_any((s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL); }
    if (!any) return prog;
    tree_t * np = ast_node_new(prog->t);
    np->v = prog->v;
    np->line = prog->line;
    tree_t * meth = NULL;
    const tree_t * meth_s = NULL;
    for (int i = 0; i < prog->n; i++) {
        const tree_t * s = prog->c[i];
        const tree_t * subj = (s && s->t == TT_STMT) ? lp_s_expr(s, ":subj") : NULL;
        const char * dm = pl_dict_def_mod(subj);
        if (dm) {
            int took = 0;
            for (int k = 0; k < subj->n; k++) {
                tree_t * mc = pl_dict_method_clause(subj->c[k], dm);
                if (!mc) continue;
                if (!meth) { meth = ast_node_new(TT_CHOICE); meth->v.sval = (char *) "$dict_method/4"; meth->line = subj->line; meth_s = s; }
                ast_push(meth, mc);
                took = 1;
            }
            if (took) continue;
        }
        if (!pl_dict_any(subj)) { ast_push(np, (tree_t *) s); continue; }
        tree_t * ns;
        if (subj->t == TT_CHOICE) {
            ns = ast_node_new(TT_CHOICE);
            ns->v = subj->v;
            ns->line = subj->line;
            int ga = subj->v.sval && !strcmp(subj->v.sval, "pj_test/4") ? 3 : -1;
            for (int k = 0; k < subj->n; k++) ast_push(ns, pl_dict_clause(subj->c[k], ga));
        } else if (subj->t == TT_CLAUSE) ns = pl_dict_clause(subj, subj->v.sval && !strcmp(subj->v.sval, "pj_test/4") ? 3 : -1);
        else { int next = -1; ns = pl_dot_goal(pl_dict_canon(subj), &next); }
        ast_push(np, pl_table_stmt(s, ns));
    }
    if (meth) ast_push(np, pl_table_stmt(meth_s, meth));
    return np;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_names_depth_limit(const tree_t * t) {
    if (!t) return 0;
    if (t->t == TT_FNC && t->n == 3 && t->v.sval && !strcmp(t->v.sval, "call_with_depth_limit")) return 1;
    for (int i = 0; i < t->n; i++) if (pl_names_depth_limit(t->c[i])) return 1;
    return 0;
}
