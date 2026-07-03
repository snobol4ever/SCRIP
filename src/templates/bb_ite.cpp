#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_ite() {
    if (PLATFORM_X86) { return x86("comment", "END RESOLVE_ITE (β-tombstone via pair table)") + x86_pair_loop(); }
    return std::string();
}
