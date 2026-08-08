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

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_concat_slot() {
    if (PLATFORM_X86 && _.op_zres)
        return x86("comment", "IR_BINOP_CONCAT zd")
             + x86_alpha()
             + x86("note", ZOPN(0)) + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0)) + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1)) + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1)) + x86("mov", "rcx", ZOPQ(1, 8))
             + x86("rtcc_wb")
             + x86("call_bare", "str_concat_d", (uint64_t)(uintptr_t)(void*)str_concat_d)
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86("rtcc_rl")
             + x86_gamma()
             + x86_beta_trampoline();   /* ZD-2a: the purest clone of bb_binop_arith's ZD arm -- operands are the two producers' suspended cells at the driver-staged differences op_zread[0]/[1] (mode 3), the result is this box's own alpha-carved cell (mode 1), and no release rides the body: cells persist to the statement boundary where op_zgpop/op_wpop restore rsp wholesale.  NO DT_FAIL test and NO omega edge, matching BOTH legacy arms below and the SPITBOL semantics they encode: concatenation propagates failure only from its OPERANDS (manual Ch.4 p.33 "if any function failed, the entire concatenation would fail" -- the LT(N,10) N + 1 idiom), which in the four-port model is the PRODUCER box taking its own omega so this box never runs; the concat operation itself has no failure mode, and a non-string non-pattern operand is an Appendix-D ERROR against &ERRLIMIT, not a statement failure.  str_concat_d stays the sole authority for the type-preserving null-string identity (manual Ch.3 p.22: (20-17) '' is the INTEGER 3, not the string) -- the DESCR pair is handed through unexamined, so the arm must never coerce or shortcut on either operand. */
    return IF(PLATFORM_X86 && bcs_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_CONCAT")
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("call_rt", "str_concat_d", (long)_.op_off, (uint64_t)(uintptr_t)(void*)str_concat_d)
         + x86_gamma()
         + x86_beta_trampoline());
}
