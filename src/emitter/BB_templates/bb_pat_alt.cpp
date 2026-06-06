#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_alt_str() {
    if (!PLATFORM_X86) return std::string();
    return x86_pair_loop();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_alt(void) { bb_emit_x86(bb_pat_alt_str()); }
