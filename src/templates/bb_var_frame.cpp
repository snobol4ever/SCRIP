#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
extern int g_gvar_flat_chain;
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var_frame() {
    if (PLATFORM_X86) {
        if (!(g_gvar_flat_chain && _.op_off >= 0)) return x86_alpha() + x86_bomb("bb_var_frame: needs gvar flat-chain + own slot");
        extern int g_emit_frame_caller_dl;
        int hops = (int)_.op_dval;
        int target = g_emit_frame_caller_dl - hops;
        const char *dreg = (hops >= 1 && target >= 1 && target <= 3) ? (target == 1 ? "r13" : target == 2 ? "r14" : "r15") : (const char *)0;
        std::string reach = dreg
            ? x86("mov", "rax", dreg)
            : x86("lea", "rax", FRQ(0)) + FOR(0, hops, [&](int h) { (void) h; return x86("mov", "rax", RDQ("rax", 0)); });
        return x86("comment", "IR_VAR_FRAME")
             + x86_alpha()
             + reach
             + x86("mov",     "rcx", RDQ("rax", 16 + (int)_.op_ival * 16))
             + x86("mov",     FRQ(_.op_off),     "rcx")
             + x86("mov",     "rcx", RDQ("rax", 16 + (int)_.op_ival * 16 + 8))
             + x86("mov",     FRQ(_.op_off + 8), "rcx")
             + x86_gamma()
             + x86_beta()
             + x86_omega();
    }
    return std::string();
}
