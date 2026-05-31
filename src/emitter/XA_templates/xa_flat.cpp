/* xa_flat.cpp — XA templates for entry_dispatch, flat_prologue, flat_epilogue, flat_data_section.
   Driver (codegen_flat_body in emit_bb.c) owns label minting and emit_label_define_bb calls.
   Templates emit only asm text: .global decls, lea r10, cmp/je/jmp, ret, etc.
   NB-3f (2026-05-25): all four bodies are PURE — zero side effects. xa_entry_dispatch /
   xa_flat_prologue / xa_flat_data_section return std::string (TEXT CONCAT); xa_flat_epilogue
   returns TEXT CONCAT or a MEDIUM_BINARY byte-string (no labels/jumps, empty bb_bin_t) routed
   through bb_emit_asm_result. The g_emit_pos bookkeeping (prologue lea r10) and the data-buffer
   flush/reset (data_section) were LIFTED to the driver (codegen_flat_body in emit_bb.c); the
   data-section content is the accumulated g_flat_data_buf sliced as a std::string. No buffer-family,
   no fwrite, no bb_emit_out reads, no global mutation remain in any body. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "xa_template_common.h"
#include "../emit_bb.h"
#include "../emit_form.h"
#include "../x86_opcodes.h"
}
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_entry_dispatch_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — XA_ENTRY_DISPATCH");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            if (!g_is_text) return std::string();
            return std::string("  cmp esi, 0\n")
                 + "  je "  + (g_emit.flat_lbl_α_body ? g_emit.flat_lbl_α_body : "?") + "\n"
                 + "  jmp " + (g_emit.flat_lbl_β      ? g_emit.flat_lbl_β      : "?") + "\n";
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define BB_BANNER_RULE_LEN 119
static std::string xa_flat_prologue_str(bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — XA_FLAT_PROLOGUE");
        if (MEDIUM_BINARY) {
            /* ICON ONE-REGISTER FRAME (FACT RULE): when the Icon driver set g_frame_active, the slab */
            /* establishes the BB-frame register ζ: `push r12` (callee-saved — survives rt_* calls; also  */
            /* provides the SAME 8-byte rsp adjustment as the legacy `sub rsp,8`, so internal call         */
            /* alignment is unchanged) then `mov r12, rdi` (frame base passed as zeta). Paired `pop r12`   */
            /* before each ret in xa_flat_epilogue. R12 is the RATIFIED ζ register (UNIFIED REGISTER       */
            /* LAYOUT, 2026-05-30) — distinct from the broker's r10 and the SM-state r13. Layout: push r12 */
            /* (2) + mov r12,rdi (3) + movabs r10 (10) + cmp esi,0 (3) + jne β (6); jne rel32 bin-site @20. */
            extern int g_frame_active;
            if (g_frame_active) {
                bin = { {20}, {g_emit.flat_β_p}, {false} };
                return bytes(2, "\x41\x54")
                     + bytes(3, "\x49\x89\xFC")
                     + bytes(2, "\x49\xBA") + u64le(TEMPLATE_ADDR_DELTA)
                     + bytes(3, "\x83\xFE\x00")
                     + bytes(2, "\x0F\x85") + u32le(0);
            }
            /* IBB-8 (2026-05-29): the slab is entered via the driver's `call fn(NULL,0)`, so on entry */
            /* rsp%16==8. The prologue pushes nothing, so every internal `call *rax` to a runtime       */
            /* helper is made at rsp%16==8 → callee enters at rsp%16==0, violating the SysV ABI which   */
            /* requires rsp%16==8 at callee entry. Non-SSE helpers tolerate it; fprintf("%g") faults on */
            /* `movaps [rbp-0x80],xmm0`. Fix: `sub rsp,8` here (48 83 EC 08) realigns so internal calls */
            /* land at rsp%16==0, giving callees the correct rsp%16==8 entry. Paired `add rsp,8` runs   */
            /* before each `ret` in xa_flat_epilogue. `sub` runs before the esi-dispatch so BOTH the α   */
            /* fall-through and the β branch carry the adjustment. The jne β rel32 bin-site shifts +4.   */
            bin = { {19}, {g_emit.flat_β_p}, {false} };
            return bytes(4, "\x48\x83\xEC\x08")
                 + bytes(2, "\x49\xBA") + u64le(TEMPLATE_ADDR_DELTA)
                 + bytes(3, "\x83\xFE\x00")
                 + bytes(2, "\x0F\x85") + u32le(0);
        }
        if (MEDIUM_TEXT) {
            if (!g_is_text) return std::string();
            std::string banner;
            if (g_emit.flat_text_externalise) {
                banner = std::string("#") + std::string(BB_BANNER_RULE_LEN, '=') + "\n"
                       + "    .global " + (g_emit.flat_lbl_α    ? g_emit.flat_lbl_α    : "") + "\n"
                       + "    .global " + (g_emit.flat_lbl_β    ? g_emit.flat_lbl_β    : "") + "\n"
                       + "    .global " + (g_emit.flat_lbl_γ ? g_emit.flat_lbl_γ : "") + "\n"
                       + "    .global " + (g_emit.flat_lbl_ω ? g_emit.flat_lbl_ω : "") + "\n";
            }
            return banner
                 + "lea r10, [rip + Δ]\n"
                 + "  cmp esi, 0\n"
                 + "  je "  + (g_emit.flat_lbl_α_body ? g_emit.flat_lbl_α_body : "?") + "\n"
                 + "  jmp " + (g_emit.flat_lbl_β      ? g_emit.flat_lbl_β      : "?") + "\n";
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_epilogue_str(bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — XA_FLAT_EPILOGUE");
        if (MEDIUM_BINARY) {
            /* IBB-8 (2026-05-29): pair the prologue's `sub rsp,8` (call-alignment fix) with `add rsp,8` */
            /* (48 83 C4 08) before each `ret`. Placed before the optional brokered `pop rbp` so the     */
            /* stack is fully unwound regardless of frame shape. succ_half grows by 4 → the fail-label    */
            /* bin-site (succ_half.size()) tracks it automatically.                                       */
            /* ICON ONE-REGISTER FRAME (FACT RULE): when g_frame_active, the prologue did `push r12`  */
            /* (not `sub rsp,8`); restore it symmetrically with `pop r12` (41 5C, 2 bytes) before each    */
            /* ret. R12 is the ratified ζ register. succ_half.size() auto-tracks the size change so the   */
            /* fail-label bin-site stays correct.                                                          */
            extern int g_frame_active;
            std::string unwind = g_frame_active ? bytes(2, "\x41\x5C") : bytes(4, "\x48\x83\xC4\x08");
            /* ICON STACKLESS EPILOGUE (GROUND ZERO 3, g_frame_active): the original success path below   */
            /* dereferences r10 (movsxd rcx,[r10], expecting r10 to still hold Δ from the prologue) and Σ */
            /* to build an SM-style return DESCR. r10 is SysV CALLER-saved, so any box that calls a        */
            /* runtime fn that clobbers it (e.g. rt_write_int_nl -> fprintf("%lld")) leaves r10 garbage →  */
            /* the epilogue then segfaults on [r10]. The driver IGNORES the Icon slab's return value (it    */
            /* does `(void)fn(rt_frame(),0)`), so this Σ/r10 computation is vestigial for Icon. Return a    */
            /* constant success (eax=1, edx=0) with NO dereference — crash-proof against any clobber, and   */
            /* symmetric with the fail-half below. NON-ICON (g_frame_active==0) keeps the original Σ/r10    */
            /* bytes verbatim → SNOBOL4/Prolog output byte-identical.                                       */
            std::string succ_half = g_frame_active
                 ? ( bytes(1, "\xB8") + u32le(1)
                   + bytes(2, "\x31\xD2")
                   + unwind
                   + (g_emit.flat_brokered ? bytes(1, "\x5D") : std::string())
                   + bytes(1, "\xC3") )
                 : ( bytes(2, "\x48\xB9") + u64le(ADDR_SIGMA)
                   + bytes(3, "\x48\x8B\x01")
                   + bytes(3, "\x49\x63\x0A")
                   + bytes(4, "\x48\x8D\x04\x08")
                   + bytes(3, "\x48\x89\xC2")
                   + bytes(1, "\xB8") + u32le(1)
                   + unwind
                   + (g_emit.flat_brokered ? bytes(1, "\x5D") : std::string())
                   + bytes(1, "\xC3") );
            std::string fail_half =
                   bytes(1, "\xB8") + u32le(99)
                 + bytes(2, "\x31\xD2")
                 + unwind
                 + (g_emit.flat_brokered ? bytes(1, "\x5D") : std::string())
                 + bytes(1, "\xC3");
            bin = { {(int)succ_half.size()}, {g_emit.flat_fail_p}, {true} };
            return succ_half + fail_half;
        }
        return std::string("lea rcx, [rip + Σ]\n")
             + "mov rax, [rcx]\n"
             + "movsxd rcx, dword ptr [r10]\n"
             + "lea rax, [rax+rcx]\n"
             + "mov rdx, rax\n"
             + "mov eax, 1\n"
             + (g_emit.flat_brokered ? std::string("pop rbp\n") : std::string())
             + "ret\n"
             + (g_emit.flat_fail_p && g_emit.flat_fail_p->name ? std::string(g_emit.flat_fail_p->name) + ":\n" : std::string())
             + "mov eax, 99\n"
             + "xor edx, edx\n"
             + (g_emit.flat_brokered ? std::string("pop rbp\n") : std::string())
             + "ret\n";
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_data_section_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — XA_FLAT_DATA_SECTION");
        if (MEDIUM_BINARY)    return std::string();
        if (MEDIUM_TEXT) {
            if (!g_flat_data_any) return std::string();
            return std::string("  .section .data\n")
                 + std::string(g_flat_data_buf, g_flat_data_len)
                 + "  .section .text\n";
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_entry_dispatch(void)    { auto s = xa_entry_dispatch_str();    if (!s.empty()) emit_text_n(s.data(), s.size()); }
extern "C" void xa_flat_prologue(void)     { bb_bin_t bin; auto s = xa_flat_prologue_str(bin); bb_emit_asm_result(s, bin); }
extern "C" void xa_flat_epilogue(void)     { bb_bin_t bin; auto s = xa_flat_epilogue_str(bin); bb_emit_asm_result(s, bin); }
extern "C" void xa_flat_data_section(void) { auto s = xa_flat_data_section_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
