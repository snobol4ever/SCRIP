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
    int nid = _.nid; int sid = 0;
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", emit_fmt("BOX LEN(%ld)  [REG-2 δ=r14 Δ=r15, x86() self-encoding]", lenN())))
             + x86("mov", "eax", "r14d")
             + x86("add", "eax", lenN())
             + x86("cmp", "eax", "r15d")
             + x86("jg",  PORT_OMEGA)
             + x86("add", "r14d", lenN())
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
/*--------------------------------------------------------------------------------------------------------------------*/
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_len(void) { bb_emit_x86(bb_pat_len_str()); }
