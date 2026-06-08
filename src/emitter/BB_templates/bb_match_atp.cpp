#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void rt_at_cursor(const char *varname, int cur_delta);
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * atp_varname() { return _.op_sval ? _.op_sval : ""; }
static inline const char * atp_label()   { const char * l = emit_intern_str(atp_varname()); if (l) return l;
                                           static char b[24]; strtab_label(b, sizeof b, atp_varname()); return b; }
static inline uint64_t     atp_var_addr(){ return (uint64_t)(uintptr_t)(const void *)atp_varname(); }
static inline uint64_t     atp_fn_ptr()  { void (*fp)(const char *, int) = rt_at_cursor; return (uint64_t)(uintptr_t)(void *)fp; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_atp_str() {
    if (PLATFORM_X86) {
        if (!atp_varname()[0]) return std::string();
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", "BOX ATP(@" + std::string(atp_varname()) + ")  [REG-3 δ=r14, x86() self-encoding]"))
             + x86("mov",  "esi", "r14d")
             + x86("lea",  "rdi", "[rip + __]", atp_var_addr(), atp_label())
             + x86("sub",  "rsp", (long)8)
             + x86("call", "rt_at_cursor", atp_fn_ptr())
             + x86("add",  "rsp", (long)8)
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_match_atp(void) { bb_emit_x86(bb_match_atp_str()); }
