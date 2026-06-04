/* bb_if.cpp — BB template for BB_IF (Icon if/while condition router).
   IBB-8b (Opus 4.8, 2026-05-29). AG-pure shape only (α==β==NULL; γ=then-entry, ω=else-entry).

   The condition expression ran ahead in the chain; its tail relop (bb_binop.cpp relop arm) called
   rt_acomp/rt_lcomp which pushed the comparison result onto the vstack and set the LAST_OK flag,
   then jmp'd here.  This router:
     1. rt_pop_void  — discard the condition value (then/else bodies do not consume it; matches the
        mode-2 ring model where the value is peeked, not popped, but on the native vstack it must be
        balanced so it does not accumulate across statements).
     2. rt_last_ok   — fetch the comparison flag into eax.
     3. test eax,eax ; jz ω (else) ; jmp γ (then).

   Mirrors the proven relop-branch byte pattern in bb_binop_gen.cpp (rt_last_ok + test + jcc). */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
void rt_pop_void(void);
int  rt_last_ok(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_if_str(BB_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — BB_IF");
    (void)pBB;
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment("# BOX BB_IF (discard cond value, read LAST_OK, branch then/else)")
             + s_2asm("call", "rt_pop_void@PLT")
             + s_2asm("call", "rt_last_ok@PLT")
             + s_2asm("test", "eax, eax")
             + s_2asm("jz",   _.lbl_ω)
             + s_2asm("jmp",  _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",  _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        /*  off  bytes                       asm                                                       */
        /*   0   48 B8 <u64 &rt_pop_void>    movabs rax, &rt_pop_void                                  */
        /*  10   FF D0                       call rax                                                  */
        /*  12   48 B8 <u64 &rt_last_ok>     movabs rax, &rt_last_ok                                   */
        /*  22   FF D0                       call rax                                                  */
        /*  24   85 C0                       test eax, eax                                             */
        /*  26   0F 84 <rel32 → ω>           jz ω             ← ω patch at offset 28                   */
        /*  32   E9 <rel32 → γ>              jmp γ            ← γ patch at offset 33                   */
        /*  37   E9 <rel32 → ω>              β: jmp ω         ← β-def at 37, ω patch at 38             */
        /*  42   end                                                                                    */
        uint64_t fpv;  { void (*fp)(void) = rt_pop_void; fpv  = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t flok; { int  (*fp)(void) = rt_last_ok; flok = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {28, 33, 37, 38},
                {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
                {false, false, true, false} };
        return bytes(2, "\x48\xB8") + u64le(fpv)
             + bytes(2, "\xFF\xD0")
             + bytes(2, "\x48\xB8") + u64le(flok)
             + bytes(2, "\xFF\xD0")
             + bytes(2, "\x85\xC0")
             + bytes(2, "\x0F\x84") + u32le(0)
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_if(BB_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_if_str(pBB, bin), bin);
}
