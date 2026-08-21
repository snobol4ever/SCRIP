#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
/* SN4 kill-manufactured-names (2026-07-22): IR_MATCH_VALUE is the VALUE-operand sibling of bb_match_defer.  It
 * matches the already-computed pattern in operand[0]'s frame slot (FR(op_a_slot)) instead of resolving a
 * manufactured global name — the eager TT_FNC pattern-position call lowered its result once, upstream, into
 * that slot.  Structurally this IS bb_match_defer with (a) the name-based value acquisition (GVA block +
 * rt_defer_get_pat_fn-by-name) replaced by one lea+call reading the operand slot, and (b) the *X / DT_X
 * owed-call transfer loop (DEFER's L2/L3/L7/L8, rt_proc_open_fn/epilogue/rt_defer_step, xfer/anchor brackets)
 * DROPPED — an eager value is concrete and never owes a call.  The DT_P pattern-fn run path (L4=γ success,
 * L5=ω fail, jmp rax with rcx/rdx continuations) and the scalar literal-match close path are IDENTICAL to
 * DEFER, so the sub-pattern-invocation and resume protocol are the proven ones. */
extern "C" int   rt_defer_close            (int cur_delta);
extern "C" void *rt_match_value_get_pat_fn (DESCR_t *pval);
extern "C" void *rt_match_value_get_pat_dtp(DESCR_t *pval);
extern "C" long  rt_match_value_open       (DESCR_t *pval);
extern "C" void *dtp_fn_of(void *headv);
extern uint64_t g_scan_hit_start;
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_value() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_MATCH_VALUE (operand[0] pattern value, no manufactured name)")
         + x86_alpha()
         /* acquire operand[0]'s pattern value by pointer; DT_P -> compiled pattern fn in rax, else NULL (scalar) */
         /* ZD-5b (s27 cross-front OMEGA): under ZD, operand[0]'s DESCR lives at ZOPQ(0,8) (depth-diff staged by the driver loop); FR(op_a_slot) is the legacy flat-frame address, still used when !op_zres. */
         + IF(_.op_zres,  x86("lea",  "rdi", ZOPQ(0, 0)))   /* qword ptr [rsp# + op_zread[0] + 0] = base of operand[0]'s 16B DESCR cell */
         + IF(!_.op_zres, x86("lea",  "rdi", FRQ(_.op_a_slot)))   /* ⭐ MV-BASE (2026-08-09, treebank/claws5 SEGV root): CONSUMER ASKS THE SAME ORACLE AS THE PRODUCER.  op_a_slot is the PRODUCER's own raw-zls slot (emit.cpp:874 nd_slot staging, PB-2's slot-authority half — retained), and the producer's ZRES store spelled that slot through THE ONE OFFSET FUNCTION (x86_frame_off via FRQ): pinned graph ⇒ [___+slot] depth-immune, unpinned ⇒ [rsp+slot+op_zdepth].  PB-2's second half FORCED the rsp arm here (x86_frame_off_rsp) — correct at pin depth where rsp==___, but inside an ARBNO body rsp has moved and [rsp+slot] names a different byte than the producer's [___+slot] (SAME OFFSET ≠ SAME OBJECT): rt_match_value_get_pat_fn read garbage, value_open pushed a junk DESCR, defer_close strlen-faulted (e1 witness, gdb-bracketed).  FRQ makes producer and consumer one decision; the PB-2 [___+128] complaint was the OLD bb_slot_get=128 slot authority, cured at emit.cpp:874, not the ___ base. */
         + x86_align_enter()
         + x86("call", "rt_match_value_get_pat_dtp", (uint64_t)(uintptr_t)(void *)(void *(*)(DESCR_t *))rt_match_value_get_pat_dtp)   /* s108: DTP twin -- fn at [dtp+0], rdx carries the DTP into the blob so $V-bearing interiors slot-read their own construction's frozen values */
         + x86_align_leave()
         + x86("mov",  "rdx", "rax")
         + x86("test", "rax", "rax")
         + x86("jz",   "L0")
         + x86("mov",  "rax", RDQ("rdx", 0))
         /* --- DT_P: run the compiled pattern fn (r10=γ wire, r11=ω wire, r8d=scan flag), exactly as DEFER --- */
         + x86("mov",  "r8d", (long)(_.op_scan ? 1 : 0))
         + bb_glue_pass_wires_blob(4, 5)   /* ⭐ MV-BLOB-GLUE (s105): rt_match_value_get_pat_fn returns dtp_fn_of(pval->p) -- THE SAME PAT$ blob pointer rt_defer_get_pat_fn returns, as its own header comment says ("the VALUE-operand sibling of rt_defer_get_pat_fn").  A PAT$ blob reads γ/ω from r10/r11 (WREG, bb_match_defer.cpp:91) and its ω is a bare `jmp r11`; the FLAT glue seats rcx/rdx and leaves r10/r11 as they were, so a call-as-pattern (`S F()` -- oracle-legal, manual p.104: a program-defined function "may appear in a statement anywhere its value is needed -- in the subject, pattern, or replacement fields") entered the blob with wires=0 and died on `jmp 0` at the blob ω.  Witnesses probe/mv/mv_call_as_pattern*; beauty_suite case_driver test 7 (`'Hello' icase('hello')`) and semantic_driver test 4 (`'' nPush()`).  s22x GLUE-SYM converted this site to the flat spelling as "the backlog's unlisted 6th member" on the strength of the shared name; the two glues are NOT interchangeable -- flat = zero-frame transfer inside the same graph, blob = jmp-entry into a separately-compiled proc. */
         + x86("def",  L(4))
         + bb_glue_wire_land()
         + IF(_.op_scan && _.op_scan_head_off >= 0 && !emit_match_owns_startd(),
               x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_scan_hit_start, "g_scan_hit_start")
             + x86("mov",  "rax", "[rcx]")
             + x86("mov",  FR(_.op_scan_head_off), "eax"))
         + x86_gamma()
         + x86("def",  L(5))
         + bb_glue_wire_land()
         + x86_omega()
         /* --- scalar: literal match at the cursor; value is concrete so no owed-call loop --- */
         + x86("def",  "L0")
         + IF(_.op_zres,  x86("lea",  "rdi", ZOPQ(0, 0)))
         + IF(!_.op_zres, x86("lea",  "rdi", FRQ(_.op_a_slot)))   /* MV-BASE: same one-authority spelling as the acquisition lea above */
         + x86_align_enter()
         + x86("call", "rt_match_value_open", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t *))rt_match_value_open)
         + x86_align_leave()
         + x86("mov",  "edi", "r14d")
         + x86_align_enter()
         + x86("call", "rt_defer_close", (uint64_t)(uintptr_t)(void *)(int (*)(int))rt_defer_close)
         + x86_align_leave()
         + x86("test", "eax", "eax")
         + x86_omega("js")
         + x86("mov",  "r14d", "eax")
         + x86_lea_id("rax", 6)
         + x86_sub("rsp", 8)
         + x86("push", "rax")
         + x86_gamma()
         + x86("def",  L(6))
         + x86_add("rsp", 16)
         + x86_omega()
         + x86_beta()
         + x86_jmp_mem("rsp", 0);
}
