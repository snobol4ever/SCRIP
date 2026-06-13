#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_lbl(IR_t *nd) {
    if (!(nd->op == IR_ATOM && IR_LIT(nd).sval)) return std::string();
    char l[64]; l[0] = 0; strtab_label(l, sizeof l, IR_LIT(nd).sval);
    return std::string(l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_tail() { return x86("test", "eax", "eax") + x86("je", "ω") + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_functor_t(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l1 = bti_lbl(a1), l2 = bti_lbl(a2);
    return hdr
         + x86("sub", "rsp", "16")
         + emit_build_compound_term(a0)
         + x86("mov", "rdi", "rax")
         + x86("mov", "esi", std::to_string((int)a1->op))
         + x86("mov", "rdx", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("lea", "rcx", std::string("[rip + ") + l1 + "]") : x86("xor", "ecx", "ecx"))
         + x86("mov", "r8d", std::to_string((int)a2->op))
         + x86("mov", "r9", std::to_string((long)IR_LIT(a2).ival))
         + (l2.size() ? x86("lea", "rax", std::string("[rip + ") + l2 + "]") : x86("xor", "eax", "eax"))
         + x86("mov", "qword ptr [rsp + 0]", "rax")
         + x86("call", "rt_functor_term@PLT")
         + x86("add", "rsp", "16")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_functor_s(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l1 = bti_lbl(a1), l2 = bti_lbl(a2), l0 = bti_lbl(a0);
    return hdr
         + x86("sub", "rsp", "32")
         + x86("mov", "edi", std::to_string((int)a0->op))
         + x86("mov", "rsi", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("lea", "rdx", std::string("[rip + ") + l0 + "]") : x86("xor", "edx", "edx"))
         + x86("mov", "ecx", std::to_string((int)a1->op))
         + x86("mov", "r8", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("lea", "r9", std::string("[rip + ") + l1 + "]") : x86("xor", "r9d", "r9d"))
         + x86("mov", RSP(0), (long)(int)a2->op)
         + x86("mov", "rax", std::to_string((long)IR_LIT(a2).ival))
         + x86("mov", "qword ptr [rsp + 8]", "rax")
         + (l2.size() ? x86("lea", "rax", std::string("[rip + ") + l2 + "]") : x86("xor", "eax", "eax"))
         + x86("mov", "qword ptr [rsp + 16]", "rax")
         + x86("call", "rt_functor@PLT")
         + x86("add", "rsp", "32")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_arg_t(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l0 = bti_lbl(a0), l2 = bti_lbl(a2);
    return hdr
         + x86("sub", "rsp", "16")
         + emit_build_compound_term(a1)
         + x86("mov", "rcx", "rax")
         + x86("mov", "edi", std::to_string((int)a0->op))
         + x86("mov", "rsi", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("lea", "rdx", std::string("[rip + ") + l0 + "]") : x86("xor", "edx", "edx"))
         + x86("mov", "r8d", std::to_string((int)a2->op))
         + x86("mov", "r9", std::to_string((long)IR_LIT(a2).ival))
         + (l2.size() ? x86("lea", "rax", std::string("[rip + ") + l2 + "]") : x86("xor", "eax", "eax"))
         + x86("mov", "qword ptr [rsp + 0]", "rax")
         + x86("call", "rt_arg_term@PLT")
         + x86("add", "rsp", "16")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_arg_s(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l0 = bti_lbl(a0), l2 = bti_lbl(a2), l1 = bti_lbl(a1);
    return hdr
         + x86("sub", "rsp", "32")
         + x86("mov", "edi", std::to_string((int)a0->op))
         + x86("mov", "rsi", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("lea", "rdx", std::string("[rip + ") + l0 + "]") : x86("xor", "edx", "edx"))
         + x86("mov", "ecx", std::to_string((int)a1->op))
         + x86("mov", "r8", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("lea", "r9", std::string("[rip + ") + l1 + "]") : x86("xor", "r9d", "r9d"))
         + x86("mov", RSP(0), (long)(int)a2->op)
         + x86("mov", "rax", std::to_string((long)IR_LIT(a2).ival))
         + x86("mov", "qword ptr [rsp + 8]", "rax")
         + (l2.size() ? x86("lea", "rax", std::string("[rip + ") + l2 + "]") : x86("xor", "eax", "eax"))
         + x86("mov", "qword ptr [rsp + 16]", "rax")
         + x86("call", "rt_arg@PLT")
         + x86("add", "rsp", "32")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_univ_tt(IR_t *a0, IR_t *a1, const std::string &hdr) {
    return hdr
         + x86("sub", "rsp", "16")
         + emit_build_compound_term(a0)
         + x86("mov", "qword ptr [rsp + 0]", "rax")
         + emit_build_compound_term(a1)
         + x86("mov", "rsi", "rax")
         + x86("mov", "rdi", "qword ptr [rsp + 0]")
         + x86("call", "rt_univ_term_term@PLT")
         + x86("add", "rsp", "16")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_univ_t1(IR_t *a0, IR_t *a1, const std::string &hdr) { std::string l1 = bti_lbl(a1);
    return hdr
         + x86("sub", "rsp", "8")
         + emit_build_compound_term(a0)
         + x86("mov", "rdi", "rax")
         + x86("mov", "esi", std::to_string((int)a1->op))
         + x86("mov", "rdx", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("lea", "rcx", std::string("[rip + ") + l1 + "]") : x86("xor", "ecx", "ecx"))
         + x86("call", "rt_univ_term@PLT")
         + x86("add", "rsp", "8")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_univ_1t(IR_t *a0, IR_t *a1, const std::string &hdr) { std::string l0 = bti_lbl(a0);
    return hdr
         + x86("sub", "rsp", "8")
         + emit_build_compound_term(a1)
         + x86("mov", "rcx", "rax")
         + x86("mov", "edi", std::to_string((int)a0->op))
         + x86("mov", "rsi", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("lea", "rdx", std::string("[rip + ") + l0 + "]") : x86("xor", "edx", "edx"))
         + x86("call", "rt_univ_term_list@PLT")
         + x86("add", "rsp", "8")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bti_txt_univ_ss(IR_t *a0, IR_t *a1, const std::string &hdr) { std::string l0 = bti_lbl(a0), l1 = bti_lbl(a1);
    return hdr
         + x86("sub", "rsp", "8")
         + x86("mov", "edi", std::to_string((int)a0->op))
         + x86("mov", "rsi", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("lea", "rdx", std::string("[rip + ") + l0 + "]") : x86("xor", "edx", "edx"))
         + x86("mov", "ecx", std::to_string((int)a1->op))
         + x86("mov", "r8", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("lea", "r9", std::string("[rip + ") + l1 + "]") : x86("xor", "r9d", "r9d"))
         + x86("call", "rt_univ@PLT")
         + x86("add", "rsp", "8")
         + bti_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_term_inspect_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) return x86_bomb("bb_term_inspect: BINARY arm removed — baked IR_t* at runtime (IR-NEVER-TOUCHED rule)");
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
