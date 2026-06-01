/* bb_binop_concat_slot.cpp — BB box: IR_BINOP CONCAT, both operands producer slots.
   GZ-11+ (Icon/Raku). Split out of bb_binop.cpp (one-box-one-file). */
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
int  bb_slot_alloc16(IR_t * nd);
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_concat_slot_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    int64_t op = pBB ? pBB->ival : -1;
    if (op != BINOP_CONCAT) return std::string();
    /* GZ-11+ (slot concat, this session): stackless `s1 || s2` where the operands are PRODUCER BOXES   */
    /* already in the flat-chain (IR_VAR / IR_LIT_S / nested concat) — the general case of GZ-4's RO-    */
    /* literal arm. Each operand wrote a 16-byte DESCR into its own ζ=r12 slot; this box loads operand a */
    /* (lo→rdi, hi→rsi) and operand b (lo→rdx, hi→rcx) from [r12+slot], calls str_concat_d by value      */
    /* (SysV: a=rdi:rsi, b=rdx:rcx → result DESCR rax:rdx), and stores the result into its OWN slot      */
    /* [r12+off]/[+8] (bb_slot_alloc16) for the consumer (write/assign/nested concat) to read by         */
    /* bb_slot_get. NO value stack — the test_icon.c named-slot model, same register convention as the   */
    /* GZ-4 RO arm but reading operand slots instead of sealed [rip+disp] addresses. Handles `s||" w"`,  */
    /* `a||b||"!"` (the inner concat is itself a slot producer). Both operands MUST have slots; if either */
    /* lacks one (e.g. a generator operand) this declines and the RO-literal / rt_gen_concat arms below  */
    /* handle it. Grounded in Icon ofncs.r cat / the str_concat_d heap-join helper.                      */
    if (g_icn_flat_chain && pBB && pBB->α && pBB->β) {
        int sa = bb_slot_get(pBB->α);
        int sb = bb_slot_get(pBB->β);
        if (sa >= 0 && sb >= 0) {
            int off = bb_slot_alloc16(pBB);
            uint64_t fptr; { DESCR_t (*fp)(DESCR_t, DESCR_t) = str_concat_d; fptr = (uint64_t)(uintptr_t)(void*)fp; }
            if (MEDIUM_BINARY) {
                /*   0    49 8B BC 24 <u32 sa>        mov rdi,[r12+sa]     (a DESCR lo)                    */
                /*   8    49 8B B4 24 <u32 sa+8>      mov rsi,[r12+sa+8]   (a DESCR hi)                    */
                /*  16    49 8B 94 24 <u32 sb>        mov rdx,[r12+sb]     (b DESCR lo)                    */
                /*  24    49 8B 8C 24 <u32 sb+8>      mov rcx,[r12+sb+8]   (b DESCR hi)                    */
                /*  32    48 B8 <u64 &str_concat_d>   movabs rax,&fn                                       */
                /*  42    FF D0                       call rax             (rax:rdx = result DESCR)         */
                /*  44    49 89 84 24 <u32 off>       mov [r12+off],rax    (result lo)                     */
                /*  52    49 89 94 24 <u32 off+8>     mov [r12+off+8],rdx  (result hi)                     */
                /*  60    E9 <rel32 → γ>              jmp γ                ← γ patch at 61                 */
                /*  65    E9 <rel32 → ω>              β: jmp ω             ← β-def 65, ω patch 66          */
                bin = { {61, 65, 66}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(4, "\x49\x8B\xBC\x24") + u32le((uint32_t)sa)
                     + bytes(4, "\x49\x8B\xB4\x24") + u32le((uint32_t)(sa + 8))
                     + bytes(4, "\x49\x8B\x94\x24") + u32le((uint32_t)sb)
                     + bytes(4, "\x49\x8B\x8C\x24") + u32le((uint32_t)(sb + 8))
                     + bytes(2, "\x48\xB8") + u64le(fptr)
                     + bytes(2, "\xFF\xD0")
                     + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
                     + bytes(4, "\x49\x89\x94\x24") + u32le((uint32_t)(off + 8))
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0);
            }
            if (MEDIUM_TEXT) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment("# BOX IR_BINOP concat [GZ-11+ stackless slot→slot DESCR]")
                     + s_2asm("mov", emit_fmt("rdi, [r12+%d]", sa))
                     + s_2asm("mov", emit_fmt("rsi, [r12+%d]", sa + 8))
                     + s_2asm("mov", emit_fmt("rdx, [r12+%d]", sb))
                     + s_2asm("mov", emit_fmt("rcx, [r12+%d]", sb + 8))
                     + s_2asm("call", "str_concat_d@PLT")
                     + s_2asm("mov", emit_fmt("[r12+%d], rax", off))
                     + s_2asm("mov", emit_fmt("[r12+%d], rdx", off + 8))
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
            }
        }
    }
    return std::string();
}
