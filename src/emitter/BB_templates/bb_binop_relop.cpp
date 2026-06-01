/* bb_binop_relop.cpp — BB box: IR_BINOP numeric relop, stackless slot→slot cmp.
   GZ-8 (Icon). Split out of bb_binop.cpp (one-box-one-file). */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "SM.h"
#include "ast.h"
#include "descr.h"
#include "../../runtime/interp/gen.h"
int  bb_slot_get(IR_t * nd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rel_is_numrel(int64_t op) { return op >= BINOP_LT && op <= BINOP_NE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rel_fail_jcc(int64_t op, const char **mnem) {
    switch (op) {
    case BINOP_LT: *mnem = "jge"; return 0x8D;
    case BINOP_LE: *mnem = "jg";  return 0x8F;
    case BINOP_GT: *mnem = "jle"; return 0x8E;
    case BINOP_GE: *mnem = "jl";  return 0x8C;
    case BINOP_EQ: *mnem = "jne"; return 0x85;
    case BINOP_NE: *mnem = "je";  return 0x84;
    default:       *mnem = "jmp"; return 0x00;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_relop_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    int64_t op = pBB ? pBB->ival : -1;
    if (g_icn_flat_chain && pBB && rel_is_numrel(op) && pBB->α && pBB->β) {
        int sa = bb_slot_get(pBB->α);
        int sb = bb_slot_get(pBB->β);
        if (sa >= 0 && sb >= 0) {
            const char *mnem = "jmp";
            int jcc2 = rel_fail_jcc(op, &mnem);
            if (MEDIUM_BINARY) {
                /*   off  bytes                       asm                                                    */
                /*   0    49 8B 84 24 <u32 sa+8>      mov rax,[r12+sa+8]   (lv int payload)                  */
                /*   8    49 8B 8C 24 <u32 sb+8>      mov rcx,[r12+sb+8]   (rv int payload)                  */
                /*   16   48 39 C8                    cmp rax, rcx                                            */
                /*   19   0F 8X <rel32 → ω>           jcc ω (negated rel → fail/else)  ← ω patch at 21        */
                /*   25   E9 <rel32 → γ>              jmp γ (rel true → succeed/then)  ← γ patch at 26        */
                /*   30   E9 <rel32 → ω>              β: jmp ω             ← β-def 30, ω patch 31             */
                /*   35   end                                                                                 */
                char jb[2]; jb[0] = (char)0x0F; jb[1] = (char)jcc2;
                bin = { {21, 26, 30, 31}, {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, false, true, false} };
                return bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)(sa + 8))
                     + bytes(4, "\x49\x8B\x8C\x24") + u32le((uint32_t)(sb + 8))
                     + bytes(3, "\x48\x39\xC8")
                     + bytes(2, jb)                 + u32le(0)
                     + bytes(1, "\xE9")             + u32le(0)
                     + bytes(1, "\xE9")             + u32le(0);
            }
            if (MEDIUM_TEXT) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment(emit_fmt("# BOX IR_BINOP relop [GZ-8 stackless cmp+%s→ω; jmp γ]", mnem))
                     + s_2asm("mov", emit_fmt("rax, [r12+%d]", sa + 8))
                     + s_2asm("mov", emit_fmt("rcx, [r12+%d]", sb + 8))
                     + s_2asm("cmp", "rax, rcx")
                     + s_2asm(mnem,  _.lbl_ω)
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
            }
        }
    }
    return std::string();
}
