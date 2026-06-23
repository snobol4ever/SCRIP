#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
/* TR-CAT: a stored-pattern LIT element is only ever reached as a gvar-chain ENTRY (it is an
   operand of an IR_PATTERN_CAT, never a bare IR_DTP_ASSIGN operand — a bare `PAT = 'x'` lowers
   to IR_ASSIGN_LIT_S, a string, not a pattern builder). So this box is always a passthrough:
   label + jmp γ. The actual LIT matcher is built inside the CAT blob by the runtime builder.   */
std::string bb_pattern_lit() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "IR_PATTERN_LIT passthrough (chain entry; matcher built inside CAT blob)")
         + x86("label",   _.lbl_α)
         + x86("jmp",  "γ")
         + x86("def",  "β")
         + x86("jmp",  "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_emit_pattern_lit_call(void) { bb_emit_x86(bb_pattern_lit()); }
