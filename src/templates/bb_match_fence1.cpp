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
 * ([___+off] BY DESIGN — no fc_geom registration, because the σ glue reads it at the DYNAMIC post-P depth, and ___ is per-activation (s79 pat-blob seed) so DEFER recursion through the same fence node
 * is depth-safe) and jmps P's entry (PAIR 0); P's commit lands the na_s glue (PAIR 2): ONE mov restores the watermark — HEAD/RELEASE's S10e unwind idiom, cell-free — releasing P's retained cells,
 * suspended DEFER records, and choice frames together, then γ.  P's leftward exhaust lands na_f (PAIR 3): the same restore (the IDENTITY when the LIFO ω-cascade arrived at mark depth; exactness if a
 * future cut arrives deep) then ω.  β ≡ abandon (the ARBNO L2 seal precedent): the lowerer emits no resume edge into a sealed span — right_sealed skips the repoint — so β simply falls into na_f.
 * dcap pends ride the r12 island (subject-offset records, no ζ pointers) and are deliberately untouched: conditional captures inside P must survive to RELEASE's commit pump.  g_patstk_sp is the
 * HEAD/RELEASE statement bracket's charge, not the fence's.  PAIR(1) (P's resume β) is allocated by the generic drive but unreferenced here — the seal is the whole point. */
static int fence_whack_on() { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_FENCE_WHACK"); v = (e && e[0] == '0') ? 0 : 1; } return v; }
static int fence_u2_frame(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_U2_FENCE"); v = (e && e[0] == '0') ? 0 : 1; } return v && x86_port_cstack() && _.op_ival != 2; }   /* ⭐ U-2 STRUCTURAL (s42+1): FENCE1 gets an OWN independent ___ frame … ival==2 (FENCE1-in-ARBNO-body) suppresses U-2: ARBNO owns ___ in its own frame dance; a nested push-___/pop-___ corrupts it (MEASURED: 142_pat_arbno_fence_arbno hang). */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ H29-FIX (s33): U-2 WATERMARK SLOT ALIASING.
 * When fence_u2_frame() is active, bb_glue_framed_enter() shifts inner____ = outer____ − 32.
 * Inner-frame ZLS nodes (ALTERNATE etc.) receive slots at inner____+N for their assigned offsets.
 * FENCE1's watermark slot is FRQ(off) = [outer____+off].  ALTERNATE's failure-continuation at
 * FRQ(off+16) = [inner____+off+16] = [outer____-32+off+16] = [outer____+off-16].  ALTERNATE's
 * saved-r14 at FR(off) = [inner____+off] = [outer____-32+off] = [outer____+off-32].
 * The ZLS planner assigns FENCE1's slot (op_off) BEFORE inner nodes; inner nodes start from op_off
 * (same base).  ALTERNATE uses op_off+0, op_off+8, op_off+16 relative to inner____, which resolve
 * to outer____+op_off-32, outer____+op_off-24, outer____+op_off-16 respectively.  FENCE1's slot at
 * [outer____+op_off] is NOT in this set — so the original save to FRQ(off) is SAFE (no alias).
 *
 * BUT: fence_release in the inner-frame paths (PAIR(2)/(3) and internal β from failed inner pattern)
 * also uses FRQ(off) = [___+off].  After framed_enter, ___=inner____, so FRQ(off)=[inner____+off]
 * =[outer____-32+off] — which IS ALTERNATE's saved-r14 slot (a cursor value, not the pre-fence rsp).
 *
 * FIX: save the watermark at TWO slots: FRQ(off) [outer____+off] for the β path (outer ___ restored),
 * AND FRQ(off+32) [outer____+off+32]=[inner____+off+64] (inner____+off+64 is beyond any ALTERNATE slot
 * at inner____+off+16) for the inner-frame paths.  fence_release uses FRQ(off+32) when the inner frame
 * is live; fence_release on β uses FRQ(off) with outer____ (always safe there).
 * This requires op_off+32+8 ≤ GRANT_SIZE.  The ZLS grant for FENCE1 is the per-node quad (32B or 48B
 * depending on the ZLS protocol).  op_off+40 must fit in the grant.  Since the ZLS planner packs FENCE1
 * first at the base and inner nodes start after, off+32 is BEFORE the inner nodes' slots (inner nodes
 * start at off + size_of_fence1_grant which is ≥40).  So off+32 is inside FENCE1's own grant window
 * and cannot alias any inner node.
 *
 * ONE AUTHORITY: both save sites and both release sites are gated by fence_u2_frame(). */
static std::string fence_mark_save(int off) {
    /* U-2: save to BOTH FRQ(off) [outer, for β] and FRQ(off+32) [outer, for inner-frame paths PAIR(2)/(3)]. */
    if (x86_port_cstack() && fence_u2_frame()) return x86("mov", FRQ(off), "rsp") + x86("mov", FRQ(off+32), "rsp");
    if (x86_port_cstack()) return x86("mov", FRQ(off), "rsp");
    if (x86_port_mode() == ZC_PORT_INLINE || x86_port_mode() == ZC_PORT_OWNED) return x86_zls2_cur_lea("rdi") + x86("mov", "rax", RDQ("rdi", 0)) + x86("mov", FRQ(off), "rax");
    if (x86_port_mode() != ZC_PORT_ALLOC) return std::string();
    return x86_align_enter() + x86("call", "rt_zls2_mark", (uint64_t)(uintptr_t)(void *)rt_zls2_mark) + x86_align_leave() + x86("mov", FRQ(off), "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string fence_release(int off) {
    /* U-2 inner-frame paths: read FRQ(off+32) = [___+off+32].  When inner frame live: ___=inner____,
     * [inner____+off+32]=[outer____-32+off+32]=[outer____+off] — same value as FRQ(off) with outer ___.
     * When inner frame dead (β pass-through): ___=outer____, [outer____+off+32] is our second save slot.
     * Both are correct.  Use off+32 uniformly for all three release sites (PAIR(2), PAIR(3), β). */
    if (x86_port_cstack() && fence_u2_frame()) return x86("mov", "rsp", FRQ(off+32));
    if (x86_port_cstack()) return x86("mov", "rsp", FRQ(off));
    if (x86_port_mode() == ZC_PORT_INLINE || x86_port_mode() == ZC_PORT_OWNED) return x86_zls2_cur_lea("rdi") + x86("mov", "rax", FRQ(off)) + x86("mov", RDQ("rdi", 0), "rax");
    if (x86_port_mode() != ZC_PORT_ALLOC) return std::string();
    return x86_align_enter() + x86("mov", "rdi", FRQ(off)) + x86("call", "rt_zls2_release_to", (uint64_t)(uintptr_t)(void *)rt_zls2_release_to) + x86_align_leave();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* s137 OVER-SEAL (Lon ruling, this session): a fence's forward commit — the OUTSIDE of its γ — demarks a sync point past which NO backtracking is guaranteed (FENCE0: backup aborts the attempt; FENCE1:
 * the s133 seal already made both directions edge-dead corpus-wide), so the ENTIRE dynamic-ζ chunk of the current activation is whacked there, not just the fence's own interior.  THE WHACK IS ONE MOV:
 * `mov rsp, ___` — ___ IS the activation's dynamic-ζ floor (the U2/U1 seed `mov ___, rsp` right after the self-alloc `sub rsp, K_total`, xa_flat.cpp ~170/229; every box carve, choice cell, and retained
 * nested-activation frame sits BELOW it; every surviving value/quad sits ABOVE it in the ___ flat frame, and dcap pends ride the r12 mmap island).  Precedent: SPD-2's scanfail block uses the identical
 * `rsp=___` bulk-free ("post-carve frontier: every element grant sits below", emit.cpp ~2070).  Retention drops O(activations) → O(depth): each committed sub-match's retained frame dies at the next
 * enclosing fence commit instead of at the match bracket.  na_s also REWRITES the watermark quad := ___ (the new floor), so a post-commit na_f arrival (the ARBNO in-body abandon route) restores to the
 * floor, never below it into whacked-and-reused bytes.  cstack/FORTH ports only — the arena ports keep the s133 own-span release (their activation floor is not plumbed; correct, unoptimized). */
static std::string fence_whack_commit(int off) {
    return fence_release(off);   /* ⭐ FENCE-WHACK-UCLAIM (this rung): the s137 non-zw floor whack `mov rsp,___` is RETIRED — under UCLAIM the statement claim (`sub rsp,K` at the statement head) lives BELOW the graph ___ floor, so whacking to ___ released the claim the statement's staged `add rsp,K` fail exits still owned: DOUBLE-RELEASE, measured as SEGV-after-correct-output (FENCE(P)+failing-defer witness f6d) and as the H26/G23 hang class (rc=124), all cured by SCRIP_FENCE_WHACK=0 — this line makes the cured path the default.  The commit now frees the fence's OWN extent only (watermark restore = P's growth), per WHACK CONTRACT clause 5: statement-accumulated ζ release is STATEMENT_END's authority, not the fence's.  Success paths were self-healing only because match_end's absolute `mov rsp,[___+64]` forgave the over-free; the seal-fail route had no absolute restore, hence the crash class.  Known cost accepted: the s137 json-match retention win narrows to P's own span (memory, not correctness).  The op_zw arm above is the W ladder's own protocol — untouched. */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_fence1() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_ival == 0)   /* FENCE0 sync box (s137): the bare-variable FENCE lowered as an INTERIOR spine element — operand-free, no watermark (its α IS the commit: match null, whack, γ); β ≡ abandon
                           * (post-commit backup is the attempt-abort, routed by the lowerer's ω edge exactly as the s133 erasure routed it — the box adds only the whack).  First-position FENCE0 (the
                           * anchor idiom) stays node-free in the lowerer: zero left context, nothing to whack. */
        return x86("comment", "IR_MATCH_FENCE1 ival=0 (FENCE0 interior sync box: alpha commits — match null — then gamma; beta abandons to omega; NO RSP whack — WHACK CONTRACT clause 5: statement ζ release is STATEMENT_END's authority; the op_zw arm that lived here double-released the UCLAIM statement claim below the ___ floor — ZWS-FENCE0-UCLAIM fix, MECH s6)")
             + x86_alpha()
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    return _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_FENCE1: watermark slot not granted (zls)")
         : x86("comment", "IR_MATCH_FENCE1 (SYNC-POINT zeta RELEASE: watermark at alpha, OVER-SEAL whack to the ___ activation floor at the seal-success glue — s137 ruling; H29-FIX s33: U-2 dual-slot save avoids ZLS alias between outer FRQ(off) and inner FRQ(off+32))")
         + x86_alpha()
         + fence_mark_save(_.op_off)   /* ⭐ H29-FIX (s33): U-2 saves to BOTH FRQ(off) [for β outer-___ reads] AND FRQ(off+32) [for inner-frame reads where ___=inner____]; non-U2: FRQ(off) only */
         + IF(fence_u2_frame(), bb_glue_framed_enter())   /* ⭐ U-2 STRUCTURAL (s42+1): establish FENCE1's OWN independent ___ frame before entering P's body.  push ___; mov ___,rsp (K=0, alignment sub handled by bb_glue_framed_enter).  The frame is independent of MATCH_BEGIN's per HQ ruling O-PB-4: ARBNO and FENCE1 must NOT touch MATCH_BEGIN's frame data.  fence_whack_commit's `mov rsp,___` at PAIR(2) then whacks P's growth to FENCE1's own floor, and the paired bb_glue_framed_leave pops to restore the outer activation's ___.  Gate: SCRIP_U2=1, x86_port_cstack() only (FORTH/CSTACK ports; arena/alloc ports keep the s133 release path). */
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + IF(fence_u2_frame(), bb_glue_framed_leave())   /* ⭐ U-2 STRUCTURAL (s42+1): paired leave — mov rsp,___; pop ___ — restores the pre-FENCE1 outer ___ BEFORE fence_whack_commit fires.  After this, ___ = pre-FENCE1 match_begin frame; fence_whack_commit's `mov rsp,___` whacks to that restored floor (or to [___-8] under op_zw — see fence_whack_commit's own ZW arm), which is exactly the activation floor just before FENCE1's entry, clearing P's ζ growth.  ONE AUTHORITY: paired with the framed_enter above. */
         + IF(emit_arbno_rbp_unwind() && x86_port_cstack(), x86_arbno_rbp_unwind_at(fence_u2_frame() ? FRQ(_.op_off+32) : FRQ(_.op_off), 11, 12))   /* ⭐ FENCE-COMMIT WALK (Lon in-chat 2026-08-14: "GAMMA final success and GAMMA FENCE'd success we'll possibly have this same problem" — he is right): the commit whack below restores rsp from the banked watermark, discarding P's interior INCLUDING any ARBNO frame gamma-suspended inside P — and after a FENCE commit those instances can NEVER be resumed (manual Ch.9 p.125-127: alternatives within P invisible when backing up), so they are permanently dead frames whose rbp chain must be walked home HERE, before the whack releases their memory (the E-1 law verbatim).  MARK = the exact operand the whack loads (fence_release's cstack arms mirrored: FRQ(off+32) under u2_frame else FRQ(off), read AFTER the framed_leave so ___ resolves identically) — banked at fence alpha, hence ABOVE every frame carved inside P and AT/BELOW any frame suspended BEFORE the fence, so the walk pops exactly P's dead interior and STOPS at pre-fence frames, which stay live (a post-commit backup pass-through resumes LEFT of the fence and may re-enter them; their sweep belongs to MATCH_END/fail-tail, whose walks compose with this one).  rax scratch: dead here (arrivals are P's success glue jmp; framed_leave touches rsp/___ only; nothing reads rax before gamma).  L(11)/L(12) free (this template uses PAIR(0..3) only).  beta/PAIR(3) exits deliberately NOT walked: P's exhaust retreats through each interior ARBNO's own omega (frames self-pop, the four-port LIFO law) and abort-class escapes route to the MATCH_BEGIN fail tail, which E-1 already covers.  cstack ports only — arena ports release into zls2, no rsp frames to orphan. */
         + fence_whack_commit(_.op_off)   /* ⭐ H29-FIX (s33): after framed_leave ___=outer____; fence_release reads FRQ(off+32)=[outer____+off+32] which is the second save slot — correct pre-fence rsp */
         + x86_gamma()
         + x86("def", PAIR(3))
         + IF(fence_u2_frame(), bb_glue_framed_leave())   /* ⭐ U-2 STRUCTURAL (s42+1): phi/fail path — symmetric leave so the ABI ___ discipline holds on P's exhaust (PAIR(3)) too.  fence_release restores the rsp watermark directly; the framed_leave here restores ___ so the outer match context is intact at omega. */
         + x86_beta()   /* ⭐ FENCE-PASS-THROUGH (this rung): β moved BELOW the framed_leave.  β is the post-commit pass-through entrance (the right context's leftward exhaust, seam-repointed here by the lowerer) — at that point the as-glue's leave already popped the fence frame, so a β arrival has NO frame: falling through the leave executed `mov rsp,___; pop ___` against the OUTER ___ (rsp := graph floor, ___ := garbage from [floor]) — measured as the T3 SEGV / H01 hang on the first pass-through wiring.  Interior exhaust (PAIR 3, frame live) takes the leave; β lands the shared watermark restore only, then ω resumes left.  γ above ends in its own jmp, so nothing falls into PAIR(3) from above. */
         + fence_release(_.op_off)   /* ⭐ H29-FIX (s33): all three release sites use FRQ(off+32); inner-frame case: [inner____+off+32]=[outer____+off]; outer-frame case (β pass-through after leave): [outer____+off+32] = second save slot. Both correct. */
         + x86_omega();
}
