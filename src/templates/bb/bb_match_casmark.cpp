#include <string>
#include <cstdint>
#include "emitter/emit_str.h"
#include "emitter/emit_regime.h"
extern "C" {
#include "emitter/emit_state_.h"
#include "bb_templates.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_rodata.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_casmark() {
    x86_begin();
    return x86("comment", "IR_MATCH_CASMARK")
         + x86_alpha()
         + IF(_.op_ival == 2, x86("mov", "rcx", std::string("[rip@got + __]"), TEMPLATE_ADDR_SIGMA, "Σ")
                            + x86("mov", RDQ("rcx", 0), "r13")
                            + x86("mov", "rcx", std::string("[rip@got + __]"), TEMPLATE_ADDR_SIGLEN, "Σlen")
                            + x86("mov", RDD("rcx", 0), "r15d"))
         + IF(_.op_ival == 0, x86("mov", RDQ("r12", 0), 0L)
                            + IF(_.op_sval && _.op_sval[0],
                                 x86("mov", RDQ("r12", 8), "rbp")
                               + x86("lea", "rax", "[rip + __]", (uint64_t)(uintptr_t)(const void *)_.op_sval, x86_strtab_lbl(_.op_sval).c_str())
                               + x86("mov", RDQ("r12", 16), "rax"))
                            + IF(!(_.op_sval && _.op_sval[0]),
                                 x86("mov", RDQ("r12", 8), 0L)
                               + x86("mov", RDQ("r12", 16), 0L))
                            + x86("add", "r12", (long)24))
         + IF(_.op_ival == 1, x86("sub", "r12", (long)24))
         + x86_abs_disp32_store64(0x70000000L, "r12")
         + x86_gamma()
         + x86_beta()
         + IF(_.op_ival == 0, x86("sub", "r12", (long)24)
                            + x86_abs_disp32_store64(0x70000000L, "r12"))
         + x86_omega();
}
