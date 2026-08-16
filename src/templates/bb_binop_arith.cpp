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
/* ===== INLINE COERCE+ARITH FUSE -- PLAY SWITCHES (Lon 2026-08-13 in-chat: "create #define switches so you
 * can play and check in the code to the repo for our later choices").  COMPILE-TIME, BOTH DEFAULT 0 =
 * DORMANT: an OFF build emits byte-identical .s to pre-rung HEAD (gated over 90 crosscheck programs at
 * landing).  Flip + rebuild scrip AND libscrip_rt (the emitter lives in the .so).
 *
 * SCRIP_DEF_ARITH_FUSE=1 -- fused numeric-coercion check + inline REAL arm for ADD/SUB/MUL, BOTH the zd
 *   (spine-cell) arm and the flat FRQ arm.  Replaces the per-operand `cmp DT_I` pair with the descr.h
 *   combined-tag law: `and tags; cmp DT_I` = BOTH-INT (2 insns), then `and combined, NUMERIC_BIT; jz slow`
 *   = BOTH-NUMERIC residual (exhaustively valid incl. all 64 user datatypes -- only DT_I/DT_R carry bit0).
 *   Both-int -> inline add/sub/imul (as today).  Both-numeric-not-both-int -> at least one real: inline
 *   movq/cvtsi2sd per operand + addsd/subsd/mulsd, result tag DT_R.  Anything else -> L(0) = the
 *   pre-existing rt call, which owns string conversion, null-string->0, left-operand-first error identity
 *   (oracle: 'NaN'+x is &ERRTYPE 1, x+'NaN' is 2), DT_DATA overloads, and DT_FAIL.  Result tags are
 *   per-arm CONSTANT stores -- the L|R identity is surrendered by design (descr.h header: I|R==0x07 is not
 *   a tag; never compute a result tag with `or`).  The fast arms touch ONLY rax/rcx/rdx/rsi/rdi/xmm0-2 --
 *   never r8-r11/xmm8-15 (RTCC VM claims) -- which is exactly why they need no rtcc_wb/rl veneer: that
 *   deletion, plus the call+PLT, is the win being bought.  No overflow guard on the int arm, matching BOTH
 *   existing paths (inline `add` and rt_add's `lea` neither test OF); divergence question flagged to Lon.
 *
 * SCRIP_DEF_I2D_MAGIC=1 -- int->double via Lon's mantissa-splice instead of cvtsi2sd, guarded to
 *   0 <= v < 2^52 with cvtsi2sd fallback: splice v into the mantissa under exponent pattern
 *   0x4330000000000000, reinterpret, subtract 2^52.0 -- and the subtrahend's BITS are the SAME constant
 *   (0x4330000000000000 IS double 4503599627370496.0 = 2^52), so one movabs serves twice.  Pre-stated
 *   prediction so it can be graded: cvtsi2sd r64 (1 insn) beats the guarded splice (~9 insns); this
 *   define exists to measure that claim, not to win. */
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
/* i2d: convert int64 in GPR `src` to double in `xd`.  Magic flavor clobbers rdx and xmm2 and burns labels
 * L(lb)/L(lb+1); cvtsi2sd flavor is one instruction and burns nothing.  Callers pass distinct lb per site. */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Fused flat-arm eligibility: drops inl_ok's !op_num_real exclusion -- statically-real operands are
 * precisely what the inline REAL arm serves.  Same slot/imm constraints otherwise. */
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
             + x86_beta_trampoline();   /* Fast arms (both-int, both-numeric-real) join at L(7) and exit the FIRST gamma with NO rtcc veneer -- they touch only rax/rcx/rdx/rsi/rdi/xmm0-2, never the r8-r11/xmm8-15 VM claims, which is the point.  L(0) is the pre-existing call interior verbatim: it alone owns strings, null->0, DT_DATA overloads, left-first error identity, DT_FAIL/omega. */
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
             + x86_beta_trampoline();   /* Same shape as the zd fuse above with FRQ accessors + the imm-staged operands folded to compile-time DT_I tags; L(0) slow path is inl_tail() = the pre-existing call, unchanged. */
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
             + x86_beta_trampoline();   /* ZD-1 (Lon s21x-v): operands are the producers' suspended cells at STAGED DIFFERENCES (mode 3; adjacency retired -- the two producers may sit at ANY depths), result is the box's own cell (mode 1), and the x86_zrelease(16) pop-shuffle is GONE: cells persist to the statement boundary where op_zgpop/op_wpop restore rsp wholesale.  The conditional omega rides the existing invert+pop synth (own K + planner wpop = statement entry). */
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
