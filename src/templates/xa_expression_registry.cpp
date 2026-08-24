#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_expression_registry_text(void) {
    return IF(g_emit.xa_expr_count > 0,
              std::string(".section .data\n.align 8\n.Lexpression_registry:\n")
            + FOR(0, g_emit.xa_expr_count, [](int i) {
                  return std::string(".quad .S") + std::to_string(g_emit.xa_expr_str_idxs[i]) + "\n"
                       + ".quad .L" + std::to_string(g_emit.xa_expr_pcs[i]) + "\n";
              })
            + ".quad 0\n.quad 0\n.text\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_expression_registry(void) {
    bb_emit_x86(xa_expression_registry_text());
}
