#include <stdlib.h>
#pragma once
#define ZW_FRAME_K       56L                        /* RUNG ZW-12: the canonical MATCH frame's own sub rsp,K -- ≤64 range; slots [rbp-16]..[rbp-64] after ZW-15 (old_rbp at [rbp-8]; lea rbp,[rbp+8] in begin raises rbp to claim_base without changing rsp -- ZW_FRAME_K is the actual sub rsp,K bytes, unchanged) */
#define ZW_FRAME_TOTAL   64L                        /* ZW_FRAME_K + the push rbp qword -- the EXACT bytes zd_plan inserts between head and END; emitted and model read THIS one name.  lea rbp,[rbp+8] is a register-only op and does NOT change rsp -- ZW_FRAME_TOTAL is unchanged at 64. */
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
extern void          (*g_emit_chain_posthook)(void);   /* RTX-FUNC-0: one-shot callback fired inside emit_chain's binary session; set before the main-graph emit_chain call, cleared on fire */
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
void drive_arg_slots_reserve (int n);   /* op_arg_slot heap growth (int*+cap since 599601e); callers fill then set op_arg_slot_n */
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
    int                          op_mon_stmt_tap;
    int                          op_bounded;
    int                          op_relop_descr;
    int                          op_num_real;
    int                          op_imm_a_ok;
    int                          op_imm_b_ok;
    long                         op_imm_a;
    long                         op_imm_b;
    int                          op_write_route;
    int                          op_call_route;
    int                          op_ab_nformals;   /* AB-3b: nformals from IR_FUNC_ACTIVATE node (IR_LIT(nd).ival set by lowerer); formals are save-set slots [1..nformals], locals [nformals+1..nsave-1] */
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
    int                          op_tail_fpr_rsp;  /* N02-FIX: actual rsp-push depth of right-spine nodes [b1+1,r1) -- fct_rsp_range (zd_k sum), NOT fct_fp_range (fc_geom).  Right-spine kinds (LIT_INTEGER etc.) push rsp cells before gamma; when they fail, their cells stay on the stack and beta arrives at element_base-fpr_rsp not element_base.  Beta pops fpr_rsp before reading element header.  N01 (empty right-spine) fpr_rsp=0, byte-identical.  APPENDED AT STRUCT END per s141 ABI law. */
    int                          op_body_has_arbno; /* SEQ-ERAD s9 nested-ARBNO view-restore gate: 1 = this ARBNO's body subgraph contains at least one nested IR_MATCH_ARBNO — its β will clobber zv()=rbp (the element view register), so the chain arm must unconditionally restore the view at PAIR(2)/PAIR(3) via lea rbp,[rsp+(24-op_sa)].  0 = body is arbno-free; the restore is still emitted (a no-op) but gated out for byte-identical behaviour vs the pre-fix baseline. */
    const char *                 op_src;           /* SRC-COMMENT (2026-07-26): verbatim source text for a statement, keyed in the bb_src registry (lower_common.c) to the statement's SURVIVING head box (the anchor IR_GOTOs are threaded away by the optimizer's branch_chain/dead_goto, so sno_build_graph's post-pass chases each anchor's γ to the first non-goto and notes THAT node); looked up at the dispatch fill for every node, NULL on miss and under SCRIP_SRC_COMMENT=0, emitted there as one TEXT-only x86("comment") per source line — the comment encoder's BINARY arm is empty by construction, so mode-3 bytes are untouched.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_res_live;      /* ZB-VAL-8b USE PREDICATE (Lon directive s182: "allocate its RESULT value, IF it has one and if it is used"): 1 = some consumer reads this node's result cell as a VALUE (an operands[] edge); 0 = MEASURED dead -- every reference to it is a γ/ω CONTROL wire, so the box may skip WRITING the cell as well as owning it.  Promoted from zls_result_live() beside the operand slots it qualifies; DEFAULTS to 1 so any node the analysis never saw keeps writing.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          flat_deep_arrival; /* FLATDISP-5 (Lon directive s192: "continue until you hit a BRICK WALL and realize, oh I need a RBP stable base pointer"): 1 = some γ/ω arrival in THIS graph can land at a depth BELOW the activation base, so the frame-base restore must be rbp-ABSOLUTE (the depth-immune `mov rsp,rbp` / `lea rsp,[rbp+kt]` form) and the caller-rbp header save/seed must be emitted.  0 = DEPTH-STATIC: every arrival lands with rsp == base by LIFO balance, the frame-pointer dance is pure ceremony, and the whole rbp quartet (save/seed/teardown/restore) is DELETED.  Named by WHAT DIFFERS, never by language (RULES.md NO-LANGUAGE-SENTINEL): the property is "can an arrival be deep?", which is a function of the CONSTRUCTS present — the generator/suspending family (IR_SCAN_*, IR_TO*, IR_LIMIT, IR_REPALT, IR_PROC_GEN, IR_CREATE, IR_ITERATE, IR_DISJUNCTION) can `return` from inside nested scan/generator depth, the determinate family cannot.  xa_flat.cpp's own line-448 comment is the provenance: the rsp-relative reads were retired because that assumption is "true for SNOBOL4's determinate procs, FALSE for Icon" — this field restores the cheap form for exactly the graphs where it was always sound.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          flat_fb_refine;   /* FB-STMT (Lon directive, 2026-07-29 "Change every RBP to RSP that can be. I want only the housekeeping data indexed by RBP"): 1 = this pinned graph's deep kinds are ALL statement-bracketed (DEFER/ARBNO/FENCE1/VALUE inside a HEAD..RELEASE/REPLACE range, whose exits rebalance rsp through the head snapshot — the 058 ground-truth law the census comment records), so per-node fb refinement is licensed: nodes OUTSIDE deep match statements arrive at static depth and speak rsp+op_flat_disp (the FLATDISP-9-corrected machinery, proven by the live unpinned regime), while deep-statement residents and every match-family box keep the pinned rbp.  0 = whole-graph rbp exactly as before (flat_pat / flat_gen / gen-proc / non-bracketed deep kinds — ABORT and CALLOUT excluded because the abort seal path skips the ω-choke rebalance, per the 3ea60fc2 pat_seal note).  Ceremony, record protocol and epilogues read emit_jmp_pin_rbp/emit_rec_pin UNCHANGED — this field refines DATA SPELLING only, so mixed spellings address the same bytes by construction (rbp==base, static reader rsp==base−live, disp compensates).  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_fb_rbp;        /* FB-STMT per-node twin, set at both promotion sites from the emit_chain map: 1 = this node's FR/FRQ/FRQB refs speak the pinned rbp base; 0 = statically-arriving node, speak rsp+op_flat_disp.  Consulted only when flat_fb_refine=1 and SCRIP_FB_STMT != 0.  Unmapped nodes (foreign drivers) default 1 — conservative.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          flat_stmt_frame;  /* STMT-FRAME REGIME (s21x-c design of record, Lon: per-BB one-instruction carves; STATEMENT bracket = the rbp dance; NO graph carve, NO zero-fill, NO anchor): 1 = this graph passed emit_stmt_frame_scan under SCRIP_STMT_FRAME=1 -- the ALL-OR-NOTHING per-graph switch (mixed regimes inside one graph are the s188 disease).  Consumers: xa_flat prologue/epilogue (pad-only 8B arms), the drive loop's per-statement head stubs (bb_glue_framed_enter at every bb_src_of head, GLUE-4) and the two chain exit cuts (bb_glue_framed_leave glue in front of lbl_γ/lbl_ω), plus the head/port retargets beside the SPD-2 scanhit redirect.  0 = legacy regime verbatim.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          flat_all_zd;      /* LP-1 LAYOUT PASS (s22c NEXT(1)): 1 = every chain node is ZD-armed (zd_on[i]==1 for all i in the drive-loop nodes[] array); the whole-graph jcon_value_region carve is then UNUSED -- every slot is a per-BB ζ cell that self-allocates at α and self-releases at γ/ω -- and xaf_outer_frame_k() may return 8 (the alignment-only pad) instead of jcon_value_region+8.  Set by codegen_flat_chain_body after the RPO walk + zd_plan run, before the prologue emits (the pre-prologue node scan); cleared at the emit_chain choke.  Non-jmp-entry RSP main graphs only -- jmp-entry blobs, flat_pat, flat_gen keep legacy verbatim (their protocols read the pinned whole-graph frame).  THE ORACLE: arithmetic.sno -- max displacement 40 against a 248-byte carve; with flat_all_zd=1, xaf_outer_frame_k returns 8 and the carve vanishes, a correct authority that licenses a 0-byte region.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_subj_cell;     /* SUBJECT-CELL rung (a) (Lon s21x directive "NO FRAME RELATIVE addressing for operands"): 1 = IR_MATCH_BEGIN's subject rides the FORTH cell -- the subject producer chain is fvl/fvb-registered (zls_build SUBJECT loop, env-gated SCRIP_STMT_FRAME+SCRIP_SUBJ_CELL) and the head POPS the 16B subject DESCR from TOS ([rsp+0]/[rsp+8], add rsp,16) at the very top of alpha, BEFORE any flat-spelled housekeeping access, so every downstream op_flat_disp prefix (fc_leaf_walk's D=32+prefix math) is byte-identical to the flat arm; 0 = legacy FRQ(op_sa) flat-slot read.  Promoted at the IR_MATCH_BEGIN drive case from fc_vread_fp(nd) >= 0 under ZC_FRAME_RSP + ZC_PORT_FORTH -- planner and emitter agree via the ONE registry (the fc_call_ok two-env precedent). */
    int                          zop_seen;         /* ZOP-1 MIX AUDIT (Lon s21x-n "parameterize the access to operands via the FOUR modes"): bitmask of the operand REGIMES x86_zop resolved to while emitting the current graph -- bit1 r12 island frame, bit2 own per-BB rsp cell, bit3 pinned rbp whole-graph frame, bit4 rsp whole-graph frame + op_flat_disp.  Bit 2 composes with exactly ONE of {1,3,4} by design (the per-BB cell is a window INTO a regime, not a regime); two or more of {1,3,4} set on one graph IS the s188 mixed-regime shape that zeta_choices.h declares must be unrepresentable, and which stayed silently representable for as long as the five deciding booleans were spread across three accessors.  Reset per graph beside the other flat_* fields; read by the ZOP audit and by test_gate_zop_one_regime.sh.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_zdepth;        /* ZTOS-1 (Lon s21x-o "keep track of sliding offsets and index operands from RSP, not RBP" + "no pre-allocation calculation is necessary"): the bytes THIS box carved on rsp below the incoming value spine, live for the whole of its own body.  Every TOS-relative operand read (ZTOS/ZTOSD) adds it, so a box's own carve and its own operand reads compose to a no-op BY CONSTRUCTION.  THE POINT IS THAT THIS IS LOCAL: the compensation a box owes is exactly what IT allocated, never a graph-wide prefix sum, which is why no pre-allocation pass is needed and why arming one more BB can no longer displace a neighbour.  This is the field that replaces op_flat_disp's role for spine references — op_flat_disp is LOWER's static answer to a question that became dynamic the moment every BB carved for itself (the s21x-n "depth displacement" negative result; 028_arith_unary_minus printed 5 for -5 because unop's own sub rsp,16 slid its operand out from under a hardcoded [rsp+0]).  0 = this box carved nothing, so its reads are already at the spine.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_zres;          /* ZD-1 (Lon s21x-v "Have each BB allocate its RESULT value ... Parameterize the access to operands via the FOUR modes"): THIS node rides the ZD regime — its result lives in its OWN alpha-carved rsp cell (suspended through gamma per S10c), its operand reads are the staged op_zread differences, and the port hook applies the suspended discipline (alpha carves; beta does NOT re-carve; gamma does NOT release; omega releases own K + op_wpop).  Driver-computed by zd_plan, the ONE execution-order walk; templates key their ZD arm on this and nothing else.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_zgpop;         /* ZD-1: statement-terminal gamma release — the bytes of ZD cells live at this node's successful exit when its gamma edge crosses a statement boundary (target is a bb_src_of head or the chain exit).  The hook emits ONE add rsp,K before the gamma jmp; 0 = intra-statement edge, nothing to release.  This is the success-path twin of op_wpop and is what makes scoping STATEMENT-LEVEL: rsp returns to statement entry on every cross-statement edge, so no graph-wide depth ever accumulates and LOWER's static readers in later statements are undisturbed.  APPENDED AT STRUCT END. */
    int                          op_zread[6];      /* ZD-1 THE DIFFERENCE OF TWO DEPTHS (s21x-u's law made mechanism): op_zread[k] = (delta_at_read(THIS) - delta_out(producer_k)) * 16 in bytes — the rsp displacement at which operand k's cell sits when THIS box's body reads it, computed by zd_plan from the execution-order walk.  A single depth cannot express this (two consumers at different depths reading one producer need different constants); the STAGED PER-CONSUMER-PER-OPERAND difference is the invariant.  Templates reach it only through ZOPQ/ZOPD (x86_asm.h), never rsp.  APPENDED AT STRUCT END. */
    int                          op_pair_rejoin;   /* ALT-NARY REJOIN (s22j): 1 = THIS node's PAIR label defines are INTERNAL REJOIN POINTS within a single traversal, not alternative box entries, so the carve-only bracket must NOT treat them as beta entries.  The bracket (x86_asm.h: both entries allocate, both exits free) is rsp-neutral only for a box entered through exactly ONE of {alpha,beta} and left through exactly ONE of {gamma,omega}.  IR_MATCH_ALTERNATE violates that: one pass runs alpha -> arm -> sigma stub -> merge, carving three times and freeing once (measured -64 on 'aaaxx' ? ('aaa'|'bbb')), after which bb_match_end read bb_match_begin's saved-rsp unwind base at the wrong depth and segvd -- every alternation in the corpus.  ⛔ THIS IS NOT UNIVERSAL TO N-ARY BOXES AND MUST NOT BE WIDENED BY SHAPE: IR_MATCH_ARBNO's pair entries ARE genuine per-iteration claims (the rsp linked-frame chain named in ARCH-SNOBOL4.md), and setting this for ARBNO regressed 065_capture_then_arbno + 164_pat_arbno_nested -- MEASURED, both approaches, do not retry on the argument that ALT and ARBNO are the same shape.  Kind dispatch lives HERE in emit.cpp's choke, never in x86_asm.h, which owns encodings only.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_wsteal;        /* END-JMP STEAL (HQ 2026-08-03): 1 = this node's ω was retargeted to its statement's IR_STATEMENT_END (the SOLE release authority, WHACK CONTRACT item 4) -- the X86H_JMP/OMEGA own-K flat_leave is suppressed; wpop is suppressed at staging exactly like the ZW-5 steal.  Per-node, cleared unconditionally at the choke.  APPENDED AT STRUCT END. */
    int                          op_zkind[6];      /* OBJ-NOTE ON-1 (s23e): the IR op of operand k's PRODUCER, staged in lockstep with op_zread[k] by the SAME zd_plan walk (the loop already holds the producer node to compute the depth difference, so this is a read of a pointer it had in hand, not a second resolution).  Templates spell it ONLY through ZOPN(k) (x86_asm.h), which renders bb_kind_name -- the same lowercase spelling as the n<uid>_<kind> labels -- so an operand read prints '[rsp+96]  # binop_arith' and names WHOSE cell arrived.  This retires the ON-2 operand-a-only interim (ZOPAN/op_a_node_kind) for operands b..n.  ANNOTATION-ONLY BY CONSTRUCTION: the sole consumer is x86("note",...), which is the empty string under MEDIUM_BINARY, so no emitted byte in either medium can depend on it.  APPENDED AT STRUCT END per the s141 ABI law (mid-struct insertion shifts baked g_emit offsets -- see op_arbno_zq). */
    int                          flat_outer_nparams; /* SHED-3 (s25a): per-graph outer nparams copied from g_flat_outer_nparams at the emit_chain choke; 0 = depth-static main (stmtframe candidate); >=1 = param-bearing DEFINE (ICNBENCH-ARGS-RSP, params at [rbp+16+8k]).  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          flat_lcl_proc;    /* ICN-PROC-FRAME (s211): jmp-entry graph with lexical locals/params requiring rbp seed + g_call_args install. Named for WHAT it decides (depth-immune-base local frame), never for a language. APPENDED AT STRUCT END per s141 ABI law. */
    int                          op_arbno_framed;  /* ⭐ ARBNO-LON (Lon rulings 2026-08-06, superseding the 48B-frame draft the same day): 1 = THIS IR_MATCH_ARBNO is NESTED inside another ARBNO's body span -- the tier whose header quads at static FR(op_off..) offsets MEASURABLY ALIAS across enclosing iterations (X02 gdb: iteration-2's inner header at phys == iteration-1's live outer element view; the s10 rsp=0x3 class, all 5 open SIGSEGVs).  Lon's resolution is FRAMELESS: ARBNO needs no frame, no counter, no element links, no view register, and no chain that is ever traversed -- growth is released wholesale at MATCH_END / FENCE commit, never by ARBNO itself.  The construct's whole state is ONE 16B alpha-carved rsp cell {DELTA0 dword, yield-cursor dword}; the sigma junction is cmp r14d,yield / je resume; the exhaust junction is cmp r14d,DELTA0 -- delta==DELTA0 iff nothing committed (sigma guarantees every committed instance advanced), and delta arrives at omega already restored by the un-match cascade, so the omega Delta-assert holds by construction.  Selected with op_arbno_body_k0 below.  0 = top-level, legacy chain arm verbatim.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_arbno_body_k0; /* ⭐ ARBNO-LON companion gate: 1 = every body-span member has zd_k()==0 (pure scanner-register matchers: LIT/LEN/ANY/NOTANY/POS/RPOS/FENCE1/ALT-envelope...), so the ζ frontier NEVER moves inside this ARBNO's activation and the one 16B cell is at [rsp+0]/[rsp+4] from EVERY port site -- alpha, sigma junction, beta, exhaust -- with zero depth arithmetic.  Bodies with K>0 members (ASSIGN_SAVE cells, DEFER records) keep the legacy arm this rung; their per-site static-depth compensation is the named next rung (the X11 class).  Computed by the containment scan from THE ONE K AUTHORITY zd_k (s22k law -- never a second spelling of the K=0 list).  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_arbno_body_kk; /* ⭐ ARB-LON-K16 (this rung): staged body-span ΣK (sum of zd_k over members) for the SEQUENCE-ONLY STATIC-K body class -- >0 admits the widened frameless arm (bb_match_arbno_frameless_k) whose one 16B cell is RE-HOMED to the frontier at each σ commit, so every read site is a compile-time offset: σ reads the previous cell at [rsp+kk] (one level in, the op_zread depth-difference shape), exhaust/retract read [rsp+0], retract discards the current copy (add rsp,16) landing exactly on the last instance's body frontier for PAIR(1).  Committed growth (kk+16 per instance) is NON-POPPING and dies at the bracket whacks (MATCH_END absolute restore / FENCE commit / STATEMENT_END), per THE MODEL.  Staged 0 (arm declines to legacy) when the body contains any container/transfer kind (ALTERNATE/ARBNO/FENCE1/DEFER/PATREF/VALUE/CALL/CALL_VALUE/DISJUNCTION/ABORT) whose frontier contribution at body-γ is path-dependent or unbounded, or when body_k0 (the in-place arm owns that class).  Summed from THE ONE K AUTHORITY zd_k in the SAME containment scan as body_k0 (s22k law).  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_stmt_dyn;      /* ⭐ ARB-LON-K16 DYNAMIC-EXTENT STATEMENT (this rung, the N03/N04 root cause): 1 = THIS statement-terminal node closes a statement containing a FRAMELESS_K-armed ARBNO, whose committed growth (kk+16 per instance) makes the statement extent COMPILE-TIME INDETERMINABLE -- the γ release must be mechanism-2 (THE MODEL clause 4: frame-pop when not determinable), spelled off the EXISTING stmt_base discipline: `lea rsp,[rbp+op_zgpop]` (rbp == claim base, proven by the [rbp+op_stmt_pin] old_rbp reads) restores the pre-claim frontier at ANY dynamic depth, then the old_rbp restore.  The static `add rsp,K` under-frees by exactly the growth and skews every later statement (measured: N04 correct output then main_γ pop-rbp on rsp=0).  Staged at the ZD-1 choke from the per-statement latch the ARBNO prelude sets; consumed and cleared at STATEMENT_END/STATEMENT.  0 = legacy add verbatim (killswitch-inert: SCRIP_ARBNO_K16=0 never sets the latch).  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          zframe_graph;     /* ICN-FR-2: 1 = this graph is an Icon ζ-frame graph (lower_icon.c set IR_graph_t.zframe_graph; copied to g_emit at the emit_chain choke, cleared by emit_jmp_entry_clear).  Gates the whole-graph ζ-frame prologue+epilogue arm: sub rsp,kt + wire header [kt-24]=γ [kt-16]=ω [kt-8]=caller_rbp + pin rbp=rsp.  Killswitch SCRIP_ICN_ZFRAME=0 leaves lower_icon.c's field at 0 so this never fires → pre-FR-2 HEAD bytes identically reproduced.  APPENDED AT STRUCT END per the s141 ABI law. */
    long                         op_suspend_stmt_uclaim; /* ICN-FR-5 SUSPEND-DO RECARVE: the enclosing statement's total uclaim (zd_uk[head_i]) staged by the drive loop when nodes[i]->op==IR_SUSPEND and lbl_t1_p (do-body present) and zframe_graph.  The γ-exit from bb_suspend frees this claim (add rsp, uclaim via X86H_JMP/X86P_GAMMA hook + bb_glue_flat_leave); the β re-entry must sub rsp back before dispatching to the do-body so the while-loop's next iteration sees the correct rsp and the matching add rsp at n_suspend_α does not drift the stack.  0 when no do-body or not zframe_graph (byte-identical).  Cleared at the per-node unconditional clear (node-scoped: each IR_SUSPEND gets its own value; plain suspend-without-body has lbl_t0=NULL, value unused).  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_ztail;
    int                          op_zpat;          /* C-9 PAT-CARVE: bytes of dynamic zeta cells carved by PATTERN-INTERIOR match primitives (TAB/RTAB/POS/RPOS -- they pop only on their FAIL edges, so on the success path the carve is still live at the splice). Staged at the choke for IR_MATCH_REPLACE only; 0 for every other kind. SUBTRACTED by bb_match_replace head reads, never folded into op_zdepth: the value-spine operand cells (lit_integer) are already absorbed by op_sa flat-slot drift, so a shared term double-adds them -- the FRQ/FRQB double-add class. MEASURED linear: correct end sits at caller [rsp+88-16N] for N carving primitives. ONE AUTHORITY: the drive-loop staging site writes it, bb_match_replace reads it. */         /* ⭐ ZK-2 IR_TO BETA-RESUME DEPTH FIX (this rung): for IR_TO on the cells arm, the bytes of cells stacked ABOVE IR_TO's own K=32 block by downstream run members that fire between γ (suspend) and β (resume).  At α the counter/limit cells sit at ZLOC(0)=[rsp+16] and ZLOC(8)=[rsp+24], which is correct: IR_TO's sub rsp,32 has just fired and no downstream carves have accumulated yet.  At β (second+ iterations), downstream nodes (IR_BINOP, IR_ASSIGN, IR_CALL_BUILTIN_ICON, etc.) have each fired their own sub rsp,K, moving RSP further down by op_ztail bytes.  ZLOC(0)=[rsp+16] then names a cell op_ztail bytes BELOW the counter — e.g. with one K=16 downstream node (BINOP), ZLOC(0) reads IR_TO's RESULT cell instead of the counter, producing silent wrong output (every total:=total+(1 to n) printed 4 instead of 15 for n=5).  FIX: at β-body sites (after x86_beta(), including the loop head re-entered via jmp), use ZLOC_B(o)=[rsp+op_ztail+16+o] instead of ZLOC(o)=[rsp+16+o].  Staged as (zd_out[terminal] - zd_out[IR_TO]) = total K of all nodes after IR_TO in the run.  0 for IR_TO at the run terminal (no downstream members — byte-identical to ZLOC).  Killswitch SCRIP_ZD_ICN_TO=0 makes bb_to take the non-ZD arm, byte-identical.  ONE AUTHORITY: only this field and ZLOC_B in x86_asm.h name this displacement; zd_k's K=32 ONE-LINE is the carve authority.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_cap_anchor;    /* ⭐ OS-2·SLICE-1 IMM/COND ANCHOR (GOAL-SN4-ZETA-MECH LADDER ONE-SYSTEM): 1 = this capture-family node (SAVE/COND/IMM) routes its δ cell through THE ONE OFFSET FUNCTION (FR(op_off): pinned rbp = depth-immune; unpinned depth-static rsp arm = consistent by the same static math both ends) instead of the ZB-FC-3c rsp fc-cell + fc_cond_fp cross-depth constant, which is emit-time distance prediction across a dynamic extent and aliases foreign cells the moment the operand contains DEFER (runtime-minted blob carves + the m4 defer continuation push are not in the static count — the X/Y/Z clobber, witnesses dc_nest2/dc_nest_bt/dc_recur/os1_runtime_k).  PRODUCER SYMMETRIC: SAVE's write and COND/IMM's read stage the SAME bit from the SAME scan, so the ends cannot desync (the CAP-SYM lesson).  Staged 0 (legacy verbatim) when the node sits inside an IR_MATCH_ARBNO body span — multi-instance suspension re-enters the SAME static node, so a per-node frame slot is per-activation only OUTSIDE ARBNO (interior = SLICE-2/3's customer, the ARBNO-K16 re-homing class) — or under killswitch SCRIP_OS_CAP=0 (byte-identical revert; env read at the ONE routing authority in emit.cpp, template keys purely on this field).  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_zfc;           /* ⭐ L-3b NON-CARVING SPLICE FIX (this session, FINDING-2026-08-12h): the FC-window displacement IR_MATCH_END's WRITE side actually used (fc_head_fp(head), the SAME value read at emit.cpp's IR_MATCH_END case via op_fc_disp -- confirmed by direct instrument, not inferred) -- op_fc_disp itself is NOT available at IR_MATCH_REPLACE (resets to -1 per node, per the shared DRIVE_FILL reset), so this field re-stages the identical fc_head_fp(head) lookup at the SAME choke g_zd_zpat already uses, for IR_MATCH_REPLACE only.  ROOT CAUSE (measured, both instrument and address arithmetic): bb_match_end.cpp's rfc()-gated write (ZC_FRAME_RSP arm) stores head.cursor/head.end at [rsp + op_off + op_fc_disp + 32] -- the FC-window rebase PLUS a +32 fixed base neither of which bb_match_replace.cpp's FR(op_off)/FRQ(op_off+24) reads ever included.  op_zpat (C-9 PAT-CARVE, the pre-existing field) is DOCUMENTED for a DIFFERENT class (TAB/RTAB/POS/RPOS carving primitives, per its own comment) and is not proven equal to fc_head_fp in general -- op_zfc is intentionally separate, not a rename, so the carving class's own (still-open, still-owned-by-a-later-rung) fix is not silently coupled to this one.  ADDED (not subtracted): the reader must reach the SAME address the writer used, i.e. op_off + op_zfc + 32, mirroring the writer's formula exactly rather than approximating it.  Staged 0 for every kind other than IR_MATCH_REPLACE (byte-identical elsewhere).  Killswitch: none yet -- this is a new mechanism, not a widened existing one; SCRIP_ZD=0 disables the whole ZD arm as the blanket revert.  APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_arbno_body_defer_unsafe; /* W-7 INTERIM GUARD (HOME-WIRES, s38): companion scan over the SAME body span as op_arbno_body_k0 -- 1 = at least one body member is IR_MATCH_DEFER with pat_static==0 (a `*X` whose target X can transitively recurse back through a DEFER, manual p.122's own recursive-LIST idiom: `ITEM = SPAN(...) | *LIST` inside `ARBNO(',' ITEM)`).  D12/D13 are this class: the plain-frameless arm's [rsp+4] yield-cursor write assumes the ζ frontier never moves inside the activation, but a recursive DEFER can suspend/resume through an unbounded nested depth, stomping a live CLASS-D resume-record landing address (gdb-confirmed, FINDING-2026-08-12k).  pat_static==1 DEFERs (write-once-frozen, transitively defer-free -- the 16-probe passer set D09/D10/D11/G19/G20/H21/H24/H25/N12/N17/X02/X03/X04/X05/X06/X11) are NOT flagged: they cannot re-enter a blob, so the frameless arm's assumption holds for them.  Consumed by bb_match_arbno() to decline with x86_bomb instead of silently emitting the unsound arm -- converts a memory-corruption SIGSEGV/hang into a loud compile-time refusal, floor-neutral (flips 0 of the 16 passers, breaks 0).  Real fix needs W-4's arena layout (anchor-relative ARBNO cell); this is the interim honesty patch. APPENDED AT STRUCT END per the s141 ABI law. */
    int                          op_frame_need;    /* ⭐ EARN-1 SLICE 1 (GOAL-RBP-EARN s47, DORMANT -- NOTHING READS THIS YET): frame_need_of(nd) verdict staged at the ONE choke (bb_prepare), per-node MATCH FAMILY only -- graph-level licenses (STATEMENT/FUNCTION/zframe/heap-adopt) stay with emit_jmp_pin_rbp/emit_heap_fb_adopt until EARN-11 merges authorities.  1 = this node EARNS a frame under THE LAW (cell-to-RSP distance not a compile-time constant at a reading site): ARBNO = 1 ALWAYS (own control cell live across its own unbounded retries, EARN Rule 1); FENCE1 = 1 (licensed today, success-edge commit whack per s28 ruling 3; EARN-6 refines to IFF-OPAQUE-SIBLING); capture family ASSIGN_IMM/COND/SAVE = earn_hazard_in operand-walk PLACEHOLDER (the REAL span authority is EARN-5's -- reuse the op_cap_anchor producer scan; γ/ω-wired spans are invisible to an operand walk, so conditional rows under-detect toward 0, graded by EARN-2's census); everything else 0 (ALT stays 0: failure-edge restore, s25 sharpening).  Hazard = OPAQUE (IR_MATCH_DEFER with pat_static==0 -- the calloc default also covers the s29 BINOP dropped-operand trap conservatively -- and IR_MATCH_VALUE, the runtime-pattern-value DEFER clone) or ARBNO (unknown COUNT is a SOURCE of unknown SIZE, one class per s28).  Diagnostic column for EARN-2: SCRIP_EARN_DIAG=1 stderr line per prepared node, default OFF so emission stays byte-identical (gate proven by full A/B .s sweep at landing).  Valid only where bb_prepare ran, same law as bb_ls/bb_rs.  APPENDED AT STRUCT END per the s141 ABI law. */
} sm_emit_t;
extern sm_emit_t g_emit;
static inline int emit_patpin_probe(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PROBE_PATPIN"); v = (e && *e == '1') ? 1 : 0; } return v; }   /* ⛔ DIAGNOSTIC PROBE ONLY, DEFAULT OFF, NEVER A LANDING: re-admits flat_pat to the pin so the pre-DEL-T1 blob frame can be measured against HEAD in one tree.  Lon forbade the restore; this exists to SIZE the remaining D-2 debt, not to pay it. */
static inline int emit_jmp_pin_rbp(void) { return g_emit.flat_deep_arrival || g_emit.flat_gen || g_emit.flat_lcl_proc || g_emit.zframe_graph || (emit_patpin_probe() && g_emit.flat_pat); }   /* ⛔ DEL-T1 D-2 (s9): flat_pat REMOVED — NO RBP frame for constant-folded patterns (Lon).  The load-bearing reason the s194 comment below records (suspending γ retains at the deep frontier and the exit protocol read the pinned rbp) is discharged by g_zctx: the CLASS D γ/ω glue, res-stub, and scanhit/scanfail now read the depth-immune runtime cell, and the record's [+8] payload is the CELL BASE.  Interior data refs flip to the rsp+op_flat_disp arm through the ONE selector; disp correctness across the match family in blob context is the PT-4 PREREQ frontier — witness-gated, and this one-liner is the independent revert if the frontier bites. */   /* ICN-FR-2: zframe_graph added — ζ-frame prologue saves caller-rbp at [rsp+kt-8] and seeds rbp=rsp; the pin is the layout contract (THE PIN IS A LAYOUT CONTRACT, NOT A SPELLING SWITCH — the ZW-1 lesson applies here too: kt is sized to include the wire header, so the save lands correctly only when pin is set, and the epilogue reads through pinned rbp).   ZW-1 s21x-m REVERTED ARM (kept as a law, do not retry blind): the universal carve tempted a `SCRIP_BB_ALLOC || ...` disjunction here to make flat refs depth-immune (the 023 intra-box displacement).  MEASURED SEGV with ZERO BBs armed -- the pin is not free: it gates the xa_flat prologue's caller-rbp save at [rsp+kt-8] + seed, and kt is sized by a layout pass that assumed NO pin for these graphs, so pinning a never-pinned graph writes the save outside its own header and returns on a clobbered rbp.  THE PIN IS A LAYOUT CONTRACT, NOT A SPELLING SWITCH.  The intra-box displacement it was meant to cure belongs to op_flat_disp (the running rsp-depth prefix sum, LOWER's own instrument) or to window migration, never to a blind pin flip.  ICN-PROC-FRAME (s211): flat_lcl_proc added -- the prologue establishes rbp=rsp, so FRQ/FR MUST use rbp-relative addressing to be depth-immune for lexical local/param access at [rbp-N]. */
static inline int emit_heap_fb_adopt(void) { extern int g_gen_proc_active; extern int g_resumable_callable_active; return g_gen_proc_active || g_resumable_callable_active; }   /* ZETA-FB-3 (s161): THE SECOND PROLOGUE ARM, NAMED.  Two prologue shapes establish rbp as the frame base: (1) xa_flat's jmp-entry hdr SAVES the caller rbp and SEEDS rbp=rsp at the activation's flat base, gated emit_jmp_pin_rbp(); (2) THIS one ADOPTS a frame that arrives in rdi already allocated on the heap (`push rbp; mov rbp,rdi`), carving NO rsp frame at all — the activation's frame must survive beta-resume, so it cannot live on a stack that unwinds.  Arm (2) was spelled as the raw `g_gen_proc_active || g_resumable_callable_active` pair at SIX xa_flat sites (4 prologue incl. both media, 2 epilogue) plus once inside emit_rec_pin — SEVEN copies of one condition, the duplication class that put s158's land mine in the tree: edit one copy, the store and the load name different base registers.  ZETA-FB-2 unified the CONSUMERS (x86_fb_pinned == emit_rec_pin); this names the PRODUCER so the union below is written ONCE as the disjunction of two named arms and every site reads a predicate instead of re-deriving one.  Behaviourally identical by construction — same boolean, one name; MEASURED byte-identical over 184 .s artifacts across three frontends. */
static inline int emit_rec_pin(void) { return emit_jmp_pin_rbp() || emit_heap_fb_adopt(); }   /* REG-7 U5 RECORD-PROTOCOL PIN (s158): emit_jmp_pin_rbp covers the FLAT jmp-entry regime only (deep arrival / pat / gen blobs).  A gen-proc or resumable-callable activation carves NO rsp frame at all -- its base is the HEAP frame in rbp (heap-fb-as-rbp, GOAL-PROLOG-BB JOB #1) -- so it needs the record protocol on rbp for exactly the same reason, and the beta dispatch below ALREADY gates on this pair.  Keeping the two predicates apart is what let the store and the load pick different base registers. */
static inline const char * emit_rec_fb(void)     { return emit_rec_pin() ? "rbp" : "rsp"; }   /* THE one record-protocol frame base: the 16B suspend record's [+8] payload is the SUSPENDED activation's frame base, and the resume-slot STORE, the res-landing POP and the beta DISPATCH must all name the SAME register or they drift.  Not x86_fb(), which s189 collapsed to the constant "rsp" after s188 compensated the ordinary flat refs -- correct for a flat activation (base == rsp), fatal for a heap-fb one, where `pop rsp` loads a heap base into the stack pointer and the next indirect jmp lands wild (the 44-test backtrack SIGSEGV). */
static inline int          emit_rec_fb_num(void) { return emit_rec_pin() ? 5 : 4; }           /* BINARY twin of emit_rec_fb -- rbp=5 / rsp=4.  R10: the two media MUST pick the same register or the mode-3 slab and the mode-4 .s diverge at the resume landing. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* emit_rec_rsp_arm — ⭐ DEL-T1 D-2: THE ORPHANED CLASS, NAMED, AND THE ONE AUTHORITY THAT DECIDES IT (Lon: "proper RSP FORTH-style stack, no RBP frames; RESTORE NOTHING").  DEL-T1 dropped flat_pat from   */
/* emit_jmp_pin_rbp(), so a PAT$ blob's prologue establishes NO rbp — MEASURED on the artifact, not inferred: proc_PAT$0_α is `sub rsp,kt` + wire/δ saves + the g_zctx cell push, carrying neither `push rbp` */
/* nor the heap-adopt `mov rbp,rdi`.  But lower_snobol4.c:2408/2631 sets resumable_callable=1 on EVERY patproc and RT recipe graph, so emit_heap_fb_adopt() keeps emit_rec_pin() true and the blob still      */
/* ADDRESSES THROUGH AN RBP NOBODY ESTABLISHED — exactly four refs per blob: the resume-slot store + β dispatch (emit.cpp record sites, via emit_rec_fb) and the capture δ save + read (FR(), via             */
/* x86_fb_pinned).  Whatever the INVOKER left in rbp is what they hit.  Under a MATCH_BEGIN statement head that is survivable (claws5 passes, it is defer-free); inside an ARBNO body it is not, and the       */
/* collision is LITERAL: the enclosing ARBNO stores its own frontier with `mov [rbp+80], rsp` at the SAME offset the blob writes its resume continuation to.  That is the 15-of-16 demo casualty set and the  */
/* "leaks the ARBNO view into the invoker" mechanism, made concrete.  ⛔ THIS PREDICATE MUST REMAIN THE SOLE DECIDER: all four refs read it, so the store and the load cannot pick different base registers —  */
/* the s158 land mine this file convicts over and over.  Scope is exactly the orphaned class (flat_pat with no pin); a GENUINE heap-fb activation (gen-proc, Prolog resumable) is untouched, because there    */
/* the frame really does live in rbp and `pop rsp` of a heap base is the 44-test backtrack SIGSEGV.  KILLSWITCH SCRIP_REC_RSP=0 restores the rbp spelling byte-identically (proved by md5 over the artifact). */
static inline int emit_rec_rsp_arm(void) { static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_REC_RSP"); on = (e && *e == '0') ? 0 : 1; } return on && g_emit.flat_pat && !emit_jmp_pin_rbp(); }
#define REC_SLOT_FROM_CELL (-24)   /* resume slot relative to the activation cell pointer (g_zctx[1] == base+kt-40): MEASURED INVARIANT across every emitted blob — kt 144/128/176 → slot 80/64/112, cell 104/88/136, difference -24 in all three.  β addresses the slot off this cell because rsp at β is the DEEP frontier (non-popping spine), not the flat base; the cell pointer is depth-immune and res has just popped it into rax. */   /* FLATDISP-7 (s194): THE one jmp-entry rbp condition — every jmp-entry gate site (prologue save+seed, determinate epilogue reads+restore, wire-adopt marshal, BOTH media) reads THIS, never the raw fields, so save/seed/read/restore cannot drift apart (the s193 xaf_deep discipline applied to the jmp-entry regime).  The flat_pat/flat_gen conjuncts are LOAD-BEARING, not conservatism: a SUSPENDING activation's γ RETAINS with rsp at the deep frontier and its resume/exit protocol reads the PINNED rbp by design ([rbp+kt-24/-16/-8], the REG-7 U5 record contract), so the pin is required even when the kind census classifies the graph depth-static — TODAY an INVARIANT PAT$ blob (pure LIT/SPAN body, no fence/arbno/defer) is exactly that case; FLATDISP-5a (inline invariant patterns, no blob) will make the flat_pat conjunct fire less, never wrong.  Determinate graphs (neither conjunct) pin only when an arrival can be deep — the classifier's per-graph answer, language-blind per NO-LANGUAGE-SENTINEL. */
int emit_match_begin_stfh_k(void);   /* ZD-8·STFH-CARVE (FINDING-2026-08-09g): THE ONE AUTHORITY for the MATCH_BEGIN head-carve byte count. Returns 64 when the stfh arm fires (OS-CAP ON + flat_deep_arrival + no jmp_entry/lcl_proc/zframe/pat/gen/replace-family) and the full flat_stmt_frame predicate fires; returns 0 otherwise. Consulted by BOTH bb_match_begin.cpp (to decide whether to emit x86_zclaim(64)) AND zd_k (IR_MATCH_BEGIN arm, to account the carve in K_total so op_zgpop releases the right byte count). ONE AUTHORITY: neither site re-spells the stfh predicate; the spelled-twice disease (MATCH_BEGIN template carves 64, zd_k accounts 0) is unconstructible by design. SCRIP_OS_CAP=0 restores 0 on both sides byte-identically. */
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
