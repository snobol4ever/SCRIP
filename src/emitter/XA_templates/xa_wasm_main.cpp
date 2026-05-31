/* xa_wasm_main.cpp — XA templates for WASM main wrapper open/close.
   XA_WASM_MAIN_OPEN: emits (func $main ...) preamble.
   XA_WASM_MAIN_CLOSE: emits core_finalize call + closing paren.
   Driver (emit_core.c) calls walk_sm_wasm between them. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_wasm_main_open_str(void) {
    return std::string("  (func $main (export \"main\")\n")
         + "    (local $pc i32)\n"
         + "    (local $tmp i32)\n"
         + "    (local $fr i32)\n"
         + "    (call $core_init)\n";
}
static std::string xa_wasm_main_close_str(void) {
    return std::string("    (call $core_finalize)\n")
         + "  )\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_wasm_main_open(void)  { auto s = xa_wasm_main_open_str();  if (!s.empty()) emit_text_n(s.data(), s.size()); }
extern "C" void xa_wasm_main_close(void) { auto s = xa_wasm_main_close_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
