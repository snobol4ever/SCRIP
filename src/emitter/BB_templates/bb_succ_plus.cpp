#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_lbl(IR_t *nd) {
    if (!(nd->op == IR_ATOM && IR_LIT(nd).sval)) return std::string();
    char l[64]; l[0] = 0; strtab_label(l, sizeof l, IR_LIT(nd).sval);
    return std::string(l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_bin_ports() { return x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_bin_succ(IR_t *a0, IR_t *a1) {
    const char *s0 = (a0->op == IR_ATOM) ? IR_LIT(a0).sval : NULL;
    const char *s1 = (a1->op == IR_ATOM) ? IR_LIT(a1).sval : NULL;
    return x86("mov32", "edi", (long)(int)a0->op)
         + x86("movabs", "rsi", (unsigned long long)(uint64_t)(long)IR_LIT(a0).ival)
         + (s0 ? x86("movabs", "rdx", (unsigned long long)(uintptr_t)s0) : x86("xor", "edx", "edx"))
         + x86("mov32", "ecx", (long)(int)a1->op)
         + x86("movabs", "r8", (unsigned long long)(uint64_t)(long)IR_LIT(a1).ival)
         + (s1 ? x86("movabs", "r9", (unsigned long long)(uintptr_t)s1) : x86("xor", "r9d", "r9d"))
         + x86("call", "rt_succ", (unsigned long long)(uintptr_t)(void*)rt_succ)
         + x86("test", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_bin_plus(IR_t *a0, IR_t *a1, IR_t *a2) {
    const char *s0 = (a0->op == IR_ATOM) ? IR_LIT(a0).sval : NULL;
    const char *s1 = (a1->op == IR_ATOM) ? IR_LIT(a1).sval : NULL;
    const char *s2 = (a2->op == IR_ATOM) ? IR_LIT(a2).sval : NULL;
    return x86("sub", "rsp", 32L)
         + x86("mov32", "edi", (long)(int)a0->op)
         + x86("movabs", "rsi", (unsigned long long)(uint64_t)(long)IR_LIT(a0).ival)
         + (s0 ? x86("movabs", "rdx", (unsigned long long)(uintptr_t)s0) : x86("xor", "edx", "edx"))
         + x86("mov32", "ecx", (long)(int)a1->op)
         + x86("movabs", "r8", (unsigned long long)(uint64_t)(long)IR_LIT(a1).ival)
         + (s1 ? x86("movabs", "r9", (unsigned long long)(uintptr_t)s1) : x86("xor", "r9d", "r9d"))
         + x86("stk32", 0L, (long)(int)a2->op)
         + x86("movabs", "rax", (unsigned long long)(uint64_t)(long)IR_LIT(a2).ival)
         + x86("mov", RSP(8), "rax")
         + (s2 ? x86("movabs", "rax", (unsigned long long)(uintptr_t)s2) : x86("xor", "eax", "eax"))
         + x86("mov", RSP(16), "rax")
         + x86("call", "rt_plus", (unsigned long long)(uintptr_t)(void*)rt_plus)
         + x86("add", "rsp", 32L)
         + x86("test", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_txt_succ(IR_t *a0, IR_t *a1, const std::string &hdr) { std::string l0 = bsp_lbl(a0), l1 = bsp_lbl(a1);
    return hdr
         + x86("ins2", "mov edi,", std::to_string((int)a0->op))
         + x86("ins2", "mov rsi,", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("ins2", "lea rdx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "edx, edx"))
         + x86("ins2", "mov ecx,", std::to_string((int)a1->op))
         + x86("ins2", "mov r8,", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("ins2", "lea r9,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "r9d, r9d"))
         + x86("ins2", "call", "rt_succ@PLT")
         + x86("ins2", "test", "eax, eax")
         + x86("ins2", "je", _.lbl_ω)
         + x86("ins2", "jmp", _.lbl_γ)
         + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bsp_txt_plus(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l0 = bsp_lbl(a0), l1 = bsp_lbl(a1), l2 = bsp_lbl(a2);
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
         + x86("ins2", "mov", "[rsp + 8], rax")
         + (l2.size() ? x86("ins2", "lea rax,", std::string("[rip + ") + l2 + "]") + x86("ins2", "mov", "[rsp + 16], rax") : x86("ins2", "mov", "qword ptr [rsp + 16], 0"))
         + x86("ins2", "call", "rt_plus@PLT")
         + x86("ins2", "add", "rsp, 32")
         + x86("ins2", "test", "eax, eax")
         + x86("ins2", "je", _.lbl_ω)
         + x86("ins2", "jmp", _.lbl_γ)
         + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_succ_plus_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if (strcmp(fn, "succ") == 0 && _.op_ival == 2 && ir_pair_arg(pBB,0) && ir_pair_arg(pBB,1)) {
            IR_t *a0 = ir_pair_arg(pBB,0), *a1 = ir_pair_arg(pBB,1);
            return bsp_bin_succ(a0, a1) + bsp_bin_ports();
        }
        if (strcmp(fn, "plus") == 0 && _.op_ival == 3 && ir_call_arg(pBB,0) && ir_call_arg(pBB,1) && ir_call_arg(pBB,2)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1), *a2 = ir_call_arg(pBB,2);
            return bsp_bin_plus(a0, a1, a2) + bsp_bin_ports();
        }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "succ") == 0 && _.op_ival == 2 && ir_pair_arg(pBB,0) && ir_pair_arg(pBB,1)) {
            IR_t *a0 = ir_pair_arg(pBB,0), *a1 = ir_pair_arg(pBB,1);
            return bsp_txt_succ(a0, a1, hdr);
        }
        if (strcmp(fn, "plus") == 0 && _.op_ival == 3 && ir_call_arg(pBB,0) && ir_call_arg(pBB,1) && ir_call_arg(pBB,2)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1), *a2 = ir_call_arg(pBB,2);
            return bsp_txt_plus(a0, a1, a2, hdr);
        }
    }
    return std::string();
}
