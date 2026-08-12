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
         : (_.op_off < 0 || _.op_sa < 0 || (_.op_sb < 0 && !_.op_zres)) ? x86_alpha() + x86_bomb("IR_MATCH_REPLACE: head/subject/repl slot unresolved")
         : x86("comment", "IR_MATCH_REPLACE")
         + x86_alpha()
         + x86_align_enter()
         + x86("mov",  "rdi", ROQ(0))
         + x86("mov",  "rsi", FRQ(_.op_sa))
         + x86("mov",  "rdx", FRQ(_.op_sa + 8))   /* ZD-5 HEAD-FOLD: armed and declined read the SAME op_sa subject slot -- the armed head mirrors the consumed subject DESCR there at fold time, restoring the OFF-world dataflow, so this arm is regime-blind again. */
         + [&]() -> std::string { std::string _cur = FR(_.op_off - _.op_zpat); std::string _end = FRQ(_.op_off + 24 - _.op_zpat);
              if (getenv("SCRIP_REPL_ADDR_DIAG")) fprintf(stderr, "[ADDR] cursor=%s end=%s op_off=%d op_zpat=%d op_zfc=%d\n", _cur.c_str(), _end.c_str(), _.op_off, _.op_zpat, _.op_zfc);
              return x86("mov", "ecx", _cur) + x86("mov", "r8", _end); }()   /* L-3b STEP-6 FIX (this session, FINDING-2026-08-12h): non-carving class root cause -- bb_match_end.cpp's rfc()-gated write (the ZC_FRAME_RSP arm, confirmed live via direct instrument: rfc=1/zc_frame=2 on every probed non-carving witness) stores head.cursor/head.end at [rsp + op_off + op_fc_disp + 32], where op_fc_disp comes from fc_head_fp(head) -- a real, LOWER-registered value (lower_snobol4.c:1828), not dead code (an earlier hand-trace this session wrongly concluded fc_head_register had zero call sites; a full-tree grep found it).  op_fc_disp itself cannot be read here (resets to -1 per node before IR_MATCH_REPLACE's own case runs), so op_zfc re-stages the SAME fc_head_fp(head) lookup at the same choke g_zd_zpat already uses (see emit.h's op_zfc field comment and g_zd_zfc's staging site for the mechanism).  ADDITIVE, matching the writer's own +op_fc_disp+32 formula exactly, replacing the prior op_zpat SUBTRACTION for this class.  op_zpat is LEFT UNCHANGED AND STILL USED (as -op_zpat) when op_zfc==0 -- op_zpat's own field comment documents it for the CARVING class (TAB/RTAB/POS/RPOS) which this fix does not touch and which remains a separate, still-open defect (LIVE CURSOR s36: TAB confirmed NOT the same defect, different signature -- displaced-but-bounded vs this class's catastrophic flat-read).  Witnesses: l3_spl_span_nonterm/arb_nonterm/break_nonterm/rem_nonterm/VACUOUS_terminal_trap (non-carving, expected to flip) vs l3_spl_tab_nonterm/rtab_nonterm/tab_linear3 (carving, expected to remain unchanged by this edit) vs l3_spl_len_pure/len_pure/lit_len/pos (op_zfc==0 controls, must stay byte-identical -- old formula with op_zpat=0 and new formula's op_zfc=0 fallback are numerically identical: -op_zpat=0 either way). */
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
         + IF(_.flat_deep_arrival, x86("note", HKN(0)) + x86("mov", "rbp", FRQ(_.op_off + 40)))   /* BRACKET-GATE (s193): paired with head's gated +40 save.  OBJ-NOTE ON-3 (s23e): third restore site of the same k=0 slot, named from the one HKN table with head and release.  REPL-PIN (this session): the !op_stmt_pin gate release:78 has carried since s22z lands here too -- the widened pin put replace statements INSIDE the pin population for the first time, and this unconditional restore mid-statement handed the terminal cut a stale base (measured: tB correct output then SEGV; patmin died on INNER re-entry).  Under the pin the terminal cut is the SOLE restore authority, per the head's own twin-gate law. */
         + x86_gamma();
}
