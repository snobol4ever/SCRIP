#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" void mon_emit_label_bin(int64_t stno);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_succeed() {
        return x86("comment", "IR_SUCCEED")
             + x86_alpha()
             + x86_gamma()
             + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void emit_mon_label_tap(int32_t stno) {
    extern int g_mon_max_stno;
    if (stno > g_mon_max_stno) g_mon_max_stno = stno;
    _.op_stno = stno;
    bb_emit_x86(
          x86("comment", "MON LABEL tap")
        + x86("mov",  "rdi", (long)_.op_stno)
        + x86("call", "mon_emit_label_bin", (uint64_t)(uintptr_t)(void *)mon_emit_label_bin));
}
