#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_scan_pos() {
    x86_begin();
    if (!(_.op_off >= 0)) return x86_alpha() + x86_bomb("bb_scan_pos: no result slot (op_off)");
    return x86("comment", "BOX ICN IR_SCAN_POS pos(n) [ICN-SCAN-3 fscan.r: i=cvpos(i,len); succeed iff i==&pos (r14+1); result {DT_I,i} normalized; single-shot beta->omega]")
         + x86_alpha()
         + IF(_.op_sa >= 0, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF(_.op_sa <  0, x86("mov", "rax", (long)_.op_sb))
         + x86("cmp64",   "rax", (long)1)
         + x86("jge",     "L0")
         + x86("add",     "rax", "r15")
         + x86("add",     "rax", (long)1)
         + x86("def",     "L0")
         + x86("cmp64",   "rax", (long)1)
         + x86_omega("jl")
         + x86("mov",     "rcx", "r15")
         + x86("add",     "rcx", (long)1)
         + x86("cmp",     "rax", "rcx")
         + x86_omega("jg")
         + x86("mov",     "rcx", "r14")
         + x86("add",     "rcx", (long)1)
         + x86("cmp",     "rax", "rcx")
         + x86_omega("jne")
         + x86("mov",     FRQ(_.op_off),     (long)DT_I)
         + x86("mov",     FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86_beta_trampoline();
}
