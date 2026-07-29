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
}
#include "x86_asm.h"
#include <cstdio>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const char * rspq(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp + %d]", off); return b[i]; }   /* ZB-VAL-1: the bb_assign_global rspq precedent */
static inline const char * rspd(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "dword ptr [rsp + %d]", off); return b[i]; }   /* ZB-VAL-5: cell type-dword read, the bb_match_capture rspd precedent */
static inline int vfcb() { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0; }   /* ZB-VAL-1/5: registered value-spine binop -- operands are the TOP TWO cells (a=[rsp+16..31], b=[rsp+0..15]); leaves may be VARS so the arm carries the FULL type structure */
/* RTX-6r (s206): ops eligible for the real-real inline arm.  ADD/SUB/MUL ONLY, and the exclusions are
 * semantic, not laziness:  DIV returns FAILDESCR when the divisor is 0.0 (arithmetic.c:239; SPITBOL
 * manual p.20 "1.0 / 0 => Error #262") so it needs a zero-test branch;  MOD needs fmod();  POW needs
 * pow().  ADD/SUB/MUL on two DT_R operands cannot fail -- arithmetic.c:236-238 returns REALVAL(...)
 * unconditionally with no FAILDESCR arm and no longjmp reachable -- which is exactly what licenses the
 * fast path to skip the DT_FAIL test the slow path performs.  Mirrors arm 1's own ADD/SUB/MUL-only
 * inline set (:39-41). */
static inline int rfast() { return (long long)_.op_ival == BINOP_ADD || (long long)_.op_ival == BINOP_SUB || (long long)_.op_ival == BINOP_MUL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_arith() {
    if (!PLATFORM_X86) return std::string();
    return IF(vfcb(),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH fc")
         + x86("mov", "eax", rspd(16))
         + x86("cmp", "eax", (long)DT_DATA)
         + x86("je", L(0))
         + x86("mov", "eax", rspd(0))
         + x86("cmp", "eax", (long)DT_DATA)
         + x86("je", L(0))
         + x86("mov", "eax", rspd(16))
         + x86("cmp", "eax", (long)DT_I)
         + x86("jne", L(2))
         + x86("mov", "eax", rspd(0))
         + x86("cmp", "eax", (long)DT_I)
         + x86("jne", L(2))
         + x86("mov", "rax", rspq(24))
         + x86("mov", "rcx", rspq(8))
         + IF((long long)_.op_ival == BINOP_ADD, x86("add",  "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_SUB, x86("sub",  "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_MUL, x86("imul", "rax", "rcx"))
         + x86("add", "rsp", (long)16)
         + x86("mov", rspq(0), (long)DT_I)
         + x86("mov", rspq(8), "rax")
         + x86_gamma()
         + x86("def", L(0))
         + x86("mov", "rdi", rspq(16))
         + x86("mov", "rsi", rspq(24))
         + x86("mov", "rdx", rspq(0))
         + x86("mov", "rcx", rspq(8))
         + x86("mov", "r8d", (long)_.op_ival)
         + x86("lea", "r9", rspq(16))
         + x86("call", "rt_binop_overload", (uint64_t)(uintptr_t)(void*)rt_binop_overload)
         + x86("test", "eax", "eax")
         + x86("je", L(2))
         + x86("add", "rsp", (long)16)
         + x86_gamma()
         + x86("def", L(2))
         + x86("mov", "rdi", rspq(16))
         + x86("mov", "rsi", rspq(24))
         + x86("mov", "rdx", rspq(0))
         + x86("mov", "rcx", rspq(8))
         + x86("mov", "r8d", (long)_.op_ival)
         + x86("call", "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86("add", "rsp", (long)16)
         + x86("mov", rspq(0), "rax")
         + x86("mov", rspq(8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline())
         + IF(!vfcb() && _.op_num_real && _.op_off >= 0 && _.op_sa >= 0 && _.op_sb >= 0,
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH_REAL")
         + IF(rfast(),
           x86("comment", "RTX-6r fast: both DT_R => inline SSE (no call, no setjmp, no op switch)")
         + x86("mov", "eax", FR(_.op_sa))
         + x86("cmp", "eax", (long)DT_R)
         + x86("jne", L(2))
         + x86("mov", "eax", FR(_.op_sb))
         + x86("cmp", "eax", (long)DT_R)
         + x86("jne", L(2))
         + x86("mov", "rax", FRQ(_.op_sa + 8))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("movq", "xmm0", "rax")
         + x86("movq", "xmm1", "rcx")
         + IF((long long)_.op_ival == BINOP_ADD, x86("addsd", "xmm0", "xmm1"))
         + IF((long long)_.op_ival == BINOP_SUB, x86("subsd", "xmm0", "xmm1"))
         + IF((long long)_.op_ival == BINOP_MUL, x86("mulsd", "xmm0", "xmm1"))
         + x86("movq", "rax", "xmm0")
         + x86("mov", FRQ(_.op_off),     (long)DT_R)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86("def", L(2)))
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8d", (long)_.op_ival)
         + x86("call", "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov", FRQ(_.op_off), "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta_trampoline())
         + IF(!vfcb() && _.op_off >= 0 && !_.op_num_real
              && ((long long)_.op_ival == BINOP_ADD || (long long)_.op_ival == BINOP_SUB || (long long)_.op_ival == BINOP_MUL
                  || (long long)_.op_ival == BINOP_DIV || (long long)_.op_ival == BINOP_MOD),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH")
         + IF(!_.op_imm_a_ok,
           x86("mov", "eax", FR(_.op_sa))
         + x86("cmp", "eax", (long)DT_DATA)
         + x86("je", L(0)))
         + IF(!_.op_imm_b_ok,
           x86("mov", "eax", FR(_.op_sb))
         + x86("cmp", "eax", (long)DT_DATA)
         + x86("je", L(0)))
         + IF(!_.op_imm_a_ok,
           x86("mov", "eax", FR(_.op_sa))
         + x86("cmp", "eax", (long)DT_I)
         + x86("jne", L(2)))
         + IF(!_.op_imm_b_ok,
           x86("mov", "eax", FR(_.op_sb))
         + x86("cmp", "eax", (long)DT_I)
         + x86("jne", L(2)))
         + x86("def", L(1))
         + IF(!_.op_imm_a_ok, x86("mov", "rax", FRQ(_.op_sa + 8)))
         + IF( _.op_imm_a_ok, x86("mov", "rax", (long)_.op_imm_a))
         + IF(!_.op_imm_b_ok, x86("mov", "rcx", FRQ(_.op_sb + 8)))
         + IF( _.op_imm_b_ok, x86("mov", "rcx", (long)_.op_imm_b))
         + IF((long long)_.op_ival == BINOP_ADD, x86("add",  "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_SUB, x86("sub",  "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_MUL, x86("imul", "rax", "rcx"))
         + IF((long long)_.op_ival == BINOP_DIV, x86("cqo")
                               + x86("idiv", "rcx"))
         + IF((long long)_.op_ival == BINOP_MOD, x86("cqo")
                               + x86("idiv", "rcx")
                               + x86("mov", "rax", "rdx"))
         + x86("mov", FRQ(_.op_off),     (long)DT_I)
         + x86("mov", FRQ(_.op_off + 8), "rax")
         + x86_gamma()
         + x86("def", L(0))
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8d", (long)_.op_ival)
         + x86("lea", "r9", FRQ(_.op_off))
         + x86("call", "rt_binop_overload", (uint64_t)(uintptr_t)(void*)rt_binop_overload)
         + x86("test", "eax", "eax")
         + x86_gamma("jne")
         + x86("def", L(2))
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8d", (long)_.op_ival)
         + x86("call", "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta()
         + x86_omega())
         + IF(_.op_off >= 0 && !_.op_num_real
              && ((long long)_.op_ival == BINOP_POW || (long long)_.op_ival == BINOP_CUNION || (long long)_.op_ival == BINOP_CDIFF || (long long)_.op_ival == BINOP_CINTER),
           x86_alpha()
         + x86("comment", "IR_BINOP_ARITH_DYN")
         + x86("mov", "rdi", FRQ(_.op_sa))
         + x86("mov", "rsi", FRQ(_.op_sa + 8))
         + x86("mov", "rdx", FRQ(_.op_sb))
         + x86("mov", "rcx", FRQ(_.op_sb + 8))
         + x86("mov", "r8d", (long)_.op_ival)
         + x86("call", "rt_num_arith", (uint64_t)(uintptr_t)(void*)rt_num_arith)
         + x86("cmp", "eax", (long)DT_FAIL)
         + x86_omega("je")
         + x86("mov", FRQ(_.op_off),     "rax")
         + x86("mov", FRQ(_.op_off + 8), "rdx")
         + x86_gamma()
         + x86_beta()
         + x86_omega());
}
