#include <string>
#include <cstdlib>
#include "emit.h"
#include "x86_asm.h"
extern "C" {
#include "xa_template_common.h"
#include "../emitter/emit.h"
}
#define ADDR_SIGMA   ((uint64_t)(uintptr_t)&Σ)
extern "C" void rt_jmp_frame_lexprep(void *, long);
extern "C" void rt_jmp_frame_lexprep2(void *, long, long);
extern "C" void rt_pl_zf_resume_clear(void);
extern "C" void *g_pl_zf_pending_cursor;
extern "C" void rt_main_args_fetch(void);
extern "C" void rt_gen_save_wires(void *gen_fb, void *gw, void *ww);
extern "C" void *rt_gen_get_gamma_wire(void *gen_fb);
extern "C" void *rt_gen_get_omega_wire(void *gen_fb);
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
static std::string xaf_seprule(char ch) { static int _sep = -1; if (_sep < 0) { const char *e = getenv("SCRIP_ASM_SEP"); _sep = (e && *e == '0') ? 0 : 1; } return _sep ? std::string("#") + std::string(BB_BANNER_RULE_LEN, ch) + "\n" : std::string(); }
static std::string xaf_ω_label(void) { return (g_emit.flat_fail_p && g_emit.flat_fail_p->name) ? xaf_seprule('-') + g_emit.flat_fail_p->name + ":\n" : std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_wired_base(void) {
    std::string s(g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "");
    return s.size() > 3 ? s.substr(0, s.size() - 3) : s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void xa_emit_one(const std::string & out, int site, bb_label_t * lbl, bool is_def) {
    if (site < 0) { bb_emit_x86(out); return; }
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
static std::string xaf_push_frame(void)  { int z = x86_zr_num(); std::string c; if (z >= 8) c += (char)0x41; c += (char)(0x50 | (z & 7)); return c; }
static std::string xaf_pop_frame(void)   { int z = x86_zr_num(); std::string c; if (z >= 8) c += (char)0x41; c += (char)(0x58 | (z & 7)); return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_ld64_from_zr8(int dst, int disp8) {
    int z = x86_zr_num(); std::string c; uint8_t rex = 0x48; if (dst >= 8) rex |= 0x04; if (z >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8B;
    int lo = z & 7; c += (char)(0x40 | ((dst & 7) << 3) | (lo == 4 ? 4 : lo)); if (lo == 4) c += (char)0x24; c += (char)(disp8 & 0xFF); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_lea_rsp_zr32(int disp32) {
    int z = x86_zr_num(); std::string c; uint8_t rex = 0x48; if (z >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8D;
    int lo = z & 7; c += (char)(0x80 | (4 << 3) | (lo == 4 ? 4 : lo)); if (lo == 4) c += (char)0x24; c += u32le((uint32_t)disp32); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_frame_store_imm32(uint32_t disp, uint32_t imm) {
    int z = x86_zr_num(), lo = z & 7; std::string c;
    c += (char)(0x48 | (z >= 8 ? 0x01 : 0x00)); c += (char)0xC7;
    c += (char)(0x80 | (lo == 4 ? 4 : lo)); if (lo == 4) c += (char)0x24;
    c += u32le(disp); c += u32le(imm); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xaf_anchor_off(void) {
    if (!g_emit_cfg || g_emit_cfg->zeta_mark_slot < 0) { fprintf(stderr, "FATAL xa_flat: frame-active graph with no zeta_mark slot -- the graph-scope anchor has no home (zls_build did not run?)\n"); abort(); }
    return g_emit_cfg->zeta_mark_slot + 8;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xaf_outer_frame_k(void) {
    if (g_emit.flat_all_zd && !g_emit.flat_jmp_entry && !g_emit.flat_pat && !g_emit.flat_gen && x86_zc_frame() == ZC_FRAME_RSP) return 8;
    int rg = g_emit_cfg ? g_emit_cfg->jcon_value_region : 0;
    return ((rg + 15) & ~15) + 8;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_frame_rsp_rm(uint32_t disp, unsigned char op) {
    int z = x86_zr_num(), lo = z & 7; std::string c;
    c += (char)(0x48 | (z >= 8 ? 0x01 : 0x00)); c += (char)op;
    c += (char)(0x80 | (4 << 3) | (lo == 4 ? 4 : lo)); if (lo == 4) c += (char)0x24;
    c += u32le(disp); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_ld64_rsp(int reg, int disp) {
    std::string c; c += (char)(0x48 | (reg >= 8 ? 0x04 : 0x00)); c += (char)0x8B; int lo = reg & 7;
    if (disp == 0)                        { c += (char)(0x00 | (lo << 3) | 4); c += (char)0x24; }
    else if (disp >= -128 && disp <= 127) { c += (char)(0x40 | (lo << 3) | 4); c += (char)0x24; c += (char)(unsigned char)disp; }
    else                                  { c += (char)(0x80 | (lo << 3) | 4); c += (char)0x24; c += u32le((uint32_t)disp); }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_addq_rsp(int imm) {
    if (imm >= -128 && imm <= 127) { std::string c = bytes(3, "\x48\x83\xC4"); c += (char)(unsigned char)imm; return c; }
    return bytes(3, "\x48\x81\xC4") + u32le((uint32_t)imm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xaf_shed_np(void) { static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_SHED_NP"); on = (e && *e == '0') ? 0 : 1; } return on; }
static int xaf_deep(void) { return g_emit.flat_deep_arrival || (!xaf_shed_np() && g_emit.flat_outer_nparams >= 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_anchor_leave_bin(void) {
    if (!x86_port_cstack()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP && !xaf_deep()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP) return bytes(3, "\x48\x89\xEC");
    return xaf_frame_rsp_rm((uint32_t)xaf_anchor_off(), 0x8B);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_anchor_leave_text(void) {
    if (!x86_port_cstack()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP && !xaf_deep()) return std::string();
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string();
    char b[160]; snprintf(b, sizeof b, "mov rsp, qword ptr [%s + %d]\n", x86_zr(), xaf_anchor_off()); return std::string(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xaf_add_rdi_imm_bin(int n) { return (n >= -128 && n <= 127) ? bytes(3, "\x48\x83\xC7") + std::string(1, (char)(unsigned char)n) : bytes(3, "\x48\x81\xC7") + u32le((uint32_t)n); }
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
extern "C" void xa_flat_data_section(void) { auto s = xa_flat_data_section_str(); if (!s.empty()) emit_text_n(s.data(), s.size()); }
extern "C" void rt_pl_dc_prep(void *, long, long, long, long, long);
extern "C" DESCR_t rt_pl_dc_leave_γ(DESCR_t, long, void *);
extern "C" DESCR_t rt_pl_dc_leave_ω(long, void *);
extern "C" void rt_arg_stage(int idx, DESCR_t v);
extern "C" void rt_icn_zframe_args_install(void *, int, int);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_dc_stub_str(void) {
    if (!PLATFORM_X86) return std::string();
    x86_begin();
    int kt = g_emit.flat_frame_bytes;
    int anchor = -1;
    int suffix = (g_emit.flat_seed_off >= 16) ? g_emit.flat_seed_off : 16;
    extern int g_flat_dc_np;
    int np = g_flat_dc_np;
    uint64_t prep_fp;  { void (*fp)(void *, long, long, long, long, long) = rt_pl_dc_prep; prep_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t lvg_fp;   { DESCR_t (*fp)(DESCR_t, long, void *) = rt_pl_dc_leave_γ; lvg_fp = (uint64_t)(uintptr_t)(void *)fp; }
    uint64_t lvw_fp;   { DESCR_t (*fp)(long, void *) = rt_pl_dc_leave_ω; lvw_fp = (uint64_t)(uintptr_t)(void *)fp; }
    static const char *argreg[4] = { "rsi", "rdx", "rcx", "r8" };
    if (g_emit.zframe_graph || (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc)) {
        static const char *dcarg4[4] = { "rsi", "rdx", "rcx", "r8" };
        uint64_t stg_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stg_fp = (uint64_t)(uintptr_t)(void *)fp; }
        bool need_align_pad = (np > 0) && (np % 2 == 1);
        std::string zs = x86("comment", "ICN-FR-3 zframe dc stub: stage args, jmp proc_f_α≡0 with wire shims")
            + x86("pop", "r11")
            + x86("push", "r11")
            + x86("push", "r11");
        if (need_align_pad) zs += x86("push", "r11");
        for (int i = np - 1; i >= 0; i--) zs += x86("push", dcarg4[i]);
        int push_bytes = np * 8 + (need_align_pad ? 8 : 0);
        for (int i = 0; i < np; i++) {
            zs += x86("mov", "r10", "[rsp + " + std::to_string(i * 8) + "]")
                + x86("mov32", "edi", (long)i)
                + x86("mov", "rsi", "[r10 + 0]")
                + x86("mov", "rdx", "[r10 + 8]")
                + x86("call", "rt_arg_stage", stg_fp);
        }
        if (push_bytes > 0) zs += x86("add", "rsp", (long)push_bytes);
        zs += x86_lea_id("rcx", 2)
            + x86_lea_id("rdx", 3)
            + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_lbl_α ? g_emit.flat_lbl_α : "?")
            + x86_deflabel_id(2)
            + x86("pop", "r11")
            + x86("pop", "r11")
            + x86("jmp", "r11")
            + x86_deflabel_id(3)
            + x86("pop", "r11")
            + x86("pop", "r11")
            + x86("mov32", "eax", 104L)
            + x86("xor", "edx", "edx")
            + x86("jmp", "r11");
        return zs;
    }
    return x86("comment", "PL-DC direct-call entry: retaddr -> kt-32 pad, wires -> local ret-shims, one prep crossing, shared body")
         + x86("pop", "r11")
         + x86("sub", "rsp", (long)(kt + 16))
         + x86_rsp_store64(kt - 8, "rsp")
         + std::string("")
         + x86("mov", FRQ(kt - 32), "r11")
         + x86_lea_id("rax", 2)
         + x86("mov", FRQ(kt - 24), "rax")
         + x86_lea_id("rax", 3)
         + x86("mov", FRQ(kt - 16), "rax")
         + IF(anchor >= 0, x86("mov", FRQ(anchor), "rsp"))
         + FOR(0, np, [&](int i) { return x86("mov", FRQ(16 + 8 * i), argreg[i]); })
         + x86("mov", "rdi", "rsp")
         + x86("mov32", "esi", (long)suffix)
         + x86("mov32", "edx", (long)(kt - 32))
         + x86("mov32", "ecx", (long)np)
         + x86("mov32", "r8d", (long)np)
         + x86("mov32", "r9d", 0L)
         + x86("call", "rt_pl_dc_prep", prep_fp)
         + x86_jmp_lblptr(g_emit.flat_dc_body_p, g_emit.flat_lbl_α_body ? g_emit.flat_lbl_α_body : "?")
         + x86_deflabel_id(2)
         + x86_rsp_load64("rdx", 0)
         + x86("mov", "rcx", "rsp")
         + x86("add", "rcx", (long)(-kt))
         + x86_rsp_load64("r11", -32)
         + x86_rsp_load64("rsp", -8)
         + x86("add", "rsp", 16L)
         + x86("push", "r11")
         + x86_jmpfn("rt_pl_dc_leave_γ", lvg_fp)
         + x86_deflabel_id(3)
         + x86_rsp_load64("rdi", 0)
         + x86("mov", "rsi", "rsp")
         + x86("add", "rsi", (long)(-kt))
         + x86_rsp_load64("r11", -32)
         + x86_rsp_load64("rsp", -8)
         + x86("add", "rsp", 16L)
         + x86("push", "r11")
         + x86_jmpfn("rt_pl_dc_leave_ω", lvw_fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_flat_dc_stub(void) { bb_emit_x86(xa_flat_dc_stub_str()); }
extern "C" void rt_lcl_proc_args_install(void *, int, int);
extern "C" void rt_icn_zframe_args_install(void *, int, int);
extern "C" void rt_arg_stage(int idx, DESCR_t v);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_zframe_prologue_str(void) {
    if (!PLATFORM_X86 || !g_emit.zframe_graph) return std::string();
    int kt = g_emit.flat_frame_bytes;
    if (kt < 48 || (kt & 15)) { fprintf(stderr, "FATAL xa_flat_zframe_prologue: kt=%d (must be 16-mult >= 48)\n", kt); abort(); }
    int np = g_emit_cfg ? g_emit_cfg->nparams : 0;
    int nl = g_emit_cfg ? g_emit_cfg->nlocals : 0;
    std::string s = x86("comment", "ICN-FR-2 zframe prologue: sub rsp,kt + wire header [kt-24]=γ [kt-16]=ω [kt-8]=caller____ + pin ___=rsp")
         + x86("sub", "rsp", (long)kt)
         + x86("mov", "[rsp + " + std::to_string(kt - 24) + "]", "rcx")
         + x86("mov", "[rsp + " + std::to_string(kt - 16) + "]", "rdx")
         + (emit_jmp_pin_legacy() ? x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rsp")
                                + std::string("")
                               : std::string());
    if (g_emit.flat_lex) {
        extern int g_flat_dc_np;
        if (g_flat_dc_np >= 0) {
            if (kt > 48) {
                int data_bytes = kt - 32;
                s += x86("mov", "rdi", "rsp")
                   + x86("xor", "eax", "eax")
                   + x86("mov32", "ecx", (long)data_bytes)
                   + x86("rep_stosb");
            }
            uint64_t _args_fp; { void (*_f)(void *, int, int) = rt_icn_zframe_args_install; _args_fp = (uint64_t)(uintptr_t)(void *)_f; }
            s += x86("mov", "rdi", "rsp")
               + x86("mov32", "esi", (long)np)
               + x86("mov32", "edx", (long)nl)
               + x86("call", "rt_icn_zframe_args_install", _args_fp);
        } else {
            uint64_t _lex_fp; { void (*_f)(void *, long, long) = rt_jmp_frame_lexprep2; _lex_fp = (uint64_t)(uintptr_t)(void *)_f; }
            int seed_off = g_emit.flat_seed_off ? g_emit.flat_seed_off : 16;
            s += x86("mov", "rdi", "rsp")
               + x86("mov32", "esi", (long)seed_off)
               + x86("mov32", "edx", (long)(kt - 32))
               + x86("call", "rt_jmp_frame_lexprep2", _lex_fp);
            if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_zframe_gen) {
                uint64_t _sw_fp;  { void (*_f)(void *, void *, void *) = rt_gen_save_wires;      _sw_fp  = (uint64_t)(uintptr_t)(void *)_f; }
                                s += x86("mov", "rdi", "rsp")
                   + x86("mov", "rsi", "qword ptr [rsp# + " + std::to_string(kt - 24) + "]")
                   + x86("mov", "rdx", "qword ptr [rsp# + " + std::to_string(kt - 16) + "]")
                   + x86("call", "rt_gen_save_wires", _sw_fp);
            }
        }
    } else {
        if (kt > 48) {
            int data_bytes = kt - 32;
            s += x86("mov", "rdi", "rsp")
               + x86("xor", "eax", "eax")
               + x86("mov32", "ecx", (long)data_bytes)
               + x86("rep_stosb");
        }
        if (np > 0 || nl > 0) {
            uint64_t _args_fp; { void (*_f)(void *, int, int) = rt_icn_zframe_args_install; _args_fp = (uint64_t)(uintptr_t)(void *)_f; }
            s += x86("mov", "rdi", "rsp")
               + x86("mov32", "esi", (long)np)
               + x86("mov32", "edx", (long)nl)
               + x86("call", "rt_icn_zframe_args_install", _args_fp);
        }
    }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xa_flat_wire_hdr_base(void) {
    int kt = g_emit.flat_frame_bytes;
    if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) kt += (g_emit_cfg->nparams + g_emit_cfg->nlocals) * 16;
    return kt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xa_flat_class_zf(void) {
    if (g_emit.zframe_graph) return 1;
    if (g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int xa_flat_class_c(void) {
    if (!g_emit.flat_jmp_entry) return 0;
    if (g_emit.flat_pat || g_emit.flat_gen || g_emit.flat_lcl_proc || g_emit.zframe_graph || g_emit.flat_stmt_frame) return 0;
    { extern int g_flat_frame_floor; if (g_flat_frame_floor > 0) return 0; }
    return (g_emit.flat_frame_bytes >= 48) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_chain_prologue_str(void) {
    if (!PLATFORM_X86 || !xa_flat_class_c()) return std::string();
    static int _cf = -1; if (_cf < 0) { const char * e = getenv("SCRIP_CHAIN_FRAME"); _cf = (e && *e == '0') ? 0 : 1; }
    if (!_cf) return std::string();
    { static int _d = -1; if (_d < 0) { const char * e = getenv("SCRIP_CHAIN_DIAG"); _d = (e && *e == '1') ? 1 : 0; } if (_d) { extern int bb_emit_pos; fprintf(stderr, "[CHAINFRAME] pos=%d kt=%d text=%d jmp=%d pat=%d\n", bb_emit_pos, g_emit.flat_frame_bytes, g_is_text ? 1 : 0, g_emit.flat_jmp_entry, g_emit.flat_pat); } }
    int kt = g_emit.flat_frame_bytes;
    if (kt & 15) { fprintf(stderr, "FATAL xa_flat_chain_prologue: kt=%d (must be a 16-multiple >= 48)\n", kt); abort(); }
    return x86("comment", "CLASS-C chain prologue (s114): carve kt + park {γ,ω} at [kt-24]/[kt-16] + save caller ___ at [kt-8]; ___ NOT pinned")
         + x86("sub", "rsp", (long)kt)
         + x86("mov", "[rsp + " + std::to_string(kt - 24) + "]", "rcx")
         + x86("mov", "[rsp + " + std::to_string(kt - 16) + "]", "rdx")
         + x86("mov", "[rsp + " + std::to_string(kt - 8) + "]", "rbp");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_chain_epilogue_str(void) {
    if (!PLATFORM_X86 || !xa_flat_class_c()) return std::string();
    static int _cf = -1; if (_cf < 0) { const char * e = getenv("SCRIP_CHAIN_FRAME"); _cf = (e && *e == '0') ? 0 : 1; }
    if (!_cf) return std::string();
    return x86("comment", "CLASS-C chain epilogue (s114): release the α carve; no whack exists on this exit")
         + x86("add", "rsp", (long)g_emit.flat_frame_bytes);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_gamma_retain_on(void) { static int _r = -1; if (_r < 0) { const char * e = getenv("SCRIP_PL_GAMMA_RETAIN"); _r = (e && *e == '1') ? 1 : 0; } return _r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_zframe_epilogue_γ_str(void) {
    if (!PLATFORM_X86 || !xa_flat_class_zf()) return std::string();
    int kt = xa_flat_wire_hdr_base();
    if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_zframe_gen) {
        uint64_t _ggw_fp; { void *(*_f)(void *) = rt_gen_get_gamma_wire; _ggw_fp = (uint64_t)(uintptr_t)(void *)_f; }
        return x86("comment", "ICN-FR-5 no-unwind epilogue-γ: r14=gen____; get γ-wire(r14)→r15; get caller____(r14)→___ (rsp stays at gen____); rdi:rsi=[r14+0/8]; rax=gen____; jmp r15")
             + x86("mov", "r14", "rsp")
             + x86("mov", "rdi", "r14")
             + x86("call", "rt_gen_get_gamma_wire", _ggw_fp)
             + x86("mov", "r15", "rax")
             + x86("mov", "rdi", "[r14 + 0]")
             + x86("mov", "rsi", "[r14 + 8]")
             + x86("mov", "rax", "r14")
             + x86("jmp", "r15");
    }
    if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->zframe_graph && !g_emit_cfg->icn_zframe_gen && g_emit_cfg->resume_slot > 0) {
        extern void *g_pl_zf_pending_cursor;
        extern void rt_pl_zf_resume_clear(void);
        uint64_t _clear_fp; { void (*_f)(void) = rt_pl_zf_resume_clear; _clear_fp = (uint64_t)(uintptr_t)(void *)_f; }
        int rs = g_emit_cfg->resume_slot;
        return x86("comment", "PL-FR-4 zframe epilogue-γ (Prolog gen): check pending resume cursor before unwind")
             + x86("lea", "r11", "[rip + __]", (uint64_t)(uintptr_t)&g_pl_zf_pending_cursor, "g_pl_zf_pending_cursor")
             + x86("mov", "r11", "[r11]")
             + x86("test", "r11", "r11")
             + x86("je", L(50))
             + x86("mov", "qword ptr [rsp# + " + std::to_string(rs) + "]", "r11")
             + x86("call", "rt_pl_zf_resume_clear", _clear_fp)
             + x86("mov", "rax", "qword ptr [rsp# + " + std::to_string(rs) + "]") + x86("jmp", "rax")
             + x86("def", L(50))
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 24) + "]")
             + (pl_gamma_retain_on()
                ? (x86("mov", "rax", "rsp")
                 + std::string(""))
                : (x86("add", "rsp", (long)kt)
                 + std::string("")))
             + x86("jmp", "rcx");
    }
    if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_cells_graph && g_emit.flat_lcl_proc) {
        return x86("comment", "Z-3 γ-RETAIN cells-arm generator: marshal rax:rdx→rdi:rsi; load γ wire; NO unwind (frame survives for β); gen____→rax; jmp γ wire")
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 24) + "]")
             + x86("mov", "rax", "rsp")
             + x86("jmp", "rcx");
    }
    return x86("comment", "ICN-FR-2 zframe epilogue-γ: marshal result rax:rdx→rdi:rsi; load γ wire from [kt-24]; unwind; jmp. NOTE: no caller-base restore happens here — the [kt-8] slot is WRITE-ONLY on every arm that fills it (s247)")
         + x86("mov", "rdi", "rax")
         + x86("mov", "rsi", "rdx")
         + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 24) + "]")
         + x86("add", "rsp", (long)kt)
         + std::string("")
         + x86("jmp", "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string xa_flat_zframe_epilogue_ω_str(void) {
    if (!PLATFORM_X86 || !xa_flat_class_zf()) return std::string();
    int kt = xa_flat_wire_hdr_base();
    if (g_emit.flat_gen && g_emit_cfg && g_emit_cfg->icn_zframe_gen) {
        uint64_t _gow_fp; { void *(*_f)(void *) = rt_gen_get_omega_wire; _gow_fp = (uint64_t)(uintptr_t)(void *)_f; }
        uint64_t _sw_fp;  { void  (*_f)(void *, void *, void *) = rt_gen_save_wires; _sw_fp = (uint64_t)(uintptr_t)(void *)_f; }
        return x86("comment", "ICN-FR-5 no-unwind epilogue-ω: r14=gen____; get ω-wire→r15; get caller____→rbx; pop gen state stack; ___=rbx; jmp r15")
             + x86("mov", "r14", "rsp")
             + x86("mov", "rdi", "r14")
             + x86("call", "rt_gen_get_omega_wire", _gow_fp)
             + x86("mov", "r15", "rax")
             + x86("mov", "rdi", "r14")
             + x86("xor", "esi", "esi")
             + x86("xor", "edx", "edx")
             + x86("call", "rt_gen_save_wires", _sw_fp)
             + x86("jmp", "r15");
    }
    return x86("comment", "ICN-FR-2 zframe epilogue-ω: load ω wire from [kt-16]; unwind to flat base; jmp. NOTE: no caller-base restore happens here — the [kt-8] slot is WRITE-ONLY on every arm that fills it (s247)")
         + x86("mov", "rcx", "qword ptr [rsp# + " + std::to_string(kt - 16) + "]")
         + x86("add", "rsp", (long)kt)
         + std::string("")
         + x86("jmp", "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void xa_flat_zframe_prologue(void) { bb_emit_x86(xa_flat_zframe_prologue_str()); }
extern "C" void xa_flat_chain_prologue(void) { bb_emit_x86(xa_flat_chain_prologue_str()); }
extern "C" int xa_flat_class_c_pred(void) { return xa_flat_class_c(); }
extern "C" void xa_flat_chain_epilogue(void) { bb_emit_x86(xa_flat_chain_epilogue_str()); }
extern "C" void xa_flat_zframe_epilogue_γ(void) { bb_emit_x86(xa_flat_zframe_epilogue_γ_str()); }
extern "C" void xa_flat_zframe_epilogue_ω(void) { bb_emit_x86(xa_flat_zframe_epilogue_ω_str()); }
