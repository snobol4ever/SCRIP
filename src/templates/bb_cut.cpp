#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" void rt_cut_set(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cut() {
    if (PLATFORM_X86) return x86("label", _.lbl_α)
                           + x86("comment", "IR_CUT")
                           + x86("call", "rt_cut_set", (uint64_t)(uintptr_t)(void*)rt_cut_set)
                           + x86("jmp", "γ")
                           + x86("def", "β")
                           + x86("jmp", "γ");
    return std::string();
}