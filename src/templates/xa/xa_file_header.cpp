#include <string>
#include "emitter/emit_state_em_03_sh_api.h"
#include "emitter/emit_str.h"
#include "templates/x86/x86_ops.h"
extern "C" {
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_file_header_text(void) {
    return std::string(".intel_syntax noprefix\n.globl main\n.type main, @function\nmain: sub rsp, 8\ncall rt_gc_init@PLT\n")
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
