#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" void mon_emit_label_bin(int64_t stno);
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_succeed() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_SUCCEED")
         + x86("label",   _.lbl_α)
         + x86("jmp",     "γ")
         + x86("def",     "β")
         + x86("jmp",     "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_mon_label_tap() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "MON LABEL tap")
         + x86("mov",  "rdi", (long)_.op_stno)
         + x86("call", "mon_emit_label_bin", (uint64_t)(uintptr_t)(void *)mon_emit_label_bin);
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_mon_label_tap(int32_t stno) {
    if (!PLATFORM_X86) return;
    { extern int g_mon_max_stno; if (stno > g_mon_max_stno) g_mon_max_stno = stno; }
    _.op_stno = stno;
    bb_emit_x86(bb_mon_label_tap());
}
