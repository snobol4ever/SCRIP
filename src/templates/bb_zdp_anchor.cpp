#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" void rt_zdp_anchor(void);
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" std::string bb_zdp_anchor(long op, long node) {
    if (!PLATFORM_X86) return std::string();   /* ⭐ LON'S ANCHOR CHECK (s135), EMIT SIDE: an anchor is a point where this graph's stack is TOTALLY EMPTY, so every anchor must observe ONE rsp; IR_STATEMENT_BEGIN and IR_MATCH_BEGIN are the anchors.  Emitted ONLY under SCRIP_ZDP_TEARDOWN=1 (gate at the emit.cpp choke) — OFF emits nothing, so the default build is byte-identical BY CONSTRUCTION rather than by measurement.  ⛔ NO DEPTH ARITHMETIC APPEARS ANYWHERE IN THIS SEQUENCE.  The anchor invariant is self-referential (all anchors see one rsp), which is precisely why it does not inherit the circularity that would sink a per-node depth check: auditing op_zdepth through an address computed from op_zdepth calibrates the instrument against the quantity under test.  ⛔ THE THREE PUSHES AND THE `add rdi,24` ARE ONE FACT AND MOVE TOGETHER: rdi must carry the TRUE anchor rsp, the value BEFORE this instrumentation touched the stack, so the addend is 8*(pushes above it).  Editing one without the other silently measures the wrong point.  mov+add is used rather than `lea rdi,[rsp+24]` because it needs no memory-operand arm from the encoder.  BOTH-MEDIUM: every byte rides x86(...) — TEXT takes the symbol, BINARY the resolved address, the same call idiom the rt_deref family already uses; zero MEDIUM_* in this file. */
    return x86("comment", "ZDP-ANCHOR")
         + x86("push", "rdi")
         + x86("push", "rsi")
         + x86("push", "rdx")
         + x86("mov",  "rdi", "rsp")
         + x86("add",  "rdi", 24L)
         + x86("mov",  "rsi", (long)op)
         + x86("mov",  "rdx", (long)node)
         + x86("call", "rt_zdp_anchor", (uint64_t)(uintptr_t)(void *)rt_zdp_anchor)
         + x86("pop",  "rdx")
         + x86("pop",  "rsi")
         + x86("pop",  "rdi");
}
