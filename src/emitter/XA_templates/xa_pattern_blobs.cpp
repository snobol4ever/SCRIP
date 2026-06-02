#include <string>
#include "emit_str.h"
extern "C" {
#include "xa_template_common.h"
}
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_pattern_blobs(void) {
    std::string s = xa_pattern_blobs_str();
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
