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
         : (_.op_off < 0 || _.op_sa < 0 || _.op_sb < 0) ? x86_alpha() + x86_bomb("IR_MATCH_REPLACE: head/subject/repl slot unresolved")
         : x86("comment", "IR_MATCH_REPLACE")
         + x86_alpha()
         + x86_align_enter()
         + x86("mov",  "rdi", ROQ(0))
         + x86("mov",  "rsi", FRQ(_.op_sa))
         + x86("mov",  "rdx", FRQ(_.op_sa + 8))
         + x86("mov",  "ecx", FR(_.op_off))
         + x86("mov",  "r8",  FRQ(_.op_off + 24))
         + x86("lea",  "r9",  FRQ(_.op_sb))
         + x86("call", "rt_match_replace", (uint64_t)(uintptr_t)(void *)rt_match_replace)
         + x86_align_leave()
         + x86_jmp_id(1)
         + x86("def",    L(0))
         + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
         + x86("label",  LS(0))
         + x86(".string", _.op_sval ? _.op_sval : "")
         + x86_deflabel_id(1)
         /* REG-3: a replacement statement's RELEASE γ defers the outer-rbp restore to HERE — REPLACE is the
          * statement's LAST window reader (its subject/start/end rode [rbp+off] under the anchored view).
          * FRQ resolves per this box's own class: anchored [rbp+off] with rbp==base, granted [rsp+off] at
          * unwound base (idempotent — rbp was never re-tenanted), flat_pat [r12+off] island. */
         + x86("mov", "rbp", FRQ(_.op_off + 40))
         + x86_gamma();
}
