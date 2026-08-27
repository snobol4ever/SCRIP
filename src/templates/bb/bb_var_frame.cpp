#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int stage2_owner_varslot(const char * proc, const char * var);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * frame_display_reg(int lvl) { return lvl == 1 ? "r13" : lvl == 2 ? "r14" : lvl == 3 ? "r15" : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var_frame() {
    x86_begin();
    int lvl = _.node ? _.node->seal : 0;
    const char * dreg = frame_display_reg(lvl);
    int off = (_.op_a_sval && _.op_sval) ? stage2_owner_varslot(_.op_a_sval, _.op_sval) : -1;
    if (!dreg)                    return x86_alpha() + x86_bomb("bb_var_frame: PAS-DISPLAY L>=4 fallback unimplemented") + x86_beta_trampoline();
    if (off < 0 || _.op_off < 0)  return x86_alpha() + x86_bomb("bb_var_frame: owner vslot unresolved") + x86_beta_trampoline();
    return x86("comment", "IR_VAR_FRAME: uplevel read via display")
         + x86_alpha()
         + x86("mov", "rax", RDQ(dreg, off))
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", "rax", RDQ(dreg, off + 8))
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86_beta_trampoline();
}
