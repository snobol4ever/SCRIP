#include "bb_builtin_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_retract_throw_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if ((strcmp(fn, "retract") == 0 || strcmp(fn, "retractall") == 0) && pBB->α) {
                extern int g_sm_native_unsupported;
                g_sm_native_unsupported = 1;
                return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
            }
            if (strcmp(fn, "throw") == 0) {
                void *alpha_ptr = (void *)pBB->α;
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x10");                                            /* sub rsp, 16 */
                b += bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)alpha_ptr);            /* movabs rdi, &α */
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_throw);  /* movabs rax, &rt_throw */
                b += bytes(2, "\xFF\xD0");                                                     /* call rax */
                b += bytes(4, "\x48\x83\xC4\x10");                                            /* add rsp, 16 */
                return x86_lit_bytes(b) + x86("jmp", PORT_OMEGA) + x86("def", PORT_BETA) + x86("jmp", PORT_OMEGA);
            }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "throw") == 0 && pBB->α) {
            IR_t *a = pBB->α;
            std::string ball_build;
            if (a->t == IR_STRUCT) {
                ball_build = emit_build_compound_term(a);
            } else {
                char slbl[64]; slbl[0] = 0;
                if (a->sval && *a->sval) strtab_label(slbl, sizeof slbl, a->sval);
                ball_build = s_2asm("mov", emit_fmt("edi, %d", (int)a->t))
                           + s_2asm("mov", emit_fmt("rsi, %ld", (long)a->ival))
                           + (slbl[0] ? s_2asm("lea", emit_fmt("rdx, [rip + %s]", slbl))
                                      : s_2asm("xor", "edx, edx"))
                           + s_2asm("xorps", "xmm0, xmm0")
                           + s_2asm("call", "rt_node_to_term@PLT");
            }
            return hdr
                 + ball_build
                 + s_2asm("mov",  "rdi, rax")
                 + s_2asm("call", "rt_throw_term@PLT")
                 + s_2asm("jmp",  _.lbl_ω)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
