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
static inline const char * cmp_fail_jcc() { switch ((int)_.op_ival) { case 0: return "jne"; case 1: return "je"; case 2: return "jns"; case 3: return "jg"; case 4: return "jle"; default: return "js"; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cmp_test() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    if (_.op_sa < 0 || _.op_sb < 0 || _.op_off < 0) return x86_bomb("bb_cmp_test: needs two coerced operand slots (op_sa/op_sb) + own value slot (op_off)");
    return x86("comment", "IR_CMP_TEST")
         + x86_alpha()
         + x86("lea",  "rdi", FRQ(_.op_sa))
         + x86("lea",  "rsi", FRQ(_.op_sb))
         + x86("call", "rt_cmp_d", (uint64_t)(uintptr_t)(void *)rt_cmp_d)
         + x86("test", "eax", "eax")
         + x86_omega(cmp_fail_jcc())
         + x86("mov",  FRQ(_.op_off),     (long)0)
         + x86("mov",  FRQ(_.op_off + 8), (long)0)
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
