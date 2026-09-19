#include <string>
#include <stdint.h>
#include <cstdlib>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/rt/rt_coexpr.h"
}
#include "x86_asm.h"
static inline bool xa_switch_record_planted() { const char * e = getenv("SCRIP_GC_SWITCH_RECORD_PLANT"); return e && *e == '1'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_coret() {
    x86_begin();
    return IF(_.op_sa < 0, x86_alpha() + x86_bomb("bb_coret: no produced-value slot (coret.operand[0] has no DESCR slot -- body value node missing from chain)"))
         + IF(!(_.op_sa < 0),
             x86("comment", "IR_CORET yield")
           + x86_alpha()
           + x86("mov",  "rdi", FRQ(_.op_sa))
           + x86("mov",  "rsi", FRQ(_.op_sa + 8))
           + x86("xor",  "edx", "edx")
           + x86("comment", "ARCH-GC 6.2f SWITCH RECORD: r13 (the subject) spilled as one tagged DESCR cell on this spine across the yield and reloaded when the body is resumed -- see bb_activate")
           + IF(!xa_switch_record_planted(), x86("sub",  "rsp", (long)16)
           + x86_rsp_store32_imm(0, (long)DT_S)
           + x86_rsp_store32(4, "r15d")
           + x86_rsp_store64(8, "r13"))
           + x86("call", "scrip_coret", (uint64_t)(uintptr_t)(void *)scrip_coret)
           + IF(!xa_switch_record_planted(), x86_rsp_load64("r13", 8)
           + x86("add",  "rsp", (long)16))
           + x86_rt_gc_poll()
           + x86_gamma());
}
