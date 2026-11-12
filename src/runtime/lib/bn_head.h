#ifndef RUNTIME_LIB_BN_HEAD_H
#define RUNTIME_LIB_BN_HEAD_H
#include "runtime/core/core.h"
#include <glob.h>
#include "runtime/core/g_runtime.h"
#include "runtime/rt/rtx_try.h"
#include "runtime/core/array.h"
#include "runtime/core/datatype.h"
#include "runtime/core/table.h"
#define ICN_ARITY_UNKNOWN (-99)
#define CALL_ARGS ((DESCR_t *)g_call_args.p)
#include "runtime/builtins/gen_runtime.h"
#include "ir/ct_vec.h"
#include "ir/ir_fwd.h"
#include "ir/bb_box.h"
#include "parsers/snobol4/scrip_cc.h"
#include "ir/SM.h"
#include "runtime/keywords.h"
#include "runtime/core/g_runtime_entry.h"
#include "runtime/core/g_runtime_vec.h"
#include "runtime/core/g_runtime_abi.h"
#include "runtime/rt/gc_heap.h"
#include "emitter/sil_macros.h"
#include "runtime/rt/rt.h"
#include "ir/gc_frame_map.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *nm; int np; } icn_bi_ent_t;
#include "parsers/prolog/pl_cell.h"
RT_ENTRY_DECL(rt_pl_fresh_var_ref, leaf, 0);
#define RK_RX '\x06'
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct rk_cbh { struct rk_cbh *prev; DESCR_t d[2]; } rk_cbh_t;
typedef struct { const char *name; int nref; } rk_cb_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *qname; const char *body; int flavor; } gram_ent_t;
#define GRAM(k) CV_AT(g_runtime.bn.gram, gram_ent_t, (k))
#define gram_n ((int)g_runtime.bn.gram.len)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rk_is_truthy, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RK_EV(m, k) (((int *) (m)->blk) + 4 * (m)->ngroups + 3 * (m)->ncaplog + 10 * (k))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { char kind; int idx; const char *nm; int nl; int rep; int start, end; } RkGrp;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { int n; const DESCR_t *el; } rk_av_t;
#define RK_SEQ_LAZY 32
#define RK_ARR_BOUND 16
#define RK_ARR_ITEM 64
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { int fl, err, mode, n, cap, nv, vi; DESCR_t *buf, *vals; } rk_hy_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { long mode, posv, xmin, xmax; const long *nth; int nnth; } RkRxOpt;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { char *b; size_t n, cap; } rk_sb_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PTF_BUF(k) (((DESCR_t *)g_runtime.pas.pas_tf.p)[2 * (k)])
#define PTF_ST(k) (((DESCR_t *)g_runtime.pas.pas_tf.p)[2 * (k) + 1])
#define PTF_HAS(k) ((int)(PTF_ST(k).i & 1))
#define PTF_MODE(k) ((int)(PTF_ST(k).i >> 1))
#define PTF_N ((int)(g_runtime.pas.pas_tf.len / 2))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PAS_VIEW_STR 64
#define PAS_VIEW_RAW 128
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct rk_redisp_t { DESCR_t self; DESCR_t mname; struct rk_redisp_t *prev; const char *cname; int found_idx; int nargs; const DESCR_t *args; int skip_mname; int local; } rk_redisp_t;
_Static_assert(sizeof(rk_redisp_t) == RK_REDISP_REC_BYTES,
    "the redispatch record is RK_REDISP_REC_BYTES of the meth_call node's frame grant (frame_layout.c grants RK_REDISP_REC_SLOTS slots after the argv)");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rk_method_open, open, 1);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_apply_open, open, 1);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_apply_land_γ, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_apply_land_ω, leaf, 0);
_Static_assert((RT_TWEAK_IC_N & (RT_TWEAK_IC_N - 1)) == 0,
    "THE CONSTRUCTOR HOOK CACHE (2026-09-25, R-1258): a type whose method-resolution chain carries no <type>__TWEAK procedure is remembered by its name pointer under a generation that moves wh"
    "enever a procedure or a type is added or the procedure table is reset, so the check that cost every record constructor a formatted name, a proc-table scan and a hash lookup runs once per type p"
    "er generation");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_tweak_open, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { char opsym[6]; signed char ar; } icn_opproc_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { uint64_t magic; const char *nm; int64_t idx; int kind; int64_t cur, lim, step; } ICN_OPGEN_t;
#define ICN_OPGEN_MAGIC 0x1CBA46E4E52DULL
#define ICN_OPGEN_BANG 0
#define ICN_OPGEN_TOBY 1
#define ICN_OPGEN_KEY 2
#define ICN_OPGEN_SCAN 3
#define ICN_OPGEN_CURSOR 4
#define ICN_OPGEN_FNAMES 5
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_call_value_gen_h, gen, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_call_apply_gen_h, gen, 0);
RT_ENTRY_DECL(rt_call_value_spine_prep, open, 1);
RT_ENTRY_DECL(rt_call_value_spine_prep_blk, open, 1);
RT_ENTRY_DECL(rt_call_apply_spine_prep, open, 1);
RT_ENTRY_DECL(rt_call_apply_spine_prep_blk, open, 1);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_call_value_resume_h, gen, 0);
typedef struct { char *buf; size_t sz; int saved_out; int idx; } PlWotFrame;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { DESCR_t root; int proven; } pl_ax_cx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef int (*dop_body_fn)(DESCR_t *, int, DESCR_t *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_unify_const_cold, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_tr_ctx_t *cx; const char *floor_; } plu_ctx_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(dop_write, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(dop_nl, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_format_cell(const char *, void *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_TYPE_LEAF(nm) DESCR_t dop_pl_##nm(DESCR_t *args, int nargs) { pl_atoms_ready(); return (nargs == 1 && rt_pl_type_test_cell(&args[0], #nm)) ? pl_ok() : FAILDESCR; }
RT_ENTRY_DECL(dop_pl_ground, leaf, 0);
RT_ENTRY_DECL(dop_pl_is_list, leaf, 0);
RT_ENTRY_DECL(dop_pl_between_special, leaf, 0);
RT_ENTRY_DECL(dop_pl_acyclic_term, leaf, 0);
RT_ENTRY_DECL(dop_pl_writeq, leaf, 0);
RT_ENTRY_DECL(dop_pl_write_canonical, leaf, 0);
RT_ENTRY_DECL(dop_pl_writeln, leaf, 0);
RT_ENTRY_DECL(dop_pl_tab, leaf, 0);
RT_ENTRY_DECL(dop_pl_put_char, leaf, 0);
RT_ENTRY_DECL(dop_pl_put_code, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(dop_pl_flush_output, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_OUT_LEAF(nm, body1) DESCR_t dop_pl_##nm##_s(DESCR_t *args, int nargs) { extern int fh_current_output(void); extern void fh_set_output(int); int idx = pl_stream_idx(args[0], 1); if (nargs \
    < 1 || idx < 0) return FAILDESCR; { int sv = fh_current_output(); fh_set_output(idx); DESCR_t r = body1(args + 1, nargs - 1); fh_set_output(sv); return r; } }
RT_ENTRY_DECL(dop_pl_set_input, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_SP_NPROP 15
#define PL_CX_LEAF_HEAD(nm, ar) DESCR_t rt_pl_dop_##nm##_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {int ok; if (nargs != ar) return FAILDESCR;pl_atoms_ready();rt_pl_tr_gc_sync(cx->tr);
#define PL_CX_LEAF_TAIL rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define pl_eng_unify_int(a, v, cx) plw_unify_vals((a), INTVAL((int64_t)(v)), (cx))
typedef struct { const char *s; size_t blen; long n; int mode; long k0, k1; const char *sub; size_t sublen; long subn; long count; } pl_sa_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_dop_sub_atom_n, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_dop_atom_concat_n, leaf, 0);
RT_ENTRY_DECL(rt_pl_dop_big, leaf, 0);
RT_ENTRY_DECL(rt_pl_dop_findall_new, leaf, 0);
RT_ENTRY_DECL(rt_pl_dop_findall_add, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_dop_bagof_group_n, leaf, 0);
#define PL_ATOM_OP_LEAF(nm, ar) PL_CX_LEAF_HEAD(nm, ar) ok = rt_pl_atom_op_cell(#nm, &args[0], ar > 1 ? (void *)&args[1] : (void *)0, ar > 2 ? (void *)&args[2] : (void *)0, cx); PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(dop_pl_at_end_of_stream, leaf, 0);
typedef struct {
    const char **nm;
    DESCR_t *v;
    int *cnt;
    int *hs;
    int n;
    int cap;
    unsigned hmask;
    const char *err;
    const tree_t *tr;
    const char *src;
    int shift;
    int base;
    const int *cm;
    int ncm;
} pl_vtab_t;
#define PL_SYNERR(vt) ((vt).err ? (vt).err : "cannot_start_term")
#define PL_READ_TERM_LEAF(nm, at) PL_CX_LEAF_HEAD(nm, 3) { ok = pl_read_term_from_text(args, cx, at); } PL_CX_LEAF_TAIL
#define PL_TEXTOP_read_term_opts 1
#define PL_TEXTOP_read 1
#define PL_TEXTOP_get_char 1
#define PL_TEXTOP_peek_char 1
#define PL_TEXTOP_get_code 1
#define PL_TEXTOP_peek_code 1
#define PL_TEXTOP_unget_char 1
#define PL_TEXTOP_unget_code 1
#define PL_TEXTOP_get_byte (-1)
#define PL_TEXTOP_peek_byte (-1)
#define PL_TEXTOP_unget_byte (-1)
#define PL_IN_LEAF(nm, ar) PL_CX_LEAF_HEAD(nm##_s, ar) { extern int fh_current_input(void); extern void fh_set_input(int); int idx = pl_stream_idx_ball(args[0], 0, PL_TEXTOP_##nm, cx); ok = 0; if ( \
    idx >= 0) { int sv = fh_current_input(); fh_set_input(idx); DESCR_t r = rt_pl_dop_##nm##_c(args + 1, ar - 1, cx); fh_set_input(sv); ok = (r.v == (DTYPE_t)DT_I); cx->ball = pl_ball_sora(cx->ball, \
    args[0]); } } PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_OUT_CX_LEAF(nm, ar) PL_CX_LEAF_HEAD(nm##_s, ar) { extern int fh_current_output(void); extern void fh_set_output(int); int idx = pl_stream_idx_ball(args[0], 1, PL_TEXTOP_##nm, cx); ok = 0; \
    if (idx >= 0) { int sv = fh_current_output(); fh_set_output(idx); DESCR_t r = rt_pl_dop_##nm##_c(args + 1, ar - 1, cx); fh_set_output(sv); ok = pl_out_ioerr(idx, args[0], cx) && r.v == (DTYPE_t) \
    DT_I; } } PL_CX_LEAF_TAIL
#define PL_OUT_BALL_LEAF(nm, body1, ar) PL_CX_LEAF_HEAD(nm##_sb, ar) { extern int fh_current_output(void); extern void fh_set_output(int); int idx = pl_stream_idx_ball(args[0], 1, 1, cx); ok = 0; if \
    (idx >= 0) { int sv = fh_current_output(); fh_set_output(idx); DESCR_t r = body1(args + 1, ar - 1); fh_set_output(sv); ok = pl_out_ioerr(idx, args[0], cx) && r.v == (DTYPE_t)DT_I; } } \
    PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_TEXTOP_put_byte (-1)
#define PL_TEXTOP_put_char_c 1
#define PL_TEXTOP_write_term 1
#define PL_TEXTOP_print 1
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_TEXTOP_display 1
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum { PLF_NONE = 0, PLF_BOOL, PLF_ATOM, PLF_INT, PLF_FLOAT, PLF_TERM, PLF_GONE };
typedef struct { const char *nm; const char *val; int mod; const char *allow[6]; int is_term; DESCR_t term; int user; int type; } pl_flag_t;
#define pl_flags ((pl_flag_t *)g_runtime.bn.g_pl_flags.p)
typedef struct { const char *dia; const char *nm; const char *val; } pl_dia_flag_t;
RT_ENTRY_DECL(rt_sno_patck_d, leaf, 0);
_Static_assert(__builtin_offsetof(pl_flag_t, nm) == 0 && __builtin_offsetof(pl_flag_t, val) == 8 && __builtin_offsetof(pl_flag_t, is_term) == 72 && __builtin_offsetof(pl_flag_t, term) == 80 &&
    sizeof(pl_flag_t) == 104, "rt_gc_flags_roots_run (rtx_gc.s) walks the Prolog flags at these offsets, 104 bytes apart");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(dop_pl_told, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(dop_pl_seen, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_GNU_SORT1_LEAF(nm, mode) PL_CX_LEAF_HEAD(nm, 1) { extern int rt_pl_sort_in_place_cell(int, void *); void *b = pl_sort_list_ball(rt_pl_deref_val(args[0]), (mode) == 2); if (b) { cx->ball = \
    b; ok = 0; } else ok = rt_pl_sort_in_place_cell((mode), &args[0]); } PL_CX_LEAF_TAIL
typedef struct { const char **nm; int n, cap; } pl_bt_t;
#define GS_LA 1
#define GS_SC 2
#define GS_QT 4
#define GS_DQ 8
#define GS_BQ 16
#define GS_GR 32
#define GS_PC 64
#define GS_DI 128
#define GS_UL 256
#define GS_CL 512
#define GS_SL 1024
#define GS_CM 2048
#define GS_EX 4096
enum { GT_NAME, GT_VAR, GT_INT, GT_FLOAT, GT_STR, GT_BQ, GT_PUNCT, GT_IOPEN, GT_STOP, GT_EOF, GT_EXT };
typedef struct { FILE *in; int idx; int c; int ty; int tk; int punct; int q; int oom; int eofn; long tl; long tc; const char *err; char *b; size_t bn; size_t bk; DESCR_t num; } pl_gs_t;
#define GP_GENERAL 0
#define GP_PREFIX 1
#define GP_INSIDE 2
typedef struct { pl_gs_t s; int have; int utk; int upunct; int uq; long utl; long utc; char *ub; size_t ubn; size_t ubk; DESCR_t unum; } pl_gp_t;
#define PL_HASH_ON_PATH ((DTYPE_t)(DT_PLREF + 1))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PAS_SET_BYTES 32
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct rk_dirstream RK_DIR;
struct rk_dirent { unsigned long d_ino; long d_off; unsigned short d_reclen; unsigned char d_type; char d_name[]; };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { int dh, dv, eh, ev, kvh, kvv, ph, pv, kh, kv2, vh, vv, nmode; } rk_adv_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(offsetof(ARBLK_t, lo) == 0 && offsetof(ARBLK_t, hi) == 4 && offsetof(ARBLK_t, data) == 32 && offsetof(ARBLK_t, ewidth) == 56 && sizeof(DESCR_t) == 16,
    "bb_call_pas_elem.cpp reads the block bounds, the data vector and the element width at these offsets");
RT_ENTRY_DECL(pas_arr_make, leaf, 0);
RT_ENTRY_DECL(pas_arr_make_raw, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(pas_arr_get, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(pas_arr_set, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *name; int nlen; int req; } rk_mc_named_t;
RT_ENTRY_DECL(c_rt_rk_arr_set, leaf, 0);
RT_ENTRY_DECL(c_rt_rk_arr_at, leaf, 0);
RT_ENTRY_DECL(rt_rk_undef, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(pas_slice, leaf, 0);
typedef struct pas_wsink_t { FILE *fp; char *b; size_t n; size_t cap; } pas_wsink_t;
RT_ENTRY_DECL(rt_call_pas_bi, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_call_arr, leaf, 0);
RT_ENTRY_DECL(rt_call_arr_bl, leaf, 0);
static inline unsigned rt_ctor_ic_h(const char *fn) __attribute__((always_inline));
static inline int rt_ctor_ic_find(const char *fn, long gen) __attribute__((always_inline));
_Static_assert((RT_CTOR_IC_N & (RT_CTOR_IC_N - 1)) == 0,
    "THE RECORD-CONSTRUCTOR INLINE CACHE (2026-09-25, R-1258): a call site whose name the full by-name path resolved to a DATA type and constructed positionally is remembered by its na"
    "me pointer under a generation that moves whenever a procedure, a type, an EVAL'd procedure or the dtax cache changes, so the next call at that site constructs directly -- the same dat_c"
    "onstruct, the same TWEAK hook, without the 5,000 instructions of checks that precede the positional arm");
RT_ENTRY_DECL(rt_call_arr_bl_strict, leaf, 0);
_Static_assert((RT_FIELD_IC_N & (RT_FIELD_IC_N - 1)) == 0,
    "THE FIELD-ACCESSOR INLINE CACHE (2026-09-25, R-1258): a call site's field name pointer and the instance's type pointer key a direct-mapped slot holding the field index; a hit is c"
    "onfirmed by strcmp against the type's own field name, so a reused string address can never answer with another field's index, and the value is the instance's own cell, exactly what dat_"
    "field_get returns");
static inline int rt_field_index_cached_i(const char *fn, const DATBLK_t *idb) __attribute__((always_inline));
RT_ENTRY_DECL(rt_call_fld_sn4, leaf, 0);
RT_ENTRY_DECL(rt_call_arr_bl_try, open, 1);
RT_ENTRY_DECL(rt_call_arr_bl_sn4_try, open, 1);
RT_ENTRY_DECL(rt_call_callee_try_sn4, open, 1);
_Static_assert(G_ERROR_EVAL_STAGE == (-9223372036854775807L - 1), "rtx_misc.s rt_call_bid_sn4 passes g_error by doubling it: only 0 and LONG_MIN double to zero");
RT_ENTRY_DECL(c_rt_call_bid_sn4, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_call_arr_gen, gen, 0);
RT_ENTRY_DECL(rt_make_list, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_main_args_stage, leaf, 0);
RT_ENTRY_DECL(rt_main_args_stage_argv, leaf, 0);
RT_ENTRY_DECL(rt_main_progname_stage, leaf, 0);
RT_ENTRY_DECL(rt_main_args_bind, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(c_rt_str_coerce, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(c_rt_jct_relop, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { int from; const char *label; DESCR_t m; } rk_cap_t;
#define _OPCOERCE(d) do { if (!IS_INT_fn(d) && !IS_REAL_fn(d)) { const char *_s = IS_STR(d) ? rt_cstr_d(d) : VARVAL_fn(d); if (_s && *_s) { char *_e=NULL; long long _iv=strtoll(_s,&_e,10); if (_e && \
    !*_e){(d)=INTVAL(_iv);} else {double _rv=strtod(_s,&_e); if(_e && !*_e){(d)=REALVAL(_rv);}else{*out=FAILDESCR;return 1;}} } else { *out=FAILDESCR; return 1; } } } while(0)
#define _NUMREL(op) do { DESCR_t _l=args[0],_r=args[1]; _OPCOERCE(_l); _OPCOERCE(_r); double _lv2=IS_REAL_fn(_l)?_l.r:(double)_l.i, _rv2=IS_REAL_fn(_r)?_r.r:(double)_r.i; *out=(_lv2 op _rv2)?(( \
    IS_REAL_fn(_l)&&!IS_REAL_fn(_r))?REALVAL(_rv2):_r):FAILDESCR; return 1; } while(0)
#define _STRREL(op) do { DESCR_t _l=args[0],_r=args[1]; const char *_ls=VARVAL_fn(_l); if(!_ls)_ls=""; const char *_rs=VARVAL_fn(_r); if(!_rs)_rs=""; int _cmp=strcmp(_ls,_rs); *out=(_cmp op 0)? \
    rtx_try_rt_str_coerce(_r):FAILDESCR; return 1; } while(0)
#define _SNOCOERCE(d) do { if (!IS_INT_fn(d) && !IS_REAL_fn(d)) { const char *_s9 = IS_STR(d) ? rt_cstr_d(d) : VARVAL_fn(d); if (!_s9 || !*_s9) { (d) = INTVAL(0); } else { _OPCOERCE(d); } } } while(0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *s; long n; int pos; uint64_t *env; } bn_win_t;
#define BN_PTRLEN(A, P, L, F) do { DESCR_t _a = (A); if ((F) && _a.v == DT_S && _a.s && _a.slen != 0xFFFFFFFFu && _a.slen != 0u) { (P) = _a.s; (L) = (size_t)_a.slen; } else { (P) = VARVAL_fn(_a); if \
    (!(P)) (P) = ""; (L) = sv_len(_a, (P)); } } while (0)
typedef g_dtax_ent_t dtax_ent_t;
typedef struct { int (*fn)(DESCR_t *, int, DESCR_t *, int); int op; int _pad; } bn_direct_t;
_Static_assert(sizeof(bn_direct_t) == 16, "rtx_misc.s indexes g_bn_direct with a 16-byte stride: fn at +0, op at +8 (2026-09-25, R-1255)");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RT_HOOK_GUARD(nm) do { if (g_runtime.abi.error != 0 || g_runtime.abi.kw_errlimit != 0) return c_rt_call_bid_sn4((nm), args, nargs, -1); } while (0)
RT_ENTRY_DECL(c_rt_sno_name_dl, leaf, 0);
RT_ENTRY_DECL(rt_sno_mkexpr_d, leaf, 0);
RT_ENTRY_DECL(rt_sno_xlv_d, leaf, 0);
typedef struct { const char *newname; const char *oldname; } rt_syn_t;
#define SYN(k) CV_AT(g_runtime.bn.rt_syn, rt_syn_t, (k))
#define g_rt_syn_n ((int)g_runtime.bn.rt_syn.len)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct pl_psched_s pl_psched_t;
typedef rt_pseg_t pl_pseg_t;
struct pl_psched_s { uint64_t entry_rsp, ret_pc, rbp, rbx, r12, r13, r14, r15; uint64_t planes[4]; int64_t k_level; int64_t leader; };
typedef struct { void *blk; void *sched; } pl_pseg_pair_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int rt_pl_db_bind(void *, int64_t, const char *, int64_t);
typedef struct { const char *nm; int ar; int tok; } pl_meta_row_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { void **addr; char **nm; int n; int cap; int *hs; int hmask; } pl_ctv_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rk_box_step, leaf, 0);
RT_ENTRY_DECL(rk_box_back, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rk_box_match_build, leaf, 0);
__attribute__((visibility("hidden"))) DESCR_t *plw_cell_deref_slow(DESCR_t *c);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) size_t sv_len(DESCR_t arg, const char *coerced) {
    if (arg.v == DT_S && arg.slen != 0xFFFFFFFFu) return arg.slen ? (size_t)arg.slen : (coerced ? strlen(coerced) : 0);
    return coerced ? strlen(coerced) : 0;
}
static inline __attribute__((always_inline)) int plw_n_self(const DESCR_t *c) { return c->v == DT_N && c->slen == 1 && c->p == (void *)c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) int plw_unbound_tag(const DESCR_t *c) { return c->v == DT_SNUL || c->v == DT_FAIL || (c->v == (DTYPE_t)DT_PLVAR && c->p == (void *)c) || plw_n_self(c); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) DESCR_t *plw_cell_deref(DESCR_t *c) {
    if (c->v != DT_N && (c->v != (DTYPE_t)DT_PLVAR || !c->p || c->p == (void *)c)) return c;
    return plw_cell_deref_slow(c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline __attribute__((always_inline)) DESCR_t *plw_entry(DESCR_t *tmp) {
    if (tmp->v == DT_N && tmp->slen == 1 && tmp->p) return (DESCR_t *)tmp->p;
    if (tmp->v == DT_N && tmp->slen == 2 && tmp->p && ((VCELL_t *)tmp->p)->cellp) return ((VCELL_t *)tmp->p)->cellp;
    return tmp;
}
static inline unsigned rt_ctor_ic_h(const char *fn) { return (unsigned)((((uintptr_t)fn) * 0x9E3779B97F4A7C15ull) >> 56) & (RT_CTOR_IC_N - 1); }
static inline int rt_ctor_ic_find(const char *fn, long gen) {
    unsigned h = rt_ctor_ic_h(fn);
    if (g_runtime.bn.ctor_ic[h].fn == fn && g_runtime.bn.ctor_ic[h].gen == gen) return (int)h;
    h ^= 1u;
    if (g_runtime.bn.ctor_ic[h].fn == fn && g_runtime.bn.ctor_ic[h].gen == gen) return (int)h;
    return -1;
}
static inline int rt_field_index_cached_i(const char *fn, const DATBLK_t *idb) {
    unsigned h = (unsigned)(((((uintptr_t)fn ^ ((uintptr_t)idb << 1)) * 0x9E3779B97F4A7C15ull) >> 56) & (RT_FIELD_IC_N - 1));
    if (g_runtime.bn.field_ic[h].fn == fn && g_runtime.bn.field_ic[h].type == idb) {
        int fi = g_runtime.bn.field_ic[h].idx;
        if (fi < idb->nfields && idb->fields[fi] && !strcmp(idb->fields[fi], fn)) return fi;
    }
    {
        const char _f0 = fn[0];
        for (int fi = 0; fi < idb->nfields; fi++) if (idb->fields[fi] && idb->fields[fi][0] == _f0 && !strcmp(idb->fields[fi], fn)) {
            g_runtime.bn.field_ic[h].fn = fn;
            g_runtime.bn.field_ic[h].type = idb;
            g_runtime.bn.field_ic[h].idx = fi;
            return fi;
        }
    }
    return -1;
}
#endif
