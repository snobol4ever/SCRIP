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
static const char * frame_display_reg(int lvl) { return lvl == 1 ? "r13" : lvl == 2 ? "r14" : lvl == 3 ? "r15" : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int frame_display_mem_off(int lvl, const char * owner_proc) {
    if (lvl <= 3 || !owner_proc) return -1;
    const char * l3proc = stage2_owner_l3_ancestor(owner_proc);
    if (!l3proc) return -1;
    char nm[32]; snprintf(nm, sizeof nm, "__pas_display_%d", lvl);
    return stage2_owner_varslot(l3proc, nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var_frame() {
    x86_begin();
    int lvl = _.node ? _.node->seal : 0;
    const char * dreg = frame_display_reg(lvl);
    if (_.op_sval && !strcmp(_.op_sval, "__pas_display")) {
        if (!dreg || _.op_off < 0) return x86_alpha() + x86_bomb("bb_var_frame: display[L] read needs L in 1..3 and a result slot") + x86_beta_trampoline();
        return x86("comment", "IR_VAR_FRAME __pas_display: display[L] itself, as an integer, for a procedural parameter's closure (ISO 7185 6.6.3.4)")
             + x86_alpha()
             + x86("mov", FRQ(_.op_off), (long)DT_I)
             + x86("mov", FRQ(_.op_off + 8), dreg)
             + x86_gamma()
             + x86_beta_trampoline();
    }
    int off = (_.op_a_sval && _.op_sval) ? stage2_owner_varslot(_.op_a_sval, _.op_sval) : -1;
    if (!dreg) {
        int disp_off = frame_display_mem_off(lvl, _.op_a_sval);
        if (disp_off < 0 || off < 0 || _.op_off < 0) return x86_alpha() + x86_bomb("bb_var_frame: PAS-DISPLAY level>3 slot unresolved") + x86_beta_trampoline();
        return x86("comment", "IR_VAR_FRAME: uplevel read via level>3 spilled display")
             + x86_alpha()
             + x86("mov", "rcx", RDQ("r15", disp_off))
             + x86("mov", "rax", RDQ("rcx", off))
             + x86("mov", FRQ(_.op_off),     "rax")
             + x86("mov", "rax", RDQ("rcx", off + 8))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline();
    }
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
