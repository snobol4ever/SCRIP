#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_ite_str() {
    if (PLATFORM_X86) return x86_bomb("bb_ite: not yet implemented as self-contained x86() template");
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ite(void) { bb_emit_x86(bb_ite_str()); }
