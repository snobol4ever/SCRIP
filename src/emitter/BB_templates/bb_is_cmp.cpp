#include "bb_common.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icm_arith(const char *fn) {
    return strcmp(fn,"=:=")==0 || strcmp(fn,"=\\=")==0 || strcmp(fn,"<")==0
        || strcmp(fn,">")==0   || strcmp(fn,"=<")==0   || strcmp(fn,"<=")==0 || strcmp(fn,">=")==0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icm_ord(const char *fn) {
    return strcmp(fn,"==")==0 || strcmp(fn,"\\==")==0 || strcmp(fn,"@<")==0 || strcmp(fn,"@>")==0
        || strcmp(fn,"@=<")==0 || strcmp(fn,"@>=")==0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icm_cmp(const char *fn) { return icm_ord(fn) || icm_arith(fn); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string icm_op() {
    return _.bb_op_lbl ? x86("lea", "rsi", std::string("[rip + ") + _.bb_op_lbl + "]")
                       : x86("xor", "esi", "esi");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string icm_tail() {
    return x86("test", "eax", "eax")
         + x86("je",   _.lbl_ω)
         + x86("jmp",  _.lbl_γ)
         + x86("def", "β") + x86("jmp", "ω");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string icm_arg_load(const char *reg, const IR_t *n) {
    if (!n || n->op != IR_LOGICVAR) return x86("xor", reg, reg);
    return x86("lea", reg, FRQ(GZ_CELL_OFF((int)IR_LIT(n).ival)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string icm_arg_load_lit(const char *reg, const IR_t *n) {
    if (!n) return x86("xor", reg, reg);
    if (n->op == IR_LOGICVAR) return x86("lea", reg, FRQ(GZ_CELL_OFF((int)IR_LIT(n).ival)));
    if (n->op == IR_LIT_I)    return x86("mov", reg, (long)IR_LIT(n).ival);
    return x86("xor", reg, reg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icm_k(const IR_t *n) { return n ? (int)n->op : -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long icm_i(const IR_t *n) { return n ? (long)IR_LIT(n).ival : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t icm_fb(const IR_t *n) {
    union { double d; uint64_t u; } v = { (n && n->op == IR_LIT_F) ? IR_LIT(n).dval : 0.0 };
    return v.u;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icm_floaty(const IR_t *rhs) {
    if (rhs->op == IR_ATOM && IR_LIT(rhs).sval && (!strcmp(IR_LIT(rhs).sval, "pi") || !strcmp(IR_LIT(rhs).sval, "e"))) return 1;
    if (rhs->op != IR_ARITH) return 0;
    return bb_op_floaty(IR_LIT(rhs).sval ? IR_LIT(rhs).sval : "+")
        || (ir_pair_arg(rhs,0) && ir_pair_arg(rhs,0)->op == IR_LIT_F)
        || (ir_pair_arg(rhs,1) && ir_pair_arg(rhs,1)->op == IR_LIT_F);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string icm_lea_disp(unsigned char rex_prefix, unsigned char modrm8, unsigned char modrm32, int off) {
    if (off >= -128 && off < 128)
        return std::string(1,(char)rex_prefix) + "\x8D" + std::string(1,(char)modrm8) + "\x24" + std::string(1,(char)(unsigned char)off);
    char buf[4]; buf[0]=(char)(off&0xFF); buf[1]=(char)((off>>8)&0xFF); buf[2]=(char)((off>>16)&0xFF); buf[3]=(char)((off>>24)&0xFF);
    return std::string(1,(char)rex_prefix) + "\x8D" + std::string(1,(char)modrm32) + "\x24" + std::string(buf,4);
}
static std::string icm_bin_arg(const char *reg64, const char *reg32, const IR_t *n) {
    (void)reg32;
    if (!n || (n->op != IR_LOGICVAR && n->op != IR_LIT_I))
        return (strcmp(reg64,"r9")==0 ? bytes(3,"\x45\x31\xC9") : bytes(2,"\x31\xC9"));
    if (n->op == IR_LOGICVAR) {
        int off = GZ_CELL_OFF((int)IR_LIT(n).ival);
        if (strcmp(reg64,"rcx")==0) return icm_lea_disp(0x49,0x4C,0x8C,off);
        if (strcmp(reg64,"r9") ==0) return icm_lea_disp(0x4D,0x4C,0x8C,off);
        return icm_lea_disp(0x49,0x7C,0xBC,off);
    }
    return bytes(2,"\x48\xB9") + u64le((uint64_t)(long)IR_LIT(n).ival);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string icm_bin_is_cell(IR_t *pBB) {
    IR_t *lhs = ir_pair_arg(pBB,0);
    IR_t *rhs = ir_pair_arg(pBB,1);
    if (!lhs || !rhs) return std::string();
    int dst_off = GZ_CELL_OFF((int)IR_LIT(lhs).ival);
    const IR_t *la = (rhs->op == IR_ARITH) ? ir_pair_arg(rhs,0) : NULL;
    const IR_t *rb = (rhs->op == IR_ARITH) ? ir_pair_arg(rhs,1) : NULL;
    const char *op_sval = IR_LIT(rhs).sval;
    int  lk  = la ? (int)la->op : -1;
    int  rk  = rb ? (int)rb->op : -1;
    uint64_t ld_bits = (la && la->op == IR_LIT_F) ? icm_fb(la) : 0ULL;
    uint64_t rd_bits = (rb && rb->op == IR_LIT_F) ? icm_fb(rb) : 0ULL;
    return bytes(4, "\x48\x83\xEC\x10")
        + icm_lea_disp(0x49,0x7C,0xBC,dst_off)
        + bytes(2, "\x48\xBE") + u64le((uint64_t)(uintptr_t)op_sval)
        + bytes(1, "\xBA") + u32le((uint32_t)(int)lk)
        + (la && la->op == IR_LOGICVAR ? icm_lea_disp(0x49,0x4C,0x8C,GZ_CELL_OFF((int)IR_LIT(la).ival))
           : la && la->op == IR_LIT_I  ? bytes(2,"\x48\xB9") + u64le((uint64_t)(long)IR_LIT(la).ival)
           : bytes(2, "\x31\xC9"))
        + bytes(2, "\x48\xB8") + u64le(ld_bits)
        + bytes(5, "\x66\x48\x0F\x6E\xC0")
        + bytes(2, "\x41\xB8") + u32le((uint32_t)(int)rk)
        + (rb && rb->op == IR_LOGICVAR ? icm_lea_disp(0x4D,0x4C,0x8C,GZ_CELL_OFF((int)IR_LIT(rb).ival))
           : rb && rb->op == IR_LIT_I  ? bytes(2,"\x49\xB9") + u64le((uint64_t)(long)IR_LIT(rb).ival)
           : bytes(3, "\x45\x31\xC9"))
        + bytes(2, "\x48\xB8") + u64le(rd_bits)
        + bytes(5, "\x66\x48\x0F\x6E\xC8")
        + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_is_cell) + bytes(2, "\xFF\xD0")
        + bytes(4, "\x48\x83\xC4\x10")
        + bytes(2, "\x85\xC0");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_is_cmp_str(IR_t *pBB, const char *fn, const std::string &hdr) {
    (void)pBB; (void)fn; (void)hdr;
    if (MEDIUM_BINARY) {
        if (strcmp(fn, "is") == 0 && ir_pair_arg(pBB,0) && ir_pair_arg(pBB,0)->op == IR_LOGICVAR && ir_pair_arg(pBB,1)
            && (ir_pair_arg(pBB,1)->op == IR_ARITH || ir_pair_arg(pBB,1)->op == IR_LIT_I || ir_pair_arg(pBB,1)->op == IR_LIT_F
                || ir_pair_arg(pBB,1)->op == IR_LOGICVAR || ir_pair_arg(pBB,1)->op == IR_ATOM))
            return x86_lit_bytes(icm_bin_is_cell(pBB)) + x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω");
        if (ir_pair_arg(pBB,0) && ir_pair_arg(pBB,1) && icm_cmp(fn)) {
            IR_t *a0 = ir_pair_arg(pBB,0), *a1 = ir_pair_arg(pBB,1);
            return x86_lit_bytes(
                bytes(4, "\x48\x83\xEC\x10")
                + bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)fn)
                + bytes(1, "\xBE") + u32le((uint32_t)(int)a0->op)
                + bytes(2, "\x48\xBA") + u64le((uint64_t)(long)IR_LIT(a0).ival)
                + ((a0->op == IR_ATOM && IR_LIT(a0).sval) ? bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)IR_LIT(a0).sval)
                                                  : bytes(2, "\x31\xC9"))
                + bytes(2, "\x41\xB8") + u32le((uint32_t)(int)a1->op)
                + bytes(2, "\x49\xB9") + u64le((uint64_t)(long)IR_LIT(a1).ival)
                + ((a1->op == IR_ATOM && IR_LIT(a1).sval) ? bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)IR_LIT(a1).sval)
                                                  : bytes(2, "\x31\xC0"))
                + bytes(4, "\x48\x89\x04\x24")
                + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(icm_arith(fn) ? (void*)rt_arith_cmp : (void*)rt_term_cmp)) + bytes(2, "\xFF\xD0")
                + bytes(4, "\x48\x83\xC4\x10")
                + bytes(2, "\x85\xC0")
            ) + x86("je", "ω") + x86("jmp", "γ") + x86("jmp", "ω");
        }
    }
    if (MEDIUM_TEXT) {
        if (strcmp(fn, "is") == 0 && ir_pair_arg(pBB,0) && ir_pair_arg(pBB,0)->op == IR_LOGICVAR && ir_pair_arg(pBB,1) && icm_floaty(ir_pair_arg(pBB,1))) {
            const IR_t *rhs = ir_pair_arg(pBB,1);
            const IR_t *la = (rhs->op == IR_ARITH) ? ir_pair_arg(rhs,0) : NULL;
            const IR_t *rb = (rhs->op == IR_ARITH) ? ir_pair_arg(rhs,1) : NULL;
            int dst_slot = (int)IR_LIT(ir_pair_arg(pBB,0)).ival;
            return hdr
                 + x86("sub", "rsp", "8")
                 + x86("lea", "rdi", FRQ(GZ_CELL_OFF(dst_slot)))
                 + icm_op()
                 + x86("mov", "edx", std::to_string(la ? (int)la->op : -1))
                 + icm_arg_load_lit("rcx", la)
                 + x86("mov", "rax", std::to_string((unsigned long long)((la && la->op == IR_LIT_F) ? icm_fb(la) : 0)))
                 + x86("movq", "xmm0", "rax")
                 + x86("mov", "r8d", std::to_string(rb ? (int)rb->op : -1))
                 + icm_arg_load_lit("r9", rb)
                 + x86("mov", "rax", std::to_string((unsigned long long)((rb && rb->op == IR_LIT_F) ? icm_fb(rb) : 0)))
                 + x86("movq", "xmm1", "rax")
                 + x86("call", "rt_is_cell@PLT")
                 + x86("add", "rsp", "8")
                 + icm_tail();
        }
        /* LOGICVAR dst, binary IR_ARITH, non-floaty (floaty caught above) — both operands present.
         * Uses rt_is_cell: rdi = frame-cell ptr for dst slot, rcx/r9 = frame-cell ptr (LOGICVAR) or value (LIT_I). */
        if (strcmp(fn, "is") == 0 && ir_pair_arg(pBB,0) && ir_pair_arg(pBB,1) && ir_pair_arg(pBB,1)->op == IR_ARITH
            && ir_pair_arg(pBB,0)->op == IR_LOGICVAR && ir_pair_arg(ir_pair_arg(pBB,1),0) && ir_pair_arg(ir_pair_arg(pBB,1),1)) {
            const IR_t *ra = ir_pair_arg(ir_pair_arg(pBB,1),0), *rb2 = ir_pair_arg(ir_pair_arg(pBB,1),1);
            int dst_slot = (int)IR_LIT(ir_pair_arg(pBB,0)).ival;
            return hdr
                 + x86("sub", "rsp", "8")
                 + x86("lea", "rdi", FRQ(GZ_CELL_OFF(dst_slot)))
                 + icm_op()
                 + x86("mov", "edx", std::to_string((int)ra->op))
                 + icm_arg_load_lit("rcx", ra)
                 + x86("mov", "rax", "0")
                 + x86("movq", "xmm0", "rax")
                 + x86("mov", "r8d", std::to_string((int)rb2->op))
                 + icm_arg_load_lit("r9", rb2)
                 + x86("mov", "rax", "0")
                 + x86("movq", "xmm1", "rax")
                 + x86("call", "rt_is_cell@PLT")
                 + x86("add", "rsp", "8")
                 + icm_tail();
        }
        /* IR_LIT_I dst, binary IR_ARITH — "N is Expr" where N is a literal integer.
         * Uses rt_is_cell_lit: rdi = lval (the literal long), same operand marshalling as above. */
        if (strcmp(fn, "is") == 0 && ir_pair_arg(pBB,0) && ir_pair_arg(pBB,1) && ir_pair_arg(pBB,1)->op == IR_ARITH
            && ir_pair_arg(pBB,0)->op == IR_LIT_I && ir_pair_arg(ir_pair_arg(pBB,1),0) && ir_pair_arg(ir_pair_arg(pBB,1),1)) {
            const IR_t *ra = ir_pair_arg(ir_pair_arg(pBB,1),0), *rb2 = ir_pair_arg(ir_pair_arg(pBB,1),1);
            return hdr
                 + x86("sub", "rsp", "8")
                 + x86("mov", "rdi", std::to_string((long)IR_LIT(ir_pair_arg(pBB,0)).ival))
                 + icm_op()
                 + x86("mov", "edx", std::to_string((int)ra->op))
                 + icm_arg_load_lit("rcx", ra)
                 + x86("mov", "rax", "0")
                 + x86("movq", "xmm0", "rax")
                 + x86("mov", "r8d", std::to_string((int)rb2->op))
                 + icm_arg_load_lit("r9", rb2)
                 + x86("mov", "rax", "0")
                 + x86("movq", "xmm1", "rax")
                 + x86("call", "rt_is_cell_lit@PLT")
                 + x86("add", "rsp", "8")
                 + icm_tail();
        }
        /* LOGICVAR dst, unary IR_ARITH (one operand, no rhs) — "X is f(A)" e.g. X is sign(N).
         * Uses rt_is_cell: rdi = frame-cell ptr, r8d = -1 (no rhs), r9 = 0 (NULL rarg), xmm1 = 0. */
        if (strcmp(fn, "is") == 0 && ir_pair_arg(pBB,0) && ir_pair_arg(pBB,1) && ir_pair_arg(pBB,1)->op == IR_ARITH
            && ir_pair_arg(pBB,0)->op == IR_LOGICVAR && ir_pair_arg(ir_pair_arg(pBB,1),0) && !ir_pair_arg(ir_pair_arg(pBB,1),1)) {
            const IR_t *ra = ir_pair_arg(ir_pair_arg(pBB,1),0);
            int dst_slot = (int)IR_LIT(ir_pair_arg(pBB,0)).ival;
            return hdr
                 + x86("sub", "rsp", "8")
                 + x86("lea", "rdi", FRQ(GZ_CELL_OFF(dst_slot)))
                 + icm_op()
                 + x86("mov", "edx", std::to_string((int)ra->op))
                 + icm_arg_load_lit("rcx", ra)
                 + x86("mov", "rax", "0")
                 + x86("movq", "xmm0", "rax")
                 + x86("mov", "r8d", "-1")
                 + x86("xor", "r9d", "r9d")
                 + x86("mov", "rax", "0")
                 + x86("movq", "xmm1", "rax")
                 + x86("call", "rt_is_cell@PLT")
                 + x86("add", "rsp", "8")
                 + icm_tail();
        }
        if (ir_pair_arg(pBB,0) && ir_pair_arg(pBB,1) && (ir_pair_arg(pBB,0)->op == IR_STRUCT || ir_pair_arg(pBB,1)->op == IR_STRUCT) && icm_ord(fn)) {
            char op_lbl[64]; strtab_label(op_lbl, sizeof op_lbl, fn);
            return hdr
                 + x86("sub", "rsp", "16")
                 + emit_build_compound_term(ir_pair_arg(pBB,0))
                 + x86("mov", "qword ptr [rsp + 0]", "rax")
                 + emit_build_compound_term(ir_pair_arg(pBB,1))
                 + x86("mov", "rdx", "rax")
                 + x86("mov", "rsi", "qword ptr [rsp + 0]")
                 + x86("lea", "rdi", std::string("[rip + ") + op_lbl + "]")
                 + x86("call", "rt_term_cmp_terms@PLT")
                 + x86("add", "rsp", "16")
                 + icm_tail();
        }
        if (ir_pair_arg(pBB,0) && ir_pair_arg(pBB,1) && icm_cmp(fn)) {
            IR_t *a0 = ir_pair_arg(pBB,0), *a1 = ir_pair_arg(pBB,1);
            char op_lbl[64]; strtab_label(op_lbl, sizeof op_lbl, fn);
            char s0lbl[64]; s0lbl[0] = 0;
            char s1lbl[64]; s1lbl[0] = 0;
            if (a0->op == IR_ATOM && IR_LIT(a0).sval) strtab_label(s0lbl, sizeof s0lbl, IR_LIT(a0).sval);
            if (a1->op == IR_ATOM && IR_LIT(a1).sval) strtab_label(s1lbl, sizeof s1lbl, IR_LIT(a1).sval);
            return hdr
                 + x86("sub", "rsp", "16")
                 + x86("lea", "rdi", std::string("[rip + ") + op_lbl + "]")
                 + x86("mov", "esi", std::to_string((int)a0->op))
                 + x86("mov", "rdx", std::to_string((long)IR_LIT(a0).ival))
                 + (s0lbl[0] ? x86("lea", "rcx", std::string("[rip + ") + s0lbl + "]") : x86("xor", "ecx", "ecx"))
                 + x86("mov", "r8d", std::to_string((int)a1->op))
                 + x86("mov", "r9", std::to_string((long)IR_LIT(a1).ival))
                 + (s1lbl[0] ? x86("lea", "rax", std::string("[rip + ") + s1lbl + "]") : x86("xor", "eax", "eax"))
                 + x86("mov", "qword ptr [rsp + 0]", "rax")
                 + x86("call", icm_arith(fn) ? "rt_arith_cmp@PLT" : "rt_term_cmp@PLT")
                 + x86("add", "rsp", "16")
                 + icm_tail();
        }
    }
    return std::string();
}
