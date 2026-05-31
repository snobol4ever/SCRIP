/* xa_epilogue.cpp — XA template for program epilogue. Converted to .cpp returning std::string (ER-6). */
#include <string>
#include "emit_str.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_epilogue_str(void) {
    if (PLATFORM_JVM) return std::string();
    if (PLATFORM_JS) {
        return std::string("default: break loop;\n")
             + "} rt._finalize();\n";
    }
    if (PLATFORM_NET) {
        return s_2asm("call", "void SnoRt::_finalize()")
             + s_1asm("ret")
             + s_1asm("}")
             + s_1asm("}");
    }
    if (PLATFORM_WASM) {
        return wasm_emit_data_segments_str() + ")\n";
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_epilogue(void) { auto s = xa_epilogue_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
