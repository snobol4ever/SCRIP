/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RTCC_H
#define RTCC_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define RTCC_GPR_COUNT  9
#define RTCC_XMM_COUNT  8
#define RTCC_GPR_BYTES  (RTCC_GPR_COUNT * 8)
#define RTCC_XMM_BYTES  (RTCC_XMM_COUNT * 16)
#define RTCC_BLOCK_BYTES (RTCC_GPR_BYTES + RTCC_XMM_BYTES)
#define RTCC_SLOT_RAX   0
#define RTCC_SLOT_RCX   1
#define RTCC_SLOT_RDX   2
#define RTCC_SLOT_RSI   3
#define RTCC_SLOT_RDI   4
#define RTCC_SLOT_R8    5
#define RTCC_SLOT_R9    6
#define RTCC_SLOT_R10   7
#define RTCC_SLOT_R11   8
#ifndef RTCC_GLOBAL_R8_ANCHOR
#define RTCC_GLOBAL_R8_ANCHOR   1
#endif
#ifndef RTCC_GLOBAL_R9_GVA
#define RTCC_GLOBAL_R9_GVA   1
#endif
#define RTCC_GVA_REG            "r9"
extern uint64_t rtccb[32];
#ifndef __cplusplus
_Static_assert(RTCC_SLOT_R8 == 5 && RTCC_SLOT_R9 == 6 && RTCC_SLOT_R10 == 7 && RTCC_SLOT_R11 == 8,
    "rtccb IS THE CALLER-SAVED SPILL BLOCK AND IT IS NOT A GC ROOT. THE BLOCK: 32 raw machine words, 64-byte aligned, addressed rip-relative from emitted text. THE WORDS: slot 5 = r8 = the ANCHOR, slot 6 = r9 = the GVA base, slots 7 and 8 = r10 and r11 = scratch, spilled before every safe point at x86_asm.h 386-389 and reloaded after it at 395-398. THE REASON NOTHING THAT MUST SURVIVE A COLLECTION MAY LIVE HERE: no root walk visits this block and none can. These are 32 untagged words, so visiting them means asking of each one does this look like it points into a heap block, which is a conservative scan by definition, and CEO-812 froze that out -- everything the collector reads is a DESCR whose type field is the only tag. A value that must survive a poll goes in a frame slot covered by that frame static map, or is handed to the shield as a tagged DESCR cell in arr[] (CEO-972). THE CONVENTION IS A MECHANISM ONLY BECAUSE A GATE HOLDS IT (CEO-1053, a convention enforced by nothing will drift): test_gate_gc_the_caller_saved_spill_block_never_holds_a_heap_reference.sh grades every witness that collects and proves its own detector fires by planting a heap pointer in slot 7 or 8, which read a measured zero. Slot 5 does NOT read zero today -- 806 heap references over eight SNOBOL4 witnesses -- and that gate names them.");
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
