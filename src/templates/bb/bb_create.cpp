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
int xa_icn_block_size(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CR_PINNED() ((g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit_cfg->zframe_pinned_base) ? 1 : 0)
#define CR_REG(k) ((k) == 0 ? "r12" : (k) == 1 ? "r13" : (k) == 2 ? "r14" : (k) == 3 ? "r15" : (k) == 4 ? "rbx" : (k) == 5 ? (CR_PINNED() ? "rbp" : "rsp") : "r9")
std::string bb_create() {
    x86_begin();
    return IF(_.op_off < 0, x86_alpha() + x86_bomb("bb_create: op_off < 0 (no slot assigned -- IR_CREATE missing from ir_node_produces_value?)"))
         + IF(_.op_off >= 0 && !_.lbl_t0,
               x86_alpha() + x86_bomb("bb_create: body-entry target (t0 port) is NULL -- codegen_flat_chain_body's IR_CREATE resolution did not thread g_create_body_entry "
                         "(operand[0] not found in this chain's nodes[]? the BFS operand[0] enqueue may be missing)"))
         + IF(_.op_off >= 0 && _.lbl_t0,
               x86("comment", "IR_CREATE")
         + x86_alpha()
         + FOR(0, 7, [&](int k) { return x86("mov", "qword ptr [" + std::string(x86_fb()) + " + " + std::to_string(_.op_off + 16 + k * 8) + "]", CR_REG(k)); })
         + xa_coexpr_body_lea("rdi")
         + x86("lea", "rsi", "qword ptr [" + std::string(x86_fb()) + " + " + std::to_string(_.op_off + 16) + "]")
         + x86("comment", "the snapshot reaches the creator's argument block (the block protocol puts a det procedure's params at [rbp+kt+16i], above the frame)")
         + x86("mov", "edx", std::to_string((_.flat_carve_total > _.frame_region ? _.flat_carve_total : (_.frame_region > 0 ? _.frame_region : 0))
                                          + (g_emit.flat_gen ? 0 : xa_icn_block_size())))
         + x86("mov", "ecx", std::to_string(CR_PINNED() ? _.flat_carve_total : 0))
         + x86_load_ro_str("r8", _.op_activate_proc ? _.op_activate_proc : "main")
         + x86("call", "scrip_coexpr_create", (uint64_t)(uintptr_t)(void *)scrip_coexpr_create)
         + x86("mov",  "qword ptr [" + std::string(x86_fb()) + " + " + std::to_string(_.op_off) + "]", (long)DT_CO)
         + x86("mov",  "qword ptr [" + std::string(x86_fb()) + " + " + std::to_string(_.op_off + 8) + "]", "rax")
         + x86_gamma()
         + x86_beta_trampoline());
}
