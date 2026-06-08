#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline std::string  pc_sa()    { return std::to_string((long)_.op_sa); }
static inline std::string  pc_sb()    { return std::to_string((long)_.op_sb); }
static inline std::string  pc_off()   { return std::to_string((long)_.op_off); }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pattern_cat_str() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", std::string("BOX PATTERN_CAT  [STITCH ζ=r12 frag@") + pc_off() + " <- @" + pc_sa() + " . @" + pc_sb() + "]"))
             + x86("ins2", "mov", "rcx, [r12 + " + pc_sb() + "]")
             + x86("ins2", "mov", "rdx, [r12 + " + pc_sa() + " + 8]")
             + x86("ins2", "mov", "[rdx], rcx")
             + x86("ins2", "mov", "rcx, [r12 + " + pc_sa() + " + 16]")
             + x86("ins2", "lea", "rcx, [rcx + 8]")
             + x86("ins2", "mov", "rdx, [r12 + " + pc_sb() + " + 16]")
             + x86("ins2", "mov", "[rdx], rcx")
             + x86("ins2", "mov", "rcx, [r12 + " + pc_sa() + "]")
             + x86("ins2", "mov", "[r12 + " + pc_off() + "], rcx")
             + x86("ins2", "mov", "rcx, [r12 + " + pc_sb() + " + 8]")
             + x86("ins2", "mov", "[r12 + " + pc_off() + " + 8], rcx")
             + x86("ins2", "mov", "rcx, [r12 + " + pc_sa() + " + 16]")
             + x86("ins2", "mov", "[r12 + " + pc_off() + " + 16], rcx")
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("jmp", "ω");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pattern_cat(void) {
    bb_emit_x86(bb_pattern_cat_str());
}
