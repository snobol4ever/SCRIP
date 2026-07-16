#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void rt_zls_release_to(void *mark);
extern "C" long rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj);
extern "C" long rt_dcap_step(DESCR_t fret);
extern "C" uint64_t g_patstk_sp;
extern "C" void rt_dcap_end_ok_close(void);
extern "C" void *rt_proc_open_fn(void);
extern "C" DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
extern "C" DESCR_t rt_proc_call_epilogue_ω(void);
#include "x86_asm.h"
/* ZB-FC-3d (partition ruling, s49): under the statement grant RELEASE stands at rsp = frontier - fp(pattern) (S10c: every box on the LINEAR success path is gamma-suspended) and reads HEAD's rsp cell
 * CROSS-BOX at [rsp + _.op_fc_disp + k] -- the 3c COND mechanism verbatim, disp = fp(pattern) from the fc_head registrar.  Pre-unwind it re-homes the match START (cell anchor @+0) into HEAD's FLAT +0
 * slot -- vacated by the granted HEAD, read UNCHANGED by REPLACE post-statement -- and stashes END at flat +24 exactly as today (the splice-survivor partition: fields with post-unwind lifetime live
 * flat).  The zls-mark read hoists BEFORE align_enter (window/rspd reads inside the align dance are wrong -- the 3c lesson; rdi rides the pushes); the unwind itself becomes the rspd variant reading
 * the PRE-PUSH mark out of the cell, releasing HEAD's cell and every suspended pattern cell in one mov.  Everything after the unwind -- the dcap pump, the mirror truncate at flat +32, the rbp restore
 * at flat +40 -- is byte-verbatim: those fields never entered the cell precisely because this code runs after it dies.  Ungranted: every line today's path (degrade never die). */
static inline int  rfc()      { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0; }
static inline const char * rspd(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "dword ptr [rsp + %d]", off); return b[i]; }
static inline const char * rspq(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp + %d]", off); return b[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string release_pump() {
    return std::string()
         + x86_xfer_enter()
         + x86_anchor_enter()
         /* REG-2 PEND-PARK: the old mirror-out died with the park (the pinned cell [RT_CAS_TOP] IS the live
          * top, so a nested match's head sees it directly).  Hand the pump its FIXED range and subject BY
          * VALUE: rdi = MARK (head's saved cursor), rsi = TOP (read from the cell), rdx = subject base (r13 —
          * immune to mid-pump Σ clobber by a nested match; the old ring snapshotted base pointers at record
          * time, the pointer-free entry moves that immunity into the ctx). */
         + (ZC_FRAME == ZC_FRAME_RSP && !_.flat_pat ? x86("mov", "rdi", rspq((int)(_.op_off + 32 + 32))) : x86("mov",  "rdi", FRQ(_.op_off + 32)))   /* R12-ERAD s65: this read sits INSIDE the 32B xfer window (post-unwind, so no fc_disp); +32 is the xfer window depth */
         + x86("mov",  "rsi", ABSQ(RT_CAS_TOP))
         + x86("mov",  "rdx", "r13")
         + x86("call", "rt_dcap_end_ok_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, const char *, const char *))rt_dcap_end_ok_open)
         /* NCB-1c M3: the 0..N computed-name (*VAR) commit transfers are EMITTED — a pump.  open returns
          * fbytes (a call owed) or 0 (flush complete); each transfer is the NCB-1b window; step assigns the
          * returned NAME and says whether another is owed.  x86_xfer_* saves r14/r15/r13: a *VAR proc body may
          * run its own match and would otherwise clobber the cursor (the M1 finding). */
         + x86("def",  L(1))
         + x86("test", "rax", "rax")
         + x86("je",   L(2))
         + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
         + IF(ZC_FRAME != ZC_FRAME_RSP || _.flat_pat, x86("push", "r12") + x86("sub",  "rsp", 8L))
         + x86_lea_id("rcx", 3)
         + x86_lea_id("rdx", 4)
         + IF(ZC_FRAME != ZC_FRAME_RSP || _.flat_pat, x86("mov",  "r12", "rsp"))
         + x86_jmp_reg("rax")
         + x86("def",  L(3))
         + IF(ZC_FRAME != ZC_FRAME_RSP || _.flat_pat, x86("mov",  "rax", "rsp")
             + x86("mov",  "rax", RDQ("rax", 8))
             + x86("mov",  "rdi", RDQ("rax", 0))
             + x86("mov",  "rsi", RDQ("rax", 8))
             + x86("mov",  "rsp", "r12")
             + x86("add",  "rsp", 8L)
             + x86("pop",  "r12"))
         + x86("call", "rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp",  L(1))
         + x86("def",  L(4))
         + IF(ZC_FRAME != ZC_FRAME_RSP || _.flat_pat, x86("mov",  "rsp", "r12")
             + x86("add",  "rsp", 8L)
             + x86("pop",  "r12"))
         + x86("call", "rt_proc_call_epilogue_ω", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(void))rt_proc_call_epilogue_ω)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp",  L(1))
         + x86("def",  L(2))
         + x86("call", "rt_dcap_end_ok_close", (uint64_t)(uintptr_t)(void *)(void (*)(void))rt_dcap_end_ok_close)
         + x86_anchor_leave()
         + x86_xfer_leave()
         /* REG-2 PEND-PARK success exit: the flush is done — truncate by storing cell ← MARK.  This mirrors
          * head's ω exactly; the cell is back at this match's floor and rbp was never touched by the pend. */
         + x86("mov", "rax", FRQ(_.op_off + 32))
         + x86("mov", ABSQ(RT_CAS_TOP), "rax")
         /* REG-3 success-edge restore: rbp ← saved outer (+40), the other half of the s61 both-edges bracket.
          * GATED on no-replacement: a replacement statement's REPLACE box runs AFTER this γ and reads its
          * subject/start/end through the anchored window base — restoring here handed it the outer rbp and
          * wiped whole lines (the wordcount/prep2 mine, bracketed by baseline-worktree .s diff s77).  When a
          * replacement follows, REPLACE owns the restore at ITS exit.  Post-unwind + post-xfer_leave rsp==base,
          * so FRQ resolves correctly whichever frame class release's own flags put it in. */
         + IF(_.op_dval == 0.0, x86("mov", "rbp", FRQ(_.op_off + 40)))
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_release() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_RELEASE: head slot not resolved (operand[0] missing or unowned)")
         : _.op_tail
         ? x86("comment", "IR_MATCH_RELEASE (R12-EXIT-1 tail: bracket read off the TOP ELEMENT, then the one-mov unwind collapses every element, every suspended cell, and HEAD's cell together)")
         + x86_alpha()
         + x86("mov", "rax", rspq((int)_.op_fc_disp + 0))
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
         + x86("mov", RDQ("rcx", 0), "rax")
         + x86("mov", "rsp", rspq((int)_.op_fc_disp + 8))
         + release_pump()
         : x86("comment", "IR_MATCH_RELEASE")
         + x86_alpha()
         /* R12-ERAD s65 (ZC_FRAME_RSP non-pat): pre-unwind rsp = base − 32 − fp(pattern), so the two FLAT re-homes compensate by op_fc_disp + 32 (explicit rspd/rspq).  The arena release is GATED OUT
          * wholesale: no heap in the BB equation — the rspd unwind (bare mov rsp,[cell+16] once the G1 dances no-op) IS the release of HEAD's cell and every suspended pattern cell.
          * Statement-success also reclaims side-stack residue (S10e) from the reincarnated +8 slot before the rsp unwind. */
         + IF(ZC_FRAME == ZC_FRAME_RSP && !_.flat_pat, (rfc() ? x86("mov", "rax", rspq((int)_.op_fc_disp + 8)) : x86("mov", "rax", FRQ(_.op_off + 8)))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", RDQ("rcx", 0), "rax"))
         + (_.op_dval != 0.0 ? IF(rfc(), x86("mov", "eax", rspd((int)_.op_fc_disp)) + (ZC_FRAME == ZC_FRAME_RSP && !_.flat_pat ? x86("mov", rspd((int)(_.op_off + _.op_fc_disp + 32)), "eax") : x86("mov", FR(_.op_off), "eax")))
                             + (ZC_FRAME == ZC_FRAME_RSP && !_.flat_pat && rfc() ? x86("mov", rspq((int)(_.op_off + 24 + _.op_fc_disp + 32)), "r14") : x86("mov", FRQ(_.op_off + 24), "r14")) : std::string())
         + IF(ZC_FRAME != ZC_FRAME_RSP || _.flat_pat, IF(rfc(),  x86("mov",  "rdi", rspq((int)_.op_fc_disp + 8)))
             + x86_align_enter()
             + IF(!rfc(), x86("mov",  "rdi", FRQ(_.op_off + 8)))
             + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to))
         + (rfc() ? x86_zls2_release_to_rspd((int)_.op_fc_disp + 16) : x86_zls2_release_to_call(_.op_off + 16))
         + x86_align_leave()
         /* ⛔ THE XFER WINDOW MUST OPEN *AFTER* THE ZETA RELEASE.  x86_zls2_release_to_call does not merely
          * call — it RESETS rsp to the statement's zeta mark (`mov rsp, [frame+off]`, closing and re-opening
          * the align dance around itself).  Pushes taken before it are ABANDONED by that reset, and the
          * matching pops then read garbage off the zeta region — a segfault in mode-4 and silent corruption in
          * mode-3.  Cost of learning this: one full crosscheck (m4 167/124).  Save AFTER the release. */
         + release_pump();

}
