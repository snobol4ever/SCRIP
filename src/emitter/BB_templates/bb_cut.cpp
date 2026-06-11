#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" void rt_cut_set(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cut_str() {
    if (PLATFORM_X86) return x86("label", _.lbl_α)
                           + x86("comment", "IR_CUT")
                           + x86("call", "rt_cut_set", (uint64_t)(uintptr_t)(void*)rt_cut_set)
                           + x86("jmp", "γ")
                           + x86("def", "β")
                           + x86("jmp", "γ");
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cut(void) { bb_emit_x86(bb_cut_str()); }
