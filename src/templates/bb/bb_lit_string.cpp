#include <string>
#include <string.h>
#include "ir/ir_node.h"
#include "emitter/emit_regime.h"
#include "emitter/emit_str.h"
extern "C" {
#include "emitter/emit_state.h"
const char * bb_ab_sym_name(const char * nm);
#include "ir/descr_tags.h"
#include "ir/dtp.h"
}
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_zeta.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define ls_slot() (_.op_off >= 0 || _.op_zres)
#define ls_rq(w) (_.op_zres ? ZRES(w) : FRQ(_.op_off + (w)))
#define ls_rd(w) (_.op_zres ? ZRESD(w) : FR(_.op_off + (w)))
#define lit_tag(base_tag) ((long)(base_tag) | _.op_lit_stamp)
#define lstr_dstar() (_.op_seal == IR_SEAL_DSTAR_REF)
#define lstr_thunk() (_.op_seal == IR_SEAL_THUNK_REF)
#define lstr_ref() (lstr_dstar() || lstr_thunk())
#define blsc_thk_label(n) (std::string(".Lthk_") + bb_ab_sym_name((n) ? (n) : ""))
#define blsc_thk_rec(n) emit_rec_or_empty(bb_thunk_rec_addr(n))
extern "C" void * bb_dstar_rec_addr(const char * star);
#define blsc_dstar_label(n) (std::string(".Ldstar_") + bb_ab_sym_name((std::string("*") + ((n) ? (n) : "")).c_str()))
#define blsc_dstar_rec(n) emit_rec_or_empty(bb_dstar_rec_addr((std::string("*") + ((n) ? (n) : "")).c_str()))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_lit_string() {
    return IF(!ls_slot(), x86_alpha() + x86_bomb("bb_lit_string: the planner granted no slot"))
         + IF(ls_slot(),
               x86("comment", "IR_LIT_STRING; a DSTAR-REF or THUNK-REF seal makes it the star or thunk record's address baked, no name at run time")
             + x86_alpha()
             + x86("note", ZRESN())
             + x86("mov", ls_rq(0), lit_tag(lstr_ref() ? DT_I : DT_S))
             + IF(!lstr_ref(),
                   x86("note", ZRESN())
                 + x86("mov", ls_rd(4), _.op_a_node_kind >= 0 ? (long)_.op_a_ival_sg : (long)(_.op_sval ? strlen(_.op_sval) : 0)))
             + x86("mov", "rax", ROQ(0))
             + x86("note", ZRESN())
             + x86("mov", ls_rq(8), "rax")
             + x86_gamma()
             + x86_beta_trampoline()
             + IF(lstr_dstar(), x86(".quad_ro", L(0), blsc_dstar_label(_.op_sval).c_str(), blsc_dstar_rec(_.op_sval)))
             + IF(lstr_thunk(), x86(".quad_ro", L(0), blsc_thk_label(_.op_sval).c_str(), blsc_thk_rec(_.op_sval)))
             + IF(!lstr_ref(),
                   x86(".quad_ro", L(0), LS(0), _.op_sval ? _.op_sval : "")
                 + x86("label", LS(0))
                 + (_.op_a_node_kind >= 0
                     ? x86(".string", _.op_sval ? _.op_sval : "", (unsigned long)_.op_a_ival_sg)
                     : x86(".string", _.op_sval ? _.op_sval : ""))));
}
