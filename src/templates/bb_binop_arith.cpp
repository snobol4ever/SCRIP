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
/* ZTOS-1 (Lon s21x-o "Do not put RSP references directly into the templates"): private raw-rsp helper RETIRED -- call sites now speak the sanctioned spine accessor ZTOS/ZTOSD (x86_asm.h), which adds op_zdepth so a box's own carve and its own TOS reads compose instead of colliding.  Byte-identical while this kind is unarmed (op_zdepth==0); correct once it is armed, which is what lets the _spine exclusion list retire. */
extern "C" int zc_nofc(void);
static inline int nofc() { return zc_nofc(); }   /* NOFC-ONE (s22r): DELEGATES to the single authority in zeta_storage.c.  s22l declared zc_nofc THE ONE AUTHORITY and moved the policy there, but these three template-local copies SURVIVED with their own getenv -- four independent reads of one switch, agreeing only by coincidence of polarity, which is precisely the producer/consumer asymmetry s22l had just finished diagnosing.  A default flip that moved zc_nofc alone would have re-armed that disease in three templates at once. */
static inline int vfcb() { return !nofc() && x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0; }   /* ZB-VAL-1/5: registered value-spine binop -- operands are the TOP TWO cells (a=[rsp+16..31], b=[rsp+0..15]); leaves may be VARS so the arm carries the FULL type structure */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int inl_ok() { long long o = (long long)_.op_ival; return !_.op_num_real && _.op_sa >= 0 && _.op_sb >= 0 && !(_.op_imm_a_ok && _.op_imm_b_ok) && (o == BINOP_ADD || o == BINOP_SUB || o == BINOP_MUL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string fc_tail() {
    return x86("mov", "rdi", ZTOS(16)) + x86("mov", "rsi", ZTOS(24)) + x86("mov", "rdx", ZTOS(0)) + x86("mov", "rcx", ZTOS(8))
         + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
         + x86("call", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival)) + x86("cmp", "eax", (long)DT_FAIL) + x86_omega("je")
         + x86_zrelease(16) + x86("mov", ZTOS(0), "rax") + x86("mov", ZTOS(8), "rdx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string inl_tail() {
    return x86("mov", "rdi", FRQ(_.op_sa)) + x86("mov", "rsi", FRQ(_.op_sa + 8)) + x86("mov", "rdx", FRQ(_.op_sb)) + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
         + x86("call", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival)) + x86("cmp", "eax", (long)DT_FAIL) + x86_omega("je")
         + x86("mov", FRQ(_.op_off), "rax") + x86("mov", FRQ(_.op_off + 8), "rdx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_arith() {
    if (!PLATFORM_X86) return std::string();
    if (_.op_zres)
        return x86("comment", "IR_BINOP_ARITH zd")
             + x86_alpha()
             + x86("mov", "rdi", ZOPQ(0, 0))
             + x86("mov", "rsi", ZOPQ(0, 8))
             + x86("mov", "rdx", ZOPQ(1, 0))
             + x86("mov", "rcx", ZOPQ(1, 8))
             + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
             + x86("call", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival))
             + x86("cmp", "eax", (long)DT_FAIL)
             + x86_omega("je")
             + x86("mov", ZRES(0), "rax")
             + x86("mov", ZRES(8), "rdx")
             + x86_gamma()
             + x86_beta_trampoline();   /* ZD-1 (Lon s21x-v): operands are the producers' suspended cells at STAGED DIFFERENCES (mode 3; adjacency retired -- the two producers may sit at ANY depths), result is the box's own cell (mode 1), and the x86_zrelease(16) pop-shuffle is GONE: cells persist to the statement boundary where op_zgpop/op_wpop restore rsp wholesale.  The conditional omega rides the existing invert+pop synth (own K + planner wpop = statement entry). */
    return IF(vfcb() && inl_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH fc inl")
         + x86("mov", "eax", ZTOSD(16))
         + x86("cmp", "eax", (long)DT_I)
         + x86("jne", L(0))
         + x86("mov", "eax", ZTOSD(0))
         + x86("cmp", "eax", (long)DT_I)
         + x86("jne", L(0))
         + x86("mov", "rax", ZTOS(24))
         + x86("mov", "rcx", ZTOS(8))
         + IF((long long)_.op_ival == BINOP_ADD, x86("add",  "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_SUB, x86("sub",  "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_MUL, x86("imul", "rax", "rcx"))
         + x86_zrelease(16)
         + x86("mov", ZTOS(0), (long)DT_I)
         + x86("mov", ZTOS(8), "rax")
         + x86_gamma()
         + x86("def", L(0))
         + fc_tail()
         + x86_gamma()
         + x86_beta_trampoline())
         + IF(vfcb() && !inl_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH fc")
         + fc_tail()
         + x86_gamma()
         + x86_beta_trampoline())
         + IF(!vfcb() && _.op_off >= 0 && inl_ok(),
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
         + IF(!vfcb() && _.op_off >= 0 && !inl_ok(),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH")
         + inl_tail()
         + x86_gamma()
         + x86_beta_trampoline());
}
