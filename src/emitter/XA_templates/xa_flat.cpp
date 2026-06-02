/* xa_flat.cpp — XA templates for entry_dispatch, flat_prologue, flat_epilogue, flat_data_section.
   Driver (codegen_flat_body in emit_bb.c) owns label minting and emit_label_define_bb calls.
   Templates emit only asm text: .global decls, lea r10, cmp/je/jmp, ret, etc.
   NB-3f (2026-05-25): all four bodies are PURE — zero side effects. xa_entry_dispatch /
   xa_flat_prologue / xa_flat_data_section return std::string (TEXT CONCAT); xa_flat_epilogue
   returns TEXT CONCAT or a MEDIUM_BINARY byte-string. TEMPLATE-REVAMP (2026-06-02): the abolished
   bb_bin_t offset-table is gone — each arm reports at most ONE patch site via out-params and a LOCAL
   xa_emit_one walks the bytes (its targets are DRIVER labels flat_β_p/flat_fail_p, not box ports, so
   the bb_emit_x86 J/D port records do not apply). The g_emit_pos bookkeeping (prologue lea r10) and the data-buffer
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
/* xa_flat keeps a LOCAL one-site emit (bb_bin_t ABOLISHED): its patch targets are DRIVER labels        */
/* (g_emit.flat_β_p / flat_fail_p), NOT box α/β/γ/ω ports, so the bb_emit_x86 J/D port records do not    */
/* apply.  Each arm reports at most ONE site via (out_site,out_lbl,out_def); xa_emit_one walks the byte  */
/* string emitting that site exactly as the old bb_emit_asm_result did — no offset table type, no        */
/* function counts bytes for a JUMP (the single site offset is the natural length of the head run).      */
static void xa_emit_one(const std::string & out, int site, bb_label_t * lbl, bool is_def) {
    if (!MEDIUM_BINARY) { if (!out.empty()) emit_text_n(out.data(), out.size()); return; }
    int pos = 0;
    if (lbl) {
        for (; pos < site; pos++) bb_emit_byte((uint8_t)(unsigned char)out[pos]);
        if (is_def) { bb_label_define(lbl); }
        else        { bb_emit_patch_rel32(lbl); pos += 4; }
    }
    for (; pos < (int)out.size(); pos++) bb_emit_byte((uint8_t)(unsigned char)out[pos]);
}
static std::string xa_flat_prologue_str(int & out_site, bb_label_t * & out_lbl, bool & out_def) {
    out_site = 0; out_lbl = nullptr; out_def = false;
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — XA_FLAT_PROLOGUE");
        if (MEDIUM_BINARY) {
            extern int g_frame_active;
            if (g_frame_active) {
                out_site = 20; out_lbl = g_emit.flat_β_p; out_def = false;
                return bytes(2, "\x41\x54")
                     + bytes(3, "\x49\x89\xFC")
                     + bytes(2, "\x49\xBA") + u64le(TEMPLATE_ADDR_DELTA)
                     + bytes(3, "\x83\xFE\x00")
                     + bytes(2, "\x0F\x85") + u32le(0);
            }
            out_site = 19; out_lbl = g_emit.flat_β_p; out_def = false;
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
            /* ICON/SNOBOL4 ONE-REGISTER FRAME (FACT RULE), mode-4 TEXT: when the driver set g_frame_active */
            /* (Icon OR SNOBOL4 mode-4), establish the BB-frame register ζ=r12 from rdi (the frame ptr the  */
            /* C `main` wrapper passes), mirroring the proven MEDIUM_BINARY frame-active arm. `push r12`     */
            /* also supplies the same 8-byte rsp adjustment the legacy `sub rsp,8` did (call alignment       */
            /* unchanged); paired `pop r12` before each ret in the epilogue. `lea r10,[rip+Δ]` is kept       */
            /* (harmless for Icon — its stackless body uses r12+rip-relative — and available for SNOBOL4).   */
            /* The C wrapper always enters fresh (esi=0), so the esi-dispatch and the `jmp β` are dead at    */
            /* the top-level boundary → fall straight through to α_body, avoiding an undefined-β reference   */
            /* for shapes whose body never defines main_β. NON-ICON-frame (g_frame_active==0) keeps the      */
            /* original `lea r10,[rip+Δ]` esi-dispatch verbatim → byte-identical for the non-mode-4 path.    */
            extern int g_frame_active;
            if (g_frame_active) {
                return banner + "push r12\n  mov r12, rdi\n  lea r10, [rip + Δ]\n";
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
static std::string xa_flat_epilogue_str(int & out_site, bb_label_t * & out_lbl, bool & out_def) {
    out_site = 0; out_lbl = nullptr; out_def = false;
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
            std::string fail_half = g_frame_active
                /* GZ-10 PROC FAIL EXIT: write FAILDESCR to frame return slot [r12+0]:[r12+8] so that        */
                /* rt_icn_call_proc_descr reads FAILDESCR when the proc body reaches its failure exit         */
                /* (no `return` executed, or `return` with a failing expression). The caller's cmp eax,99     */
                /* gate then routes to ω. Without this write, frame[0] stays NULVCL (pre-seeded by            */
                /* rt_icn_call_proc_descr), causing silent false-success on proc failure (rung03 bug).        */
                /* 49 C7 84 24 00000000 63000000   mov qword [r12+0], 99  (DT_FAIL=99, slen=0)   12 bytes    */
                /* 49 C7 84 24 08000000 00000000   mov qword [r12+8], 0   (payload=0)             12 bytes    */
                 ? ( bytes(4, "\x49\xC7\x84\x24") + u32le(0u) + u32le(99u)
                   + bytes(4, "\x49\xC7\x84\x24") + u32le(8u) + u32le(0u)
                   + bytes(1, "\xB8") + u32le(99)
                   + bytes(2, "\x31\xD2")
                   + unwind
                   + (g_emit.flat_brokered ? bytes(1, "\x5D") : std::string())
                   + bytes(1, "\xC3") )
                 : ( bytes(1, "\xB8") + u32le(99)
                   + bytes(2, "\x31\xD2")
                   + unwind
                   + (g_emit.flat_brokered ? bytes(1, "\x5D") : std::string())
                   + bytes(1, "\xC3") );
            out_site = (int)succ_half.size(); out_lbl = g_emit.flat_fail_p; out_def = true;
            return succ_half + fail_half;
        }
        {
            /* ICON/SNOBOL4 STACKLESS EPILOGUE (g_frame_active, MEDIUM_TEXT): mirror the BINARY arm above —
               return a constant success (eax=1, edx=0) with NO Σ/r10 dereference, then `pop r12` (paired with
               the prologue's `push r12`). The legacy Σ/[r10] deref below is the non-frame Icon path ONLY; r10 is
               SysV caller-saved, so any box that calls a runtime fn (e.g. rt_nv_assign_str@PLT / rt_write_*)
               leaves r10 clobbered → the deref segfaults. The driver ignores the slab's return value, so the
               deref is vestigial for a frame-active statement BB. */
            extern int g_frame_active;
            if (g_frame_active) {
                return std::string("mov eax, 1\n")
                     + "xor edx, edx\n"
                     + (g_emit.flat_brokered ? std::string("pop rbp\n") : std::string())
                     + "pop r12\n"
                     + "ret\n"
                     + (g_emit.flat_fail_p && g_emit.flat_fail_p->name ? std::string(g_emit.flat_fail_p->name) + ":\n" : std::string())
                     + "# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_icn_call_proc_descr sees failure\n"
                     + "mov dword ptr [r12+0], 99\n"
                     + "mov dword ptr [r12+4], 0\n"
                     + "mov qword ptr [r12+8], 0\n"
                     + "mov eax, 99\n"
                     + "xor edx, edx\n"
                     + (g_emit.flat_brokered ? std::string("pop rbp\n") : std::string())
                     + "pop r12\n"
                     + "ret\n";
            }
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
extern "C" void xa_flat_prologue(void)     { int st; bb_label_t * lb; bool df; auto s = xa_flat_prologue_str(st, lb, df); xa_emit_one(s, st, lb, df); }
extern "C" void xa_flat_epilogue(void)     { int st; bb_label_t * lb; bool df; auto s = xa_flat_epilogue_str(st, lb, df); xa_emit_one(s, st, lb, df); }
extern "C" void xa_flat_data_section(void) { auto s = xa_flat_data_section_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
