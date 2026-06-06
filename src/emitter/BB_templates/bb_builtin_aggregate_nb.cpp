#include "bb_builtin_common.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string anlbl(const char *s) { char b[64]; b[0] = 0; if (s && *s) strtab_label(b, sizeof b, s); return std::string(b); }
static std::string agg_build_term(IR_t *a) {
    if (!a) return x86("ins2", "xor", "eax, eax");
    if (a->t == IR_STRUCT) return emit_build_compound_term(a);
    return x86("ins2", "mov", std::string("edi, ") + std::to_string((int)a->t))
         + x86("ins2", "mov", std::string("rsi, ") + std::to_string((long)a->ival))
         + IF(a->sval && *a->sval, x86("ins2", "lea", std::string("rdx, [rip + ") + anlbl(a->sval) + "]"))
         + IF(!(a->sval && *a->sval), x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "xorps", "xmm0, xmm0")
         + x86("ins2", "call", "rt_node_to_term@PLT");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string agg_bin_all(IR_t *a, IR_t *b, IR_t *c) { int kres = (int)c->t; long ires = (long)c->ival; const char *sres = (kres == IR_ATOM) ? c->sval : NULL;
    return x86_lit_bytes(bytes(4, "\x48\x83\xEC\x10")
         + emit_term_from_node_bin(a)
         + bytes(4, "\x48\x89\x04\x24")
         + emit_term_from_node_bin(b)
         + bytes(3, "\x48\x89\xC6")
         + bytes(4, "\x48\x8B\x3C\x24")
         + bytes(1, "\xBA") + u32le((uint32_t)kres)
         + bytes(2, "\x48\xB9") + u64le((uint64_t)ires)
         + IF(sres != NULL, bytes(2, "\x49\xB8") + u64le((uint64_t)(uintptr_t)sres)) + IF(sres == NULL, bytes(3, "\x45\x31\xC0"))
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_aggregate_all_term) + bytes(2, "\xFF\xD0")
         + bytes(4, "\x48\x83\xC4\x10")
         + bytes(2, "\x85\xC0"))
         + x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω");
}
static std::string agg_bin_nbset(IR_t *a, IR_t *b) {
    return x86_lit_bytes(bytes(4, "\x48\x83\xEC\x10")
         + emit_term_from_node_bin(a)
         + bytes(4, "\x48\x89\x04\x24")
         + emit_term_from_node_bin(b)
         + bytes(3, "\x48\x89\xC6")
         + bytes(4, "\x48\x8B\x3C\x24")
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_nb_setval_term) + bytes(2, "\xFF\xD0")
         + bytes(4, "\x48\x83\xC4\x10")
         + bytes(2, "\x85\xC0"))
         + x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω");
}
static std::string agg_bin_nbget(IR_t *a, IR_t *b) { int kres = (int)b->t; long ires = (long)b->ival; const char *sres = (kres == IR_ATOM) ? b->sval : NULL;
    return x86_lit_bytes(bytes(4, "\x48\x83\xEC\x10")
         + emit_term_from_node_bin(a)
         + bytes(3, "\x48\x89\xC7")
         + bytes(1, "\xBE") + u32le((uint32_t)kres)
         + bytes(2, "\x48\xBA") + u64le((uint64_t)ires)
         + IF(sres != NULL, bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)sres)) + IF(sres == NULL, bytes(2, "\x31\xC9"))
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_nb_getval_term) + bytes(2, "\xFF\xD0")
         + bytes(4, "\x48\x83\xC4\x10")
         + bytes(2, "\x85\xC0"))
         + x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω");
}
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string agg_txt_all(const std::string &hdr, IR_t *a, IR_t *b, IR_t *c) {
    return hdr
         + agg_build_term(a) + x86("ins2", "push", "rax")
         + agg_build_term(b) + x86("ins2", "push", "rax")
         + agg_build_term(c) + x86("ins2", "push", "rax")
         + x86("ins2", "sub", "rsp, 8")
         + x86("ins2", "mov", "rdx, [rsp + 8]")
         + x86("ins2", "mov", "rsi, [rsp + 16]")
         + x86("ins2", "mov", "rdi, [rsp + 24]")
         + x86("ins2", "call", "rt_aggregate_all_meta@PLT")
         + x86("ins2", "add", "rsp, 32")
         + x86("ins2", "test", "eax, eax")
         + x86("ins2", "je",   _.lbl_ω)
         + x86("ins2", "jmp",  _.lbl_γ)
         + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
}
static std::string agg_txt_nbset(const std::string &hdr, IR_t *a, IR_t *b) {
    return hdr
         + agg_build_term(a) + x86("ins2", "push", "rax")
         + x86("ins2", "sub", "rsp, 8")
         + agg_build_term(b)
         + x86("ins2", "mov", "rsi, rax")
         + x86("ins2", "mov", "rdi, [rsp + 8]")
         + x86("ins2", "call", "rt_nb_setval_term@PLT")
         + x86("ins2", "add", "rsp, 16")
         + x86("ins2", "test", "eax, eax")
         + x86("ins2", "je",   _.lbl_ω)
         + x86("ins2", "jmp",  _.lbl_γ)
         + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
}
static std::string agg_txt_nbget(const std::string &hdr, IR_t *a, IR_t *b) { int kres = (int)b->t; long ires = (long)b->ival; const char *sres = (kres == IR_ATOM) ? b->sval : NULL;
    return hdr
         + agg_build_term(a)
         + x86("ins2", "mov", "rdi, rax")
         + x86("ins2", "mov", std::string("esi, ") + std::to_string(kres))
         + x86("ins2", "mov", std::string("rdx, ") + std::to_string(ires))
         + IF(sres && *sres, x86("ins2", "lea", std::string("rcx, [rip + ") + anlbl(sres) + "]"))
         + IF(!(sres && *sres), x86("ins2", "xor", "ecx, ecx"))
         + x86("ins2", "call", "rt_nb_getval_term@PLT")
         + x86("ins2", "test", "eax, eax")
         + x86("ins2", "je",   _.lbl_ω)
         + x86("ins2", "jmp",  _.lbl_γ)
         + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_builtin_aggregate_nb_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    if (MEDIUM_BINARY) {
        return IF(!strcmp(fn, "aggregate_all") && pBB->α && pBB->α->γ && pBB->α->γ->γ && _.op_ival == 3, agg_bin_all(pBB->α, pBB->α->γ, pBB->α->γ->γ))
             + IF(!strcmp(fn, "nb_setval") && pBB->α && pBB->α->γ && _.op_ival == 2, agg_bin_nbset(pBB->α, pBB->α->γ))
             + IF(!strcmp(fn, "nb_getval") && pBB->α && pBB->α->γ && _.op_ival == 2, agg_bin_nbget(pBB->α, pBB->α->γ));
    }
    if (MEDIUM_TEXT) {
        return IF(!strcmp(fn, "aggregate_all") && pBB->α && pBB->α->γ && pBB->α->γ->γ && _.op_ival == 3, agg_txt_all(hdr, pBB->α, pBB->α->γ, pBB->α->γ->γ))
             + IF(!strcmp(fn, "nb_setval") && pBB->α && pBB->α->γ && _.op_ival == 2, agg_txt_nbset(hdr, pBB->α, pBB->α->γ))
             + IF(!strcmp(fn, "nb_getval") && pBB->α && pBB->α->γ && _.op_ival == 2, agg_txt_nbget(hdr, pBB->α, pBB->α->γ));
    }
    return std::string();
}
