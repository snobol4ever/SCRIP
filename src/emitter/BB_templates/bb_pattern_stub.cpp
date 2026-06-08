#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pattern_stub_str(const char * which) {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT, x86("label", _.lbl_α))
             + x86_bomb(which)
             + x86("def", "β")
             + x86_bomb(which);
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pattern_stub(const char * which) {
    bb_emit_x86(bb_pattern_stub_str(which));
}
