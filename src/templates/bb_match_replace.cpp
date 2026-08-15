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
         : (_.op_off < 0 || (_.op_sa < 0 && !(_.op_zres && _.op_zread[2] >= 0)) || (_.op_sb < 0 && !_.op_zres)) ? x86_alpha() + x86_bomb("IR_MATCH_REPLACE: head/subject/repl slot unresolved")
         : x86("comment", "IR_MATCH_REPLACE")
         + x86_alpha()
         + x86_align_enter()
         + x86("mov",  "rdi", ROQ(0))
         + IF(_.op_zres && _.op_zread[2] >= 0,
                x86("note", ZOPN(2)) + x86("mov", "rsi", ZOPQ(2, 0))
              + x86("note", ZOPN(2)) + x86("mov", "rdx", ZOPQ(2, 8)))   /* ⭐ R-3(c) SUBJECT SEAT (s99 "fourth seat"): the armed splice reads the subject from its producer's LIVE spine cell via the staged op_zread[2] (emit.cpp REPLACE choke -- zunder + producer-below-head offset, physically exact post-whack), per the ZD-5 MATCH-SPINE ZOPQ(2) design of record.  The op_sa flat slot has NO writer in the armed world since s97 deleted the head's mirror from both arms (the deletion was right -- the mirror wrote unbacked flat coordinates -- the READER was the half left behind). */
         + IF(!(_.op_zres && _.op_zread[2] >= 0),
                x86("mov",  "rsi", FRQ(_.op_sa))
              + x86("mov",  "rdx", FRQ(_.op_sa + 8)))   /* legacy/declined + -1-sentinel fallback: byte-identical to the pre-fix read -- degrade never die. */
         + [&]() -> std::string { int _dispc = _.op_zfc ? _.op_off : (_.op_off - _.op_zpat); int _dispe = _.op_zfc ? (_.op_off + 24) : (_.op_off + 24 - _.op_zpat);
              std::string _cur = FR(_dispc); std::string _end = FRQ(_dispe);
              if (getenv("SCRIP_REPL_ADDR_DIAG")) fprintf(stderr, "[ADDR] cursor=%s end=%s op_off=%d op_zpat=%d op_zfc=%d\n", _cur.c_str(), _end.c_str(), _.op_off, _.op_zpat, _.op_zfc);
              return x86("mov", "ecx", _cur) + x86("mov", "r8", _end); }()   /* L-3b STEP-0' FIX (s38, superseding the s37 STEP-6 comment's unwired ADDITIVE description -- that comment's formula, op_off+op_zfc, was tried and DISPROVED by direct counter-example this session, see FINDING-2026-08-12j).  ROOT CAUSE, confirmed by a THREE-witness triangulation (l3_spl_len_pure 0 carvers / l3_spl_span_nonterm 1 carver / the newly-minted l3_spl_span_span_double 2 carvers, all op_off=48): the WRITER's own op_off+op_fc_disp+32 formula (bb_match_end.cpp) already exactly CANCELS however many bytes of un-popped match-primitive backtrack cells (op_fc_disp = the sum of every intervening SPAN/ARB/BAL-family primitive's un-popped `sub rsp,K`) accumulate before MATCH_END runs -- so the writer lands on the SAME fixed absolute address (measured: RSP_B_common+48, where RSP_B_common is the unwind target `cas_rsp_mark` saved at MATCH_BEGIN's own alpha, BEFORE any match-primitive fires, and is therefore IDENTICAL across every witness sharing this op_off, independent of pattern shape) regardless of op_fc_disp's value.  The READER must reach that SAME carve-independent address, which means op_fc_disp/op_zfc must NOT appear in the read formula's arithmetic at all -- FR's own op_zdepth term (x86_frame_off, tracking ONLY the replacement expression's own K, uniformly +16 for a 1-char literal across all three witnesses, confirmed by back-solving the baseline formula's actual emission in each) supplies exactly the remaining compensation.  Net: FR(op_off) / FRQ(op_off+24) reaches the writer's address on every non-carving (op_zfc!=0) witness by construction, with ZERO op_fc_disp/op_zpat/op_zfc term in the passed offset.  op_zpat's SUBTRACTIVE formula is LEFT UNTOUCHED for the disjoint carving class (op_zfc==0: TAB/RTAB/POS/RPOS, still a separate open defect, unaffected by this edit).  GATE WITNESSES: l3_spl_span_nonterm/arb_nonterm/break_nonterm/rem_nonterm/VACUOUS_terminal_trap/span_concat/span_span_double (non-carving, op_zfc!=0, expected to flip PASS) vs l3_spl_tab_nonterm/rtab_nonterm/tab_linear3 (carving, op_zpat arm untouched) vs l3_spl_len_pure/len_nonterm/lit_len/pos (op_zfc==0 controls, byte-identical -- the ternary's false arm is the exact untouched legacy expression). */
         + IF(_.op_zres,  x86("note", ZOPN(1)) + x86("lea", "r9", ZOPQ(1, 0)))   /* ZD-5 MATCH-SPINE: the replacement travels BY ADDRESS -- r9 points at rv's own cell instead of the flat replp slot the armed producer no longer writes. */
         + IF(!_.op_zres, x86("lea",  "r9",  FRQ(_.op_sb)))
         + x86("call", "rt_match_replace", (uint64_t)(uintptr_t)(void *)rt_match_replace)
         + x86_align_leave()
         + x86_jmp_id(1)
         + x86("def",    L(0))
         + x86(".quad",  LS(0), _.op_sval ? _.op_sval : "")
         + x86("label",  LS(0))
         + x86(".string", _.op_sval ? _.op_sval : "")
         + x86_deflabel_id(1)
         + IF(_.flat_deep_arrival, x86("note", HKN(0)) + std::string(""))   /* BRACKET-GATE (s193): paired with head's gated +40 save.  OBJ-NOTE ON-3 (s23e): third restore site of the same k=0 slot, named from the one HKN table with head and release.  REPL-PIN (this session): the !op_stmt_pin gate release:78 has carried since s22z lands here too -- the widened pin put replace statements INSIDE the pin population for the first time, and this unconditional restore mid-statement handed the terminal cut a stale base (measured: tB correct output then SEGV; patmin died on INNER re-entry).  Under the pin the terminal cut is the SOLE restore authority, per the head's own twin-gate law. */
         + x86_gamma();
}
