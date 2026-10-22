/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RTCC_H
#define RTCC_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define RTCC_GPR_COUNT 9
#define RTCC_XMM_COUNT 8
#define RTCC_GPR_BYTES (RTCC_GPR_COUNT * 8)
#define RTCC_XMM_BYTES (RTCC_XMM_COUNT * 16)
#define RTCC_BLOCK_BYTES (RTCC_GPR_BYTES + RTCC_XMM_BYTES)
#define RTCC_SLOT_RAX 0
#define RTCC_SLOT_RCX 1
#define RTCC_SLOT_RDX 2
#define RTCC_SLOT_RSI 3
#define RTCC_SLOT_RDI 4
#define RTCC_SLOT_R8 5
#define RTCC_SLOT_R9 6
#define RTCC_SLOT_R10 7
#define RTCC_SLOT_R11 8
#define RTCC_SLOT_MATCHAT 31
#ifndef RTCC_GLOBAL_R8_ANCHOR
#define RTCC_GLOBAL_R8_ANCHOR 1
#endif
#ifndef RTCC_GLOBAL_R9_GVA
#define RTCC_GLOBAL_R9_GVA 1
#endif
#define RTCC_GVA_REG "r9"
extern uint64_t g_rtccb[32];
#ifndef __cplusplus
_Static_assert(RTCC_SLOT_R8 == 5 && RTCC_SLOT_R9 == 6 && RTCC_SLOT_R10 == 7 && RTCC_SLOT_R11 == 8,
    "g_rtccb IS THE CALLER-SAVED SPILL BLOCK AND IT IS NOT A GC ROOT. THE BLOCK: 32 raw machine words, 64-byte aligned, addressed rip-relative from emitted text. THE WORDS: slot 5 = r8 = the ANCHO"
    "R, slot 6 = r9 = the GVA base, slots 7 and 8 = r10 and r11 = scratch, spilled before every safe point at x86_asm.h 386-389 and reloaded after it at 395-398. THE REASON NOTHING THAT MUST SUR"
    "VIVE A COLLECTION MAY LIVE HERE: no root walk visits this block and none can. These are 32 untagged words, so visiting them means asking of each one does this look like it points into a hea"
    "p block, which is a conservative scan by definition, and CEO-812 froze that out -- everything the collector reads is a DESCR whose type field is the only tag. A value that must survive a po"
    "ll goes in a frame slot covered by that frame static map, or is handed to the shield as a tagged DESCR cell in arr[] (CEO-972). THE CONVENTION IS A MECHANISM ONLY BECAUSE A GATE HOLDS IT (C"
    "EO-1053, a convention enforced by nothing will drift): test_gate_gc_the_caller_saved_spill_block_never_holds_a_heap_reference.sh grades every witness that collects and proves its own detect"
    "or fires by planting a heap pointer in slot 7 or 8, which read a measured zero. Slot 5 does NOT read zero today -- 806 heap references over eight SNOBOL4 witnesses -- and that gate names th"
    "em.");
_Static_assert(RTCC_SLOT_MATCHAT == 31,
    "THE OTHER 23 WORDS ARE SPOKEN FOR AND A NEW USE TAKES A FREE ONE BY NAME HERE, NEVER A LITERAL INDEX (hq_snocone 2026-10-14, the coo's SncBench red bisected to 6a3ff25a9): slots 9..24 are th"
    "e XMM half of the spill block (RTCC_XMM_BYTES past the nine GPR words); slots 25..30 are SETEXIT's -- 25 the pending handler, 26..29 the resume label, rsp, rbp and r12 for CONTINUE, 30 the e"
    "rror code rt_setexit_abort reads; slot 31 is the start offset of the capture whose deferred call is being pumped (rt_dcap_pump writes it, the MATCHAT builtin reads it, an integer never a he"
    "ap pointer). 6a3ff25a9 wrote that offset into slot 30 and every capture pump overwrote SETEXIT's error code: parser_snobol4.sc as a mode-3 program then died SIGSEGV in the chain walk.");
#endif
extern unsigned char g_rtcc_on;
void rtcc_coexpr_save(uint64_t *dst_256);
void rtcc_coexpr_restore(const uint64_t *src_256);
void rtcc_load_scratch(void);
void rtcc_load_all(void);
#ifdef __cplusplus
}
#endif
#endif
