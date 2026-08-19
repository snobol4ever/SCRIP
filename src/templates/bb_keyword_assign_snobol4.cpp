#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int rt_kw_static_on(void);
int rt_kw_index(const char *kw);
void rt_kw_write_idx(int64_t idx, DESCR_t v);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* KW-3b (GOAL-SNOBOL4-100 D-3): THE STATIC KEYWORD SLOT ON THE WRITE SIDE. The read half (bb_keyword_snobol4, KW-3) seals the keyword's canonical block index as a static quad and loads it rip-relative;
 * this is the same shape for `&KW = value`. It replaces a BY-NAME BUILTIN CALL -- the KW-1 census measured `.string "SNO$KWSET"` -> rt_call_arr@PLT -> to_cstring -> a ~60-arm strcmp cascade, and THEN a
 * second cascade inside rt_keyword_write_snobol4 to find the keyword itself -- with one rip-relative load and an O(1) array index. The lowerer only mints this kind when the killswitch is armed AND the
 * block actually names the keyword, so kwi < 0 is unreachable in a well-formed graph and is bombed rather than silently mis-writing a neighbour's cell.
 * ⛔ GAMMA-ONLY, AND THAT IS THE ORACLE'S TOPOLOGY, NOT AN OMISSION. A SNOBOL4 keyword write does not FAIL -- it RAISES (208 value-not-integer / 209 protected) -- and at the default &ERRLIMIT of 0 an
 * execution error TERMINATES the program. Manual Ch.16 &ERRLIMIT is explicit that only a NON-ZERO &ERRLIMIT decrements the count, suppresses the message and "converts the error to statement failure",
 * and SCRIP has no such mechanism today (core_runtime_error exits; kw_protected_write is red for exactly this reason and is routed to the KW-5/ERRLIMIT rung). Giving this box an omega edge would
 * therefore encode a statement-failure path that neither the oracle nor this runtime can currently take -- the same reasoning that keeps the READ template gamma-only. Icon's IR_KEYWORD_ASSIGN is the
 * opposite case (`&pos := v` genuinely fails and takes omega), which is one of the two measured reasons this is a separate KIND rather than an arm of bb_keyword_assign; the other is that `TRACE` and
 * `DUMP` are named by BOTH keyword namespaces, so a shared template dispatching on the keyword NAME could not tell an Icon &trace from a SNOBOL4 &TRACE.
 * VALUE CONTRACT PRESERVED: the box yields the ASSIGNED VALUE, because the SNO$KWSET builtin this replaces ends `*out = args[1]` (by_name_dispatch.c). rt_kw_write_idx returns void, so the value is
 * copied from the operand slot into the result slot directly rather than from a return register -- one fewer dependency than the read side, not a different contract. */
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
             + x86("mov",     "rax", ZOPQ(0, 0))
             + x86("mov",     "rdx", ZOPQ(0, 8))
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline()
             + x86_ro_seal_q(0, (uint64_t)(int64_t)kwi);   /* The operand slot is re-read AFTER the call rather than stashed across it: rsi/rdx are caller-saved and rt_kw_write_idx may clobber both, and the slot is this box's own frame storage which the callee cannot touch. Reloading is two moves; spilling to a scratch cell would cost a slot and a store for the same result. */
    if (!(_.op_off >= 0)) return x86_alpha() + x86_bomb("bb_keyword_assign_snobol4: no result slot");
    return x86("comment", "IR_KEYWORD_ASSIGN_SNOBOL4 [KW-3b static idx]")
         + x86_alpha()
         + x86_ro_load_q("rdi", 0)
         + x86("mov",     "rsi", FRQ(_.op_a_slot))
         + x86("mov",     "rdx", FRQ(_.op_a_slot + 8))
         + x86("call",    "rt_kw_write_idx", (uint64_t)(uintptr_t)(void *)rt_kw_write_idx)
         + x86("mov",     "rax", FRQ(_.op_a_slot))
         + x86("mov",     "rdx", FRQ(_.op_a_slot + 8))
         + x86("mov",     FRQ(_.op_off),     "rax")
         + x86("mov",     FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline()
         + x86_ro_seal_q(0, (uint64_t)(int64_t)kwi);
}
