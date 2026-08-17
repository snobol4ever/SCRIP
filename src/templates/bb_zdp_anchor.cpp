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
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void rt_zdp_origin(void);
extern "C" void rt_zdp_probe(void);
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" std::string bb_zdp_origin(long node) {
    if (!PLATFORM_X86) return std::string();   /* ⭐⭐⭐ LON'S EVERY-PORT PROBE (s136), THE ORIGIN.  Lon in-chat: "Instrument each BB graph's FIRST BB to save RSP (RSP0)".  RSP0 is the datum every subsequent α/β measures against, so it is captured ONCE per graph, before the graph carves anything.  rdi carries the TRUE pre-push rsp (+16 undoes this stub's own two pushes) so the datum is the graph's real entry frontier and not an artifact of the instrument -- the s134 lesson that a measurement normalised at the point of measurement measures the normalisation. */
    return x86("comment", "ZDP-ORIGIN")
         + x86("push", "rdi")
         + x86("push", "rsi")
         + x86("mov",  "rdi", "rsp")
         + x86("add",  "rdi", 16L)
         + x86("mov",  "rsi", (long)node)
         + x86("call", "rt_zdp_origin", (uint64_t)(uintptr_t)(void *)rt_zdp_origin)
         + x86("pop",  "rsi")
         + x86("pop",  "rdi");
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" std::string bb_zdp_probe(long op, long node, long port, long expect, long want_rbp) {
    if (!PLATFORM_X86) return std::string();   /* ⭐⭐⭐ LON'S EVERY-PORT PROBE (s136), THE CHECK.  Lon in-chat: "instrument every ALPHA and BETA to check the diff of RSP to this RSP0 and report if different than SCRIP's CALCULATED expected value.  This will find all your PROBLEM spots automatically.  Also have BB's that use RBP check at BETA for equality to saved RBP."  expect = the lattice's prediction in bytes, -1 = ⊤ (RECORD-ONLY: a ⊤ whose delta is constant is a PRECISION bug, one whose delta varies is a ⊤ that was earned -- rows 3/4 of Lon's table, and nothing static can separate them).  FIVE arg registers, all pushed and popped here, so the probe is invisible to the measured program: r10/r11 (the γ/ω WIRES) and rbx/r12/r13/r14/r15 are NEVER touched on this path -- the agreeing case never reaches C at all (rtx_zdp.S compares in hand asm), which is the whole reason this is asm and not a C tap. */
    return x86("comment", "ZDP-PROBE")
         + x86("push", "rdi")
         + x86("push", "rsi")
         + x86("push", "rdx")
         + x86("push", "rcx")
         + x86("push", "r8")
         + x86("mov",  "rdi", "rsp")
         + x86("add",  "rdi", 40L)
         + x86("mov",  "rsi", (long)op)
         + x86("mov",  "rdx", (long)node)
         + x86("mov",  "rcx", (long)expect)
         + x86("mov",  "r8",  (long)(port | (want_rbp == 1L ? 4L : 0L) | (want_rbp == 2L ? 8L : 0L)))
         + x86("call", "rt_zdp_probe", (uint64_t)(uintptr_t)(void *)rt_zdp_probe)
         + x86("pop",  "r8")
         + x86("pop",  "rcx")
         + x86("pop",  "rdx")
         + x86("pop",  "rsi")
         + x86("pop",  "rdi");
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int x86_zdp_on_c(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZDP_TEARDOWN"); v = (e && *e == '1') ? 1 : 0; } return v; }   /* ⭐ ONE killswitch, C-callable so emit.cpp's graph-entry origin hook and x86_asm.h's port arm read THE SAME predicate -- never two spellings of one flag (the s68/s70 spelled-twice disease). */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int zzone_on_c(void) { return zzone_on(); }   /* ⭐ ζ-ONE U-1 (s136): C-callable face of the ONE zone killswitch, so the planner hook in scrip_ir.c and the ZREF accessor in x86_asm.h read THE SAME predicate -- never two spellings of one flag (the s68/s70 spelled-twice disease). */
