#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string btio_lbl(IR_t *nd) {
    if (!(nd->op == IR_ATOM && IR_LIT(nd).sval)) return std::string();
    char l[64]; l[0] = 0; strtab_label(l, sizeof l, IR_LIT(nd).sval);
    return std::string(l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string btio_bin_ports() { return x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string btio_txt_tail() { return x86("ins2", "test", "eax, eax") + x86("ins2", "je", _.lbl_ω) + x86("ins2", "jmp", _.lbl_γ) + x86("Lins2", std::string(_.lbl_β) + ":", "jmp", _.lbl_ω); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string btio_bin_numbervars(IR_t *a0, IR_t *a1, IR_t *a2) {
    const char *s2 = (a2->op == IR_ATOM) ? IR_LIT(a2).sval : NULL;
    return x86("sub", "rsp", 8L)
         + x86_lit_bytes(emit_term_from_node_bin(a0))
         + x86("mov", "rdi", "rax")
         + x86("movabs", "rsi", (unsigned long long)(uint64_t)(long)IR_LIT(a1).ival)
         + x86("mov32", "edx", (long)(int)a2->op)
         + x86("movabs", "rcx", (unsigned long long)(uint64_t)(long)IR_LIT(a2).ival)
         + (s2 ? x86("movabs", "r8", (unsigned long long)(uintptr_t)s2) : x86("xor", "r8d", "r8d"))
         + x86("call", "rt_numbervars_term", (unsigned long long)(uintptr_t)(void*)rt_numbervars_term)
         + x86("add", "rsp", 8L)
         + x86("test", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string btio_bin_term_to_atom(IR_t *a0, IR_t *a1) {
    const char *s1 = (a1->op == IR_ATOM) ? IR_LIT(a1).sval : NULL;
    return x86("sub", "rsp", 16L)
         + x86_lit_bytes(emit_term_from_node_bin(a0))
         + x86("mov", "rdi", "rax")
         + x86("mov32", "esi", (long)(int)a1->op)
         + x86("movabs", "rdx", (unsigned long long)(uint64_t)(long)IR_LIT(a1).ival)
         + (s1 ? x86("movabs", "rcx", (unsigned long long)(uintptr_t)s1) : x86("xor", "ecx", "ecx"))
         + x86("call", "rt_term_to_atom_term", (unsigned long long)(uintptr_t)(void*)rt_term_to_atom_term)
         + x86("add", "rsp", 16L)
         + x86("test", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string btio_bin_format_b(IR_t *a0, IR_t *a1, long arity) {
    const char *s0 = (a0->op == IR_ATOM) ? IR_LIT(a0).sval : NULL;
    return x86("sub", "rsp", 16L)
         + x86_lit_bytes(emit_term_from_node_bin(a1))
         + x86("mov", "r8", "rax")
         + x86("mov32", "edi", arity)
         + x86("mov32", "esi", (long)(int)a0->op)
         + x86("movabs", "rdx", (unsigned long long)(uint64_t)(long)IR_LIT(a0).ival)
         + (s0 ? x86("movabs", "rcx", (unsigned long long)(uintptr_t)s0) : x86("xor", "ecx", "ecx"))
         + x86("call", "rt_format_term", (unsigned long long)(uintptr_t)(void*)rt_format_term)
         + x86("add", "rsp", 16L)
         + x86("test", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string btio_bin_format_a(IR_t *a0, IR_t *a1, long arity) {
    const char *s0 = (a0->op == IR_ATOM) ? IR_LIT(a0).sval : NULL;
    const char *s1 = (a1 && a1->op == IR_ATOM) ? IR_LIT(a1).sval : NULL;
    return x86("sub", "rsp", 16L)
         + x86("mov32", "edi", arity)
         + x86("mov32", "esi", (long)(int)a0->op)
         + x86("movabs", "rdx", (unsigned long long)(uint64_t)(long)IR_LIT(a0).ival)
         + (s0 ? x86("movabs", "rcx", (unsigned long long)(uintptr_t)s0) : x86("xor", "ecx", "ecx"))
         + x86("mov32", "r8d", (long)(a1 ? (int)a1->op : 0))
         + x86("movabs", "r9", (unsigned long long)(uint64_t)(a1 ? (long)IR_LIT(a1).ival : 0L))
         + (s1 ? x86("movabs", "rax", (unsigned long long)(uintptr_t)s1) : x86("xor", "eax", "eax"))
         + x86("mov", RSP(0), "rax")
         + x86("call", "rt_format", (unsigned long long)(uintptr_t)(void*)rt_format)
         + x86("add", "rsp", 16L)
         + x86("test", "eax", "eax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string btio_txt_numbervars(IR_t *a0, IR_t *a1, IR_t *a2, const std::string &hdr) { std::string l2 = btio_lbl(a2);
    return hdr
         + x86("ins2", "sub", "rsp, 8")
         + emit_build_compound_term(a0)
         + x86("ins2", "mov", "rdi, rax")
         + x86("ins2", "mov rsi,", std::to_string((long)IR_LIT(a1).ival))
         + x86("ins2", "mov edx,", std::to_string((int)a2->op))
         + x86("ins2", "mov rcx,", std::to_string((long)IR_LIT(a2).ival))
         + (l2.size() ? x86("ins2", "lea r8,", std::string("[rip + ") + l2 + "]") : x86("ins2", "xor", "r8d, r8d"))
         + x86("ins2", "call", "rt_numbervars_term@PLT")
         + x86("ins2", "add", "rsp, 8")
         + btio_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string btio_txt_term_to_atom(IR_t *a0, IR_t *a1, const std::string &hdr) { std::string l1 = btio_lbl(a1);
    return hdr
         + x86("ins2", "sub", "rsp, 16")
         + emit_build_compound_term(a0)
         + x86("ins2", "mov", "rdi, rax")
         + x86("ins2", "mov esi,", std::to_string((int)a1->op))
         + x86("ins2", "mov rdx,", std::to_string((long)IR_LIT(a1).ival))
         + (l1.size() ? x86("ins2", "lea rcx,", std::string("[rip + ") + l1 + "]") : x86("ins2", "xor", "ecx, ecx"))
         + x86("ins2", "call", "rt_term_to_atom_term@PLT")
         + x86("ins2", "add", "rsp, 16")
         + btio_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string btio_txt_format_b(IR_t *a0, IR_t *a1, long arity, const std::string &hdr) { std::string l0 = btio_lbl(a0);
    return hdr
         + x86("ins2", "sub", "rsp, 16")
         + emit_build_compound_term(a1)
         + x86("ins2", "mov", "r8, rax")
         + x86("ins2", "mov edi,", std::to_string(arity))
         + x86("ins2", "mov esi,", std::to_string((int)a0->op))
         + x86("ins2", "mov rdx,", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("ins2", "lea rcx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "ecx, ecx"))
         + x86("ins2", "call", "rt_format_term@PLT")
         + x86("ins2", "add", "rsp, 16")
         + btio_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string btio_txt_format_a(IR_t *a0, IR_t *a1, long arity, const std::string &hdr) { std::string l0 = btio_lbl(a0), l1 = a1 ? btio_lbl(a1) : std::string();
    return hdr
         + x86("ins2", "sub", "rsp, 16")
         + x86("ins2", "mov edi,", std::to_string(arity))
         + x86("ins2", "mov esi,", std::to_string((int)a0->op))
         + x86("ins2", "mov rdx,", std::to_string((long)IR_LIT(a0).ival))
         + (l0.size() ? x86("ins2", "lea rcx,", std::string("[rip + ") + l0 + "]") : x86("ins2", "xor", "ecx, ecx"))
         + x86("ins2", "mov r8d,", std::to_string(a1 ? (int)a1->op : 0))
         + x86("ins2", "mov r9,", std::to_string(a1 ? (long)IR_LIT(a1).ival : 0L))
         + (l1.size()
               ? (x86("ins2", "lea rax,", std::string("[rip + ") + l1 + "]") + x86("ins2", "mov", "[rsp + 0], rax"))
               : x86("ins2", "mov", "qword ptr [rsp + 0], 0"))
         + x86("ins2", "call", "rt_format@PLT")
         + x86("ins2", "add", "rsp, 16")
         + btio_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_term_io_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if (strcmp(fn, "numbervars") == 0 && _.op_ival == 3 && ir_call_arg(pBB,0) && ir_call_arg(pBB,1) && ir_call_arg(pBB,2)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1), *a2 = ir_call_arg(pBB,2);
            return btio_bin_numbervars(a0, a1, a2) + btio_bin_ports();
        }
        if ((strcmp(fn, "term_to_atom") == 0 || strcmp(fn, "term_string") == 0) && ir_call_arg(pBB,0) && ir_call_arg(pBB,1)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1);
            return btio_bin_term_to_atom(a0, a1) + btio_bin_ports();
        }
        if (strcmp(fn, "format") == 0 && ir_call_arg(pBB,0) && (_.op_ival == 1 || _.op_ival == 2)) {
            IR_t *a0 = ir_call_arg(pBB,0); IR_t *a1 = (_.op_ival == 2) ? ir_call_arg(pBB,1) : (IR_t*)0;
            return (a1 && a1->op == IR_STRUCT ? btio_bin_format_b(a0, a1, (long)_.op_ival) : btio_bin_format_a(a0, a1, (long)_.op_ival)) + btio_bin_ports();
        }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "numbervars") == 0 && _.op_ival == 3 && ir_call_arg(pBB,0) && ir_call_arg(pBB,1) && ir_call_arg(pBB,2)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1), *a2 = ir_call_arg(pBB,2);
            return btio_txt_numbervars(a0, a1, a2, hdr);
        }
        if ((strcmp(fn, "term_to_atom") == 0 || strcmp(fn, "term_string") == 0) && ir_call_arg(pBB,0) && ir_call_arg(pBB,1)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1);
            return btio_txt_term_to_atom(a0, a1, hdr);
        }
        if (strcmp(fn, "format") == 0 && ir_call_arg(pBB,0) && (_.op_ival == 1 || _.op_ival == 2)) {
            IR_t *a0 = ir_call_arg(pBB,0); IR_t *a1 = (_.op_ival == 2) ? ir_call_arg(pBB,1) : (IR_t*)0;
            return a1 && a1->op == IR_STRUCT ? btio_txt_format_b(a0, a1, (long)_.op_ival, hdr) : btio_txt_format_a(a0, a1, (long)_.op_ival, hdr);
        }
    }
    return std::string();
}
