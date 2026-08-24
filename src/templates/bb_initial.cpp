#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
int64_t rt_initial_fire(int64_t site);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_initial() {
    x86_begin();
    return x86("comment", "IR_INITIAL run-once guard")
         + x86_alpha()
         + x86("mov",   "rdi", (long)_.op_sc)
         + x86("sub",   "rsp", (long)16)
         + x86("call",  "rt_initial_fire", (uint64_t)(uintptr_t)(void *)rt_initial_fire)
         + x86("add",   "rsp", (long)16)
         + x86("test",  "rax", "rax")
         + x86_gamma("je")
         + x86_pair_jmp(0)
         + x86_beta_trampoline();
}
