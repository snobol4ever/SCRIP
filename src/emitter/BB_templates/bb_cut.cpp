#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" void rt_cut_set(void);
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cut_str() {
    if (!PLATFORM_X86) return std::string();
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX RESOLVE_CUT  [x86() self-encoding]"))
         + x86("call", "rt_cut_set", (uint64_t)(uintptr_t)(void*)rt_cut_set)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "γ");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cut(void) { bb_emit_x86(bb_cut_str()); }
