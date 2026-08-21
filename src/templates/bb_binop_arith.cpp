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
DESCR_t rt_sub(DESCR_t a, DESCR_t b);
DESCR_t rt_mul(DESCR_t a, DESCR_t b);
DESCR_t rt_div(DESCR_t a, DESCR_t b);
DESCR_t rt_mod(DESCR_t a, DESCR_t b);
DESCR_t rt_pow(DESCR_t a, DESCR_t b);
DESCR_t rt_cunion(DESCR_t a, DESCR_t b);
DESCR_t rt_cdiff(DESCR_t a, DESCR_t b);
DESCR_t rt_cinter(DESCR_t a, DESCR_t b);
}
#include "x86_asm.h"
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void * rtop_addr(long long op) {
    switch (op) {
        case BINOP_ADD:    return (void*)rt_add;
        case BINOP_SUB:    return (void*)rt_sub;
        case BINOP_MUL:    return (void*)rt_mul;
        case BINOP_DIV:    return (void*)rt_div;
        case BINOP_MOD:    return (void*)rt_mod;
        case BINOP_POW:    return (void*)rt_pow;
        case BINOP_CUNION: return (void*)rt_cunion;
        case BINOP_CDIFF:  return (void*)rt_cdiff;
        case BINOP_CINTER: return (void*)rt_cinter;
        default:           return (void*)rt_num_arith;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * rtop_name(long long op) {
    switch (op) {
        case BINOP_ADD:    return "rt_add";
        case BINOP_SUB:    return "rt_sub";
        case BINOP_MUL:    return "rt_mul";
        case BINOP_DIV:    return "rt_div";
        case BINOP_MOD:    return "rt_mod";
        case BINOP_POW:    return "rt_pow";
        case BINOP_CUNION: return "rt_cunion";
        case BINOP_CDIFF:  return "rt_cdiff";
        case BINOP_CINTER: return "rt_cinter";
        default:           return "rt_num_arith";
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int rtop_is_dyn(long long op) { return rtop_addr(op) == (void*)rt_num_arith; }
#define SCRIP_DEF_ARITH_FUSE 1
#define SCRIP_DEF_I2D_MAGIC  0
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int fuse_on() { return SCRIP_DEF_ARITH_FUSE; }
static inline int fuse_op_ok() { long long o = (long long)_.op_ival; return o == BINOP_ADD || o == BINOP_SUB || o == BINOP_MUL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string sse_op(const char * xd, const char * xs) {
    long long o = (long long)_.op_ival;
    return x86(o == BINOP_SUB ? "subsd" : o == BINOP_MUL ? "mulsd" : "addsd", xd, xs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string i2d(const char * xd, const char * src, int lb) {
#if SCRIP_DEF_I2D_MAGIC
    return x86("movabs", "rdx", (uint64_t)0xFFF0000000000000ULL)
         + x86("test", src, "rdx")
         + x86("jnz", L(lb))
         + x86("movabs", "rdx", (uint64_t)0x4330000000000000ULL)
         + x86("or", src, "rdx")
         + x86("movq", xd, src)
         + x86("movq", "xmm2", "rdx")
         + x86("subsd", xd, "xmm2")
         + x86("jmp", L(lb + 1))
         + x86("def", L(lb))
         + x86("cvtsi2sd", xd, src)
         + x86("def", L(lb + 1));
#else
    (void)lb; return x86("cvtsi2sd", xd, src);
#endif
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int inl_ok() { long long o = (long long)_.op_ival; return !_.op_num_real && _.op_sa >= 0 && _.op_sb >= 0 && !(_.op_imm_a_ok && _.op_imm_b_ok) && (o == BINOP_ADD || o == BINOP_SUB || o == BINOP_MUL); }
static inline int inl2_ok() { return fuse_op_ok() && _.op_sa >= 0 && _.op_sb >= 0 && !(_.op_imm_a_ok && _.op_imm_b_ok); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string fc_tail() {
    return x86("mov", "rdi", ZTOS(16)) + x86("mov", "rsi", ZTOS(24)) + x86("mov", "rdx", ZTOS(0)) + x86("mov", "rcx", ZTOS(8))
         + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
         + x86("rtcc_wb")
         + x86("call_bare", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival))
         + x86("cmp", "eax", (long)DT_FAIL) + x86_omega("je")
         + x86_zrelease(16) + x86("mov", ZTOS(0), "rax") + x86("mov", ZTOS(8), "rdx")
         + x86("rtcc_rl");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string inl_tail() {
    return x86("mov", "rdi", FRQ(_.op_sa)) + x86("mov", "rsi", FRQ(_.op_sa + 8)) + x86("mov", "rdx", FRQ(_.op_sb)) + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
         + x86("rtcc_wb")
         + x86("call_bare", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival))
         + x86("cmp", "eax", (long)DT_FAIL) + x86_omega("je")
         + x86("mov", FRQ(_.op_off), "rax") + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86("rtcc_rl");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_arith() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres && fuse_on() && fuse_op_ok()) {
        return x86("comment", "IR_BINOP_ARITH zd fuse")
             + x86_alpha()
             + x86("note", ZOPN(0)) + x86("mov", "eax", ZOPD(0, 0))
             + x86("note", ZOPN(1)) + x86("mov", "ecx", ZOPD(1, 0))
             + x86("mov", "edx", "eax")
             + x86("and", "edx", "ecx")
             + x86("cmp", "edx", (long)DT_I)
             + x86("jne", L(2))
             + x86("note", ZOPN(0)) + x86("mov", "rax", ZOPQ(0, 8))
             + x86("note", ZOPN(1)) + x86("mov", "rdx", ZOPQ(1, 8))
             + IF((long long)_.op_ival == BINOP_ADD, x86("add",  "rax", "rdx"))
             + IF((long long)_.op_ival == BINOP_SUB, x86("sub",  "rax", "rdx"))
             + IF((long long)_.op_ival == BINOP_MUL, x86("imul", "rax", "rdx"))
             + x86("note", ZRESN()) + x86("mov", ZRES(0), (long)DT_I)
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rax")
             + x86("jmp", L(7))
             + x86("def", L(2))
             + x86("and", "edx", (long)DT_NUMERIC_BIT)
             + x86("jz", L(0))
             + x86("note", ZOPN(0)) + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1)) + x86("mov", "rdi", ZOPQ(1, 8))
             + x86("cmp", "eax", (long)DT_R)
             + x86("je", L(3))
             + i2d("xmm0", "rsi", 8)
             + x86("jmp", L(4))
             + x86("def", L(3))
             + x86("movq", "xmm0", "rsi")
             + x86("def", L(4))
             + x86("cmp", "ecx", (long)DT_R)
             + x86("je", L(5))
             + i2d("xmm1", "rdi", 10)
             + x86("jmp", L(6))
             + x86("def", L(5))
             + x86("movq", "xmm1", "rdi")
             + x86("def", L(6))
             + sse_op("xmm0", "xmm1")
             + x86("movq", "rax", "xmm0")
             + x86("note", ZRESN()) + x86("mov", ZRES(0), (long)DT_R)
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rax")
             + x86("def", L(7))
             + x86_gamma()
             + x86("def", L(0))
             + x86("note", ZOPN(0)) + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0)) + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1)) + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1)) + x86("mov", "rcx", ZOPQ(1, 8))
             + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
             + x86("rtcc_wb")
             + x86("call_bare", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival))
             + x86("cmp", "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86("rtcc_rl")
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
             + x86("cmp", "edx", (long)DT_I)
             + x86("jne", L(2))
             + IF(!_.op_imm_a_ok, x86("mov", "rax", FRQ(_.op_sa + 8)))
             + IF( _.op_imm_a_ok, x86("mov", "rax", (long)_.op_imm_a))
             + IF(!_.op_imm_b_ok, x86("mov", "rdx", FRQ(_.op_sb + 8)))
             + IF( _.op_imm_b_ok, x86("mov", "rdx", (long)_.op_imm_b))
             + IF((long long)_.op_ival == BINOP_ADD, x86("add",  "rax", "rdx"))
             + IF((long long)_.op_ival == BINOP_SUB, x86("sub",  "rax", "rdx"))
             + IF((long long)_.op_ival == BINOP_MUL, x86("imul", "rax", "rdx"))
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
             + x86("cmp", "eax", (long)DT_R)
             + x86("je", L(3))
             + i2d("xmm0", "rsi", 8)
             + x86("jmp", L(4))
             + x86("def", L(3))
             + x86("movq", "xmm0", "rsi")
             + x86("def", L(4))
             + x86("cmp", "ecx", (long)DT_R)
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
             + x86("note", ZOPN(0)) + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0)) + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1)) + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1)) + x86("mov", "rcx", ZOPQ(1, 8))
             + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
             + x86("rtcc_wb")
             + x86("call_bare", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival))
             + x86("cmp", "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86("rtcc_rl")
             + x86_gamma()
             + x86_beta_trampoline();
    return IF(_.op_off >= 0 && inl_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH inl")
         + IF(!_.op_imm_a_ok, x86("mov", "eax", FR(_.op_sa)) + x86("cmp", "eax", (long)DT_I) + x86("jne", L(0)))
         + IF(!_.op_imm_b_ok, x86("mov", "eax", FR(_.op_sb)) + x86("cmp", "eax", (long)DT_I) + x86("jne", L(0)))
         + IF(!_.op_imm_a_ok, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF( _.op_imm_a_ok, x86("mov", "rax", (long)_.op_imm_a))
         + IF(!_.op_imm_b_ok, x86("mov", "rcx", FRQ(_.op_sb + 8)))
         + IF( _.op_imm_b_ok, x86("mov", "rcx", (long)_.op_imm_b))
         + IF((long long)_.op_ival == BINOP_ADD, x86("add",  "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_SUB, x86("sub",  "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_MUL, x86("imul", "rax", "rcx"))
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
