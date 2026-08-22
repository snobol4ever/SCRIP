#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int rt_cmp_d(const DESCR_t *a, const DESCR_t *b);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* THE FAIL CONDITION, twice.  ct_fail_sign() reads the SIGN rt_cmp_d returned in eax after `test eax,eax`; ct_fail_int() reads the
   flags `cmp rax,rdx` set directly on two DT_I values.  Four of the six predicates share a mnemonic between the two -- only LT and
   GE differ (jns/js are sign-of-a-difference tests, jge/jl are the signed comparisons) -- so the pair is written out, not derived. */
static inline const char * ct_fail_sign() { return (int)_.op_ival == 0 ? "jne" : (int)_.op_ival == 1 ? "je" : (int)_.op_ival == 2 ? "jns" : (int)_.op_ival == 3 ? "jg" : (int)_.op_ival == 4 ? "jle" : "js"; }
static inline const char * ct_fail_int()  { return (int)_.op_ival == 0 ? "jne" : (int)_.op_ival == 1 ? "je" : (int)_.op_ival == 2 ? "jge" : (int)_.op_ival == 3 ? "jg" : (int)_.op_ival == 4 ? "jle" : "jl"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cmp_test() {
    x86_begin();
    if (PLATFORM_X86) {
        /* ⭐ INTEGER COMPARE IS THREE INSTRUCTIONS, NOT A CALL (s249).  Both operands arrive from coerce_numeric, so DT_I ⊕ DT_I is the
           overwhelmingly common shape and `cmp rax, rdx` decides it outright; rt_cmp_d stays for everything else (real, string, mixed).
           Measured on arith_loop: the box was 13 in-line + 14 inside rt_cmp_d = 27 of 133 instructions per iteration for one `LT`. */
        if (_.op_zres)
            return x86("comment", "IR_CMP_TEST zd")
                 + x86_alpha()
                 + x86("note", ZOPN(0)) + x86("mov", "eax", ZOPD(0, 0))
                 + x86("note", ZOPN(1)) + x86("mov", "ecx", ZOPD(1, 0))
                 + x86("mov", "edx", "eax")
                 + x86("and", "edx", "ecx")
                 + x86("cmp", "edx", (long)DT_I)
                 + x86("jne", L(0))
                 + x86("note", ZOPN(0)) + x86("mov", "rax", ZOPQ(0, 8))
                 + x86("note", ZOPN(1)) + x86("mov", "rdx", ZOPQ(1, 8))
                 + x86("cmp", "rax", "rdx")
                 + x86_omega(ct_fail_int())
                 + IF(_.op_res_live, x86("note", ZRESN()) + x86("mov", ZRES(0), (long)0)
                                   + x86("note", ZRESN()) + x86("mov", ZRES(8), (long)0))
                 + x86_gamma()
                 + x86("def", L(0))
                 + x86("note", ZOPN(0)) + x86("lea",  "rdi", ZOPQ(0, 0))
                 + x86("note", ZOPN(1)) + x86("lea",  "rsi", ZOPQ(1, 0))
                 + x86("call", "rt_cmp_d", (uint64_t)(uintptr_t)(void *)rt_cmp_d)
                 + x86("test", "eax", "eax")
                 + x86_omega(ct_fail_sign())
                 + IF(_.op_res_live, x86("note", ZRESN()) + x86("mov", ZRES(0), (long)0)
                                   + x86("note", ZRESN()) + x86("mov", ZRES(8), (long)0))
                 + x86_gamma()
                 + x86_beta_trampoline();
        return (_.op_sa >= 0 && _.op_sb >= 0 && _.op_off >= 0)
                 ? x86("comment", "IR_CMP_TEST")
                 + x86_alpha()
                 + x86("lea",  "rdi", FRQ(_.op_sa))
                 + x86("lea",  "rsi", FRQ(_.op_sb))
                 + x86("call", "rt_cmp_d", (uint64_t)(uintptr_t)(void *)rt_cmp_d)
                 + x86("test", "eax", "eax")
                 + x86_omega(ct_fail_sign())
                 + IF(_.op_res_live, x86("mov",  FRQ(_.op_off),     (long)0)
                                   + x86("mov",  FRQ(_.op_off + 8), (long)0))
                 + x86_gamma()
                 + x86_beta_trampoline()
             : x86_bomb("bb_cmp_test: needs two coerced operand slots (op_sa/op_sb) + own value slot (op_off)");
    }
    return std::string();
}
