#include <string>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*-*/
static uint64_t blsc_bits(double d, uint64_t b = 0) { memcpy(&b, &d, 8); return b; }
/*-*/
static inline const char * ls_rq(int w) { return _.op_zres ? ZRES(w) : FRQ(_.op_off + w); }   /* ZD-1 (Lon s21x-v): destination switch -- op_zres lands the literal in the box's own alpha-carved cell (mode 1 of the FOUR modes), consumers read it via staged op_zread differences; legacy = the flat slot.  One body, both regimes. */
static inline const char * ls_rd(int w) { return _.op_zres ? ZRESD(w) : FR(_.op_off + w); }
/*-*/
std::string bb_lit_scalar() {
    return !PLATFORM_X86 ? std::string()
         : _.op_node_kind == (int)IR_LIT_INTEGER && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_INTEGER")
             + x86_alpha()
             + x86("mov",    ls_rq(0), (long)DT_I)
             + x86("mov",    "rax", ROQ(0))
             + x86("mov",    ls_rq(8), "rax")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  (uint64_t)_.op_ival)
         : _.op_node_kind == (int)IR_LIT_STRING && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_STRING")
             + x86_alpha()
             + x86("mov",    ls_rq(0), (long)DT_S)
             + x86("mov",    ls_rd(4), (long)(_.op_sval ? strlen(_.op_sval) : 0))
             + x86("mov",    "rax", ROQ(0))
             + x86("mov",    ls_rq(8), "rax")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
             + x86("label",  LS(0))
             + x86(".string", _.op_sval ? _.op_sval : "")
         : _.op_node_kind == (int)IR_LIT_CHARSET && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_CHARSET")
             + x86_alpha()
             + x86("mov",    ls_rq(0), (long)DT_S)
             + x86("mov",    ls_rd(4), (long)-1)
             + x86("mov",    "rax", ROQ(0))
             + x86("mov",    ls_rq(8), "rax")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
             + x86("label",  LS(0))
             + x86(".string", _.op_sval ? _.op_sval : "")
         : _.op_node_kind == (int)IR_LIT_REAL && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_REAL")
             + x86_alpha()
             + x86("mov",    ls_rq(0), (long)DT_R)
             + x86("mov",    "rax", ROQ(0))
             + x86("mov",    ls_rq(8), "rax")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  blsc_bits(_.op_dval))
         : x86("comment", "IR_LIT_scalar")
             + x86_alpha()
             + x86_gamma()
             + x86_beta_trampoline();
}
