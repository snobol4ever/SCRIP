#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void *rt_zls2_mark(void);
extern "C" void  rt_zls2_release_to(void *);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_MATCH_FENCE1 — the FENCE1 = FENCE(P) FUNCTION form (the bare-variable FENCE0 stays node-free: its whole box body would be α→γ, β→ω, so the lowerer erases it into edge rewiring).  SYNC-POINT ζ RELEASE, sync point 2 (Lon ruling s132): FENCE1's success exit is a declared sync point — the seal makes every alternative inside P invisible on backup (manual ln
 * 4716), so every ζ cell P's boxes retained under uniform-β is DEAD the instant P commits, yet the old edge-only seal left the whole span on the stack until the match bracket died (measured: >32MB
 * ≤64MB retained ζ on json-match.sno's 632KB subject — the FENCE-per-token ws pattern).  The box is the ALT/SEQ σ/φ-glue shape with N hardwired 1: α records the watermark into its granted frame quad
 * ([rbp+off] BY DESIGN — no fc_geom registration, because the σ glue reads it at the DYNAMIC post-P depth, and rbp is per-activation (s79 pat-blob seed) so DEFER recursion through the same fence node
 * is depth-safe) and jmps P's entry (PAIR 0); P's commit lands the na_s glue (PAIR 2): ONE mov restores the watermark — HEAD/RELEASE's S10e unwind idiom, cell-free — releasing P's retained cells,
 * suspended DEFER records, and choice frames together, then γ.  P's leftward exhaust lands na_f (PAIR 3): the same restore (the IDENTITY when the LIFO ω-cascade arrived at mark depth; exactness if a
 * future cut arrives deep) then ω.  β ≡ abandon (the ARBNO L2 seal precedent): the lowerer emits no resume edge into a sealed span — right_sealed skips the repoint — so β simply falls into na_f.
 * dcap pends ride the r12 island (subject-offset records, no ζ pointers) and are deliberately untouched: conditional captures inside P must survive to RELEASE's commit pump.  g_patstk_sp is the
 * HEAD/RELEASE statement bracket's charge, not the fence's.  PAIR(1) (P's resume β) is allocated by the generic drive but unreferenced here — the seal is the whole point. */
static std::string fence_mark_save(int off) {
    if (x86_port_cstack()) return x86("mov", FRQ(off), "rsp");
    if (x86_port_mode() == ZC_PORT_INLINE || x86_port_mode() == ZC_PORT_OWNED) return x86_zls2_cur_lea("rdi") + x86("mov", "rax", RDQ("rdi", 0)) + x86("mov", FRQ(off), "rax");
    if (x86_port_mode() != ZC_PORT_ALLOC) return std::string();
    return x86_align_enter() + x86("call", "rt_zls2_mark", (uint64_t)(uintptr_t)(void *)rt_zls2_mark) + x86_align_leave() + x86("mov", FRQ(off), "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string fence_release(int off) {
    if (x86_port_cstack()) return x86("mov", "rsp", FRQ(off));
    if (x86_port_mode() == ZC_PORT_INLINE || x86_port_mode() == ZC_PORT_OWNED) return x86_zls2_cur_lea("rdi") + x86("mov", "rax", FRQ(off)) + x86("mov", RDQ("rdi", 0), "rax");
    if (x86_port_mode() != ZC_PORT_ALLOC) return std::string();
    return x86_align_enter() + x86("mov", "rdi", FRQ(off)) + x86("call", "rt_zls2_release_to", (uint64_t)(uintptr_t)(void *)rt_zls2_release_to) + x86_align_leave();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_fence1() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_FENCE1: watermark slot not granted (zls)")
         : x86("comment", "IR_MATCH_FENCE1 (SYNC-POINT zeta RELEASE: watermark at alpha, bulk-restore at the seal-success glue)")
         + x86_alpha()
         + fence_mark_save(_.op_off)
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + fence_release(_.op_off)
         + x86_gamma()
         + x86_beta()
         + x86("def", PAIR(3))
         + fence_release(_.op_off)
         + x86_omega();
}
