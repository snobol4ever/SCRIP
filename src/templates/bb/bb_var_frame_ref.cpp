#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string frame_reach(const char * reg, int hops) {
    extern int g_emit_frame_caller_dl;
    return [&](int target) { return hops >= 1 && target >= 1 && target <= 3
             ? x86("mov", reg, target == 1 ? "r13" : target == 2 ? "r14" : "r15")
             : x86("lea", reg, FRQ(0))
             + FOR(0, hops, [&](int h) { (void) h; return x86("mov", reg, RDQ(reg, 0)); }); }(g_emit_frame_caller_dl - hops);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var_frame_ref() {
        return _.op_off < 0 ? x86_bomb("bb_var_frame_ref: needs gvar flat-chain + own slot") :
               x86("comment", "IR_VAR_FRAME_REF")
             + x86_alpha()
             + frame_reach("rax", (int) _.op_dval)
             + x86("mov",     "rax", RDQ("rax", 16 + (int)_.op_ival * 16 + 8))
             + x86("mov",     "rcx", RDQ("rax", 0))
             + x86("mov",     FRQ(_.op_off),     "rcx")
             + x86("mov",     "rcx", RDQ("rax", 8))
             + x86("mov",     FRQ(_.op_off + 8), "rcx")
             + x86_gamma()
             + x86_beta_trampoline();
}
