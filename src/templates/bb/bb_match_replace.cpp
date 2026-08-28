#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
void rt_match_replace(const char *name, uint64_t sub_lo, uint64_t sub_hi, int64_t start, int64_t end, DESCR_t *replp);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_replace() {
    x86_begin();
    return (_.op_off < 0 || (_.op_sa < 0 && !(_.op_zres && _.op_zread[2] >= 0)) || (_.op_sb < 0 && !_.op_zres)) ? x86_alpha() + x86_bomb("IR_MATCH_REPLACE: head/subject/repl slot unresolved")
         : x86("comment", "IR_MATCH_REPLACE")
         + x86_alpha()
         + x86_align_enter()
         + x86("mov",  "rdi", ROQ(0))
         + IF(_.op_zres && _.op_zread[2] >= 0,
                x86("note", ZOPN(2))
              + x86("mov", "rsi", ZOPQ(2, 0))
              + x86("note", ZOPN(2))
              + x86("mov", "rdx", ZOPQ(2, 8)))
         + IF(!(_.op_zres && _.op_zread[2] >= 0),
                x86("mov",  "rsi", FRQ(_.op_sa))
              + x86("mov",  "rdx", FRQ(_.op_sa + 8)))
         + (emit_match_rbp()
              ? x86("note", "repl_start")
              + x86("mov", "ecx", RDD("r12", -16))
              + x86("note", "repl_end")
              + x86("mov", "r8",  RDQ("r12", -8))
              + x86("sub", "r12", (long)16)
              : [&]() -> std::string { int _dispc = _.op_zfc ? _.op_off : (_.op_off - _.op_zpat); int _dispe = _.op_zfc ? (_.op_off + 24) : (_.op_off + 24 - _.op_zpat);
              std::string _cur = FR(_dispc); std::string _end = FRQ(_dispe);
              if (getenv("SCRIP_REPL_ADDR_DIAG")) fprintf(stderr, "[ADDR] cursor=%s end=%s op_off=%d op_zpat=%d op_zfc=%d\n", _cur.c_str(), _end.c_str(), _.op_off, _.op_zpat, _.op_zfc);
              return x86("mov", "ecx", _cur)
                   + x86("mov", "r8", _end); }())
         + IF(_.op_zres,  x86("note", ZOPN(1))
                        + x86("lea", "r9", ZOPQ(1, 0)))
         + IF(!_.op_zres, x86("lea",  "r9",  FRQ(_.op_sb)))
         + x86("call", "rt_match_replace", (uint64_t)(uintptr_t)(void *)rt_match_replace)
         + x86_align_leave()
         + IF(_.op_zres && _.op_zdepth > 0, x86("note", "repl_subtree_free")
                                           + x86("add", "rsp", (long)_.op_zdepth))
         + x86_jmp_id(1)
         + x86("def",    L(0))
         + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
         + x86("label",  LS(0))
         + x86(".string", _.op_sval ? _.op_sval : "")
         + x86_deflabel_id(1)
         + IF(_.flat_deep_arrival, x86("note", HKN(0)) + std::string(""))
         + x86_gamma();
}
