#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "templates/x86/x86_ops.h"
#include "emitter/emit_medium.h"
#include "emitter/bb_label.h"
#include "emitter/emit_state.h"
#include "emitter/emit_data.h"
#include "emitter/emit_walk.h"
#include "emitter/emit_str.h"
#include "emitter/emit_gc_site.h"
#include "ir/descr.h"
#include "runtime/rtx/rtcc.h"
#include "templates/x86/x86_arg_roles.h"
#include "templates/x86/x86_call.h"
#include "templates/x86/x86_frame.h"
#include "templates/x86/x86_gc.h"
#include "templates/x86/x86_jump.h"
#include "templates/x86/x86_rsp_tracker.h"
#include "templates/x86/x86_rtcc.h"
#include "templates/x86/x86_rodata.h"
#include "templates/x86/x86_zverify.h"
#include "runtime/core/g_runtime.h"
extern "C" { extern uint64_t g_rtccb[32]; }
extern "C" { long *rt_anchor_ptr(void); }
extern "C" void rt_bomb(const char * msg);
static int x86_tabs_on(void);
static int x86_4col_joinon(void);
static int x86_col4(void);
static int x86_rec_id(const std::string & s, size_t & i);
static bool x86_is64(const char * r);
static bool x86_is8(const char * r);
static std::string x86_alu_rr(const char * mnem, uint8_t op, const char * rm, const char * reg);
static std::string x86_mov (const char * rm, const char * reg);
static std::string x86_cmp (const char * rm, const char * reg);
static std::string x86_test(const char * rm, const char * reg);
static std::string x86_xor_rr(const char * rm, const char * reg);
static std::string x86_add_rr(const char * rm, const char * reg);
static std::string x86_sub_rr(const char * rm, const char * reg);
static std::string x86_imul_rr(const char * dst, const char * src);
static std::string x86_cqo();
static std::string x86_rep_stosb();
static std::string x86_rep_stosq();
static std::string x86_rep_movsq();
static std::string x86_xorps_xmm0();
static std::string x86_movq_xmm0_r64(const char * src);
static std::string x86_movq_xmm_r64(const char * dst, const char * src);
static std::string x86_sse2_xx(const char * mnem, uint8_t op, const char * xd, const char * xs);
static std::string x86_cvtsi2sd_xmm_r64(const char * xd, const char * src);
static std::string x86_movq_r64_xmm(const char * dst, const char * xs);
static std::string x86_set_xmm0_double(double d);
static std::string x86_idiv(const char * reg);
static std::string x86_neg(const char * reg);
static std::string x86_movsxd(const char * dst64, const char * src32);
static std::string x86_lea_subj_cursor(const char * dst);
static std::string x86_movzx_subj_byte(const char * dst, int disp);
static std::string x86_movzx_bir(const char * dst, const char * base, const char * idx);
static std::string x86_mov_subj_q(const char * dst, int disp);
static std::string x86_mov_subj_d(const char * dst, int disp);
static std::string x86_push(const char * r);
static std::string x86_pop (const char * r);
static std::string x86_movimm(const char * dst, long imm);
static std::string x86_movimm32(const char * dst, long imm);
static std::string x86_and(const char * reg, long imm);
static std::string x86_cmp_imm(const char * reg, long imm);
static std::string x86_movsxd_frame(const char * dst64, int off);
static const char * LFCQ(int d);
static const char * AFC(int d);
static const char * CFCQ(int d);
static const char * CROQ(int cro, int d);
static const char * CROD(int cro, int d);
static void x86_rd32_modrm(std::string & c, int g, int b);
static std::string x86_movups_store_xmm0(const char * base, int disp);
static std::string x86_reg_disp32_inc64(const char * base, int disp);
static std::string x86_reg_disp32_cmp_imm(const char * base, int disp, long imm);
static std::string x86_reg_disp32_cmp_r64(const char * dst, const char * base, int disp);
static std::string x86_reg_disp32_and_r64(const char * dst, const char * base, int disp);
static std::string x86_reg_disp32_or_r64(const char * dst, const char * base, int disp);
static std::string x86_inc_r(const char * reg);
static std::string x86_abs_disp32_load64(const char * dst, long va);
static std::string x86_abs_disp32_addsub_imm8(int is_sub, long va, long imm);
static std::string x86_cmp_reg_abs64(const char * reg, long va);
static std::string x86_reg_disp32_add32(const char * dst, const char * base, int disp);
static std::string x86_reg_disp32_store32(const char * base, int disp, const char * src);
static std::string x86_reg_disp32_store_imm64(const char * base, int disp, long imm);
static std::string x86_reg_disp32_store_imm32(const char * base, int disp, long imm);
static std::string x86_reg_disp32_add_imm32(const char * base, int disp, long imm);
static std::string x86_reg_disp_add_imm64(const char * base, int disp, long imm);
static const char * F64(double d);
static std::string x86_cmp_imm64(const char * reg, long imm);
static std::string x86_load_indexed8(const char * dst, const char * base, const char * idx);
static std::string x86_load_mem64(const char * dst, const char * basebr);
static int x86_is_reg(const char * s);
static void x86_parse(const xop & x, opnd & o);
static int x86_disp_w(const char * s, size_t n);
static void x86_4col_pad(std::string & o, const char * s, size_t n, int width);
static int x86_argreg_slot(const char * s, size_t n);
static const x86_argrole_t * x86_argrole_find(const char * s, size_t n);
static void x86_line_parts(const std::string & o, size_t ls, size_t le, size_t & ms, size_t & ml, size_t & os_, size_t & ol);
static void x86_argnote(std::string & o);
static void x86_4col_to(std::string & o, size_t ls, int col);
static void x86_rec_split(const char * p, size_t len, x86_rec_t & r);
static int x86_rec_kind(const x86_rec_t & r);
static __attribute__((noreturn)) void x86_bare_target_refused(const char * mnem, const char * sym);
static bool x86_is_scan_builtin_name(const char *fn);
static void x86_text_rsp_notes_strip(std::string & r);
static int x86_tabs_on(void) { static int t = -1; if (t < 0) { const char * e = getenv("SCRIP_ASM_TABS"); t = (e && *e == '0') ? 0 : 1; } return t; }
std::string x86_rec(const char * op) { return x86_tabs_on() ? (std::string("\t") + op + "\t") : (std::string(" ") + op + " "); }
std::string x86_recn(const char * op) { return x86_tabs_on() ? (std::string("\t") + op) : (std::string(" ") + op); }
std::string x86_reclbl(const std::string & nm) { return x86_tabs_on() ? (nm + ":\t") : (nm + ":"); }
static int x86_4col_joinon(void) { static int j = -1; if (j < 0) { const char * e = getenv("SCRIP_ASM_JOIN"); j = (e && *e == '0') ? 0 : 1; } return j; }
static int x86_col4(void) { return x86_4col_joinon() ? 78 : 88; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int x86_rnum(const char * r) {
    if (!r || !r[0] || !r[1]) return 0;
    if (!r[2]) { switch (r[0]) { case 'r': return r[1] == '8' ? 8 : r[1] == '9' ? 9 : 0; case 'a': return 0; case 'c': return r[1] == 'l' ? 1 : 0; case 'd': return r[1] == 'l' ? 2 : 0;
        case 'b': return r[1] == 'l' ? 3 : 0; default: return 0; } }
    if (!r[3]) {
        if (r[0] == 'e' || r[0] == 'r') { switch (r[1] * 256 + r[2]) { case 'a' * 256 + 'x': return 0; case 'c' * 256 + 'x': return 1; case 'd' * 256 + 'x': return 2; case 'b' * 256 + 'x': return 3;
            case 's' * 256 + 'p': return 4; case 's' * 256 + 'i': return 6; case 'd' * 256 + 'i': return 7; case 'b' * 256 + 'p': return r[0] == 'r' ? 5 : 0; default: break; } }
        if (r[0] == 'r' && (r[1] == '8' || r[1] == '9') && (r[2] == 'd' || r[2] == 'b')) return r[1] - '0';
        if (r[0] == 'r' && r[1] == '1' && r[2] >= '0' && r[2] <= '5') return 10 + (r[2] - '0');
        if (r[2] == 'l') { switch (r[0] * 256 + r[1]) { case 's' * 256 + 'p': return 4; case 'b' * 256 + 'p': return 5; case 's' * 256 + 'i': return 6; case 'd' * 256 + 'i': return 7;
            default: return 0; } }
        return 0;
    }
    if (!r[4] && r[0] == 'r' && r[1] == '1' && r[2] >= '0' && r[2] <= '5' && (r[3] == 'd' || r[3] == 'b')) return 10 + (r[2] - '0');
    return 0;
}
std::string x86_Lrec(const std::string & b) { std::string r; r += (char)'L'; r += (char)(unsigned char)b.size(); r += b; return r; }
std::string x86_Jrec(int port) { std::string r; r += (char)'J'; r += (char)(unsigned char)(port & 255); r += (char)(unsigned char)((port >> 8) & 255); return r; }
std::string x86_Drec(int port) { std::string r; r += (char)'D'; r += (char)(unsigned char)(port & 255); r += (char)(unsigned char)((port >> 8) & 255); return r; }
static int x86_rec_id(const std::string & s, size_t & i) { int v = (unsigned char)s[i] | ((unsigned char)s[i + 1] << 8); i += 2; return v; }
std::string x86_b1(uint8_t a) { std::string s; s += (char)a; return s; }
std::string x86_b2(uint8_t a, uint8_t b) { std::string s; s += (char)a; s += (char)b; return s; }
std::string x86_b3(uint8_t a, uint8_t b, uint8_t c) { std::string s; s += (char)a; s += (char)b; s += (char)c; return s; }
std::string x86_b4(uint8_t a, uint8_t b, uint8_t c, uint8_t d) { std::string s; s += (char)a; s += (char)b; s += (char)c; s += (char)d; return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bool x86_is64(const char * r) {
    if (!r) return false;
    if (r[0] != 'r') return false;
    size_t n = strlen(r);
    if (r[n - 1] == 'd') return false;
    return true;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bool x86_is8(const char * r) {
    if (!r) return false;
    static const char * const names8[] = { "al","cl","dl","bl","spl","bpl","sil","dil",
                                            "r8b","r9b","r10b","r11b","r12b","r13b","r14b","r15b" };
    for (size_t i = 0; i < sizeof(names8) / sizeof(names8[0]); ++i) if (!strcmp(r, names8[i])) return true;
    return false;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_alu_rr(const char * mnem, uint8_t op, const char * rm, const char * reg) {
    int m = x86_rnum(rm), g = x86_rnum(reg);
    uint8_t rex = 0x40; if (x86_is64(rm) || x86_is64(reg)) rex |= 0x08; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; if (rex != 0x40) code += (char)rex; code += (char)op; code += (char)(0xC0 | ((g & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : x86_rec(mnem) + rm + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_mov (const char * rm, const char * reg) { return x86_alu_rr("mov", 0x89, rm, reg); }
static std::string x86_cmp (const char * rm, const char * reg) { return x86_alu_rr("cmp", 0x39, rm, reg); }
static std::string x86_test(const char * rm, const char * reg) { return x86_alu_rr("test", 0x85, rm, reg); }
static std::string x86_xor_rr(const char * rm, const char * reg) { return x86_alu_rr("xor", 0x31, rm, reg); }
static std::string x86_add_rr(const char * rm, const char * reg) { return x86_alu_rr("add", 0x01, rm, reg); }
static std::string x86_sub_rr(const char * rm, const char * reg) { return x86_alu_rr("sub", 0x29, rm, reg); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_imul_rr(const char * dst, const char * src) {
    int g = x86_rnum(dst), m = x86_rnum(src);
    uint8_t rex = 0x40; if (x86_is64(dst) || x86_is64(src)) rex |= 0x08; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; if (rex != 0x40) code += (char)rex; code += (char)0x0F; code += (char)0xAF; code += (char)(0xC0 | ((g & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("imul") + dst + ", " + src + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_cqo() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b2(0x48, 0x99)) : x86_recn("cqo") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_rep_stosb() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b2(0xF3, 0xAA)) : x86_rec("rep") + "stosb\n";
}
static std::string x86_rep_stosq() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b3(0xF3, 0x48, 0xAB)) : x86_rec("rep") + "stosq\n";
}
static std::string x86_rep_movsq() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b3(0xF3, 0x48, 0xA5)) : x86_rec("rep") + "movsq\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_xorps_xmm0() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b3(0x0F, 0x57, 0xC0)) : x86_rec("xorps") + "xmm0, xmm0\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_movabs_r64(const char * dst, uint64_t imm) {
    int m = x86_rnum(dst);
    if (MEDIUM_BINARY) { std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le(imm); return x86_Lrec(code); }
    return x86_rec("movabs") + dst + ", " + std::to_string((unsigned long long)imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_movq_xmm0_r64(const char * src) {
    int m = x86_rnum(src); uint8_t rex = 0x48; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)0x66; code += (char)rex; code += (char)0x0F; code += (char)0x6E; code += (char)(0xC0 | (0 << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movq") + "xmm0, " + src + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_movq_xmm_r64(const char * dst, const char * src) {
    int xn = (dst && !strncmp(dst, "xmm", 3)) ? atoi(dst + 3) : 0; int m = x86_rnum(src); uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; if (xn >= 8) rex |= 0x04;
    std::string code; code += (char)0x66; code += (char)rex; code += (char)0x0F; code += (char)0x6E; code += (char)(0xC0 | ((xn & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movq") + dst + ", " + src + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_sse2_xx(const char * mnem, uint8_t op, const char * xd, const char * xs) {
    int d = (xd && !strncmp(xd, "xmm", 3)) ? atoi(xd + 3) : 0; int s = (xs && !strncmp(xs, "xmm", 3)) ? atoi(xs + 3) : 0;
    std::string code; code += (char)0xF2; uint8_t rex = 0x40; if (d >= 8) rex |= 0x04; if (s >= 8) rex |= 0x01; if (rex != 0x40) code += (char)rex;
    code += (char)0x0F; code += (char)op; code += (char)(0xC0 | ((d & 7) << 3) | (s & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec(mnem) + xd + ", " + xs + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_cvtsi2sd_xmm_r64(const char * xd, const char * src) {
    int d = (xd && !strncmp(xd, "xmm", 3)) ? atoi(xd + 3) : 0; int m = x86_rnum(src);
    std::string code; code += (char)0xF2; uint8_t rex = 0x48; if (d >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01; code += (char)rex;
    code += (char)0x0F; code += (char)0x2A; code += (char)(0xC0 | ((d & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("cvtsi2sd") + xd + ", " + src + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_movq_r64_xmm(const char * dst, const char * xs) {
    int s = (xs && !strncmp(xs, "xmm", 3)) ? atoi(xs + 3) : 0; int m = x86_rnum(dst);
    uint8_t rex = 0x48; if (s >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)0x66; code += (char)rex; code += (char)0x0F; code += (char)0x7E; code += (char)(0xC0 | ((s & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movq") + dst + ", " + xs + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_set_xmm0_double(double d) {
    uint64_t bits = 0; memcpy(&bits, &d, sizeof bits);
    return x86_movabs_r64("rax", bits) + x86_movq_xmm0_r64("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_idiv(const char * reg) {
    int m = x86_rnum(reg); uint8_t rex = 0x48; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0xF7; code += (char)(0xC0 | (7 << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("idiv") + reg + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_neg(const char * reg) {
    int m = x86_rnum(reg); uint8_t rex = 0x48; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0xF7; code += (char)(0xC0 | (3 << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("neg") + reg + "\n");
}
std::string x86_add(const char * reg, long imm) {
    int m = x86_rnum(reg); bool w = x86_is64(reg);
    std::string code;
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (m >= 8) rex |= 0x01;
    if (imm >= -128 && imm <= 127) { if (rex != 0x40) code += (char)rex; code += (char)0x83; code += (char)(0xC0 | (0 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else if (m == 0 && !w) { code += (char)0x05; code += u32le((uint32_t)imm); }
    else { if (rex != 0x40) code += (char)rex; code += (char)0x81; code += (char)(0xC0 | (0 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return (!strcmp(reg, "rsp") ? x86_rsp_rec(-imm) : std::string()) + (MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("add") + reg + ", " + std::to_string(imm) + "\n"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_sub(const char * reg, long imm) {
    int m = x86_rnum(reg); bool w = x86_is64(reg);
    std::string code;
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (m >= 8) rex |= 0x01; if (rex != 0x40) code += (char)rex;
    if (imm >= -128 && imm <= 127) { code += (char)0x83; code += (char)(0xC0 | (5 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else { code += (char)0x81; code += (char)(0xC0 | (5 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return (!strcmp(reg, "rsp") ? x86_rsp_rec(imm) : std::string()) + (MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("sub") + reg + ", " + std::to_string(imm) + "\n"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_movsxd(const char * dst64, const char * src32) {
    int g = x86_rnum(dst64), m = x86_rnum(src32);
    uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0x63; code += (char)(0xC0 | ((g & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movsxd") + dst64 + ", " + src32 + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_lea_subj_cursor(const char * dst) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x48 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x8D; code += (char)modrm; code += (char)sib; code += (char)0x00;
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("lea") + dst + ", [r13 + rcx]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_movzx_subj_byte(const char * dst, int disp) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x40 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x0F; code += (char)0xB6; code += (char)modrm; code += (char)sib; code += (char)(disp & 0xFF);
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movzx") + dst + ", byte ptr [r13+rcx" + (disp ? std::string("+") + std::to_string(disp) : std::string()) + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_movzx_bir(const char * dst, const char * base, const char * idx) {
    int g = x86_rnum(dst), bn = x86_rnum(base), in = x86_rnum(idx);
    uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (in >= 8) rex |= 0x02; if (bn >= 8) rex |= 0x01;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib = (uint8_t)((0 << 6) | ((in & 7) << 3) | (bn & 7));
    std::string code; code += (char)rex; code += (char)0x0F; code += (char)0xB6; code += (char)modrm; code += (char)sib; code += (char)0x00;
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movzx") + dst + ", byte ptr [" + base + " + " + idx + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_mov_subj_q(const char * dst, int disp) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x48 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x8B; code += (char)modrm; code += (char)sib; code += (char)(disp & 0xFF);
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("mov") + dst + ", qword ptr [r13+rcx" + (disp ? std::string("+") + std::to_string(disp) : std::string()) + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_mov_subj_d(const char * dst, int disp) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x41; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x8B; code += (char)modrm; code += (char)sib; code += (char)(disp & 0xFF);
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("mov") + dst + ", dword ptr [r13+rcx" + (disp ? std::string("+") + std::to_string(disp) : std::string()) + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_push(const char * r) {
    int m = x86_rnum(r); std::string code; if (m >= 8) code += (char)0x41; code += (char)(0x50 | (m & 7)); return (MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("push") + r + "\n")) + x86_rsp_rec(8);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_pop (const char * r) {
    int m = x86_rnum(r); std::string code; if (m >= 8) code += (char)0x41; code += (char)(0x58 | (m & 7)); return (MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("pop") + r + "\n")) + x86_rsp_rec(-8);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_movimm(const char * dst, long imm) {
    int m = x86_rnum(dst);
    if (MEDIUM_BINARY) { std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le((uint64_t)imm); return x86_Lrec(code); }
    return x86_rec("mov") + dst + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_movimm32(const char * dst, long imm) {
    int m = x86_rnum(dst);
    if (MEDIUM_BINARY) { std::string code; if (m >= 8) code += (char)0x41; code += (char)(0xB8 | (m & 7)); code += u32le((uint32_t)imm); return x86_Lrec(code); }
    return x86_rec("mov") + dst + ", " + std::to_string(imm) + "\n";
}
std::string x86_alpha() { return x86_deflabel(X86P_ALPHA) + x86_alpha_zchk(); }
std::string x86_beta() { return x86_deflabel(X86P_BETA) + x86_beta_zchk(); }
std::string x86_beta_trampoline() { return _.op_beta_dead ? std::string() : (x86_deflabel(X86P_BETA) + x86_jmp(X86P_OMEGA)); }
std::string x86_gamma() { return x86_jmp(X86P_GAMMA); }
std::string x86_gamma(const char * mnem) { return x86_jcc(mnem, X86P_GAMMA); }
std::string x86_omega() { return x86_jmp(X86P_OMEGA); }
std::string x86_omega(const char * mnem) { return x86_jcc(mnem, X86P_OMEGA); }
const char * x86_boxkind(void) { return _.x86_uid_kind ? _.x86_uid_kind : "anon"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void x86_begin() { if (!MEDIUM_BINARY) _.x86_uid = g_flat_node_id++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_and(const char * reg, long imm) {
    int m = x86_rnum(reg); bool w = x86_is64(reg);
    std::string code;
    if (imm >= -128 && imm <= 127) {
        uint8_t rex = 0x40; if (w) rex |= 0x08; if (m >= 8) rex |= 0x01; if (rex != 0x40) code += (char)rex;
        code += (char)0x83; code += (char)(0xC0 | (4 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm;
    } else if (m == 0 && !w) {
        code += (char)0x25; code += u32le((uint32_t)imm);
    } else {
        uint8_t rex = 0x40; if (w) rex |= 0x08; if (m >= 8) rex |= 0x01; if (rex != 0x40) code += (char)rex;
        code += (char)0x81; code += (char)(0xC0 | (4 << 3) | (m & 7)); code += u32le((uint32_t)imm);
    }
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("and") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_or_imm(const char * reg, long imm) {
    int m = x86_rnum(reg); bool w = x86_is64(reg);
    std::string code;
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (m >= 8) rex |= 0x01; if (rex != 0x40) code += (char)rex;
    if (imm >= -128 && imm <= 127) { code += (char)0x83; code += (char)(0xC0 | (1 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else { code += (char)0x81; code += (char)(0xC0 | (1 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("or") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_shift_imm(const char * mnem, int ext, const char * reg, long imm) {
    int m = x86_rnum(reg); bool w = x86_is64(reg);
    std::string code;
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (m >= 8) rex |= 0x01; if (rex != 0x40) code += (char)rex;
    code += (char)0xC1; code += (char)(0xC0 | (ext << 3) | (m & 7)); code += (char)(uint8_t)imm;
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec(mnem) + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_cmp_imm(const char * reg, long imm) {
    int m = x86_rnum(reg);
    std::string code;
    if (x86_is8(reg)) { uint8_t rex = 0x40; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)0x80; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += (char)(uint8_t)imm; }
    else if (imm >= -128 && imm <= 127) { if (m >= 8) code += (char)0x41; code += (char)0x83; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else if (m == 0) { code += (char)0x3D; code += u32le((uint32_t)imm); }
    else { if (m >= 8) code += (char)0x41; code += (char)0x81; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("cmp") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_movsxd_frame(const char * dst64, int off) {
    int g = x86_rnum(dst64);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x63; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("movsxd") + dst64 + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
const char * ROQ(int n) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rip + %d]", n); return b[i]; }
const char * RDQ(const char * base, int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [%s + %d]", base, off); return b[i]; }
const char * ABSQ(unsigned long va) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [%lu]", va); return b[i]; }
const char * GVARQ(int k, int w) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [" RTCC_GVA_REG " + %d]", k * 16 + w); return b[i]; }
const char * FR(int off) { return x86_zop(off, 0, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * FRQ(int off) { return x86_zop(off, 1, 0); }
const char * FRQB(int off, int bump) { return x86_zop(off, 1, bump); }
const char * RDD(const char * base, int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "dword ptr [%s + %d]", base, off); return b[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * LFC(int d) { return FR(_.x86_scratch_off + d); }
static const char * LFCQ(int d) { return FRQ(_.x86_scratch_off + d); }
const char * LFDQ(int d, int sd) { (void)d; return LFCQ(sd); }
const char * LFD(int d, int sd) { (void)d; return LFC(sd); }
static const char * AFC(int d) { return RDD("rbp", _.op_arbno_frame_off + d); }
const char * AFCQ(int d) { return RDQ("rbp", _.op_arbno_frame_off + d); }
const char * CFC(int d) { return RDD("rbp", _.op_cap_frame_off + d); }
static const char * CFCQ(int d) { return RDQ("rbp", _.op_cap_frame_off + d); }
const char * FFC(int d) { return RDD("rbp", _.op_fence_frame_off + d); }
const char * FFCQ(int d) { return RDQ("rbp", _.op_fence_frame_off + d); }
static const char * CROQ(int cro, int d) { (void)cro; return RDQ("rsp", d); }
static const char * CROD(int cro, int d) { (void)cro; return RDD("rsp", d); }
const char * LIDX(long k) { static char b[8][32]; static int i; i = (i + 1) & 7; if (k) snprintf(b[i], 32, "[r13+rcx+%ld]", k); else snprintf(b[i], 32, "[r13+rcx]"); return b[i]; }
long LITQ(long k) { uint64_t w; memcpy(&w, _.op_sval + k, 8); return (long) w; }
static void x86_rd32_modrm(std::string & c, int g, int b) { c += (char)(0x80 | ((g & 7) << 3) | (b & 7)); if ((b & 7) == 4) c += (char)0x24; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_reg_disp32_load64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8B; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("mov") + dst + ", qword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_reg_disp32_store64(const char * base, int disp, const char * src) {
    int g = x86_rnum(src), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x89; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("mov") + "qword ptr [" + base + " + " + std::to_string(disp) + "], " + src + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_movups_store_xmm0(const char * base, int disp) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; if (b >= 8) c += (char)0x41; c += (char)0x0F; c += (char)0x11; x86_rd32_modrm(c, 0, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("movups") + "xmmword ptr [" + base + " + " + std::to_string(disp) + "], xmm0\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_reg_disp32_inc64(const char * base, int disp) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0xFF; x86_rd32_modrm(c, 0, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("inc") + "qword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_reg_disp32_cmp_imm(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (b >= 8) rex |= 0x01; c += (char)rex;
        if (imm >= -128 && imm <= 127) { c += (char)0x83; x86_rd32_modrm(c, 7, b); c += u32le((uint32_t)disp); c += (char)(uint8_t)(int8_t)imm; }
        else { c += (char)0x81; x86_rd32_modrm(c, 7, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return x86_rec("cmp") + "qword ptr [" + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_reg_disp32_cmp_r64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x3B; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("cmp") + dst + ", qword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_reg_disp32_and_r64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x23; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("and") + dst + ", qword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_reg_disp32_or_r64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x0B; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("or") + dst + ", qword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_inc_r(const char * reg) {
    int m = x86_rnum(reg); int w64 = !(reg && (reg[0] == 'e' || (reg[0] && reg[strlen(reg) - 1] == 'd')));
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = (uint8_t)((w64 ? 0x48 : 0x40) | (m >= 8 ? 0x01 : 0)); if (rex != 0x40) c += (char)rex; c += (char)0xFF; c += (char)(0xC0 | (m & 7)); return x86_Lrec(c);
    }
    return x86_rec("inc") + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_abs_disp32_load64(const char * dst, long va) {
    int g = x86_rnum(dst);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8B; c += (char)(0x04 | ((g & 7) << 3)); c += (char)0x25;
        c += u32le((uint32_t)va); return x86_Lrec(c);
    }
    return x86_rec("mov") + dst + ", qword ptr [" + std::to_string(va) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_abs_disp32_store64(long va, const char * src) {
    int g = x86_rnum(src);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x89; c += (char)(0x04 | ((g & 7) << 3)); c += (char)0x25;
        c += u32le((uint32_t)va); return x86_Lrec(c);
    }
    return x86_rec("mov") + "qword ptr [" + std::to_string(va) + "], " + src + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_abs_disp32_addsub_imm8(int is_sub, long va, long imm) {
    if (imm < -128 || imm > 127) { fprintf(stderr, "FATAL x86_abs_disp32_addsub_imm8: imm %ld outside imm8 (REG-2 shape is imm8-only; widen the encoder before widening a client)\n", imm); abort(); }
    if (MEDIUM_BINARY) {
        std::string c; c += (char)0x48; c += (char)0x83; c += (char)(is_sub ? 0x2C : 0x04); c += (char)0x25;
        c += u32le((uint32_t)va); c += (char)(uint8_t)(int8_t)imm; return x86_Lrec(c);
    }
    return std::string(is_sub ? " sub" : " add") + " qword ptr [" + std::to_string(va) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_cmp_reg_abs64(const char * reg, long va) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x3B; c += (char)(0x04 | ((g & 7) << 3)); c += (char)0x25;
        c += u32le((uint32_t)va); return x86_Lrec(c);
    }
    return x86_rec("cmp") + reg + ", qword ptr [" + std::to_string(va) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_reg_disp32_load32(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0x8B; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("mov") + dst + ", dword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_reg_disp32_add32(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0x03; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("add") + dst + ", dword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_reg_disp32_store32(const char * base, int disp, const char * src) {
    int g = x86_rnum(src), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0x89; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("mov") + "dword ptr [" + base + " + " + std::to_string(disp) + "], " + src + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_reg_disp32_store_imm64(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0xC7; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm);
        return x86_Lrec(c);
    }
    return x86_rec("mov") + "qword ptr [" + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_reg_disp32_store_imm32(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0xC7; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm);
        return x86_Lrec(c);
    }
    return x86_rec("mov") + "dword ptr [" + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_reg_disp32_add_imm32(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex;
        if (imm >= -128 && imm <= 127) { c += (char)0x83; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += (char)(uint8_t)(int8_t)imm; }
        else { c += (char)0x81; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return x86_rec("add") + "dword ptr [" + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_reg_disp_add_imm64(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (b >= 8) rex |= 0x01; c += (char)rex;
        if (imm >= -128 && imm <= 127) { c += (char)0x83; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += (char)(uint8_t)(int8_t)imm; }
        else { c += (char)0x81; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return x86_rec("add") + "qword ptr [" + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_reg_disp32_lea64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8D; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("lea") + dst + ", [" + base + " + " + std::to_string(disp) + "]\n";
}
static const char * F64(double d) { static char b[4][32]; static int i; i = (i + 1) & 3; uint64_t bits; memcpy(&bits, &d, 8); snprintf(b[i], 32, "f64:%llu", (unsigned long long)bits); return b[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_cmp_imm64(const char * reg, long imm) {
    int m = x86_rnum(reg);
    uint8_t rex = 0x48; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex;
    if (imm >= -128 && imm <= 127) { code += (char)0x83; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else { code += (char)0x81; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("cmp") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_load_indexed8(const char * dst, const char * base, const char * idx) {
    int g = x86_rnum(dst), b = x86_rnum(base), x = x86_rnum(idx);
    uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (x >= 8) rex |= 0x02; if (b >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0x8B;
    code += (char)(0x00 | ((g & 7) << 3) | 0x04);
    code += (char)((3 << 6) | ((x & 7) << 3) | (b & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("mov") + dst + ", [" + base + " + " + idx + "*8]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string x86_load_mem64(const char * dst, const char * basebr) {
    char rb[(basebr ? strlen(basebr) : 0) + 1]; int k = 0; for (const char * p = basebr; p && *p; ++p) if (*p != '[' && *p != ']' && *p != ' ') rb[k++] = *p; rb[k] = 0;
    int g = x86_rnum(dst), m = x86_rnum(rb);
    uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0x8B; code += (char)((0 << 6) | ((g & 7) << 3) | (m & 7));
    if ((m & 7) == 4) code += (char)0x24;
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("mov") + dst + ", qword ptr [" + rb + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * x86_strkeep(const std::string & v) {
    static cv_t ring[16]; static int i;
    i = (i + 1) & 15;
    size_t n = v.size(); cv_reserve(&ring[i], 1, (uint64_t)n + 1, "x86_strkeep");
    memcpy(ring[i].p, v.data(), n); ((char *)ring[i].p)[n] = 0;
    return (const char *)ring[i].p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_is_reg(const char * s) {
    if (!s[0] || !s[1]) return 0;
    if (s[0] == 'x') return (s[1] == 'm' && s[2] == 'm' && s[3] >= '0' && s[3] <= '7' && !s[4]) ? 1 : 0;
    if (!s[2]) return ((s[0] == 'r' && (s[1] == '8' || s[1] == '9')) || (s[1] == 'l' && (s[0] == 'a' || s[0] == 'c' || s[0] == 'd' || s[0] == 'b'))) ? 1 : 0;
    if (!s[3]) {
        if (s[0] == 'e' || s[0] == 'r') { switch (s[1] * 256 + s[2]) { case 'a' * 256 + 'x': case 'c' * 256 + 'x': case 'd' * 256 + 'x': case 'b' * 256 + 'x':
            case 's' * 256 + 'p': case 'b' * 256 + 'p': case 's' * 256 + 'i': case 'd' * 256 + 'i': return 1; default: break; } }
        if (s[0] == 'r' && (s[1] == '8' || s[1] == '9') && (s[2] == 'd' || s[2] == 'b')) return 1;
        if (s[0] == 'r' && s[1] == '1' && s[2] >= '0' && s[2] <= '5') return 1;
        if (s[2] == 'l') { switch (s[0] * 256 + s[1]) { case 's' * 256 + 'p': case 'b' * 256 + 'p': case 's' * 256 + 'i': case 'd' * 256 + 'i': return 1; default: return 0; } }
        return 0;
    }
    if (!s[4]) return (s[0] == 'r' && s[1] == '1' && s[2] >= '0' && s[2] <= '5' && (s[3] == 'd' || s[3] == 'b')) ? 1 : 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void x86_parse(const xop & x, opnd & o) {
    memset(&o, 0, sizeof o); o.kind = XK_NONE; o.txt = x.s;
    if (x.tag == 0) return;
    if (x.tag == 2) { o.kind = XK_IMM; o.imm = (long)(int64_t)x.u; return; }
    const char * s = x.s;
    if (x86_is_reg(s)) { o.kind = XK_REG; o.reg = x86_rnum(s); return; }
    const int dq = (s[0] == 'd' || s[0] == 'q') && !strncmp(s + 1, "word ptr [", 10);
    int p = x86_port_of(s);
    if (p >= 0 && (s[2] == 0)) { fprintf(stderr, "x86_parse: string port operand \"%s\" is RETIRED (Lon 2026-07-08 s5) — ports go through x86_alpha/x86_beta/x86_gamma/x86_omega ONLY\n", s); abort();
        }
    if (s[0] == 'L' && s[1] >= '0' && s[1] <= '9') { int n = atoi(s + 1); o.kind = XK_ILBL; o.lbl = n; return; }
    if (s[0] == 'P' && s[1] >= '0' && s[1] <= '9') { int _pdig = 1; for (const char * q = s + 1; *q; q++) if (*q < '0' || *q > '9') { _pdig = 0; break; } if (_pdig) { int n = atoi(s + 1);
        o.kind = XK_PAIR; o.lbl = n; return; } }
    if (s[0] == 'e' && !strcmp(s, "extlbl")) { o.kind = XK_EXTLBL; return; }
    if (dq && !strncmp(s, "dword ptr [rsp# + ", 18)) { o.kind = XK_RSP32; o.off = atoi(s + 18); return; }
    if (dq && !strncmp(s, "qword ptr [rsp# + ", 18)) { o.kind = XK_RSP64; o.off = atoi(s + 18); return; }
    if (dq && !strncmp(s, x86_fr32_prefix(), strlen(x86_fr32_prefix()))) { o.kind = XK_FR32; o.off = atoi(s + strlen(x86_fr32_prefix())); return; }
    if (dq && !strncmp(s, x86_fr64_prefix(), strlen(x86_fr64_prefix()))) { o.kind = XK_FR64; o.off = atoi(s + strlen(x86_fr64_prefix())); return; }
    if (dq && !strncmp(s, "dword ptr [rsp + ", 17)) { o.kind = XK_RSP32; o.off = atoi(s + 17); return; }
    if (dq && !strncmp(s, "dword ptr [", 11)) { const char * lb = s + 10; const char * pl = strstr(lb, " + ");
      if (pl) { size_t bl = (size_t)(pl - (lb + 1)); if (bl > 7) bl = 7; memcpy(o.base, lb + 1, bl); o.base[bl] = 0;
        char * ep = 0; long d = strtol(pl + 3, &ep, 10); if (x86_is_reg(o.base) && ep && *ep == ']') { o.kind = XK_REGDISP32; o.off = (int)d; return; } } }
    if (dq && !strncmp(s, "qword ptr [rip + ", 17)) { o.kind = XK_ROSLOT; o.off = atoi(s + 17); return; }
    if (dq && !strncmp(s, "qword ptr [rsp + ", 17)) { o.kind = XK_RSP64; o.off = atoi(s + 17); return; }
    if (dq && !strncmp(s, "qword ptr [", 11) && s[11] >= '0' && s[11] <= '9') { char * ep = 0; unsigned long a = strtoul(s + 11, &ep, 10); if (ep && *ep == ']' && !ep[1]) { o.kind = XK_ABS64;
        o.imm = (long)a; return; } }
    if (dq && !strncmp(s, "qword ptr [", 11)) { const char * lb = s + 10; const char * pl = strstr(lb, " + ");
      if (pl) { size_t bl = (size_t)(pl - (lb + 1)); if (bl > 7) bl = 7; memcpy(o.base, lb + 1, bl); o.base[bl] = 0;
        char * ep = 0; long d = strtol(pl + 3, &ep, 10); if (x86_is_reg(o.base) && ep && *ep == ']') { o.kind = XK_REGDISP; o.off = (int)d; return; } } }
    if (strchr(s, '[')) { char ns[strlen(s) + 1]; int k = 0; for (const char * q = s; *q; q++) if (*q != ' ') ns[k++] = *q; ns[k] = 0;
      if (!strncmp(ns, "[r13+rcx+", 9)) { o.kind = XK_R13RCX; o.off = atoi(ns + 9); return; }
      if (!strcmp(ns, "[r13+rcx]")) { o.kind = XK_R13RCX; o.off = 0; return; }
      { const char * pp = strchr(ns, '+'); size_t nn = strlen(ns);
        if (ns[0] == '[' && pp && nn >= 3 && ns[nn - 1] == ']' && !strchr(pp + 1, '+') && !strchr(ns, '*')) {
          size_t bl = (size_t)(pp - (ns + 1)); if (bl > 7) bl = 7; char bb[bl + 1]; memcpy(bb, ns + 1, bl); bb[bl] = 0;
          size_t il = (size_t)((ns + nn - 1) - (pp + 1)); if (il > 7) il = 7; char ii[il + 1]; memcpy(ii, pp + 1, il); ii[il] = 0;
          if (x86_is_reg(bb) && x86_is_reg(ii)) { memcpy(o.base, bb, bl + 1); memcpy(o.idx, ii, il + 1); o.kind = XK_MEMBI; return; } } } }
    if (s[0] == '[' && !strcmp(s, "[rip + __]")) { o.kind = XK_RIPSEAL; return; }
    if (s[0] == '[' && !strcmp(s, "[rip@got + __]")) { o.kind = XK_RIPGOT; return; }
    if (s[0] == '[' && !strcmp(s, "[rip@cell + __]")) { o.kind = XK_RIPCELL; return; }
    if (s[0] == 'f' && !strncmp(s, "f64:", 4)) {
        o.kind = XK_IMM; o.imm = 0; o.txt = s; { unsigned long long bb = strtoull(s + 4, 0, 10); memcpy(&o.imm, &bb, sizeof(long) < 8 ? sizeof(long) : 8); } o.off = 1; return;
    }
    if (s[0] == '[') {
        const char * star = strstr(s, "*8]");
        if (star) {
            const char * plus = strchr(s, '+'); if (!plus) plus = s;
            size_t bl = (size_t)(plus - s - 1); if (bl > 7) bl = 7;
            memcpy(o.base, s + 1, bl); o.base[bl] = 0;
            const char * ip = plus + 1; while (*ip == ' ') ip++;
            size_t il = (size_t)(star - ip); if (il > 7) il = 7;
            memcpy(o.idx, ip, il); o.idx[il] = 0;
            char * t; t = o.base; while (*t) { if (*t == ' ') { *t = 0; break; } t++; }
            t = o.idx; while (*t) { if (*t == ' ') { *t = 0; break; } t++; }
            o.kind = XK_MEMIDX8; return;
        }
        { const char * pl = strstr(s, " + "); if (pl) { size_t bl = (size_t)(pl - (s + 1)); if (bl > 7) bl = 7;
          char bbr[bl + 1]; memcpy(bbr, s + 1, bl); bbr[bl] = 0; char * ep = 0; long d = strtol(pl + 3, &ep, 10);
          if (x86_is_reg(bbr) && ep && *ep == ']') { memcpy(o.base, bbr, bl + 1); o.kind = XK_REGDISP; o.off = (int)d; return; } } }
        size_t n = strlen(s);
        if (n >= 3 && s[n - 1] == ']') {
            size_t bl = n - 2; if (bl > 7) bl = 7;
            memcpy(o.base, s + 1, bl); o.base[bl] = 0;
            if (x86_is_reg(o.base)) { o.kind = XK_MEMIND; return; }
        }
    }
    if (x86_is_reg(s)) { o.kind = XK_REG; o.reg = x86_rnum(s); return; }
    char * endp = 0; long v = strtol(s, &endp, 0);
    if (endp && endp != s && *endp == 0) { o.kind = XK_IMM; o.imm = v; return; }
    o.kind = XK_SYM; o.sym = s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_disp_w(const char * s, size_t n) { int w = 0; for (size_t i = 0; i < n; i++) if (((unsigned char)s[i] & 0xC0) != 0x80) w++; return w; }
static void x86_4col_pad(std::string & o, const char * s, size_t n, int width) { o.append(s, n); int pad = width - x86_disp_w(s, n); if (pad < 1) pad = 1; o.append((size_t)pad, ' '); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_argreg_slot(const char * s, size_t n) {
    static const char * const r[6][4] = { { "rdi", "edi", "di", "dil" }, { "rsi", "esi", "si", "sil" }, { "rdx", "edx", "dx", "dl" }, { "rcx", "ecx", "cx", "cl" }, { "r8", "r8d", "r8w", "r8b" },
        { "r9", "r9d", "r9w", "r9b" } };
    for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) { size_t l = strlen(r[i][j]); if (l == n && !strncmp(s, r[i][j], l)) return i; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const x86_argrole_t * x86_argrole_find(const char * s, size_t n) {
    for (size_t i = 0; i < sizeof(x86_argroles) / sizeof(x86_argroles[0]); i++) { const char * c = x86_argrole_str(x86_argroles[i].callee);
        if (strlen(c) == n && !strncmp(s, c, n)) return &x86_argroles[i]; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void x86_line_parts(const std::string & o, size_t ls, size_t le, size_t & ms, size_t & ml, size_t & os_, size_t & ol) {
    size_t i = ls; ms = ml = os_ = ol = 0;
    while (i < le && (o[i] == ' ' || o[i] == '\t')) i++;
    if (i >= le) return;
    size_t k = i; while (k < le && o[k] != ' ' && o[k] != '\t') k++;
    if (k > i && o[k - 1] == ':') { while (k < le && (o[k] == ' ' || o[k] == '\t')) k++; i = k; while (k < le && o[k] != ' ' && o[k] != '\t') k++; if (i >= le) return; }
    ms = i; ml = k - i;
    while (k < le && (o[k] == ' ' || o[k] == '\t')) k++;
    os_ = k; ol = (k < le) ? le - k : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void x86_argnote(std::string & o) {
    if (o.find("call") == std::string::npos) return;
    std::vector<size_t> beg, end; size_t i = 0, n = o.size();
    while (i < n) { size_t e = o.find('\n', i); if (e == std::string::npos) e = n; beg.push_back(i); end.push_back(e); i = e + 1; }
    std::vector<const char *> ann(beg.size(), (const char *)0);
    for (size_t L = 0; L < beg.size(); L++) {
        size_t ms, ml, os_, ol; x86_line_parts(o, beg[L], end[L], ms, ml, os_, ol);
        if (ml != 4 || strncmp(o.data() + ms, "call", 4) || !ol) continue;
        size_t ce = os_; while (ce < os_ + ol && o[ce] != '@' && o[ce] != ' ' && o[ce] != '\t' && o[ce] != ',' && o[ce] != '#') ce++;
        const x86_argrole_t * rr = x86_argrole_find(o.data() + os_, ce - os_);
        if (!rr) continue;
        for (size_t B = L; B-- > 0; ) {
            size_t bs, bl, bo, bol; x86_line_parts(o, beg[B], end[B], bs, bl, bo, bol);
            if (!bl) break;
            if (o[bs] == '#' || o[bs] == '.' || o[bs] == 'j') break;
            if (bl == 4 && !strncmp(o.data() + bs, "call", 4)) break;
            int isld = (bl == 3 && (!strncmp(o.data() + bs, "mov", 3) || !strncmp(o.data() + bs, "lea", 3) || !strncmp(o.data() + bs, "xor", 3))) ||
                (bl == 6 && !strncmp(o.data() + bs, "movsxd", 6)) || (bl == 5 && !strncmp(o.data() + bs, "movzx", 5)) || (bl == 6 && !strncmp(o.data() + bs, "movabs", 6));
            if (!isld || !bol) break;
            size_t de = bo; while (de < bo + bol && o[de] != ',' && o[de] != ' ' && o[de] != '\t') de++;
            int slot = x86_argreg_slot(o.data() + bo, de - bo);
            if (slot < 0) break;
            if (o.find('#', beg[B]) < end[B]) continue;
            const char * rs_ = x86_argrole_str(rr->role[slot]); if (rs_[0]) ann[B] = rs_;
        }
    }
    std::string out; out.reserve(o.size() + o.size() / 8);
    for (size_t L = 0; L < beg.size(); L++) {
        out.append(o, beg[L], end[L] - beg[L]);
        if (ann[L]) { int w = x86_disp_w(o.data() + beg[L], end[L] - beg[L]); int pd = x86_col4() - w; if (pd < 1) pd = 1; out.append((size_t)pd, ' '); out.append("# "); out.append(ann[L]); }
        if (end[L] < o.size()) out.append(1, '\n');
    }
    o.swap(out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void x86_4col_to(std::string & o, size_t ls, int col) { int w = x86_disp_w(o.data() + ls, o.size() - ls); int pd = col - w; if (pd < 1) pd = 1; o.append((size_t)pd, ' '); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void x86_rec_split(const char * p, size_t len, x86_rec_t & r) {
    r.lb = r.op = r.ar = 0; r.ll = r.ol = r.al = 0; r.marg = 0;
    size_t b0 = 0; while (b0 < len && p[b0] == ' ') b0++;
    if (b0 < len && p[b0] == '#') { r.marg = 1; r.ar = p + b0; r.al = len - b0; return; }
    const char * tb = (const char *) memchr(p, '\t', len);
    if (tb) { r.lb = p; r.ll = (size_t)(tb - p); const char * q = tb + 1; size_t ql = len - r.ll - 1; const char * t2 = (const char *) memchr(q, '\t', ql);
        if (t2) { r.op = q; r.ol = (size_t)(t2 - q); r.ar = t2 + 1; r.al = ql - r.ol - 1; } else { r.op = q; r.ol = ql; }
        if (!r.ll && !r.ol && !r.al) r.marg = 1;
        return; }
    size_t b = b0; while (b < len && (p[b] == ' ' || p[b] == '\t')) b++;
    const char * t = p + b; size_t tl = len - b;
    if (tl == 0) { r.marg = 1; return; }
    size_t k = 0; while (k < tl && t[k] != ' ' && t[k] != '\t') k++;
    if (k > 0 && t[k - 1] == ':') { r.lb = t; r.ll = k; size_t rr = k; while (rr < tl && (t[rr] == ' ' || t[rr] == '\t')) rr++; if (rr >= tl) return; t += rr; tl -= rr; k = 0;
        while (k < tl && t[k] != ' ' && t[k] != '\t') k++; }
    r.op = t; r.ol = k;
    if ((k == 3 && !strncmp(t, "rep", 3)) || (k == 4 && (!strncmp(t, "repe", 4) || !strncmp(t, "repz", 4) || !strncmp(t, "lock", 4))) ||
        (k == 5 && (!strncmp(t, "repne", 5) || !strncmp(t, "repnz", 5)))) {
        size_t w = k; while (w < tl && (t[w] == ' ' || t[w] == '\t')) w++;
        if (w < tl) { size_t m2 = w; while (m2 < tl && t[m2] != ' ' && t[m2] != '\t') m2++; r.ol = m2; } }
    size_t rr = r.ol; while (rr < tl && (t[rr] == ' ' || t[rr] == '\t')) rr++;
    if (rr < tl) { r.ar = t + rr; r.al = tl - rr; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int x86_rec_kind(const x86_rec_t & r) { if (r.marg) return r.al ? 4 : 0; if (!r.ol) return 1; return (r.op[0] == 'j') ? 3 : 2; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_4col(const std::string & s) {
    if (MEDIUM_BINARY || MEDIUM_MACRO_DEF) return s;
    { static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_ASM_COLUMNS"); on = (e && *e == '0') ? 0 : 1; } if (!on) return s; }
    const int jn = x86_4col_joinon(); const int CJ = x86_col4();
    std::string o; o.reserve(s.size() + s.size() / 2);
    std::string note, prevnote;
    size_t i = 0, n = s.size();
    int pend = 0; size_t pls = 0;
    while (i < n) {
        size_t e = s.find('\n', i); size_t len = (e == std::string::npos ? n : e) - i;
        const char * p = s.data() + i; size_t inext = (e == std::string::npos) ? n : e + 1;
        x86_rec_t r; x86_rec_split(p, len, r);
        if (r.marg && r.al >= 2 && r.ar[0] == '#' && r.ar[1] == '@') { note.assign(r.ar + 2, r.al - 2); i = inext; continue; }
        int ck = x86_rec_kind(r); int hasl = (r.ll != 0);
        int nk = 0, nhasl = 0;
        for (size_t j2 = inext; j2 < n; ) { size_t e2 = s.find('\n', j2); size_t l2 = (e2 == std::string::npos ? n : e2) - j2; x86_rec_t r2; x86_rec_split(s.data() + j2, l2, r2);
            if (r2.marg && r2.al >= 2 && r2.ar[0] == '#' && r2.ar[1] == '@') { j2 = (e2 == std::string::npos) ? n : e2 + 1; continue; } nk = x86_rec_kind(r2); nhasl = (r2.ll != 0); break; }
        int join = jn && !hasl && ((pend == 1 && (ck == 2 || ck == 3)) || (pend == 2 && ck == 3));
        if (join) { int pw = x86_disp_w(o.data() + pls, o.size() - pls);
            if (pend == 1) { if (pw >= 24) join = 0; }
            else if (pw + 1 >= CJ) join = 0; }
        int willjoin = jn && !nhasl && ck == 2 && nk == 3;
        size_t ls;
        if (join) { if (pend == 1) x86_4col_to(o, pls, 24); else o.append(1, ';'); if (ck == 3) x86_4col_to(o, pls, CJ); ls = pls; }
        else { if (pend) o.append(1, '\n'); ls = o.size(); }
        int inst = 0, isj = 0;
        if (r.marg) { if (r.al) o.append(r.ar, r.al); }
        else {
            if (r.ll) { if (r.ol) x86_4col_pad(o, r.lb, r.ll, 24); else o.append(r.lb, r.ll); }
            else if (!join) o.append((size_t)24, ' ');
            if (r.ol) {
                if (r.op[0] == 'j') { if (!join) x86_4col_to(o, ls, CJ); if (!r.al) o.append(r.op, r.ol); else { x86_4col_pad(o, r.op, r.ol, 6); o.append(r.ar, r.al); } inst = 1; isj = 1; }
                else if (!r.al) { o.append(r.op, r.ol); inst = 1; }
                else { x86_4col_pad(o, r.op, r.ol, 17); o.append(r.ar, r.al); inst = 1; }
            }
        }
        if (inst && !note.empty()) {
            int drop = isj || (willjoin && x86_disp_w(o.data() + ls, o.size() - ls) + 1 < CJ);
            int fresh = note != prevnote || !note.compare(0, 8, "gc_poll ");
            if (!drop && fresh && o.find('#', ls) == std::string::npos) { x86_4col_to(o, ls, CJ); o.append("# "); o.append(note); prevnote = note; }
            if (!drop) { if (note != prevnote) prevnote.clear(); }
            note.clear(); }
        pend = (ck == 0 || ck == 4) ? 4 : ck; pls = ls;
        i = inext;
    }
    if (pend) o.append(1, '\n');
    if (!note.empty()) { o.append("#@"); o.append(note); o.append(1, '\n'); }
    x86_argnote(o);
    return o;
}
static __attribute__((noreturn)) void x86_bare_target_refused(const char * mnem, const char * sym) {
    fprintf(stderr,
        "FATAL x86: '%s' to the bare name '%s' has no encoding in the binary medium -- mode 3 would fall through to the next instruction; pass the target as a port (lbl_t0/lbl_t1 through x86_jcc_tgt"
        "/x86_jmp_tgt) or an internal label id, which both media encode (hq_snobol4's finding 2026-09-25)\n", mnem ? mnem : "?", sym ? sym : "?");
    abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86(const char * mnem, xop xa, xop xb, xop xc, xop xd) { return x86_core_(mnem, xa, xb, xc, xd); }
std::string x86_core_(const char * mnem, xop xa, xop xb, xop xc, xop xd) {
    std::string r = x86_core_inner_(mnem, xa, xb, xc, xd);
    if (xa.tag == 1 && xa.s && !strcmp(xa.s, "rsp") &&
        (X86_MEQ(mnem, "mov") || X86_MEQ(mnem, "lea") || X86_MEQ(mnem, "and") || X86_MEQ(mnem, "or") || X86_MEQ(mnem, "xor") || ((X86_MEQ(mnem, "sub") || X86_MEQ(mnem, "add")) && xb.tag == 1)))
        r += x86_rsp_unk();
    return r;
}
std::string x86_core_inner_(const char * mnem, xop xa, xop xb, xop xc, xop xd) {
    if (X86_MEQ(mnem, "label")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF) ? std::string() : x86_reclbl(std::string(xa.s ? xa.s : "")) + "\n";
    if (X86_MEQ(mnem, "comment")) return std::string();
    if (X86_MEQ(mnem, "note")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF || !xa.s || !xa.s[0]) ? std::string() : (std::string("#@") + xa.s + "\n");
    if (X86_MEQ(mnem, "srccomment")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF) ? std::string() : (std::string("# ") + (xa.s ? xa.s : "") + "\n");
    if (X86_MEQ(mnem, "loc")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF || !xa.s || !xa.s[0]) ? std::string() : (std::string(".loc ") + std::to_string(xc.u) + " " + std::to_string(xb.u) + " 0\n");
    if (X86_MEQ(mnem, "commentrule")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF) ? std::string() : (std::string("#") + (xa.s ? xa.s : "") + "\n");
    if (X86_MEQ(mnem, "directive")) return MEDIUM_BINARY ? std::string() : (std::string("  ") + (xa.s ? xa.s : "") + "\n");
    if (X86_MEQ(mnem, "raw")) return MEDIUM_BINARY ? std::string() : (std::string(" ") + (xa.s ? xa.s : "") + "\n");
    if (X86_MEQ(mnem, ".quad")) {
        opnd a; x86_parse(xa, a);
        if (a.kind == XK_ILBL) return x86_quad_ilbl(a.lbl);
        if (xa.tag == 2) return MEDIUM_BINARY ? x86_Lrec(u64le(xa.u)) : (std::string(" .quad ") + std::to_string((unsigned long long)xa.u) + "\n");
        if (xa.tag == 1 && xb.tag == 1) return MEDIUM_BINARY ? x86_Lrec(u64le((uint64_t)(uintptr_t)(xb.s ? xb.s : ""))) : (std::string(" .quad ") + (xa.s ? xa.s : "") + "\n");
        if (xa.tag == 1 && xb.tag == 2) return MEDIUM_BINARY ? x86_Lrec(u64le(xb.u)) : (std::string(" .quad ") + (xa.s ? xa.s : "0") + "\n");
        return std::string();
    }
    if (X86_MEQ(mnem, ".quad_ro")) {
        opnd a; x86_parse(xa, a);
        if (a.kind != XK_ILBL) { fprintf(stderr, "FATAL x86: .quad_ro takes an internal label L(n) first, then the target's text name and its binary value\n"); abort(); }
        if (MEDIUM_BINARY) return x86_deflabel_id(a.lbl) + x86_Lrec(u64le(xc.tag == 2 ? xc.u : (uint64_t)(uintptr_t)(xc.s ? xc.s : "")));
        return x86_ro_cell(x86_internal_name(a.lbl), std::string(xb.s ? xb.s : "0"));
    }
    if (X86_MEQ(mnem, ".cset_hdr")) {
        uint64_t hq[CSET_BLK_HDR / 8];
        std::string hr;
        if (MEDIUM_BINARY || !xa.s) return std::string();
        memcpy(hq, xa.s + CSET_BLK_LEN_OFF, sizeof hq);
        for (size_t hi = 0; hi < CSET_BLK_HDR / 8; hi++) hr += std::string(" .quad ") + std::to_string((unsigned long long)hq[hi]) + "\n";
        return hr;
    }
    if (X86_MEQ(mnem, ".string")) {
        if (MEDIUM_BINARY) return std::string();
        return xb.tag == 2
             ? (std::string(" .string \"") + x86_asm_str_escape(xa.s ? xa.s : "", (size_t)xb.u) + "\"\n")
             : (std::string(" .string \"") + x86_asm_str_escape(xa.s ? xa.s : "") + "\"\n");
    }
    opnd a, b, c; x86_parse(xa, a); x86_parse(xb, b); x86_parse(xc, c);
    if (X86_MEQ(mnem, "ret")) return (MEDIUM_BINARY ? x86_Lrec(std::string(1, (char)0xC3)) : x86_recn("ret") + "\n") + x86_rsp_goto();
    if (X86_MEQ(mnem, "cqo")) return x86_cqo();
    if (X86_MEQ(mnem, "rep_stosb")) return x86_rep_stosb();
    if (X86_MEQ(mnem, "rep_stosq")) return x86_rep_stosq();
    if (X86_MEQ(mnem, "rep_movsq")) return x86_rep_movsq();
    if (X86_MEQ(mnem, "def")) {
        if (a.kind == XK_PORT) return x86_deflabel(a.port);
        if (a.kind == XK_ILBL) return x86_deflabel_id(a.lbl);
        if (a.kind == XK_PAIR) return x86_deflabel_pair(a.lbl);
        return std::string();
    }
    if (X86_MEQ(mnem, "jmp_fn_cell")) { if (a.kind == XK_SYM && xb.tag == 2) return x86_jmp_through_fn_cell(a.sym, xb.u); return std::string(); }
    if (X86_MEQ(mnem, "jmp_cellp")) { if (a.kind == XK_RIPCELL && xb.tag == 2 && xc.tag == 1) return x86_jmp_via_cellp(xc.s, xb.u); return std::string(); }
    if (X86_MEQ(mnem, "jmp")) {
        if (a.kind == XK_PORT) return x86_jmp(a.port);
        if (a.kind == XK_ILBL) return x86_jmp_id(a.lbl);
        if (a.kind == XK_PAIR) return x86_jmp_pair(a.lbl);
        if (a.kind == XK_FR64) return x86_jmp_frame64(a.off);
        if (a.kind == XK_EXTLBL && xb.tag == 2) return x86_jmp_ext((const struct bb_label_t *)(uintptr_t)xb.u);
        if (a.kind == XK_RIPCELL && xb.tag == 2 && xc.tag == 1) return x86_jmp_via_cell(xc.s, xb.u);
        if (a.kind == XK_REG) {
            int m = x86_rnum(a.txt); uint8_t modrm = (uint8_t)(0xE0 | (m & 7)); uint8_t rex = (m >= 8) ? 0x41 : 0x40;
            return MEDIUM_BINARY ? x86_Lrec(std::string((char)rex == 0x40 ? "" : std::string(1, (char)rex)) + (char)0xFF + (char)modrm) : (x86_rec("jmp") + a.txt + "\n");
        }
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return x86_rec("jmp") + a.sym + "\n";
        if (a.kind == XK_SYM) x86_bare_target_refused(mnem, a.sym);
        return std::string();
    }
    if (mnem[0] == 'j') {
        if (a.kind == XK_PORT) return x86_jcc(mnem, a.port);
        if (a.kind == XK_ILBL) return x86_jcc_id(mnem, a.lbl);
        if (a.kind == XK_PAIR) return x86_jcc_pair(mnem, a.lbl);
        if (a.kind == XK_EXTLBL && xb.tag == 2) return x86_jcc_ext(mnem, (const struct bb_label_t *)(uintptr_t)xb.u);
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return x86_rec(mnem) + a.sym + "\n";
        if (a.kind == XK_SYM) x86_bare_target_refused(mnem, a.sym);
        return std::string();
    }
    if (X86_MEQ(mnem, "call")) {
        if (a.kind == XK_PORT) return x86_align_assert() + (MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE8)) + x86_Jrec(a.port))
                                                    : (x86_rec("call") + x86_portname(a.port) + "\n")) + x86_gc_site(X86_SITE_CALL);
        if (a.kind == XK_SYM && xb.tag == 2) return x86_rtcc_call(a.sym, xb.u);
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return x86_align_assert() + x86_call_text(a.sym, "");
        if (a.kind == XK_REG) {
            int m = x86_rnum(a.txt); uint8_t modrm = (uint8_t)(0xD0 | (m & 7)); uint8_t rex = (m >= 8) ? 0x41 : 0x40;
            return x86_align_assert() + (MEDIUM_BINARY ? x86_Lrec(std::string((char)rex == 0x40 ? "" : std::string(1,(char)rex)) + (char)0xFF + (char)modrm) : (x86_rec("call") + a.txt + "\n")) +
                x86_gc_site(X86_SITE_CALL);
        }
        return std::string();
    }
    if (X86_MEQ(mnem, "call_rt")) {
        if (a.kind == XK_SYM && b.kind == XK_IMM && c.kind == XK_IMM)
            return x86_rtcc_call_descr(a.sym, (uint64_t)c.imm, (int)b.imm);
        return std::string();
    }
    if (X86_MEQ(mnem, "call_bare")) {
        if (a.kind == XK_SYM && xb.tag == 2) return x86_call_ro(a.sym, xb.u);
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return x86_align_assert() + x86_call_text(a.sym, "");
        return std::string();
    }
    if (X86_MEQ(mnem, "rtcc_wb")) {
        unsigned vm = x86_rtcc_live_mask(); if (!vm) return std::string();
        uint64_t block = (uint64_t)(uintptr_t)g_rtccb;
        return MEDIUM_BINARY ? x86_Lrec(x86_rtcc_wb_bin(block, RTCC_C_ALL & vm)) : x86_rtcc_wb_text(RTCC_C_ALL & vm);
    }
    if (X86_MEQ(mnem, "rtcc_rl")) {
        unsigned vm = x86_rtcc_live_mask(); if (!vm) return std::string();
        uint64_t block = (uint64_t)(uintptr_t)g_rtccb;
        return MEDIUM_BINARY ? x86_Lrec(x86_rtcc_rl_bin(block, RTCC_C_ALL & vm)) : x86_rtcc_rl_text(RTCC_C_ALL & vm);
    }
    if (X86_MEQ(mnem, "rtcc_anchor_cmp")) {
        if (!RTCC_GLOBAL_R8_ANCHOR) {
            uint64_t anchor_addr = (uint64_t)(uintptr_t)(const void *)rt_anchor_ptr();
            if (MEDIUM_BINARY) {
                std::string s;
                s += (char)0x48; s += (char)0xB9; s += u64le(anchor_addr);
                s += (char)0x4C; s += (char)0x8B; s += (char)0x01;
                s += (char)0x48; s += (char)0x83; s += (char)0xF8; s += (char)0x00;
                return x86_Lrec(s);
            }
            return x86_rec("mov") + "rcx, qword ptr [rip + g_runtime@GOTPCREL]\n mov rax, qword ptr [rcx + " + std::to_string(GRT_AT(anchor)) + "]\n cmp rax, 0\n";
        }
        if (MEDIUM_BINARY) { std::string s; s += (char)0x4D; s += (char)0x85; s += (char)0xC0; return x86_Lrec(s); }
        return x86_rec("test") + "r8, r8\n";
    }
    if (X86_MEQ(mnem, "push")) return x86_push(a.txt);
    if (X86_MEQ(mnem, "pop")) return x86_pop(a.txt);
    if (X86_MEQ(mnem, "idiv")) return x86_idiv(a.txt);
    if (X86_MEQ(mnem, "neg")) return x86_neg(a.txt);
    if (X86_MEQ(mnem, "inc")) {
        if (a.kind == XK_FR64) return x86_frame_inc64(a.off);
        if (a.kind == XK_REG) return x86_inc_r(a.txt);
        if (a.kind == XK_REGDISP) return x86_reg_disp32_inc64(a.base, a.off);
        if (a.kind == XK_RSP64) return x86_reg_disp32_inc64("rsp", a.off);
        return x86_bomb(
            "x86: inc operand kind unencodable -- an encoder that cannot encode must refuse loudly, never emit nothing (the silent empty here deleted the to-box increment from every armed generator "
            "graph; s283)");
    }
    if (X86_MEQ(mnem, "movabs")) {
        if (a.kind == XK_REG && b.kind == XK_IMM) return x86_movabs_r64(a.txt, (uint64_t)b.imm);
    }
    if (X86_MEQ(mnem, "mov")) {
        if (a.kind == XK_REG && b.kind == XK_R13RCX && a.txt && a.txt[0] == 'r' && a.txt[strlen(a.txt) - 1] != 'd') return x86_mov_subj_q(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_R13RCX) return x86_mov_subj_d(a.txt, b.off);
        if (a.kind == XK_FR32 && b.kind == XK_REG) return x86_frame_store(a.off, b.txt);
        if (a.kind == XK_FR32 && b.kind == XK_IMM) return x86_frame_mov_imm(a.off, b.imm);
        if (a.kind == XK_FR64 && b.kind == XK_REG) return x86_frame_store64(a.off, b.txt);
        if (a.kind == XK_FR64 && b.kind == XK_IMM) return x86_frame_mov_imm64(a.off, b.imm);
        if (a.kind == XK_RSP64 && b.kind == XK_REG) return x86_rsp_store64(a.off, b.txt);
        if (a.kind == XK_RSP64 && b.kind == XK_IMM) return x86_rsp_store64_imm(a.off, b.imm);
        if (a.kind == XK_RSP32 && b.kind == XK_REG) return x86_rsp_store32(a.off, b.txt);
        if (a.kind == XK_RSP32 && b.kind == XK_IMM) return x86_rsp_store32_imm(a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_FR32) return x86_frame_load(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_FR64) return x86_frame_load64(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_ROSLOT) return x86_ro_load_q(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_RSP64) return x86_rsp_load64(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_RSP32) return x86_rsp_load32(a.txt, b.off);
        if (a.kind == XK_REGDISP && b.kind == XK_REG) return x86_reg_disp32_store64(a.base, a.off, b.txt);
        if (a.kind == XK_REGDISP && b.kind == XK_IMM) return x86_reg_disp32_store_imm64(a.base, a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_REGDISP) return x86_reg_disp32_load64(a.txt, b.base, b.off);
        if (a.kind == XK_ABS64 && b.kind == XK_REG) return x86_abs_disp32_store64(a.imm, b.txt);
        if (a.kind == XK_REG && b.kind == XK_ABS64) return x86_abs_disp32_load64(a.txt, b.imm);
        if (a.kind == XK_REGDISP32 && b.kind == XK_REG) return x86_reg_disp32_store32(a.base, a.off, b.txt);
        if (a.kind == XK_REGDISP32 && b.kind == XK_IMM) return x86_reg_disp32_store_imm32(a.base, a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_REGDISP32) return x86_reg_disp32_load32(a.txt, b.base, b.off);
        if (a.kind == XK_REG && b.kind == XK_MEMIDX8) return x86_load_indexed8(a.txt, b.base, b.idx);
        if (a.kind == XK_REG && b.kind == XK_MEMIND) return x86_load_mem64(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_RIPGOT) return x86_load_got(a.txt, xd.s, xc.u);
        if (a.kind == XK_REG && b.kind == XK_RIPSEAL) return x86_load_ro(a.txt, xd.s, xc.u);
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_mov(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM) return x86_movimm(a.txt, b.imm);
        fprintf(stderr, "FATAL x86(\"mov\"): no dispatch arm for operand pair kinds (%d, %d) — dest '%s', src '%s'.  Add the encoder + dispatch case here (R7); never let a mov emit nothing.\n",
                a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
        abort();
    }
    if (X86_MEQ(mnem, "mov32")) { if (a.kind == XK_REG && b.kind == XK_IMM) return x86_movimm32(a.txt, b.imm); return std::string(); }
    if (X86_MEQ(mnem, "stk32")) { if (a.kind == XK_IMM && b.kind == XK_IMM) return x86_rsp_store32_imm((int)a.imm, b.imm); return std::string(); }
    if (X86_MEQ(mnem, "movabs")) { if (a.kind == XK_REG && xb.tag == 2) return x86_movabs_r64(a.txt, xb.u); return std::string(); }
    if (X86_MEQ(mnem, "xor")) { if (a.kind == XK_REG && b.kind == XK_REG) return x86_xor_rr(a.txt, b.txt); return std::string(); }
    if (X86_MEQ(mnem, "lea")) {
        if (a.kind == XK_REG && b.kind == XK_ILBL) return x86_lea_rip_id(a.txt, b.lbl);
        if (a.kind == XK_REG && b.kind == XK_RIPSEAL) return x86_load_ro(a.txt, xd.s, xc.u);
        if (a.kind == XK_REG && b.kind == XK_RIPGOT) return x86_load_got(a.txt, xd.s, xc.u);
        if (a.kind == XK_REG && b.kind == XK_EXTLBL && xc.tag == 2) return x86_lea_ext(a.txt, (const struct bb_label_t *)(uintptr_t)xc.u);
        if (a.kind == XK_REG && (b.kind == XK_FR32 || b.kind == XK_FR64)) return x86_frame_lea(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_REGDISP) return x86_reg_disp32_lea64(a.txt, b.base, b.off);
        if (a.kind == XK_REG && b.kind == XK_REGDISP32) return x86_reg_disp32_lea64(a.txt, b.base, b.off);
        if (a.kind == XK_REG && b.kind == XK_R13RCX) return x86_lea_subj_cursor(a.txt);
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_lea_subj_cursor(a.txt);
        if (a.kind == XK_REG && (b.kind == XK_RSP32 || b.kind == XK_RSP64)) return x86_reg_disp32_lea64(a.txt, "rsp", b.off);
        if (b.txt && strstr(b.txt, "rip")) return x86_bomb("lea: unsealed [rip + label] operand — use the [rip + __] sealed form with (ptr,label) args");
        if (b.kind == XK_RSP32 || b.kind == XK_RSP64 || b.kind == XK_FR32 || b.kind == XK_FR64 || b.kind == XK_REGDISP || b.kind == XK_REGDISP32) {
            fprintf(stderr,
                "FATAL x86(\"lea\"): no dispatch arm for frame/cell operand kind %d — dest '%s', src '%s'.  A lea that emits nothing is the ZB-FC-1 silent-drop corruption class (measured: bb_match"
                "_arbno's PAIR(2)/PAIR(3) view leas were dropped for months, masked only by r12 being callee-saved); add the encoder + dispatch case here (R7).\n",
                    b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
            abort();
        }
        return std::string();
    }
    if (X86_MEQ(mnem, "add")) {
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_add_rr(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM) return x86_add(a.txt, b.imm);
        if (a.kind == XK_ABS64 && b.kind == XK_IMM) return x86_abs_disp32_addsub_imm8(0, a.imm, b.imm);
        if (a.kind == XK_REG && b.kind == XK_FR32) return x86_frame_add_to_reg(a.txt, b.off);
        if (a.kind == XK_FR32 && b.kind == XK_IMM) return x86_frame_add_imm(a.off, b.imm);
        if (a.kind == XK_REGDISP32 && b.kind == XK_IMM) return x86_reg_disp32_add_imm32(a.base, a.off, b.imm);
        if (a.kind == XK_REGDISP && b.kind == XK_IMM) return x86_reg_disp_add_imm64(a.base, a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_REGDISP32) return x86_reg_disp32_add32(a.txt, b.base, b.off);
        if (a.kind == XK_REG && b.kind == XK_RSP32) return x86_rsp_add_to_reg32(a.txt, b.off);
        if (a.kind == XK_RSP32 && b.kind == XK_IMM) return x86_rsp_add_imm32(a.off, b.imm);
        if (a.kind == XK_FR32 || a.kind == XK_FR64 || a.kind == XK_RSP32 || a.kind == XK_RSP64 || a.kind == XK_REGDISP || a.kind == XK_REGDISP32 || b.kind == XK_FR32 || b.kind == XK_FR64 ||
            b.kind == XK_RSP32 || b.kind == XK_RSP64 || b.kind == XK_REGDISP || b.kind == XK_REGDISP32) {
            fprintf(stderr,
                "FATAL x86(\"add\"): no dispatch arm for frame/cell operand pair kinds (%d, %d) — dest '%s', src '%s'.  A frame/cell access that emits nothing is the ZB-FC-1 silent-drop corruption"
                " class (the mov precedent, 2026-07-08); add the encoder + dispatch case here (R7).\n",
                    a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
            abort();
        }
        return std::string();
    }
    if (X86_MEQ(mnem, "sub")) {
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_sub_rr(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM) return x86_sub(a.txt, b.imm);
        if (a.kind == XK_ABS64 && b.kind == XK_IMM) return x86_abs_disp32_addsub_imm8(1, a.imm, b.imm);
        if (a.kind == XK_REG && b.kind == XK_FR32) return x86_frame_sub_from_reg(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_RSP32) return x86_rsp_sub_from_reg32(a.txt, b.off);
        if (a.kind == XK_FR32 || a.kind == XK_FR64 || a.kind == XK_RSP32 || a.kind == XK_RSP64 || b.kind == XK_FR32 || b.kind == XK_FR64 || b.kind == XK_RSP32 || b.kind == XK_RSP64) {
            fprintf(stderr,
                "FATAL x86(\"sub\"): no dispatch arm for frame/cell operand pair kinds (%d, %d) — dest '%s', src '%s'.  A frame/cell access that emits nothing is the ZB-FC-1 silent-drop corruption"
                " class (the mov precedent, 2026-07-08); add the encoder + dispatch case here (R7).\n",
                    a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
            abort();
        }
        return std::string();
    }
    if (X86_MEQ(mnem, "imul")) { return x86_imul_rr(a.txt, b.txt); }
    if (X86_MEQ(mnem, "and")) {
        if (b.kind == XK_IMM) return x86_and(a.txt, b.imm);
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_alu_rr("and", 0x21, a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_REGDISP) return x86_reg_disp32_and_r64(a.txt, b.base, b.off);
        if (a.kind == XK_FR32 || a.kind == XK_FR64 || a.kind == XK_RSP32 || a.kind == XK_RSP64 || a.kind == XK_REGDISP || a.kind == XK_REGDISP32 ||
            b.kind == XK_FR32 || b.kind == XK_FR64 || b.kind == XK_RSP32 || b.kind == XK_RSP64 || b.kind == XK_REGDISP || b.kind == XK_REGDISP32) {
            fprintf(stderr,
                "FATAL x86(\"and\"): no dispatch arm for frame/cell operand pair kinds (%d, %d) — dest '%s', src '%s'.  An and that emits nothing leaves a following branch testing STALE FLAGS — "
                "the ZB-FC-1 silent-drop class; add the encoder + dispatch case here (R7).\n",
                    a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
            abort();
        }
        return std::string();
    }
    if (X86_MEQ(mnem, "or")) {
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_alu_rr("or", 0x09, a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM) return x86_or_imm(a.txt, b.imm);
        if (a.kind == XK_REG && b.kind == XK_REGDISP) return x86_reg_disp32_or_r64(a.txt, b.base, b.off);
        if (a.kind == XK_FR32 || a.kind == XK_FR64 || a.kind == XK_RSP32 || a.kind == XK_RSP64 || a.kind == XK_REGDISP || a.kind == XK_REGDISP32 ||
            b.kind == XK_FR32 || b.kind == XK_FR64 || b.kind == XK_RSP32 || b.kind == XK_RSP64 || b.kind == XK_REGDISP || b.kind == XK_REGDISP32) {
            fprintf(stderr,
                "FATAL x86(\"or\"): no dispatch arm for frame/cell operand pair kinds (%d, %d) — dest '%s', src '%s'.  An or that emits nothing leaves a following branch testing STALE FLAGS — th"
                "e ZB-FC-1 silent-drop class; add the encoder + dispatch case here (R7).\n",
                    a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
            abort();
        }
        return std::string();
    }
    if (X86_MEQ(mnem, "shl") && a.kind == XK_REG && b.kind == XK_IMM) return x86_shift_imm("shl", 4, a.txt, b.imm);
    if (X86_MEQ(mnem, "shr") && a.kind == XK_REG && b.kind == XK_IMM) return x86_shift_imm("shr", 5, a.txt, b.imm);
    if (X86_MEQ(mnem, "cmp")) {
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_cmp(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM) return x86_cmp_imm(a.txt, b.imm);
        if (a.kind == XK_REG && b.kind == XK_ABS64) return x86_cmp_reg_abs64(a.txt, b.imm);
        if (a.kind == XK_REGDISP && b.kind == XK_IMM) return x86_reg_disp32_cmp_imm(a.base, a.off, b.imm);
        if (a.kind == XK_RSP64 && b.kind == XK_IMM) return x86_reg_disp32_cmp_imm("rsp", a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_REGDISP) return x86_reg_disp32_cmp_r64(a.txt, b.base, b.off);
        if (a.kind == XK_FR32 || a.kind == XK_FR64 || a.kind == XK_RSP32 || a.kind == XK_RSP64 || a.kind == XK_REGDISP || a.kind == XK_REGDISP32 ||
            b.kind == XK_FR32 || b.kind == XK_FR64 || b.kind == XK_RSP32 || b.kind == XK_RSP64 || b.kind == XK_REGDISP || b.kind == XK_REGDISP32) {
            fprintf(stderr,
                "FATAL x86(\"cmp\"): no dispatch arm for frame/cell operand pair kinds (%d, %d) — dest '%s', src '%s'.  A cmp that emits nothing leaves the following jcc testing STALE FLAGS — th"
                "e ZB-FC-1 silent-drop class (measured s23o: SPD-2's guard cmps vanished when RDQ(\"___\",·) collided with the pinned fr64 prefix and parsed XK_FR64; only the .s region diff caught "
                "it, the probes stayed green on garbage flags).  Add the encoder + dispatch case here (R7).\n",
                    a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
            abort();
        }
        return std::string();
    }
    if (X86_MEQ(mnem, "cmp64")) { if (b.kind == XK_IMM) return x86_cmp_imm64(a.txt, b.imm); return std::string(); }
    if (X86_MEQ(mnem, "test")) { return x86_test(a.txt, b.txt); }
    if (X86_MEQ(mnem, "movsxd")) {
        if (a.kind == XK_REG && b.kind == XK_FR32) return x86_movsxd_frame(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_movsxd(a.txt, b.txt);
        fprintf(stderr,
            "FATAL x86(\"movsxd\"): no dispatch arm for operand pair kinds (%d, %d) — dest '%s', src '%s'.  A movsxd emitting the reg/reg form for a memory source is the mode-3/mode-4 divergence c"
            "lass (scan-nary saved_delta, 2026-07-13); add the encoder + dispatch case here (R7).\n",
                a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
        abort();
    }
    if (X86_MEQ(mnem, "movzx")) {
        if (b.kind == XK_MEMBI) return x86_movzx_bir(a.txt, b.base, b.idx);
        if (b.kind == XK_REG) {
            int g = x86_rnum(a.txt), m = x86_rnum(b.txt);
            uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
            uint8_t modrm = (uint8_t)(0xC0 | ((g & 7) << 3) | (m & 7));
            if (MEDIUM_BINARY) return x86_Lrec(x86_b4(rex, 0x0F, 0xB6, modrm));
            return x86_rec("movzx") + a.txt + std::string(", ") + b.txt + std::string("\n");
        }
        return x86_movzx_subj_byte(a.txt, b.kind == XK_R13RCX ? b.off : 0);
    }
    if (X86_MEQ(mnem, "cmpb0")) { (void)a; (void)b; return x86_cset_probe(); }
    if (X86_MEQ(mnem, "bt")) { (void)a; (void)b; return x86_cset_bt(); }
    if (X86_MEQ(mnem, "xorps")) { return x86_xorps_xmm0(); }
    if (X86_MEQ(mnem, "movups")) { if (a.kind == XK_REGDISP && b.kind == XK_REG && !strcmp(b.txt, "xmm0")) return x86_movups_store_xmm0(a.base, a.off);
        return x86_bomb("movups: only [reg + disp], xmm0 is encodable -- an encoder that cannot encode refuses loudly"); }
    if (X86_MEQ(mnem, "movsd")) {
        if (b.txt && !strncmp(b.txt, "f64:", 4)) { uint64_t bits = strtoull(b.txt + 4, 0, 10); double d; memcpy(&d, &bits, 8); return x86_set_xmm0_double(d); }
        return std::string();
    }
    if (X86_MEQ(mnem, "movq")) {
        if (a.kind == XK_REG && b.kind == XK_REG && a.txt && !strncmp(a.txt, "xmm", 3)) return x86_movq_xmm_r64(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_REG && b.txt && !strncmp(b.txt, "xmm", 3)) return x86_movq_r64_xmm(a.txt, b.txt);
        return std::string();
    }
    if (X86_MEQ(mnem, "addsd")) { if (a.kind == XK_REG && b.kind == XK_REG) return x86_sse2_xx("addsd", 0x58, a.txt, b.txt); return std::string(); }
    if (X86_MEQ(mnem, "subsd")) { if (a.kind == XK_REG && b.kind == XK_REG) return x86_sse2_xx("subsd", 0x5C, a.txt, b.txt); return std::string(); }
    if (X86_MEQ(mnem, "mulsd")) { if (a.kind == XK_REG && b.kind == XK_REG) return x86_sse2_xx("mulsd", 0x59, a.txt, b.txt); return std::string(); }
    if (X86_MEQ(mnem, "cvtsi2sd")) { if (a.kind == XK_REG && b.kind == XK_REG) return x86_cvtsi2sd_xmm_r64(a.txt, b.txt); return std::string(); }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bool x86_is_scan_builtin_name(const char *fn) {
    if (!fn) return false;
    return !strcmp(fn, "tab") || !strcmp(fn, "move") || !strcmp(fn, "upto") || !strcmp(fn, "many")
        || !strcmp(fn, "any") || !strcmp(fn, "find") || !strcmp(fn, "match")|| !strcmp(fn, "pos")
        || !strcmp(fn, "bal");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_bomb(const char * msg) {
    const char * m = msg ? msg : "(unimplemented box)";
    const char * lbl = emit_intern_str(m);
    char lblbuf[24];
    if (!MEDIUM_BINARY && (!lbl || !lbl[0])) { strtab_label(lblbuf, sizeof lblbuf, m); lbl = lblbuf; }
    uint64_t fp = (uint64_t)(uintptr_t)(void *)rt_bomb;
    return x86_load_ro("rdi", lbl, (uint64_t)(uintptr_t)(const void *)m)
         + x86_call_ro("rt_bomb", fp)
         + (MEDIUM_BINARY ? x86_Lrec(x86_b2(0x0F, 0x0B)) : x86_recn("ud2") + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int x86_ir_digit(char c) { return c >= '0' && c <= '9'; }
long x86_ir_num(const std::string & s, size_t & i) { long v = 0; size_t n = s.size(); while (i < n && x86_ir_digit(s[i])) v = v * 10 + (s[i++] - '0'); return v; }
static void x86_text_rsp_notes_strip(std::string & r) {
    std::string o; o.reserve(r.size()); size_t i = 0, n = r.size();
    while (i < n) { size_t e = r.find('\n', i); if (e == std::string::npos) e = n; else e++; if (r.compare(i, 5, "#@rsp") != 0) o.append(r, i, e - i); i = e; }
    r.swap(o);
}
void bb_emit_x86(const std::string & s) {
    if (!MEDIUM_BINARY) { if (!s.empty()) { std::string r = x86_internal_resolve(s); { x86_rsp_pre_t before = x86_rsp_prepass_open(); x86_text_sites_scan(r, false);
        x86_rsp_prepass_commit(before); } x86_text_sites_scan(r, true); x86_text_rsp_notes_strip(r); emit_text_n(r.data(), r.size()); } return; }
    int hi_internal = -1;
    { x86_rsp_pre_t before = x86_rsp_prepass_open(); x86_rsp_st_t & S = x86_rsp_S(); size_t i = 0, n = s.size();
      while (i < n) { char tag = s[i++];
        if (tag == 'L') { int k = (unsigned char)s[i++]; i += (size_t)k; if (k > 0) S.dead = 0; }
        else if (tag == 'J') { int id = x86_rec_id(s, i); if (id - X86_INTERNAL_BASE > hi_internal) hi_internal = id - X86_INTERNAL_BASE; x86_rsp_jump(1, (uint64_t)id); }
        else if (tag == 'D') { int id = x86_rec_id(s, i); if (id - X86_INTERNAL_BASE > hi_internal) hi_internal = id - X86_INTERNAL_BASE; if (id == X86P_ALPHA) { S.delta = 0; S.dead = 0;
            S.unknown = 0;
            } else if (id == X86P_BETA) { S.delta = _.op_fc_bytes > 0 ? (int)_.op_fc_bytes : 0; S.dead = 0; S.unknown = 0; } else x86_rsp_define(1, (uint64_t)id); }
        else if (tag == 'Q') { int id = x86_rec_id(s, i); if (id - X86_INTERNAL_BASE > hi_internal) hi_internal = id - X86_INTERNAL_BASE; }
        else if (tag == 'E') { int idx = x86_pair_idx_read(s, i); x86_rsp_define(2, (uint64_t)idx); }
        else if (tag == 'F') { int idx = x86_pair_idx_read(s, i); x86_rsp_jump(2, (uint64_t)idx); }
        else if (tag == 'X' || tag == 'Y') { uint64_t v = 0; for (int j = 0; j < 8; j++) v |= ((uint64_t)(unsigned char)s[i++]) << (8 * j); if (tag == 'X') x86_rsp_jump(3, v);
            else x86_rsp_define(3, v); }
        else if (tag == 'G') { S.dead = 1; }
        else if (tag == 'K') { S.mark = S.delta; S.mark_unknown = S.unknown; }
        else if (tag == 'M') { x86_rsp_mark9_push(); }
        else if (tag == 'N') { x86_rsp_mark9_pop(); }
        else if (tag == 'k' || tag == 'R' || tag == 'S') { uint32_t v = 0; for (int j = 0; j < 4; j++) v |= ((uint32_t)(unsigned char)s[i++]) << (8 * j);
            if (tag == 'k') { S.delta = S.mark + (int32_t)v; S.dead = 0; S.unknown = S.mark_unknown; } else if (tag == 'R' && !S.aligned) S.delta += (int32_t)v; }
        else if (tag == 'U') { if (!S.aligned) S.unknown = 1; }
        else if (tag == 'A') { S.aligned = 1; S.before = S.delta; }
        else if (tag == 'Z') { S.aligned = 0; S.delta = S.before; }
        else break; }
      x86_rsp_prepass_commit(before); }
    std::vector<bb_label_t> internal((size_t)(hi_internal + 1)); std::vector<std::array<char, 16>> inm(internal.size());
    for (size_t k = 0; k < internal.size(); k++) { internal[k].offset = BB_LABEL_UNRESOLVED; internal[k].name = ""; internal[k].name_cap = 0; }
    size_t i = 0, n = s.size(); x86_rsp_st_t & RS = x86_rsp_S();
    while (i < n) {
        char tag = s[i++];
        if (tag == 'L') { int k = (unsigned char)s[i++]; bb_emit_bytes((const uint8_t *)s.data() + i, (size_t)k); i += (size_t)k; if (k > 0) RS.dead = 0; }
        else if (tag == 'J') { int id = x86_rec_id(s, i); x86_rsp_jump(1, (uint64_t)id); bb_emit_patch_rel32(x86_label_for(id, internal, inm)); }
        else if (tag == 'D') { int id = x86_rec_id(s, i);
            if (id >= X86_INTERNAL_BASE && (size_t)(id - X86_INTERNAL_BASE) < internal.size() && internal[(size_t)(id - X86_INTERNAL_BASE)].offset != BB_LABEL_UNRESOLVED)
            { fprintf(stderr, "FATAL bb_emit_x86: internal label L(%d) defined twice in one %s box\n", id - X86_INTERNAL_BASE, x86_boxkind()); abort(); }
            if (id == X86P_ALPHA) { RS.delta = 0; RS.dead = 0; RS.unknown = 0;
            } else if (id == X86P_BETA) { RS.delta = _.op_fc_bytes > 0 ? (int)_.op_fc_bytes : 0; RS.dead = 0; RS.unknown = 0; } else x86_rsp_define(1, (uint64_t)id);
            bb_label_define(x86_label_for(id, internal, inm)); }
        else if (tag == 'Q') { int id = x86_rec_id(s, i); bb_emit_patch_abs64(x86_label_for(id, internal, inm)); }
        else if (tag == 'E') { int idx = x86_pair_idx_read(s, i); x86_rsp_define(2, (uint64_t)idx); if (XA_PAIR(idx).define) bb_label_define(XA_PAIR(idx).define); }
        else if (tag == 'F') { int idx = x86_pair_idx_read(s, i); x86_rsp_jump(2, (uint64_t)idx); bb_label_t * _t = x86_pair_tgt(idx); if (_t) bb_emit_patch_rel32(_t); }
        else if (tag == 'X') { uint64_t v = 0; for (int j = 0; j < 8; j++) v |= ((uint64_t)(unsigned char)s[i++]) << (8 * j); x86_rsp_jump(3, v); bb_emit_patch_rel32((bb_label_t *)(uintptr_t)v); }
        else if (tag == 'Y') { uint64_t v = 0; for (int j = 0; j < 8; j++) v |= ((uint64_t)(unsigned char)s[i++]) << (8 * j); x86_rsp_define(3, v); bb_label_define((bb_label_t *)(uintptr_t)v); }
        else if (tag == 'G') { RS.dead = 1; }
        else if (tag == 'K') { RS.mark = RS.delta; RS.mark_unknown = RS.unknown; }
        else if (tag == 'M') { x86_rsp_mark9_push(); }
        else if (tag == 'N') { x86_rsp_mark9_pop(); }
        else if (tag == 'k') { uint32_t v = 0; for (int j = 0; j < 4; j++) v |= ((uint32_t)(unsigned char)s[i++]) << (8 * j); RS.delta = RS.mark + (int32_t)v; RS.dead = 0;
            RS.unknown = RS.mark_unknown; }
        else if (tag == 'S') { uint32_t v = 0; for (int j = 0; j < 4; j++) v |= ((uint32_t)(unsigned char)s[i++]) << (8 * j); extern bb_buf_t bb_emit_buf; extern int bb_emit_pos;
            emit_gc_site_play((int)v, (const void *)(bb_emit_buf + bb_emit_pos), g_x86_rsp_aligned ? g_x86_rsp_before : g_x86_rsp_delta, g_x86_rsp_aligned, g_x86_rsp_unknown); }
        else if (tag == 'R') { uint32_t v = 0; for (int j = 0; j < 4; j++) v |= ((uint32_t)(unsigned char)s[i++]) << (8 * j); if (!g_x86_rsp_aligned) g_x86_rsp_delta += (int32_t)v; }
        else if (tag == 'U') { if (!g_x86_rsp_aligned) g_x86_rsp_unknown = 1; }
        else if (tag == 'A') { g_x86_rsp_aligned = 1; g_x86_rsp_before = g_x86_rsp_delta; }
        else if (tag == 'Z') { g_x86_rsp_aligned = 0; g_x86_rsp_delta = g_x86_rsp_before; }
        else break;
    }
}
long x86_rec_bytes(const std::string & s) { long n = 0; size_t i = 0; while (i < s.size()) { char t = s[i++]; if (t == 'L') { int k = (unsigned char)s[i++]; n += k; i += (size_t)k;
    } else if (t == 'J') { i += 2; n += 4; } else if (t == 'F') { i += 4; n += 4; } else if (t == 'X') { i += 8; n += 4; } else if (t == 'D') { i += 2; } else if (t == 'E') { i += 4;
    } else if (t == 'Y') { i += 8; } else if (t == 'Q') { i += 2; n += 8; } else if (t == 'S') { i += 4; } else if (t == 'R') { i += 4;
    } else if (t == 'U' || t == 'A' || t == 'Z' || t == 'G' || t == 'K' || t == 'M' || t == 'N') { } else if (t == 'k') { i += 4; } else return -1; } return n; }
