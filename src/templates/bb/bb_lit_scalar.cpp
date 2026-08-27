#include <string>
#include <string.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t blsc_bits(double d, uint64_t b = 0) { memcpy(&b, &d, 8); return b; }
static int descr_stamp_on(void) { static int on = -1; if (on < 0) { const char *e = getenv("SCRIP_DESCR_STAMP"); on = (e && *e == '1') ? 1 : 0; } return on; }
static long lit_tag_imm(long base_tag) {
    if (!descr_stamp_on()) return base_tag;
    int raw_nid = _.nid;
    long src_node = (raw_nid <= 0 || raw_nid > 0xFFFE) ? (long)DESCR_SRC_NODE_OVERFLOW : (long)raw_nid;
    long mod_op = _.node ? (long)_.node->op + 1 : (long)DESCR_MOD_OP_UNSTAMPED;
    return base_tag | (mod_op << 8) | (src_node << 16);
}
static inline const char * ls_rq(int w) { return _.op_zres ? ZRES(w) : FRQ(_.op_off + w); }
static inline const char * ls_rd(int w) { return _.op_zres ? ZRESD(w) : FR(_.op_off + w); }
static inline std::string ls_dual(int w) { return (_.op_zres && _.op_off >= 0 && g_emit_cfg && g_emit_cfg->pl_cells_graph) ? x86("mov", "rax", ZRES(w)) + x86("mov", FRQ(_.op_off + w), "rax") : std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_lit_scalar() {
    return _.op_node_kind == (int)IR_LIT_INTEGER && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_INTEGER")
             + x86_alpha()
             + x86("note", ZRESN()) + x86("mov",    ls_rq(0), lit_tag_imm((long)DT_I))
             + x86("mov",    "rax", ROQ(0))
             + x86("note", ZRESN()) + x86("mov",    ls_rq(8), "rax")
             + ls_dual(0) + ls_dual(8)
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  (uint64_t)_.op_ival)
         : _.op_node_kind == (int)IR_LIT_STRING && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_STRING")
             + x86_alpha()
             + x86("note", ZRESN()) + x86("mov",    ls_rq(0), lit_tag_imm((long)DT_S))
             + x86("note", ZRESN()) + x86("mov",    ls_rd(4), (long)(_.op_sval ? strlen(_.op_sval) : 0))
             + x86("mov",    "rax", ROQ(0))
             + x86("note", ZRESN()) + x86("mov",    ls_rq(8), "rax")
             + ls_dual(0) + ls_dual(8)
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
             + x86("label",  LS(0))
             + x86(".string", _.op_sval ? _.op_sval : "")
         : _.op_node_kind == (int)IR_LIT_NAME && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_NAME")
             + x86_alpha()
             + x86("note", ZRESN()) + x86("mov",    ls_rq(0), lit_tag_imm((long)DT_N))
             + x86("note", ZRESN()) + x86("mov",    ls_rd(4), (long)0)
             + x86("mov",    "rax", ROQ(0))
             + x86("note", ZRESN()) + x86("mov",    ls_rq(8), "rax")
             + ls_dual(0) + ls_dual(8)
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
             + x86("label",  LS(0))
             + x86(".string", _.op_sval ? _.op_sval : "")
         : _.op_node_kind == (int)IR_LIT_CHARSET && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_CHARSET")
             + x86_alpha()
             + x86("note", ZRESN()) + x86("mov",    ls_rq(0), lit_tag_imm((long)DT_S))
             + x86("note", ZRESN()) + x86("mov",    ls_rd(4), (long)-1)
             + x86("mov",    "rax", ROQ(0))
             + x86("note", ZRESN()) + x86("mov",    ls_rq(8), "rax")
             + ls_dual(0) + ls_dual(8)
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
             + x86("label",  LS(0))
             + x86(".string", _.op_sval ? _.op_sval : "")
         : _.op_node_kind == (int)IR_LIT_REAL && (_.op_off >= 0 || _.op_zres)
         ? x86("comment", "IR_LIT_REAL")
             + x86_alpha()
             + x86("note", ZRESN()) + x86("mov",    ls_rq(0), lit_tag_imm((long)DT_R))
             + x86("mov",    "rax", ROQ(0))
             + x86("note", ZRESN()) + x86("mov",    ls_rq(8), "rax")
             + ls_dual(0) + ls_dual(8)
             + x86_gamma()
             + x86_beta_trampoline()
             + x86("def",    L(0))
             + x86(".quad",  blsc_bits(_.op_dval))
         : x86("comment", "IR_LIT_scalar")
             + x86_alpha()
             + x86_gamma()
             + x86_beta_trampoline();
}
