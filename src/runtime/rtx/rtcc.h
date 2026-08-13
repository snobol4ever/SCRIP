/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rtcc.h — RTCC: Global-register liberation block (GOAL-RTCC.md RC-1).                                                                                                                               */
/* THE CONTRACT (BLOCK-CANONICAL LAW): registers are a CACHE valid ONLY inside generated code.  Every                                                                                                 */
/* generated→C crossing = WRITEBACK (regs→block); every C→generated crossing = LOAD (block→regs).                                                                                                    */
/* Default is ON (Lon s13: "Make RTCC=ON ALWAYS"; see rtcc_init.c). With SCRIP_RTCC=0 (emergency bisect only, nothing may depend on it) the block exists but is never written; all crossing macros are no-ops;   */
/* the emitted binary is byte-identical to the pre-RTCC tree (killswitch law).                                                                                                                        */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RTCC_H
#define RTCC_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* RTCC block layout (cache-line-aligned BSS in rtcc_init.c).                                                                                                                                         */
/* SCRATCH TIER (RC-2): R10 R11 R8 R9 — never/rarely used for C-call arg staging; claimable with zero template changes.                                                                              */
/* ARG TIER (RC-4): RAX RCX RDX RSI RDI — SysV argument/return registers; claiming forces arg-staging re-plumb.                                                                                      */
/* XMM TIER (RC-2): XMM8–XMM15 — 8 × 16B = 128B; caller-saved, zero xmm-arg rt calls confirmed by RC-0(c) census.                                                                                  */
/* NOTE: callee-saved set (RBX ___ R12 R13 R14 R15 RSP) is ALREADY SCRIP-owned and NOT stored here.                                                                                                  */
#define RTCC_GPR_COUNT  9           /* RAX RCX RDX RSI RDI R8 R9 R10 R11 — the nine caller-saved GPRs */
#define RTCC_XMM_COUNT  8           /* XMM8–XMM15 */
#define RTCC_GPR_BYTES  (RTCC_GPR_COUNT * 8)       /* 72 bytes */
#define RTCC_XMM_BYTES  (RTCC_XMM_COUNT * 16)      /* 128 bytes */
#define RTCC_BLOCK_BYTES (RTCC_GPR_BYTES + RTCC_XMM_BYTES) /* 200 bytes; padded to 256 for alignment */
/* Slot indices into the 8-byte GPR array.                                                                                                                                                            */
/* ⛔ SEALED ABI — DO NOT #ifndef-GUARD THESE (s16).  The emitter spells the SAME offsets as raw literals in x86_rtcc_wb_bin/x86_rtcc_rl_bin; these macros are only the C half.  Guarding them would   */
/* let -DRTCC_SLOT_R9=7 move the C seed while the literals keep addressing block+48 — PROVEN by probe s16 — giving the H2 SIGSEGV class.  The coupling is enforced by static_assert in x86_asm.h; a     */
/* drift is a BUILD ERROR.  The two KILLSWITCHES below ARE guarded, and correctly so: both halves read them, so a -D reaches the emitted bytes.  KNOB ⇒ guard; ABI ⇒ seal.  The distinction is the rung. */
#define RTCC_SLOT_RAX   0
#define RTCC_SLOT_RCX   1
#define RTCC_SLOT_RDX   2
#define RTCC_SLOT_RSI   3
#define RTCC_SLOT_RDI   4
#define RTCC_SLOT_R8    5
#define RTCC_SLOT_R9    6
#define RTCC_SLOT_R10   7
#define RTCC_SLOT_R11   8
/* XMM slots start at byte offset RTCC_GPR_BYTES from block base (16-byte each) */
/* RTCC_XMM_SLOT(n) for XMM(8+n), n=0..7 */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* RC-5 VM GLOBAL ASSIGNMENTS — one per measured rung; each its own commit on the fixed rail.                                                                                                          */
/* The block slot is the CANONICAL home; the register is a CACHE valid only inside generated code.                                                                                                     */
/* Every C-side write to a claimed global MUST also update the block slot (BLOCK-CANONICAL LAW).                                                                                                       */
/* RC-5 rung 1: R8 = rt_anchor_g (&ANCHOR value, int64_t).  REVERTED 1.000x rail (anchor retry path too cold). */
/*   Infrastructure COMMITTED (block-canonical write companions in keywords.c / core.c; encoder arm wired).    */
/*   Template changes (bb_match_begin/bb_match_advance rtcc_anchor_cmp) REVERTED.                              */
#ifndef RTCC_GLOBAL_R8_ANCHOR
#define RTCC_GLOBAL_R8_ANCHOR   1   /* RC-5-ANCHOR killswitch: 0 = OFF (byte-identical); 1 = ON.  GUARDED (s11) so -DRTCC_GLOBAL_R8_ANCHOR=0 from the build actually overrides -- it did NOT before, and that voided both RC-5 rail numbers. */
#endif
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* RC-5 rung 2: R9 = RT_GVA_VA (GVA island base pointer, constant for process lifetime).                      */
/*   BLOCK-CANONICAL EXCEPTION: RT_GVA_VA is a compile-time constant (pin_va.h); never changes after mmap.   */
/*   Block slot written ONCE at rtcc_init; no C-side companion writes needed.                                  */
/*   Read sites: EVERY GVA variable access — bb_var_global, bb_assign_global, bb_binop_gvar_arith,            */
/*     bb_call, bb_call_proc_staged, bb_save_restore, bb_func_activate (48 ABSQ sites → [r9+k*16+w]).         */
/*   Census: 1038 ABSQ GVA accesses across 21 benchmark files.  disp8 covers k*16 for k≤7 (all roman.sno     */
/*   variables).  Expected encoding win: 7B (ABSQ abs32+REX) → 4B ([r9+disp8]) per access.                   */
/*   SysV arg5 (r9) is confirmed unused by rt calls per RC-0(c) census (zero ≥6-arg rt calls).               */
#ifndef RTCC_GLOBAL_R9_GVA
#define RTCC_GLOBAL_R9_GVA   1   /* RC-5-GVA killswitch: 0 = OFF (byte-identical); 1 = ON.  GUARDED (s11) so -DRTCC_GLOBAL_R9_GVA=0 from the build actually overrides -- it did NOT before, and that voided both RC-5 rail numbers. */
#endif
#define RTCC_GVA_REG            "r9"  /* the register that holds RT_GVA_VA inside generated code.  ⛔ SEALED ABI, NOT A KNOB (s16): the reload encoders hardcode `mov r9,[r11+48]`, so renaming this alone would make GVARQ address a register the veneer never seeds.  static_assert in x86_asm.h enforces it. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* The block itself — declared in rtcc_init.c; extern here for the GC and coexpr paths.                                                                                                              */
/* 256-byte aligned so every slot fits in one or two L1 cache lines.                                                                                                                                  */
extern uint64_t g_rtcc_block[32];   /* 32 × 8B = 256B; XMM slots at [9]..[24] as two uint64 each */
extern unsigned char g_rtcc_on;     /* 1 = ON (default, s13); 0 = OFF (SCRIP_RTCC=0, emergency bisect only, nothing may depend on it) */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GC integration: call at runtime init (done in rtcc_init constructor).                                                                                                                              */
void rtcc_gc_register(void);
/* Coexpr block-swap (Option B ruling, RC-0 FINDING): save/restore full block into scrip_coctx_t.                                                                                                    */
/* Called from scrip_coswitch at the existing __asm__ save and sem_wait restore sites.                                                                                                                */
void rtcc_coexpr_save(uint64_t *dst_256);   /* dst must be 256B aligned or at least 256B large */
void rtcc_coexpr_restore(const uint64_t *src_256);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* C-side crossing helpers (used by inbound stubs: bb_glue, rt_chain_enter glue, proc prologues).                                                                                                    */
/* These are NO-OPs when g_rtcc_on == 0 (the killswitch path).                                                                                                                                        */
/* rtcc_load_scratch: RC-2 LOAD block→{R10,R11,R8,R9} at C→generated boundaries (scratch tier only).        */
/* Retained for RC-2/RC-3 inbound stubs that have not yet been converted to rtcc_load_all.                   */
void rtcc_load_scratch(void);
/* rtcc_load_all: RC-4 LOAD block→{RAX,RCX,RDX,RSI,RDI,R8,R9,R10,R11} at C→generated boundaries.          */
/* The full 9-GPR set.  Clobbers all nine.  Gate: g_rtcc_on==0 → no-op (killswitch law).                    */
void rtcc_load_all(void);
#ifdef __cplusplus
}
#endif
#endif
