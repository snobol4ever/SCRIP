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
    if (PLATFORM_X86) {
        x86_begin();
        if (_.op_sb) {
            return IF(MEDIUM_TEXT,
                       s_1asm(std::string(_.lbl_α) + ":")
                     + s_comment("# BOX DET_WRITE(cell)  [PL-GZ-3: Term* from [ζ+off], deref+print via ONE rt VALUE call]"))
                 + x86("mov", "rdi", FRQ(GZ_CELL_OFF(_.op_off)))
                 + x86("call", "rt_pl_write_cell", (uint64_t)(uintptr_t)(void *)rt_pl_write_cell)
                 + x86("jmp", PORT_GAMMA)
                 + x86("def", PORT_BETA)
                 + x86("jmp", PORT_OMEGA);
        }
        if (_.op_sval) {
            return IF(MEDIUM_TEXT,
                       s_1asm(std::string(_.lbl_α) + ":")
                     + s_comment("# BOX DET_WRITE(atom)  [PL-GZ det VALUE call; operand sealed RO in-box, [rip+disp]]"))
                 + x86_ro_load_q("rdi", 0)
                 + x86("call", "rt_write_atom", (uint64_t)(uintptr_t)(void *)rt_write_atom)
                 + x86("jmp", PORT_GAMMA)
                 + x86("def", PORT_BETA)
                 + x86("jmp", PORT_OMEGA)
                 + x86_ro_seal_str(0, _.op_sval);
        }
        return IF(MEDIUM_TEXT,
                   s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment("# BOX DET_WRITE(int)  [PL-GZ det VALUE call]"))
             + x86("mov", "rdi", (long)_.op_ival)
             + x86("call", "rt_write_int", (uint64_t)(uintptr_t)(void *)rt_write_int)
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("jmp", PORT_OMEGA);
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_det_write(void) { bb_emit_x86(bb_det_write_str()); }
