#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
ScanSubjRegs rt_match_enter(uint64_t lo, uint64_t hi);
void * rt_zls_mark(void);
void   rt_zls_release_to(void *mark);
extern long g_anchor;
}
#include "x86_asm.h"
extern "C" uint64_t g_patstk_sp;   /* R12-ERAD s65: pattern side stack frontier — statement-bracket mark rides the reincarnated arena slot (cell/flat +8) */
/* ZB-FC-3d (partition ruling, s49): under the statement grant (op_fc_wbytes window, hook dormant) HEAD self-pushes a 32-byte rsp cell after alpha holding the three MATCH-SPAN-lifetime fields -- anchor
 * @+0, zls mark @+8, rsp mark @+16 -- which the window rebases with zero arithmetic below (every FR/FRQ in [op_off, op_off+24) becomes [rsp+k]; +32/+40 fall outside and stay FLAT, exactly right: the
 * dcap mark and saved rbp have POST-UNWIND consumers).  The rsp mark = the PRE-PUSH rsp (mov rax,rsp; add rax,32 -- lea [rsp+K] deliberately avoided: x86_reg_disp32_lea64 emits no SIB so an rsp base
 * mis-encodes in BINARY), so the existing zls2 unwind releases this cell and every suspended pattern cell in one mov, at BOTH statement exits, unchanged.  The flat +0 write disappears under grant --
 * RELEASE re-homes the match START there pre-unwind for REPLACE (the splice-survivor partition).  L(1)'s zls-mark read hoists BEFORE align_enter (a window read inside the align dance is wrong -- the
 * 3c lesson); rdi rides the pushes.  Ungranted: every line byte-verbatim today's path (degrade never die). */
static inline int hfc() { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_wbytes > 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_head() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return (_.op_sa < 0 || _.op_off < 0)
         ? x86_bomb("IR_MATCH_HEAD: subject/start slot not promoted (emit_drive)")
         : x86("comment", "IR_MATCH_HEAD")
         + x86_alpha()
         /* REG-3 FRAME-RBP: EVERY head saves the outer rbp at flat +40 (the slot REG-2 parked — re-tenanted
          * here as the saved-outer-rbp of the frame chain).  FN-SEAL-RBP s83: the save is FRQ — rbp-relative,
          * self-referential, matching ALL THREE restores (head:101 ω, release:89 γ, replace:36) — depth-immune
          * by the activation seeds (U1 outer, U2/U2b non-pat blobs, s79 pat blobs); the old rsp-explicit
          * rspq40 arm named a DIFFERENT slot than the FRQ restores whenever a fence-seal-cut re-arrival
          * displaced rsp (the 067 rbp=0 mine, FINDING-2026-07-17-...-FN-SEAL-RBP).  A grant-DECLINED
          * statement's HEAD then materializes the window view — rbp = rsp =
          * the flat frame base (no cells pushed between statements, S10e) — so every FR/FRQ in the window
          * (op_anchored boxes emit rbp-based refs) is motion-immune exactly as under the dead s64/s66 r12
          * window, and BOTH statement exits restore rbp from +40 (the s61 both-edges lesson).  The saved-rbp
          * chain is the LIFO activation linkage of the s73 six-register map. */
         + x86("mov", FRQ(_.op_off + 40), "rbp")
         + IF(ZC_FRAME == ZC_FRAME_RSP && _.op_anchor_head, x86("mov", "rbp", "rsp"))
         /* R12-ERAD s65 (ZC_FRAME_RSP): FLAT-FIRST ordering — subject load, rt_match_enter, and the rbp/dcap mirror saves all run at rsp = frame base (D=0), THEN the 32B cell pushes and only
          * window-relative writes follow.  Under R12 the original order is byte-verbatim (rsp motion is invisible to r12-based refs).  The RSP cell field +16 = pre-push rsp = the frame base; the
          * old arena-mark slot +8 reincarnates as the PATTERN SIDE-STACK mark (S10e statement bracket for suspended activations).  Pat blobs (flat_pat, r12-island) take the R12 arm. */
         + IF(ZC_FRAME != ZC_FRAME_RSP || _.flat_pat, IF(hfc(), x86("sub", "rsp", (long)32))
             + IF(hfc(), x86("call", "rt_zls_mark", (uint64_t)(uintptr_t)(void *)rt_zls_mark) + x86("mov", FRQ(_.op_off + 8), "rax"))
             + (hfc() ? x86("mov", "rax", "rsp") + x86("add", "rax", (long)32) + x86("mov", FRQ(_.op_off + 16), "rax")
                      : x86_zls2_mark_save(_.op_off + 16)))
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("call", "rt_match_enter", (uint64_t)(uintptr_t)(void *)rt_match_enter)
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         /* REG-6 PEND-PROMOTE α (was REG-2 cell): the pend top lives in r12 (seeded once at the outer-graph
          * prologue from [RT_CAS_TOP]; jmp-entry blobs inherit it callee-saved) — save it as this match's
          * MARK, one direct frame store, the rax hop and the cell read both deleted.  Nested heads read the
          * live register.  The +40 caller-rbp slot stays dead-allocated v1 (REG-2 note stands). */
         + x86("mov", FRQ(_.op_off + 32), "r12")
         + IF(ZC_FRAME == ZC_FRAME_RSP && !_.flat_pat, (hfc() ? x86("mov", "rax", "rsp") + x86("sub", "rsp", (long)32) + x86("mov", FRQ(_.op_off + 16), "rax")
                                                               : x86_zls2_mark_save(_.op_off + 16))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", "rax", RDQ("rcx", 0))
             + x86("mov", FRQ(_.op_off + 8), "rax"))
         + x86("mov", FR(_.op_off), (long)0)
         + x86("def", L(0))
         + x86("mov", "r14d", FR(_.op_off))
         + x86_gamma()
         + x86_beta()
         + x86("add", FR(_.op_off), (long)1)
         + x86("mov", "eax", FR(_.op_off))
         + x86("cmp", "eax", "r15d")
         + x86("jg",  L(1))
         + x86("mov", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_anchor, "g_anchor")
         + x86("mov", "rax", "[rcx]")
         + x86("cmp64", "rax", (long)0)
         + x86("jne", L(1))
         + x86("jmp", L(0))
         + x86("def", L(1))
         /* R12-ERAD (ZC_FRAME_RSP + hfc): cell's rsp-mark lives at [rsp+16] (32B FORTH cell field +16).
          * No align dance open — bare mov rsp,[rsp+16] pops the 32B cell and restores pre-HEAD rsp.
          * s65: statement-fail also reclaims side-stack residue (S10e) from the reincarnated +8 slot. */
         + IF(ZC_FRAME == ZC_FRAME_RSP && !_.flat_pat, (hfc() ? x86("mov", "rax", "qword ptr [rsp + 8]") : x86("mov", "rax", FRQ(_.op_off + 8)))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", RDQ("rcx", 0), "rax"))
         + (hfc() && ZC_FRAME == ZC_FRAME_RSP && !_.flat_pat
             ? x86("mov", "rsp", "qword ptr [rsp + 16]")
             : ZC_FRAME == ZC_FRAME_RSP && !_.flat_pat
             ? x86_zls2_release_to_call(_.op_off + 16)   /* declined statement — the FORTH mark restore (bare mov rsp,[rsp+off], dances no-op'd); the arena call is gated out with the arena */
             : ( IF(hfc(), x86("mov", "rdi", FRQ(_.op_off + 8)))
               + x86_align_enter()
               + IF(!hfc(), x86("mov",  "rdi", FRQ(_.op_off + 8)))
               + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)
               + x86_zls2_release_to_call(_.op_off + 16)
               + x86_align_leave()))
         /* REG-6 PEND-PROMOTE ω (all anchors exhausted): truncate = r12 ← MARK, one frame load — the rax hop
          * and the cell store both deleted (was REG-2's inline rt_dcap_end_fail; the cell is never written
          * during a match at all now — seed-read at prologue is its whole running life). */
         + x86("mov", "r12", FRQ(_.op_off + 32))
         /* REG-3 fail-edge restore: rbp ← saved outer (+40).  FRQ is correct in ALL THREE frame classes here:
          * anchored → [rbp+off] with rbp==base (the classic frame-pointer pop), granted → [rsp+off] with rsp
          * unwound to base, flat_pat → [r12+off] island view. */
         + x86("mov", "rbp", FRQ(_.op_off + 40))
         + x86_omega();
}
