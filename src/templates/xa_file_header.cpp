#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_file_header_text(void) {
    return std::string(".intel_syntax noprefix\n.globl main\n.type main, @function\nmain: sub rsp, 8\ncall rt_gc_init@PLT\n")
            + IF(x86_zeta_mode() != (int)ZC_ZETA, std::string("mov edi, ") + std::to_string(x86_zeta_mode()) + "\ncall rt_zeta_set_mode@PLT\n")
            + IF(g_emit.hdr_has_expr_reg, std::string("lea rdi, [rip + .Lexpression_registry]\ncall rt_register_expressions@PLT\n"))
            + IF(!g_emit.hdr_has_expr_reg, std::string("xor edi, edi\ncall rt_register_expressions@PLT\n"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_file_footer_text(void) {
    return std::string("call rt_finalize@PLT\nadd rsp, 8\nret\n.size main, .-main\n.section .note.GNU-stack\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_file_header(void) {
    bb_emit_x86(xa_file_header_text());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_file_footer(void) {
    bb_emit_x86(xa_file_footer_text());
}
