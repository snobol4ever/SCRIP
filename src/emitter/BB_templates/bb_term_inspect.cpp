#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_lbl(IR_t *nd) {
    if (!(nd->t == IR_ATOM && nd->sval)) return std::string();
    char l[64]; l[0] = 0; strtab_label(l, sizeof l, nd->sval);
    return std::string(l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_ports() { return x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_tail() { return x86("ins2", "test", "eax, eax") + x86("ins2", "je", _.lbl_ω) + x86("ins2", "jmp", _.lbl_γ) + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_functor(IR_t *a0, IR_t *a1, IR_t *a2) {
    const char *s1 = (a1->t == IR_ATOM) ? a1->sval : NULL;
    const char *s2 = (a2->t == IR_ATOM) ? a2->sval : NULL;
    return bytes(4, "\x48\x83\xEC\x10")
         + emit_term_from_node_bin(a0)
         + bytes(3, "\x48\x89\xC7")
         + bytes(1, "\xBE") + u32le((uint32_t)(int)a1->t)
         + bytes(2, "\x48\xBA") + u64le((uint64_t)(long)a1->ival)
         + (s1 ? bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s1) : bytes(2, "\x31\xC9"))
         + bytes(2, "\x41\xB8") + u32le((uint32_t)(int)a2->t)
         + bytes(2, "\x49\xB9") + u64le((uint64_t)(long)a2->ival)
         + (s2 ? bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s2) : bytes(2, "\x31\xC0"))
         + bytes(4, "\x48\x89\x04\x24")
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_functor_term) + bytes(2, "\xFF\xD0")
         + bytes(4, "\x48\x83\xC4\x10")
         + bytes(2, "\x85\xC0");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_arg(IR_t *a0, IR_t *a1, IR_t *a2) {
    const char *s0 = (a0->t == IR_ATOM) ? a0->sval : NULL;
    const char *s2 = (a2->t == IR_ATOM) ? a2->sval : NULL;
    return bytes(4, "\x48\x83\xEC\x10")
         + emit_term_from_node_bin(a1)
         + bytes(3, "\x48\x89\xC1")
         + bytes(1, "\xBF") + u32le((uint32_t)(int)a0->t)
         + bytes(2, "\x48\xBE") + u64le((uint64_t)(long)a0->ival)
         + (s0 ? bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)s0) : bytes(2, "\x31\xD2"))
         + bytes(2, "\x41\xB8") + u32le((uint32_t)(int)a2->t)
         + bytes(2, "\x49\xB9") + u64le((uint64_t)(long)a2->ival)
         + (s2 ? bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s2) : bytes(2, "\x31\xC0"))
         + bytes(4, "\x48\x89\x04\x24")
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_arg_term) + bytes(2, "\xFF\xD0")
         + bytes(4, "\x48\x83\xC4\x10")
         + bytes(2, "\x85\xC0");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_univ_tt(IR_t *a0, IR_t *a1) {
    return bytes(4, "\x48\x83\xEC\x10")
         + emit_term_from_node_bin(a0)
         + bytes(4, "\x48\x89\x04\x24")
         + emit_term_from_node_bin(a1)
         + bytes(3, "\x48\x89\xC6")
         + bytes(4, "\x48\x8B\x3C\x24")
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_univ_term_term) + bytes(2, "\xFF\xD0")
         + bytes(4, "\x48\x83\xC4\x10")
         + bytes(2, "\x85\xC0");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_univ_t1(IR_t *a0, IR_t *a1) {
    const char *s1 = (a1->t == IR_ATOM) ? a1->sval : NULL;
    return bytes(4, "\x48\x83\xEC\x08")
         + emit_term_from_node_bin(a0)
         + bytes(3, "\x48\x89\xC7")
         + bytes(1, "\xBE") + u32le((uint32_t)(int)a1->t)
         + bytes(2, "\x48\xBA") + u64le((uint64_t)(long)a1->ival)
         + (s1 ? bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s1) : bytes(2, "\x31\xC9"))
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_univ_term) + bytes(2, "\xFF\xD0")
         + bytes(4, "\x48\x83\xC4\x08")
         + bytes(2, "\x85\xC0");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_univ_1t(IR_t *a0, IR_t *a1) {
    const char *s0 = (a0->t == IR_ATOM) ? a0->sval : NULL;
    return bytes(4, "\x48\x83\xEC\x08")
         + emit_term_from_node_bin(a1)
         + bytes(3, "\x48\x89\xC1")
         + bytes(1, "\xBF") + u32le((uint32_t)(int)a0->t)
         + bytes(2, "\x48\xBE") + u64le((uint64_t)(long)a0->ival)
         + (s0 ? bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)s0) : bytes(2, "\x31\xD2"))
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_univ_term_list) + bytes(2, "\xFF\xD0")
         + bytes(4, "\x48\x83\xC4\x08")
         + bytes(2, "\x85\xC0");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_functor_t(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l1 = bti_lbl(a1), l2 = bti_lbl(a2);
    return hdr
         + x86("ins2", "sub", "rsp, 16")
         + emit_build_compound_term(a0)
         + x86("ins2", "mov", "rdi, rax")
         + x86("ins2", "mov esi,", std::to_string((int)a1->t))
         + x86("ins2", "mov rdx,", std::to_string((long)a1->ival))
         + (l1.size() ? x86("ins2", "lea rcx,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "ecx, ecx"))
         + x86("ins2", "mov r8d,", std::to_string((int)a2->t))
         + x86("ins2", "mov r9,", std::to_string((long)a2->ival))
         + (l2.size() ? x86("ins2", "lea rax,", std::string("[rip + ") + l2 + "]") : x86("ins2", "xor", "eax, eax"))
         + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
         + x86("ins2", "call", "rt_functor_term@PLT")
         + x86("ins2", "add", "rsp, 16")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_functor_s(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l1 = bti_lbl(a1), l2 = bti_lbl(a2), l0 = bti_lbl(a0);
    return hdr
         + x86("ins2", "sub", "rsp, 32")
         + x86("ins2", "mov edi,", std::to_string((int)a0->t))
         + x86("ins2", "mov rsi,", std::to_string((long)a0->ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "mov ecx,", std::to_string((int)a1->t))
         + x86("ins2", "mov r8,", std::to_string((long)a1->ival))
         + (l1.size() ? x86("ins2", "lea r9,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "r9d, r9d"))
         + x86("ins2", "mov dword ptr [rsp + 0],", std::to_string((int)a2->t))
         + x86("ins2", "mov rax,", std::to_string((long)a2->ival))
         + x86("ins2", "mov", "qword ptr [rsp + 8], rax")
         + (l2.size() ? x86("ins2", "lea rax,", std::string("[rip + ") + l2 + "]") : x86("ins2", "xor", "eax, eax"))
         + x86("ins2", "mov", "qword ptr [rsp + 16], rax")
         + x86("ins2", "call", "rt_functor@PLT")
         + x86("ins2", "add", "rsp, 32")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_arg_t(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l0 = bti_lbl(a0), l2 = bti_lbl(a2);
    return hdr
         + x86("ins2", "sub", "rsp, 16")
         + emit_build_compound_term(a1)
         + x86("ins2", "mov", "rcx, rax")
         + x86("ins2", "mov edi,", std::to_string((int)a0->t))
         + x86("ins2", "mov rsi,", std::to_string((long)a0->ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "mov r8d,", std::to_string((int)a2->t))
         + x86("ins2", "mov r9,", std::to_string((long)a2->ival))
         + (l2.size() ? x86("ins2", "lea rax,", std::string("[rip + ") + l2 + "]") : x86("ins2", "xor", "eax, eax"))
         + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
         + x86("ins2", "call", "rt_arg_term@PLT")
         + x86("ins2", "add", "rsp, 16")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_arg_s(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l0 = bti_lbl(a0), l2 = bti_lbl(a2), l1 = bti_lbl(a1);
    return hdr
         + x86("ins2", "sub", "rsp, 32")
         + x86("ins2", "mov edi,", std::to_string((int)a0->t))
         + x86("ins2", "mov rsi,", std::to_string((long)a0->ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "mov ecx,", std::to_string((int)a1->t))
         + x86("ins2", "mov r8,", std::to_string((long)a1->ival))
         + (l1.size() ? x86("ins2", "lea r9,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "r9d, r9d"))
         + x86("ins2", "mov dword ptr [rsp + 0],", std::to_string((int)a2->t))
         + x86("ins2", "mov rax,", std::to_string((long)a2->ival))
         + x86("ins2", "mov", "qword ptr [rsp + 8], rax")
         + (l2.size() ? x86("ins2", "lea rax,", std::string("[rip + ") + l2 + "]") : x86("ins2", "xor", "eax, eax"))
         + x86("ins2", "mov", "qword ptr [rsp + 16], rax")
         + x86("ins2", "call", "rt_arg@PLT")
         + x86("ins2", "add", "rsp, 32")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_univ_tt(IR_t *a0, IR_t *a1, const std::string &hdr) {
    return hdr
         + x86("ins2", "sub", "rsp, 16")
         + emit_build_compound_term(a0)
         + x86("ins2", "mov", "qword ptr [rsp + 0], rax")
         + emit_build_compound_term(a1)
         + x86("ins2", "mov", "rsi, rax")
         + x86("ins2", "mov", "rdi, qword ptr [rsp + 0]")
         + x86("ins2", "call", "rt_univ_term_term@PLT")
         + x86("ins2", "add", "rsp, 16")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_univ_t1(IR_t *a0, IR_t *a1, const std::string &hdr) { std::string l1 = bti_lbl(a1);
    return hdr
         + x86("ins2", "sub", "rsp, 8")
         + emit_build_compound_term(a0)
         + x86("ins2", "mov", "rdi, rax")
         + x86("ins2", "mov esi,", std::to_string((int)a1->t))
         + x86("ins2", "mov rdx,", std::to_string((long)a1->ival))
         + (l1.size() ? x86("ins2", "lea rcx,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "ecx, ecx"))
         + x86("ins2", "call", "rt_univ_term@PLT")
         + x86("ins2", "add", "rsp, 8")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_univ_1t(IR_t *a0, IR_t *a1, const std::string &hdr) { std::string l0 = bti_lbl(a0);
    return hdr
         + x86("ins2", "sub", "rsp, 8")
         + emit_build_compound_term(a1)
         + x86("ins2", "mov", "rcx, rax")
         + x86("ins2", "mov edi,", std::to_string((int)a0->t))
         + x86("ins2", "mov rsi,", std::to_string((long)a0->ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "call", "rt_univ_term_list@PLT")
         + x86("ins2", "add", "rsp, 8")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_univ_ss(IR_t *a0, IR_t *a1, const std::string &hdr) { std::string l0 = bti_lbl(a0), l1 = bti_lbl(a1);
    return hdr
         + x86("ins2", "sub", "rsp, 8")
         + x86("ins2", "mov edi,", std::to_string((int)a0->t))
         + x86("ins2", "mov rsi,", std::to_string((long)a0->ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "mov ecx,", std::to_string((int)a1->t))
         + x86("ins2", "mov r8,", std::to_string((long)a1->ival))
         + (l1.size() ? x86("ins2", "lea r9,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "r9d, r9d"))
         + x86("ins2", "call", "rt_univ@PLT")
         + x86("ins2", "add", "rsp, 8")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_term_inspect_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if (strcmp(fn, "functor") == 0 && _.op_ival == 3 && pBB->α && pBB->α->γ && pBB->α->γ->γ && pBB->α->t == IR_STRUCT) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            return x86_lit_bytes(bti_bin_functor(a0, a1, a2)) + bti_bin_ports();
        }
        if (strcmp(fn, "arg") == 0 && _.op_ival == 3 && pBB->α && pBB->α->γ && pBB->α->γ->γ && pBB->α->γ->t == IR_STRUCT) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            return x86_lit_bytes(bti_bin_arg(a0, a1, a2)) + bti_bin_ports();
        }
        if (strcmp(fn, "=..") == 0 && _.op_ival == 2 && pBB->α && pBB->α->γ && (pBB->α->t == IR_STRUCT || pBB->α->γ->t == IR_STRUCT)) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            return x86_lit_bytes(a0->t == IR_STRUCT && a1->t == IR_STRUCT ? bti_bin_univ_tt(a0, a1) : a0->t == IR_STRUCT ? bti_bin_univ_t1(a0, a1) : bti_bin_univ_1t(a0, a1)) + bti_bin_ports();
        }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "functor") == 0 && _.op_ival == 3 && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            return a0->t == IR_STRUCT ? bti_txt_functor_t(a0, a1, a2, hdr) : bti_txt_functor_s(a0, a1, a2, hdr);
        }
        if (strcmp(fn, "arg") == 0 && _.op_ival == 3 && pBB->α && pBB->α->γ && pBB->α->γ->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ, *a2 = a1->γ;
            return a1->t == IR_STRUCT ? bti_txt_arg_t(a0, a1, a2, hdr) : bti_txt_arg_s(a0, a1, a2, hdr);
        }
        if (strcmp(fn, "=..") == 0 && _.op_ival == 2 && pBB->α && pBB->α->γ) {
            IR_t *a0 = pBB->α, *a1 = a0->γ;
            return a0->t == IR_STRUCT && a1->t == IR_STRUCT ? bti_txt_univ_tt(a0, a1, hdr) : a0->t == IR_STRUCT ? bti_txt_univ_t1(a0, a1, hdr) : a1->t == IR_STRUCT ? bti_txt_univ_1t(a0, a1, hdr) : bti_txt_univ_ss(a0, a1, hdr);
        }
    }
    return std::string();
}
