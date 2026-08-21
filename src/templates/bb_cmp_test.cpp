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
std::string bb_cmp_test() {
    x86_begin();
    if (PLATFORM_X86) {
        if (_.op_zres)
            return x86("comment", "IR_CMP_TEST zd")
                 + x86_alpha()
                 + x86("note", ZOPN(0)) + x86("lea",  "rdi", ZOPQ(0, 0))
                 + x86("note", ZOPN(1)) + x86("lea",  "rsi", ZOPQ(1, 0))
                 + x86("call", "rt_cmp_d", (uint64_t)(uintptr_t)(void *)rt_cmp_d)
                 + x86("test", "eax", "eax")
                 + x86_omega((int)_.op_ival == 0 ? "jne" : (int)_.op_ival == 1 ? "je" : (int)_.op_ival == 2 ? "jns" : (int)_.op_ival == 3 ? "jg" : (int)_.op_ival == 4 ? "jle" : "js")
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
                 + x86_omega((int)_.op_ival == 0 ? "jne" : (int)_.op_ival == 1 ? "je" : (int)_.op_ival == 2 ? "jns" : (int)_.op_ival == 3 ? "jg" : (int)_.op_ival == 4 ? "jle" : "js")
                 + IF(_.op_res_live, x86("mov",  FRQ(_.op_off),     (long)0)
                                   + x86("mov",  FRQ(_.op_off + 8), (long)0))
                 + x86_gamma()
                 + x86_beta_trampoline()
             : x86_bomb("bb_cmp_test: needs two coerced operand slots (op_sa/op_sb) + own value slot (op_off)");
    }
    return std::string();
}
