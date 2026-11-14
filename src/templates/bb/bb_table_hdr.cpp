#include <string>
#include <cstdint>
#include "emitter/emit_str.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/ir_graph.h"
extern "C" {
#include "emitter/emit_state_em_03_sh_api.h"
#include "emitter/emit_state_.h"
#include "ir/descr.h"
#include "ir/ir_node.h"
DESCR_t rt_pl_dop_tbl_open(DESCR_t *, int);
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_gc.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_frame.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define TH_REC(k) FRQ(_.op_off + (k))
#define TH_TAIL(k) TH_REC(16 + 16 * (TABLE_HDR_CELLS - 5) + (k))
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
             + x86("lea", "rcx", RDQ("rbp", g_emit.flat_frame_bytes + 16 * (g_emit_cfg ? g_emit_cfg->nparams : 0)))
             + x86("mov", TH_TAIL(8), "rcx")
             + x86("mov", TH_TAIL(0), "rdx")
             + x86("mov", TH_TAIL(32), "rdx")
             + x86("mov", TH_TAIL(40), 0L)
             + x86("mov", TH_TAIL(48), "rdx")
             + x86("lea", "rcx", RDQ("rbp", g_emit.flat_frame_bytes - 24))
             + x86("mov", TH_TAIL(56), "rcx")
             + x86("mov", TH_TAIL(64), "rdx")
             + x86("mov", TH_TAIL(72), 0L)
             + x86("lea", "rdi", TH_REC(0))
             + x86("mov32", "esi", 1L)
             + x86("call", "rt_pl_dop_tbl_open", (uint64_t)(uintptr_t)(void *)rt_pl_dop_tbl_open)
             + x86_rt_gc_poll_fails()
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86_gamma()
             + x86_beta_trampoline());
}
