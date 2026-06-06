#include "bb_builtin_common.h"
extern "C" {
#include "IR_interp_state.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bff_goal(const IR_t *gn) {
    if (gn->t != IR_FAIL && gn->t != IR_SUCCEED)
        return emit_build_compound_term(gn);
    char slbl[64]; strtab_label(slbl, sizeof slbl, (gn->t == IR_FAIL) ? "fail" : "true");
    return x86("ins2", "mov",  std::string("edi, ") + std::to_string((int)IR_ATOM))
         + x86("ins2", "xor",  "rsi, rsi")
         + x86("ins2", "lea",  std::string("rdx, [rip + ") + slbl + "]")
         + x86("ins2", "xorps","xmm0, xmm0")
         + x86("ins2", "call", "rt_node_to_term@PLT");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_findall_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if (strcmp(fn, "findall") == 0)
            return x86_lit_bytes(
                bytes(4, "\x48\x83\xEC\x10")
                + bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)(void *)(intptr_t)_.op_ival)
                + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_findall) + bytes(2, "\xFF\xD0")
                + bytes(4, "\x48\x83\xC4\x10")
                + bytes(2, "\x85\xC0")
            ) + x86("je", _.lbl_ω) + x86("jmp", _.lbl_γ) + x86("jmp", _.lbl_ω);
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "findall") == 0) {
            bb_findall_state_t *fs = (bb_findall_state_t *)(intptr_t)_.op_ival;
            if (!fs || !fs->goal_node || !fs->tmpl || !fs->result) {
                extern int g_sm_native_unsupported;
                g_sm_native_unsupported = 1;
                return hdr + x86("ins2", "jmp", _.lbl_ω) + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
            }
            const IR_t *gn = fs->goal_node;
            if (fs->gcfg && fs->gcfg->all) {
                for (int gi = 0; gi < fs->gcfg->n; gi++) {
                    IR_t *cand = fs->gcfg->all[gi];
                    if (cand && cand->t == IR_GCONJ) { gn = cand; break; }
                }
            }
            return hdr
                 + bff_goal(gn)                           + x86("ins2", "push", "rax")
                 + emit_build_compound_term(fs->tmpl)      + x86("ins2", "push", "rax")
                 + emit_build_compound_term(fs->result)    + x86("ins2", "push", "rax")
                 + x86("ins2", "sub", "rsp, 8")
                 + x86("ins2", "mov", "rdx, [rsp + 8]")
                 + x86("ins2", "mov", "rsi, [rsp + 16]")
                 + x86("ins2", "mov", "rdi, [rsp + 24]")
                 + x86("ins2", "call", "rt_findall_term@PLT")
                 + x86("ins2", "add", "rsp, 32")
                 + x86("ins2", "test", "eax, eax")
                 + x86("ins2", "je",   _.lbl_ω)
                 + x86("ins2", "jmp",  _.lbl_γ)
                 + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
