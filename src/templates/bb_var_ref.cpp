#include <string>
#include <stdint.h>
#include <string.h>
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
             : _.op_gva_k < 0 && _.op_sa < 0 ? x86_alpha() + x86_bomb(strdup((std::string("bb_var_ref: variable '") + (_.op_sval ? _.op_sval : "?")
                 + "' has no addressable cell (NV-fallback global?) — needs a GVA/frame cell").c_str()))
             : x86("comment", "IR_VAR_REF")
             + x86_alpha()
             + x86("mov", "rax", (long)0x100000009L)
             + (_.op_gva_k >= 0
                 ? x86("note", gva_name(_.op_gva_k)) + x86("mov", "rdx", (long)(RT_GVA_VA + _.op_gva_k * 16))
                 : x86("lea", "rdx", FRQ(_.op_sa)))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    return std::string();
}
