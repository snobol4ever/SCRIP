#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_lbl(IR_t *nd) {
    if (!(nd->t == IR_ATOM && IR_LIT(nd).sval)) return std::string();
    char l[64]; l[0] = 0; strtab_label(l, sizeof l, IR_LIT(nd).sval);
    return std::string(l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_bin_ports() { return x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_txt_tail() { return x86("ins2", "test", "eax, eax") + x86("ins2", "je", _.lbl_ω) + x86("ins2", "jmp", _.lbl_γ) + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_bin_succ(IR_t *a0, IR_t *a1) {
    const char *s0 = (a0->t == IR_ATOM) ? IR_LIT(a0).sval : NULL;
    const char *s1 = (a1->t == IR_ATOM) ? IR_LIT(a1).sval : NULL;
    return bytes(1, "\xBF") + u32le((uint32_t)(int)a0->t)
         + bytes(2, "\x48\xBE") + u64le((uint64_t)(long)IR_LIT(a0).ival)
         + (s0 ? bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)s0) : bytes(2, "\x31\xD2"))
         + bytes(1, "\xB9") + u32le((uint32_t)(int)a1->t)
         + bytes(2, "\x49\xB8") + u64le((uint64_t)(long)IR_LIT(a1).ival)
         + (s1 ? bytes(2, "\x49\xB9") + u64le((uint64_t)(uintptr_t)s1) : bytes(3, "\x45\x31\xC9"))
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_succ) + bytes(2, "\xFF\xD0")
         + bytes(2, "\x85\xC0");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_bin_plus(IR_t *a0, IR_t *a1, IR_t *a2) {
    const char *s0 = (a0->t == IR_ATOM) ? IR_LIT(a0).sval : NULL;
    const char *s1 = (a1->t == IR_ATOM) ? IR_LIT(a1).sval : NULL;
    const char *s2 = (a2->t == IR_ATOM) ? IR_LIT(a2).sval : NULL;
    return bytes(4, "\x48\x83\xEC\x20")
         + bytes(1, "\xBF") + u32le((uint32_t)(int)a0->t)
         + bytes(2, "\x48\xBE") + u64le((uint64_t)(long)IR_LIT(a0).ival)
         + (s0 ? bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)s0) : bytes(2, "\x31\xD2"))
         + bytes(1, "\xB9") + u32le((uint32_t)(int)a1->t)
         + bytes(2, "\x49\xB8") + u64le((uint64_t)(long)IR_LIT(a1).ival)
         + (s1 ? bytes(2, "\x49\xB9") + u64le((uint64_t)(uintptr_t)s1) : bytes(3, "\x45\x31\xC9"))
         + bytes(3, "\xC7\x04\x24") + u32le((uint32_t)(int)a2->t)
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(long)IR_LIT(a2).ival)
         + bytes(5, "\x48\x89\x44\x24\x08")
         + (s2 ? bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s2) : bytes(2, "\x31\xC0"))
         + bytes(5, "\x48\x89\x44\x24\x10")
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_plus) + bytes(2, "\xFF\xD0")
         + bytes(4, "\x48\x83\xC4\x20")
         + bytes(2, "\x85\xC0");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_txt_succ(IR_t *a0, IR_t *a1, const std::string &hdr) { std::string l0 = bsp_lbl(a0), l1 = bsp_lbl(a1);
    return hdr
         + x86("ins2", "mov edi,", std::to_string((int)a0->t))
         + x86("ins2", "mov rsi,", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "mov ecx,", std::to_string((int)a1->t))
         + x86("ins2", "mov r8,", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("ins2", "lea r9,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "r9d, r9d"))
         + x86("ins2", "call", "rt_succ@PLT")
         + bsp_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_txt_plus(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l0 = bsp_lbl(a0), l1 = bsp_lbl(a1), l2 = bsp_lbl(a2);
    return hdr
         + x86("ins2", "sub", "rsp, 32")
         + x86("ins2", "mov edi,", std::to_string((int)a0->t))
         + x86("ins2", "mov rsi,", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "mov ecx,", std::to_string((int)a1->t))
         + x86("ins2", "mov r8,", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("ins2", "lea r9,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "r9d, r9d"))
         + x86("ins2", "mov dword ptr [rsp + 0],", std::to_string((int)a2->t))
         + x86("ins2", "mov rax,", std::to_string((long)IR_LIT(a2).ival))
         + x86("ins2", "mov", "[rsp + 8], rax")
         + (l2.size() ? x86("ins2", "lea rax,", std::string("[rip + ") + l2 + "]") + x86("ins2", "mov", "[rsp + 16], rax") : x86("ins2", "mov", "qword ptr [rsp + 16], 0"))
         + x86("ins2", "call", "rt_plus@PLT")
         + x86("ins2", "add", "rsp, 32")
         + bsp_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_succ_plus_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if (strcmp(fn, "succ") == 0 && _.op_ival == 2 && pBB->α && pBB->β) {
            IR_t *a0 = pBB->α, *a1 = pBB->β;
            return x86_lit_bytes(bsp_bin_succ(a0, a1)) + bsp_bin_ports();
        }
        if (strcmp(fn, "plus") == 0 && _.op_ival == 3 && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            return x86_lit_bytes(bsp_bin_plus(a0, a1, a2)) + bsp_bin_ports();
        }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "succ") == 0 && _.op_ival == 2 && pBB->α && pBB->β) {
            IR_t *a0 = pBB->α, *a1 = pBB->β;
            return bsp_txt_succ(a0, a1, hdr);
        }
        if (strcmp(fn, "plus") == 0 && _.op_ival == 3 && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            return bsp_txt_plus(a0, a1, a2, hdr);
        }
    }
    return std::string();
}
