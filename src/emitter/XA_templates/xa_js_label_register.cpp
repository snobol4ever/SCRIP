/* xa_js_label_register.cpp — XA template for JS label PC registration.
   Driver (emit_core.c) scans SM sequence and fills g_emit.xa_label_names/pcs/count.
   Template iterates the collection (FOR over g_emit scalars) to emit the JS object literal. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_js_label_register_str(void) {
    std::string r = "rt._register_label_pcs({";
    for (int i = 0; i < g_emit.xa_label_count; i++) {
        if (i > 0) r += ",";
        r += js_escape_string_str(g_emit.xa_label_names[i]) + emit_fmt(":%d", g_emit.xa_label_pcs[i]);
    }
    return r + "});\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_js_label_register(void) { auto s = xa_js_label_register_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
