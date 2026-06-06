#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline long lenN() { return (long)(int)_.op_ival; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_len_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", emit_fmt("BOX LEN(%ld)  [Σ=r13 δ=r14 Δ=r15, x86() self-encoding]", lenN())))
             + x86("mov", "eax", "r14d")
             + x86("add", "eax", lenN())
             + x86("cmp", "eax", "r15d")
             + x86("jg",  "\xCF\x89")
             + x86("add", "r14d", lenN())
             + x86("jmp", "\xCE\xB3")
             + x86("def", "\xCE\xB2")
             + x86("jmp", "\xCF\x89");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_len(void) { bb_emit_x86(bb_pat_len_str()); }
