#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZTOS-1 (Lon s21x-o "Do not put RSP references directly into the templates"): private raw-rsp helper RETIRED -- call sites now speak the sanctioned spine accessor ZTOS/ZTOSD (x86_asm.h), which adds op_zdepth so a box's own carve and its own TOS reads compose instead of colliding.  Byte-identical while this kind is unarmed (op_zdepth==0); correct once it is armed, which is what lets the _spine exclusion list retire. */
static inline int bcs_ok() { return _.op_off >= 0 && _.op_ival == BINOP_CONCAT && _.op_sa >= 0 && _.op_sb >= 0; }
extern "C" int zc_nofc(void);
static inline int nofc() { return zc_nofc(); }   /* NOFC-ONE (s22r): DELEGATES to the single authority in zeta_storage.c.  s22l declared zc_nofc THE ONE AUTHORITY and moved the policy there, but these three template-local copies SURVIVED with their own getenv -- four independent reads of one switch, agreeing only by coincidence of polarity, which is precisely the producer/consumer asymmetry s22l had just finished diagnosing.  A default flip that moved zc_nofc alone would have re-armed that disease in three templates at once. */
static inline int vfcc() { return !nofc() && x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0 && _.op_ival == BINOP_CONCAT; }   /* ZB-VAL-6a: registered value-spine concat -- operands are the TOP TWO cells (a=[rsp+16..31], b=[rsp+0..15]).  str_concat_d is TYPE-BLIND (it takes whatever DESCR pair it is handed and owns the null-string identity + the to-string coercion, SPITBOL manual p.21-22), so unlike the arith arm this needs NO fast/overload/generic ladder and NO omega edge -- the whole box is ONE call plus the net */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_concat_slot() {
    if (PLATFORM_X86 && _.op_zres)
        return x86("comment", "IR_BINOP_CONCAT zd")
             + x86_alpha()
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("mov", "rcx", ZOPQ(1, 8))
             + x86("call", "str_concat_d", (uint64_t)(uintptr_t)(void*)str_concat_d)
             + x86("mov", ZRES(0), "rax")
             + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();   /* ZD-2a: the purest clone of bb_binop_arith's ZD arm -- operands are the two producers' suspended cells at the driver-staged differences op_zread[0]/[1] (mode 3), the result is this box's own alpha-carved cell (mode 1), and no release rides the body: cells persist to the statement boundary where op_zgpop/op_wpop restore rsp wholesale.  NO DT_FAIL test and NO omega edge, matching BOTH legacy arms below and the SPITBOL semantics they encode: concatenation propagates failure only from its OPERANDS (manual Ch.4 p.33 "if any function failed, the entire concatenation would fail" -- the LT(N,10) N + 1 idiom), which in the four-port model is the PRODUCER box taking its own omega so this box never runs; the concat operation itself has no failure mode, and a non-string non-pattern operand is an Appendix-D ERROR against &ERRLIMIT, not a statement failure.  str_concat_d stays the sole authority for the type-preserving null-string identity (manual Ch.3 p.22: (20-17) '' is the INTEGER 3, not the string) -- the DESCR pair is handed through unexamined, so the arm must never coerce or shortcut on either operand. */
    return IF(PLATFORM_X86 && vfcc(),
           x86_alpha()
         + x86("comment", "IR_BINOP_CONCAT fc")
         + x86("mov", "rdi", ZTOS(16))
         + x86("mov", "rsi", ZTOS(24))
         + x86("mov", "rdx", ZTOS(0))
         + x86("mov", "rcx", ZTOS(8))
         + x86("call", "str_concat_d", (uint64_t)(uintptr_t)(void*)str_concat_d)
         + x86_zrelease(16)
         + x86("mov", ZTOS(0), "rax")
         + x86("mov", ZTOS(8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline())
         + IF(PLATFORM_X86 && !vfcc() && bcs_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_CONCAT")
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("call", "str_concat_d", (uint64_t)(uintptr_t)(void*)str_concat_d)
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline());
}
