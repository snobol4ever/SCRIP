#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_sequence() {
    /* SEQUENCE IS WIRING (proved 2026-08-04, SE-4 2026-08-04): sigma/phi edges are statically wired
     * by LOWER.  H1b in emit.cpp aliases all four trampolines directly to their targets, so this box
     * emits zero net instructions.  Counter arm deleted (SEQ-ERAD SE-4): no DAG, no CHAIN, no FOREIGN
     * customer exists in the full corpus; the prepass that computed seqclean was all dead weight. */
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_SEQ_NARY (pure wiring, four trampolines aliased by H1b)")
         + x86_alpha()
         + x86("jmp", PAIR(0))
         + x86("def", PAIR((int)(2 * _.op_ival)))
         + x86_gamma()
         + x86_beta()
         + x86("jmp", PAIR((int)(2 * _.op_ival - 1)))
         + x86("def", PAIR((int)(2 * _.op_ival + 1)))
         + x86_omega();
}
