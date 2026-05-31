/* bb_binop.cpp — BB template for BB_BINOP (Icon arithmetic + relop apply).
   IBB-3 (Opus 4.7, 2026-05-28): arithmetic apply. IBB-8b (Opus 4.8, 2026-05-29): relop + strrel apply.

   Two apply shapes, both AG-pure (operands already on the vstack, pushed by chain predecessors):

   ARITHMETIC (BINOP_ADD..MOD, POW): rt_arith(sm_op) pops 2, pushes result, then jmp γ.
     movabs rdi, sm_op ; movabs rax, &rt_arith ; call rax ; jmp γ ; β: jmp ω   (32 bytes)
     Driven by flat_drive_binop_tree (legacy tree-shape α=lhs/β=rhs) OR as an AG-pure apply.

   RELOP / STRREL (BINOP_LT..NE numeric, BINOP_SLT..SNE string): the condition of an Icon
     if/while.  rt_acomp(tt_op) (numeric) or rt_lcomp(tt_op) (string) pops 2, pushes the result
     (value on success, FAIL on relop-false) and sets the LAST_OK flag.  The relop then jmps γ
     UNCONDITIONALLY — both ports of an AG-pure relop point at the BB_IF router (mode-2 bb_exec.c
     BB_BINOP AG-pure arm returns γ on success and ω on fail, but lower_new_If_ag wires γ==ω==
     BB_IF, so the single jmp γ here lands at the router regardless).  BB_IF (bb_if.cpp) reads
     LAST_OK and branches then/else.
       movabs rdi, tt_op ; movabs rax, &rt_acomp|&rt_lcomp ; call rax ; jmp γ ; β: jmp ω   (32 bytes)

   ICN_BINOP_* to SM_/TT_ mapping mirrors bb_binop_gen.cpp. BINOP_CONCAT routes through
   bb_lconcat (not here). */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "../../runtime/interp/gen.h"
void rt_arith(int op);
void rt_acomp(int op);
void rt_lcomp(int op);
void rt_gen_concat(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gen_to_sm(int64_t op) {
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
static int gen_is_numrel(int64_t op) { return op >= BINOP_LT  && op <= BINOP_NE; }
static int gen_is_strrel(int64_t op) { return op >= BINOP_SLT && op <= BINOP_SNE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gen_rel_to_tt(int64_t op) {
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
static std::string bb_binop_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_BINOP");
    int64_t op = pBB ? pBB->ival : -1;
    int is_rel = pBB && (gen_is_numrel(op) || gen_is_strrel(op));
    if (is_rel) {
        int tt   = gen_rel_to_tt(op);
        int strr = gen_is_strrel(op);
        if (MEDIUM_TEXT) {
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_comment(emit_fmt("# BOX BB_BINOP relop tt=%d (%s: pop 2, push result, set LAST_OK)", tt, strr ? "rt_lcomp" : "rt_acomp"))
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
                 + s_comment("# BOX BB_BINOP concat (rt_gen_concat: pop 2, push CONCAT result, set LAST_OK)")
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
    int sm_op = pBB ? gen_to_sm(op) : -1;
    if (sm_op < 0) {
        if (MEDIUM_BINARY || MEDIUM_TEXT) {
            fprintf(stderr, "[IBB] FATAL bb_binop: unsupported op ival=%lld (arith + relop only)\n",
                    pBB ? (long long)pBB->ival : -1LL);
            abort();
        }
    }
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX BB_BINOP op=%d (apply: rt_arith pops 2, pushes result)", sm_op))
             + s_2asm("mov edi,", emit_fmt("%d", sm_op))
             + s_2asm("call",     "rt_arith@PLT")
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        /* movabs rdi, sm_op (10) ; movabs rax, &rt_arith (10) ; call rax (2) ; jmp γ (5) ; β: jmp ω (5). */
        /* Total 32. Patch: 23 → γ, 27 → β-def, 28 → ω. Same layout as bb_lit_scalar BB_LIT_I path.     */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_binop(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_binop_str(pBB, bin), bin);
}
