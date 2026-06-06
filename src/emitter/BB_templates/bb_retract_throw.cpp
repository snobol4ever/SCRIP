#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string rtt_lbl(IR_t *a) {
    if (!(a->sval && *a->sval)) return std::string();
    char l[64]; l[0] = 0; strtab_label(l, sizeof l, a->sval);
    return std::string(l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string rtt_ball_scalar(IR_t *a) { std::string sl = rtt_lbl(a);
    return x86("ins2", "mov", std::string("edi, ") + std::to_string((int)a->t))
         + x86("ins2", "mov", std::string("rsi, ") + std::to_string((long)a->ival))
         + (sl.size() ? x86("ins2", "lea", std::string("rdx, [rip + ") + sl + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "xorps", "xmm0, xmm0")
         + x86("ins2", "call", "rt_node_to_term@PLT");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_retract_throw_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if ((strcmp(fn, "retract") == 0 || strcmp(fn, "retractall") == 0) && pBB->α) {
            extern int g_sm_native_unsupported;
            g_sm_native_unsupported = 1;
            return bytes(1, "\xE9") + u32le(0) + bytes(1, "\xE9") + u32le(0);
        }
        if (strcmp(fn, "throw") == 0) {
            return x86_lit_bytes(bytes(4, "\x48\x83\xEC\x10")
                               + bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)(void *)pBB->α)
                               + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_throw)
                               + bytes(2, "\xFF\xD0")
                               + bytes(4, "\x48\x83\xC4\x10"))
                 + x86("jmp", "ω") + x86("def", "β") + x86("jmp", "ω");
        }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "throw") == 0 && pBB->α) {
            IR_t *a = pBB->α;
            return hdr
                 + IF(a->t == IR_STRUCT, emit_build_compound_term(a))
                 + IF(a->t != IR_STRUCT, rtt_ball_scalar(a))
                 + x86("ins2", "mov",  "rdi, rax")
                 + x86("ins2", "call", "rt_throw_term@PLT")
                 + x86("ins2", "jmp",  _.lbl_ω)
                 + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
