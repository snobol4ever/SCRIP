#include <string>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
void rt_gvar_assign_descr(const char * name, int64_t lo, int64_t hi);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * dst_name()  { return _.op_sval ? _.op_sval : ""; }
static inline uint64_t     dst_addr()  { return (uint64_t)(uintptr_t)dst_name(); }
static inline const char * dst_label() { const char * l = emit_intern_str(dst_name()); if (l) return l; static char b[64]; strtab_label(b, sizeof b, dst_name()); return b; }
static inline uint64_t     fn_descr()  { void (*f)(const char *, int64_t, int64_t) = rt_gvar_assign_descr; return (uint64_t)(uintptr_t)(void *)f; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_gvar_assign_call_str() {
    if (PLATFORM_X86) {
        int slot = _.op_a_slot;
        if (slot < 0) return x86_bomb("bb_gvar_assign_call: op_a_slot==-1 (call result slot not promoted)");
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", "BOX IR_ASSIGN_CALL store = DESCR from call zeta-slot [RO dst ptr, @PLT]"))
             + x86("lea",  "rdi", "[rip + __]", dst_addr(), dst_label())
             + x86_frame_load64("rsi", slot)
             + x86_frame_load64("rdx", slot + 8)
             + x86("call", "rt_gvar_assign_descr", fn_descr())
             + x86("jmp",  "\xCE\xB3")
             + x86("def",  "\xCE\xB2")
             + x86("jmp",  "\xCF\x89");
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_gvar_assign_call(void) { bb_emit_x86(bb_gvar_assign_call_str()); }
