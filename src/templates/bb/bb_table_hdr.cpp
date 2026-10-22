#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
DESCR_t rt_pl_dop_tbl_open(DESCR_t *, int);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define TH_REC(k) FRQ(_.op_off + (k))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_table_hdr() {
    x86_begin();
    return IF(_.op_off < 0, x86_alpha() + x86_bomb("bb_table_hdr: a tabling header needs its own frame slots (op_off)"))
         + IF(_.op_off >= 0,
               x86("comment", "IR_TABLE_HDR: a tabled evaluation's state lives in these frame slots; the open leaf banks the current header in cell 0 and makes this one current (a trailed write)")
             + x86_alpha()
             + x86("lea", "rax", TH_REC(16))
             + x86("mov32", "edx", (long)DT_I)
             + x86("mov", TH_REC(0), "rdx")
             + x86("mov", TH_REC(8), "rax")
             + x86("lea", "rdi", TH_REC(0))
             + x86("mov32", "esi", 1L)
             + x86("call", "rt_pl_dop_tbl_open", (uint64_t)(uintptr_t)(void *)rt_pl_dop_tbl_open)
             + x86_rt_gc_poll()
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86_gamma()
             + x86_beta_trampoline());
}
