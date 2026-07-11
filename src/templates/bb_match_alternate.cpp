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
std::string bb_match_alternate() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_MATCH_ALTERNATE: cursor slot not granted (zls)")
         : (int)_.op_phase == 0
             ? x86("comment", "IR_MATCH_ALT_SAVE")
             + x86_alpha()
             + x86("mov", FR(_.op_off), "r14d")
             + x86_align_enter()
             + x86("call", "rt_dcap_height", (uint64_t)(uintptr_t)(void *)(int (*)(void))rt_dcap_height)
             + x86_align_leave()
             + x86("mov", FR(_.op_off + 4), "eax")
             + x86_gamma()
             + x86_beta()
             + x86("jmp", FRQ(_.op_off + 8))
         : x86("comment", "IR_MATCH_ALT_JOIN")
             + x86_alpha()
             + x86("lea", "rax", L(0))
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86_omega()
             + x86_beta()
             + x86("def", L(0))
             + x86("mov", "r14d", FR(_.op_off))
             + x86("mov", "edi", FR(_.op_off + 4))
             + x86_align_enter()
             + x86("call", "rt_dcap_restore_to", (uint64_t)(uintptr_t)(void *)(void (*)(int))rt_dcap_restore_to)
             + x86_align_leave()
             + x86_gamma();
}
