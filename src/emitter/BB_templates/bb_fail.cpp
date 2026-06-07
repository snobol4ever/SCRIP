#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_fail_str() {
    if (PLATFORM_X86) {
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α)
                             + x86("comment", "BOX FAIL()  [x86() self-encoding]"))
             + x86("jmp", "ω")
             + x86("def", "β")
             + x86("jmp", "ω");
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_fail(IR_t * pBB) { (void)pBB; bb_emit_x86(bb_fail_str()); }
