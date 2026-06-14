#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
#include "IR.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_CELL_CATCH — placeholder. Real body lands when catch admission is wired; until then IR_CATCH is
 * not admitted by pl_gz_admit, so this is unreachable and the bomb only fires if that invariant breaks. */
std::string bb_cell_catch() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return x86_bomb("bb_cell_catch: catch box reached the emitter but is not yet implemented");
}
