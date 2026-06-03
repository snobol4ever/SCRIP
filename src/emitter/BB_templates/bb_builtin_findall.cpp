#include "bb_builtin_common.h"
extern "C" {
#include "IR_interp_state.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_findall_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
            if (strcmp(fn, "findall") == 0) {
                void *fs_ptr = (void *)(intptr_t)pBB->ival;
                std::string b;
                b += bytes(4, "\x48\x83\xEC\x10");                /* sub rsp, 16 */
                b += bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)fs_ptr);
                b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_findall) + bytes(2, "\xFF\xD0");
                b += bytes(4, "\x48\x83\xC4\x10");                /* add rsp, 16 */
                b += bytes(2, "\x85\xC0");
                return x86_lit_bytes(b) + x86("je", PORT_OMEGA) + x86("jmp", PORT_GAMMA) + x86("jmp", PORT_OMEGA);
            }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "findall") == 0) {
            bb_findall_state_t *fs = (bb_findall_state_t *)(intptr_t)pBB->ival;
            if (!fs || !fs->goal_node || !fs->tmpl || !fs->result) {
                extern int g_sm_native_unsupported;
                g_sm_native_unsupported = 1;
                return hdr + s_2asm("jmp", _.lbl_ω) + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
            }
            std::string goal_build;
            if (fs->goal_node->t == IR_FAIL || fs->goal_node->t == IR_SUCCEED) {
                const char *gs = (fs->goal_node->t == IR_FAIL) ? "fail" : "true";
                char slbl[64]; strtab_label(slbl, sizeof slbl, gs);
                goal_build = s_2asm("mov",  emit_fmt("edi, %d", (int)IR_ATOM))
                           + s_2asm("xor",  "rsi, rsi")
                           + s_2asm("lea",  emit_fmt("rdx, [rip + %s]", slbl))
                           + s_2asm("xorps","xmm0, xmm0")
                           + s_2asm("call", "rt_node_to_term@PLT");
            } else {
                goal_build = emit_build_compound_term(fs->goal_node);
            }
            return hdr
                 + goal_build                               + s_2asm("push", "rax")
                 + emit_build_compound_term(fs->tmpl)      + s_2asm("push", "rax")
                 + emit_build_compound_term(fs->result)    + s_2asm("push", "rax")
                 + s_2asm("sub", "rsp, 8")
                 + s_2asm("mov", "rdx, [rsp + 8]")
                 + s_2asm("mov", "rsi, [rsp + 16]")
                 + s_2asm("mov", "rdi, [rsp + 24]")
                 + s_2asm("call", "rt_findall_term@PLT")
                 + s_2asm("add", "rsp, 32")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("je",   _.lbl_ω)
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
