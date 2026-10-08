#include <string>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int stage2_owner_varslot(const char * proc, const char * var);
const char * stage2_owner_l3_ancestor(const char * proc);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define VF_PD() (_.op_sval && !strcmp(_.op_sval, "__pas_display"))
std::string bb_var_frame() {
    x86_begin();
    return IF(VF_PD() && (!_.op_name1 || _.op_off < 0),
               x86_alpha() + x86_bomb("bb_var_frame: display[L] read needs L in 1..3 and a result slot") + x86_beta_trampoline())
         + IF(VF_PD() && _.op_name1 && _.op_off >= 0,
               x86("comment", "IR_VAR_FRAME __pas_display: display[L] itself, as an integer, for a procedural parameter's closure (ISO 7185 6.6.3.4)")
             + x86_alpha()
             + x86("mov", FRQ(_.op_off), (long)DT_I)
             + x86("mov", FRQ(_.op_off + 8), _.op_name1)
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!VF_PD() && !_.op_name1 && (_.op_sb < 0 || _.op_sa < 0 || _.op_off < 0),
               x86_alpha() + x86_bomb("bb_var_frame: PAS-DISPLAY level>3 slot unresolved") + x86_beta_trampoline())
         + IF(!VF_PD() && !_.op_name1 && !(_.op_sb < 0 || _.op_sa < 0 || _.op_off < 0),
               x86("comment", "IR_VAR_FRAME: uplevel read via level>3 spilled display")
             + x86_alpha()
             + x86("mov", "rcx", RDQ("r15", _.op_sb))
             + x86("mov", "rax", RDQ("rcx", _.op_sa))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", "rax", RDQ("rcx", _.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!VF_PD() && _.op_name1 && (_.op_sa < 0 || _.op_off < 0),
               x86_alpha() + x86_bomb("bb_var_frame: owner vslot unresolved") + x86_beta_trampoline())
         + IF(!VF_PD() && _.op_name1 && !(_.op_sa < 0 || _.op_off < 0),
               x86("comment", "IR_VAR_FRAME: uplevel read via display")
             + x86_alpha()
             + x86("mov", "rax", RDQ(_.op_name1, _.op_sa))
             + x86("mov", FRQ(_.op_off), "rax")
             + x86("mov", "rax", RDQ(_.op_name1, _.op_sa + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline());
}
