#include <string>
#include "emit.h"
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_wasm_main_close_str(void) {
    return std::string("    (call $core_finalize)\n")
         + "  )\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_wasm_main_open(void) { emit_text_s(xa_wasm_main_open_str()); }
extern "C" void xa_wasm_main_close(void) { emit_text_s(xa_wasm_main_close_str()); }
