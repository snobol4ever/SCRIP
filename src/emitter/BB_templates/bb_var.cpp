/* bb_var.cpp — BB template for IR_VAR (Icon variable read).
   IBB-7 (2026-05-29). Leaf node: pBB->sval = variable name. On α-entry, push the
   variable's value onto the runtime vstack via rt_nv_get(name); on success jmp γ;
   on β re-entry jmp ω (IR_VAR is a non-resumable leaf — single value, no retry).

   Mirrors bb_lit_scalar.cpp IR_LIT_I arm (32-byte push-via-runtime-call), with
   movabs of the name pointer (stable in AST pool) instead of an int64 literal,
   and rt_nv_get instead of rt_push_int.

   Bytes (32 total):
       0  : 48 BF + u64le name_ptr      movabs rdi, name_ptr
       10 : 48 B8 + u64le fn_ptr        movabs rax, &rt_nv_get
       20 : FF D0                       call rax
       22 : E9 + u32le γ_rel32          jmp γ              (patch at 23 → lbl_γ)
       27 : E9 + u32le ω_rel32          β: jmp ω           (β-def at 27, ω patch at 28)
       32 : end
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
void rt_nv_get(const char *name);
int  bb_slot_alloc(IR_t * nd);
int  bb_slot_alloc16(IR_t * nd);
int  bb_varslot_peek(const char * name);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_var_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_VAR");
    const char *name = (pBB && pBB->sval) ? pBB->sval : "";
    /* GZ-7 (GROUND ZERO 3) flat-chain slot model: a variable read copies the named variable's ζ=r12     */
    /* frame slot [r12+off_var] into this box's OWN slot [r12+off] so a consumer reads it by             */
    /* bb_slot_get(this). The variable slot was written by an earlier IR_ASSIGN(name) (bb_varslot keyed   */
    /* by name → both share one slot). 16-byte DESCR copy (v.lo + payload.hi). NO rt_nv_get, NO ring.    */
    if (g_icn_flat_chain && pBB) {
        int voff = bb_varslot_peek(name);
        if (voff >= 0) {
            int off = bb_slot_alloc16(pBB);
            if (MEDIUM_BINARY) {
                /*   0    49 8B 84 24 <u32 voff>      mov rax,[r12+voff]      (read var slot lo eightbyte)    */
                /*   8    49 89 84 24 <u32 off>       mov [r12+off],rax       (write own slot lo)             */
                /*  16    49 8B 84 24 <u32 voff+8>    mov rax,[r12+voff+8]    (read var slot hi eightbyte)    */
                /*  24    49 89 84 24 <u32 off+8>     mov [r12+off+8],rax     (write own slot hi)             */
                /*  32    E9 <rel32 → γ>              jmp γ                   ← γ patch at 33                 */
                /*  37    E9 <rel32 → ω>              β: jmp ω                ← β-def 37, ω patch 38          */
                /*  42    end                                                                                 */
                bin = { {33, 37, 38}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
                return bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)voff)
                     + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)off)
                     + bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)(voff + 8))
                     + bytes(4, "\x49\x89\x84\x24") + u32le((uint32_t)(off + 8))
                     + bytes(1, "\xE9") + u32le(0)
                     + bytes(1, "\xE9") + u32le(0);
            }
            if (MEDIUM_TEXT) {
                return s_1asm(emit_fmt("%s:", _.lbl_α))
                     + s_comment(emit_fmt("# BOX IR_VAR read(\"%s\") [GZ-7 flat-chain var slot → own slot]", name))
                     + s_2asm("mov", emit_fmt("rax, [r12+%d]", voff))
                     + s_2asm("mov", emit_fmt("[r12+%d], rax", off))
                     + s_2asm("mov", emit_fmt("rax, [r12+%d]", voff + 8))
                     + s_2asm("mov", emit_fmt("[r12+%d], rax", off + 8))
                     + s_2asm("jmp", _.lbl_γ)
                     + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
                     + s_2asm("jmp", _.lbl_ω);
            }
        }
    }
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment(emit_fmt("# BOX IR_VAR read(\"%s\") [IBB-7 rt_nv_get push]", name))
             + s_2asm("call",     "rt_nv_get@PLT")
             + s_2asm("jmp",      _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("jmp",      _.lbl_ω);
    }
    if (MEDIUM_BINARY) {
        uint64_t nptr = (uint64_t)(uintptr_t)name;
        uint64_t fptr; { void (*fp)(const char *) = rt_nv_get; fptr = (uint64_t)(uintptr_t)(void*)fp; }
        bin = { {23, 27, 28}, {_.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p}, {false, true, false} };
        return bytes(2, "\x48\xBF") + u64le(nptr)
             + bytes(2, "\x48\xB8") + u64le(fptr)
             + bytes(2, "\xFF\xD0")
             + bytes(1, "\xE9")     + u32le(0)
             + bytes(1, "\xE9")     + u32le(0);
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_var(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_var_str(pBB, bin), bin); }
