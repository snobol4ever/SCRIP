#include <string>
#include "emitter/emit_state_em_03_sh_api.h"
#include "emitter/emit_str.h"
#include "templates/x86/x86_ops.h"
extern "C" {
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_pattern_blobs_text(void) {
    return IF(g_emit.xa_pat_blob_invariant_n > 0, std::string(".intel_syntax noprefix\n") + ".text\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_pattern_blobs(void) {
    bb_emit_x86(xa_pattern_blobs_text());
}
