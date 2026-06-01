/* bb_binop_concat_lit.cpp — BB box: IR_BINOP CONCAT, both operands IR_LIT_S (RO literals).
   GZ-4 (Icon). Split out of bb_binop.cpp (one-box-one-file). */
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
int  bb_slot_alloc16(IR_t * nd);
DESCR_t str_concat_d(DESCR_t a, DESCR_t b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_binop_concat_lit_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    int64_t op = pBB ? pBB->ival : -1;
    if (op != BINOP_CONCAT) return std::string();
    /* GZ-4-string (GROUND ZERO 3, this session): stackless `s1 || s2` over two READ-ONLY string    */
    /* literal operands — the string analog of GZ-3's RO-int ADD/SUB. Both operands are IR_LIT_S     */
    /* compile-time constants: their bytes (NUL-terminated) are sealed RO data INSIDE this box's own  */
    /* blob (after the jmps, never executed) and their ADDRESSES are read `lea …,[rip+disp]` (disp =  */
    /* emit-time constant, data+access share the blob — NO patch, NO absolute address, NO value       */
    /* stack). str_concat_d(DESCR a, DESCR b) (gen_runtime.c — heap-allocates the joined NUL-term     */
    /* buffer) is the non-stack concat helper: SysV passes a→rdi:rsi, b→rdx:rcx and returns the 16-   */
    /* byte result DESCR in rax:rdx. The result is the box's READ-WRITE state → stored into a per-     */
    /* sequence frame DESCR slot [r12+off] (ζ=r12; 16 bytes via bb_slot_alloc16); the consumer (write) */
    /* recovers off via bb_slot_get and reads the payload ptr at [r12+off+8]. Deterministic single-    */
    /* shot (both operands const) → α computes+stores+jmp γ ; β jmp ω. Non-literal operands fall      */
    /* through to the rt_gen_concat arm below (stackless-abort until its own rung).                    */
    if (MEDIUM_BINARY && pBB && pBB->α && pBB->β && pBB->α->t == IR_LIT_S && pBB->β->t == IR_LIT_S
        && pBB->α->sval && pBB->β->sval) {
        int          off    = bb_slot_alloc16(pBB);
        const char * sa     = pBB->α->sval;
        const char * sb     = pBB->β->sval;
        int          la     = (int)strlen(sa) + 1;
        uint64_t     fptr; { DESCR_t (*fp)(DESCR_t, DESCR_t) = str_concat_d; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        /*   off  bytes                       asm                                                         */
        /*   0    BF 01 00 00 00              mov edi,1            (a.v=DT_S; slen=0 — upper cleared)      */
        /*   5    48 8D 35 <u32 d_a=50>       lea rsi,[rip+d_a]    (rip-base=12; str_a@62; d_a=62-12=50)   */
        /*   12   BA 01 00 00 00              mov edx,1            (b.v=DT_S; slen=0)                      */
        /*   17   48 8D 0D <u32 d_b>          lea rcx,[rip+d_b]    (rip-base=24; str_b@62+la; d_b=38+la)   */
        /*   24   48 B8 <u64 &str_concat_d>   movabs rax,&fn                                               */
        /*   34   FF D0                       call rax             (rax:rdx = result DESCR)                */
        /*   36   49 89 84 24 <u32 off>       mov [r12+off],rax    (DESCR eightbyte0 = v+slen)             */
        /*   44   49 89 94 24 <u32 off+8>     mov [r12+off+8],rdx  (DESCR eightbyte1 = ptr)                */
        /*   52   E9 <rel32 → γ>              jmp γ                ← γ patch at 53                         */
        /*   57   E9 <rel32 → ω>              β: jmp ω             ← β-def 57, ω patch 58                  */
        /*   62   <sa bytes + NUL>            sealed RO operand a  (str_a@62)                              */
        /*   62+la <sb bytes + NUL>           sealed RO operand b  (str_b@62+la)                           */
        bin = { {53, 57, 58}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(1, "\xBF") + u32le(1u)
             + bytes(3, "\x48\x8D\x35") + u32le(50u)
             + bytes(1, "\xBA") + u32le(1u)
             + bytes(3, "\x48\x8D\x0D") + u32le((uint32_t)(38 + la))
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
             + bytes(4, "\x49\x89\x94\x24") + u32le((uint32_t)(off + 8))
             + bytes(1, "\xE9") + u32le(0)
             + bytes(1, "\xE9") + u32le(0)
             + std::string(sa, (size_t)la)
             + std::string(sb, (size_t)(strlen(sb) + 1));
    }
    if (MEDIUM_TEXT && pBB && pBB->α && pBB->β && pBB->α->t == IR_LIT_S && pBB->β->t == IR_LIT_S
        && pBB->α->sval && pBB->β->sval) {
        /* GZ-4 (GROUND ZERO 3), mode-4 TEXT twin of the BINARY str_concat_d arm above: both operands */
        /* are READ-ONLY string literals → emit them NUL-terminated to .rodata, pass each as a 16-byte */
        /* DESCR in the SysV register pair (a={edi=DT_S:rsi=ptr}, b={edx=DT_S:rcx=ptr}), call          */
        /* str_concat_d, and store the returned DESCR (rax:rdx) into the ζ frame slot [r12+off]/[+8]   */
        /* (16 bytes via bb_slot_alloc16). The consumer (write) reads the payload ptr at [r12+off+8].  */
        int          off = bb_slot_alloc16(pBB);
        int          nid = bb_node_id(pBB);
        std::string  la  = emit_fmt(".Lconcat%d_a", nid);
        std::string  lb  = emit_fmt(".Lconcat%d_b", nid);
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment("# BOX IR_BINOP concat [GZ-4 stackless RO-str → ζ slot DESCR]")
             + s_directive(".section .rodata")
             + s_directive(la + ": .string \"" + pBB->α->sval + "\"")
             + s_directive(lb + ": .string \"" + pBB->β->sval + "\"")
             + s_directive(".section .text")
             + s_directive(".intel_syntax noprefix")
             + s_2asm("mov edi,", "1")
             + s_2asm("lea rsi,", "[rip + " + la + "]")
             + s_2asm("mov edx,", "1")
             + s_2asm("lea rcx,", "[rip + " + lb + "]")
             + s_2asm("call",     "str_concat_d@PLT")
             + s_2asm("mov",      emit_fmt("[r12 + %d], rax", off))
             + s_2asm("mov",      emit_fmt("[r12 + %d], rdx", off + 8))
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    return std::string();
}
