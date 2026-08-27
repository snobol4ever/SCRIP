#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_pattern_blobs_text(void) {
    return IF(g_emit.xa_pat_blob_invariant_n > 0, std::string(".intel_syntax noprefix\n") + ".text\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_pattern_blobs(void) {
    bb_emit_x86(xa_pattern_blobs_text());
}
