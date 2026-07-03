#include <string>
#include "emit.h"
#include "x86_asm.h"
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
        return x86("comment", "call void SnoRt::_finalize()")
             + x86("ret")
             + x86("}")
             + x86("}");
    }
    if (PLATFORM_WASM) {
        return wasm_emit_data_segments_str() + ")\n";
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_epilogue(void) { auto s = xa_epilogue_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
