#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
ScanSubjRegs rt_match_enter(uint64_t lo, uint64_t hi);
void * rt_zls_mark(void);
void   rt_zls_release_to(void *mark);
extern long g_anchor;
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_head() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return (_.op_sa < 0 || _.op_off < 0)
         ? x86_bomb("IR_MATCH_HEAD: subject/start slot not promoted (emit_drive)")
         : x86("comment", "IR_MATCH_HEAD")
         + x86_alpha()
         /* BB-OWNED-ζ statement-scope pivot (this session): mark ONCE per statement scan-entry, before
          * rt_match_enter -- so nothing from THAT call's result (consumed into r13/r15 below) is at risk
          * from rt_zls_mark's own return-register clobber.  Stored immediately to the frame slot (op_off+8,
          * the padding zls_grant newly carved out of this node's existing 16B quad) rather than left live in
          * a register across the second call.  IR_MATCH_RELEASE (the pattern's spliced success-tail) reads
          * this same slot via operand[0] at the statement's true success exit. */
         + x86("call", "rt_zls_mark", (uint64_t)(uintptr_t)(void *)rt_zls_mark)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         /* ZLS2 twin (ZC_PORT_ALLOC only, inert otherwise): mark the ZLS2 cursor at the same statement
          * entry, into the second quad (+16, zeta_storage.c).  Same placement rationale as the line above:
          * before rt_match_enter, stored immediately, read back by this node's own failure choke L(1) and
          * by IR_MATCH_RELEASE's success alpha.  Closes the v1-ARBNO success-path leak on the ZLS2 arena. */
         + x86_zls2_mark_save(_.op_off + 16)
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("call", "rt_match_enter", (uint64_t)(uintptr_t)(void *)rt_match_enter)
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         + x86("mov", FR(_.op_off), (long)0)
         + x86("def", L(0))
         + x86("mov", "r14d", FR(_.op_off))
         + x86_gamma()
         + x86_beta()
         + x86("add", FR(_.op_off), (long)1)
         + x86("mov", "eax", FR(_.op_off))
         + x86("cmp", "eax", "r15d")
         + x86("jg",  L(1))
         + x86("mov", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_anchor, "g_anchor")
         + x86("mov", "rax", "[rcx]")
         + x86("cmp64", "rax", (long)0)
         + x86("jne", L(1))
         + x86("jmp", L(0))
         /* BB-OWNED-ζ statement-scope pivot: this node's own omega IS the statement's failure exit (head's
          * ω was already wired to fJ by sno_lower_match, unchanged) -- both scanner-exhaustion exits
          * (limit reached / &ANCHOR set) now redirect through this ONE local label instead of jumping to
          * omega directly, so the release_to call exists once, not duplicated at each conditional jump.
          * Reads the SAME slot (op_off+8) the alpha above just wrote, and the SAME primitive
          * IR_MATCH_RELEASE's own alpha calls on the success side -- one mark, released exactly once on
          * whichever exit the statement actually takes, never both (success and failure are mutually
          * exclusive control-flow paths out of this node, never both reached in one execution). */
         + x86("def", L(1))
         + x86_align_enter()
         + x86("mov",  "rdi", FRQ(_.op_off + 8))
         + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)
         + x86_zls2_release_to_call(_.op_off + 16)
         + x86_align_leave()
         + x86_omega();
}
