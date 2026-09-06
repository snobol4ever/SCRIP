#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../runtime/builtins/gen.h"
DESCR_t rt_num_arith(DESCR_t a, DESCR_t b, int op);
int rt_binop_overload(DESCR_t a, DESCR_t b, int op, DESCR_t *out);
DESCR_t rt_add(DESCR_t a, DESCR_t b);
DESCR_t rt_add_big(DESCR_t a, DESCR_t b);
DESCR_t rt_sub_big(DESCR_t a, DESCR_t b);
DESCR_t rt_mul_big(DESCR_t a, DESCR_t b);
DESCR_t rt_sub(DESCR_t a, DESCR_t b);
DESCR_t rt_mul(DESCR_t a, DESCR_t b);
DESCR_t rt_div(DESCR_t a, DESCR_t b);
DESCR_t rt_mod(DESCR_t a, DESCR_t b);
DESCR_t rt_pow(DESCR_t a, DESCR_t b);
DESCR_t rt_powreal(DESCR_t a, DESCR_t b);
DESCR_t rt_cunion(DESCR_t a, DESCR_t b);
DESCR_t rt_cdiff(DESCR_t a, DESCR_t b);
DESCR_t rt_cinter(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
#include <cstdlib>
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline long long binop_base(long long op) {
    return op == BINOP_ADD_BIG ? BINOP_ADD : op == BINOP_SUB_BIG ? BINOP_SUB : op == BINOP_MUL_BIG ? BINOP_MUL : op;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int binop_promotes(long long op) {
    return op == BINOP_ADD_BIG || op == BINOP_SUB_BIG || op == BINOP_MUL_BIG;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void * rtop_addr(long long op) {
    switch (op) {
        case BINOP_ADD_BIG: return (void*)rt_add_big;
        case BINOP_SUB_BIG: return (void*)rt_sub_big;
        case BINOP_MUL_BIG: return (void*)rt_mul_big;
        case BINOP_ADD:    return (void*)rt_add;
        case BINOP_SUB:    return (void*)rt_sub;
        case BINOP_MUL:    return (void*)rt_mul;
        case BINOP_DIV:    return (void*)rt_div;
        case BINOP_MOD:    return (void*)rt_mod;
        case BINOP_POW:    return (void*)rt_pow;
        case BINOP_POW_PROMOTE: return (void*)rt_powreal;
        case BINOP_CUNION: return (void*)rt_cunion;
        case BINOP_CDIFF:  return (void*)rt_cdiff;
        case BINOP_CINTER: return (void*)rt_cinter;
        default:           return (void*)rt_num_arith;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * rtop_name(long long op) {
    switch (op) {
        case BINOP_ADD_BIG: return "rt_add_big";
        case BINOP_SUB_BIG: return "rt_sub_big";
        case BINOP_MUL_BIG: return "rt_mul_big";
        case BINOP_ADD:    return "rt_add";
        case BINOP_SUB:    return "rt_sub";
        case BINOP_MUL:    return "rt_mul";
        case BINOP_DIV:    return "rt_div";
        case BINOP_MOD:    return "rt_mod";
        case BINOP_POW:    return "rt_pow";
        case BINOP_POW_PROMOTE: return "rt_powreal";
        case BINOP_CUNION: return "rt_cunion";
        case BINOP_CDIFF:  return "rt_cdiff";
        case BINOP_CINTER: return "rt_cinter";
        default:           return "rt_num_arith";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define rtop_is_dyn(op) (rtop_addr(op) == (void*)rt_num_arith)
#define SCRIP_DEF_ARITH_FUSE 1
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define fuse_on() (SCRIP_DEF_ARITH_FUSE)
#define fuse_op_ok() (binop_base((long long)_.op_ival) == BINOP_ADD || binop_base((long long)_.op_ival) == BINOP_SUB || binop_base((long long)_.op_ival) == BINOP_MUL)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define sse_op(xd, xs) x86(binop_base((long long)_.op_ival) == BINOP_SUB ? "subsd" : binop_base((long long)_.op_ival) == BINOP_MUL ? "mulsd" : "addsd", (xd), (xs))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define i2d(xd, src, lb) x86("cvtsi2sd", (xd), (src))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define inl_ok() ( \
       !_.op_num_real && _.op_sa >= 0 && _.op_sb >= 0 && !(_.op_imm_a_ok && _.op_imm_b_ok) \
    && (binop_base((long long)_.op_ival) == BINOP_ADD || binop_base((long long)_.op_ival) == BINOP_SUB || binop_base((long long)_.op_ival) == BINOP_MUL) \
)
#define inl2_ok() (fuse_op_ok() && _.op_sa >= 0 && _.op_sb >= 0 && !(_.op_imm_a_ok && _.op_imm_b_ok))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define inl_tail() ( \
      x86("mov", "rdi", FRQ(_.op_sa)) \
    + x86("mov", "rsi", FRQ(_.op_sa + 8)) \
    + x86("mov", "rdx", FRQ(_.op_sb)) \
    + x86("mov", "rcx", FRQ(_.op_sb + 8)) \
    + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival)) \
    + x86("rtcc_wb") \
    + x86("call_bare", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival)) \
    + x86("rtcc_rl") \
    + x86("cmp", "al", (long)DT_FAIL) \
    + x86_omega("je") \
    + x86("mov", FRQ(_.op_off), "rax") \
    + x86("mov", FRQ(_.op_off + 8), "rdx") \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_arith() {
    if (_.op_zres && fuse_on() && fuse_op_ok()) {
        const int off  = getenv("SCRIP_OPT_BINIMM") && getenv("SCRIP_OPT_BINIMM")[0] == '0';
        const int both = _.op_imm_a_ok && _.op_imm_b_ok;
        const int ia = (both || off) ? 0 : _.op_imm_a_ok, ib = (both || off) ? 0 : _.op_imm_b_ok;
        const int fold_imm = ib && !ia && (binop_base((long long)_.op_ival) == BINOP_ADD || binop_base((long long)_.op_ival) == BINOP_SUB);
        return x86("comment", "IR_BINOP_ARITH zd fuse")
             + x86_alpha()
             + IF(!ia && !ib, x86("note", ZOPN(0))
                            + x86("mov", "eax", ZOPD(0, 0))
                            + x86("note", ZOPN(1))
                            + x86("mov", "ecx", ZOPD(1, 0))
                            + x86("mov", "edx", "eax")
                            + x86("and", "edx", "ecx")
                            + x86("cmp", "dl", (long)DT_I))
             + IF(!ia &&  ib, x86("note", ZOPN(0))
                            + x86("mov", "ecx", ZOPD(0, 0))
                            + x86("note", ZOPN(0))
                            + x86("mov", "rax", ZOPQ(0, 8))
                            + x86("cmp", "cl", (long)DT_I))
             + IF( ia && !ib, x86("note", ZOPN(1))
                            + x86("mov", "eax", ZOPD(1, 0))
                            + x86("note", ZOPN(1))
                            + x86("mov", "rdx", ZOPQ(1, 8))
                            + x86("cmp", "al", (long)DT_I))
             + x86("jne", L(2))
             + IF(!ia && !ib, x86("note", ZOPN(0))
                            + x86("mov", "rax", ZOPQ(0, 8))
                            + x86("note", ZOPN(1))
                            + x86("mov", "rdx", ZOPQ(1, 8)))
             + IF( ia, x86("mov", "rax", (long)_.op_imm_a))
             + IF( ib && !fold_imm, x86("mov", "rdx", (long)_.op_imm_b))
             + IF( fold_imm && binop_base((long long)_.op_ival) == BINOP_ADD, x86("add", "rax", (long)_.op_imm_b))
             + IF( fold_imm && binop_base((long long)_.op_ival) == BINOP_SUB, x86("sub", "rax", (long)_.op_imm_b))
             + IF(!fold_imm && binop_base((long long)_.op_ival) == BINOP_ADD, x86("add",  "rax", "rdx"))
             + IF(!fold_imm && binop_base((long long)_.op_ival) == BINOP_SUB, x86("sub",  "rax", "rdx"))
             + IF(binop_base((long long)_.op_ival) == BINOP_MUL, x86("imul", "rax", "rdx"))
             + IF(binop_promotes((long long)_.op_ival), x86("jo", L(2)))
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), (long)DT_I)
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rax")
             + x86("jmp", L(7))
             + x86("def", L(2))
             + IF(!ia &&  ib, x86("mov", "eax", "ecx")
                            + x86("mov", "edx", "ecx"))
             + IF( ia && !ib, x86("mov", "ecx", "eax")
                            + x86("mov", "edx", "eax"))
             + x86("and", "edx", (long)DT_NUMERIC_BIT)
             + x86("jz", L(0))
             + IF(!ia, x86("note", ZOPN(0))
                     + x86("mov", "rsi", ZOPQ(0, 8)))
             + IF( ia, x86("mov", "rsi", (long)_.op_imm_a))
             + IF(!ib, x86("note", ZOPN(1))
                     + x86("mov", "rdi", ZOPQ(1, 8)))
             + IF( ib, x86("mov", "rdi", (long)_.op_imm_b))
             + IF(!ia, x86("cmp", "al", (long)DT_R)
                     + x86("je", L(3))
                     + i2d("xmm0", "rsi", 8)
                     + x86("jmp", L(4))
                     + x86("def", L(3))
                     + x86("movq", "xmm0", "rsi")
                     + x86("def", L(4)))
             + IF( ia, i2d("xmm0", "rsi", 8))
             + IF(!ib, x86("cmp", "cl", (long)DT_R)
                     + x86("je", L(5))
                     + i2d("xmm1", "rdi", 10)
                     + x86("jmp", L(6))
                     + x86("def", L(5))
                     + x86("movq", "xmm1", "rdi")
                     + x86("def", L(6)))
             + IF( ib, i2d("xmm1", "rdi", 10))
             + sse_op("xmm0", "xmm1")
             + x86("movq", "rax", "xmm0")
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), (long)DT_R)
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rax")
             + x86("def", L(7))
             + x86_gamma()
             + x86("def", L(0))
             + x86("note", ZOPN(0))
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1))
             + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1))
             + x86("mov", "rcx", ZOPQ(1, 8))
             + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
             + x86("rtcc_wb")
             + x86("call_bare", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival))
             + x86("rtcc_rl")
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    }
    if (!_.op_zres && fuse_on() && inl2_ok() && _.op_off >= 0) {
        return x86("comment", "IR_BINOP_ARITH inl fuse")
             + x86_alpha()
             + IF(!_.op_imm_a_ok, x86("mov", "eax", FR(_.op_sa)))
             + IF( _.op_imm_a_ok, x86("mov", "eax", (long)DT_I))
             + IF(!_.op_imm_b_ok, x86("mov", "ecx", FR(_.op_sb)))
             + IF( _.op_imm_b_ok, x86("mov", "ecx", (long)DT_I))
             + x86("mov", "edx", "eax")
             + x86("and", "edx", "ecx")
             + x86("cmp", "dl", (long)DT_I)
             + x86("jne", L(2))
             + IF(!_.op_imm_a_ok, x86("mov", "rax", FRQ(_.op_sa + 8)))
             + IF( _.op_imm_a_ok, x86("mov", "rax", (long)_.op_imm_a))
             + IF(!_.op_imm_b_ok, x86("mov", "rdx", FRQ(_.op_sb + 8)))
             + IF( _.op_imm_b_ok, x86("mov", "rdx", (long)_.op_imm_b))
             + IF(binop_base((long long)_.op_ival) == BINOP_ADD, x86("add",  "rax", "rdx"))
             + IF(binop_base((long long)_.op_ival) == BINOP_SUB, x86("sub",  "rax", "rdx"))
             + IF(binop_base((long long)_.op_ival) == BINOP_MUL, x86("imul", "rax", "rdx"))
             + IF(binop_promotes((long long)_.op_ival), x86("jo", L(2)))
             + x86("mov", FRQ(_.op_off),     (long)DT_I)
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86("jmp", L(7))
             + x86("def", L(2))
             + x86("and", "edx", (long)DT_NUMERIC_BIT)
             + x86("jz", L(0))
             + IF(!_.op_imm_a_ok, x86("mov", "rsi", FRQ(_.op_sa + 8)))
             + IF( _.op_imm_a_ok, x86("mov", "rsi", (long)_.op_imm_a))
             + IF(!_.op_imm_b_ok, x86("mov", "rdi", FRQ(_.op_sb + 8)))
             + IF( _.op_imm_b_ok, x86("mov", "rdi", (long)_.op_imm_b))
             + x86("cmp", "al", (long)DT_R)
             + x86("je", L(3))
             + i2d("xmm0", "rsi", 8)
             + x86("jmp", L(4))
             + x86("def", L(3))
             + x86("movq", "xmm0", "rsi")
             + x86("def", L(4))
             + x86("cmp", "cl", (long)DT_R)
             + x86("je", L(5))
             + i2d("xmm1", "rdi", 10)
             + x86("jmp", L(6))
             + x86("def", L(5))
             + x86("movq", "xmm1", "rdi")
             + x86("def", L(6))
             + sse_op("xmm0", "xmm1")
             + x86("movq", "rax", "xmm0")
             + x86("mov", FRQ(_.op_off),     (long)DT_R)
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86("def", L(7))
             + x86_gamma()
             + x86("def", L(0))
             + inl_tail()
             + x86_gamma()
             + x86_beta_trampoline();
    }
    if (_.op_zres)
        return x86("comment", "IR_BINOP_ARITH zd")
             + x86_alpha()
             + x86("note", ZOPN(0))
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1))
             + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1))
             + x86("mov", "rcx", ZOPQ(1, 8))
             + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
             + x86("rtcc_wb")
             + x86("call_bare", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival))
             + x86("rtcc_rl")
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();
    return IF(_.op_off >= 0 && inl_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH inl")
         + IF(!_.op_imm_a_ok, x86("mov", "eax", FR(_.op_sa))
                            + x86("cmp", "al", (long)DT_I)
                            + x86("jne", L(0)))
         + IF(!_.op_imm_b_ok, x86("mov", "eax", FR(_.op_sb))
                            + x86("cmp", "al", (long)DT_I)
                            + x86("jne", L(0)))
         + IF(!_.op_imm_a_ok, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF( _.op_imm_a_ok, x86("mov", "rax", (long)_.op_imm_a))
         + IF(!_.op_imm_b_ok, x86("mov", "rcx", FRQ(_.op_sb + 8)))
         + IF( _.op_imm_b_ok, x86("mov", "rcx", (long)_.op_imm_b))
         + IF(binop_base((long long)_.op_ival) == BINOP_ADD, x86("add",  "rax", "rcx"))
         + IF(binop_base((long long)_.op_ival) == BINOP_SUB, x86("sub",  "rax", "rcx"))
         + IF(binop_base((long long)_.op_ival) == BINOP_MUL, x86("imul", "rax", "rcx"))
         + IF(binop_promotes((long long)_.op_ival), x86("jo", L(0)))
         + x86("mov", FRQ(_.op_off),     (long)DT_I)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86("def", L(0))
         + inl_tail()
         + x86_gamma()
         + x86_beta_trampoline())
         + IF(_.op_off >= 0 && !inl_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH")
         + inl_tail()
         + x86_gamma()
         + x86_beta_trampoline());
}
