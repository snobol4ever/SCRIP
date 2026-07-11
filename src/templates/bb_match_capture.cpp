#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" long rt_cap_open(const char *varname, int saved_delta, int cur_delta, int is_imm);
extern "C" void *rt_frame_prep(void *fb, long fbytes);
extern "C" void rt_cap_finish(DESCR_t fret);
extern "C" void rt_cap_push(void *slot, int delta);
extern "C" void rt_cap_pop(void *slot);
extern "C" int rt_cap_top(void *slot);
extern "C" void rt_dcap_pop(void);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SN4-PAT-CAPTURE-STACK (Lon directive 2026-07-05).  Capture frames live on a per-box stack in the SAVE
 * box's 16B zls slot (+0 GC u32[] buf: [0]=cap, frames from [1]; +8 gen; +12 sp) so the β-resume chain
 * survives generator re-entry between capture-open and capture-close.  _.op_phase = 0 SAVE / 1 COND / 2 IMM;
 * for 1/2 _.op_off = the SAVE box's slot (COND.operands[1]).
 *   phase 0 (SAVE) — α: rt_cap_push(&slot, δ) — forward entry pushes a frame; jmp γ (inner entry).
 *                    β: rt_cap_pop(&slot) — backtrack past the open pops it; jmp ω (the capture's fail,
 *                    wired β-aware by LOWER so a generator to the LEFT resumes).
 *   phase 1/2      — α: δs = rt_cap_top(&slot); rt_cap_assign_cursor(name, δs, δ, is_imm); jmp γ.  Fires at
 *                    every inner yield, so a resumed generator re-assigns the extended span (last one wins).
 *                    β: jmp ω — the backtrack-in edge; LOWER points ω at the inner generator's β (extend)
 *                    or at SAVE's β (deterministic inner: pop and fail leftward). */
std::string bb_match_capture() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    if (_.op_off < 0) return x86_alpha() + x86_bomb("IR_MATCH_ASSIGN: capture stack slot not promoted (flat_drive_capture)");
    if (!(_.op_sval ? _.op_sval : "")[0]) return x86_alpha() + x86_bomb("IR_MATCH_ASSIGN: empty capture variable name");
    if ((int)_.op_phase == 0)
        return x86("comment", "IR_MATCH_CAPTURE_SAVE push")
             + x86_alpha()
             + x86("lea",  "rdi", FR(_.op_off))
             + x86("mov",  "esi", "r14d")
             + x86_align_enter()
             + x86("call", "rt_cap_push", (uint64_t)(uintptr_t)(void *)(void (*)(void *, int))rt_cap_push)
             + x86_align_leave()
             + x86_gamma()
             + x86_beta()
             + x86("lea",  "rdi", FR(_.op_off))
             + x86_align_enter()
             + x86("call", "rt_cap_pop", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_cap_pop)
             + x86_align_leave()
             + x86_omega();
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    return x86("comment", (int)_.op_phase == 2 ? "IR_MATCH_CAPTURE_IMM" : "IR_MATCH_CAPTURE_COND")
         + x86_alpha()
         + x86_align_enter()
         + x86("lea",  "rdi", FR(_.op_off))
         + x86("call", "rt_cap_top", (uint64_t)(uintptr_t)(void *)(int (*)(void *))rt_cap_top)
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
         + x86("mov",  "esi", "eax")
         + x86("mov",  "edx", "r14d")
         + x86("mov",  "ecx", (long)((int)_.op_phase == 2 ? 1 : 0))
         /* NCB-1c M2 (2026-07-11): the *VAR computed-name transfer is EMITTED — open returns fbytes (a proc
          * call owed) or 0 (handled).  The window is the NCB-1b arm verbatim: sub rsp IS the frame, call rax
          * IS the BB→BB bridge, align_leave IS the release (F2).  Every exit passes through align_leave. */
         + x86("call", "rt_cap_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int, int, int))rt_cap_open)
         + x86("test", "rax", "rax")
         + x86("je",   L(1))
         + x86("sub",  "rsp", "rax")
         + x86("mov",  "rdi", "rsp")
         + x86("mov",  "rsi", "rax")
         + x86("call", "rt_frame_prep", (uint64_t)(uintptr_t)(void *)(void *(*)(void *, long))rt_frame_prep)
         + x86("mov",  "rdi", "rsp")
         + x86("xor",  "esi", "esi")
         + x86("call", "rax")
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_cap_finish", (uint64_t)(uintptr_t)(void *)(void (*)(DESCR_t))rt_cap_finish)
         + x86("def",  L(1))
         + x86_align_leave()
         + x86_gamma()
         + x86_beta()
         /* PUSH/POP MODEL (Lon directive 2026-07-11, s21): the pend ring is a pure stack — COND.α pushes a
          * pend (rt_dcap_record, upsert deleted), COND.β pops it (the Python Δ model: pop after yield fires
          * exactly when the match backtracks in to re-pump the subpattern; oracle p5 'AXB'?'A' ARB . OUTPUT
          * ('B'|'C') prints X only — the abandoned trial's pend must NOT survive a re-yield).  Phase 2 (IMM)
          * bypasses the ring, so no pop.  s7's unpend-on-β revert note is SUPERSEDED: that experiment failed
          * because ALT's try-next-alternative edge routes through this β — if p3-class probes regress, the
          * fix is in LOWER (alt-switch must restore its own cursor, not ride the capture's β), not here. */
         + IF((int)_.op_phase == 1,
               x86_align_enter()
             + x86("call", "rt_dcap_pop", (uint64_t)(uintptr_t)(void *)(void (*)(void))rt_dcap_pop)
             + x86_align_leave())
         + x86_omega();
}
