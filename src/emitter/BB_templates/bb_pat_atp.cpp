/* bb_pat_atp.cpp — BB template for @var cursor capture (SBL-ATP). x86() self-encoding (template-revamp,
   2026-06-02, Opus 4.8). @var is a zero-width single-shot leaf: α writes the current 0-based cursor δ to
   var as an integer (via rt_at_cursor), then succeeds to γ; β fails to ω — a single attempt, like a leaf
   POS (SPITBOL Manual ch.18 immediate-assignment family; mirrors the mode-2 oracle bb_exec.c IR_PAT_ATP
   and lower.c TT_CAPT_CURSOR). REG-3 registers: cursor δ=R14d (ratified, established by BB_MATCH α per
   REG-0) — the legacy [r10] cursor cell is GONE (δ read straight from r14d). rt_at_cursor(varname, δ) is
   side-effecting (NV_SET on an associated/OUTPUT var can enter the print path which clobbers caller-saved
   r10), so push r10 TWICE around the call — preserve r10 AND keep rsp 16-aligned. The varname is a RO
   constant reached IP-relative (lea[rip] in TEXT / movabs in BINARY — REG-RO unifies later, encapsulated in
   x86_load_ro). The varname is _.op_sval (identical to the op_name1 the driver fills for IR_PAT_ATP).
   x86 arm: ONE return, pure x86() concat, NO bb_bin_t, medium invisible, pBB-free. X86 ONLY — no other arm. */
#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void rt_at_cursor(const char *varname, int cur_delta);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * atp_varname() { return _.op_sval ? _.op_sval : ""; }
static inline const char * atp_label()   { return emit_intern_str(atp_varname()); }
static inline uint64_t     atp_var_addr(){ return (uint64_t)(uintptr_t)(const void *)atp_varname(); }
static inline uint64_t     atp_fn_ptr()  { void (*fp)(const char *, int) = rt_at_cursor; return (uint64_t)(uintptr_t)(void *)fp; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_atp_str() {
    if (PLATFORM_X86) {
        if (!atp_varname()[0]) return std::string();   /* no varname — honest skip */
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment(emit_fmt("# BOX ATP(@%s)  [REG-3 δ=r14, x86() self-encoding]", atp_varname())))
             + x86("mov",  "esi", "r14d")                                  /* esi = cur_delta = δ   */
             + x86("lea",  "rdi", "[rip + __]", atp_var_addr(), atp_label())
             + x86("push", "r10")                                          /* preserve &cursor      */
             + x86("push", "r10")                                          /* keep rsp 16-aligned   */
             + x86("call", "rt_at_cursor", atp_fn_ptr())
             + x86("pop",  "r10")
             + x86("pop",  "r10")
             + x86("jmp",  PORT_GAMMA)
             + x86("def",  PORT_BETA)
             + x86("jmp",  PORT_OMEGA);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_atp(void) { bb_emit_x86(bb_pat_atp_str()); }
