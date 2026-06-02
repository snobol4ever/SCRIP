/* bb_pat_defer.cpp — IR_PAT_DEFER: runtime-resolved pattern-valued variable.  x86() self-encoding
   (template-revamp, 2026-06-02).  sval = varname; ival = 0 for direct (PAT in pattern pos),
   1 for *V deref form.  REG-3: δ=R14d (ratified, established by BB_MATCH α per REG-0).
   α: call rt_defer_match(varname, ival, r14d) → eax = new_δ (≥0) or -1 (fail).
      if eax < 0 → ω; δ = eax; → γ.
   β: → ω directly (single-attempt, no retry).
   16-byte stack alignment around the call: sub-pattern path hits SSE stores (exec_stmt); rsp
   must be aligned on entry to rt_defer_match.  Push r10 (caller-saved, ABI convention used by
   bb_lit/bb_match) + rbx (callee-saved) to save/restore rsp, then and rsp,-16 before call.
   r14/r15 are callee-saved and survive the call; δ lives in r14d.
   pBB-free: operands from _.op_sval / _.op_ival (promoted at dispatch). */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
extern "C" int rt_defer_match(const char *varname, int ival_flag, int cur_delta);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * dvar_chars()  { return _.op_sval ? _.op_sval : ""; }
static inline const char * dvar_label()  { return emit_intern_str(dvar_chars()); }
static inline uint64_t     dvar_addr()   { return (uint64_t)(uintptr_t)(const void *)dvar_chars(); }
static inline int          defer_ival()  { return (int)(int64_t)_.op_ival; }
static inline uint64_t     defer_fn()    { int (*fp)(const char *, int, int) = rt_defer_match;
                                           return (uint64_t)(uintptr_t)(void *)fp; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_defer_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment(emit_fmt("# BOX DEFER(%s%s)  [REG-3 δ=r14, x86() self-encoding]",
                                       defer_ival() ? "*" : "", dvar_chars())))
             /* rdi = varname (RO pointer: lea [rip+label] TEXT, movabs BINARY) */
             + x86("lea",  "rdi", "[rip + __]", dvar_addr(), dvar_label())
             /* esi = ival flag */
             + x86("mov",  "esi", (long)defer_ival())
             /* edx = current δ (the cursor passed to rt_defer_match) */
             + x86("mov",  "edx", "r14d")
             /* Save caller-saved r10 + callee-saved rbx so we can restore rsp cleanly */
             + x86("push", "r10")
             + x86("push", "rbx")
             /* rbx = rsp (remember pre-alignment rsp) */
             + IF(MEDIUM_TEXT,  std::string(" mov rbx, rsp\n"))
             + IF(MEDIUM_BINARY, x86_Lrec(x86_b3(0x48, 0x89, 0xE3)))
             /* rsp &= -16 (force 16-byte alignment for sub-pattern SSE path) */
             + IF(MEDIUM_TEXT,  std::string(" and rsp, -16\n"))
             + IF(MEDIUM_BINARY, x86_Lrec(std::string("\x48\x83\xE4\xF0", 4)))
             /* call rt_defer_match */
             + x86("call", "rt_defer_match", defer_fn())
             /* restore rsp via rbx */
             + IF(MEDIUM_TEXT,  std::string(" mov rsp, rbx\n"))
             + IF(MEDIUM_BINARY, x86_Lrec(x86_b3(0x48, 0x89, 0xDC)))
             + x86("pop",  "rbx")
             + x86("pop",  "r10")
             /* if eax < 0 (sign bit set) → ω */
             + x86("test", "eax", "eax")
             + x86("js",   PORT_OMEGA)
             /* δ = eax (new cursor after match) */
             + x86("mov",  "r14d", "eax")
             /* → γ */
             + x86("jmp",  PORT_GAMMA)
             /* β: → ω (no retry) */
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_defer(void) { bb_emit_x86(bb_pat_defer_str()); }
