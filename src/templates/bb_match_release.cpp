#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void rt_zls_release_to(void *mark);
extern "C" void rt_dcap_end_ok(void);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_release() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_RELEASE: head slot not resolved (operand[0] missing or unowned)")
         : x86("comment", "IR_MATCH_RELEASE")
         + x86_alpha()
         + (_.op_dval != 0.0 ? x86("mov", FRQ(_.op_off + 24), "r14") : std::string())
         + x86_align_enter()
         + x86("mov",  "rdi", FRQ(_.op_off + 8))
         + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)
         + x86_zls2_release_to_call(_.op_off + 16)
         + x86("call", "rt_dcap_end_ok", (uint64_t)(uintptr_t)(void *)rt_dcap_end_ok)
         + x86_align_leave()
         + x86_gamma();
}
