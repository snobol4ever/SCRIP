#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_cell_cut_str() {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    return IF(MEDIUM_TEXT, x86("label", _.lbl_α) + x86("comment", "BOX CELL_CUT lexical cut = PURE WIRING  [PL-GZ-6 seed firstpath: α falls to γ; β jmp ω, and the driver wired ω = the callee fail landing — redo never resumes earlier goals nor later clauses; bindings made before the cut undo only at that landing's unwind]"))
         + x86("jmp", "γ")
         + x86("def", "β")
         + x86("jmp", "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_cell_cut(void) { bb_emit_x86(bb_cell_cut_str()); }
