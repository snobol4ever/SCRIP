#include <string>
#include <stdint.h>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern DESCR_t rt_var_ref_cell(DESCR_t *cellp);
extern DESCR_t rt_pl_fresh_var_ref(void);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var_ref() {
    x86_begin();
    if (_.op_off == -1) return x86_alpha() + x86_bomb("bb_var_ref: needs own slot");
    if (_.op_gva_k < 0 && _.op_sa == -1) {
        return x86("comment", "IR_VAR_REF anon: rt_pl_fresh_var_ref -> PLJ PLVAR cell")
             + x86_alpha()
             + x86("call", "rt_pl_fresh_var_ref", (uint64_t)(uintptr_t)(void *)rt_pl_fresh_var_ref)
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    }
    if (_.op_zres && (_.op_sa >= 0 || _.op_gva_k >= 0) && !(g_emit_cfg && g_emit_cfg->pl_cells_graph))
        return x86("comment", "IR_VAR_REF icn cells zd: NAMETRAP{DT_N,slen=1,&____slot} -> ZRES")
             + x86_alpha()
             + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + (_.op_gva_k >= 0
                 ? x86("note", gva_name(_.op_gva_k))
                 + x86("mov", "rax", (long)(RT_GVA_VA + _.op_gva_k * 16))
                 : x86("lea", "rax", FRQ(_.op_sa)))
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rax")
             + x86_gamma()
             + x86_beta_trampoline();
    return x86("comment", "IR_VAR_REF")
         + x86_alpha()
         + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))
         + (_.op_gva_k >= 0
             ? x86("note", gva_name(_.op_gva_k))
             + x86("mov", "rdx", (long)(RT_GVA_VA + _.op_gva_k * 16))
             : x86("lea", "rdx", FRQ(_.op_sa)))
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline();
}
