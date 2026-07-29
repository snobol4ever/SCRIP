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
static inline const char * rspq(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp + %d]", off); return b[i]; }   /* ZB-VAL-1: the bb_assign_global rspq precedent */
static inline int vfcb() { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0; }   /* ZB-VAL-1/5: registered value-spine binop -- operands are the TOP TWO cells (a=[rsp+16..31], b=[rsp+0..15]); leaves may be VARS so the arm carries the FULL type structure */
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
std::string bb_binop_arith() {
    if (!PLATFORM_X86) return std::string();
    return IF(vfcb(),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH fc")
         + x86("mov", "rdi", rspq(16))
         + x86("mov", "rsi", rspq(24))
         + x86("mov", "rdx", rspq(0))
         + x86("mov", "rcx", rspq(8))
         + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
         + x86("call", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival))
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86("add", "rsp", (long)16)
         + x86("mov", rspq(0), "rax")
         + x86("mov", rspq(8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline())
         + IF(!vfcb() && _.op_off >= 0,
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH")
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + IF(rtop_is_dyn(_.op_ival), x86("mov", "r8d", (long)_.op_ival))
         + x86("call", rtop_name(_.op_ival), (uint64_t)(uintptr_t)rtop_addr(_.op_ival))
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline());
}
