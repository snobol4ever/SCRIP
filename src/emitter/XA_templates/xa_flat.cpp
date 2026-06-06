#include <string>
#include "emit_str.h"
#include "../BB_templates/x86_asm.h"
extern "C" {
#include "xa_template_common.h"
#include "../emit_bb.h"
#include "../emit_form.h"
#include "../x86_opcodes.h"
}
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
#define BB_BANNER_RULE_LEN 119
static std::string xa_wired_base(void) {
    std::string s(g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "");
    return s.size() > 3 ? s.substr(0, s.size() - 3) : s;
}
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
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FLAT_PROLOGUE");
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
            extern int g_frame_active;
            if (g_emit.flat_wired) {
                return banner;
            }
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
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_epilogue_str(int & out_site, bb_label_t * & out_lbl, bool & out_def) {
    out_site = 0; out_lbl = nullptr; out_def = false;
    if (PLATFORM_X86) {
        if (MEDIUM_MACRO_DEF) return x86("comment", "# no macro form — XA_FLAT_EPILOGUE");
        if (MEDIUM_BINARY) {
            extern int g_frame_active;
            std::string unwind = g_frame_active ? bytes(2, "\x41\x5C") : bytes(4, "\x48\x83\xC4\x08");
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
            extern int g_frame_active;
            if (g_emit.flat_wired) {
                return std::string(" jmp ") + xa_wired_base() + "_wγ\n"
                     + (g_emit.flat_fail_p && g_emit.flat_fail_p->name ? std::string(g_emit.flat_fail_p->name) + ":\n" : std::string())
                     + " jmp " + xa_wired_base() + "_wω\n";
            }
            if (g_frame_active) {
                return std::string("mov eax, 1\n")
                     + "xor edx, edx\n"
                     + (g_emit.flat_brokered ? std::string("pop rbp\n") : std::string())
                     + "pop r12\n"
                     + "ret\n"
                     + (g_emit.flat_fail_p && g_emit.flat_fail_p->name ? std::string(g_emit.flat_fail_p->name) + ":\n" : std::string())
                     + "# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure\n"
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
/*--------------------------------------------------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_entry_dispatch(void)    { auto s = xa_entry_dispatch_str();    if (!s.empty()) emit_text_n(s.data(), s.size()); }
extern "C" void xa_flat_prologue(void)     { int st; bb_label_t * lb; bool df; auto s = xa_flat_prologue_str(st, lb, df); xa_emit_one(s, st, lb, df); }
extern "C" void xa_flat_epilogue(void)     { int st; bb_label_t * lb; bool df; auto s = xa_flat_epilogue_str(st, lb, df); xa_emit_one(s, st, lb, df); }
extern "C" void xa_flat_data_section(void) { auto s = xa_flat_data_section_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
