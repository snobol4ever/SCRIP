#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GLUE-2 FRAMED (Lon directive s21x-n: "the second the same plus setup of stack frame").  THE FRAMED GLUE: everything the flat glue does, PLUS establishing ___ as a depth-immune activation base.
 * WHO GETS IT is closed by the s21x-c design of record law 4 -- the FOUR ___ CONSTRUCTS: STATEMENT, FUNCTION, ARBNO, FENCE1.  Nothing else.  The reason is variable extent: these four cannot know their
 * own depth at emit time (ARBNO's housekeeping is per-iteration over variable-length children; FENCE1 unwinds to a floor; the statement bracket must rebalance from an arbitrary carve depth after a
 * scanfail -- SPITBOL Ch.18 step 6, and &ANCHOR is a RUNTIME keyword so no static classifier can retire it).  Everything else is depth-static and belongs to the flat twin.
 * ⛔ THIS IS A LAYOUT CONTRACT, NOT A SPELLING SWITCH (s21x-m law 3, measured SEGV -- do not retry blind).  Emitting this glue for a graph means that graph's PROLOGUE established ___; the two decisions
 * are one decision.  Widening emit_jmp_pin____() to make flat reads depth-immune SEGV'd with ZERO BBs armed, because the pin also gates xa_flat's caller-___ save at [rsp+kt-8] and kt is sized by a
 * layout pass that assumed no pin -- so pinning a never-pinned graph writes its save outside its own header and returns on a clobbered ___.  Whoever wires this template wires the prologue with it.
 * K=0 IS the statement bracket (WIRED s21x-p GLUE-4: emit.cpp head stubs + chain-exit cuts call this pair directly; the old x86_stmt_enter/leave spelling is deleted), so converting ARBNO / FUNCTION / FENCE1 onto it
 * parameterizes ONE shape instead of growing a fourth spelling of the same three instructions.  The pad arithmetic (((K+8+15) & ~15) - 8) keeps rsp 16-aligned mod 16 ACROSS the pushed ___, which is what
 * preserves the ambient call-site alignment the bare-call idiom depends on -- the same argument as the CSTACK k16 rounding. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
   /* ⭐ ___-KILL killswitch (Lon s51): DEFAULT OFF -- with SCRIP_____KILL unset this file is byte-identical to HEAD, so the arm ships dormant per the killswitch law and costs nothing until measured.  SCRIP_____KILL=1 runs the RSP-only experiment; the board's BROKEN set under that arm is the empirical license set.  File-static env cache, single read, no g_* writes. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_framed_enter() {
    /* GLUE-3 (Lon s21x-o): NO x86_begin() here BY DESIGN.  x86_begin mints a fresh x86_uid from g_flat_node_id in TEXT medium, and that uid names the box's RO constant labels (.Lx<uid>_0).  The glue is not a
     * top-level box template -- it is a FRAGMENT emitted inside another box's alpha/beta/gamma/omega port, so it must inherit that box's uid rather than mint one.  Calling x86_begin advanced the counter once
     * per port and renumbered every downstream label, which is exactly what the byte-identity A/B caught: instructions identical, .Lx5_0 became .Lx8_0.  A pure label rename is harmless to execution and would
     * have been invisible in a run-only test -- and would then have churned every committed .s artifact for no reason, which is precisely the kind of unexplained diff the artifact discipline exists to prevent. */
    if (!PLATFORM_X86) return std::string();
   /* ⭐ ___-KILL (Lon s51 directive: "establish EVERYTHING as RSP and when it breaks turn it into an ___") -- the EMPIRICAL license discovery, replacing the predicate-first approach.  Keep the carve (the cell region is real and its consumers read it), drop ONLY the base-pointer ceremony.  The BROKEN set of board_patterns_set.sh under this arm IS the license set: whatever fails here is a construct that genuinely reads a cell across an unknown-size span and earns its frame back; whatever still passes never needed one.  This cannot under-detect the way earn_hazard_in does (measured: it returns 0 for 95/95 ASSIGN_SAVE nodes -- a capture family that never once earns across 122 pattern programs is a stuck classifier, not a measurement). */
    return std::string("")
         + std::string("")
         + x86("sub", "rsp", ((_.op_fc_bytes + 8 + 15) & ~15L) - 8);
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* THE LEAVE SIDE TAKES NO K BY DESIGN -- that is the entire reason the framed flavor exists.  `mov rsp, ___` discards the activation WHOLESALE, at whatever depth it actually reached, without anyone
 * having had to predict that depth at emit time.  A framed box may therefore carve freely inside itself (ARBNO's per-iteration elements, a pattern blob's dynamic extent) and still unwind exactly.
 * This is also why the four constructs are the ONLY ones that get it: paying for a frame pointer buys nothing for a box whose depth was statically known, and costs a GPR for the whole activation. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_framed_leave() {
    if (!PLATFORM_X86) return std::string();
    return x86("add", "rsp", ((_.op_fc_bytes + 8 + 15) & ~15L) - 8);
   /* ___-KILL: the enter arm emitted no push and no pin, so there is nothing to discard -- and `mov rsp,___` against a base this arm never established would load a STALE ___ into rsp, which is the exact failure bb_glue_framed.cpp's own header names ("Reversing (1)/(3) or omitting (1) while keeping (3) loads the CRT caller's ___ into rsp").  The statement-terminal op_zgpop release is the sole authority for freeing the carve under this arm, which is what the FORTH-spine model says it always should have been. */
    return std::string("")
         + std::string("");
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
