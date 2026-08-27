#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
extern int rt_case_eq(const DESCR_t *sel, const DESCR_t *key);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_case_arm() {
    if (_.op_ival == 1) {
        if (_.op_sb < 0 || _.op_off < 0) return x86_bomb("bb_case_arm(take): needs value slot (op_sb) + case slot (op_off)");
        return x86("comment", "IR_CASE_ARM take")
             + x86("mov",     "rax", FRQ(_.op_sb))
             + x86("mov",     "rdx", FRQ(_.op_sb + 8))
             + x86("mov",     FRQ(_.op_off),     "rax")
             + x86("mov",     FRQ(_.op_off + 8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    }
    if (_.op_sa < 0 || _.op_sb < 0) return x86_bomb("bb_case_arm: needs selector slot (op_sa) + key slot (op_sb)");
    return x86("comment", "IR_CASE_ARM")
         + x86("lea",     "rdi", FRQ(_.op_sa))
         + x86("lea",     "rsi", FRQ(_.op_sb))
         + x86("call",    "rt_case_eq", (uint64_t)(uintptr_t)(void *)rt_case_eq)
         + x86("test",    "eax", "eax")
         + x86_omega("jz")
         + x86_gamma()
         + x86_beta_trampoline();
}
