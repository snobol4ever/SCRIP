#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void rt_zls_release_to(void *mark);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BB-OWNED-ζ statement-scope pivot (this session).  IR_MATCH_RELEASE owns the pattern-match's SUCCESS exit
 * the way IR_MATCH_HEAD's own omega already owns the FAILURE exit for free (HEAD is the scanner's single
 * fixed exhaustion choke point; SUCCESS has no such fixed point of its own -- it's whichever pattern element
 * the match happens to end on -- so this node IS that fixed point, spliced in by sno_lower_match as the
 * pattern's own new tail).  Single alpha, no beta/omega: this node cannot itself fail, it only runs once
 * control has genuinely reached the statement's true success.  op_off = drive_value_slot(operand[0]), where
 * operand[0] is the statement's own IR_MATCH_HEAD node -- the same operand[0]-owner convention
 * IR_MATCH_ARBNO's non-owner phases already use to read role 0's slot (emit.cpp IR_MATCH_ARBNO drive case).
 * head.zeta_mark lives at head's own slot +8 (zeta_storage.c IR_MATCH_HEAD grant); head.cursor (the existing
 * int32 counter) stays at +0, untouched. */
std::string bb_match_release() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_off < 0) return x86_alpha() + x86_bomb("IR_MATCH_RELEASE: head slot not resolved (operand[0] missing or unowned)");
    return x86("comment", "IR_MATCH_RELEASE")
         + x86_alpha()
         + x86_align_enter()
         + x86("mov",  "rdi", FRQ(_.op_off + 8))
         + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)
         /* ZLS2 twin (ZC_PORT_ALLOC only, inert otherwise): release the ZLS2 cursor to the mark head's
          * alpha saved at +16 — the success half of the same statement-scope backstop, reclaiming any v1
          * ARBNO activation that succeeded and left via this join without reaching its own role-2 pop. */
         + x86_zls2_release_to_call(_.op_off + 16)
         + x86_align_leave()
         + x86_gamma();
}
