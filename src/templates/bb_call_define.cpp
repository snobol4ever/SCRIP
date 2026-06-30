#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
extern DESCR_t rt_proc_define(const char * spec);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_define() {
    if (!PLATFORM_X86) return std::string();
    return x86("label", _.lbl_α)
         + x86("comment", "IR_CALL_DEFINE")
         + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t) _.op_sval, _.op_parts_lbl[0])
         + x86("call", "rt_proc_define", (uint64_t)(uintptr_t)(void *) rt_proc_define)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
