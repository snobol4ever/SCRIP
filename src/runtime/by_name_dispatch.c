#include "by_name_dispatch.h"
#include <unistd.h>
#include <setjmp.h>
int core_icn_error(int code, DESCR_t val);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) size_t sv_len(DESCR_t arg, const char *coerced) {
    if (arg.v == DT_S && arg.slen != 0xFFFFFFFFu) return arg.slen ? (size_t)arg.slen : (coerced ? strlen(coerced) : 0);
    return coerced ? strlen(coerced) : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int icn_nxttab(int col, const int *stops, int nstops, int gap) {
    for (int k = 0; k < nstops; k++) if (stops[k] > col) return stops[k];
    int base = stops[nstops - 1]; int beyond = col - base;
    return base + ((beyond / gap) + 1) * gap;
}
#include "rt/rt_arena.h"
#include "rt/gc_heap.h"
#include "builtins/gen_value.h"
#include "builtins/gen_runtime.h"
#include "../driver/driver_private.h"
#include "../parsers/raku/re.h"
#include "core.h"
#include "core/utf8.h"
#include "builtin_ids.h"
#include "pattern_match.h"
#include "rt/rt.h"
#include "rt/rt_list_view.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_builtin_is_generator(const char *name)
{
    if (!name) return 0;
    return !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "any")
        || !strcmp(name, "many") || !strcmp(name, "bal") || !strcmp(name, "key")
        || !strcmp(name, "seq")
        || !strcmp(name, "push") || !strcmp(name, "put");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int builtin_is_generator(const char *name) { return rt_builtin_is_generator(name); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_builtin_is_generator(const char *name)
{
    if (!name) return 0;
    return !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "any")
        || !strcmp(name, "many") || !strcmp(name, "bal")
        || !strcmp(name, "key") || !strcmp(name, "seq");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_builtin_is_known(const char *name)
{
    if (!name || !name[0]) return 0;
    static const char *icn_known[] = {
        "integer", "real", "string", "numeric", "char", "cset", "abs", "ord",
        "iand", "ior", "ixor", "ishift", "icom", "proc",
        "sqrt", "sin", "cos", "tan", "atan", "log", "exp",
        "asin", "acos", "dtor", "rtod",
        "max", "min",
        "args", "collect", "copy",
        "image", "name", "type", "variable", "sort", "sortf",
        "center", "detab", "entab", "left", "map", "repl", "reverse", "right", "trim",
        "delete", "insert", "list", "member", "pull", "get", "pop",
        "push", "put", "set", "table",
        "close", "getenv", "open", "read", "reads",
        "seek", "where",
        "stop", "write", "writes",
        NULL
    };
    for (int i = 0; icn_known[i]; i++) if (!strcmp(icn_known[i], name)) return 1;
    return 0;
}
#include "../parsers/prolog/pl_cell.h"
#include "../parsers/prolog/term.h"
extern pl_trail_t g_pl_trail;
typedef struct { const char *key; void *alpha; void *beta; int nslots; } plw_pred_t;
static plw_pred_t g_plw_preds[512]; static int g_plw_pred_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_pred_bind(const char *key, void *alpha, void *beta, int nslots) {
    for (int i = 0; i < g_plw_pred_n; i++) if (!strcmp(g_plw_preds[i].key, key)) { g_plw_preds[i].alpha = alpha; g_plw_preds[i].beta = beta; g_plw_preds[i].nslots = nslots; return; }
    if (g_plw_pred_n < 512) { g_plw_preds[g_plw_pred_n].key = strdup(key); g_plw_preds[g_plw_pred_n].alpha = alpha; g_plw_preds[g_plw_pred_n].beta = beta; g_plw_preds[g_plw_pred_n].nslots = nslots; g_plw_pred_n++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_pred_alpha(const char *key) { for (int i = 0; i < g_plw_pred_n; i++) if (!strcmp(g_plw_preds[i].key, key)) return g_plw_preds[i].alpha; fprintf(stderr, "rt_pl_pred_alpha: unknown predicate %s\n", key); abort(); }
void *rt_pl_pred_beta(const char *key)  { for (int i = 0; i < g_plw_pred_n; i++) if (!strcmp(g_plw_preds[i].key, key)) return g_plw_preds[i].beta;  fprintf(stderr, "rt_pl_pred_beta: unknown predicate %s\n", key);  abort(); }
int rt_pl_pred_nslots_rt(const char *key) { for (int i = 0; i < g_plw_pred_n; i++) if (!strcmp(g_plw_preds[i].key, key)) return g_plw_preds[i].nslots; return 8; }
static inline __attribute__((always_inline)) int plw_unbound_tag(const DESCR_t *c) { return c->v == DT_SNUL || c->v == DT_FAIL || (c->v == (DTYPE_t)DT_PLVAR && c->p == (void *)c); }
static int g_plw_poison = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plw_poison_trap(void) { if (g_plw_poison < 0) { const char *e = getenv("SCRIP_PL_POISON_TRAP"); g_plw_poison = (e && atoi(e) != 0) ? 1 : 0; } return g_plw_poison; }
__attribute__((constructor)) static void plw_poison_init(void) { (void)plw_poison_trap(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *plw_cell_deref_slow(DESCR_t *c) {
    DESCR_t *prev = (DESCR_t *)0;
    int pt = g_plw_poison > 0;
    for (int guard = 0; guard < 4096; guard++) {
        if (pt && *(unsigned int *)c == 0xDDDDDDDDu) { fprintf(stderr, "[POISON-READ] c=%p prev=%p prev={v=%d slen=%u p=%p}\n", (void *)c, (void *)prev, prev ? (int)prev->v : -1, prev ? prev->slen : 0u, prev ? prev->p : (void *)0); fflush(stderr); abort(); }
        if (c->v == DT_N && c->slen == 1 && c->p) { prev = c; c = (DESCR_t *)c->p; continue; }
        if (c->v == DT_N && c->slen == 2 && c->p && ((VCELL_t *)c->p)->cellp) { prev = c; c = ((VCELL_t *)c->p)->cellp; continue; }
        if (c->v == (DTYPE_t)DT_PLVAR && c->p && c->p != (void *)c) { prev = c; c = (DESCR_t *)c->p; continue; }
        return c;
    }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) DESCR_t *plw_cell_deref(DESCR_t *c) {
    if (__builtin_expect(g_plw_poison > 0, 0)) return plw_cell_deref_slow(c);
    if (c->v != DT_N && (c->v != (DTYPE_t)DT_PLVAR || !c->p || c->p == (void *)c)) return c;
    return plw_cell_deref_slow(c);
}
extern void *rt_plj_alloc(size_t);
_Static_assert(sizeof(pl_trail_ent_t) == 24 && sizeof(DESCR_t) == 16 && offsetof(DESCR_t, slen) == 4, "PL-SINK-1: cell/trail-entry layout is baked into bb_call_fn.cpp sink_* emitters — update both together");
_Static_assert(offsetof(pl_trail_t, area) == 0 && offsetof(pl_area_t, base) == 0 && offsetof(pl_area_t, cap) == 24 && offsetof(pl_trail_t, top) == 32, "PL-SINK-1: pl_trail_t field offsets are baked into bb_call_fn.cpp sink_trailpush — update both together");
_Static_assert(offsetof(DESCR_t, v) == 0 && offsetof(DESCR_t, i) == 8 && sizeof(((pl_trail_t *)0)->top) == 4, "PL-SINK-8: $trail_mark's inline result build ({DT_I,0,(long long)top} as rax=q0, rdx=movsxd top) bakes DESCR v@0 / payload@8 and a 32-bit signed trail top — update bb_call_fn.cpp sink_trail_mark_str together");
uint32_t g_plw_dot_sl = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plw_bind(DESCR_t *cell, DESCR_t word) { pl_trail_push(&g_pl_trail, cell); *cell = word; }
static int plw_vvb_on(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_NO_VVB"); p = (e && e[0] == '1') ? 0 : 1; } return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plw_unify_cells(DESCR_t *a, DESCR_t *b) {
    DESCR_t *A = plw_cell_deref(a), *B = plw_cell_deref(b);
    if (A == B) return 1;
    int av = plw_unbound_tag(A), bv = plw_unbound_tag(B);
    if (av && bv) {
        char probe; char *floor_ = &probe;
        if (plw_vvb_on() && (char *)A > floor_ && (char *)B > floor_) {
            DESCR_t *lo = A < B ? A : B; DESCR_t *hi = A < B ? B : A;
            DESCR_t r; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)hi;
            if (hi->v != (DTYPE_t)DT_PLVAR || hi->p != (void *)hi) { DESCR_t u; u.v = (DTYPE_t)DT_PLVAR; u.slen = 0; u.p = (void *)hi; plw_bind(hi, u); }
            plw_bind(lo, r); return 1;
        }
        { DESCR_t *j = (DESCR_t *)rt_plj_alloc(sizeof(DESCR_t)); j->v = (DTYPE_t)DT_PLVAR; j->slen = 0; j->p = (void *)j; DESCR_t r; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)j; plw_bind(A, r); plw_bind(B, r); return 1; } }
    if (av) { plw_bind(A, *B); return 1; }
    if (bv) { plw_bind(B, *A); return 1; }
    if (A->v == (DTYPE_t)DT_PLREF && B->v == (DTYPE_t)DT_PLREF) {
        if (A->slen != B->slen) return 0;
        int ar = (int)(A->slen & 0xFFFFu);
        DESCR_t *aa = (DESCR_t *)A->p, *bb = (DESCR_t *)B->p;
        for (int i = 0; i < ar; i++) if (!plw_unify_cells(&aa[i], &bb[i])) return 0;
        return 1;
    }
    if (A->v == (DTYPE_t)DT_PLREF || B->v == (DTYPE_t)DT_PLREF) return 0;
    if (A->v == DT_I && B->v == DT_I && !A->slen && !B->slen) return A->i == B->i;
    { extern int rt_descr_equal(DESCR_t, DESCR_t); return rt_descr_equal(*A, *B); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) DESCR_t *plw_entry(DESCR_t *tmp) {
    if (tmp->v == DT_N && tmp->slen == 1 && tmp->p) return (DESCR_t *)tmp->p;
    if (tmp->v == DT_N && tmp->slen == 2 && tmp->p && ((VCELL_t *)tmp->p)->cellp) return ((VCELL_t *)tmp->p)->cellp;
    return tmp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((visibility("hidden"))) int plw_unify_vals(DESCR_t va, DESCR_t vb) {
    DESCR_t ta = va, tb = vb;
    return plw_unify_cells(plw_entry(&ta), plw_entry(&tb));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *plw_det_cell(DESCR_t *tmp) {
    { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); }
    DESCR_t *c = plw_cell_deref(plw_entry(tmp));
    if (c->v == DT_SNUL || c->v == DT_FAIL) { c->v = (DTYPE_t)DT_PLVAR; c->slen = 0; c->p = (void *)c; }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_deref_val(DESCR_t v) { DESCR_t t = v; return *plw_cell_deref(plw_entry(&t)); }
DESCR_t rt_pl_fresh_var_ref(void) { DESCR_t *j = (DESCR_t *)rt_plj_alloc(sizeof(DESCR_t)); j->v = (DTYPE_t)DT_PLVAR; j->slen = 0; j->p = (void *)j; DESCR_t r; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)j; return r; }
typedef struct { int m; arena_mark_t am; } plw_cwpair_t;
static plw_cwpair_t *g_plw_cwp = 0; static int g_plw_cwp_n = 0, g_plw_cwp_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plw_cw_mark_push(int m) {
    extern int rt_pl_cellws_on(void); extern arena_mark_t rt_pl_cellws_mark(void);
    if (!rt_pl_cellws_on()) return;
    while (g_plw_cwp_n > 0 && g_plw_cwp[g_plw_cwp_n - 1].m >= m) g_plw_cwp_n--;
    if (g_plw_cwp_n >= g_plw_cwp_cap) { g_plw_cwp_cap = g_plw_cwp_cap ? g_plw_cwp_cap * 2 : 64; g_plw_cwp = (plw_cwpair_t *)realloc(g_plw_cwp, (size_t)g_plw_cwp_cap * sizeof(plw_cwpair_t)); if (!g_plw_cwp) abort(); }
    g_plw_cwp[g_plw_cwp_n].m = m; g_plw_cwp[g_plw_cwp_n].am = rt_pl_cellws_mark(); g_plw_cwp_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plw_cw_kill_to(int m) {
    extern int rt_pl_cellws_on(void); extern void rt_pl_cellws_release(arena_mark_t);
    if (!rt_pl_cellws_on()) return;
    while (g_plw_cwp_n > 0 && g_plw_cwp[g_plw_cwp_n - 1].m > m) g_plw_cwp_n--;
    if (g_plw_cwp_n > 0 && g_plw_cwp[g_plw_cwp_n - 1].m == m) rt_pl_cellws_release(g_plw_cwp[g_plw_cwp_n - 1].am);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plw_zh_mark_push(int m) { plw_cw_mark_push(m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plw_zh_kill_to(int m) { plw_cw_kill_to(m); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_builtin_is_known(const char *name)
{
    if (!name || !name[0]) return 0;
    if (!strcmp(name, "$unify") || !strcmp(name, "$unify_lst") || !strcmp(name, "$ix_g") || !strcmp(name, "$mkc") || !strcmp(name, "$trail_mark") || !strcmp(name, "$trail_unwind")) return 1;
    if (!strcmp(name, "$throw") || !strcmp(name, "$catch_check") || !strcmp(name, "$unwind_nothrow") || !strcmp(name, "$existence_error")) return 1;
    if (!strncmp(name, "$is_", 4) || !strncmp(name, "$cmp_", 5) || !strncmp(name, "$ax_", 4)) return 1;
    if (!strcmp(name, "$succ") || !strcmp(name, "$plus")) return 1;
    if (!strcmp(name, "$atom_length") || !strcmp(name, "$upcase_atom") || !strcmp(name, "$downcase_atom") || !strcmp(name, "$atom_concat") || !strcmp(name, "$atom_chars") || !strcmp(name, "$atom_codes") || !strcmp(name, "$string_chars") || !strcmp(name, "$string_codes") || !strcmp(name, "$name")) return 1;
    if (!strcmp(name, "$findall_new") || !strcmp(name, "$findall_add") || !strcmp(name, "$findall_result")) return 1;
    if (!strcmp(name, "$write")) return 1;
    if (!strcmp(name, "$write2") || !strcmp(name, "$nl1") || !strcmp(name, "$nl0")) return 1;
    if (!strcmp(name, "$halt0") || !strcmp(name, "$halt1")) return 1;
    if (!strcmp(name, "$writeq2") || !strcmp(name, "$write_canonical2") || !strcmp(name, "$write_term3") || !strcmp(name, "$format3")) return 1;
    if (!strcmp(name, "$put_char") || !strcmp(name, "$tab")) return 1;
    if (!strcmp(name, "$get_char") || !strcmp(name, "$peek_char") || !strcmp(name, "$get_code") || !strcmp(name, "$peek_code") || !strcmp(name, "$put_code") || !strcmp(name, "$get_byte") || !strcmp(name, "$peek_byte") || !strcmp(name, "$put_byte")) return 1;
    if (!strcmp(name, "$unget_char") || !strcmp(name, "$unget_code") || !strcmp(name, "$unget_byte")) return 1;
    if (!strcmp(name, "$get1") || !strcmp(name, "$skip1")) return 1;
    if (!strcmp(name, "$number_atom")) return 1;
    if (!strcmp(name, "$sort") || !strcmp(name, "$msort") || !strcmp(name, "$char_type") || !strcmp(name, "$numbervars")) return 1;
    if (!strcmp(name, "$acyclic_term")) return 1;
    if (!strcmp(name, "$get_print_stream") || !strcmp(name, "$name_singleton_vars") || !strcmp(name, "$name_query_vars") || !strcmp(name, "$bind_variables")) return 1;
    if (!strcmp(name, "$writeq") || !strcmp(name, "$print") || !strcmp(name, "$write_canonical")) return 1;
    if (!strcmp(name, "$display") || !strcmp(name, "$display2") || !strcmp(name, "$print2")) return 1;
    if (!strcmp(name, "$format1") || !strcmp(name, "$format2") || !strcmp(name, "$copy_term")) return 1;
    if (!strcmp(name, "$write_term")) return 1;
    if (!strcmp(name, "$functor") || !strcmp(name, "$arg") || !strcmp(name, "$univ")) return 1;
    if (!strcmp(name, "$can_compare")) return 1;
    if (!strcmp(name, "$compare")) return 1;
    if (!strcmp(name, "$term_variables") || !strcmp(name, "$subsumes_term")) return 1;
    if (!strncmp(name, "$atop_", 6) || !strncmp(name, "$tt_", 4) || !strncmp(name, "$aop_", 5)) return 1;
    if (!strcmp(name, "$term_string") || !strncmp(name, "$agg_", 5) || !strcmp(name, "$nb_setval") || !strcmp(name, "$nb_getval")) return 1;
    if (!strcmp(name, "$write_to_atom") || !strcmp(name, "$format_to_atom") || !strcmp(name, "$read_from_atom")) return 1;
    if (!strcmp(name, "$write_to_chars") || !strcmp(name, "$write_to_codes") || !strcmp(name, "$writeq_to_atom") || !strcmp(name, "$writeq_to_chars") || !strcmp(name, "$writeq_to_codes")) return 1;
    if (!strcmp(name, "$write_canonical_to_atom") || !strcmp(name, "$write_canonical_to_chars") || !strcmp(name, "$write_canonical_to_codes")) return 1;
    if (!strcmp(name, "$display_to_atom") || !strcmp(name, "$display_to_chars") || !strcmp(name, "$display_to_codes")) return 1;
    if (!strcmp(name, "$print_to_atom") || !strcmp(name, "$print_to_chars") || !strcmp(name, "$print_to_codes")) return 1;
    if (!strcmp(name, "$write_term_to_atom") || !strcmp(name, "$write_term_to_chars") || !strcmp(name, "$write_term_to_codes")) return 1;
    if (!strcmp(name, "$format_to_chars") || !strcmp(name, "$format_to_codes") || !strcmp(name, "$read_from_chars") || !strcmp(name, "$read_from_codes")) return 1;
    if (!strcmp(name, "$read_term_from_atom") || !strcmp(name, "$read_term_from_chars") || !strcmp(name, "$read_term_from_codes")) return 1;
    if (!strcmp(name, "$wot_begin") || !strcmp(name, "$wot_end") || !strcmp(name, "$wot_abort")) return 1;
    if (!strcmp(name, "$sub_atom") || !strcmp(name, "$atom_to_term") || !strcmp(name, "$read") || !strcmp(name, "$read2")) return 1;
    if (!strcmp(name, "$read_term2") || !strcmp(name, "$read_term3")) return 1;
    if (!strcmp(name, "$bag_prep_b") || !strcmp(name, "$bag_prep_s") || !strcmp(name, "$keysort") || !strcmp(name, "$bag_group")) return 1;
    if (!strcmp(name, "$group_pairs_by_key") || !strcmp(name, "$pairs_keys_values")) return 1;
    if (!strcmp(name, "$dyn_assertz") || !strcmp(name, "$dyn_asserta") || !strcmp(name, "$retract") || !strcmp(name, "$abolish") || !strcmp(name, "$dyn_iter") || !strcmp(name, "$call")) return 1;
    if (!strcmp(name, "$clause") || !strcmp(name, "$current_predicate") || !strcmp(name, "$predicate_property")) return 1;
    if (!strcmp(name, "$current_op")) return 1;
    if (!strcmp(name, "$current_prolog_flag")) return 1;
    if (!strcmp(name, "$current_stream")) return 1;
    if (!strcmp(name, "$stream_property")) return 1;
    if (!strcmp(name, "$set_prolog_flag")) return 1;
    if (!strcmp(name, "$current_output") || !strcmp(name, "$current_input")) return 1;
    if (!strcmp(name, "$set_output") || !strcmp(name, "$set_input")) return 1;
    if (!strcmp(name, "$flush_output") || !strcmp(name, "$flush_output1")) return 1;
    if (!strcmp(name, "$open") || !strcmp(name, "$close")) return 1;
    if (!strcmp(name, "$see") || !strcmp(name, "$tell") || !strcmp(name, "$append")) return 1;
    if (!strcmp(name, "$seeing") || !strcmp(name, "$telling") || !strcmp(name, "$seen") || !strcmp(name, "$told")) return 1;
    if (!strcmp(name, "$at_end_of_stream")) return 1;
    if (!strcmp(name, "$op")) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_builtin_is_known(const char *name)
{
    if (!name) return 0;
    if (rt_proc_is_registered(name)) return 0;
    if (builtin_is_generator(name))  return 0;
    if (!strncmp(name, "__rk_test_", 10)) return 1;
    static const char *known[] = {
        "write", "writes", "stop",
        "integer", "real", "string", "numeric", "char", "chr", "ord", "cset",
        "type", "image", "proc", "args", "copy",
        "abs", "sqrt", "sin", "cos", "tan", "exp", "log",
        "asin", "acos", "atan", "dtor", "rtod",
        "max", "min",
        "trim", "reverse", "repl", "map", "left", "center", "right",
        "detab", "entab", "read", "reads",
        "sprintf", "printf",
        "iand", "ior", "ixor", "ishift", "icom",
        "table", "list", "set", "sort", "sortf", "get", "pop", "pull",
        "member", "insert", "delete", "key",
        "[]",
        "__apply__",
        "MAKELIST",
        "__rk_arr", "__rk_arr_lit", "arr_get", "arr_set_pure", "arr_init", "arr_last", "array_sort", "arr_make",
        "__rk_arr_xx", "__rk_arr_at", "__rk_arr_sort", "__rk_arr_min", "__rk_arr_max", "__rk_arr_first",
        "__rk_arr_keys", "__rk_arr_values", "__rk_range_arr", "__rk_arr_slice", "__rk_arr_pick",
        "__rk_reduce_add", "__rk_reduce_sub", "__rk_reduce_mul", "__rk_reduce_cat", "__rk_reduce_min", "__rk_reduce_max",
        "__rk_div", "rk_write", "rk_writes", "rk_write_arr", "__rk_named_call", "__rk_rep", "__rk_exit",
        "__pas_ca_pack", "__pas_ca_unpack",
        "__rk_hash",
        "elems", "push_pure", "unshift_pure", "arr_tail",
        "hash_get", "hash_set_pure", "hash_delete_pure", "hash_exists",
        "hash_keys", "hash_values", "hash_pairs", "hash_kv",
        "__rk_jct_any", "__rk_jct_all", "__rk_jct_one", "__rk_jct_none",
        "obj_new", "meth_call", "field_set", "field_set_pub", "field_get_pub",
        "die", "script_die", "srand",
        "callsame", "nextsame", "callwith",
        "__multi_call", "__param_check", "__blk_ref", "__blk_invoke",
        "TIME", "DATE",
        "IDENTICAL", "getenv", "open", "where", "close", "collect", "seek",
        "LT", "LE", "GT", "GE", "EQ", "NE", "LGT", "LLT", "LGE", "LLE", "LEQ", "LNE",
        "IDENT", "DIFFER", "SIZE", "TRIM", "DUPL", "REPLACE", "REMDR", "SNO$NAME",
        "SUBSTR", "REVERSE", "LPAD", "RPAD", "INTEGER", "DATATYPE",
        "ARRAY", "TABLE", "ITEM", "PROTOTYPE", "CONVERT", "DATA", "APPLY", "OPSYN", "VALUE", "SNO$KWSET", "SNO$NRET", "SNO$WANTNM",
        "EVAL", "SNO$MKEXPR", "SNO$MKPAT", "SNO$STMT",
        "$unify", "$unify_lst", "$ix_g",
        NULL
    };
    for (int i = 0; known[i]; i++) if (!strcmp(known[i], name)) return 1;
    {
        if (dat_find_type(name)) return 1;
    }
    { extern int rt_dat_field_of_any(const char *); if (rt_dat_field_of_any(name)) return 1; }
    { extern const char *rt_builtin_synonym(const char *); if (rt_builtin_synonym(name)) return 1; }
    return 0;
}
#define SOH '\x01'
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *itos(long long v, char *buf, size_t cap) {
    if (cap < 2) { buf[0] = '\0'; return buf; }
    if (v == 0) { buf[0] = '0'; buf[1] = '\0'; return buf; }
    char tmp[24]; int n = 0; int neg = (v < 0);
    unsigned long long u = neg ? (unsigned long long)(-(v + 1)) + 1ULL : (unsigned long long)v;
    while (u) { tmp[n++] = (char)('0' + (int)(u % 10)); u /= 10; }
    if (neg) tmp[n++] = '-';
    size_t len = (size_t)n; if (len >= cap) len = cap - 1;
    for (size_t i = 0; i < len; i++) buf[i] = tmp[len - 1 - i];
    buf[len] = '\0'; return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *rtos(double r, char *buf, size_t cap) {
    gcvt(r, 14, buf); (void)cap; return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *to_cstring(DESCR_t v, char *scratch, size_t scap) {
    if (IS_INT_fn(v))  { return itos((long long)v.i, scratch, scap); }
    if (IS_REAL_fn(v)) { return rtos(v.r, scratch, scap); }
    const char *s = VARVAL_fn(v); return s ? s : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t elem_to_descr(const char *s, size_t slen) {
    char *buf = rt_ws_alloc(slen + 1);
    memcpy(buf, s, slen); buf[slen] = '\0';
    char *ep; long iv = strtol(buf, &ep, 10);
    if (*ep == '\0' && ep > buf) return INTVAL(iv);
    return STRVAL(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int junction_is(DESCR_t v) {
    return (IS_STR_fn(v) || v.v == DT_SNUL) && v.s && v.s[0] == '\x03';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int jct_one_cmp_num(double a, double b, int op) {
    switch (op) { case TT_EQ: return a == b; case TT_NE: return a != b; case TT_LT: return a < b;
                  case TT_LE: return a <= b; case TT_GT: return a > b; case TT_GE: return a >= b;
                  default: return a == b; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int jct_one_cmp_str(const char *a, const char *b, int op) {
    int c = strcmp(a, b);
    switch (op) { case TT_EQ: return c == 0; case TT_NE: return c != 0; case TT_LT: return c < 0;
                  case TT_LE: return c <= 0; case TT_GT: return c > 0; case TT_GE: return c >= 0;
                  default: return c == 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric) {
    const char *s = jct.s; if (!s || s[0] != '\x03' || !s[1]) return 0;
    char flav = s[1];
    double sx = numeric ? to_real(scalar) : 0.0;
    const char *sstr = numeric ? NULL : VARVAL_fn(scalar); if (!sstr) sstr = "";
    int total = 0, hits = 0;
    const char *p = s + 2;
    while (*p == SOH) {
        p++; const char *start = p;
        int hit;
        if (*p == '\x03') {
            int depth = 1; p++;
            while (*p && depth > 0) { if (*p == '\x03') depth++; else if (*p == '\x04') depth--; p++; }
            size_t L = (size_t)(p - start);
            char *mb = rt_ws_alloc(L + 1); memcpy(mb, start, L); mb[L] = '\0';
            hit = junction_collapse(scalar, STRVAL(mb), op, numeric);
        } else {
            while (*p && *p != SOH && *p != '\x04') p++;
            size_t L = (size_t)(p - start);
            char mb[64]; size_t cl = L < 63 ? L : 63; memcpy(mb, start, cl); mb[cl] = '\0';
            hit = numeric ? jct_one_cmp_num(sx, strtod(mb, NULL), op) : jct_one_cmp_str(sstr, mb, op);
        }
        total++; if (hit) hits++;
    }
    switch (flav) { case 'a': return hits >= 1; case 'l': return total > 0 && hits == total;
                    case 'o': return hits == 1; case 'n': return hits == 0; default: return 0; }
}
#define GRAMMAR_MAX 128
#define RK_NAMED_MAX 32
static struct { const char *qname; const char *body; int flavor; } gram_reg[GRAMMAR_MAX];
static int gram_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gram_set(const char *qname, const char *body, int flavor) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) { gram_reg[i].body = rt_ws_strdup(body); gram_reg[i].flavor = flavor; return; }
    if (gram_n < GRAMMAR_MAX) { gram_reg[gram_n].qname = rt_ws_strdup(qname); gram_reg[gram_n].body = rt_ws_strdup(body); gram_reg[gram_n].flavor = flavor; gram_n++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gram_get_flavor(const char *qname) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) return gram_reg[i].flavor;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *gram_get(const char *qname) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) return gram_reg[i].body;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_grammar_builtin_class(const char *nm) {
    if (!nm) return NULL;
    if (!strcmp(nm, "digit"))  return "[0-9]";
    if (!strcmp(nm, "alpha"))  return "[a-zA-Z]";
    if (!strcmp(nm, "alnum"))  return "[a-zA-Z0-9]";
    if (!strcmp(nm, "upper"))  return "[A-Z]";
    if (!strcmp(nm, "lower"))  return "[a-z]";
    if (!strcmp(nm, "space"))  return "[ \\t\\n\\r]";
    if (!strcmp(nm, "ws"))     return "[ \\t\\n\\r]*";
    if (!strcmp(nm, "xdigit")) return "[0-9a-fA-F]";
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gram_expand(const char *gname, const char *body, int flavor, char *out, int outsz, int depth) {
    int op = 0, n = (int)strlen(body);
    for (int i = 0; i < n && op < outsz - 1; ) {
        char c = body[i];
        if (c == '\\' && i + 1 < n) { out[op++] = c; if (op < outsz - 1) out[op++] = body[i + 1]; i += 2; continue; }
        if (c == '\'' || c == '"') {
            char q = c; i++;
            while (i < n && body[i] != q && op < outsz - 2) {
                if (body[i] == '\\' && i + 1 < n) { out[op++] = body[i]; out[op++] = body[i+1]; i += 2; continue; }
                char lc = body[i++];
                if (strchr(".^$*+?()[]{}|\\", lc)) out[op++] = '\\';
                out[op++] = lc;
            }
            if (i < n) i++;
            continue;
        }
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            while (i < n && (body[i]==' '||body[i]=='\t'||body[i]=='\n'||body[i]=='\r')) i++;
            if (flavor == 1 && op < outsz - 3) { out[op++]='\\'; out[op++]='s'; out[op++]='*'; }
            continue;
        }
        if (c == '<' && i + 1 < n) {
            int ns = i + 1;
            if (body[ns] == '.' && ns + 1 < n && isalpha((unsigned char)body[ns + 1])) ns++;
            if (isalpha((unsigned char)body[ns])) {
                int j = ns; char nm[64]; int nl = 0;
                while (j < n && (isalnum((unsigned char)body[j]) || body[j] == '_') && nl < 63) nm[nl++] = body[j++];
                nm[nl] = '\0';
                if (j < n && body[j] == '>' && !(j + 1 < n && body[j + 1] == '(')) {
                    const char *sub = NULL; int subfl = 0;
                    if (depth < 16) { char qn[256]; snprintf(qn, sizeof qn, "%s::%s", gname, nm); sub = gram_get(qn); subfl = gram_get_flavor(qn); }
                    if (sub) {
                        char tmp[4096]; gram_expand(gname, sub, subfl, tmp, sizeof tmp, depth + 1);
                        if (op < outsz - 1) out[op++] = '(';
                        for (int k = 0; tmp[k] && op < outsz - 1; k++) out[op++] = tmp[k];
                        if (op < outsz - 1) out[op++] = ')';
                        i = j + 1; continue;
                    }
                    const char *bc = rk_grammar_builtin_class(nm);
                    if (bc) {
                        for (int k = 0; bc[k] && op < outsz - 1; k++) out[op++] = bc[k];
                        i = j + 1; continue;
                    }
                }
            }
        }
        out[op++] = c; i++;
    }
    out[op] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_grammar_register(const char *qname, const char *body, int flavor) { if (qname && body) gram_set(qname, body, flavor); }
int rt_grammar_count(void) { return gram_n; }
const char *rt_grammar_qname(int i) { return (i >= 0 && i < gram_n) ? gram_reg[i].qname : NULL; }
const char *rt_grammar_body(int i) { return (i >= 0 && i < gram_n) ? gram_reg[i].body : NULL; }
int rt_grammar_flavor(int i) { return (i >= 0 && i < gram_n) ? gram_reg[i].flavor : 0; }
int rt_grammar_has_top(const char *gname) { if (!gname) return 0; char qn[256]; snprintf(qn, sizeof qn, "%s::TOP", gname); return gram_get(qn) != NULL; }
extern DESCR_t rk_gram_enter_box(bb_box_fn fn, const char *sigma, long delta, void *zeta, long *out_delta);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_gram_run_native(bb_box_fn bf, const char *subj, DESCR_t *out) {
    long Delta = (long)strlen(subj); long final_delta = 0; char fb[256] __attribute__((aligned(16))); memset(fb, 0, sizeof fb);
    DESCR_t r = rk_gram_enter_box(bf, subj, Delta, (void *)fb, &final_delta);
    int matched = (r.v != DT_FAIL); int full = matched && (final_delta == Delta);
    *out = full ? STRVAL(rt_ws_strdup_c(subj)) : NULVCL; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int grammar_parse_core(const char *gname, const char *subj, DESCR_t *out) {
    if (!gname) gname = ""; if (!subj) subj = "";
    { char gpn[320]; snprintf(gpn, sizeof gpn, "gram__%s__TOP", gname);
      extern void *rt_proc_get_fn(const char *); bb_box_fn bf = (bb_box_fn)rt_proc_get_fn(gpn); if (bf) return rk_gram_run_native(bf, subj, out); }
    char qn[256]; snprintf(qn, sizeof qn, "%s::TOP", gname);
    const char *body = gram_get(qn);
    if (!body) { *out = FAILDESCR; return 1; }
    int topflv = gram_get_flavor(qn);
    char pat[4096]; gram_expand(gname, body, topflv, pat, sizeof pat, 0);
    Nfa *nfa = nfa_build(pat);
    if (!nfa) { *out = FAILDESCR; return 1; }
    Match m; nfa_exec(nfa, subj, &m);
    nfa_free(nfa);
    int slen = (int)strlen(subj);
    int ok = m.matched && m.full_start == 0 && m.full_end == slen;
    *out = ok ? STRVAL(rt_ws_strdup_c(subj)) : NULVCL; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_str_method(const char *meth, DESCR_t recv, const DESCR_t *margs, int nmargs, DESCR_t *out) {
    if (!meth || !*meth) return 0;
    char sb[64]; const char *s = to_cstring(recv, sb, sizeof sb); if (!s) s = ""; size_t n = strlen(s);
    if (!strcmp(meth, "chars")) { *out = INTVAL((long)utf8_strlen(s)); return 1; }
    if (!strcmp(meth, "uc")) { char *r = (char *)rt_ws_alloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = (char)toupper((unsigned char)s[i]); r[n] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "lc") || !strcmp(meth, "fc")) {
        char *r = (char *)rt_ws_alloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = (char)tolower((unsigned char)s[i]); r[n] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "tc")) { char *r = (char *)rt_ws_alloc(n + 1); memcpy(r, s, n + 1); if (n > 0) r[0] = (char)toupper((unsigned char)r[0]); *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "tclc")) {
        char *r = (char *)rt_ws_alloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = (char)tolower((unsigned char)s[i]); r[n] = '\0'; if (n > 0) r[0] = (char)toupper((unsigned char)r[0]);
        *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "flip")) { char *r = (char *)rt_ws_alloc(n + 1); for (size_t i = 0; i < n; i++) r[i] = s[n - 1 - i]; r[n] = '\0'; *out = STRVAL(r); return 1; }
    if (!strcmp(meth, "trim")) {
        size_t a = 0, b = n; while (a < b && isspace((unsigned char)s[a])) a++; while (b > a && isspace((unsigned char)s[b - 1])) b--; char *r = (char *)rt_ws_alloc(b - a + 1);
        memcpy(r, s + a, b - a); r[b - a] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "Str")) { *out = STRVAL(rt_ws_strdup_c(s)); return 1; }
    if (!strcmp(meth, "Int")) { if (IS_INT_fn(recv)) { *out = recv; return 1; } if (IS_REAL_fn(recv)) { *out = INTVAL((long)recv.r); return 1; } *out = INTVAL((long)atoll(s)); return 1; }
    if (!strcmp(meth, "contains") && nmargs >= 1) { char nb[64]; const char *nd = to_cstring(margs[0], nb, sizeof nb); if (!nd) nd = ""; *out = INTVAL(strstr(s, nd) ? 1 : 0); return 1; }
    if (!strcmp(meth, "starts-with") && nmargs >= 1) { char nb[128]; const char *nd = to_cstring(margs[0], nb, sizeof nb); if (!nd) nd = ""; size_t ln = strlen(nd); *out = INTVAL((ln <= n && !strncmp(s, nd, ln)) ? 1 : 0); return 1; }
    if (!strcmp(meth, "ends-with") && nmargs >= 1) { char nb[128]; const char *nd = to_cstring(margs[0], nb, sizeof nb); if (!nd) nd = ""; size_t ln = strlen(nd); *out = INTVAL((ln <= n && !strncmp(s + (n - ln), nd, ln)) ? 1 : 0); return 1; }
    if (!strcmp(meth, "subst") && nmargs >= 1) {
        char nb[256]; const char *needle = to_cstring(margs[0], nb, sizeof nb); if (!needle) needle = "";
        char rb[256]; const char *repl = (nmargs >= 2) ? to_cstring(margs[1], rb, sizeof rb) : ""; if (!repl) repl = "";
        size_t nl = strlen(needle), rl = strlen(repl);
        const char *hit = (nl > 0) ? strstr(s, needle) : NULL;
        if (!hit) { char *o = (char *)rt_ws_alloc(n + 1); memcpy(o, s, n + 1); *out = STRVAL(o); return 1; }
        size_t pre = (size_t)(hit - s); char *o = (char *)rt_ws_alloc(n - nl + rl + 1);
        memcpy(o, s, pre); memcpy(o + pre, repl, rl); memcpy(o + pre + rl, hit + nl, n - pre - nl); o[n - nl + rl] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(meth, "index") && nmargs >= 1) {
        char nb[64]; const char *nd = to_cstring(margs[0], nb, sizeof nb); if (!nd) nd = ""; const char *hit = strstr(s, nd); *out = hit ? INTVAL((long)(hit - s)) : NULVCL; return 1;
    }
    if (!strcmp(meth, "substr") && nmargs >= 1) {
        long from = IS_INT_fn(margs[0]) ? (long)margs[0].i : atol(to_cstring(margs[0], sb, sizeof sb));
        long ln = (nmargs >= 2) ? (IS_INT_fn(margs[1]) ? (long)margs[1].i : atol(to_cstring(margs[1], sb, sizeof sb))) : (long)utf8_strlen(s) - from; if (from < 0) from = 0; if (ln < 0) ln = 0;
        long avail = (long)utf8_strlen(s) - from; if (avail < 0) avail = 0; if (ln > avail) ln = avail;
        if (ln == 0) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        *out = SUBSTR_fn(recv, INTVAL(from + 1), INTVAL(ln)); return 1;
    }
    if (!strcmp(meth, "chr")) { long cp = IS_INT_fn(recv) ? (long)recv.i : (long)atoll(s); char *r = (char *)rt_ws_alloc(2); r[0] = (char)(cp & 0xFF); r[1] = '\0'; *out = BSTRVAL(r, 1); return 1; }
    if (!strcmp(meth, "ord")) {
        /* NOT the shared `n` above (strlen(s), computed before this dispatch knew which method it would
           serve): a single character extracted from a cset/string containing chr(0) has strlen()==0
           despite being one real character (icon-ascii-cset-keywords-built-off-by-one). descr_slen(recv)
           reads the DESCRIPTOR's own stamped length instead of re-deriving one from the (here, misleading)
           byte content. */
        size_t on = descr_slen(recv);
        if (!s || !on) { *out = FAILDESCR; return 1; }
        *out = INTVAL((unsigned char)s[0]); return 1;
    }
    if (!strcmp(meth, "abs")) { if (IS_INT_fn(recv)) { long v = (long)recv.i; *out = INTVAL(v < 0 ? -v : v); } else { *out = REALVAL(fabs(to_real(recv))); } return 1; }
    if (!strcmp(meth, "floor")) { *out = INTVAL((long)floor(to_real(recv))); return 1; }
    if (!strcmp(meth, "ceiling")) { *out = INTVAL((long)ceil(to_real(recv))); return 1; }
    if (!strcmp(meth, "round")) { *out = INTVAL((long)floor(to_real(recv) + 0.5)); return 1; }
    if (!strcmp(meth, "rand")) { *out = REALVAL((double)rand() / RAND_MAX * to_real(recv)); return 1; }
    if (!strcmp(meth, "Bool") || !strcmp(meth, "so") || !strcmp(meth, "not")) {
        int truthy; if (IS_INT_fn(recv)) truthy = (recv.i != 0); else if (IS_REAL_fn(recv)) truthy = (recv.r != 0.0); else truthy = (n > 0);
        *out = INTVAL(!strcmp(meth, "not") ? (truthy ? 0 : 1) : (truthy ? 1 : 0)); return 1;
    }
    if (!strcmp(meth, "defined")) { *out = INTVAL(recv.v != DT_SNUL ? 1 : 0); return 1; }
    if ((!strcmp(meth, "succ") || !strcmp(meth, "pred")) && (IS_INT_fn(recv) || IS_REAL_fn(recv))) {
        int d = !strcmp(meth, "succ") ? 1 : -1; if (IS_INT_fn(recv)) *out = INTVAL((long)recv.i + d); else *out = REALVAL(recv.r + d); return 1;
    }
    if (!strcmp(meth, "words")) {
        char *r = (char *)rt_ws_alloc(n + 1); int op = 0, first = 1; size_t i = 0;
        while (i < n) {
            while (i < n && isspace((unsigned char)s[i])) i++; if (i >= n) break; if (!first) r[op++] = SOH; first = 0; while (i < n && !isspace((unsigned char)s[i])) r[op++] = s[i++];
        } r[op] = '\0';
        *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "comb")) {
        char *r = (char *)rt_ws_alloc(2 * n + 1); int op = 0;
        for (size_t i = 0; i < n; ) { int cl = utf8_seqlen((unsigned char)s[i]); if (i) r[op++] = SOH; for (int k = 0; k < cl && i < n; k++) r[op++] = s[i++]; } r[op] = '\0'; *out = STRVAL(r);
        return 1;
    }
    if (!strcmp(meth, "split") && nmargs >= 1) {
        char sept[64]; const char *sep = to_cstring(margs[0], sept, sizeof sept); if (!sep) sep = ""; size_t sl = strlen(sep); char *r = (char *)rt_ws_alloc(2 * n + 2); int op = 0;
        if (sl == 0) {
            for (size_t i = 0; i < n; ) { int cl = utf8_seqlen((unsigned char)s[i]); if (i) r[op++] = SOH; for (int k = 0; k < cl && i < n; k++) r[op++] = s[i++]; }
        } else { const char *p = s, *hit;
        while ((hit = strstr(p, sep)) != NULL) { size_t sg = (size_t)(hit - p); memcpy(r + op, p, sg); op += (int)sg; r[op++] = SOH; p = hit + sl; } size_t tl = strlen(p); memcpy(r + op, p, tl);
        op += (int)tl; } r[op] = '\0';
        *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "chomp")) {
        size_t b = n; if (b > 0 && (s[b - 1] == '\n' || s[b - 1] == '\r')) b--; char *r = (char *)rt_ws_alloc(b + 1); memcpy(r, s, b); r[b] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "wordcase")) {
        char *r = (char *)rt_ws_alloc(n + 1); memcpy(r, s, n + 1); int start = 1;
        for (size_t i = 0; i < n; i++) { if (isspace((unsigned char)r[i])) start = 1; else { if (start) r[i] = (char)toupper((unsigned char)r[i]); start = 0; } } *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "lines")) {
        char *r = (char *)rt_ws_alloc(2 * n + 1); int op = 0, first = 1; size_t i = 0;
        while (i < n) { if (!first) r[op++] = SOH; first = 0; while (i < n && s[i] != '\n') { if (s[i] != '\r') r[op++] = s[i]; i++; } if (i < n) i++; } r[op] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "elems")) { if (n == 0) { *out = INTVAL(0); return 1; } int c = 1; for (size_t i = 0; i < n; i++) if (s[i] == SOH) c++; *out = INTVAL(c); return 1; }
    if (!strcmp(meth, "trans") && nmargs >= 1) {
        /* Str.trans(@from => @to): substring-replace scan, matched against the "one Pair, list-key,
           list-value" shape __rk_pair builds -- the combined SOH-joined element list is split evenly
           in half by COUNT (first half = FROM patterns, second half = TO replacements, zipped by
           position). Not general Raku .trans (no multi-pair lists, no character ranges) -- the
           minimal version the one real caller (string-escape.raku) needs. */
        const char *lst = VARVAL_fn(margs[0]); if (!lst) lst = "";
        int total = lst[0] ? 1 : 0; for (const char *p = lst; *p; p++) if (*p == SOH) total++;
        if (total < 2 || (total % 2) != 0) { char *r = (char *)rt_ws_alloc(n + 1); memcpy(r, s, n + 1); *out = STRVAL(r); return 1; }
        int half = total / 2; if (half > 128) half = 128;
        const char *from[128]; size_t fromlen[128]; const char *to[128]; size_t tolen[128];
        size_t maxto = 1; int idx = 0; const char *seg = lst;
        for (;;) {
            const char *nx = strchr(seg, SOH); size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            if (idx < half) { from[idx] = seg; fromlen[idx] = L; }
            else if (idx - half < 128) { to[idx - half] = seg; tolen[idx - half] = L; if (L > maxto) maxto = L; }
            idx++; if (!nx) break; seg = nx + 1;
        }
        char *r = (char *)rt_ws_alloc(n * maxto + n + 1); size_t op = 0, i = 0;
        while (i < n) {
            int matched = -1;
            for (int k = 0; k < half; k++) { if (fromlen[k] > 0 && i + fromlen[k] <= n && memcmp(s + i, from[k], fromlen[k]) == 0) { matched = k; break; } }
            if (matched >= 0) { memcpy(r + op, to[matched], tolen[matched]); op += tolen[matched]; i += fromlen[matched]; }
            else { r[op++] = s[i++]; }
        }
        r[op] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "join")) {
        const char *sep = ""; char jb[64]; if (nmargs >= 1) { sep = to_cstring(margs[0], jb, sizeof jb); if (!sep) sep = ""; } size_t sl = strlen(sep); int nsep = 0;
        for (size_t i = 0; i < n; i++) if (s[i] == SOH) nsep++; char *r = (char *)rt_ws_alloc(n + (size_t)nsep * sl + 1); int op = 0;
        for (size_t i = 0; i < n; i++) { if (s[i] == SOH) { memcpy(r + op, sep, sl); op += (int)sl; } else r[op++] = s[i]; } r[op] = '\0'; *out = STRVAL(r); return 1;
    }
    if (!strcmp(meth, "fmt")) {
        extern void rk_sprintf_core(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen);
        char fb[256]; const char *fmt = (nmargs >= 1) ? to_cstring(margs[0], fb, sizeof fb) : "%s"; if (!fmt) fmt = "%s";
        DESCR_t fa[2]; fa[0] = margs ? margs[0] : NULVCL; fa[1] = recv;
        char *r = NULL; size_t rl = 0; rk_sprintf_core(fmt, fa, 2, 1, &r, &rl); *out = BSTRVAL(r, rl); return 1;
    }
    return 0;
}
static long g_pas_heap_ctr = 0;
static DESCR_t *g_pas_heap = (DESCR_t *)0;
static long g_pas_heap_cap = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *pas_heap_cell(long n, int grow) {
    if (n <= 0) return (DESCR_t *)0;
    if (n >= g_pas_heap_cap) {
        if (!grow) return (DESCR_t *)0;
        long nc = g_pas_heap_cap ? g_pas_heap_cap : 1024;
        while (nc <= n) nc *= 2;
        DESCR_t *nh = (DESCR_t *)(g_pas_heap ? rt_ws_realloc(g_pas_heap, (size_t)nc * sizeof(DESCR_t)) : rt_ws_alloc((size_t)nc * sizeof(DESCR_t)));
        if (!nh) return (DESCR_t *)0;
        memset(nh + g_pas_heap_cap, 0, (size_t)(nc - g_pas_heap_cap) * sizeof(DESCR_t));
        g_pas_heap = nh; g_pas_heap_cap = nc;
    }
    return &g_pas_heap[n];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *resolve_method_chain(const char *cls, const char *mname, char *buf, int bufsz, int *pfound) {
    extern int dat_mro(const char *name, const char **out, int max);
    extern int dat_roles(const char *name, const char **out, int max);
    extern int rt_proc_has_native_fn(const char *name);
    if (pfound) *pfound = -1;
    const char *chain[64]; int nchain = dat_mro(cls, chain, 64);
    if (nchain == 0) { chain[0] = cls; nchain = 1; }
    for (int ci = 0; ci < nchain; ci++) {
        snprintf(buf, bufsz, "%s__%s", chain[ci], mname);
        int found = rt_proc_has_native_fn(buf);
        if (!found) for (int pi = 0; pi < g_stage2.proc_count; pi++)
            if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, buf)) { found = 1; break; }
        if (found) { if (pfound) *pfound = ci; return buf; }
        const char *roles[8]; int nr = dat_roles(chain[ci], roles, 8);
        for (int ri = 0; ri < nr; ri++) {
            snprintf(buf, bufsz, "%s__%s", roles[ri], mname);
            int rf = rt_proc_has_native_fn(buf);
            if (!rf) for (int pi = 0; pi < g_stage2.proc_count; pi++)
                if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, buf)) { rf = 1; break; }
            if (rf) { if (pfound) *pfound = ci; return buf; }
        }
    }
    snprintf(buf, bufsz, "%s__%s", cls, mname);
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int meth_is_user_proc(const char *procname) {
    extern int rt_proc_has_native_fn(const char *name);
    if (procname && rt_proc_has_native_fn(procname)) return 1;
    if (procname) for (int pi = 0; pi < g_stage2.proc_count; pi++)
        if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, procname)) return 1;
    return 0;
}
typedef struct { DESCR_t self; char mname[128]; const char *mro[64]; int mro_len; int found_idx; DESCR_t args[16]; int nargs; } RedispFrame;
static RedispFrame g_redisp[64];
static int g_redisp_top = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t invoke_method_proc(const char *procname, DESCR_t *callargs, int total) {
    int pi;
    for (pi = 0; pi < g_stage2.proc_count; pi++)
        if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, procname) == 0) break;
    if (pi >= g_stage2.proc_count) {
        extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
        for (int k = 0; k < total && k < 64; k++) g_call_args[k] = callargs[k];
        return rt_call_proc_descr(procname, total);
    }
    if (g_stage2.proc_table[pi].bb_idx >= 0) {
        extern DESCR_t ir_call_proc(int pix, DESCR_t *a, int na); extern int rt_proc_has_native_fn(const char *name);
        if (rt_proc_has_native_fn(procname)) {
            extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
            for (int k = 0; k < total && k < 64; k++) g_call_args[k] = callargs[k];
            return rt_call_proc_descr(procname, total);
        }
        return ir_call_proc(pi, callargs, total);
    }
    return proc_table_call(pi, callargs, total);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_fire_buildplan_tweak(const char *cname, DESCR_t self) {
    extern int dat_mro(const char *name, const char **out, int max);
    extern int rt_proc_has_native_fn(const char *name);
    if (!cname || !*cname) return;
    const char *chain[64]; int n = dat_mro(cname, chain, 64);
    if (n == 0) { chain[0] = cname; n = 1; }
    for (int i = n - 1; i >= 0; i--) {
        char proc[256]; snprintf(proc, sizeof proc, "%s__TWEAK", chain[i]);
        if (!meth_is_user_proc(proc)) continue;
        int pi; for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, proc)) break;
        if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(proc)) {
            extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
            g_call_args[0] = self; rt_call_proc_descr(proc, 1);
        } else {
            extern DESCR_t ir_call_proc(int pix, DESCR_t *a, int na); DESCR_t a0 = self; ir_call_proc(pi, &a0, 1);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_fire_build(const char *cname, DESCR_t self, DESCR_t *named, int nnamed) {
    extern int dat_mro(const char *name, const char **out, int max);
    extern int dat_class_has_build(const char *cls);
    extern int dat_build_keys(const char *cls, const char **out, int max);
    extern int rt_proc_has_native_fn(const char *name);
    if (!cname || !*cname) return;
    const char *chain[64]; int n = dat_mro(cname, chain, 64);
    if (n == 0) { chain[0] = cname; n = 1; }
    for (int i = n - 1; i >= 0; i--) {
        if (!dat_class_has_build(chain[i])) continue;
        char proc[256]; snprintf(proc, sizeof proc, "%s__BUILD", chain[i]);
        if (!meth_is_user_proc(proc)) continue;
        const char *keys[16]; int nk = dat_build_keys(chain[i], keys, 16);
        DESCR_t callargs[20]; callargs[0] = self;
        for (int k = 0; k < nk && k < 19; k++) { DESCR_t v = NULVCL;
            for (int ci = 0; ci + 1 < nnamed; ci += 2) { const char *kn = VARVAL_fn(named[ci]); if (kn && keys[k] && !strcmp(kn, keys[k])) { v = named[ci + 1]; break; } }
            callargs[1 + k] = v; }
        int total = 1 + nk;
        int pi; for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, proc)) break;
        if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(proc)) {
            extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
            for (int k = 0; k < total && k < 64; k++) g_call_args[k] = callargs[k]; rt_call_proc_descr(proc, total);
        } else {
            extern DESCR_t ir_call_proc(int pix, DESCR_t *a, int na); ir_call_proc(pi, callargs, total);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pas_nrec_subrec_set(const char *cur, long fi, long ei, const char *val) {
    if (!cur) cur = ""; if (!val) val = ""; if (fi < 0) return rt_ws_strdup(cur); if (ei < 0) ei = 0;
    const char *s = cur; long k = 0; const char *fstart = NULL; const char *fend = NULL;
    for (;;) { const char *nx = strchr(s, SOH); if (k == fi) { fstart = s; fend = nx; break; } if (!nx) { fstart = NULL; break; } s = nx + 1; k++; }
    if (!fstart) return rt_ws_strdup(cur);
    size_t flen = fend ? (size_t)(fend - fstart) : strlen(fstart);
    char *field = rt_ws_alloc(flen + 1); memcpy(field, fstart, flen); field[flen] = '\0';
    long nsub = 1; for (size_t j = 0; j < flen; j++) if (field[j] == '\x05') nsub++;
    long want = (ei + 1 > nsub) ? ei + 1 : nsub;
    const char **elems = (const char **)rt_ws_alloc((size_t)want * sizeof(char *));
    const char *p = field; long ix = 0;
    for (;;) {
        const char *nx = strchr(p, '\x05'); size_t el = nx ? (size_t)(nx - p) : strlen(p); char *e = rt_ws_alloc(el + 1); memcpy(e, p, el); e[el] = '\0'; if (ix < want) elems[ix] = e; ix++;
        if (!nx) break; p = nx + 1;
    }
    for (long j = nsub; j < want; j++) elems[j] = "0";
    elems[ei] = val;
    size_t newflen = 0; for (long j = 0; j < want; j++) newflen += strlen(elems[j]); newflen += (size_t)(want - 1);
    char *newf = rt_ws_alloc(newflen + 1); size_t fp = 0;
    for (long j = 0; j < want; j++) { if (j) newf[fp++] = '\x05'; size_t L = strlen(elems[j]); memcpy(newf + fp, elems[j], L); fp += L; } newf[fp] = '\0';
    size_t pre = (size_t)(fstart - cur); size_t post = fend ? strlen(fend) : 0;
    char *o = rt_ws_alloc(pre + fp + post + 1); memcpy(o, cur, pre); memcpy(o + pre, newf, fp); if (fend) memcpy(o + pre + fp, fend, post); o[pre + fp + post] = '\0';
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rt_mc_type_name(DESCR_t d) {
    switch (d.v) {
    case DT_I: return "Int";
    case DT_R: return "Num";
    case DT_S: return "Str";
    case DT_DATA: { if (d.u && d.u->type && d.u->type->name) return d.u->type->name; return "Any"; }
    default: return "Any";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_mc_is_subtype(const char *a, const char *b) {
    if (!a || !b) return 0;
    if (!strcmp(b, "Any") || !strcmp(b, "Mu") || !strcmp(b, "Cool")) return 1;
    if (!strcmp(a, b)) return 1;
    int a_numleaf = (!strcmp(a, "Int") || !strcmp(a, "Num") || !strcmp(a, "Rat"));
    if ((!strcmp(b, "Numeric") || !strcmp(b, "Real")) && a_numleaf) return 1;
    { extern int dat_mro(const char *name, const char **out, int max); const char *mro[64]; int n = dat_mro(a, mro, 64);
      for (int i = 0; i < n; i++) if (mro[i] && !strcmp(mro[i], b)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_mc_accepts(const char *ptype, DESCR_t arg) {
    if (!ptype || !strcmp(ptype, "Any") || !strcmp(ptype, "Mu")) return 1;
    int require_def = 0, require_undef = 0;
    const char *colon = strrchr(ptype, ':');
    const char *under = strrchr(ptype, '_');
    char base[64]; base[0] = 0;
    if (colon && (!strcmp(colon, ":D") || !strcmp(colon, ":U"))) {
        require_def   = !strcmp(colon, ":D");
        require_undef = !strcmp(colon, ":U");
        int bl = (int)(colon - ptype); if (bl > 63) bl = 63; memcpy(base, ptype, (size_t)bl); base[bl] = 0;
    } else if (under && (!strcmp(under, "_D") || !strcmp(under, "_U"))) {
        require_def   = !strcmp(under, "_D");
        require_undef = !strcmp(under, "_U");
        int bl = (int)(under - ptype); if (bl > 63) bl = 63; memcpy(base, ptype, (size_t)bl); base[bl] = 0;
    } else { snprintf(base, sizeof base, "%s", ptype); }
    int is_undef = (arg.v == DT_SNUL);
    if (require_def   && is_undef) return 0;
    if (require_undef) return is_undef;
    if (!strcmp(base, "Any") || !strcmp(base, "Mu") || !strcmp(base, "Cool")) return 1;
    return rt_mc_is_subtype(rt_mc_type_name(arg), base);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_mc_narrower(char (*ta)[32], char (*tb)[32], int na) {
    int narrower = 0, tied = 0;
    for (int i = 0; i < na; i++) {
        const char *A = ta[i], *B = tb[i];
        if (!strcmp(A, B)) { tied++; continue; }
        const char *cA = strrchr(A, ':'), *cB = strrchr(B, ':');
        if (!cA || (!strcmp(cA,":D") && !strcmp(cA,":U"))) cA = strrchr(A, '_');
        if (!cB || (!strcmp(cB,":D") && !strcmp(cB,":U"))) cB = strrchr(B, '_');
        int aD = cA && (!strcmp(cA, ":D")||!strcmp(cA, "_D")), aU = cA && (!strcmp(cA, ":U")||!strcmp(cA, "_U"));
        int bD = cB && (!strcmp(cB, ":D")||!strcmp(cB, "_D")), bU = cB && (!strcmp(cB, ":U")||!strcmp(cB, "_U"));
        char baseA[32], baseB[32];
        { int l = cA && (aD||aU) ? (int)(cA-A) : (int)strlen(A); if(l>31)l=31; memcpy(baseA,A,l); baseA[l]=0; }
        { int l = cB && (bD||bU) ? (int)(cB-B) : (int)strlen(B); if(l>31)l=31; memcpy(baseB,B,l); baseB[l]=0; }
        int same_base = !strcmp(baseA, baseB);
        if (same_base && (aD||aU) && !(bD||bU)) { narrower++; continue; }
        if (same_base && !(aD||aU) && (bD||bU)) { continue; }
        if (rt_mc_is_subtype(baseA, baseB) && !rt_mc_is_subtype(baseB, baseA)) narrower++;
        else if (!rt_mc_is_subtype(baseA, baseB) && !rt_mc_is_subtype(baseB, baseA)) tied++;
    }
    return narrower > 0 && (narrower + tied == na);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_multi_meth_dispatch(const char *cname, const char *mname, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!cname || !mname) return 0;
    int nm = nargs - 2; if (nm < 0) return 0; DESCR_t *ma = &args[2];
    extern int dat_mro(const char *name, const char **out, int max);
    extern int rt_proc_enum_count(void); extern const char *rt_proc_enum_name(int i);
    const char *mro[64]; int mn = dat_mro(cname, mro, 64); if (mn == 0) { mro[0] = cname; mn = 1; }
    static char acc_names[256][192]; static char acc_types[256][8][32]; int nacc = 0;
    int pcount = rt_proc_enum_count();
    for (int ci = 0; ci < mn && nacc < 256; ci++) {
        if (!mro[ci]) continue;
        char prefix[192]; int pl = snprintf(prefix, sizeof prefix, "%s__%s$", mro[ci], mname);
        for (int pi = 0; pi < pcount && nacc < 256; pi++) {
            const char *pn = rt_proc_enum_name(pi);
            if (!pn || strncmp(pn, prefix, (size_t)pl)) continue;
            const char *p = pn + pl; const char *e = strchr(p, '$');
            int arity = atoi(p); if (arity != nm) continue;
            int nt = 0; const char *q = e ? e + 1 : (const char *)0;
            while (q && nt < 8) {
                const char *nx = strchr(q, '$'); int len = nx ? (int)(nx - q) : (int)strlen(q); if (len > 31) len = 31; memcpy(acc_types[nacc][nt], q, (size_t)len); acc_types[nacc][nt][len] = 0;
                nt++; if (!nx) break; q = nx + 1;
            }
            int ok = 1; for (int i = 0; i < nm && i < nt; i++) if (!rt_mc_accepts(acc_types[nacc][i], ma[i])) { ok = 0; break; }
            if (!ok) continue;
            int dup = 0; for (int k = 0; k < nacc; k++) if (!strcmp(acc_names[k], pn)) { dup = 1; break; }
            if (dup) continue;
            snprintf(acc_names[nacc], sizeof acc_names[nacc], "%s", pn); nacc++;
        }
    }
    if (nacc == 0) return 0;
    int win = -1;
    for (int i = 0; i < nacc; i++) { int beaten = 0;
        for (int j = 0; j < nacc; j++) { if (i == j) continue; if (rt_mc_narrower(acc_types[j], acc_types[i], nm)) { beaten = 1; break; } }
        if (!beaten) { win = i; break; } }
    if (win < 0) win = 0;
    int total = 1 + nm; DESCR_t *ca = rt_ws_alloc((size_t)total * sizeof(DESCR_t));
    ca[0] = args[0]; for (int k = 0; k < nm; k++) ca[1 + k] = ma[k];
    *out = invoke_method_proc(acc_names[win], ca, total); return 1;
}
DESCR_t rt_call_arr(const char *fn, DESCR_t *args, int nargs);
int try_call_builtin_by_name_bl(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, int bidlen);
DESCR_t rt_call_arr_bl(const char *fn, DESCR_t *args, int nargs, int bidlen);
extern const char *icon_real_str(double r, char *buf, int bufsz);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_real_str(double r, char *buf, int bufsz) {
    if (isfinite(r) && r == floor(r) && fabs(r) < 1e15) { snprintf(buf, (size_t)bufsz, "%lld", (long long)r); return buf; }
    return icon_real_str(r, buf, bufsz);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *icn_pad_str(DESCR_t d, char *buf, int bufsz) {
    if (IS_REAL_fn(d)) return icon_real_str(d.r, buf, bufsz);
    return VARVAL_fn(d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * procval_name(DESCR_t v) {
    if (v.v != DT_E) return 0;
    if (v.slen == 0xFFFFFFFEu) return v.s;
    for (int i = 0; i < g_stage2.proc_count; i++)
        if (g_stage2.proc_table[i].entry_pc == (int)v.i) return g_stage2.proc_table[i].name;
    return 0;
}
extern int rt_proc_is_registered(const char *name);
extern int rt_proc_nparams(const char *name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_value(DESCR_t callee, DESCR_t *argv, int n) {
    if (IS_INT_fn(callee)) { long i = (long)callee.i; if (i < 0) i = n + i + 1; if (i >= 1 && i <= n) return argv[i - 1]; return FAILDESCR; }
    const char *nm = procval_name(callee);
    if (!nm && IS_STR_fn(callee) && callee.s) nm = callee.s;
    if (!nm) return FAILDESCR;
    if (rt_proc_is_registered(nm) || !strcmp(nm, "main")) {
        extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
        for (int k = 0; k < n && k < 64; k++) g_call_args[k] = argv[k]; for (int k = (n < 0 ? 0 : n); k < 64; k++) g_call_args[k] = (DESCR_t){0};
        return rt_call_proc_descr(nm, n);
    }
    return rt_call_arr(nm, argv, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_value_gen_h(DESCR_t callee, DESCR_t *argv, int n, void **hslot) {
    if (hslot) *hslot = (void *)0;
    if (IS_INT_fn(callee)) { long i = (long)callee.i; if (i < 0) i = n + i + 1; if (i >= 1 && i <= n) return argv[i - 1]; return FAILDESCR; }
    const char *nm = procval_name(callee);
    if (!nm && IS_STR_fn(callee) && callee.s) nm = callee.s;
    if (!nm) return FAILDESCR;
    if (rt_proc_is_registered(nm)) {
        extern DESCR_t g_call_args[]; extern DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **hout);
        for (int k = 0; k < n && k < 64; k++) g_call_args[k] = argv[k]; for (int k = (n < 0 ? 0 : n); k < 64; k++) g_call_args[k] = (DESCR_t){0};
        return rt_proc_call_gen_h(nm, n, hslot);
    }
    return rt_call_value(callee, argv, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_apply_gen_h(DESCR_t callee, DESCR_t lv, void **hslot) {
    DESCR_t buf[64]; int n = 0;
    if (lv.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(lv, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
            int ln = (int)FIELD_GET_fn(lv, "frame_size").i;
            DESCR_t ea = FIELD_GET_fn(lv, "frame_elems");
            DESCR_t *arr = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
            if (ln > 64) ln = 64;
            if (arr) { for (int k = 0; k < ln; k++) buf[k] = arr[k]; n = ln; }
        } else { buf[0] = lv; n = 1; }
    } else { buf[0] = lv; n = 1; }
    return rt_call_value_gen_h(callee, buf, n, hslot);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_call_value_spine_prep(DESCR_t callee, DESCR_t *argv, int n) {
    extern int rt_proc_jmp_entry(const char *name); extern void *rt_proc_fn(const char *name); extern long rt_proc_call_open(const char *name, int nargs);
    const char *nm = procval_name(callee);
    if (!nm && IS_STR_fn(callee) && callee.s) nm = callee.s;
    if (!nm || !rt_proc_is_registered(nm) || !rt_proc_jmp_entry(nm) || !rt_proc_is_generator(nm)) return (void *)0;
    { extern int rt_proc_gen_region_ft(const char *); if (rt_proc_gen_region_ft(nm) > 0) return (void *)0; }   /* N-2 (ceo s283h): bb_call_value's spine transfer pushes only the wire pair, so a region-resident callee would read garbage at [rsp+16] (seat10's apply-call SIGSEGV). Route N-2 generators through rt_proc_call_gen_h's coexpr window, whose n2 entry shim supplies a real region; the direct spine fast path for apply is its own follow-on row. */
    { extern DESCR_t g_call_args[]; for (int k = 0; k < n && k < 64; k++) g_call_args[k] = argv[k]; for (int k = (n < 0 ? 0 : n); k < 64; k++) g_call_args[k] = (DESCR_t){0}; }
    if (!rt_proc_call_open(nm, n)) return (void *)0;
    return rt_proc_fn(nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_call_apply_spine_prep(DESCR_t callee, DESCR_t lv) {
    DESCR_t buf[64]; int n = 0;
    if (lv.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(lv, "gen_type");
        if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
            int ln = (int)FIELD_GET_fn(lv, "frame_size").i;
            DESCR_t ea = FIELD_GET_fn(lv, "frame_elems");
            DESCR_t *arr = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
            if (ln > 64) ln = 64;
            if (arr) { for (int k = 0; k < ln; k++) buf[k] = arr[k]; n = ln; }
        } else { buf[0] = lv; n = 1; }
    } else { buf[0] = lv; n = 1; }
    return rt_call_value_spine_prep(callee, buf, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_value_resume_h(void **hslot) {
    extern DESCR_t rt_proc_resume_frame_h(void **hslot);
    if (!hslot || !*hslot) return FAILDESCR;
    return rt_proc_resume_frame_h(hslot);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_descr_equal(DESCR_t a, DESCR_t b) {
    if (a.v == DT_I && b.v == DT_I) return a.i == b.i;
    if (a.v == DT_R && b.v == DT_R) return a.r == b.r;
    if ((a.v == DT_S || a.v == DT_SNUL) && (b.v == DT_S || b.v == DT_SNUL)) { const char *x = a.s ? a.s : ""; const char *y = b.s ? b.s : ""; return strcmp(x, y) == 0; }
    { const char *x = VARVAL_fn(a); const char *y = VARVAL_fn(b); if (x && y) return strcmp(x, y) == 0; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_op_code(const char *s) {
    if (!strcmp(s, "add")) return BINOP_ADD; if (!strcmp(s, "sub")) return BINOP_SUB; if (!strcmp(s, "mul")) return BINOP_MUL;
    if (!strcmp(s, "div")) return BINOP_DIV; if (!strcmp(s, "idiv")) return BINOP_DIV; if (!strcmp(s, "mod")) return BINOP_MOD; if (!strcmp(s, "pow")) return BINOP_POW;
    return BINOP_ADD;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_num_cmp(const char *s, DESCR_t a, DESCR_t b) {
    double av = IS_REAL_fn(a) ? a.r : (double)a.i; double bv = IS_REAL_fn(b) ? b.r : (double)b.i;
    if (!strcmp(s, "lt")) return av < bv; if (!strcmp(s, "gt")) return av > bv; if (!strcmp(s, "le")) return av <= bv;
    if (!strcmp(s, "ge")) return av >= bv; if (!strcmp(s, "eq")) return av == bv; if (!strcmp(s, "ne")) return av != bv;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * pl_real_iso_str(double fv, char * fb, int bufsz) {
    for (int prec = 15; prec <= 17; prec++) { snprintf(fb, (size_t)bufsz, "%.*g", prec, fv); if (strtod(fb, (char **)0) == fv) break; }
    if (!strpbrk(fb, ".eEnN")) { size_t n = strlen(fb); if (n + 2 < (size_t)bufsz) { fb[n] = '.'; fb[n + 1] = '0'; fb[n + 2] = '\0'; } }
    return fb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_arith2(const char *op, DESCR_t a, DESCR_t b) {
    extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
    int ai = (a.v == DT_I), bi = (b.v == DT_I);
    if (ai && bi && !strcmp(op, "idiv")) { if (b.i == 0) return FAILDESCR; return INTVAL(a.i / b.i); }
    if (ai && bi && !strcmp(op, "div"))  { if (b.i == 0) return FAILDESCR; return REALVAL((double)a.i / (double)b.i); }
    if (ai && bi && !strcmp(op, "mod"))  { if (b.i == 0) return FAILDESCR; long long m = a.i % b.i; if (m && ((m < 0) != (b.i < 0))) m += b.i; return INTVAL(m); }
    return rt_num_arith(a, b, pl_is_op_code(op));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_atom_str(DESCR_t v) {
    if (v.v == DT_S) return v.s ? v.s : "";
    if (v.v == (DTYPE_t)DT_A) { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name((int)v.i); return nm ? nm : ""; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_mk_atom(const char *s) { DESCR_t d; d.v = DT_S; d.slen = (uint32_t)strlen(s); d.s = s; return d; }
static DESCR_t pl_nil(void) { return pl_mk_atom("[]"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_cons(DESCR_t head, DESCR_t tail) {
    extern int prolog_atom_intern(const char *);
    DESCR_t *kids = (DESCR_t *)rt_plj_alloc(2 * sizeof(DESCR_t)); kids[0] = head; kids[1] = tail;
    DESCR_t c; c.v = (DTYPE_t)DT_PLREF; c.slen = (((uint32_t)prolog_atom_intern(".")) << 16) | (2u & 0xFFFFu); c.p = (void *)kids;
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_list_from_arr(DESCR_t *elems, int n) { DESCR_t acc = pl_nil(); for (int i = n - 1; i >= 0; i--) acc = pl_cons(elems[i], acc); return acc; }
static void *pl_var_cell_ptr(DESCR_t v) { extern DESCR_t rt_pl_deref_val(DESCR_t); DESCR_t d = rt_pl_deref_val(v); return (d.v == (DTYPE_t)DT_PLVAR) ? d.p : (void *)0; }
static void pl_count_var_occ(DESCR_t t, void *target, int *cnt) { extern DESCR_t rt_pl_deref_val(DESCR_t); DESCR_t d = rt_pl_deref_val(t); if (d.v == (DTYPE_t)DT_PLVAR) { if (d.p == target) (*cnt)++; return; } if (d.v == (DTYPE_t)DT_PLREF) { int ar = (int)(d.slen & 0xFFFFu); DESCR_t *kids = (DESCR_t *)d.p; for (int i = 0; i < ar; i++) pl_count_var_occ(kids[i], target, cnt); } }
static DESCR_t pl_mk_atom_dup(const char *s, size_t n) { extern int prolog_atom_intern(const char *); char *o = (char *)rt_ws_alloc(n + 1); if (n) memcpy(o, s, n); o[n] = 0; DESCR_t d; d.v = DT_S; d.slen = (uint32_t)n; d.s = o; (void)prolog_atom_intern(o); return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_sink_kind(DESCR_t a) {
    extern DESCR_t rt_pl_deref_val(DESCR_t); extern const char *prolog_atom_name(int);
    DESCR_t v = rt_pl_deref_val(a);
    if ((int)v.v != DT_PLREF) return 0;
    int fid = (int)(v.slen >> 16); int ar = (int)(v.slen & 0xFFFFu); if (ar != 1) return 0;
    const char *fnm = prolog_atom_name(fid); if (!fnm) return 0;
    if (!strcmp(fnm, "atom")) return 1;
    if (!strcmp(fnm, "string")) return 2;
    if (!strcmp(fnm, "codes")) return 3;
    if (!strcmp(fnm, "chars")) return 4;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_sink_build(int kind, const char *s, size_t n) {
    if (kind == 1 || kind == 2) return pl_mk_atom_dup(s, n);
    DESCR_t *elems = (DESCR_t *)rt_ws_alloc((n > 0 ? n : 1) * sizeof(DESCR_t));
    for (size_t i = 0; i < n; i++) {
        if (kind == 3) { elems[i].v = (DTYPE_t)DT_I; elems[i].slen = 0; elems[i].i = (unsigned char)s[i]; }
        else { char *o = (char *)rt_ws_alloc(2); o[0] = s[i]; o[1] = 0; elems[i] = pl_mk_atom_dup(o, 1); }
    }
    return pl_list_from_arr(elems, (int)n);
}
typedef struct { char *buf; size_t sz; int saved_out; int idx; } PlWotFrame;
static PlWotFrame pl_wot_stk[32];
static int pl_wot_sp = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_nil(DESCR_t v) {
    if (v.v == DT_S || v.v == DT_SNUL) return v.s && !strcmp(v.s, "[]");
    if (v.v == (DTYPE_t)DT_A) { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name((int)v.i); return nm && !strcmp(nm, "[]"); }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_list_to_arr(DESCR_t lst, DESCR_t *out, int max) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    int n = 0; DESCR_t cur = rt_pl_deref_val(lst);
    while (cur.v == (DTYPE_t)DT_PLREF && (int)(cur.slen & 0xFFFFu) == 2) {
        DESCR_t *kids = (DESCR_t *)cur.p; if (n >= max) return -1;
        out[n++] = rt_pl_deref_val(kids[0]); cur = rt_pl_deref_val(kids[1]);
    }
    return pl_is_nil(cur) ? n : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *pl_list_to_cstr(DESCR_t arg, int codes) {
    DESCR_t elems[4096]; int n = pl_list_to_arr(arg, elems, 4096);
    if (n < 0) return (char *)0;
    char *o = (char *)malloc((size_t)n + 1);
    if (!o) return (char *)0;
    for (int i = 0; i < n; i++) {
        if (codes) { if (elems[i].v != DT_I) { free(o); return (char *)0; } o[i] = (char)elems[i].i; }
        else { const char *cs = pl_atom_str(elems[i]); if (!cs || !cs[0]) { free(o); return (char *)0; } o[i] = cs[0]; }
    }
    o[n] = 0; return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_term_to_cell(Term *t) {
    t = term_deref(t);
    if (!t) return pl_nil();
    switch (t->tag) {
        case TERM_INT: { DESCR_t d; d.v = (DTYPE_t)DT_I; d.slen = 0; d.i = t->ival; return d; }
        case TERM_FLOAT: { DESCR_t d; d.v = (DTYPE_t)DT_R; d.slen = 0; d.r = t->fval; return d; }
        case TERM_ATOM: { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name(t->atom_id); return pl_mk_atom(nm ? nm : ""); }
        case TERM_VAR: { DESCR_t *c = (DESCR_t *)rt_plj_alloc(sizeof(DESCR_t)); c->v = (DTYPE_t)DT_PLVAR; c->slen = 0; c->p = (void *)c; return *c; }
        case TERM_COMPOUND: {
            int ar = t->compound.arity;
            DESCR_t *kids = (DESCR_t *)rt_plj_alloc((size_t)(ar > 0 ? ar : 1) * sizeof(DESCR_t));
            for (int i = 0; i < ar; i++) kids[i] = pl_term_to_cell(t->compound.args[i]);
            DESCR_t d; d.v = (DTYPE_t)DT_PLREF; d.slen = (((uint32_t)t->compound.functor) << 16) | ((uint32_t)ar & 0xFFFFu); d.p = (void *)kids;
            return d;
        }
        default: return pl_nil();
    }
}
typedef struct { Term **items; int n; int cap; } PlFindallAcc;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_findall_new(void) {
    PlFindallAcc *acc = (PlFindallAcc *)rt_ws_alloc(sizeof(PlFindallAcc)); acc->items = NULL; acc->n = 0; acc->cap = 0;
    DESCR_t h; h.v = (DTYPE_t)DT_I; h.slen = 0; h.i = (int64_t)(intptr_t)acc; return h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rt_findall_add(DESCR_t handle, DESCR_t tmpl_val) {
    PlFindallAcc *acc = (PlFindallAcc *)(intptr_t)handle.i; DESCR_t tmp = tmpl_val;
    extern Term *rt_pl_cell_to_term(void *); Term *snap = rt_pl_cell_to_term(&tmp);
    if (acc->n >= acc->cap) { int nc = acc->cap ? acc->cap * 2 : 8; Term **ni = (Term **)rt_ws_alloc((size_t)nc * sizeof(Term *)); for (int i = 0; i < acc->n; i++) ni[i] = acc->items[i]; acc->items = ni; acc->cap = nc; }
    acc->items[acc->n++] = snap;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_findall_result(DESCR_t handle) {
    PlFindallAcc *acc = (PlFindallAcc *)(intptr_t)handle.i;
    DESCR_t *elems = (DESCR_t *)rt_ws_alloc((size_t)(acc->n > 0 ? acc->n : 1) * sizeof(DESCR_t));
    for (int i = 0; i < acc->n; i++) elems[i] = pl_term_to_cell(acc->items[i]);
    return pl_list_from_arr(elems, acc->n);
}
static void out_write_descr(FILE *dest, DESCR_t av, int use_gist);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t plc_iso_atom(const char *nm) { extern int prolog_atom_intern(const char *); DESCR_t c; int id = prolog_atom_intern(nm); c.v = (DTYPE_t)DT_A; c.slen = (uint32_t)id; c.i = id; return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t plc_iso_comp(const char *fn, int ar, DESCR_t *kids) {
    extern int prolog_atom_intern(const char *);
    DESCR_t *h = (DESCR_t *)rt_ws_alloc((size_t)(ar > 0 ? ar : 1) * sizeof(DESCR_t));
    for (int i = 0; i < ar; i++) h[i] = kids[i];
    DESCR_t c; c.v = (DTYPE_t)DT_PLREF; c.slen = (((uint32_t)prolog_atom_intern(fn)) << 16) | ((uint32_t)ar & 0xFFFFu); c.p = (void *)h; return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t plc_iso_fresh(void) { DESCR_t *j = (DESCR_t *)rt_plj_alloc(sizeof(DESCR_t)); j->v = (DTYPE_t)DT_PLVAR; j->slen = 0; j->p = (void *)j; DESCR_t r; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)j; return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_iso_ball(DESCR_t formal) {
    extern void rt_pl_throw_set(void *); extern int rt_pl_throw_pending(void);
    if (rt_pl_throw_pending()) return;
    DESCR_t kids[2]; kids[0] = formal; kids[1] = plc_iso_fresh();
    DESCR_t ball = plc_iso_comp("error", 2, kids);
    rt_pl_throw_set((void *)&ball);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_instantiation(void) { plc_iso_ball(plc_iso_atom("instantiation_error")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_pi(const char *errfn, const char *what, const char *nm, int ar) {
    DESCR_t pia[2]; pia[0] = plc_iso_atom(nm ? nm : "?"); pia[1].v = (DTYPE_t)DT_I; pia[1].slen = 0; pia[1].i = ar;
    DESCR_t kids[2]; kids[0] = plc_iso_atom(what); kids[1] = plc_iso_comp("/", 2, pia);
    plc_iso_ball(plc_iso_comp(errfn, 2, kids));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_existence_key(const char *key) {
    char nm[200]; int ar = 0;
    const char *sl = key ? strrchr(key, '/') : (const char *)0;
    if (sl) { int kl = (int)(sl - key); if (kl > 199) kl = 199; memcpy(nm, key, (size_t)kl); nm[kl] = 0; ar = atoi(sl + 1); }
    else { snprintf(nm, sizeof nm, "%s", key ? key : "?"); ar = 0; }
    rt_pl_iso_throw_pi("existence_error", "procedure", nm, ar);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_permission(const char *op, const char *type, const char *nm, int ar) {
    DESCR_t pia[2]; pia[0] = plc_iso_atom(nm ? nm : "?"); pia[1].v = (DTYPE_t)DT_I; pia[1].slen = 0; pia[1].i = ar;
    DESCR_t kids[3]; kids[0] = plc_iso_atom(op ? op : "access"); kids[1] = plc_iso_atom(type ? type : "private_procedure"); kids[2] = plc_iso_comp("/", 2, pia);
    plc_iso_ball(plc_iso_comp("permission_error", 3, kids));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_permission_t(const char *op, const char *type, DESCR_t culprit) {
    DESCR_t kids[3]; kids[0] = plc_iso_atom(op ? op : "access"); kids[1] = plc_iso_atom(type ? type : "stream"); kids[2] = culprit;
    plc_iso_ball(plc_iso_comp("permission_error", 3, kids));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_type(const char *type, DESCR_t culprit) {
    DESCR_t kids[2]; kids[0] = plc_iso_atom(type ? type : "?"); kids[1] = culprit;
    plc_iso_ball(plc_iso_comp("type_error", 2, kids));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_domain(const char *domain, DESCR_t culprit) {
    DESCR_t kids[2]; kids[0] = plc_iso_atom(domain ? domain : "?"); kids[1] = culprit;
    plc_iso_ball(plc_iso_comp("domain_error", 2, kids));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_iso_throw_existence(const char *type, DESCR_t culprit) {
    DESCR_t kids[2]; kids[0] = plc_iso_atom(type ? type : "?"); kids[1] = culprit;
    plc_iso_ball(plc_iso_comp("existence_error", 2, kids));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_stream_term(int idx) { DESCR_t k[1]; k[0].v = (DTYPE_t)DT_I; k[0].slen = 0; k[0].i = idx; return plc_iso_comp("$stream", 1, k); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_stream_alias_idx(const char *nm) {
    if (!nm) return -1;
    if (!strcmp(nm, "user_input")) return 0; if (!strcmp(nm, "user_output")) return 1; if (!strcmp(nm, "user_error")) return 2;
    extern FILE *fh_get(int); extern char *fh_name[]; for (int i = 0; i < 64; i++) { if (fh_get(i) && fh_name[i] && !strcmp(fh_name[i], nm)) return i; } return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_resolve_stream_arg(DESCR_t a, const char *errfn, int want_output) {
    extern FILE *fh_get(int);
    DESCR_t v = rt_pl_deref_val(a);
    (void)errfn;
    if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); return -1; }
    int idx = -1;
    if (IS_FH_fn(v)) idx = (int)v.i;
    else if ((int)v.v == DT_PLREF) { int fid = (int)(v.slen >> 16); int ar = (int)(v.slen & 0xFFFFu); const char *fnm = prolog_atom_name(fid); if (fnm && !strcmp(fnm, "$stream") && ar == 1 && v.p) { DESCR_t k = rt_pl_deref_val(((DESCR_t *)v.p)[0]); if (k.v == DT_I) idx = (int)k.i; } if (idx < 0) { rt_pl_iso_throw_domain("stream_or_alias", v); return -1; } }
    else if ((int)v.v == DT_A || v.v == DT_S) { const char *nm = pl_atom_str(v); idx = pl_stream_alias_idx(nm); if (idx < 0) { rt_pl_iso_throw_existence("stream", v); return -1; } }
    else { rt_pl_iso_throw_domain("stream_or_alias", v); return -1; }
    if (idx < 0 || idx >= 64 || !fh_get(idx)) { rt_pl_iso_throw_existence("stream", v); return -1; }
    if (want_output == 1 && idx == 0) { rt_pl_iso_throw_permission("output", "stream", "user_input", 0); return -1; }
    if (want_output == 0 && (idx == 1 || idx == 2)) { rt_pl_iso_throw_permission("input", "stream", idx == 1 ? "user_output" : "user_error", 0); return -1; }
    if (idx >= 3) { extern char fh_mode[]; char md = fh_mode[idx];
        if (want_output == 0 && (md == 'w' || md == 'a')) { rt_pl_iso_throw_permission_t("input", "stream", pl_stream_term(idx)); return -1; }
        if (want_output == 1 && md == 'r') { rt_pl_iso_throw_permission_t("output", "stream", pl_stream_term(idx)); return -1; } }
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_proc_defined_static(const char *name, long arity) {
    if (!name) return 0;
    char key[256]; snprintf(key, sizeof key, "%s/%ld", name, arity);
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, key) && !g_stage2.proc_table[i].is_generator) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_iso_evaluable(DESCR_t v) {
    extern const char *prolog_atom_name(int);
    if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); return; }
    if ((int)v.v == DT_A) { rt_pl_iso_throw_pi("type_error", "evaluable", prolog_atom_name((int)v.i), 0); return; }
    if (v.v == DT_S) { rt_pl_iso_throw_pi("type_error", "evaluable", v.s, 0); return; }
    if ((int)v.v == DT_PLREF) { rt_pl_iso_throw_pi("type_error", "evaluable", prolog_atom_name((int)(v.slen >> 16)), (int)(v.slen & 0xFFFFu)); return; }
    rt_pl_iso_throw_pi("type_error", "evaluable", "?", 0);
}
static long g_tap_run = 0;
static long g_tap_failed = 0;
static long g_tap_planned = 0;
static int g_tap_no_plan = 1;
static long g_tap_todo_upto = 0;
static char g_tap_todo_reason[512] = "";
static int g_tap_done_run = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tap_truthy(DESCR_t v) {
    if (IS_FAIL_fn(v)) return 0;
    if (IS_INT_fn(v)) return (v.i != 0);
    if (IS_REAL_fn(v)) return (v.r != 0.0);
    if (v.v == DT_SNUL) return 0;
    { const char *s = v.s ? v.s : ""; return (s[0] != '\0' && !(s[0] == '0' && s[1] == '\0')); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tap_desc_escape(const char *desc, char *out, size_t cap) {
    size_t o = 0; if (!desc) { out[0] = '\0'; return; }
    for (size_t i = 0; desc[i] && o + 4 < cap; i++) {
        if (desc[i] == '#') { out[o++] = ' '; out[o++] = '\\'; out[o++] = '#'; }
        else if (desc[i] == '\n') { out[o++] = '\n'; out[o++] = '#'; out[o++] = ' '; }
        else out[o++] = desc[i];
    }
    out[o] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tap_proclaim(int cond, const char *desc, const char *prefix) {
    char esc[1024]; char line[2048]; int in_todo;
    g_tap_run++;
    in_todo = (g_tap_todo_reason[0] != '\0' && g_tap_run <= g_tap_todo_upto);
    if (!cond && !(g_tap_run <= g_tap_todo_upto)) g_tap_failed++;
    rk_tap_desc_escape(desc ? desc : "", esc, sizeof esc);
    snprintf(line, sizeof line, "%sok %ld - %s%s%s", cond ? "" : "not ", g_tap_run, prefix ? prefix : "", esc, in_todo ? g_tap_todo_reason : "");
    puts(line); fflush(stdout);
    return cond;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tap_diag(const char *msg) {
    fprintf(stderr, "# %s\n", msg ? msg : ""); fflush(stderr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_sprintf_core(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen) {
    size_t cap = strlen(fmt) + 64, len = 0; char *buf = (char *)rt_ws_alloc(cap); int ai = from;
    for (const char *p = fmt; *p; ) {
        if (*p != '%') { if (len + 2 > cap) { cap = cap * 2 + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = *p++; continue; }
        const char *start = p; p++;
        if (*p == '%') { if (len + 2 > cap) { cap = cap * 2 + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = '%'; p++; continue; }
        char spec[64]; int sp = 0; spec[sp++] = '%';
        while (*p == '-' || *p == '+' || *p == ' ' || *p == '0' || *p == '#') { if (sp < 60) spec[sp++] = *p; p++; }
        long width = -1;
        if (*p == '*') { width = (ai < nargs) ? (IS_INT_fn(args[ai]) ? (long)args[ai].i : (IS_REAL_fn(args[ai]) ? (long)args[ai].r : 0)) : 0; ai++; sp += snprintf(spec + sp, sizeof spec - sp, "%ld", width); p++; }
        else while (*p >= '0' && *p <= '9') { if (sp < 60) spec[sp++] = *p; p++; }
        if (*p == '.') { if (sp < 60) spec[sp++] = '.'; p++;
            if (*p == '*') { long prec = (ai < nargs) ? (IS_INT_fn(args[ai]) ? (long)args[ai].i : (IS_REAL_fn(args[ai]) ? (long)args[ai].r : 0)) : 0; ai++; sp += snprintf(spec + sp, sizeof spec - sp, "%ld", prec); p++; }
            else while (*p >= '0' && *p <= '9') { if (sp < 60) spec[sp++] = *p; p++; } }
        char conv = *p; if (!conv) { if (len + 1 < cap) buf[len++] = '%'; break; } p++;
        DESCR_t a = (ai < nargs) ? args[ai] : NULVCL; ai++;
        char piece[512]; char sb[256]; int done = 0;
        if (conv == 'd' || conv == 'i' || conv == 'u' || conv == 'x' || conv == 'X' || conv == 'o') {
            long lv = IS_INT_fn(a) ? (long)a.i : (IS_REAL_fn(a) ? (long)a.r : atol(to_cstring(a, sb, sizeof sb)));
            char cspec[68]; int cl = 0; for (int k = 0; k < sp; k++) cspec[cl++] = spec[k]; cspec[cl++] = 'l'; cspec[cl++] = conv; cspec[cl] = 0;
            snprintf(piece, sizeof piece, cspec, lv); done = 1;
        } else if (conv == 'e' || conv == 'E' || conv == 'f' || conv == 'F' || conv == 'g' || conv == 'G') {
            double dv = IS_REAL_fn(a) ? a.r : (IS_INT_fn(a) ? (double)a.i : atof(to_cstring(a, sb, sizeof sb)));
            char cspec[68]; int cl = 0; for (int k = 0; k < sp; k++) cspec[cl++] = spec[k]; cspec[cl++] = conv; cspec[cl] = 0;
            snprintf(piece, sizeof piece, cspec, dv); done = 1;
        } else if (conv == 'c') {
            long lv = IS_INT_fn(a) ? (long)a.i : (IS_REAL_fn(a) ? (long)a.r : atol(to_cstring(a, sb, sizeof sb)));
            char cspec[68]; int cl = 0; for (int k = 0; k < sp; k++) cspec[cl++] = spec[k]; cspec[cl++] = 'c'; cspec[cl] = 0;
            snprintf(piece, sizeof piece, cspec, (int)lv); done = 1;
        } else if (conv == 's') {
            const char *sv = to_cstring(a, sb, sizeof sb); if (!sv) sv = "";
            char cspec[68]; int cl = 0; for (int k = 0; k < sp; k++) cspec[cl++] = spec[k]; cspec[cl++] = 's'; cspec[cl] = 0;
            int need = snprintf(NULL, 0, cspec, sv);
            if (need >= (int)sizeof piece) { char *big = (char *)rt_ws_alloc((size_t)need + 1); snprintf(big, (size_t)need + 1, cspec, sv);
                if (len + (size_t)need + 1 > cap) { cap = len + (size_t)need + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; }
                memcpy(buf + len, big, (size_t)need); len += (size_t)need; continue; }
            snprintf(piece, sizeof piece, cspec, sv); done = 1;
        } else if (conv == 'b') {
            unsigned long uv = IS_INT_fn(a) ? (unsigned long)a.i : (IS_REAL_fn(a) ? (unsigned long)a.r : (unsigned long)atol(to_cstring(a, sb, sizeof sb)));
            char bits[72]; int bn = 0; if (uv == 0) bits[bn++] = '0'; else { char tmp[72]; int tn = 0; while (uv) { tmp[tn++] = (char)('0' + (uv & 1)); uv >>= 1; } while (tn) bits[bn++] = tmp[--tn]; } bits[bn] = 0;
            long w = 0; int zero = 0; for (int k = 1; k < sp; k++) { if (spec[k] == '0') zero = 1; else if (spec[k] >= '1' && spec[k] <= '9') { w = w * 10 + (spec[k] - '0'); } }
            int pad = (int)w - bn; if (pad > 0) { char pc = zero ? '0' : ' '; for (int k = 0; k < pad; k++) { if (len + 1 >= cap) { cap = cap * 2 + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = pc; } }
            for (int k = 0; k < bn; k++) { if (len + 1 >= cap) { cap = cap * 2 + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = bits[k]; } continue;
        } else { for (const char *q = start; q <= start + (p - start) - 1; q++) { if (len + 1 >= cap) { cap = cap * 2 + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } buf[len++] = *q; } continue; }
        if (done) { size_t pl = strlen(piece); if (len + pl + 1 > cap) { cap = len + pl + 8; char *nb = (char *)rt_ws_alloc(cap); memcpy(nb, buf, len); buf = nb; } memcpy(buf + len, piece, pl); len += pl; }
    }
    buf[len] = 0; *outp = buf; if (outlen) *outlen = len;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_ax(const char *op, DESCR_t *args, int nargs, DESCR_t *out) {
    extern DESCR_t rt_pl_deref_val(DESCR_t); extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
    if (nargs == 0) { if (!strcmp(op, "pi")) { *out = REALVAL(M_PI); return 1; } *out = FAILDESCR; return 1; }
    DESCR_t a = rt_pl_deref_val(args[0]);
    int ai = (a.v == DT_I), arl = (a.v == DT_R);
    if (!ai && !arl) { plc_iso_evaluable(a); *out = FAILDESCR; return 1; }
    double ad = arl ? a.r : (double)a.i;
    if (nargs == 1) {
        if (!strcmp(op, "neg"))   { *out = ai ? INTVAL(-a.i) : REALVAL(-ad); return 1; }
        if (!strcmp(op, "pos"))   { *out = a; return 1; }
        if (!strcmp(op, "abs"))   { *out = ai ? INTVAL(a.i < 0 ? -a.i : a.i) : REALVAL(fabs(ad)); return 1; }
        if (!strcmp(op, "sign"))  { *out = ai ? INTVAL((a.i > 0) - (a.i < 0)) : REALVAL((double)((ad > 0) - (ad < 0))); return 1; }
        if (!strcmp(op, "trunc")) { *out = INTVAL((long long)ad); return 1; }
        if (!strcmp(op, "intg"))  { *out = ai ? a : INTVAL((long long)llround(ad)); return 1; }
        if (!strcmp(op, "flt"))   { *out = REALVAL(ad); return 1; }
        if (!strcmp(op, "floor")) { *out = INTVAL((long long)floor(ad)); return 1; }
        if (!strcmp(op, "ceil"))  { *out = INTVAL((long long)ceil(ad)); return 1; }
        if (!strcmp(op, "round")) { *out = INTVAL((long long)llround(ad)); return 1; }
        if (!strcmp(op, "sqrt"))  { *out = REALVAL(sqrt(ad)); return 1; }
        if (!strcmp(op, "sin"))   { *out = REALVAL(sin(ad)); return 1; }
        if (!strcmp(op, "cos"))   { *out = REALVAL(cos(ad)); return 1; }
        if (!strcmp(op, "atan"))  { *out = REALVAL(atan(ad)); return 1; }
        if (!strcmp(op, "log"))   { *out = REALVAL(log(ad)); return 1; }
        if (!strcmp(op, "exp"))   { *out = REALVAL(exp(ad)); return 1; }
        if (!strcmp(op, "fip"))   { *out = REALVAL(trunc(ad)); return 1; }
        if (!strcmp(op, "ffp"))   { *out = REALVAL(ad - trunc(ad)); return 1; }
        if (!strcmp(op, "msb"))   { if (!ai || a.i <= 0) { *out = FAILDESCR; return 1; } *out = INTVAL(63 - __builtin_clzll((unsigned long long)a.i)); return 1; }
        if (!strcmp(op, "bnot"))  { if (!ai) { plc_iso_evaluable(a); *out = FAILDESCR; return 1; } *out = INTVAL(~a.i); return 1; }
        *out = FAILDESCR; return 1;
    }
    DESCR_t b = rt_pl_deref_val(args[1]);
    int bi = (b.v == DT_I), brl = (b.v == DT_R);
    if (!bi && !brl) { plc_iso_evaluable(b); *out = FAILDESCR; return 1; }
    double bd = brl ? b.r : (double)b.i;
    if (!strcmp(op, "fpow")) { *out = REALVAL(pow(ad, bd)); return 1; }
    if (!strcmp(op, "min")) { *out = (ai && bi) ? INTVAL(a.i < b.i ? a.i : b.i) : REALVAL(ad < bd ? ad : bd); return 1; }
    if (!strcmp(op, "max")) { *out = (ai && bi) ? INTVAL(a.i > b.i ? a.i : b.i) : REALVAL(ad > bd ? ad : bd); return 1; }
    if (!strcmp(op, "gcd")) { if (!ai || !bi) { *out = FAILDESCR; return 1; } long long x = a.i < 0 ? -a.i : a.i, y = b.i < 0 ? -b.i : b.i; while (y) { long long t2 = x % y; x = y; y = t2; } *out = INTVAL(x); return 1; }
    if (!strcmp(op, "rem")) { if (!ai || !bi || b.i == 0) { *out = FAILDESCR; return 1; } *out = INTVAL(a.i % b.i); return 1; }
    if (!strcmp(op, "xor")) { if (!ai || !bi) { *out = FAILDESCR; return 1; } *out = INTVAL(a.i ^ b.i); return 1; }
    if (!strcmp(op, "shl")) { if (!ai || !bi) { *out = FAILDESCR; return 1; } *out = INTVAL(a.i << b.i); return 1; }
    if (!strcmp(op, "shr")) { if (!ai || !bi) { *out = FAILDESCR; return 1; } *out = INTVAL(a.i >> b.i); return 1; }
    if (!strcmp(op, "band")) { if (!ai || !bi) { *out = FAILDESCR; return 1; } *out = INTVAL(a.i & b.i); return 1; }
    if (!strcmp(op, "bor"))  { if (!ai || !bi) { *out = FAILDESCR; return 1; } *out = INTVAL(a.i | b.i); return 1; }
    { DESCR_t r = pl_arith2(op, a, b); if (r.v == DT_FAIL) { *out = FAILDESCR; return 1; } *out = r; return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *plw_mkc_kids(DESCR_t *srcs, int ar) {
    DESCR_t *kids = (DESCR_t *)rt_plj_alloc((size_t)(ar > 0 ? ar : 1) * sizeof(DESCR_t));
    for (int i = 0; i < ar; i++) {
        DESCR_t t = srcs[i];
        DESCR_t *F = plw_cell_deref(plw_entry(&t));
        if (plw_unbound_tag(F)) {
            kids[i].v = (DTYPE_t)DT_PLVAR; kids[i].slen = 0; kids[i].p = (void *)&kids[i];
            DESCR_t r; r.v = (DTYPE_t)DT_PLVAR; r.slen = 0; r.p = (void *)&kids[i];
            plw_bind(F, r);
        } else kids[i] = *F;
    }
    return kids;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_mkc(DESCR_t *args, int nargs, DESCR_t *out) {
    const char *fname = VARVAL_fn(args[0]); if (!fname) fname = "?";
    int ar = nargs - 1;
    extern int prolog_atom_intern(const char *);
    DESCR_t *kids = plw_mkc_kids(args + 1, ar);
    DESCR_t c; c.v = (DTYPE_t)DT_PLREF; c.slen = (((uint32_t)prolog_atom_intern(fname)) << 16) | ((uint32_t)ar & 0xFFFFu); c.p = (void *)kids;
    *out = c; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_unwind_nothrow(DESCR_t *args, int nargs, DESCR_t *out) {
    (void)nargs;
    extern int rt_pl_throw_pending(void);
    pl_trail_unwind(&g_pl_trail, (int)args[0].i);
    plw_zh_kill_to((int)args[0].i);
    if (rt_pl_throw_pending()) { *out = FAILDESCR; return 1; }
    DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_is_v(DESCR_t *args, int nargs, DESCR_t *out) {
    (void)nargs;
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    DESCR_t v = rt_pl_deref_val(args[1]);
    if (v.v != DT_I && v.v != DT_R) { plc_iso_evaluable(v); *out = FAILDESCR; return 1; }
    if (plw_unify_vals(args[0], v)) { *out = v; return 1; }
    *out = FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_cmp(const char *op, DESCR_t *args, int nargs, DESCR_t *out) {
    (void)nargs;
    extern DESCR_t rt_pl_deref_val(DESCR_t); DESCR_t a = rt_pl_deref_val(args[0]); DESCR_t b = rt_pl_deref_val(args[1]);
    if (pl_num_cmp(op, a, b)) { *out = a; return 1; }
    *out = FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_unify(DESCR_t *args, int nargs, DESCR_t *out) {
    (void)nargs;
    if (plw_unify_vals(args[0], args[1])) { *out = rt_pl_deref_val(args[0]); return 1; }
    *out = FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_unify_lst(DESCR_t *args, int nargs, DESCR_t *out) {
    (void)nargs;
    extern int prolog_atom_intern(const char *);
    static uint32_t dot_sl = 0; if (!dot_sl) { dot_sl = (((uint32_t)prolog_atom_intern(".")) << 16) | 2u; g_plw_dot_sl = dot_sl; }
    DESCR_t t0 = args[0];
    DESCR_t *c = plw_cell_deref(plw_entry(&t0));
    if (plw_unbound_tag(c)) {
        DESCR_t *kids = plw_mkc_kids(args + 1, 2);
        DESCR_t w; w.v = (DTYPE_t)DT_PLREF; w.slen = dot_sl; w.p = (void *)kids;
        plw_bind(c, w); *out = w; return 1;
    }
    if (c->v == (DTYPE_t)DT_PLREF && c->slen == dot_sl) {
        DESCR_t *kids = (DESCR_t *)c->p;
        DESCR_t t1 = args[1]; DESCR_t *hc = plw_cell_deref(plw_entry(&t1));
        if (plw_unbound_tag(hc)) plw_bind(hc, kids[0]);
        else if (!plw_unify_cells(hc, &kids[0])) { *out = FAILDESCR; return 1; }
        DESCR_t t2 = args[2]; DESCR_t *tc = plw_cell_deref(plw_entry(&t2));
        if (plw_unbound_tag(tc)) plw_bind(tc, kids[1]);
        else if (!plw_unify_cells(tc, &kids[1])) { *out = FAILDESCR; return 1; }
        *out = *c; return 1;
    }
    *out = FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_ix_g(DESCR_t *args, int nargs, DESCR_t *out) {
    (void)nargs;
    extern int prolog_atom_intern(const char *);
    static uint32_t dot_sl = 0; if (!dot_sl) dot_sl = (((uint32_t)prolog_atom_intern(".")) << 16) | 2u;
    long long ka = args[1].i; int kk = (int)(ka & 0xFF); int kar = (int)(ka >> 8);
    DESCR_t t0 = args[0]; DESCR_t *c = plw_cell_deref(plw_entry(&t0));
    DESCR_t ok; ok.v = DT_I; ok.slen = 0; ok.i = 1;
    if (plw_unbound_tag(c)) { *out = ok; return 1; }
    if (c->v == (DTYPE_t)DT_PLREF) {
        if (kk == 3) { *out = (c->slen == dot_sl) ? ok : FAILDESCR; return 1; }
        if (kk == 4) { uint32_t want = (((uint32_t)prolog_atom_intern(args[2].s ? args[2].s : "?")) << 16) | ((uint32_t)kar & 0xFFFFu); *out = (c->slen == want) ? ok : FAILDESCR; return 1; }
        *out = FAILDESCR; return 1;
    }
    if (c->v == DT_I && !c->slen) { if (kk == 1) { *out = (c->i == args[2].i) ? ok : FAILDESCR; return 1; } if (kk == 3 || kk == 4) { *out = FAILDESCR; return 1; } *out = ok; return 1; }
    if (c->v == DT_S && !c->slen && c->s) { if (kk == 2 && args[2].s) { *out = strcmp(c->s, args[2].s) ? FAILDESCR : ok; return 1; } if (kk == 3 || kk == 4) { *out = FAILDESCR; return 1; } *out = ok; return 1; }
    *out = ok; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_ix_g(DESCR_t *args, int nargs) { DESCR_t out = FAILDESCR; if (nargs == 3) dop_ix_g(args, 3, &out); return out; }
static int dop_trail_mark(DESCR_t *args, int nargs, DESCR_t *out) { (void)args; (void)nargs; DESCR_t m; m.v = DT_I; m.slen = 0; m.i = (long long)pl_trail_mark(&g_pl_trail); plw_zh_mark_push((int)m.i); *out = m; return 1; }
static int dop_trail_unwind(DESCR_t *args, int nargs, DESCR_t *out) { (void)nargs; pl_trail_unwind(&g_pl_trail, (int)args[0].i); plw_zh_kill_to((int)args[0].i); DESCR_t r; r.v = DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
typedef int (*dop_body_fn)(DESCR_t *, int, DESCR_t *);
char *g_plw_unwind_floor = 0;
int   g_plw_floor_bypass = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void  rt_plw_floor_bypass_on(void) { g_plw_floor_bypass = 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t dop_call(dop_body_fn body, DESCR_t *args, int nargs) {
    extern jmp_buf g_core_errjmp_stk[64]; extern int g_core_errjmp_n; extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    DESCR_t out = FAILDESCR;
    char *fl = g_plw_unwind_floor;
    { extern int g_plw_floor_bypass; if (!g_plw_floor_bypass) g_plw_unwind_floor = (char *)__builtin_frame_address(0); }
    if (g_core_errjmp_n >= 64) { rt_gc_point_arr(args, nargs, (const char **)0); body(args, nargs, &out); g_plw_unwind_floor = fl; return out; }
    int my = g_core_errjmp_n;
    if (setjmp(g_core_errjmp_stk[my])) { g_core_errjmp_n = my; g_plw_unwind_floor = fl; return FAILDESCR; }
    g_core_errjmp_n = my + 1;
    rt_gc_point_arr(args, nargs, (const char **)0);
    body(args, nargs, &out);
    g_core_errjmp_n = my;
    g_plw_unwind_floor = fl;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t dop_call_nothrow(dop_body_fn body, DESCR_t *args, int nargs) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0); extern int g_plw_floor_bypass;
    DESCR_t out = FAILDESCR;
    char *fl = g_plw_unwind_floor;
    if (!g_plw_floor_bypass) g_plw_unwind_floor = (char *)__builtin_frame_address(0);
    rt_gc_point_arr(args, nargs, (const char **)0);
    body(args, nargs, &out);
    g_plw_unwind_floor = fl;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dop_ax_add(DESCR_t *a, int n, DESCR_t *o) { return dop_ax("add", a, n, o); }
static int dop_ax_sub(DESCR_t *a, int n, DESCR_t *o) { return dop_ax("sub", a, n, o); }
static int dop_ax_mul(DESCR_t *a, int n, DESCR_t *o) { return dop_ax("mul", a, n, o); }
static int dop_ax_div(DESCR_t *a, int n, DESCR_t *o) { return dop_ax("div", a, n, o); }
static int dop_ax_idiv(DESCR_t *a, int n, DESCR_t *o) { return dop_ax("idiv", a, n, o); }
static int dop_ax_mod(DESCR_t *a, int n, DESCR_t *o) { return dop_ax("mod", a, n, o); }
static int dop_cmp_lt(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("lt", a, n, o); }
static int dop_cmp_gt(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("gt", a, n, o); }
static int dop_cmp_le(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("le", a, n, o); }
static int dop_cmp_ge(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("ge", a, n, o); }
static int dop_cmp_eq(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("eq", a, n, o); }
static int dop_cmp_ne(DESCR_t *a, int n, DESCR_t *o) { return dop_cmp("ne", a, n, o); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_pl_dop_unify(DESCR_t *args, int nargs) {
    (void)args; (void)nargs;
    { extern void rt_bomb(const char *); rt_bomb("c_rt_pl_dop_unify: DELETED (s196 Lon one-to-maintain) — rt_pl_dop_unify in rtx_plunify.s is the sole spelling (zero bails, gate removed)"); }
    return FAILDESCR;
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_unify_ci(DESCR_t *args, long long imm) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    char *fl = g_plw_unwind_floor; DESCR_t out;
    g_plw_unwind_floor = (char *)__builtin_frame_address(0);
    rt_gc_point_arr(args, 1, (const char **)0);
    { DESCR_t t = args[0]; DESCR_t *c = plw_cell_deref(plw_entry(&t));
      if (plw_unbound_tag(c)) { DESCR_t w; w.v = DT_I; w.slen = 0; w.i = imm; plw_bind(c, w); out = w; }
      else if (c->v == DT_I && !c->slen) out = (c->i == imm) ? *c : FAILDESCR;
      else if (c->v == (DTYPE_t)DT_PLREF) out = FAILDESCR;
      else { DESCR_t w; w.v = DT_I; w.slen = 0; w.i = imm; out = plw_unify_vals(args[0], w) ? rt_pl_deref_val(args[0]) : FAILDESCR; } }
    g_plw_unwind_floor = fl;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_unify_cs(DESCR_t *args, const char *cs) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    char *fl = g_plw_unwind_floor; DESCR_t out;
    g_plw_unwind_floor = (char *)__builtin_frame_address(0);
    rt_gc_point_arr(args, 1, (const char **)0);
    { DESCR_t t = args[0]; DESCR_t *c = plw_cell_deref(plw_entry(&t));
      if (plw_unbound_tag(c)) { DESCR_t w; w.v = DT_S; w.slen = 0; w.s = cs; plw_bind(c, w); out = w; }
      else if (c->v == (DTYPE_t)DT_PLREF) out = FAILDESCR;
      else { DESCR_t w; w.v = DT_S; w.slen = 0; w.s = cs; out = plw_unify_vals(args[0], w) ? rt_pl_deref_val(args[0]) : FAILDESCR; } }
    g_plw_unwind_floor = fl;
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_unify_lst(DESCR_t *args, int nargs) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    if (nargs != 3) return FAILDESCR;
    { char *fl = g_plw_unwind_floor; DESCR_t out = FAILDESCR;
      g_plw_unwind_floor = (char *)__builtin_frame_address(0);
      rt_gc_point_arr(args, 3, (const char **)0);
      dop_unify_lst(args, 3, &out);
      g_plw_unwind_floor = fl;
      return out; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_mkc(DESCR_t *args, int nargs) { return nargs >= 1 ? dop_call(dop_mkc, args, nargs) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_trail_mark(DESCR_t *args, int nargs) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    if (nargs != 0) return FAILDESCR;
    { char *fl = g_plw_unwind_floor; DESCR_t m;
      g_plw_unwind_floor = (char *)__builtin_frame_address(0);
      rt_gc_point_arr(args, 0, (const char **)0);
      m.v = DT_I; m.slen = 0; m.i = (long long)pl_trail_mark(&g_pl_trail); plw_zh_mark_push((int)m.i);
      g_plw_unwind_floor = fl;
      return m; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_trail_unwind(DESCR_t *args, int nargs) { return nargs == 1 ? dop_call_nothrow(dop_trail_unwind, args, nargs) : FAILDESCR; }
DESCR_t rt_pl_dop_unwind_nothrow(DESCR_t *args, int nargs) { return nargs == 1 ? dop_call_nothrow(dop_unwind_nothrow, args, nargs) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_is_v(DESCR_t *args, int nargs) {
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    if (nargs != 2) return FAILDESCR;
    { DESCR_t v = rt_pl_deref_val(args[1]);
      if (v.v == DT_I || v.v == DT_R) {
          char *fl = g_plw_unwind_floor; DESCR_t out;
          g_plw_unwind_floor = (char *)__builtin_frame_address(0);
          rt_gc_point_arr(args, 2, (const char **)0);
          out = plw_unify_vals(args[0], v) ? v : FAILDESCR;
          g_plw_unwind_floor = fl;
          return out; }
      return dop_call(dop_is_v, args, nargs); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_ax_add(DESCR_t *args, int nargs) {
    if (nargs == 2) { DESCR_t a = rt_pl_deref_val(args[0]), b = rt_pl_deref_val(args[1]); long long r;
      if (a.v == DT_I && b.v == DT_I && !__builtin_add_overflow(a.i, b.i, &r)) return INTVAL(r); }
    return nargs == 2 ? dop_call(dop_ax_add, args, nargs) : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_ax_sub(DESCR_t *args, int nargs) {
    if (nargs == 2) { DESCR_t a = rt_pl_deref_val(args[0]), b = rt_pl_deref_val(args[1]); long long r;
      if (a.v == DT_I && b.v == DT_I && !__builtin_sub_overflow(a.i, b.i, &r)) return INTVAL(r); }
    return nargs == 2 ? dop_call(dop_ax_sub, args, nargs) : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_ax_mul(DESCR_t *args, int nargs) {
    if (nargs == 2) { DESCR_t a = rt_pl_deref_val(args[0]), b = rt_pl_deref_val(args[1]); long long r;
      if (a.v == DT_I && b.v == DT_I && !__builtin_mul_overflow(a.i, b.i, &r)) return INTVAL(r); }
    return nargs == 2 ? dop_call(dop_ax_mul, args, nargs) : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_ax_div(DESCR_t *args, int nargs) { return nargs == 2 ? dop_call(dop_ax_div, args, nargs) : FAILDESCR; }
DESCR_t rt_pl_dop_ax_idiv(DESCR_t *args, int nargs) { return nargs == 2 ? dop_call(dop_ax_idiv, args, nargs) : FAILDESCR; }
DESCR_t rt_pl_dop_ax_mod(DESCR_t *args, int nargs) { return nargs == 2 ? dop_call(dop_ax_mod, args, nargs) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t dop_cmp_fast(DESCR_t *args, int rel, dop_body_fn slow) {
    DESCR_t a = rt_pl_deref_val(args[0]), b = rt_pl_deref_val(args[1]);
    if ((a.v == DT_I || a.v == DT_R) && (b.v == DT_I || b.v == DT_R)) {
        double av = IS_REAL_fn(a) ? a.r : (double)a.i, bv = IS_REAL_fn(b) ? b.r : (double)b.i;
        int t = rel == 0 ? av < bv : rel == 1 ? av > bv : rel == 2 ? av <= bv : rel == 3 ? av >= bv : rel == 4 ? av == bv : av != bv;
        return t ? a : FAILDESCR; }
    return dop_call(slow, args, 2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_cmp_lt(DESCR_t *args, int nargs) { return nargs == 2 ? dop_cmp_fast(args, 0, dop_cmp_lt) : FAILDESCR; }
DESCR_t rt_pl_dop_cmp_gt(DESCR_t *args, int nargs) { return nargs == 2 ? dop_cmp_fast(args, 1, dop_cmp_gt) : FAILDESCR; }
DESCR_t rt_pl_dop_cmp_le(DESCR_t *args, int nargs) { return nargs == 2 ? dop_cmp_fast(args, 2, dop_cmp_le) : FAILDESCR; }
DESCR_t rt_pl_dop_cmp_ge(DESCR_t *args, int nargs) { return nargs == 2 ? dop_cmp_fast(args, 3, dop_cmp_ge) : FAILDESCR; }
DESCR_t rt_pl_dop_cmp_eq(DESCR_t *args, int nargs) { return nargs == 2 ? dop_cmp_fast(args, 4, dop_cmp_eq) : FAILDESCR; }
DESCR_t rt_pl_dop_cmp_ne(DESCR_t *args, int nargs) { return nargs == 2 ? dop_cmp_fast(args, 5, dop_cmp_ne) : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_read_src_from_fp(FILE *f, char *rb, int cap) {
    int rn = 0; int rc; int rql = 0; char rq = 0;
    for (;;) { rc = fgetc(f); if (rc == EOF) break; if (rql) { if (rc == '\n') rql = 0; continue; } if (rc == ' ' || rc == '\t' || rc == '\n' || rc == '\r') continue; if (rc == '%') { rql = 1; continue; } break; }
    if (rc == EOF) return -1;
    for (;;) { if (rq) { if (rn < cap - 1) rb[rn++] = (char)rc; if (rc == rq) rq = 0; rc = fgetc(f); if (rc == EOF) break; continue; } if (rc == '\'' || rc == '"' || rc == '`') { rq = (char)rc; if (rn < cap - 1) rb[rn++] = (char)rc; rc = fgetc(f); if (rc == EOF) break; continue; } if (rc == '.') { int nx = fgetc(f); if (nx == EOF || nx == ' ' || nx == '\t' || nx == '\n' || nx == '\r' || nx == '%') break; if (rn < cap - 1) rb[rn++] = '.'; rc = nx; continue; } if (rn < cap - 1) rb[rn++] = (char)rc; rc = fgetc(f); if (rc == EOF) break; }
    rb[rn] = 0; return rn;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_read_apply_opts(DESCR_t optlist, DESCR_t tval, DESCR_t *bv, char (*bn)[64], int nb) {
    extern const char *prolog_atom_name(int);
    DESCR_t opts[64]; int no = pl_list_to_arr(rt_pl_deref_val(optlist), opts, 64);
    if (no < 0) return 0;
    for (int oi = 0; oi < no; oi++) {
        DESCR_t od = rt_pl_deref_val(opts[oi]); if (od.v != (DTYPE_t)DT_PLREF || (int)(od.slen & 0xFFFFu) != 1) continue;
        const char *onm = prolog_atom_name((int)(od.slen >> 16)); DESCR_t oarg = ((DESCR_t *)od.p)[0]; if (!onm) continue;
        if (!strcmp(onm, "variables")) { if (!plw_unify_vals(oarg, pl_list_from_arr(bv, nb))) return 0; }
        else if (!strcmp(onm, "variable_names")) { DESCR_t pr[64]; for (int i = 0; i < nb; i++) { DESCR_t two[2]; two[0] = pl_mk_atom(bn[i]); two[1] = bv[i]; pr[i] = plc_iso_comp("=", 2, two); } if (!plw_unify_vals(oarg, pl_list_from_arr(pr, nb))) return 0; }
        else if (!strcmp(onm, "singletons")) { DESCR_t sg[64]; int ns = 0; for (int i = 0; i < nb; i++) { void *vp = pl_var_cell_ptr(bv[i]); int c = 0; if (vp) pl_count_var_occ(tval, vp, &c); if (c == 1) { DESCR_t two[2]; two[0] = pl_mk_atom(bn[i]); two[1] = bv[i]; sg[ns++] = plc_iso_comp("=", 2, two); } } if (!plw_unify_vals(oarg, pl_list_from_arr(sg, ns))) return 0; }
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_make_nested_agg(DESCR_t *args, int nargs) {
    if (nargs <= 0 || !args) { char *e = rt_ws_alloc(1); e[0] = '\0'; return STRVAL(e); }
    size_t total = 0;
    for (int i = 0; i < nargs; i++) { char scratch[64]; const char *cs = to_cstring(args[i], scratch, sizeof scratch); total += strlen(cs) + 1; }
    char *buf = rt_ws_alloc(total + 1); size_t p = 0;
    for (int i = 0; i < nargs; i++) { char scratch[64]; const char *cs = to_cstring(args[i], scratch, sizeof scratch); size_t L = strlen(cs); if (p > 0) buf[p++] = SOH; memcpy(buf + p, cs, L); p += L; }
    buf[p] = '\0';
    return STRVAL(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs) {
    if (nargs <= 0 || !args) { char *e = rt_ws_alloc(1); e[0] = '\0'; return STRVAL(e); }
    const char **els = rt_ws_alloc((size_t)nargs * 64 * sizeof(const char *));
    size_t *lens = rt_ws_alloc((size_t)nargs * 64 * sizeof(size_t));
    int nel = 0, cap = nargs * 64;
    for (int i = 0; i < nargs; i++) {
        char scratch[64];
        const char *cs = to_cstring(args[i], scratch, sizeof scratch);
        const char *seg = cs;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            if (nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
            if (!nx) break;
            seg = nx + 1;
        }
    }
    size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
    char *buf = rt_ws_alloc(total + 1); size_t p = 0;
    for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
    buf[p] = '\0';
    return STRVAL(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!strcmp(fn, "where") && nargs == 1) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        if (!IS_INT_fn(args[0]) && !IS_FH_fn(args[0])) { *out = FAILDESCR; return 1; }
        fh_ensure_init();
        FILE *fp = fh_get((int)args[0].i);
        if (!fp) { *out = FAILDESCR; return 1; }
        long pos = ftell(fp);
        if (pos < 0) { *out = FAILDESCR; return 1; }
        *out = INTVAL(pos + 1);
        return 1;
    }
    if (!strcmp(fn, "seek") && (nargs == 1 || nargs == 2)) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        if (!IS_INT_fn(args[0]) && !IS_FH_fn(args[0])) { *out = FAILDESCR; return 1; }
        fh_ensure_init();
        FILE *fp = fh_get((int)args[0].i);
        if (!fp) { *out = FAILDESCR; return 1; }
        long o = (nargs == 2 && IS_INT_fn(args[1])) ? (long)args[1].i : 1L;
        int rc = (o > 0) ? fseek(fp, o - 1, SEEK_SET) : fseek(fp, o, SEEK_END);
        if (rc != 0) { *out = FAILDESCR; return 1; }
        *out = args[0]; return 1;
    }
    if (!fn) return 0;
    extern int fh_capture_begin(char **, size_t *, int *); extern void fh_capture_end(int, int);
    if (!strcmp(fn, "__rk_undef")) { (void) args; (void) nargs; *out = NULVCL; return 1; }
    if (!strcmp(fn, "__rk_exit") && nargs == 1) {
        long code = IS_INT_fn(args[0]) ? args[0].i : 0;
        exit((int) code);
    }
    if (!strcmp(fn, "__rk_arr_at") && nargs == 2) {
        if (args[0].v == DT_A && args[0].arr) {
            ARBLK_t *b = (ARBLK_t *) args[0].arr; long i = IS_INT_fn(args[1]) ? args[1].i : 0; if (i < b->lo || i > b->hi) { *out = NULVCL; return 1; } *out = b->data[i - b->lo]; return 1;
        }
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        if (idx < 0) { *out = NULVCL; return 1; }
        if (idx == 0 && !*cur) { *out = NULVCL; return 1; }
        if (idx >= 1 && !strchr(cur, SOH) && (size_t)idx <= strlen(cur)) { *out = INTVAL((long long)(unsigned char)cur[idx - 1]); return 1; }
        const char *seg = cur; long k = 0;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            if (k == idx) { size_t elen = nx ? (size_t)(nx - seg) : strlen(seg); *out = elem_to_descr(seg, elen); return 1; }
            if (!nx) { *out = NULVCL; return 1; }
            seg = nx + 1; k++;
        }
    }
    if (!strncmp(fn, "__rk_test_", 10)) {
        const char *op = fn + 10; char sb1[512]; char sb2[512]; char msg[1024];
        if (!strcmp(op, "plan")) { long n = (nargs > 0 && IS_INT_fn(args[0])) ? (long)args[0].i : 0; g_tap_planned = n; g_tap_no_plan = 0; printf("1..%ld\n", n); fflush(stdout); *out = NULVCL; return 1; }
        if (!strcmp(op, "ok")) { int c = (nargs > 0) ? rk_tap_truthy(args[0]) : 0; const char *d = (nargs > 1) ? to_cstring(args[1], sb1, sizeof sb1) : ""; rk_tap_proclaim(c, d, ""); *out = INTVAL(c); return 1; }
        if (!strcmp(op, "nok")) { int c = (nargs > 0) ? !rk_tap_truthy(args[0]) : 1; const char *d = (nargs > 1) ? to_cstring(args[1], sb1, sizeof sb1) : ""; rk_tap_proclaim(c, d, ""); *out = INTVAL(c); return 1; }
        if (!strcmp(op, "is")) { const char *g = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; const char *e = (nargs > 1) ? to_cstring(args[1], sb2, sizeof sb2) : ""; int c = !strcmp(g, e); char gd[512]; char ed[512]; snprintf(gd, sizeof gd, "%s", g); snprintf(ed, sizeof ed, "%s", e); const char *d = (nargs > 2) ? to_cstring(args[2], msg, sizeof msg) : ""; rk_tap_proclaim(c, d, ""); if (!c) { char b[1024]; snprintf(b, sizeof b, "expected: '%s'", ed); rk_tap_diag(b); snprintf(b, sizeof b, "     got: '%s'", gd); rk_tap_diag(b); } *out = INTVAL(c); return 1; }
        if (!strcmp(op, "isnt")) { const char *g = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; const char *e = (nargs > 1) ? to_cstring(args[1], sb2, sizeof sb2) : ""; int c = strcmp(g, e) != 0; const char *d = (nargs > 2) ? to_cstring(args[2], msg, sizeof msg) : ""; rk_tap_proclaim(c, d, ""); *out = INTVAL(c); return 1; }
        if (!strcmp(op, "pass")) { const char *d = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; rk_tap_proclaim(1, d, ""); *out = INTVAL(1); return 1; }
        if (!strcmp(op, "flunk")) { const char *d = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; rk_tap_proclaim(0, d, ""); *out = INTVAL(0); return 1; }
        if (!strcmp(op, "diag")) { const char *d = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; rk_tap_diag(d); *out = NULVCL; return 1; }
        if (!strcmp(op, "todo")) { const char *r = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; long n = (nargs > 1 && IS_INT_fn(args[1])) ? (long)args[1].i : 1; g_tap_todo_upto = g_tap_run + n; snprintf(g_tap_todo_reason, sizeof g_tap_todo_reason, " # TODO %s", r); *out = NULVCL; return 1; }
        if (!strcmp(op, "skip")) { const char *r = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : ""; long n = (nargs > 1 && IS_INT_fn(args[1])) ? (long)args[1].i : 1; for (long i = 0; i < n; i++) rk_tap_proclaim(1, r, "# SKIP "); *out = NULVCL; return 1; }
        if (!strcmp(op, "skip_rest")) { const char *r = (nargs > 0) ? to_cstring(args[0], sb1, sizeof sb1) : "<unknown>"; if (g_tap_no_plan) { rk_tap_diag("A plan is required in order to use skip-rest"); *out = NULVCL; return 1; } for (long i = g_tap_run; i < g_tap_planned; ) { rk_tap_proclaim(1, r, "# SKIP "); i = g_tap_run; } *out = NULVCL; return 1; }
        if (!strcmp(op, "done")) { g_tap_done_run = 1; if (g_tap_no_plan) { g_tap_planned = g_tap_run; printf("1..%ld\n", g_tap_planned); fflush(stdout); } if ((g_tap_planned || g_tap_run) && g_tap_planned != g_tap_run) { snprintf(msg, sizeof msg, "You planned %ld test%s, but ran %ld", g_tap_planned, g_tap_planned == 1 ? "" : "s", g_tap_run); rk_tap_diag(msg); } if (g_tap_failed) { snprintf(msg, sizeof msg, "You failed %ld test%s of %ld", g_tap_failed, g_tap_failed == 1 ? "" : "s", g_tap_run); rk_tap_diag(msg); } *out = NULVCL; return 1; }
        *out = NULVCL; return 1;
    }
    if (!strncmp(fn, "$ax_", 4)) return dop_ax(fn + 4, args, nargs, out);
    if (!strcmp(fn, "$is_v") && nargs == 2) return dop_is_v(args, nargs, out);
    if (!strncmp(fn, "$is_", 4) && nargs == 3) {
        extern DESCR_t rt_pl_deref_val(DESCR_t); extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
        DESCR_t a = rt_pl_deref_val(args[1]); DESCR_t b = rt_pl_deref_val(args[2]); DESCR_t r = pl_arith2(fn + 4, a, b);
        if (r.v == DT_FAIL) { *out = FAILDESCR; return 1; }
        if (plw_unify_vals(args[0], r)) { *out = r; return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strncmp(fn, "$cmp_", 5) && nargs == 2) return dop_cmp(fn + 5, args, nargs, out);
    if (!strcmp(fn, "$succ") && nargs == 2) {
        extern DESCR_t rt_pl_deref_val(DESCR_t);
        DESCR_t a = rt_pl_deref_val(args[0]); DESCR_t b = rt_pl_deref_val(args[1]); DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0;
        if (a.v == DT_I) { r.i = a.i + 1; if (plw_unify_vals(args[1], r)) { *out = r; return 1; } *out = FAILDESCR; return 1; }
        if (b.v == DT_I) { if (b.i <= 0) { *out = FAILDESCR; return 1; } r.i = b.i - 1; if (plw_unify_vals(args[0], r)) { *out = r; return 1; } *out = FAILDESCR; return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$plus") && nargs == 3) {
        extern DESCR_t rt_pl_deref_val(DESCR_t);
        DESCR_t a = rt_pl_deref_val(args[0]); DESCR_t b = rt_pl_deref_val(args[1]); DESCR_t c = rt_pl_deref_val(args[2]); DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0;
        if (a.v == DT_I && b.v == DT_I) { r.i = a.i + b.i; if (plw_unify_vals(args[2], r)) { *out = r; return 1; } *out = FAILDESCR; return 1; }
        if (a.v == DT_I && c.v == DT_I) { r.i = c.i - a.i; if (plw_unify_vals(args[1], r)) { *out = r; return 1; } *out = FAILDESCR; return 1; }
        if (b.v == DT_I && c.v == DT_I) { r.i = c.i - b.i; if (plw_unify_vals(args[0], r)) { *out = r; return 1; } *out = FAILDESCR; return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$atom_length") && nargs == 2) {
        extern DESCR_t rt_pl_deref_val(DESCR_t);
        const char *s = pl_atom_str(rt_pl_deref_val(args[0])); if (!s) { *out = FAILDESCR; return 1; }
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = (long long)strlen(s);
        if (plw_unify_vals(args[1], r)) { *out = r; return 1; } *out = FAILDESCR; return 1;
    }
    if ((!strcmp(fn, "$upcase_atom") || !strcmp(fn, "$downcase_atom")) && nargs == 2) {
        extern DESCR_t rt_pl_deref_val(DESCR_t);
        const char *s = pl_atom_str(rt_pl_deref_val(args[0])); if (!s) { *out = FAILDESCR; return 1; }
        size_t n = strlen(s); char *o = (char *)rt_ws_alloc(n + 1);
        for (size_t i = 0; i < n; i++) o[i] = (fn[1] == 'u') ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        o[n] = 0; DESCR_t r = pl_mk_atom(o);
        if (plw_unify_vals(args[1], r)) { *out = r; return 1; } *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$atom_concat") && nargs == 3) {
        extern DESCR_t rt_pl_deref_val(DESCR_t);
        const char *a = pl_atom_str(rt_pl_deref_val(args[0])); const char *b = pl_atom_str(rt_pl_deref_val(args[1]));
        if (!a || !b) { *out = FAILDESCR; return 1; }
        size_t na = strlen(a), nb = strlen(b); char *o = (char *)rt_ws_alloc(na + nb + 1);
        memcpy(o, a, na); memcpy(o + na, b, nb); o[na + nb] = 0; DESCR_t r = pl_mk_atom(o);
        if (plw_unify_vals(args[2], r)) { *out = r; return 1; } *out = FAILDESCR; return 1;
    }
    if ((!strcmp(fn, "$atom_chars") || !strcmp(fn, "$atom_codes") || !strcmp(fn, "$string_chars") || !strcmp(fn, "$string_codes")) && nargs == 2) {
        extern DESCR_t rt_pl_deref_val(DESCR_t);
        int codes = !strcmp(fn, "$atom_codes") || !strcmp(fn, "$string_codes");
        DESCR_t a0 = rt_pl_deref_val(args[0]); const char *s = pl_atom_str(a0);
        if (s) {
            size_t n = strlen(s); DESCR_t *elems = (DESCR_t *)rt_ws_alloc((n > 0 ? n : 1) * sizeof(DESCR_t));
            for (size_t i = 0; i < n; i++) {
                if (codes) { elems[i].v = (DTYPE_t)DT_I; elems[i].slen = 0; elems[i].i = (unsigned char)s[i]; }
                else { char *o = (char *)rt_ws_alloc(2); o[0] = s[i]; o[1] = 0; elems[i] = pl_mk_atom(o); }
            }
            DESCR_t lst = pl_list_from_arr(elems, (int)n);
            if (plw_unify_vals(args[1], lst)) { *out = lst; return 1; } *out = FAILDESCR; return 1;
        }
        DESCR_t elems[4096]; int n = pl_list_to_arr(args[1], elems, 4096);
        if (n < 0) { *out = FAILDESCR; return 1; }
        char *o = (char *)rt_ws_alloc((size_t)n + 1);
        for (int i = 0; i < n; i++) {
            if (codes) { if (elems[i].v != DT_I) { *out = FAILDESCR; return 1; } o[i] = (char)elems[i].i; }
            else { const char *cs = pl_atom_str(elems[i]); if (!cs || !cs[0]) { *out = FAILDESCR; return 1; } o[i] = cs[0]; }
        }
        o[n] = 0; DESCR_t r = pl_mk_atom(o);
        if (plw_unify_vals(args[0], r)) { *out = r; return 1; } *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$name") && nargs == 2) {
        extern DESCR_t rt_pl_deref_val(DESCR_t);
        DESCR_t a0 = rt_pl_deref_val(args[0]);
        if (a0.v == DT_S || a0.v == (DTYPE_t)DT_A || a0.v == DT_I || a0.v == DT_R) {
            char nb[64]; const char *s;
            if (a0.v == DT_I) { snprintf(nb, sizeof nb, "%lld", (long long)a0.i); s = nb; }
            else if (a0.v == DT_R) { pl_real_iso_str(a0.r, nb, sizeof nb); s = nb; }
            else s = pl_atom_str(a0);
            size_t n = strlen(s); DESCR_t *elems = (DESCR_t *)rt_ws_alloc((n > 0 ? n : 1) * sizeof(DESCR_t));
            for (size_t i = 0; i < n; i++) { elems[i].v = (DTYPE_t)DT_I; elems[i].slen = 0; elems[i].i = (unsigned char)s[i]; }
            DESCR_t lst = pl_list_from_arr(elems, (int)n);
            if (plw_unify_vals(args[1], lst)) { *out = lst; return 1; } *out = FAILDESCR; return 1;
        }
        { char buf[1025]; int n; DESCR_t a1 = rt_pl_deref_val(args[1]);
          if (a1.v == DT_S || a1.v == (DTYPE_t)DT_A) { const char *cs = pl_atom_str(a1); if (!cs) { *out = FAILDESCR; return 1; } n = (int)strlen(cs); if (n > 1024) n = 1024; memcpy(buf, cs, (size_t)n); }
          else { DESCR_t elems[1024]; n = pl_list_to_arr(args[1], elems, 1024);
            if (n < 0) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
            for (int i = 0; i < n; i++) { if (elems[i].v != DT_I) { *out = FAILDESCR; return 1; } buf[i] = (char)elems[i].i; } }
          buf[n] = 0;
          char *e1 = (char *)0, *e2 = (char *)0; long long iv = strtoll(buf, &e1, 10); double dv = strtod(buf, &e2);
          DESCR_t r;
          if (n > 0 && e2 > e1 && e2 && !*e2) { r.v = DT_R; r.slen = 0; r.r = dv; }
          else if (n > 0 && e1 && !*e1 && e1 != buf) { r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = iv; }
          else { char *o = (char *)rt_ws_alloc((size_t)n + 1); memcpy(o, buf, (size_t)n); o[n] = 0; r = pl_mk_atom(o); }
          if (plw_unify_vals(args[0], r)) { *out = r; return 1; } *out = FAILDESCR; return 1; }
    }
    if (!strcmp(fn, "$op") && nargs == 3) {
        extern DESCR_t rt_pl_deref_val(DESCR_t);
        extern int prolog_op_table_add(const char *, int, const char *);
        extern Term *rt_pl_cell_to_term(void *);
        DESCR_t pv = rt_pl_deref_val(args[0]); DESCR_t tv = rt_pl_deref_val(args[1]);
        if (pv.v != DT_I) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        const char *tystr = pl_atom_str(tv); if (!tystr || !tystr[0]) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        int prec = (int)pv.i; int added = 0;
        DESCR_t nv = rt_pl_deref_val(args[2]); const char *single = pl_atom_str(nv);
        if (single && single[0] && strcmp(single, "[]") != 0) { added = prolog_op_table_add(single, prec, tystr); }
        else { DESCR_t tmp = args[2]; Term *lt = rt_pl_cell_to_term(&tmp); lt = lt ? term_deref(lt) : (Term *)0; int guard = 0;
            while (lt && lt->tag == TERM_COMPOUND && lt->compound.arity == 2 && guard++ < 4096) { Term *hd = term_deref(lt->compound.args[0]); if (hd && hd->tag == TERM_ATOM) { extern const char *prolog_atom_name(int); const char *hn = prolog_atom_name(hd->atom_id); if (hn && prolog_op_table_add(hn, prec, tystr)) added = 1; } lt = term_deref(lt->compound.args[1]); } }
        if (!added) { rt_pl_iso_throw_pi("domain_error", "operator_specifier", tystr, 0); *out = FAILDESCR; return 1; }
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$set_prolog_flag") && nargs == 2) {
        extern int rt_pl_set_prolog_flag(DESCR_t, DESCR_t);
        if (rt_pl_set_prolog_flag(args[0], args[1])) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$current_output") && nargs == 1) {
        extern int fh_current_output(void);
        DESCR_t v = rt_pl_deref_val(args[0]);
        if ((int)v.v == DT_A || v.v == DT_S) { rt_pl_iso_throw_domain("stream", v); *out = FAILDESCR; return 1; }
        DESCR_t st = pl_stream_term(fh_current_output());
        if (plw_unify_vals(args[0], st)) { *out = st; return 1; } *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$current_input") && nargs == 1) {
        extern int fh_current_input(void);
        DESCR_t v = rt_pl_deref_val(args[0]);
        if ((int)v.v == DT_A || v.v == DT_S) { rt_pl_iso_throw_domain("stream", v); *out = FAILDESCR; return 1; }
        DESCR_t st = pl_stream_term(fh_current_input());
        if (plw_unify_vals(args[0], st)) { *out = st; return 1; } *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$set_output") && nargs == 1) {
        extern void fh_set_output(int); int idx = pl_resolve_stream_arg(args[0], "set_output/1", 1);
        if (idx < 0) { *out = FAILDESCR; return 1; }
        fh_set_output(idx); DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$set_input") && nargs == 1) {
        extern void fh_set_input(int); int idx = pl_resolve_stream_arg(args[0], "set_input/1", 0);
        if (idx < 0) { *out = FAILDESCR; return 1; }
        fh_set_input(idx); DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$flush_output") && (nargs == 0 || nargs == 1)) {
        extern FILE *fh_cur_out_fp(void); fflush(fh_cur_out_fp());
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$flush_output1") && nargs == 1) {
        extern FILE *fh_get(int); int idx = pl_resolve_stream_arg(args[0], "flush_output/1", 1);
        if (idx < 0) { *out = FAILDESCR; return 1; }
        FILE *fp = fh_get(idx); if (fp) fflush(fp); DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$char_code") && nargs == 2) {
        extern DESCR_t rt_pl_deref_val(DESCR_t);
        DESCR_t a = rt_pl_deref_val(args[0]); const char *cs = pl_atom_str(a);
        if (cs && cs[0] && !cs[1]) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = (unsigned char)cs[0]; if (plw_unify_vals(args[1], r)) { *out = r; return 1; } *out = FAILDESCR; return 1; }
        { DESCR_t b = rt_pl_deref_val(args[1]);
          if (b.v == DT_I && b.i >= 0 && b.i < 256) { char *o = (char *)rt_ws_alloc(2); o[0] = (char)b.i; o[1] = 0; DESCR_t r = pl_mk_atom(o); if (plw_unify_vals(args[0], r)) { *out = r; return 1; } } }
        if ((a.v == (DTYPE_t)DT_PLVAR || a.v == DT_SNUL || a.v == DT_FAIL)) { DESCR_t b = rt_pl_deref_val(args[1]); if (b.v == (DTYPE_t)DT_PLVAR || b.v == DT_SNUL || b.v == DT_FAIL) rt_pl_iso_throw_instantiation(); }
        *out = FAILDESCR; return 1;
    }
    if ((!strcmp(fn, "$number_chars") || !strcmp(fn, "$number_codes")) && nargs == 2) {
        extern DESCR_t rt_pl_deref_val(DESCR_t);
        int codes = (fn[9] == 'o');
        DESCR_t n0 = rt_pl_deref_val(args[0]);
        if (n0.v == DT_I || n0.v == DT_R) {
            char buf[64]; if (n0.v == DT_I) snprintf(buf, sizeof buf, "%lld", (long long)n0.i); else pl_real_iso_str(n0.r, buf, sizeof buf);
            size_t bl = strlen(buf); DESCR_t *elems = (DESCR_t *)rt_ws_alloc((bl > 0 ? bl : 1) * sizeof(DESCR_t));
            for (size_t i = 0; i < bl; i++) {
                if (codes) { elems[i].v = (DTYPE_t)DT_I; elems[i].slen = 0; elems[i].i = (unsigned char)buf[i]; }
                else { char *o = (char *)rt_ws_alloc(2); o[0] = buf[i]; o[1] = 0; elems[i] = pl_mk_atom(o); }
            }
            DESCR_t lst = pl_list_from_arr(elems, (int)bl);
            if (plw_unify_vals(args[1], lst)) { *out = lst; return 1; } *out = FAILDESCR; return 1;
        }
        { DESCR_t a1 = rt_pl_deref_val(args[1]); char buf[513]; int n;
          if (a1.v == DT_S || a1.v == (DTYPE_t)DT_A) { const char * cs = pl_atom_str(a1); if (!cs) { *out = FAILDESCR; return 1; } n = (int)strlen(cs); if (n > 512) n = 512; memcpy(buf, cs, (size_t)n); }
          else { DESCR_t elems[512]; n = pl_list_to_arr(args[1], elems, 512);
          if (n < 0) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
          for (int i = 0; i < n; i++) { if (codes) { if (elems[i].v != DT_I) { *out = FAILDESCR; return 1; } buf[i] = (char)elems[i].i; } else { const char *cs = pl_atom_str(elems[i]); if (!cs || !cs[0]) { *out = FAILDESCR; return 1; } buf[i] = cs[0]; } } }
          buf[n] = 0;
          char *e1 = (char *)0, *e2 = (char *)0; long long iv = strtoll(buf, &e1, 10); double dv = strtod(buf, &e2);
          DESCR_t r;
          if (e2 > e1 && e2 && !*e2) { r.v = DT_R; r.slen = 0; r.r = dv; }
          else if (e1 && !*e1 && e1 != buf) { r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = iv; }
          else { *out = FAILDESCR; return 1; }
          if (plw_unify_vals(args[0], r)) { *out = r; return 1; } *out = FAILDESCR; return 1; }
    }
    if (!strcmp(fn, "$write") && nargs == 1) {
        extern FILE *fh_cur_out_fp(void); FILE *_wd = fh_cur_out_fp();
        DESCR_t v = rt_pl_deref_val(args[0]);
        if (v.v == DT_R) { extern const char * pl_real_iso_str(double, char *, int); char _fb[64]; fputs(pl_real_iso_str(v.r, _fb, sizeof _fb), _wd); DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
        out_write_descr(_wd, v, 0);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$put_char") && (nargs == 1 || nargs == 2)) {
        extern FILE *fh_cur_out_fp(void); extern FILE *fh_get(int);
        FILE *f; int vidx; if (nargs == 2) { int idx = pl_resolve_stream_arg(args[0], "put_char/2", 1); if (idx < 0) { *out = FAILDESCR; return 1; } f = fh_get(idx); if (!f) f = fh_cur_out_fp(); vidx = 1; } else { f = fh_cur_out_fp(); vidx = 0; }
        DESCR_t v = rt_pl_deref_val(args[vidx]);
        if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        const char *s = pl_atom_str(v);
        if (!s || !s[0] || s[1]) { rt_pl_iso_throw_type("character", v); *out = FAILDESCR; return 1; }
        fputc((unsigned char)s[0], f);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$at_end_of_stream") && (nargs == 0 || nargs == 1)) {
        extern FILE *fh_cur_in_fp(void); extern FILE *fh_get(int);
        FILE *f; if (nargs == 1) { int idx = pl_resolve_stream_arg(args[0], "at_end_of_stream/1", 0); if (idx < 0) { *out = FAILDESCR; return 1; } f = fh_get(idx); if (!f) f = fh_cur_in_fp(); } else f = fh_cur_in_fp();
        int c = fgetc(f); if (c != EOF) ungetc(c, f);
        if (c == EOF) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$tab") && nargs == 1) {
        extern FILE *fh_cur_out_fp(void); FILE *_td = fh_cur_out_fp();
        DESCR_t v = rt_pl_deref_val(args[0]); long n = IS_INT_fn(v) ? (long)v.i : (IS_REAL_fn(v) ? (long)v.r : 0);
        for (long i = 0; i < n; i++) fputc(' ', _td);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if ((!strcmp(fn, "$get_char") || !strcmp(fn, "$peek_char") || !strcmp(fn, "$get_code") || !strcmp(fn, "$peek_code")) && (nargs == 1 || nargs == 2)) {
        extern FILE *fh_cur_in_fp(void); extern FILE *fh_get(int); extern DESCR_t rt_pl_deref_val(DESCR_t);
        int peek = (!strcmp(fn, "$peek_char") || !strcmp(fn, "$peek_code")); int code = (!strcmp(fn, "$get_code") || !strcmp(fn, "$peek_code")); int ridx = (nargs == 2) ? 1 : 0;
        FILE *f; if (nargs == 2) { int idx = pl_resolve_stream_arg(args[0], code ? (peek ? "peek_code/2" : "get_code/2") : (peek ? "peek_char/2" : "get_char/2"), 0); if (idx < 0) { *out = FAILDESCR; return 1; } f = fh_get(idx); if (!f) f = fh_cur_in_fp(); } else f = fh_cur_in_fp();
        int c = fgetc(f); if (peek && c != EOF) ungetc(c, f);
        DESCR_t r;
        if (code) { r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = (c == EOF) ? -1 : (unsigned char)c; }
        else if (c == EOF) { r = pl_mk_atom("end_of_file"); }
        else { char *o = (char *)rt_ws_alloc(2); o[0] = (char)c; o[1] = 0; r = pl_mk_atom(o); }
        if (plw_unify_vals(args[ridx], r)) { *out = r; return 1; } *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$put_code") && (nargs == 1 || nargs == 2)) {
        extern FILE *fh_cur_out_fp(void); extern FILE *fh_get(int); extern DESCR_t rt_pl_deref_val(DESCR_t);
        FILE *f; int vidx; if (nargs == 2) { int idx = pl_resolve_stream_arg(args[0], "put_code/2", 1); if (idx < 0) { *out = FAILDESCR; return 1; } f = fh_get(idx); if (!f) f = fh_cur_out_fp(); vidx = 1; } else { f = fh_cur_out_fp(); vidx = 0; }
        DESCR_t v = rt_pl_deref_val(args[vidx]);
        if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        long code = IS_INT_fn(v) ? (long)v.i : (IS_REAL_fn(v) ? (long)v.r : -1);
        if (code < 0) { rt_pl_iso_throw_type("integer", v); *out = FAILDESCR; return 1; }
        fputc((int)code, f);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$get1") && nargs == 1) {
        extern FILE *fh_cur_in_fp(void);
        FILE *f = fh_cur_in_fp(); int c; do { c = fgetc(f); } while (c != EOF && c <= 32);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = (c == EOF) ? -1 : (unsigned char)c;
        if (plw_unify_vals(args[0], r)) { *out = r; return 1; } *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$skip1") && nargs == 1) {
        extern FILE *fh_cur_in_fp(void); extern DESCR_t rt_pl_deref_val(DESCR_t);
        DESCR_t v = rt_pl_deref_val(args[0]);
        if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        if (v.v != DT_I) { rt_pl_iso_throw_type("integer", v); *out = FAILDESCR; return 1; }
        long target = (long)v.i; FILE *f = fh_cur_in_fp(); int c; do { c = fgetc(f); } while (c != EOF && c != (int)target);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$number_atom") && nargs == 2) {
        extern DESCR_t rt_pl_deref_val(DESCR_t); DESCR_t a = rt_pl_deref_val(args[1]);
        if (a.v == DT_S || a.v == (DTYPE_t)DT_A) {
            const char *str = pl_atom_str(a); if (!str) { rt_pl_iso_throw_type("atom", a); *out = FAILDESCR; return 1; }
            char *end; long long iv = strtoll(str, &end, 10); DESCR_t num;
            if (end != str && *end == 0) { num.v = (DTYPE_t)DT_I; num.slen = 0; num.i = (long)iv; }
            else { double dv = strtod(str, &end); if (end == str || *end != 0) { *out = FAILDESCR; return 1; } num.v = (DTYPE_t)DT_R; num.slen = 0; num.r = dv; }
            if (plw_unify_vals(args[0], num)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; } *out = FAILDESCR; return 1;
        }
        if (a.v == (DTYPE_t)DT_PLVAR || a.v == DT_SNUL || a.v == DT_FAIL) {
            DESCR_t n = rt_pl_deref_val(args[0]); char nb[64];
            if (n.v == (DTYPE_t)DT_PLVAR || n.v == DT_SNUL || n.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
            if (IS_INT_fn(n)) snprintf(nb, sizeof nb, "%ld", (long)n.i);
            else if (IS_REAL_fn(n)) { const char *fs = pl_real_iso_str(n.r, nb, sizeof nb); if (fs != nb) { size_t L = strlen(fs); if (L < sizeof nb) memcpy(nb, fs, L + 1); } }
            else { rt_pl_iso_throw_type("number", n); *out = FAILDESCR; return 1; }
            DESCR_t atm = pl_mk_atom_dup(nb, strlen(nb));
            if (plw_unify_vals(args[1], atm)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; } *out = FAILDESCR; return 1;
        }
        rt_pl_iso_throw_type("atom", a); *out = FAILDESCR; return 1;
    }
    if ((!strcmp(fn, "$get_byte") || !strcmp(fn, "$peek_byte")) && (nargs == 1 || nargs == 2)) {
        extern FILE *fh_cur_in_fp(void); extern FILE *fh_get(int); extern DESCR_t rt_pl_deref_val(DESCR_t);
        int peek = !strcmp(fn, "$peek_byte"); int ridx = (nargs == 2) ? 1 : 0;
        FILE *f; if (nargs == 2) { int idx = pl_resolve_stream_arg(args[0], peek ? "peek_byte/2" : "get_byte/2", 0); if (idx < 0) { *out = FAILDESCR; return 1; } f = fh_get(idx); if (!f) f = fh_cur_in_fp(); } else f = fh_cur_in_fp();
        { DESCR_t bv = rt_pl_deref_val(args[ridx]); if (bv.v == DT_I) { if (bv.i < -1 || bv.i > 255) { rt_pl_iso_throw_type("in_byte", bv); *out = FAILDESCR; return 1; } } else if (bv.v == (DTYPE_t)DT_A || bv.v == DT_S) { rt_pl_iso_throw_type("in_byte", bv); *out = FAILDESCR; return 1; } }
        int c = fgetc(f); if (peek && c != EOF) ungetc(c, f);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = (c == EOF) ? -1 : (unsigned char)c;
        if (plw_unify_vals(args[ridx], r)) { *out = r; return 1; } *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$put_byte") && (nargs == 1 || nargs == 2)) {
        extern FILE *fh_cur_out_fp(void); extern FILE *fh_get(int); extern DESCR_t rt_pl_deref_val(DESCR_t);
        FILE *f; int vidx; if (nargs == 2) { int idx = pl_resolve_stream_arg(args[0], "put_byte/2", 1); if (idx < 0) { *out = FAILDESCR; return 1; } f = fh_get(idx); if (!f) f = fh_cur_out_fp(); vidx = 1; } else { f = fh_cur_out_fp(); vidx = 0; }
        DESCR_t v = rt_pl_deref_val(args[vidx]);
        if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        if (v.v != DT_I) { rt_pl_iso_throw_type("byte", v); *out = FAILDESCR; return 1; }
        if (v.i < 0 || v.i > 255) { rt_pl_iso_throw_type("byte", v); *out = FAILDESCR; return 1; }
        fputc((int)v.i, f);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if ((!strcmp(fn, "$unget_char") || !strcmp(fn, "$unget_code") || !strcmp(fn, "$unget_byte")) && (nargs == 1 || nargs == 2)) {
        extern FILE *fh_cur_in_fp(void); extern FILE *fh_get(int); extern DESCR_t rt_pl_deref_val(DESCR_t);
        int kind = (!strcmp(fn, "$unget_char")) ? 0 : (!strcmp(fn, "$unget_code")) ? 1 : 2; int vidx = (nargs == 2) ? 1 : 0;
        FILE *f; if (nargs == 2) { int idx = pl_resolve_stream_arg(args[0], (kind == 0) ? "unget_char/2" : (kind == 1) ? "unget_code/2" : "unget_byte/2", 0); if (idx < 0) { *out = FAILDESCR; return 1; } f = fh_get(idx); if (!f) f = fh_cur_in_fp(); } else f = fh_cur_in_fp();
        DESCR_t v = rt_pl_deref_val(args[vidx]);
        if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        int ch;
        if (kind == 0) { const char *cs = pl_atom_str(v); if (!cs || !cs[0] || cs[1]) { rt_pl_iso_throw_type("character", v); *out = FAILDESCR; return 1; } ch = (unsigned char)cs[0]; }
        else if (kind == 1) { if (v.v != DT_I) { rt_pl_iso_throw_type("integer", v); *out = FAILDESCR; return 1; } if (v.i == -1) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; } if (v.i < 0 || v.i > 255) { rt_pl_iso_throw_type("integer", v); *out = FAILDESCR; return 1; } ch = (int)v.i; }
        else { if (v.v != DT_I) { rt_pl_iso_throw_type("byte", v); *out = FAILDESCR; return 1; } if (v.i == -1) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; } if (v.i < 0 || v.i > 255) { rt_pl_iso_throw_type("byte", v); *out = FAILDESCR; return 1; } ch = (int)v.i; }
        ungetc(ch, f);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$open") && (nargs == 3 || nargs == 4)) {
        extern int fh_alloc(FILE *); extern char *fh_name[]; extern DESCR_t rt_pl_deref_val(DESCR_t); extern const char *prolog_atom_name(int);
        DESCR_t ss = rt_pl_deref_val(args[0]);
        if (ss.v == (DTYPE_t)DT_PLVAR || ss.v == DT_SNUL || ss.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        if (ss.v != (DTYPE_t)DT_A && ss.v != DT_S) { rt_pl_iso_throw_domain("source_sink", ss); *out = FAILDESCR; return 1; }
        const char *path = pl_atom_str(ss); if (!path || !path[0]) { rt_pl_iso_throw_domain("source_sink", ss); *out = FAILDESCR; return 1; }
        DESCR_t mo = rt_pl_deref_val(args[1]);
        if (mo.v == (DTYPE_t)DT_PLVAR || mo.v == DT_SNUL || mo.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        if (mo.v != (DTYPE_t)DT_A && mo.v != DT_S) { rt_pl_iso_throw_type("atom", mo); *out = FAILDESCR; return 1; }
        const char *ms = pl_atom_str(mo); if (!ms) { rt_pl_iso_throw_type("atom", mo); *out = FAILDESCR; return 1; }
        int binary = 0;
        if (nargs == 4) { DESCR_t oel[32]; int on = pl_list_to_arr(args[3], oel, 32); if (on < 0) { rt_pl_iso_throw_type("list", rt_pl_deref_val(args[3])); *out = FAILDESCR; return 1; } for (int i = 0; i < on; i++) { DESCR_t o = rt_pl_deref_val(oel[i]); if ((int)o.v == DT_PLREF && o.p) { const char *ofn = prolog_atom_name((int)(o.slen >> 16)); int oar = (int)(o.slen & 0xFFFFu); if (ofn && !strcmp(ofn, "type") && oar == 1) { const char *ts = pl_atom_str(rt_pl_deref_val(((DESCR_t *)o.p)[0])); if (ts && !strcmp(ts, "binary")) binary = 1; else if (ts && !strcmp(ts, "text")) binary = 0; } } } }
        const char *fmode; if (!strcmp(ms, "read")) fmode = binary ? "rb" : "r"; else if (!strcmp(ms, "write")) fmode = binary ? "wb" : "w"; else if (!strcmp(ms, "append")) fmode = binary ? "ab" : "a"; else { rt_pl_iso_throw_domain("io_mode", mo); *out = FAILDESCR; return 1; }
        errno = 0; FILE *fp = fopen(path, fmode);
        if (!fp) { if (errno == ENOENT || errno == ENOTDIR) rt_pl_iso_throw_existence("source_sink", ss); else rt_pl_iso_throw_permission("open", "source_sink", path, 0); *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp); if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        fh_name[idx] = strdup(path);
        { extern char fh_mode[]; extern char fh_type[]; fh_mode[idx] = ms[0]; fh_type[idx] = binary ? 'b' : 't'; }
        DESCR_t st = pl_stream_term(idx);
        if (plw_unify_vals(args[2], st)) { *out = st; return 1; } *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$close") && (nargs == 1 || nargs == 2)) {
        extern FILE *fh_get(int); extern void fh_free(int); extern char *fh_name[];
        int idx = pl_resolve_stream_arg(args[0], "close", -1); if (idx < 0) { *out = FAILDESCR; return 1; }
        FILE *fp = fh_get(idx);
        if (idx >= 3 && fp) { fflush(fp); fclose(fp); if (fh_name[idx]) { free(fh_name[idx]); fh_name[idx] = (char *)0; } fh_free(idx); }
        else if (fp) fflush(fp);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if ((!strcmp(fn, "$see") || !strcmp(fn, "$tell") || !strcmp(fn, "$append")) && nargs == 1) {
        extern int fh_alloc(FILE *); extern FILE *fh_get(int); extern char *fh_name[]; extern char fh_mode[]; extern char fh_type[]; extern void fh_ensure_init(void); extern void fh_set_input(int); extern void fh_set_output(int);
        int out_dir = strcmp(fn, "$see") != 0; const char *omode = !strcmp(fn, "$append") ? "a" : (out_dir ? "w" : "r"); char mch = !strcmp(fn, "$append") ? 'a' : (out_dir ? 'w' : 'r');
        DESCR_t v = rt_pl_deref_val(args[0]);
        if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        fh_ensure_init(); int idx = -1;
        if ((int)v.v == DT_PLREF) { idx = pl_resolve_stream_arg(v, fn, out_dir); if (idx < 0) { *out = FAILDESCR; return 1; } }
        else if ((int)v.v == DT_A || v.v == DT_S) { const char *nm = pl_atom_str(v); if (!nm || !nm[0]) { rt_pl_iso_throw_domain("source_sink", v); *out = FAILDESCR; return 1; }
            if (!out_dir && (!strcmp(nm, "user") || !strcmp(nm, "user_input"))) idx = 0;
            else if (out_dir && (!strcmp(nm, "user") || !strcmp(nm, "user_output"))) idx = 1;
            else { int e = pl_stream_alias_idx(nm); if (e >= 3 && fh_get(e) && ((out_dir && (fh_mode[e] == 'w' || fh_mode[e] == 'a')) || (!out_dir && fh_mode[e] == 'r'))) idx = e;
                else { errno = 0; FILE *fp = fopen(nm, omode); if (!fp) { if (errno == ENOENT || errno == ENOTDIR) rt_pl_iso_throw_existence("source_sink", v); else rt_pl_iso_throw_permission("open", "source_sink", nm, 0); *out = FAILDESCR; return 1; }
                    idx = fh_alloc(fp); if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; } fh_name[idx] = strdup(nm); fh_mode[idx] = mch; fh_type[idx] = 't'; } } }
        else { rt_pl_iso_throw_domain("source_sink", v); *out = FAILDESCR; return 1; }
        if (out_dir) fh_set_output(idx); else fh_set_input(idx);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if ((!strcmp(fn, "$seeing") || !strcmp(fn, "$telling")) && nargs == 1) {
        extern int fh_current_input(void); extern int fh_current_output(void); extern char *fh_name[];
        int out_dir = strcmp(fn, "$seeing") != 0; int idx = out_dir ? fh_current_output() : fh_current_input();
        DESCR_t nameterm; if (idx <= 2) nameterm = plc_iso_atom("user"); else if (fh_name[idx] && fh_name[idx][0]) nameterm = pl_mk_atom_dup(fh_name[idx], strlen(fh_name[idx])); else nameterm = pl_stream_term(idx);
        if (plw_unify_vals(args[0], nameterm)) { *out = rt_pl_deref_val(args[0]); return 1; } *out = FAILDESCR; return 1;
    }
    if ((!strcmp(fn, "$seen") || !strcmp(fn, "$told")) && (nargs == 0 || nargs == 1)) {
        extern int fh_current_input(void); extern int fh_current_output(void); extern void fh_set_input(int); extern void fh_set_output(int); extern FILE *fh_get(int); extern void fh_free(int); extern char *fh_name[];
        int out_dir = strcmp(fn, "$seen") != 0; int idx = out_dir ? fh_current_output() : fh_current_input(); FILE *fp = fh_get(idx);
        if (idx >= 3 && fp) { fflush(fp); fclose(fp); if (fh_name[idx]) { free(fh_name[idx]); fh_name[idx] = (char *)0; } fh_free(idx); }
        else if (fp) fflush(fp);
        if (out_dir) fh_set_output(1); else fh_set_input(0);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$write2") && nargs == 2) {
        extern FILE *fh_get(int); int idx = pl_resolve_stream_arg(args[0], "write/2", 1);
        if (idx < 0) { *out = FAILDESCR; return 1; }
        FILE *wd = fh_get(idx); if (!wd) wd = stdout;
        DESCR_t v = rt_pl_deref_val(args[1]);
        if (v.v == DT_R) { extern const char * pl_real_iso_str(double, char *, int); char _fb[64]; fputs(pl_real_iso_str(v.r, _fb, sizeof _fb), wd); }
        else out_write_descr(wd, v, 0);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$nl1") && nargs == 1) {
        extern FILE *fh_get(int); int idx = pl_resolve_stream_arg(args[0], "nl/1", 1);
        if (idx < 0) { *out = FAILDESCR; return 1; }
        FILE *wd = fh_get(idx); if (!wd) wd = stdout; fputc('\n', wd);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$nl0") && (nargs == 0 || nargs == 1)) {
        extern FILE *fh_cur_out_fp(void); fputc('\n', fh_cur_out_fp());
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$halt0") && nargs == 0) { exit(0); }
    if (!strcmp(fn, "$halt1") && nargs == 1) {
        extern DESCR_t rt_pl_deref_val(DESCR_t);
        DESCR_t v = rt_pl_deref_val(args[0]);
        if (v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        if (v.v != DT_I) { rt_pl_iso_throw_type("integer", v); *out = FAILDESCR; return 1; }
        exit((int)v.i);
    }
    if ((!strcmp(fn, "$writeq2") || !strcmp(fn, "$write_canonical2")) && nargs == 2) {
        extern int fh_current_output(void); extern void fh_set_output(int);
        extern void rt_pl_writeq_cell(void *); extern void rt_pl_write_canonical_cell(void *);
        int idx = pl_resolve_stream_arg(args[0], fn[6] == 'q' ? "writeq/2" : "write_canonical/2", 1);
        if (idx < 0) { *out = FAILDESCR; return 1; }
        int old = fh_current_output(); fh_set_output(idx);
        DESCR_t t1 = args[1];
        if (fn[6] == 'q') rt_pl_writeq_cell((void *)plw_det_cell(&t1)); else rt_pl_write_canonical_cell((void *)plw_det_cell(&t1));
        fh_set_output(old);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if ((!strcmp(fn, "$display2") || !strcmp(fn, "$print2")) && nargs == 2) {
        extern int fh_current_output(void); extern void fh_set_output(int);
        extern void rt_pl_display_cell(void *); extern void rt_pl_write_cell(void *);
        int idx = pl_resolve_stream_arg(args[0], fn[1] == 'd' ? "display/2" : "print/2", 1);
        if (idx < 0) { *out = FAILDESCR; return 1; }
        int old = fh_current_output(); fh_set_output(idx);
        DESCR_t t1 = args[1];
        if (fn[1] == 'd') rt_pl_display_cell((void *)plw_det_cell(&t1)); else rt_pl_write_cell((void *)plw_det_cell(&t1));
        fh_set_output(old);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$write_term3") && nargs == 3) {
        extern int fh_current_output(void); extern void fh_set_output(int); extern void rt_pl_write_term_cell(void *, void *);
        int idx = pl_resolve_stream_arg(args[0], "write_term/3", 1);
        if (idx < 0) { *out = FAILDESCR; return 1; }
        int old = fh_current_output(); fh_set_output(idx);
        DESCR_t t1 = args[1], t2 = args[2]; rt_pl_write_term_cell((void *)plw_det_cell(&t1), (void *)plw_det_cell(&t2));
        fh_set_output(old);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$format3") && nargs == 3) {
        extern int fh_current_output(void); extern void fh_set_output(int); extern void rt_pl_format_cell(const char *, void *);
        int sink = pl_sink_kind(args[0]);
        const char *fmt = pl_atom_str(rt_pl_deref_val(args[1]));
        if (!fmt) { *out = FAILDESCR; return 1; }
        if (sink) {
            char *buf = (char *)0; size_t sz = 0; int saved_out = 0;
            int cidx = fh_capture_begin(&buf, &sz, &saved_out);
            if (cidx < 0) { *out = FAILDESCR; return 1; }
            DESCR_t t2 = args[2]; rt_pl_format_cell(fmt, (void *)plw_det_cell(&t2));
            fh_capture_end(cidx, saved_out);
            DESCR_t res = pl_sink_build(sink, buf ? buf : "", sz); if (buf) free(buf);
            DESCR_t inner = ((DESCR_t *)rt_pl_deref_val(args[0]).p)[0];
            if (plw_unify_vals(inner, res)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
        }
        int idx = pl_resolve_stream_arg(args[0], "format/3", 1);
        if (idx < 0) { *out = FAILDESCR; return 1; }
        int old = fh_current_output(); fh_set_output(idx);
        DESCR_t t2 = args[2]; rt_pl_format_cell(fmt, (void *)plw_det_cell(&t2));
        fh_set_output(old);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    {
        int wk = 0, sk = 0;
        if (!strcmp(fn, "$write_to_atom")) { wk = 1; sk = 1; } else if (!strcmp(fn, "$write_to_chars")) { wk = 1; sk = 4; } else if (!strcmp(fn, "$write_to_codes")) { wk = 1; sk = 3; }
        else if (!strcmp(fn, "$writeq_to_atom")) { wk = 2; sk = 1; } else if (!strcmp(fn, "$writeq_to_chars")) { wk = 2; sk = 4; } else if (!strcmp(fn, "$writeq_to_codes")) { wk = 2; sk = 3; }
        else if (!strcmp(fn, "$write_canonical_to_atom")) { wk = 3; sk = 1; } else if (!strcmp(fn, "$write_canonical_to_chars")) { wk = 3; sk = 4; } else if (!strcmp(fn, "$write_canonical_to_codes")) { wk = 3; sk = 3; }
        else if (!strcmp(fn, "$display_to_atom")) { wk = 3; sk = 1; } else if (!strcmp(fn, "$display_to_chars")) { wk = 3; sk = 4; } else if (!strcmp(fn, "$display_to_codes")) { wk = 3; sk = 3; }
        else if (!strcmp(fn, "$print_to_atom")) { wk = 1; sk = 1; } else if (!strcmp(fn, "$print_to_chars")) { wk = 1; sk = 4; } else if (!strcmp(fn, "$print_to_codes")) { wk = 1; sk = 3; }
        if (wk && nargs == 2) {
            extern void rt_pl_write_cell(void *); extern void rt_pl_writeq_cell(void *); extern void rt_pl_write_canonical_cell(void *);
            char *buf = (char *)0; size_t sz = 0; int saved_out = 0;
            int cidx = fh_capture_begin(&buf, &sz, &saved_out);
            if (cidx < 0) { *out = FAILDESCR; return 1; }
            DESCR_t t1 = args[1]; void *tc = (void *)plw_det_cell(&t1);
            if (wk == 1) rt_pl_write_cell(tc); else if (wk == 2) rt_pl_writeq_cell(tc); else rt_pl_write_canonical_cell(tc);
            fh_capture_end(cidx, saved_out);
            DESCR_t res = pl_sink_build(sk, buf ? buf : "", sz); if (buf) free(buf);
            if (plw_unify_vals(args[0], res)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
        }
    }
    {
        int sk = 0;
        if (!strcmp(fn, "$write_term_to_atom")) sk = 1; else if (!strcmp(fn, "$write_term_to_chars")) sk = 4; else if (!strcmp(fn, "$write_term_to_codes")) sk = 3;
        if (sk && nargs == 3) {
            extern void rt_pl_write_term_cell(void *, void *);
            char *buf = (char *)0; size_t sz = 0; int saved_out = 0;
            int cidx = fh_capture_begin(&buf, &sz, &saved_out);
            if (cidx < 0) { *out = FAILDESCR; return 1; }
            DESCR_t t1 = args[1], t2 = args[2]; rt_pl_write_term_cell((void *)plw_det_cell(&t1), (void *)plw_det_cell(&t2));
            fh_capture_end(cidx, saved_out);
            DESCR_t res = pl_sink_build(sk, buf ? buf : "", sz); if (buf) free(buf);
            if (plw_unify_vals(args[0], res)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
        }
    }
    {
        int sk = 0;
        if (!strcmp(fn, "$format_to_chars")) sk = 4; else if (!strcmp(fn, "$format_to_codes")) sk = 3;
        if (sk && nargs == 3) {
            extern void rt_pl_format_cell(const char *, void *);
            const char *fmt = pl_atom_str(rt_pl_deref_val(args[1]));
            if (!fmt) { *out = FAILDESCR; return 1; }
            char *buf = (char *)0; size_t sz = 0; int saved_out = 0;
            int cidx = fh_capture_begin(&buf, &sz, &saved_out);
            if (cidx < 0) { *out = FAILDESCR; return 1; }
            DESCR_t t2 = args[2]; rt_pl_format_cell(fmt, (void *)plw_det_cell(&t2));
            fh_capture_end(cidx, saved_out);
            DESCR_t res = pl_sink_build(sk, buf ? buf : "", sz); if (buf) free(buf);
            if (plw_unify_vals(args[0], res)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
        }
    }
    {
        int rc = 0;
        if (!strcmp(fn, "$read_from_chars")) rc = 0; else if (!strcmp(fn, "$read_from_codes")) rc = 1; else rc = -1;
        if (rc >= 0 && nargs == 2) {
            extern const char *plc_rd_entry(const char *, DESCR_t *, DESCR_t *, char (*)[64], int *, int);
            char *txt = pl_list_to_cstr(rt_pl_deref_val(args[0]), rc);
            if (!txt) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
            DESCR_t tval; DESCR_t bv[64]; char bn[64][64]; int nb = 0;
            const char *e = plc_rd_entry(txt, &tval, bv, bn, &nb, 64); free(txt);
            if (!e) { *out = FAILDESCR; return 1; }
            if (plw_unify_vals(args[1], tval)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
        }
    }
    {
        int src = -1;
        if (!strcmp(fn, "$read_term_from_atom")) src = 2; else if (!strcmp(fn, "$read_term_from_chars")) src = 0; else if (!strcmp(fn, "$read_term_from_codes")) src = 1;
        if (src >= 0 && nargs == 3) {
            extern const char *plc_rd_entry(const char *, DESCR_t *, DESCR_t *, char (*)[64], int *, int); extern const char *prolog_atom_name(int);
            char *txt = (src == 2) ? (char *)0 : pl_list_to_cstr(rt_pl_deref_val(args[0]), src);
            const char *ctxt = (src == 2) ? pl_atom_str(rt_pl_deref_val(args[0])) : txt;
            if (!ctxt) { if (txt) free(txt); rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
            DESCR_t tval; DESCR_t bv[64]; char bn[64][64]; int nb = 0;
            const char *e = plc_rd_entry(ctxt, &tval, bv, bn, &nb, 64); if (txt) free(txt);
            if (!e) { *out = FAILDESCR; return 1; }
            if (!plw_unify_vals(args[1], tval)) { *out = FAILDESCR; return 1; }
            if (!pl_read_apply_opts(args[2], tval, bv, bn, nb)) { *out = FAILDESCR; return 1; }
            DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
        }
    }
    if (!strcmp(fn, "$write_to_atom") && nargs == 2) {
        extern void rt_pl_write_cell(void *);
        char *buf = (char *)0; size_t sz = 0; int saved_out = 0;
        int cidx = fh_capture_begin(&buf, &sz, &saved_out);
        if (cidx < 0) { *out = FAILDESCR; return 1; }
        DESCR_t t1 = args[1]; rt_pl_write_cell((void *)plw_det_cell(&t1));
        fh_capture_end(cidx, saved_out);
        DESCR_t res = pl_mk_atom_dup(buf ? buf : "", sz); if (buf) free(buf);
        if (plw_unify_vals(args[0], res)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$format_to_atom") && nargs == 3) {
        extern void rt_pl_format_cell(const char *, void *);
        const char *fmt = pl_atom_str(rt_pl_deref_val(args[1]));
        if (!fmt) { *out = FAILDESCR; return 1; }
        char *buf = (char *)0; size_t sz = 0; int saved_out = 0;
        int cidx = fh_capture_begin(&buf, &sz, &saved_out);
        if (cidx < 0) { *out = FAILDESCR; return 1; }
        DESCR_t t2 = args[2]; rt_pl_format_cell(fmt, (void *)plw_det_cell(&t2));
        fh_capture_end(cidx, saved_out);
        DESCR_t res = pl_mk_atom_dup(buf ? buf : "", sz); if (buf) free(buf);
        if (plw_unify_vals(args[0], res)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$read_from_atom") && nargs == 2) {
        extern const char *plc_rd_entry(const char *, DESCR_t *, DESCR_t *, char (*)[64], int *, int);
        const char *txt = pl_atom_str(rt_pl_deref_val(args[0]));
        if (!txt) { rt_pl_iso_throw_instantiation(); *out = FAILDESCR; return 1; }
        DESCR_t tval; DESCR_t bv[64]; char bn[64][64]; int nb = 0;
        const char *e = plc_rd_entry(txt, &tval, bv, bn, &nb, 64);
        if (!e) { *out = FAILDESCR; return 1; }
        if (plw_unify_vals(args[1], tval)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$wot_begin") && (nargs == 0 || nargs == 1)) {
        if (pl_wot_sp >= 0 && pl_wot_sp + 1 >= (int)(sizeof pl_wot_stk / sizeof pl_wot_stk[0])) { *out = FAILDESCR; return 1; }
        int slot = pl_wot_sp + 1; pl_wot_stk[slot].buf = (char *)0; pl_wot_stk[slot].sz = 0; pl_wot_stk[slot].saved_out = 0;
        int cidx = fh_capture_begin(&pl_wot_stk[slot].buf, &pl_wot_stk[slot].sz, &pl_wot_stk[slot].saved_out);
        if (cidx < 0) { *out = FAILDESCR; return 1; }
        pl_wot_stk[slot].idx = cidx; pl_wot_sp = slot;
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$wot_end") && nargs == 1) {
        if (pl_wot_sp < 0) { *out = FAILDESCR; return 1; }
        int slot = pl_wot_sp--; fh_capture_end(pl_wot_stk[slot].idx, pl_wot_stk[slot].saved_out);
        int sink = pl_sink_kind(args[0]);
        char *buf = pl_wot_stk[slot].buf; size_t sz = pl_wot_stk[slot].sz;
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1;
        if (!sink) { if (buf) free(buf); *out = FAILDESCR; return 1; }
        DESCR_t res = pl_sink_build(sink, buf ? buf : "", sz); if (buf) free(buf);
        DESCR_t inner = ((DESCR_t *)rt_pl_deref_val(args[0]).p)[0];
        if (plw_unify_vals(inner, res)) *out = r; else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$wot_abort") && (nargs == 0 || nargs == 1)) {
        if (pl_wot_sp >= 0) { int slot = pl_wot_sp--; fh_capture_end(pl_wot_stk[slot].idx, pl_wot_stk[slot].saved_out); if (pl_wot_stk[slot].buf) free(pl_wot_stk[slot].buf); }
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$findall_new") && nargs == 0) { *out = rt_findall_new(); return 1; }
    if (!strcmp(fn, "$findall_add") && nargs == 2) { rt_findall_add(args[0], args[1]); DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
    if (!strcmp(fn, "$findall_result") && nargs == 1) { *out = rt_findall_result(args[0]); return 1; }
    if ((!strcmp(fn, "$sort") || !strcmp(fn, "$msort")) && nargs == 2) {
        extern int rt_pl_sort_cell(int, void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_sort_cell(fn[1] == 'm', (void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if ((!strcmp(fn, "$bag_prep_b") || !strcmp(fn, "$bag_prep_s")) && nargs == 2) {
        extern int rt_pl_bag_prep_cell(int, void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_bag_prep_cell(fn[10] == 's', (void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$keysort") && nargs == 2) {
        extern int rt_pl_keysort_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_keysort_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$group_pairs_by_key") && nargs == 2) {
        extern int rt_pl_group_pairs_by_key_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_group_pairs_by_key_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$pairs_keys_values") && nargs == 3) {
        extern int rt_pl_pairs_keys_values_cell(void *, void *, void *);
        DESCR_t t0 = args[0], t1 = args[1], t2 = args[2];
        int ok = rt_pl_pairs_keys_values_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1), (void *)plw_det_cell(&t2));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strncmp(fn, "$atop_", 6) && nargs == 2) {
        extern int rt_pl_atop_cell(int, void *, void *);
        int op = (!strcmp(fn + 6, "lt")) ? 0 : (!strcmp(fn + 6, "le")) ? 1 : (!strcmp(fn + 6, "gt")) ? 2 : (!strcmp(fn + 6, "ge")) ? 3 : (!strcmp(fn + 6, "eq")) ? 4 : 5;
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_atop_cell(op, (void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$compare") && nargs == 3) {
        extern int rt_pl_compare_cell(void *, void *, void *);
        DESCR_t t0 = args[0], t1 = args[1], t2 = args[2];
        int ok = rt_pl_compare_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1), (void *)plw_det_cell(&t2));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$can_compare") && nargs == 2) {
        extern int rt_pl_can_compare_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_can_compare_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$term_variables") && (nargs == 2 || nargs == 3)) {
        extern int rt_pl_term_variables_cell(void *, void *, void *);
        DESCR_t t0 = args[0], t1 = args[1], t2 = (nargs == 3) ? args[2] : args[0];
        int ok = rt_pl_term_variables_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1), (nargs == 3) ? (void *)plw_det_cell(&t2) : (void *)0);
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$subsumes_term") && nargs == 2) {
        extern int rt_pl_subsumes_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_subsumes_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$char_type") && nargs == 2) {
        extern int rt_pl_char_type_cell(void *, void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_char_type_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1), (void *)0);
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$lower_upper") && nargs == 2) {
        extern int rt_pl_lower_upper_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_lower_upper_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$numbervars") && nargs == 3) {
        extern int rt_pl_numbervars_cell(void *, void *, void *);
        DESCR_t t0 = args[0], t1 = args[1], t2 = args[2];
        int ok = rt_pl_numbervars_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1), (void *)plw_det_cell(&t2));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$numbervars") && nargs == 1) {
        extern int rt_pl_numbervars1_cell(void *);
        DESCR_t t0 = args[0];
        int ok = rt_pl_numbervars1_cell((void *)plw_det_cell(&t0));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$acyclic_term") && nargs == 1) {
        extern int rt_pl_acyclic_cell(void *);
        DESCR_t t0 = args[0];
        int ok = rt_pl_acyclic_cell((void *)plw_det_cell(&t0));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$get_print_stream") && nargs == 1) {
        extern int rt_pl_get_print_stream_cell(void *);
        DESCR_t t0 = args[0];
        int ok = rt_pl_get_print_stream_cell((void *)plw_det_cell(&t0));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$name_singleton_vars") && nargs == 1) {
        extern int rt_pl_name_singleton_vars_cell(void *);
        DESCR_t t0 = args[0];
        int ok = rt_pl_name_singleton_vars_cell((void *)plw_det_cell(&t0));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$name_query_vars") && nargs == 2) {
        extern int rt_pl_name_query_vars_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_name_query_vars_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$bind_variables") && nargs == 2) {
        extern int rt_pl_bind_variables_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_bind_variables_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$writeq") && nargs == 1) {
        extern void rt_pl_writeq_cell(void *);
        DESCR_t t0 = args[0];
        rt_pl_writeq_cell((void *)plw_det_cell(&t0));
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$display") && nargs == 1) {
        extern void rt_pl_display_cell(void *);
        DESCR_t t0 = args[0];
        rt_pl_display_cell((void *)plw_det_cell(&t0));
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$print") && nargs == 1) {
        extern void rt_pl_write_cell(void *);
        DESCR_t t0 = args[0];
        rt_pl_write_cell((void *)plw_det_cell(&t0));
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$write_canonical") && nargs == 1) {
        extern void rt_pl_write_canonical_cell(void *);
        DESCR_t t0 = args[0];
        rt_pl_write_canonical_cell((void *)plw_det_cell(&t0));
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if ((!strcmp(fn, "$format1") && nargs == 1) || (!strcmp(fn, "$format2") && nargs == 2)) {
        extern void rt_pl_format_cell(const char *, void *);
        const char *fmt = pl_atom_str(rt_pl_deref_val(args[0]));
        if (!fmt) { *out = FAILDESCR; return 1; }
        if (nargs == 2) { DESCR_t t1 = args[1]; rt_pl_format_cell(fmt, (void *)plw_det_cell(&t1)); }
        else rt_pl_format_cell(fmt, (void *)0);
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$write_term") && nargs == 2) {
        extern void rt_pl_write_term_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        rt_pl_write_term_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
    }
    if (!strcmp(fn, "$copy_term") && nargs == 2) {
        extern int rt_pl_copy_term_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_copy_term_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strncmp(fn, "$tt_", 4) && nargs == 1) {
        extern int rt_pl_type_test_cell(void *, const char *);
        DESCR_t t0 = args[0];
        int ok = rt_pl_type_test_cell((void *)plw_det_cell(&t0), fn + 4);
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$functor") && nargs == 3) {
        extern int rt_pl_functor_cell(void *, void *, void *);
        DESCR_t t0 = args[0], t1 = args[1], t2 = args[2];
        int ok = rt_pl_functor_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1), (void *)plw_det_cell(&t2));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$arg") && nargs == 3) {
        extern int rt_pl_arg_cell(void *, void *, void *);
        DESCR_t t0 = args[0], t1 = args[1], t2 = args[2];
        int ok = rt_pl_arg_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1), (void *)plw_det_cell(&t2));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$univ") && nargs == 2) {
        extern int rt_pl_univ_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_univ_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strncmp(fn, "$aop_", 5) && nargs >= 1 && nargs <= 3) {
        extern int rt_pl_atom_op_cell(const char *, void *, void *, void *);
        DESCR_t t0 = args[0], t1, t2;
        void *c0 = (void *)plw_det_cell(&t0), *c1 = (void *)0, *c2 = (void *)0;
        if (nargs > 1) { t1 = args[1]; c1 = (void *)plw_det_cell(&t1); }
        if (nargs > 2) { t2 = args[2]; c2 = (void *)plw_det_cell(&t2); }
        int ok = rt_pl_atom_op_cell(fn + 5, c0, c1, c2);
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$atom_to_term") && nargs == 3) {
        extern DESCR_t rt_pl_deref_val(DESCR_t); extern int rt_pl_term_string_cell(void *, void *);
        extern const char *plc_rd_entry(const char *, DESCR_t *, DESCR_t *, char (*)[64], int *, int);
        DESCR_t a0 = rt_pl_deref_val(args[0]);
        const char *txt = pl_atom_str(a0);
        if (txt) {
            DESCR_t tval; DESCR_t bv[16]; char bn[16][64]; int nb = 0;
            const char *e = plc_rd_entry(txt, &tval, bv, bn, &nb, 16);
            if (!e) { *out = FAILDESCR; return 1; }
            DESCR_t *tc = (DESCR_t *)rt_ws_alloc(sizeof(DESCR_t)); *tc = tval;
            if (!plw_unify_vals(args[1], *tc)) { *out = FAILDESCR; return 1; }
            DESCR_t lst = pl_mk_atom("[]");
            for (int i = nb - 1; i >= 0; i--) { char *nmcp = (char *)rt_ws_alloc(strlen(bn[i]) + 1); strcpy(nmcp, bn[i]); DESCR_t two[2]; two[0] = pl_mk_atom(nmcp); two[1] = bv[i]; DESCR_t pr = plc_iso_comp("=", 2, two); DESCR_t cons[2]; cons[0] = pr; cons[1] = lst; lst = plc_iso_comp(".", 2, cons); }
            if (!plw_unify_vals(args[2], lst)) { *out = FAILDESCR; return 1; }
            DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1;
        }
        { DESCR_t t0 = args[0], t1 = args[1];
          int ok = rt_pl_term_string_cell((void *)plw_det_cell(&t1), (void *)plw_det_cell(&t0));
          if (ok) { DESCR_t nil = pl_mk_atom("[]"); if (!plw_unify_vals(args[2], nil)) ok = 0; }
          if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1; }
    }
    if (!strcmp(fn, "$read") && (nargs == 1 || nargs == 2)) {
        extern const char *plc_rd_entry(const char *, DESCR_t *, DESCR_t *, char (*)[64], int *, int); extern FILE *fh_cur_in_fp(void);
        char rb[8192]; int rn = pl_read_src_from_fp(fh_cur_in_fp(), rb, (int)sizeof rb);
        if (rn < 0) { DESCR_t eofa = pl_mk_atom("end_of_file"); if (!plw_unify_vals(args[0], eofa)) { *out = FAILDESCR; return 1; } DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
        { DESCR_t tval; DESCR_t bv[64]; char bn[64][64]; int nb = 0; const char *e = plc_rd_entry(rb, &tval, bv, bn, &nb, 64); if (!e) { *out = FAILDESCR; return 1; } if (!plw_unify_vals(args[0], tval)) { *out = FAILDESCR; return 1; } DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
    }
    if (!strcmp(fn, "$read2") && nargs == 2) {
        extern const char *plc_rd_entry(const char *, DESCR_t *, DESCR_t *, char (*)[64], int *, int); extern FILE *fh_get(int); extern FILE *fh_cur_in_fp(void);
        int idx = pl_resolve_stream_arg(args[0], "read/2", 0); if (idx < 0) { *out = FAILDESCR; return 1; }
        FILE *f = fh_get(idx); if (!f) f = fh_cur_in_fp();
        char rb[8192]; int rn = pl_read_src_from_fp(f, rb, (int)sizeof rb);
        if (rn < 0) { DESCR_t eofa = pl_mk_atom("end_of_file"); if (!plw_unify_vals(args[1], eofa)) { *out = FAILDESCR; return 1; } DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
        { DESCR_t tval; DESCR_t bv[64]; char bn[64][64]; int nb = 0; const char *e = plc_rd_entry(rb, &tval, bv, bn, &nb, 64); if (!e) { *out = FAILDESCR; return 1; } if (!plw_unify_vals(args[1], tval)) { *out = FAILDESCR; return 1; } DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
    }
    if (!strcmp(fn, "$read_term2") && nargs == 2) {
        extern const char *plc_rd_entry(const char *, DESCR_t *, DESCR_t *, char (*)[64], int *, int); extern FILE *fh_cur_in_fp(void);
        char rb[8192]; int rn = pl_read_src_from_fp(fh_cur_in_fp(), rb, (int)sizeof rb);
        if (rn < 0) { DESCR_t eofa = pl_mk_atom("end_of_file"); if (!plw_unify_vals(args[0], eofa)) { *out = FAILDESCR; return 1; } DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
        { DESCR_t tval; DESCR_t bv[64]; char bn[64][64]; int nb = 0; const char *e = plc_rd_entry(rb, &tval, bv, bn, &nb, 64); if (!e) { *out = FAILDESCR; return 1; } if (!plw_unify_vals(args[0], tval)) { *out = FAILDESCR; return 1; } if (!pl_read_apply_opts(args[1], tval, bv, bn, nb)) { *out = FAILDESCR; return 1; } DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
    }
    if (!strcmp(fn, "$read_term3") && nargs == 3) {
        extern const char *plc_rd_entry(const char *, DESCR_t *, DESCR_t *, char (*)[64], int *, int); extern FILE *fh_get(int); extern FILE *fh_cur_in_fp(void);
        int idx = pl_resolve_stream_arg(args[0], "read_term/3", 0); if (idx < 0) { *out = FAILDESCR; return 1; }
        FILE *f = fh_get(idx); if (!f) f = fh_cur_in_fp();
        char rb[8192]; int rn = pl_read_src_from_fp(f, rb, (int)sizeof rb);
        if (rn < 0) { DESCR_t eofa = pl_mk_atom("end_of_file"); if (!plw_unify_vals(args[1], eofa)) { *out = FAILDESCR; return 1; } DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
        { DESCR_t tval; DESCR_t bv[64]; char bn[64][64]; int nb = 0; const char *e = plc_rd_entry(rb, &tval, bv, bn, &nb, 64); if (!e) { *out = FAILDESCR; return 1; } if (!plw_unify_vals(args[1], tval)) { *out = FAILDESCR; return 1; } if (!pl_read_apply_opts(args[2], tval, bv, bn, nb)) { *out = FAILDESCR; return 1; } DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
    }
    if (!strcmp(fn, "$term_string") && nargs == 2) {
        extern int rt_pl_term_string_cell(void *, void *); extern DESCR_t rt_pl_deref_val(DESCR_t);
        extern const char *plc_rd_entry(const char *, DESCR_t *, DESCR_t *, char (*)[64], int *, int);
        DESCR_t t0 = args[0], t1 = args[1];
        DESCR_t d0 = rt_pl_deref_val(args[0]);
        if (d0.v == (DTYPE_t)DT_PLVAR || d0.v == DT_SNUL || d0.v == DT_FAIL) {
            const char *txt = pl_atom_str(rt_pl_deref_val(args[1]));
            if (txt) {
                DESCR_t tval; DESCR_t bv[16]; char bn[16][64]; int nb = 0;
                const char *e = plc_rd_entry(txt, &tval, bv, bn, &nb, 16);
                if (e && plw_unify_vals(args[0], tval)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; return 1; }
                *out = FAILDESCR; return 1;
            }
        }
        int ok = rt_pl_term_string_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strncmp(fn, "$agg_", 5) && nargs == 2) {
        extern int rt_pl_agg_count_finish(void *, void *);
        extern int rt_pl_agg_sum_finish(void *, void *);
        extern int rt_pl_agg_max_finish(void *, void *);
        extern int rt_pl_agg_min_finish(void *, void *);
        void *acc = (void *)(intptr_t)args[0].i;
        DESCR_t t1 = args[1]; void *rc = (void *)plw_det_cell(&t1);
        int ok = (!strcmp(fn + 5, "count")) ? rt_pl_agg_count_finish(acc, rc)
               : (!strcmp(fn + 5, "sum"))   ? rt_pl_agg_sum_finish(acc, rc)
               : (!strcmp(fn + 5, "max"))   ? rt_pl_agg_max_finish(acc, rc)
               :                              rt_pl_agg_min_finish(acc, rc);
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if ((!strcmp(fn, "$dyn_assertz") || !strcmp(fn, "$dyn_asserta")) && nargs == 1) {
        extern int rt_pl_dyn_assertz_cell(void *, int);
        DESCR_t t0 = args[0];
        int ok = rt_pl_dyn_assertz_cell((void *)plw_det_cell(&t0), fn[11] == 'a');
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$retract") && nargs == 1) {
        extern int rt_pl_dyn_retract_cell(void *);
        DESCR_t t0 = args[0];
        int ok = rt_pl_dyn_retract_cell((void *)plw_det_cell(&t0));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$abolish") && nargs == 2) {
        extern int rt_pl_dyn_abolish_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        int ok = rt_pl_dyn_abolish_cell((void *)plw_det_cell(&t0), (void *)plw_det_cell(&t1));
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if ((!strcmp(fn, "$nb_setval") || !strcmp(fn, "$nb_getval")) && nargs == 2) {
        extern int rt_pl_nb_setval_cell(void *, void *);
        extern int rt_pl_nb_getval_cell(void *, void *);
        DESCR_t t0 = args[0], t1 = args[1];
        void *c0 = (void *)plw_det_cell(&t0), *c1 = (void *)plw_det_cell(&t1);
        int ok = (fn[4] == 's') ? rt_pl_nb_setval_cell(c0, c1) : rt_pl_nb_getval_cell(c0, c1);
        if (ok) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "$unify") && nargs == 2) return dop_unify(args, nargs, out);
    if (!strcmp(fn, "$unify_lst") && nargs == 3) return dop_unify_lst(args, nargs, out);
    if (!strcmp(fn, "$ix_g") && nargs == 3) return dop_ix_g(args, nargs, out);
    if (!strcmp(fn, "$mkc") && nargs >= 1) return dop_mkc(args, nargs, out);
    if (!strcmp(fn, "$trail_mark") && nargs == 0) return dop_trail_mark(args, nargs, out);
    if (!strcmp(fn, "$trail_unwind") && nargs == 1) return dop_trail_unwind(args, nargs, out);
    if (!strcmp(fn, "$throw") && nargs == 1) { extern void rt_pl_throw_set(void *); DESCR_t t0 = args[0]; rt_pl_throw_set((void *)plw_det_cell(&t0)); *out = FAILDESCR; return 1; }
    if (!strcmp(fn, "$existence_error") && nargs == 1) { const char *k = pl_atom_str(rt_pl_deref_val(args[0])); rt_pl_iso_throw_existence_key(k ? k : "?"); *out = FAILDESCR; return 1; }
    if (!strcmp(fn, "$unwind_nothrow") && nargs == 1) return dop_unwind_nothrow(args, nargs, out);
    if (!strcmp(fn, "$catch_check") && nargs == 2) {
        extern int rt_pl_throw_pending(void); extern int rt_pl_throw_match(void *);
        if (!rt_pl_throw_pending()) { *out = FAILDESCR; return 1; }
        pl_trail_unwind(&g_pl_trail, (int)args[0].i);
        plw_zh_kill_to((int)args[0].i);
        DESCR_t t1 = args[1];
        if (rt_pl_throw_match((void *)plw_det_cell(&t1))) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *out = r; } else *out = FAILDESCR;
        return 1;
    }
    if (!strcmp(fn, "__multi_call") && nargs >= 1) {
        const char *base = VARVAL_fn(args[0]); if (!base) { *out = FAILDESCR; return 1; }
        int na = nargs - 1; DESCR_t *aa = &args[1];
        char prefix[160]; int pl = snprintf(prefix, sizeof prefix, "%s$", base);
        static int acc_idx[256]; static char acc_names[256][160]; static char acc_types[256][8][32]; static int acc_var[256]; static int acc_fixed[256]; int nacc = 0;
        extern int rt_proc_enum_count(void); extern const char *rt_proc_enum_name(int i);
        int pcount = rt_proc_enum_count();
        for (int pi = 0; pi < pcount && nacc < 256; pi++) {
            const char *pn = rt_proc_enum_name(pi);
            if (!pn || strncmp(pn, prefix, (size_t)pl)) continue;
            const char *p = pn + pl; const char *e = strchr(p, '$');
            int arity = atoi(p);
            int nt = 0; const char *q = e ? e + 1 : (const char *)0;
            while (q && nt < 8) {
                const char *nx = strchr(q, '$'); int len = nx ? (int)(nx - q) : (int)strlen(q); if (len > 31) len = 31; memcpy(acc_types[nacc][nt], q, (size_t)len); acc_types[nacc][nt][len] = 0;
                nt++; if (!nx) break; q = nx + 1;
            }
            int isvar = (nt > 0 && !strcmp(acc_types[nacc][nt - 1], "Slurpy"));
            int fixed = isvar ? arity - 1 : arity;
            if (isvar ? (na < fixed) : (arity != na)) continue;
            int ok = 1; for (int i = 0; i < fixed && i < nt; i++) if (!rt_mc_accepts(acc_types[nacc][i], aa[i])) { ok = 0; break; }
            if (!ok) continue;
            acc_var[nacc] = isvar; acc_fixed[nacc] = fixed;
            acc_idx[nacc] = pi; snprintf(acc_names[nacc], sizeof acc_names[nacc], "%s", pn); nacc++;
        }
        if (nacc == 0) {
            extern void rt_script_die_surface(const char *msg); char m[256]; snprintf(m, sizeof m, "Cannot resolve caller %s(...); no candidate matches the argument types", base);
            rt_script_die_surface(m); *out = FAILDESCR; return 1;
        }
        int win = -1;
        for (int pass = 0; pass < 2 && win < 0; pass++)
        for (int i = 0; i < nacc; i++) { int beaten = 0; if (acc_var[i] != pass) continue;
            for (int j = 0; j < nacc; j++) { if (i == j || acc_var[j] != pass) continue; if (rt_mc_narrower(acc_types[j], acc_types[i], acc_fixed[i])) { beaten = 1; break; } }
            if (!beaten) { win = i; break; } }
        if (win < 0) win = 0;
        const char *wname = acc_names[win]; (void)acc_idx;
        extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
        for (int k = 0; k < na && k < 64; k++) g_call_args[k] = aa[k];
        *out = rt_call_proc_descr(wname, na); return 1;
    }
    if (!strcmp(fn, "__blk_ref") && nargs == 1) {
        const char *bn = VARVAL_fn(args[0]); DESCR_t b; b.v = DT_BLK; b.slen = 0; b.s = (char *)(bn ? bn : "");
        *out = b; return 1;
    }
    if (!strcmp(fn, "__blk_invoke") && nargs >= 1) {
        const char *bn = (args[0].v == DT_BLK) ? args[0].s : VARVAL_fn(args[0]); if (!bn || !*bn) { *out = FAILDESCR; return 1; }
        int na = nargs - 1; extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
        for (int k = 0; k < na && k < 64; k++) g_call_args[k] = args[k + 1];
        *out = rt_call_proc_descr(bn, na); return 1;
    }
    if (!strcmp(fn, "__param_check") && nargs >= 2) {
        const char *ptype = VARVAL_fn(args[0]); if (!ptype) ptype = "Any";
        const char *pname = (nargs >= 3) ? VARVAL_fn(args[2]) : "?"; if (!pname) pname = "?";
        if (!rt_mc_accepts(ptype, args[1])) {
            extern void rt_script_die_surface(const char *msg);
            char m[256]; snprintf(m, sizeof m, "Type check failed in binding to parameter '$%s'; expected %s", pname, ptype);
            rt_script_die_surface(m); *out = FAILDESCR; return 1;
        }
        *out = args[1]; return 1;
    }
    if (!strcmp(fn, "__pas_sqr") && nargs == 1) {
        if (IS_REAL_fn(args[0])) { double d = args[0].r; DESCR_t r; r.v = DT_R; r.r = d * d; *out = r; return 1; }
        long v = IS_INT_fn(args[0]) ? args[0].i : 0;
        *out = INTVAL(v * v); return 1;
    }
    if (!strcmp(fn, "__pas_alloc") && nargs == 0) {
        g_pas_heap_ctr++; DESCR_t *c = pas_heap_cell(g_pas_heap_ctr, 1);
        if (c) *c = INTVAL(0); *out = INTVAL(g_pas_heap_ctr); return 1;
    }
    if (!strcmp(fn, "__pas_alloc_rec") && nargs == 1) {
        long nf = IS_INT_fn(args[0]) ? args[0].i : 1; if (nf < 1) nf = 1;
        size_t len = (size_t)(nf * 2 - 1); char *seg = rt_ws_alloc(len + 1); size_t p = 0;
        for (long k = 0; k < nf; k++) { if (k) seg[p++] = SOH; seg[p++] = '0'; } seg[p] = '\0';
        g_pas_heap_ctr++; DESCR_t *c = pas_heap_cell(g_pas_heap_ctr, 1);
        if (c) *c = STRVAL(seg); *out = INTVAL(g_pas_heap_ctr); return 1;
    }
    if (!strcmp(fn, "__pas_alpha_str") && nargs == 2) {
        const char *sa = VARVAL_fn(args[0]); if (!sa) sa = "";
        long lo = IS_INT_fn(args[1]) ? (long)args[1].i : 0; if (lo < 0) lo = 0;
        size_t sl = strlen(sa); char *o = rt_ws_alloc(sl + 2); size_t oi = 0; const char *pp = sa; long k = 0;
        while (*pp) { char *ep = NULL; long v = strtol(pp, &ep, 10); if (ep == pp) break; if (k >= lo) o[oi++] = (char)v; k++; pp = ep; if (*pp == SOH) pp++; else break; }
        o[oi] = '\0'; *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "__pas_ca_pack") && nargs >= 1) {
        const char *sa = VARVAL_fn(args[0]); if (!sa) sa = "";
        size_t sl = strlen(sa); char *o = rt_ws_alloc(sl + 1);
        for (size_t i = 0; i < sl; i++) o[i] = (sa[i] == SOH) ? '\x1e' : sa[i];
        o[sl] = '\0'; *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "__pas_ca_unpack") && nargs >= 1) {
        const char *sa = VARVAL_fn(args[0]); if (!sa) sa = "";
        size_t sl = strlen(sa); char *o = rt_ws_alloc(sl + 1);
        for (size_t i = 0; i < sl; i++) o[i] = (sa[i] == '\x1e') ? SOH : sa[i];
        o[sl] = '\0'; *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "__pas_field_set") && nargs == 3) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0; if (n <= 0) { *out = args[2]; return 1; }
        DESCR_t *hc = pas_heap_cell(n, 0); if (!hc) { *out = args[2]; return 1; }
        const char *cur = VARVAL_fn(*hc); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        char rb[64]; const char *rv = to_cstring(args[2], rb, sizeof rb);
        const char *s = cur; long k = 0; const char *tstart = NULL; const char *tend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == idx) { tstart = s; tend = nx; break; } if (!nx) { tstart = NULL; break; } s = nx + 1; k++; }
        if (!tstart) { *out = args[2]; return 1; }
        size_t pre = (size_t)(tstart - cur); size_t post = tend ? strlen(tend) : 0; size_t rvl = strlen(rv);
        char *o = rt_ws_alloc(pre + rvl + post + 1); memcpy(o, cur, pre); memcpy(o + pre, rv, rvl);
        if (tend) memcpy(o + pre + rvl, tend, post); o[pre + rvl + post] = '\0';
        *hc = STRVAL(o); *out = args[2]; return 1;
    }
    if (!strcmp(fn, "__pas_nrec_pfield_set") && nargs == 3) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0; if (n <= 0) { *out = args[2]; return 1; }
        DESCR_t *hc = pas_heap_cell(n, 0); if (!hc) { *out = args[2]; return 1; }
        const char *cur = VARVAL_fn(*hc); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        char rb[64]; const char *rv0 = to_cstring(args[2], rb, sizeof rb); if (!rv0) rv0 = "";
        size_t rvl = strlen(rv0); char *rv = rt_ws_alloc(rvl + 1); for (size_t j = 0; j < rvl; j++) rv[j] = (rv0[j] == SOH) ? '\x05' : rv0[j]; rv[rvl] = '\0';
        const char *s = cur; long k = 0; const char *tstart = NULL; const char *tend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == idx) { tstart = s; tend = nx; break; } if (!nx) { tstart = NULL; break; } s = nx + 1; k++; }
        if (!tstart) { *out = args[2]; return 1; }
        size_t pre = (size_t)(tstart - cur); size_t post = tend ? strlen(tend) : 0;
        char *o = rt_ws_alloc(pre + rvl + post + 1); memcpy(o, cur, pre); memcpy(o + pre, rv, rvl);
        if (tend) memcpy(o + pre + rvl, tend, post); o[pre + rvl + post] = '\0';
        *hc = STRVAL(o); *out = args[2]; return 1;
    }
    if (!strcmp(fn, "__pas_field_idx_set") && nargs == 4) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0; if (n <= 0) { *out = args[3]; return 1; }
        DESCR_t *hc = pas_heap_cell(n, 0); if (!hc) { *out = args[3]; return 1; }
        const char *cur = VARVAL_fn(*hc); if (!cur) cur = "";
        long fidx = IS_INT_fn(args[1]) ? args[1].i : 0; long eidx = IS_INT_fn(args[2]) ? args[2].i : 0; if (eidx < 1) { *out = args[3]; return 1; }
        const char *s = cur; long k = 0; const char *fstart = NULL; const char *fend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == fidx) { fstart = s; fend = nx; break; } if (!nx) { fstart = NULL; break; } s = nx + 1; k++; }
        if (!fstart) { *out = args[3]; return 1; }
        size_t flen = fend ? (size_t)(fend - fstart) : strlen(fstart);
        unsigned char ch;
        if (IS_INT_fn(args[3])) {
            long cv = args[3].i; if (cv < 0) cv = 0; if (cv > 255) cv = 255; ch = (unsigned char)cv;
        } else { const char *vs = VARVAL_fn(args[3]); ch = (unsigned char)((vs && vs[0]) ? vs[0] : ' '); }
        size_t nflen = ((size_t)eidx > flen) ? (size_t)eidx : flen; char *nfb = rt_ws_alloc(nflen + 1);
        for (size_t j = 0; j < nflen; j++) nfb[j] = (j < flen) ? fstart[j] : ' '; nfb[eidx - 1] = (char)ch; nfb[nflen] = '\0';
        size_t pre = (size_t)(fstart - cur); size_t post = fend ? strlen(fend) : 0;
        char *o = rt_ws_alloc(pre + nflen + post + 1); memcpy(o, cur, pre); memcpy(o + pre, nfb, nflen);
        if (fend) memcpy(o + pre + nflen, fend, post); o[pre + nflen + post] = '\0';
        *hc = STRVAL(o); *out = args[3]; return 1;
    }
    if (!strcmp(fn, "__pas_deref") && nargs == 1) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0;
        DESCR_t *hc = pas_heap_cell(n, 0);
        *out = hc ? *hc : INTVAL(0); return 1;
    }
    if (!strcmp(fn, "__pas_deref_set") && nargs == 2) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0;
        DESCR_t *hc = pas_heap_cell(n, 0); if (hc) *hc = args[1];
        *out = args[1]; return 1;
    }
    if (!strcmp(fn, "__pas_nrec_get") && nargs == 3) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long fi = IS_INT_fn(args[1]) ? args[1].i : 0; long ei = IS_INT_fn(args[2]) ? args[2].i : 0;
        if (fi < 0 || ei < 0) { *out = INTVAL(0); return 1; }
        const char *s = cur; long k = 0; const char *fstart = NULL; const char *fend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == fi) { fstart = s; fend = nx; break; } if (!nx) { fstart = NULL; break; } s = nx + 1; k++; }
        if (!fstart) { *out = INTVAL(0); return 1; }
        size_t flen = fend ? (size_t)(fend - fstart) : strlen(fstart);
        const char *p = fstart; const char *pend = fstart + flen; long ix = 0;
        for (;;) {
            const char *nx = (const char *)memchr(p, '\x05', (size_t)(pend - p)); const char *eend = nx ? nx : pend;
            if (ix == ei) { *out = elem_to_descr(p, (size_t)(eend - p)); return 1; } if (!nx) { *out = INTVAL(0); return 1; } p = nx + 1; ix++;
        }
    }
    if (!strcmp(fn, "__pas_nrec_update") && nargs == 4) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long fi = IS_INT_fn(args[1]) ? args[1].i : 0; long ei = IS_INT_fn(args[2]) ? args[2].i : 0;
        char rb[64]; const char *rv = to_cstring(args[3], rb, sizeof rb);
        *out = STRVAL(pas_nrec_subrec_set(cur, fi, ei, rv)); return 1;
    }
    if (!strcmp(fn, "__pas_nrec_field_set") && nargs == 3) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long fi = IS_INT_fn(args[1]) ? args[1].i : 0; if (fi < 0) { *out = args[0]; return 1; }
        char rb[64]; const char *rv = to_cstring(args[2], rb, sizeof rb); if (!rv) rv = "";
        size_t rvl = strlen(rv); char *nf = rt_ws_alloc(rvl + 1); for (size_t j = 0; j < rvl; j++) nf[j] = (rv[j] == SOH) ? '\x05' : rv[j]; nf[rvl] = '\0';
        const char *s = cur; long k = 0; const char *fstart = NULL; const char *fend = NULL;
        for (;;) { const char *nx = strchr(s, SOH); if (k == fi) { fstart = s; fend = nx; break; } if (!nx) { fstart = NULL; break; } s = nx + 1; k++; }
        if (!fstart) { *out = args[0]; return 1; }
        size_t pre = (size_t)(fstart - cur); size_t post = fend ? strlen(fend) : 0;
        char *o = rt_ws_alloc(pre + rvl + post + 1); memcpy(o, cur, pre); memcpy(o + pre, nf, rvl); if (fend) memcpy(o + pre + rvl, fend, post); o[pre + rvl + post] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "__pas_nrec_deref_set") && nargs == 4) {
        long n = IS_INT_fn(args[0]) ? args[0].i : 0; if (n <= 0) { *out = args[3]; return 1; }
        DESCR_t *hc = pas_heap_cell(n, 0); if (!hc) { *out = args[3]; return 1; }
        const char *cur = VARVAL_fn(*hc); if (!cur) cur = "";
        long fi = IS_INT_fn(args[1]) ? args[1].i : 0; long ei = IS_INT_fn(args[2]) ? args[2].i : 0;
        char rb[64]; const char *rv = to_cstring(args[3], rb, sizeof rb);
        *hc = STRVAL(pas_nrec_subrec_set(cur, fi, ei, rv)); *out = args[3]; return 1;
    }
    if (!strcmp(fn, "__pas_fassign") && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        *out = STRVAL(rt_ws_strdup_c(s)); return 1;
    }
    if (!strcmp(fn, "__pas_rewrite") && nargs == 1) {
        extern int fh_alloc(FILE *);
        const char *nm = VARVAL_fn(args[0]); if (!nm || !nm[0]) { *out = FAILDESCR; return 1; }
        FILE *fp = fopen(nm, "w"); if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp); if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        *out = FHVAL(idx); return 1;
    }
    if (!strcmp(fn, "__pas_reset") && nargs == 1) {
        extern int fh_alloc(FILE *);
        const char *nm = VARVAL_fn(args[0]); if (!nm || !nm[0]) { *out = FAILDESCR; return 1; }
        FILE *fp = fopen(nm, "r"); if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp); if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        *out = FHVAL(idx); return 1;
    }
    if (!strcmp(fn, "__pas_fclose") && nargs == 1) {
        extern FILE *fh_get(int); extern void fh_free(int);
        if (IS_FH_fn(args[0])) { int idx = (int)args[0].i; FILE *fp = fh_get(idx); if (fp && fp != stdout && fp != stderr && fp != stdin) { fflush(fp); fclose(fp); fh_free(idx); } }
        *out = NULVCL; return 1;
    }
    if (!strcmp(fn, "__pas_in") && nargs == 2) {
        long e = IS_INT_fn(args[0]) ? args[0].i : -1;
        long s = IS_INT_fn(args[1]) ? args[1].i : 0;
        *out = INTVAL((e >= 0 && e < 64 && ((s >> e) & 1L)) ? 1 : 0); return 1;
    }
    if (!strcmp(fn, "__pas_set")) {
        long s = 0;
        for (int k = 0; k < nargs; k++) { long e = IS_INT_fn(args[k]) ? args[k].i : -1; if (e >= 0 && e < 64) s |= (1L << e); }
        *out = INTVAL(s); return 1;
    }
    if (nargs == 2 && (!strcmp(fn, "__pas_setuni") || !strcmp(fn, "__pas_setint") || !strcmp(fn, "__pas_setdif")
                    || !strcmp(fn, "__pas_subset") || !strcmp(fn, "__pas_super"))) {
        long a = IS_INT_fn(args[0]) ? args[0].i : 0;
        long b = IS_INT_fn(args[1]) ? args[1].i : 0;
        long r;
        if      (!strcmp(fn, "__pas_setuni")) r = a | b;
        else if (!strcmp(fn, "__pas_setint")) r = a & b;
        else if (!strcmp(fn, "__pas_setdif")) r = a & ~b;
        else if (!strcmp(fn, "__pas_subset")) r = ((a & ~b) == 0) ? 1 : 0;
        else                                  r = ((b & ~a) == 0) ? 1 : 0;
        *out = INTVAL(r); return 1;
    }
    if (nargs >= 1 && (!strcmp(fn, "__rk_jct_any") || !strcmp(fn, "__rk_jct_all")
                    || !strcmp(fn, "__rk_jct_one") || !strcmp(fn, "__rk_jct_none"))) {
        char flav = !strcmp(fn, "__rk_jct_any") ? 'a'
                  : !strcmp(fn, "__rk_jct_all") ? 'l'
                  : !strcmp(fn, "__rk_jct_one") ? 'o' : 'n';
        size_t total = 2;
        char **ms = rt_ws_alloc((size_t)nargs * sizeof(char *));
        size_t *mlen = rt_ws_alloc((size_t)nargs * sizeof(size_t));
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            size_t L = strlen(cs);
            char *cp = rt_ws_alloc(L + 1); memcpy(cp, cs, L + 1);
            ms[i] = cp; mlen[i] = L; total += 1 + L;
        }
        char *buf = rt_ws_alloc(total + 2);
        size_t p = 0; buf[p++] = '\x03'; buf[p++] = flav;
        for (int i = 0; i < nargs; i++) { buf[p++] = SOH; memcpy(buf + p, ms[i], mlen[i]); p += mlen[i]; }
        buf[p++] = '\x04'; buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "elems") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = INTVAL(0); return 1; }
        int cnt = 1; for (const char *p = cur; *p; p++) if (*p == SOH) cnt++;
        *out = INTVAL(cnt); return 1;
    }
    if ((!strcmp(fn, "__rk_arr") || !strcmp(fn, "__rk_arr_lit")) && nargs >= 0) {
        extern DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs);
        *out = rt_make_flat_agg(args, nargs); return 1;
    }
    if (!strcmp(fn, "__rk_materialize") && nargs == 1) {
        /* row raku-eager-materialization-family (seat06 2026-08-30): a generator's yield count is
           unknown until it finishes, so a drive-loop collects yields into a list()/put() DT_DATA
           accumulator; but every Raku array consumer (.elems above, .sum, iteration) reads the
           OTHER, SOH-joined-flat-STRVAL shape rt_make_flat_agg/__rk_arr produce -- the two are not
           interchangeable (checked, not assumed: elems() here counts SOH bytes in a string, it does
           not branch on DT_DATA at all). This is the one-time conversion between them, called once
           when the drive loop reaches its ω (generator exhausted). */
        DESCR_t *a = 0; int n = 0;
        if (!rt_lv_is_list(args[0], &a, &n)) return 0;
        extern DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs);
        *out = rt_make_flat_agg(a, n); return 1;
    }
    if (!strcmp(fn, "__rk_pair") && nargs == 2) {
        /* general expr OP_FATARROW expr: no Pair value type exists in this runtime, so this builds
           the minimal thing the one real caller (.trans, below) needs -- key and value concatenated
           via rt_make_nested_agg (SOH-joined, no flattening of either side's own internal SOH
           structure), which .trans splits back in half by COUNT. Correct specifically for the
           "one Pair, list-key, list-value, zipped element-wise" shape real Raku also uses for
           .trans(@from => @to) -- not a general Pair (no .key/.value accessors, not usable as a
           hash key), per this row's own "minimal version this needs" discipline. */
        *out = rt_make_nested_agg(args, 2); return 1;
    }
    if (!strcmp(fn, "__rk_named_call") && nargs >= 2) {
        extern const char *rt_proc_pname(const char *name, int k);
        char pb[256]; const char *pname = to_cstring(args[0], pb, sizeof pb);
        int npos = IS_INT_fn(args[1]) ? (int)args[1].i : 0;
        if (npos < 0) npos = 0;
        if (npos > RK_NAMED_MAX) npos = RK_NAMED_MAX;
        DESCR_t slots[RK_NAMED_MAX];
        for (int i = 0; i < RK_NAMED_MAX; i++) slots[i] = NULVCL;
        int np = 0; while (np < RK_NAMED_MAX && rt_proc_pname(pname, np)) np++;
        int maxslot = 0;
        extern int rt_proc_named_rest(const char *name);
        int nrest = rt_proc_named_rest(pname); int nrx = nrest > 0 ? nrest - 1 : -1;
        size_t hcap = 1; for (int i = 2 + npos; i + 1 < nargs; i += 2) { char kb0[128], vb0[256]; const char *k0 = to_cstring(args[i], kb0, sizeof kb0); const char *v0 = to_cstring(args[i + 1], vb0, sizeof vb0); hcap += (k0 ? strlen(k0) : 0) + (v0 ? strlen(v0) : 0) + 2; }
        char *hbuf = rt_ws_alloc(hcap); size_t hp = 0; hbuf[0] = '\0';
        for (int i = 0; i < npos && (2 + i) < nargs; i++) { slots[i] = args[2 + i]; if (i + 1 > maxslot) maxslot = i + 1; }
        for (int i = 2 + npos; i + 1 < nargs; i += 2) {
            char kb[128]; const char *k = to_cstring(args[i], kb, sizeof kb);
            int found = -1;
            for (int s = 0; s < np; s++) { if (s == nrx) continue; const char *pn = rt_proc_pname(pname, s); if (pn && k && !strcmp(pn, k)) { found = s; break; } }
            if (found < 0) {
                if (nrx < 0) continue;
                char vb2[256]; const char *v = to_cstring(args[i + 1], vb2, sizeof vb2); if (!k) k = ""; if (!v) v = "";
                if (hp > 0) hbuf[hp++] = SOH;
                size_t kl = strlen(k); memcpy(hbuf + hp, k, kl); hp += kl; hbuf[hp++] = '\x02';
                size_t vl = strlen(v); memcpy(hbuf + hp, v, vl); hp += vl; hbuf[hp] = '\0';
                continue;
            }
            slots[found] = args[i + 1]; if (found + 1 > maxslot) maxslot = found + 1;
        }
        if (nrx >= 0 && nrx < RK_NAMED_MAX) { slots[nrx] = STRVAL(hbuf); if (nrx + 1 > maxslot) maxslot = nrx + 1; }
        int total = np > maxslot ? np : maxslot;
        if (total > RK_NAMED_MAX) total = RK_NAMED_MAX;
        *out = invoke_method_proc(pname, slots, total); return 1;
    }
    if (!strcmp(fn, "__rk_rep") && nargs == 2) {
        char sb[256]; const char *src = to_cstring(args[0], sb, sizeof sb);
        long long n = IS_INT_fn(args[1]) ? (long long)args[1].i : (IS_REAL_fn(args[1]) ? (long long)args[1].r : 0);
        if (!src) src = "";
        if (n < 0) n = 0;
        size_t L = strlen(src);
        char *buf = rt_ws_alloc(L * (size_t)(n > 0 ? n : 0) + 1); size_t p = 0;
        for (long long k = 0; k < n; k++) { memcpy(buf + p, src, L); p += L; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "__rk_range_arr") && nargs == 2) {
        long long lo = IS_INT_fn(args[0]) ? (long long)args[0].i : (IS_REAL_fn(args[0]) ? (long long)args[0].r : 0);
        long long hi = IS_INT_fn(args[1]) ? (long long)args[1].i : (IS_REAL_fn(args[1]) ? (long long)args[1].r : 0);
        if (hi < lo) { char *e = rt_ws_alloc(1); e[0] = '\0'; *out = STRVAL(e); return 1; }
        long long cnt = hi - lo + 1;
        char *buf = rt_ws_alloc((size_t)cnt * 24 + 1); size_t p = 0;
        for (long long v = lo; v <= hi; v++) { if (p > 0) buf[p++] = SOH; char eb[24]; int el = snprintf(eb, sizeof eb, "%lld", v); memcpy(buf + p, eb, (size_t)el); p += (size_t)el; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if ((!strcmp(fn, "rk_write") || !strcmp(fn, "rk_writes"))) {
        DESCR_t *tmp = (DESCR_t *)rt_ws_alloc((size_t)(nargs > 0 ? nargs : 1) * sizeof(DESCR_t));
        for (int _ri = 0; _ri < nargs; _ri++) {
            if (IS_REAL_fn(args[_ri])) { char *_rb = rt_ws_alloc(64); rk_real_str(args[_ri].r, _rb, 64); tmp[_ri] = STRVAL(_rb); }
            else tmp[_ri] = args[_ri];
        }
        *out = rt_call_arr(!strcmp(fn, "rk_write") ? "write" : "writes", tmp, nargs); return 1;
    }
    if (!strcmp(fn, "rk_write_arr") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        size_t n = strlen(cur); char *buf = rt_ws_alloc(n + 3); size_t p = 0;
        buf[p++] = '[';
        for (size_t i = 0; i < n; i++) buf[p++] = (cur[i] == SOH) ? ' ' : cur[i];
        buf[p++] = ']'; buf[p] = '\0';
        DESCR_t tmp1 = STRVAL(buf);
        *out = rt_call_arr("write", &tmp1, 1); return 1;
    }
    if (!strcmp(fn, "__rk_div") && nargs == 2) {
        extern void rt_script_die_surface(const char *msg);
        DESCR_t a = args[0], b = args[1];
        int ai = IS_INT_fn(a), arl = IS_REAL_fn(a), bi = IS_INT_fn(b), brl = IS_REAL_fn(b);
        double ad = arl ? a.r : (ai ? (double)a.i : 0.0), bd = brl ? b.r : (bi ? (double)b.i : 0.0);
        if (!ai && !arl) { char sa[64]; const char *cs = to_cstring(a, sa, sizeof sa); ad = cs ? strtod(cs, (char **)0) : 0.0; }
        if (!bi && !brl) { char sb[64]; const char *cs = to_cstring(b, sb, sizeof sb); bd = cs ? strtod(cs, (char **)0) : 0.0; }
        if (ai && bi) {
            if (b.i == 0) { rt_script_die_surface("Attempt to divide by zero"); *out = FAILDESCR; return 1; }
            if (b.i == -1) { *out = INTVAL(-a.i); return 1; }
            if ((a.i % b.i) == 0) { *out = INTVAL(a.i / b.i); return 1; }
            *out = REALVAL((double)a.i / (double)b.i); return 1;
        }
        if (bd == 0.0) { rt_script_die_surface("Attempt to divide by zero"); *out = FAILDESCR; return 1; }
        *out = REALVAL(ad / bd); return 1;
    }
    if (!strcmp(fn, "__rk_arr_slice") && nargs == 3) {
        char scratch[64]; const char *cs = to_cstring(args[0], scratch, sizeof scratch); if (!cs) cs = "";
        long long lo = IS_INT_fn(args[1]) ? (long long)args[1].i : (IS_REAL_fn(args[1]) ? (long long)args[1].r : 0);
        long long hi = IS_INT_fn(args[2]) ? (long long)args[2].i : (IS_REAL_fn(args[2]) ? (long long)args[2].r : 0);
        if (lo < 0) lo = 0;
        if (hi < lo) { char *e = rt_ws_alloc(1); e[0] = '\0'; *out = STRVAL(e); return 1; }
        char *buf = rt_ws_alloc(strlen(cs) + 1); size_t p = 0; const char *seg = cs; long long k = 0; int wrote = 0;
        for (;;) {
            const char *nx = strchr(seg, SOH); size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            if (k >= lo && k <= hi) { if (wrote) buf[p++] = SOH; memcpy(buf + p, seg, L); p += L; wrote = 1; }
            if (!nx || k >= hi) break;
            seg = nx + 1; k++;
        }
        buf[p] = '\0'; *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "__rk_arr_pick") && nargs >= 2) {
        char scratch[64]; const char *cs = to_cstring(args[0], scratch, sizeof scratch); if (!cs) cs = "";
        size_t tot = strlen(cs); int nsel = nargs - 1;
        const char **els = rt_ws_alloc((tot + 2) * sizeof(const char *)); size_t *lens = rt_ws_alloc((tot + 2) * sizeof(size_t));
        int nel = 0; const char *seg = cs;
        for (;;) { const char *nx = strchr(seg, SOH); els[nel] = seg; lens[nel] = nx ? (size_t)(nx - seg) : strlen(seg); nel++; if (!nx) break; seg = nx + 1; }
        char *buf = rt_ws_alloc((tot + 2) * (size_t)nsel + 2); size_t p = 0;
        for (int i = 1; i < nargs; i++) {
            long long k = IS_INT_fn(args[i]) ? (long long)args[i].i : (IS_REAL_fn(args[i]) ? (long long)args[i].r : 0);
            if (i > 1) buf[p++] = SOH;
            if (k >= 0 && k < nel) { memcpy(buf + p, els[k], lens[k]); p += lens[k]; }
        }
        buf[p] = '\0'; *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "__rk_arr_xx") && nargs == 2) {
        char scratch[64];
        const char *cs = to_cstring(args[0], scratch, sizeof scratch);
        long cnt = IS_INT_fn(args[1]) ? args[1].i : (IS_REAL_fn(args[1]) ? (long)args[1].r : 0);
        if (cnt < 1) { char *e = rt_ws_alloc(1); e[0] = '\0'; *out = STRVAL(e); return 1; }
        const char **els = rt_ws_alloc((size_t)64 * sizeof(const char *));
        size_t *lens = rt_ws_alloc((size_t)64 * sizeof(size_t));
        int nel = 0, cap = 64;
        const char *seg = cs;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            if (nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
            if (!nx) break;
            seg = nx + 1;
        }
        size_t one = 0; for (int i = 0; i < nel; i++) one += lens[i] + 1;
        size_t total = one * (size_t)cnt; char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (long r = 0; r < cnt; r++) for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "__rk_hash")) {
        size_t total = 1; char kb[256]; char vb[256];
        for (int i = 0; i + 1 < nargs; i += 2) { total += strlen(to_cstring(args[i], kb, sizeof kb)) + strlen(to_cstring(args[i + 1], vb, sizeof vb)) + 2; }
        char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (int i = 0; i + 1 < nargs; i += 2) { const char *k = to_cstring(args[i], kb, sizeof kb); const char *v = to_cstring(args[i + 1], vb, sizeof vb);
            if (p > 0) buf[p++] = '\x01'; size_t kl = strlen(k); memcpy(buf + p, k, kl); p += kl; buf[p++] = '\x02'; size_t vl = strlen(v); memcpy(buf + p, v, vl); p += vl; }
        buf[p] = '\0'; *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "reverse") && nargs >= 1) {
        const char **els = rt_ws_alloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = rt_ws_alloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (int i = nel - 1; i >= 0; i--) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if ((!strcmp(fn, "head") || !strcmp(fn, "tail")) && nargs >= 1) {
        int is_tail = (fn[0] == 't');
        long long n = 1; int listargs = nargs;
        if (nargs >= 2) {
            char nb[64]; const char *ns = to_cstring(args[nargs - 1], nb, sizeof nb);
            char *ep; long long v = strtoll(ns ? ns : "", &ep, 10);
            if (ns && ep > ns) n = v;
            listargs = nargs - 1;
        }
        const char **els = rt_ws_alloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = rt_ws_alloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < listargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        if (n < 0) n = 0;
        int lo, hi;
        if (is_tail) { lo = (int)(nel - n); if (lo < 0) lo = 0; hi = nel; }
        else         { lo = 0; hi = (int)(n < nel ? n : nel); }
        size_t total = 0; for (int i = lo; i < hi; i++) total += lens[i] + 1;
        char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (int i = lo; i < hi; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "unique") && nargs >= 1) {
        const char **els = rt_ws_alloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = rt_ws_alloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                int dup = 0;
                for (int j = 0; j < nel; j++) if (lens[j] == L && memcmp(els[j], seg, L) == 0) { dup = 1; break; }
                if (!dup && nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "__rk_arr_sort") && nargs >= 1) {
        const char **els = rt_ws_alloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = rt_ws_alloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = rt_ws_alloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        for (int a = 1; a < nel; a++) {
            const char *keys = els[a]; size_t keyl = lens[a]; int b = a - 1;
            while (b >= 0) {
                const char *xs = els[b]; const char *ys = keys; char *xe; char *ye;
                long long xi = strtoll(xs, &xe, 10); long long yi = strtoll(ys, &ye, 10);
                int both_num = (*xe == '\0' && xe != xs && *ye == '\0' && ye != ys);
                int cmp = both_num ? (xi > yi ? 1 : xi < yi ? -1 : 0) : strcmp(xs, ys);
                if (cmp <= 0) break;
                els[b + 1] = els[b]; lens[b + 1] = lens[b]; b--;
            }
            els[b + 1] = keys; lens[b + 1] = keyl;
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = rt_ws_alloc(total + 1); size_t p = 0;
        for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if ((!strcmp(fn, "__rk_arr_min") || !strcmp(fn, "__rk_arr_max")) && nargs >= 1) {
        int want_max = (fn[10] == 'a');
        const char *best = NULL; size_t bestl = 0; long long bestn = 0; int best_num = 0; int have = 0;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                char eb[64]; size_t cl = L < 63 ? L : 63; memcpy(eb, seg, cl); eb[cl] = '\0';
                char *ep; long long v = strtoll(eb, &ep, 10); int isn = (*ep == '\0' && ep != eb);
                int take;
                if (!have) take = 1;
                else if (isn && best_num) take = want_max ? (v > bestn) : (v < bestn);
                else { int c = strcmp(eb, best ? best : ""); take = want_max ? (c > 0) : (c < 0); }
                if (take) { char *cp = rt_ws_alloc(cl + 1); memcpy(cp, eb, cl); cp[cl] = '\0'; best = cp; bestl = cl; bestn = v; best_num = isn; have = 1; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        if (!have) { *out = NULVCL; return 1; }
        *out = best_num ? INTVAL(bestn) : STRVAL(rt_ws_strdup_c(best)); return 1;
    }
    if (!strcmp(fn, "__rk_arr_first") && nargs >= 1) {
        char scratch[64];
        const char *cs = to_cstring(args[0], scratch, sizeof scratch);
        const char *nx = strchr(cs, SOH);
        size_t L = nx ? (size_t)(nx - cs) : strlen(cs);
        if (L == 0 && !nx && (!cs || !*cs)) { *out = NULVCL; return 1; }
        char *cp = rt_ws_alloc(L + 1); memcpy(cp, cs, L); cp[L] = '\0';
        char *ep; long long v = strtoll(cp, &ep, 10);
        *out = (*ep == '\0' && ep != cp) ? INTVAL(v) : STRVAL(cp); return 1;
    }
    if (!strcmp(fn, "__rk_arr_values") && nargs >= 1) {
        char scratch[64];
        const char *cs = to_cstring(args[0], scratch, sizeof scratch);
        *out = STRVAL(rt_ws_strdup_c(cs ? cs : "")); return 1;
    }
    if (!strcmp(fn, "__rk_arr_keys") && nargs >= 1) {
        char scratch[64];
        const char *cs = to_cstring(args[0], scratch, sizeof scratch);
        if (!cs || !*cs) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int nel = 1; for (const char *p = cs; *p; p++) if (*p == SOH) nel++;
        char *buf = rt_ws_alloc((size_t)nel * 24 + 1); int p = 0;
        for (int i = 0; i < nel; i++) { if (i) buf[p++] = SOH; char nb[24]; int L = snprintf(nb, sizeof nb, "%d", i); memcpy(buf + p, nb, (size_t)L); p += L; }
        buf[p] = '\0'; *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "sum") && nargs >= 1) {
        long long isum = 0; double rsum = 0.0; int any_real = 0;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                char eb[64]; size_t cl = L < 63 ? L : 63; memcpy(eb, seg, cl); eb[cl] = '\0';
                char *ep; long long iv = strtoll(eb, &ep, 10);
                if (*ep == '\0' && ep > eb) { isum += iv; rsum += (double)iv; }
                else { double dv = strtod(eb, NULL); any_real = 1; rsum += dv; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        *out = any_real ? REALVAL(rsum) : INTVAL(isum); return 1;
    }
    if ((!strcmp(fn, "__rk_reduce_add") || !strcmp(fn, "__rk_reduce_sub") || !strcmp(fn, "__rk_reduce_mul")) && nargs >= 1) {
        int is_add = !strcmp(fn + 12, "add"); int is_mul = !strcmp(fn + 12, "mul");
        char scratch[64]; const char *cs = to_cstring(args[0], scratch, sizeof scratch); if (!cs) cs = "";
        if (*cs == '\0') { *out = INTVAL(is_mul ? 1 : 0); return 1; }
        long long iacc = 0; double racc = 0.0; int any_real = 0; int first = 1;
        const char *seg = cs;
        for (;;) {
            const char *nx = strchr(seg, SOH); size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            char eb[64]; size_t cl = L < 63 ? L : 63; memcpy(eb, seg, cl); eb[cl] = '\0';
            char *ep; long long iv = strtoll(eb, &ep, 10); int isn = (*ep == '\0' && ep != eb);
            double dv = isn ? (double)iv : strtod(eb, NULL); if (!isn) any_real = 1;
            if (first) { iacc = iv; racc = dv; first = 0; }
            else if (is_add) { iacc += iv; racc += dv; }
            else if (is_mul) { iacc *= iv; racc *= dv; }
            else { iacc -= iv; racc -= dv; }
            if (!nx) break; seg = nx + 1;
        }
        *out = any_real ? REALVAL(racc) : INTVAL(iacc); return 1;
    }
    if (!strcmp(fn, "__rk_reduce_cat") && nargs >= 1) {
        char scratch[64]; const char *cs = to_cstring(args[0], scratch, sizeof scratch); if (!cs) cs = "";
        if (*cs == '\0') { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *buf = rt_ws_strdup(""); size_t blen = 0; const char *seg = cs;
        for (;;) {
            const char *nx = strchr(seg, SOH); size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            char *no = rt_ws_alloc(blen + L + 1); memcpy(no, buf, blen); memcpy(no + blen, seg, L); no[blen + L] = '\0';
            buf = no; blen += L;
            if (!nx) break; seg = nx + 1;
        }
        *out = STRVAL(buf); return 1;
    }
    if ((!strcmp(fn, "__rk_reduce_min") || !strcmp(fn, "__rk_reduce_max")) && nargs >= 1) {
        int want_max = !strcmp(fn + 12, "max");
        char scratch[64]; const char *cs = to_cstring(args[0], scratch, sizeof scratch); if (!cs) cs = "";
        if (*cs == '\0') { *out = NULVCL; return 1; }
        const char *best = NULL; long long bestn = 0; int best_num = 0; int have = 0; const char *seg = cs;
        for (;;) {
            const char *nx = strchr(seg, SOH); size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
            char eb[64]; size_t cl = L < 63 ? L : 63; memcpy(eb, seg, cl); eb[cl] = '\0';
            char *ep; long long v = strtoll(eb, &ep, 10); int isn = (*ep == '\0' && ep != eb);
            int take; if (!have) take = 1; else if (isn && best_num) take = want_max ? (v > bestn) : (v < bestn);
            else { int c = strcmp(eb, best ? best : ""); take = want_max ? (c > 0) : (c < 0); }
            if (take) { char *cp = rt_ws_alloc(cl + 1); memcpy(cp, eb, cl); cp[cl] = '\0'; best = cp; bestn = v; best_num = isn; have = 1; }
            if (!nx) break; seg = nx + 1;
        }
        *out = best_num ? INTVAL(bestn) : STRVAL(rt_ws_strdup_c(best ? best : "")); return 1;
    }
    if (!strcmp(fn, "join") && nargs >= 1) {
        char sb[64]; const char *sep = to_cstring(args[0], sb, sizeof sb);
        char *sepd = rt_ws_strdup(sep ? sep : ""); size_t seplen = strlen(sepd);
        char *buf = rt_ws_strdup(""); size_t blen = 0; int first = 1;
        for (int i = 1; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                size_t add = (first ? 0 : seplen) + L;
                char *no = rt_ws_alloc(blen + add + 1);
                memcpy(no, buf, blen); size_t p = blen;
                if (!first) { memcpy(no + p, sepd, seplen); p += seplen; }
                memcpy(no + p, seg, L); p += L; no[p] = '\0';
                buf = no; blen = p; first = 0;
                if (!nx) break;
                seg = nx + 1;
            }
        }
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "arr_make") && nargs == 1) {
        long long hi = IS_INT_fn(args[0]) ? args[0].i : 0; long long n = hi + 1; if (n < 1) n = 1;
        ARBLK_t *b = (ARBLK_t *) rt_ws_alloc_tag(sizeof(ARBLK_t), HB_ARR); b->id = rt_agg_serial_list(); b->lo = 0; b->hi = (int) hi;
        b->ndim = 1;
        b->ndim = 1;
        b->lo2 = 0; b->hi2 = 0; b->proto_bare = 0; b->data = (DESCR_t *) rt_ws_alloc(sizeof(DESCR_t) * (size_t) n); for (long long k = 0; k < n; k++) b->data[k] = INTVAL(0); DESCR_t d; d.v = DT_A;
        d.slen = 0; d.arr = b; *out = d; return 1;
    }
    if (!strcmp(fn, "arr_get") && nargs == 2) {
        if (args[0].v == DT_A && args[0].arr) {
            ARBLK_t *b = (ARBLK_t *) args[0].arr; long i = IS_INT_fn(args[1]) ? args[1].i : 0; if (i < b->lo || i > b->hi) { *out = FAILDESCR; return 1; } *out = b->data[i - b->lo]; return 1;
        }
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        if (idx < 0 || !*cur) { *out = FAILDESCR; return 1; }
        if (idx >= 1 && !strchr(cur, SOH) && (size_t)idx <= strlen(cur)) { *out = INTVAL((long long)(unsigned char)cur[idx - 1]); return 1; }
        const char *seg = cur;
        long k = 0;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            if (k == idx) {
                size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
                *out = elem_to_descr(seg, elen);
                return 1;
            }
            if (!nx) { *out = FAILDESCR; return 1; }
            seg = nx + 1; k++;
        }
    }
    if (!strcmp(fn, "str_substr") || (!strcmp(fn, "substr") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        long slen = (long)strlen(s);
        long start = IS_INT_fn(args[1]) ? args[1].i : 0;
        if (start < 0) start = slen + start;
        if (start < 0) start = 0;
        if (start > slen) start = slen;
        long len = slen - start;
        if (nargs >= 3) {
            long ll = IS_INT_fn(args[2]) ? args[2].i : len;
            if (ll < 0) ll = 0;
            if (start + ll > slen) ll = slen - start;
            len = ll;
        }
        char *o = rt_ws_alloc((size_t)len + 1);
        memcpy(o, s + start, (size_t)len); o[len] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "str_index") || (!strcmp(fn, "index") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        const char *n = VARVAL_fn(args[1]); if (!n) n = "";
        long from = (nargs >= 3 && IS_INT_fn(args[2])) ? args[2].i : 0;
        if (from < 0) from = 0;
        if (*n == '\0') { *out = INTVAL(from); return 1; }
        const char *p = strstr(s + from, n);
        *out = p ? INTVAL((long)(p - s)) : INTVAL(-1); return 1;
    }
    if (!strcmp(fn, "str_rindex") || (!strcmp(fn, "rindex") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        const char *n = VARVAL_fn(args[1]); if (!n) n = "";
        long slen = (long)strlen(s);
        long from = (nargs >= 3 && IS_INT_fn(args[2])) ? args[2].i : slen;
        size_t nlen = strlen(n);
        if (nlen == 0) { *out = INTVAL(from < slen ? from : slen); return 1; }
        long best = -1;
        for (long i = 0; i <= slen - (long)nlen && i <= from; i++)
            if (memcmp(s + i, n, nlen) == 0) best = i;
        *out = INTVAL(best); return 1;
    }
    if ((!strcmp(fn, "uc") || !strcmp(fn, "str_uc")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        size_t n = strlen(s); char *o = rt_ws_alloc(n + 1);
        for (size_t i = 0; i < n; i++) o[i] = (char)((s[i] >= 'a' && s[i] <= 'z') ? s[i] - 32 : s[i]);
        o[n] = '\0'; *out = STRVAL(o); return 1;
    }
    if ((!strcmp(fn, "lc") || !strcmp(fn, "str_lc")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        size_t n = strlen(s); char *o = rt_ws_alloc(n + 1);
        for (size_t i = 0; i < n; i++) o[i] = (char)((s[i] >= 'A' && s[i] <= 'Z') ? s[i] + 32 : s[i]);
        o[n] = '\0'; *out = STRVAL(o); return 1;
    }
    if ((!strcmp(fn, "chars") || !strcmp(fn, "length")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        *out = INTVAL((long)strlen(s)); return 1;
    }
    if ((!strcmp(fn, "str_trim") || !strcmp(fn, "trim")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
        size_t n = strlen(s);
        while (n > 0 && (s[n-1] == ' ' || s[n-1] == '\t' || s[n-1] == '\n' || s[n-1] == '\r')) n--;
        char *o = rt_ws_alloc(n + 1); memcpy(o, s, n); o[n] = '\0';
        *out = STRVAL(o); return 1;
    }
    if ((!strcmp(fn, "sprintf") || !strcmp(fn, "__rk_sprintf")) && nargs >= 1) {
        char fb[512]; const char *fmt = to_cstring(args[0], fb, sizeof fb); if (!fmt) fmt = "";
        char *r = NULL; size_t rl = 0; rk_sprintf_core(fmt, args, nargs, 1, &r, &rl);
        *out = BSTRVAL(r, rl); return 1;
    }
    if ((!strcmp(fn, "printf") || !strcmp(fn, "__rk_printf")) && nargs >= 1) {
        char fb[512]; const char *fmt = to_cstring(args[0], fb, sizeof fb); if (!fmt) fmt = "";
        char *r = NULL; size_t rl = 0; rk_sprintf_core(fmt, args, nargs, 1, &r, &rl);
        fwrite(r, 1, rl, stdout); *out = INTVAL(1); return 1;
    }
    if (!strcmp(fn, "array_sort") && nargs == 1) {
        const char *as = VARVAL_fn(args[0]); if (!as || !*as) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int cnt = 1; for (const char *p = as; *p; p++) if (*p == SOH) cnt++;
        char **elems = rt_ws_alloc((size_t)cnt * sizeof(char*));
        int idx = 0; const char *seg = as;
        do {
            const char *nx = strchr(seg, SOH);
            size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
            char *el = rt_ws_alloc(elen + 1); memcpy(el, seg, elen); el[elen] = '\0';
            elems[idx++] = el;
            seg = nx ? nx + 1 : NULL;
        } while (seg && idx < cnt);
        int all_int = 1;
        for (int i = 0; i < cnt && all_int; i++) {
            char *ep; strtol(elems[i], &ep, 10);
            if (*ep) all_int = 0;
        }
        if (all_int) {
            for (int i = 1; i < cnt; i++) {
                char *key = elems[i]; long kv = atol(key); int j = i - 1;
                while (j >= 0 && atol(elems[j]) > kv) { elems[j+1] = elems[j]; j--; }
                elems[j+1] = key;
            }
        } else {
            for (int i = 1; i < cnt; i++) {
                char *key = elems[i]; int j = i - 1;
                while (j >= 0 && strcmp(elems[j], key) > 0) { elems[j+1] = elems[j]; j--; }
                elems[j+1] = key;
            }
        }
        size_t total = 0; for (int i = 0; i < cnt; i++) total += strlen(elems[i]) + 1;
        char *o = rt_ws_alloc(total + 1); o[0] = '\0';
        for (int i = 0; i < cnt; i++) {
            if (i) { size_t ol = strlen(o); o[ol] = SOH; o[ol+1] = '\0'; }
            strcat(o, elems[i]);
        }
        *out = STRVAL(o); return 1;
    }
    if ((!strcmp(fn, "die") || !strcmp(fn, "script_die")) && nargs >= 1) {
        const char *m = VARVAL_fn(args[0]); if (!m) m = "Died";
        extern void rt_script_die_surface(const char *msg);
        rt_script_die_surface(m);
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "srand") && nargs == 1) {
        long seed = IS_INT_fn(args[0]) ? (long)args[0].i : (IS_REAL_fn(args[0]) ? (long)args[0].r : 0);
        srand((unsigned int)seed);
        *out = INTVAL(seed); return 1;
    }
    if (!strcmp(fn, "callsame") || !strcmp(fn, "nextsame") || !strcmp(fn, "callwith")) {
        if (g_redisp_top <= 0) { *out = FAILDESCR; return 1; }
        RedispFrame *f = &g_redisp[g_redisp_top - 1];
        int next = -1;
        for (int i = f->found_idx + 1; i < f->mro_len; i++) {
            char cand[256]; snprintf(cand, sizeof cand, "%s__%s", f->mro[i], f->mname);
            if (meth_is_user_proc(cand)) { next = i; break; }
        }
        if (next < 0) { *out = FAILDESCR; return 1; }
        char nproc[256]; snprintf(nproc, sizeof nproc, "%s__%s", f->mro[next], f->mname);
        DESCR_t ca[16]; int total;
        if (!strcmp(fn, "callwith")) { total = 1 + nargs; if (total > 16) total = 16; ca[0] = f->self; for (int k = 0; k + 1 < total; k++) ca[1 + k] = args[k]; }
        else { total = f->nargs; if (total > 16) total = 16; for (int k = 0; k < total; k++) ca[k] = f->args[k]; }
        const char *smro[64]; int smro_len = f->mro_len < 64 ? f->mro_len : 64; for (int k = 0; k < smro_len; k++) smro[k] = f->mro[k];
        char smname[128]; snprintf(smname, sizeof smname, "%s", f->mname); DESCR_t sself = f->self;
        int rd = -1;
        if (g_redisp_top < 64) {
            rd = g_redisp_top++;
            g_redisp[rd].self = sself; snprintf(g_redisp[rd].mname, sizeof g_redisp[rd].mname, "%s", smname);
            for (int k = 0; k < smro_len; k++) g_redisp[rd].mro[k] = smro[k];
            g_redisp[rd].mro_len = smro_len; g_redisp[rd].found_idx = next;
            g_redisp[rd].nargs = total; for (int k = 0; k < total; k++) g_redisp[rd].args[k] = ca[k];
        }
        DESCR_t r = invoke_method_proc(nproc, ca, total);
        if (rd >= 0) g_redisp_top--;
        *out = r; return 1;
    }
    if (!strcmp(fn, "exc_clear") && nargs == 0) {
        extern char g_script_exception[512];
        g_script_exception[0] = '\0';
        *out = STRVAL(rt_ws_strdup_c("")); return 1;
    }
    if (!strcmp(fn, "try_enter") && nargs == 0) {
        extern char g_script_exception[512]; extern int g_script_try_depth;
        g_script_try_depth++; g_script_exception[0] = '\0';
        *out = STRVAL(rt_ws_strdup_c("")); return 1;
    }
    if (!strcmp(fn, "try_exit") && nargs == 0) {
        extern int g_script_try_depth;
        if (g_script_try_depth > 0) g_script_try_depth--;
        *out = STRVAL(rt_ws_strdup_c("")); return 1;
    }
    if (!strcmp(fn, "exc_check") && nargs == 0) {
        extern char g_script_exception[512];
        if (g_script_exception[0] != '\0') { *out = INTVAL(1); return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "exc_get") && nargs == 0) {
        extern char g_script_exception[512];
        *out = STRVAL(rt_ws_strdup_c(g_script_exception)); return 1;
    }
    if (!strcmp(fn, "fh_capture") && nargs == 1) {
        extern void fh_ensure_init(void);
        fh_ensure_init();
        int idx = (int)(IS_INT_fn(args[0]) ? args[0].i : 0);
        *out = FHVAL(idx); return 1;
    }
    if (!strcmp(fn, "open") && (nargs == 1 || nargs == 2)) {
        const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = FAILDESCR; return 1; }
        const char *mode = "r";
        if (nargs == 2) {
            const char *ms = VARVAL_fn(args[1]); if (!ms) ms = "";
            if (strstr(ms, ":w") || strstr(ms, "w")) mode = "w";
            else if (strstr(ms, ":a") || strstr(ms, "a")) mode = "a";
        }
        extern void fh_ensure_init(void);
        extern int fh_alloc(FILE *);
        fh_ensure_init();
        FILE *fp = fopen(path, mode);
        if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp);
        if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        *out = INTVAL(idx); return 1;
    }
    if (!strcmp(fn, "close") && nargs == 1) {
        extern FILE *fh_get(int);
        extern void  fh_free(int);
        int idx = (int)(IS_INT_fn(args[0]) ? args[0].i : 0);
        FILE *fp = fh_get(idx);
        if (fp) { fclose(fp); fh_free(idx); }
        *out = INTVAL(0); return 1;
    }
    if (!strcmp(fn, "slurp") && nargs == 1) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        FILE *fp = NULL; int need_close = 0;
        if (IS_INT_fn(args[0]) || IS_FH_fn(args[0])) {
            fh_ensure_init();
            fp = fh_get((int)args[0].i);
        } else {
            const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
            fp = fopen(path, "r"); need_close = 1;
        }
        if (!fp) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        fseek(fp, 0, SEEK_END); long sz = ftell(fp); rewind(fp);
        char *buf = rt_ws_alloc(sz + 1);
        size_t nr = fread(buf, 1, (size_t)sz, fp); buf[nr] = '\0';
        if (need_close) fclose(fp);
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "lines") && nargs == 1) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        FILE *fp = NULL; int need_close = 0;
        if (IS_INT_fn(args[0]) || IS_FH_fn(args[0])) {
            fh_ensure_init();
            fp = fh_get((int)args[0].i);
        } else {
            const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
            fp = fopen(path, "r"); need_close = 1;
        }
        if (!fp) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *acc = rt_ws_alloc(65536); acc[0] = '\0'; size_t cap = 65536, used = 0; int first = 1;
        char line[4096];
        while (fgets(line, sizeof line, fp)) {
            size_t ll = strlen(line);
            while (ll > 0 && (line[ll-1] == '\n' || line[ll-1] == '\r')) line[--ll] = '\0';
            size_t need = used + ll + 2;
            if (need > cap) { cap = need * 2; char *nb = rt_ws_alloc(cap); memcpy(nb, acc, used); acc = nb; }
            if (!first) { acc[used++] = '\x01'; }
            memcpy(acc + used, line, ll); used += ll; acc[used] = '\0'; first = 0;
        }
        if (need_close) fclose(fp);
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "spurt") && nargs == 2) {
        const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = FAILDESCR; return 1; }
        const char *content = VARVAL_fn(args[1]); if (!content) content = "";
        FILE *fp = fopen(path, "w"); if (!fp) { *out = FAILDESCR; return 1; }
        fputs(content, fp); fclose(fp);
        *out = INTVAL(0); return 1;
    }
    if ((!strcmp(fn, "print_fh") || !strcmp(fn, "say_fh")) && nargs == 2) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        fh_ensure_init();
        int idx = (int)(IS_INT_fn(args[0]) || IS_FH_fn(args[0]) ? args[0].i : 1);
        FILE *fp = fh_get(idx); if (!fp) fp = stdout;
        if (fp != stdout) fflush(stdout);
        char scratch[64]; const char *s = to_cstring(args[1], scratch, sizeof scratch);
        fputs(s, fp);
        if (!strcmp(fn, "say_fh")) fputc('\n', fp);
        *out = INTVAL(0); return 1;
    }
    if (!strcmp(fn, "field_get_pub") && nargs == 2) {
        const char *fname = VARVAL_fn(args[1]); if (!fname) fname = "";
        if (args[0].v == DT_DATA && args[0].u) {
            DATINST_t *di = (DATINST_t *)args[0].u;
            const char *cn = (di && di->type) ? di->type->name : NULL;
            extern int dat_field_is_private(const char *cls, const char *field);
            if (cn && dat_field_is_private(cn, fname)) {
                extern void rt_script_die_surface(const char *msg);
                char _m[256]; snprintf(_m, sizeof _m, "Attribute '$!%s' not accessible outside of class %s (it is private)", fname, cn);
                rt_script_die_surface(_m); *out = FAILDESCR; return 1;
            }
        }
        extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
        *out = dat_field_get(fname, args[0]); return 1;
    }
    if (!strcmp(fn, "field_set") && nargs == 3) {
        extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
        const char *fname = VARVAL_fn(args[1]); if (!fname) fname = "";
        DESCR_t *cell = data_field_ptr(fname, args[0]);
        if (cell) *cell = args[2];
        *out = args[2]; return 1;
    }
    if (!strcmp(fn, "field_set_pub") && nargs == 3) {
        extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
        const char *fname = VARVAL_fn(args[1]); if (!fname) fname = "";
        if (args[0].v == DT_DATA && args[0].u) {
            DATINST_t *di = (DATINST_t *)args[0].u;
            const char *cn = (di && di->type) ? di->type->name : NULL;
            extern int dat_field_is_private(const char *cls, const char *field);
            if (cn && dat_field_is_private(cn, fname)) {
                extern void rt_script_die_surface(const char *msg);
                char _m[256]; snprintf(_m, sizeof _m, "Attribute '$!%s' not accessible outside of class %s (it is private)", fname, cn);
                rt_script_die_surface(_m); *out = FAILDESCR; return 1;
            }
            DatType *dt = cn ? dat_find_type(cn) : NULL;
            if (dt) { int isrw = 0, found = 0;
                for (int i = 0; i < dt->nfields; i++) if (!strcmp(dt->fields[i], fname)) { found = 1; isrw = dt->rw[i]; break; }
                if (found && !isrw) {
                    extern void rt_script_die_surface(const char *msg);
                    char _m[256]; snprintf(_m, sizeof _m, "Cannot modify an immutable attribute '$.%s' (declare it 'is rw' to allow assignment)", fname);
                    rt_script_die_surface(_m); *out = FAILDESCR; return 1; } }
        }
        DESCR_t *cell = data_field_ptr(fname, args[0]);
        if (cell) *cell = args[2];
        *out = args[2]; return 1;
    }
    if (!strcmp(fn, "obj_new") && nargs >= 1) {
        const char *cname = VARVAL_fn(args[0]); if (!cname || !*cname) { *out = FAILDESCR; return 1; }
        char newproc[256]; resolve_method_chain(cname, "new", newproc, sizeof newproc, NULL);
        extern int rt_proc_has_native_fn(const char *name);
        if (meth_is_user_proc(newproc)) {
            int pi;
            for (pi = 0; pi < g_stage2.proc_count; pi++)
                if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, newproc)) break;
            if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(newproc)) {
                extern DESCR_t g_call_args[];
                extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
                int total = nargs; for (int k = 0; k < total && k < 64; k++) g_call_args[k] = args[k];
                *out = rt_call_proc_descr(newproc, total); return 1;
            }
            extern DESCR_t ir_call_proc(int pi, DESCR_t *args, int nargs);
            *out = ir_call_proc(pi, args, nargs); return 1;
        }
        DatType *dt = dat_find_type(cname); if (!dt) { *out = FAILDESCR; return 1; }
        extern int dat_has_build_mro(const char *cls);
        if (dat_has_build_mro(cname)) {
            extern DESCR_t rt_construct_build(DatType *t, DESCR_t *named, int nnamed);
            *out = rt_construct_build(dt, &args[1], nargs - 1); return 1;
        }
        DESCR_t fvals[64];
        for (int fi = 0; fi < dt->nfields && fi < 64; fi++) fvals[fi] = NULVCL;
        for (int ci = 1; ci + 1 < nargs; ci += 2) {
            const char *kname = VARVAL_fn(args[ci]); if (!kname) continue;
            for (int fi = 0; fi < dt->nfields && fi < 64; fi++) {
                if (strcmp(dt->fields[fi], kname) == 0) { fvals[fi] = args[ci + 1]; break; }
            }
        }
        *out = dat_construct(dt, fvals, dt->nfields); return 1;
    }
    if (!strcmp(fn, "meth_call") && nargs >= 2) {
        const char *mname0 = VARVAL_fn(args[1]);
        if (mname0 && mname0[0] == '^') {
            const char *mm = mname0 + 1; const char *cn = NULL;
            if (args[0].v == DT_DATA && args[0].u) { DATINST_t *di = (DATINST_t *)args[0].u; cn = (di && di->type) ? di->type->name : NULL; }
            else { cn = VARVAL_fn(args[0]); if (cn && !dat_find_type(cn)) cn = NULL; }
            if (cn && !strcmp(mm, "name")) { *out = STRVAL(rt_ws_strdup_c(cn)); return 1; }
            if (cn && !strcmp(mm, "parents")) {
                extern int dat_mro(const char *name, const char **out, int max); const char *mro[64]; int mn = dat_mro(cn, mro, 64);
                char buf[1024]; int pos = 0; buf[0] = 0;
                for (int i = 1; i < mn; i++) { if (!mro[i]) continue; pos += snprintf(buf + pos, (int)sizeof buf - pos, "%s%s", pos ? " " : "", mro[i]); }
                *out = STRVAL(rt_ws_strdup_c(buf)); return 1;
            }
            if (cn && !strcmp(mm, "methods")) {
                extern int dat_methods(const char *name, const char **out, int max); const char *ms[256]; int mn = dat_methods(cn, ms, 256);
                char buf[2048]; int pos = 0; buf[0] = 0;
                for (int i = 0; i < mn; i++) { if (!ms[i]) continue; pos += snprintf(buf + pos, (int)sizeof buf - pos, "%s%s", pos ? " " : "", ms[i]); }
                *out = STRVAL(rt_ws_strdup_c(buf)); return 1;
            }
            if (cn && !strcmp(mm, "attributes")) {
                extern int dat_attributes(const char *name, const char **out, int max); const char *as[256]; int an = dat_attributes(cn, as, 256);
                char buf[2048]; int pos = 0; buf[0] = 0;
                for (int i = 0; i < an; i++) { if (!as[i]) continue; pos += snprintf(buf + pos, (int)sizeof buf - pos, "%s%s", pos ? " " : "", as[i]); }
                *out = STRVAL(rt_ws_strdup_c(buf)); return 1;
            }
            *out = FAILDESCR; return 1;
        }
        if (mname0 && !strcmp(mname0, "WHAT")) {
            const char *cn = NULL;
            if (args[0].v == DT_DATA && args[0].u) { DATINST_t *di = (DATINST_t *)args[0].u; cn = (di && di->type) ? di->type->name : NULL; }
            else { cn = VARVAL_fn(args[0]); if (cn && !dat_find_type(cn)) cn = NULL; }
            if (cn) { *out = STRVAL(rt_ws_strdup_c(cn)); return 1; }
            *out = FAILDESCR; return 1;
        }
        if (mname0 && (!strcmp(mname0, "isa") || !strcmp(mname0, "does")) && nargs >= 3) {
            const char *cn = NULL;
            if (args[0].v == DT_DATA && args[0].u) { DATINST_t *di = (DATINST_t *)args[0].u; cn = (di && di->type) ? di->type->name : NULL; }
            else { cn = VARVAL_fn(args[0]); if (cn && !dat_find_type(cn)) cn = NULL; }
            const char *target = VARVAL_fn(args[2]);
            if (!cn || !target) { *out = INTVAL(0); return 1; }
            extern int dat_mro(const char *name, const char **out, int max);
            extern int dat_roles(const char *name, const char **out, int max);
            int hit = 0; const char *mro[64]; int mn = dat_mro(cn, mro, 64);
            if (mn == 0) { mro[0] = cn; mn = 1; }
            for (int i = 0; i < mn && !hit; i++) if (mro[i] && !strcmp(mro[i], target)) hit = 1;
            if (!hit && !strcmp(mname0, "does")) {
                for (int i = 0; i < mn && !hit; i++) { const char *rls[8]; int rn = dat_roles(mro[i], rls, 8);
                    for (int j = 0; j < rn && !hit; j++) if (rls[j] && !strcmp(rls[j], target)) hit = 1; } }
            *out = INTVAL(hit ? 1 : 0); return 1;
        }
        if (mname0 && !strcmp(mname0, "bless")) {
            const char *cname = VARVAL_fn(args[0]); if (!cname || !*cname) { *out = FAILDESCR; return 1; }
            DatType *dt = dat_find_type(cname); if (!dt) { *out = FAILDESCR; return 1; }
            DESCR_t fvals[64];
            for (int fi = 0; fi < dt->nfields && fi < 64; fi++) fvals[fi] = NULVCL;
            for (int ci = 2; ci + 1 < nargs; ci += 2) {
                const char *kname = VARVAL_fn(args[ci]); if (!kname) continue;
                for (int fi = 0; fi < dt->nfields && fi < 64; fi++)
                    if (strcmp(dt->fields[fi], kname) == 0) { fvals[fi] = args[ci + 1]; break; }
            }
            *out = dat_construct(dt, fvals, dt->nfields); return 1;
        }
        if (mname0 && !strcmp(mname0, "clone") && args[0].v == DT_DATA && args[0].u) {
            DATINST_t *src = (DATINST_t *)args[0].u; const char *cname = (src && src->type) ? src->type->name : NULL;
            DatType *dt = cname ? dat_find_type(cname) : NULL; if (!dt) { *out = FAILDESCR; return 1; }
            DESCR_t fvals[64];
            for (int fi = 0; fi < dt->nfields && fi < 64; fi++) fvals[fi] = (src->fields && fi < src->type->nfields) ? src->fields[fi] : NULVCL;
            for (int ci = 2; ci + 1 < nargs; ci += 2) {
                const char *kname = VARVAL_fn(args[ci]); if (!kname) continue;
                for (int fi = 0; fi < dt->nfields && fi < 64; fi++) if (strcmp(dt->fields[fi], kname) == 0) { fvals[fi] = args[ci + 1]; break; }
            }
            *out = dat_construct(dt, fvals, dt->nfields); return 1;
        }
        if (mname0 && !strcmp(mname0, "parse") && nargs == 3) {
            const char *gname = VARVAL_fn(args[0]);
            if (gname && rt_grammar_has_top(gname)) { const char *subj = VARVAL_fn(args[2]); return grammar_parse_core(gname, subj, out); }
        }
        if (nargs >= 2 && args[0].v != DT_DATA && rt_str_method(mname0, args[0], &args[2], nargs - 2, out)) return 1;
        if (args[0].v != DT_DATA && mname0) {
            const char *rtn = VARVAL_fn(args[0]);
            int is_dat_recv = (rtn && dat_find_type(rtn));
            if (!is_dat_recv && rtn && strchr(rtn, '\x02')) {
                int is_hashm = !strcmp(mname0, "keys") || !strcmp(mname0, "values") || !strcmp(mname0, "pairs") || !strcmp(mname0, "kv");
                if (is_hashm) {
                    const char *hfn = !strcmp(mname0, "keys") ? "hash_keys" : !strcmp(mname0, "values") ? "hash_values"
                                    : !strcmp(mname0, "pairs") ? "hash_pairs" : "hash_kv";
                    DESCR_t fa[1]; fa[0] = args[0];
                    extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
                    if (script_try_call_builtin_by_name(hfn, fa, 1, out)) return 1;
                }
            }
            if (!is_dat_recv) {
                int is_arrm = !strcmp(mname0, "reverse") || !strcmp(mname0, "unique") || !strcmp(mname0, "sort")
                           || !strcmp(mname0, "elems") || !strcmp(mname0, "end") || !strcmp(mname0, "join") || !strcmp(mname0, "sum")
                           || !strcmp(mname0, "head") || !strcmp(mname0, "tail") || !strcmp(mname0, "min")
                           || !strcmp(mname0, "max") || !strcmp(mname0, "first")
                           || !strcmp(mname0, "keys") || !strcmp(mname0, "values");
                if (is_arrm) {
                    const char *afn = !strcmp(mname0, "sort") ? "__rk_arr_sort" : !strcmp(mname0, "min") ? "__rk_arr_min"
                                    : !strcmp(mname0, "max") ? "__rk_arr_max" : !strcmp(mname0, "first") ? "__rk_arr_first"
                                    : !strcmp(mname0, "keys") ? "__rk_arr_keys" : !strcmp(mname0, "values") ? "__rk_arr_values"
                                    : !strcmp(mname0, "end") ? "elems" : mname0;
                    int total = 1 + (nargs - 2);
                    DESCR_t *fa = rt_ws_alloc((size_t)total * sizeof(DESCR_t));
                    if (!strcmp(mname0, "join")) { for (int k = 0; k < nargs - 2; k++) fa[k] = args[2 + k]; fa[nargs - 2] = args[0]; }
                    else { fa[0] = args[0]; for (int k = 0; k < nargs - 2; k++) fa[1 + k] = args[2 + k]; }
                    extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
                    if (script_try_call_builtin_by_name(afn, fa, total, out)) {
                        if (!strcmp(mname0, "end") && IS_INT_fn(*out)) *out = INTVAL(out->i - 1);   /* @a.end == @a.elems - 1 (last valid index; -1 for an empty array) */
                        return 1;
                    }
                }
            }
        }
        if (args[0].v != DT_DATA) {
            const char *tname = VARVAL_fn(args[0]);
            extern int rt_proc_has_native_fn(const char *name);
            if (tname && dat_find_type(tname)) {
                char tproc[256]; resolve_method_chain(tname, mname0, tproc, sizeof tproc, NULL);
                if (meth_is_user_proc(tproc)) {
                    int nextra = nargs - 2, total = 1 + nextra;
                    DESCR_t *ca = rt_ws_alloc((size_t)total * sizeof(DESCR_t));
                    ca[0] = args[0]; for (int k = 0; k < nextra; k++) ca[1 + k] = args[2 + k];
                    int pi; for (pi = 0; pi < g_stage2.proc_count; pi++)
                        if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, tproc)) break;
                    if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(tproc)) {
                        extern DESCR_t g_call_args[]; extern DESCR_t rt_call_proc_descr(const char *name, int nargs);
                        for (int k = 0; k < total && k < 64; k++) g_call_args[k] = ca[k];
                        *out = rt_call_proc_descr(tproc, total); return 1;
                    }
                    extern DESCR_t ir_call_proc(int pi, DESCR_t *args, int nargs);
                    *out = ir_call_proc(pi, ca, total); return 1;
                }
            }
        }
        if (args[0].v != DT_DATA || !args[0].u) { *out = FAILDESCR; return 1; }
        DATINST_t *inst = (DATINST_t *)args[0].u;
        const char *cname = (inst && inst->type) ? inst->type->name : NULL;
        if (!cname) { *out = FAILDESCR; return 1; }
        const char *mname = VARVAL_fn(args[1]); if (!mname || !*mname) { *out = FAILDESCR; return 1; }
        char procname[256];
        int found_idx = -1;
        resolve_method_chain(cname, mname, procname, sizeof procname, &found_idx);
        if (!meth_is_user_proc(procname) && rt_multi_meth_dispatch(cname, mname, args, nargs, out)) return 1;
        if (nargs == 2 && !meth_is_user_proc(procname)) {
            extern int dat_field_is_private(const char *cls, const char *field);
            if (dat_field_is_private(cname, mname)) {
                extern void rt_script_die_surface(const char *msg);
                char _m[256]; snprintf(_m, sizeof _m, "Attribute '$!%s' not accessible outside of class %s (it is private)", mname, cname);
                rt_script_die_surface(_m); *out = FAILDESCR; return 1;
            }
            extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
            DESCR_t *acc = data_field_ptr(mname, args[0]);
            if (acc) { *out = *acc; return 1; }
        }
        if (!meth_is_user_proc(procname)) {
            extern int dat_handles_field(const char *cls, const char *meth, char *out, int outsz);
            char delegfield[64];
            if (dat_handles_field(cname, mname, delegfield, sizeof delegfield)) {
                extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
                DESCR_t deleg = dat_field_get(delegfield, args[0]);
                DESCR_t *fwd = rt_ws_alloc((size_t)nargs * sizeof(DESCR_t)); fwd[0] = deleg; fwd[1] = args[1];
                for (int k = 2; k < nargs; k++) fwd[k] = args[k];
                extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
                return script_try_call_builtin_by_name("meth_call", fwd, nargs, out);
            }
        }
        int nextra = nargs - 2;
        int total = 1 + nextra;
        DESCR_t *callargs = rt_ws_alloc((size_t)total * sizeof(DESCR_t));
        callargs[0] = args[0];
        for (int k = 0; k < nextra; k++) callargs[1 + k] = args[2 + k];
        int rd = -1;
        if (g_redisp_top < 64) {
            rd = g_redisp_top++;
            extern int dat_mro(const char *name, const char **out, int max);
            g_redisp[rd].self = args[0]; snprintf(g_redisp[rd].mname, sizeof g_redisp[rd].mname, "%s", mname);
            g_redisp[rd].mro_len = dat_mro(cname, g_redisp[rd].mro, 64);
            if (g_redisp[rd].mro_len == 0) { g_redisp[rd].mro[0] = cname; g_redisp[rd].mro_len = 1; }
            g_redisp[rd].found_idx = found_idx;
            g_redisp[rd].nargs = total < 16 ? total : 16;
            for (int k = 0; k < g_redisp[rd].nargs; k++) g_redisp[rd].args[k] = callargs[k];
        }
        DESCR_t _mr = invoke_method_proc(procname, callargs, total);
        if (rd >= 0) g_redisp_top--;
        *out = _mr; return 1;
    }
    if (!strcmp(fn, "re_match") && nargs == 2) {
        const char *subj = VARVAL_fn(args[0]); if (!subj) subj = "";
        const char *pat  = VARVAL_fn(args[1]); if (!pat)  pat  = "";
        Nfa *nfa = nfa_build(pat);
        if (!nfa) { *out = FAILDESCR; return 1; }
        nfa_exec(nfa, subj, &g_match);
        g_subject = subj;
        int verdict = g_match.matched ? 1 : 0;
        nfa_free(nfa);
        *out = verdict ? INTVAL(1) : FAILDESCR; return 1;
    }
    if (!strcmp(fn, "nfa_accepts") && nargs == 2) {
        DESCR_t topic = args[0], matcher = args[1];
        if (junction_is(matcher)) {
            int numeric = (IS_INT_fn(topic) || IS_REAL_fn(topic));
            int ok = junction_collapse(topic, matcher, TT_EQ, numeric);
            *out = ok ? INTVAL(1) : FAILDESCR; return 1;
        }
        int both_num = (IS_INT_fn(topic) || IS_REAL_fn(topic)) && (IS_INT_fn(matcher) || IS_REAL_fn(matcher));
        int ok;
        if (both_num) {
            ok = (to_real(topic) == to_real(matcher));
        } else {
            const char *ts = VARVAL_fn(topic); if (!ts) ts = "";
            const char *ms = VARVAL_fn(matcher); if (!ms) ms = "";
            ok = (strcmp(ts, ms) == 0);
        }
        *out = ok ? INTVAL(1) : FAILDESCR; return 1;
    }
    if (!strcmp(fn, "grammar_register") && nargs == 3) {
        const char *qn   = VARVAL_fn(args[0]);
        const char *body = VARVAL_fn(args[1]);
        const char *flv  = VARVAL_fn(args[2]);
        if (qn && body) gram_set(qn, body, flv ? atoi(flv) : 0);
        *out = INTVAL(1); return 1;
    }
    if (!strcmp(fn, "grammar_parse") && nargs == 2) {
        const char *gname = VARVAL_fn(args[0]);
        const char *subj  = VARVAL_fn(args[1]);
        return grammar_parse_core(gname, subj, out);
    }
    if (!strcmp(fn, "re_match_global") && nargs == 2) {
        const char *subj = VARVAL_fn(args[0]); if (!subj) subj = "";
        const char *pat  = VARVAL_fn(args[1]); if (!pat)  pat  = "";
        Nfa *nfa = nfa_build(pat);
        if (!nfa) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int slen = (int)strlen(subj);
        char *acc = rt_ws_alloc((size_t)slen * 4 + 4); acc[0] = '\0';
        int pos = 0, count = 0;
        while (pos <= slen) {
            Match m; nfa_exec(nfa, subj + pos, &m);
            if (!m.matched) break;
            int mlen = m.full_end - m.full_start;
            if (count > 0) { int ol = (int)strlen(acc); acc[ol] = SOH; acc[ol + 1] = '\0'; }
            strncat(acc, subj + pos + m.full_start, (size_t)mlen);
            g_match = m;
            g_match.full_start += pos;
            g_match.full_end   += pos;
            for (int g = 0; g < m.ngroups; g++) {
                if (m.group_start[g] >= 0) g_match.group_start[g] += pos;
                if (m.group_end[g]   >= 0) g_match.group_end[g]   += pos;
            }
            g_subject = subj;
            pos += m.full_start + (mlen > 0 ? mlen : 1);
            count++;
        }
        nfa_free(nfa);
        *out = count > 0 ? STRVAL(acc) : FAILDESCR; return 1;
    }
    if (!strcmp(fn, "re_subst") && nargs == 2) {
        const char *subj = VARVAL_fn(args[0]); if (!subj) subj = "";
        const char *tok  = VARVAL_fn(args[1]); if (!tok)  tok  = "";
        const char *sep1 = strchr(tok, SOH);
        if (!sep1) { *out = args[0]; return 1; }
        const char *sep2 = strchr(sep1 + 1, SOH);
        if (!sep2) { *out = args[0]; return 1; }
        int plen = (int)(sep1 - tok);
        int rlen = (int)(sep2 - (sep1 + 1));
        char *pat  = rt_ws_alloc((size_t)plen + 1); memcpy(pat, tok, (size_t)plen); pat[plen] = '\0';
        char *repl = rt_ws_alloc((size_t)rlen + 1); memcpy(repl, sep1 + 1, (size_t)rlen); repl[rlen] = '\0';
        int global = (*(sep2 + 1) == 'g');
        Nfa *nfa = nfa_build(pat);
        if (!nfa) { *out = args[0]; return 1; }
        int slen = (int)strlen(subj);
        char *res = rt_ws_alloc((size_t)slen * 4 + (size_t)rlen * 8 + 4); res[0] = '\0';
        int pos = 0, did_one = 0;
        while (pos <= slen) {
            Match m; nfa_exec(nfa, subj + pos, &m);
            if (!m.matched) { strncat(res, subj + pos, (size_t)(slen - pos)); break; }
            strncat(res, subj + pos, (size_t)m.full_start);
            strcat(res, repl);
            g_match = m; g_subject = subj;
            int advance = m.full_start + (m.full_end - m.full_start > 0 ? m.full_end - m.full_start : 1);
            pos += advance; did_one = 1;
            if (!global) { strncat(res, subj + pos, (size_t)(slen - pos)); break; }
        }
        nfa_free(nfa);
        *out = did_one ? STRVAL(res) : args[0]; return 1;
    }
    if (!strcmp(fn, "nfa_compile") && nargs == 1) {
        const char *pat = VARVAL_fn(args[0]); if (!pat) pat = "";
        Nfa *nfa = nfa_build(pat);
        if (!nfa) { printf("NFA:%s:ERROR\n", pat); *out = INTVAL(0); return 1; }
        printf("NFA:%s:states=%d\n", pat, nfa_state_count(nfa));
        nfa_free(nfa);
        *out = INTVAL(0); return 1;
    }
    if (!strcmp(fn, "re_capture") && nargs == 1) {
        int n = (int)(IS_INT_fn(args[0]) ? args[0].i : 0);
        if (!g_match.matched || n < 0 || n >= g_match.ngroups || g_match.group_start[n] < 0) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int gs = g_match.group_start[n], ge = g_match.group_end[n];
        if (ge < gs) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int len = ge - gs; char *o = rt_ws_alloc((size_t)len + 1);
        memcpy(o, g_subject + gs, (size_t)len); o[len] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "re_named_capture") && nargs == 1) {
        const char *name = VARVAL_fn(args[0]); if (!name) name = "";
        if (!g_match.matched) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int g = -1;
        for (int i = 0; i < g_match.ngroups; i++) if (strcmp(g_match.group_name[i], name) == 0) { g = i; break; }
        if (g < 0 || g_match.group_start[g] < 0) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int gs = g_match.group_start[g], ge = g_match.group_end[g];
        if (ge < gs) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        int len = ge - gs; char *o = rt_ws_alloc((size_t)len + 1);
        memcpy(o, g_subject + gs, (size_t)len); o[len] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "push_pure") && nargs >= 2) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char *acc = rt_ws_strdup(cur);
        for (int i = 1; i < nargs; i++) {
            char rb[64]; const char *rv = to_cstring(args[i], rb, sizeof rb);
            size_t ol = strlen(acc), rl = strlen(rv);
            char *no = rt_ws_alloc(ol + rl + 2);
            memcpy(no, acc, ol);
            if (ol > 0) { no[ol] = SOH; memcpy(no + ol + 1, rv, rl); no[ol + 1 + rl] = '\0'; }
            else        { memcpy(no, rv, rl); no[rl] = '\0'; }
            acc = no;
        }
        *out = STRVAL(acc); return 1;
    }
    /* ⭐ unshift_pure -- row raku-silent-wrong-answers, seat15 2026-08-30. Same accumulation loop as
       push_pure immediately above (new elements joined IN ORDER, exactly mirroring its style), just
       built into its own buffer starting empty rather than from `cur`, then placed BEFORE the current
       array content instead of after -- @a.unshift(4,5,6) on [1,2] yields [4,5,6,1,2], matching real
       Raku (the new elements keep their own relative order; only their position moves). */
    if (!strcmp(fn, "unshift_pure") && nargs >= 2) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char *acc = rt_ws_strdup("");
        for (int i = 1; i < nargs; i++) {
            char rb[64]; const char *rv = to_cstring(args[i], rb, sizeof rb);
            size_t ol = strlen(acc), rl = strlen(rv);
            char *no = rt_ws_alloc(ol + rl + 2);
            memcpy(no, acc, ol);
            if (ol > 0) { no[ol] = SOH; memcpy(no + ol + 1, rv, rl); no[ol + 1 + rl] = '\0'; }
            else        { memcpy(no, rv, rl); no[rl] = '\0'; }
            acc = no;
        }
        if (!*cur) { *out = STRVAL(acc); return 1; }
        size_t al = strlen(acc), cl = strlen(cur);
        char *o = rt_ws_alloc(al + 1 + cl + 1);
        memcpy(o, acc, al); o[al] = SOH; memcpy(o + al + 1, cur, cl); o[al + 1 + cl] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "arr_set_pure") && nargs >= 3) {
        if (args[0].v == DT_A && args[0].arr) {
            ARBLK_t *b = (ARBLK_t *) args[0].arr; long i = IS_INT_fn(args[1]) ? args[1].i : 0; if (i < b->lo || i > b->hi) { *out = FAILDESCR; return 1; } b->data[i - b->lo] = args[2];
            *out = args[0]; return 1;
        }
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        char rb[64]; const char *rv = to_cstring(args[2], rb, sizeof rb);
        size_t rvl = strlen(rv);
        if (!strchr(cur, SOH)) {
            size_t slen = strlen(cur);
            if (idx == 0) { char *e0 = rt_ws_alloc(rvl + 1); memcpy(e0, rv, rvl); e0[rvl] = '\0'; *out = STRVAL(e0); return 1; }
            if (idx < 1 || (size_t)idx > slen) { *out = FAILDESCR; return 1; }
            char *buf = rt_ws_alloc(slen * 5 + rvl + 4);
            size_t pos = 0; buf[pos++] = '0';
            for (size_t j = 1; j <= slen; j++) {
                buf[pos++] = SOH;
                if ((long)j == idx) { memcpy(buf + pos, rv, rvl); pos += rvl; }
                else { int od = (unsigned char)cur[j-1]; pos += (size_t)sprintf(buf + pos, "%d", od); }
            }
            buf[pos] = '\0'; *out = STRVAL(buf); return 1;
        }
        const char *seg = cur;
        long k = 0; const char *tstart = NULL; const char *tend = NULL;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            if (k == idx) { tstart = seg; tend = nx; break; }
            if (!nx) { *out = FAILDESCR; return 1; }
            seg = nx + 1; k++;
        }
        size_t pre = (size_t)(tstart - cur);
        size_t post = tend ? strlen(tend) : 0;
        size_t total = pre + rvl + post;
        char *o = rt_ws_alloc(total + 1);
        memcpy(o, cur, pre);
        memcpy(o + pre, rv, rvl);
        if (tend) memcpy(o + pre + rvl, tend, post);
        o[total] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "arr_last") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = FAILDESCR; return 1; }
        const char *last = strrchr(cur, SOH);
        const char *pstart = last ? last + 1 : cur;
        *out = elem_to_descr(pstart, strlen(pstart)); return 1;
    }
    if (!strcmp(fn, "arr_init") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        const char *last = strrchr(cur, SOH);
        if (!last) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        size_t nl = (size_t)(last - cur);
        char *o = rt_ws_alloc(nl + 1); memcpy(o, cur, nl); o[nl] = '\0';
        *out = STRVAL(o); return 1;
    }
    /* ⭐ arr_tail -- row raku-silent-wrong-answers, seat15 2026-08-30. arr_init's mirror image: everything
       AFTER the first element instead of everything BEFORE the last -- the array-mutation half of
       @a.shift (its "removed element" half reuses the already-existing __rk_arr_first, unmodified). */
    if (!strcmp(fn, "arr_tail") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        const char *first = strchr(cur, SOH);
        if (!first) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        *out = STRVAL(rt_ws_strdup_c(first + 1)); return 1;
    }
    if (!strcmp(fn, "hash_set_pure") && nargs >= 3) {
        extern char *script_hash_set_str(const char *h, const char *key, const char *val);
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        char vb[64]; const char *val = to_cstring(args[2], vb, sizeof vb);
        *out = STRVAL(script_hash_set_str(cur, key, val)); return 1;
    }
    if (!strcmp(fn, "hash_delete_pure") && nargs >= 2) {
        extern char *script_hash_delete_str(const char *h, const char *key);
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        *out = STRVAL(script_hash_delete_str(cur, key)); return 1;
    }
    extern int script_try_hash_builtin(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
    if (script_try_hash_builtin(fn, args, nargs, out)) return 1;
    return 0;
}
#define STX '\x02'
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *hash_find(const char *h, const char *key, const char **p_pair_start) {
    if (!h || !key) return NULL;
    size_t klen = strlen(key);
    const char *seg = h;
    while (*seg) {
        const char *nx   = strchr(seg, SOH);
        size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
        const char *stx  = (const char *)memchr(seg, STX, plen);
        if (stx) {
            size_t this_klen = (size_t)(stx - seg);
            if (this_klen == klen && memcmp(seg, key, klen) == 0) {
                if (p_pair_start) *p_pair_start = seg;
                return stx + 1;
            }
        }
        if (!nx) break;
        seg = nx + 1;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *script_hash_set_str(const char *h, const char *key, const char *val) {
    if (!h) h = "";
    size_t klen = strlen(key), vlen = strlen(val);
    const char *pair_start = NULL;
    hash_find(h, key, &pair_start);
    if (pair_start) {
        const char *nx = strchr(pair_start, SOH);
        size_t pre  = (size_t)(pair_start - h);
        size_t post = nx ? strlen(nx) : 0;
        size_t total = pre + klen + 1 + vlen + post;
        char *o = rt_ws_alloc(total + 1);
        memcpy(o, h, pre);
        memcpy(o + pre, key, klen);
        o[pre + klen] = STX;
        memcpy(o + pre + klen + 1, val, vlen);
        if (nx) memcpy(o + pre + klen + 1 + vlen, nx, post);
        o[total] = '\0'; return o;
    }
    size_t hlen = strlen(h);
    int need_sep = (hlen > 0);
    size_t total = hlen + (need_sep ? 1 : 0) + klen + 1 + vlen;
    char *o = rt_ws_alloc(total + 1);
    memcpy(o, h, hlen);
    if (need_sep) o[hlen] = SOH;
    memcpy(o + hlen + (need_sep ? 1 : 0), key, klen);
    o[hlen + (need_sep ? 1 : 0) + klen] = STX;
    memcpy(o + hlen + (need_sep ? 1 : 0) + klen + 1, val, vlen);
    o[total] = '\0'; return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *script_hash_delete_str(const char *h, const char *key) {
    if (!h || !*h) return rt_ws_strdup("");
    const char *pair_start = NULL;
    hash_find(h, key, &pair_start);
    if (!pair_start) return rt_ws_strdup(h);
    const char *nx = strchr(pair_start, SOH);
    size_t pre = (size_t)(pair_start - h);
    size_t trim_pre = pre;
    if (pre > 0) trim_pre = pre - 1;
    else if (nx) { nx++; }
    size_t post = nx ? strlen(nx) : 0;
    size_t total = trim_pre + post;
    char *o = rt_ws_alloc(total + 1);
    if (pre > 0) { memcpy(o, h, trim_pre); if (nx) memcpy(o + trim_pre, nx, post); }
    else if (nx) memcpy(o, nx, post);
    o[total] = '\0'; return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int script_try_hash_builtin(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!fn || nargs < 1) return 0;
    if (args[0].v != DT_S && args[0].v != DT_SNUL) return 0;
    const char *h = VARVAL_fn(args[0]); if (!h) h = "";
    if (!strcmp(fn, "hash_get") && nargs >= 2) {
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        const char *vstart = hash_find(h, key, NULL);
        if (!vstart) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        const char *vend = strchr(vstart, SOH);
        size_t vlen = vend ? (size_t)(vend - vstart) : strlen(vstart);
        char *v = rt_ws_alloc(vlen + 1); memcpy(v, vstart, vlen); v[vlen] = '\0';
        char *ep; long iv = strtol(v, &ep, 10);
        if (*ep == '\0' && ep > v) { *out = INTVAL(iv); return 1; }
        *out = STRVAL(v); return 1;
    }
    if (!strcmp(fn, "hash_exists") && nargs >= 2) {
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        *out = hash_find(h, key, NULL) ? INTVAL(1) : INTVAL(0); return 1;
    }
    if (!strcmp(fn, "hash_keys") && nargs >= 1) {
        if (!*h) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *acc = rt_ws_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                size_t klen = (size_t)(stx - seg);
                char *k = rt_ws_alloc(klen + 1); memcpy(k, seg, klen); k[klen] = '\0';
                size_t al = strlen(acc);
                char *na = rt_ws_alloc(al + (al ? 1 : 0) + klen + 1);
                memcpy(na, acc, al);
                if (al) na[al++] = SOH;
                memcpy(na + al, k, klen); na[al + klen] = '\0';
                acc = na;
            }
            if (!nx) break;
            seg = nx + 1;
        }
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "hash_values") && nargs >= 1) {
        if (!*h) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *acc = rt_ws_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                const char *vstart = stx + 1;
                size_t vlen = nx ? (size_t)(nx - vstart) : strlen(vstart);
                char *v = rt_ws_alloc(vlen + 1); memcpy(v, vstart, vlen); v[vlen] = '\0';
                size_t al = strlen(acc);
                char *na = rt_ws_alloc(al + (al ? 1 : 0) + vlen + 1);
                memcpy(na, acc, al);
                if (al) na[al++] = SOH;
                memcpy(na + al, v, vlen); na[al + vlen] = '\0';
                acc = na;
            }
            if (!nx) break;
            seg = nx + 1;
        }
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "hash_pairs") && nargs >= 1) {
        if (!*h) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *acc = rt_ws_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                size_t klen = (size_t)(stx - seg);
                const char *vstart = stx + 1;
                size_t vlen = nx ? (size_t)(nx - vstart) : strlen(vstart);
                size_t pl = klen + 1 + vlen;
                char *pair = rt_ws_alloc(pl + 1);
                memcpy(pair, seg, klen); pair[klen] = ':';
                memcpy(pair + klen + 1, vstart, vlen); pair[pl] = '\0';
                size_t al = strlen(acc);
                char *na = rt_ws_alloc(al + (al ? 1 : 0) + pl + 1);
                memcpy(na, acc, al);
                if (al) na[al++] = SOH;
                memcpy(na + al, pair, pl); na[al + pl] = '\0';
                acc = na;
            }
            if (!nx) break;
            seg = nx + 1;
        }
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "hash_kv") && nargs >= 1) {
        if (!*h) { *out = STRVAL(rt_ws_strdup_c("")); return 1; }
        char *acc = rt_ws_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                size_t klen = (size_t)(stx - seg);
                const char *vstart = stx + 1;
                size_t vlen = nx ? (size_t)(nx - vstart) : strlen(vstart);
                size_t al = strlen(acc);
                char *na = rt_ws_alloc(al + (al ? 1 : 0) + klen + 1 + vlen + 1);
                memcpy(na, acc, al);
                if (al) na[al++] = SOH;
                memcpy(na + al, seg, klen); al += klen; na[al++] = SOH;
                memcpy(na + al, vstart, vlen); na[al + vlen] = '\0';
                acc = na;
            }
            if (!nx) break;
            seg = nx + 1;
        }
        *out = STRVAL(acc); return 1;
    }
    return 0;
}
#undef STX
#undef SOH
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_pl_ax_suffix(const char *s, int ar) {
    if (!s) return (const char *)0;
    if (ar == 2) {
        if (!strcmp(s, "+")) return "add"; if (!strcmp(s, "-")) return "sub"; if (!strcmp(s, "*")) return "mul";
        if (!strcmp(s, "/")) return "div"; if (!strcmp(s, "//")) return "idiv"; if (!strcmp(s, "div")) return "idiv";
        if (!strcmp(s, "mod")) return "mod"; if (!strcmp(s, "rem")) return "rem"; if (!strcmp(s, "**")) return "fpow"; if (!strcmp(s, "^")) return "pow";
        if (!strcmp(s, "min")) return "min"; if (!strcmp(s, "max")) return "max"; if (!strcmp(s, "gcd")) return "gcd"; if (!strcmp(s, "xor")) return "xor";
        if (!strcmp(s, ">>")) return "shr"; if (!strcmp(s, "<<")) return "shl"; if (!strcmp(s, "/\\")) return "band"; if (!strcmp(s, "\\/")) return "bor";
        return (const char *)0;
    }
    if (ar == 1) {
        if (!strcmp(s, "-")) return "neg"; if (!strcmp(s, "+")) return "pos"; if (!strcmp(s, "abs")) return "abs"; if (!strcmp(s, "sign")) return "sign";
        if (!strcmp(s, "truncate")) return "trunc"; if (!strcmp(s, "integer")) return "intg"; if (!strcmp(s, "float")) return "flt";
        if (!strcmp(s, "floor")) return "floor"; if (!strcmp(s, "ceiling")) return "ceil"; if (!strcmp(s, "round")) return "round";
        if (!strcmp(s, "sqrt")) return "sqrt"; if (!strcmp(s, "msb")) return "msb"; if (!strcmp(s, "\\")) return "bnot"; if (!strcmp(s, "sin")) return "sin"; if (!strcmp(s, "cos")) return "cos";
        if (!strcmp(s, "atan")) return "atan"; if (!strcmp(s, "log")) return "log"; if (!strcmp(s, "exp")) return "exp";
        if (!strcmp(s, "float_integer_part")) return "fip"; if (!strcmp(s, "float_fractional_part")) return "ffp";
        return (const char *)0;
    }
    if (ar == 0) { if (!strcmp(s, "pi")) return "pi"; return (const char *)0; }
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_pl_cmp_suffix(const char *s) {
    if (!s) return (const char *)0;
    if (!strcmp(s, "<")) return "lt"; if (!strcmp(s, ">")) return "gt"; if (!strcmp(s, "=<")) return "le";
    if (!strcmp(s, ">=")) return "ge"; if (!strcmp(s, "=:=")) return "eq"; if (!strcmp(s, "=\\=")) return "ne";
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_pl_det_builtin_target(const char *nm, int ar) {
    static const struct { const char *nm; int ar; const char *tgt; } tab[] = {
        { "sort", 2, "$sort" }, { "msort", 2, "$msort" }, { "keysort", 2, "$keysort" }, { "$bag_prep_b", 2, "$bag_prep_b" }, { "$bag_prep_s", 2, "$bag_prep_s" }, { "numbervars", 3, "$numbervars" }, { "numbervars", 1, "$numbervars" }, { "copy_term", 2, "$copy_term" }, { "acyclic_term", 1, "$acyclic_term" },
        { "group_pairs_by_key", 2, "$group_pairs_by_key" }, { "pairs_keys_values", 3, "$pairs_keys_values" },
        { "get_print_stream", 1, "$get_print_stream" }, { "name_singleton_vars", 1, "$name_singleton_vars" }, { "name_query_vars", 2, "$name_query_vars" }, { "bind_variables", 2, "$bind_variables" },
        { "char_type", 2, "$char_type" }, { "lower_upper", 2, "$lower_upper" }, { "writeq", 1, "$writeq" }, { "print", 1, "$print" }, { "write_canonical", 1, "$write_canonical" },
        { "functor", 3, "$functor" }, { "arg", 3, "$arg" }, { "=..", 2, "$univ" },
        { "compound", 1, "$tt_compound" }, { "callable", 1, "$tt_callable" }, { "ground", 1, "$tt_ground" }, { "is_list", 1, "$tt_is_list" },
        { "var", 1, "$tt_var" }, { "nonvar", 1, "$tt_nonvar" }, { "atom", 1, "$tt_atom" }, { "number", 1, "$tt_number" },
        { "integer", 1, "$tt_integer" }, { "float", 1, "$tt_float" }, { "atomic", 1, "$tt_atomic" },
        { "==", 2, "$atop_eq" }, { "\\==", 2, "$atop_ne" }, { "?=", 2, "$can_compare" },
        { "term_variables", 2, "$term_variables" }, { "term_variables", 3, "$term_variables" }, { "subsumes_term", 2, "$subsumes_term" },
        { "format", 1, "$format1" }, { "format", 2, "$format2" },
        { "write_term", 2, "$write_term" },
        { "atom_string", 2, "$aop_atom_string" }, { "number_string", 2, "$aop_number_string" }, { "atom_number", 2, "$aop_atom_number" },
        { "string_upper", 2, "$aop_string_upper" }, { "string_lower", 2, "$aop_string_lower" },
        { "string_concat", 3, "$aop_string_concat" }, { "string_length", 2, "$aop_string_length" }, { "string_to_atom", 2, "$aop_string_to_atom" },
        { "atomic_list_concat", 2, "$aop_atomic_list_concat" }, { "atomic_list_concat", 3, "$aop_atomic_list_concat" },
        { "concat_atom", 2, "$aop_concat_atom" }, { "concat_atom", 3, "$aop_concat_atom" },
        { "term_string", 2, "$term_string" }, { "term_to_atom", 2, "$term_string" },
        { "write_to_atom", 2, "$write_to_atom" }, { "format_to_atom", 3, "$format_to_atom" }, { "read_from_atom", 2, "$read_from_atom" },
        { "write_to_chars", 2, "$write_to_chars" }, { "write_to_codes", 2, "$write_to_codes" },
        { "writeq_to_atom", 2, "$writeq_to_atom" }, { "writeq_to_chars", 2, "$writeq_to_chars" }, { "writeq_to_codes", 2, "$writeq_to_codes" },
        { "write_canonical_to_atom", 2, "$write_canonical_to_atom" }, { "write_canonical_to_chars", 2, "$write_canonical_to_chars" }, { "write_canonical_to_codes", 2, "$write_canonical_to_codes" },
        { "display_to_atom", 2, "$display_to_atom" }, { "display_to_chars", 2, "$display_to_chars" }, { "display_to_codes", 2, "$display_to_codes" },
        { "print_to_atom", 2, "$print_to_atom" }, { "print_to_chars", 2, "$print_to_chars" }, { "print_to_codes", 2, "$print_to_codes" },
        { "write_term_to_atom", 3, "$write_term_to_atom" }, { "write_term_to_chars", 3, "$write_term_to_chars" }, { "write_term_to_codes", 3, "$write_term_to_codes" },
        { "format_to_chars", 3, "$format_to_chars" }, { "format_to_codes", 3, "$format_to_codes" },
        { "read_from_chars", 2, "$read_from_chars" }, { "read_from_codes", 2, "$read_from_codes" },
        { "read_term_from_atom", 3, "$read_term_from_atom" }, { "read_term_from_chars", 3, "$read_term_from_chars" }, { "read_term_from_codes", 3, "$read_term_from_codes" },
        { "with_output_to", 2, "$with_output_to" },
        { "$wot_begin", 0, "$wot_begin" }, { "$wot_end", 1, "$wot_end" }, { "$wot_abort", 0, "$wot_abort" },
        { "nb_setval", 2, "$nb_setval" }, { "nb_getval", 2, "$nb_getval" },
        { "assertz", 1, "$dyn_assertz" }, { "assert", 1, "$dyn_assertz" }, { "asserta", 1, "$dyn_asserta" }, { "retract", 1, "$retract" },
        { "compare", 3, "$compare" }, { "@<", 2, "$atop_lt" }, { "@=<", 2, "$atop_le" }, { "@>", 2, "$atop_gt" }, { "@>=", 2, "$atop_ge" },
        { "throw", 1, "$throw" },
        { "atom_to_term", 3, "$atom_to_term" }, { "char_code", 2, "$char_code" }, { "number_chars", 2, "$number_chars" }, { "number_codes", 2, "$number_codes" },
        { "read", 1, "$read" }, { "read", 2, "$read2" }, { "read_term", 2, "$read_term2" }, { "read_term", 3, "$read_term3" },
        { "succ", 2, "$succ" }, { "plus", 3, "$plus" }, { "atom_length", 2, "$atom_length" }, { "upcase_atom", 2, "$upcase_atom" }, { "downcase_atom", 2, "$downcase_atom" },
        { "atom_concat", 3, "$atom_concat" }, { "atom_chars", 2, "$atom_chars" }, { "atom_codes", 2, "$atom_codes" }, { "string_chars", 2, "$string_chars" }, { "string_codes", 2, "$string_codes" }, { "name", 2, "$name" }, { "write", 1, "$write" },
        { "put_char", 1, "$put_char" }, { "put_char", 2, "$put_char" }, { "tab", 1, "$tab" },
        { "at_end_of_stream", 0, "$at_end_of_stream" }, { "at_end_of_stream", 1, "$at_end_of_stream" },
        { "get_char", 1, "$get_char" }, { "get_char", 2, "$get_char" }, { "peek_char", 1, "$peek_char" }, { "peek_char", 2, "$peek_char" },
        { "get_code", 1, "$get_code" }, { "get_code", 2, "$get_code" }, { "peek_code", 1, "$peek_code" }, { "peek_code", 2, "$peek_code" },
        { "put_code", 1, "$put_code" }, { "put_code", 2, "$put_code" },
        { "get0", 1, "$get_code" }, { "put", 1, "$put_code" }, { "get", 1, "$get1" }, { "skip", 1, "$skip1" },
        { "number_atom", 2, "$number_atom" },
        { "get_byte", 1, "$get_byte" }, { "get_byte", 2, "$get_byte" }, { "peek_byte", 1, "$peek_byte" }, { "peek_byte", 2, "$peek_byte" },
        { "put_byte", 1, "$put_byte" }, { "put_byte", 2, "$put_byte" },
        { "unget_char", 1, "$unget_char" }, { "unget_char", 2, "$unget_char" }, { "unget_code", 1, "$unget_code" }, { "unget_code", 2, "$unget_code" },
        { "unget_byte", 1, "$unget_byte" }, { "unget_byte", 2, "$unget_byte" },
        { "op", 3, "$op" },
        { "set_prolog_flag", 2, "$set_prolog_flag" },
        { "current_output", 1, "$current_output" }, { "current_input", 1, "$current_input" },
        { "set_output", 1, "$set_output" }, { "set_input", 1, "$set_input" },
        { "flush_output", 1, "$flush_output1" },
        { "open", 3, "$open" }, { "open", 4, "$open" }, { "close", 1, "$close" }, { "close", 2, "$close" },
        { "writeq", 2, "$writeq2" }, { "write_canonical", 2, "$write_canonical2" }, { "write_term", 3, "$write_term3" }, { "format", 3, "$format3" },
        { "write", 2, "$write2" }, { "nl", 1, "$nl1" }, { "halt", 0, "$halt0" }, { "halt", 1, "$halt1" },
        { "display", 1, "$display" }, { "display", 2, "$display2" }, { "print", 2, "$print2" },
        { "see", 1, "$see" }, { "seeing", 1, "$seeing" }, { "seen", 0, "$seen" },
        { "tell", 1, "$tell" }, { "telling", 1, "$telling" }, { "told", 0, "$told" },
        { "append", 1, "$append" },
        { 0, 0, 0 } };
    if (!nm) return (const char *)0;
    for (int i = 0; tab[i].nm; i++) if (!strcmp(nm, tab[i].nm) && ar == tab[i].ar) return tab[i].tgt;
    return (const char *)0;
}
enum { PLCK_TRUE, PLCK_FAILK, PLCK_CUT, PLCK_CONJ, PLCK_DISJ, PLCK_ITE, PLCK_NAF, PLCK_META, PLCK_PRED, PLCK_DET };
typedef struct plc_slv { int kind; int phase; int mark; int op; struct plc_slv *a, *b, *c; DESCR_t *gcell; DESCR_t **av; int nav; const char *pi; const char *det; void *h; int *cut; } plc_slv_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static plc_slv_t *plc_new(int kind, int *cut) { plc_slv_t *s = (plc_slv_t *)rt_ws_alloc(sizeof *s); memset(s, 0, sizeof *s); s->kind = kind; s->cut = cut; return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t *plc_cell_persist(DESCR_t *v, DESCR_t *arr, int narr) {
    DESCR_t *c = plw_cell_deref(plw_entry(v));
    if (arr && c >= arr && c < arr + narr) { DESCR_t *cp = (DESCR_t *)rt_ws_alloc(sizeof(DESCR_t)); *cp = *c; return cp; }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_eval(DESCR_t *c, DESCR_t *out) {
    extern const char *prolog_atom_name(int);
    DESCR_t *d = plw_cell_deref(c);
    if (d->v == DT_I || d->v == DT_R) { *out = *d; return 1; }
    if (plw_unbound_tag(d)) { rt_pl_iso_throw_instantiation(); return 0; }
    if ((int)d->v == DT_A) {
        const char *nm = prolog_atom_name((int)d->i); const char *sf = nm ? rt_pl_ax_suffix(nm, 0) : (const char *)0;
        if (!sf) { plc_iso_evaluable(*d); return 0; }
        char nb[24]; snprintf(nb, sizeof nb, "$ax_%s", sf);
        DESCR_t o; if (!script_try_call_builtin_by_name(nb, (DESCR_t *)0, 0, &o) || o.v == DT_FAIL) return 0;
        *out = o; return 1;
    }
    if ((int)d->v == DT_PLREF) {
        int ar = (int)(d->slen & 0xFFFFu); const char *nm = prolog_atom_name((int)(d->slen >> 16));
        const char *sf = (nm && ar >= 1 && ar <= 2) ? rt_pl_ax_suffix(nm, ar) : (const char *)0;
        if (!sf) { plc_iso_evaluable(*d); return 0; }
        DESCR_t in[2]; DESCR_t *hh = (DESCR_t *)d->p;
        for (int i = 0; i < ar; i++) if (!plc_eval(&hh[i], &in[i])) return 0;
        char nb[24]; snprintf(nb, sizeof nb, "$ax_%s", sf);
        DESCR_t o; if (!script_try_call_builtin_by_name(nb, in, ar, &o) || o.v == DT_FAIL) return 0;
        *out = o; return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_det_exec(plc_slv_t *s) {
    switch (s->op) {
    case 'u': return plw_unify_cells(s->av[0], s->av[1]);
    case 'n': { int m = pl_trail_mark(&g_pl_trail); int r = plw_unify_cells(s->av[0], s->av[1]); pl_trail_unwind(&g_pl_trail, m); plw_zh_kill_to(m); return !r; }
    case 'i': { DESCR_t v; if (!plc_eval(s->av[1], &v)) return 0; DESCR_t *tv = (DESCR_t *)rt_ws_alloc(sizeof(DESCR_t)); *tv = v; return plw_unify_cells(s->av[0], tv); }
    case 'c': { DESCR_t a2, b2, o; if (!plc_eval(s->av[0], &a2) || !plc_eval(s->av[1], &b2)) return 0; char nb[24]; snprintf(nb, sizeof nb, "$cmp_%s", s->det); DESCR_t in[2]; in[0] = a2; in[1] = b2;
                if (!script_try_call_builtin_by_name(nb, in, 2, &o)) return 0; return o.v != DT_FAIL; }
    case 'l': { extern FILE *fh_cur_out_fp(void); fputc('\n', fh_cur_out_fp()); return 1; }
    case 'W': { extern FILE *fh_cur_out_fp(void); DESCR_t rv; rv.v = (DTYPE_t)DT_PLVAR; rv.slen = 0; rv.p = (void *)s->av[0]; DESCR_t o; script_try_call_builtin_by_name("$write", &rv, 1, &o); fputc('\n', fh_cur_out_fp()); return 1; }
    case 'd': { DESCR_t in[16]; int n = s->nav > 16 ? 16 : s->nav;
                for (int i = 0; i < n; i++) { in[i].v = (DTYPE_t)DT_PLVAR; in[i].slen = 0; in[i].p = (void *)s->av[i]; }
                DESCR_t o; if (!script_try_call_builtin_by_name(s->det, in, n, &o)) return 0; return o.v != DT_FAIL; }
    }
    return 0;
}
static plc_slv_t *plc_build(DESCR_t *gc, DESCR_t **xav, int nx, int *cut);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static plc_slv_t *plc_build_resolved(DESCR_t *d, DESCR_t **xav, int nx, int *cut) {
    extern const char *prolog_atom_name(int); extern int rt_proc_is_generator(const char *);
    const char *nm = (const char *)0; DESCR_t *heap = (DESCR_t *)0; int ar = 0;
    if ((int)d->v == DT_A) nm = prolog_atom_name((int)d->i);
    else if (d->v == DT_S) nm = d->s;
    else if ((int)d->v == DT_PLREF) { nm = prolog_atom_name((int)(d->slen >> 16)); heap = (DESCR_t *)d->p; ar = (int)(d->slen & 0xFFFFu); }
    if (!nm) { fprintf(stderr, "[PL] call: goal is not callable\n"); return plc_new(PLCK_FAILK, cut); }
    int n = ar + nx; if (n > 15) n = 15;
    if (n == 0) {
        if (!strcmp(nm, "true")) return plc_new(PLCK_TRUE, cut);
        if (!strcmp(nm, "fail") || !strcmp(nm, "false")) return plc_new(PLCK_FAILK, cut);
        if (!strcmp(nm, "!")) return plc_new(PLCK_CUT, cut);
        if (!strcmp(nm, "nl")) { plc_slv_t *s = plc_new(PLCK_DET, cut); s->op = 'l'; return s; }
    }
    if (heap && nx == 0) {
        if (!strcmp(nm, ",") && ar == 2) { plc_slv_t *s = plc_new(PLCK_CONJ, cut); s->a = plc_build(&heap[0], (DESCR_t **)0, 0, cut); s->b = plc_build(&heap[1], (DESCR_t **)0, 0, cut); return s; }
        if (!strcmp(nm, ";") && ar == 2) {
            DESCR_t *l = plw_cell_deref(&heap[0]);
            if ((int)l->v == DT_PLREF && (int)(l->slen & 0xFFFFu) == 2 && prolog_atom_name((int)(l->slen >> 16)) && !strcmp(prolog_atom_name((int)(l->slen >> 16)), "->")) {
                DESCR_t *lh = (DESCR_t *)l->p;
                plc_slv_t *s = plc_new(PLCK_ITE, cut); s->a = plc_build(&lh[0], (DESCR_t **)0, 0, cut); s->b = plc_build(&lh[1], (DESCR_t **)0, 0, cut); s->c = plc_build(&heap[1], (DESCR_t **)0, 0, cut); return s;
            }
            plc_slv_t *s = plc_new(PLCK_DISJ, cut); s->a = plc_build(&heap[0], (DESCR_t **)0, 0, cut); s->b = plc_build(&heap[1], (DESCR_t **)0, 0, cut); return s;
        }
        if (!strcmp(nm, "->") && ar == 2) { plc_slv_t *s = plc_new(PLCK_ITE, cut); s->a = plc_build(&heap[0], (DESCR_t **)0, 0, cut); s->b = plc_build(&heap[1], (DESCR_t **)0, 0, cut); s->c = plc_new(PLCK_FAILK, cut); return s; }
        if ((!strcmp(nm, "\\+") || !strcmp(nm, "not")) && ar == 1) { plc_slv_t *s = plc_new(PLCK_NAF, cut); s->a = plc_build(&heap[0], (DESCR_t **)0, 0, cut); return s; }
        if (!strcmp(nm, "once") && ar == 1) { plc_slv_t *s = plc_new(PLCK_ITE, cut); s->a = plc_build(&heap[0], (DESCR_t **)0, 0, cut); s->b = plc_new(PLCK_TRUE, cut); s->c = plc_new(PLCK_FAILK, cut); return s; }
        if (!strcmp(nm, "ignore") && ar == 1) { plc_slv_t *s = plc_new(PLCK_ITE, cut); s->a = plc_build(&heap[0], (DESCR_t **)0, 0, cut); s->b = plc_new(PLCK_TRUE, cut); s->c = plc_new(PLCK_TRUE, cut); return s; }
    }
    DESCR_t **argv = (DESCR_t **)rt_ws_alloc((size_t)(n > 0 ? n : 1) * sizeof(DESCR_t *));
    for (int i = 0; i < ar && i < n; i++) argv[i] = &heap[i];
    for (int j = 0; j < nx && ar + j < n; j++) argv[ar + j] = xav[j];
    if (!strcmp(nm, "call") && n >= 1) { plc_slv_t *s = plc_new(PLCK_META, cut); s->gcell = argv[0]; s->av = argv + 1; s->nav = n - 1; return s; }
    if (!strcmp(nm, "=") && n == 2) { plc_slv_t *s = plc_new(PLCK_DET, cut); s->op = 'u'; s->av = argv; s->nav = 2; return s; }
    if (!strcmp(nm, "\\=") && n == 2) { plc_slv_t *s = plc_new(PLCK_DET, cut); s->op = 'n'; s->av = argv; s->nav = 2; return s; }
    if (!strcmp(nm, "is") && n == 2) { plc_slv_t *s = plc_new(PLCK_DET, cut); s->op = 'i'; s->av = argv; s->nav = 2; return s; }
    { const char *cs = (n == 2) ? rt_pl_cmp_suffix(nm) : (const char *)0;
      if (cs) { plc_slv_t *s = plc_new(PLCK_DET, cut); s->op = 'c'; s->det = cs; s->av = argv; s->nav = 2; return s; } }
    if (!strcmp(nm, "writeln") && n == 1) { plc_slv_t *s = plc_new(PLCK_DET, cut); s->op = 'W'; s->av = argv; s->nav = 1; return s; }
    { const char *dt = rt_pl_det_builtin_target(nm, n);
      if (dt) { plc_slv_t *s = plc_new(PLCK_DET, cut); s->op = 'd'; s->det = dt; s->av = argv; s->nav = n; return s; } }
    { char pib[224]; if (n == 0 && !strcmp(nm, "main")) snprintf(pib, sizeof pib, "main"); else snprintf(pib, sizeof pib, "%s/%d", nm, n);
      if (rt_proc_is_generator(pib)) { plc_slv_t *s = plc_new(PLCK_PRED, cut); s->pi = strdup(pib); s->av = argv; s->nav = n; return s; } }
    rt_pl_iso_throw_pi("existence_error", "procedure", nm, n);
    return plc_new(PLCK_FAILK, cut);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static plc_slv_t *plc_build(DESCR_t *gc, DESCR_t **xav, int nx, int *cut) {
    DESCR_t *d = plw_cell_deref(gc);
    if (plw_unbound_tag(d)) { plc_slv_t *s = plc_new(PLCK_META, cut); s->gcell = gc; s->av = xav; s->nav = nx; return s; }
    return plc_build_resolved(d, xav, nx, cut);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_reset(plc_slv_t *s) {
    if (!s) return;
    s->phase = 0; s->h = (void *)0;
    if (s->kind == PLCK_META) { s->a = (plc_slv_t *)0; return; }
    plc_reset(s->a); plc_reset(s->b); plc_reset(s->c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_next(plc_slv_t *s) {
    extern int rt_pl_throw_pending(void);
    if (!s || rt_pl_throw_pending()) return 0;
    switch (s->kind) {
    case PLCK_TRUE: if (s->phase) return 0; s->phase = 1; return 1;
    case PLCK_FAILK: return 0;
    case PLCK_CUT: if (s->phase) return 0; s->phase = 1; *s->cut = 1; return 1;
    case PLCK_DET: if (s->phase) return 0; s->phase = 1; return plc_det_exec(s);
    case PLCK_CONJ:
        if (s->phase == 0) { if (!plc_next(s->a)) return 0; s->mark = pl_trail_mark(&g_pl_trail); s->phase = 1; }
        for (;;) {
            if (plc_next(s->b)) return 1;
            if (rt_pl_throw_pending() || *s->cut) return 0;
            pl_trail_unwind(&g_pl_trail, s->mark); plw_zh_kill_to(s->mark);
            plc_reset(s->b);
            if (!plc_next(s->a)) { s->phase = 0; return 0; }
            s->mark = pl_trail_mark(&g_pl_trail);
        }
    case PLCK_DISJ:
        if (s->phase == 0) { s->mark = pl_trail_mark(&g_pl_trail); s->phase = 1; }
        if (s->phase == 1) { if (plc_next(s->a)) return 1; if (rt_pl_throw_pending() || *s->cut) return 0; pl_trail_unwind(&g_pl_trail, s->mark); plw_zh_kill_to(s->mark); s->phase = 2; }
        if (s->phase == 2) { if (plc_next(s->b)) return 1; s->phase = 3; }
        return 0;
    case PLCK_ITE:
        if (s->phase == 0) {
            s->mark = pl_trail_mark(&g_pl_trail);
            if (plc_next(s->a)) s->phase = 1;
            else { if (rt_pl_throw_pending()) return 0; pl_trail_unwind(&g_pl_trail, s->mark); plw_zh_kill_to(s->mark); s->phase = 2; }
        }
        return plc_next(s->phase == 1 ? s->b : s->c);
    case PLCK_NAF:
        if (s->phase) return 0;
        s->phase = 1; s->mark = pl_trail_mark(&g_pl_trail);
        if (plc_next(s->a)) { pl_trail_unwind(&g_pl_trail, s->mark); plw_zh_kill_to(s->mark); return 0; }
        if (rt_pl_throw_pending()) return 0;
        return 1;
    case PLCK_META:
        if (s->phase == 0) {
            s->phase = 1;
            DESCR_t *d2 = plw_cell_deref(s->gcell);
            if (plw_unbound_tag(d2)) { fprintf(stderr, "[PL] call: unbound goal\n"); s->a = plc_new(PLCK_FAILK, s->cut); }
            else s->a = plc_build_resolved(d2, s->av, s->nav, s->cut);
        }
        return plc_next(s->a);
    case PLCK_PRED:
        if (s->phase == 0) {
            extern void rt_arg_stage(int, DESCR_t); extern DESCR_t rt_proc_call_gen_h(const char *, int, void **);
            s->phase = 1;
            for (int i = 0; i < s->nav; i++) { DESCR_t rv; rv.v = (DTYPE_t)DT_PLVAR; rv.slen = 0; rv.p = (void *)s->av[i]; rt_arg_stage(i, rv); }
            { DESCR_t r = rt_proc_call_gen_h(s->pi, s->nav, &s->h); return r.v != DT_FAIL; }
        }
        if (!s->h) return 0;
        { extern DESCR_t rt_proc_resume_frame_h(void **); DESCR_t r = rt_proc_resume_frame_h(&s->h); return r.v != DT_FAIL; }
    }
    return 0;
}
typedef struct { long long cur; long long hi; int mark; } plc_between_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_is_unbound(DESCR_t v) { return v.v == (DTYPE_t)DT_PLVAR || v.v == DT_SNUL || v.v == DT_FAIL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_int_check(DESCR_t v, long long *out) {
    if (plc_is_unbound(v)) { rt_pl_iso_throw_instantiation(); return 0; }
    if (v.v == (DTYPE_t)DT_I) { *out = (long long)v.i; return 1; }
    rt_pl_iso_throw_type("integer", v); return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_between_gen(DESCR_t *args, int nargs, int64_t *resume) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    if (nargs < 3) return FAILDESCR;
    if (*resume == 0) {
        long long lo = 0, hi = 0;
        if (!plc_int_check(rt_pl_deref_val(args[0]), &lo)) return FAILDESCR;
        if (!plc_int_check(rt_pl_deref_val(args[1]), &hi)) return FAILDESCR;
        DESCR_t x = rt_pl_deref_val(args[2]);
        if (!plc_is_unbound(x)) { long long i = 0; if (!plc_int_check(x, &i)) return FAILDESCR; if (i >= lo && i <= hi) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; *resume = -1; return r; } return FAILDESCR; }
        plc_between_t *it = (plc_between_t *)rt_ws_alloc(sizeof *it);
        it->cur = lo; it->hi = hi; it->mark = pl_trail_mark(&g_pl_trail);
        *resume = (int64_t)(intptr_t)it;
    }
    if (*resume == -1) return FAILDESCR;
    plc_between_t *it = (plc_between_t *)(intptr_t)*resume;
    while (it->cur <= it->hi) {
        long long i = it->cur++;
        pl_trail_unwind(&g_pl_trail, it->mark); plw_zh_kill_to(it->mark);
        DESCR_t iv; iv.v = (DTYPE_t)DT_I; iv.slen = 0; iv.i = i;
        if (plw_unify_vals(args[2], iv)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
    }
    pl_trail_unwind(&g_pl_trail, it->mark); plw_zh_kill_to(it->mark);
    return FAILDESCR;
}
typedef struct { const char *s; int len; int b; int l; int mark; } plc_subatom_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_sub_atom_gen(DESCR_t *args, int nargs, int64_t *resume) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    if (nargs < 5) return FAILDESCR;
    if (*resume == 0) {
        DESCR_t a0 = rt_pl_deref_val(args[0]); const char *str = pl_atom_str(a0);
        if (!str) { if (a0.v == (DTYPE_t)DT_PLVAR || a0.v == DT_SNUL || a0.v == DT_FAIL) rt_pl_iso_throw_instantiation(); return FAILDESCR; }
        plc_subatom_t *it = (plc_subatom_t *)rt_ws_alloc(sizeof *it);
        it->s = str; it->len = (int)strlen(str); it->b = 0; it->l = 0; it->mark = pl_trail_mark(&g_pl_trail);
        *resume = (int64_t)(intptr_t)it;
    }
    plc_subatom_t *it = (plc_subatom_t *)(intptr_t)*resume;
    for (; it->b <= it->len; it->b++, it->l = 0) {
        for (; it->l <= it->len - it->b; ) {
            int b = it->b, l = it->l; it->l++;
            pl_trail_unwind(&g_pl_trail, it->mark); plw_zh_kill_to(it->mark);
            DESCR_t bv; bv.v = (DTYPE_t)DT_I; bv.slen = 0; bv.i = b;
            DESCR_t lv; lv.v = (DTYPE_t)DT_I; lv.slen = 0; lv.i = l;
            DESCR_t av; av.v = (DTYPE_t)DT_I; av.slen = 0; av.i = it->len - b - l;
            char *sub = (char *)rt_ws_alloc((size_t)l + 1); memcpy(sub, it->s + b, (size_t)l); sub[l] = 0;
            DESCR_t sv = pl_mk_atom(sub);
            if (plw_unify_vals(args[1], bv) && plw_unify_vals(args[2], lv) && plw_unify_vals(args[3], av) && plw_unify_vals(args[4], sv)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
        }
    }
    pl_trail_unwind(&g_pl_trail, it->mark); plw_zh_kill_to(it->mark);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_rd_skip(const char *p) { while (*p == ' ' || *p == '\t' || *p == '\n') p++; return p; }
static const char *plc_rd_term(const char *p, DESCR_t *out, DESCR_t *binds, char (*bnames)[64], int *nb, int cap);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_rd_args(const char *p, DESCR_t *kids, int *nk, int cap, char close, DESCR_t *binds, char (*bnames)[64], int *nb, int bcap) {
    p = plc_rd_skip(p);
    if (*p == close) return p + 1;
    for (;;) {
        if (*nk >= cap) return (const char *)0;
        p = plc_rd_term(p, &kids[(*nk)++], binds, bnames, nb, bcap);
        if (!p) return (const char *)0;
        p = plc_rd_skip(p);
        if (*p == ',') { p++; continue; }
        if (*p == close) return p + 1;
        return (const char *)0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_rd_term(const char *p, DESCR_t *out, DESCR_t *binds, char (*bnames)[64], int *nb, int cap) {
    p = plc_rd_skip(p);
    if (*p == '-' || (*p >= '0' && *p <= '9')) {
        char *e = (char *)0; long long v = strtoll(p, &e, 10);
        if (e && *e == '.' && e[1] >= '0' && e[1] <= '9') { char *e2 = (char *)0; double d = strtod(p, &e2); DESCR_t c; c.v = DT_R; c.slen = 0; c.r = d; *out = c; return e2; }
        if (e == p) return (const char *)0;
        { DESCR_t c; c.v = (DTYPE_t)DT_I; c.slen = 0; c.i = v; *out = c; return e; }
    }
    if (*p == '[') {
        DESCR_t kids[32]; int nk = 0;
        p = plc_rd_args(p + 1, kids, &nk, 32, ']', binds, bnames, nb, cap);
        if (!p) return (const char *)0;
        DESCR_t lst = pl_mk_atom("[]");
        for (int i = nk - 1; i >= 0; i--) { DESCR_t two[2]; two[0] = kids[i]; two[1] = lst; lst = plc_iso_comp(".", 2, two); }
        *out = lst; return p;
    }
    if (*p == '\'') {
        const char *q = p + 1; char buf[256]; int bl = 0;
        while (*q && *q != '\'' && bl < 255) buf[bl++] = *q++;
        if (*q != '\'') return (const char *)0;
        buf[bl] = 0; q++;
        q = plc_rd_skip(q);
        if (*q == '(') { DESCR_t kids[32]; int nk = 0; q = plc_rd_args(q + 1, kids, &nk, 32, ')', binds, bnames, nb, cap); if (!q) return (const char *)0; *out = plc_iso_comp(buf, nk, kids); return q; }
        { char *cp = (char *)rt_ws_alloc((size_t)bl + 1); memcpy(cp, buf, (size_t)bl + 1); *out = pl_mk_atom(cp); } return q;
    }
    if ((*p >= 'A' && *p <= 'Z') || *p == '_') {
        char nm[64]; int nl = 0;
        while ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_') { if (nl < 63) nm[nl++] = *p; p++; }
        nm[nl] = 0;
        if (strcmp(nm, "_")) for (int i = 0; i < *nb; i++) if (!strcmp(bnames[i], nm)) { *out = binds[i]; return p; }
        { DESCR_t v = plc_iso_fresh(); if (*nb < cap && strcmp(nm, "_")) { snprintf(bnames[*nb], 64, "%s", nm); binds[*nb] = v; (*nb)++; } *out = v; return p; }
    }
    if (*p >= 'a' && *p <= 'z') {
        char nm[128]; int nl = 0;
        while ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_') { if (nl < 127) nm[nl++] = *p; p++; }
        nm[nl] = 0;
        p = plc_rd_skip(p);
        if (*p == '(') { DESCR_t kids[32]; int nk = 0; p = plc_rd_args(p + 1, kids, &nk, 32, ')', binds, bnames, nb, cap); if (!p) return (const char *)0; *out = plc_iso_comp(nm, nk, kids); return p; }
        { char *cp = (char *)rt_ws_alloc((size_t)nl + 1); memcpy(cp, nm, (size_t)nl + 1); *out = pl_mk_atom(cp); } return p;
    }
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *plc_rd_entry(const char *txt, DESCR_t *out, DESCR_t *binds, char (*bnames)[64], int *nb, int cap) { return plc_rd_term(txt, out, binds, bnames, nb, cap); }
typedef struct { plc_slv_t *root; int mark; int cut; } plc_top_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_call_gen(DESCR_t *args, int nargs, int64_t *resume) {
    { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); }
    if (*resume == 0) {
        plc_top_t *tp = (plc_top_t *)rt_ws_alloc(sizeof *tp);
        tp->mark = pl_trail_mark(&g_pl_trail); tp->cut = 0;
        int nx = nargs - 1; if (nx < 0) nx = 0;
        DESCR_t **xav = (DESCR_t **)rt_ws_alloc((size_t)(nx > 0 ? nx : 1) * sizeof(DESCR_t *));
        for (int j = 0; j < nx; j++) xav[j] = plc_cell_persist(&args[1 + j], args, nargs);
        tp->root = plc_build(plc_cell_persist(&args[0], args, nargs), xav, nx, &tp->cut);
        *resume = (int64_t)(intptr_t)tp;
    }
    plc_top_t *tp = (plc_top_t *)(intptr_t)*resume;
    if (plc_next(tp->root)) { DESCR_t r; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
    pl_trail_unwind(&g_pl_trail, tp->mark); plw_zh_kill_to(tp->mark);
    return FAILDESCR;
}
static DESCR_t rt_call_arr_impl(const char *fn, DESCR_t *args, int nargs, int bidlen);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_arr(const char *fn, DESCR_t *args, int nargs) { return rt_call_arr_bl(fn, args, nargs, -1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ bidlen is the emitter's pre-resolved (strlen(fn) << 16) | bid_of(fn,len); NEGATIVE means "resolve it yourself" -- see the note on try_call_builtin_by_name_bl. */
DESCR_t rt_call_arr_bl(const char *fn, DESCR_t *args, int nargs, int bidlen) {
    extern jmp_buf g_core_errjmp_stk[64]; extern int g_core_errjmp_n;
    extern char *g_plw_unwind_floor;
    { static long _rspc = -1; if (_rspc == -1) { const char *ev = getenv("SCRIP_CALLARR_TRACE"); _rspc = (ev && *ev && *ev != '0') ? 0 : -2; } if (_rspc >= 0) { void *rsp_now; __asm__ volatile ("mov %%rsp, %0" : "=r"(rsp_now)); _rspc++; fprintf(stderr, "[RSP] %ld fn='%s' rsp=%p\n", _rspc, fn ? fn : "(null)", rsp_now); fflush(stderr); } }
    char *fl = g_plw_unwind_floor;
    g_plw_unwind_floor = (char *)__builtin_frame_address(0);
    if (g_core_errjmp_n >= 64) { DESCR_t r0 = rt_call_arr_impl(fn, args, nargs, bidlen); g_plw_unwind_floor = fl; return r0; }
    int my = g_core_errjmp_n;
    if (setjmp(g_core_errjmp_stk[my])) { g_core_errjmp_n = my; g_plw_unwind_floor = fl; return FAILDESCR; }
    g_core_errjmp_n = my + 1;
    DESCR_t r = rt_call_arr_impl(fn, args, nargs, bidlen);
    g_core_errjmp_n = my;
    g_plw_unwind_floor = fl;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rt_call_arr_impl(const char *fn, DESCR_t *args, int nargs, int bidlen) {
    DESCR_t out = FAILDESCR;
    extern void rt_gc_point_arr(DESCR_t *arr, int n, const char **r0);
    extern int g_gc_pending;
    { static long _cac = -1; if (_cac == -1) { const char *ev = getenv("SCRIP_CALLARR_TRACE"); _cac = (ev && *ev && *ev != '0') ? 0 : -2; } if (_cac >= 0) { extern int g_core_errjmp_n; _cac++; fprintf(stderr, "[CAC] %ld fn='%s' nargs=%d errjmp_n=%d\n", _cac, fn ? fn : "(null)", nargs, g_core_errjmp_n); fflush(stderr); } }
    /* ⭐ INLINE-CHEAP-CHECK (perf-dispatch-gc-safepoint-necessity): rt_gc_point_arr's asm veneer pays an UNCONDITIONAL push/pop of all 6 callee-saved
       registers before rt_gc_point_arr_c ever reads g_gc_pending -- the overwhelming majority of calls (no collection due) pay that cost for nothing.
       PRECEDENT: this exact shape is already landed and shipping at a sibling by-name-dispatch call site (rtx_plunify.s rt_pl_dop_unify, "absorbed
       rt_gc_point_arr" comment) -- check g_gc_pending BEFORE paying for the veneer, call the REAL, UNCHANGED veneer only when a collection is due.
       The cold (collecting) path below is byte-identical to today's unconditional call, so correctness when a collection actually runs is untouched
       (same register-parking, same g_gc_seam_sp/g_gc_shield_arr); only the zero-cost-today fast path changes. Matches g_gc_pending ONLY, same as the
       precedent -- not rt_gc_point_arr_c's second (heap-gcline) condition, deferring that trigger to whichever later safepoint next sees it, same
       tradeoff the precedent already ships with. Killswitch SCRIP_DISPATCH_GC_INLINE=0 restores the unconditional call on the same binary. */
    { static int _gcik = -1; if (_gcik == -1) { const char *ev = getenv("SCRIP_DISPATCH_GC_INLINE"); _gcik = (ev && *ev == '0') ? 0 : 1; }
      if (!_gcik || g_gc_pending) rt_gc_point_arr(args, nargs, (const char **)0); }
    if (!fn) return out;
    /* ⭐ FIRST-TWO-CHARACTER GUARD, the s260 FAIL-strcmp cure's shape extended (perf-by-name-builtin-dispatch, s271).
       MEASURED, string_manip.sno -O0 N=20000, callgrind: the single-char fn[0]=='S' guard still let EVERY ordinary
       'S'-leading builtin (SIZE, SPAN, SORT, STRING, SUBSTR, SET, SUCCEED, ...) fall through into a real
       __strcmp_avx2 PLT hop -- 21,000 SIZE calls alone cost 651,000 Ir (1.27% of the kernel) proving a name that
       cannot possibly match past the second byte.  strcmp(fn,"SNO$NOFAIL")==0 IMPLIES fn[1]=='N' (every internal
       SNO$xxx name shares that prefix, but no ordinary builtin does), so adding the second-character test is a
       strictly-narrower pre-filter -- it can only skip strcmp calls that were already guaranteed to fail, never
       change the outcome for SNO$NOFAIL itself or any SNO$xxx sibling (those still fall through to strcmp). */
    if (fn[0] == 'S' && fn[1] == 'N' && !strcmp(fn, "SNO$NOFAIL")) { extern void rt_nofail_abort(void); rt_nofail_abort(); return out; }
    if (fn[0] == '$' && fn[1]) { if (script_try_call_builtin_by_name(fn, args, nargs, &out)) return out; out = FAILDESCR; }
    if (fn[0] && !((fn[0] >= 'a' && fn[0] <= 'z') || (fn[0] >= 'A' && fn[0] <= 'Z') || fn[0] == '_' || fn[0] == '&')) {
        extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
        if (nargs == 1) {
            extern DESCR_t rt_random_var(DESCR_t); extern DESCR_t rt_deref(DESCR_t);
            DESCR_t a = args[0];
            if (!strcmp(fn, "-"))  return rt_num_arith(INTVAL(0), a, BINOP_SUB);
            if (!strcmp(fn, "+"))  return rt_num_arith(INTVAL(0), a, BINOP_ADD);
            if (!strcmp(fn, "*"))  { extern DESCR_t rt_call_arr(const char *, DESCR_t *, int); DESCR_t _a = a; return try_call_builtin_by_name("*", &_a, 1, &out) ? out : FAILDESCR; }
            if (!strcmp(fn, "/"))  return (a.v == DT_SNUL || a.v == 0) ? a : FAILDESCR;
            if (!strcmp(fn, "\\")) return (a.v == DT_SNUL || a.v == 0) ? FAILDESCR : a;
            if (!strcmp(fn, "?"))  return rt_deref(rt_random_var(a));
        }
        DESCR_t a = (nargs > 0) ? args[0] : NULVCL, b = (nargs > 1) ? args[1] : NULVCL;
        if (!strcmp(fn, "[]")) { extern DESCR_t rt_subscript_var(DESCR_t, DESCR_t); extern DESCR_t rt_deref(DESCR_t); DESCR_t v = rt_subscript_var(a, b); if (IS_FAIL_fn(v)) return FAILDESCR; return rt_deref(v); }
        if (!strcmp(fn, "++")) return rt_num_arith(a, b, BINOP_CUNION);
        if (!strcmp(fn, "--")) return rt_num_arith(a, b, BINOP_CDIFF);
        if (!strcmp(fn, "**")) return rt_num_arith(a, b, BINOP_CINTER);
        if (!strcmp(fn, "+")) return rt_num_arith(a, b, BINOP_ADD);
        if (!strcmp(fn, "-")) return rt_num_arith(a, b, BINOP_SUB);
        if (!strcmp(fn, "*")) return rt_num_arith(a, b, BINOP_MUL);
        if (!strcmp(fn, "/")) return rt_num_arith(a, b, BINOP_DIV);
        if (!strcmp(fn, "%")) return rt_num_arith(a, b, BINOP_MOD);
        if (!strcmp(fn, "^")) return rt_num_arith(a, b, BINOP_POW);
        if (!strcmp(fn, "||")) { const char *x = VARVAL_fn(a), *y = VARVAL_fn(b); if (!x) x = ""; if (!y) y = ""; size_t lx = strlen(x), ly = strlen(y); char *o = rt_str_alloc((int)(lx + ly)); memcpy(o, x, lx); memcpy(o + lx, y, ly); o[lx + ly] = 0; return STRVAL(o); }
        { DESCR_t rt_str_coerce(DESCR_t); int oc = -1;
          if      (!strcmp(fn, "="))    oc = BINOP_EQ;  else if (!strcmp(fn, "~="))   oc = BINOP_NE;
          else if (!strcmp(fn, "<"))    oc = BINOP_LT;  else if (!strcmp(fn, "<="))   oc = BINOP_LE;
          else if (!strcmp(fn, ">"))    oc = BINOP_GT;  else if (!strcmp(fn, ">="))   oc = BINOP_GE;
          else if (!strcmp(fn, "=="))   oc = BINOP_SEQ; else if (!strcmp(fn, "~=="))  oc = BINOP_SNE;
          else if (!strcmp(fn, "<<"))   oc = BINOP_SLT; else if (!strcmp(fn, "<<="))  oc = BINOP_SLE;
          else if (!strcmp(fn, ">>"))   oc = BINOP_SGT; else if (!strcmp(fn, ">>="))  oc = BINOP_SGE;
          else if (!strcmp(fn, "==="))  oc = BINOP_EQV; else if (!strcmp(fn, "~===")) oc = BINOP_NEQV;
          if (oc >= 0) return rt_jct_relop(a, b, oc) ? ((oc >= BINOP_SLT && oc <= BINOP_SNE) ? rt_str_coerce(b) : b) : FAILDESCR; }
    }
    if (try_call_builtin_by_name_bl(fn, args, nargs, &out, bidlen)) return out;
    out = APPLY_fn(fn, args, nargs);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_arr_gen(const char *fn, DESCR_t *args, int nargs, int64_t *resume) {
    DESCR_t out = FAILDESCR;
    if (fn && resume && fn[1] == 'b' && !strcmp(fn, "$between") && nargs >= 3) return rt_pl_between_gen(args, nargs, resume);
    if (fn && resume && !strcmp(fn, "$dyn_iter") && nargs >= 1) {
        extern DESCR_t rt_pl_dyn_iter_gen(DESCR_t *, int, int64_t *);
        { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); }
        return rt_pl_dyn_iter_gen(args, nargs, resume);
    }
    if (fn && resume && !strcmp(fn, "$call") && nargs >= 1) return rt_pl_call_gen(args, nargs, resume);
    if (fn && resume && !strcmp(fn, "$clause") && nargs >= 2) { extern DESCR_t rt_pl_clause_gen(DESCR_t *, int, int64_t *); { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); } return rt_pl_clause_gen(args, nargs, resume); }
    if (fn && resume && !strcmp(fn, "$current_predicate") && nargs >= 1) { extern DESCR_t rt_pl_current_predicate_gen(DESCR_t *, int, int64_t *); { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); } return rt_pl_current_predicate_gen(args, nargs, resume); }
    if (fn && resume && !strcmp(fn, "$predicate_property") && nargs >= 2) { extern DESCR_t rt_pl_predicate_property_gen(DESCR_t *, int, int64_t *); { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); } return rt_pl_predicate_property_gen(args, nargs, resume); }
    if (fn && resume && !strcmp(fn, "$current_op") && nargs >= 3) { extern DESCR_t rt_pl_current_op_gen(DESCR_t *, int, int64_t *); { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); } return rt_pl_current_op_gen(args, nargs, resume); }
    if (fn && resume && !strcmp(fn, "$current_prolog_flag") && nargs >= 2) { extern DESCR_t rt_pl_current_prolog_flag_gen(DESCR_t *, int, int64_t *); { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); } return rt_pl_current_prolog_flag_gen(args, nargs, resume); }
    if (fn && resume && !strcmp(fn, "$current_stream") && nargs >= 1) { extern DESCR_t rt_pl_current_stream_gen(DESCR_t *, int, int64_t *); { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); } return rt_pl_current_stream_gen(args, nargs, resume); }
    if (fn && resume && !strcmp(fn, "$stream_property") && nargs >= 2) { extern DESCR_t rt_pl_stream_property_gen(DESCR_t *, int, int64_t *); { extern int ATOM_DOT; extern void prolog_atom_init(void); if (ATOM_DOT <= 0) prolog_atom_init(); } return rt_pl_stream_property_gen(args, nargs, resume); }
    if (fn && resume && !strcmp(fn, "$sub_atom") && nargs >= 5) return rt_pl_sub_atom_gen(args, nargs, resume);
    if (fn && resume && !strcmp(fn, "$between") && nargs >= 3) return rt_pl_between_gen(args, nargs, resume);
    if (fn && resume && !strcmp(fn, "$for") && nargs >= 3) { DESCR_t a3[3]; a3[0] = args[1]; a3[1] = args[2]; a3[2] = args[0]; return rt_pl_between_gen(a3, 3, resume); }
    if (fn && resume && !strcmp(fn, "$bag_group") && nargs >= 3) { extern DESCR_t rt_pl_bag_group_gen(DESCR_t *, int, int64_t *); return rt_pl_bag_group_gen(args, nargs, resume); }
    if (fn && resume && nargs >= 2 && nargs <= 4 && (!strcmp(fn, "find") || !strcmp(fn, "upto"))) {
        DESCR_t a4[4]; a4[0] = args[0]; a4[1] = args[1];
        long i1 = (nargs >= 3 && (IS_INT_fn(args[2]) || IS_REAL_fn(args[2]))) ? (long)to_int(args[2]) : 1;
        if (*resume > 0 && (long)*resume > i1) i1 = (long)*resume;
        a4[2] = INTVAL(i1);
        if (nargs >= 4) a4[3] = args[3];
        if (try_call_builtin_by_name(fn, a4, (nargs >= 4) ? 4 : 3, &out) && !IS_FAIL_fn(out)) { *resume = (int)out.i + 1; return out; }
        return FAILDESCR;
    }
    return rt_call_arr(fn, args, nargs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_make_list(DESCR_t *args, int nargs) {
    static int list_reg3 = 0;
    if (!list_reg3) { DEFDAT_fn("list(frame_elems,frame_size,gen_type,frame_cap)"); list_reg3 = 1; }
    DESCR_t *elems = rt_ws_alloc((nargs>0?nargs:1)*sizeof(DESCR_t));
    for (int _j=0;_j<nargs;_j++) elems[_j]=args[_j];
    DESCR_t eptr; eptr.v=DT_DATA; eptr.slen=0; eptr.ptr=(void*)elems;
    return DATCON_fn("list", eptr, INTVAL(nargs), STRVAL("list"), INTVAL(nargs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_args_list_from(char **v, int n) {
    if (n < 0 || !v) n = 0;
    DESCR_t *tmp = rt_ws_alloc((n>0?n:1)*sizeof(DESCR_t));
    for (int _i=0;_i<n;_i++) tmp[_i] = STRVAL(v[_i]);
    return rt_make_list(tmp, n);
}
static DESCR_t g_main_args_descr;
static int g_main_args_staged = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_main_args_stage(char **v, int n) { extern DESCR_t g_call_args[]; g_main_args_descr = rt_args_list_from(v, n); g_main_args_staged = 1; if (!getenv("SCRIP_NO_MAIN_ARGS")) g_call_args[0] = g_main_args_descr; }
DESCR_t rt_main_args_fetch(void) { if (!g_main_args_staged) rt_main_args_stage((char **)0, 0); return g_main_args_descr; }
extern int junction_is(DESCR_t v);
extern int junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int relop_num_coerce(DESCR_t v, DESCR_t *out) {
    if (IS_INT_fn(v) || IS_REAL_fn(v)) { *out = v; return 1; }
    const char *s = IS_STR_fn(v) ? v.s : IS_CSET_fn(v) ? v.s : (const char *)0;
    if (!s) return 0;
    const char *t = s; while (*t == ' ') t++; if (!*t) return 0;
    char *endi = 0, *endd = 0; long long iv = strtoll(t, &endi, 10); double dv = strtod(t, &endd);
    const char *e = (endd > endi) ? endd : endi; if (e == t) return 0;
    while (*e == ' ') e++; if (*e != '\0') return 0;
    *out = (endd > endi) ? REALVAL(dv) : INTVAL((int64_t)iv); return 1;
}
DESCR_t rt_str_coerce(DESCR_t d);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t c_rt_str_coerce(DESCR_t d) {
    if (!IS_CSET_fn(d)) return d;
    const char *cp; int cl; if (!cset_resolve(d, &cp, &cl) || cl < 0) return d;
    char *b = rt_ws_alloc((size_t)cl + 1); memcpy(b, cp, (size_t)cl); b[cl] = 0;
    for (int i = 1; i < cl; i++) { char t = b[i]; int j = i - 1; while (j >= 0 && (unsigned char)b[j] > (unsigned char)t) { b[j+1] = b[j]; j--; } b[j+1] = t; }
    return STRVAL(b);
}
static int rt_jct_relop_impl(DESCR_t lhs, DESCR_t rhs, int op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int c_rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op) {
    extern jmp_buf g_core_errjmp_stk[64]; extern int g_core_errjmp_n;
    if (g_core_errjmp_n >= 64) return rt_jct_relop_impl(lhs, rhs, op);
    int my = g_core_errjmp_n;
    if (setjmp(g_core_errjmp_stk[my])) { g_core_errjmp_n = my; return 0; }
    g_core_errjmp_n = my + 1;
    int r = rt_jct_relop_impl(lhs, rhs, op);
    g_core_errjmp_n = my;
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_jct_relop_impl(DESCR_t lhs, DESCR_t rhs, int op) {
    if (op == BINOP_EQV || op == BINOP_NEQV) {
        int eq = 0;
        int lcs = (lhs.v == DT_S && lhs.slen == 0xFFFFFFFFu), rcs = (rhs.v == DT_S && rhs.slen == 0xFFFFFFFFu);
        if (lhs.v == rhs.v && lhs.i == rhs.i) eq = 1;
        else if (lcs || rcs) {
            if (lcs && rcs) { const char *ca, *cb; int la, lb; eq = cset_resolve(lhs, &ca, &la) && cset_resolve(rhs, &cb, &lb) && la == lb && memcmp(ca, cb, (size_t)la) == 0; }
        }
        else if ((lhs.v == DT_S || lhs.v == DT_SNUL) && (rhs.v == DT_S || rhs.v == DT_SNUL)) {
            const char *a = lhs.s ? lhs.s : ""; const char *b = rhs.s ? rhs.s : "";
            long la = lhs.slen ? (long)lhs.slen : (long)strlen(a); long lb = rhs.slen ? (long)rhs.slen : (long)strlen(b);
            eq = (la == lb) && memcmp(a, b, (size_t)la) == 0;
        }
        else if (lhs.v == rhs.v && lhs.v == DT_I) eq = (lhs.i == rhs.i);
        else if (lhs.v == rhs.v && lhs.v == DT_R) eq = (lhs.r == rhs.r);
        return (op == BINOP_EQV) ? eq : !eq;
    }
    int lj = junction_is(lhs), rj = junction_is(rhs);
    int num_rel = (op == BINOP_EQ || op == BINOP_NE || op == BINOP_LT || op == BINOP_LE || op == BINOP_GT || op == BINOP_GE);
    int str_rel = (op == BINOP_SEQ || op == BINOP_SNE || op == BINOP_SLT || op == BINOP_SLE || op == BINOP_SGT || op == BINOP_SGE);
    if ((lj || rj) && (num_rel || str_rel)) {
        DESCR_t jct    = lj ? lhs : rhs;
        DESCR_t scalar = lj ? rhs : lhs;
        int tt_op = (op == BINOP_EQ || op == BINOP_SEQ) ? TT_EQ : (op == BINOP_NE || op == BINOP_SNE) ? TT_NE :
                    (op == BINOP_LT || op == BINOP_SLT) ? TT_LT : (op == BINOP_LE || op == BINOP_SLE) ? TT_LE :
                    (op == BINOP_GT || op == BINOP_SGT) ? TT_GT : TT_GE;
        int numeric = str_rel ? 0 : (IS_INT_fn(scalar) || IS_REAL_fn(scalar));
        return junction_collapse(scalar, jct, tt_op, numeric) ? 1 : 0;
    }
    if (num_rel) { DESCR_t L, R;
        if (relop_num_coerce(lhs, &L) && relop_num_coerce(rhs, &R)) {
            if (IS_REAL_fn(L) || IS_REAL_fn(R)) { double a = to_real(L), b = to_real(R);
                switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b;
                              case BINOP_LE: return a<=b; case BINOP_GT: return a>b;  case BINOP_GE: return a>=b; } return 0; }
            int64_t a = L.i, b = R.i;
            switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b;
                          case BINOP_LE: return a<=b; case BINOP_GT: return a>b;  case BINOP_GE: return a>=b; } return 0; } }
    if (num_rel && (IS_REAL_fn(lhs) || IS_REAL_fn(rhs)) && (IS_INT_fn(lhs) || IS_REAL_fn(lhs)) && (IS_INT_fn(rhs) || IS_REAL_fn(rhs))) {
        double a = to_real(lhs), b = to_real(rhs);
        switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b;
                      case BINOP_LE: return a<=b; case BINOP_GT: return a>b;  case BINOP_GE: return a>=b; }
        return 0;
    }
    if (num_rel && IS_INT_fn(lhs) && IS_INT_fn(rhs)) {
        int64_t a = lhs.i, b = rhs.i;
        switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b;
                      case BINOP_LE: return a<=b; case BINOP_GT: return a>b;  case BINOP_GE: return a>=b; }
        return 0;
    }
    { const char *a = VARVAL_fn(lhs), *b = VARVAL_fn(rhs); if (!a) a=""; if (!b) b="";
      int c = strcmp(a, b);
      switch (op) { case BINOP_EQ: case BINOP_SEQ: return c==0; case BINOP_NE: case BINOP_SNE: return c!=0;
                    case BINOP_LT: case BINOP_SLT: return c<0;  case BINOP_LE: case BINOP_SLE: return c<=0;
                    case BINOP_GT: case BINOP_SGT: return c>0;  case BINOP_GE: case BINOP_SGE: return c>=0; }
      return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_real_str(double r, char *buf, int bufsz, int prec) {
    if (prec < 1) prec = 1; if (prec > 16) prec = 16;
    char tmp[64]; snprintf(tmp, sizeof tmp, "%.*E", prec, r);
    char *ep = strchr(tmp, 'E');
    if (!ep) { snprintf(buf, bufsz, "%s", tmp); return; }
    char sign = ep[1]; const char *digits = ep + 2; int ndig = (int)strlen(digits);
    char mant[48]; int ml = (int)(ep - tmp); if (ml >= 48) ml = 47; memcpy(mant, tmp, ml); mant[ml] = '\0';
    if (ndig < 3) snprintf(buf, bufsz, "%sE%c%0*d", mant, sign, 3, atoi(digits));
    else snprintf(buf, bufsz, "%s", tmp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t proc_as_value(const char *name) {
    if (!name || name[0] == '&') return FAILDESCR;
    for (int i = 0; i < g_stage2.proc_count; i++) {
        if (g_stage2.proc_table[i].name && strcmp(g_stage2.proc_table[i].name, name) == 0) {
            DESCR_t pv; pv.v = DT_E;
            pv.slen = (uint32_t)i;
            pv.i    = g_stage2.proc_table[i].entry_pc;
            return pv;
        }
    }
    static const char *builtins[] = {
        "__pas_writeln","__pas_write","__pas_chr","__pas_chrlit","__pas_enum_name","__pas_read_i","__pas_read_c","__pas_readln","__pas_eof","__pas_eoln","__pas_trunc","__pas_abs","__pas_sin",
        "__pas_read_i_f","__pas_read_c_f","__pas_readln_f","__pas_eof_f","__pas_eoln_f","__pas_getbufch","__pas_getbufch_f",
        "__pas_ca_pack","__pas_ca_unpack",
        "__pas_cos","__pas_exp","__pas_sqrt","__pas_ln","__pas_arctan","__pas_fassign","__pas_rewrite","__pas_reset","__pas_fclose","write","writes","read","reads","close","open","remove",
        "flush",
        "put","get","pull","push","pop","list","image","proc","type","copy",
        "string","integer","real","numeric","ord","char","reverse","sort","sortf",
        "find","match","many","any","upto","bal","move","tab","pos",
        "map","repl","trim","left","right","center","detab","entab",
        "abs","sqrt","sin","cos","tan","asin","acos","atan","exp","log",
        "dtor","rtod",
        "iand","ior","ixor","ishift","icom",
        "table","key","insert","delete","member","args","level",
        "collect","stop","exit","runerr","name","variable","seq",
        NULL
    };
    for (int i = 0; builtins[i]; i++) if (strcmp(builtins[i], name) == 0) return STRVAL(name);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void out_write_str(FILE *dest, const char *s) {
    if (!s || !*s) return;
    if (s[0] == '\x03') {
        char flav = s[1];
        const char *tname = (flav == 'a') ? "any" : (flav == 'l') ? "all" : (flav == 'o') ? "one" : "none";
        fputs(tname, dest); fputc('(', dest);
        const char *p = s + 2; int first = 1;
        while (*p == '\x01') {
            p++;
            if (!first) fputs(", ", dest); first = 0;
            if (*p == '\x03') {
                int depth = 1; const char *start = p; p++;
                while (*p && depth > 0) { if (*p == '\x03') depth++; else if (*p == '\x04') depth--; p++; }
                size_t L = (size_t)(p - start);
                char *mb = rt_ws_alloc(L + 1); memcpy(mb, start, L); mb[L] = '\0';
                out_write_str(dest, mb);
            } else {
                while (*p && *p != '\x01' && *p != '\x04') { fputc((unsigned char)*p, dest); p++; }
            }
        }
        fputc(')', dest); return;
    }
    if (strchr(s, '\x01')) {
        const char *p = s; int first = 1;
        while (*p) {
            const char *seg = p;
            while (*p && *p != '\x01') p++;
            if (!first) fputc(' ', dest);
            fwrite(seg, 1, (size_t)(p - seg), dest);
            first = 0;
            if (*p == '\x01') p++;
        }
        return;
    }
    fputs(s, dest);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_obj_stringify(DESCR_t d, int use_gist) {
    if (d.v == DT_DATA && d.u && d.u->type && d.u->type->name) {
        const char *mname = use_gist ? "gist" : "Str";
        char proc[256]; resolve_method_chain(d.u->type->name, mname, proc, sizeof proc, NULL);
        if (meth_is_user_proc(proc)) { DESCR_t self1 = d; DESCR_t r = invoke_method_proc(proc, &self1, 1); const char *s = VARVAL_fn(r); return s ? s : ""; }
    }
    const char *s = VARVAL_fn(d); return s ? s : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void out_write_descr(FILE *dest, DESCR_t av, int use_gist) {
    if (IS_INT_fn(av))  { fprintf(dest, "%lld", (long long)av.i); return; }
    if (IS_REAL_fn(av)) { char _rb[64]; fprintf(dest, "%s", icon_real_str(av.r,_rb,sizeof _rb)); return; }
    if (IS_CSET_fn(av)) { if (av.s) fwrite(av.s, 1, strlen(av.s), dest); return; }
    if (av.v == (DTYPE_t)DT_PLREF || av.v == (DTYPE_t)DT_PLVAR) { extern struct Term *rt_pl_cell_to_term_named(void *); extern void pl_write(struct Term *); extern void pl_wr_set_fp(FILE *); DESCR_t _pt = av; fflush(dest); arena_mark_t _cm = rt_pl_cterm_mark(); pl_wr_set_fp(dest); pl_write(rt_pl_cell_to_term_named(plw_entry(&_pt))); pl_wr_set_fp((FILE *)0); if (rt_pl_ctr_on()) rt_pl_cterm_release(_cm); return; }
    if (av.v == DT_DATA) { const char *s = rk_obj_stringify(av, use_gist); if (s) out_write_str(dest, s); return; }
    const char *s = VARVAL_fn(av); if (s) out_write_str(dest, s);
}
#define _OPCOERCE(d) do { \
        if (!IS_INT_fn(d) && !IS_REAL_fn(d)) { \
            const char *_s = VARVAL_fn(d); \
            if (_s && *_s) { char *_e=NULL; long long _iv=strtoll(_s,&_e,10); \
                if (_e && !*_e){(d)=INTVAL(_iv);} \
                else {double _rv=strtod(_s,&_e); \
                      if(_e && !*_e){(d)=REALVAL(_rv);}else{*out=FAILDESCR;return 1;}} \
            } else { *out=FAILDESCR; return 1; } } } while(0)
#define _NUMREL(op) do { DESCR_t _l=args[0],_r=args[1]; _OPCOERCE(_l); _OPCOERCE(_r); \
        double _lv2=IS_REAL_fn(_l)?_l.r:(double)_l.i, _rv2=IS_REAL_fn(_r)?_r.r:(double)_r.i; \
        *out=(_lv2 op _rv2)?_r:FAILDESCR; return 1; } while(0)
#define _STRREL(op) do { DESCR_t _l=args[0],_r=args[1]; \
        const char *_ls=VARVAL_fn(_l); if(!_ls)_ls=""; \
        const char *_rs=VARVAL_fn(_r); if(!_rs)_rs=""; \
        int _cmp=strcmp(_ls,_rs); *out=(_cmp op 0)?rt_str_coerce(_r):FAILDESCR; return 1; } while(0)
#define _SNOCOERCE(d) do { \
        if (!IS_INT_fn(d) && !IS_REAL_fn(d)) { \
            const char *_s9 = VARVAL_fn(d); \
            if (!_s9 || !*_s9) { (d) = INTVAL(0); } else { _OPCOERCE(d); } } } while(0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bn_cvpos(long long pos, int len, int *out_p) {
    if (pos < -(long long)len || pos > (long long)len + 1) return 0;
    *out_p = (int)(pos > 0 ? pos : (long long)len + pos + 1);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bn_str_anal(DESCR_t *args, int nargs, int si, const char **out_s, int *out_i, int *out_j) {
    extern const char *scan_subj;
    extern int scan_pos;
    int have_s = (nargs >= si + 1) && !IS_FAIL_fn(args[si])     && args[si].v     != DT_SNUL;
    int have_i = (nargs >= si + 2) && !IS_FAIL_fn(args[si + 1]) && args[si + 1].v != DT_SNUL;
    int have_j = (nargs >= si + 3) && !IS_FAIL_fn(args[si + 2]) && args[si + 2].v != DT_SNUL;
    const char *s;
    int i, j, slen;
    if (!have_s) {
        if (!scan_subj) return 0;
        s = scan_subj;
        i = scan_pos;
    } else {
        s = VARVAL_fn(args[si]);
        if (!s) s = "";
        i = 1;
    }
    slen = (int)strlen(s);
    if (have_i && !bn_cvpos((long long)to_int(args[si + 1]), slen, &i)) return 0;
    if (!have_j) j = slen + 1;
    else {
        if (!bn_cvpos((long long)to_int(args[si + 2]), slen, &j)) return 0;
        if (i > j) { int t = i; i = j; j = t; }
    }
    if (i < 1 || i > slen + 1 || j < 1 || j > slen + 1) return 0;
    *out_s = s; *out_i = i; *out_j = j;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_numrel(DESCR_t *args, int nargs, DESCR_t *out, int op) {
    if (nargs != 2) return -1;
    if (IS_FAIL_fn(args[0]) || IS_FAIL_fn(args[1])) { *out = FAILDESCR; return 1; }
    DESCR_t _l = args[0], _r = args[1]; _SNOCOERCE(_l); _SNOCOERCE(_r);
    double a = IS_REAL_fn(_l) ? _l.r : (double)_l.i, b = IS_REAL_fn(_r) ? _r.r : (double)_r.i;
    int ok = op==0?(a==b):op==1?(a!=b):op==2?(a<b):op==3?(a<=b):op==4?(a>b):(a>=b);
    *out = ok ? NULVCL : FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_lexrel(DESCR_t *args, int nargs, DESCR_t *out, int op) {
    if (nargs != 2) return -1;
    if (IS_FAIL_fn(args[0]) || IS_FAIL_fn(args[1])) { *out = FAILDESCR; return 1; }
    const char *_ls = VARVAL_fn(args[0]); if (!_ls) _ls = "";
    const char *_rs = VARVAL_fn(args[1]); if (!_rs) _rs = "";
    int c = strcmp(_ls, _rs);
    int ok = op==0?(c>0):op==1?(c<0):op==2?(c>=0):op==3?(c<=0):op==4?(c==0):(c!=0);
    *out = ok ? NULVCL : FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_identdiffer(DESCR_t *args, int nargs, DESCR_t *out, int ident) {
    extern int descr_identical(DESCR_t, DESCR_t);
    DESCR_t a = nargs > 0 ? args[0] : NULVCL, b = nargs > 1 ? args[1] : NULVCL;
    int same = descr_identical(a, b);
    *out = (ident ? same : !same) ? NULVCL : FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⛔ v.v==DT_I / DT_R / (DT_S && slen==0xFFFFFFFFu) are IS_INT_fn/IS_REAL_fn/IS_CSET_fn's bodies (core.h),
   duplicated literally rather than called: at -O0 `static inline` is a real call/ret (FACT RULE, no -O2),
   and this leaf is the row's own measured 2.38%-of-kernel cost on string_manip.sno. NOT `always_inline` --
   that was TESTED AND REVERTED s264 (GOAL-HQ-PERFORM.md:157), it forces the DESCR_t out of memory into a
   register across a call boundary the GC's conservative stack scanner needs to see, breaking deferred
   capture (058/059/060). Plain duplication forces no residency -- -O0 still spills/reloads exactly as
   around a call -- so the s264 mechanism does not apply here; empirically re-proven safe by 067's ARBNO
   witness (bn_size reached via *SIZE(...) inside a deferred pattern element, under active backtracking).
   Row perf-core-tag-predicate-o0-call-tax; do not relax core.h's own declarations, only this call site. */
static __attribute__((noinline)) int bn_size(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return -1;
    DESCR_t v = args[0];
    if (IS_FAIL_fn(v)) { *out = FAILDESCR; return 1; }
    if (v.v == DT_T)   { *out = INTVAL(v.tbl ? v.tbl->size : 0); return 1; }
    if (v.v == DT_A)   { *out = INTVAL(v.arr ? (v.arr->hi - v.arr->lo + 1) : 0); return 1; }
    if (v.v == DT_DATA) {
        DESCR_t tag = FIELD_GET_fn(v,"gen_type");
        if (tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0) {
            *out = INTVAL((int)FIELD_GET_fn(v,"frame_size").i); return 1;
        }
    }
    if (v.v == DT_I) { const char *is = VARVAL_fn(v); *out = INTVAL(is ? (long)strlen(is) : 0); return 1; }
    if (v.v == DT_R) { *out = INTVAL(0); return 1; }
    if (v.v == DT_S && v.slen == 0xFFFFFFFFu) {
        int klen = kw_cset_len(v.s);
        *out = INTVAL(klen >= 0 ? klen : (v.s ? (long)strlen(v.s) : 0));
        return 1;
    }
    const char *s = VARVAL_fn(v); if (!s) { *out = INTVAL(0); return 1; }
    if (strchr(s,'\x01')) {
        long n=1; for(const char *p=s;*p;p++) if(*p=='\x01') n++;
        *out = INTVAL(n); return 1;
    }
    long len = v.slen > 0 ? v.slen : (long)strlen(s);
    *out = INTVAL(len); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* NS-TIME (s249): ONE clock, shared with core.c's _TIME_ -- this used to be a SECOND, WORSE TIME(): clock()*1000/CLOCKS_PER_SEC,
   i.e. CPU time truncated to whole milliseconds, and it is the arm that actually runs (core.c's registered _TIME_ never fired). */
static __attribute__((noinline)) int bn_time(DESCR_t *args, int nargs, DESCR_t *out) {
    (void)args; if (nargs > 1) return -1;
    *out = INTVAL(rt_time_ns()); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_date(DESCR_t *args, int nargs, DESCR_t *out) {
    (void)args; if (nargs > 1) return -1;
    time_t now = time(NULL); struct tm *tm = localtime(&now);
    char *buf = (char *)rt_ws_alloc_c(64); strftime(buf, 64, "%m/%d/%Y %H:%M:%S", tm);
    *out = STRVAL(buf); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_trim(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return -1;
    const char *sv = VARVAL_fn(args[0]); if (!sv) sv = "";
    size_t n = sv_len(args[0], sv); while (n > 0 && sv[n-1] == ' ') n--;
    char *buf = (char *)rt_ws_alloc_c(n + 1); memcpy(buf, sv, n); buf[n] = 0;
    *out = BSTRVAL(buf, n); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_dupl(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2) return -1;
    const char *sv = VARVAL_fn(args[0]); if (!sv) sv = "";
    DESCR_t nn = args[1]; _SNOCOERCE(nn);
    long long k = IS_REAL_fn(nn) ? (long long)nn.r : nn.i;
    if (k < 0) { *out = FAILDESCR; return 1; }
    size_t sl = sv_len(args[0], sv); char *buf = (char *)rt_ws_alloc_c(sl * (size_t)k + 1);
    for (long long i = 0; i < k; i++) memcpy(buf + (size_t)i * sl, sv, sl);
    buf[sl * (size_t)k] = 0; *out = BSTRVAL(buf, sl * (size_t)k); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐ THE CEREMONY, NOT THE LOOP (hq_P s262).  MEASURED, roman.sno -O0 fixed work N=2000, callgrind line annotation: bn_replace is 480 Ir per call and the translate loop `buf[i] = map[sv[i]]` is 45 of them.  Of the other 435: three VARVAL_fn calls 48, three sv_len calls 81, the 4-slot map cache and its two memcmp PLT hops 101, rt_ws_alloc_c 60, result store 17.  ⭐ Ninety per cent of REPLACE was getting ready to translate five characters.
   ⛔ WHY THE CALLS EXIST AT ALL, AND WHY `static inline` DID NOT SAVE US: at -O0 gcc honours neither `inline` nor its own cost model, so every `static inline` helper in this file is a REAL CALL with a real prologue -- and DESCR_t is 16 bytes passed BY VALUE, so sv_len pays stack traffic on top.  Under the s262 NO-`-O2` fact rule -O0 is the number of record, so a hot leaf cannot delegate its field reads to a helper and hope the optimizer folds them; it has to read the fields itself.
   ⭐ EQUIVALENCE IS EXACT, NOT ARGUED FROM SHAPE.  VARVAL_fn is `cmp dil, DT_S / jne c_VARVAL_fn / test rsi,rsi / jz c_VARVAL_fn / mov rax,rsi / ret` (rtx_str.s:312) -- for DT_S with non-NULL .s it returns v.s unchanged and allocates nothing.  sv_len for DT_S with slen neither 0xFFFFFFFF nor 0 returns slen.  The guard below admits EXACTLY that intersection and every other descriptor -- other tags, NULL .s, the 0xFFFFFFFF sentinel, slen 0 -- falls through to the UNCHANGED pair.  It cannot answer differently, only sooner.
   KILLSWITCH SCRIP_REPL_PL=0 restores the call pair on the same binary. */
static int repl_pl_off(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_REPL_PL"); v = (e && *e == '0') ? 1 : 0; } return v; }
/* RTX PORT (row perf-replace-translate-loop-scalar-byte-copy): the translate loop below is a pure gather-scatter
   with no branch and no call -- 31.90% of string_manip.sno's fixed-work kernel Ir at -O0 N=20000 (14,553,000 Ir) --
   so it is extracted to its own symbol and ported to rtx_str.s under the existing STR family gate, same class of
   cure as rtx_table.s's hot-path ports. This is the C of record; rt_translate_bytes (rtx_str.s) owns the exported
   name and tail-jumps here when SCRIP_RTX_STR=0. */
void c_rt_translate_bytes(char *dst, const char *src, size_t n, const char *map) {
    for (size_t i = 0; i < n; i++) dst[i] = map[(unsigned char)src[i]];
}
/* ⛔ THE KILLSWITCH READ IS HOISTED, AND THAT IS NOT A DETAIL: the first draft called repl_pl_off() INSIDE the macro, so a getenv-memo function ran three times per REPLACE -- 26,400 non-inlined calls -- and ate two thirds of the cure it was guarding (measured -0.92% where the line annotation predicted -2.6%).  At -O0 a `static int f(void){static int v;...}` is a real call every time; a control arm has to be read ONCE and carried in a local. */
#define BN_PTRLEN(A, P, L, F) do { DESCR_t _a = (A); \
    if ((F) && _a.v == DT_S && _a.s && _a.slen != 0xFFFFFFFFu && _a.slen != 0u) { (P) = _a.s; (L) = (size_t)_a.slen; } \
    else { (P) = VARVAL_fn(_a); if (!(P)) (P) = ""; (L) = sv_len(_a, (P)); } } while (0)
static __attribute__((noinline)) int bn_replace(DESCR_t *args, int nargs, DESCR_t *out) {
    static struct { unsigned char v, n; char f[63], t[63], map[256]; } g_rm[4];
    static int g_rm_off = -1;
    if (nargs != 3) return -1;
    const char *sv, *fv, *tv; size_t sl, fl, tl;
    const int _plf = !repl_pl_off();
    BN_PTRLEN(args[0], sv, sl, _plf);
    BN_PTRLEN(args[1], fv, fl, _plf);
    BN_PTRLEN(args[2], tv, tl, _plf);
    if (fl != tl || !fl) { *out = FAILDESCR; return 1; }
    if (g_rm_off < 0) { const char *e = getenv("SCRIP_REPLMAP_OFF"); g_rm_off = (e && *e) ? 1 : 0; }
    char mloc[256]; char *map = mloc;
    if (!g_rm_off && fl < 63) { unsigned s = ((unsigned char)fv[0] * 31u + (unsigned)fl) & 3u;
        if (g_rm[s].v && g_rm[s].n == (unsigned char)fl && !memcmp(g_rm[s].f, fv, fl) && !memcmp(g_rm[s].t, tv, fl)) map = g_rm[s].map;
        else { map = g_rm[s].map; for (int i = 0; i < 256; i++) map[i] = (char)i; for (size_t k = 0; k < fl; k++) map[(unsigned char)fv[k]] = tv[k];
               memcpy(g_rm[s].f, fv, fl); memcpy(g_rm[s].t, tv, fl); g_rm[s].n = (unsigned char)fl; g_rm[s].v = 1; } }
    else { for (int i = 0; i < 256; i++) map[i] = (char)i; for (size_t k = 0; k < fl; k++) map[(unsigned char)fv[k]] = tv[k]; }
    size_t n = sl; char *buf = (char *)rt_ws_alloc_c(n + 1);
    rt_translate_bytes(buf, sv, n, map);
    buf[n] = 0; *out = BSTRVAL(buf, n); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_substr(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2 && nargs != 3) return -1;
    *out = SUBSTR_fn(args[0], args[1], (nargs == 3) ? args[2] : INTVAL(0)); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_reverse(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return -1;
    *out = REVERS_fn(args[0]); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_lpad(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2 && nargs != 3) return -1;
    *out = lpad_fn(args[0], args[1], (nargs == 3) ? args[2] : STRVAL(" ")); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_rpad(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2 && nargs != 3) return -1;
    *out = rpad_fn(args[0], args[1], (nargs == 3) ? args[2] : STRVAL(" ")); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_integer(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return -1;
    DESCR_t av = args[0];
    if (IS_INT_fn(av))  { *out = NULVCL; return 1; }
    if (IS_REAL_fn(av)) { *out = FAILDESCR; return 1; }
    const char *sv = VARVAL_fn(av); if (!sv) sv = "";
    const char *p = sv; while (*p == ' ' || *p == '\t') p++;
    if (*p == '\0') { *out = NULVCL; return 1; }
    if (*p == '+' || *p == '-') p++;
    const char *d = p; while (*p >= '0' && *p <= '9') p++;
    const char *e = p; while (*e == ' ' || *e == '\t') e++;
    *out = (p != d && *e == '\0') ? NULVCL : FAILDESCR; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static __attribute__((noinline)) int bn_remdr(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 2) return -1;
    DESCR_t a = args[0], b = args[1]; _SNOCOERCE(a); _SNOCOERCE(b);
    if (IS_REAL_fn(a) || IS_REAL_fn(b)) {
        double rb = IS_REAL_fn(b) ? b.r : (double)b.i;
        if (rb == 0.0) { *out = FAILDESCR; return 1; }
        double rr = fmod(IS_REAL_fn(a) ? a.r : (double)a.i, rb);
        if (isinf(rr)) { core_runtime_error(312, "remdr caused real overflow"); *out = FAILDESCR; return 1; }
        *out = REALVAL(rr); return 1;
    }
    long long ai = a.i, bi = b.i;
    if (bi == 0) { *out = FAILDESCR; return 1; }
    *out = INTVAL(ai % bi); return 1;
}
extern unsigned rt_dtax_gen;
typedef struct { unsigned gen; unsigned char len; signed char kind; short nf; char nm[14]; void *ctor; const char *syn; } dtax_ent_t;
static dtax_ent_t g_dtax[256];
static dtax_ent_t g_dtax_bid[1025];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dtax_off(void) { static int p = -1; if (p < 0) { const char *e = getenv("SCRIP_DTAX_OFF"); p = (e && *e) ? 1 : 0; } return p; }
static void dtx4(dtax_ent_t *e, unsigned char l, const char *nm, void *h) { if (!e || !l) return; e->gen = rt_dtax_gen; e->len = l; e->kind = 4; e->nf = 0; memcpy(e->nm, nm, l); e->ctor = h; e->syn = 0; }
static void dtx5(dtax_ent_t *e, unsigned char l, const char *nm, void *h, short x) { if (!e || !l) return; e->gen = rt_dtax_gen; e->len = l; e->kind = 5; e->nf = x; memcpy(e->nm, nm, l); e->ctor = h; e->syn = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bn_type_datatype(const char *fn, DESCR_t *args, int nargs, DESCR_t *out)
{
    DESCR_t av = args[0];
    const char *t;
    int declared = 0;
    (void)nargs;
    if (IS_INT_fn(av))       t="integer";
    else if (IS_REAL_fn(av)) t="real";
    else if (av.v==DT_T)     t=(av.tbl && av.tbl->is_set) ? "set" : "table";
    else if (av.v==DT_A)     t=(!strcmp(fn,"DATATYPE")) ? "array" : "list";
    else if (av.v==DT_DATA)  {
        DESCR_t tag = FIELD_GET_fn(av,"gen_type");
        if (tag.v==DT_S && tag.s) t = tag.s;
        else { DATINST_t *di = (DATINST_t *)av.u; declared = (di && di->type && di->type->name) ? 1 : 0; t = declared ? di->type->name : "record"; }
    }
    else if (IS_CSET_fn(av)) t="cset";
    else if (IS_FH_fn(av))   t="file";
    else if (av.v==DT_E) {
        t = "procedure";
        if (!strcmp(fn,"DATATYPE")) { t = "function";
            if (av.slen == 0xFFFFFFFEu && av.s) {
                for (int _ti=0;_ti<g_stage2.proc_count;_ti++) if (g_stage2.proc_table[_ti].name && !strcmp(g_stage2.proc_table[_ti].name,av.s)){t="procedure";break;}
                if (!strcmp(t,"function")) { extern int rt_proc_is_registered(const char *); if (rt_proc_is_registered(av.s)) t="procedure"; }
            } else t="procedure"; }
    }
    else if (av.v==DT_C)     t="CODE";
    else if (av.v==DT_X)     t="EXPRESSION";
    else if (av.v==DT_N)     t="name";
    else if (av.v==DT_P)     t="PATTERN";
    else if (av.v==DT_SNUL)  t=(!strcmp(fn,"DATATYPE")) ? "string" : "null";
    else t="string";
    if (!strcmp(fn,"DATATYPE") && declared) { *out = STRVAL(rt_ws_strdup_c(t)); return 1; }
    if (!strcmp(fn,"DATATYPE")) { static char ub[32]; int ui=0;
        for (; t[ui] && ui<31; ui++) ub[ui]=(char)((t[ui]>='a'&&t[ui]<='z')?t[ui]-32:t[ui]); ub[ui]=0; *out = STRVAL(rt_ws_strdup_c(ub)); return 1; }
    *out = STRVAL(t); return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long rt_record_image_id(void *inst)
{
    return inst ? ((DATINST_t *)inst)->id : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bn_sno_name(DESCR_t *args, int nargs, DESCR_t *out)
{
    const char *sv = rt_sno_indirect_name(args[0]);
    (void)nargs;
    if (!sv || !*sv) { *out = FAILDESCR; return 1; }
    { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_N; d.slen = 0; d.s = rt_ws_strdup(sv); *out = d; return 1; }
}
long g_bidprof[1024]; int g_bidprof_on = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bidprof_dump(void) { for (int i = 0; i < 1024; i++) if (g_bidprof[i]) fprintf(stderr, "BIDPROF %d %ld\n", i, g_bidprof[i]); }
void bidprof_init(void) { const char *e = getenv("SCRIP_BID_PROF"); g_bidprof_on = (e && e[0]=='1') ? 1 : 0; if (g_bidprof_on) atexit(bidprof_dump); }
int g_bidjmp_on = 1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *sort_key_cstr(DESCR_t v, char *buf, int bufsz) {
    if (v.v == DT_S)    return v.s ? v.s : "";
    if (v.v == DT_SNUL) return "";
    if (v.v == DT_I)    { snprintf(buf, (size_t)bufsz, "%lld", (long long)v.i); return buf; }
    { const char *s = VARVAL_fn(v); return s ? s : ""; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sort_descr_cmp(DESCR_t a, DESCR_t b) {
    if (IS_INT_fn(a) && IS_INT_fn(b)) return (a.i > b.i) ? 1 : (a.i < b.i) ? -1 : 0;
    { char ba[64], bb[64]; const char *sa = sort_key_cstr(a, ba, (int)sizeof ba), *sb = sort_key_cstr(b, bb, (int)sizeof bb); return strcmp(sa, sb); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t sort_field_of(DESCR_t v, int field_idx) {
    if (field_idx >= 0 && v.v == DT_DATA && v.u) { DATINST_t *ia = (DATINST_t *)v.u; if (ia->type && field_idx < ia->type->nfields) return ia->fields[field_idx]; }
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sort_msort_descr(DESCR_t *a, DESCR_t *tmp, int n, int field_idx) {
    if (n < 2) return;
    { int m = n / 2, i = 0, j = n / 2, k = 0, t;
      sort_msort_descr(a, tmp, m, field_idx); sort_msort_descr(a + m, tmp + m, n - m, field_idx);
      while (i < m && j < n) { if (sort_descr_cmp(sort_field_of(a[j], field_idx), sort_field_of(a[i], field_idx)) < 0) tmp[k++] = a[j++]; else tmp[k++] = a[i++]; }
      while (i < m) tmp[k++] = a[i++];
      while (j < n) tmp[k++] = a[j++];
      for (t = 0; t < n; t++) a[t] = tmp[t]; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sort_msort_pairs(TBPAIR_t **a, TBPAIR_t **tmp, int n, int by_val) {
    if (n < 2) return;
    { int m = n / 2, i = 0, j = n / 2, k = 0, t;
      sort_msort_pairs(a, tmp, m, by_val); sort_msort_pairs(a + m, tmp + m, n - m, by_val);
      while (i < m && j < n) { DESCR_t x = by_val ? a[i]->val : a[i]->key_descr, y = by_val ? a[j]->val : a[j]->key_descr; if (sort_descr_cmp(y, x) < 0) tmp[k++] = a[j++]; else tmp[k++] = a[i++]; }
      while (i < m) tmp[k++] = a[i++];
      while (j < n) tmp[k++] = a[j++];
      for (t = 0; t < n; t++) a[t] = tmp[t]; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*⭐⭐ BAKED-BID ENTRY (hq_P s262).  MEASURED, roman.sno -O0 fixed work N=2000, callgrind: bid_of was 1,826,260 Ir = 4.16% of the WHOLE PROGRAM, 11,002 calls at 166 Ir each -- and its ONLY job is to turn a string into the array index g_dtax_bid[_bid].  The line annotation showed the cache it indexes is ALREADY HOT: `if (_dx->kind == 4 && _dx->ctor) return ...` dispatched 8,799 of roman's 8,800 REPLACE calls straight to bn_replace.  So the program was paying a djb2 walk plus a table probe, per call, to look up a pointer it had already cached.
  ⭐ THE CALL SITE KNOWS THE NAME AT COMPILE TIME -- it is a baked string literal in .rodata -- so the emitter computes bid_of() ITSELF and hands the answer over as an immediate.  bidlen packs (strlen(fn) << 16) | bid_of(fn,len); a NEGATIVE value means "not baked, resolve it yourself" and is what every non-emitted caller passes.
  ⛔ THIS CANNOT ANSWER DIFFERENTLY, ONLY SOONER: the baked integer is bit-for-bit the one bid_of() would have returned for the same bytes, because emitter and runtime compile the SAME builtin_ids.h table.  A name that is not a builtin bakes 0, which is exactly bid_of()'s miss value.  Redefinition (OPSYN/DEFINE) is unaffected -- it is handled downstream by rt_dtax_gen and the APPLY_fn fallback, neither of which this touches.
  ⛔ NO PER-OP FILTER (Lon 2026-08-20): every by-name call site bakes its bid, builtin or not.  There is no list of blessed names anywhere in this cure -- that is what makes it a class fix and not an exception table like dop_direct_fp's.
  ⛔ NO NEW GLOBAL: the bid rides an argument register as an immediate operand baked into the instruction stream.  Nothing persists across the call. */
int try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) { return try_call_builtin_by_name_bl(fn, args, nargs, out, -1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int try_call_builtin_by_name_bl(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, int bidlen)
{
    if (nargs == 1 && args[0].v == DT_DATA && args[0].u && args[0].u->type) {
        DATBLK_t *idb = args[0].u->type; const char _f0 = fn ? fn[0] : 0;
        for (int fi = 0; fi < idb->nfields; fi++) if (idb->fields[fi] && idb->fields[fi][0] == _f0 && !strcmp(idb->fields[fi], fn)) {
            extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
            *out = dat_field_get(fn, args[0]); return 1;
        }
    }
    if (!fn || !out) return 0;
    /* ⭐ NAME-INDEPENDENT FAST PATH (perf-dispatch-callsite-cache STEP3, s272).  When rt_dtax_gen==0, NOT ONE OPSYN or
       DATA() has EVER executed in this run (exactly two write sites in the whole tree -- rt_builtin_synonym_add and
       driver_data.c's DATA() registration, re-grepped for this change; rt_dtax_gen starts at 0, driver_data.c:13) --
       so bidlen's baked _bid, for this CLOSED, ENUMERATED set of bid-keyed builtins, PROVABLY still means its plain
       compiled-in self; skip g_dtax_bid/g_dtax and the branch-selection between them entirely and call the C
       function directly.  Scoped to the fourteen names whose _bid the switch below ALREADY re-checks explicitly
       (SIZE..DIFFER) -- the relop family (EQ/NE/LT/LE/GT/GE/LGT/LLT/LGE/LLE/LEQ/LNE) has NO builtin_ids.h entry at
       all (grepped, zero hits), so bid_of() bakes 0 for them and they fall through unchanged to the hash-path cache.
       ⭐ EXTENDED TO SNO$NAME (perf-dispatch-fastpath-name-indirect, s276).  SNO$NAME (BID_SNOx24NAME) is bid-eligible
       but MONADIC, not niladic, and unlike the fourteen above it NEVER gets a g_dtax_bid cache write at all (its own
       case arm in the _fl>=2&&_fl<=8 switch below calls bn_sno_name directly with no dtx4/dtx5 call) -- every
       $-indirection call was paying this whole cache-probe machinery for a slot that can never hit. STEP 1 verified:
       (a) bn_sno_name -> rt_sno_indirect_name CAN reach core_runtime_error (core.c, via kwb_error(239,...) when the
       indirect operand is not a name) -> longjmp(g_core_errjmp_stk[...]) under Icon &error trapping, the SAME hazard
       class as bn_remdr above; safe for the identical reason -- this fast path never leaves
       try_call_builtin_by_name_bl, itself reached only from inside rt_call_arr_bl's setjmp-protected region, so the
       longjmp still targets THIS call's own frame. (b) runs after the SAME DT_DATA block above, unchanged. ⛔ UNLIKE
       the other fourteen this arm KEEPS an explicit nargs==1 guard: the pre-existing slow-path arms for SNO$NAME (the
       _fl-switch case below, and L_bidjmp_6468) both check nargs==1 before calling bn_sno_name and fall through to
       "unhandled" otherwise -- preserved here rather than assumed unreachable.
       ⛔ ORDER MATTERS, TWICE: (a) runs AFTER the DT_DATA field-precedence block above -- a record field literally
       named SIZE/TRIM/... still wins, unchanged; (b) this function is reached ONLY from rt_call_arr_impl, itself
       reached ONLY from inside rt_call_arr_bl's setjmp-protected region (no other caller in the tree) -- so a
       longjmp from deep inside e.g. bn_remdr's core_runtime_error(312, "remdr caused real overflow") path (the only
       core_runtime_error/core_icn_error call among these fourteen bodies, all read in full before writing this)
       still unwinds to THIS SAME call's own handler under Icon &error trapping, byte-for-byte as today, because
       this fast path never leaves that call chain -- an earlier sketch of this idea that called bn_* from EMITTED
       CODE, bypassing rt_call_arr_bl's setjmp altogether, WOULD have retargeted that longjmp and was rejected
       before being written, on exactly this ground.  Return-value/*out shape is EXACT, not argued from shape: every
       case below is the identical `return bn_X(args, nargs, out[, op])` the array-cache-hit path already performs
       (kind 4/5 a few lines below), just reached without a cache-cell read; a nargs mismatch still returns -1 with
       *out untouched, propagating exactly as it does today.  bidlen<0 (a non-emitted caller) always fails the
       guard.  ⛔ NO NEW GLOBAL: every read here is bidlen (a parameter), rt_dtax_gen (pre-existing extern), or
       dtax_off()'s memoized static (pre-existing) -- nothing new persists across calls. */
    if (bidlen >= 0 && rt_dtax_gen == 0 && !dtax_off()) {
        const int _fb = bidlen & 0xFFFF;
        extern long g_bidprof[1024]; extern int g_bidprof_on; extern void bidprof_init(void);
        if (g_bidprof_on < 0) bidprof_init();
        if (g_bidprof_on && _fb >= 0 && _fb < 1024) g_bidprof[_fb]++;
        if (_fb == BID_SIZE)    return bn_size(args, nargs, out);
        if (_fb == BID_REPLACE) return bn_replace(args, nargs, out);
        if (_fb == BID_TRIM)    return bn_trim(args, nargs, out);
        if (_fb == BID_SUBSTR)  return bn_substr(args, nargs, out);
        if (_fb == BID_REVERSE) return bn_reverse(args, nargs, out);
        if (_fb == BID_INTEGER) return bn_integer(args, nargs, out);
        if (_fb == BID_DUPL)    return bn_dupl(args, nargs, out);
        if (_fb == BID_LPAD)    return bn_lpad(args, nargs, out);
        if (_fb == BID_RPAD)    return bn_rpad(args, nargs, out);
        if (_fb == BID_REMDR)   return bn_remdr(args, nargs, out);
        if (_fb == BID_DATE)    return bn_date(args, nargs, out);
        if (_fb == BID_TIME)    return bn_time(args, nargs, out);
        if (_fb == BID_IDENT)   return bn_identdiffer(args, nargs, out, 1);
        if (_fb == BID_DIFFER)  return bn_identdiffer(args, nargs, out, 0);
        if (_fb == BID_SNOx24NAME && nargs == 1) return bn_sno_name(args, nargs, out);
    }
    const size_t _fnlen = (bidlen >= 0) ? (size_t)((unsigned)bidlen >> 16) : strlen(fn);
    const int _bid = (bidlen >= 0) ? (int)(bidlen & 0xFFFF) : bid_of(fn, (unsigned)_fnlen);
    { extern long g_bidprof[1024]; extern int g_bidprof_on; extern void bidprof_init(void); if (g_bidprof_on < 0) bidprof_init(); if (g_bidprof_on && _bid >= 0 && _bid < 1024) g_bidprof[_bid]++; }
    /* ⭐ ARRAY-PATH GEN-ONLY VALIDATION (perf-dispatch-callsite-cache STEP2, s271).  bid_of() (builtin_ids.h) is an
       INJECTIVE map over its nonzero range -- 176 distinct builtin/operator names, 176 distinct ids, each returned
       only after bid_of's OWN internal memcmp against g_bid_tab -- so _bid uniquely determines fn's bytes whenever
       _bid>0, the only case g_dtax_bid[_bid] is ever indexed (the bid==0 miss value routes to g_dtax[] instead).
       Every write into a g_dtax_bid[] slot (dtx4/dtx5, and the DATA-type/synonym/miss-cache writes further below)
       is keyed by THAT SAME call's fn, so a later call reaching the identical slot can only do so by presenting
       the identical fn bytes again -- the len+memcmp recheck the hash path g_dtax[] still needs (arbitrary names
       CAN collide into one of its 256 buckets) is therefore provably redundant here once gen alone matches. */
    dtax_ent_t *_dx = 0; int _dx_hit = 0; int _dx_skip_ctor = 0; int _dx_skip_syn = 0; unsigned _dxh = 5381u; unsigned char _dxl = 0; int _dx_bid_path = 0;
    if (!dtax_off()) { if (_bid > 0 && _bid <= 1024 && _fnlen && _fnlen < 14) { _dxl = (unsigned char)_fnlen; _dx = &g_dtax_bid[_bid]; _dx_bid_path = 1; } else { const char *_q = fn; while (*_q && _dxl < 14) { _dxh = _dxh * 131u + (unsigned char)*_q; _q++; _dxl++; } if (!(!*_q && _dxl)) _dxl = 0; else _dx = &g_dtax[_dxh & 255u]; }
      if (_dx) {
        if (_dx->gen == rt_dtax_gen && (_dx_bid_path || (_dx->len == _dxl && !memcmp(_dx->nm, fn, _dxl)))) {
          if (_dx->kind == 4 && _dx->ctor) return ((int (*)(DESCR_t *, int, DESCR_t *))_dx->ctor)(args, nargs, out);
          if (_dx->kind == 5 && _dx->ctor) return ((int (*)(DESCR_t *, int, DESCR_t *, int))_dx->ctor)(args, nargs, out, (int)_dx->nf);
          if (_dx->kind == 2 && _dx->syn) return try_call_builtin_by_name(_dx->syn, args, nargs, out);
          if (_dx->kind == 1 && _dx->ctor) { extern DESCR_t dat_construct_byref(void *, DESCR_t *, int);
            if (nargs <= (int)_dx->nf) { DESCR_t _fv[64]; int _nf = _dx->nf > 64 ? 64 : (int)_dx->nf;
              for (int _i = 0; _i < _nf; _i++) _fv[_i] = (_i < nargs) ? args[_i] : NULVCL;
              *out = dat_construct_byref(_dx->ctor, _fv, _nf); return 1; }
            _dx_hit = 1; _dx_skip_ctor = 1; }
          else if (_dx->kind == 0) { _dx_hit = 1; _dx_skip_ctor = 1; _dx_skip_syn = 1; }
          else if (_dx->kind == 3) { _dx_hit = 1; _dx_skip_ctor = 1; } } } }
    if (!_dx_skip_ctor) { extern DatType *dat_find_type(const char *); extern DESCR_t dat_construct(DatType *, DESCR_t *, int); extern int dat_nfields_byref(void *);
      DatType *_udt = dat_find_type(fn);
      if (_udt && _dx) { _dx->gen = rt_dtax_gen; _dx->len = _dxl; _dx->kind = 1; _dx->nf = (short)dat_nfields_byref((void *)_udt); memcpy(_dx->nm, fn, _dxl); _dx->ctor = (void *)_udt; _dx->syn = 0; }
      if (_udt && nargs <= _udt->nfields) {
          DESCR_t _fv[64]; int _nf = _udt->nfields > 64 ? 64 : _udt->nfields;
          for (int _i = 0; _i < _nf; _i++) _fv[_i] = (_i < nargs) ? args[_i] : NULVCL;
          *out = dat_construct(_udt, _fv, _nf); return 1;
      }
      if (!_udt && _dx && !_dx_hit) { _dx->gen = rt_dtax_gen; _dx->len = _dxl; _dx->kind = 3; _dx->nf = 0; memcpy(_dx->nm, fn, _dxl); _dx->ctor = 0; _dx->syn = 0; } }
    { size_t _fl = _fnlen;
      if (_fl >= 2 && _fl <= 8) { int _r = -1;
        switch (((unsigned)_fl << 8) | (unsigned char)fn[0]) {
        case (2u<<8)|'E': if (fn[1]=='Q') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 0); _r = bn_numrel(args, nargs, out, 0); } break;
        case (2u<<8)|'N': if (fn[1]=='E') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 1); _r = bn_numrel(args, nargs, out, 1); } break;
        case (2u<<8)|'L': if (fn[1]=='T') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 2); _r = bn_numrel(args, nargs, out, 2); } else if (fn[1]=='E') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 3); _r = bn_numrel(args, nargs, out, 3); } break;
        case (2u<<8)|'G': if (fn[1]=='T') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 4); _r = bn_numrel(args, nargs, out, 4); } else if (fn[1]=='E') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 5); _r = bn_numrel(args, nargs, out, 5); } break;
        case (3u<<8)|'L': { int _o = fn[1]=='G'&&fn[2]=='T' ? 0 : fn[1]=='L'&&fn[2]=='T' ? 1 : fn[1]=='G'&&fn[2]=='E' ? 2 : fn[1]=='L'&&fn[2]=='E' ? 3 : fn[1]=='E'&&fn[2]=='Q' ? 4 : fn[1]=='N'&&fn[2]=='E' ? 5 : -1;
                            if (_o >= 0) { dtx5(_dx, _dxl, fn, (void *)bn_lexrel, (short)_o); _r = bn_lexrel(args, nargs, out, _o); } } break;
        case (4u<<8)|'S': if ((_bid == BID_SIZE)) { dtx4(_dx, _dxl, fn, (void *)bn_size); _r = bn_size(args, nargs, out); } break;
        case (4u<<8)|'T': if ((_bid == BID_TIME)) { dtx4(_dx, _dxl, fn, (void *)bn_time); _r = bn_time(args, nargs, out); } else if ((_bid == BID_TRIM)) { dtx4(_dx, _dxl, fn, (void *)bn_trim); _r = bn_trim(args, nargs, out); } break;
        case (4u<<8)|'D': if ((_bid == BID_DATE)) { dtx4(_dx, _dxl, fn, (void *)bn_date); _r = bn_date(args, nargs, out); } else if ((_bid == BID_DUPL)) { dtx4(_dx, _dxl, fn, (void *)bn_dupl); _r = bn_dupl(args, nargs, out); } break;
        case (4u<<8)|'F': if ((_bid == BID_FAIL)) { *out = FAILDESCR; return 1; } break;
        case (4u<<8)|'L': if ((_bid == BID_LPAD)) { dtx4(_dx, _dxl, fn, (void *)bn_lpad); _r = bn_lpad(args, nargs, out); } break;
        case (4u<<8)|'R': if ((_bid == BID_RPAD)) { dtx4(_dx, _dxl, fn, (void *)bn_rpad); _r = bn_rpad(args, nargs, out); } break;
        case (5u<<8)|'I': if ((_bid == BID_IDENT)) { dtx5(_dx, _dxl, fn, (void *)bn_identdiffer, 1); _r = bn_identdiffer(args, nargs, out, 1); } break;
        case (5u<<8)|'R': if ((_bid == BID_REMDR)) { dtx4(_dx, _dxl, fn, (void *)bn_remdr); _r = bn_remdr(args, nargs, out); } break;
        case (6u<<8)|'S': if ((_bid == BID_SUBSTR)) { dtx4(_dx, _dxl, fn, (void *)bn_substr); _r = bn_substr(args, nargs, out); } break;
        case (6u<<8)|'D': if ((_bid == BID_DIFFER)) { dtx5(_dx, _dxl, fn, (void *)bn_identdiffer, 0); _r = bn_identdiffer(args, nargs, out, 0); } break;
        case (7u<<8)|'R': if ((_bid == BID_REPLACE)) { dtx4(_dx, _dxl, fn, (void *)bn_replace); _r = bn_replace(args, nargs, out); } else if ((_bid == BID_REVERSE)) { dtx4(_dx, _dxl, fn, (void *)bn_reverse); _r = bn_reverse(args, nargs, out); } break;
        case (7u<<8)|'I': if ((_bid == BID_INTEGER)) { dtx4(_dx, _dxl, fn, (void *)bn_integer); _r = bn_integer(args, nargs, out); } break;
        case (7u<<8)|'S': if ((_bid == BID_SUCCEED)) { *out = NULVCL; return 1; } break;
        case (8u<<8)|'D': if ((_bid == BID_DATATYPE) && nargs == 1) _r = bn_type_datatype(fn, args, nargs, out); break;
        case (8u<<8)|'S': if ((_bid == BID_SNOx24NAME) && nargs == 1) _r = bn_sno_name(args, nargs, out); else if ((_bid == BID_SNOx24NRET)) { extern int rt_g_ret_by_name; rt_g_ret_by_name = 1; *out = NULVCL; _r = 1; } break;
        default: break; }
        if (_r >= 0) return _r; } }
    if (g_bidjmp_on) switch (_bid) { case BID___rk_bool: goto L_bidjmp_5076; case BID___rk_defined: goto L_bidjmp_5087; case BID___rk_dor: goto L_bidjmp_5090; case BID___rk_bool_val: goto L_bidjmp_5093; case BID___pas_chr: goto L_bidjmp_5103; case BID___pas_chrlit: goto L_bidjmp_5109; case BID___pas_enum_name: goto L_bidjmp_5112; case BID___pas_read_i: goto L_bidjmp_5121; case BID___pas_read_c: goto L_bidjmp_5124; case BID___pas_readln: goto L_bidjmp_5127; case BID___pas_eof: goto L_bidjmp_5130; case BID___pas_eoln: goto L_bidjmp_5135; case BID___pas_getbufch: goto L_bidjmp_5149; case BID___pas_trunc: goto L_bidjmp_5152; case BID___pas_abs: goto L_bidjmp_5156; case BID___pas_writeln: goto L_bidjmp_5171; case BID___pas_write: goto L_bidjmp_5171; case BID_write: goto L_bidjmp_5209; case BID_writes: goto L_bidjmp_5209; case BID_integer: goto L_bidjmp_5232; case BID_real: goto L_bidjmp_5264; case BID_string: goto L_bidjmp_5273; case BID_numeric: goto L_bidjmp_5283; case BID_cset: goto L_bidjmp_5320; case BID_ord: goto L_bidjmp_5330; case BID_image: goto L_bidjmp_5336; case BID_args: goto L_bidjmp_5339; case BID_proc: goto L_bidjmp_5385; case BID_repl: goto L_bidjmp_5524; case BID_reverse: goto L_bidjmp_5531; case BID_map: goto L_bidjmp_5537; case BID_trim: goto L_bidjmp_5567; case BID_getenv: goto L_bidjmp_5576; case BID_collect: goto L_bidjmp_5584; case BID_left: goto L_bidjmp_5585; case BID_right: goto L_bidjmp_5613; case BID_center: goto L_bidjmp_5639; case BID_detab: goto L_bidjmp_5671; case BID_entab: goto L_bidjmp_5704; case BID_abs: goto L_bidjmp_5747; case BID_max: goto L_bidjmp_5754; case BID_min: goto L_bidjmp_5765; case BID_sqrt: goto L_bidjmp_5777; case BID_atan: goto L_bidjmp_5791; case BID_log: goto L_bidjmp_5797; case BID_dtor: goto L_bidjmp_5803; case BID_rtod: goto L_bidjmp_5804; case BID_iand: goto L_bidjmp_5805; case BID_ior: goto L_bidjmp_5806; case BID_ixor: goto L_bidjmp_5807; case BID_ishift: goto L_bidjmp_5808; case BID_icom: goto L_bidjmp_5809; case BID_copy: goto L_bidjmp_5811; case BID_list: goto L_bidjmp_5840; case BID_table: goto L_bidjmp_5864; case BID_read: goto L_bidjmp_5874; case BID_reads: goto L_bidjmp_5883; case BID_runerr: goto L_bidjmp_5894; case BID_stop: goto L_bidjmp_5895; case BID_ICN_SCAN_PUSH: goto L_bidjmp_5900; case BID_ICN_SCAN_POP: goto L_bidjmp_5912; case BID_any: goto L_bidjmp_5920; case BID_many: goto L_bidjmp_5938; case BID_upto: goto L_bidjmp_5958; case BID_tab: goto L_bidjmp_5978; case BID_move: goto L_bidjmp_5991; case BID_pos: goto L_bidjmp_6004; case BID_match: goto L_bidjmp_6012; case BID_bal: goto L_bidjmp_6033; case BID_find: goto L_bidjmp_6067; case BID_NONNULL: goto L_bidjmp_6081; case BID_ICN_CASE_EQ: goto L_bidjmp_6088; case BID_ICN_SWAP_TOP2: goto L_bidjmp_6104; case BID_ICN_NULL: goto L_bidjmp_6108; case BID_insert: goto L_bidjmp_6115; case BID_delete: goto L_bidjmp_6124; case BID_member: goto L_bidjmp_6132; case BID_key: goto L_bidjmp_6140; case BID___apply__: goto L_bidjmp_6149; case BID_push: goto L_bidjmp_6162; case BID_put: goto L_bidjmp_6182; case BID_get: goto L_bidjmp_6206; case BID_pop: goto L_bidjmp_6221; case BID_pull: goto L_bidjmp_6236; case BID_sort: goto L_bidjmp_6249; case BID_FIELD_GET: goto L_bidjmp_6322; case BID_FIELD_SET: goto L_bidjmp_6330; case BID_MAKELIST: goto L_bidjmp_6341; case BID_RECORD_REGISTER: goto L_bidjmp_6345; case BID_RECORD_MAKE: goto L_bidjmp_6351; case BID_open: goto L_bidjmp_6361; case BID_remove: goto L_bidjmp_6376; case BID_close: goto L_bidjmp_6381; case BID_IDENTICAL: goto L_bidjmp_6411; case BID_set: goto L_bidjmp_6423; case BID_ASGN: goto L_bidjmp_6439; case BID_name: goto L_bidjmp_6446; case BID_variable: goto L_bidjmp_6462; case BID_SNOx24NAME: goto L_bidjmp_6468; case BID_ARRAY: goto L_bidjmp_6469; case BID_TABLE: goto L_bidjmp_6479; case BID_ITEM: goto L_bidjmp_6484; case BID_PROTOTYPE: goto L_bidjmp_6490; case BID_CONVERT: goto L_bidjmp_6499; case BID_DATA: goto L_bidjmp_6521; case BID_SNOx24KWSET: goto L_bidjmp_6528; case BID_SNOx24STMT: goto L_bidjmp_6534; case BID_SNOx24MKEXPR: goto L_bidjmp_6540; case BID_SNOx24PBK: goto L_bidjmp_6545; case BID_SNOx24PBN: goto L_bidjmp_6546; case BID_SNOx24PB0: goto L_bidjmp_6547; case BID_SNOx24PBC: goto L_bidjmp_6548; case BID_SNOx24PCUR: goto L_bidjmp_6549; case BID_SNOx24PBALT: goto L_bidjmp_6550; case BID_SNOx24PARB: goto L_bidjmp_6551; case BID_SNOx24PFEN: goto L_bidjmp_6552; case BID_SNOx24PDEF: goto L_bidjmp_6553; case BID_SNOx24MKPAT: goto L_bidjmp_6554; case BID_OPSYN: goto L_bidjmp_6563; case BID_CODE: goto L_bidjmp_6567; case BID_EVAL: goto L_bidjmp_6572; case BID_VALUE: goto L_bidjmp_6581; case BID_SNOx24NRET: goto L_bidjmp_6587; case BID_SNOx24WANTNM: goto L_bidjmp_6588; case BID_APPLY: goto L_bidjmp_6589; default: break; }
    L_bidjmp_5076: ;
    if ((_bid == BID___rk_bool) && nargs == 1) {
        DESCR_t v = args[0];
        int t = 0;
        if (IS_FAIL_fn(v))  t = 0;
        else if (IS_INT_fn(v))  t = (v.i != 0);
        else if (IS_REAL_fn(v)) t = (v.r != 0.0);
        else if (v.v == DT_SNUL) t = 0;
        else { const char *s = v.s ? v.s : ""; t = (s[0] != '\0' && !(s[0]=='0' && s[1]=='\0')); }
        if (!t) { *out = FAILDESCR; return 1; }
        *out = v; return 1;
    }
    L_bidjmp_5087: ;
    if ((_bid == BID___rk_defined) && nargs == 1) {
        DESCR_t v = args[0]; int t = (!IS_FAIL_fn(v)) && (v.v != DT_SNUL); *out = t ? INTVAL(1) : FAILDESCR; return 1;
    }
    L_bidjmp_5090: ;
    if ((_bid == BID___rk_dor) && nargs == 2) {
        DESCR_t a = args[0]; int def = (!IS_FAIL_fn(a)) && (a.v != DT_SNUL); *out = def ? a : args[1]; return 1;
    }
    L_bidjmp_5093: ;
    if ((_bid == BID___rk_bool_val) && nargs == 1) {
        DESCR_t v = args[0];
        int t = 0;
        if (IS_FAIL_fn(v))  t = 0;
        else if (IS_INT_fn(v))  t = (v.i != 0);
        else if (IS_REAL_fn(v)) t = (v.r != 0.0);
        else if (v.v == DT_SNUL) t = 0;
        else { const char *s = v.s ? v.s : ""; t = (s[0] != '\0' && !(s[0]=='0' && s[1]=='\0')); }
        *out = INTVAL(t); return 1;
    }
    L_bidjmp_5103: ;
    if ((_bid == BID___pas_chr) && nargs == 1) {
        long long cv = IS_INT_fn(args[0]) ? args[0].i : 0;
        if (cv < 0) cv = 0; if (cv > 255) cv = 255;
        char *s = (char *)rt_ws_alloc(2); s[0] = (char)(unsigned char)cv; s[1] = '\0';
        *out = (DESCR_t){ .v = DT_S, .s = s }; return 1;
    }
    L_bidjmp_5109: ;
    if ((_bid == BID___pas_chrlit) && nargs == 1) {
        *out = IS_INT_fn(args[0]) ? args[0] : INTVAL(0); return 1;
    }
    L_bidjmp_5112: ;
    if ((_bid == BID___pas_enum_name) && nargs == 2) {
        long long ord = IS_INT_fn(args[0]) ? args[0].i : (IS_REAL_fn(args[0]) ? (long long)args[0].r : 0);
        const char *csv = VARVAL_fn(args[1]); if (!csv) csv = "";
        const char *p = csv; long long k = 0;
        while (*p && k < ord) { if (*p == ',') k++; p++; }
        const char *st = p; while (*p && *p != ',') p++;
        size_t L = (size_t)(p - st); char *s = (char *)rt_ws_alloc(L + 1); memcpy(s, st, L); s[L] = '\0';
        *out = (DESCR_t){ .v = DT_S, .s = s }; return 1;
    }
    L_bidjmp_5121: ;
    if ((_bid == BID___pas_read_i) && nargs == 0) {
        long long v = 0; scanf(" %lld", &v); *out = INTVAL(v); return 1;
    }
    L_bidjmp_5124: ;
    if ((_bid == BID___pas_read_c) && nargs == 0) {
        int c = getchar(); if (c == EOF) c = 26; *out = INTVAL((long long)(unsigned char)c); return 1;
    }
    L_bidjmp_5127: ;
    if ((_bid == BID___pas_readln) && nargs == 0) {
        int c; while ((c = getchar()) != '\n' && c != EOF) (void)c; *out = NULVCL; return 1;
    }
    L_bidjmp_5130: ;
    if ((_bid == BID___pas_eof) && nargs == 0) {
        int c = getchar();
        if (c == EOF) { *out = INTVAL(1); return 1; }
        ungetc(c, stdin); *out = INTVAL(0); return 1;
    }
    L_bidjmp_5135: ;
    if ((_bid == BID___pas_eoln) && nargs == 0) {
        int c = getchar();
        if (c == EOF || c == '\n') { if (c != EOF) ungetc(c, stdin); *out = INTVAL(1); return 1; }
        ungetc(c, stdin); *out = INTVAL(0); return 1;
    }
    if (((_bid == BID___pas_read_i_f) || (_bid == BID___pas_read_c_f) || (_bid == BID___pas_readln_f) || (_bid == BID___pas_eof_f) || (_bid == BID___pas_eoln_f) || (_bid == BID___pas_getbufch_f)) && nargs == 1) {
        extern FILE *fh_get(int); FILE *f = IS_FH_fn(args[0]) ? fh_get((int)args[0].i) : NULL; if (!f) f = stdin;
        if ((_bid == BID___pas_read_i_f)) { long long v = 0; if (fscanf(f, " %lld", &v) != 1) v = 0; *out = INTVAL(v); return 1; }
        if ((_bid == BID___pas_read_c_f)) { int c = fgetc(f); if (c == EOF) c = 26; *out = INTVAL((long long)(unsigned char)c); return 1; }
        if ((_bid == BID___pas_readln_f)) { int c; while ((c = fgetc(f)) != '\n' && c != EOF) (void)c; *out = NULVCL; return 1; }
        if ((_bid == BID___pas_eof_f)) { int c = fgetc(f); if (c == EOF) { *out = INTVAL(1); return 1; } ungetc(c, f); *out = INTVAL(0); return 1; }
        if ((_bid == BID___pas_eoln_f)) { int c = fgetc(f); if (c == EOF || c == '\n') { if (c != EOF) ungetc(c, f); *out = INTVAL(1); return 1; } ungetc(c, f); *out = INTVAL(0); return 1; }
        { int c = fgetc(f); if (c == EOF) { *out = INTVAL((long long)' '); return 1; } ungetc(c, f); *out = INTVAL((long long)(unsigned char)c); return 1; }
    }
    L_bidjmp_5149: ;
    if ((_bid == BID___pas_getbufch) && nargs == 0) {
        int c = getchar(); if (c == EOF) { *out = INTVAL((long long)' '); return 1; } ungetc(c, stdin); *out = INTVAL((long long)(unsigned char)c); return 1;
    }
    L_bidjmp_5152: ;
    if ((_bid == BID___pas_trunc) && nargs == 1) {
        double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
        *out = INTVAL((long long)d); return 1;
    }
    L_bidjmp_5156: ;
    if ((_bid == BID___pas_abs) && nargs == 1) {
        if (IS_REAL_fn(args[0])) { double d = args[0].r; DESCR_t r; r.v = DT_R; r.r = d < 0 ? -d : d; *out = r; return 1; }
        long long v = IS_INT_fn(args[0]) ? args[0].i : 0; *out = INTVAL(v < 0 ? -v : v); return 1;
    }
    if (nargs == 1 && ((_bid == BID___pas_sin)||(_bid == BID___pas_cos)||(_bid == BID___pas_exp)||(_bid == BID___pas_sqrt)||(_bid == BID___pas_ln)||(_bid == BID___pas_arctan))) {
        double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
        double r;
        if      ((_bid == BID___pas_sin))    r = sin(d);
        else if ((_bid == BID___pas_cos))    r = cos(d);
        else if ((_bid == BID___pas_exp))    r = exp(d);
        else if ((_bid == BID___pas_sqrt))   r = sqrt(d);
        else if ((_bid == BID___pas_ln))     r = log(d);
        else                                 r = atan(d);
        DESCR_t rv; rv.v = DT_R; rv.r = r; *out = rv; return 1;
    }
    L_bidjmp_5171: ;
    if ((_bid == BID___pas_writeln) || (_bid == BID___pas_write)) {
        int nl = (fn[6] == 'w' && fn[7] == 'r' && fn[8] == 'i' && fn[9] == 't' && fn[10] == 'e' && fn[11] == 'l');
        int _start = 0;
        FILE *_dest = stdout;
        if (nargs >= 1 && IS_FH_fn(args[0])) { extern FILE *fh_get(int); FILE *_fp = fh_get((int)args[0].i); if (_fp) _dest = _fp; _start = 2; }
        for (int _pi = _start; _pi + 1 < nargs; _pi += 2) {
            DESCR_t av = args[_pi];
            DESCR_t aw = args[_pi + 1];
            int w = IS_INT_fn(aw) ? (aw.i == -3 ? -3 : (aw.i >= 0 ? (int)aw.i : -1)) : -1;
            if (w == -3) { double _rv = IS_REAL_fn(av) ? av.r : (IS_INT_fn(av) ? (double)av.i : 0.0);
                long _fw2 = 0, _fp2 = 0;
                if (_pi + 3 < nargs) { if (IS_INT_fn(args[_pi+2])) _fw2 = (long)args[_pi+2].i; if (IS_INT_fn(args[_pi+3])) _fp2 = (long)args[_pi+3].i; }
                if (_fw2 < 0) _fw2 = 0; if (_fp2 < 0) _fp2 = 0; if (_fp2 > 16) _fp2 = 16;
                fprintf(_dest, "%*.*f", (int)_fw2, (int)_fp2, _rv); _pi += 2; continue; }
            if (IS_INT_fn(av)) {
                char _pb[32];
                int _pfmtlen = snprintf(_pb, sizeof _pb, "%lld", (long long)av.i);
                int _fw = (w < 0) ? 11 : (w > _pfmtlen ? w : _pfmtlen);
                fprintf(_dest, "%*s", _fw, _pb);
            } else if (IS_REAL_fn(av)) {
                char _rb[64];
                int _prec = (w < 0) ? 12 : (w - 8 < 1 ? 1 : (w - 8 > 16 ? 16 : w - 8));
                pas_real_str(av.r, _rb, sizeof _rb, _prec);
                int _pfmtlen = (int)strlen(_rb);
                int _fw = (w < 0) ? 20 : (_pfmtlen + 1 > w ? _pfmtlen + 1 : w);
                fprintf(_dest, "%*s", _fw, _rb);
            } else {
                const char *_ps = VARVAL_fn(av);
                if (_ps) {
                    if (w == -2) { fprintf(_dest, "%c", (int)(unsigned char)_ps[0]); }
                    else if (w >= 0) { fprintf(_dest, "%*s", w, _ps); }
                    else { fputs(_ps, _dest); }
                }
            }
        }
        if (nl) fputc('\n', _dest);
        *out = NULVCL; return 1;
    }
    L_bidjmp_5209: ;
    if ((_bid == BID_write) || (_bid == BID_writes)) {
        { extern int g_icon_write_reassignable; if (!g_icon_write_reassignable) {  goto L_write_body_5209; } }
        { DESCR_t _wv = NV_GET_fn(fn);
          int _is_self_default = (_wv.v == DT_E && _wv.slen == 0xFFFFFFFEu && _wv.s && !strcmp(_wv.s, fn));
          if (!_is_self_default) { *out = rt_call_value(_wv, args, nargs); return 1; } }
        L_write_body_5209: ;
        int nl = (fn[5] == '\0');
        int start = 0;
        FILE *dest = stdout;
        if (nargs > 0 && IS_FH_fn(args[0])) {
            FILE *fp = fh_get((int)args[0].i);
            if (fp) { if (nl && fp != stdout) fflush(stdout); dest = fp; }
            start = 1;
        }
        for (int _wi = start; _wi < nargs; _wi++) {
            DESCR_t av = args[_wi];
            if (IS_FAIL_fn(av)) { *out = FAILDESCR; return 1; }
            if (av.v == DT_SNUL) continue;
            if (dest != stdout && dest != stderr && IS_STR_fn(av)) { const char *_bs = VARVAL_fn(av); uint32_t _bn = av.slen ? av.slen : (_bs ? (uint32_t)strlen(_bs) : 0u); if (_bs && _bn) fwrite(_bs, 1, _bn, dest); continue; }
            out_write_descr(dest, av, nl);
        }
        if (nl) fputc('\n', dest);
        *out = nargs > start ? args[nargs-1] : (nargs > 0 ? args[0] : NULVCL);
        return 1;
    }
    L_bidjmp_5232: ;
    if ((_bid == BID_integer) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_INT_fn(av))  { *out = av; return 1; }
        if (IS_REAL_fn(av)) { *out = INTVAL((long long)av.r); return 1; }
        const char *s = VARVAL_fn(av); if (!s) { *out = FAILDESCR; return 1; }
        {
            const char *p = s;
            while (*p == ' ' || *p == '\t') p++;
            int neg = 0; if (*p == '+') p++; else if (*p == '-') { neg = 1; p++; }
            int base = 0; const char *bstart = p;
            while (*p >= '0' && *p <= '9') { base = base * 10 + (*p - '0'); p++; }
            if (p > bstart && (*p == 'r' || *p == 'R') && base >= 2 && base <= 36) {
                p++; const char *dstart = p; long long v = 0;
                while (*p) {
                    int d = -1;
                    if (*p >= '0' && *p <= '9') d = *p - '0';
                    else if (*p >= 'a' && *p <= 'z') d = *p - 'a' + 10;
                    else if (*p >= 'A' && *p <= 'Z') d = *p - 'A' + 10;
                    if (d < 0 || d >= base) break;
                    v = v * base + d;
                    p++;
                }
                while (*p == ' ' || *p == '\t') p++;
                if (p > dstart && *p == '\0') { *out = INTVAL(neg ? -v : v); return 1; }
            }
        }
        char *end; long long iv = strtoll(s, &end, 10);
        if (end != s && (*end=='\0'||*end==' ')) { *out = INTVAL(iv); return 1; }
        double rv = strtod(s, &end);
        if (end != s && (*end=='\0'||*end==' ')) { *out = INTVAL((long long)rv); return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_5264: ;
    if ((_bid == BID_real) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_REAL_fn(av)) { *out = av; return 1; }
        if (IS_INT_fn(av))  { *out = REALVAL((double)av.i); return 1; }
        const char *s = VARVAL_fn(av); if (!s) { *out = FAILDESCR; return 1; }
        char *end; double rv = strtod(s, &end);
        if (end != s && (*end=='\0'||*end==' ')) { *out = REALVAL(rv); return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_5273: ;
    if ((_bid == BID_string) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_CSET_fn(av)) { *out = rt_str_coerce(av); return 1; }
        if (IS_STR_fn(av)) { *out = av; return 1; }
        char *buf = rt_ws_alloc(64);
        if (IS_INT_fn(av))       snprintf(buf,64,"%lld",(long long)av.i);
        else if (IS_REAL_fn(av)) { icon_real_str(av.r,buf,64); }
        else { *out = NULVCL; return 1; }
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5283: ;
    if ((_bid == BID_numeric) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_INT_fn(av)||IS_REAL_fn(av)) { *out = av; return 1; }
        const char *s = VARVAL_fn(av); if (!s||!*s) { *out = FAILDESCR; return 1; }
        {
            const char *p = s;
            while (*p == ' ' || *p == '\t') p++;
            int neg = 0; if (*p == '+') p++; else if (*p == '-') { neg = 1; p++; }
            int base = 0; const char *bstart = p;
            while (*p >= '0' && *p <= '9') { base = base * 10 + (*p - '0'); p++; }
            if (p > bstart && (*p == 'r' || *p == 'R') && base >= 2 && base <= 36) {
                p++; const char *dstart = p; long long v = 0;
                while (*p) {
                    int d = -1;
                    if (*p >= '0' && *p <= '9') d = *p - '0';
                    else if (*p >= 'a' && *p <= 'z') d = *p - 'a' + 10;
                    else if (*p >= 'A' && *p <= 'Z') d = *p - 'A' + 10;
                    if (d < 0 || d >= base) break;
                    v = v * base + d;
                    p++;
                }
                while (*p == ' ' || *p == '\t') p++;
                if (p > dstart && *p == '\0') { *out = INTVAL(neg ? -v : v); return 1; }
            }
        }
        char *end; long long iv = strtoll(s, &end, 10);
        if (end != s && (*end=='\0'||*end==' ')) { *out = INTVAL(iv); return 1; }
        double rv = strtod(s, &end);
        if (end != s && (*end=='\0'||*end==' ')) { *out = REALVAL(rv); return 1; }
        *out = FAILDESCR; return 1;
    }
    if (((_bid == BID_char) || (_bid == BID_chr)) && nargs == 1) {
        DESCR_t av = args[0];
        int n = (int)(IS_INT_fn(av) ? av.i : (long long)strtol(VARVAL_fn(av)?VARVAL_fn(av):"0",NULL,10));
        char *buf = rt_ws_alloc(2); buf[0]=(char)(n&0xFF); buf[1]='\0';
        *out = BSTRVAL(buf, 1); return 1;
    }
    L_bidjmp_5320: ;
    if ((_bid == BID_cset) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_CSET_fn(av)) { *out = av; return 1; }
        char _cbuf[64];
        const char *raw;
        if (IS_INT_fn(av))       { snprintf(_cbuf,sizeof _cbuf,"%lld",(long long)av.i); raw=_cbuf; }
        else if (IS_REAL_fn(av)) { icon_real_str(av.r,_cbuf,sizeof _cbuf); raw=_cbuf; }
        else { raw = VARVAL_fn(av); if (!raw) raw = ""; }
        *out = CSETVAL(cset_canonical(raw)); return 1;
    }
    L_bidjmp_5330: ;
    if ((_bid == BID_ord) && nargs == 1) {
        DESCR_t av = args[0];
        const char *s = VARVAL_fn(av);
        /* NOT !*s (icon-ascii-cset-keywords-built-off-by-one): a legitimate one-character result whose
           character IS chr(0) has *s==0 despite being non-empty; descr_slen(av) reads the descriptor's
           own stamped length instead of testing the byte content for the C-string "empty" convention. */
        if (!s || !descr_slen(av)) { *out = FAILDESCR; return 1; }
        *out = INTVAL((unsigned char)s[0]); return 1;
    }
    if (((_bid == BID_type) || (_bid == BID_DATATYPE)) && nargs == 1) return bn_type_datatype(fn, args, nargs, out);
    L_bidjmp_5336: ;
    if ((_bid == BID_image) && nargs == 0) {
        *out = STRVAL("&null"); return 1;
    }
    L_bidjmp_5339: ;
    if ((_bid == BID_args) && nargs == 1) {
        DESCR_t a = args[0];
        if (a.v == DT_E && a.slen == 0xFFFFFFFEu) {
            static const struct { const char *nm; int np; } _bt[] = {
                {"push",-2},{"put",-2},{"insert",-2},{"delete",-2},
                {"pop",1},{"get",1},{"pull",1},{"bal",3},{"find",3},{"upto",3},
                {"any",3},{"many",3},{"match",3},{"tab",1},{"move",1},{"pos",1},
                {"write",-1},{"writes",-1},{"writef",-1},
                {"read",0},{"reads",1},{"close",1},{"open",2},{"flush",1},
                {"image",1},{"type",1},{"copy",1},{"sort",2},{"sortf",2},
                {"reverse",1},{"size",1},{"left",3},{"right",3},{"center",3},
                {"string",1},{"integer",1},{"real",1},{"char",1},{"ord",1},
                {"trim",2},{"map",3},{"repl",2},{"cset",1},{"list",2},
                {"table",1},{"set",1},{"key",1},{"member",2},
                {"proc",2},{"args",1},{"name",1},{"variable",1},
                {"abs",1},{"sqrt",1},{"sin",1},{"cos",1},{"atan",2},
                {"exp",1},{"log",2},{"max",-1},{"min",-1},
                {"iand",2},{"ior",2},{"ixor",2},{"icom",1},{"ishift",2},
                {"numeric",1},{"entab",-1},{"detab",-1},{"seq",2},
                {"collect",2},{"display",2},{"runerr",2},{"errorclear",0},
                {"function",0},{"serial",1},{"system",1},{"exit",1},
                {NULL,0}};
            int np = rt_proc_nparams(a.s);
            if (np < 0 && a.s) for (int _bi=0;_bt[_bi].nm;_bi++) if (!strcmp(_bt[_bi].nm,a.s)){np=_bt[_bi].np;break;}
            if (np < 0 && a.s && !strcmp(a.s, "main")) {
                np = 0;
                for (int i = 0; i < g_stage2.proc_count; i++)
                    if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, "main")) { np = g_stage2.proc_table[i].nparams; break; }
            }
            if (np >= 0) { *out = INTVAL(np); return 1; }
            *out = INTVAL(np < -1 ? np : -1); return 1;
        }
        if (a.v == DT_E) {
            for (int i=0;i<g_stage2.proc_count;i++) {
                if (g_stage2.proc_table[i].entry_pc == (int)a.i) {
                    *out = INTVAL(g_stage2.proc_table[i].nparams <= 0 ? -2 : g_stage2.proc_table[i].nparams);
                    return 1;
                }
            }
            *out = INTVAL(-2); return 1;
        }
        if (IS_STR_fn(a)) {
            *out = INTVAL(-2); return 1;
        }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_5385: ;
    if ((_bid == BID_proc) && (nargs == 2 || nargs == 1)) {
        const char *pname = VARVAL_fn(args[0]);
        int arity = (nargs >= 2) ? (int)to_int(args[1]) : -1;
        if (!pname) { *out = FAILDESCR; return 1; }
        for (int i = 0; i < g_stage2.proc_count; i++) {
            if (g_stage2.proc_table[i].name && strcmp(g_stage2.proc_table[i].name, pname) == 0) {
                if (arity < 0 || g_stage2.proc_table[i].nparams == arity || g_stage2.proc_table[i].nparams <= 0) {
                    extern DESCR_t rt_proc_value(const char *);
                    *out = rt_proc_value(g_stage2.proc_table[i].name); return 1;
                }
            }
        }
        { extern int rt_proc_is_registered(const char *name); extern int rt_proc_nparams(const char *name);
          if (rt_proc_is_registered(pname)) { int np = rt_proc_nparams(pname);
              if (arity < 0 || np == arity || np <= 0) { extern DESCR_t rt_proc_value(const char *); *out = rt_proc_value(rt_ws_strdup(pname)); return 1; } } }
        if (icn_builtin_is_known(pname) || rt_builtin_is_known(pname)) {
            DESCR_t bv; bv.v = DT_E; bv.slen = 0xFFFFFFFEu; bv.s = rt_ws_strdup(pname); *out = bv; return 1;
        }
        { static const char *op2[] = { "+","-","*","/","%","^","||","|||","++","--","**","<","<=",">",">=","=","~=","<<","<<=",">>",">>=","==","~==","===","~===", 0 };
          static const char *op1[] = { "+","-","*","/","\\","=","?","~","!","@","^", 0 };
          const char **tbl = (arity == 2 || arity == 3) ? op2 : (arity == 1 || arity < 0) ? op1 : 0;
          if (tbl) for (int oi = 0; tbl[oi]; oi++) if (!strcmp(tbl[oi], pname)) { DESCR_t bv; bv.v = DT_E; bv.slen = 0xFFFFFFFEu; bv.s = rt_ws_strdup(pname); *out = bv; return 1; } }
        *out = FAILDESCR; return 1;
    }
    if ((_bid == BID_image) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_FAIL_fn(av)) { *out = FAILDESCR; return 1; }
        char *buf = rt_ws_alloc(256);
        if (av.v == DT_SNUL)     { *out = STRVAL("&null"); return 1; }
        if (av.v == DT_E) {
            const char *nm = procval_name(av);
            if (!nm) nm = "?";
            snprintf(buf,256, (rt_proc_is_registered(nm) || !strcmp(nm, "main")) ? "procedure %s" : "function %s", nm);
            *out = STRVAL(buf); return 1;
        }
        if (IS_FH_fn(av)) {
            int idx = (int)av.i;
            if (idx == 0) { snprintf(buf,256,"&input");  *out = STRVAL(buf); return 1; }
            if (idx == 1) { snprintf(buf,256,"&output"); *out = STRVAL(buf); return 1; }
            if (idx == 2) { snprintf(buf,256,"&errout"); *out = STRVAL(buf); return 1; }
            if (idx >= 0 && idx < FH_MAX && fh_name[idx]) {
                snprintf(buf,256,"file(%s)",fh_name[idx]);
                *out = STRVAL(buf); return 1;
            }
            snprintf(buf,256,"file(?)"); *out = STRVAL(buf); return 1;
        }
        if (IS_INT_fn(av)) {
            snprintf(buf,256,"%lld",(long long)av.i); *out = STRVAL(buf); return 1;
        }
        if (IS_REAL_fn(av))      { icon_real_str(av.r,buf,128); *out = STRVAL(buf); return 1; }
        if (av.v==DT_T)          { snprintf(buf,128,"table(%d)",av.tbl?av.tbl->size:0); *out = STRVAL(buf); return 1; }
        if (av.v==DT_DATA && av.u) {
            const char *tname = av.u->type ? av.u->type->name : "record";
            if (strcmp(tname,"list")==0) {
                int cnt = (av.u->type && av.u->type->nfields>=2 && av.u->fields)
                          ? (int)av.u->fields[1].i : 0;
                snprintf(buf,128,"list(%d)",cnt); *out = STRVAL(buf); return 1;
            }
            { int nf = (av.u->type ? av.u->type->nfields : 0); long id = rt_record_image_id(av.u); snprintf(buf,256,"record %s_%ld(%d)",tname,id,nf); *out = STRVAL(buf); return 1; }
        }
        if (av.v==DT_DATA)       { *out = STRVAL("record"); return 1; }
        if (av.v==DT_E) {
            for (int i=0;i<g_stage2.proc_count;i++)
                if (g_stage2.proc_table[i].entry_pc==(int)av.i)
                    { snprintf(buf,128,"procedure %s",g_stage2.proc_table[i].name); *out=STRVAL(buf); return 1; }
            snprintf(buf,128,"procedure"); *out=STRVAL(buf); return 1;
        }
        if (IS_CSET_fn(av)) {
            const char *cs = av.s ? av.s : "";
            const char *kname = kw_cset_name(cs);
            if (kname) { *out = STRVAL(kname); return 1; }
            int cslen = (int)strlen(cs);
            char *outs = rt_ws_alloc(cslen * 4 + 3);
            int o = 0;
            outs[o++] = '\'';
            for (int i = 0; i < cslen; i++) {
                unsigned char c = (unsigned char)cs[i];
                if (c == '\'') { outs[o++] = '\\'; outs[o++] = '\''; }
                else if (c < 0x20 || c >= 0x7f) { o += snprintf(outs+o, 5, "\\x%02x", c); }
                else outs[o++] = (char)c;
            }
            outs[o++] = '\'';
            outs[o] = '\0';
            *out = STRVAL(outs); return 1;
        }
        if (IS_STR_fn(av) && av.s) {
        }
        const char *s=VARVAL_fn(av); if (!s) s = "";
        int sl = (int)strlen(s);
        char *outs = rt_ws_alloc(sl*4 + 3);
        int o = 0;
        outs[o++] = '"';
        for (int i = 0; i < sl; i++) {
            unsigned char c = (unsigned char)s[i];
            switch (c) {
                case '"':  outs[o++]='\\'; outs[o++]='"';  break;
                case '\\': outs[o++]='\\'; outs[o++]='\\'; break;
                case '\n': outs[o++]='\\'; outs[o++]='n';  break;
                case '\t': outs[o++]='\\'; outs[o++]='t';  break;
                case '\r': outs[o++]='\\'; outs[o++]='r';  break;
                default:
                    if (c < 0x20 || c == 0x7f) {
                        o += snprintf(outs+o, 5, "\\x%02x", c);
                    } else {
                        outs[o++] = (char)c;
                    }
            }
        }
        outs[o++] = '"';
        outs[o] = '\0';
        *out = STRVAL(outs); return 1;
    }
    if ((_bid == BID_image) && nargs >= 2) {
        DESCR_t av = args[0];
        if (0 && IS_STR_fn(av) && av.s) { }
        DESCR_t one_out = FAILDESCR;
        if (try_call_builtin_by_name("image", args, 1, &one_out))
            { *out = one_out; return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_5524: ;
    if ((_bid == BID_repl) && nargs == 2) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        int n=(int)to_int(args[1]); if(n<0)n=0;
        int sl=(int)strlen(s); char *buf=rt_ws_alloc(sl*n+1); buf[0]='\0';
        for(int i=0;i<n;i++) memcpy(buf+i*sl,s,sl); buf[sl*n]='\0';
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5531: ;
    if ((_bid == BID_reverse) && nargs == 1) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        int sl=(int)strlen(s); char *buf=rt_ws_alloc(sl+1);
        for(int i=0;i<sl;i++) buf[i]=s[sl-1-i]; buf[sl]='\0';
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5537: ;
    if ((_bid == BID_map) && nargs >= 1 && nargs <= 3) {
        static const char *UCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static const char *LCASE = "abcdefghijklmnopqrstuvwxyz";
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        const char *from = UCASE, *to = LCASE;
        if (nargs >= 2) {
            DESCR_t fv=args[1];
            if (fv.v != DT_SNUL) {
                const char *fs = VARVAL_fn(fv);
                if (fs) from = fs;
            }
        }
        if (nargs >= 3) {
            DESCR_t tv=args[2];
            if (tv.v != DT_SNUL) {
                const char *ts = VARVAL_fn(tv);
                if (ts) to = ts;
            }
        }
        int sl=(int)strlen(s); char *buf=rt_ws_alloc(sl+1);
        int fl=(int)strlen(from), tl=(int)strlen(to);
        for (int i=0;i<sl;i++) {
            char c=s[i]; int hit=0;
            for (int j=fl-1;j>=0;j--) {
                if (from[j]==c) { buf[i] = (j<tl) ? to[j] : c; hit=1; break; }
            }
            if (!hit) buf[i]=c;
        }
        buf[sl]='\0'; *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5567: ;
    if ((_bid == BID_trim) && (nargs == 1 || nargs == 2)) {
        const char *s=VARVAL_fn(args[0]); if(!s)s="";
        const char *cset = " ";
        if (nargs == 2) { DESCR_t cv = args[1]; if (cv.v != DT_SNUL) { const char *cs = VARVAL_fn(cv); if (cs) cset = cs; } }
        int sl=(int)strlen(s);
        while (sl > 0 && strchr(cset, s[sl-1])) sl--;
        char *buf=rt_ws_alloc(sl+1); memcpy(buf,s,sl); buf[sl]='\0';
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5576: ;
    if ((_bid == BID_getenv) && nargs == 1) {
        const char *name = VARVAL_fn(args[0]);
        if (!name) { *out = FAILDESCR; return 1; }
        const char *val = getenv(name);
        if (!val) { *out = FAILDESCR; return 1; }
        size_t vl = strlen(val); char *buf = rt_ws_alloc(vl+1); memcpy(buf, val, vl); buf[vl] = '\0';
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5584: ;
    if ((_bid == BID_collect) && nargs <= 2) { extern long rt_gc_collect(void); rt_gc_collect(); *out = NULVCL; return 1; }
    L_bidjmp_5585: ;
    if ((_bid == BID_left) && nargs >= 1) {
        char _pb[64]; const char *s=icn_pad_str(args[0],_pb,sizeof _pb); if(!s)s="";
        int sl=(int)strlen(s);
        int n = 1;
        if (nargs >= 2) {
            DESCR_t nv = args[1];
            if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv);
        }
        if (n < 0) n = 0;
        const char *fill=" "; int fl=1;
        if (nargs >= 3) {
            DESCR_t fd = args[2];
            if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) {
                const char *fs = VARVAL_fn(fd);
                if (fs && *fs) { fill = fs; fl = (int)strlen(fs); }
            }
        }
        char *buf=rt_ws_alloc(n+1);
        int copy = sl < n ? sl : n;
        for (int i = 0; i < copy; i++) buf[i] = s[i];
        int rpad = n - copy;
        for (int k = 0; k < rpad; k++) {
            int idx = ((k + fl - rpad) % fl + fl) % fl;
            buf[copy + k] = fill[idx];
        }
        buf[n]='\0';
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5613: ;
    if ((_bid == BID_right) && nargs >= 1) {
        char _pb[64]; const char *s=icn_pad_str(args[0],_pb,sizeof _pb); if(!s)s="";
        int sl=(int)strlen(s);
        int n = 1;
        if (nargs >= 2) {
            DESCR_t nv = args[1];
            if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv);
        }
        if (n < 0) n = 0;
        const char *fill=" "; int fl=1;
        if (nargs >= 3) {
            DESCR_t fd = args[2];
            if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) {
                const char *fs = VARVAL_fn(fd);
                if (fs && *fs) { fill = fs; fl = (int)strlen(fs); }
            }
        }
        char *buf=rt_ws_alloc(n+1);
        int pad = n - sl; if (pad < 0) pad = 0;
        for (int i = 0; i < pad; i++) buf[i] = fill[i % fl];
        int srcoff = (sl > n) ? (sl - n) : 0;
        int copy = sl - srcoff; if (pad + copy > n) copy = n - pad;
        for (int i = 0; i < copy; i++) buf[pad + i] = s[srcoff + i];
        buf[n]='\0';
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5639: ;
    if ((_bid == BID_center) && nargs >= 1) {
        char _pb[64]; const char *s=icn_pad_str(args[0],_pb,sizeof _pb); if(!s)s="";
        int sl=(int)strlen(s);
        int n = 1;
        if (nargs >= 2) {
            DESCR_t nv = args[1];
            if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv);
        }
        if (n < 0) n = 0;
        const char *fill=" "; int fl=1;
        if (nargs >= 3) {
            DESCR_t fd = args[2];
            if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) {
                const char *fs = VARVAL_fn(fd);
                if (fs && *fs) { fill = fs; fl = (int)strlen(fs); }
            }
        }
        char *buf=rt_ws_alloc(n+1);
        int lpad = (n - sl) / 2; if (lpad < 0) lpad = 0;
        int srcoff = (sl > n) ? (sl - n + 1) / 2 : 0;
        int copy = sl - srcoff; if (lpad + copy > n) copy = n - lpad;
        int rpad = n - lpad - copy;
        for (int i = 0; i < lpad; i++) buf[i] = fill[i % fl];
        for (int i = 0; i < copy; i++) buf[lpad + i] = s[srcoff + i];
        for (int k = 0; k < rpad; k++) {
            int idx = ((k + fl - rpad) % fl + fl) % fl;
            buf[lpad + copy + k] = fill[idx];
        }
        buf[n]='\0';
        *out = STRVAL(buf); return 1;
    }
    if (((_bid == BID_detab) || (_bid == BID_entab)) && nargs == 0) { core_icn_error(103, NULVCL); *out = FAILDESCR; return 1; }
    L_bidjmp_5671: ;
    if ((_bid == BID_detab) && nargs == 0) { core_icn_error(103, NULVCL); *out = FAILDESCR; return 1; }
    if ((_bid == BID_detab) && nargs >= 1) {
        if (args[0].v == DT_I || args[0].v == DT_R) { *out = FAILDESCR; return 1; }
        if (args[0].v == DT_A || args[0].v == DT_T || args[0].v == DT_DATA) { core_icn_error(103, args[0]); *out = FAILDESCR; return 1; }
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        int stops[32], nstops = 0;
        for (int j = 1; j < nargs && nstops < 32; j++) {
            if (IS_FAIL_fn(args[j]) || args[j].v == DT_SNUL) continue;
            if (!IS_INT_fn(args[j]) && !IS_REAL_fn(args[j])) {
                int _cok = 0;
                if (args[j].v == DT_S && args[j].s) { const char *_p = args[j].s; while (*_p == ' ') _p++; if (*_p == '+' || *_p == '-') _p++; int _d = 0; while (*_p >= '0' && *_p <= '9') { _p++; _d = 1; } while (*_p == ' ') _p++; _cok = _d && !*_p; }
                if (!_cok) { core_icn_error(101, args[j]); *out = FAILDESCR; return 1; }
            }
            { int _ns = (int)to_int(args[j]); if (nstops > 0 && _ns <= stops[nstops-1]) { core_icn_error(210, args[j]); *out = FAILDESCR; return 1; } stops[nstops++] = _ns; }
        }
        if (nstops == 0) { stops[0] = 9; nstops = 1; }
        int gap = (nstops >= 2) ? stops[nstops-1] - stops[nstops-2] : stops[0] - 1;
        if (gap < 1) gap = 1;
        int cap = 4096; char *buf = rt_ws_alloc(cap); int bi = 0, col = 0;
        for (int i = 0; s[i]; i++) {
            if (s[i] == '\t') {
                int next = -1;
                for (int k = 0; k < nstops; k++) if (stops[k] > col+1) { next=stops[k]; break; }
                if (next < 0) {
                    int base = stops[nstops-1];
                    int beyond = col + 1 - base;
                    next = base + ((beyond / gap) + 1) * gap;
                }
                int sp = next - (col+1);
                while (sp-- > 0) { if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=' '; col++; }
            } else if (s[i] == '\b') {
                if (col > 0) col--;
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=s[i];
            } else if (s[i] == '\n' || s[i] == '\r') {
                col = 0;
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=s[i];
            } else {
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=s[i];
                if (isprint((unsigned char)s[i])) col++;
            }
        }
        buf[bi]='\0'; *out=STRVAL(buf); return 1;
    }
    L_bidjmp_5704: ;
    if ((_bid == BID_entab) && nargs == 0) { core_icn_error(103, NULVCL); *out = FAILDESCR; return 1; }
    if ((_bid == BID_entab) && nargs >= 1) {
        if (args[0].v == DT_I || args[0].v == DT_R) { *out = FAILDESCR; return 1; }
        if (args[0].v == DT_A || args[0].v == DT_T || args[0].v == DT_DATA) { core_icn_error(103, args[0]); *out = FAILDESCR; return 1; }
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        int stops[32], nstops = 0;
        for (int j = 1; j < nargs && nstops < 32; j++) {
            if (IS_FAIL_fn(args[j]) || args[j].v == DT_SNUL) continue;
            if (!IS_INT_fn(args[j]) && !IS_REAL_fn(args[j])) {
                int _cok = 0;
                if (args[j].v == DT_S && args[j].s) { const char *_p = args[j].s; while (*_p == ' ') _p++; if (*_p == '+' || *_p == '-') _p++; int _d = 0; while (*_p >= '0' && *_p <= '9') { _p++; _d = 1; } while (*_p == ' ') _p++; _cok = _d && !*_p; }
                if (!_cok) { core_icn_error(101, args[j]); *out = FAILDESCR; return 1; }
            }
            { int _ns = (int)to_int(args[j]); if (nstops > 0 && _ns <= stops[nstops-1]) { core_icn_error(210, args[j]); *out = FAILDESCR; return 1; } stops[nstops++] = _ns; }
        }
        if (nstops == 0) { stops[0] = 9; nstops = 1; }
        int gap = (nstops >= 2) ? stops[nstops-1] - stops[nstops-2] : stops[0] - 1;
        if (gap < 1) gap = 1;
        int cap = 4096; char *buf = rt_ws_alloc(cap); int bi = 0, col = 1;
        int slen = (int)strlen(s);
        for (int i = 0; i < slen; ) {
            char c = s[i];
            if (c == ' ') {
                int target = col + 1; int j = i + 1;
                while (j < slen && s[j] == ' ') { target++; j++; }
                if (target - col > 1) {
                    int nt = icn_nxttab(col, stops, nstops, gap);
                    int do_tab = 1;
                    if (nt == col + 1) { int nt1 = icn_nxttab(nt, stops, nstops, gap); if (nt1 > target) do_tab = 0; }
                    if (do_tab) {
                        while (nt <= target) { if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]='\t'; col = nt; nt = icn_nxttab(col, stops, nstops, gap); }
                        while (col++ < target) { if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=' '; }
                    } else {
                        while (col < target) { if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=' '; col++; }
                    }
                } else {
                    if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=' ';
                }
                col = target; i = j;
            } else if (c == '\t') {
                col = icn_nxttab(col, stops, nstops, gap);
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]='\t'; i++;
            } else if (c == '\b') {
                if (col > 1) col--;
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=c; i++;
            } else if (c == '\n' || c == '\r') {
                col = 1;
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=c; i++;
            } else {
                if (bi>=cap-1){cap*=2;buf=rt_ws_realloc(buf,cap);} buf[bi++]=c;
                if (isprint((unsigned char)c)) col++;
                i++;
            }
        }
        buf[bi]='\0'; *out=STRVAL(buf); return 1;
    }
    L_bidjmp_5747: ;
    if ((_bid == BID_abs) && nargs == 1) {
        extern void rt_coerce_num2_d(const DESCR_t *self, const DESCR_t *other, DESCR_t *out, long codes);
        DESCR_t av = args[0]; DESCR_t nv;
        rt_coerce_num2_d(&av, &av, &nv, 0);
        if (IS_REAL_fn(nv)) { *out = REALVAL(fabs(nv.r)); return 1; }
        *out = INTVAL(nv.i < 0 ? -nv.i : nv.i); return 1;
    }
    L_bidjmp_5754: ;
    if ((_bid == BID_max) && nargs >= 2) {
        DESCR_t best = args[0];
        for (int _j = 1; _j < nargs; _j++) {
            DESCR_t cv = args[_j];
            int gt = (IS_REAL_fn(best)||IS_REAL_fn(cv))
                ? ((IS_REAL_fn(best)?best.r:(double)best.i) < (IS_REAL_fn(cv)?cv.r:(double)cv.i))
                : (best.i < cv.i);
            if (gt) best = cv;
        }
        *out = best; return 1;
    }
    L_bidjmp_5765: ;
    if ((_bid == BID_min) && nargs >= 2) {
        DESCR_t best = args[0];
        for (int _j = 1; _j < nargs; _j++) {
            DESCR_t cv = args[_j];
            int lt = (IS_REAL_fn(best)||IS_REAL_fn(cv))
                ? ((IS_REAL_fn(best)?best.r:(double)best.i) > (IS_REAL_fn(cv)?cv.r:(double)cv.i))
                : (best.i > cv.i);
            if (lt) best = cv;
        }
        *out = best; return 1;
    }
#define TONUM(av) (IS_REAL_fn(av) ? (av).r : IS_INT_fn(av) ? (double)(av).i : ((av).v==DT_S && (av).s ? strtod((av).s,NULL) : 0.0))
    L_bidjmp_5777: ;
    if ((_bid == BID_sqrt) && nargs >= 1) {
        DESCR_t av = args[0];
        double v = TONUM(av);
        *out = REALVAL(sqrt(v)); return 1;
    }
#define MATH1(fname, cfn) \
    if (!strcmp(fn, fname) && nargs >= 1) { double _v = TONUM(args[0]); *out = REALVAL(cfn(_v)); return 1; }
    MATH1("sin",  sin)
    MATH1("cos",  cos)
    MATH1("tan",  tan)
    MATH1("asin", asin)
    MATH1("acos", acos)
    MATH1("exp",  exp)
#undef MATH1
    L_bidjmp_5791: ;
    if ((_bid == BID_atan) && nargs >= 1) {
        double v = TONUM(args[0]);
        if (nargs >= 2 && args[1].v != DT_SNUL) { double v2 = TONUM(args[1]); *out = REALVAL(atan2(v,v2)); }
        else *out = REALVAL(atan(v));
        return 1;
    }
    L_bidjmp_5797: ;
    if ((_bid == BID_log) && nargs >= 1) {
        double v = TONUM(args[0]);
        if (nargs >= 2 && args[1].v != DT_SNUL) { double base = TONUM(args[1]); *out = REALVAL(log(v)/log(base)); }
        else *out = REALVAL(log(v));
        return 1;
    }
    L_bidjmp_5803: ;
    if ((_bid == BID_dtor) && nargs >= 1) { double v=TONUM(args[0]); *out=REALVAL(v*3.14159265358979323846/180.0); return 1; }
    L_bidjmp_5804: ;
    if ((_bid == BID_rtod) && nargs >= 1) { double v=TONUM(args[0]); *out=REALVAL(v*180.0/3.14159265358979323846); return 1; }
    L_bidjmp_5805: ;
    if ((_bid == BID_iand)  && nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(a&b); return 1; }
    L_bidjmp_5806: ;
    if ((_bid == BID_ior)   && nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(a|b); return 1; }
    L_bidjmp_5807: ;
    if ((_bid == BID_ixor)  && nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(a^b); return 1; }
    L_bidjmp_5808: ;
    if ((_bid == BID_ishift)&& nargs==2) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r, b=IS_INT_fn(args[1])?args[1].i:(int64_t)args[1].r; *out=INTVAL(b>=0?a<<b:a>>(-b)); return 1; }
    L_bidjmp_5809: ;
    if ((_bid == BID_icom)  && nargs==1) { int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r; *out=INTVAL(~a); return 1; }
#undef TONUM
    L_bidjmp_5811: ;
    if ((_bid == BID_copy) && nargs == 1) {
        DESCR_t src = args[0];
        if (src.v == DT_T && src.tbl) {
            TBBLK_t *nt = table_new();
            nt->dflt = src.tbl->dflt;
            nt->init = src.tbl->init;
            nt->inc  = src.tbl->inc;
            nt->is_set = src.tbl->is_set;
            { TBPAIR_t *p; TBL_FOREACH(src.tbl, p) table_set_descr_d(nt, p->key_descr, p->val); }
            DESCR_t d; d.v = DT_T; d.slen = 0; d.tbl = nt;
            *out = d; return 1;
        }
        if (src.v == DT_DATA) {
            DESCR_t tag = FIELD_GET_fn(src, "gen_type");
            if (tag.v == DT_S && tag.s && strcmp(tag.s, "list") == 0) {
                DESCR_t ea = FIELD_GET_fn(src, "frame_elems");
                int n = (int)FIELD_GET_fn(src, "frame_size").i;
                DESCR_t *src_elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
                DESCR_t *new_elems = (DESCR_t *)rt_ws_alloc((size_t)(n > 0 ? n : 1) * sizeof(DESCR_t));
                if (src_elems && n > 0) memcpy(new_elems, src_elems, (size_t)n * sizeof(DESCR_t));
                DESCR_t eptr; eptr.v = DT_DATA; eptr.slen = 0; eptr.ptr = (void *)new_elems;
                *out = DATCON_fn("list", eptr, INTVAL(n), STRVAL("list"), INTVAL(n));
                return 1;
            }
        }
        *out = src; return 1;
    }
    L_bidjmp_5840: ;
    if ((_bid == BID_list) && nargs >= 0) {
        int n = 0;
        DESCR_t init = NULVCL;
        if (nargs >= 1) {
            DESCR_t nv = args[0];
            if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) {
                if (IS_INT_fn(nv)) n = (int)nv.i;
                else if (IS_REAL_fn(nv)) n = (int)nv.r;
                else { *out = FAILDESCR; return 1; }
                if (n < 0) { *out = FAILDESCR; return 1; }
            }
        }
        if (nargs >= 2) {
            DESCR_t iv = args[1];
            if (!IS_FAIL_fn(iv)) init = iv;
        }
        static int list_reg2 = 0;
        if (!list_reg2) { DEFDAT_fn("list(frame_elems,frame_size,gen_type,frame_cap)"); list_reg2 = 1; }
        DESCR_t *elems = rt_ws_alloc((n>0?n:1)*sizeof(DESCR_t));
        for (int i = 0; i < n; i++) elems[i] = init;
        DESCR_t eptr; eptr.v=DT_DATA; eptr.slen=0; eptr.ptr=(void*)elems;
        *out = DATCON_fn("list", eptr, INTVAL(n), STRVAL("list"), INTVAL(n));
        return 1;
    }
    L_bidjmp_5864: ;
    if ((_bid == BID_table) && nargs <= 2) {
        TBBLK_t *tbl = table_new();
        if (nargs == 1) {
            tbl->dflt = args[0];
        } else {
            tbl->dflt = NULVCL;
        }
        DESCR_t d; d.v = DT_T; d.slen = 0; d.tbl = tbl;
        *out = d; return 1;
    }
    L_bidjmp_5874: ;
    if ((_bid == BID_read) && nargs == 0) {
        char buf[4096];
        if (!fgets(buf, sizeof buf, stdin)) { *out = FAILDESCR; return 1; }
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
        if (len > 0 && buf[len-1] == '\r') buf[--len] = '\0';
        char *r = rt_ws_alloc(len + 1); memcpy(r, buf, len + 1);
        *out = STRVAL(r); return 1;
    }
    L_bidjmp_5883: ;
    if ((_bid == BID_reads) && nargs == 1 && (IS_INT_fn(args[0]) || IS_REAL_fn(args[0]))) {
        DESCR_t nd = args[0];
        int n = (int)to_int(nd);
        if (n <= 0) { *out = FAILDESCR; return 1; }
        char *buf = rt_ws_alloc(n + 1);
        int got = (int)fread(buf, 1, (size_t)n, stdin);
        if (got <= 0) { *out = FAILDESCR; return 1; }
        buf[got] = '\0';
        DESCR_t r; r.v = DT_S; r.slen = (uint32_t)got; r.s = buf;
        *out = r; return 1;
    }
    L_bidjmp_5894: ;
    if ((_bid == BID_runerr) && nargs >= 1) { long long _ec = IS_INT_fn(args[0]) ? (long long)args[0].i : 500; fprintf(stderr, "Run-time error %lld\n", _ec); if (nargs >= 2) { DESCR_t _im = FAILDESCR; if (try_call_builtin_by_name("image", args + 1, 1, &_im) && !IS_FAIL_fn(_im)) { const char *_is = VARVAL_fn(_im); fprintf(stderr, "offending value: %s\n", _is ? _is : ""); } } exit(1); }
    L_bidjmp_5895: ;
    if ((_bid == BID_stop)) { for (int _si = 0; _si < nargs; _si++) { DESCR_t _a = args[_si]; if (IS_INT_fn(_a)) fprintf(stderr, "%lld", (long long)_a.i); else if (IS_REAL_fn(_a)) { char _rb[64]; real_str(_a.r, _rb, sizeof _rb); fprintf(stderr, "%s", _rb); } else { const char *_s = VARVAL_fn(_a); if (_s) fprintf(stderr, "%s", _s); } } if (nargs) fprintf(stderr, "\n"); exit(0); }
    extern const char *scan_subj;
    extern int         scan_pos;
    extern int         scan_depth;
    L_bidjmp_5900: ;
    if ((_bid == BID_ICN_SCAN_PUSH) && nargs == 1) {
        const char *s;
        if (IS_REAL_fn(args[0])) { char _rb[64]; real_str(args[0].r,_rb,sizeof _rb); s = rt_ws_strdup(_rb); }
        else { s = VARVAL_fn(args[0]); if (!s) s = ""; }
        scan_depth++;
        scan_subj = rt_ws_strdup(s); scan_pos = 1;
        *out = args[0]; return 1;
    }
    L_bidjmp_5912: ;
    if ((_bid == BID_ICN_SCAN_POP) && nargs == 1) {
        if (scan_depth > 0) scan_depth--;
        *out = args[0]; return 1;
    }
    L_bidjmp_5920: ;
    if ((_bid == BID_any) && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv = VARVAL_fn(args[0]); if (!cv) { *out = FAILDESCR; return 1; }
        const char *s; int i1, i2;
        if (!bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
        if (i1 >= i2) { *out = FAILDESCR; return 1; }
        if (!strchr(cv, s[i1 - 1])) { *out = FAILDESCR; return 1; }
        *out = INTVAL(i1 + 1); return 1;
    }
    L_bidjmp_5938: ;
    if ((_bid == BID_many) && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv = VARVAL_fn(args[0]); if (!cv) { *out = FAILDESCR; return 1; }
        const char *s; int i1, i2;
        if (!bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
        int p = i1;
        while (p < i2 && strchr(cv, s[p - 1])) p++;
        if (p == i1) { *out = FAILDESCR; return 1; }
        *out = INTVAL(p); return 1;
    }
    L_bidjmp_5958: ;
    if ((_bid == BID_upto) && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv = VARVAL_fn(args[0]); if (!cv) { *out = FAILDESCR; return 1; }
        const char *s; int i1, i2;
        if (!bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
        int p = i1;
        while (p < i2 && !strchr(cv, s[p - 1])) p++;
        if (p >= i2) { *out = FAILDESCR; return 1; }
        *out = INTVAL(p); return 1;
    }
    L_bidjmp_5978: ;
    if ((_bid == BID_tab) && nargs == 1 && scan_pos > 0) {
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(scan_subj);
        int target = (int)to_int(args[0]);
        if (target <= 0) target = slen + 1 + target;
        if (target < 1 || target > slen + 1) { *out = FAILDESCR; return 1; }
        int old = scan_pos; scan_pos = target;
        int lo = old < target ? old : target, hi = old < target ? target : old;
        int len = hi - lo;
        char *buf = rt_ws_alloc(len + 1);
        memcpy(buf, scan_subj + lo - 1, len); buf[len] = '\0';
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_5991: ;
    if ((_bid == BID_move) && nargs == 1 && scan_pos > 0) {
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(scan_subj);
        int n = (int)to_int(args[0]);
        int target = scan_pos + n;
        if (target < 1 || target > slen + 1) { *out = FAILDESCR; return 1; }
        int old = scan_pos; scan_pos = target;
        int lo = old < target ? old : target, hi = old < target ? target : old;
        int len = hi - lo;
        char *buf = rt_ws_alloc(len + 1);
        memcpy(buf, scan_subj + lo - 1, len); buf[len] = '\0';
        *out = STRVAL(buf); return 1;
    }
    L_bidjmp_6004: ;
    if ((_bid == BID_pos) && nargs == 1 && scan_pos > 0) {
        if (!scan_subj) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(scan_subj);
        int target = (int)to_int(args[0]);
        if (target <= 0) target = slen + 1 + target;
        if (target < 1 || target > slen + 1 || target != scan_pos) { *out = FAILDESCR; return 1; }
        *out = INTVAL(target); return 1;
    }
    L_bidjmp_6012: ;
    if ((_bid == BID_match) && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *pat = VARVAL_fn(args[0]); if (!pat) { *out = FAILDESCR; return 1; }
        const char *s; int i1, i2;
        if (!bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(s), plen2 = (int)strlen(pat), p0 = i1 - 1;
        if (p0 + plen2 > slen || i1 + plen2 > i2) { *out = FAILDESCR; return 1; }
        if (strncmp(s + p0, pat, (size_t)plen2) != 0) { *out = FAILDESCR; return 1; }
        *out = INTVAL(i1 + plen2); return 1;
    }
    L_bidjmp_6033: ;
    if ((_bid == BID_bal) && (scan_pos > 0 || nargs >= 4)) {
        const char *c1 = 0; int c1len = 0; int c1any = 0;
        if (nargs < 1 || IS_FAIL_fn(args[0]) || args[0].v == DT_SNUL) c1any = 1;
        else if (!cset_resolve(args[0], &c1, &c1len)) { *out = FAILDESCR; return 1; }
        const char *c2 = "("; int c2len = 1;
        const char *c3 = ")"; int c3len = 1;
        if (nargs >= 2) { const char *v; int vlen; if (cset_resolve(args[1], &v, &vlen) && vlen > 0) { c2 = v; c2len = vlen; } }
        if (nargs >= 3) { const char *v; int vlen; if (cset_resolve(args[2], &v, &vlen) && vlen > 0) { c3 = v; c3len = vlen; } }
        const char *s; int slen; int p;
        if (nargs >= 4 && !IS_FAIL_fn(args[3]) && args[3].v != DT_SNUL) {
            s = VARVAL_fn(args[3]); if (!s) s = "";
            slen = (int)strlen(s);
            int i1 = (nargs >= 5 && (IS_INT_fn(args[4]) || IS_REAL_fn(args[4]))) ? (int)to_int(args[4]) : 1;
            int i2 = (nargs >= 6 && (IS_INT_fn(args[5]) || IS_REAL_fn(args[5]))) ? (int)to_int(args[5]) : slen + 1;
            if (i1 <= 0) i1 = slen + 1 + i1;
            if (i2 <= 0) i2 = slen + 1 + i2;
            if (i1 < 1 || i1 > slen + 1) { *out = FAILDESCR; return 1; }
            if (i2 - 1 < slen) slen = i2 - 1;
            p = i1 - 1;
        } else {
            s = scan_subj ? scan_subj : ""; slen = (int)strlen(s);
            p = scan_pos - 1;
        }
        long long cnt = 0;
        while (p < slen) {
            unsigned char ch = (unsigned char)s[p];
            if (cnt == 0 && (c1any || cset_has(c1, c1len, ch))) { *out = INTVAL(p + 1); return 1; }
            if (cset_has(c2, c2len, ch)) cnt++;
            else if (cset_has(c3, c3len, ch)) { cnt--; if (cnt < 0) { *out = FAILDESCR; return 1; } }
            p++;
        }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_6067: ;
    if ((_bid == BID_find) && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *needle = VARVAL_fn(args[0]); if (!needle) { *out = FAILDESCR; return 1; }
        const char *hay    = (nargs >= 2) ? VARVAL_fn(args[1]) : (const char *)0; if (!hay) hay = scan_subj ? scan_subj : "";
        int nlen = (int)strlen(needle), hlen = (int)strlen(hay);
        int i1 = (nargs >= 3) ? (int)args[2].i : (scan_pos > 0 && nargs < 2 ? scan_pos : 1);
        int i2 = (nargs >= 4) ? (int)args[3].i : hlen + 1;
        if (i1 <= 0 || i1 > hlen + 1) { *out = FAILDESCR; return 1; }
        if (i2 <= 0 || i2 > hlen + 1) i2 = hlen + 1;
        int term = (i2 - 1) - nlen;
        for (int i = i1 - 1; i <= term; i++) {
            if (strncmp(hay + i, needle, nlen) == 0) { *out = INTVAL(i + 1); return 1; }
        }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6081: ;
    if ((_bid == BID_NONNULL) && nargs == 1) {
        DESCR_t v = args[0];
        if (IS_FAIL_fn(v))  { *out = FAILDESCR; return 1; }
        if (v.v == DT_SNUL) { *out = FAILDESCR; return 1; }
        if (v.v == DT_S && (!v.s || v.s[0]=='\0')) { *out = FAILDESCR; return 1; }
        *out = v; return 1;
    }
    L_bidjmp_6088: ;
    if ((_bid == BID_ICN_CASE_EQ) && nargs == 2) {
        DESCR_t topic = args[0], val = args[1];
        if (IS_FAIL_fn(topic) || IS_FAIL_fn(val)) { *out = FAILDESCR; return 1; }
        int eq = 0;
        if ((IS_INT_fn(topic) || IS_REAL_fn(topic)) &&
            (IS_INT_fn(val)   || IS_REAL_fn(val))) {
            double tv = IS_REAL_fn(topic) ? topic.r : (double)topic.i;
            double vv = IS_REAL_fn(val)   ? val.r   : (double)val.i;
            eq = (tv == vv);
        } else {
            const char *ts = VARVAL_fn(topic); if (!ts) ts = "";
            const char *vs = VARVAL_fn(val);   if (!vs) vs = "";
            eq = (strcmp(ts, vs) == 0);
        }
        *out = eq ? val : FAILDESCR; return 1;
    }
    L_bidjmp_6104: ;
    if ((_bid == BID_ICN_SWAP_TOP2) && nargs == 2) {
        *out = args[0];
        return 1;
    }
    L_bidjmp_6108: ;
    if ((_bid == BID_ICN_NULL) && nargs == 1) {
        DESCR_t v = args[0];
        if (IS_FAIL_fn(v))  { *out = FAILDESCR; return 1; }
        if (v.v == DT_SNUL) { *out = NULVCL; return 1; }
        if (v.v == DT_S && (!v.s || v.s[0]=='\0')) { *out = NULVCL; return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6115: ;
    if ((_bid == BID_insert) && nargs >= 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = (nargs >= 2) ? args[1] : NULVCL;
        DESCR_t vd = (td.tbl && td.tbl->is_set) ? kd : ((nargs >= 3) ? args[2] : NULVCL);
        char kb[64]; const char *ks = tbl_key_str(kd, kb, sizeof kb);
        table_set_descr_d(td.tbl, kd, vd);
        *out = td; return 1;
    }
    L_bidjmp_6124: ;
    if ((_bid == BID_delete) && nargs >= 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = (nargs >= 2) ? args[1] : NULVCL;
        char kb[64]; const char *ks = tbl_key_str(kd, kb, sizeof kb);
        table_delete_d(td.tbl, kd);
        *out = td; return 1;
    }
    L_bidjmp_6132: ;
    if ((_bid == BID_member) && nargs >= 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = (nargs >= 2) ? args[1] : NULVCL;
        char kb[64]; const char *ks = tbl_key_str(kd, kb, sizeof kb);
        if (!table_has_d(td.tbl,kd)) { *out=FAILDESCR; return 1; }
        *out = table_get_d(td.tbl, kd); return 1;
    }
    L_bidjmp_6140: ;
    if ((_bid == BID_key) && nargs == 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T || !td.tbl) { *out=FAILDESCR; return 1; }
        for (unsigned _bi=0;_bi<td.tbl->nbuck;_bi++)
            if (td.tbl->buckets[_bi] && td.tbl->buckets[_bi]->len) {
                *out = td.tbl->buckets[_bi]->ent[0].key_descr; return 1;
            }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6149: ;
    if ((_bid == BID___apply__) && nargs == 2) {
        DESCR_t callee = args[0]; DESCR_t lv = args[1];
        if (lv.v == DT_DATA) {
            DESCR_t tag = FIELD_GET_fn(lv,"gen_type");
            if (tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0) {
                int n=(int)FIELD_GET_fn(lv,"frame_size").i;
                DESCR_t ea=FIELD_GET_fn(lv,"frame_elems");
                DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
                *out = rt_call_value(callee, arr, (arr?n:0)); return 1;
            }
        }
        { DESCR_t a1 = lv; *out = rt_call_value(callee, &a1, 1); return 1; }
    }
    L_bidjmp_6162: ;
    if ((_bid == BID_push) && nargs >= 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        { DESCR_t *_a=0; int _n=0; if (rt_lv_is_list(ld,&_a,&_n)) {
            int _nv2 = (nargs > 1) ? nargs - 1 : 1;
            for (int _pi = 0; _pi < _nv2; _pi++) {
                DESCR_t _vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
                DESCR_t *_nb=rt_ws_alloc((_n+1)*sizeof(DESCR_t));
                _nb[0]=_vd;
                if(_a&&_n>0) memcpy(_nb+1,_a,_n*sizeof(DESCR_t));
                _a=_nb; _n++;
                rt_lv_set_elems(ld,(DESCR_t){.v=DT_DATA,.ptr=_nb}); rt_lv_set_size(ld,_n); rt_lv_set_cap(ld,_n); }
            *out = ld; return 1; } }
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        int _nv = (nargs > 1) ? nargs - 1 : 1;
        for (int _pi = 0; _pi < _nv; _pi++) {
            DESCR_t vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
            int n=(int)FIELD_GET_fn(ld,"frame_size").i;
            DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
            DESCR_t *old=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
            DESCR_t *nb=rt_ws_alloc((n+1)*sizeof(DESCR_t));
            nb[0]=vd;
            if(old&&n>0) memcpy(nb+1,old,n*sizeof(DESCR_t));
            FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=nb});
            FIELD_SET_fn(ld,"frame_size",INTVAL(n+1));
            FIELD_SET_fn(ld,"frame_cap",INTVAL(n+1));
        }
        *out = ld; return 1;
    }
    L_bidjmp_6182: ;
    if ((_bid == BID_put) && nargs >= 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        { DESCR_t *_a=0; int _n=0; if (rt_lv_is_list(ld,&_a,&_n)) {
            int _nv2 = (nargs > 1) ? nargs - 1 : 1;
            for (int _pi = 0; _pi < _nv2; _pi++) {
                DESCR_t _vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
                long _c = rt_lv_cap(ld);
                if(_a && _c>=0 && _n<_c){ _a[_n]=_vd; _n++; rt_lv_set_size(ld,_n); continue; }
                long _ncap=(_n>0)?(long)_n*2:8; if(_ncap<_n+1)_ncap=_n+1;
                DESCR_t *_nb=rt_ws_alloc(_ncap*sizeof(DESCR_t));
                if(_a&&_n>0) memcpy(_nb,_a,_n*sizeof(DESCR_t));
                _nb[_n]=_vd; _a=_nb; _n++;
                rt_lv_set_elems(ld,(DESCR_t){.v=DT_DATA,.ptr=_nb}); rt_lv_set_size(ld,_n); rt_lv_set_cap(ld,_ncap); }
            *out = ld; return 1; } }
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        int _nv = (nargs > 1) ? nargs - 1 : 1;
        for (int _pi = 0; _pi < _nv; _pi++) {
            DESCR_t vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
            int n=(int)FIELD_GET_fn(ld,"frame_size").i;
            DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
            DESCR_t *old=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
            DESCR_t capd=FIELD_GET_fn(ld,"frame_cap");
            long cap=(capd.v==DT_I)?capd.i:-1;
            if(old&&cap>=0&&n<cap){ old[n]=vd; FIELD_SET_fn(ld,"frame_size",INTVAL(n+1)); continue; }
            long ncap=(n>0)?(long)n*2:8; if(ncap<n+1)ncap=n+1;
            DESCR_t *nb=rt_ws_alloc(ncap*sizeof(DESCR_t));
            if(old&&n>0) memcpy(nb,old,n*sizeof(DESCR_t));
            nb[n]=vd;
            FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=nb});
            FIELD_SET_fn(ld,"frame_size",INTVAL(n+1));
            FIELD_SET_fn(ld,"frame_cap",INTVAL(ncap));
        }
        *out = ld; return 1;
    }
    L_bidjmp_6206: ;
    if ((_bid == BID_get) && nargs == 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        { DESCR_t *_a=0; int _n=0; if (rt_lv_is_list(ld,&_a,&_n)) {
            if(!_a||_n<=0) { *out=FAILDESCR; return 1; }
            DESCR_t _r=_a[0]; rt_lv_set_elems(ld,(DESCR_t){.v=DT_DATA,.ptr=_a+1}); rt_lv_set_size(ld,_n-1);
            { long _c=rt_lv_cap(ld); if(_c>0) rt_lv_set_cap(ld,_c-1); }
            *out=_r; return 1; } }
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
        int n=(int)FIELD_GET_fn(ld,"frame_size").i;
        DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
        if(!arr||n<=0) { *out=FAILDESCR; return 1; }
        DESCR_t ret=arr[0];
        FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=arr+1});
        FIELD_SET_fn(ld,"frame_size",INTVAL(n-1));
        { DESCR_t capd=FIELD_GET_fn(ld,"frame_cap"); if(capd.v==DT_I&&capd.i>0) FIELD_SET_fn(ld,"frame_cap",INTVAL(capd.i-1)); }
        *out = ret; return 1;
    }
    L_bidjmp_6221: ;
    if ((_bid == BID_pop) && nargs == 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        { DESCR_t *_a=0; int _n=0; if (rt_lv_is_list(ld,&_a,&_n)) {
            if(!_a||_n<=0) { *out=FAILDESCR; return 1; }
            DESCR_t _r=_a[0]; rt_lv_set_elems(ld,(DESCR_t){.v=DT_DATA,.ptr=_a+1}); rt_lv_set_size(ld,_n-1);
            { long _c=rt_lv_cap(ld); if(_c>0) rt_lv_set_cap(ld,_c-1); }
            *out=_r; return 1; } }
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
        int n=(int)FIELD_GET_fn(ld,"frame_size").i;
        DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
        if(!arr||n<=0) { *out=FAILDESCR; return 1; }
        DESCR_t ret=arr[0];
        FIELD_SET_fn(ld,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=arr+1});
        FIELD_SET_fn(ld,"frame_size",INTVAL(n-1));
        { DESCR_t capd=FIELD_GET_fn(ld,"frame_cap"); if(capd.v==DT_I&&capd.i>0) FIELD_SET_fn(ld,"frame_cap",INTVAL(capd.i-1)); }
        *out = ret; return 1;
    }
    L_bidjmp_6236: ;
    if ((_bid == BID_pull) && nargs == 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        { DESCR_t *_a=0; int _n=0; if (rt_lv_is_list(ld,&_a,&_n)) {
            if(!_a||_n<=0) { *out=FAILDESCR; return 1; }
            DESCR_t _r=_a[_n-1]; rt_lv_set_size(ld,_n-1); *out=_r; return 1; } }
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
        int n=(int)FIELD_GET_fn(ld,"frame_size").i;
        DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
        if(!arr||n<=0) { *out=FAILDESCR; return 1; }
        DESCR_t ret=arr[n-1];
        FIELD_SET_fn(ld,"frame_size",INTVAL(n-1));
        *out = ret; return 1;
    }
    L_bidjmp_6249: ;
    if ((_bid == BID_sort) && nargs >= 1 && args[0].v == DT_T && args[0].tbl) {
        TBBLK_t *tb = args[0].tbl;
        int i_mode = (nargs >= 2) ? (int)to_int(args[1]) : 1;
        if (i_mode < 1 || i_mode > 4) i_mode = 1;
        int n = 0;
        { TBPAIR_t *e; TBL_FOREACH(tb, e) n++; }
        TBPAIR_t **ent = rt_ws_alloc((n>0?n:1)*sizeof(TBPAIR_t*));
        { int _k = 0; TBPAIR_t *e; TBL_FOREACH(tb, e) ent[_k++] = e; }
        int by_val = (i_mode % 2 == 0);
        { TBPAIR_t **_tmp = rt_ws_alloc((n>0?n:1)*sizeof(TBPAIR_t*)); sort_msort_pairs(ent, _tmp, n, by_val); }
        extern DESCR_t rt_make_list(DESCR_t *a, int nn);
        if (tb->is_set) {
            DESCR_t *mem = rt_ws_alloc((n>0?n:1)*sizeof(DESCR_t));
            for (int _k = 0; _k < n; _k++) mem[_k] = ent[_k]->key_descr;
            *out = rt_make_list(mem, n); return 1;
        }
        if (i_mode >= 3) {
            DESCR_t *flat = rt_ws_alloc((2*n>0?2*n:1)*sizeof(DESCR_t));
            for (int _k = 0; _k < n; _k++) { flat[2*_k] = ent[_k]->key_descr; flat[2*_k+1] = ent[_k]->val; }
            *out = rt_make_list(flat, 2*n); return 1;
        }
        DESCR_t *pairs = rt_ws_alloc((n>0?n:1)*sizeof(DESCR_t));
        for (int _k = 0; _k < n; _k++) { DESCR_t pv[2] = { ent[_k]->key_descr, ent[_k]->val }; pairs[_k] = rt_make_list(pv, 2); }
        *out = rt_make_list(pairs, n); return 1;
    }
    if (((_bid == BID_sort)&&(nargs==1||nargs==2))||((_bid == BID_sortf)&&nargs==2)) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        DESCR_t tag = FIELD_GET_fn(ld,"gen_type");
        if (!(tag.v==DT_S && tag.s && strcmp(tag.s,"list")==0)) return 0;
        DESCR_t ea=FIELD_GET_fn(ld,"frame_elems");
        int n=(int)FIELD_GET_fn(ld,"frame_size").i;
        if (n<=0) { *out=ld; return 1; }
        DESCR_t *arr=(ea.v==DT_DATA)?(DESCR_t*)ea.ptr:NULL;
        if(!arr) { *out=ld; return 1; }
        DESCR_t *sorted=rt_ws_alloc(n*sizeof(DESCR_t));
        memcpy(sorted,arr,n*sizeof(DESCR_t));
        int field_idx=((_bid == BID_sortf)&&nargs==2)?(int)to_int(args[1])-1:-1;
        { DESCR_t *_tmp = rt_ws_alloc((n>0?n:1)*sizeof(DESCR_t)); sort_msort_descr(sorted, _tmp, n, field_idx); }
        DESCR_t res=ld;
        FIELD_SET_fn(res,"frame_elems",(DESCR_t){.v=DT_DATA,.ptr=sorted});
        FIELD_SET_fn(res,"frame_size",INTVAL(n));
        FIELD_SET_fn(res,"frame_cap",INTVAL(n));
        *out=res; return 1;
    }
    L_bidjmp_6322: ;
    if ((_bid == BID_FIELD_GET) && nargs == 2) {
        DESCR_t obj  = args[0];
        DESCR_t fname_d = args[1];
        const char *fname = VARVAL_fn(fname_d);
        if (!fname || obj.v != DT_DATA) { *out=FAILDESCR; return 1; }
        extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
        *out = dat_field_get(fname, obj); return 1;
    }
    L_bidjmp_6330: ;
    if ((_bid == BID_FIELD_SET) && nargs == 3) {
        DESCR_t val    = args[0];
        DESCR_t obj    = args[1];
        DESCR_t fname_d = args[2];
        const char *fname = VARVAL_fn(fname_d);
        if (!fname || obj.v != DT_DATA) { *out=FAILDESCR; return 1; }
        extern DESCR_t *data_field_ptr(const char *field, DESCR_t obj);
        DESCR_t *cell = data_field_ptr(fname, obj);
        if (cell) { *cell = val; *out = val; return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6341: ;
    if ((_bid == BID_MAKELIST)) {
        extern DESCR_t rt_make_list(DESCR_t *args, int nargs);
        *out = rt_make_list(args, nargs); return 1;
    }
    L_bidjmp_6345: ;
    if ((_bid == BID_RECORD_REGISTER) && nargs >= 1) {
        extern void record_register(const char *spec);
        const char *spec = VARVAL_fn(args[0]);
        if (spec && *spec) record_register(spec);
        *out = NULVCL; return 1;
    }
    L_bidjmp_6351: ;
    if ((_bid == BID_RECORD_MAKE) && nargs >= 1) {
        const char *rname = VARVAL_fn(args[0]);
        if (!rname || !*rname) { *out=FAILDESCR; return 1; }
        DatType *_dt = dat_find_type(rname);
        if (!_dt) { *out=FAILDESCR; return 1; }
        DESCR_t fargs[FRAME_SLOT_MAX];
        int nf = nargs - 1;
        for (int _j=0;_j<nf&&_j<FRAME_SLOT_MAX;_j++) fargs[_j]=args[1+_j];
        *out = dat_construct(_dt, fargs, nf); return 1;
    }
    L_bidjmp_6361: ;
    if ((_bid == BID_open) && (nargs == 1 || nargs == 2)) {
        const char *path = (args[0].v == DT_S || args[0].v == DT_SNUL) ? args[0].s : NULL;
        if (!path) { *out = FAILDESCR; return 1; }
        const char *mode = (nargs == 2 && (args[1].v == DT_S||args[1].v == DT_SNUL) && args[1].s)
                           ? args[1].s : "r";
        const char *cmode = "r";
        if (strstr(mode,"w")) cmode = "w";
        else if (strstr(mode,"a")) cmode = "a";
        FILE *fp = fopen(path, cmode);
        if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp);
        if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        if (idx >= 0 && idx < FH_MAX) fh_name[idx] = rt_ws_strdup(path);
        *out = FHVAL(idx); return 1;
    }
    L_bidjmp_6376: ;
    if ((_bid == BID_remove) && nargs == 1) {
        const char *_rp = VARVAL_fn(args[0]); if (!_rp) _rp = "";
        if (unlink(_rp) == 0) { *out = NULVCL; return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6381: ;
    if ((_bid == BID_close) && nargs == 1) {
        if (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) {
            int idx = (int)args[0].i;
            FILE *fp = fh_get(idx);
            if (fp && idx > 2) { fclose(fp); fh_free(idx); }
        }
        *out = args[0]; return 1;
    }
    if ((_bid == BID_read) && nargs == 1) {
        FILE *fp = (args[0].v == DT_SNUL) ? fh_get(0) : (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) ? fh_get((int)args[0].i) : NULL;
        if (!fp) { *out = FAILDESCR; return 1; }
        char buf[4096];
        if (!fgets(buf, sizeof buf, fp)) { *out = FAILDESCR; return 1; }
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
        if (len > 0 && buf[len-1] == '\r') buf[--len] = '\0';
        *out = STRVAL(rt_ws_strdup_c(buf)); return 1;
    }
    if ((_bid == BID_reads) && nargs >= 1) {
        FILE *fp = (args[0].v == DT_SNUL) ? fh_get(0) : (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) ? fh_get((int)args[0].i) : NULL;
        if (!fp) { *out = FAILDESCR; return 1; }
        int n = (nargs >= 2 && args[1].v != DT_SNUL && !IS_FAIL_fn(args[1])) ? (int)to_int(args[1]) : 1;
        if (n <= 0) { *out = FAILDESCR; return 1; }
        char *buf = rt_ws_alloc(n + 1);
        int got = (int)fread(buf, 1, (size_t)n, fp);
        if (got <= 0) { *out = FAILDESCR; return 1; }
        buf[got] = '\0';
        DESCR_t r; r.v = DT_S; r.slen = (uint32_t)got; r.s = buf;
        *out = r; return 1;
    }
    L_bidjmp_6411: ;
    if ((_bid == BID_IDENTICAL) && nargs == 2) {
        DESCR_t a = args[0], b = args[1];
        int same = (a.v == b.v);
        if (same) {
            if      (a.v == DT_I)               same = (a.i == b.i);
            else if (a.v == DT_R)               same = (a.r == b.r);
            else if (a.v == DT_S || a.v == DT_SNUL)
                same = (a.s == b.s || (a.s && b.s && strcmp(a.s,b.s)==0));
            else                                same = (a.ptr == b.ptr);
        }
        *out = same ? b : FAILDESCR; return 1;
    }
    L_bidjmp_6423: ;
    if ((_bid == BID_set) && nargs <= 1) {
        TBBLK_t *tbl = table_new();
        tbl->is_set = 1;
        if (nargs == 1 && args[0].v == DT_DATA) {
            DESCR_t tag = FIELD_GET_fn(args[0], "gen_type");
            if (tag.v == DT_S && tag.s && strcmp(tag.s,"list")==0) {
                DESCR_t ea = FIELD_GET_fn(args[0], "frame_elems");
                int n = (int)FIELD_GET_fn(args[0], "frame_size").i;
                DESCR_t *elems = (ea.v == DT_DATA) ? (DESCR_t *)ea.ptr : NULL;
                char kb[64];
                if (elems) for (int _i = 0; _i < n; _i++)
                    table_set_descr_d(tbl, elems[_i], elems[_i]);
            }
        }
        *out = TABLE_VAL(tbl); return 1;
    }
    L_bidjmp_6439: ;
    if ((_bid == BID_ASGN) && nargs == 2) {
        DESCR_t rhs = args[0];
        if (IS_FAIL_fn(rhs)) { *out = FAILDESCR; return 1; }
        DESCR_t lref = args[1];
        if (lref.v == DT_S && lref.s) NV_SET_fn(lref.s, rhs);
        *out = rhs; return 1;
    }
    L_bidjmp_6446: ;
    if ((_bid == BID_name) && nargs == 1) {
        DESCR_t a = args[0];
        if (a.v == DT_N && a.slen == 0 && a.s && *a.s) { *out = STRVAL(rt_ws_strdup_c(a.s)); return 1; }
        if (IS_NAMETRAP_fn(a) && a.p) {
            VCELL_t *vc = (VCELL_t *)a.p;
            if (vc->key && *vc->key) { *out = STRVAL(rt_ws_strdup_c(vc->key)); return 1; }
            if (vc->sv.v == DT_N && (vc->sv.slen == 0 || vc->sv.slen == 2) && vc->len > 0) {
                DESCR_t bn; int r1 = try_call_builtin_by_name("name", &vc->sv, 1, &bn);
                const char *base = (r1 && bn.v == DT_S && bn.s) ? bn.s : "";
                char sb[128]; snprintf(sb, sizeof sb, "%s[%ld:%ld]", base, (long)vc->pos, (long)(vc->pos + vc->len));
                *out = STRVAL(rt_ws_strdup_c(sb)); return 1;
            }
            *out = FAILDESCR; return 1;
        }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6462: ;
    if ((_bid == BID_variable) && nargs == 1) {
        const char *vname = (args[0].v == DT_S || args[0].v == DT_SNUL) ? args[0].s : NULL;
        if (!vname) { *out = FAILDESCR; return 1; }
        DESCR_t v = NV_GET_fn(vname);
        *out = IS_FAIL_fn(v) ? FAILDESCR : v; return 1;
    }
    L_bidjmp_6468: ;
    if ((_bid == BID_SNOx24NAME) && nargs == 1) return bn_sno_name(args, nargs, out);
    L_bidjmp_6469: ;
    if ((_bid == BID_ARRAY) && nargs >= 1) {
        extern DESCR_t sno_array_from_proto(const char *proto, DESCR_t init);
        DESCR_t init = (nargs >= 2) ? args[1] : NULVCL;
        char pb[64]; const char *proto;
        /*⭐⭐ ARRAY(n) WITH AN INTEGER BOUND NO LONGER ROUND-TRIPS THROUGH TEXT (hq_P s264, measured on the json deserializer).
           This arm used to snprintf the integer into a decimal string, hand that string to sno_array_from_proto, which copied it
           byte by byte into its own buffer, ran strchr for a ':' and then strtol'd it BACK to the integer we started with -- and
           then rt_ws_strdup'd the string a third time onto the block.  MEASURED: json builds one key-order ARRAY per object, so a
           400-object parse made 400 of these, 4,405 strtol calls per 11 iterations, for arrays ONE element long; sno_array_from_proto
           + array_new were 14.7% of the whole json run on an input containing ZERO json arrays.
           ⛔ THE ->proto STRING IS DELIBERATELY NOT STORED, AND THAT IS NOT A LOSS: agg_prototype() (aggregates.c:114) ALREADY
           reconstructs it from lo/hi/ndim whenever ->proto is NULL, and for the lo==1 case it formats exactly "%d" of a->hi --
           byte-identical to what this line used to strdup.  A NULL ->proto is an established state (array_new itself sets it, and
           pattern_match.c:532 does too), so PROTOTYPE() is unaffected.  Same lazy-mint discipline as tbl_pair_key().
           ⛔ Only the INTEGER form takes this path.  ARRAY('3,4') and ARRAY('0:9') still carry real prototype syntax and still go
           through sno_array_from_proto, unchanged. */
        if (IS_INT_fn(args[0])) {
            const long long _n = (long long)args[0].i;
            if (_n < 0) { *out = FAILDESCR; return 1; }                       /* hi < lo-1, matching sno_array_from_proto */
            extern ARBLK_t *array_new(int lo, int hi);
            ARBLK_t *_a = array_new(1, (int)_n);
            if (!_a) { *out = FAILDESCR; return 1; }
            if (!(init.v == DT_SNUL && init.slen == 0)) for (long long _k = 0; _k < _n; _k++) _a->data[_k] = init;   /* array_new already laid down NULVCL */
            DESCR_t _r; memset(&_r, 0, sizeof _r); _r.v = DT_A; _r.slen = 0; _r.arr = _a;
            *out = _r; return 1;
        }
        proto = VARVAL_fn(args[0]); if (!proto || !*proto) { *out = FAILDESCR; return 1; }
        (void)pb;
        DESCR_t r = sno_array_from_proto(proto, init);
        if (r.v == DT_A && r.arr) ((ARBLK_t *)r.arr)->proto = rt_ws_strdup(proto);
        *out = r; return 1;
    }
    L_bidjmp_6479: ;
    if ((_bid == BID_TABLE) && nargs <= 3) {
        TBBLK_t *tb = table_new_args(nargs >= 1 ? (int)to_int(args[0]) : 0, nargs >= 2 ? (int)to_int(args[1]) : 0);
        if (nargs >= 3) tb->dflt = args[2];   /* manual v3.7 sec 4214: Arg3 is the value returned for a missing-key lookup */
        DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_T; d.slen = 0; d.tbl = tb;
        *out = d; return 1;
    }
    L_bidjmp_6484: ;
    if ((_bid == BID_ITEM) && nargs >= 2) {
        extern DESCR_t rt_subscript_var(DESCR_t base, DESCR_t idx); extern DESCR_t rt_deref(DESCR_t v);
        DESCR_t cur = args[0];
        for (int k = 1; k < nargs; k++) { cur = rt_subscript_var(cur, args[k]); if (IS_FAIL_fn(cur)) { *out = FAILDESCR; return 1; } }
        *out = rt_deref(cur); return 1;
    }
    L_bidjmp_6490: ;
    if (_bid == BID_PROTOTYPE) {
        *out = agg_prototype(nargs >= 1 ? args[0] : NULVCL); return 1;
    }
    L_bidjmp_6499: ;
    if ((_bid == BID_CONVERT) && nargs == 2) {
        char tb[32]; const char *ts = to_cstring(args[1], tb, sizeof tb); if (!ts) ts = "";
        char tu[32]; { int k = 0; for (; ts[k] && k < 31; k++) tu[k] = (ts[k] >= 'a' && ts[k] <= 'z') ? (char)(ts[k] - 32) : ts[k]; tu[k] = 0; }
        DESCR_t a = args[0];
        if (!strcmp(tu,"INTEGER")) {
            if (IS_INT_fn(a)) { *out = a; return 1; }
            if (IS_REAL_fn(a)) { *out = INTVAL((long long)a.r); return 1; }
            const char *sv = VARVAL_fn(a); if (!sv) { *out = FAILDESCR; return 1; }
            char *e = NULL; long long iv = strtoll(sv, &e, 10); if (e && *e == '\0' && e != sv) { *out = INTVAL(iv); return 1; }
            double dv = strtod(sv, &e); if (e && *e == '\0' && e != sv) { *out = INTVAL((long long)dv); return 1; }
            *out = FAILDESCR; return 1;
        }
        if (!strcmp(tu,"REAL")) {
            if (IS_REAL_fn(a)) { *out = a; return 1; }
            if (IS_INT_fn(a)) { *out = REALVAL((double)a.i); return 1; }
            const char *sv = VARVAL_fn(a); if (!sv) { *out = FAILDESCR; return 1; }
            char *e = NULL; double dv = strtod(sv, &e); if (e && *e == '\0' && e != sv) { *out = REALVAL(dv); return 1; }
            *out = FAILDESCR; return 1;
        }
        if (!strcmp(tu,"STRING")) { const char *sv = VARVAL_fn(a); *out = STRVAL(rt_ws_strdup_c(sv ? sv : "")); return 1; }
        return 0;
    }
    L_bidjmp_6521: ;
    if ((_bid == BID_DATA) && nargs == 1) {
        extern DatType *dat_register(const char *spec);
        const char *sp = VARVAL_fn(args[0]); if (!sp || !*sp) { *out = FAILDESCR; return 1; }
        char nb[128]; int k = 0; for (; sp[k] && sp[k] != '(' && k < 127; k++) nb[k] = sp[k]; nb[k] = 0;
        if (!dat_find_type(nb)) dat_register(sp);
        *out = NULVCL; return 1;
    }
    L_bidjmp_6528: ;
    if ((_bid == BID_SNOx24KWSET) && nargs == 2) {
        extern int rt_keyword_write_snobol4(const char *sval, DESCR_t v);
        char kb[64]; const char *kn = to_cstring(args[0], kb, sizeof kb);
        *out = rt_keyword_write_snobol4(kn ? kn : "", args[1]) ? args[1] : FAILDESCR; return 1;
    }
    L_bidjmp_6534: ;
    if ((_bid == BID_SNOx24STMT) && (nargs == 1 || nargs == 2 || nargs == 3)) {
        extern void rt_stmt_enter(long stno, long line);
        long n = IS_INT(args[0]) ? (long)args[0].i : 0;
        long ln = (nargs >= 2 && IS_INT(args[1])) ? (long)args[1].i : 0;
        /* 3rd arg is the source path, baked once at the program's first statement only (see
           lower_snobol4.c's SNO$STMT emission) -- set &FILE's backing global before rt_stmt_enter
           runs so its own g_stcount==0 check (first statement) still reads &LASTFILE as empty. */
        if (nargs == 3) { extern void rt_stmt_file_init(const char *file); const char *fp = VARVAL_fn(args[2]); rt_stmt_file_init(fp ? fp : ""); }
        rt_stmt_enter(n, ln);
        *out = NULVCL; return 1;
    }
    L_bidjmp_6540: ;
    if ((_bid == BID_SNOx24MKEXPR) && nargs == 1) {
        const char *nm = VARVAL_fn(args[0]); if (!nm) nm = "";
        DESCR_t xd; xd.v = DT_X; xd.slen = (uint32_t)strlen(nm); xd.s = rt_ws_strdup(nm);
        *out = xd; return 1;
    }
    L_bidjmp_6545: ;
    if ((_bid == BID_SNOx24PBK) && nargs == 2) { extern DESCR_t pat_mk_cset(int, const char *); *out = pat_mk_cset((int)to_int(args[0]), VARVAL_fn(args[1])); return 1; }
    L_bidjmp_6546: ;
    if ((_bid == BID_SNOx24PBN) && nargs == 2) { extern DESCR_t pat_mk_num(int, int64_t); *out = pat_mk_num((int)to_int(args[0]), to_int(args[1])); return 1; }
    L_bidjmp_6547: ;
    if ((_bid == BID_SNOx24PB0) && nargs == 1) { extern DESCR_t pat_mk_nil(int); *out = pat_mk_nil((int)to_int(args[0])); return 1; }
    L_bidjmp_6548: ;
    if ((_bid == BID_SNOx24PBC) && nargs == 3) { extern DESCR_t pat_mk_capt(int, const char *, DESCR_t); *out = pat_mk_capt((int)to_int(args[0]), VARVAL_fn(args[1]), args[2]); return 1; }
    L_bidjmp_6549: ;
    if ((_bid == BID_SNOx24PCUR) && nargs == 1) { extern DESCR_t pat_mk_cursor(const char *); *out = pat_mk_cursor(VARVAL_fn(args[0])); return 1; }
    L_bidjmp_6550: ;
    if ((_bid == BID_SNOx24PBALT) && nargs == 2) { extern DESCR_t pat_alt(DESCR_t, DESCR_t); *out = pat_alt(args[0], args[1]); return 1; }
    L_bidjmp_6551: ;
    if ((_bid == BID_SNOx24PARB) && nargs == 1) { extern DESCR_t pat_arbno(DESCR_t); *out = pat_arbno(args[0]); return 1; }
    L_bidjmp_6552: ;
    if ((_bid == BID_SNOx24PFEN) && nargs == 1) { extern DESCR_t pat_fence_p(DESCR_t); *out = pat_fence_p(args[0]); return 1; }
    L_bidjmp_6553: ;
    if ((_bid == BID_SNOx24PDEF) && nargs == 1) { extern DESCR_t pat_defer(const char *); *out = pat_defer(VARVAL_fn(args[0])); return 1; }
    L_bidjmp_6554: ;
    if ((_bid == BID_SNOx24MKPAT) && nargs >= 1) {
        extern void *rt_proc_get_fn(const char *name);
        const char *nm = VARVAL_fn(args[0]); if (!nm) nm = "";
        void *pf = rt_proc_get_fn(nm);
        if (!pf) { fprintf(stderr, "[SNO] SNO$MKPAT: compiled pattern blob '%s' not registered\n", nm); *out = FAILDESCR; return 1; }
        extern void *dtp_wrap_fn_sz(void *, int64_t, int32_t); extern long rt_fn_frame_bytes_known(void *); extern long rt_fn_zstatic_known(void *);
        DESCR_t pd; pd.v = DT_P; pd.slen = 0; pd.p = dtp_wrap_fn_sz(pf, (int64_t)rt_fn_frame_bytes_known(pf), (int32_t)rt_fn_zstatic_known(pf));
        if (nargs >= 2) { extern void rt_patv_freeze(void *, const char *, long); long _n = 0; if (IS_INT_fn(args[1])) _n = (long)args[1].i; else { const char *cs = VARVAL_fn(args[1]); _n = cs ? atol(cs) : 0; } if (_n > 0) rt_patv_freeze(pd.p, nm, _n); }
        *out = pd; return 1;
    }
    L_bidjmp_6563: ;
    if ((_bid == BID_OPSYN) && nargs >= 2) {
        extern DESCR_t opsyn(DESCR_t, DESCR_t, DESCR_t);
        *out = opsyn(args[0], args[1], nargs > 2 ? args[2] : NULVCL); return 1;
    }
    L_bidjmp_6567: ;
    if ((_bid == BID_CODE) && nargs == 1) {
        extern DESCR_t code(const char *);
        const char *cs = VARVAL_fn(args[0]);
        *out = cs ? code(cs) : FAILDESCR; return 1;
    }
    L_bidjmp_6572: ;
    if ((_bid == BID_EVAL) && nargs == 1) {
        extern DESCR_t rt_call_named_proc(const char *name, DESCR_t *args, int nargs);
        DESCR_t av = args[0];
        if (av.v == DT_X) { *out = rt_call_named_proc(av.s ? av.s : "", (DESCR_t *)0, 0); return 1; }
        if (IS_INT_fn(av) || IS_REAL_fn(av)) { *out = av; return 1; }
        if (av.v == DT_SNUL) { *out = NULVCL; return 1; }
        if (av.v == DT_S) { extern DESCR_t EVAL_fn(DESCR_t); *out = EVAL_fn(av); return 1; }
        *out = FAILDESCR; return 1;
    }
    L_bidjmp_6581: ;
    if ((_bid == BID_VALUE) && nargs == 1) {
        extern DESCR_t NV_GET_fn(const char *); extern DESCR_t rt_deref(DESCR_t);
        if (args[0].v == DT_N) { *out = rt_deref(args[0]); return 1; }
        const char *nm = VARVAL_fn(args[0]); if (!nm || !*nm) { *out = FAILDESCR; return 1; }
        *out = NV_GET_fn(nm); return 1;
    }
    L_bidjmp_6587: ;
    if ((_bid == BID_SNOx24NRET)) { extern int rt_g_ret_by_name; rt_g_ret_by_name = 1; *out = NULVCL; return 1; }
    L_bidjmp_6588: ;
    if ((_bid == BID_SNOx24WANTNM)) { extern int rt_g_want_name; rt_g_want_name = 1; *out = NULVCL; return 1; }
    L_bidjmp_6589: ;
    if ((_bid == BID_APPLY) && nargs >= 1) {
        const char *pn = (args[0].v == DT_N && args[0].slen == 0) ? args[0].s : VARVAL_fn(args[0]);
        if (!pn || !*pn) { *out = FAILDESCR; return 1; }
        extern int rt_proc_is_registered(const char *); extern DESCR_t rt_call_proc_descr(const char *, int); extern DESCR_t g_call_args[];
        if (rt_proc_is_registered(pn)) { int na = nargs - 1; if (na > 64) na = 64; for (int k = 0; k < na; k++) g_call_args[k] = args[k + 1]; *out = rt_call_proc_descr(pn, na); return 1; }
        return try_call_builtin_by_name(pn, args + 1, nargs - 1, out);
    }
    if (nargs == 1) {
        DESCR_t a = args[0];
        if (IS_FAIL_fn(a)) { *out = FAILDESCR; return 1; }
        if (fn[0]=='+' && fn[1]=='\0') {
            if (IS_INT_fn(a)||IS_REAL_fn(a)) { *out=a; return 1; }
            const char *s=VARVAL_fn(a); if(!s||!*s){*out=FAILDESCR;return 1;}
            char *e=NULL; long long iv=strtoll(s,&e,10); if(e&&*e=='\0'){*out=INTVAL(iv);return 1;}
            double dv=strtod(s,&e); if(e&&*e=='\0'){*out=REALVAL(dv);return 1;}
            *out=FAILDESCR; return 1;
        }
        if (fn[0]=='-' && fn[1]=='\0') {
            _OPCOERCE(a);
            *out = IS_REAL_fn(a) ? REALVAL(-a.r) : INTVAL(-a.i); return 1;
        }
        if (fn[0]=='*' && fn[1]=='\0') {
            if (IS_INT_fn(a)||IS_REAL_fn(a)) { *out=INTVAL(1); return 1; }
            if (a.v==DT_DATA && a.u && a.u->type) { *out=INTVAL(a.u->type->nfields); return 1; }
            const char *s=VARVAL_fn(a); *out=INTVAL(s?(long long)strlen(s):0LL); return 1;
        }
        if (fn[0]=='!' && fn[1]=='\0') {
            if (IS_INT_fn(a)||IS_REAL_fn(a)) { *out=a; return 1; }
            const char *s=VARVAL_fn(a);
            if (s&&*s) { char *ch=rt_ws_alloc(2); ch[0]=s[0]; ch[1]='\0'; *out=STRVAL(ch); return 1; }
            *out=FAILDESCR; return 1;
        }
        if (fn[0]=='/' && fn[1]=='\0') {
            if (IS_INT_fn(a))  { *out=(a.i==0)?a:FAILDESCR; return 1; }
            if (IS_REAL_fn(a)) { *out=(a.r==0.0)?a:FAILDESCR; return 1; }
            *out=FAILDESCR; return 1;
        }
        if (fn[0]=='\\' && fn[1]=='\0') {
            *out=(a.v==DT_SNUL)?FAILDESCR:a; return 1;
        }
        if (fn[0]=='~' && fn[1]=='\0') {
            const char *s=NULL; int slen=-1;
            if (IS_INT_fn(a)) { char *nb=rt_ws_alloc(32); snprintf(nb,32,"%lld",(long long)a.i); s=nb; }
            else if (IS_REAL_fn(a)) { char *nb=rt_ws_alloc(64); icon_real_str(a.r,nb,64); s=nb; }
            else { s=VARVAL_fn(a); if (IS_CSET_fn(a)) slen=kw_cset_len(s); }
            if(!s) s="";
            /* NOT `for(;*p;p++)` (icon-ascii-cset-keywords-built-off-by-one): stops scanning at the
               first byte-0 member, so a correctly-built &ascii/&cset (which now legitimately starts
               with chr(0)) read as an EMPTY input set -- the complement then wrongly claimed almost
               every byte. kw_cset_len(s), when s is a registered keyword cset, gives the true length
               without touching the general embedded-NUL literal gap (still open, still out of scope). */
            if (slen<0) slen=(int)strlen(s);
            unsigned char in_set[256]={0}; for (int i=0;i<slen;i++) in_set[(unsigned char)s[i]]=1;
            char *buf=rt_ws_alloc(256); int n=0;
            for(int c=1;c<256;c++) if(!in_set[c]) buf[n++]=(char)c; buf[n]='\0';
            *out=STRVAL(buf); return 1;
        }
        if (fn[0]=='?' && fn[1]=='\0') {
            if (IS_INT_fn(a)) { *out=(a.i>0)?INTVAL((long long)(rand()%(int)a.i)+1):FAILDESCR; return 1; }
            if (IS_REAL_fn(a)) { *out=REALVAL((double)rand()/RAND_MAX*a.r); return 1; }
            const char *s=VARVAL_fn(a);
            if (s&&*s) { int n=(int)strlen(s); char *ch=rt_ws_alloc(2); ch[0]=s[rand()%n]; ch[1]='\0'; *out=STRVAL(ch); return 1; }
            *out=FAILDESCR; return 1;
        }
    }
    if (nargs == 2) {
        DESCR_t l=args[0], r=args[1];
        if (IS_FAIL_fn(l)||IS_FAIL_fn(r)) { *out=FAILDESCR; return 1; }
        if (fn[0]=='+' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=add(l,r); return 1; }
        if (fn[0]=='-' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=sub(l,r); return 1; }
        if (fn[0]=='*' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=mul(l,r); return 1; }
        if (fn[0]=='/' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=DIVIDE_fn(l,r); return 1; }
        if (fn[0]=='%' && fn[1]=='\0') {
            _OPCOERCE(l); _OPCOERCE(r);
            long li=IS_INT_fn(l)?l.i:(long)l.r, ri=IS_INT_fn(r)?r.i:(long)r.r;
            *out=ri?INTVAL(li%ri):FAILDESCR; return 1;
        }
        if (fn[0]=='^' && fn[1]=='\0') {
            _OPCOERCE(l); _OPCOERCE(r);
            if (IS_INT_fn(l)&&IS_INT_fn(r)&&r.i>=0) {
                long long base=l.i, res=1; for(int k=0;k<(int)r.i;k++) res*=base;
                *out=INTVAL(res); return 1;
            }
            if (IS_INT_fn(l)&&IS_INT_fn(r)&&r.i<0) {
                double rv=pow((double)l.i,(double)r.i);
                *out=INTVAL((long long)rv); return 1;
            }
            double base=IS_REAL_fn(l)?l.r:(double)l.i, exp=IS_REAL_fn(r)?r.r:(double)r.i;
            *out=(DESCR_t){.v=DT_R,.r=pow(base,exp)}; return 1;
        }
        if ((_bid == BID_x3C))  _NUMREL(<);
        if ((_bid == BID_x3Cx3D)) _NUMREL(<=);
        if ((_bid == BID_x3E))  _NUMREL(>);
        if ((_bid == BID_x3Ex3D)) _NUMREL(>=);
        if ((_bid == BID_x3D))  _NUMREL(==);
        if ((_bid == BID_x7Ex3D)) _NUMREL(!=);
        if ((_bid == BID_x3Cx3C))  _STRREL(<);
        if ((_bid == BID_x3Cx3Cx3D)) _STRREL(<=);
        if ((_bid == BID_x3Ex3E))  _STRREL(>);
        if ((_bid == BID_x3Ex3Ex3D)) _STRREL(>=);
        if ((_bid == BID_x3Dx3D))  _STRREL(==);
        if ((_bid == BID_x7Ex3Dx3D)) _STRREL(!=);
        if ((_bid == BID_x3Dx3Dx3D)) {
            extern int descr_identical(DESCR_t, DESCR_t);
            *out=descr_identical(l,r)?r:FAILDESCR; return 1;
        }
        if ((_bid == BID_x7Ex3Dx3Dx3D)) {
            extern int descr_identical(DESCR_t, DESCR_t);
            *out=descr_identical(l,r)?FAILDESCR:r; return 1;
        }
        if ((_bid == BID_x5Bx5D)) {
            *out=subscript_get(l,r); return 1;
        }
        if ((_bid == BID_x2Bx2B) || (_bid == BID_x2Dx2D) || (_bid == BID_x2Ax2A)) {
            if (l.v == DT_T && l.tbl && l.tbl->is_set && r.v == DT_T && r.tbl && r.tbl->is_set) {
                if (fn[0]=='+') *out=TABLE_VAL(set_union(l.tbl, r.tbl));
                else if (fn[1]=='-') *out=TABLE_VAL(set_diff(l.tbl, r.tbl));
                else *out=TABLE_VAL(set_inter(l.tbl, r.tbl));
                return 1;
            }
            char _lbuf[64], _rbuf[64];
            const char *la, *ra;
            if (IS_INT_fn(l))       { snprintf(_lbuf,sizeof _lbuf,"%lld",(long long)l.i); la=_lbuf; }
            else if (IS_REAL_fn(l)) { icon_real_str(l.r,_lbuf,sizeof _lbuf); la=_lbuf; }
            else                    { la=VARVAL_fn(l); if(!la) la=""; }
            if (IS_INT_fn(r))       { snprintf(_rbuf,sizeof _rbuf,"%lld",(long long)r.i); ra=_rbuf; }
            else if (IS_REAL_fn(r)) { icon_real_str(r.r,_rbuf,sizeof _rbuf); ra=_rbuf; }
            else                    { ra=VARVAL_fn(r); if(!ra) ra=""; }
            int lalen = IS_CSET_fn(l) ? kw_cset_len(la) : -1; if (lalen < 0) lalen = (int)strlen(la);
            int ralen = IS_CSET_fn(r) ? kw_cset_len(ra) : -1; if (ralen < 0) ralen = (int)strlen(ra);
            if (fn[0]=='+') *out=CSETVAL(cset_canonical(cset_union(la,lalen,ra,ralen)));
            else if (fn[1]=='-') *out=CSETVAL(cset_canonical(cset_diff(la,lalen,ra,ralen)));
            else *out=CSETVAL(cset_canonical(cset_inter(la,lalen,ra,ralen)));
            return 1;
        }
    }
#undef _OPCOERCE
#undef _NUMREL
#undef _STRREL
    {
        DatType *_rdt = dat_find_type(fn);
        if (_rdt) { *out = dat_construct(_rdt, args, nargs); return 1; }
    }
    if (script_try_call_builtin_by_name(fn, args, nargs, out)) return 1;
    { DatType *dt = dat_find_type(fn);
      if (dt && nargs <= dt->nfields) {
          DESCR_t fv[64]; int nf = dt->nfields > 64 ? 64 : dt->nfields;
          for (int i = 0; i < nf; i++) fv[i] = (i < nargs) ? args[i] : NULVCL;
          *out = dat_construct(dt, fv, nf); return 1;
      } }
    { extern int rt_dat_field_of_any(const char *);
      if (nargs == 1 && args[0].v == DT_DATA && rt_dat_field_of_any(fn)) {
          extern DESCR_t dat_field_get(const char *field, DESCR_t obj);
          *out = dat_field_get(fn, args[0]); return 1;
      } }
    if (!_dx_skip_syn) { extern const char *rt_builtin_synonym(const char *);
      const char *syn = rt_builtin_synonym(fn);
      if (syn) { if (_dx && !_dx_hit) { _dx->gen = rt_dtax_gen; _dx->len = _dxl; _dx->kind = 2; _dx->nf = 0; memcpy(_dx->nm, fn, _dxl); _dx->ctor = 0; _dx->syn = syn; }
        return try_call_builtin_by_name(syn, args, nargs, out); }
      if (_dx && !_dx_hit) { _dx->gen = rt_dtax_gen; _dx->len = _dxl; _dx->kind = 0; _dx->nf = 0; memcpy(_dx->nm, fn, _dxl); _dx->ctor = 0; _dx->syn = 0; } }
    { extern int core_call_registered_fn(const char *, DESCR_t *, int, DESCR_t *);
      char c0 = fn[0];
      if (c0 && !((c0 >= 'A' && c0 <= 'Z') || (c0 >= 'a' && c0 <= 'z') || c0 == '_')
          && core_call_registered_fn(fn, args, nargs, out)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_dat_field_of_any(const char *name) {
    extern int dat_type_count(void); extern int dat_type_nfields(int); extern const char *dat_type_field(int, int);
    if (!name || !name[0]) return 0;
    for (int c = 0; c < dat_type_count(); c++) for (int f = 0; f < dat_type_nfields(c); f++) { const char *fn2 = dat_type_field(c, f); if (fn2 && !strcmp(fn2, name)) return 1; }
    return 0;
}
#define RT_SYN_MAX 64
static const char *g_rt_syn_new[RT_SYN_MAX]; static const char *g_rt_syn_old[RT_SYN_MAX]; static int g_rt_syn_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_builtin_synonym_add(const char *newname, const char *oldname) {
    if (!newname || !oldname || g_rt_syn_n >= RT_SYN_MAX) return;
    rt_dtax_gen++;
    for (int i = 0; i < g_rt_syn_n; i++) if (!strcmp(g_rt_syn_new[i], newname)) { g_rt_syn_old[i] = oldname; return; }
    g_rt_syn_new[g_rt_syn_n] = newname; g_rt_syn_old[g_rt_syn_n] = oldname; g_rt_syn_n++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_builtin_synonym(const char *name) {
    if (!name) return (const char *)0;
    for (int i = 0; i < g_rt_syn_n; i++) if (!strcmp(g_rt_syn_new[i], name)) return g_rt_syn_old[i];
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t sno_array_from_proto(const char *proto, DESCR_t init) {
    extern ARBLK_t *array_new(int lo, int hi);
    if (!proto || !*proto) return FAILDESCR;
    char buf[128]; int bn = 0;
    for (const char *q = proto; *q && *q != ',' && bn < 127; q++) buf[bn++] = *q;
    buf[bn] = 0;
    const char *rest = proto[bn] == ',' ? proto + bn + 1 : (const char *)0;
    long lo = 1, hi;
    char *colon = strchr(buf, ':');
    if (colon) { *colon = 0; lo = strtol(buf, (char **)0, 10); hi = strtol(colon + 1, (char **)0, 10); }
    else hi = strtol(buf, (char **)0, 10);
    if (hi < lo - 1) return FAILDESCR;
    ARBLK_t *a = array_new((int)lo, (int)hi);
    if (!a) return FAILDESCR;
    int n = (int)(hi - lo + 1);
    for (int k = 0; k < n; k++) a->data[k] = rest ? sno_array_from_proto(rest, init) : init;
    DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_A; d.slen = 0; d.arr = a;
    return d;
}
