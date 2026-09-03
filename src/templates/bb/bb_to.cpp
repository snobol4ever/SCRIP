#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
void    rt_pl_tr_unwind(void *);
void    rt_pl_disj_open(void *, void *);
int     rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
int64_t to_int(DESCR_t v);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ RUNG 7 (ARCH sec B.13, sec E rung 7) -- THE GENERATOR'S OWN CHOICE POINT NEEDS THE TRAIL, AND ONLY WHERE A TRAIL EXISTS.
   `between(L,H,X)` lowers to this box (Lon: share BB's, do not clone them), and a Prolog generator differs from Icon's `i to j`
   in exactly one way: each value it hands out may BIND variables, and a redo must UNDO those bindings before handing out the next.
   Without this, `between(1,3,X), write(X), nl, fail` prints `1` and stops -- X is still bound to 1 when the box offers 2, the
   unify fails against the stale binding, and the generator drains in silence. MEASURED, that is exactly what it printed.
   ⛔ THE REGIME KEY IS `x86_fb_pinned()`, NEVER A LANGUAGE NAME -- the same predicate bb_disjunction uses for its own step unwind,
   and the same discipline the s272 shared-node cure landed on after a language-blind grant over IR_DISJUNCTION cost 47 Icon
   programs. Icon and Raku also lower to IR_TO (`grep -c IR_TO src/lower/lower_*.c`: prolog 3, raku 3, icon 2); neither pins the
   frame base, so both emit NOT ONE BYTE of this and are graded to prove it, not assumed to be safe.
   ⭐ The mark is r12 (TR) read at alpha into the box's own state slot; `op_off + 24` is granted for every IR_TO -- the real-number
   arm above writes it unconditionally -- and is unused by this integer arm. rt_pl_tr_unwind takes the mark in rdi and is the ONLY
   writer of r12 on this path, which keeps the quad gate's enrolment intact. */
static std::string to_trail_mark() {
    if (!x86_fb_pinned()) return std::string();
    int kt = g_emit.flat_frame_bytes;
    uint64_t fp; { void (*f)(void *, void *) = rt_pl_disj_open; fp = (uint64_t)(uintptr_t)(void *)f; }
    /* ⛔⭐ BANKING THE MARK IS NOT ENOUGH -- THE GENERATOR MUST *OPEN A CHOICE*, AND THAT IS WHY THE FIRST CUT OF THIS SILENTLY DID NOTHING (ceo ruling 2026-09-02 20:30,
       superseding sec B.13 (i)'s bare `F.B := rbp`). Prolog trails CONDITIONALLY: the rung-2 clause step re-seeds clause locals with `rep stosb` precisely because they are
       younger than any live choice and so were never logged. A generator that only marks r12 registers no choice at all, every local stays younger than everything, NOTHING
       IS TRAILED, and the beta unwind below runs over an empty suffix. MEASURED before the cure: `between(1,3,X), write(X), nl, fail` printed `1` and stopped -- X was still
       bound to 1 when the box offered 2, so the unify failed against its own stale binding. Right control flow, wrong answer.
       ⭐ rt_pl_disj_open is hq_C's rung-3 mechanism REUSED, not a second one: it lowers THIS frame's log threshold F.HI at [H+32] to the frame base -- so every cell of this
       activation becomes loggable while a value is untried -- and raises B to H only when the live choice is OLDER than this frame or absent, leaving a younger retained
       callee as the choice. B is never written by emitted code, only by the named rtx helper. Cut's B lifecycle (B := F.B0 restore) is rung 4's, hq_C's, not this box's,
       which is why omega carries no restore arm here -- bb_disjunction has none either. */
    return x86("comment", "PL GENERATOR CHOICE (rung 7, ARCH sec B.13 (i) as ruled): bank the trail top as this generator's mark AND open the choice through the same rtx helper the rung-3 disjunction uses, so the values this box hands out are loggable and its redo has something to undo.")
         + x86("mov", FRQ(_.op_off + 24), "r12")
         + x86("lea", "rdi", RDQ(x86_fb(), kt - 64))
         + x86("mov", "rsi", x86_fb())
         + x86("call_bare", "rt_pl_disj_open", fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string to_trail_unwind() {
    if (!x86_fb_pinned()) return std::string();
    uint64_t fp; { void (*f)(void *) = rt_pl_tr_unwind; fp = (uint64_t)(uintptr_t)(void *)f; }
    return x86("comment", "PL GENERATOR STEP (rung 7, ARCH sec B.13): a solution was refused, so undo everything it bound back to this box's mark before the cursor advances -- the same rtx helper the rung-2 clause step and the rung-3 disjunction step use.")
         + x86("mov", "rdi", FRQ(_.op_off + 24))
         + x86("call_bare", "rt_pl_tr_unwind", fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_to() {
    x86_begin();
    /* ⛔⭐ RUNG 7 SAFETY (hq_C review point 2, 2026-09-02): THE TRAIL MARK BELOW LIVES AT op_off+24, WHICH IS `to.limit` IN THE OTHER TWO ARMS.
       zeta_storage.c grants IR_TO the 16-byte result plus `to.I` at +16 and `to.limit` at +24. The zd arm and the real arm both WRITE the limit
       there at alpha and re-read it every iteration; only the integer-static-operand arm keeps its limit in op_sb+8, leaving +24 free -- and that
       is the arm between/3 and sub_atom/5 take today. ⛔ NOTHING ENFORCES WHICH ARM IS REACHED, so a selector change or a generator that lands on
       either other arm would ALIAS THE MARK ONTO THE LOOP BOUND and run to a garbage limit -- a wrong answer, not a crash.
       ⭐ SO A PINNED GRAPH REACHING THOSE ARMS REFUSES LOUDLY RATHER THAN COMPUTING SOMETHING PLAUSIBLE. hq_C recommended instead granting IR_TO a
       FOURTH word so the MAP reserves the mark; that is the better long-term shape and is the one to take when a rung is willing to pay for it --
       but the grant is language-blind, so it moves every Icon and Raku frame that holds a `to` as well, and ceo's rung-7 ruling requires those
       graphs to stay BYTE-IDENTICAL. Refusing keeps both properties until the layout change can be made deliberately and graded on all three. */
    if (x86_fb_pinned() && (_.op_zres || _.op_num_real))
        return x86_alpha() + x86_bomb("IR_TO: a Prolog generator reached the zd/real arm, where op_off+24 is to.limit and the rung-7 trail mark would alias the loop bound -- grant IR_TO a fourth word before enabling this path");
        if (_.op_zres)
            return x86("comment", "IR_TO zd")
                 + x86_alpha()
                 + x86("note",  ZOPN(0))
                 + x86("mov", "rdi", ZOPQ(0, 0))
                 + x86("note",  ZOPN(0))
                 + x86("mov", "rsi", ZOPQ(0, 8))
                 + x86("call",  "to_int", (uint64_t)(uintptr_t)(void*)to_int)
                 + x86("mov",   FRQ(_.op_off + 16), "rax")
                 + x86("note",  ZOPN(1))
                 + x86("mov", "rdi", ZOPQ(1, 0))
                 + x86("note",  ZOPN(1))
                 + x86("mov", "rsi", ZOPQ(1, 8))
                 + x86("call",  "to_int", (uint64_t)(uintptr_t)(void*)to_int)
                 + x86("mov",   FRQ(_.op_off + 24), "rax")
                 + x86("def",   L(0))
                 + x86("mov",   "rax",   FRQ(_.op_off + 16))
                 + x86("mov",   "rcx",   FRQ(_.op_off + 24))
                 + x86("cmp",   "rax",   "rcx")
                 + x86_omega(  "jg")
                 + x86("note",  ZRESN())
                 + x86("mov", ZRES(0),  (long)DT_I)
                 + x86("note",  ZRESN())
                 + x86("mov", ZRES(8),  "rax")
                 + x86_gamma()
                 + x86_beta()
                 + x86("inc",   FRQ(_.op_off + 16))
                 + x86("jmp",   L(0));
        return !(_.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0) ? x86_alpha() + x86_bomb("bb_to: unhandled (needs static operands, descr flat-chain)") :
               _.op_num_real ?
               x86("comment", "IR_TO")
             + x86_alpha()
             + x86("mov",     "rax", FRQ(_.op_sa))
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 24), "rax")
             + x86("def",     L(10))
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", FRQ(_.op_off + 24))
             + x86("mov",     "rdx", FRQ(_.op_sb))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("mov",     "r8d", (long)BINOP_LE)
             + x86("call",    "rt_jct_relop", (uint64_t)(uintptr_t)(void*)rt_jct_relop)
             + x86("test",    "eax", "eax")
             + x86_omega("jz")
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     "rax", FRQ(_.op_off + 24))
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             + x86_beta()
             + x86("mov",     "rdi", FRQ(_.op_off + 16))
             + x86("mov",     "rsi", FRQ(_.op_off + 24))
             + x86("mov",     "rdx", ROQ(0))
             + x86("mov",     "rcx", ROQ(1))
             + x86("mov",     "r8d", (long)BINOP_ADD)
             + x86("call",    "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + x86("mov",     FRQ(_.op_off + 24), "rdx")
             + x86("jmp",     L(10))
             + x86("def",     L(0))
             + x86(".quad",   (uint64_t)(int64_t)DT_R)
             + x86("def",     L(1))
             + x86(".quad",   (uint64_t)(int64_t)1) :
               x86("comment", "IR_TO")
             + x86_alpha()
             + x86("mov",     "rdi", FRQ(_.op_sa))
             + x86("mov",     "rsi", FRQ(_.op_sa + 8))
             + x86("call",    "to_int", (uint64_t)(uintptr_t)(void*)to_int)
             + x86("mov",     FRQ(_.op_sa),     (long)DT_I)
             + x86("mov",     FRQ(_.op_sa + 8), "rax")
             + x86("mov",     "rdi", FRQ(_.op_sb))
             + x86("mov",     "rsi", FRQ(_.op_sb + 8))
             + x86("call",    "to_int", (uint64_t)(uintptr_t)(void*)to_int)
             + x86("mov",     FRQ(_.op_sb),     (long)DT_I)
             + x86("mov",     FRQ(_.op_sb + 8), "rax")
             + x86("mov",     "rax", FRQ(_.op_sa + 8))
             + x86("mov",     FRQ(_.op_off + 16), "rax")
             + to_trail_mark()
             + x86("def",     L(0))
             + x86("mov",     "rax", FRQ(_.op_off + 16))
             + x86("mov",     "rcx", FRQ(_.op_sb + 8))
             + x86("cmp",     "rax", "rcx")
             + x86_omega("jg")
             + x86("mov",     FRQ(_.op_off),     (long)DT_I)
             + x86("mov",     FRQ(_.op_off + 8), "rax")
             + x86_gamma()
             /* ⛔ NO OMEGA ARM, AND THAT IS DELIBERATE -- DO NOT "FIX" IT (hq_C review point 3, 2026-09-02). "Omega restores as bb_disjunction does"
                is satisfied by doing NOTHING: bb_disjunction leaves F.HI lowered on purpose, because over-logging is safe and restoring it would need
                kt at a site that does not otherwise want it. Cut's B lifecycle (B := F.B0) is rung 4's and hq_C's, not this box's. The beta below also
                needs NO `test r15,r15` ball guard: sec A.1 review C9 puts that on beta arms that can hold a RETAINED CALLEE, and a generator leaf holds none. */
             + x86_beta()
             + to_trail_unwind()
             + x86("inc",     FRQ(_.op_off + 16))
             + x86("jmp",     L(0));
}
