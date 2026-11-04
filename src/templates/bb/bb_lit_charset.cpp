#include <string>
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state.h"
#include "ir/descr_tags.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_zeta.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ls_slot() (_.op_off >= 0 || _.op_zres)
#define ls_rq(w) (_.op_zres ? ZRES(w) : FRQ(_.op_off + (w)))
#define ls_rd(w) (_.op_zres ? ZRESD(w) : FR(_.op_off + (w)))
#define lit_tag(base_tag) ((long)(base_tag) | _.op_lit_stamp)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_lit_charset() {
    return IF(!ls_slot(), x86_alpha() + x86_bomb("bb_lit_charset: the planner granted no slot"))
         + IF(ls_slot(),
               x86("comment", "IR_LIT_CHARSET")
             + x86_alpha()
             + x86("note", ZRESN())
             + x86("mov", ls_rq(0), lit_tag(DT_S))
             + x86("note", ZRESN())
             + x86("mov", ls_rd(4), (long)-1)
             + x86("mov", "rax", ROQ(0))
             + x86("note", ZRESN())
             + x86("mov", ls_rq(8), "rax")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86(".quad_ro", L(0), LS(0), _.op_sval ? _.op_sval : "")
             + x86(".cset_hdr", _.op_sval)
             + x86("label", LS(0))
             + (_.op_a_node_kind >= 0
                 ? x86(".string", _.op_sval ? _.op_sval : "", (unsigned long)_.op_a_ival_sg)
                 : x86(".string", _.op_sval ? _.op_sval : "")));
}
