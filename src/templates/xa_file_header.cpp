#include <string>
#include <cstdio>
#include "emit_str.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string xa_file_header_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FILE_HEADER");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            extern int g_lang;
            char buf[64]; snprintf(buf, sizeof(buf), "mov edi, %d\ncall rt_set_lang@PLT\n", g_lang);
            return std::string(".intel_syntax noprefix\n.globl main\n.type main, @function\nmain: push rbp\nmov rbp, rsp\ncall rt_gc_init@PLT\n")
                + std::string(buf)
                + (g_emit.hdr_has_expr_reg
                    ? std::string("lea rdi, [rip + .Lexpression_registry]\ncall rt_register_expressions@PLT\n")
                    : std::string("xor edi, edi\ncall rt_register_expressions@PLT\n"));
        }
    }
    return std::string();
}
static std::string xa_file_footer_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FILE_FOOTER");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            return std::string("call rt_finalize@PLT\npop rbp\nret\n.size main, .-main\n.section .note.GNU-stack\n");
        }
    }
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_file_header(void) {
    std::string s = xa_file_header_str();
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
extern "C" void xa_file_footer(void) { auto s = xa_file_footer_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
