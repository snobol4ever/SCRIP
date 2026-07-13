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
extern "C" const char *g_dcap_top;
extern "C" void rt_dcap_end_ok_close(void);
extern "C" void *rt_frame_prep(void *fb, long fbytes);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_release() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_RELEASE: head slot not resolved (operand[0] missing or unowned)")
         : x86("comment", "IR_MATCH_RELEASE")
         + x86_alpha()
         + (_.op_dval != 0.0 ? x86("mov", FRQ(_.op_off + 24), "r14") : std::string())
         + x86_align_enter()
         + x86("mov",  "rdi", FRQ(_.op_off + 8))
         + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)
         + x86_zls2_release_to_call(_.op_off + 16)
         + x86_align_leave()
         /* ⛔ THE XFER WINDOW MUST OPEN *AFTER* THE ZETA RELEASE.  x86_zls2_release_to_call does not merely
          * call — it RESETS rsp to the statement's zeta mark (`mov rsp, [frame+off]`, closing and re-opening
          * the align dance around itself).  Pushes taken before it are ABANDONED by that reset, and the
          * matching pops then read garbage off the zeta region — a segfault in mode-4 and silent corruption in
          * mode-3.  Cost of learning this: one full crosscheck (m4 167/124).  Save AFTER the release. */
         + x86_xfer_enter()
         + x86_anchor_enter()
         /* rbp-dcap: mirror rbp out FIRST (a *VAR commit transfer below may run a nested match whose head
          * loads g_dcap_top — it must see the live top, not a stale-low value that would overwrite the very
          * pends we are flushing), then hand the pump its FIXED range and subject BY VALUE: rdi = MARK (head's
          * saved cursor), rsi = TOP (live rbp), rdx = subject base (r13 — immune to mid-pump Σ clobber by a
          * nested match; the old ring snapshotted base pointers at record time, the pointer-free entry moves
          * that immunity into the ctx). */
         + x86("mov",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_dcap_top, "g_dcap_top")
         + x86("mov",  RDQ("rcx", 0), "rbp")
         + x86("mov",  "rdi", FRQ(_.op_off + 32))
         + x86("mov",  "rsi", "rbp")
         + x86("mov",  "rdx", "r13")
         + x86("call", "rt_dcap_end_ok_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, const char *, const char *))rt_dcap_end_ok_open)
         /* NCB-1c M3: the 0..N computed-name (*VAR) commit transfers are EMITTED — a pump.  open returns
          * fbytes (a call owed) or 0 (flush complete); each transfer is the NCB-1b window; step assigns the
          * returned NAME and says whether another is owed.  x86_xfer_* saves r14/r15/r13: a *VAR proc body may
          * run its own match and would otherwise clobber the cursor (the M1 finding). */
         + x86("def",  L(1))
         + x86("test", "rax", "rax")
         + x86("je",   L(2))
         + x86_frame_sink()
         + x86_frame_base("rdi")
         + x86("mov",  "rsi", "rax")
         + x86("call", "rt_frame_prep", (uint64_t)(uintptr_t)(void *)(void *(*)(void *, long))rt_frame_prep)
         + x86_frame_base("rdi")
         + x86("xor", "esi", "esi")
         + x86("call", "rax")
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86_frame_unsink()
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp",  L(1))
         + x86("def",  L(2))
         + x86("call", "rt_dcap_end_ok_close", (uint64_t)(uintptr_t)(void *)(void (*)(void))rt_dcap_end_ok_close)
         + x86_anchor_leave()
         + x86_xfer_leave()
         /* rbp-dcap success exit: the flush is done — truncate by storing mirror ← MARK and restore the C
          * caller's rbp (saved by head α at +40).  This mirrors head's ω exactly; between matches rbp is the
          * caller's frame pointer again and the mirror is back at this match's floor. */
         + x86("mov", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_dcap_top, "g_dcap_top")
         + x86("mov", "rax", FRQ(_.op_off + 32))
         + x86("mov", RDQ("rcx", 0), "rax")
         + x86("mov", "rbp", FRQ(_.op_off + 40))
         + x86_gamma();
}
