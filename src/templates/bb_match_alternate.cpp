#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
#include "x86_asm.h"
extern "C" int  rt_dcap_height(void);
extern "C" void rt_dcap_restore_to(int h);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string alt_dispatch_chain(long N, int base, int lo) { std::string r; for (long i = lo; i < N; i++) r += x86("cmp", "eax", (int)i) + x86("je", PAIR((int)(base + i))); return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_alternate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_MATCH_ALTERNATE: cursor slot not granted (zls)")
             : x86("comment", "IR_MATCH_ALT_NARY")
             + x86_alpha()
             + x86("mov", FR(_.op_off), "r14d")
             + x86_align_enter()
             + x86("call", "rt_dcap_height", (uint64_t)(uintptr_t)(void *)(int (*)(void))rt_dcap_height)
             + x86_align_leave()
             + x86("mov", FR(_.op_off + 4), "eax")
             + x86("mov", FR(_.op_off + 8), 0)
             + x86("jmp", PAIR(0))
             + x86("def", PAIR((int)(2 * _.op_ival)))
             + x86_gamma()
             + x86_beta()
             + x86("mov", "eax", FR(_.op_off + 8))
             + alt_dispatch_chain(_.op_ival - 1, (int)_.op_ival, 0)
             + x86("jmp", PAIR((int)(_.op_ival + _.op_ival - 1)))
             + x86("def", PAIR((int)(2 * _.op_ival + 1)))
             + x86("add", FR(_.op_off + 8), 1)
             + x86("mov", "r14d", FR(_.op_off))
             + x86("mov", "edi", FR(_.op_off + 4))
             + x86_align_enter()
             + x86("call", "rt_dcap_restore_to", (uint64_t)(uintptr_t)(void *)(void (*)(int))rt_dcap_restore_to)
             + x86_align_leave()
             + x86("mov", "eax", FR(_.op_off + 8))
             + alt_dispatch_chain(_.op_ival, 0, 1)
             + x86_omega();
}
