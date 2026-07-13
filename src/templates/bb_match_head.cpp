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
extern const char *g_dcap_top;
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
         + x86("call", "rt_zls_mark", (uint64_t)(uintptr_t)(void *)rt_zls_mark)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86_zls2_mark_save(_.op_off + 16)
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("call", "rt_match_enter", (uint64_t)(uintptr_t)(void *)rt_match_enter)
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         /* rbp-dcap α: save the C caller's rbp (callee-saved contract — graphs stay rbp-free, match spans own
          * the register), load the live pend cursor from the mirror (rt_match_enter just lazy-init'd the
          * island), and save it as this match's MARK.  Nested heads see the outer's live top because every
          * mid-match transfer window in a match-family box mirrors rbp out first. */
         + x86("mov", FRQ(_.op_off + 40), "rbp")
         + x86("mov", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_dcap_top, "g_dcap_top")
         + x86("mov", "rbp", RDQ("rcx", 0))
         + x86("mov", FRQ(_.op_off + 32), "rbp")
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
         + x86("def", L(1))
         + x86_align_enter()
         + x86("mov",  "rdi", FRQ(_.op_off + 8))
         + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)
         + x86_zls2_release_to_call(_.op_off + 16)
         + x86_align_leave()
         /* rbp-dcap ω (all anchors exhausted): truncate = store mirror ← MARK, then restore the C caller's
          * rbp.  This IS the old rt_dcap_end_fail, inline — the depth array died with the ring. */
         + x86("mov", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_dcap_top, "g_dcap_top")
         + x86("mov", "rax", FRQ(_.op_off + 32))
         + x86("mov", RDQ("rcx", 0), "rax")
         + x86("mov", "rbp", FRQ(_.op_off + 40))
         + x86_omega();
}
