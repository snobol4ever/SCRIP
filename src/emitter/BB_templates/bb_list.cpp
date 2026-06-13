#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bls_lbl(IR_t *nd) {
    if (!(nd && nd->op == IR_ATOM && IR_LIT(nd).sval)) return std::string();
    char l[64]; l[0] = 0; strtab_label(l, sizeof l, IR_LIT(nd).sval);
    return std::string(l);
}
static std::string bls_lea(const char *reg, IR_t *nd) {
    if (!(nd && nd->op == IR_ATOM && IR_LIT(nd).sval)) return x86("xor", std::string(reg[1]=='8'?"r8d":"eax"), std::string(reg[1]=='8'?"r8d":"eax"));
    const char *s = IR_LIT(nd).sval;
    char l[64]; l[0] = 0; strtab_label(l, sizeof l, s);
    return x86("lea", reg, "[rip + __]", (uint64_t)(uintptr_t)s, l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bls_bin_tail() { return bytes(4, "\x48\x83\xC4\x10") + bytes(2, "\x85\xC0"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bls_bin_ports() { return x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bls_txt_tail() { return x86("add", "rsp", "16") + x86("test", "eax", "eax") + x86("je", "ω") + x86("jmp", "γ") + x86("def", "β") + x86("jmp", "ω"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bls_bin_alc(IR_t *a0, IR_t *sepN, IR_t *resN, int arity) {
    const char *ssep = (sepN && sepN->op == IR_ATOM) ? IR_LIT(sepN).sval : NULL;
    const char *sres = (resN && resN->op == IR_ATOM) ? IR_LIT(resN).sval : NULL;
    return bytes(4, "\x48\x83\xEC\x10")
         + emit_term_from_node_bin(a0)
         + bytes(3, "\x48\x89\xC7")
         + bytes(1, "\xBE") + u32le((uint32_t)arity)
         + bytes(1, "\xBA") + u32le((uint32_t)(sepN ? (int)sepN->op : 0))
         + bytes(2, "\x48\xB9") + u64le((uint64_t)(sepN ? (long)IR_LIT(sepN).ival : 0))
         + (ssep ? bytes(2, "\x49\xB8") + u64le((uint64_t)(uintptr_t)ssep) : bytes(3, "\x45\x31\xC0"))
         + bytes(2, "\x41\xB9") + u32le((uint32_t)(resN ? (int)resN->op : 0))
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(resN ? (long)IR_LIT(resN).ival : 0))
         + bytes(4, "\x48\x89\x04\x24")
         + (sres ? bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)sres) : bytes(2, "\x31\xC0"))
         + bytes(5, "\x48\x89\x44\x24\x08")
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_atomic_list_concat_term) + bytes(2, "\xFF\xD0")
         + bls_bin_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bls_bin_sort_term(IR_t *a0, IR_t *a1, int do_msort) {
    const char *s1 = (a1->op == IR_ATOM) ? IR_LIT(a1).sval : NULL;
    return bytes(4, "\x48\x83\xEC\x10")
         + emit_term_from_node_bin(a0)
         + bytes(3, "\x48\x89\xC6")
         + bytes(1, "\xBF") + u32le((uint32_t)do_msort)
         + bytes(1, "\xBA") + u32le((uint32_t)(int)a1->op)
         + bytes(2, "\x48\xB9") + u64le((uint64_t)(long)IR_LIT(a1).ival)
         + (s1 ? bytes(2, "\x49\xB8") + u64le((uint64_t)(uintptr_t)s1) : bytes(3, "\x45\x31\xC0"))
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_sort_msort_term) + bytes(2, "\xFF\xD0")
         + bls_bin_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bls_bin_sort_scalar(IR_t *a0, IR_t *a1, int do_msort) {
    const char *s0 = (a0->op == IR_ATOM) ? IR_LIT(a0).sval : NULL;
    const char *s1 = (a1->op == IR_ATOM) ? IR_LIT(a1).sval : NULL;
    return bytes(4, "\x48\x83\xEC\x10")
         + bytes(1, "\xBF") + u32le((uint32_t)do_msort)
         + bytes(1, "\xBE") + u32le((uint32_t)(int)a0->op)
         + bytes(2, "\x48\xBA") + u64le((uint64_t)(long)IR_LIT(a0).ival)
         + (s0 ? bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)s0) : bytes(2, "\x31\xC9"))
         + bytes(2, "\x41\xB8") + u32le((uint32_t)(int)a1->op)
         + bytes(2, "\x49\xB9") + u64le((uint64_t)(long)IR_LIT(a1).ival)
         + (s1 ? bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)s1) : bytes(2, "\x31\xC0"))
         + bytes(4, "\x48\x89\x04\x24")
         + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_sort_msort) + bytes(2, "\xFF\xD0")
         + bls_bin_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bls_txt_alc(IR_t *a0, IR_t *sepN, IR_t *resN, int arity, const std::string &hdr) {
    return hdr
         + x86("sub", "rsp", "16")
         + emit_build_compound_term(a0)
         + x86("mov", "rdi", "rax")
         + x86("mov", "esi", std::to_string(arity))
         + x86("mov", "edx", std::to_string(sepN ? (int)sepN->op : 0))
         + x86("mov", "rcx", std::to_string(sepN ? (long)IR_LIT(sepN).ival : 0))
         + (sepN && sepN->op == IR_ATOM && IR_LIT(sepN).sval ? bls_lea("r8", sepN) : x86("xor", "r8d", "r8d"))
         + x86("mov", "r9d", std::to_string(resN ? (int)resN->op : 0))
         + x86("mov", "rax", std::to_string(resN ? (long)IR_LIT(resN).ival : 0))
         + x86("mov", "[rsp + 0]", "rax")
         + (resN && resN->op == IR_ATOM && IR_LIT(resN).sval ? bls_lea("rax", resN) + x86("mov", "[rsp + 8]", "rax") : x86("mov", "qword ptr [rsp + 8]", "0"))
         + x86("call", "rt_atomic_list_concat_term@PLT")
         + bls_txt_tail();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_list_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if ((strcmp(fn, "atomic_list_concat") == 0 || strcmp(fn, "concat_atom") == 0) && ir_call_arg(pBB,0) && (_.op_ival == 2 || _.op_ival == 3)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1), *a2 = ir_call_arg(pBB,2);
            return x86_lit_bytes(bls_bin_alc(a0, (_.op_ival == 3) ? a1 : NULL, (_.op_ival == 3) ? a2 : a1, (int)_.op_ival)) + bls_bin_ports();
        }
        if ((strcmp(fn, "sort") == 0 || strcmp(fn, "msort") == 0) && ir_call_arg(pBB,0) && ir_call_arg(pBB,1)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1);
            return x86_lit_bytes(IF(a0->op == IR_STRUCT, bls_bin_sort_term(a0, a1, (strcmp(fn, "msort") == 0) ? 1 : 0)) + IF(a0->op != IR_STRUCT, bls_bin_sort_scalar(a0, a1, (strcmp(fn, "msort") == 0) ? 1 : 0))) + bls_bin_ports();
        }
    }
    if (MEDIUM_TEXT) {
        if ((strcmp(fn, "atomic_list_concat") == 0 || strcmp(fn, "concat_atom") == 0) && ir_call_arg(pBB,0) && (_.op_ival == 2 || _.op_ival == 3)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1), *a2 = ir_call_arg(pBB,2);
            return bls_txt_alc(a0, (_.op_ival == 3) ? a1 : NULL, (_.op_ival == 3) ? a2 : a1, (int)_.op_ival, hdr);
        }
        if ((strcmp(fn, "sort") == 0 || strcmp(fn, "msort") == 0) && ir_call_arg(pBB,0) && ir_call_arg(pBB,1)) {
            IR_t *a0 = ir_call_arg(pBB,0), *a1 = ir_call_arg(pBB,1);
            return IF(a0->op == IR_STRUCT,
                      hdr
                    + x86("sub", "rsp", "16")
                    + emit_build_compound_term(a0)
                    + x86("mov", "rsi", "rax")
                    + x86("mov", "edi", std::to_string((strcmp(fn, "msort") == 0) ? 1 : 0))
                    + x86("mov", "edx", std::to_string((int)a1->op))
                    + x86("mov", "rcx", std::to_string((long)IR_LIT(a1).ival))
                    + (a1->op == IR_ATOM && IR_LIT(a1).sval ? bls_lea("r8", a1) : x86("xor", "r8d", "r8d"))
                    + x86("call", "rt_sort_msort_term@PLT")
                    + bls_txt_tail())
                 + IF(a0->op != IR_STRUCT,
                      hdr
                    + x86("sub", "rsp", "16")
                    + x86("mov", "edi", std::to_string((strcmp(fn, "msort") == 0) ? 1 : 0))
                    + x86("mov", "esi", std::to_string((int)a0->op))
                    + x86("mov", "rdx", std::to_string((long)IR_LIT(a0).ival))
                    + (a0->op == IR_ATOM && IR_LIT(a0).sval ? bls_lea("rcx", a0) : x86("xor", "ecx", "ecx"))
                    + x86("mov", "r8d", std::to_string((int)a1->op))
                    + x86("mov", "r9", std::to_string((long)IR_LIT(a1).ival))
                    + (a1->op == IR_ATOM && IR_LIT(a1).sval ? bls_lea("rax", a1) : x86("xor", "eax", "eax"))
                    + x86("mov", "qword ptr [rsp + 0]", "rax")
                    + x86("call", "rt_sort_msort@PLT")
                    + bls_txt_tail());
        }
    }
    return std::string();
}
