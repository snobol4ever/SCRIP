#include <string>
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state_.h"
#include "ir/descr_tags_dt_platom.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_zeta.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ls_slot() (_.op_off >= 0 || _.op_zres)
#define ls_rq(w) (_.op_zres ? ZRES(w) : FRQ(_.op_off + (w)))
#define lit_tag(base_tag) ((long)(base_tag) | _.op_lit_stamp)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_lit_integer() {
    return IF(!ls_slot(), x86_alpha() + x86_bomb("bb_lit_integer: the planner granted no slot"))
         + IF(ls_slot(),
               x86("comment", "IR_LIT_INTEGER")
             + x86_alpha()
             + x86("note", ZRESN())
             + x86("mov", ls_rq(0), lit_tag(DT_I))
             + x86("mov", "rax", ROQ(0))
             + x86("note", ZRESN())
             + x86("mov", ls_rq(8), "rax")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def", L(0))
             + x86(".quad", (uint64_t)_.op_ival));
}
