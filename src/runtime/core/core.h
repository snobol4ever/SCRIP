#ifndef SNOBOL4_H
#define SNOBOL4_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "descr.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline size_t descr_slen(DESCR_t d) {
    if (d.v == DT_S) {
        if (d.slen == 0xFFFFFFFFu) return d.s ? strlen(d.s) : 0;
        if (d.slen) return (size_t)d.slen;
        return d.s ? strlen(d.s) : 0;
    }
    return 0;
}
#define NULVCL    ((DESCR_t){ .v = DT_SNUL, .slen = 0, .s = "" })
#define STRVAL(s_) ((DESCR_t){ .v = DT_S,  .slen = 0, .s = (s_) })
#define BSTRVAL(s_, len_) ((DESCR_t){ .v = DT_S, .slen = (uint32_t)(len_), .s = (s_) })
#define INTVAL(i_) ((DESCR_t){ .v = DT_I,  .i = (i_) })
#define REALVAL(r_)((DESCR_t){ .v = DT_R, .r = (r_) })
#define CSETVAL(s_) ((DESCR_t){ .v = DT_S, .slen = 0xFFFFFFFFu, .s = (s_) })
#define NAMEPTR(dp_) ((DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void*)(dp_) })
#define NAMEVAL(s_)  ((DESCR_t){ .v = DT_N, .slen = 0, .s = (char *)(s_) })
#define STYPE(v_)    ((v_).v)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int IS_NULL_fn(DESCR_t v)  { return v.v == DT_SNUL || (v.v == DT_S && v.slen == 0 && (!v.s || !*v.s)); }
static inline int IS_STR_fn(DESCR_t v)   { return v.v == DT_S || v.v == DT_SNUL; }
static inline int IS_INT_fn(DESCR_t v)   { return v.v == DT_I; }
static inline int IS_REAL_fn(DESCR_t v)  { return v.v == DT_R; }
static inline int IS_CSET_fn(DESCR_t v)  { return v.v == DT_S && v.slen == 0xFFFFFFFFu; }
static inline int IS_DATA_fn(DESCR_t v)  { return v.v == DT_DATA; }
char *VARVAL_fn(DESCR_t v);
DESCR_t INVOKE_fn(const char *name, DESCR_t *args, int nargs);
DESCR_t dat_field_call(const char *name, DESCR_t *args, int nargs);
DESCR_t ARGVAL_fn(DESCR_t d);
DESCR_t VARVAL_d_fn(DESCR_t d);
const char *rt_sno_indirect_name(DESCR_t v);
DESCR_t INTVAL_fn(DESCR_t d);
DESCR_t PATVAL_fn(DESCR_t d);
DESCR_t VARVUP_fn(DESCR_t d);
/* ⭐⭐ ONE PASS INSTEAD OF libc's NUMBER PARSER (hq_P s262).  MEASURED across the 17-kernel field, marginal profiles:
   `mixed_workload` -- the fair single number for a realistic program, 3.28x slower than SPITBOL -- spends
   __strtod_l_internal 8.81% + ____strtol_l_internal 4.57% + str_to_mpn 2.55% = 15.9% of its steady-state
   instructions inside libc's locale-aware number parser.  `operand_is_real_str` was calling strtoll AND strtod on
   every string operand purely to decide "integer or real", and `is_numeric_like` called strtod on every string to
   decide "is this numeric at all".  strtod is not a cheap function: it is locale-sensitive and any longish digit
   run walks its bignum path, which is what put str_to_mpn on the profile.
   ⭐ A PLAIN INTEGER NEEDS NO CONVERSION TO CLASSIFY -- optional blanks, optional sign, digits, optional blanks,
   NUL.  That is a single character scan and it is the overwhelmingly common case in this corpus.
   ⛔ IT CANNOT ANSWER DIFFERENTLY, ONLY SOONER, and both callers are checked against libc's own behaviour:
   is_numeric_like returns 1 for such a string because strtod consumes the digits and leaves only blanks;
   operand_is_real_str returns 0 because strtod and strtoll consume exactly the same span (`endd <= endi`).
   Everything else -- a '.', an exponent, "inf"/"nan", a 0x hex float, leading junk, overflow -- falls through to
   the UNCHANGED libc path, which is why none of those cases had to be re-implemented here.
   ⛔ always_inline, not plain `inline`: under the s262 NO-`-O2` fact rule -O0 is the number of record, and at -O0
   gcc emits a real call for a `static inline` -- a lesson this seat paid 0.80% to learn earlier the same day. */
static inline __attribute__((always_inline)) int rt_plain_int_str(const char *s) {
    if (!s) return 0;
    while (*s == ' ' || *s == '\t') s++;
    if (*s == '+' || *s == '-') s++;
    if (*s < '0' || *s > '9') return 0;
    while (*s >= '0' && *s <= '9') s++;
    while (*s == ' ' || *s == '\t') s++;
    return *s == '\0';
}
int is_numeric_like(DESCR_t d);
int64_t to_int(DESCR_t v);
double to_real(DESCR_t v);
char *STRCONCAT_fn(const char *a, const char *b);
DESCR_t CONCAT_fn(DESCR_t a, DESCR_t b);
char *STRDUP_fn(const char *s);
int64_t size(const char *s);
const char *datatype(DESCR_t v);
typedef struct _TREEBLK_t {
    char   *tag;
    DESCR_t  val;
    int     n;
    int     cap;
    struct _TREEBLK_t **c;
} TREEBLK_t;
TREEBLK_t *expr_new(const char *tag, DESCR_t val);
TREEBLK_t *tree_new0(const char *tag);
void  tree_append(TREEBLK_t *x, TREEBLK_t *y);
void  tree_prepend(TREEBLK_t *x, TREEBLK_t *y);
void  tree_insert(TREEBLK_t *x, TREEBLK_t *y, int place);
TREEBLK_t *tree_remove(TREEBLK_t *x, int place);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char *t(TREEBLK_t *x) { return x ? x->tag : ""; }
static inline DESCR_t      v(TREEBLK_t *x) { return x ? x->val  : NULVCL; }
static inline int         n(TREEBLK_t *x) { return x ? x->n    : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline TREEBLK_t       *c_i(TREEBLK_t *x, int i) {
    if (!x || i < 1 || i > x->n) return NULL;
    return x->c[i-1];
}
typedef struct _ARBLK_t {
    int     lo, hi;
    int     ndim;
    int     lo2, hi2;
    int     proto_bare;
    const char *proto;
    DESCR_t *data;
    long    id;
} ARBLK_t;
long rt_agg_serial_list(void);
long rt_agg_serial_table(void);
ARBLK_t *array_new(int lo, int hi);
ARBLK_t *array_new2d(int lo1, int hi1, int lo2, int hi2);
DESCR_t    array_get(ARBLK_t *a, int i);
void      array_set(ARBLK_t *a, int i, DESCR_t v);
DESCR_t    array_get2(ARBLK_t *a, int i, int j);
void      array_set2(ARBLK_t *a, int i, int j, DESCR_t v);
/*⭐⭐ THE TABLE IS A SORTED-INDEX HASH (Lon, 2026-08-23 s262).  Hash by DATATYPE first, then by VALUE; one
  CONTIGUOUS array per bucket; BINARY SEARCH inside the bucket.  `TBPAIR_t.next` is RETIRED -- buckets are no
  longer chains -- and its 8 bytes now carry `hkey`, the (datatype,value) sort key.
  ⛔ 48 BYTES, key@0, val@24 STILL PINNED by _Static_assert in rtx_init.c; only the last member changed name.
  ⭐⭐ THE ENTRIES ARE INLINE IN THE BUCKET, AND THAT IS LON'S RULING (2026-08-23 s262), verbatim: "all references are
  supposed to be downstream so that everything is easily slidable.  And we should never have in our code a place that
  depends on that pointer not moving."
  ⛔ THE FIRST CUT OF THIS FILE GOT THAT BACKWARDS.  It put entries OUT of line and kept them still, because
  pattern_match.c minted a VCELL_t holding `cellp = &e->val` and stored through it later -- so the data structure was
  bent around one caller's assumption that an address into it would stay valid.  That assumption is the bug, not the
  constraint: it forced a whole extra indirection layer AND it required pinning table blocks against a COMPACTING
  collector.  The caller was fixed instead -- a table VCELL now names (tbl, key_descr) and re-resolves on use, which
  also picks up the table's default value correctly, which the raw cell pointer never did.
  ⭐ SO A BUCKET IS ONE CONTIGUOUS BLOCK OF ENTRIES, hkey-sorted, binary-searched, and its only outbound pointers are
  `ent` and `key` -- both downstream, both slidable, nothing anywhere holding an interior address across a statement. */
typedef struct _TBBLK_tEntry {
    char              *key;
    DESCR_t            key_descr;
    DESCR_t            val;
    unsigned long long hkey;
} TBPAIR_t;
/*⭐⭐ len AND cap LIVE IN THE ENTRY BLOCK, NOT IN THE BUCKET VECTOR -- AND THAT IS A MEASUREMENT, NOT A TASTE.
  The first cut made a bucket {TBPAIR_t *ent; unsigned len, cap} = 16 bytes, which DOUBLED TBBLK_t from 2,088 to
  4,160 bytes.  Every TABLE() memsets that vector, and programs that build MANY SMALL TABLES pay it per table:
  CLAWS5 (a 3-level TABLE of TABLE of TABLES, one inner table per num and per word) went to 114.7M instructions
  from 118.7M -- FEWER instructions -- yet 68.4M cycles against 63.8M, IPC 1.65 against 1.82, and 5% slower on the
  wall.  The work got cheaper and the program got slower, because the clearing and the sparse bucket vector
  evicted everything else.  ⛔ INSTRUCTION COUNT COULD NOT SEE THIS AND CALLGRIND Ir SAID THE OPPOSITE: `rep
  stosb` is one instruction whatever the length.  perf cycles/IPC was the only instrument that could answer.
  ⭐ So the bucket is a bare pointer again (8 bytes, TBBLK_t back to 2,088 = the chained original's size), an
  EMPTY bucket is NULL and allocates nothing, and len/cap ride in the entry block -- which is strictly better
  locality than the first cut had, because they now share a cache line with ent[0] instead of sitting in a
  separate 4 KB vector. */
typedef struct _TBBUCK_t { unsigned len, cap; TBPAIR_t ent[]; } TBBUCK_t;
/*⭐⭐ 256 BUCKETS -- KEPT, AND KEPT ON A MEASUREMENT THAT CONTRADICTED THE ARGUMENT FOR CHANGING IT (s262).
  THE ARGUMENT WAS PLAUSIBLE AND WRONG.  A CHAINED bucket had to stay shallow because depth was linear cost, so 256 heads were the price of speed; a SEARCHED bucket
  supposedly inverts that -- depth is cheap, so fewer and deeper should be strictly better, and TBBLK_t would shrink from 4.1 KB to 1 KB with it.  The sweep says no.
  callgrind Ir at fixed work (200 rebuilds x 500 integer keys, RT_OPT=-O0), chained baseline 185,809,919:
        256 -> 180,173,520 (-3.0%)      128 -> 184,480,220 (-0.7%)      64 -> 189,515,120 (+2.0%)      32 -> 200,307,020 (+7.8%)
  Monotone, and it crosses the baseline between 128 and 64.  The reason is that the search is LINEAR below TBL_LINEAR_MAX, which is where every one of these depths
  lands, so halving the bucket count does not buy a halved binary search -- it buys a doubled linear scan, and the bucket vector it saves is memset by `rep stosb`,
  which is nearly free.  Depth is only cheap once it is deep enough to reach the binary arm, and a table that deep is not what SNOBOL4 programs build.
  ⛔ TWO MEASUREMENT TRAPS ON THE WAY TO THIS NUMBER, BOTH RECORDED BECAUSE BOTH LOOKED LIKE ANSWERS: (1) the first sweep changed this constant WITHOUT updating the
  _Static_assert in rtx_init.c that pinned it, so make failed and every arm silently re-measured the 256 binary -- the differing wall-clock figures were pure box
  noise presented as a result.  (2) wall-clock on this box swings +/-30% run to run (6,144..8,704 iters for ONE binary), so it cannot resolve a 3% effect at all.
  Ir at fixed work was the only instrument that could answer, which is what RULES.md says to reach for first. */
#define TABLE_BUCKETS 256
typedef struct _TBBLK_t {
    TBBUCK_t      *buckets[TABLE_BUCKETS];
    int            size;
    int            init, inc;
    int            is_set;
    DESCR_t        dflt;
    long           id;
} TBBLK_t;
/*⭐ THE ONLY SANCTIONED WALKS.  A bucket is {slot,len,cap}, never a chain -- `for (e = t->buckets[b]; e; e = e->next)`
  no longer compiles, which is deliberate: it is how every one of the twelve former chain walks was found.  Entries
  are dense in slot[0..len-1] (delete compacts), so the null test doubles as the loop bound.  Caller declares e_. */
#define TBL_FOREACH(t_, e_)            for (int _tb = 0; _tb < TABLE_BUCKETS; _tb++) if ((t_)->buckets[_tb]) for (unsigned _ts = 0; _ts < (t_)->buckets[_tb]->len && ((e_) = &(t_)->buckets[_tb]->ent[_ts]) != (TBPAIR_t *)0; _ts++)
#define TBL_BUCKET_FOREACH(t_, b_, e_) if ((t_)->buckets[b_]) for (unsigned _ts = 0; _ts < (t_)->buckets[b_]->len && ((e_) = &(t_)->buckets[b_]->ent[_ts]) != (TBPAIR_t *)0; _ts++)
TBBLK_t *table_new(void);
TBBLK_t *table_new_args(int init, int inc);
DESCR_t agg_prototype(DESCR_t v);
const char *tbl_key_str(DESCR_t kd, char *buf, size_t bufn);
/* ⭐ DESCRIPTOR-KEYED TABLE API -- hash by DATATYPE, then by VALUE (Lon s262).  These are the ONLY sound entry
   points once any key is hashed by value: a table whose entries were placed by _tbl_hash_d cannot be found by the
   string-keyed calls above, which hash the ENCODED key as text.  Convert callers, do not mix. */
/*⭐ THE PRINTABLE KEY IS LAZY (s262).  e->key is NOT built on insert any more -- nothing on the hot path reads it, because
   _tbl_eq_d compares key_descr.  Iteration, CONVERT, sorting and set output go through this accessor, which materialises
   the encoding on FIRST demand and caches it in the entry.  ⛔ It ALLOCATES: never call it from the collector -- gc_heap.c
   tests the raw e->key field instead, and a null there simply means "never printed". */
const char *tbl_pair_key(TBPAIR_t *e);
TBPAIR_t  *table_find_pair_d(TBBLK_t *tbl, DESCR_t k);   /* rtx_table.S owns this symbol; RTX_GATE tail-jumps to the C body below */
TBPAIR_t  *c_table_find_pair_d(TBBLK_t *tbl, DESCR_t k);
DESCR_t    table_get_d(TBBLK_t *tbl, DESCR_t k);
DESCR_t    table_get_found_d(TBBLK_t *tbl, DESCR_t k, int *found);
int        table_has_d(TBBLK_t *tbl, DESCR_t k);
int        table_delete_d(TBBLK_t *tbl, DESCR_t k);
void       table_set_descr_d(TBBLK_t *tbl, DESCR_t k, DESCR_t val);
TBBLK_t  *set_union(TBBLK_t *x, TBBLK_t *y);
TBBLK_t  *set_diff(TBBLK_t *x, TBBLK_t *y);
TBBLK_t  *set_inter(TBBLK_t *x, TBBLK_t *y);
typedef struct _DATINST_tType {
    char   *name;
    int     nfields;
    char  **fields;
    struct _DATINST_tType *next;
    long    serial_next;
} DATBLK_t;
typedef struct _DATINST_t {
    DATBLK_t *type;
    DESCR_t   *fields;
    long      id;
} DATINST_t;
void DEFDAT_fn(const char *spec);
DESCR_t DATCON_fn(const char *type_name, ...);
DESCR_t FIELD_GET_fn(DESCR_t obj, const char *field);
void    FIELD_SET_fn(DESCR_t obj, const char *field, DESCR_t val);
void   FIELD_SET_fn(DESCR_t obj, const char *field, DESCR_t val);
DESCR_t  NV_GET_fn(const char *name);
DESCR_t  NV_SET_fn(const char *name, DESCR_t val);
void    NV_CLEAR_fn(void);
typedef struct { const char *name; DESCR_t val; } NvPair;
int     nv_snapshot(NvPair **out);
void    nv_restore(const NvPair *pairs, int n);
DESCR_t  INDR_GET_fn(const char *name);
void    INDR_SET_fn(const char *name, DESCR_t val);
DESCR_t  NAME_fn(const char *varname);
int      ASGNIC_fn(const char *kw_name, DESCR_t val);
void    NAME_pop(void);
void    NAME_commit(void);
typedef struct NAME_ctx_s {
    void              *entries;
    int                cap;
    int                top;
    struct NAME_ctx_s *parent;
} NAME_ctx_t;
void    NAME_ctx_enter(NAME_ctx_t *ctx);
void    NAME_ctx_leave(void);
void    NPUSH_fn(void);
int     NHAS_FRAME_fn(void);
int     NTOP_INDEX_fn(void);
int64_t NSTACK_AT_fn(int frame);
void    NINC_fn(void);
void    NINC_AT_fn(int frame);
void    NDEC_fn(void);
int64_t ntop(void);
void    NPOP_fn(void);
void   PUSH_fn(DESCR_t v);
DESCR_t POP_fn(void);
DESCR_t TOP_fn(void);
int    STACK_DEPTH_fn(void);
typedef DESCR_t (*FNCPTR_t)(DESCR_t *args, int nargs);
void    DEFINE_fn(const char *spec, FNCPTR_t fn);
void    DEFINE_fn_entry(const char *spec, FNCPTR_t fn, const char *entry_label);
void    register_fn_alias(const char *newname, const char *oldname);
int     core_call_registered_fn(const char *name, DESCR_t *args, int nargs, DESCR_t *out);
DESCR_t  APPLY_fn(const char *name, DESCR_t *args, int nargs);
int     FNCEX_fn(const char *name);
int         FUNC_NPARAMS_fn(const char *fname);
int         FUNC_NLOCALS_fn(const char *fname);
const char *FUNC_PARAM_fn(const char *fname, int i);
const char *FUNC_LOCAL_fn(const char *fname, int i);
const char *FUNC_ENTRY_fn(const char *fname);
int     FUNC_IS_ENTRY_LABEL(const char *label);
#define RETCODE_SUCCEED  0
#define RETCODE_CONCEDE  1
#define SGOTO     2
#define SRETURN   3
#define FRETURN  4
#define NRETURN  5
#define RETCODE_END  6
DESCR_t SIZE_fn(DESCR_t s);
DESCR_t DUPL_fn(DESCR_t s, DESCR_t n);
DESCR_t REPLACE_fn(DESCR_t s, DESCR_t from, DESCR_t to);
DESCR_t SUBSTR_fn(DESCR_t s, DESCR_t i, DESCR_t n);
DESCR_t TRIM_fn(DESCR_t s);
DESCR_t lpad_fn(DESCR_t s, DESCR_t n, DESCR_t pad);
DESCR_t rpad_fn(DESCR_t s, DESCR_t n, DESCR_t pad);
DESCR_t REVERS_fn(DESCR_t s);
DESCR_t BCHAR_fn(DESCR_t n);
DESCR_t INTGER_fn(DESCR_t v);
DESCR_t real_fn(DESCR_t v);
DESCR_t string_fn(DESCR_t v);
int eq(DESCR_t a, DESCR_t b);
int ne(DESCR_t a, DESCR_t b);
int lt(DESCR_t a, DESCR_t b);
int le(DESCR_t a, DESCR_t b);
int gt(DESCR_t a, DESCR_t b);
int ge(DESCR_t a, DESCR_t b);
int ident(DESCR_t a, DESCR_t b);
int differ(DESCR_t a, DESCR_t b);
DESCR_t add(DESCR_t a, DESCR_t b);
DESCR_t sub(DESCR_t a, DESCR_t b);
DESCR_t mul(DESCR_t a, DESCR_t b);
DESCR_t DIVIDE_fn(DESCR_t a, DESCR_t b);
DESCR_t POWER_fn(DESCR_t a, DESCR_t b);
DESCR_t neg(DESCR_t a);
DESCR_t pos(DESCR_t a);
void   output_val(DESCR_t v);
DESCR_t input_read(void);
void   output_str(const char *s);
extern int monitor_fd;
extern int g_monitor_bin;
extern int monitor_quiet_depth;
void comm_stno(int n);
void comm_var(const char *name, DESCR_t val);
void mon_emit_label_bin(int64_t stno);
void mon_emit_value_bin(const char *name, DESCR_t val);
void mon_emit_call_bin(const char *fname);
void mon_emit_return_bin(const char *fname, DESCR_t retval);
void comm_call(const char *fname);
void comm_return(const char *fname, DESCR_t retval);
int  trace_is_active(const char *name);
extern int64_t kw_fullscan;
extern int64_t kw_maxlngth;
extern int64_t kw_anchor;
extern int64_t kw_trim;
extern int64_t kw_stlimit;
extern int64_t kw_stcount;
extern int64_t kw_ftrace;
extern int64_t kw_trace;
extern int64_t kw_errlimit;
extern int64_t kw_code;
extern int64_t kw_fnclevel;
extern char    kw_rtntype[16];
#include <setjmp.h>
void core_runtime_error(int code, const char *msg);
extern int g_kw_ctx;
extern jmp_buf g_core_err_jmp;
extern int     g_core_err_active;
extern int     g_core_err_stmt;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int core_err_is_terminal(int code) {
    switch (code) {
        case 20: case 21: case 22: case 23:
        case 26: case 27: case 29: case 30:
        case 31: case 39:
            return 1;
        default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int core_err_is_fatal(int code) {
    switch (code) {
        case 19: case 24: case 25: case 35:
            return 1;
        default: return 0;
    }
}
extern char ucase[27];
extern char lcase[27];
extern char alphabet[257];
extern char digits[11];
void core_lib_init(void);
DESCR_t pat_lit(const char *s);
DESCR_t pat_span(const char *chars);
DESCR_t pat_break_(const char *chars);
DESCR_t pat_breakx (const char *chars);
DESCR_t pat_any_cs(const char *chars);
DESCR_t pat_notany(const char *chars);
DESCR_t pat_len(int64_t n);
DESCR_t pat_pos(int64_t n);
DESCR_t pat_rpos(int64_t n);
DESCR_t pat_tab(int64_t n);
DESCR_t pat_rtab(int64_t n);
DESCR_t pat_arb(void);
DESCR_t pat_arbno(DESCR_t inner);
DESCR_t pat_rem(void);
DESCR_t pat_fence(void);
DESCR_t pat_fence_p(DESCR_t inner);
DESCR_t pat_fail(void);
DESCR_t pat_abort(void);
DESCR_t pat_succeed(void);
DESCR_t pat_bal(void);
DESCR_t pat_epsilon(void);
DESCR_t pat_cat(DESCR_t left, DESCR_t right);
DESCR_t pat_alt(DESCR_t left, DESCR_t right);
DESCR_t pat_ref(const char *name);
DESCR_t pat_assign_imm(DESCR_t child, DESCR_t var);
DESCR_t pat_assign_cond(DESCR_t child, DESCR_t var);
DESCR_t pat_assign_callcap(DESCR_t child, const char *fnc_name, DESCR_t *args, int nargs);
DESCR_t pat_assign_callcap_named(DESCR_t child, const char *fnc_name,
                                  DESCR_t *args, int nargs,
                                  char **arg_names, int n_arg_names);
DESCR_t pat_assign_callcap_named_imm(DESCR_t child, const char *fnc_name,
                                      DESCR_t *args, int nargs,
                                      char **arg_names, int n_arg_names);
DESCR_t var_as_pattern(DESCR_t v);
DESCR_t pat_user_call(const char *name, DESCR_t *args, int nargs);
DESCR_t subscript_get(DESCR_t arr, DESCR_t idx);
DESCR_t rt_subscript_var(DESCR_t base, DESCR_t idx);
DESCR_t rt_subscript_var_container_only(DESCR_t base, DESCR_t idx);
DESCR_t rt_deref(DESCR_t d);
DESCR_t rt_assign_var(DESCR_t var, DESCR_t val);
DESCR_t c_rt_assign_var(DESCR_t var, DESCR_t val);
DESCR_t rt_cset_compl(DESCR_t a);
DESCR_t *NV_PTR_fn(const char *name);
int NV_bind_gva(const char *name, DESCR_t *cell);
int NV_EXISTS_fn(const char *name);
DESCR_t NV_KW_GET_fn(const char *name);
DESCR_t NV_KW_SET_fn(const char *name, DESCR_t val);
int NV_CONST_ASSIGNED_fn(const char *name);
DESCR_t *gva_register(const char **names, DESCR_t *cells, int n);
const char *NV_name_from_ptr(const DESCR_t *ptr);
extern DESCR_t (*g_eval_pat_hook)(DESCR_t pat);
extern DESCR_t (*g_eval_str_hook)(const char *s);
DESCR_t *array_ptr(ARBLK_t *a, int i);
extern DESCR_t (*g_user_call_hook)(const char *name, DESCR_t *args, int nargs);
int    subscript_set(DESCR_t arr, DESCR_t idx, DESCR_t val);
DESCR_t subscript_get2(DESCR_t arr, DESCR_t i, DESCR_t j);
DESCR_t subscript_get2_ext(DESCR_t arr, DESCR_t i, DESCR_t end);
int    subscript_set2(DESCR_t arr, DESCR_t i, DESCR_t j, DESCR_t val);
int    val_stack_depth(void);
void   register_fn(const char *name, DESCR_t (*fn)(DESCR_t*, int), int min_args, int max_args);
int64_t rt_time_ns(void);   /* NS-TIME (s249): THE clock behind TIME() -- CLOCK_MONOTONIC nanoseconds since program start */
DESCR_t EVAL_fn(DESCR_t expr);
DESCR_t compile_to_expression(const char *src);
DESCR_t EXPVAL_fn(DESCR_t expr_d);
DESCR_t CONVE_fn(DESCR_t str_d);
DESCR_t CODE_fn(DESCR_t str_d);
DESCR_t opsyn(DESCR_t newname, DESCR_t oldname, DESCR_t type);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline DESCR_t opsyn2(DESCR_t a, DESCR_t b) { return opsyn(a, b, NULVCL); }
DESCR_t sort_fn(DESCR_t arr);
DESCR_t rsort_fn(DESCR_t arr);
void    core_set_label_exists_hook(int (*fn)(const char *));
const char *setexit_label_get(void);
#define TABLE_VAL(tbl_) ((DESCR_t){ .v = DT_T, .tbl = (tbl_) })
#define ARRAY_VAL(a_)   ((DESCR_t){ .v = DT_A, .arr = (a_)   })
#endif
void indirect_goto(const char *varname);
extern int _x4_pending_parent_frame;
extern int _command_pending_parent_frame;
