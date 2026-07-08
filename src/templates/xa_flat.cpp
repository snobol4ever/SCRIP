#include <string>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
#include "../emitter/emit.h"
}
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_entry_dispatch_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_ENTRY_DISPATCH");
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
#define BB_BANNER_RULE_LEN 119
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_wired_base(void) {
    std::string s(g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "");
    return s.size() > 3 ? s.substr(0, s.size() - 3) : s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Frame-register byte fragments (2026-07-08 session 2, ZC_FRAME switch).  These five helpers are the ONLY
 * frame-dependent machine bytes in this file; everything routes through x86_zr_num() so the r12 default is
 * byte-identical to the former hard-coded \x41\x54 / \x49\x89\xFC / \x4D\x89\xE5-family / \x41\x5C /
 * \x49\xC7\x84\x24 forms, and ZC_FRAME_RBP gets 0x55 / 48 89 FD / 49 89 ED-family / 0x5D / 48 C7 85 (shorter:
 * no REX.B on push/pop, no SIB on the store).  Patch-site offsets below are COMPUTED from the built string's
 * actual length, never hand-counted — hand counts break the moment fragment sizes become frame-dependent
 * (and hand-counting bytes is this repo's named bb_bin_t-era debt anyway). */
static std::string xaf_push_frame(void)  { int z = x86_zr_num(); std::string c; if (z >= 8) c += (char)0x41; c += (char)(0x50 | (z & 7)); return c; }
static std::string xaf_pop_frame(void)   { int z = x86_zr_num(); std::string c; if (z >= 8) c += (char)0x41; c += (char)(0x58 | (z & 7)); return c; }
static std::string xaf_mov_frame_rdi(void) { int z = x86_zr_num(); std::string c; c += (char)(0x48 | (z >= 8 ? 0x01 : 0x00)); c += (char)0x89; c += (char)(0xC0 | (7 << 3) | (z & 7)); return c; }
static std::string xaf_mov_dreg_frame(int d) { int z = x86_zr_num(); std::string c; c += (char)(0x48 | (z >= 8 ? 0x04 : 0x00) | (d >= 8 ? 0x01 : 0x00)); c += (char)0x89; c += (char)(0xC0 | ((z & 7) << 3) | (d & 7)); return c; }
static std::string xaf_frame_store_imm32(uint32_t disp, uint32_t imm) {
    int z = x86_zr_num(), lo = z & 7; std::string c;
    c += (char)(0x48 | (z >= 8 ? 0x01 : 0x00)); c += (char)0xC7;
    c += (char)(0x80 | (lo == 4 ? 4 : lo)); if (lo == 4) c += (char)0x24;
    c += u32le(disp); c += u32le(imm); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_prologue_str(int & out_site, bb_label_t * & out_lbl, bool & out_def) {
    out_site = 0; out_lbl = nullptr; out_def = false;
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FLAT_PROLOGUE");
        if (MEDIUM_BINARY) {
            extern int g_frame_active;
            if (g_frame_active) {
                extern int g_emit_frame_caller_dl;
                std::string disp;
                if      (g_emit_frame_caller_dl == 1) disp = bytes(2, "\x41\x55") + xaf_mov_dreg_frame(13) + bytes(4, "\x48\x83\xEC\x08");
                else if (g_emit_frame_caller_dl == 2) disp = bytes(2, "\x41\x56") + xaf_mov_dreg_frame(14) + bytes(4, "\x48\x83\xEC\x08");
                else if (g_emit_frame_caller_dl == 3) disp = bytes(2, "\x41\x57") + xaf_mov_dreg_frame(15) + bytes(4, "\x48\x83\xEC\x08");
                std::string r = xaf_push_frame()
                              + xaf_mov_frame_rdi()
                              + disp
                              + bytes(3, "\x83\xFE\x00")
                              + bytes(2, "\x0F\x85") + u32le(0);
                out_site = (int)r.size() - 4; out_lbl = g_emit.flat_β_p; out_def = false;
                return r;
            }
            out_site = 9; out_lbl = g_emit.flat_β_p; out_def = false;
            return bytes(4, "\x48\x83\xEC\x08")
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
            extern int g_frame_active;
            extern int g_gen_proc_active;
            if (g_emit.flat_wired) {
                return banner;
            }
            if (g_frame_active) {
                extern int g_emit_frame_caller_dl;
                const char *dreg = (g_emit_frame_caller_dl == 1) ? "r13" : (g_emit_frame_caller_dl == 2) ? "r14" : (g_emit_frame_caller_dl == 3) ? "r15" : (const char *)0;
                std::string disp = dreg ? (std::string("  push ") + dreg + "\n  mov " + dreg + ", " + x86_zr() + "\n  sub rsp, 8\n") : std::string();
                std::string pro = banner + "push " + std::string(x86_zr()) + "\n  mov " + x86_zr() + ", rdi\n" + disp;
                if (g_gen_proc_active)
                    pro += std::string("  cmp esi, 0\n")
                         + "  jne " + (g_emit.flat_lbl_β ? g_emit.flat_lbl_β : "?") + "\n";
                return pro;
            }
            return banner
                 + "  cmp esi, 0\n"
                 + "  je "  + (g_emit.flat_lbl_α_body ? g_emit.flat_lbl_α_body : "?") + "\n"
                 + "  jmp " + (g_emit.flat_lbl_β      ? g_emit.flat_lbl_β      : "?") + "\n";
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_epilogue_str(int & out_site, bb_label_t * & out_lbl, bool & out_def, std::string * out_succ, std::string * out_fail) {
    out_site = 0; out_lbl = nullptr; out_def = false;
    if (out_succ) out_succ->clear();
    if (out_fail) out_fail->clear();
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FLAT_EPILOGUE");
        if (MEDIUM_BINARY) {
            extern int g_frame_active;
            extern int g_emit_frame_caller_dl;
            std::string dpop;
            if      (g_frame_active && g_emit_frame_caller_dl == 1) dpop = bytes(4, "\x48\x83\xC4\x08") + bytes(2, "\x41\x5D");
            else if (g_frame_active && g_emit_frame_caller_dl == 2) dpop = bytes(4, "\x48\x83\xC4\x08") + bytes(2, "\x41\x5E");
            else if (g_frame_active && g_emit_frame_caller_dl == 3) dpop = bytes(4, "\x48\x83\xC4\x08") + bytes(2, "\x41\x5F");
            std::string unwind = g_frame_active ? (dpop + xaf_pop_frame()) : bytes(4, "\x48\x83\xC4\x08");
            std::string succ_half = g_frame_active
                 ? ( bytes(1, "\xB8") + u32le(1)
                   + bytes(2, "\x31\xD2")
                   + unwind
                   + bytes(1, "\xC3") )
                 : ( bytes(2, "\x48\xB9") + u64le(ADDR_SIGMA)
                   + bytes(3, "\x48\x8B\x01")
                   + bytes(3, "\x49\x63\xCE")
                   + bytes(4, "\x48\x8D\x04\x08")
                   + bytes(3, "\x48\x89\xC2")
                   + bytes(1, "\xB8") + u32le(1)
                   + unwind
                   + bytes(1, "\xC3") );
            std::string fail_half = g_frame_active
                 ? ( xaf_frame_store_imm32(0u, 99u)
                   + xaf_frame_store_imm32(8u, 0u)
                   + bytes(1, "\xB8") + u32le(99)
                   + bytes(2, "\x31\xD2")
                   + unwind
                   + bytes(1, "\xC3") )
                 : ( bytes(1, "\xB8") + u32le(99)
                   + bytes(2, "\x31\xD2")
                   + unwind
                   + bytes(1, "\xC3") );
            out_site = (int)succ_half.size(); out_lbl = g_emit.flat_fail_p; out_def = true;
            if (out_succ) *out_succ = succ_half;
            if (out_fail) *out_fail = fail_half;
            return succ_half + fail_half;
        }
        {
            extern int g_frame_active;
            if (g_emit.flat_wired) {
                return std::string(" jmp ") + xa_wired_base() + "_wγ\n"
                     + (g_emit.flat_fail_p && g_emit.flat_fail_p->name ? std::string(g_emit.flat_fail_p->name) + ":\n" : std::string())
                     + " jmp " + xa_wired_base() + "_wω\n";
            }
            if (g_frame_active) {
                extern int g_emit_frame_caller_dl;
                const char *dreg = (g_emit_frame_caller_dl == 1) ? "r13" : (g_emit_frame_caller_dl == 2) ? "r14" : (g_emit_frame_caller_dl == 3) ? "r15" : (const char *)0;
                std::string dpop = dreg ? (std::string("add rsp, 8\npop ") + dreg + "\n") : std::string();
                std::string succ_half = std::string("mov eax, 1\n")
                     + "xor edx, edx\n"
                     + dpop
                     + "pop " + x86_zr() + "\n"
                     + "ret\n";
                std::string fail_half = (g_emit.flat_fail_p && g_emit.flat_fail_p->name ? std::string(g_emit.flat_fail_p->name) + ":\n" : std::string())
                     + "# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure\n"
                     + "mov dword ptr [" + x86_zr() + "+0], 99\n"
                     + "mov dword ptr [" + x86_zr() + "+4], 0\n"
                     + "mov qword ptr [" + x86_zr() + "+8], 0\n"
                     + "mov eax, 99\n"
                     + "xor edx, edx\n"
                     + dpop
                     + "pop " + x86_zr() + "\n"
                     + "ret\n";
                if (out_succ) *out_succ = succ_half;
                if (out_fail) *out_fail = fail_half;
                return succ_half + fail_half;
            }
        }
        return std::string("lea rcx, [rip + Σ]\n")
             + "mov rax, [rcx]\n"
             + "movsxd rcx, r14d\n"
             + "lea rax, [rax+rcx]\n"
             + "mov rdx, rax\n"
             + "mov eax, 1\n"
             + "ret\n"
             + (g_emit.flat_fail_p && g_emit.flat_fail_p->name ? std::string(g_emit.flat_fail_p->name) + ":\n" : std::string())
             + "mov eax, 99\n"
             + "xor edx, edx\n"
             + "ret\n";
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_data_section_str(void) {
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FLAT_DATA_SECTION");
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* xa_flat_prologue/epilogue: the mark/release_to calls are emitted via bb_emit_x86 (the ordinary template
 * dispatch consumer, tag-decodes x86()'s L/J/D/E/F/X record stream) as SEPARATE statements around the
 * existing xa_emit_one call (which still only ever sees the original, unmodified raw prologue/epilogue
 * bytes it always has) — never spliced into the string xa_emit_one patches. This keeps xa_emit_one's own
 * patch-site byte-offset arithmetic completely untouched by this rung, and keeps zero manual MEDIUM_* checks
 * in this file: bb_emit_x86 and xa_emit_one each already dispatch on medium internally, exactly once, inside
 * their own bodies — this wrapper just sequences calls, it never inspects MEDIUM_* itself. */
extern "C" void xa_flat_prologue(void) {
    int st; bb_label_t * lb; bool df;
    auto s = xa_flat_prologue_str(st, lb, df);
    xa_emit_one(s, st, lb, df);
    extern int g_frame_active;
    if (g_frame_active && g_emit_cfg && g_emit_cfg->zeta_mark_slot >= 0) bb_emit_x86(x86_zeta_mark_call(g_emit_cfg->zeta_mark_slot));
}
extern "C" void xa_flat_epilogue(void) {
    int st; bb_label_t * lb; bool df; std::string succ, fail;
    auto s = xa_flat_epilogue_str(st, lb, df, &succ, &fail);
    extern int g_frame_active;
    bool have_halves = g_frame_active && g_emit_cfg && g_emit_cfg->zeta_mark_slot >= 0 && (!succ.empty() || !fail.empty());
    if (!have_halves) { xa_emit_one(s, st, lb, df); return; }
    int off = g_emit_cfg->zeta_mark_slot;
    xa_emit_one(succ, 0, nullptr, false);
    bb_emit_x86(x86_zeta_release_to_call(off));
    xa_emit_one(fail, 0, lb, df);
}
extern "C" void xa_flat_data_section(void) { auto s = xa_flat_data_section_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
