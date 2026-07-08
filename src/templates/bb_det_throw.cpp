#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "IR.h"
}
#include "x86_asm.h"
extern std::string gzu_build(const IR_t *nd);
extern "C" void rt_pl_throw_set(void *ball_cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_throw() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    const IR_t *ball = (const IR_t *)(intptr_t)_.op_parts_ival[0];
    if (!ball) return x86_bomb("bb_det_throw: missing ball node");
    return x86_alpha()
         + x86("comment", "IR_DET_THROW")
         + gzu_build(ball)
         + x86("mov", "rdi", "rax")
         + x86("call", "rt_pl_throw_set", (uint64_t)(uintptr_t)(void *)rt_pl_throw_set)
         + x86_omega()
         + IF(!_.op_bounded, x86_beta() + x86_omega());
}
