#include <string>
#include "emit_str.h"
extern "C" {
#include "xa_template_common.h"
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string xa_macro_library_open_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — XA_MACRO_LIBRARY_OPEN (it IS the macro-def pass)");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            return std::string("# === BEGIN sm macro library (via MEDIUM_MACRO_DEF template dispatch) ===\n")
                 + ".intel_syntax noprefix\n";
        }
    }
    return std::string();
}
static std::string xa_macro_library_close_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — XA_MACRO_LIBRARY_CLOSE");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT)      return std::string("# === END sm macro library ===\n");
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_macro_library_open(void)  { auto s = xa_macro_library_open_str();  if (!s.empty()) emit_text_n(s.data(), s.size()); }
extern "C" void xa_macro_library_close(void) { auto s = xa_macro_library_close_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
