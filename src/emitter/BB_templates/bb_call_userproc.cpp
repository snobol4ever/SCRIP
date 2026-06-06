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
static std::string bcup_bin() { uint64_t fptr; { void (*fp)(const char *, int) = rt_call_proc; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    return x86("mov", "rdi", (uint64_t)(uintptr_t)(_.op_sval ? _.op_sval : ""))
         + x86("mov32", "esi", (long)_.op_ival)
         + x86("call", "rt_call_proc", fptr)
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_userproc_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_TEXT) return x86("label", _.lbl_α)
                          + x86("ins2", "call", "rt_call_proc@PLT")
                          + x86("ins2", "jmp",  _.lbl_γ)
                          + x86("Lins1", std::string(_.lbl_β) + ":", "")
                          + x86("ins2", "jmp",  _.lbl_ω);
    if (MEDIUM_BINARY) return bcup_bin();
    return std::string();
}
