#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "emit.h"
#include "emit_bb.h"
void rt_call_builtin(const char *name, int nargs);
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_fn_str(IR_t * pBB) {
    if (!PLATFORM_X86) return std::string();
    const char * fn   = _.op_sval ? _.op_sval : "";
    int64_t      narg = _.op_ival;
    if (MEDIUM_TEXT) {
        return x86("label", _.lbl_α)
             + x86("ins2", "call", "rt_call_builtin@PLT")
             + x86("ins2", "jmp",  _.lbl_γ)
             + x86("Lins1", emit_fmt("%s:", _.lbl_β), "")
             + x86("ins2", "jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t fptr; { void (*fp)(const char *, int) = rt_call_builtin; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        std::string s;
        s += x86("mov", "rdi", (uint64_t)(uintptr_t)fn);
        s += x86("mov32", "esi", (long)narg);
        s += x86("call", "rt_call_builtin", fptr);
        s += x86("jmp", PORT_GAMMA);
        s += x86("def", PORT_BETA);
        s += x86("jmp", PORT_OMEGA);
        return s;
    }
    return std::string();
}
