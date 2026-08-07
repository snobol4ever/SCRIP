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
        return _.op_off == -1 ? x86_alpha() + x86_bomb("bb_var_ref: needs own slot")   /* PL-FR-2: was < 0; negative is a valid rbp-relative zframe offset; -1 is the absent sentinel */
             : _.op_gva_k < 0 && _.op_sa == -1 ? x86_alpha() + x86_bomb(strdup((std::string("bb_var_ref: variable '") + (_.op_sval ? _.op_sval : "?")
                 + "' has no addressable cell (NV-fallback global?) — needs a GVA/frame cell").c_str()))   /* PL-FR-2: op_sa < 0 -> == -1; negative is a valid rbp-relative frame offset. ⚠ OPEN (PL-FR-2): Prolog anonymous vars (G0/G1 etc) hit here; fix = allocate per-var PLJ cell at emit time (rt_plj_alloc) or grant a dedicated ZLS cell quad separate from the result slot */
             : x86("comment", "IR_VAR_REF")
             + x86_alpha()
             + x86("mov", "rax", (long)((long)1 << 32 | (long)DT_N))   /* PL-FR-2 TAG-FIX: DT_N was renumbered 9→0x28 (40) in TAG-3 commit 03cecd87; emit {v=DT_N, slen=1} so plw_entry's `v==DT_N && slen==1` guard dereferences the frame-cell pointer correctly. Was (long)0x100000009L = v=9 slen=1 = OLD DT_N. New: (long)DT_N | (1L<<32) = v=DT_N slen=1, correct for any future DT_N value. */
             + (_.op_gva_k >= 0
                 ? x86("note", gva_name(_.op_gva_k)) + x86("mov", "rdx", (long)(RT_GVA_VA + _.op_gva_k * 16))
                 : x86("lea", "rdx", FRQ(_.op_sa)))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    return std::string();
}
