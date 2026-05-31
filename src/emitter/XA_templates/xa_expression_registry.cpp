/* xa_expression_registry.cpp — XA_EXPRESSION_REGISTRY: emit .data expression fn-ptr table.
   Driver fills g_emit.xa_expr_pcs, xa_expr_str_idxs, xa_expr_count, then calls xa_dispatch. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_expression_registry_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — XA_EXPRESSION_REGISTRY");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            if (g_emit.xa_expr_count <= 0) return std::string();
            return std::string(".section .data\n.align 8\n.Lexpression_registry:\n")
                + emit_for(0, g_emit.xa_expr_count, [](int i) {
                    return emit_fmt(".quad .S%d\n", g_emit.xa_expr_str_idxs[i])
                         + emit_fmt(".quad .L%d\n", g_emit.xa_expr_pcs[i]);
                  })
                + ".quad 0\n.quad 0\n.text\n";
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_expression_registry(void) {
    std::string s = xa_expression_registry_str();
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
