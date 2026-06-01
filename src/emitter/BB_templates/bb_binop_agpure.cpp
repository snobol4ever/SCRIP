/* bb_binop_agpure.cpp — BB box: IR_BINOP legacy AG-pure apply (rt_arith/rt_acomp/rt_lcomp/rt_gen_concat).
   Operands already on the vstack, pushed by chain predecessors. The pre-flat-chain fallback path.
   Split out of bb_binop.cpp (one-box-one-file). */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/interp/gen.h"
void rt_arith(int op);
void rt_acomp(int op);
void rt_lcomp(int op);
void rt_gen_concat(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int agp_to_sm(int64_t op) {
    switch (op) {
    case BINOP_ADD: return (int)SM_ADD;
    case BINOP_SUB: return (int)SM_SUB;
    case BINOP_MUL: return (int)SM_MUL;
    case BINOP_DIV: return (int)SM_DIV;
    case BINOP_MOD: return (int)SM_MOD;
    case BINOP_POW: return (int)SM_EXP;
    default: return -1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int agp_is_numrel(int64_t op) { return op >= BINOP_LT  && op <= BINOP_NE; }
static int agp_is_strrel(int64_t op) { return op >= BINOP_SLT && op <= BINOP_SNE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int agp_rel_to_tt(int64_t op) {
    switch (op) {
    case BINOP_LT:  return (int)TT_LT;   case BINOP_LE:  return (int)TT_LE;
    case BINOP_GT:  return (int)TT_GT;   case BINOP_GE:  return (int)TT_GE;
    case BINOP_EQ:  return (int)TT_EQ;   case BINOP_NE:  return (int)TT_NE;
    case BINOP_SLT: return (int)TT_LLT;  case BINOP_SLE: return (int)TT_LLE;
    case BINOP_SGT: return (int)TT_LGT;  case BINOP_SGE: return (int)TT_LGE;
    case BINOP_SEQ: return (int)TT_LEQ;  case BINOP_SNE: return (int)TT_LNE;
    default: return (int)TT_EQ;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_agpure_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_BINOP");
    int64_t op = pBB ? pBB->ival : -1;
    int is_rel = pBB && (agp_is_numrel(op) || agp_is_strrel(op));
    if (is_rel) {
        int tt   = agp_rel_to_tt(op);
        int strr = agp_is_strrel(op);
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX IR_BINOP relop tt=%d (%s: pop 2, push result, set LAST_OK)", tt, strr ? "rt_lcomp" : "rt_acomp"))
                 + s_2asm("mov edi,", emit_fmt("%d", tt))
                 + s_2asm("call",     strr ? "rt_lcomp@PLT" : "rt_acomp@PLT")
                 + s_2asm("jmp",      _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",      _.lbl_ω);
        }
        if (MEDIUM_BINARY) {
            /* movabs rdi, tt           (10: 48 BF + u64le)                                            */
            /* movabs rax, &rt_*comp    (10: 48 B8 + u64le)                                            */
            /* call rax                  ( 2: FF D0)                                                   */
            /* jmp γ                     ( 5: E9 + u32le)   ← γ patch at offset 23                     */
            /* β: jmp ω                  ( 5: E9 + u32le)   ← β-def at 27, ω patch at 28               */
            uint64_t fptr; { void (*fp)(int) = strr ? rt_lcomp : rt_acomp; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(2, "\x48\xBF") + u64le((uint64_t)(uint32_t)tt)
                 + bytes(2, "\x48\xB8") + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(1, "\xE9")     + u32le(0)
                 + bytes(1, "\xE9")     + u32le(0);
        }
        return std::string();
    }
    if (op == BINOP_CONCAT) {
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment("# BOX IR_BINOP concat (rt_gen_concat: pop 2, push CONCAT result, set LAST_OK)")
                 + s_2asm("call", "rt_gen_concat@PLT")
                 + s_2asm("jmp",  _.lbl_γ)
                 + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                 + s_2asm("jmp",  _.lbl_ω);
        }
        if (MEDIUM_BINARY) {
            /* rt_gen_concat takes no argument, but keep the proven 32-byte arith layout (dead movabs    */
            /* rdi,0 so γ/β/ω patch offsets stay {23,27,28}); rt_gen_concat ignores rdi.                 */
            /* movabs rdi,0 (10) ; movabs rax,&rt_gen_concat (10) ; call rax (2) ; jmp γ (5) ; β: jmp ω (5). */
            uint64_t fptr; { void (*fp)(void) = rt_gen_concat; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return bytes(2, "\x48\xBF") + u64le(0)
                 + bytes(2, "\x48\xB8") + u64le(fptr)
                 + bytes(2, "\xFF\xD0")
                 + bytes(1, "\xE9")     + u32le(0)
                 + bytes(1, "\xE9")     + u32le(0);
        }
        return std::string();
    }
    int sm_op = pBB ? agp_to_sm(op) : -1;
    if (sm_op < 0) {
        if (MEDIUM_BINARY || MEDIUM_TEXT) {
            fprintf(stderr, "[IBB] FATAL bb_binop: unsupported op ival=%lld (arith + relop only)\n",
                    pBB ? (long long)pBB->ival : -1LL);
            abort();
        }
    }
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX IR_BINOP op=%d (apply: rt_arith pops 2, pushes result)", sm_op))
             + s_2asm("mov edi,", emit_fmt("%d", sm_op))
             + s_2asm("call",     "rt_arith@PLT")
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        /* movabs rdi, sm_op (10) ; movabs rax, &rt_arith (10) ; call rax (2) ; jmp γ (5) ; β: jmp ω (5). */
        /* Total 32. Patch: 23 → γ, 27 → β-def, 28 → ω. Same layout as bb_lit_scalar IR_LIT_I path.     */
        uint64_t fptr; { void (*fp)(int) = rt_arith; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xBF") + u64le((uint64_t)(uint32_t)sm_op)
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
