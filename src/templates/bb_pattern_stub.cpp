#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pattern_stub(const char * which) {
    bb_emit_x86(PLATFORM_X86
        ? x86_alpha()
        + x86_bomb(which)
        + x86_beta()
        + x86_bomb(which)
        : std::string());
}
