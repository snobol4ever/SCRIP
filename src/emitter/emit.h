#pragma once
#define REX_W            0x48
#define REX_WR           0x4C
#define REX_B            0x41
#define REX_WB           0x49
#define MOV_EAX_IMM32    0xB8
#define MOV_ECX_IMM32    0xB9
#define MOV_EDX_IMM32    0xBA
#define MOV_ESI_IMM32    0xBE
#define MOV_EDI_IMM32    0xBF
#define MOV_RM_R         0x89
#define MOV_R_RM         0x8B
#define MODRM_ECX_EAX    0xC1
#define MODRM_EAX_ECX    0xC8
#define MODRM_RDI_RAX    0xC7
#define MODRM_RBP_RSP    0xE5
#define MODRM_RSP_RBP    0xEC
#define MODRM_R10_INDIR  0x02
#define MODRM_RCX_INDIR  0x01
#define MODRM_R10D_EAX   0x02
#define LEA              0x8D
#define MODRM_RAX_RAXRCX 0x04
#define SIB_RAX_RCX      0x08
#define ESC              0x0F
#define MOVZX_R_RM8      0xB6
#define MODRM_EAX_EDI7   0x47
#define MOVSXD_R_RM      0x63
#define MODRM_RCX_R10    0x0A
#define CMP_RM_IMM8      0x83
#define CMP_RM_IMM32     0x81
#define MODRM_CMP_ESI    0xFE
#define CMP_EAX_IMM32    0x3D
#define CMP_AL_IMM8      0x3C
#define CMP_R_RM         0x3B
#define CMP_RM_R         0x39
#define MODRM_CMP_RSP    0xC4
#define ADD_EAX_IMM32    0x05
#define SUB_EAX_IMM32    0x2D
#define XOR_RM_R         0x31
#define MODRM_EAX_EAX    0xC0
#define TEST_RM_R        0x85
#define MODRM_RAX_RAX    0xC0
#define INC_CALL_FF      0xFF
#define MODRM_CALL_RAX   0xD0
#define MODRM_INC_R13D   0x45
#define REX_B_PUSH_R12   0x54
#define REX_B_POP_R12    0x5C
#define REX_B_PUSH_R10   0x52
#define REX_B_POP_R10    0x5A
#define PUSH_RBP         0x55
#define POP_RBP          0x5D
#define JMP_REL8         0xEB
#define JMP_REL32        0xE9
#define JL_REL8          0x7C
#define JGE_REL8         0x7D
#define JE_REL8          0x74
#define JNE_REL8         0x75
#define JNE_REL32_X      0x85
#define JE_REL32_X       0x84
#define JL_REL32_X       0x8C
#define JGE_REL32_X      0x8D
#define JG_REL32_X       0x8F
#define RET              0xC3
#define NOP              0x90
#ifdef __cplusplus
extern "C" {
#endif
#define TEXT_MODE_INVOCATION  0
#define TEXT_MODE_DEFINITION  1
#include "rt/rt_arena.h"
#include "bb_pool.h"
#include "IR.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
typedef enum {
    EMIT_TEXT             = 0,
    EMIT_BINARY_WIRED     = 1,
    EMIT_MACRO_DEF        = 3,
    EMIT_TEXT_INLINE      = 4,
    EMIT_JVM              = 5,
    EMIT_JS               = 6,
    EMIT_NET              = 7,
    EMIT_WASM             = 8
} bb_emit_mode_t;
#define EMIT_BINARY     EMIT_BINARY_WIRED
typedef enum {
    BB_PLATFORM_X86  = 0,
    BB_PLATFORM_JVM  = 1,
    BB_PLATFORM_NET  = 2,
    BB_PLATFORM_JS   = 3,
    BB_PLATFORM_WASM = 4
} bb_platform_t;
typedef enum {
    BB_MEDIUM_TEXT      = 0,
    BB_MEDIUM_BINARY    = 1,
    BB_MEDIUM_MACRO_DEF = 2
} bb_medium_t;
extern bb_platform_t   g_platform;
extern bb_medium_t     g_medium;
extern int             g_use_sm_macros;
extern int             g_use_bb_macros;
#define PLATFORM_X86   (g_platform == BB_PLATFORM_X86)
#define PLATFORM_JVM   (g_platform == BB_PLATFORM_JVM)
#define PLATFORM_NET   (g_platform == BB_PLATFORM_NET)
#define PLATFORM_JS    (g_platform == BB_PLATFORM_JS)
#define PLATFORM_WASM  (g_platform == BB_PLATFORM_WASM)
#define MEDIUM_TEXT      (g_medium == BB_MEDIUM_TEXT)
#define MEDIUM_BINARY    (g_medium == BB_MEDIUM_BINARY)
#define MEDIUM_MACRO_DEF (g_medium == BB_MEDIUM_MACRO_DEF)
#define USE_SM_MACROS  (g_use_sm_macros)
#define USE_BB_MACROS  (g_use_bb_macros)
#define BB_LABEL_NAME_MAX   80
#define BB_LABEL_UNRESOLVED (-1)
#define EMIT_UNRESOLVED     BB_LABEL_UNRESOLVED
#define EMIT_LABEL_MAX      BB_LABEL_NAME_MAX
typedef struct bb_label_t { char name[BB_LABEL_NAME_MAX]; int offset; } bb_label_t;
#define bb_label_defined(lbl)  ((lbl)->offset != BB_LABEL_UNRESOLVED)
#define emit_label_ok(l)       bb_label_defined(l)
typedef enum { JMP_JMP = 0, JMP_JE, JMP_JNE, JMP_JL, JMP_JGE, JMP_JG } jmp_kind_t;
#define BB_PATCH_MAX   65536
#define EMIT_PATCH_MAX BB_PATCH_MAX
typedef enum { PATCH_REL8, PATCH_REL32 } bb_patch_kind_t;
typedef struct { int site; bb_label_t * label; bb_patch_kind_t kind; } bb_patch_t;
typedef int emitter_t;
extern bb_emit_mode_t  bb_emit_mode;
extern int             g_sm_native_unsupported;
extern FILE          * bb_emit_out;
extern bb_buf_t        bb_emit_buf;
extern int             bb_emit_pos;
extern int             bb_emit_size;
extern bb_patch_t      bb_patch_list[BB_PATCH_MAX];
extern int             bb_patch_count;
extern int             g_is_text;
extern int             g_emit_text_mode;
void     bb_emit_begin      (bb_buf_t buf, int size);
int      bb_emit_end        (void);
void     bb_emit_patch_rel32(bb_label_t * lbl);
void     bb_emit_byte       (uint8_t b);
void     bb_emit_u32        (uint32_t v);
void     bb_emit_u64        (uint64_t v);
void     bb_emit_i32        (int32_t v);
FILE *   emit_outf             (void);
void     fmt_body_append       (const char * instr, const char * operands);
void     emit_label_initf      (bb_label_t * lbl, const char * fmt, ...);
void     bb_label_define       (bb_label_t * lbl);
bb_label_t * emit_label_alloc  (const char * fmt, ...);
bb_label_t * emit_label_intern (const char * name);
void         emit_label_pool_reset(void);
void         bb_label_alias      (bb_label_t * stub, bb_label_t * tgt);
bb_label_t * bb_label_fold       (bb_label_t * l);
void         bb_label_alias_reset(void);
void     emit_label_define_bb  (bb_label_t * lbl);
void     emit_jmp_label        (bb_label_t * target, jmp_kind_t kind);
struct IR_t;
int walk_bb_node(struct IR_t * nd, FILE * out);
void jvm_push_int2(FILE * out, long v);
void jvm_emit_ldc_string(FILE * out, const char * s);
void js_escape_string(FILE * out, const char * s);
#include "XA.h"
struct tree_t;
void strtab_label(char *buf, size_t bufsz, const char *s);
void xa_emit_strtab_rodata(void);
void strtab_reset(void);
int  strtab_intern(const char *s);
const void *csettab_label(char *buf, size_t bufsz, const char *cset);
void xa_emit_csettab_rodata(void);
void csettab_reset(void);
void xa_dispatch(XA_op_t op);
#ifdef __cplusplus
}
#endif
#include "IR.h"
typedef struct {
    DESCR_t value; int64_t counter; int state;
    void   *resolve_cs;
    int     ch_cur;
    int     ch_mark;
    void   *ch_saved_env;
    void   *ch_last_body;
    void   *ch_last_act;
    void   *ch_cp;
    void   *ch_cut_barrier;
    void  **ch_body_snaps;
    int     ch_nbodies;
} bb_node_state_t;
typedef struct { IR_t * root; int succ_idx; int fail_idx; int is_terminal; } stmt_t;
typedef struct { stmt_t * stmts; int n; int entry_idx; } prog_t;
typedef struct { IR_t ** kids; int nkids; IR_graph_t * inner; int * pos_stack; int cap; int saved_delta; } bb_arbno_state_t;
typedef struct { IR_t ** goals; int ngoals; } bb_conj_state_t;
typedef struct { IR_t * cond; IR_t * then_; IR_t * else_; IR_t * then_root; IR_t * else_root; IR_t * cond_root; void * cp_mark; int committed; long seen_seq; } bb_ite_state_t;
typedef struct { IR_graph_t * goal_g; IR_t * catcher; IR_graph_t * rec_g; } bb_catch_state_t;
typedef struct { IR_graph_t ** bodies; int nbodies; int cur; int mark; void * saved_env;
                 IR_graph_t * last_body; void * last_act;
                 void * cp;
                 void * cut_barrier;
                 long * idx_key;
                 int idx_ok;
               } bb_choice_state_t;
#define RESOLVE_IDX_VAR    0L
#define RESOLVE_IDX_NOKEY  (-1L)
#define RESOLVE_IDX_CLS_ATOM (1L << 60)
#define RESOLVE_IDX_CLS_INT  (2L << 60)
#define RESOLVE_IDX_CLS_FLT  (3L << 60)
#define RESOLVE_IDX_CLS_CMP  (4L << 60)
#define RESOLVE_IDX_PAYLOAD_MASK ((1L << 60) - 1L)
#define RESOLVE_IDX_ATOM(id)        (RESOLVE_IDX_CLS_ATOM | ((long)(id) & RESOLVE_IDX_PAYLOAD_MASK))
#define RESOLVE_IDX_INT(v)          (RESOLVE_IDX_CLS_INT  | ((long)(v)  & RESOLVE_IDX_PAYLOAD_MASK))
#define RESOLVE_IDX_FLT             (RESOLVE_IDX_CLS_FLT)
#define RESOLVE_IDX_CMP(fn,ar)      (RESOLVE_IDX_CLS_CMP  | ((((long)(fn) << 16) | ((long)(ar) & 0xFFFF)) & RESOLVE_IDX_PAYLOAD_MASK))
typedef struct { IR_t ** args; int nargs; const char * callee; int arity; void * cs; } bb_goal_state_t;
typedef struct { int nclauses; int arity; int mark_slot; IR_t ** args; IR_t ** consts; } pl_gz_choice_state_t;
typedef struct { void * graph_key; int base; int arity; int nlocals; int mark_slot; IR_t * body_head; IR_t * frame_node; void * lblA; void * lblB; int nchild;
                 int nclauses; IR_t ** clause_head; int body_emitted; } pl_gz_callee_t;
typedef struct { pl_gz_callee_t * callee; int nargs; IR_t ** args; int child_slot; int det; } pl_gz_call_state_t;
typedef struct { pl_gz_callee_t ** v; int n; int cap; } pl_gz_callee_vec_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline pl_gz_callee_t * pl_gz_callees_push(pl_gz_callee_vec_t * cv, pl_gz_callee_t * ce) {
    if (cv->n >= cv->cap) {
        int nc = cv->cap ? cv->cap * 2 : 8; pl_gz_callee_t ** nv = (pl_gz_callee_t **)rt_ws_alloc(sizeof(pl_gz_callee_t *) * nc); if (!nv) return (pl_gz_callee_t *)0;
        for (int i = 0; i < cv->n; i++) nv[i] = cv->v[i]; cv->v = nv; cv->cap = nc;
    }
    cv->v[cv->n++] = ce; return ce;
}
typedef struct { pl_gz_call_state_t * call; IR_t * tmpl; int result_slot; int acc_slot; int is_fail; int agg_mode; } pl_gz_findall_state_t;
typedef struct { int functor_atom; const char * functor_name; int arity; int cursor_slot; int mark_slot; } pl_gz_dyniter_state_t;
typedef struct { IR_t * cond_head; IR_t * then_head; IR_t * else_head; int gate_slot; } pl_gz_ite_state_t;
typedef struct { IR_t * goal_head; IR_t * recovery_head; IR_t * catcher; int mark_slot; } pl_gz_catch_state_t;
typedef struct { IR_graph_t * gcfg; IR_t * tmpl; IR_t * result; IR_t * goal_node; } bb_findall_state_t;
typedef struct { IR_t ** kids; int nkids; } bb_match_kids_state_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int bb_match_nkids(const IR_t * nd) {
    if (!nd) return 0;
    bb_match_kids_state_t * zk = (bb_match_kids_state_t *)0;
    return zk ? zk->nkids : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline IR_t * bb_match_kid(const IR_t * nd, int i) {
    if (!nd) return (IR_t *)0;
    bb_match_kids_state_t * zk = (bb_match_kids_state_t *)0;
    if (!zk || i < 0 || i >= zk->nkids) return (IR_t *)0;
    return zk->kids[i];
}
bb_node_state_t * bb_snapshot_state(IR_graph_t * cfg);
void              bb_restore_state(IR_graph_t * cfg, bb_node_state_t * snap);
#ifdef __cplusplus
extern "C" {
#endif
#include "bb_pool.h"
#include "core.h"
#include "bb_box.h"
#include "IR.h"
#include <stdio.h>
int  bb_call_route_classify(IR_t * nd);
bb_box_fn emit_chain(IR_t * entry, FILE * out, const char * prefix);
int  bb_varslot_peek(const char * name);
void lower_flat_set_intern_str(const char * (*fn)(const char *));
const char * emit_intern_str(const char * s);
void lower_flat_reset        (void);
void walk_bb_register_child_label(IR_t * nd, const char * alpha_label);
extern int g_flat_node_id;
void walk_bb_flat(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
void resolve_call_block_label(char *dst, size_t dsz, const char *name, int arity);
void sub_label(char *dst, size_t dsz, const char *name);
void resolve_choice_clause_label(char *dst, size_t dsz, int id, int ci, const char *suffix);
int resolve_emit_callee_block_body(const char *name, int arity, bb_label_t *bγ, bb_label_t *bω, bb_label_t *bβ);
extern IR_t *resolve_bb_entry_node(const char *name, int arity);
int  bb_kind_is_driver_owned(int t);
void bb_prepare_capture_arbno(IR_t *nd, int imm);
void bb_emit_limit_init(int limit_slot_off);
void bb_emit_repalt_clear(int off);
void bb_emit_repalt_yield(int off, int e_slot);
void bb_emit_repalt_test(int off);
const char * child_cache_get_lbl   (bb_box_fn fn);
extern void (*g_cap_fixup_cb)      (void *cap_ptr, const char *child_alpha_label);
extern char   g_flat_data_buf[];
extern size_t g_flat_data_len;
extern int    g_flat_data_any;
void data_buf_flush_pending_label(void);
void data_buf_reset(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline bb_label_t bb_label_from_name(const char *name) {
    bb_label_t lbl = { {0}, -1 };
    if (name) { strncpy(lbl.name, name, BB_LABEL_NAME_MAX - 1); lbl.name[BB_LABEL_NAME_MAX - 1] = '\0'; }
    return lbl;
}
#define bb_flat_set_intern_str(fn)        lower_flat_set_intern_str(fn)
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "bb_pool.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
void    emitter_init_text      (FILE * out, int mode);
void    emitter_init_binary    (bb_buf_t buf, int size);
FILE *  emitter_text_out       (void);
int     emitter_pos            (void);
int     emitter_end            (void);
extern int g_is_text;
extern int g_emit_text_mode;
#define TEXT_MODE_INVOCATION  0
#define TEXT_MODE_DEFINITION  1
void emit_banner              (const char * text);
void emit_bb_dispatch_jne_jmp (bb_label_t * lbl_γ, bb_label_t * lbl_ω);
void emit_bb_zeta_rdi         (uint64_t ptr, const char * sym);
void emit_blank_line          (void);
void emit_data_long           (int32_t val);
void emit_data_quad           (uint64_t val);
void emit_data_quad_sym       (const char * sym);
void emit_data_string         (const char * bytes, size_t len);
void emit_comment             (const char * line);
void emit_directive           (const char * line);
void emit_fprintf_raw         (const char * fmt, ...);
void emit_global_sym          (const char * name);
void emit_jmp_label           (bb_label_t * target, jmp_kind_t kind);
void emit_aligned_call_rt     (const char * sym, void * addr);
void emit_label_name          (const char * name);
void emit_macro_param_ref     (const char * name);
void emit_minor_break         (const char * text);
void emit_pc_label            (int pc);
void emit_section             (const char * name);
#ifdef __cplusplus
}
extern "C++" {
#include <string>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline void emit_comment (const std::string & line) { emit_comment(line.c_str()); }
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
extern int g_is_text;
extern int g_emit_text_mode;
extern int g_emit_pos;
typedef int emitter_t;
void emitter_init_binary    (bb_buf_t buf, int size);
void emitter_init_text      (FILE * out, int mode);
FILE * emitter_text_out     (void);
int    emitter_pos          (void);
int    emitter_end          (void);
void emit_label_define_bb    (bb_label_t * lbl);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "IR.h"
enum {
    CALL_ROUTE_FATAL = 0, CALL_ROUTE_BYNAME = 1, CALL_ROUTE_RK_BOOL_COND = 2, CALL_ROUTE_DVAL2_BOMB = 3, CALL_ROUTE_PROC_STAGED = 5,
    CALL_ROUTE_RK_BOOL_SLOT = 6, CALL_ROUTE_WRITE_SLOT = 7, CALL_ROUTE_WRITE_BINOP = 8, CALL_ROUTE_WRITE_LEGACY = 9, CALL_ROUTE_WRITE_EMPTY = 10, CALL_ROUTE_FN = 11, CALL_ROUTE_BYNAME_GEN = 12
};
struct SrcLines;
typedef struct {
    int                          backend;
    int                          is_binary;
    int                          i;
    int                          n;
    IR_t *                       node;
    int                          sid;
    int                          nid;
    const char *                 op_sval;
    const char *                 op_sval_lbl;
    int64_t                      op_ival;
    int                          op_node_kind;
    double                       op_dval;
    int64_t                      op_counter;
    const char *                 op_a_sval;
    int                          op_a_node_kind;
    int                          op_a_slot;
    int64_t                      op_a_counter;
    int64_t                      op_a_ival_sg;
    double                       op_a_dval;
    int                          op_a_descr;
    int                          op_arith_descr;
    int                          op_parts_n;
    int                          op_parts_tag[32];
    const char *                 op_parts_str[32];
    const char *                 op_parts_lbl[32];
    int64_t                      op_parts_ival[32];
    int64_t *                    gz_arg_slots;
    int                          gz_arg_slots_cap;
    int64_t                      op_scan_pat;
    int64_t                      op_scan_subj;
    int64_t                      op_scan_repl;
    const char *                 op_scan_pat_lit;
    const char *                 op_scan_subj_lit;
    const char *                 op_scan_replace_lit;
    int                          op_sa;
    int                          op_sb;
    int                          op_sc;
    int                          op_off;
    /* BB-OWNED-ζ STEP 1 (Lon pivot, this session).  Set once per node in codegen_flat_chain_body, BEFORE
     * emit_drive(nodes[i],...) is called for that node, at the exact point the flattening loop already
     * computes omega_resolved/otgt (emit.cpp ~line 1498-1500).  True (1) iff this node's ω-edge falls through
     * to the chain's own outer lbl_ω AND the unresolved target is not an IR_SUCCEED node -- i.e., genuinely
     * "falls off the end of this chain to the real failure continuation," which per lower_snobol4.c's
     * sno_ω_to(F, fail) construction (verified this session for IR_MATCH_ARBNO roles 2/5) is exactly the
     * point nothing in this chain can ever read this node's zls slot again.  False for every other resolution
     * (lands at another node's α = clean handoff to a fresh activation; lands at another node's β = that
     * node's β reads THIS node's shared slot per the SN4-PAT-ARBNO role-0/role-2 aliasing, so freeing here
     * would be a premature free one instruction before the last legitimate read).  x86_jmp() in x86_asm.h
     * checks this ONLY when port==OMEGA; every other jump ignores it.  Defaults to 0 (never free) so any
     * node kind this rung hasn't explicitly reasoned through stays inert under ZC_SELFLOAD_ALLOC. */
    int                          op_omega_is_death;
    /* ZLS2 ARENA (Lon directive 2026-07-08, this session) — per-node self-alloc frame size in BYTES for the
     * DOWN-GROWING bare-bump ζ arena: α = `sub r12, K`, single-exit ω = `add r12, K`, both emit-time
     * constants of the box's OWN frame (down-growth is what makes both constants the box's own — up-growth
     * would need the PREDECESSOR's size, unknowable at emit time).  Read by the α-define central hook in
     * x86_asm.h ONLY under ZC_PORT_ALLOC (SCRIP_ZETA_PORT=2); the matching free is NOT hooked centrally at
     * x86_jmp(ω) — the six-decoy-ω finding forbids that — a construct places x86("add","r12",K) at its own
     * single true-exit label instead.  Defaults to 0 (no self-alloc) at every DRIVE_FILL, so every node this
     * ladder hasn't explicitly converted stays byte-identical in all port modes. */
    long                         op_zls2_bytes;
    /* ZLS2 PORT-HOOK FRAME PROTOCOL (Lon directive 2026-07-08 session 3, completing the field above): the
     * per-node injection geometry, computed by zls2_geom (zeta_storage.c — the save-slot layout authority)
     * at the walk_bb_node dispatch cases and read by x86_port_hook under ZC_PORT_ALLOC.  op_zls2_slot = the
     * absolute [ZR+off] frame offset of the node's activation-block save slot (-1 = none); op_zls2_ops = the
     * ZLS2_BUMP/RESTORE/RELEASE bitmask (zeta_choices.h) selecting which port injections fire.  BOTH default
     * inert at every DRIVE_FILL (slot -1, ops 0), so nodes outside the granted consumers stay byte-identical
     * in every port mode.  The ops==0 && bytes>0 combination remains the DORMANT direct-sub discipline (the
     * recorded future full-discipline design: sub ZR,K at α for boxes that never touch the graph frame) —
     * the C-call push/pop scheme keys on ops!=0 and never collides with it. */
    int                          op_own_mark;
    int                          op_own_ci;
    int                          op_zls2_slot;
    int                          op_zls2_ops;
    /* ZC_PORT_FORTH FIXED-CELL GRANT (RUNG ZB-FC-0, Lon 2026-07-12; authority fc_geom, zeta_storage.c; design
     * zeta_choices.h ZC_PORT_FORTH + ARCH-ZETA S10): op_fc_bytes = the box's fixed 16-multiple cell size
     * (0 = no grant, hook + FR translation inert -- every ungranted node byte-identical in all port modes);
     * op_fc_base = the flat-frame base the box's own FR/FRQ offsets are rebased against ([rsp + off - base]
     * while control is inside the box, the S10c port invariant); x86_fc_synth = the descending internal-label
     * id pool for the x86_jcc conditional-omega invert+pop+jmp synth (reset per node at DRIVE_FILL; starts at
     * 240, below X86_INTERNAL_MAX and far above any template's own L(n) ids). */
    long                         op_fc_bytes;
    int                          op_fc_base;
    long                         op_fc_fpmax;      /* ZB-FC-3a: pad-to-max footprint for a granted ALTERNATE; -1 = not a granted ALT */
    int                          op_fc_arm_fp[16]; /* ZB-FC-3a: per-arm EXACT static footprints (pads = fpmax - fp[j]) */
    long                         op_fc_disp;       /* ZB-FC-3c: COND/IMM cross-box read displacement to SAVE's cell = fp(inner) ([rsp + disp] at the yield frontier, S10c); -1 = ungranted, the flat rt_cap array path */
    int                          op_tail;          /* R12-EXIT-1 CARRY-THE-TAIL: this box participates in a tail-granted ARBNO statement (elements ride rsp, [rsp+const] everywhere, no window register) */
    int                          op_tail_fpb;      /* fp(body) — granted-cell footprint suspended at every yield; the α phantom pad and the σ/β header displacement */
    int                          op_tail_fpl;      /* fp(left-spine) — HEAD-cell distance at ARBNO α (bracket copy source) */
    int                          op_tail_seal;     /* R12-EXIT-1 L2: FENCE-rooted body (operands[3]==self) — resume ≡ abandon; the template's PAIR(1) jumps retarget the seal glue (add rsp,FPB into φ) */
    int                          op_tail_ncap;     /* R12-EXIT-1 WRAP-CAPTURE: number of ARBNO(body).V wrap pairs — 16-byte element slots above the bracket (α copies from flat [KA+16j], β carries forward, COND reads [HDRA+32+16j]) */
    long                         op_fc_wbytes;     /* ZB-FC-3d: WINDOW-ONLY width for a self-pushing box (HEAD): feeds the FR/FRQ rebase like op_fc_bytes but NEVER arms the alpha-sub/omega-add hook (the box pushes itself, the statement UNWIND releases); 0 = no window */
    int                          op_scan;          /* SPD-2 RETRY-INTERNAL: this IR_MATCH_DEFER/PATREF is the statement's sole frozen-pattern root (head-γ successor, ω back to head-β) -- its fast-arm transfer sets r8=1 so the flat_pat blob runs the positional retry internally */
    int                          op_seal;          /* s137 OVER-SEAL: this IR_MATCH_DEFER/PATREF's target is a statically right-sealed stored pattern (IR_t.seal) -- α saves an rsp watermark in the defer.pad quad (op_off), the γ/ω glues bulk-restore it (whacking the callee subtree at the fence-demarked sync point), β is an unconditional exhaust */
    int                          op_scan_head_off; /* SPD-2: the statement head's position-counter frame slot (FR), for the γ-side winning-start write-back (bb_match_replace reads it as the match start) */
    int                          op_seq_static;    /* SPD SEQ-STATIC: 1 = celled IR_MATCH_SEQUENCE proven CLEAN by the chain prepass (all sigma/phi sources are element/resume roots, all roots chain-resident) -- template takes the zero-counter statically-wired arm; 0 = counter-dispatch arm verbatim */
    int                          op_fc_seq;        /* ZB-FC-3b: 1 = this IR_MATCH_SEQUENCE is FORTH-converted (ZERO cell -- seq_i/delta retired; the LIFO stack position IS the element index, so sigma/phi become static edge re-points and the box owns no rsp storage) */
    int                          op_arbno_chain;   /* ZB-FC-4 (Lon s50 S14): 1 = this IR_MATCH_ARBNO uses the s32 LINKED FRAME CHAIN on rsp (per-iteration nodes pushed by beta, chained by prev-link, unwound at omega via the saved pre-alpha rsp) instead of the rt_zcol_push heap COLLECTION; set under ZC_PORT_FORTH only, so default is byte-identical */
    int                          x86_fc_synth;
    int                          op_selfload;
    int                          op_beta_dead;     /* BP-9 DEAD-β ELISION: 1 = the flat-chain driver proved this det leaf's β label has ZERO inbound edges (wire scan mirrors the loop's β-tag routing); converted templates elide the β define + trampoline jmp via x86_beta_trampoline(); 0 = emit as always */
    int                          op_wpop;          /* BP-9 (ii) ΣK ζ-POP FOLD: summed fc-cell bytes of the trivial-β trampolines the driver's ω-wire chase inlined past for THIS node (node_ω retargeted to the chain's final label); the X86H_JMP/OMEGA hook arm spends it as ONE add rsp,ΣK.  0 = no fold.  Per-node, reset every drive. */
    int                          op_phase;
    int                          op_binop_kind;
    int                          op_gva_k;
    int                          op_gva_k1;
    int                          op_gva_k2;
    int                          op_proc_k;
    int32_t                      op_stno;
    int                          op_bounded;
    int                          op_relop_descr;
    int                          op_num_real;
    int                          op_imm_a_ok;
    int                          op_imm_b_ok;
    long                         op_imm_a;
    long                         op_imm_b;
    int                          op_write_route;
    int                          op_call_route;
    const char *                 lbl_α;
    const char *                 lbl_γ;
    const char *                 lbl_ω;
    const char *                 lbl_β;
    const char *                 lbl_t0;
    const char *                 lbl_t1;
    struct bb_label_t *          lbl_α_p;
    struct bb_label_t *          lbl_γ_p;
    struct bb_label_t *          lbl_ω_p;
    struct bb_label_t *          lbl_β_p;
    struct bb_label_t *          lbl_t0_p;
    struct bb_label_t *          lbl_t1_p;
    void *                       child_fn;
    const char *                 op_name1;
    const char *                 op_name2;
    const char *                 op_kind;
    int                          resolve_choice_id;
    int                          resolve_choice_n;
    int                          in_body;
    const char *                 in_my_method;
    const int *                  pc_to_fn;
    const char **                fn_names;
    const int *                  fn_pcs;
    int                          fn_count;
    const struct SrcLines *      srclines;
    int                          win_exec_pat_id;
    int                          hdr_count;
    int                          hdr_has_expr_reg;
    int                          hdr_has_reg;
    int                          reg_expr_count;
    int                          reg_count;
    char                         bb_ptr_slot_lbl[88];
    int                          bb_cs_id;
    void *                       bb_cs_zeta;
    void *                       bb_rt_obj;
    const char *                 bb_child_lbl;
    void *                       bb_child_fn;
    char                         bb_ls_buf[64];
    char                         bb_rs_buf[64];
    char                         bb_op_buf[64];
    const char *                 bb_ls;
    int                          pat_via_dtp;
    const char *                 bb_rs;
    const char *                 bb_op_lbl;
    int                          bb_lk;
    int64_t                      bb_li;
    int                          bb_rk;
    int64_t                      bb_ri;
    void *                       bb_ln;
    void *                       bb_rn;
    void *                       bb_zn;
    const char *                 flat_lbl_α_body;
    const char *                 flat_lbl_β;
    const char *                 flat_lbl_α;
    const char *                 flat_lbl_γ;
    const char *                 flat_lbl_ω;
    int                          flat_text_externalise;
    const char *                 enclosing_fname;
    const char *                 prev_instr_name;
    int                          flat_wired;
    int                          flat_jmp_entry;      /* ZS-2 (Lon s58): rtpat/rtlen/rtbrk blob regime — jmp-entered new activation, self-allocates on rsp, 32B wire header at [rsp+0..31], γ suspends / ω unwinds via the wired outside edges; NO esi dispatch, NO ret */
    int                          flat_pat;         /* R12-ERAD s65: this jmp-entry graph is a PAT$ SUSPENDING blob — r12-frame island arms in xa_flat + r12 FR prefix (set by emit_jmp_entry_for_patproc, cleared with flat_jmp_entry) */
    int                          flat_lex;         /* NCB-1d: this jmp-entry graph is a DET LEXICAL proc (Icon/Raku, dyn_scope=0) — the prologue appends the rt_jmp_frame_lexprep call (NULVCL fill + frame arg-bind, replacing the call-regime rt_frame_prep the site no longer runs); set by emit_jmp_entry_for_proc, cleared with flat_jmp_entry.  PAT$/EVAL/LBL__ never set it (no matching pcall record to read). */
    int                          flat_gen;         /* GENP-SPINE (Lon 2026-07-17 s92: "generator procedures on the main spine; co-expressions are the only construct requiring a separate stack/pthread"): this jmp-entry graph is a GENERATOR proc resident on the ONE RSP/RBP ζ stack — xa_flat's γ epilogue takes the RETAINING suspend form (rdi:rsi result preload + 16B resume record at the deep frontier, no unwind), the res landing re-pins rbp and dispatches jmp [rbp+resume_slot] (slot armed by each bb_suspend α, POISONED to lbl_ω by bb_return so post-return resumption fails), ω unwinds absolutely; the caller's β edge is jmp qword [rsp].  Supersedes the s91 per-instance coexpr-stack model (rt_genp_*, retained only for legacy non-RSP configs + C-window value calls).  Set by emit_jmp_entry_for_proc, cleared with flat_jmp_entry. */
    int                          frame_region;
    int                          flat_frame_bytes;    /* ZS-2: K_total for the jmp-entry prologue's sub rsp (32B header + zls region, 16-mult), computed by the blob builder BEFORE emit from zls_g_region */
    int                          flat_seed_off;       /* PL-REGAIN-4: byte offset where the frame's NULVCL-seed suffix starts (resume slot if present, else zeta-mark; named locals follow to region end) — set with flat_frame_bytes from zls_g_resume/zls_g_zeta_mark; 0 = layout unknown, prologue falls back to the full stosb+lexprep fill.  Consumed only by xa_flat's flat_lex arm (rt_jmp_frame_lexprep2). */
    int                          flat_layout_unknown; /* SPD-NOFILL (s139): 1 = emit_jmp_entry_arm_region took the rg<=0 fallback (zls layout unknown, region defaulted 4096) — xa_flat's no-fill arm is FORBIDDEN for such graphs (slot classification unavailable), they keep the eager stosb; 0 = layout granted through zeta_storage, seed_off trustworthy.  Set beside flat_seed_off, cleared with flat_jmp_entry. */
    struct bb_label_t *          flat_res_p;          /* ZS-2: the β-resume landing stub (lea r12,[rsp+32] falling into lbl_β); prologue stores its address into the [rsp+0] wire */
    struct bb_label_t *          flat_succ_p;
    struct bb_label_t *          flat_fail_p;
    struct bb_label_t *          flat_β_p;
    struct bb_label_t *          flat_dc_body_p;      /* PL-DC (REGAIN-1C s108): the graph's α_body label, handed to xa_flat_dc_stub so the per-proc direct-call stub can 'X'-jmp the shared body (same emission session — the pointer is the chain-body local, live through the stub emission).  NULL outside dc-eligible graphs. */
    const char **                xa_label_names;
    int *                        xa_label_pcs;
    int                          xa_label_count;
    const char **                xa_expr_names;
    int *                        xa_expr_pcs;
    int *                        xa_expr_str_idxs;
    int                          xa_expr_count;
    int                          xa_strtab_n;
    const char **                xa_strtab_labels;
    const char **                xa_strtab_escaped;
    int                          xa_csettab_n;
    const char **                xa_csettab_labels;
    const char **                xa_csettab_rows;
    const char *                 xa_cap_dlbl;
    const char *                 xa_cap_child_lbl;
    int                          xa_cap_is_arbno;
    int                          xa_cap_is_callcap;
    const char *                 xa_cap_varname_lbl;
    int                          xa_cap_immediate;
    int                          xa_pat_blob_invariant_n;
#define XA_BB_EMIT_PAIR_MAX 1024
    struct bb_label_t *          xa_bb_emit_pair_define[XA_BB_EMIT_PAIR_MAX];
    struct bb_label_t *          xa_bb_emit_pair_jmp[XA_BB_EMIT_PAIR_MAX];
    int                          xa_bb_emit_pair_n;
    int                          x86_uid;
    int                          x86_scratch_off;
    int *                        op_arg_slot;
    int                          op_arg_slot_cap;
    int                          op_arg_slot_n;
    int                          op_c2;   /* CALL2BB slice 2: 1 iff THIS call node's role-0 IR_SAVE_RESTORE partner emitted the SCC prefix (node-exact handoff, consumed in the call-family drive arm) — the staged det arm selects its skip variant on this, never on emission adjacency (BFS interleaves; control follows γ edges) */
    int                          op_arbno_zq[8];   /* s141 ARBNO-NOFILL: ζ offsets of the body's IR_MATCH_ASSIGN_SAVE cap BUF quads (implicit-zero citizens, s139 rt_cap head-cell class) — chain-β zeroes ONLY these per element.  APPENDED AT STRUCT END: mid-struct insertion shifted downstream g_emit offsets and broke the m4 runtime-EVAL path (jmp-entry K_total=0) — layout-append is the shared-ABI rule for this struct. */
    int                          op_arbno_nzq;     /* count for op_arbno_zq; 9 = >8-capture / no-field sentinel → template keeps the eager blanket fill */
    int                          sn4_defer_cell_n; /* s142 DEFER-SITE DIET: monotonic emit-time index into g_sno_defer_cells (runtime .bss, pattern_match.c) — one RW quad per WRITE-ONCE (IR_t.seal==2) GVA defer site caching the resolved blob fn, replacing the 8-Ir GVA/DT_P/memo dance with lea+load+test on the steady path.  Never reset: indices only need uniqueness within one scrip process; ≥4096 falls back to the uncached path.  APPENDED AT STRUCT END per the s141 ABI law (mid-struct insertion breaks baked offsets — see op_arbno_zq's comment). */
    int                          flat_cap_off[48]; /* CAP-NOFILL (s143): distinct IR_MATCH_ASSIGN_SAVE capture-slot region offsets for the CURRENT jmp-entry graph — each is a 16B rt_cap grant (+0 buf ptr, +8 gen, +12 sp; a garbage buf ptr with self-healed gen is deref'd by rt_cap_push, hence both quads must read zero) that xa_flat's SPD-NOFILL arm zeroes INDIVIDUALLY instead of pulling flat_seed_off down to the lowest one and blanketing the whole span above it (expression-eval: 560B rep stosb per PAT$ activation = 52% of blob Ir).  Set by emit_jmp_entry_arm_region beside flat_seed_off; SCRIP_CAP_NOFILL=0 or >48 distinct sites falls back to the s139 pull-down.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          flat_cap_n;       /* CAP-NOFILL (s143): count of live entries in flat_cap_off; 0 = none/fallback (xa_flat then behaves exactly as s139).  Cleared with the rest of the jmp-entry arm in emit_jmp_entry_clear. */
    int                          op_arbno_dt;      /* PS-3 (s152) ARBNO(*P) DEFER-TAIL: 1 = body is a single WRITE-ONCE (seal==2) prologue-bound defer whose target PAT$ registered a uniform suspension footprint — the zero-ceremony arm (no link, no view, no element window; the blob's own carve+frontier record IS the iteration frame).  Staged at the IR_MATCH_ARBNO drive, template-selected before op_arbno_chain; SCRIP_ARBNO_LATCH=0 kills.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_arbno_dt_susp; /* PS-3 (s152): the target blob's per-activation suspension footprint = align16(32 + frame_bytes) + fp_total + 16 (γ-record) from the emit_patzeta registry — the compile-time ζ size of the arriving DT_P.  σ reads the element's entry cursor at [rsp + susp] from the uniform yield depth.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_defer_leaf_susp; /* PS-3 (s153): >0 = this IR_MATCH_DEFER/PATREF is a PRICED leaf of a FINALIZED R12-EXIT tail candidate (fc_tail_defer_susp_g) and its β must be the ZERO-GUARDED form — test the γ-record at [rsp+0]; on zero (the ε element's phantom FPB pad) pop this SUSP share and ω-transit like an exhausted leaf; else jmp through the record as ever.  -1 keeps the original β byte-verbatim.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_tail_dfr;      /* PS-3 (s153): 1 = this finalized tail candidate carries priced defer leaves — the tail α zeroes the phantom FPB pad [0,FPB) so the guarded defer β's zero-discriminator is deterministic (the pad is otherwise dirty stack; granted-leaf βs are content-blind but the defer β is content-sensitive).  APPENDED AT STRUCT END per the s141 ABI law. */
    const char *                 op_src;           /* SRC-COMMENT (2026-07-26): verbatim source text for a statement, keyed in the bb_src registry (lower_common.c) to the statement's SURVIVING head box (the anchor IR_GOTOs are threaded away by the optimizer's branch_chain/dead_goto, so sno_build_graph's post-pass chases each anchor's γ to the first non-goto and notes THAT node); looked up at the dispatch fill for every node, NULL on miss and under SCRIP_SRC_COMMENT=0, emitted there as one TEXT-only x86("comment") per source line — the comment encoder's BINARY arm is empty by construction, so mode-3 bytes are untouched.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_res_live;      /* ZB-VAL-8b USE PREDICATE (Lon directive s182: "allocate its RESULT value, IF it has one and if it is used"): 1 = some consumer reads this node's result cell as a VALUE (an operands[] edge); 0 = MEASURED dead -- every reference to it is a γ/ω CONTROL wire, so the box may skip WRITING the cell as well as owning it.  Promoted from zls_result_live() beside the operand slots it qualifies; DEFAULTS to 1 so any node the analysis never saw keeps writing.  APPENDED AT STRUCT END per the s141 ABI law. */
    long                         op_flat_disp;     /* FLATDISP-RESTORE: under ZC_FRAME_RSP the static bytes between rsp and the FLAT frame base at this box's own code (LOWER's fc_leaf registrar, still populated every compile since s65; the consumer was deleted s83 REG-7 U3 and the value silently discarded ever since).  FR/FRQ add it to every non-window ref when SCRIP_FLAT_DISP=1, restoring [rsp+off+D] general addressing instead of the per-kind fc_geom whitelist.  0 = at base.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          flat_deep_arrival; /* FLATDISP-5 (Lon directive s192: "continue until you hit a BRICK WALL and realize, oh I need a RBP stable base pointer"): 1 = some γ/ω arrival in THIS graph can land at a depth BELOW the activation base, so the frame-base restore must be rbp-ABSOLUTE (the depth-immune `mov rsp,rbp` / `lea rsp,[rbp+kt]` form) and the caller-rbp header save/seed must be emitted.  0 = DEPTH-STATIC: every arrival lands with rsp == base by LIFO balance, the frame-pointer dance is pure ceremony, and the whole rbp quartet (save/seed/teardown/restore) is DELETED.  Named by WHAT DIFFERS, never by language (RULES.md NO-LANGUAGE-SENTINEL): the property is "can an arrival be deep?", which is a function of the CONSTRUCTS present — the generator/suspending family (IR_SCAN_*, IR_TO*, IR_LIMIT, IR_REPALT, IR_PROC_GEN, IR_CREATE, IR_ITERATE, IR_DISJUNCTION) can `return` from inside nested scan/generator depth, the determinate family cannot.  xa_flat.cpp's own line-448 comment is the provenance: the rsp-relative reads were retired because that assumption is "true for SNOBOL4's determinate procs, FALSE for Icon" — this field restores the cheap form for exactly the graphs where it was always sound.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          flat_fb_refine;   /* FB-STMT (Lon directive, 2026-07-29 "Change every RBP to RSP that can be. I want only the housekeeping data indexed by RBP"): 1 = this pinned graph's deep kinds are ALL statement-bracketed (DEFER/ARBNO/FENCE1/VALUE inside a HEAD..RELEASE/REPLACE range, whose exits rebalance rsp through the head snapshot — the 058 ground-truth law the census comment records), so per-node fb refinement is licensed: nodes OUTSIDE deep match statements arrive at static depth and speak rsp+op_flat_disp (the FLATDISP-9-corrected machinery, proven by the live unpinned regime), while deep-statement residents and every match-family box keep the pinned rbp.  0 = whole-graph rbp exactly as before (flat_pat / flat_gen / gen-proc / non-bracketed deep kinds — ABORT and CALLOUT excluded because the abort seal path skips the ω-choke rebalance, per the 3ea60fc2 pat_seal note).  Ceremony, record protocol and epilogues read emit_jmp_pin_rbp/emit_rec_pin UNCHANGED — this field refines DATA SPELLING only, so mixed spellings address the same bytes by construction (rbp==base, static reader rsp==base−live, disp compensates).  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_fb_rbp;        /* FB-STMT per-node twin, set at both promotion sites from the emit_chain map: 1 = this node's FR/FRQ/FRQB refs speak the pinned rbp base; 0 = statically-arriving node, speak rsp+op_flat_disp.  Consulted only when flat_fb_refine=1 and SCRIP_FB_STMT != 0.  Unmapped nodes (foreign drivers) default 1 — conservative.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          flat_stmt_frame;  /* STMT-FRAME REGIME (s21x-c design of record, Lon: per-BB one-instruction carves; STATEMENT bracket = the rbp dance; NO graph carve, NO zero-fill, NO anchor): 1 = this graph passed emit_stmt_frame_scan under SCRIP_STMT_FRAME=1 -- the ALL-OR-NOTHING per-graph switch (mixed regimes inside one graph are the s188 disease).  Consumers: xa_flat prologue/epilogue (pad-only 8B arms), the drive loop's per-statement head stubs (bb_glue_framed_enter at every bb_src_of head, GLUE-4) and the two chain exit cuts (bb_glue_framed_leave glue in front of lbl_γ/lbl_ω), plus the head/port retargets beside the SPD-2 scanhit redirect.  0 = legacy regime verbatim.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_subj_cell;     /* SUBJECT-CELL rung (a) (Lon s21x directive "NO FRAME RELATIVE addressing for operands"): 1 = IR_MATCH_HEAD's subject rides the FORTH cell -- the subject producer chain is fvl/fvb-registered (zls_build SUBJECT loop, env-gated SCRIP_STMT_FRAME+SCRIP_SUBJ_CELL) and the head POPS the 16B subject DESCR from TOS ([rsp+0]/[rsp+8], add rsp,16) at the very top of alpha, BEFORE any flat-spelled housekeeping access, so every downstream op_flat_disp prefix (fc_leaf_walk's D=32+prefix math) is byte-identical to the flat arm; 0 = legacy FRQ(op_sa) flat-slot read.  Promoted at the IR_MATCH_HEAD drive case from fc_vread_fp(nd) >= 0 under ZC_FRAME_RSP + ZC_PORT_FORTH -- planner and emitter agree via the ONE registry (the fc_call_ok two-env precedent). */
    int                          zop_seen;         /* ZOP-1 MIX AUDIT (Lon s21x-n "parameterize the access to operands via the FOUR modes"): bitmask of the operand REGIMES x86_zop resolved to while emitting the current graph -- bit1 r12 island frame, bit2 own per-BB rsp cell, bit3 pinned rbp whole-graph frame, bit4 rsp whole-graph frame + op_flat_disp.  Bit 2 composes with exactly ONE of {1,3,4} by design (the per-BB cell is a window INTO a regime, not a regime); two or more of {1,3,4} set on one graph IS the s188 mixed-regime shape that zeta_choices.h declares must be unrepresentable, and which stayed silently representable for as long as the five deciding booleans were spread across three accessors.  Reset per graph beside the other flat_* fields; read by the ZOP audit and by test_gate_zop_one_regime.sh.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_zdepth;        /* ZTOS-1 (Lon s21x-o "keep track of sliding offsets and index operands from RSP, not RBP" + "no pre-allocation calculation is necessary"): the bytes THIS box carved on rsp below the incoming value spine, live for the whole of its own body.  Every TOS-relative operand read (ZTOS/ZTOSD) adds it, so a box's own carve and its own operand reads compose to a no-op BY CONSTRUCTION.  THE POINT IS THAT THIS IS LOCAL: the compensation a box owes is exactly what IT allocated, never a graph-wide prefix sum, which is why no pre-allocation pass is needed and why arming one more BB can no longer displace a neighbour.  This is the field that replaces op_flat_disp's role for spine references — op_flat_disp is LOWER's static answer to a question that became dynamic the moment every BB carved for itself (the s21x-n "depth displacement" negative result; 028_arith_unary_minus printed 5 for -5 because unop's own sub rsp,16 slid its operand out from under a hardcoded [rsp+0]).  0 = this box carved nothing, so its reads are already at the spine.  APPENDED AT STRUCT END per the s141 ABI law. */
} sm_emit_t;
extern sm_emit_t g_emit;
static inline int emit_jmp_pin_rbp(void) { return g_emit.flat_deep_arrival || g_emit.flat_pat || g_emit.flat_gen; }   /* ZW-1 s21x-m REVERTED ARM (kept as a law, do not retry blind): the universal carve tempted a `SCRIP_BB_ALLOC || ...` disjunction here to make flat refs depth-immune (the 023 intra-box displacement).  MEASURED SEGV with ZERO BBs armed -- the pin is not free: it gates the xa_flat prologue's caller-rbp save at [rsp+kt-8] + seed, and kt is sized by a layout pass that assumed NO pin for these graphs, so pinning a never-pinned graph writes the save outside its own header and returns on a clobbered rbp.  THE PIN IS A LAYOUT CONTRACT, NOT A SPELLING SWITCH.  The intra-box displacement it was meant to cure belongs to op_flat_disp (the running rsp-depth prefix sum, LOWER's own instrument) or to window migration, never to a blind pin flip. */
static inline int emit_heap_fb_adopt(void) { extern int g_gen_proc_active; extern int g_resumable_callable_active; return g_gen_proc_active || g_resumable_callable_active; }   /* ZETA-FB-3 (s161): THE SECOND PROLOGUE ARM, NAMED.  Two prologue shapes establish rbp as the frame base: (1) xa_flat's jmp-entry hdr SAVES the caller rbp and SEEDS rbp=rsp at the activation's flat base, gated emit_jmp_pin_rbp(); (2) THIS one ADOPTS a frame that arrives in rdi already allocated on the heap (`push rbp; mov rbp,rdi`), carving NO rsp frame at all — the activation's frame must survive beta-resume, so it cannot live on a stack that unwinds.  Arm (2) was spelled as the raw `g_gen_proc_active || g_resumable_callable_active` pair at SIX xa_flat sites (4 prologue incl. both media, 2 epilogue) plus once inside emit_rec_pin — SEVEN copies of one condition, the duplication class that put s158's land mine in the tree: edit one copy, the store and the load name different base registers.  ZETA-FB-2 unified the CONSUMERS (x86_fb_pinned == emit_rec_pin); this names the PRODUCER so the union below is written ONCE as the disjunction of two named arms and every site reads a predicate instead of re-deriving one.  Behaviourally identical by construction — same boolean, one name; MEASURED byte-identical over 184 .s artifacts across three frontends. */
static inline int emit_rec_pin(void) { return emit_jmp_pin_rbp() || emit_heap_fb_adopt(); }   /* REG-7 U5 RECORD-PROTOCOL PIN (s158): emit_jmp_pin_rbp covers the FLAT jmp-entry regime only (deep arrival / pat / gen blobs).  A gen-proc or resumable-callable activation carves NO rsp frame at all -- its base is the HEAP frame in rbp (heap-fb-as-rbp, GOAL-PROLOG-BB JOB #1) -- so it needs the record protocol on rbp for exactly the same reason, and the beta dispatch below ALREADY gates on this pair.  Keeping the two predicates apart is what let the store and the load pick different base registers. */
static inline const char * emit_rec_fb(void)     { return emit_rec_pin() ? "rbp" : "rsp"; }   /* THE one record-protocol frame base: the 16B suspend record's [+8] payload is the SUSPENDED activation's frame base, and the resume-slot STORE, the res-landing POP and the beta DISPATCH must all name the SAME register or they drift.  Not x86_fb(), which s189 collapsed to the constant "rsp" after s188 compensated the ordinary flat refs -- correct for a flat activation (base == rsp), fatal for a heap-fb one, where `pop rsp` loads a heap base into the stack pointer and the next indirect jmp lands wild (the 44-test backtrack SIGSEGV). */
static inline int          emit_rec_fb_num(void) { return emit_rec_pin() ? 5 : 4; }           /* BINARY twin of emit_rec_fb -- rbp=5 / rsp=4.  R10: the two media MUST pick the same register or the mode-3 slab and the mode-4 .s diverge at the resume landing. */   /* FLATDISP-7 (s194): THE one jmp-entry rbp condition — every jmp-entry gate site (prologue save+seed, determinate epilogue reads+restore, wire-adopt marshal, BOTH media) reads THIS, never the raw fields, so save/seed/read/restore cannot drift apart (the s193 xaf_deep discipline applied to the jmp-entry regime).  The flat_pat/flat_gen conjuncts are LOAD-BEARING, not conservatism: a SUSPENDING activation's γ RETAINS with rsp at the deep frontier and its resume/exit protocol reads the PINNED rbp by design ([rbp+kt-24/-16/-8], the REG-7 U5 record contract), so the pin is required even when the kind census classifies the graph depth-static — TODAY an INVARIANT PAT$ blob (pure LIT/SPAN body, no fence/arbno/defer) is exactly that case; FLATDISP-5a (inline invariant patterns, no blob) will make the flat_pat conjunct fire less, never wrong.  Determinate graphs (neither conjunct) pin only when an arrival can be deep — the classifier's per-graph answer, language-blind per NO-LANGUAGE-SENTINEL. */
extern IR_graph_t * g_emit_cfg;
extern const char *Σ;
extern int         Σlen;
extern int         Δ;
#define TEMPLATE_ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
#define TEMPLATE_ADDR_SIGLEN  ((uint64_t)(uintptr_t)&Σlen)
#define TEMPLATE_ADDR_DELTA   ((uint64_t)(uintptr_t)&Δ)
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "IR.h"
IR_t * bb_child0(const IR_t *n);
IR_t * bb_child1(const IR_t *n);
int    binop_slot_kind(IR_t *nd);
int    bb_call_write_route(IR_t *nd);
int    emit_binop_opnd_slot(IR_t *o);
int    binop_is_num_real(IR_graph_t *g, IR_t *nd);
void   bb_fill_alpha(IR_t *nd);
void   emit_drive(IR_t *nd, bb_label_t *lbl_α, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "core.h"
void emit_sm_halt           ();
void emit_sm_push_lit_i     (int64_t val);
void emit_sm_push_lit_s     (const char * str_lbl, uint64_t str_ptr, int len);
void emit_sm_push_var       (const char * name_lbl, uint64_t name_ptr);
void emit_sm_store_var      (const char * name_lbl, uint64_t name_ptr);
void emit_sm_push_expression(uint64_t entry_ptr, int arity);
void emit_sm_call_expression(const char * tgt_sym);
void emit_sm_exec_stmt      (const char * subj_lbl, uint64_t subj_ptr, int has_repl);
void emit_sm_call_fn        (const char * name_lbl, uint64_t name_ptr, int nargs);
void emit_sm_define         ();         void emit_sm_define_entry   ();
void emit_sm_jump           (int target_pc);
void emit_sm_jump_s         (int target_pc);
void emit_sm_jump_f         (int target_pc);
void emit_sm_label          ();
void emit_sm_return         ();
void emit_sm_return_variant (int kind, int cond, int pc);
void emit_sm_freturn        (int pc);   void emit_sm_nreturn        (int pc);
void emit_sm_return_s       (int pc);   void emit_sm_return_f       (int pc);
void emit_sm_freturn_s      (int pc);   void emit_sm_freturn_f      (int pc);
void emit_sm_nreturn_s      (int pc);   void emit_sm_nreturn_f      (int pc);
void emit_sm_suspend        ();         void emit_sm_suspend_value  ();
void emit_sm_bb_pump        ();         void emit_sm_bb_once        ();
void emit_sm_bb_pump_case   ();         void emit_sm_bb_pump_sm     ();
void emit_sm_bb_pump_every  ();         void emit_sm_bb_pump_ast    ();
void emit_sm_load_glocal    ();         void emit_sm_store_glocal   ();
void emit_sm_icmp_gt        ();         void emit_sm_icmp_lt        ();
void emit_sm_load_frame     ();         void emit_sm_store_frame    ();
void emit_sm_pat_lit        (const char * name_lbl, uint64_t name_ptr);
void emit_sm_pat_refname    (const char * name_lbl, uint64_t name_ptr);
void emit_sm_pat_usercall   (const char * name_lbl, uint64_t name_ptr);
void emit_sm_pat_capture        (const char * name_lbl, uint64_t name_ptr, int kind);
void emit_sm_pat_usercall_args  (const char * name_lbl, uint64_t name_ptr, int nargs);
void emit_sm_pat_capture_fn     (const char * fname_lbl, uint64_t fname_ptr, int is_imm,
                                  const char * namelist_lbl, uint64_t namelist_ptr);
void emit_sm_pat_capture_fn_args(const char * fname_lbl, uint64_t fname_ptr, int is_imm, int nargs);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdio.h>
void emit_text_n(const char * s, size_t n);
void emit_text_flush(void);
long emit_text_count(void);
void emit_textf (const char * fmt, ...)
                 __attribute__((format(printf, 1, 2)));
void emit_textf_flush(void);
void emit_sep_rule_c(char ch);
void emit_set_sink(FILE * out);
#ifdef __cplusplus
}
extern "C++" {
#include <string>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline void emit_1asm (const std::string & a)            { emit_1asm(a.c_str()); }
inline void emit_2asm (const std::string & a, const std::string & b) { emit_2asm(a.c_str(), b.c_str()); }
}
#else
#endif
#ifdef __cplusplus
#include <string>
#include <cstdio>
#include <sstream>
namespace EmitStr {
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
template<typename... Args>
inline std::string format_str(const char * fmt, Args... args) {
    char buf[4096];
    snprintf(buf, sizeof buf, fmt, args...);
    return std::string(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string emit_comment_str(const char * line) {
    return std::string(line) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string emit_directive_str(const char * line) {
    return std::string(line) + "\n";
}
}
using namespace EmitStr;
#ifdef __cplusplus
#include <string>
#include <vector>
#include <cstdio>
#include <cstdint>
extern "C" {
}
std::string emit_fmt(const char * f, ...) __attribute__((format(printf, 1, 2)));
std::string u8   (unsigned v);
std::string u32le(uint32_t v);
std::string u64le(uint64_t v);
std::string bytes(size_t n, const char * lit);
std::string bomb_text (const char * msg);
std::string bomb_bytes(const char * msg);
std::string jvm_push_int2_str(long v);
std::string jvm_emit_ldc_string_str(const char * s);
std::string js_escape_string_str(const char * s);
std::string gas_escape_str(const char * s);
std::string wasm_emit_data_segments_str(void);
std::string jvm_class_hdr_str(const char * name);
std::string jvm_init_ms_str_str(const char * name, const char * field);
std::string jvm_init_ms_only_str(const char * name);
std::string jvm_init_ms_int_str(const char * name, const char * field);
std::string jvm_val_helper_str(const char * name);
std::string net_escape_ldstr_str(const char * s);
std::string net_class_hdr_str(int sid, int nid);
std::string net_α_hdr_str();
std::string net_β_hdr_str();
std::string net_fail_ret_str();
std::string net_cursor_load_str();
std::string net_ms_length_str();
std::string net_spec_of_str();
std::string net_charset_class_str(int sid, int nid, const char * tag);
std::string net_push_i4_str(int v);
std::string net_ctor_none_str(int sid, int nid);
std::string net_spec_zw_str();
#define IF(c, ...) ((c) ? (__VA_ARGS__) : std::string())
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
template<typename F>
inline std::string FOR(int lo, int hi, F f) {
    std::string r;
    for (int i = lo; i < hi; i++) r += f(i);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
template<typename F>
inline std::string emit_for(int lo, int hi, F f) { return FOR(lo, hi, f); }
#endif
#endif
