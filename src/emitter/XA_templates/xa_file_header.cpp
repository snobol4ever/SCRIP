/* xa_file_header.cpp — XA template for file header/footer. */
#include <string>
#include <cstdio>
#include "emit_str.h"
extern "C" {
#include "xa_template_common.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_file_header_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — XA_FILE_HEADER");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            /* V-4 (2026-05-27): rt_register_predicates_pl call RETIRED. Predicate BB graphs */
            /* are now inlined as flat x86 by SM_BB_SWITCH RESOLVE_ENTRY; standalone-binary rebuild */
            /* of the graph at startup (the path that called this helper) is dead code. */
            /* RK-BB-3.0a (2026-05-27, Opus 4.7): emit `mov edi, <g_lang>; call rt_set_lang@PLT` */
            /* immediately after rt_gc_init so g_lang in the linked libscrip_rt.so is set to the */
            /* source language before any rt_call dispatch runs. Without this, g_lang stays 0 */
            /* (LANG_SNO) and the LANG_RAKU-gated raku-builtin chain in rt_call is skipped. */
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
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — XA_FILE_FOOTER");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            return std::string("call rt_finalize@PLT\npop rbp\nret\n.size main, .-main\n.section .note.GNU-stack\n");
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_file_header(void) {
    std::string s = xa_file_header_str();
    if (!s.empty()) emit_text_n(s.data(), s.size());
}
extern "C" void xa_file_footer(void) { auto s = xa_file_footer_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
