#include <string>
#include <cstdint>
#include <cstdio>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int stage2_owner_varslot(const char * proc, const char * var);
const char * stage2_owner_l3_ancestor(const char * proc);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int frame_display_mem_off(int lvl, const char * owner_proc) {
    if (lvl <= 3 || !owner_proc) return -1;
    const char * l3proc = stage2_owner_l3_ancestor(owner_proc);
    if (!l3proc) return -1;
    char nm[32]; snprintf(nm, sizeof nm, "__pas_display_%d", lvl);
    return stage2_owner_varslot(l3proc, nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_assign_frame() {
    x86_begin();
    int lvl = _.node ? _.node->seal : 0;
    const char * dreg = lvl == 1 ? "r13" : lvl == 2 ? "r14" : lvl == 3 ? "r15" : (const char *)0;
    int off = (_.op_a_sval && _.op_sval) ? stage2_owner_varslot(_.op_a_sval, _.op_sval) : -1;
    if (off < 0)           return x86_alpha() + x86_bomb("bb_assign_frame: owner vslot unresolved") + x86_beta_trampoline();
    if (_.op_a_slot < 0)   return x86_alpha() + x86_bomb("bb_assign_frame: rhs slot unresolved") + x86_beta_trampoline();
    if (!dreg) {
        int disp_off = frame_display_mem_off(lvl, _.op_a_sval);
        if (disp_off < 0) return x86_alpha() + x86_bomb("bb_assign_frame: PAS-DISPLAY level>3 slot unresolved") + x86_beta_trampoline();
        return x86("comment", "IR_ASSIGN_FRAME: uplevel write via level>3 spilled display")
             + x86_alpha()
             + x86("mov", "rax", FRQ(_.op_a_slot))
             + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
             + x86("mov", "rcx", RDQ("r15", disp_off))
             + x86("mov", RDQ("rcx", off),     "rax")
             + x86("mov", RDQ("rcx", off + 8), "rdx")
             + IF(_.op_res_live && _.op_off >= 0, x86("mov", FRQ(_.op_off),     "rax")
                                                + x86("mov", FRQ(_.op_off + 8), "rdx"))
             + x86_gamma()
             + x86_beta_trampoline();
    }
    return x86("comment", "IR_ASSIGN_FRAME: uplevel write via display")
         + x86_alpha()
         + x86("mov", "rax", FRQ(_.op_a_slot))
         + x86("mov", "rdx", FRQ(_.op_a_slot + 8))
         + x86("mov", RDQ(dreg, off),     "rax")
         + x86("mov", RDQ(dreg, off + 8), "rdx")
         + IF(_.op_res_live && _.op_off >= 0, x86("mov", FRQ(_.op_off),     "rax")
                                            + x86("mov", FRQ(_.op_off + 8), "rdx"))
         + x86_gamma()
         + x86_beta_trampoline();
}
