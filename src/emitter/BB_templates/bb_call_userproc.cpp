#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "emit_bb.h"
void rt_call_proc(const char *name, int nargs);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_userproc_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_CALL")
         + x86("label",   _.lbl_α)
         + x86("mov",     "rdi", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""))
         + x86("mov32",   "esi", (long)_.op_ival)
         + x86("call",    "rt_call_proc", (unsigned long long)(uintptr_t)(void*)rt_call_proc)
         + x86("jmp",     "γ")
         + x86("def",     "β")
         + x86("jmp",     "ω");
}
