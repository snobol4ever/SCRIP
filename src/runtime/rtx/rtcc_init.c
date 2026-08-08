/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* rtcc_init.c — RTCC block storage, gate initialisation, GC root, coexpr block-swap (GOAL-RTCC.md RC-1).                                                                                            */
/* KILLSWITCH LAW: SCRIP_RTCC=0 is the default; the block exists in BSS but is never accessed by the                                                                                                  */
/* veneer (the x86("call") dispatch arm is a dead branch when g_rtcc_on==0); binary output is byte-identical                                                                                          */
/* to the pre-RTCC tree at gate OFF. SCRIP_RTCC=1 arms the writeback/load protocol at every boundary.                                                                                                 */
/* COEXPR RULING (RC-0 FINDING, Option B): block-swap at the existing scrip_coswitch save/restore sites                                                                                               */
/* — no new encoder arm, zero cost for non-coexpr programs, one memcpy of RTCC_BLOCK_BYTES at each switch.                                                                                            */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "rtcc.h"
#include "gc_heap.h"
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* The RTCC block: 256-byte aligned BSS.  32 uint64 = 256 bytes.                                                                                                                                      */
/* GPR slots [0..8]: RAX RCX RDX RSI RDI R8 R9 R10 R11 (8 bytes each).                                                                                                                               */
/* XMM slots [9..24]: XMM8..XMM15 (two uint64 each, low then high).                                                                                                                                   */
/* Slots [25..31]: reserved/pad (keep block cache-line-multiple).                                                                                                                                      */
__attribute__((aligned(64))) uint64_t g_rtcc_block[32];
unsigned char g_rtcc_on = 0;   /* default OFF — killswitch law */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned char rtcc_env_on(const char *name) { const char *e = getenv(name); if (!e || !*e) return 0; return (unsigned char)(e[0] != '0'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
__attribute__((constructor)) static void rtcc_init(void) { g_rtcc_on = rtcc_env_on("SCRIP_RTCC"); if (g_rtcc_on) rtcc_gc_register(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rtcc_gc_register(void) { rt_gc_root_pin_add((const char *)&g_rtcc_block[0]); }   /* RC-0(d) BLOCK-CANONICAL: at any GC point all claimed-register values sit in the block; registering it is sufficient — no per-register pin needed. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Coexpr block-swap (Option B): called from scrip_coswitch at save and restore sites.                                                                                                                */
/* When g_rtcc_on==0 these are no-ops; the coswitch path is unchanged.                                                                                                                                */
void rtcc_coexpr_save(uint64_t *dst_256) { if (!g_rtcc_on) return; memcpy(dst_256, g_rtcc_block, RTCC_BLOCK_BYTES); }
void rtcc_coexpr_restore(const uint64_t *src_256) { if (!g_rtcc_on) return; memcpy(g_rtcc_block, src_256, RTCC_BLOCK_BYTES); }
