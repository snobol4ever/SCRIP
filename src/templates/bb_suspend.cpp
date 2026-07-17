#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
}
#include "x86_asm.h"
extern "C" void rt_genp_yield(uint64_t, uint64_t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline bool genp_regime() { return _.flat_gen != 0; }
static inline uint64_t genp_yield_fp() { void (*fp)(uint64_t, uint64_t) = rt_genp_yield; return (uint64_t)(uintptr_t)(void *)fp; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_suspend() {
    x86_begin();
    if (PLATFORM_X86)
        return (_.op_sa < 0) ? x86_alpha() + x86_bomb("bb_suspend: no expr-value slot (needs descr flat-chain producer)") :
               x86("comment", "IR_SUSPEND yield+resume")
             + x86_alpha()
             + (_.op_sb >= 0 && _.lbl_t1_p ? x86_lea_tgt("rax", X86T_TGT1) + x86("mov", FRQ(_.op_sb), "rax") : std::string())
             + x86("mov", "rax", FRQ(_.op_sa))
             + x86("mov", FRQ(0), "rax")
             + x86("mov", "rax", FRQ(_.op_sa + 8))
             + x86("mov", FRQ(8), "rax")
             + (genp_regime()
                    ? x86("mov", "rdi", FRQ(0)) + x86("mov", "rsi", FRQ(8)) + x86("call", "rt_genp_yield", genp_yield_fp())
                    : x86_gamma())
             + x86_beta()
             + (_.lbl_t0 ? x86_jmp_tgt(X86T_TGT0) : x86_omega());
    return std::string();
}
