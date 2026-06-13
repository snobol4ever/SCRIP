#include "bb_common.h"
extern "C" {
#include "IR_interp_state.h"
}
extern "C" int rt_aggregate(void *agg_ptr);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bff_goal(const IR_t *gn) {
    if (gn->op != IR_FAIL && gn->op != IR_SUCCEED)
        return emit_build_compound_term(gn);
    char slbl[64]; strtab_label(slbl, sizeof slbl, (gn->op == IR_FAIL) ? "fail" : "true");
    return x86("mov32", "edi", (long)(int)IR_ATOM)
         + x86("xor", "rsi", "rsi")
         + x86("lea", "rdx", std::string("[rip + ") + slbl + "]")
         + x86("xorps", "xmm0", "xmm0")
         + x86("call", "rt_node_to_term@PLT");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_findall_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if (strcmp(fn, "findall") == 0)
            return hdr
                 + x86("sub", "rsp", 16L)
                 + x86("movabs", "rdi", (uint64_t)(uintptr_t)(void *)(intptr_t)_.op_ival)
                 + x86("call", "rt_findall", (uint64_t)(uintptr_t)(void*)rt_findall)
                 + x86("add", "rsp", 16L)
                 + x86("test", "eax", "eax")
                 + x86("je", "ω") + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω");
        if (strcmp(fn, "aggregate_all") == 0)
            return hdr
                 + x86("sub", "rsp", 16L)
                 + x86("movabs", "rdi", (uint64_t)(uintptr_t)(void *)(intptr_t)_.op_ival)
                 + x86("call", "rt_aggregate", (uint64_t)(uintptr_t)(void*)rt_aggregate)
                 + x86("add", "rsp", 16L)
                 + x86("test", "eax", "eax")
                 + x86("je", "ω") + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω");
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "findall") == 0) {
            bb_findall_state_t *fs = (bb_findall_state_t *)(intptr_t)_.op_ival;
            if (!fs || !fs->goal_node || !fs->tmpl || !fs->result) {
                extern int g_sm_native_unsupported;
                g_sm_native_unsupported = 1;
                return hdr + x86("jmp", "ω") + x86("def", "β") + x86("jmp", "ω");
            }
            const IR_t *gn = fs->goal_node;
            if (fs->gcfg && fs->gcfg->all) {
                for (int gi = 0; gi < fs->gcfg->n; gi++) {
                    IR_t *cand = fs->gcfg->all[gi];
                    if (cand && cand->op == IR_GCONJ) { gn = cand; break; }
                }
            }
            return hdr
                 + bff_goal(gn)                           + x86("push", "rax")
                 + emit_build_compound_term(fs->tmpl)      + x86("push", "rax")
                 + emit_build_compound_term(fs->result)    + x86("push", "rax")
                 + x86("sub", "rsp", "8")
                 + x86("mov", "rdx", "[rsp + 8]")
                 + x86("mov", "rsi", "[rsp + 16]")
                 + x86("mov", "rdi", "[rsp + 24]")
                 + x86("call", "rt_findall_term@PLT")
                 + x86("add", "rsp", "32")
                 + x86("test", "eax", "eax")
                 + x86("je",   _.lbl_ω)
                 + x86("jmp",  _.lbl_γ)
                 + x86("def", "β") + x86("jmp", "ω");
        }
        if (strcmp(fn, "aggregate_all") == 0) {
            extern int g_sm_native_unsupported;
            g_sm_native_unsupported = 1;
            return hdr + x86("jmp", "ω") + x86("def", "β") + x86("jmp", "ω");
        }
    }
    return std::string();
}
