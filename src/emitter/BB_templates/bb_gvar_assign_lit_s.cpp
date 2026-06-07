#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
void rt_gvar_assign_str(const char * name, const char * str);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * dst_name()  { return _.op_sval   ? _.op_sval   : ""; }
static inline uint64_t     dst_addr()  { return (uint64_t)(uintptr_t)dst_name(); }
static inline const char * dst_label() { const char * l = emit_intern_str(dst_name()); if (l) return l; static char b[64]; strtab_label(b, sizeof b, dst_name()); return b; }
static inline const char * rhs_sval()  { return _.op_a_sval ? _.op_a_sval : ""; }
static inline uint64_t     rhs_addr()  { return (uint64_t)(uintptr_t)rhs_sval(); }
static inline const char * rhs_label() { const char * l = emit_intern_str(rhs_sval()); if (l) return l; static char b[64]; strtab_label(b, sizeof b, rhs_sval()); return b; }
static inline uint64_t     fn_lit_s()  { void (*f)(const char *, const char *) = rt_gvar_assign_str; return (uint64_t)(uintptr_t)(void *)f; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_gvar_assign_lit_s_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", "BOX IR_ASSIGN(lit_s) store = literal [RO ptrs, @PLT]"))
             + x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
             + x86("lea",  "rsi", "[rip + __]", rhs_addr(), rhs_label())
             + x86("call", "rt_gvar_assign_str", fn_lit_s())
             + x86("jmp",  "γ")
             + x86("def",  "β")
             + x86("jmp",  "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_gvar_assign_lit_s(void) { bb_emit_x86(bb_gvar_assign_lit_s_str()); }
