/* xa_pattern_blobs.cpp — XA_PATTERN_BLOBS: emit the invariant pattern-blob section prelude.
   Driver (walk_bb_pattern_blobs) counts invariant windows into g_emit.xa_pat_blob_invariant_n,
   then calls xa_dispatch(XA_PATTERN_BLOBS). The per-window BB bodies are built by the driver via
   codegen_flat_build (which routes through BB templates); this template owns only the fixed
   .intel_syntax/.text section directives that precede them. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_pattern_blobs_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — XA_PATTERN_BLOBS");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            if (g_emit.xa_pat_blob_invariant_n <= 0) return std::string();
            return std::string(".intel_syntax noprefix\n") + ".text\n";
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_pattern_blobs(void) {
    std::string s = xa_pattern_blobs_str();
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
