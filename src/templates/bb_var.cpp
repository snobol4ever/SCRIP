#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_var() {
    x86_begin();
    if (PLATFORM_X86) {
        if (_.op_zres)
            return x86("comment", "IR_VAR local -> ZRES (ZK-2 cells arm)")
                 + x86_alpha()
                 + x86("mov",  "rax", FRQ(_.op_sa))
                 + x86("mov",  "rdx", FRQ(_.op_sa + 8))
                 + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
                 + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
                 + ((_.op_off >= 0 && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? x86("mov", FRQ(_.op_off), "rax") + x86("mov", FRQ(_.op_off + 8), "rdx") : std::string())
                 + x86_gamma()
                 + x86_beta_trampoline();
        return _.op_off != -1 && _.op_sa != -1 ?
               x86("comment", "IR_VAR")
             + x86_alpha()
             + x86("mov",     "rax", FRQ(_.op_sa))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta_trampoline() :
               x86_bomb("bb_var: unhandled arm (no flat-chain mode or missing slot)");
    }
    return std::string();
}
