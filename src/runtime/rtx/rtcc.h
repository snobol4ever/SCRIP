/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rtcc.h — RTCC: Global-register liberation block (GOAL-RTCC.md RC-1).                                                                                                                               */
/* THE CONTRACT (BLOCK-CANONICAL LAW): registers are a CACHE valid ONLY inside generated code.  Every                                                                                                 */
/* generated→C crossing = WRITEBACK (regs→block); every C→generated crossing = LOAD (block→regs).                                                                                                    */
/* With SCRIP_RTCC=0 (default) the block exists but is never written; all crossing macros are no-ops;                                                                                                 */
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
/* NOTE: callee-saved set (RBX RBP R12 R13 R14 R15 RSP) is ALREADY SCRIP-owned and NOT stored here.                                                                                                  */
#define RTCC_GPR_COUNT  9           /* RAX RCX RDX RSI RDI R8 R9 R10 R11 — the nine caller-saved GPRs */
#define RTCC_XMM_COUNT  8           /* XMM8–XMM15 */
#define RTCC_GPR_BYTES  (RTCC_GPR_COUNT * 8)       /* 72 bytes */
#define RTCC_XMM_BYTES  (RTCC_XMM_COUNT * 16)      /* 128 bytes */
#define RTCC_BLOCK_BYTES (RTCC_GPR_BYTES + RTCC_XMM_BYTES) /* 200 bytes; padded to 256 for alignment */
/* Slot indices into the 8-byte GPR array */
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
/* The block itself — declared in rtcc_init.c; extern here for the GC and coexpr paths.                                                                                                              */
/* 256-byte aligned so every slot fits in one or two L1 cache lines.                                                                                                                                  */
extern uint64_t g_rtcc_block[32];   /* 32 × 8B = 256B; XMM slots at [9]..[24] as two uint64 each */
extern unsigned char g_rtcc_on;     /* 0 = OFF (default, killswitch); 1 = ON (SCRIP_RTCC=1) */
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
#ifdef __cplusplus
}
#endif
#endif
