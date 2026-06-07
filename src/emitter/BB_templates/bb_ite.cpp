#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_ite_str() {
    if (PLATFORM_X86) { return x86("comment", "END RESOLVE_ITE (β-tombstone via pair table)") + x86_pair_loop(); }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_ite(void) { bb_emit_x86(bb_ite_str()); }
