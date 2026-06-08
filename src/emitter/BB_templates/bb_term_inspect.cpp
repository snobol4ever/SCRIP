#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_lbl(IR_t *nd) {
    if (!(nd->op == IR_ATOM && IR_LIT(nd).sval)) return std::string();
    char l[64]; l[0] = 0; strtab_label(l, sizeof l, IR_LIT(nd).sval);
    return std::string(l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_ports() { return x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_tail() { return x86("ins2", "test", "eax, eax") + x86("ins2", "je", _.lbl_ω) + x86("ins2", "jmp", _.lbl_γ) + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_functor(IR_t *a0, IR_t *a1, IR_t *a2) {
    const char *s1 = (a1->op == IR_ATOM) ? IR_LIT(a1).sval : NULL;
    const char *s2 = (a2->op == IR_ATOM) ? IR_LIT(a2).sval : NULL;
    return x86("sub", "rsp", 16L)
         + x86_lit_bytes(emit_term_from_node_bin(a0))
         + x86("mov", "rdi", "rax")
         + x86("mov32", "esi", (long)(int)a1->op)
         + x86("movabs", "rdx", (unsigned long long)(uint64_t)(long)IR_LIT(a1).ival)
         + (s1 ? x86("movabs", "rcx", (unsigned long long)(uintptr_t)s1) : x86("xor", "ecx", "ecx"))
         + x86("mov32", "r8d", (long)(int)a2->op)
         + x86("movabs", "r9", (unsigned long long)(uint64_t)(long)IR_LIT(a2).ival)
         + (s2 ? x86("movabs", "rax", (unsigned long long)(uintptr_t)s2) : x86("xor", "eax", "eax"))
         + x86("mov", RSP(0), "rax")
         + x86("call", "rt_functor_term", (unsigned long long)(uintptr_t)(void*)rt_functor_term)
         + x86("add", "rsp", 16L)
         + x86("test", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_arg(IR_t *a0, IR_t *a1, IR_t *a2) {
    const char *s0 = (a0->op == IR_ATOM) ? IR_LIT(a0).sval : NULL;
    const char *s2 = (a2->op == IR_ATOM) ? IR_LIT(a2).sval : NULL;
    return x86("sub", "rsp", 16L)
         + x86_lit_bytes(emit_term_from_node_bin(a1))
         + x86("mov", "rcx", "rax")
         + x86("mov32", "edi", (long)(int)a0->op)
         + x86("movabs", "rsi", (unsigned long long)(uint64_t)(long)IR_LIT(a0).ival)
         + (s0 ? x86("movabs", "rdx", (unsigned long long)(uintptr_t)s0) : x86("xor", "edx", "edx"))
         + x86("mov32", "r8d", (long)(int)a2->op)
         + x86("movabs", "r9", (unsigned long long)(uint64_t)(long)IR_LIT(a2).ival)
         + (s2 ? x86("movabs", "rax", (unsigned long long)(uintptr_t)s2) : x86("xor", "eax", "eax"))
         + x86("mov", RSP(0), "rax")
         + x86("call", "rt_arg_term", (unsigned long long)(uintptr_t)(void*)rt_arg_term)
         + x86("add", "rsp", 16L)
         + x86("test", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_univ_tt(IR_t *a0, IR_t *a1) {
    return x86("sub", "rsp", 16L)
         + x86_lit_bytes(emit_term_from_node_bin(a0))
         + x86("mov", RSP(0), "rax")
         + x86_lit_bytes(emit_term_from_node_bin(a1))
         + x86("mov", "rsi", "rax")
         + x86("mov", "rdi", RSP(0))
         + x86("call", "rt_univ_term_term", (unsigned long long)(uintptr_t)(void*)rt_univ_term_term)
         + x86("add", "rsp", 16L)
         + x86("test", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_univ_t1(IR_t *a0, IR_t *a1) {
    const char *s1 = (a1->op == IR_ATOM) ? IR_LIT(a1).sval : NULL;
    return x86("sub", "rsp", 8L)
         + x86_lit_bytes(emit_term_from_node_bin(a0))
         + x86("mov", "rdi", "rax")
         + x86("mov32", "esi", (long)(int)a1->op)
         + x86("movabs", "rdx", (unsigned long long)(uint64_t)(long)IR_LIT(a1).ival)
         + (s1 ? x86("movabs", "rcx", (unsigned long long)(uintptr_t)s1) : x86("xor", "ecx", "ecx"))
         + x86("call", "rt_univ_term", (unsigned long long)(uintptr_t)(void*)rt_univ_term)
         + x86("add", "rsp", 8L)
         + x86("test", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_bin_univ_1t(IR_t *a0, IR_t *a1) {
    const char *s0 = (a0->op == IR_ATOM) ? IR_LIT(a0).sval : NULL;
    return x86("sub", "rsp", 8L)
         + x86_lit_bytes(emit_term_from_node_bin(a1))
         + x86("mov", "rcx", "rax")
         + x86("mov32", "edi", (long)(int)a0->op)
         + x86("movabs", "rsi", (unsigned long long)(uint64_t)(long)IR_LIT(a0).ival)
         + (s0 ? x86("movabs", "rdx", (unsigned long long)(uintptr_t)s0) : x86("xor", "edx", "edx"))
         + x86("call", "rt_univ_term_list", (unsigned long long)(uintptr_t)(void*)rt_univ_term_list)
         + x86("add", "rsp", 8L)
         + x86("test", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_functor_t(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l1 = bti_lbl(a1), l2 = bti_lbl(a2);
    return hdr
         + x86("ins2", "sub", "rsp, 16")
         + emit_build_compound_term(a0)
         + x86("ins2", "mov", "rdi, rax")
         + x86("ins2", "mov esi,", std::to_string((int)a1->op))
         + x86("ins2", "mov rdx,", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("ins2", "lea rcx,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "ecx, ecx"))
         + x86("ins2", "mov r8d,", std::to_string((int)a2->op))
         + x86("ins2", "mov r9,", std::to_string((long)IR_LIT(a2).ival))
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
         + x86("ins2", "mov edi,", std::to_string((int)a0->op))
         + x86("ins2", "mov rsi,", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "mov ecx,", std::to_string((int)a1->op))
         + x86("ins2", "mov r8,", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("ins2", "lea r9,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "r9d, r9d"))
         + x86("ins2", "mov dword ptr [rsp + 0],", std::to_string((int)a2->op))
         + x86("ins2", "mov rax,", std::to_string((long)IR_LIT(a2).ival))
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
         + x86("ins2", "mov edi,", std::to_string((int)a0->op))
         + x86("ins2", "mov rsi,", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "mov r8d,", std::to_string((int)a2->op))
         + x86("ins2", "mov r9,", std::to_string((long)IR_LIT(a2).ival))
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
         + x86("ins2", "mov edi,", std::to_string((int)a0->op))
         + x86("ins2", "mov rsi,", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "mov ecx,", std::to_string((int)a1->op))
         + x86("ins2", "mov r8,", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("ins2", "lea r9,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "r9d, r9d"))
         + x86("ins2", "mov dword ptr [rsp + 0],", std::to_string((int)a2->op))
         + x86("ins2", "mov rax,", std::to_string((long)IR_LIT(a2).ival))
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
         + x86("ins2", "mov esi,", std::to_string((int)a1->op))
         + x86("ins2", "mov rdx,", std::to_string((long)IR_LIT(a1).ival))
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
         + x86("ins2", "mov edi,", std::to_string((int)a0->op))
         + x86("ins2", "mov rsi,", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "call", "rt_univ_term_list@PLT")
         + x86("ins2", "add", "rsp, 8")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_univ_ss(IR_t *a0, IR_t *a1, const std::string &hdr) { std::string l0 = bti_lbl(a0), l1 = bti_lbl(a1);
    return hdr
         + x86("ins2", "sub", "rsp, 8")
         + x86("ins2", "mov edi,", std::to_string((int)a0->op))
         + x86("ins2", "mov rsi,", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "mov ecx,", std::to_string((int)a1->op))
         + x86("ins2", "mov r8,", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("ins2", "lea r9,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "r9d, r9d"))
         + x86("ins2", "call", "rt_univ@PLT")
         + x86("ins2", "add", "rsp, 8")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_term_inspect_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if (strcmp(fn, "functor") == 0 && _.op_ival == 3 && ir_call_arg(pBB,0) && ir_call_arg(pBB,1) && ir_call_arg(pBB,2) && ir_call_arg(pBB,0)->op == IR_STRUCT) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1), *a2 = ir_call_arg(pBB,2);
            return bti_bin_functor(a0, a1, a2) + bti_bin_ports();
        }
        if (strcmp(fn, "arg") == 0 && _.op_ival == 3 && ir_call_arg(pBB,0) && ir_call_arg(pBB,1) && ir_call_arg(pBB,2) && ir_call_arg(pBB,1)->op == IR_STRUCT) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1), *a2 = ir_call_arg(pBB,2);
            return bti_bin_arg(a0, a1, a2) + bti_bin_ports();
        }
        if (strcmp(fn, "=..") == 0 && _.op_ival == 2 && ir_call_arg(pBB,0) && ir_call_arg(pBB,1) && (ir_call_arg(pBB,0)->op == IR_STRUCT || ir_call_arg(pBB,1)->op == IR_STRUCT)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1);
            return (a0->op == IR_STRUCT && a1->op == IR_STRUCT ? bti_bin_univ_tt(a0, a1) : a0->op == IR_STRUCT ? bti_bin_univ_t1(a0, a1) : bti_bin_univ_1t(a0, a1)) + bti_bin_ports();
        }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "functor") == 0 && _.op_ival == 3 && ir_call_arg(pBB,0) && ir_call_arg(pBB,1) && ir_call_arg(pBB,2)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1), *a2 = ir_call_arg(pBB,2);
            return a0->op == IR_STRUCT ? bti_txt_functor_t(a0, a1, a2, hdr) : bti_txt_functor_s(a0, a1, a2, hdr);
        }
        if (strcmp(fn, "arg") == 0 && _.op_ival == 3 && ir_call_arg(pBB,0) && ir_call_arg(pBB,1) && ir_call_arg(pBB,2)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1), *a2 = ir_call_arg(pBB,2);
            return a1->op == IR_STRUCT ? bti_txt_arg_t(a0, a1, a2, hdr) : bti_txt_arg_s(a0, a1, a2, hdr);
        }
        if (strcmp(fn, "=..") == 0 && _.op_ival == 2 && ir_call_arg(pBB,0) && ir_call_arg(pBB,1)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1);
            return a0->op == IR_STRUCT && a1->op == IR_STRUCT ? bti_txt_univ_tt(a0, a1, hdr) : a0->op == IR_STRUCT ? bti_txt_univ_t1(a0, a1, hdr) : a1->op == IR_STRUCT ? bti_txt_univ_1t(a0, a1, hdr) : bti_txt_univ_ss(a0, a1, hdr);
        }
    }
    return std::string();
}
