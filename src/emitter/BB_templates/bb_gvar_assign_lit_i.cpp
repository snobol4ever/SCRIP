#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
void rt_gvar_assign_int(const char * name, int64_t val);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * dst_name()  { return _.op_sval   ? _.op_sval   : ""; }
static inline uint64_t     dst_addr()  { return (uint64_t)(uintptr_t)dst_name(); }
static inline const char * dst_label() { const char * l = emit_intern_str(dst_name()); if (l) return l; static char b[64]; strtab_label(b, sizeof b, dst_name()); return b; }
static inline uint64_t     fn_lit_i()  { void (*f)(const char *, int64_t) = rt_gvar_assign_int; return (uint64_t)(uintptr_t)(void *)f; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_gvar_assign_lit_i_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", "BOX IR_ASSIGN(lit_i) store = imm64 [RO dst ptr, @PLT]"))
             + x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
             + x86_movabs_r64("rsi", (uint64_t)_.op_a_ival_sg)
             + x86("call", "rt_gvar_assign_int", fn_lit_i())
             + x86("jmp",  "\xCE\xB3")
             + x86("def",  "\xCE\xB2")
             + x86("jmp",  "\xCF\x89");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_gvar_assign_lit_i(void) { bb_emit_x86(bb_gvar_assign_lit_i_str()); }
