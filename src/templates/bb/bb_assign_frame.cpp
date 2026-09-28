#include <string>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_assign_frame() {
    x86_begin();
    if (_.op_sval && !strcmp(_.op_sval, "__pas_display")) {
        if (!_.op_name1 || _.op_a_slot < 0) return x86_alpha() + x86_bomb("bb_assign_frame: display[L] write needs L in 1..3 and an rhs slot") + x86_beta_trampoline();
        return x86("comment", "IR_ASSIGN_FRAME __pas_display: display[L] = a closure's captured frame, or its saved value back (ISO 7185 6.6.3.4)")
             + x86_alpha()
             + x86("mov", _.op_name1, FRQ(_.op_a_slot + 8))
             + x86_gamma()
             + x86_beta_trampoline();
    }
    if (_.op_sa < 0)       return x86_alpha() + x86_bomb("bb_assign_frame: owner vslot unresolved") + x86_beta_trampoline();
    if (_.op_a_slot < 0)   return x86_alpha() + x86_bomb("bb_assign_frame: rhs slot unresolved") + x86_beta_trampoline();
    if (!_.op_name1) {
        if (_.op_sb < 0) return x86_alpha() + x86_bomb("bb_assign_frame: PAS-DISPLAY level>3 slot unresolved") + x86_beta_trampoline();
        return x86("comment", "IR_ASSIGN_FRAME: uplevel write via level>3 spilled display")
             + x86_alpha()
             + x86("mov", "rax", FRQ(_.op_a_slot))
             + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
             + x86("mov", "rcx", RDQ("r15", _.op_sb))
             + x86("mov", RDQ("rcx", _.op_sa),     "rax")
             + x86("mov", RDQ("rcx", _.op_sa + 8), "rdx")
             + IF(_.op_res_live && _.op_off >= 0, x86("mov", FRQ(_.op_off),     "rax")
                                                + x86("mov", FRQ(_.op_off + 8), "rdx"))
             + x86_gamma()
             + x86_beta_trampoline();
    }
    return x86("comment", "IR_ASSIGN_FRAME: uplevel write via display")
         + x86_alpha()
         + x86("mov", "rax", FRQ(_.op_a_slot))
         + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
         + x86("mov", RDQ(_.op_name1, _.op_sa),     "rax")
         + x86("mov", RDQ(_.op_name1, _.op_sa + 8), "rdx")
         + IF(_.op_res_live && _.op_off >= 0, x86("mov", FRQ(_.op_off),     "rax")
                                            + x86("mov", FRQ(_.op_off + 8), "rdx"))
         + x86_gamma()
         + x86_beta_trampoline();
}
