#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_atom_str(IR_t * pBB) {
    return IF(PLATFORM_X86,
           IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                         + x86("comment", std::string("BOX RESOLVE_ATOM('") + (pBB && _.op_sval ? _.op_sval : "") + "') [stackless pass-through]"))
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω"));
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_atom(IR_t * pBB) { bb_emit_x86(bb_atom_str(pBB)); }
