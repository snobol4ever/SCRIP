#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int rt_kw_static_on(void);
int rt_kw_index(const char *kw);
DESCR_t rt_kw_write_idx(int64_t idx, DESCR_t v);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* KW-3b (GOAL-SNOBOL4-100 D-3): THE STATIC KEYWORD SLOT ON THE WRITE SIDE. The read half (bb_keyword_snobol4, KW-3) seals the keyword's canonical block index as a static quad and loads it rip-relative;
 * this is the same shape for `&KW = value`. It replaces a BY-NAME BUILTIN CALL -- the KW-1 census measured `.string "SNO$KWSET"` -> rt_call_arr@PLT -> to_cstring -> a ~60-arm strcmp cascade, and THEN a
 * second cascade inside rt_keyword_write_snobol4 to find the keyword itself -- with one rip-relative load and an O(1) array index. The lowerer only mints this kind when the killswitch is armed AND the
 * block actually names the keyword, so kwi < 0 is unreachable in a well-formed graph and is bombed rather than silently mis-writing a neighbour's cell.
 * ⭐ KW-5: THE OMEGA EDGE THIS BOX WAS BORN WITHOUT, NOW EARNED. A SNOBOL4 keyword write RAISES (208 value-not-integer / 209 protected), and at the default &ERRLIMIT of 0 an execution error still
 * TERMINATES the program inside kwb_write_ent -- the omega below is never reached and the old gamma-only topology is preserved. But manual Ch.16 &ERRLIMIT says a NON-ZERO &ERRLIMIT decrements the
 * count, suppresses the message and "converts the error to statement failure", and kwb_error (keywords.c) now implements exactly the no-SETEXIT arm of that sentence: rt_kw_write_idx answers FAILDESCR,
 * the cmp/je takes the omega edge the lowerer has wired since KW-3b (lc_build(..., γ, ω)), and the statement takes its :F branch with &ERRTYPE/&ERRTEXT already holding the code and text -- the
 * kw_protected_write witness reads both from its failure arm. Icon's IR_KEYWORD_ASSIGN remains a separate KIND for the two measured reasons of record: `&pos := v` fails on a DIFFERENT topology
 * (genuine failure, no error machinery), and `TRACE`/`DUMP` are named by BOTH keyword namespaces so a shared template dispatching on the NAME could not tell an Icon &trace from a SNOBOL4 &TRACE.
 * VALUE CONTRACT PRESERVED: the box yields the ASSIGNED VALUE, because the SNO$KWSET builtin this replaces ends `*out = args[1]` (by_name_dispatch.c). rt_kw_write_idx now RETURNS that value
 * (or FAILDESCR), so the result is stored from the return registers exactly like the read side -- the old operand-slot reload is retired along with its caller-saved-register rationale. */
std::string bb_keyword_assign_snobol4() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_a_slot < 0 && !_.op_zres) return x86_alpha() + x86_bomb("bb_keyword_assign_snobol4: rhs operand slot unresolved");
    const int kwi = (rt_kw_static_on() && _.op_sval) ? rt_kw_index(_.op_sval) : -1;
    if (kwi < 0) return x86_alpha() + x86_bomb("bb_keyword_assign_snobol4: keyword not named by the block");
    if (_.op_zres)
        return x86("comment", "IR_KEYWORD_ASSIGN_SNOBOL4 zd [KW-3b static idx]")
             + x86_alpha()
             + x86_ro_load_q("rdi", 0)
             + x86("mov",     "rsi", ZOPQ(0, 0))
             + x86("mov",     "rdx", ZOPQ(0, 8))
             + x86("call",    "rt_kw_write_idx", (uint64_t)(uintptr_t)(void *)rt_kw_write_idx)
             + x86("cmp",     "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86_ro_seal_q(0, (uint64_t)(int64_t)kwi);   /* KW-5: the result is the CALL'S RETURN (rax:rdx = assigned value, or FAILDESCR routed to omega above), so no operand reload is needed -- same store shape as bb_keyword_snobol4's read. */
    if (!(_.op_off >= 0)) return x86_alpha() + x86_bomb("bb_keyword_assign_snobol4: no result slot");
    return x86("comment", "IR_KEYWORD_ASSIGN_SNOBOL4 [KW-3b static idx]")
         + x86_alpha()
         + x86_ro_load_q("rdi", 0)
         + x86("mov",     "rsi", FRQ(_.op_a_slot))
         + x86("mov",     "rdx", FRQ(_.op_a_slot + 8))
         + x86("call",    "rt_kw_write_idx", (uint64_t)(uintptr_t)(void *)rt_kw_write_idx)
         + x86("cmp",     "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86_ro_seal_q(0, (uint64_t)(int64_t)kwi);
}
