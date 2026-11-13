#include <string>
#include <cstdint>
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state.h"
#include "ir/descr.h"
void rt_arg_stage_d(const DESCR_t *cnt, const DESCR_t *val, DESCR_t *out, long spread);
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_gc.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_zeta.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_arg_stage() {
    x86_begin();
    return IF(_.op_zres,
               x86("comment", "IR_ARG_STAGE zd")
             + x86_alpha()
             + x86("note", ZOPN(0))
             + x86("lea", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(1))
             + x86("lea", "rsi", ZOPQ(1, 0))
             + x86("note", ZRESN())
             + x86("lea", "rdx", ZRES(0))
             + x86("mov", "rcx", (long)_.op_ival)
             + x86("call", "rt_arg_stage_d", (uint64_t)(uintptr_t)(void *)rt_arg_stage_d)
             + x86_rt_gc_poll()
             + x86("note", ZRESN())
             + x86("mov", "eax", ZRESD(0))
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!(_.op_zres),
               IF(_.op_sa < 0 || _.op_sb < 0 || _.op_off < 0, x86_bomb("bb_arg_stage: needs the count slot (op_sa), the value slot (op_sb) and its own value slot (op_off)"))
         + IF(!(_.op_sa < 0 || _.op_sb < 0 || _.op_off < 0),
             x86("comment", "IR_ARG_STAGE")
           + x86_alpha()
           + x86("lea", "rdi", FRQ(_.op_sa))
           + x86("lea", "rsi", FRQ(_.op_sb))
           + x86("lea", "rdx", FRQ(_.op_off))
           + x86("mov", "rcx", (long)_.op_ival)
           + x86("call", "rt_arg_stage_d", (uint64_t)(uintptr_t)(void *)rt_arg_stage_d)
           + x86_rt_gc_poll()
           + x86("mov", "eax", FR(_.op_off))
           + x86("cmp", "al", (long)DT_FAIL)
           + x86_omega("je")
           + x86_gamma()
           + x86_beta_trampoline()));
}
