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
/* SN4-REPL (doctrine stages 4/5).  α-only splice: fires once, after RELEASE has stashed the end cursor at
 * head slot +24 and after the replacement expression chain has produced its value.  operands (drive):
 * op_off = head's slot (start int32 @+0, end quad @+24), op_sa = subject value slot (raw DESCR quads —
 * coerced runtime-side exactly as rt_match_enter coerces the same slot), op_sb = replacement value slot,
 * op_sval = subject variable name (sealed .rodata adjacent to the blob for mode-4, the bb_call precedent).
 * Marshal: rt_match_replace(name, sub_lo, sub_hi, start, end, &repl) builds prefix+repl+suffix and stores
 * by name via NV_SET_fn (GVA-forwarding-correct).  Cannot fail; γ = the statement's true sJ. */
std::string bb_match_replace() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0 || _.op_sa < 0 || _.op_sb < 0) return x86_alpha() + x86_bomb("IR_MATCH_REPLACE: head/subject/repl slot unresolved");
    return x86("comment", "IR_MATCH_REPLACE")
         + x86_alpha()
         + x86_align_enter()
         + x86_ro_load_q("rdi", 0)
         + x86("mov",  "rsi", FRQ(_.op_sa))
         + x86("mov",  "rdx", FRQ(_.op_sa + 8))
         + x86("mov",  "ecx", FR(_.op_off))
         + x86("mov",  "r8",  FRQ(_.op_off + 24))
         + x86("lea",  "r9",  FRQ(_.op_sb))
         + x86("call", "rt_match_replace", (uint64_t)(uintptr_t)(void *)rt_match_replace)
         + x86_align_leave()
         + x86_jmp_id(1)
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "")
         + x86_deflabel_id(1)
         + x86_gamma();
}
