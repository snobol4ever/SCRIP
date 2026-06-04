#include <string>
#include <cstring>
#include <cstdint>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * subj_chars() { return _.op_a_sval; }
static inline const char * subj_label() { return emit_intern_str(subj_chars() ? subj_chars() : ""); }
static inline uint64_t     subj_addr()  { return (uint64_t)(uintptr_t)(const void *)subj_chars(); }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_subject_str() {
    if (!PLATFORM_X86) return std::string();
    if (!subj_chars()) return x86_bomb("IR_SUBJECT: non-literal subject (PB-RB ladder)");
    int  sa  = _.op_sa;
    long len = (long)strlen(subj_chars());
    if (sa < 0) return x86_bomb("IR_SUBJECT: subject slot not promoted (flat_drive_subject)");
    return IF(MEDIUM_TEXT,
               s_1asm(std::string(_.lbl_α) + ":")
             + s_comment("# BOX SUBJECT  [REG-0 sigma ptr + Delta len -> zeta-slot, x86() self-encoding]"))
         + x86("mov", "rax", "[rip + __]", subj_addr(), subj_label())
         + x86("mov", FRQ(sa), "rax")
         + x86("mov", FR(sa + 8), len)
         + x86("jmp", PORT_GAMMA)
         + x86("def", PORT_BETA)
         + x86("jmp", PORT_OMEGA);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_subject(void) {
    x86_begin();
    bb_emit_x86(bb_subject_str());
}
