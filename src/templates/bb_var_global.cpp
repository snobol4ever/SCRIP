#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int g_gva_active;
DESCR_t NV_GET_fn(const char * name);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * vg_res(int w) { return _.op_zres ? ZRES(w) : FRQ(_.op_off + w); }   /* ZD-1 (Lon s21x-v): the ONE destination switch -- under op_zres the result IS the box's own alpha-carved cell (mode 1 of the FOUR), read downstream via the staged op_zread differences; otherwise the legacy flat slot.  Same body serves both regimes, zero arm duplication. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var_global() {
    if (PLATFORM_X86)
        return !_.op_zres && _.op_off < 0 ? x86_alpha() + x86_bomb("bb_var_global: unhandled (needs descr flat-chain + own slot)")
             : g_gva_active && _.op_gva_k >= 0 ?
               x86("comment", "IR_VAR")
             + x86_alpha()
             + x86("note", gva_name(_.op_gva_k)) + x86("mov",    "rax", ABSQ(RT_GVA_VA + _.op_gva_k * 16))
             + x86("note", gva_name(_.op_gva_k)) + x86("mov",    "rdx", ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))
             + x86("mov",    vg_res(0), "rax")
             + x86("mov",    vg_res(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             : x86("comment", "IR_VAR")
             + x86_alpha()
             + x86("mov",    "rdi", ROQ(0))
             + x86("call",   "NV_GET_fn", (uint64_t)(uintptr_t)(void *)NV_GET_fn)
             + x86("cmp",    "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov",    vg_res(0), "rax")
             + x86("mov",    vg_res(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  LS(0), _.op_sval)
             + x86("label",  LS(0))
             + x86(".string", _.op_sval);
    return std::string();
}
