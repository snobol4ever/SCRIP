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
    }
    return std::string();
}
