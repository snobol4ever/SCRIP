/* bb_binop_jct_relop.cpp — BB box: IR_BINOP relop where an operand is a __rk_jct_* junction.
   RK-EMIT-3 (2026-05-31, Raku). Split out of bb_binop.cpp (one-box-one-file). */
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/interp/gen.h"
int  bb_slot_get(IR_t * nd);
int rt_rk_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int jct_is_numrel(int64_t op) { return op >= BINOP_LT  && op <= BINOP_NE; }
static int jct_is_strrel(int64_t op) { return op >= BINOP_SLT && op <= BINOP_SNE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_jct_relop_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    int64_t op = pBB ? pBB->ival : -1;
    /* RK-EMIT-3 (2026-05-31): junction-collapse relop. If either operand producer is an IR_CALL to a       */
    /* __rk_jct_* constructor, the relop must autothread+collapse (docs.raku.org/type/Junction), not do a   */
    /* raw scalar compare (the junction operand's slot holds a tagged-string pointer, not an int). Detected */
    /* at emit time by the operand node's sval prefix; emits a by-value call to rt_rk_jct_relop (the SAME    */
    /* mode-2 junction_collapse) → eax truth, then branch jz ω / jmp γ (relop-false routes to ω/else, exactly */
    /* like the GZ-8 scalar relop). Both DESCRs are read from their producer slots (16 bytes each).          */
    if (g_icn_flat_chain && pBB && (jct_is_numrel(op) || jct_is_strrel(op)) && pBB->α && pBB->β) {
        int lhs_jct = (pBB->α->t == IR_CALL && pBB->α->sval && strncmp(pBB->α->sval, "__rk_jct_", 9) == 0);
        int rhs_jct = (pBB->β->t == IR_CALL && pBB->β->sval && strncmp(pBB->β->sval, "__rk_jct_", 9) == 0);
        if (lhs_jct || rhs_jct) {
            int sa = bb_slot_get(pBB->α), sb = bb_slot_get(pBB->β);
            if (sa >= 0 && sb >= 0) {
                if (MEDIUM_TEXT) {
                    return s_1asm(emit_fmt("%s:", _.lbl_α))
                         + s_comment(emit_fmt("# BOX IR_BINOP relop [RK-EMIT-3 junction collapse op=%lld -> rt_rk_jct_relop]", (long long)op))
                         + s_2asm("mov", emit_fmt("rdi, [r12+%d]", sa))
                         + s_2asm("mov", emit_fmt("rsi, [r12+%d]", sa + 8))
                         + s_2asm("mov", emit_fmt("rdx, [r12+%d]", sb))
                         + s_2asm("mov", emit_fmt("rcx, [r12+%d]", sb + 8))
                         + s_2asm("mov", emit_fmt("r8d, %lld", (long long)op))
                         + s_2asm("call", "rt_rk_jct_relop@PLT")
                         + s_2asm("test", "eax, eax")
                         + s_2asm("jz", _.lbl_ω)
                         + s_2asm("jmp", _.lbl_γ)
                         + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                         + s_2asm("jmp", _.lbl_ω);
                }
                if (MEDIUM_BINARY) {
                    uint64_t fptr; { int (*fp)(DESCR_t, DESCR_t, int) = rt_rk_jct_relop; fptr = (uint64_t)(uintptr_t)(void*)fp; }
                    std::string s;
                    s += bytes(4, "\x49\x8B\xBC\x24") + u32le((uint32_t)sa);        /* mov rdi,[r12+sa]   */
                    s += bytes(4, "\x49\x8B\xB4\x24") + u32le((uint32_t)(sa + 8));  /* mov rsi,[r12+sa+8] */
                    s += bytes(4, "\x49\x8B\x94\x24") + u32le((uint32_t)sb);        /* mov rdx,[r12+sb]   */
                    s += bytes(4, "\x49\x8B\x8C\x24") + u32le((uint32_t)(sb + 8));  /* mov rcx,[r12+sb+8] */
                    s += bytes(2, "\x41\xB8") + u32le((uint32_t)op);                /* mov r8d, op        */
                    s += bytes(2, "\x48\xB8") + u64le(fptr);                        /* movabs rax, &fn    */
                    s += bytes(2, "\xFF\xD0");                                      /* call rax           */
                    s += bytes(2, "\x85\xC0");                                      /* test eax, eax      */
                    size_t pjz = s.size();
                    s += bytes(2, "\x0F\x84") + u32le(0);                           /* jz ω  (patch pjz+2)*/
                    size_t pjg = s.size();
                    s += bytes(1, "\xE9") + u32le(0);                              /* jmp γ (patch pjg+1)*/
                    size_t pbeta = s.size();
                    s += bytes(1, "\xE9") + u32le(0);                              /* β: jmp ω           */
                    bin = { { (int)(pjz + 2), (int)(pjg + 1), (int)pbeta, (int)(pbeta + 1) },
                            { _.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p }, { false, false, true, false } };
                    return s;
                }
            }
        }
    }
    return std::string();
}
