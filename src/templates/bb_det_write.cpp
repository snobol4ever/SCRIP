#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" void rt_write_atom(const char * s);
extern "C" void rt_write_int(long v);
extern "C" void rt_pl_write_cell(void * cell_term);
extern "C" void rt_pl_writeq_cell(void * cell_term);
extern "C" void rt_pl_write_canonical_cell(void * cell_term);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_det_write() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    return x86_alpha()
         + x86("comment", "IR_DET_WRITE")
         + IF(_.op_sb && _.op_ival == 1,
               x86("lea", "rdi", FR(GZ_CELL_OFF(_.op_off)))
             + x86("call", "rt_pl_writeq_cell", (uint64_t)(uintptr_t)(void *)rt_pl_writeq_cell))
         + IF(_.op_sb && _.op_ival == 2,
               x86("lea", "rdi", FR(GZ_CELL_OFF(_.op_off)))
             + x86("call", "rt_pl_write_canonical_cell", (uint64_t)(uintptr_t)(void *)rt_pl_write_canonical_cell))
         + IF(_.op_sb && _.op_ival == 0,
               x86("lea", "rdi", FR(GZ_CELL_OFF(_.op_off)))
             + x86("call", "rt_pl_write_cell", (uint64_t)(uintptr_t)(void *)rt_pl_write_cell))
         + IF(!_.op_sb && _.op_sval,
               x86("mov", "rdi", ROQ(0))
             + x86("call", "rt_write_atom", (uint64_t)(uintptr_t)(void *)rt_write_atom))
         + IF(!_.op_sb && !_.op_sval,
               x86("mov", "rdi", (long)_.op_ival)
             + x86("call", "rt_write_int", (uint64_t)(uintptr_t)(void *)rt_write_int))
         + x86_gamma()
         + IF(!_.op_bounded, x86_beta() + x86_omega())
         + IF(!_.op_sb && _.op_sval,
               x86("def", L(0))
             + x86(".quad", LS(0), _.op_sval)
             + x86("label", LS(0))
             + x86(".string", _.op_sval));
}
