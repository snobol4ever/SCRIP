#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pattern_stub(const char * which) {
    bb_emit_x86(PLATFORM_X86
        ? x86("label", _.lbl_α)
        + x86_bomb(which)
        + x86("def", "β")
        + x86_bomb(which)
        : std::string());
}
