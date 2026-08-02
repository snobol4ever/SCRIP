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
    return !PLATFORM_X86 ? std::string()
         : (_.op_off < 0 || _.op_sa < 0 || (_.op_sb < 0 && !_.op_zres)) ? x86_alpha() + x86_bomb("IR_MATCH_REPLACE: head/subject/repl slot unresolved")
         : x86("comment", "IR_MATCH_REPLACE")
         + x86_alpha()
         + x86_align_enter()
         + x86("mov",  "rdi", ROQ(0))
         + x86("mov",  "rsi", FRQ(_.op_sa))
         + x86("mov",  "rdx", FRQ(_.op_sa + 8))   /* ZD-5 HEAD-FOLD: armed and declined read the SAME op_sa subject slot -- the armed head mirrors the consumed subject DESCR there at fold time, restoring the OFF-world dataflow, so this arm is regime-blind again. */
         + x86("mov",  "ecx", FR(_.op_off))
         + x86("mov",  "r8",  FRQ(_.op_off + 24))
         + IF(_.op_zres,  x86("note", ZOPN(1)) + x86("lea", "r9", ZOPQ(1, 0)))   /* ZD-5 MATCH-SPINE: the replacement travels BY ADDRESS -- r9 points at rv's own cell instead of the flat replp slot the armed producer no longer writes. */
         + IF(!_.op_zres, x86("lea",  "r9",  FRQ(_.op_sb)))
         + x86("call", "rt_match_replace", (uint64_t)(uintptr_t)(void *)rt_match_replace)
         + x86_align_leave()
         + x86_jmp_id(1)
         + x86("def",    L(0))
         + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
         + x86("label",  LS(0))
         + x86(".string", _.op_sval ? _.op_sval : "")
         + x86_deflabel_id(1)
         + IF(_.flat_deep_arrival && !_.op_stmt_pin, x86("note", HKN(0)) + x86("mov", "rbp", FRQ(_.op_off + 40)))   /* BRACKET-GATE (s193): paired with head's gated +40 save.  OBJ-NOTE ON-3 (s23e): third restore site of the same k=0 slot, named from the one HKN table with head and release.  REPL-PIN (this session): the !op_stmt_pin gate release:78 has carried since s22z lands here too -- the widened pin put replace statements INSIDE the pin population for the first time, and this unconditional restore mid-statement handed the terminal cut a stale base (measured: tB correct output then SEGV; patmin died on INNER re-entry).  Under the pin the terminal cut is the SOLE restore authority, per the head's own twin-gate law. */
         + x86_gamma();
}
