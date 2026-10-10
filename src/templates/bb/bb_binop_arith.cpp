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
DESCR_t rt_div_strict(DESCR_t, DESCR_t);
DESCR_t rt_mod_strict(DESCR_t, DESCR_t);
DESCR_t rt_pow_strict(DESCR_t, DESCR_t);
DESCR_t rt_powreal_strict(DESCR_t, DESCR_t);
DESCR_t rt_cunion_strict(DESCR_t, DESCR_t);
DESCR_t rt_cdiff_strict(DESCR_t, DESCR_t);
DESCR_t rt_cinter_strict(DESCR_t, DESCR_t);
DESCR_t rt_num_arith_strict(DESCR_t, DESCR_t, int);
DESCR_t rt_add_sno(DESCR_t, DESCR_t);
DESCR_t rt_sub_sno(DESCR_t, DESCR_t);
DESCR_t rt_mul_sno(DESCR_t, DESCR_t);
DESCR_t rt_div_sno(DESCR_t, DESCR_t);
DESCR_t rt_mod_sno(DESCR_t, DESCR_t);
DESCR_t rt_pow_sno(DESCR_t, DESCR_t);
DESCR_t rt_powreal_sno(DESCR_t, DESCR_t);
DESCR_t rt_num_arith_sno(DESCR_t, DESCR_t, int);
DESCR_t c_rt_add(DESCR_t, DESCR_t);
DESCR_t c_rt_sub(DESCR_t, DESCR_t);
DESCR_t c_rt_mul(DESCR_t, DESCR_t);
DESCR_t c_rt_add_sno(DESCR_t, DESCR_t);
DESCR_t c_rt_sub_sno(DESCR_t, DESCR_t);
DESCR_t c_rt_mul_sno(DESCR_t, DESCR_t);
}
#include "x86_asm.h"
#include <cstdlib>
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define binop_base(op) ((op) == BINOP_ADD_BIG ? BINOP_ADD : (op) == BINOP_SUB_BIG ? BINOP_SUB : (op) == BINOP_MUL_BIG ? BINOP_MUL : (op))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define binop_promotes(op) ((op) == BINOP_ADD_BIG || (op) == BINOP_SUB_BIG || (op) == BINOP_MUL_BIG)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const struct { long long op; int col; const char * name; void * addr; const char * cname; void * caddr; } rtop_tab[] = {
    { BINOP_ADD_BIG, 0, "rt_add_big", (void*)rt_add_big },
    { BINOP_ADD_BIG, 1, "rt_add_big", (void*)rt_add_big },
    { BINOP_ADD_BIG, 2, "rt_add_big", (void*)rt_add_big },
    { BINOP_SUB_BIG, 0, "rt_sub_big", (void*)rt_sub_big },
    { BINOP_SUB_BIG, 1, "rt_sub_big", (void*)rt_sub_big },
    { BINOP_SUB_BIG, 2, "rt_sub_big", (void*)rt_sub_big },
    { BINOP_MUL_BIG, 0, "rt_mul_big", (void*)rt_mul_big },
    { BINOP_MUL_BIG, 1, "rt_mul_big", (void*)rt_mul_big },
    { BINOP_MUL_BIG, 2, "rt_mul_big", (void*)rt_mul_big },
    { BINOP_ADD, 0, "rt_add", (void*)rt_add, "c_rt_add", (void*)c_rt_add },
    { BINOP_ADD, 1, "rt_add", (void*)rt_add, "c_rt_add", (void*)c_rt_add },
    { BINOP_ADD, 2, "rt_add_sno", (void*)rt_add_sno, "c_rt_add_sno", (void*)c_rt_add_sno },
    { BINOP_SUB, 0, "rt_sub", (void*)rt_sub, "c_rt_sub", (void*)c_rt_sub },
    { BINOP_SUB, 1, "rt_sub", (void*)rt_sub, "c_rt_sub", (void*)c_rt_sub },
    { BINOP_SUB, 2, "rt_sub_sno", (void*)rt_sub_sno, "c_rt_sub_sno", (void*)c_rt_sub_sno },
    { BINOP_MUL, 0, "rt_mul", (void*)rt_mul, "c_rt_mul", (void*)c_rt_mul },
    { BINOP_MUL, 1, "rt_mul", (void*)rt_mul, "c_rt_mul", (void*)c_rt_mul },
    { BINOP_MUL, 2, "rt_mul_sno", (void*)rt_mul_sno, "c_rt_mul_sno", (void*)c_rt_mul_sno },
    { BINOP_DIV, 0, "rt_div", (void*)rt_div },
    { BINOP_DIV, 1, "rt_div_strict", (void*)rt_div_strict },
    { BINOP_DIV, 2, "rt_div_sno", (void*)rt_div_sno },
    { BINOP_MOD, 0, "rt_mod", (void*)rt_mod },
    { BINOP_MOD, 1, "rt_mod_strict", (void*)rt_mod_strict },
    { BINOP_MOD, 2, "rt_mod_sno", (void*)rt_mod_sno },
    { BINOP_POW, 0, "rt_pow", (void*)rt_pow },
    { BINOP_POW, 1, "rt_pow_strict", (void*)rt_pow_strict },
    { BINOP_POW, 2, "rt_pow_sno", (void*)rt_pow_sno },
    { BINOP_POW_PROMOTE, 0, "rt_powreal", (void*)rt_powreal },
    { BINOP_POW_PROMOTE, 1, "rt_powreal_strict", (void*)rt_powreal_strict },
    { BINOP_POW_PROMOTE, 2, "rt_powreal_sno", (void*)rt_powreal_sno },
    { BINOP_CUNION, 0, "rt_cunion", (void*)rt_cunion },
    { BINOP_CUNION, 1, "rt_cunion_strict", (void*)rt_cunion_strict },
    { BINOP_CUNION, 2, "rt_cunion", (void*)rt_cunion },
    { BINOP_CDIFF, 0, "rt_cdiff", (void*)rt_cdiff },
    { BINOP_CDIFF, 1, "rt_cdiff_strict", (void*)rt_cdiff_strict },
    { BINOP_CDIFF, 2, "rt_cdiff", (void*)rt_cdiff },
    { BINOP_CINTER, 0, "rt_cinter", (void*)rt_cinter },
    { BINOP_CINTER, 1, "rt_cinter_strict", (void*)rt_cinter_strict },
    { BINOP_CINTER, 2, "rt_cinter", (void*)rt_cinter },
    { -1, 0, "rt_num_arith", (void*)rt_num_arith },
    { -1, 1, "rt_num_arith_strict", (void*)rt_num_arith_strict },
    { -1, 2, "rt_num_arith_sno", (void*)rt_num_arith_sno },
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int rtop_row(long long op, int strict, int i) {
    return (rtop_tab[i].op == op || rtop_tab[i].op == -1) && rtop_tab[i].col == (strict == 2 ? 2 : strict != 0) ? i : rtop_row(op, strict, i + 1);
}
#define rtop_addr_s(op, strict) (rtop_tab[rtop_row((op), (strict), 0)].addr)
#define rtop_is_dyn(op) (rtop_addr_s((op), 0) == (void*)rt_num_arith)
#define RTOP_I() rtop_row(_.op_ival, _.op_strict, 0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define rtop_call_leaf() ( \
      x86("call", rtop_tab[RTOP_I()].name, (uint64_t)(uintptr_t)rtop_tab[RTOP_I()].addr) \
    + IF(rtop_tab[RTOP_I()].cname, x86("cmp", "al", (long)RTX_NOT_HANDLED) \
                     + x86("jne", L(11)) \
                     + x86("call", rtop_tab[RTOP_I()].cname, (uint64_t)(uintptr_t)rtop_tab[RTOP_I()].caddr) \
                     + x86("def", L(11))) \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define rtop_call_c() ( \
      IF(rtop_tab[RTOP_I()].cname, x86("call", rtop_tab[RTOP_I()].cname, (uint64_t)(uintptr_t)rtop_tab[RTOP_I()].caddr)) \
    + IF(!rtop_tab[RTOP_I()].cname, x86("call", rtop_tab[RTOP_I()].name, (uint64_t)(uintptr_t)rtop_tab[RTOP_I()].addr)) \
)
#define SCRIP_DEF_ARITH_FUSE 1
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define fuse_on() (SCRIP_DEF_ARITH_FUSE)
#define fuse_op_ok() (binop_base((long long)_.op_ival) == BINOP_ADD || binop_base((long long)_.op_ival) == BINOP_SUB || binop_base((long long)_.op_ival) == BINOP_MUL)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define sse_op(xd, xs) x86(binop_base((long long)_.op_ival) == BINOP_SUB ? "subsd" : binop_base((long long)_.op_ival) == BINOP_MUL ? "mulsd" : "addsd", (xd), (xs))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define sse_finite(lb) ( \
      x86("mov", "rdx", "rax") \
    + x86("add", "rdx", "rdx") \
    + x86("movabs", "rcx", (uint64_t)0xFFE0000000000000ULL) \
    + x86("cmp", "rdx", "rcx") \
    + x86("jae", L(lb)) \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define i2d(xd, src, lb) x86("cvtsi2sd", (xd), (src))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define inl_ok() ( \
       !_.op_num_real && _.op_sa >= 0 && _.op_sb >= 0 && !(_.op_imm_a_ok && _.op_imm_b_ok) \
    && (binop_base((long long)_.op_ival) == BINOP_ADD || binop_base((long long)_.op_ival) == BINOP_SUB || binop_base((long long)_.op_ival) == BINOP_MUL) \
)
#define inl2_ok() (fuse_op_ok() && _.op_sa >= 0 && _.op_sb >= 0 && !(_.op_imm_a_ok && _.op_imm_b_ok))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define inl_tail_by(to_c) ( \
      ARITH_ARMED_FR() + x86("mov", "rdi", FRQ(_.op_sa)) \
    + x86("mov", "rsi", FRQ(_.op_sa + 8)) \
    + x86("mov", "rdx", FRQ(_.op_sb)) \
    + x86("mov", "rcx", FRQ(_.op_sb + 8)) \
    + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival)) \
    + IF(!(to_c), rtop_call_leaf()) \
    + IF((to_c), rtop_call_c()) \
    + x86("cmp", "al", (long)DT_FAIL) \
    + x86_omega("je") \
    + x86("mov", FRQ(_.op_off), "rax") \
    + x86("mov", FRQ(_.op_off + 8), "rdx") \
    + x86_rt_gc_poll() \
)
#define inl_tail() inl_tail_by(0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define BA_IA() ((_.op_imm_a_ok && _.op_imm_b_ok) || sn4_opt_binimm_off() ? 0 : _.op_imm_a_ok)
#define BA_IB() ((_.op_imm_a_ok && _.op_imm_b_ok) || sn4_opt_binimm_off() ? 0 : _.op_imm_b_ok)
#define BA_FOLD() (BA_IB() && !BA_IA() && (binop_base((long long)_.op_ival) == BINOP_ADD || binop_base((long long)_.op_ival) == BINOP_SUB))
#define BAR_FUSE1() (_.op_zres && fuse_on() && fuse_op_ok())
#define BAR_FUSE2() (!_.op_zres && fuse_on() && inl2_ok() && _.op_off >= 0)
std::string bb_binop_arith() {
    return IF(BAR_FUSE1(),
               x86("comment", "IR_BINOP_ARITH zd fuse")
             + x86_alpha()
             + IF(!BA_IA() && !BA_IB(), x86("note", ZOPN(0))
                            + x86("mov", "eax", ZOPD(0, 0))
                            + x86("note", ZOPN(1))
                            + x86("mov", "ecx", ZOPD(1, 0))
                            + x86("mov", "edx", "eax")
                            + x86("and", "edx", "ecx")
                            + x86("cmp", "dl", (long)DT_I))
             + IF(!BA_IA() && BA_IB(), x86("note", ZOPN(0))
                            + x86("mov", "ecx", ZOPD(0, 0))
                            + x86("note", ZOPN(0))
                            + x86("mov", "rax", ZOPQ(0, 8))
                            + x86("cmp", "cl", (long)DT_I))
             + IF( BA_IA() && !BA_IB(), x86("note", ZOPN(1))
                            + x86("mov", "eax", ZOPD(1, 0))
                            + x86("note", ZOPN(1))
                            + x86("mov", "rdx", ZOPQ(1, 8))
                            + x86("cmp", "al", (long)DT_I))
             + x86("jne", L(2))
             + IF(!BA_IA() && !BA_IB(), x86("note", ZOPN(0))
                            + x86("mov", "rax", ZOPQ(0, 8))
                            + x86("note", ZOPN(1))
                            + x86("mov", "rdx", ZOPQ(1, 8)))
             + IF( BA_IA(), x86("mov", "rax", (long)_.op_imm_a))
             + IF( BA_IB() && !BA_FOLD(), x86("mov", "rdx", (long)_.op_imm_b))
             + IF( BA_FOLD() && binop_base((long long)_.op_ival) == BINOP_ADD, x86("add", "rax", (long)_.op_imm_b))
             + IF( BA_FOLD() && binop_base((long long)_.op_ival) == BINOP_SUB, x86("sub", "rax", (long)_.op_imm_b))
             + IF(!BA_FOLD() && binop_base((long long)_.op_ival) == BINOP_ADD, x86("add", "rax", "rdx"))
             + IF(!BA_FOLD() && binop_base((long long)_.op_ival) == BINOP_SUB, x86("sub", "rax", "rdx"))
             + IF(binop_base((long long)_.op_ival) == BINOP_MUL, x86("imul", "rax", "rdx"))
             + IF(binop_promotes((long long)_.op_ival) || _.op_strict == 2, x86("jo", L(0)))
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), (long)DT_I)
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rax")
             + x86("jmp", L(7))
             + x86("def", L(2))
             + IF(!BA_IA() && BA_IB(), x86("mov", "eax", "ecx")
                            + x86("mov", "edx", "ecx"))
             + IF( BA_IA() && !BA_IB(), x86("mov", "ecx", "eax")
                            + x86("mov", "edx", "eax"))
             + x86("and", "edx", (long)DT_NUMERIC_BIT)
             + x86("jz", L(0))
             + IF(!BA_IA(), x86("note", ZOPN(0))
                     + x86("mov", "rsi", ZOPQ(0, 8)))
             + IF( BA_IA(), x86("mov", "rsi", (long)_.op_imm_a))
             + IF(!BA_IB(), x86("note", ZOPN(1))
                     + x86("mov", "rdi", ZOPQ(1, 8)))
             + IF( BA_IB(), x86("mov", "rdi", (long)_.op_imm_b))
             + IF(!BA_IA(), x86("cmp", "al", (long)DT_R)
                     + x86("je", L(3))
                     + i2d("xmm0", "rsi", 8)
                     + x86("jmp", L(4))
                     + x86("def", L(3))
                     + x86("movq", "xmm0", "rsi")
                     + x86("def", L(4)))
             + IF( BA_IA(), i2d("xmm0", "rsi", 8))
             + IF(!BA_IB(), x86("cmp", "cl", (long)DT_R)
                     + x86("je", L(5))
                     + i2d("xmm1", "rdi", 10)
                     + x86("jmp", L(6))
                     + x86("def", L(5))
                     + x86("movq", "xmm1", "rdi")
                     + x86("def", L(6)))
             + IF( BA_IB(), i2d("xmm1", "rdi", 10))
             + sse_op("xmm0", "xmm1")
             + x86("movq", "rax", "xmm0")
             + sse_finite(0)
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), (long)DT_R)
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rax")
             + x86("def", L(7))
             + x86_gamma()
             + x86("def", L(0)) + ARITH_ARMED_ZD()
             + x86("note", ZOPN(0))
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1))
             + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1))
             + x86("mov", "rcx", ZOPQ(1, 8))
             + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
             + rtop_call_c()
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!BAR_FUSE1() && BAR_FUSE2(),
               x86("comment", "IR_BINOP_ARITH inl fuse")
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
             + IF(binop_base((long long)_.op_ival) == BINOP_ADD, x86("add", "rax", "rdx"))
             + IF(binop_base((long long)_.op_ival) == BINOP_SUB, x86("sub", "rax", "rdx"))
             + IF(binop_base((long long)_.op_ival) == BINOP_MUL, x86("imul", "rax", "rdx"))
             + IF(binop_promotes((long long)_.op_ival) || _.op_strict == 2, x86("jo", L(0)))
             + x86("mov", FRQ(_.op_off), (long)DT_I)
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
             + sse_finite(0)
             + x86("mov", FRQ(_.op_off), (long)DT_R)
             + x86("mov", FRQ(_.op_off + 8), "rax")
             + x86("def", L(7))
             + x86_gamma()
             + x86("def", L(0))
             + inl_tail_by(1)
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!BAR_FUSE1() && !BAR_FUSE2() && _.op_zres,
               x86("comment", "IR_BINOP_ARITH zd")
             + x86_alpha() + ARITH_ARMED_ZD()
             + x86("note", ZOPN(0))
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("note", ZOPN(0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("note", ZOPN(1))
             + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("note", ZOPN(1))
             + x86("mov", "rcx", ZOPQ(1, 8))
             + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
             + rtop_call_leaf()
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86_rt_gc_poll()
             + x86_gamma()
             + x86_beta_trampoline())
         + IF(!BAR_FUSE1() && !BAR_FUSE2() && !_.op_zres,
               IF(_.op_off >= 0 && inl_ok(),
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
         + IF(binop_base((long long)_.op_ival) == BINOP_ADD, x86("add", "rax", "rcx"))
         + IF(binop_base((long long)_.op_ival) == BINOP_SUB, x86("sub", "rax", "rcx"))
         + IF(binop_base((long long)_.op_ival) == BINOP_MUL, x86("imul", "rax", "rcx"))
         + IF(binop_promotes((long long)_.op_ival) || _.op_strict == 2, x86("jo", L(0)))
         + x86("mov", FRQ(_.op_off), (long)DT_I)
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
         + x86_beta_trampoline()));
}
