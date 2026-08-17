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
 *   (3) OWN-CARVED RECORD (s61, Lon in-chat: "the ALPHA is not carving its own BB LOCAL to store the NEXT
 *       POINTER").  WAS the registered-flat trio FR(op_off+0)/FRQ(op_off+8)/FRQ(op_off+16) — writes into the
 *       residual flat_frame_bytes graph region the box never carved (emit.cpp:2918 CARVE-ERAD names ALTERNATE
 *       as the witness of exactly this aliasing class), per-BLOB not per-activation, so nested/recursive
 *       activations of one node shared ONE trio (the DEL-T1 disease bb_match_arbno's ROOT-SPINE note
 *       documents).  NOW: alpha carves its OWN 32B record (sub rsp,32 — 16B quantum law), delta dword at
 *       [rsp+0] (+4 dead), resume quad [rsp+8], next quad [rsp+16], [rsp+24] pad; RDD/RSP direct spellings
 *       escape the FR classifier so no frame-base re-canonicalization.  alpha/sigma/beta/af all execute at
 *       the ALT's post-carve frontier (flat arms, LIFO downstream) so every offset is static; the record is
 *       per-activation BY CONSTRUCTION.  Release follows the unwind law clause 2 (emit.cpp:1937): the
 *       generator frees its OWN K at exhaust — add rsp,32 at L(19) before omega; beta and af keep the record
 *       live (backtrack re-entry + next-arm entry stubs still write [rsp+16]); success-side growth is
 *       NON-POPPING by THE MODEL, released by the bracket whacks, never by ALTERNATE.  zd_k stays 0 this
 *       rung (planner depth model unchanged; the sweep is the gate — flip to 32 there if FR readers
 *       downstream of a live ALT record measure displaced).
 * Pair map unchanged: 0..N-1 arm entries, N..2N-1 arm resumes, 2N merge, 2N+1 fail-advance, 2N+2+j sigma
 * stubs (driver mints them for EVERY match-alt now, emit.cpp).  Internal labels: 19 = pre-omega terminal,
 * 20+j = entry stubs (j=1..N-1), 40+j = resume trampolines. */
extern "C" int sn4_choice_rbp_off(void);   /* s128: ONE AUTHORITY in emit.cpp -- 0 = legacy rsp-carved record (byte-identical), else the negative rbp base of THIS blob's frame-resident 32B choice record (blob_frame_bytes carves it; res re-seats rbp from record[+24], so every spelling below is depth-immune under arm-interior carves) */
static std::string alt_entry_stubs(long N, int cro) {
    std::string r;
    for (long j = 1; j < N; j++)
        r += x86("def", L((int)(20 + j)))
           + x86_lea_rip_id("rax", (j + 1 < N) ? (int)(20 + j + 1) : 19)
           + (cro ? x86("mov", CROQ(cro, 16), "rax") : x86("mov", RSP(16), "rax"))
           + x86("jmp", PAIR((int)j));
    return r;
}
static std::string alt_sigma_stubs(long N, int cro) {
    std::string r;
    for (long j = 0; j < N; j++)
        r += x86("def", PAIR((int)(2 * N + 2 + j)))
           + x86_lea_rip_id("rax", (int)(40 + j))
           + (cro ? x86("mov", CROQ(cro, 8), "rax") : x86("mov", RSP(8), "rax"))
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
    const int cro = sn4_choice_rbp_off();   /* s128: 0 keeps every spelling below byte-identical; nonzero moves the WHOLE record (delta dword, resume quad, next quad) to [rbp+cro..+31] -- no carve, no free, per-activation by the blob frame (D12) */
    return x86("comment", cro ? "IR_MATCH_ALT_NARY (ALT-RBP s128, frame-resident choice record)" : "IR_MATCH_ALT_NARY (ALT-FLAT, s61 own-carved record)")
             + x86_alpha()
             + (cro ? x86("mov", CROD(cro, 0), "r14d")
                    : x86("sub", "rsp", 32L) + x86("mov", RDD("rsp", 0), "r14d"))
             + x86_lea_rip_id("rax", (_.op_ival > 1) ? 21 : 19)
             + (cro ? x86("mov", CROQ(cro, 16), "rax") : x86("mov", RSP(16), "rax"))
             + x86("jmp", PAIR(0))
             + alt_entry_stubs(_.op_ival, cro)
             + alt_sigma_stubs(_.op_ival, cro)
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86_gamma()
             + x86_beta()
             + (cro ? x86("mov", "rax", CROQ(cro, 8)) : x86("mov", "rax", RSP(8)))
             + x86_jmp_reg("rax")
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + (cro ? x86("mov", "r14d", CROD(cro, 0)) : x86("mov", "r14d", RDD("rsp", 0)))
             + (cro ? x86("mov", "rax", CROQ(cro, 16)) : x86("mov", "rax", RSP(16)))
             + x86_jmp_reg("rax")
             + x86("def", L(19))
             + (cro ? std::string() : x86("add", "rsp", 32L))
             + x86_omega();
}
