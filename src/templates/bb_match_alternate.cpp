#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ALT-FLAT (s202, Lon design session): the N-ary alternation as a ZERO-CELL flat box with ADDRESS dispatch.
 * Three facts replace the whole ZB-FC-3a pad-to-max machinery (S10d), which is deleted:
 *   (1) FLAT ARMS.  Every node inside a granted ALT's arms declines its FORTH cell (fc_arm_member,
 *       zeta_storage.c) and keeps its flat zls quad -- an arm's rsp footprint is 0, so every arm yields at
 *       the ALT's own frontier and nodes after the ALT sit at the SAME static depth for every arm.  No pad,
 *       no per-arm exact footprints, no uniform-depth stubs.  "Fixed offsets all the way down the graph."
 *   (2) ADDRESS OVER INDEX.  The live-alternative counter alt_i and BOTH of its cmp-chains (beta resume
 *       dispatch, af entry advance) are dead.  Each arm's sigma stub stores its own resume trampoline's
 *       address (x86_lea_rip_id -- the encoder whose own comment names this use) into the box's
 *       alt.resume quad; alpha and each entry stub store the NEXT arm's entry-stub address into alt.next.
 *       beta = one indirect jmp through resume; af = delta restore + one indirect jmp through next.
 *   (3) ZETA STORAGE.  Box state is the flat quad pair: FR(op_off+0) delta (4B, +4 dead), FRQ(op_off+8)
 *       resume (ZK_PTR_CODE), FRQ(op_off+16) next (ZK_PTR_CODE).  Reads/writes happen at alpha/sigma/beta/af,
 *       all of which execute at the ALT's frontier (flat arms, LIFO downstream), so the registered flat
 *       displacement is correct in every regime the old !afc legacy path already served.
 * Pair map unchanged: 0..N-1 arm entries, N..2N-1 arm resumes, 2N merge, 2N+1 fail-advance, 2N+2+j sigma
 * stubs (driver mints them for EVERY match-alt now, emit.cpp).  Internal labels: 19 = pre-omega terminal,
 * 20+j = entry stubs (j=1..N-1), 40+j = resume trampolines. */
static std::string alt_entry_stubs(long N) {
    std::string r;
    for (long j = 1; j < N; j++)
        r += x86("def", L((int)(20 + j)))
           + x86_lea_rip_id("rax", (j + 1 < N) ? (int)(20 + j + 1) : 19)
           + x86("mov", FRQ(_.op_off + 16), "rax")
           + x86("jmp", PAIR((int)j));
    return r;
}
static std::string alt_sigma_stubs(long N) {
    std::string r;
    for (long j = 0; j < N; j++)
        r += x86("def", PAIR((int)(2 * N + 2 + j)))
           + x86_lea_rip_id("rax", (int)(40 + j))
           + x86("mov", FRQ(_.op_off + 8), "rax")
           + x86("jmp", PAIR((int)(2 * N)));
    for (long j = 0; j < N; j++)
        r += x86("def", L((int)(40 + j)))
           + x86("jmp", PAIR((int)(N + j)));
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_alternate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_MATCH_ALTERNATE: cursor slot not granted (zls)")
             : x86("comment", "IR_MATCH_ALT_NARY (ALT-FLAT)")
             + x86_alpha()
             + x86("mov", FR(_.op_off), "r14d")
             + x86_lea_rip_id("rax", (_.op_ival > 1) ? 21 : 19)
             + x86("mov", FRQ(_.op_off + 16), "rax")
             + x86("jmp", PAIR(0))
             + alt_entry_stubs(_.op_ival)
             + alt_sigma_stubs(_.op_ival)
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86_gamma()
             + x86_beta()
             + x86("mov", "rax", FRQ(_.op_off + 8))
             + x86_jmp_reg("rax")
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("mov", "r14d", FR(_.op_off))
             + x86("mov", "rax", FRQ(_.op_off + 16))
             + x86_jmp_reg("rax")
             + x86("def", L(19))
             + x86_omega();
}
