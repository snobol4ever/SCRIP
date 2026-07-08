#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_var_ref_cell(DESCR_t *cellp);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var_ref() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86("def", "α") + x86_bomb("bb_var_ref: needs own slot");
    if (_.op_gva_k >= 0)
        return x86("comment", "IR_VAR_REF gva")
             + x86("def",     "α")
             + x86("lea",     "rdi", RDQ("rbx", _.op_gva_k * 16))
             + x86("call",    "rt_var_ref_cell", (uint64_t)(uintptr_t)(void *)rt_var_ref_cell)
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("jmp",     "ω");
    if (_.op_sa >= 0)
        return x86("comment", "IR_VAR_REF local")
             + x86("def",     "α")
             + x86("lea",     "rdi", FRQ(_.op_sa))
             + x86("call",    "rt_var_ref_cell", (uint64_t)(uintptr_t)(void *)rt_var_ref_cell)
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86("jmp",     "γ")
             + x86("def",     "β")
             + x86("jmp",     "ω");
    return x86("def", "α") + x86_bomb("bb_var_ref: variable has no addressable cell (NV-fallback global?) — tvsubs needs a GVA/frame cell");
}
