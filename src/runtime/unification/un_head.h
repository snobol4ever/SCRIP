#ifndef RUNTIME_UNIFICATION_UN_HEAD_H
#define RUNTIME_UNIFICATION_UN_HEAD_H
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#include "runtime/prolog_db_blocks.h"
#include "runtime/core/g_runtime_entry.h"
#include "runtime/rt/rt_pl_trail.h"
#include "runtime/core/g_runtime_vec.h"
#include "runtime/core/g_runtime_abi.h"
#include "ir/ir_fwd.h"
#include "runtime/rt/pl_rational.h"
#define PL_CELL_ALLOC(n) (rt_ws_alloc_descr((size_t)(n) / sizeof(pl_cell_t)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct plc_vmap plc_vmap;
#define PLC_WQ_NOESC 2
#define PLC_WQ_NONASCII 4
#define PLC_WQ_XESC 8
struct plc_vmap {
    pl_cell_t *seen[1024];
    int n;
    FILE *fp;
    pl_cell_t *vnl;
    int vnc;
    int (*portray)(pl_cell_t *, plc_vmap *);
    pl_tr_ctx_t *cx;
    void *pthrown;
    long pheld;
    int space_args;
    int next_arg;
    pl_cell_t *pgoal;
    pl_cell_t *popts;
    int ibase;
    int inamed;
    int ipfx;
    int xstr;
    int fprec;
    char fspec;
    int xopf;
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PLC_WC_IGNORE_OPS 1
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { long k; pl_cell_t *p; } plc_wsref;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { void *blk; pl_cell_t *nb; int done; } plc_fz_ent_t;
typedef struct { plr_stk_t ag, lg; pl_cell_t *s; pl_cell_t *d; long next_k; int named; int dag; pl_cell_t knodes; plc_fz_ent_t *tab; long mask; } plc_fz_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern int rt_last_ok(void);
extern long rt_arith(int lk, long li, const char *ls, int rk, long ri, const char *rs, const char *op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { char *b; size_t n; size_t cap; size_t seg; size_t fpos[256]; int fchr[256]; int nf; pl_tr_ctx_t *cx; long cadj; long segadj; } plc_fb;
#define PLC_CMP_MAP 16384
typedef struct { pl_cell_t *a, *b; int i, n; } plc_cmp_fr_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_cell_t **a; pl_cell_t **b; int n; int cap; } pl_vmap_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_cell_t **pool; int *n; int *counts; } plc_tv_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_cell_t *head; int other; } plc_tv1_t;
typedef struct { pl_cell_t *head; long st; } plc_tv1a_t;
_Static_assert(PLR_LINK == (DTYPE_t)(DT_PLREF + 2) && PLR_SEEN == (DTYPE_t)(DT_PLREF + 3) && PLR_VCOPY == (DTYPE_t)(DT_PLREF + 5) && __builtin_offsetof(DESCR_t, v) == 0 &&
    __builtin_offsetof(DESCR_t, slen) == 4 && __builtin_offsetof(DESCR_t, p) == 8 && __builtin_offsetof(VCELL_t, cellp) == 0,
    "rt_pl_tv1_walk_a (rtx_plunify.s) is PLR_WALK1 over plc_tv1_visit in asm: it reads the tag byte at 0, the functor id at 4 and the pointer at 8, follows a DT_N name to VCELL_t.cellp at 0, marks P"
    "LR_SEEN and PLR_VCOPY at DT_PLREF + 3 and + 5, resolves PLR_LINK at + 2, keeps its agenda (128 entries) in its frame and answers 2 when it fills, so the C walk takes the term");
    _Static_assert(PLR_VCOPY != (DTYPE_t)DT_PLVAR,
    "TERM_VARIABLES IN ONE WALK (hq_runtime 2026-10-17, R-1682; hq_prolog's go): a variable is marked PLR_VCOPY the first time the walk meets it and its pointer word threads the variables found so"
    " far"
    ", so the walk needs no pool, no counting walk to size one and no linear search to de-duplicate (det.pl spent 1.8 G instructions in those); copy_term's PLR_VCOPY marks never meet these, since ea"
    "ch runtime " "call restores its marks before it returns, and an unbound DT_SNUL or DT_FAIL cell, which carries no self pointer to restore, sends the call back to the pool road");
typedef struct { pl_cell_t *rest; int mark; } pl_baggrp_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { long counter; int var_id; pl_tr_ctx_t *cx; } plc_nv_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_ACYC_ON_PATH ((DTYPE_t)(DT_PLREF + 1))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PLC_CC 16
typedef struct { pl_cell_t *aa; pl_cell_t *na; uint32_t sl; } plc_cc_t;
typedef struct { plr_stk_t ag, lg; pl_cell_t *s; pl_cell_t *d; int atts; pl_tr_ctx_t *cx; long nfr; plc_cc_t cc[PLC_CC]; } plc_cp_t;
#define PLC_SH_FRAME (1ull << 63)
#define PLC_SH_CHG (1ull << 62)
#define PLC_SH_ATT (1ull << 61)
#define PLC_SH_CNT 0xFFFFFFFFull
#define PLC_SH_BUDGET 1024
_Static_assert(PLC_SH_BUDGET > 0,
    "copy_term shares a subterm its copy would leave unchanged (SWI shares ground subterms), deciding at the subterm's end, so a compound still unchanged when its last argument comes "
    "up keeps its frame; past this many kept frames a compound is copied instead of shared, so a long ground spine costs the copier no more stack than the plain copy's own marks");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_root_omega, leaf, 0);
typedef struct dyn_clause { pl_cell_t *head; pl_cell_t *body; struct dyn_clause *next; } dyn_clause_t;
typedef struct { dyn_clause_t *next; } dyn_cursor_t;
typedef struct { dyn_clause_t *cur; int mark; } pl_dyn_it_t;
typedef struct { dyn_clause_t *cur; int mark; } pl_clause_it_t;
typedef struct { const char *name; long arity; } pl_pi_cand_t;
typedef struct { pl_pi_cand_t *v; int n; int i; int mark; } pl_curpred_it_t;
typedef struct { const char *name; int prec; const char *type; } pl_op_cand_t;
typedef struct { pl_op_cand_t *v; int n; int i; int mark; } pl_curop_it_t;
typedef struct { int i; int mark; } pl_flagit_t;
typedef struct { int si; int pi; int mark; } pl_spropit_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_DB_CELL0 24
#define PL_DB_FRAG_PAGES(f) (((f) >> 20) & 0x3FF)
#define PL_DB_FRAG_HELD 0x40000000
#define PL_DB_FRAG_GONE ((int)0x80000000u)
#define PL_DB_FRAG_REC(f) (((f) & 0x3FFFFFFF) != 0)
_Static_assert(__builtin_offsetof(pl_db_slot_t, frag) == 36,
    "frag at +36 is the C side's alone (no emitted code reads it): the fragment's first code-pool page in the low 20 bits, its page count in the next 10, HELD (a reclaim pass saw a holder) in bit 30"
    " and " "GONE (its pages reclaimed) in bit 31, 0 when unrecorded; a range of 1024 pages or more is never recorded, and 1024 pages is FLAT_BUF_MAX's floor, so a run-time compile nested inside the "
    "fragment's own emission (which leaves the emission's whole reservation below it) can never be taken for the fragment");
_Static_assert(sizeof(pl_db_slot_t) == 40 && __builtin_offsetof(pl_db_slot_t, erased) == 16 && __builtin_offsetof(pl_db_slot_t, ref) == 20 && __builtin_offsetof(pl_db_slot_t, ridx) == 24 &&
    __builtin_offsetof(pl_db_slot_t, chain_off) == 28 && __builtin_offsetof(pl_db_slot_t, next_idx) == 32,
    "xa_flat.cpp's chain-omega and bb_to.cpp's db walk bake the slot layout (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 A/B)");
_Static_assert(__builtin_offsetof(pl_db_t, s) == 0 && __builtin_offsetof(pl_db_t, n) == 8 && __builtin_offsetof(pl_db_t, next_ref) == 20 && __builtin_offsetof(pl_db_t, head_idx) == 24,
    "xa_flat.cpp's packet entry and bb_to.cpp's db walk bake the store layout (ARCH-PROLOG-C-OUT-OF-THE-BOX 5.2 A/B)");
_Static_assert(__builtin_offsetof(pl_db_t, ix) == 64, "rtx_gc.s's HB_PLDB visitor marks the index block through the word at +64 (ARCH-PROLOG-DATABASE section 7)");
_Static_assert(__builtin_offsetof(pl_db_t, src) == 96, "rtx_gc.s's HB_PLDB visitor marks the source record through the word at +96 (ARCH-PROLOG-DATABASE section 23)");
_Static_assert(__builtin_offsetof(pl_db_t, trim_wait) == 112 && __builtin_offsetof(pl_db_t, cell_k) == 32,
    "bb_to.cpp's db walk reads the store's trim_wait word at +112 and its cell number at +32 at its alpha");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_IX_ATOM 1
#define PL_IX_INT 2
#define PL_IX_FUNC 3
#define PL_IX_OTHER 4
#define PL_IX_MIN 8
typedef struct { int cap; int hcap; int hused[2]; int deep[2]; int np; int off2; } pl_ix_t;
typedef struct { int64_t key; int first; int last; int n; } pl_ix_ent_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_db_cand_first, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_db_cand_next, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_db_walk_closed, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_db_cand_last, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PLC_GVAR_ABSENT 0x47564152u
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PL_DB_REGISTRY_CELL 0
#define PL_DB_KEY_VLA(buf, nm, ar) char buf[fmt_len("%s/%d", (nm), (int)(ar))]; snprintf(buf, sizeof buf, "%s/%d", (nm), (int)(ar))
_Static_assert(__builtin_offsetof(pl_db_reg_t, hix) == 56, "rt_gc_mark_run (rtx_gc.s) visits the registry's key index at [registry + 56], an unscanned block of entry numbers");
_Static_assert(__builtin_offsetof(pl_db_reg_t, ovf) == 24,
    "xa_flat.cpp's packet road loads a root cell numbered PL_DB_FRAME_CELLS or more through [registry + 24], the overflow vector -- the cells past the root frame live there, in an HB_PVEC that grows"
    " by doubling");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
_Static_assert(__builtin_offsetof(pl_db_t, s) == 0 && __builtin_offsetof(pl_db_slot_t, cl) == 0 && sizeof(pl_db_slot_t) == 40 && __builtin_offsetof(pl_db_reg_t, e) == 0 &&
    __builtin_offsetof(pl_db_reg_t, ovf) == 24 && __builtin_offsetof(pl_db_reg_t, tries) == 32 && __builtin_offsetof(pl_db_key_t, key) == 0 && __builtin_offsetof(pl_db_key_t, db) == 16 &&
    sizeof(pl_db_key_t) == 48 && __builtin_offsetof(pl_db_reg_t, hcap) == 64,
    "rt_gc_mark_run and rt_pl_db_reg_find_na (rtx_gc.s, rtx_plunify.s) read the Prolog db store, slots, registry and keys at these offsets");
typedef struct { int atom; int pad; int64_t size; int64_t limit; } pl_db_mod_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { plr_stk_t ag, lg; pl_cell_t *s; pl_trie_node_t *cur; int create; int nvar; int miss; int attv; } pl_trie_w_t;
#define PL_TRIE_LINEAR_KIDS 8
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_mkground, leaf, 0);
RT_ENTRY_DECL(rt_pl_db_decls_install, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_pl_db_slot_ensure, leaf, 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { pl_db_t *db; unsigned char *mk; long frames; } pl_db_rr_t;
typedef struct { uint64_t not_mark; int live; } pl_db_tw_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { const char *lo; int slot; } pl_db_rcix_t;
typedef struct { pl_db_t *db; const char *lo; const char *hi; int min_g; int c_lo; int c_hi; const pl_db_rcix_t *ix; int nix; } pl_db_rc_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif
