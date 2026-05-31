/* bb_arith.cpp — BB template for IR_ARITH: Prolog arithmetic (Y is X+2).
   PL-T-3 (GOAL-BB-TEMPLATE-LADDER.md). x86 only per Invariant #14 (2026-05-22). */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
extern long rt_pl_arith(int lk, long li, const char *ls, int rk, long ri, const char *rs, const char *op);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_arith_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        const char *op = pBB->sval ? pBB->sval : "+";
        if (MEDIUM_MACRO_DEF) {
            return s_comment("# no macro form — RESOLVE_ARITH");
        }
        if (!pBB->α || !pBB->β) {
            if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — RESOLVE_ARITH (vacuous)");
            if (MEDIUM_BINARY) {
                bin = { {1, 5, 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0);
            }
            return s_1asm(emit_fmt("# BOX RESOLVE_ARITH(op='%s')", op))
                 + s_1asm("# RESOLVE_ARITH: missing operands — vacuous")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
        IR_t *lhs = pBB->α, *rhs = pBB->β;
        if (MEDIUM_BINARY) {
            /* M3-PL-NOINTERP-1c (Opus 4.8, 2026-05-29): full 7-arg port of the TEXT arm's
               rt_pl_arith(lk,li,ls, rk,ri,rs, op) call. SysV: edi=lk rsi=li rdx=ls ecx=rk
               r8=ri r9=rs, 7th arg (op) pushed. Box-entry rsp is 8-misaligned (call ret addr);
               one 8-byte push realigns to 16 for the call; `add rsp,8` restores after.
               Previously a few stray mov-rcx loads + a call-0 stub — wrong regs and a null
               target (would segfault). */
            extern int g_sm_native_unsupported;
            std::string b;
            /* mov edi, lhs->t          BF [4]                                                   */
            b += bytes(1, "\xBF") + u32le((uint32_t)(int)lhs->t);
            /* mov rsi, lhs->ival       48 BE [8]                                                */
            b += bytes(2, "\x48\xBE") + u64le((uint64_t)(long)lhs->ival);
            /* mov rdx, lhs->sval (or xor edx,edx)   48 BA [8] / 31 D2                           */
            if (_.bb_ls) b += bytes(2, "\x48\xBA") + u64le((uint64_t)(uintptr_t)lhs->sval);
            else            b += bytes(2, "\x31\xD2");
            /* mov ecx, rhs->t          B9 [4]                                                   */
            b += bytes(1, "\xB9") + u32le((uint32_t)(int)rhs->t);
            /* mov r8, rhs->ival        49 B8 [8]                                                */
            b += bytes(2, "\x49\xB8") + u64le((uint64_t)(long)rhs->ival);
            /* mov r9, rhs->sval (or xor r9d,r9d)    49 B9 [8] / 45 31 C9                        */
            if (_.bb_rs) b += bytes(2, "\x49\xB9") + u64le((uint64_t)(uintptr_t)rhs->sval);
            else            b += bytes(3, "\x45\x31\xC9");
            /* push op: mov rax, op(imm64); push rax   48 B8 [8] 50   (or push 0 = 6A 00)        */
            if (_.bb_op_lbl) b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)op) + bytes(1, "\x50");
            else                b += bytes(2, "\x6A\x00");
            /* movabs rax, &rt_pl_arith; call rax      48 B8 [8] FF D0                           */
            b += bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)(void*)rt_pl_arith) + bytes(2, "\xFF\xD0");
            /* add rsp, 8               48 83 C4 08                                              */
            b += bytes(4, "\x48\x83\xC4\x08");
            int j = (int)b.size();
            bin = { {j + 1, j + 5, j + 6}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
            return b + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0);
        }
        if (MEDIUM_TEXT) {
            std::string load_rdx = _.bb_ls
                ? s_2asm("lea rcx,", emit_fmt("[rip + %s]", _.bb_ls)) + s_2asm("mov", "rdx, rcx")
                : s_2asm("xor", "edx, edx");
            std::string load_r9 = _.bb_rs
                ? s_2asm("lea rcx,", emit_fmt("[rip + %s]", _.bb_rs)) + s_2asm("mov", "r9, rcx")
                : s_2asm("xor", "r9d, r9d");
            std::string push_op = _.bb_op_lbl
                ? s_2asm("lea rcx,", emit_fmt("[rip + %s]", _.bb_op_lbl)) + s_2asm("push", "rcx")
                : s_2asm("push", "0");
            return s_1asm(emit_fmt("%s:", _.lbl_α))
                 + s_1asm(emit_fmt("# BOX RESOLVE_ARITH(op='%s')", op))
                 + s_2asm("mov edi,", emit_fmt("%d", (int)lhs->t))
                 + s_2asm("mov esi,", emit_fmt("%ld", (long)lhs->ival))
                 + load_rdx
                 + s_2asm("mov ecx,", emit_fmt("%d", (int)rhs->t))
                 + s_2asm("mov r8,",  emit_fmt("%ld", (long)rhs->ival))
                 + load_r9
                 + push_op
                 + s_2asm("call", "rt_pl_arith@PLT")
                 + s_2asm("add", "rsp, 8")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_L2asm(emit_fmt("%s:", _.lbl_β), "jmp", _.lbl_ω);
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_arith(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_arith_str(pBB, bin), bin);
}
