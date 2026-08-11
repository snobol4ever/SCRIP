#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" int  rt_trail_mark(void);
extern "C" void rt_trail_unwind(int mark);
extern "C" void rt_pl_cells_init(void ** cells, int n);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *bcf_areg(int i) { static const char * t[4] = { "rsi", "rdx", "rcx", "r8" }; return t[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_callee_frame() {
    if (PLATFORM_X86)
        return (_.op_parts_n == 0 || _.op_parts_ival[0] < 0 || _.op_parts_ival[1] < 0) ? x86_bomb("bb_callee_frame: unadmitted callee shape reached the emitter")
             : IF(_.op_sa == 0,
                   x86("comment", "IR_CALLEE_FRAME")
                 + x86("push", x86_zr())
                 + x86("mov", x86_zr(), "rdi")
                 + FOR(0, ((int)_.op_parts_ival[0] < 4 ? (int)_.op_parts_ival[0] : 4), [&](int i) {
                     return x86("mov", "rax", RDQ(bcf_areg(i), 0)) + x86("mov", FRQ(GZ_CELL_OFF(i)), "rax")
                          + x86("mov", "rax", RDQ(bcf_areg(i), 8)) + x86("mov", FRQ(GZ_CELL_OFF(i) + 8), "rax");
                 })
                 + x86("call", "rt_trail_mark", (uint64_t)(uintptr_t)(void *)rt_trail_mark)
                 + x86("mov", FR(0), "eax")
                 + IF(_.op_parts_ival[2] > 1, x86("mov", FR(4), (long)1))
                 + IF(_.op_parts_ival[1] > 0,
                       x86("lea", "rdi", FR(GZ_CELL_OFF((int)_.op_parts_ival[0])))
                     + x86("mov32", "esi", (long)_.op_parts_ival[1])
                     + x86("call", "rt_pl_cells_init", (uint64_t)(uintptr_t)(void *)rt_pl_cells_init))
                 + x86("jmp", "γ"))
             + IF(_.op_sa == 1,
                   x86("comment", "IR_CALLEE_FRAME")
                 + x86("def", "γ")
                 + x86("mov32", "eax", 1L)
                 + x86("pop", x86_zr())
                 + x86("ret")
                 + x86("def", "ω")
                 + x86("mov", "edi", FR(0))
                 + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
                 + x86("mov32", "eax", 0L)
                 + x86("pop", x86_zr())
                 + x86("ret")
                 + x86("def", "β")
                 + x86("push", x86_zr())
                 + x86("mov", x86_zr(), "rdi")
                 + IF(_.op_parts_ival[2] <= 1, x86_jmp_tgt(X86T_TGT0)))
             + IF(_.op_sa == 2,
                   x86("mov", "eax", FR(4))
                 + x86("cmp", "eax", (long)_.op_off)
                 + x86_jcc_tgt("je", X86T_TGT0))
             + IF(_.op_sa == 3, x86_jmp_tgt(X86T_TGT0))
             + IF(_.op_sa == 4,
                   x86("comment", "IR_CALLEE_FRAME")
                 + x86("mov", FR(4), (long)_.op_off)
                 + x86("mov", "edi", FR(0))
                 + x86("call", "rt_trail_unwind", (uint64_t)(uintptr_t)(void *)rt_trail_unwind)
                 + x86_jmp_tgt(X86T_TGT0))
             + IF(_.op_sa < 0 || _.op_sa > 4, x86_bomb("bb_callee_frame: unknown aspect"));
    return std::string();
}
