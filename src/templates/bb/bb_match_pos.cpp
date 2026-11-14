#include <string>
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state_.h"
#include "bb_templates.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_zeta.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_pos_body() {
    return x86("comment", "IR_MATCH_POS")
         + x86_alpha()
         + IF(_.op_sa >= 0 && _.op_zres, x86("mov", "rax", ZOPQ(0, 8)))
         + IF(_.op_sa >= 0 && !_.op_zres, x86("mov", "rax", XSAQ(8)))
         + IF(_.op_sa < 0, x86("mov", "rax", (long)_.op_sb))
         + x86("cmp", "r14d", "eax")
         + x86_omega("jne")
         + x86_gamma()
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_pos() {
    return IF(_.op_sval != NULL,
               x86("comment", "IR_MATCH_POS defer")
             + x86_alpha()
             + bb_glue_name_or_rec_lea(_.op_sval + 1)
             + bb_glue_prim_int(50, 50)
             + bb_glue_prim_err(50)
             + x86("test", "rax", "rax")
             + x86_omega("js")
             + x86("cmp", "r14d", "eax")
             + x86_omega("jne")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(_.op_sval == NULL, bb_match_pos_body());
}
