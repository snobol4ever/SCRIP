#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "templates/x86/x86_frame.h"
#include "emitter/emit_medium.h"
#include "emitter/emit_state_em_03_sh_api.h"
#include "emitter/emit_state_.h"
#include "emitter/emit_regime.h"
#include "emitter/emit_str.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_ops.h"
#include "templates/x86/x86_rsp_tracker.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/ir_graph.h"
static const char * x86_zr();
static int x86_zr_num();
static int x86_fb_stmt_on();
static int x86_frame_off_rsp(int off);
static const char * x86_fb();
static int x86_align_assert_on(void);
static std::string x86_rsp_modrm(int regfield, int off);
static const char * RSP(int off);
static std::string x86_frame_sink();
static std::string x86_frame_base(const char * r);
static std::string x86_frame_unsink();
static long x86_xfer_base(long at);
static std::string x86_alpha_carve(long K);
static std::string x86_gamma_free(long K);
static std::string x86_gamma_result(long K, long n);
static std::string x86_cell_push(int tag, int resume_ilbl);
static std::string x86_cell_unwind_tail();
static std::string x86_cell_fail_body();
static std::string x86_cell_cut_keep(const char * base);
static std::string x86_chain_prev(const char * dst, const char * src);
static std::string x86_chain_tag_load(const char * dst32, const char * cell);
static std::string x86_zclaim(long b);
static std::string x86_arbno_rbp_unwind_at(const std::string & mark_mem, int l_loop, int l_done);
static std::string x86_arbno_rbp_unwind(const char * mark_base, int mark_off, int l_loop, int l_done);
static std::string x86_call_frame_enter(int gamma_ilbl, int omega_ilbl);
static std::string x86_srf_floater(int wire_disp);
static std::string x86_return_floater();
static std::string x86_freturn_floater();
static const char * x86_zr() { return "rsp"; }
static int x86_zr_num() { return 4; }
int x86_resumable_frame() { return emit_resumable_frame(); }
int x86_serial_vars() { return (g_emit_cfg && g_emit_cfg->unbound_serial) ? 1 : 0; }
static int x86_fb_stmt_on() { static int m = -1; if (m < 0) { const char * e = getenv("SCRIP_FB_STMT"); m = (e && *e == '0') ? 0 : 1; } return m; }
int x86_fb_data() { return 0; }
static int x86_frame_off_rsp(int off) { return off + _.op_zdepth; }
int x86_frame_off(int off) { return _.op_zslid ? -1 : off + _.op_zdepth; }
int x86_hb_on() { return (g_emit.zframe_graph && !(g_emit_cfg && g_emit_cfg->cell_frame)) || (g_emit_cfg && g_emit_cfg->cell_frame && g_emit.flat_lcl_proc && !g_emit.flat_gen); }
int gen_frame_top() { return g_emit_cfg ? g_emit_cfg->pf.generator_top : 0; }
int icn_frame_rbp() { return (g_emit_cfg && g_emit_cfg->cell_frame && (g_emit.flat_gen || (g_emit.flat_lcl_proc && g_emit.frame_retained))) ? 1 : 0; }
const char * x86_hb() { return "rbp"; }
int x86_fb_num() { return 4; }
static const char * x86_fb() { return "rsp"; }
const char * x86_fr32_prefix() { return "dword ptr [rsp$ + "; }
const char * x86_fr64_prefix() { return "qword ptr [rsp$ + "; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_align_assert_on(void) {
    static int v = -1;
    if (v < 0) { const char * e = getenv("SCRIP_ALIGN_ASSERT"); v = (e && *e && *e != '0') ? 1 : 0; }
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_align_assert() {
    if (!x86_align_assert_on()) return std::string();
    if (MEDIUM_BINARY) return x86_Lrec(x86_b2(0x40, 0xF6) + x86_b2(0xC4, 0x0F) + x86_b2(0x74, 0x02) + x86_b2(0x0F, 0x0B));
    return x86_rec("test") + "spl, 15\n jz 1f\n ud2\n1:\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_port_canary() {
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_rsp_modrm(int regfield, int off) {
    std::string s; int rf = regfield & 7;
    int mod = (off == 0) ? 0 : (off >= -128 && off <= 127) ? 1 : 2;
    s += (char)((mod << 6) | (rf << 3) | 4); s += (char)0x24;
    if (mod == 1) s += (char)(int8_t)off;
    else if (mod == 2) s += u32le((uint32_t)off);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_modrm(int regfield, int off) {
    off = x86_frame_off(off);
    std::string s; int rf = regfield & 7; int b = x86_fb_num() & 7; int sib = (b == 4);
    int mod = (off == 0 && b != 5) ? 0 : (off >= -128 && off <= 127) ? 1 : 2;
    s += (char)((mod << 6) | (rf << 3) | (sib ? 4 : b));
    if (sib) s += (char)0x24;
    if (mod == 1) s += (char)(int8_t)off;
    else if (mod == 2) s += u32le((uint32_t)off);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_rex(int w, int regfield) {
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (regfield >= 8) rex |= 0x04; if (x86_fb_num() >= 8) rex |= 0x01;
    std::string s; if (rex != 0x40) s += (char)rex;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_text_mem(int off) { return std::string("[") + x86_fb() + " + " + std::to_string(x86_frame_off(off)) + "]"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_lea(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x8D; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("lea") + reg + ", " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_mov_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, 0); c += (char)0xC7; c += x86_frame_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return x86_rec("mov") + "dword ptr " + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_store(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x89; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + "dword ptr " + x86_frame_text_mem(off) + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_load(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x8B; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_add_imm(int off, long imm) {
    if (MEDIUM_BINARY) {
        std::string c; c += x86_frame_rex(0, 0);
        if (imm >= -128 && imm <= 127) { c += (char)0x83; c += x86_frame_modrm(0, off); c += (char)(uint8_t)(int8_t)imm; }
        else { c += (char)0x81; c += x86_frame_modrm(0, off); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return x86_rec("add") + "dword ptr " + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_add_to_reg(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x03; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("add") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_sub_from_reg(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x2B; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("sub") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_load64(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x8B; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + reg + ", qword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_store64(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x89; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + "qword ptr " + x86_frame_text_mem(off) + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_mov_imm64(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, 0); c += (char)0xC7; c += x86_frame_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return x86_rec("mov") + "qword ptr " + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
const char * x86_frame_top() { return (g_emit_cfg && g_emit_cfg->pf.home_frame) ? RDQ("rbp", g_emit.flat_frame_bytes) : FRQ(g_emit.flat_frame_bytes); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rsp_store64(int off, const char * reg) {
    int g = x86_rnum(reg); uint8_t rex = 0x48; if (g >= 8) rex |= 0x04;
    if (MEDIUM_BINARY) { std::string c; c += (char)rex; c += (char)0x89; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + "qword ptr [rsp + " + std::to_string(off) + "], " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rsp_store32_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0xC7; c += x86_rsp_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return x86_rec("mov") + "dword ptr [rsp + " + std::to_string(off) + "], " + std::to_string((uint32_t)imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rsp_load64(const char * reg, int off) {
    int g = x86_rnum(reg); uint8_t rex = 0x48; if (g >= 8) rex |= 0x04;
    if (MEDIUM_BINARY) { std::string c; c += (char)rex; c += (char)0x8B; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + reg + ", qword ptr [rsp + " + std::to_string(off) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rsp_store64_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0x48; c += (char)0xC7; c += x86_rsp_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return x86_rec("mov") + "qword ptr [rsp + " + std::to_string(off) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_raw_pack(const char * reg) { return x86_shift_imm("shl", 4, reg, 8) + x86_or_imm(reg, (long)DT_RAW); }
std::string x86_raw_unpack(const char * reg) { return x86_shift_imm("shr", 5, reg, 8); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rsp_store32(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x89; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + "dword ptr [rsp + " + std::to_string(off) + "], " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rsp_load32(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x8B; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + reg + ", dword ptr [rsp + " + std::to_string(off) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rsp_add_imm32(int off, long imm) {
    if (MEDIUM_BINARY) {
        std::string c;
        if (imm >= -128 && imm <= 127) { c += (char)0x83; c += x86_rsp_modrm(0, off); c += (char)(uint8_t)(int8_t)imm; }
        else { c += (char)0x81; c += x86_rsp_modrm(0, off); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return x86_rec("add") + "dword ptr [rsp + " + std::to_string(off) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rsp_add_to_reg32(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x03; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("add") + reg + ", dword ptr [rsp + " + std::to_string(off) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rsp_sub_from_reg32(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x2B; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("sub") + reg + ", dword ptr [rsp + " + std::to_string(off) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * RSP(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp + %d]", off); return b[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_frame_inc64(int off) {
    std::string code; code += x86_frame_rex(1, 0); code += (char)0xFF; code += x86_frame_modrm(0, off);
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("inc") + "qword ptr " + x86_frame_text_mem(off) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_align_enter() { return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_align_leave() { return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_anchor_enter() { return x86_align_enter(); }
std::string x86_anchor_leave() { return x86_align_leave(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_frame_sink() {
    return x86("mov", "rcx", "rsp")
         + x86("sub", "rsp", "rax")
         + x86("sub", "rsp", 16L)
         + x86("and", "rsp", -16L)
         + x86("mov", "qword ptr [rsp + 0]", "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_frame_base(const char * r) {
    return x86("mov", r, "rsp") + x86("add", r, 16L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_frame_unsink() {
    return x86("mov", "rsp", "qword ptr [rsp + 0]");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long x86_xfer_base(long at) { return at < 0 ? 0 : at; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_xfer_enter(long at) {
    return (at < 0 ? x86("sub", "rsp", 32L) : std::string())
         + x86_rsp_store32_imm(x86_xfer_base(at) + 16, (long)DT_I) + x86_rsp_store32_imm(x86_xfer_base(at) + 20, 0L) + x86_rsp_store64(x86_xfer_base(at) + 24, "r14")
         + x86_rsp_store32_imm(x86_xfer_base(at) + 0, (long)DT_S) + x86_rsp_store32(x86_xfer_base(at) + 4, "r15d") + x86_rsp_store64(x86_xfer_base(at) + 8, "r13");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_xfer_leave(long at) {
    return x86_rsp_load64("r13", x86_xfer_base(at) + 8) + x86_rsp_load32("r15d", x86_xfer_base(at) + 4) + x86_rsp_load64("r14", x86_xfer_base(at) + 24)
         + (at < 0 ? x86("add", "rsp", 32L) : std::string());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_alpha_carve(long K) { return K > 0 ? x86("sub", "rsp", K) : std::string(); }
static std::string x86_gamma_free(long K) { return K > 0 ? x86("add", "rsp", K) : std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_gamma_result(long K, long n) {
    long drop = K + 16 * (n - 1);
    return x86("mov", "rax", RDQ("rsp", 0)) + x86("mov", "rdx", RDQ("rsp", 8))
         + (drop > 0 ? x86("add", "rsp", drop) : std::string())
         + x86("mov", RDQ("rsp", 0), "rax") + x86("mov", RDQ("rsp", 8), "rdx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_cell_push(int tag, int resume_ilbl) {
    return x86("xor", "rax", "rax") + x86("push", "rax")
         + std::string("")
         + x86("push", "r14")
         + (tag ? x86_rsp_store32_imm(4, (long)tag) : std::string())
         + x86_lea_rip_id("rax", resume_ilbl)
         + x86("push", "rax")
         + std::string("");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_main_prologue() { return std::string(); }
static std::string x86_cell_unwind_tail() { return x86("pop", "rax") + x86("pop", "r14") + std::string("") + x86("add", "rsp", 8L) + x86_jmp_reg("rax"); }
static std::string x86_cell_fail_body() { return std::string("") + x86_cell_unwind_tail(); }
static std::string x86_cell_cut_keep(const char * base) { return x86_reg_disp32_load64("rsp", base, 16) + x86_reg_disp32_lea64("rsp", base, 32); }
static std::string x86_chain_prev(const char * dst, const char * src) { return x86_reg_disp32_load64(dst, src, 16); }
static std::string x86_chain_tag_load(const char * dst32, const char * cell) { return x86_reg_disp32_load32(dst32, cell, 12); }
static std::string x86_zclaim(long b) { return x86("sub", "rsp", b); }
std::string x86_zrelease(long b) { return x86("add", "rsp", b); }
static std::string x86_arbno_rbp_unwind_at(const std::string & mark_mem, int l_loop, int l_done) { return x86("note", "arbno_rbp_unwind") + x86("mov", "rax", mark_mem) + x86("def", L(l_loop)) +
    x86("cmp", "rbp", "rax") + x86("jae", L(l_done)) + x86("test", "rbp", "rbp") + x86("je", L(l_done)) + x86("mov", "rbp", RDQ("rbp", 24)) + x86("jmp", L(l_loop)) + x86("def", L(l_done)); }
static std::string x86_arbno_rbp_unwind(const char * mark_base, int mark_off, int l_loop, int l_done) { return x86_arbno_rbp_unwind_at(RDQ(mark_base, mark_off), l_loop, l_done); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_call_frame_enter(int gamma_ilbl, int omega_ilbl) {
    return x86_lea_rip_id("rcx", gamma_ilbl)
         + x86_lea_rip_id("rdx", omega_ilbl)
         + x86("push", "rdx")
         + x86("push", "rcx")
         + std::string("")
         + x86("sub", "rsp", 8L)
         + std::string("");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_srf_floater(int wire_disp) {
    return std::string("")
         + x86_reg_disp32_load64("rcx", "rsp", wire_disp)
         + x86_reg_disp32_load64("rsp", "rsp", 8)
         + x86("add", "rsp", 32L)
         + x86_jmp_reg("rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_return_floater() { return x86_srf_floater(16); }
static std::string x86_freturn_floater() { return x86_srf_floater(24); }
std::string x86_align_call_enter() {
    return x86_rsp_mark('A') + x86("mov", "r11", "rsp")
         + x86("and", "rsp", (long)-16)
         + x86("sub", "rsp", (long)16)
         + x86_rsp_store64(0, "r11");
}
std::string x86_align_call_leave() {
    return x86_rsp_load64("rsp", 0) + x86_rsp_mark('Z');
}
