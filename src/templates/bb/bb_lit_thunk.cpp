#include <string>
#include "emitter/emit_regime.h"
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state_.h"
const char * bb_ab_sym_name(const char * nm);
#include "ir/descr_tags_dt_platom.h"
#include "ir/dtp.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_zeta.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ls_slot() (_.op_off >= 0 || _.op_zres)
#define ls_rq(w) (_.op_zres ? ZRES(w) : FRQ(_.op_off + (w)))
#define lit_tag(base_tag) ((long)(base_tag) | _.op_lit_stamp)
#define blsc_thk_label(n) (std::string(".Lthk_") + bb_ab_sym_name((n) ? (n) : ""))
#define blsc_thk_rec(n) emit_rec_or_empty(bb_thunk_rec_addr(n))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_lit_thunk() {
    return IF(!ls_slot(), x86_alpha() + x86_bomb("bb_lit_thunk: the planner granted no slot"))
         + IF(ls_slot(),
               x86("comment", "IR_LIT_THUNK; the thunk record's address baked, no name at run time")
             + x86_alpha()
             + x86("note", ZRESN())
             + x86("mov", ls_rq(0), lit_tag(DT_I))
             + x86("mov", "rax", ROQ(0))
             + x86("note", ZRESN())
             + x86("mov", ls_rq(8), "rax")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86(".quad_ro", L(0), blsc_thk_label(_.op_sval).c_str(), blsc_thk_rec(_.op_sval)));
}
