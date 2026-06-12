#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string rtt_lbl(IR_t *a) {
    if (!(IR_LIT(a).sval && *IR_LIT(a).sval)) return std::string();
    char l[64]; l[0] = 0; strtab_label(l, sizeof l, IR_LIT(a).sval);
    return std::string(l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string rtt_ball_scalar(IR_t *a) { std::string sl = rtt_lbl(a);
    return x86("mov", "edi", std::to_string((int)a->op))
         + x86("mov", "rsi", std::to_string((long)IR_LIT(a).ival))
         + (sl.size() ? x86("lea", "rdx", std::string("[rip + ") + sl + "]") : x86("xor", "edx", "edx"))
         + x86("xorps", "xmm0", "xmm0")
         + x86("call", "rt_node_to_term@PLT");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_retract_throw_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if ((strcmp(fn, "retract") == 0 || strcmp(fn, "retractall") == 0) && ir_call_arg(pBB,0)) {
            extern int g_sm_native_unsupported;
            g_sm_native_unsupported = 1;
            return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
        }
        if (strcmp(fn, "throw") == 0) {
            return x86_lit_bytes(bytes(4, "\x48\x83\xEC\x10")
                               + bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)(void *)ir_call_arg(pBB,0))
                               + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_throw)
                               + bytes(2, "\xFF\xD0")
                               + bytes(4, "\x48\x83\xC4\x10"))
                 + x86("jmp", "ω") + x86("def", "β") + x86("jmp", "ω");
        }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "throw") == 0 && ir_call_arg(pBB,0)) {
            IR_t *a = ir_call_arg(pBB,0);
            return hdr
                 + IF(a->op == IR_STRUCT, emit_build_compound_term(a))
                 + IF(a->op != IR_STRUCT, rtt_ball_scalar(a))
                 + x86("mov", "rdi", "rax")
                 + x86("call", "rt_throw_term@PLT")
                 + x86("jmp",  _.lbl_ω)
                 + x86("def", "β") + x86("jmp", "ω");
        }
    }
    return std::string();
}
