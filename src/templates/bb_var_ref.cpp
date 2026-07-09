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
    if (PLATFORM_X86)
        return _.op_off < 0 ? x86_alpha() + x86_bomb("bb_var_ref: needs own slot")
             : _.op_gva_k < 0 && _.op_sa < 0 ? x86_alpha() + x86_bomb("bb_var_ref: variable has no addressable cell (NV-fallback global?) — tvsubs needs a GVA/frame cell")
             : x86("comment", "IR_VAR_REF")
             + x86_alpha()
             + x86("lea",     "rdi", _.op_gva_k >= 0 ? RDQ("rbx", _.op_gva_k * 16) : FRQ(_.op_sa))
             + x86("call",    "rt_var_ref_cell", (uint64_t)(uintptr_t)(void *)rt_var_ref_cell)
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    return std::string();
}
