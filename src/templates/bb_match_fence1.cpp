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
/* s137 OVER-SEAL (Lon ruling, this session): a fence's forward commit — the OUTSIDE of its γ — demarks a sync point past which NO backtracking is guaranteed (FENCE0: backup aborts the attempt; FENCE1:
 * the s133 seal already made both directions edge-dead corpus-wide), so the ENTIRE dynamic-ζ chunk of the current activation is whacked there, not just the fence's own interior.  THE WHACK IS ONE MOV:
 * `mov rsp, rbp` — rbp IS the activation's dynamic-ζ floor (the U2/U1 seed `mov rbp, rsp` right after the self-alloc `sub rsp, K_total`, xa_flat.cpp ~170/229; every box carve, choice cell, and retained
 * nested-activation frame sits BELOW it; every surviving value/quad sits ABOVE it in the rbp flat frame, and dcap pends ride the r12 mmap island).  Precedent: SPD-2's scanfail block uses the identical
 * `rsp=rbp` bulk-free ("post-carve frontier: every element grant sits below", emit.cpp ~2070).  Retention drops O(activations) → O(depth): each committed sub-match's retained frame dies at the next
 * enclosing fence commit instead of at the match bracket.  na_s also REWRITES the watermark quad := rbp (the new floor), so a post-commit na_f arrival (the ARBNO in-body abandon route) restores to the
 * floor, never below it into whacked-and-reused bytes.  cstack/FORTH ports only — the arena ports keep the s133 own-span release (their activation floor is not plumbed; correct, unoptimized). */
static int fence_whack_on() { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_FENCE_WHACK"); v = (e && e[0] == '0') ? 0 : 1; } return v; }
static int fence_u2_frame(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_U2"); v = (e && e[0] == '1') ? 1 : 0; } return v && x86_port_cstack(); }   /* ⭐ U-2 STRUCTURAL (s42+1): FENCE1 gets an OWN independent RBP frame (push rbp; mov rbp,rsp at alpha; mov rsp,rbp; pop rbp before commit whack) so P's body executes with its own activation floor.  fence_whack_commit's `mov rsp,rbp` then whacks to FENCE1's floor, and pop rbp restores the pre-FENCE1 outer activation.  Independent of MATCH_BEGIN's frame per HQ ruling O-PB-4.  SCRIP_U2=1 to enable (default OFF); x86_port_cstack() guard: FORTH/CSTACK only. */
static std::string fence_whack_commit(int off) {
    if (x86_port_cstack() && fence_whack_on()) {
        if (_.op_zw) return x86("mov", "rsp", "qword ptr [rbp# + -8]") + x86("mov", FRQ(off), "rbp");   /* ⭐ O-7 ZW frame / ZW-15: old rbp (activation floor) is at [rbp-8] -- ZW-15 `lea rbp,[rbp+8]` raised rbp to claim_base; old_rbp is at [rbp-8] = claim_base-8 = rsp_just_after_push_rbp.  FENCE1 bulk-whack restores rsp to that floor (claim_base-8) and FRQ(off) := rbp (depth-free claim_base marker, unchanged). rbp# escape: raw register reference, must parse XK_REGDISP not XK_FR64 (ZB-FC-1 class). */
        return x86("mov", "rsp", "rbp") + x86("mov", FRQ(off), "rbp");
    }
    return fence_release(off);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_fence1() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_ival == 0)   /* FENCE0 sync box (s137): the bare-variable FENCE lowered as an INTERIOR spine element — operand-free, no watermark (its α IS the commit: match null, whack, γ); β ≡ abandon
                           * (post-commit backup is the attempt-abort, routed by the lowerer's ω edge exactly as the s133 erasure routed it — the box adds only the whack).  First-position FENCE0 (the
                           * anchor idiom) stays node-free in the lowerer: zero left context, nothing to whack. */
        return x86("comment", "IR_MATCH_FENCE1 ival=0 (FENCE0 interior sync box: alpha commits — whack the activation's dynamic zeta to the rbp floor — then gamma; beta abandons to omega)")
             + x86_alpha()
             + IF(x86_port_cstack() && fence_whack_on(), x86("mov", "rsp", "rbp"))
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    return _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_FENCE1: watermark slot not granted (zls)")
         : x86("comment", "IR_MATCH_FENCE1 (SYNC-POINT zeta RELEASE: watermark at alpha, OVER-SEAL whack to the rbp activation floor at the seal-success glue — s137 ruling)")
         + x86_alpha()
         + fence_mark_save(_.op_off)
         + IF(fence_u2_frame(), bb_glue_framed_enter())   /* ⭐ U-2 STRUCTURAL (s42+1): establish FENCE1's OWN independent RBP frame before entering P's body.  push rbp; mov rbp,rsp (K=0, alignment sub handled by bb_glue_framed_enter).  The frame is independent of MATCH_BEGIN's per HQ ruling O-PB-4: ARBNO and FENCE1 must NOT touch MATCH_BEGIN's frame data.  fence_whack_commit's `mov rsp,rbp` at PAIR(2) then whacks P's growth to FENCE1's own floor, and the paired bb_glue_framed_leave pops to restore the outer activation's rbp.  Gate: SCRIP_U2=1, x86_port_cstack() only (FORTH/CSTACK ports; arena/alloc ports keep the s133 release path). */
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + IF(fence_u2_frame(), bb_glue_framed_leave())   /* ⭐ U-2 STRUCTURAL (s42+1): paired leave — mov rsp,rbp; pop rbp — restores the pre-FENCE1 outer rbp BEFORE fence_whack_commit fires.  After this, rbp = pre-FENCE1 match_begin frame; fence_whack_commit's `mov rsp,rbp` whacks to that restored floor (or to [rbp-8] under op_zw — see fence_whack_commit's own ZW arm), which is exactly the activation floor just before FENCE1's entry, clearing P's ζ growth.  ONE AUTHORITY: paired with the framed_enter above. */
         + fence_whack_commit(_.op_off)
         + x86_gamma()
         + x86_beta()
         + x86("def", PAIR(3))
         + IF(fence_u2_frame(), bb_glue_framed_leave())   /* ⭐ U-2 STRUCTURAL (s42+1): phi/fail path — symmetric leave so the ABI rbp discipline holds on P's exhaust (PAIR(3)) too.  fence_release restores the rsp watermark directly; the framed_leave here restores rbp so the outer match context is intact at omega. */
         + fence_release(_.op_off)
         + x86_omega();
}
