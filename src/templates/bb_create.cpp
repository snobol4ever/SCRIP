#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
#include "../runtime/rt/rt_coexpr.h"
}
#include "x86_asm.h"
std::string xa_coexpr_body_lea(const char * dst);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_create() {
    x86_begin();
    if (!PLATFORM_X86) return x86_alpha() + x86_bomb("bb_create: no x86 platform");
    if (_.op_off < 0) return x86_alpha() + x86_bomb("bb_create: op_off < 0 (no slot assigned -- IR_CREATE missing from ir_node_produces_value?)");
    if (!_.lbl_t0)
        return x86_alpha() + x86_bomb("bb_create: body-entry target (t0 port) is NULL -- codegen_flat_chain_body's IR_CREATE resolution did not thread g_create_body_entry "
                         "(operand[0] not found in this chain's nodes[]? the BFS operand[0] enqueue may be missing)");
    int op_off2 = _.op_off + 16;
    std::string s = x86("comment", "IR_CREATE")
                   + x86_alpha();
    static const char *contract_regs[6] = {"r12", "r13", "r14", "r15", "rbx", "rsp"};
    for (int k = 0; k < 6; k++) {
        s += x86("mov", "qword ptr [" + std::string(x86_fb()) + " + " + std::to_string(op_off2 + k * 8) + "]", contract_regs[k]);
    }
    s += xa_coexpr_body_lea("rdi");
    s += x86_frame_lea("rsi", op_off2)
       + x86("mov", "edx", std::to_string(_.frame_region > 0 ? _.frame_region : 0))
       + x86("call", "scrip_coexpr_create", (uint64_t)(uintptr_t)(void *)scrip_coexpr_create)
       + x86("mov",  "qword ptr [" + std::string(x86_fb()) + " + " + std::to_string(_.op_off) + "]", "rax")
       + x86_gamma()
       + x86_beta_trampoline();
    return s;
}
