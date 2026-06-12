#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
void rt_gvar_assign_var(const char * dst, const char * src);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * dst_name()  { return _.op_sval   ? _.op_sval   : ""; }
static inline uint64_t     dst_addr()  { return (uint64_t)(uintptr_t)dst_name(); }
static inline const char * dst_label() { const char * l = emit_intern_str(dst_name()); if (l) return l; static char b[64]; strtab_label(b, sizeof b, dst_name()); return b; }
static inline const char * src_name()  { return _.op_a_sval ? _.op_a_sval : ""; }
static inline uint64_t     src_addr()  { return (uint64_t)(uintptr_t)src_name(); }
static inline const char * src_label() { const char * l = emit_intern_str(src_name()); if (l) return l; static char b[64]; strtab_label(b, sizeof b, src_name()); return b; }
static inline uint64_t     fn_var()    { void (*f)(const char *, const char *) = rt_gvar_assign_var; return (uint64_t)(uintptr_t)(void *)f; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_gvar_assign_var() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", "BOX IR_ASSIGN_VAR store = read(src) [RO ptrs, @PLT]"))
             + x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
             + x86("lea",  "rsi", "[rip + __]", src_addr(), src_label())
             + x86("call", "rt_gvar_assign_var", fn_var())
             + x86("jmp",  "\xCE\xB3")
             + x86("def",  "\xCE\xB2")
             + x86("jmp",  "\xCF\x89");
    return std::string();
}