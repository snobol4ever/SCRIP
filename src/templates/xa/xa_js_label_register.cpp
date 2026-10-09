#include <string>
#include "emit.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_js_label_register_str(void) {
    return std::string("rt._register_label_pcs({")
         + emit_for(0, g_emit.xa_label_count, [](int i) { return std::string(i > 0 ? "," : "") + js_escape_string_str(g_emit.xa_label_names[i]) + ":" + std::to_string(g_emit.xa_label_pcs[i]); })
         + "});\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_js_label_register(void) { emit_text_s(xa_js_label_register_str()); }
