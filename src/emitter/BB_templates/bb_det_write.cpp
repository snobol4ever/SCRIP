#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
extern "C" void rt_write_atom(const char * s);
extern "C" void rt_write_int(long v);
extern "C" void rt_pl_write_cell(void * cell_term);
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_det_write_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    return x86("label", _.lbl_α)
         + x86("comment", "IR_DET_WRITE")
         + IF(_.op_sb,
               x86("mov", "rdi", FRQ(GZ_CELL_OFF(_.op_off)))
             + x86("call", "rt_pl_write_cell", (uint64_t)(uintptr_t)(void *)rt_pl_write_cell))
         + IF(!_.op_sb && _.op_sval,
               x86("ro_load_q", "rdi", 0)
             + x86("call", "rt_write_atom", (uint64_t)(uintptr_t)(void *)rt_write_atom))
         + IF(!_.op_sb && !_.op_sval,
               x86("mov", "rdi", (long)_.op_ival)
             + x86("call", "rt_write_int", (uint64_t)(uintptr_t)(void *)rt_write_int))
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω")
         + IF(!_.op_sb && _.op_sval, x86("ro_seal_str", 0, _.op_sval));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_write(void) { bb_emit_x86(bb_det_write_str()); }
