#ifndef X86_ASM_H
#define X86_ASM_H
#include <string>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "emit.h"
#include "zeta_choices.h"
#include "pin_va.h"
extern "C" {
extern uint64_t rtccb[32];
extern unsigned char g_rtcc_on;
long *rt_anchor_ptr(void);
}
#include "rtx/rtcc.h"
#ifndef _
#define _ g_emit
#endif
extern "C" {
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int x86_tabs_on(void) { static int t = -1; if (t < 0) { const char * e = getenv("SCRIP_ASM_TABS"); t = (e && *e == '0') ? 0 : 1; } return t; }
inline std::string x86_rec(const char * op) { return x86_tabs_on() ? (std::string("\t") + op + "\t") : (std::string(" ") + op + " "); }
inline std::string x86_recn(const char * op) { return x86_tabs_on() ? (std::string("\t") + op) : (std::string(" ") + op); }
inline std::string x86_reclbl(const std::string & nm) { return x86_tabs_on() ? (nm + ":\t") : (nm + ":"); }
static inline int x86_4col_joinon(void) { static int j = -1; if (j < 0) { const char * e = getenv("SCRIP_ASM_JOIN"); j = (e && *e == '0') ? 0 : 1; } return j; }
static inline int x86_col4(void) { return x86_4col_joinon() ? 78 : 88; }
enum { X86P_ALPHA = 0, X86P_BETA = 1, X86P_GAMMA = 2, X86P_OMEGA = 3 };
#define PORT_ALPHA   "\xCE\xB1"
#define PORT_BETA    "\xCE\xB2"
#define PORT_GAMMA   "\xCE\xB3"
#define PORT_OMEGA   "\xCF\x89"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_rnum(const char * r) {
    if (!r) return 0;
    if (!strcmp(r, "eax") || !strcmp(r, "rax")) return 0;
    if (!strcmp(r, "ecx") || !strcmp(r, "rcx")) return 1;
    if (!strcmp(r, "edx") || !strcmp(r, "rdx")) return 2;
    if (!strcmp(r, "ebx") || !strcmp(r, "rbx")) return 3;
    if (!strcmp(r, "esp") || !strcmp(r, "rsp")) return 4;
    if (r[0]==0x72 && r[1]==0x62 && r[2]==0x70 && !r[3]) return 5;
    if (!strcmp(r, "esi") || !strcmp(r, "rsi")) return 6;
    if (!strcmp(r, "edi") || !strcmp(r, "rdi")) return 7;
    if (!strcmp(r, "r8d")  || !strcmp(r, "r8"))  return 8;
    if (!strcmp(r, "r9d")  || !strcmp(r, "r9"))  return 9;
    if (!strcmp(r, "r10d") || !strcmp(r, "r10")) return 10;
    if (!strcmp(r, "r11d") || !strcmp(r, "r11")) return 11;
    if (!strcmp(r, "r12d") || !strcmp(r, "r12")) return 12;
    if (!strcmp(r, "r13d") || !strcmp(r, "r13")) return 13;
    if (!strcmp(r, "r14d") || !strcmp(r, "r14")) return 14;
    if (!strcmp(r, "r15d") || !strcmp(r, "r15")) return 15;
    if (!strcmp(r, "al"))  return 0; if (!strcmp(r, "cl"))  return 1;
    if (!strcmp(r, "dl"))  return 2; if (!strcmp(r, "bl"))  return 3;
    if (!strcmp(r, "spl")) return 4; if (!strcmp(r, "bpl")) return 5;
    if (!strcmp(r, "sil")) return 6; if (!strcmp(r, "dil")) return 7;
    if (!strcmp(r, "r8b"))  return 8;  if (!strcmp(r, "r9b"))  return 9;
    if (!strcmp(r, "r10b")) return 10; if (!strcmp(r, "r11b")) return 11;
    if (!strcmp(r, "r12b")) return 12; if (!strcmp(r, "r13b")) return 13;
    if (!strcmp(r, "r14b")) return 14; if (!strcmp(r, "r15b")) return 15;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * x86_portname(int p) {
    switch (p) { case X86P_ALPHA: return _.lbl_α; case X86P_BETA: return _.lbl_β;
                 case X86P_GAMMA: return _.lbl_γ; default: return _.lbl_ω; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline struct bb_label_t * x86_portlbl(int p) {
    switch (p) { case X86P_ALPHA: return _.lbl_α_p; case X86P_BETA: return _.lbl_β_p;
                 case X86P_GAMMA: return _.lbl_γ_p; default: return _.lbl_ω_p; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char *           x86_tgt0()    { return _.lbl_t0; }
inline struct bb_label_t *    x86_tgt0_p()  { return _.lbl_t0_p; }
inline const char *           x86_tgt1()    { return _.lbl_t1; }
inline struct bb_label_t *    x86_tgt1_p()  { return _.lbl_t1_p; }
inline std::string x86_Lrec(const std::string & b) { std::string r; r += (char)'L'; r += (char)(unsigned char)b.size(); r += b; return r; }
inline std::string x86_Jrec(int port)              { std::string r; r += (char)'J'; r += (char)(unsigned char)port; return r; }
inline std::string x86_Drec(int port)              { std::string r; r += (char)'D'; r += (char)(unsigned char)port; return r; }
inline std::string x86_b1(uint8_t a)                          { std::string s; s += (char)a; return s; }
inline std::string x86_b2(uint8_t a, uint8_t b)               { std::string s; s += (char)a; s += (char)b; return s; }
inline std::string x86_b3(uint8_t a, uint8_t b, uint8_t c)    { std::string s; s += (char)a; s += (char)b; s += (char)c; return s; }
inline std::string x86_b4(uint8_t a, uint8_t b, uint8_t c, uint8_t d) { std::string s; s += (char)a; s += (char)b; s += (char)c; s += (char)d; return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline bool x86_is64(const char * r) {
    if (!r) return false;
    if (r[0] != 'r') return false;
    size_t n = strlen(r);
    if (r[n - 1] == 'd') return false;
    return true;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_is8 (row descr-stamp-asm-mints, found chasing a mode-3-only SIGSEGV): x86_rnum() maps "al"
 * to the SAME index as "eax" (0), by design, for callers like movzx where the surrounding opcode
 * already fixes the operand width. x86_cmp_imm (and every other x86_alu_rr-based ALU-immediate/
 * ALU-register encoder) had NO width detection at all, so a template requesting an 8-bit compare
 * — e.g. bb_subscript.cpp's `x86("cmp","al",(long)DT_FAIL)`, part of the 171-site conversion this
 * row's sibling row (descr-stamp-fields) landed to stop 32-bit tag-word compares from reading
 * mod_op/src_node — silently fell through to the 32-bit CMP r/m32,imm opcode in the BINARY medium
 * (mode-3's own hand-rolled encoder; mode-4's TEXT medium hands the identical "cmp al, N" string to
 * the real `as`, which encodes it correctly, so mode-4 was never wrong). Dormant since mod_op/
 * src_node were always zero before this row: a 32-bit compare against a value with zero upper
 * bytes gives the identical verdict to an 8-bit one. This row's stamping is the first thing to set
 * those bytes on a corpus-live path, which is what surfaced it (SIGSEGV via rt_deref's matching
 * 32-bit-compare defect in rt_asm_helpers.S, fixed separately; this one silently let `a<4>` on a
 * 3-element array read as in-bounds instead of failing). */
inline bool x86_is8(const char * r) {
    if (!r) return false;
    static const char * const names8[] = { "al","cl","dl","bl","spl","bpl","sil","dil",
                                            "r8b","r9b","r10b","r11b","r12b","r13b","r14b","r15b" };
    for (size_t i = 0; i < sizeof(names8) / sizeof(names8[0]); ++i) if (!strcmp(r, names8[i])) return true;
    return false;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_alu_rr(const char * mnem, uint8_t op, const char * rm, const char * reg) {
    int m = x86_rnum(rm), g = x86_rnum(reg);
    uint8_t rex = 0x40; if (x86_is64(rm) || x86_is64(reg)) rex |= 0x08; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; if (rex != 0x40) code += (char)rex; code += (char)op; code += (char)(0xC0 | ((g & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : x86_rec(mnem) + rm + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_mov (const char * rm, const char * reg) { return x86_alu_rr("mov",  0x89, rm, reg); }
inline std::string x86_cmp (const char * rm, const char * reg) { return x86_alu_rr("cmp",  0x39, rm, reg); }
inline std::string x86_test(const char * rm, const char * reg) { return x86_alu_rr("test", 0x85, rm, reg); }
inline std::string x86_xor_rr(const char * rm, const char * reg) { return x86_alu_rr("xor", 0x31, rm, reg); }
inline std::string x86_add_rr(const char * rm, const char * reg) { return x86_alu_rr("add", 0x01, rm, reg); }
inline std::string x86_sub_rr(const char * rm, const char * reg) { return x86_alu_rr("sub", 0x29, rm, reg); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_imul_rr(const char * dst, const char * src) {
    int g = x86_rnum(dst), m = x86_rnum(src);
    uint8_t rex = 0x40; if (x86_is64(dst) || x86_is64(src)) rex |= 0x08; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; if (rex != 0x40) code += (char)rex; code += (char)0x0F; code += (char)0xAF; code += (char)(0xC0 | ((g & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("imul") + dst + ", " + src + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_cqo() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b2(0x48, 0x99)) : x86_recn("cqo") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rep_stosb() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b2(0xF3, 0xAA)) : x86_rec("rep") + "stosb\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_xorps_xmm0() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b3(0x0F, 0x57, 0xC0)) : x86_rec("xorps") + "xmm0, xmm0\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movabs_r64(const char * dst, uint64_t imm) {
    int m = x86_rnum(dst);
    if (MEDIUM_BINARY) { std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le(imm); return x86_Lrec(code); }
    return x86_rec("movabs") + dst + ", " + std::to_string((unsigned long long)imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movq_xmm0_r64(const char * src) {
    int m = x86_rnum(src); uint8_t rex = 0x48; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)0x66; code += (char)rex; code += (char)0x0F; code += (char)0x6E; code += (char)(0xC0 | (0 << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movq") + "xmm0, " + src + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movq_xmm_r64(const char * dst, const char * src) {
    int xn = (dst && !strncmp(dst, "xmm", 3)) ? atoi(dst + 3) : 0; int m = x86_rnum(src); uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; if (xn >= 8) rex |= 0x04;
    std::string code; code += (char)0x66; code += (char)rex; code += (char)0x0F; code += (char)0x6E; code += (char)(0xC0 | ((xn & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movq") + dst + ", " + src + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_sse2_xx(const char * mnem, uint8_t op, const char * xd, const char * xs) {
    int d = (xd && !strncmp(xd, "xmm", 3)) ? atoi(xd + 3) : 0; int s = (xs && !strncmp(xs, "xmm", 3)) ? atoi(xs + 3) : 0;
    std::string code; code += (char)0xF2; uint8_t rex = 0x40; if (d >= 8) rex |= 0x04; if (s >= 8) rex |= 0x01; if (rex != 0x40) code += (char)rex;
    code += (char)0x0F; code += (char)op; code += (char)(0xC0 | ((d & 7) << 3) | (s & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec(mnem) + xd + ", " + xs + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_cvtsi2sd_xmm_r64(const char * xd, const char * src) {
    int d = (xd && !strncmp(xd, "xmm", 3)) ? atoi(xd + 3) : 0; int m = x86_rnum(src);
    std::string code; code += (char)0xF2; uint8_t rex = 0x48; if (d >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01; code += (char)rex;
    code += (char)0x0F; code += (char)0x2A; code += (char)(0xC0 | ((d & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("cvtsi2sd") + xd + ", " + src + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movq_r64_xmm(const char * dst, const char * xs) {
    int s = (xs && !strncmp(xs, "xmm", 3)) ? atoi(xs + 3) : 0; int m = x86_rnum(dst);
    uint8_t rex = 0x48; if (s >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)0x66; code += (char)rex; code += (char)0x0F; code += (char)0x7E; code += (char)(0xC0 | ((s & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movq") + dst + ", " + xs + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_set_xmm0_double(double d) {
    uint64_t bits = 0; memcpy(&bits, &d, sizeof bits);
    return x86_movabs_r64("rax", bits) + x86_movq_xmm0_r64("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_idiv(const char * reg) {
    int m = x86_rnum(reg); uint8_t rex = 0x48; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0xF7; code += (char)(0xC0 | (7 << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("idiv") + reg + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_neg(const char * reg) {
    int m = x86_rnum(reg); uint8_t rex = 0x48; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0xF7; code += (char)(0xC0 | (3 << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("neg") + reg + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_add(const char * reg, long imm) {
    int m = x86_rnum(reg); bool w = x86_is64(reg);
    std::string code;
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (m >= 8) rex |= 0x01;
    if (imm >= -128 && imm <= 127) { if (rex != 0x40) code += (char)rex; code += (char)0x83; code += (char)(0xC0 | (0 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else if (m == 0 && !w)         { code += (char)0x05; code += u32le((uint32_t)imm); }
    else                           { if (rex != 0x40) code += (char)rex; code += (char)0x81; code += (char)(0xC0 | (0 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("add") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_sub(const char * reg, long imm) {
    int m = x86_rnum(reg); bool w = x86_is64(reg);
    std::string code;
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (m >= 8) rex |= 0x01; if (rex != 0x40) code += (char)rex;
    if (imm >= -128 && imm <= 127) { code += (char)0x83; code += (char)(0xC0 | (5 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else                           { code += (char)0x81; code += (char)(0xC0 | (5 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("sub") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movsxd(const char * dst64, const char * src32) {
    int g = x86_rnum(dst64), m = x86_rnum(src32);
    uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0x63; code += (char)(0xC0 | ((g & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movsxd") + dst64 + ", " + src32 + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_lea_subj_cursor(const char * dst) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x48 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x8D; code += (char)modrm; code += (char)sib; code += (char)0x00;
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("lea") + dst + ", [r13 + rcx]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movzx_subj_byte(const char * dst, int disp) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x40 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x0F; code += (char)0xB6; code += (char)modrm; code += (char)sib; code += (char)(disp & 0xFF);
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movzx") + dst + ", byte ptr [r13+rcx" + (disp ? std::string("+") + std::to_string(disp) : std::string()) + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movzx_bir(const char * dst, const char * base, const char * idx) {
    int g = x86_rnum(dst), bn = x86_rnum(base), in = x86_rnum(idx);
    uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (in >= 8) rex |= 0x02; if (bn >= 8) rex |= 0x01;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | ((in & 7) << 3) | (bn & 7));
    std::string code; code += (char)rex; code += (char)0x0F; code += (char)0xB6; code += (char)modrm; code += (char)sib; code += (char)0x00;
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("movzx") + dst + ", byte ptr [" + base + " + " + idx + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_mov_subj_q(const char * dst, int disp) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x48 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x8B; code += (char)modrm; code += (char)sib; code += (char)(disp & 0xFF);
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("mov") + dst + ", qword ptr [r13+rcx" + (disp ? std::string("+") + std::to_string(disp) : std::string()) + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_mov_subj_d(const char * dst, int disp) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x41; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x8B; code += (char)modrm; code += (char)sib; code += (char)(disp & 0xFF);
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("mov") + dst + ", dword ptr [r13+rcx" + (disp ? std::string("+") + std::to_string(disp) : std::string()) + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_cset_probe() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b4(0x80, 0x3C, 0x37, 0x00)) : x86_rec("cmp") + "byte ptr [rdi+rsi], 0\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_cset_bt() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b3(0x0F, 0xA3, 0x37)) : x86_rec("bt") + "dword ptr [rdi], esi\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_push(const char * r) {
    int m = x86_rnum(r); std::string code; if (m >= 8) code += (char)0x41; code += (char)(0x50 | (m & 7)); return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("push") + r + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_pop (const char * r) {
    int m = x86_rnum(r); std::string code; if (m >= 8) code += (char)0x41; code += (char)(0x58 | (m & 7)); return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("pop")  + r + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movimm(const char * dst, long imm) {
    int m = x86_rnum(dst);
    if (MEDIUM_BINARY) { std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le((uint64_t)imm); return x86_Lrec(code); }
    return x86_rec("mov") + dst + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movimm32(const char * dst, long imm) {
    int m = x86_rnum(dst);
    if (MEDIUM_BINARY) { std::string code; if (m >= 8) code += (char)0x41; code += (char)(0xB8 | (m & 7)); code += u32le((uint32_t)imm); return x86_Lrec(code); }
    return x86_rec("mov") + dst + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_load_ro(const char * dst, const char * label, uint64_t ptr) {
    if (MEDIUM_BINARY) {
        int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le(ptr); return x86_Lrec(code);
    }
    return x86_rec("lea") + dst + ", [rip + " + (label ? label : "??") + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_load_got(const char * dst, const char * label, uint64_t ptr) {
    if (MEDIUM_BINARY) {
        int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le(ptr); return x86_Lrec(code);
    }
    return x86_rec("mov") + dst + ", qword ptr [rip + " + (label ? label : "??") + "@GOTPCREL]\n";
}
inline std::string x86_align_assert();
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_call_ro(const char * sym, uint64_t ptr) {
    if (MEDIUM_BINARY) { std::string code; code += (char)0x48; code += (char)0xB8; code += u64le(ptr); code += (char)0xFF; code += (char)0xD0; return x86_align_assert() + x86_Lrec(code); }
    return x86_align_assert() + x86_rec("call") + sym + "@PLT\n";
}
#define RTCC_C_R8   1u
#define RTCC_C_R9   2u
/* ⭐ r10/r11 JOIN THE VENEER (Lon s258): they carry the DIAG telemetry -- r10 = SNOBOL4 statement number,
   r11 = BB node id (see the stamp below, and c951f257). The stamp was emitted but NOT PROTECTED, so any
   runtime call could clobber it and a crash dump would name a stale statement with nothing marking it
   stale. rtcc.h already reserved slots 7 and 8 (rtccb+56, +64) and RTCC_GPR_COUNT is 9 -- the block was
   always big enough; only the save/restore was missing. Eradicating r10/r11 from 153 runtime asm lines
   was the hard way round; protecting them across the veneer is the cheap way. */
#define RTCC_C_R10  4u
#define RTCC_C_R11  8u
#define RTCC_C_ALL  (RTCC_C_R8 | RTCC_C_R9 | RTCC_C_R10 | RTCC_C_R11)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline bool x86_rtcc_noclob_on(void) { const char * e = getenv("SCRIP_RTCC_NOCLOB"); return !(e && *e == '0'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ MEASUREMENT KILLSWITCH (hq_P s262): SCRIP_RTCC_VENEER=0 emits NO save/restore around ANY runtime call, so the A/B on one binary pair is the veneer's exact cost.  ⛔ It is an EMIT-time switch -- toggling it on an already-baked .s proves nothing, the arm must be re-COMPILED.  ⛔ It is NOT a shipping configuration: with it off, any call that really does clobber a live r8/r9/r10/r11 answers wrong.  A `check: 1102` under it is EVIDENCE ABOUT LIVENESS, not a licence to ship. */
static inline unsigned x86_rtcc_veneer_mask(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_RTCC_VENEER"); v = (e && *e) ? (int)strtoul(e, 0, 0) : (int)RTCC_C_ALL; } return (unsigned)v; }
static inline int x86_rtcc_veneer_on(void) { return x86_rtcc_veneer_mask() != 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline unsigned x86_rtcc_clob_raw(const char * sym);
static inline unsigned x86_rtcc_clob(const char * sym) { return x86_rtcc_clob_raw(sym) & x86_rtcc_veneer_mask(); }
static inline unsigned x86_rtcc_clob_raw(const char * sym) {
    if (!sym) return RTCC_C_ALL;
    static const struct { const char * n; unsigned m; } LEAF[] = {
        /* ⭐ r10/r11 now carry DIAG telemetry and are veneer-protected, so a leaf that WRITES them must
           declare it or the veneer drops a live slot. The callee-class gate re-derives these from the .S
           and caught every one -- this table is not hand-maintained trust, it is checked. */
        { "rt_cmp_d", RTCC_C_R10 | RTCC_C_R11 },
    };
    if (x86_rtcc_noclob_on()) for (size_t i = 0; i < sizeof(LEAF) / sizeof(LEAF[0]); i++) if (strcmp(sym, LEAF[i].n) == 0) return LEAF[i].m;
    static const struct { const char * n; unsigned m; } T[] = {
        { "rt_dcap_end_ok_close",       0 }, { "rt_faildescr",              0 },
        { "rt_is_truthy",               0 }, { "rt_proc_value",             0 },
        { "rt_patstk_lazy_init",        0 }, { "rt_gen_spine_resume_enter", 0 },
        { "rt_gen_spine_pass_\u03b3",   0 }, { "rt_gen_spine_pass_\u03c9",  0 },
        { "rt_cap_match_begin", RTCC_C_R10 }, { "rt_cap_pop",      RTCC_C_R10 },
        { "rt_cap_top",         RTCC_C_R10 }, { "rt_match_ctx_restore", RTCC_C_R10 },
    };
    for (size_t i = 0; i < sizeof(T) / sizeof(T[0]); i++) if (strcmp(sym, T[i].n) == 0) return T[i].m;
    return RTCC_C_ALL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
constexpr bool x86_rtcc_streq(const char * a, const char * b) { return *a == *b && (*a == '\0' ? true : x86_rtcc_streq(a + 1, b + 1)); }
static_assert(RTCC_SLOT_RAX * 8 ==  0, "RTCC ABI drift: RTCC_SLOT_RAX no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RCX * 8 ==  8, "RTCC ABI drift: RTCC_SLOT_RCX no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RDX * 8 == 16, "RTCC ABI drift: RTCC_SLOT_RDX no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RSI * 8 == 24, "RTCC ABI drift: RTCC_SLOT_RSI no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RDI * 8 == 32, "RTCC ABI drift: RTCC_SLOT_RDI no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_R8  * 8 == 40, "RTCC ABI drift: RTCC_SLOT_R8 no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin (and keywords.c ANCHOR companion write)");
static_assert(RTCC_SLOT_R9  * 8 == 48, "RTCC ABI drift: RTCC_SLOT_R9 no longer matches the literal offset in x86_rtcc_rl_bin (and the rtcc_init RT_GVA_VA seed) — this is the H2 SIGSEGV class");
static_assert(x86_rtcc_streq(RTCC_GVA_REG, "r9"), "RTCC ABI drift: RTCC_GVA_REG no longer names the register the reload encoders load from slot 6 (mov r9,[rcx+48]) — GVARQ would address a register the veneer never seeds");
static_assert(RTCC_GPR_COUNT == 9 && RTCC_GPR_BYTES == 72, "RTCC ABI drift: GPR tier width no longer matches the 9 slots the encoders write back and reload");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string x86_rtcc_wb_bin(uint64_t block, unsigned m = RTCC_C_ALL) {
    std::string wb;
    wb += (char)0x48; wb += (char)0xB8; wb += u64le(block);
    if (m & RTCC_C_R8)  { wb += (char)0x4C; wb += (char)0x89; wb += (char)0x40; wb += (char)40; }
    if ((m & RTCC_C_R9) && !RTCC_GLOBAL_R9_GVA) { wb += (char)0x4C; wb += (char)0x89; wb += (char)0x48; wb += (char)48; }
    if (m & RTCC_C_R10) { wb += (char)0x4C; wb += (char)0x89; wb += (char)0x50; wb += (char)56; }   /* mov [rax+56], r10 */
    if (m & RTCC_C_R11) { wb += (char)0x4C; wb += (char)0x89; wb += (char)0x58; wb += (char)64; }   /* mov [rax+64], r11 */
    return wb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string x86_rtcc_rl_bin(uint64_t block, unsigned m = RTCC_C_ALL) {
    std::string rl;
    rl += (char)0x48; rl += (char)0xB9; rl += u64le(block);
    if (m & RTCC_C_R8)  { rl += (char)0x4C; rl += (char)0x8B; rl += (char)0x41; rl += (char)40; }
    if (m & RTCC_C_R9)  { rl += (char)0x4C; rl += (char)0x8B; rl += (char)0x49; rl += (char)48; }
    if (m & RTCC_C_R10) { rl += (char)0x4C; rl += (char)0x8B; rl += (char)0x51; rl += (char)56; }   /* mov r10, [rcx+56] */
    if (m & RTCC_C_R11) { rl += (char)0x4C; rl += (char)0x8B; rl += (char)0x59; rl += (char)64; }   /* mov r11, [rcx+64] */
    return rl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string x86_rtcc_wb_text(unsigned m = RTCC_C_ALL) {
    std::string wb;
    if (m & RTCC_C_R8)  wb += x86_rec("mov") + "qword ptr [rip + rtccb+40], r8\n";
    if ((m & RTCC_C_R9) && !RTCC_GLOBAL_R9_GVA) wb += x86_rec("mov") + "qword ptr [rip + rtccb+48], r9\n";
    if (m & RTCC_C_R10) wb += x86_rec("mov") + "qword ptr [rip + rtccb+56], r10\n";
    if (m & RTCC_C_R11) wb += x86_rec("mov") + "qword ptr [rip + rtccb+64], r11\n";
    return wb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string x86_rtcc_rl_text(unsigned m = RTCC_C_ALL) {
    std::string rl;
    if (m & RTCC_C_R8)  rl += x86_rec("mov") + "r8,  qword ptr [rip + rtccb+40]\n";
    if (m & RTCC_C_R9)  rl += x86_rec("mov") + "r9,  qword ptr [rip + rtccb+48]\n";
    if (m & RTCC_C_R10) rl += x86_rec("mov") + "r10, qword ptr [rip + rtccb+56]\n";
    if (m & RTCC_C_R11) rl += x86_rec("mov") + "r11, qword ptr [rip + rtccb+64]\n";
    return rl;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rtcc_call(const char * sym, uint64_t ptr) {
    unsigned m = x86_rtcc_clob(sym);
    if (m == 0) return x86_call_ro(sym, ptr);
    uint64_t block = (uint64_t)(uintptr_t)rtccb;
    if (MEDIUM_BINARY) {
        std::string call_b;
        call_b += (char)0x48; call_b += (char)0xB8; call_b += u64le(ptr);
        call_b += (char)0xFF; call_b += (char)0xD0;
        return x86_align_assert() + x86_Lrec(x86_rtcc_wb_bin(block, m)) + x86_Lrec(call_b) + x86_Lrec(x86_rtcc_rl_bin(block, m));
    }
    return x86_align_assert() + x86_rtcc_wb_text(m) + x86_rec("call") + sym + "@PLT\n" + x86_rtcc_rl_text(m);
}
inline std::string x86_rtcc_call_descr(const char * sym, uint64_t ptr, int slot);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline uint8_t x86_jcc_op(const char * mnem) {
    if (!strcmp(mnem, "je")  || !strcmp(mnem, "jz"))  return 0x84;
    if (!strcmp(mnem, "jne") || !strcmp(mnem, "jnz")) return 0x85;
    if (!strcmp(mnem, "jb")  || !strcmp(mnem, "jc")  || !strcmp(mnem, "jnae")) return 0x82;
    if (!strcmp(mnem, "jae") || !strcmp(mnem, "jnc") || !strcmp(mnem, "jnb"))  return 0x83;
    if (!strcmp(mnem, "jbe") || !strcmp(mnem, "jna"))  return 0x86;
    if (!strcmp(mnem, "ja")  || !strcmp(mnem, "jnbe")) return 0x87;
    if (!strcmp(mnem, "js"))  return 0x88;
    if (!strcmp(mnem, "jns")) return 0x89;
    if (!strcmp(mnem, "jl")  || !strcmp(mnem, "jnge")) return 0x8C;
    if (!strcmp(mnem, "jge") || !strcmp(mnem, "jnl"))  return 0x8D;
    if (!strcmp(mnem, "jle") || !strcmp(mnem, "jng"))  return 0x8E;
    if (!strcmp(mnem, "jg")  || !strcmp(mnem, "jnle")) return 0x8F;
    fprintf(stderr, "[x86] FATAL x86_jcc_op: unknown condition code '%s' (no BINARY opcode; add it)\n", mnem); abort();
}
enum { X86H_DEF = 0, X86H_JMP = 1, X86H_JCC = 2, X86H_DEF_PAIR = 3 };
inline std::string x86_port_hook(int site, int port);
inline int x86_fc_on();
inline std::string x86_fc_jcc_omega(const char * mnem);
inline std::string x86_fc_jcc_gamma(const char * mnem);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jcc(const char * mnem, int port) {
    if (port == X86P_OMEGA && (x86_fc_on() || _.op_wpop > 0)) return x86_fc_jcc_omega(mnem);
    if (port == X86P_GAMMA && ((x86_fc_on() && _.op_fc_base < 0) || _.op_zgpop > 0)) return x86_fc_jcc_gamma(mnem);
    return x86_port_hook(X86H_JCC, port)
         + (MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(port))
                          : x86_rec(mnem) + x86_portname(port) + "\n");
}
extern "C" int rt_zeta_port_mode(void);
extern "C" int rt_zc_frame_live(void);
extern "C" int rt_zeta_storage_get(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_zc_frame() { return rt_zc_frame_live(); }
inline int x86_zstorage() { return rt_zeta_storage_get(); }
extern "C" void *rt_zh_bump_slow(long bytes);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline void *rt_zh_bump_slow_addr() { return (void *)rt_zh_bump_slow; }
inline int x86_port_mode() { return rt_zeta_port_mode(); }
inline int x86_port_cstack() { int m = x86_port_mode(); return m == ZC_PORT_CSTACK || m == ZC_PORT_FORTH; }
inline int x86_fc_on()       { return (x86_port_mode() == ZC_PORT_FORTH || x86_port_mode() == ZC_PORT_HEAP) && _.op_fc_bytes > 0; }
inline int x86_fc_miss(int bump) { static int n = 0; if (bump) n++; return n; }
inline int x86_fc_hit(int off) { int w = _.op_fc_bytes > 0 ? (int)_.op_fc_bytes : (int)_.op_fc_wbytes; int granted = (x86_port_mode() == ZC_PORT_FORTH || x86_port_mode() == ZC_PORT_HEAP) && w > 0 && _.op_fc_base >= 0; int hit = granted && off >= _.op_fc_base && off < _.op_fc_base + w; if (granted && !hit) { int own = _.op_own_ci > 0 && off < (int)_.op_own_ci; int fullcell = _.op_fc_bytes > 0; int defect = own && fullcell; if (defect) x86_fc_miss(1); static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_FC_AUDIT"); on = (e && *e == '1') ? 1 : 0; } if (on) fprintf(stderr, "[FC-%s] granted box falls back to [off %d]: window=[%d,%d) w=%d ci=%ld\n", defect ? "MISS" : (own ? "FLAT-BYDESIGN" : "CROSS"), off, _.op_fc_base, _.op_fc_base + w, w, (long)_.op_own_ci); } return hit; }
inline std::string x86_fc_jcc_omega(const char * mnem);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * x86_jcc_canon(uint8_t op) {
    switch (op) {
        case 0x82: return "jb";  case 0x83: return "jae";
        case 0x84: return "je";  case 0x85: return "jne";
        case 0x86: return "jbe"; case 0x87: return "ja";
        case 0x88: return "js";  case 0x89: return "jns";
        case 0x8C: return "jl";  case 0x8D: return "jge";
        case 0x8E: return "jle"; case 0x8F: return "jg";
    }
    fprintf(stderr, "[x86] FATAL x86_jcc_canon: unknown jcc opcode 0x%02X (x86_jcc_op grew an arm this switch does not spell)\n", (unsigned)op); abort();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * x86_jcc_invert(const char * m) { return x86_jcc_canon((uint8_t)(x86_jcc_op(m) ^ 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_selfload_mode() {
    return (int)ZC_SELFLOAD;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * x86_zr()         { return "rsp"; }
inline int          x86_zr_num()     { return x86_zc_frame() == ZC_FRAME_RSP ? 4 : 5; }
inline int x86_fb_pinned() { return 0; }
static inline int x86_fb_stmt_on() { static int m = -1; if (m < 0) { const char * e = getenv("SCRIP_FB_STMT"); m = (e && *e == '0') ? 0 : 1; } return m; }
inline int x86_fb_data() { return 0; }
inline int x86_frame_off_rsp(int off) { return off + _.op_zdepth; }
inline int x86_rsp_slide_known() { return 1; }
inline int x86_frame_off(int off) { return x86_rsp_slide_known() ? off + _.op_zdepth : -1; }
inline const char * x86_fb()         { return "rsp"; }
inline int          x86_fb_num()     { return x86_fb_data() ? 5 : 4; }
inline const char * x86_fr32_prefix() { return "dword ptr [rsp$ + "; }
inline const char * x86_fr64_prefix() { return "qword ptr [rsp$ + "; }
extern "C" int rt_zeta_mode(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_zeta_mode() { return rt_zeta_mode(); }
inline std::string x86_sub(const char * reg, long imm);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_align_assert_on(void) {
    static int v = -1;
    if (v < 0) { const char * e = getenv("SCRIP_ALIGN_ASSERT"); v = (e && *e && *e != '0') ? 1 : 0; }
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_align_assert() {
    if (!x86_align_assert_on()) return std::string();
    if (MEDIUM_BINARY) return x86_Lrec(x86_b2(0x40, 0xF6) + x86_b2(0xC4, 0x0F) + x86_b2(0x74, 0x02) + x86_b2(0x0F, 0x0B));
    return x86_rec("test") + "spl, 15\n jz 1f\n ud2\n1:\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_port_canary() {
    if (x86_port_mode() != ZC_PORT_INSTRUMENTED) return std::string();
    if (MEDIUM_BINARY) {
        int z = x86_zr_num(), lo = z & 7;
        uint8_t rex = (uint8_t)(0x48 | (z >= 8 ? 0x05 : 0x00)), modrm = (uint8_t)(0xC0 | (lo << 3) | lo);
        return x86_Lrec(x86_b3(rex, 0x85, modrm) + x86_b2(0x75, 0x02) + x86_b2(0x0F, 0x0B));
    }
    return x86_rec("test") + x86_zr() + ", " + x86_zr() + "\n jnz 1f\n ud2\n1:\n";
}
extern "C" void rt_zls_release(void *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_own_floor_store() {
    if (MEDIUM_BINARY)
        return x86_Lrec(x86_b3(0x48, 0x39, 0xC1) + x86_b2(0x73, 0x03) + x86_b3(0x48, 0x89, 0x0F));
    return x86_rec("cmp") + "rcx, rax\n jae 1f\n mov qword ptr [rdi], rcx\n1:\n";
}
inline std::string x86_zeta_free_call();
inline std::string x86_zdp_rbp_omega_at(int port);
inline std::string x86_zdp_rbp_gamma_at(int port);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp(int port) {
    return x86_port_hook(X86H_JMP, port)
         + x86_zdp_rbp_omega_at(port)
         + x86_zdp_rbp_gamma_at(port)
         + (MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(port))
                          : (x86_rec("jmp") + x86_portname(port) + "\n"));
}
inline std::string x86_zdp_probe_at(int port);
inline std::string x86_zdp_rbp_at(int port);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_deflabel(int port) {
    std::string s = MEDIUM_BINARY ? x86_Drec(port) : (std::string(1, '\x01') + (char)('0' + port) + x86_reclbl(x86_portname(port)) + "\n");
    return s + x86_port_hook(X86H_DEF, port) + x86_zdp_probe_at(port) + x86_zdp_rbp_at(port);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_alpha()                    { return x86_deflabel(X86P_ALPHA); }
inline std::string x86_beta()                     { return x86_deflabel(X86P_BETA); }
inline std::string x86_beta_trampoline()          { return _.op_beta_dead ? std::string() : (x86_deflabel(X86P_BETA) + x86_jmp(X86P_OMEGA)); }
inline std::string x86_gamma()                    { return x86_jmp(X86P_GAMMA); }
inline std::string x86_gamma(const char * mnem)   { return x86_jcc(mnem, X86P_GAMMA); }
inline std::string x86_omega()                    { return x86_jmp(X86P_OMEGA); }
inline std::string x86_omega(const char * mnem)   { return x86_jcc(mnem, X86P_OMEGA); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zls2_free() {
    if (_.op_zls2_bytes > 0 && _.op_zls2_ops == 0 && x86_port_mode() == ZC_PORT_ALLOC) return x86_add(x86_zr(), _.op_zls2_bytes);
    return std::string();
}
enum { X86T_TGT0 = 4, X86T_TGT1 = 5 };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_tgt(int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    return MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(t))
                         : (x86_rec("jmp") + nm + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jcc_tgt(const char * mnem, int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    return MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(t))
                         : x86_rec(mnem) + nm + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_call_tgt(int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    return MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE8)) + x86_Jrec(t))
                         : (x86_rec("call") + nm + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_lea_tgt(const char * dst, int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    if (MEDIUM_BINARY) {
        int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x04; code += (char)rex; code += (char)0x8D; code += (char)(0x05 | ((m & 7) << 3));
        return x86_Lrec(code) + x86_Jrec(t);
    }
    return x86_rec("lea") + dst + ", [rip + " + nm + "]\n";
}
#define X86_INTERNAL_BASE 6
#define X86_INTERNAL_MAX  250
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_internal_id(int n) {
    if (n < 0 || n >= X86_INTERNAL_MAX) { fprintf(stderr, "FATAL x86_asm: internal label L(%d) out of range [0,%d) -- the one-byte record id would corrupt or truncate; split the box or raise X86_INTERNAL_MAX\n", n, X86_INTERNAL_MAX); abort(); }
    return X86_INTERNAL_BASE + n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * L(int n) { static char b[8][8]; static int i; i = (i + 1) & 7; snprintf(b[i], 8, "L%d", n); return b[i]; }
inline std::string x86_internal_name(int n) { return std::string(".Lx") + std::to_string(_.x86_uid) + "_" + std::to_string(n); }
inline std::string LS(int n) { return x86_internal_name(n) + "_s"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_id(int n) {
    int id = x86_internal_id(n);
    return MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(id))
                         : (x86_rec("jmp") + x86_internal_name(n) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jcc_id(const char * mnem, int n) {
    int id = x86_internal_id(n);
    return MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(id))
                         : x86_rec(mnem) + x86_internal_name(n) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_deflabel_id(int n) {
    int id = x86_internal_id(n);
    return MEDIUM_BINARY ? x86_Drec(id) : x86_reclbl(x86_internal_name(n)) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_lea_id(const char * dst, int n) {
    int id = x86_internal_id(n);
    if (MEDIUM_BINARY) { int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x04; code += (char)rex; code += (char)0x8D; code += (char)(0x05 | ((m & 7) << 3)); return x86_Lrec(code) + x86_Jrec(id); }
    return x86_rec("lea") + dst + ", [rip + " + x86_internal_name(n) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_reg(const char * r) {
    if (MEDIUM_BINARY) { int m = x86_rnum(r); std::string code; if (m >= 8) code += (char)0x41; code += (char)0xFF; code += (char)(0xE0 | (m & 7)); return x86_Lrec(code); }
    return x86_rec("jmp") + r + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_call_dc(const char * dcname, uint64_t slot) {
    if (MEDIUM_BINARY) { std::string r = x86_movabs_r64("rax", slot); std::string c; c += (char)0xFF; c += (char)0x10; r += x86_Lrec(c); return r; }
    return x86_rec("call") + dcname + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_lblptr(bb_label_t * l, const char * txt) {
    if (MEDIUM_BINARY) { std::string r = x86_Lrec(x86_b1(0xE9)); r += (char)'X'; uint64_t v = (uint64_t)(uintptr_t)l; for (int j = 0; j < 8; j++) r += (char)(unsigned char)(v >> (8 * j)); return r; }
    return x86_rec("jmp") + txt + "\n";
}
inline std::string x86_jmp_fn_body(const char * label, uint64_t fp);
inline std::string x86_jmp_through_fn_cell(const char * label, uint64_t cell);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmpfn(const char * sym, uint64_t fp) {
    if (MEDIUM_BINARY) { std::string r = x86_movabs_r64("rax", fp); std::string c; c += (char)0xFF; c += (char)0xE0; r += x86_Lrec(c); return r; }
    return x86_rec("jmp") + sym + "@PLT\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_mem(const char * base, int disp) {
    if (MEDIUM_BINARY) {
        int b = x86_rnum(base); std::string code; if (b >= 8) code += (char)0x41; code += (char)0xFF; int lo = b & 7; int sib = (lo == 4);
        if (disp == 0 && lo != 5) { code += (char)(0x20 | lo); if (sib) code += (char)0x24; }
        else if (disp >= -128 && disp <= 127) { code += (char)(0x60 | lo); if (sib) code += (char)0x24; code += (char)(disp & 0xFF); }
        else { code += (char)(0xA0 | lo); if (sib) code += (char)0x24; code += u32le((uint32_t)disp); }
        return x86_Lrec(code);
    }
    char b2[96]; if (disp) snprintf(b2, sizeof b2, "qword ptr [%s + %d]\n", base, disp); else snprintf(b2, sizeof b2, "qword ptr [%s]\n", base);
    return x86_rec("jmp") + b2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_fc_jcc_omega(const char * mnem) {
    int id = _.x86_fc_synth--;
    return x86_jcc_id(x86_jcc_invert(mnem), id)
         + x86_jmp(X86P_OMEGA)
         + x86_deflabel_id(id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_fc_jcc_gamma(const char * mnem) {
    int id = _.x86_fc_synth--;
    return x86_jcc_id(x86_jcc_invert(mnem), id)
         + x86_jmp(X86P_GAMMA)
         + x86_deflabel_id(id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_ext_ptr_bytes(const void * p) {
    uint64_t v = (uint64_t)(uintptr_t)p; std::string s; for (int i = 0; i < 8; i++) { s += (char)(unsigned char)(v & 0xFF); v >>= 8; } return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_def_ext(const struct bb_label_t * lbl) {
    if (MEDIUM_BINARY) { std::string r; r += (char)'Y'; r += x86_ext_ptr_bytes(lbl); return r; }
    return x86_reclbl(std::string(lbl && lbl->name ? lbl->name : "?")) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_ext(const struct bb_label_t * lbl) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b1(0xE9)); r += (char)'X'; r += x86_ext_ptr_bytes(lbl); return r; }
    return x86_rec("jmp") + (lbl ? lbl->name : "?") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jcc_ext(const char * mnem, const struct bb_label_t * lbl) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))); r += (char)'X'; r += x86_ext_ptr_bytes(lbl); return r; }
    return x86_rec(mnem) + (lbl ? lbl->name : "?") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_lea_ext(const char * dst, const struct bb_label_t * lbl) {
    if (MEDIUM_BINARY) { int g = x86_rnum(dst); std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8D; c += (char)(0x05 | ((g & 7) << 3)); std::string r = x86_Lrec(c); r += (char)'X'; r += x86_ext_ptr_bytes(lbl); return r; }
    return x86_rec("lea") + dst + ", [rip + " + (lbl && lbl->name ? lbl->name : "?") + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_via_cell(const char * label, uint64_t cell) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0x48; c += (char)0xB8; c += u64le(cell); return x86_Lrec(c) + x86_Lrec(std::string("\x48\x8B\x00", 3)) + x86_Lrec(std::string("\xFF\xE0", 2)); }
    return x86_rec("lea") + "rax, [rip + " + (label ? label : "??") + "]\n" + x86_rec("jmp") + "rax\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_quad_ilbl(int n) {
    int id = x86_internal_id(n);
    if (MEDIUM_BINARY) { std::string r; r += (char)'Q'; r += (char)(unsigned char)id; return r; }
    return std::string(" .quad ") + x86_internal_name(n) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_lea_rip_id(const char * reg, int n) {
    int g = x86_rnum(reg);
    int id = x86_internal_id(n);
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8D; c += (char)(0x05 | ((g & 7) << 3)); return x86_Lrec(c) + x86_Jrec(id); }
    return x86_rec("lea") + reg + ", [rip + " + x86_internal_name(n) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline void x86_begin() { if (!MEDIUM_BINARY) _.x86_uid = g_flat_node_id++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_ro_load_q(const char * reg, int n) {
    if (MEDIUM_BINARY) {
        int m = x86_rnum(reg); uint8_t rex = 0x48; if (m >= 8) rex |= 0x04;
        std::string code; code += (char)rex; code += (char)0x8B; code += (char)((0 << 6) | ((m & 7) << 3) | 5);
        return x86_Lrec(code) + x86_Jrec(X86_INTERNAL_BASE + n);
    }
    return x86_rec("mov") + reg + ", qword ptr [rip + " + x86_internal_name(n) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_ro_seal_q(int n, uint64_t val) {
    if (MEDIUM_BINARY) return x86_Drec(X86_INTERNAL_BASE + n) + x86_Lrec(u64le(val));
    return x86_reclbl(x86_internal_name(n)) + "\n" + x86_rec(".quad") + std::to_string((unsigned long long)val) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_asm_str_escape(const char * s) {
    std::string o;
    for (const char * p = s; p && *p; ++p) {
        char c = *p;
        if (c == '\\' || c == '"') { o += '\\'; o += c; }
        else if (c == '\n')        { o += "\\n"; }
        else if (c == '\t')        { o += "\\t"; }
        else                        o += c;
    }
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_ro_seal_str(int n, const char * lit) {
    const char * s = lit ? lit : "";
    if (MEDIUM_BINARY) return x86_Drec(X86_INTERNAL_BASE + n) + x86_Lrec(u64le((uint64_t)(uintptr_t)s));
    std::string slot = x86_internal_name(n);
    std::string sbuf = slot + "_s";
    return slot + ":\n .quad " + sbuf + "\n" + sbuf + ":\n .string \"" + x86_asm_str_escape(s) + "\"\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_and(const char * reg, long imm) {
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
inline std::string x86_cmp_imm(const char * reg, long imm) {
    int m = x86_rnum(reg);
    std::string code;
    if (x86_is8(reg)) { uint8_t rex = 0x40; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)0x80; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += (char)(uint8_t)imm; }
    else if (imm >= -128 && imm <= 127) { if (m >= 8) code += (char)0x41; code += (char)0x83; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else if (m == 0)               { code += (char)0x3D; code += u32le((uint32_t)imm); }
    else                           { if (m >= 8) code += (char)0x41; code += (char)0x81; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("cmp") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_modrm(int regfield, int off) {
    std::string s; int rf = regfield & 7;
    int mod = (off == 0) ? 0 : (off >= -128 && off <= 127) ? 1 : 2;
    s += (char)((mod << 6) | (rf << 3) | 4); s += (char)0x24;
    if (mod == 1) s += (char)(int8_t)off;
    else if (mod == 2) s += u32le((uint32_t)off);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_modrm(int regfield, int off) {
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
inline std::string x86_frame_rex(int w, int regfield) {
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (regfield >= 8) rex |= 0x04; if (x86_fb_num() >= 8) rex |= 0x01;
    std::string s; if (rex != 0x40) s += (char)rex;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_text_mem(int off) { return std::string("[") + x86_fb() + " + " + std::to_string(x86_frame_off(off)) + "]"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_lea(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x8D; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("lea") + reg + ", " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movsxd_frame(const char * dst64, int off) {
    int g = x86_rnum(dst64);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x63; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("movsxd") + dst64 + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_mov_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, 0); c += (char)0xC7; c += x86_frame_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return x86_rec("mov") + "dword ptr " + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_store(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x89; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + "dword ptr " + x86_frame_text_mem(off) + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_load(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x8B; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_add_imm(int off, long imm) {
    if (MEDIUM_BINARY) {
        std::string c; c += x86_frame_rex(0, 0);
        if (imm >= -128 && imm <= 127) { c += (char)0x83; c += x86_frame_modrm(0, off); c += (char)(uint8_t)(int8_t)imm; }
        else                           { c += (char)0x81; c += x86_frame_modrm(0, off); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return x86_rec("add") + "dword ptr " + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_add_to_reg(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x03; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("add") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_sub_from_reg(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x2B; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("sub") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * RDQ(const char * base, int off);
inline const char * RDD(const char * base, int off);
inline int icn_genframe2();
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int icn_gen_zeta_ft() {   /* ⭐⭐ N-2 ITEM 1 (hq_P s276): RE-HOME GENERATOR ζ FROM THE RSP SPINE TO THE RBP ACTIVATION FRAME. Returns the α carve size (frame_total) when this graph is an ARMED Icon suspend-generator, else 0. ⭐ WHY A PURE REBASE IS SOUND AND EXACT: the N-2 α carve is `push rbp; mov rbp,rsp; sub rsp,frame_total` and rsp does NOT move again between α and γ (measured on the four-line witness: zero pushes in the body), so `[rsp + off]` and `[rbp + off - frame_total]` are THE SAME ADDRESS. This change therefore alters addressing ONLY -- it must be behaviourally invisible, which is exactly what makes it gradeable: the D2 witness set must not move and SNOBOL4 must stay 365/365. ⛔ IT IS NOT THE CURE AND MUST NOT BE READ AS ONE: the frame still lives on the shared stack below the point the caller resumes to, so bb_call_proc_staged.cpp:733's `lea rsp,[rax+32]` still discards it. This is the PREREQUISITE for item 2. ⛔ ITEM 2 IS NOT WHAT THIS SENTENCE USED TO SAY: the phrase "re-point rbp at the heap island e->frame" is VOID -- Lon deleted the workspace island (rt_icn_gen_frame_alloc/_retire removed at SCRIP 915bdaa4) and RULES.md:72 THE STORAGE ANSWER rules that a suspend-surviving frame carves in the ENCLOSING graph's RBP activation frame, never the heap. Item 2 is therefore a CALLER-SIDE change: the host reserves the callee's compile-time-known frame bytes inside its own carve. That size is computable from GRAPH FIELDS ALONE -- ft == ((48 + jcon_value_region + 15) & ~15) + (nparams + nlocals) * 16 -- measured 1308/1308 in both arms and gated by scripts/test_icn_n2_ft_formula.sh, so the host never needs the callee to have been emitted first. ⛔⭐ SHARED-NODE VERDICT SCOPE, THE s272 LESSON (47 Icon programs lost to a language-blind widening): this deliberately does NOT touch the SNOBOL4 pattern re-homing (xop_frame_member / frame_slot_scan / op_xf_off), which the rung's own NEXT proposed extending. Keying a grant on a SNOBOL4-pattern predicate is precisely the shape that regressed. Instead the rebase is keyed on the CONSUMING ζ REGIME -- icn_genframe2() && flat_gen -- two conditions that no SNOBOL4 or Prolog graph can satisfy, and the first of which is DEFAULT OFF, so an unarmed build is byte-identical by construction. */
    if (!icn_genframe2() || !_.flat_gen) return 0;
    int ft = _.flat_frame_bytes + (g_emit_cfg ? (g_emit_cfg->nparams + g_emit_cfg->nlocals) * 16 : 0);
    return ft > 0 ? ft : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_patzeta_frame_reserve(const char * name, int * bytes);   /* N-2 step 2b: defined in src/ir/zeta_storage.c over the EXISTING pz[] registry. Branch on the RETURN (1 = known, 0 = forward reference), never on the value. */
extern "C" int rt_proc_is_registered(const char * name);
extern "C" int rt_proc_is_generator(const char * name);
inline int icn_gen_host_reserve(const char * prefix) {   /* ⭐⭐ N-2 ITEM 2 STEP 2b (hq_P s278): THE HOST RESERVATION -- bytes THIS graph must add to its own α carve to hold the activation frames of the suspend-generators it directly calls, or 0. ⛔⭐ IT LIVES IN THIS HEADER FOR THE SAME REASON icn_genframe2() DOES, AND I LEARNED IT THE HARD WAY: the α carve is emitted from the DRIVER (emit.cpp) while the γ/ω releases are emitted from a template into libscrip_rt.so -- TWO LINK UNITS. Defining it in emit.cpp linked clean for the driver and died `undefined reference to icn_gen_host_reserve(char const*)` when the RT tried to call it. ⛔⛔ THE CARVE AND THE RELEASE MUST DERIVE THIS FROM ONE FUNCTION: they were two copies of one formula and they DRIFTED -- measured on a proc host, armed carve 240 / release 144, with `jmp qword ptr [rsp]` then reading a wrong return address. Armed-only, so it never shipped broken, but it would have surfaced when items 3-4 armed the path and read as THEIR defect. ⭐ Pure function of g_emit_cfg + the gate, so both sides may call it and nothing is stored -- no new global. ⛔ A FORWARD REFERENCE RESERVES NOTHING AND SAYS SO: step 1 measured that a host which is itself a proc calling a generator declared LATER is not yet registered; reading that as 0 would hand step 3 a carve silently too small, the class ceo refused worst-case reservation over. `prefix` non-NULL announces that refusal; the epilogues pass NULL so one graph reports once. ⛔ SUM, NOT MAX: two generators live at once each need their own region. */
    if (!icn_genframe2() || !g_emit_cfg) return 0;
    int total = 0, forward = 0;
    for (int i = 0; i < g_emit_cfg->n; i++) {
        IR_t * hn = g_emit_cfg->all[i]; if (!hn) continue;
        if (!ir_is_call_kind(hn->op) && hn->op != IR_CALL && hn->op != IR_PROC_GEN) continue;
        { const char * cn = IR_LIT(hn).sval;
          if (!cn || !cn[0] || !rt_proc_is_registered(cn) || !rt_proc_is_generator(cn)) continue;
          { int fb = -1; if (emit_patzeta_frame_reserve(cn, &fb) && fb > 0) total += (((fb + 15) & ~15) + 48); else forward++; } }   /* N-2 STEP 3 (ceo s283): +48 = the region HEADER above the callee's ft bytes -- [H+0]=saved caller rbp [H+8]=gamma [H+16]=omega [H+24]=ANCHOR (caller pre-pad rsp0) [H+32]=resume label [H+40]=spare/16B-align. The slice is [R, R+ft+48) with H=R+ft; the generator runs with rbp=H so every FRQ/ZOPQ spelling from item 1 lands inside the region unchanged. ⛔ ALL THREE SCANS IN THIS HEADER (reserve/offset/selftest) MUST CARRY THE SAME PER-CALLEE ARITHMETIC -- two copies of one formula drift, every time (the 240-vs-144 lesson above). */
    }
    if (forward) { if (prefix) fprintf(stderr, "[GENHOST] \u26d4 host=%s RESERVES NOTHING: %d generator callee(s) not yet registered (forward reference). A partial carve would be silently too small.\n", prefix, forward); return 0; }
    return total;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int icn_gen_host_reserved(void) {   /* ⛔⭐ N-2 ITEM 3 (hq_P s282, UPDATED 2026-08-29): DID **THIS** HOST ACTUALLY RESERVE? -- the question icn_gen_host_reserve_offset() was answering by assumption. icn_gen_host_reserve() is now called from BOTH non-zframe arms of the three-arm prologue chain: `if (zframe_graph)` :2829 (never reserves) / `else if (icn_genframe2() && flat_gen)` :2832 (N-2 item 3 flat_gen half, 2026-08-29 -- mirrors the flat_lcl_proc arm's own reservation computation) / `else if (flat_lcl_proc)` :2845 (step 2b). A graph can be flat_gen AND flat_lcl_proc at once (emit.cpp:3535-3536 sets flat_lcl_proc under `_gfr && icn_cells_graph`), in which case the flat_gen arm wins because it is tested first -- so `flat_lcl_proc` ALONE IS NOT THE PREDICATE, and neither arm may be assumed silent without checking emit.cpp directly. ⛔⛔ THIS MIRRORS AN else-if CHAIN IN ANOTHER TRANSLATION UNIT AND THERE IS NO COMPILER CHECK THAT IT STILL DOES: if a fifth arm is added, or either reserve call moves, THIS PREDICATE MUST MOVE WITH IT -- caught ONE session late here: extending the flat_gen arm's carve without updating this function was measured live via SCRIP_N2_OFFSET_SELFTEST=1 (host=proc_outer on nested.icn: expect_off=0, got_off=-1, MISMATCH) before it shipped. test_icn_n2_host_reserved_agrees.sh is the canary -- it compares this predicate against the carve actually emitted; its witness assertions were updated in the same commit to expect BOTH host kinds to answer now, not just flat_lcl_proc. */
    if (g_emit.zframe_graph) return 0;
    if (icn_genframe2() && g_emit.flat_gen) return 1;
    return g_emit.flat_lcl_proc ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int icn_gen_host_reserve_offset(const char * prefix, const IR_t * call_node, int * base_out = 0) {   /* N-2 ITEM 3 PREP (seat01, 2026-08-28): PER-CALLEE OFFSET WITHIN icn_gen_host_reserve()'s summed region -- that function answers "how many bytes total", never "which slice is THIS call site's". \u26d4 KEYED ON NODE IDENTITY, NEVER ON CALLEE NAME: the same generator proc called twice in one host is two independent live activations under SUM-not-max (see icn_gen_host_reserve()'s own comment), so a name-keyed lookup would hand both call sites the SAME offset. \u2b50 Walks g_emit_cfg->all[] with the IDENTICAL predicate icn_gen_host_reserve() uses, in the SAME order -- two independently-written scans over the same array is exactly the shape that drifted once already (that function's own comment names the carve/release incident); MEASURE the two agree before trusting either in a new consumer, never assume it from matching source. Landed INERT: zero call sites reference this yet. Returns -1 (never a guessed offset) if call_node is not a registered-generator call in this graph, or if a forward-referenced callee sits before it in scan order -- a partial answer is exactly the silently-too-small class the forward-reference guard already exists to refuse.
       \u2b50\u2b50 EXTENDED (seat01, 2026-08-28, second pass, per hq_P's LEDGER-s281 answer): also exposes the host's OWN carve base via `base_out`, so carve, release, offset AND base all derive from ONE function instead of a second copy of `flat_frame_bytes + (np+nl)*16` living in a template -- exactly the shape that drifted 240-vs-144 once already (see icn_gen_host_reserve()'s own comment). hq_P's retraction is binding here: emit.cpp's `host_frame_base` local is computed and explicitly discarded (`(void)host_frame_base`), never recorded or exported, so a caller in a DIFFERENT translation unit re-deriving it independently would be relying on `g_emit.flat_frame_bytes`/`g_emit_cfg->nparams`/`nlocals` still holding their host-\u03b1 values at a later, unverified point -- the same unverifiable-by-construction premise that was wrong at step 1b (`ft` "obviously" 0, measured 96). Centralizing here removes that premise: there is exactly one place this arithmetic is written down for step 3 to consume. */
    if (base_out) *base_out = -1;
    if (!icn_genframe2() || !g_emit_cfg || !call_node) return -1;
    if (!icn_gen_host_reserved()) return -1;   /* ⛔⛔⭐ N-2 ITEM 3 (hq_P s282) -- THE FUNCTION WAS VIOLATING ITS OWN DOCUMENTED CONTRACT, AND MEASURED, NOT REASONED: on suspend_nested the inner() call site from flat_gen host outer() returned `off=0 base=128`, and on a two-caller witness the SAME generator returned `off=0 base=128` from its flat_gen host and `off=128 base=240` from its flat_lcl_proc host. ⛔ THE SCAN ABOVE IS HOST-KIND-BLIND: it walks g_emit_cfg->all[] and hands back an offset into a region THIS host never carved. Zero is the most dangerous possible answer here -- it is indistinguishable from a correct first-slot answer, so a step-3 consumer would carve at base+0 of a region that does not exist and corrupt the host frame, surfacing three layers away. That is the exact plausible-zero class this rung has already been bitten by twice (step 1 `hosts=0`, step 1b `ft` "obviously" 0 and measured 96). ⛔ THE RECORD SAID OTHERWISE: LEDGER-seat01 states this "correctly returns -1 ... because nothing was ever reserved for it". It does not, and did not; the refusal is added HERE. ⭐ INERT BY CONSTRUCTION -- this function has no emission consumer yet (the two references are a getenv diagnostic and the selftest), so no emitted byte can move; proven by .s byte-identity across all four frontends rather than argued. */
    if (base_out) *base_out = g_emit.flat_frame_bytes + ((g_emit_cfg->nparams + g_emit_cfg->nlocals) * 16);
    int off = 0;
    for (int i = 0; i < g_emit_cfg->n; i++) {
        IR_t * hn = g_emit_cfg->all[i]; if (!hn) continue;
        if (!ir_is_call_kind(hn->op) && hn->op != IR_CALL && hn->op != IR_PROC_GEN) continue;
        { const char * cn = IR_LIT(hn).sval;
          if (!cn || !cn[0] || !rt_proc_is_registered(cn) || !rt_proc_is_generator(cn)) continue;
          if (hn == call_node) return off;
          { int fb = -1; if (emit_patzeta_frame_reserve(cn, &fb) && fb > 0) off += (((fb + 15) & ~15) + 48);   /* N-2 step 3: ft + 48-byte header per slice -- MUST match icn_gen_host_reserve()'s arithmetic above */
            else { if (prefix) fprintf(stderr, "[GENHOST-OFFSET] \u26d4 host=%s a forward-referenced generator callee sits BEFORE the requested call site -- its offset cannot be trusted either.\n", prefix); return -1; } }
        }
    }
    return -1;   /* call_node was never seen as a registered-generator call in this graph's own scan */
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline void icn_gen_host_reserve_selftest(const char * prefix) {   /* N-2 ITEM 3 PREP (seat01, 2026-08-28): PROVES icn_gen_host_reserve_offset() against icn_gen_host_reserve()'s own total AND against an INDEPENDENTLY-accumulated local expectation -- a check that only re-derives the function under test is not a proof (RULES.md TWO-PART PROOF). getenv-gated (SCRIP_N2_OFFSET_SELFTEST), stderr only, zero emission effect -- inert by construction, same as N-2 step 1's own diagnostic. */
    if (!icn_genframe2() || !g_emit_cfg) return;
    int total = icn_gen_host_reserve(0);
    int expect = 0, calls = 0, mismatches = 0;
    for (int i = 0; i < g_emit_cfg->n; i++) {
        IR_t * hn = g_emit_cfg->all[i]; if (!hn) continue;
        if (!ir_is_call_kind(hn->op) && hn->op != IR_CALL && hn->op != IR_PROC_GEN) continue;
        const char * cn = IR_LIT(hn).sval;
        if (!cn || !cn[0] || !rt_proc_is_registered(cn) || !rt_proc_is_generator(cn)) continue;
        int got = icn_gen_host_reserve_offset(0, hn);
        calls++;
        if (got != expect) { mismatches++; fprintf(stderr, "[GENHOST-SELFTEST] ⛔ host=%s call#%d name=%s expect_off=%d got_off=%d MISMATCH\n", prefix ? prefix : "?", calls, cn, expect, got); }
        { int fb = -1; if (emit_patzeta_frame_reserve(cn, &fb) && fb > 0) expect += (((fb + 15) & ~15) + 48); }   /* N-2 step 3: ft + 48-byte header -- the independent accumulation must mirror the reserve arithmetic or the selftest proves nothing */
    }
    if (calls > 0) fprintf(stderr, "[GENHOST-SELFTEST] host=%s calls=%d total=%d expect_sum=%d %s mismatches=%d\n", prefix ? prefix : "?", calls, total, expect, (expect == total) ? "AGREE" : "⛔ DISAGREE", mismatches);
}
inline int x86_zop_regime(int off) { if (x86_zstorage() == ZC_STORAGE_FRAME_R12) return 1; if (x86_fc_hit(off)) return 2; return x86_fb_data() ? 3 : 4; }
inline void x86_zop_note(int r) { if (r < 1 || r > 5) return; _.zop_seen |= (1 << r); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * x86_zop(int off, int q, int bump) {   /* ⭐ THE FR (FRAME) ζ FAMILY. ⛔ MEASURED hq_P s276, AND IT CORRECTS THIS RUNG'S ORDERED WORK: generator ζ is split across TWO addressing families that are INDISTINGUISHABLE in the emitted .s (both print `qword ptr [rsp + N]`), which is why the s275 .s-grep read "9 of 9 ζ refs are rsp-relative" as ONE homogeneous problem. It is two. This function is the FR family (FRQ/FR, XK_FR*) and it had NO rbp arm anywhere; ZRES/ZOPQ are the SPINE family (rsp#, XK_RSP*) and already had one via op_xf_off. ⛔⭐ THE HALF THAT HOLDS THE YIELDED VALUE IS THIS ONE: on the four-line witness the literal's result descriptor lands at [rsp+16]/[rsp+24] through bb_lit_scalar.cpp:19's FRQ(_.op_off + w) arm -- proven by elimination, since ZRES's base is 0 and could not produce 16 -- so re-homing ONLY ZOPQ/ZRES, as this rung's NEXT instructed, would have re-homed the half that does NOT carry the value, and split the frame across two bases for item 2 to trip over. */
    static char b[16][48]; static int i; i = (i + 1) & 15; int r = x86_zop_regime(off); x86_zop_note(r); int eff, spine;
    if (r == 2) { if (getenv("SCRIP_ZOP_DIAG")) fprintf(stderr, "[ZOP] off=%d op_fc_base=%d bump=%d computed=%d\n", off, _.op_fc_base, bump, off - _.op_fc_base + bump); eff = off - _.op_fc_base + bump; spine = 1; }
    else if (bump && !x86_fb_data() && !_.op_stmt_dyn) { eff = x86_frame_off(off) + bump; spine = 1; }
    else { eff = off + ((x86_fb_data() || _.op_stmt_dyn) ? 0 : bump); spine = 0; }
    { int ft = icn_gen_zeta_ft(); if (ft > 0) return q ? RDQ("rbp", eff - ft) : RDD("rbp", eff - ft); }
    if (spine) snprintf(b[i], 48, "%s ptr [rsp# + %d]", q ? "qword" : "dword", eff);
    else snprintf(b[i], 48, "%s%d]", q ? x86_fr64_prefix() : x86_fr32_prefix(), eff);
    return b[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * x86_ztos(int off, int q) {
    static char b[16][48]; static int i; i = (i + 1) & 15; x86_zop_note(5);
    snprintf(b[i], 48, "%s ptr [rsp + %d]", q ? "qword" : "dword", off + _.op_zdepth);
    return b[i];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * ZTOS(int off)  { return x86_ztos(off, 1); }
inline const char * ZTOSD(int off) { return x86_ztos(off, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * x86_zref(int off, int q) {   /* ⭐ THE SPINE ζ FAMILY (rsp#, XK_RSP*) -- ZRES/ZRESD/ZOPQ/ZOPD/ZLOC all bottom out here when op_xf_off/op_zread_xf say -1, which for an Icon generator graph is ALWAYS (xop_frame_member is gated on sn4_pt_opframe(), a SNOBOL4-pattern predicate no flat_gen graph enters). Rebasing HERE rather than by granting generators membership in the SNOBOL4 slot allocator is the whole point: one leaf function, no frame_slot_scan, no MATCH_BEGIN anchoring, and zero blast radius into pattern machinery. See icn_gen_zeta_ft() for why the rebase is exact. */
    static char b[16][48]; static int i; i = (i + 1) & 15;
    { int ft = icn_gen_zeta_ft(); if (ft > 0) return q ? RDQ("rbp", off - ft) : RDD("rbp", off - ft); }
    snprintf(b[i], 48, "%s ptr [rsp# + %d]", q ? "qword" : "dword", off);
    return b[i];
}
inline const char * RDQ(const char * base, int off);
inline const char * RDD(const char * base, int off);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * ZRES(int w)        { return _.op_xf_off != -1 ? RDQ("rbp", _.op_xf_off + w) : x86_zref(w, 1); }
inline const char * ZRESD(int w)       { return _.op_xf_off != -1 ? RDD("rbp", _.op_xf_off + w) : x86_zref(w, 0); }
inline const char * ZRES_SPINE(int w)  { return x86_zref(w, 1); }
inline const char * ZLOC(int o)        { return x86_zref(16 + o, 1); }
inline const char * ZLOCD(int o)       { return x86_zref(16 + o, 0); }
inline const char * ZLOC_B(int o)      { return x86_zref(_.op_ztail + 16 + o, 1); }
inline const char * ZOPQ(int k, int w) { return _.op_zread_xf[k] != -1 ? RDQ("rbp", _.op_zread_xf[k] + w) : x86_zref(_.op_zread[k] + w, 1); }
inline const char * ZOPD(int k, int w) { return _.op_zread_xf[k] != -1 ? RDD("rbp", _.op_zread_xf[k] + w) : x86_zref(_.op_zread[k] + w, 0); }
inline const char * FR(int off)            { return x86_zop(off, 0, 0); }
inline const char * PAIR(int idx) { static char b[8][16]; static int i; i = (i + 1) & 7; snprintf(b[i], 16, "P%d", idx); return b[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_load64(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x8B; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + reg + ", qword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_store64(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x89; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + "qword ptr " + x86_frame_text_mem(off) + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_frame64(int off) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, 0); c += (char)0xFF; c += x86_frame_modrm(4, off); return x86_Lrec(c); }
    return x86_rec("jmp") + "qword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_mov_imm64(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, 0); c += (char)0xC7; c += x86_frame_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return x86_rec("mov") + "qword ptr " + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * FRQ(int off)           { return x86_zop(off, 1, 0); }
inline const char * FRQB(int off, int bump) { return x86_zop(off, 1, bump); }
inline const char * ROQ(int n)   { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rip + %d]", n); return b[i]; }
inline const char * RDQ(const char * base, int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [%s + %d]", base, off); return b[i]; }
inline const char * ABSQ(unsigned long va) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [%lu]", va); return b[i]; }
inline const char * GVARQ(int k, int w) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [" RTCC_GVA_REG " + %d]", k * 16 + w); return b[i]; }
extern "C" const char * gva_name(int k);
extern "C" const char * bb_kind_name(int op);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * ZOPAN() { if (_.op_a_node_kind < 0) return ""; static char b[8][48]; static int i; i = (i + 1) & 7; const char * n = bb_kind_name(_.op_a_node_kind); snprintf(b[i], 48, "%s", n ? n : ""); return b[i]; }
inline const char * ZRESN() { return "result"; }
inline const char * ZOPN(int k) { if (k < 0 || k >= 6) return ""; int kk = _.op_zkind[k]; if (kk < 0 && k == 0) kk = _.op_a_node_kind; if (kk < 0) return ""; static char b[8][48]; static int i; i = (i + 1) & 7; const char * n = bb_kind_name(kk); snprintf(b[i], 48, "%s", n ? n : ""); return b[i]; }
inline const char * HKN(int k) { static const char * n[6] = { "old____", "outer_Σ", "outer_δ", "outer_Δ", "cap_gen", "zls2_mark" }; return (k >= 0 && k < 6) ? n[k] : ""; }
inline const char * RDD(const char * base, int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "dword ptr [%s + %d]", base, off); return b[i]; }
inline const char * XSAQ(int d) { return _.op_zread_xf[0] != -1 ? RDQ("rbp", _.op_zread_xf[0] + d) : FRQ(_.op_sa + d); }
inline const char * XSAD(int d) { return _.op_zread_xf[0] != -1 ? RDD("rbp", _.op_zread_xf[0] + d) : FR(_.op_sa + d); }
inline const char * zone_ref(int rbp_off, int spine_base, int d, int w) { return (rbp_off != -1) ? ((w == 8) ? RDQ("rbp", rbp_off + d) : RDD("rbp", rbp_off + d)) : ((w == 8) ? FRQ(spine_base + d) : FR(spine_base + d)); }
inline int LFC_ON() { return _.op_leaf_frame_off != -1; }
extern "C" int zzone_tier_of_cur(void);
extern "C" int zzone_off_of_cur(void);
extern "C" int zzone_off_cur_for(int customer);
extern "C" void zzone_disagree(int customer, int staged, int planned);
enum { ZSP_SCRATCH = 0, ZSP_RAW = 1 };
inline const char * ZREFS(int reg_off, int d, int w, int customer, int spine);
enum { ZC_LEAF = 0, ZC_ARBNO = 1, ZC_CAPTURE = 2, ZC_FENCE = 3, ZC_CHOICE = 4 };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int zzone_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZONE"); v = (e && *e == '1') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * ZREFC(int reg_off, int d, int w, int customer) {
    return ZREFS(reg_off, d, w, customer, ZSP_SCRATCH);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * ZREFS(int reg_off, int d, int w, int customer, int spine) {
    int off = reg_off, tier = 2;
    if (zzone_on()) {
        tier = zzone_tier_of_cur();
        int planned = zzone_off_cur_for(customer);
        zzone_disagree(customer, off, planned);
        if (planned != -1) off = planned;
    }
    (void)tier;
    if (off != -1) return (w == 8) ? RDQ("rbp", off + d) : RDD("rbp", off + d);
    if (spine == ZSP_RAW) return (w == 8) ? RDQ("rsp", d) : RDD("rsp", d);
    return (w == 8) ? FRQ(_.x86_scratch_off + d) : FR(_.x86_scratch_off + d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * ZREF(int reg_off, int d, int w) { return ZREFC(reg_off, d, w, ZC_LEAF); }
inline const char * LFC(int d)  { return ZREF(_.op_leaf_frame_off, d, 4); }
inline const char * LFCQ(int d) { return ZREF(_.op_leaf_frame_off, d, 8); }
inline const char * AFC(int d)  { return ZREFC(_.op_arbno_frame_off, d, 4, ZC_ARBNO); }
inline const char * AFCQ(int d) { return ZREFC(_.op_arbno_frame_off, d, 8, ZC_ARBNO); }
inline const char * CFC(int d)  { return ZREFC(_.op_cap_frame_off, d, 4, ZC_CAPTURE); }
inline const char * CFCQ(int d) { return ZREFC(_.op_cap_frame_off, d, 8, ZC_CAPTURE); }
inline const char * FFC(int d)  { return ZREFC(_.op_fence_frame_off, d, 4, ZC_FENCE); }
inline const char * FFCQ(int d) { return ZREFC(_.op_fence_frame_off, d, 8, ZC_FENCE); }
inline const char * CROQ(int cro, int d) { return ZREFS(cro ? cro : -1, d, 8, ZC_CHOICE, ZSP_RAW); }
inline const char * CROD(int cro, int d) { return ZREFS(cro ? cro : -1, d, 4, ZC_CHOICE, ZSP_RAW); }
inline const char * LIDX(long k)  { static char b[8][24]; static int i; i = (i + 1) & 7; if (k) snprintf(b[i], 24, "[r13+rcx+%ld]", k); else snprintf(b[i], 24, "[r13+rcx]"); return b[i]; }
inline long LITQ(long k) { uint64_t w; memcpy(&w, _.op_sval + k, 8); return (long) w; }
inline void x86_rd32_modrm(std::string & c, int g, int b) { c += (char)(0x80 | ((g & 7) << 3) | (b & 7)); if ((b & 7) == 4) c += (char)0x24; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_load64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8B; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("mov") + dst + ", qword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_store64(const char * base, int disp, const char * src) {
    int g = x86_rnum(src), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x89; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("mov") + "qword ptr [" + base + " + " + std::to_string(disp) + "], " + src + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_cmp_imm(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (b >= 8) rex |= 0x01; c += (char)rex;
        if (imm >= -128 && imm <= 127) { c += (char)0x83; x86_rd32_modrm(c, 7, b); c += u32le((uint32_t)disp); c += (char)(uint8_t)(int8_t)imm; }
        else                           { c += (char)0x81; x86_rd32_modrm(c, 7, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return x86_rec("cmp") + "qword ptr [" + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_inc_r(const char * reg) {
    int m = x86_rnum(reg); int w64 = !(reg && (reg[0] == 'e' || (reg[0] && reg[strlen(reg) - 1] == 'd')));
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = (uint8_t)((w64 ? 0x48 : 0x40) | (m >= 8 ? 0x01 : 0)); if (rex != 0x40) c += (char)rex; c += (char)0xFF; c += (char)(0xC0 | (m & 7)); return x86_Lrec(c);
    }
    return x86_rec("inc") + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_abs_disp32_load64(const char * dst, long va) {
    int g = x86_rnum(dst);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8B; c += (char)(0x04 | ((g & 7) << 3)); c += (char)0x25;
        c += u32le((uint32_t)va); return x86_Lrec(c);
    }
    return x86_rec("mov") + dst + ", qword ptr [" + std::to_string(va) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_abs_disp32_store64(long va, const char * src) {
    int g = x86_rnum(src);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x89; c += (char)(0x04 | ((g & 7) << 3)); c += (char)0x25;
        c += u32le((uint32_t)va); return x86_Lrec(c);
    }
    return x86_rec("mov") + "qword ptr [" + std::to_string(va) + "], " + src + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_abs_disp32_addsub_imm8(int is_sub, long va, long imm) {
    if (imm < -128 || imm > 127) { fprintf(stderr, "FATAL x86_abs_disp32_addsub_imm8: imm %ld outside imm8 (REG-2 shape is imm8-only; widen the encoder before widening a client)\n", imm); abort(); }
    if (MEDIUM_BINARY) {
        std::string c; c += (char)0x48; c += (char)0x83; c += (char)(is_sub ? 0x2C : 0x04); c += (char)0x25;
        c += u32le((uint32_t)va); c += (char)(uint8_t)(int8_t)imm; return x86_Lrec(c);
    }
    return std::string(is_sub ? " sub" : " add") + " qword ptr [" + std::to_string(va) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_cmp_reg_abs64(const char * reg, long va) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x3B; c += (char)(0x04 | ((g & 7) << 3)); c += (char)0x25;
        c += u32le((uint32_t)va); return x86_Lrec(c);
    }
    return x86_rec("cmp") + reg + ", qword ptr [" + std::to_string(va) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_load32(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0x8B; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("mov") + dst + ", dword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_add32(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0x03; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("add") + dst + ", dword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_store32(const char * base, int disp, const char * src) {
    int g = x86_rnum(src), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0x89; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("mov") + "dword ptr [" + base + " + " + std::to_string(disp) + "], " + src + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_store_imm64(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0xC7; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm);
        return x86_Lrec(c);
    }
    return x86_rec("mov") + "qword ptr [" + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_store_imm32(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0xC7; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm);
        return x86_Lrec(c);
    }
    return x86_rec("mov") + "dword ptr [" + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_add_imm32(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex;
        if (imm >= -128 && imm <= 127) { c += (char)0x83; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += (char)(uint8_t)(int8_t)imm; }
        else                           { c += (char)0x81; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return x86_rec("add") + "dword ptr [" + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_lea64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8D; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return x86_rec("lea") + dst + ", [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_store64(int off, const char * reg) {
    int g = x86_rnum(reg); uint8_t rex = 0x48; if (g >= 8) rex |= 0x04;
    if (MEDIUM_BINARY) { std::string c; c += (char)rex; c += (char)0x89; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + "qword ptr [rsp + " + std::to_string(off) + "], " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_store32_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0xC7; c += x86_rsp_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return x86_rec("mov") + "dword ptr [rsp + " + std::to_string(off) + "], " + std::to_string((uint32_t)imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_load64(const char * reg, int off) {
    int g = x86_rnum(reg); uint8_t rex = 0x48; if (g >= 8) rex |= 0x04;
    if (MEDIUM_BINARY) { std::string c; c += (char)rex; c += (char)0x8B; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + reg + ", qword ptr [rsp + " + std::to_string(off) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_store64_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0x48; c += (char)0xC7; c += x86_rsp_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return x86_rec("mov") + "qword ptr [rsp + " + std::to_string(off) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_store32(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x89; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + "dword ptr [rsp + " + std::to_string(off) + "], " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_load32(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x8B; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("mov") + reg + ", dword ptr [rsp + " + std::to_string(off) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_add_imm32(int off, long imm) {
    if (MEDIUM_BINARY) {
        std::string c;
        if (imm >= -128 && imm <= 127) { c += (char)0x83; c += x86_rsp_modrm(0, off); c += (char)(uint8_t)(int8_t)imm; }
        else                           { c += (char)0x81; c += x86_rsp_modrm(0, off); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return x86_rec("add") + "dword ptr [rsp + " + std::to_string(off) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_add_to_reg32(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x03; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("add") + reg + ", dword ptr [rsp + " + std::to_string(off) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_sub_from_reg32(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x2B; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return x86_rec("sub") + reg + ", dword ptr [rsp + " + std::to_string(off) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * RSP(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp + %d]", off); return b[i]; }
inline const char * F64(double d) { static char b[4][32]; static int i; i = (i + 1) & 3; uint64_t bits; memcpy(&bits, &d, 8); snprintf(b[i], 32, "f64:%llu", (unsigned long long)bits); return b[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_cmp_imm64(const char * reg, long imm) {
    int m = x86_rnum(reg);
    uint8_t rex = 0x48; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex;
    if (imm >= -128 && imm <= 127) { code += (char)0x83; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else                           { code += (char)0x81; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("cmp") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_load_indexed8(const char * dst, const char * base, const char * idx) {
    int g = x86_rnum(dst), b = x86_rnum(base), x = x86_rnum(idx);
    uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (x >= 8) rex |= 0x02; if (b >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0x8B;
    code += (char)(0x00 | ((g & 7) << 3) | 0x04);
    code += (char)((3 << 6) | ((x & 7) << 3) | (b & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("mov") + dst + ", [" + base + " + " + idx + "*8]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_inc64(int off) {
    std::string code; code += x86_frame_rex(1, 0); code += (char)0xFF; code += x86_frame_modrm(0, off);
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("inc") + "qword ptr " + x86_frame_text_mem(off) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_load_mem64(const char * dst, const char * basebr) {
    char rb[8]; int k = 0; for (const char * p = basebr; p && *p && k < 7; ++p) if (*p != '[' && *p != ']' && *p != ' ') rb[k++] = *p; rb[k] = 0;
    int g = x86_rnum(dst), m = x86_rnum(rb);
    uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0x8B; code += (char)((0 << 6) | ((g & 7) << 3) | (m & 7));
    if ((m & 7) == 4) code += (char)0x24;
    return MEDIUM_BINARY ? x86_Lrec(code) : (x86_rec("mov") + dst + ", qword ptr [" + rb + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * x86_strkeep(const std::string & v) {
    static char ring[16][192]; static int i;
    i = (i + 1) & 15;
    size_t n = v.size(); if (n > 191) n = 191;
    memcpy(ring[i], v.data(), n); ring[i][n] = 0;
    return ring[i];
}
struct xop {
    const char * s; uint64_t u; int tag;
    xop()                    : s(0), u(0), tag(0) {}
    xop(const char * p)      : s(p), u(0), tag(p ? 1 : 0) {}
    xop(char * p)            : s(p), u(0), tag(p ? 1 : 0) {}
    xop(const std::string & v) : s(x86_strkeep(v)), u(0), tag(1) {}
    xop(int v)               : s(0), u((uint64_t)(int64_t)v), tag(2) {}
    xop(long v)              : s(0), u((uint64_t)v), tag(2) {}
    xop(long long v)         : s(0), u((uint64_t)v), tag(2) {}
    xop(unsigned v)          : s(0), u(v), tag(2) {}
    xop(unsigned long v)     : s(0), u(v), tag(2) {}
    xop(unsigned long long v): s(0), u(v), tag(2) {}
};
enum { XK_NONE = 0, XK_REG, XK_IMM, XK_PORT, XK_ILBL, XK_FR32, XK_FR64, XK_RSP64, XK_RSP32, XK_MEMIND, XK_MEMIDX8, XK_R13RCX, XK_RIPSEAL, XK_REGDISP, XK_REGDISP32, XK_SYM, XK_ROSLOT, XK_EXTLBL, XK_PAIR, XK_ABS64, XK_MEMBI, XK_RIPGOT, XK_RIPCELL };
struct opnd {
    int kind; const char * txt;
    int reg; long imm; int port; int lbl; int off;
    char base[8]; char idx[8]; char sym[96];
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_port_of(const char * s) {
    if ((unsigned char)s[0] == 0xCE) {
        switch ((unsigned char)s[1]) { case 0xB1: return X86P_ALPHA; case 0xB2: return X86P_BETA;
                                       case 0xB3: return X86P_GAMMA; }
    }
    if ((unsigned char)s[0] == 0xCF && (unsigned char)s[1] == 0x89) return X86P_OMEGA;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_is_reg(const char * s) {
    static const char * regs[] = { "rax","rbx","rcx","rdx","rsi","rdi","rsp","rb" "p","r8","r9","r10","r11","r12","r13","r14","r15",
                                   "eax","ebx","ecx","edx","esi","edi","esp","ebp","r8d","r9d","r10d","r11d","r12d","r13d","r14d","r15d",
                                   "al","cl","dl","bl","sil","dil","spl","bpl","r8b","r9b","r10b","r11b","r12b","r13b","r14b","r15b",
                                   "xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7" };
    for (size_t i = 0; i < sizeof regs / sizeof *regs; i++) if (!strcmp(s, regs[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline void x86_parse(const xop & x, opnd & o) {
    memset(&o, 0, sizeof o); o.kind = XK_NONE; o.txt = x.s;
    if (x.tag == 0) return;
    if (x.tag == 2) { o.kind = XK_IMM; o.imm = (long)(int64_t)x.u; return; }
    const char * s = x.s;
    int p = x86_port_of(s);
    if (p >= 0 && (s[2] == 0)) { fprintf(stderr, "x86_parse: string port operand \"%s\" is RETIRED (Lon 2026-07-08 s5) — ports go through x86_alpha/x86_beta/x86_gamma/x86_omega ONLY\n", s); abort(); }
    if (s[0] == 'L' && s[1] >= '0' && s[1] <= '9') { int n = atoi(s + 1); o.kind = XK_ILBL; o.lbl = n; return; }
    if (s[0] == 'P' && s[1] >= '0' && s[1] <= '9') { int _pdig = 1; for (const char * q = s + 1; *q; q++) if (*q < '0' || *q > '9') { _pdig = 0; break; } if (_pdig) { int n = atoi(s + 1); o.kind = XK_PAIR; o.lbl = n; return; } }
    if (!strcmp(s, "extlbl")) { o.kind = XK_EXTLBL; return; }
    if (!strncmp(s, "dword ptr [rsp# + ", 18)) { o.kind = XK_RSP32; o.off = atoi(s + 18); return; }
    if (!strncmp(s, "qword ptr [rsp# + ", 18)) { o.kind = XK_RSP64; o.off = atoi(s + 18); return; }
    if (!strncmp(s, x86_fr32_prefix(), strlen(x86_fr32_prefix()))) { o.kind = XK_FR32;  o.off = atoi(s + strlen(x86_fr32_prefix())); return; }
    if (!strncmp(s, x86_fr64_prefix(), strlen(x86_fr64_prefix()))) { o.kind = XK_FR64;  o.off = atoi(s + strlen(x86_fr64_prefix())); return; }
    if (!strncmp(s, "dword ptr [rsp + ", 17)) { o.kind = XK_RSP32; o.off = atoi(s + 17); return; }
    if (!strncmp(s, "dword ptr [", 11)) { const char * lb = s + 10; const char * pl = strstr(lb, " + ");
      if (pl) { size_t bl = (size_t)(pl - (lb + 1)); if (bl > 7) bl = 7; memcpy(o.base, lb + 1, bl); o.base[bl] = 0;
        char * ep = 0; long d = strtol(pl + 3, &ep, 10); if (x86_is_reg(o.base) && ep && *ep == ']') { o.kind = XK_REGDISP32; o.off = (int)d; return; } } }
    if (!strncmp(s, "qword ptr [rip + ", 17)) { o.kind = XK_ROSLOT; o.off = atoi(s + 17); return; }
    if (!strncmp(s, "qword ptr [rsp + ", 17)) { o.kind = XK_RSP64; o.off = atoi(s + 17); return; }
    if (!strncmp(s, "qword ptr [", 11) && s[11] >= '0' && s[11] <= '9') { char * ep = 0; unsigned long a = strtoul(s + 11, &ep, 10); if (ep && *ep == ']' && !ep[1]) { o.kind = XK_ABS64; o.imm = (long)a; return; } }
    if (!strncmp(s, "qword ptr [", 11)) { const char * lb = s + 10; const char * pl = strstr(lb, " + ");
      if (pl) { size_t bl = (size_t)(pl - (lb + 1)); if (bl > 7) bl = 7; memcpy(o.base, lb + 1, bl); o.base[bl] = 0;
        char * ep = 0; long d = strtol(pl + 3, &ep, 10); if (x86_is_reg(o.base) && ep && *ep == ']') { o.kind = XK_REGDISP; o.off = (int)d; return; } } }
    { char ns[32]; int k = 0; for (const char * q = s; *q && k < 31; q++) if (*q != ' ') ns[k++] = *q; ns[k] = 0;
      if (!strncmp(ns, "[r13+rcx+", 9)) { o.kind = XK_R13RCX; o.off = atoi(ns + 9); return; }
      if (!strcmp(ns, "[r13+rcx]")) { o.kind = XK_R13RCX; o.off = 0; return; }
      { const char * pp = strchr(ns, '+'); size_t nn = strlen(ns);
        if (ns[0] == '[' && pp && nn >= 3 && ns[nn - 1] == ']' && !strchr(pp + 1, '+') && !strchr(ns, '*')) {
          size_t bl = (size_t)(pp - (ns + 1)); if (bl > 7) bl = 7; char bb[8]; memcpy(bb, ns + 1, bl); bb[bl] = 0;
          size_t il = (size_t)((ns + nn - 1) - (pp + 1)); if (il > 7) il = 7; char ii[8]; memcpy(ii, pp + 1, il); ii[il] = 0;
          if (x86_is_reg(bb) && x86_is_reg(ii)) { memcpy(o.base, bb, bl + 1); memcpy(o.idx, ii, il + 1); o.kind = XK_MEMBI; return; } } } }
    if (!strcmp(s, "[rip + __]"))              { o.kind = XK_RIPSEAL; return; }
    if (!strcmp(s, "[rip@got + __]"))          { o.kind = XK_RIPGOT; return; }
    if (!strcmp(s, "[rip@cell + __]"))         { o.kind = XK_RIPCELL; return; }
    if (!strncmp(s, "f64:", 4))                {
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
            t = o.idx;  while (*t) { if (*t == ' ') { *t = 0; break; } t++; }
            o.kind = XK_MEMIDX8; return;
        }
        { const char * pl = strstr(s, " + "); if (pl) { size_t bl = (size_t)(pl - (s + 1)); if (bl > 7) bl = 7;
          char bbr[8]; memcpy(bbr, s + 1, bl); bbr[bl] = 0; char * ep = 0; long d = strtol(pl + 3, &ep, 10);
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
    o.kind = XK_SYM; size_t sl = strlen(s); if (sl > 95) sl = 95; memcpy(o.sym, s, sl); o.sym[sl] = 0;
}
inline std::string x86_bomb(const char * msg);
inline std::string x86_deflabel_pair(int idx);
inline std::string x86_jmp_pair(int idx);
inline std::string x86_jcc_pair(const char * mnem, int idx);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int x86_disp_w(const char * s, size_t n) { int w = 0; for (size_t i = 0; i < n; i++) if (((unsigned char)s[i] & 0xC0) != 0x80) w++; return w; }
static inline void x86_4col_pad(std::string & o, const char * s, size_t n, int width) { o.append(s, n); int pad = width - x86_disp_w(s, n); if (pad < 1) pad = 1; o.append((size_t)pad, ' '); }
#include "x86_arg_roles.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int x86_argreg_slot(const char * s, size_t n) {
    static const char * const r[6][4] = { { "rdi", "edi", "di", "dil" }, { "rsi", "esi", "si", "sil" }, { "rdx", "edx", "dx", "dl" }, { "rcx", "ecx", "cx", "cl" }, { "r8", "r8d", "r8w", "r8b" }, { "r9", "r9d", "r9w", "r9b" } };
    for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) { size_t l = strlen(r[i][j]); if (l == n && !strncmp(s, r[i][j], l)) return i; }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline const x86_argrole_t * x86_argrole_find(const char * s, size_t n) {
    for (size_t i = 0; i < sizeof(x86_argroles) / sizeof(x86_argroles[0]); i++) { const char * c = x86_argroles[i].callee; if (strlen(c) == n && !strncmp(s, c, n)) return &x86_argroles[i]; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void x86_line_parts(const std::string & o, size_t ls, size_t le, size_t & ms, size_t & ml, size_t & os_, size_t & ol) {
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
static inline void x86_argnote(std::string & o) {
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
            int isld = (bl == 3 && (!strncmp(o.data() + bs, "mov", 3) || !strncmp(o.data() + bs, "lea", 3) || !strncmp(o.data() + bs, "xor", 3))) || (bl == 6 && !strncmp(o.data() + bs, "movsxd", 6)) || (bl == 5 && !strncmp(o.data() + bs, "movzx", 5)) || (bl == 6 && !strncmp(o.data() + bs, "movabs", 6));
            if (!isld || !bol) break;
            size_t de = bo; while (de < bo + bol && o[de] != ',' && o[de] != ' ' && o[de] != '\t') de++;
            int slot = x86_argreg_slot(o.data() + bo, de - bo);
            if (slot < 0) break;
            if (o.find('#', beg[B]) < end[B]) continue;
            if (rr->role[slot] && rr->role[slot][0]) ann[B] = rr->role[slot];
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
static inline void x86_4col_to(std::string & o, size_t ls, int col) { int w = x86_disp_w(o.data() + ls, o.size() - ls); int pd = col - w; if (pd < 1) pd = 1; o.append((size_t)pd, ' '); }
struct x86_rec_t { const char * lb; size_t ll; const char * op; size_t ol; const char * ar; size_t al; int marg; };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void x86_rec_split(const char * p, size_t len, x86_rec_t & r) {
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
    if (k > 0 && t[k - 1] == ':') { r.lb = t; r.ll = k; size_t rr = k; while (rr < tl && (t[rr] == ' ' || t[rr] == '\t')) rr++; if (rr >= tl) return; t += rr; tl -= rr; k = 0; while (k < tl && t[k] != ' ' && t[k] != '\t') k++; }
    r.op = t; r.ol = k;
    if ((k == 3 && !strncmp(t, "rep", 3)) || (k == 4 && (!strncmp(t, "repe", 4) || !strncmp(t, "repz", 4) || !strncmp(t, "lock", 4))) || (k == 5 && (!strncmp(t, "repne", 5) || !strncmp(t, "repnz", 5)))) {
        size_t w = k; while (w < tl && (t[w] == ' ' || t[w] == '\t')) w++;
        if (w < tl) { size_t m2 = w; while (m2 < tl && t[m2] != ' ' && t[m2] != '\t') m2++; r.ol = m2; } }
    size_t rr = r.ol; while (rr < tl && (t[rr] == ' ' || t[rr] == '\t')) rr++;
    if (rr < tl) { r.ar = t + rr; r.al = tl - rr; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int x86_rec_kind(const x86_rec_t & r) { if (r.marg) return r.al ? 4 : 0; if (!r.ol) return 1; return (r.op[0] == 'j') ? 3 : 2; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_4col(const std::string & s) {
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
        if (inst && !note.empty()) { int drop = isj || (willjoin && x86_disp_w(o.data() + ls, o.size() - ls) + 1 < CJ); if (!drop && note != prevnote && o.find('#', ls) == std::string::npos) { x86_4col_to(o, ls, CJ); o.append("# "); o.append(note); prevnote = note; } if (!drop) { if (note != prevnote) prevnote.clear(); } note.clear(); }
        pend = (ck == 0 || ck == 4) ? 4 : ck; pls = ls;
        i = inext;
    }
    if (pend) o.append(1, '\n');
    if (!note.empty()) { o.append("#@"); o.append(note); o.append(1, '\n'); }
    x86_argnote(o);
    return o;
}
inline std::string x86_core_(const char * mnem, xop xa, xop xb, xop xc, xop xd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86(const char * mnem, xop xa = xop(), xop xb = xop(), xop xc = xop(), xop xd = xop()) { return x86_core_(mnem, xa, xb, xc, xd); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_core_(const char * mnem, xop xa, xop xb, xop xc, xop xd) {
    opnd a, b, c; x86_parse(xa, a); x86_parse(xb, b); x86_parse(xc, c);
    if (!strcmp(mnem, "label"))     return (MEDIUM_BINARY || MEDIUM_MACRO_DEF) ? std::string() : x86_reclbl(std::string(xa.s ? xa.s : "")) + "\n";
    if (!strcmp(mnem, "comment"))   return std::string();
    if (!strcmp(mnem, "note")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF || !xa.s || !xa.s[0]) ? std::string() : (std::string("#@") + xa.s + "\n");
    if (!strcmp(mnem, "srccomment")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF) ? std::string() : (std::string("# ") + (xa.s ? xa.s : "") + "\n");
    if (!strcmp(mnem, "loc")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF || !xa.s || !xa.s[0]) ? std::string() : (std::string(".loc ") + std::to_string(xc.u) + " " + std::to_string(xb.u) + " 0\n");
    if (!strcmp(mnem, "commentrule")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF) ? std::string() : (std::string("#") + (xa.s ? xa.s : "") + "\n");
    if (!strcmp(mnem, "directive")) return MEDIUM_BINARY ? std::string() : (std::string("  ") + (xa.s ? xa.s : "") + "\n");
    if (!strcmp(mnem, "raw"))       return MEDIUM_BINARY ? std::string() : (std::string(" ") + (xa.s ? xa.s : "") + "\n");
    if (!strcmp(mnem, ".quad")) {
        if (a.kind == XK_ILBL) return x86_quad_ilbl(a.lbl);
        if (xa.tag == 2) return MEDIUM_BINARY ? x86_Lrec(u64le(xa.u)) : (std::string(" .quad ") + std::to_string((unsigned long long)xa.u) + "\n");
        if (xa.tag == 1 && xb.tag == 1) return MEDIUM_BINARY ? x86_Lrec(u64le((uint64_t)(uintptr_t)(xb.s ? xb.s : ""))) : (std::string(" .quad ") + (xa.s ? xa.s : "") + "\n");
        return std::string();
    }
    if (!strcmp(mnem, ".string")) return MEDIUM_BINARY ? std::string() : (std::string(" .string \"") + x86_asm_str_escape(xa.s ? xa.s : "") + "\"\n");
    if (!strcmp(mnem, "ret")) return MEDIUM_BINARY ? x86_Lrec(std::string(1, (char)0xC3)) : x86_recn("ret") + "\n";
    if (!strcmp(mnem, "cqo")) return x86_cqo();
    if (!strcmp(mnem, "rep_stosb")) return x86_rep_stosb();
    if (!strcmp(mnem, "def")) {
        if (a.kind == XK_PORT) return x86_deflabel(a.port);
        if (a.kind == XK_ILBL) return x86_deflabel_id(a.lbl);
        if (a.kind == XK_PAIR) return x86_deflabel_pair(a.lbl);
        return std::string();
    }
    if (!strcmp(mnem, "jmp")) {
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
        return std::string();
    }
    if (mnem[0] == 'j') {
        if (a.kind == XK_PORT) return x86_jcc(mnem, a.port);
        if (a.kind == XK_ILBL) return x86_jcc_id(mnem, a.lbl);
        if (a.kind == XK_PAIR) return x86_jcc_pair(mnem, a.lbl);
        if (a.kind == XK_EXTLBL && xb.tag == 2) return x86_jcc_ext(mnem, (const struct bb_label_t *)(uintptr_t)xb.u);
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return x86_rec(mnem) + a.sym + "\n";
        return std::string();
    }
    if (!strcmp(mnem, "call")) {
        if (a.kind == XK_PORT) return x86_align_assert() + (MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE8)) + x86_Jrec(a.port))
                                                    : (x86_rec("call") + x86_portname(a.port) + "\n"));
        if (a.kind == XK_SYM && xb.tag == 2) return x86_rtcc_call(a.sym, xb.u);
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return x86_align_assert() + x86_rec("call") + a.sym + "\n";
        if (a.kind == XK_REG) {
            int m = x86_rnum(a.txt); uint8_t modrm = (uint8_t)(0xD0 | (m & 7)); uint8_t rex = (m >= 8) ? 0x41 : 0x40;
            return x86_align_assert() + (MEDIUM_BINARY ? x86_Lrec(std::string((char)rex == 0x40 ? "" : std::string(1,(char)rex)) + (char)0xFF + (char)modrm) : (x86_rec("call") + a.txt + "\n"));
        }
        return std::string();
    }
    if (!strcmp(mnem, "call_rt")) {
        if (a.kind == XK_SYM && b.kind == XK_IMM && c.kind == XK_IMM)
            return x86_rtcc_call_descr(a.sym, (uint64_t)c.imm, (int)b.imm);
        return std::string();
    }
    if (!strcmp(mnem, "jmp_fn")) { if (a.kind == XK_SYM && xb.tag == 2) return x86_jmp_fn_body(a.sym, xb.u); return std::string(); }
    if (!strcmp(mnem, "jmp_fn_cell")) { if (a.kind == XK_SYM && xb.tag == 2) return x86_jmp_through_fn_cell(a.sym, xb.u); return std::string(); }
    if (!strcmp(mnem, "call_bare")) {
        if (a.kind == XK_SYM && xb.tag == 2) return x86_call_ro(a.sym, xb.u);
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return x86_align_assert() + x86_rec("call") + a.sym + "\n";
        return std::string();
    }
    if (!strcmp(mnem, "rtcc_wb")) {
        unsigned vm = x86_rtcc_veneer_mask(); if (!vm) return std::string();
        uint64_t block = (uint64_t)(uintptr_t)rtccb;
        return MEDIUM_BINARY ? x86_Lrec(x86_rtcc_wb_bin(block, RTCC_C_ALL & vm)) : x86_rtcc_wb_text(RTCC_C_ALL & vm);
    }
    if (!strcmp(mnem, "rtcc_rl")) {
        unsigned vm = x86_rtcc_veneer_mask(); if (!vm) return std::string();
        uint64_t block = (uint64_t)(uintptr_t)rtccb;
        return MEDIUM_BINARY ? x86_Lrec(x86_rtcc_rl_bin(block, RTCC_C_ALL & vm)) : x86_rtcc_rl_text(RTCC_C_ALL & vm);
    }
    if (!strcmp(mnem, "rtcc_anchor_cmp")) {
        if (!RTCC_GLOBAL_R8_ANCHOR) {
            uint64_t anchor_addr = (uint64_t)(uintptr_t)(const void *)rt_anchor_ptr();
            if (MEDIUM_BINARY) {
                std::string s;
                s += (char)0x48; s += (char)0xB9; s += u64le(anchor_addr);
                s += (char)0x4C; s += (char)0x8B; s += (char)0x01;
                s += (char)0x48; s += (char)0x83; s += (char)0xF8; s += (char)0x00;
                return x86_Lrec(s);
            }
            return x86_rec("mov") + "rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]\n mov rax, qword ptr [rcx]\n cmp rax, 0\n";
        }
        if (MEDIUM_BINARY) { std::string s; s += (char)0x4D; s += (char)0x85; s += (char)0xC0; return x86_Lrec(s); }
        return x86_rec("test") + "r8, r8\n";
    }
    if (!strcmp(mnem, "push")) return x86_push(a.txt);
    if (!strcmp(mnem, "pop"))  return x86_pop(a.txt);
    if (!strcmp(mnem, "idiv")) return x86_idiv(a.txt);
    if (!strcmp(mnem, "neg"))  return x86_neg(a.txt);
    if (!strcmp(mnem, "inc")) {
        if (a.kind == XK_FR64) return x86_frame_inc64(a.off);
        if (a.kind == XK_REG) return x86_inc_r(a.txt);
        return std::string();
    }
    if (!strcmp(mnem, "movabs")) {
        if (a.kind == XK_REG && b.kind == XK_IMM)      return x86_movabs_r64(a.txt, (uint64_t)b.imm);
    }
    if (!strcmp(mnem, "mov")) {
        if (a.kind == XK_REG && b.kind == XK_R13RCX && a.txt && a.txt[0] == 'r' && a.txt[strlen(a.txt) - 1] != 'd') return x86_mov_subj_q(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_R13RCX)                return x86_mov_subj_d(a.txt, b.off);
        if (a.kind == XK_FR32 && b.kind == XK_REG)     return x86_frame_store(a.off, b.txt);
        if (a.kind == XK_FR32 && b.kind == XK_IMM)     return x86_frame_mov_imm(a.off, b.imm);
        if (a.kind == XK_FR64 && b.kind == XK_REG)     return x86_frame_store64(a.off, b.txt);
        if (a.kind == XK_FR64 && b.kind == XK_IMM)     return x86_frame_mov_imm64(a.off, b.imm);
        if (a.kind == XK_RSP64 && b.kind == XK_REG)    return x86_rsp_store64(a.off, b.txt);
        if (a.kind == XK_RSP64 && b.kind == XK_IMM)    return x86_rsp_store64_imm(a.off, b.imm);
        if (a.kind == XK_RSP32 && b.kind == XK_REG)    return x86_rsp_store32(a.off, b.txt);
        if (a.kind == XK_RSP32 && b.kind == XK_IMM)    return x86_rsp_store32_imm(a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_FR32)     return x86_frame_load(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_FR64)     return x86_frame_load64(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_ROSLOT)   return x86_ro_load_q(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_RSP64)    return x86_rsp_load64(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_RSP32)    return x86_rsp_load32(a.txt, b.off);
        if (a.kind == XK_REGDISP && b.kind == XK_REG)  return x86_reg_disp32_store64(a.base, a.off, b.txt);
        if (a.kind == XK_REGDISP && b.kind == XK_IMM)  return x86_reg_disp32_store_imm64(a.base, a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_REGDISP)  return x86_reg_disp32_load64(a.txt, b.base, b.off);
        if (a.kind == XK_ABS64 && b.kind == XK_REG)    return x86_abs_disp32_store64(a.imm, b.txt);
        if (a.kind == XK_REG && b.kind == XK_ABS64)    return x86_abs_disp32_load64(a.txt, b.imm);
        if (a.kind == XK_REGDISP32 && b.kind == XK_REG)  return x86_reg_disp32_store32(a.base, a.off, b.txt);
        if (a.kind == XK_REGDISP32 && b.kind == XK_IMM)  return x86_reg_disp32_store_imm32(a.base, a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_REGDISP32)  return x86_reg_disp32_load32(a.txt, b.base, b.off);
        if (a.kind == XK_REG && b.kind == XK_MEMIDX8)  return x86_load_indexed8(a.txt, b.base, b.idx);
        if (a.kind == XK_REG && b.kind == XK_MEMIND)   return x86_load_mem64(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_RIPGOT)   return x86_load_got(a.txt, xd.s, xc.u);
        if (a.kind == XK_REG && b.kind == XK_RIPSEAL)  return x86_load_ro(a.txt, xd.s, xc.u);
        if (a.kind == XK_REG && b.kind == XK_REG)      return x86_mov(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM)      return x86_movimm(a.txt, b.imm);
        fprintf(stderr, "FATAL x86(\"mov\"): no dispatch arm for operand pair kinds (%d, %d) — dest '%s', src '%s'.  Add the encoder + dispatch case here (R7); never let a mov emit nothing.\n",
                a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
        abort();
    }
    if (!strcmp(mnem, "mov32")) { if (a.kind == XK_REG && b.kind == XK_IMM) return x86_movimm32(a.txt, b.imm); return std::string(); }
    if (!strcmp(mnem, "stk32")) { if (a.kind == XK_IMM && b.kind == XK_IMM) return x86_rsp_store32_imm((int)a.imm, b.imm); return std::string(); }
    if (!strcmp(mnem, "movabs")) { if (a.kind == XK_REG && xb.tag == 2) return x86_movabs_r64(a.txt, xb.u); return std::string(); }
    if (!strcmp(mnem, "xor"))    { if (a.kind == XK_REG && b.kind == XK_REG) return x86_xor_rr(a.txt, b.txt); return std::string(); }
    if (!strcmp(mnem, "lea")) {
        if (a.kind == XK_REG && b.kind == XK_ILBL)                  return x86_lea_rip_id(a.txt, b.lbl);
        if (a.kind == XK_REG && b.kind == XK_RIPSEAL)               return x86_load_ro(a.txt, xd.s, xc.u);
        if (a.kind == XK_REG && b.kind == XK_RIPGOT)                return x86_load_got(a.txt, xd.s, xc.u);
        if (a.kind == XK_REG && b.kind == XK_EXTLBL && xc.tag == 2) return x86_lea_ext(a.txt, (const struct bb_label_t *)(uintptr_t)xc.u);
        if (a.kind == XK_REG && (b.kind == XK_FR32 || b.kind == XK_FR64)) return x86_frame_lea(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_REGDISP)              return x86_reg_disp32_lea64(a.txt, b.base, b.off);
        if (a.kind == XK_REG && b.kind == XK_REGDISP32)            return x86_reg_disp32_lea64(a.txt, b.base, b.off);
        if (a.kind == XK_REG && b.kind == XK_R13RCX)                return x86_lea_subj_cursor(a.txt);
        if (a.kind == XK_REG && b.kind == XK_REG)                   return x86_lea_subj_cursor(a.txt);
        if (a.kind == XK_REG && (b.kind == XK_RSP32 || b.kind == XK_RSP64)) return x86_reg_disp32_lea64(a.txt, "rsp", b.off);
        if (b.txt && strstr(b.txt, "rip"))                          return x86_bomb("lea: unsealed [rip + label] operand — use the [rip + __] sealed form with (ptr,label) args");
        if (b.kind == XK_RSP32 || b.kind == XK_RSP64 || b.kind == XK_FR32 || b.kind == XK_FR64 || b.kind == XK_REGDISP || b.kind == XK_REGDISP32) {
            fprintf(stderr, "FATAL x86(\"lea\"): no dispatch arm for frame/cell operand kind %d — dest '%s', src '%s'.  A lea that emits nothing is the ZB-FC-1 silent-drop corruption class (measured: bb_match_arbno's PAIR(2)/PAIR(3) view leas were dropped for months, masked only by r12 being callee-saved); add the encoder + dispatch case here (R7).\n",
                    b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
            abort();
        }
        return std::string();
    }
    if (!strcmp(mnem, "add")) {
        if (a.kind == XK_REG && b.kind == XK_REG)  return x86_add_rr(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM)  return x86_add(a.txt, b.imm);
        if (a.kind == XK_ABS64 && b.kind == XK_IMM) return x86_abs_disp32_addsub_imm8(0, a.imm, b.imm);
        if (a.kind == XK_REG && b.kind == XK_FR32) return x86_frame_add_to_reg(a.txt, b.off);
        if (a.kind == XK_FR32 && b.kind == XK_IMM) return x86_frame_add_imm(a.off, b.imm);
        if (a.kind == XK_REGDISP32 && b.kind == XK_IMM) return x86_reg_disp32_add_imm32(a.base, a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_REGDISP32) return x86_reg_disp32_add32(a.txt, b.base, b.off);
        if (a.kind == XK_REG && b.kind == XK_RSP32) return x86_rsp_add_to_reg32(a.txt, b.off);
        if (a.kind == XK_RSP32 && b.kind == XK_IMM) return x86_rsp_add_imm32(a.off, b.imm);
        if (a.kind == XK_FR32 || a.kind == XK_FR64 || a.kind == XK_RSP32 || a.kind == XK_RSP64 || a.kind == XK_REGDISP || a.kind == XK_REGDISP32 || b.kind == XK_FR32 || b.kind == XK_FR64 || b.kind == XK_RSP32 || b.kind == XK_RSP64 || b.kind == XK_REGDISP || b.kind == XK_REGDISP32) {
            fprintf(stderr, "FATAL x86(\"add\"): no dispatch arm for frame/cell operand pair kinds (%d, %d) — dest '%s', src '%s'.  A frame/cell access that emits nothing is the ZB-FC-1 silent-drop corruption class (the mov precedent, 2026-07-08); add the encoder + dispatch case here (R7).\n",
                    a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
            abort();
        }
        return std::string();
    }
    if (!strcmp(mnem, "sub")) {
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_sub_rr(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM) return x86_sub(a.txt, b.imm);
        if (a.kind == XK_ABS64 && b.kind == XK_IMM) return x86_abs_disp32_addsub_imm8(1, a.imm, b.imm);
        if (a.kind == XK_REG && b.kind == XK_FR32) return x86_frame_sub_from_reg(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_RSP32) return x86_rsp_sub_from_reg32(a.txt, b.off);
        if (a.kind == XK_FR32 || a.kind == XK_FR64 || a.kind == XK_RSP32 || a.kind == XK_RSP64 || b.kind == XK_FR32 || b.kind == XK_FR64 || b.kind == XK_RSP32 || b.kind == XK_RSP64) {
            fprintf(stderr, "FATAL x86(\"sub\"): no dispatch arm for frame/cell operand pair kinds (%d, %d) — dest '%s', src '%s'.  A frame/cell access that emits nothing is the ZB-FC-1 silent-drop corruption class (the mov precedent, 2026-07-08); add the encoder + dispatch case here (R7).\n",
                    a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
            abort();
        }
        return std::string();
    }
    if (!strcmp(mnem, "imul"))   { return x86_imul_rr(a.txt, b.txt); }
    if (!strcmp(mnem, "and"))    { if (b.kind == XK_IMM) return x86_and(a.txt, b.imm); if (a.kind == XK_REG && b.kind == XK_REG) return x86_alu_rr("and", 0x21, a.txt, b.txt); return std::string(); }
    if (!strcmp(mnem, "or"))     { if (a.kind == XK_REG && b.kind == XK_REG) return x86_alu_rr("or", 0x09, a.txt, b.txt); return std::string(); }
    if (!strcmp(mnem, "cmp")) {
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_cmp(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM) return x86_cmp_imm(a.txt, b.imm);
        if (a.kind == XK_REG && b.kind == XK_ABS64) return x86_cmp_reg_abs64(a.txt, b.imm);
        if (a.kind == XK_REGDISP && b.kind == XK_IMM) return x86_reg_disp32_cmp_imm(a.base, a.off, b.imm);
        if (a.kind == XK_FR32 || a.kind == XK_FR64 || a.kind == XK_RSP32 || a.kind == XK_RSP64 || a.kind == XK_REGDISP || a.kind == XK_REGDISP32) {
            fprintf(stderr, "FATAL x86(\"cmp\"): no dispatch arm for frame/cell operand kinds (%d, %d) — dest '%s', src '%s'.  A cmp that emits nothing leaves the following jcc testing STALE FLAGS — the ZB-FC-1 silent-drop class (measured s23o: SPD-2's guard cmps vanished when RDQ(\"___\",·) collided with the pinned fr64 prefix and parsed XK_FR64; only the .s region diff caught it, the probes stayed green on garbage flags).  Add the encoder + dispatch case here (R7).\n",
                    a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
            abort();
        }
        return std::string();
    }
    if (!strcmp(mnem, "cmp64"))  { if (b.kind == XK_IMM) return x86_cmp_imm64(a.txt, b.imm); return std::string(); }
    if (!strcmp(mnem, "test"))   { return x86_test(a.txt, b.txt); }
    if (!strcmp(mnem, "movsxd")) {
        if (a.kind == XK_REG && b.kind == XK_FR32) return x86_movsxd_frame(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_REG)  return x86_movsxd(a.txt, b.txt);
        fprintf(stderr, "FATAL x86(\"movsxd\"): no dispatch arm for operand pair kinds (%d, %d) — dest '%s', src '%s'.  A movsxd emitting the reg/reg form for a memory source is the mode-3/mode-4 divergence class (scan-nary saved_delta, 2026-07-13); add the encoder + dispatch case here (R7).\n",
                a.kind, b.kind, a.txt ? a.txt : "(null)", b.txt ? b.txt : "(null)");
        abort();
    }
    if (!strcmp(mnem, "movzx"))  {
        if (b.kind == XK_MEMBI)  return x86_movzx_bir(a.txt, b.base, b.idx);
        if (b.kind == XK_REG) {
            int g = x86_rnum(a.txt), m = x86_rnum(b.txt);
            uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
            uint8_t modrm = (uint8_t)(0xC0 | ((g & 7) << 3) | (m & 7));
            if (MEDIUM_BINARY) return x86_Lrec(x86_b4(rex, 0x0F, 0xB6, modrm));
            return x86_rec("movzx") + a.txt + std::string(", ") + b.txt + std::string("\n");
        }
        return x86_movzx_subj_byte(a.txt, b.kind == XK_R13RCX ? b.off : 0);
    }
    if (!strcmp(mnem, "cmpb0"))  { (void)a; (void)b; return x86_cset_probe(); }
    if (!strcmp(mnem, "bt"))     { (void)a; (void)b; return x86_cset_bt(); }
    if (!strcmp(mnem, "xorps"))  { return x86_xorps_xmm0(); }
    if (!strcmp(mnem, "movsd"))  {
        if (b.txt && !strncmp(b.txt, "f64:", 4)) { uint64_t bits = strtoull(b.txt + 4, 0, 10); double d; memcpy(&d, &bits, 8); return x86_set_xmm0_double(d); }
        return std::string();
    }
    if (!strcmp(mnem, "movq")) {
        if (a.kind == XK_REG && b.kind == XK_REG && a.txt && !strncmp(a.txt, "xmm", 3)) return x86_movq_xmm_r64(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_REG && b.txt && !strncmp(b.txt, "xmm", 3)) return x86_movq_r64_xmm(a.txt, b.txt);
        return std::string();
    }
    if (!strcmp(mnem, "addsd"))    { if (a.kind == XK_REG && b.kind == XK_REG) return x86_sse2_xx("addsd", 0x58, a.txt, b.txt); return std::string(); }
    if (!strcmp(mnem, "subsd"))    { if (a.kind == XK_REG && b.kind == XK_REG) return x86_sse2_xx("subsd", 0x5C, a.txt, b.txt); return std::string(); }
    if (!strcmp(mnem, "mulsd"))    { if (a.kind == XK_REG && b.kind == XK_REG) return x86_sse2_xx("mulsd", 0x59, a.txt, b.txt); return std::string(); }
    if (!strcmp(mnem, "cvtsi2sd")) { if (a.kind == XK_REG && b.kind == XK_REG) return x86_cvtsi2sd_xmm_r64(a.txt, b.txt); return std::string(); }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_align_enter() {
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string();
    if (MEDIUM_BINARY) return x86_Lrec(x86_b1(0x54) + x86_b3(0xFF, 0x34, 0x24) + x86_b2(0x48, 0x83) + x86_b2(0xE4, 0xF0));
    return x86_rec("push") + "rsp\n push qword ptr [rsp]\n and rsp, -16\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_align_leave() {
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string();
    if (MEDIUM_BINARY) return x86_Lrec(x86_b3(0x48, 0x8B, 0x64) + x86_b2(0x24, 0x08));
    return x86_rec("mov") + "rsp, [rsp + 8]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_anchor_enter() { return x86_align_enter(); }
inline std::string x86_anchor_leave() { return x86_align_leave(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_sink() {
    return x86("mov", "rcx", "rsp")
         + x86("sub", "rsp", "rax")
         + x86("sub", "rsp", 16L)
         + x86("and", "rsp", -16L)
         + x86("mov", "qword ptr [rsp + 0]", "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_base(const char * r) {
    return x86("mov", r, "rsp") + x86("add", r, 16L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_unsink() {
    return x86("mov", "rsp", "qword ptr [rsp + 0]");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_xfer_enter() {
    if (x86_zc_frame() == ZC_FRAME_RSP) return x86("push", "r14") + x86("push", "r15") + x86("push", "r13") + x86("sub", "rsp", 8L);
    return x86("push", "r14") + x86("push", "r15") + x86("push", "r13");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_xfer_leave() {
    if (x86_zc_frame() == ZC_FRAME_RSP) return x86("add", "rsp", 8L) + x86("pop", "r13") + x86("pop", "r15") + x86("pop", "r14");
    return x86("pop", "r13") + x86("pop", "r15") + x86("pop", "r14");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_alpha_carve(long K)          { return K > 0 ? x86("sub", "rsp", K) : std::string(); }
inline std::string x86_gamma_free(long K)           { return K > 0 ? x86("add", "rsp", K) : std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_gamma_result(long K, long n) {
    long drop = K + 16 * (n - 1);
    return x86("mov", "rax", RDQ("rsp", 0)) + x86("mov", "rdx", RDQ("rsp", 8))
         + (drop > 0 ? x86("add", "rsp", drop) : std::string())
         + x86("mov", RDQ("rsp", 0), "rax") + x86("mov", RDQ("rsp", 8), "rdx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_cell_push(int tag, int resume_ilbl) {
    return x86("xor", "rax", "rax") + x86("push", "rax")
         + std::string("")
         + x86("push", "r14")
         + (tag ? x86_rsp_store32_imm(4, (long)tag) : std::string())
         + x86_lea_rip_id("rax", resume_ilbl)
         + x86("push", "rax")
         + std::string("");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_main_prologue()             { return std::string(); }
inline std::string x86_cell_unwind_tail()           { return x86("pop", "rax") + x86("pop", "r14") + std::string("") + x86("add", "rsp", 8L) + x86_jmp_reg("rax"); }
inline std::string x86_cell_fail_body()             { return std::string("") + x86_cell_unwind_tail(); }
inline std::string x86_cell_cut_keep(const char * base) { return x86_reg_disp32_load64("rsp", base, 16) + x86_reg_disp32_lea64("rsp", base, 32); }
inline std::string x86_chain_prev(const char * dst, const char * src) { return x86_reg_disp32_load64(dst, src, 16); }
inline std::string x86_chain_tag_load(const char * dst32, const char * cell) { return x86_reg_disp32_load32(dst32, cell, 12); }
inline std::string x86_zclaim(long b)   { return x86("sub", "rsp", b); }
inline std::string x86_zrelease(long b) { return x86("add", "rsp", b); }
inline std::string x86_arbno_rbp_unwind_at(const std::string & mark_mem, int l_loop, int l_done) { return x86("note", "arbno_rbp_unwind") + x86("mov", "rax", mark_mem) + x86("def", L(l_loop)) + x86("cmp", "rbp", "rax") + x86("jae", L(l_done)) + x86("test", "rbp", "rbp") + x86("je", L(l_done)) + x86("mov", "rbp", RDQ("rbp", 24)) + x86("jmp", L(l_loop)) + x86("def", L(l_done)); }
inline std::string x86_arbno_rbp_unwind(const char * mark_base, int mark_off, int l_loop, int l_done) { return x86_arbno_rbp_unwind_at(RDQ(mark_base, mark_off), l_loop, l_done); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_call_frame_enter(int gamma_ilbl, int omega_ilbl) {
    return x86_lea_rip_id("rcx", gamma_ilbl)
         + x86_lea_rip_id("rdx", omega_ilbl)
         + x86("push", "rdx")
         + x86("push", "rcx")
         + std::string("")
         + x86("sub", "rsp", 8L)
         + std::string("");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_srf_floater(int wire_disp) {
    return std::string("")
         + x86_reg_disp32_load64("rcx", "rsp", wire_disp)
         + x86_reg_disp32_load64("rsp", "rsp", 8)
         + x86("add", "rsp", 32L)
         + x86_jmp_reg("rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_return_floater()  { return x86_srf_floater(16); }
inline std::string x86_freturn_floater() { return x86_srf_floater(24); }
extern "C" void rt_scan_sync_out(uint64_t delta);
extern "C" uint64_t rt_scan_sync_in(void);
extern "C" int g_scan_regs_live;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_scan_sync_out() {
    if (!g_scan_regs_live) return std::string();
    return x86("mov", "rdi", "r14") + x86("call", "rt_scan_sync_out", (uint64_t)(uintptr_t)(void *)rt_scan_sync_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_scan_sync_in_rr() {
    if (!g_scan_regs_live) return std::string();
    return x86("push", "rax") + x86("push", "rdx")
         + x86("call", "rt_scan_sync_in", (uint64_t)(uintptr_t)(void *)rt_scan_sync_in)
         + x86("mov", "r14", "rax")
         + x86("pop", "rdx") + x86("pop", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_scan_sync_out_force() {
    return x86("mov", "rdi", "r14") + x86("call", "rt_scan_sync_out", (uint64_t)(uintptr_t)(void *)rt_scan_sync_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_scan_sync_in_rr_force() {
    return x86("push", "rax") + x86("push", "rdx")
         + x86("call", "rt_scan_sync_in", (uint64_t)(uintptr_t)(void *)rt_scan_sync_in)
         + x86("mov", "r14", "rax")
         + x86("pop", "rdx") + x86("pop", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline bool x86_is_scan_builtin_name(const char *fn) {
    if (!fn) return false;
    return !strcmp(fn, "tab")  || !strcmp(fn, "move") || !strcmp(fn, "upto") || !strcmp(fn, "many")
        || !strcmp(fn, "any")  || !strcmp(fn, "find") || !strcmp(fn, "match")|| !strcmp(fn, "pos")
        || !strcmp(fn, "bal");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zeta_free_call() {
    return x86_align_enter()
         + x86("mov",  "rdi", x86_zr())
         + x86("call", "rt_zls_release", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_release)
         + x86_align_leave();
}
extern "C" void *rt_zls_mark(void);
extern "C" void  rt_zls_release_to(void *);
extern "C" void *rt_zls2_mark(void);
extern "C" void  rt_zls2_release_to(void *);
extern "C" char *g_zls2_cur;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zls2_cur_lea(const char * reg) {
    return x86_load_ro(reg, "g_zls2_cur", (uint64_t)(uintptr_t)(void *)&g_zls2_cur);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zls2_mark_save(const char * slot) {
    if (x86_port_cstack())
        return x86("note", HKN(5)) + x86("mov", slot, "rsp");
    if (x86_port_mode() == ZC_PORT_INLINE || x86_port_mode() == ZC_PORT_OWNED)
        return x86_zls2_cur_lea("rdi")
             + x86("mov", "rax", RDQ("rdi", 0))
             + x86("note", HKN(5)) + x86("mov", slot, "rax");
    if (x86_port_mode() != ZC_PORT_ALLOC) return std::string();
    return x86("call", "rt_zls2_mark", (uint64_t)(uintptr_t)(void *)rt_zls2_mark)
         + x86("note", HKN(5)) + x86("mov", slot, "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zls2_release_to_call(const char * slot) {
    if (x86_port_cstack())
        return x86_align_leave()
             + x86("mov", "rsp", slot)
             + x86_align_enter();
    if (x86_port_mode() == ZC_PORT_INLINE || x86_port_mode() == ZC_PORT_OWNED)
        return x86_zls2_cur_lea("rdi")
             + x86("mov", "rax", slot)
             + x86("mov", RDQ("rdi", 0), "rax");
    if (x86_port_mode() != ZC_PORT_ALLOC) return std::string();
    return x86("note", HKN(5)) + x86("mov",  "rdi", slot)
         + x86("call", "rt_zls2_release_to", (uint64_t)(uintptr_t)(void *)rt_zls2_release_to);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zls2_release_to_rspd(int disp) {
    if (!x86_port_cstack()) return x86_bomb("x86_zls2_release_to_rspd: FORTH-grant-only helper reached on a non-cstack port");
    static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp + %d]", disp);
    return x86_align_leave()
         + x86("mov", "rsp", b[i])
         + x86_align_enter();
}
extern "C" void *rt_zls2_push(long k);
extern "C" void  rt_zls2_pop(long k);
extern "C" void *rt_zls_alloc(long bytes);
extern "C" void  rt_zls_release(void *fb);
extern "C" void  rt_zls_arbno_step1_store(void *p);
extern "C" void *rt_zls_arbno_step1_load(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zls2_push_call(long k) {
    return x86_align_enter()
         + x86("mov",  "rdi", k)
         + x86("call", "rt_zls2_push", (uint64_t)(uintptr_t)(void *)(void * (*)(long))rt_zls2_push)
         + x86_align_leave();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zls2_pop_call(long k) {
    return x86_align_enter()
         + x86("mov",  "rdi", k)
         + x86("call", "rt_zls2_pop", (uint64_t)(uintptr_t)(void *)(void (*)(long))rt_zls2_pop)
         + x86_align_leave();
}
extern "C" void rt_zls2_release_to(void *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zls2_release_to_reg(const char * reg, long disp) {
    return x86_align_enter()
         + (disp ? (x86("mov", "rdi", reg) + x86("add", "rdi", disp)) : x86("mov", "rdi", reg))
         + x86("call", "rt_zls2_release_to", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls2_release_to)
         + x86_align_leave();
}
std::string bb_glue_flat_enter();
std::string bb_glue_flat_leave();
std::string bb_glue_framed_enter();
std::string bb_glue_framed_leave();
std::string bb_glue_outer_γ();
std::string bb_glue_outer_ω();
std::string bb_glue_wire_exit(int is_gamma);
std::string bb_glue_wire_γ();
std::string bb_glue_wire_ω();
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int emit_diag_regs_suppress(void);
inline int x86_diag_regs_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DIAG_REGS"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" uint64_t * rt_port_counts_slot(int uid, int port, const char * label);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_portcount_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PORT_COUNTS"); v = (e && *e && *e != '0') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐ SLICE 4 ROUTE (a): EXACT per-box α/β execution counts in MODE 3. Global granted by Lon in-chat via CEO,
 * 2026-08-28, GOAL-CEO CEO-75. ⛔ ABSENT WHEN OFF -- with SCRIP_PORT_COUNTS unset this contributes not one byte,
 * so the default build is byte-identical to before (clause-10 control arm).
 *
 * ⛔⭐ THE SEQUENCE TOUCHES NO FLAGS, AND THAT IS THE WHOLE DESIGN, NOT A DETAIL. A port label is a JUMP TARGET: the
 * `jcc` that arrived set the flags, and code after the label may still read them. Every memory-increment instruction
 * x86 offers -- `inc`, `add` -- WRITES FLAGS, so the obvious `incq [cell]` would silently corrupt a conditional
 * downstream of any instrumented port, on an arm that is only ever on during measurement. That is the worst possible
 * failure shape: a bug that exists only while you are looking. `mov` and `lea` write no flags, so the read-modify-
 * write goes through `lea rcx,[rcx+1]` instead of `inc`, and the whole sequence is flag-transparent by construction.
 * ⚠️ It DOES cost two pushes; rsp is ζ-SPINE, so the pair is strictly balanced and nothing runs between them.
 *
 * ⛔ MODE-4 EMITS NOTHING HERE, DELIBERATELY, AND THIS IS THE ONE ASYMMETRY IN THIS FILE THAT IS ARGUED RATHER THAN
 * INHERITED: m4 ALREADY has exact per-port counts, from scripts/util_port_counts.py, which reads callgrind Ir at the
 * α/β symbols the linker already carries. An emitted counter in m4 would be strictly WORSE than the instrument that
 * already exists -- it perturbs the binary, needs the granted global reachable across a .so boundary, and buys
 * nothing. m3 has no such option: its boxes live in an anonymous sealed slab with no symbols at all. The medium
 * branch lives HERE, in the encoder, where medium-specific encoding belongs -- never in a bb_*.cpp (BOTH-MEDIUM
 * MANDATORY governs templates, and no template can see this). */
inline std::string x86_portcount(int port) {
    if (!x86_portcount_on() || (port != X86P_ALPHA && port != X86P_BETA) || !MEDIUM_BINARY) return std::string();
    uint64_t * cell = rt_port_counts_slot(_.x86_uid, port == X86P_ALPHA ? 0 : 1, x86_portname(port));
    if (!cell) return std::string();
    return x86("push", "rax") + x86("push", "rcx")
         + x86_movabs_r64("rax", (uint64_t)(uintptr_t)cell)
         + x86("mov", "rcx", RDQ("rax", 0))
         + x86("lea", "rcx", RDQ("rcx", 1))
         + x86("mov", RDQ("rax", 0), "rcx")
         + x86("pop", "rcx") + x86("pop", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_port_hook(int site, int port) {
    std::string s;
    if (site == X86H_DEF) s += x86_portcount(port);
    if (site == X86H_JMP) s += x86_port_canary();
    if (site == X86H_JMP && port == X86P_OMEGA && getenv("SCRIP_ZETA_OMEGA_TRACE"))
        fprintf(stderr, "[OMEGA-TRACE] x86_uid=%d op_omega_is_death=%s\n", _.x86_uid, _.op_omega_is_death ? "TRUE-DEATH" : "internal-alias");
    if (x86_port_mode() == ZC_PORT_ALLOC && _.op_zls2_ops && _.op_zls2_slot >= 0) {
        if (site == X86H_DEF && port == X86P_ALPHA && (_.op_zls2_ops & ZLS2_BUMP))
            s += x86_zls2_push_call(_.op_zls2_bytes)
               + x86("mov", "rcx", FRQ(_.op_zls2_slot))
               + x86("mov", RDQ("rax", 0), "rcx")
               + x86("mov", FRQ(_.op_zls2_slot), "rax");
        if (site == X86H_JMP && port == X86P_OMEGA && (_.op_zls2_ops & ZLS2_RELEASE))
            s += x86("mov", "rax", FRQ(_.op_zls2_slot))
               + x86("mov", "rcx", RDQ("rax", 0))
               + x86("mov", FRQ(_.op_zls2_slot), "rcx")
               + x86_zls2_release_to_reg("rax", _.op_zls2_bytes);
    }
    if ((x86_port_mode() == ZC_PORT_INLINE || x86_port_mode() == ZC_PORT_OWNED) && _.op_zls2_ops && _.op_zls2_slot >= 0) {
        if (site == X86H_DEF && port == X86P_ALPHA && (_.op_zls2_ops & ZLS2_BUMP))
            s += x86_zls2_cur_lea("rdi")
               + x86("mov", "rax", RDQ("rdi", 0))
               + x86_sub("rax", _.op_zls2_bytes)
               + x86("mov", RDQ("rdi", 0), "rax")
               + x86("mov", "rcx", FRQ(_.op_zls2_slot))
               + x86("mov", RDQ("rax", 0), "rcx")
               + x86("mov", FRQ(_.op_zls2_slot), "rax");
        if (site == X86H_JMP && port == X86P_OMEGA && (_.op_zls2_ops & ZLS2_RELEASE))
            s += x86("mov", "rax", FRQ(_.op_zls2_slot))
               + x86("mov", "rcx", RDQ("rax", 0))
               + x86("mov", FRQ(_.op_zls2_slot), "rcx")
               + x86_add("rax", _.op_zls2_bytes)
               + x86_zls2_cur_lea("rdi")
               + x86("mov", RDQ("rdi", 0), "rax");
    }
    if (x86_port_cstack() && _.op_zls2_ops && _.op_zls2_slot >= 0) {
        long k16 = (_.op_zls2_bytes + 15L) & ~15L;
        if (site == X86H_DEF && port == X86P_ALPHA && (_.op_zls2_ops & ZLS2_BUMP))
            s += x86_sub("rsp", k16)
               + x86("mov", "rcx", FRQ(_.op_zls2_slot))
               + x86("mov", RSP(0), "rcx")
               + x86("mov", FRQ(_.op_zls2_slot), "rsp");
        if (site == X86H_JMP && port == X86P_OMEGA && (_.op_zls2_ops & ZLS2_RELEASE))
            s += x86("mov", "rax", FRQ(_.op_zls2_slot))
               + x86("mov", "rcx", RDQ("rax", 0))
               + x86("mov", FRQ(_.op_zls2_slot), "rcx")
               + x86_reg_disp32_lea64("rsp", "rax", (int)k16);
    }
    if (x86_fc_on()) {
        int zwco = _.op_fc_base < 0 && !_.op_zres;
        if (site == X86H_DEF && port == X86P_ALPHA) s += bb_glue_flat_enter();
        if (zwco && site == X86H_DEF && port == X86P_BETA) s += bb_glue_flat_enter();
        if (zwco && site == X86H_DEF_PAIR && port == X86P_BETA && !_.op_pair_rejoin) s += bb_glue_flat_enter();
        if (zwco && site == X86H_JMP && port == X86P_GAMMA) s += bb_glue_flat_leave();
        if (site == X86H_JMP && port == X86P_OMEGA && !_.op_wsteal) s += bb_glue_flat_leave();
    }
    if (site == X86H_JMP && port == X86P_GAMMA && _.op_zgpop > 0) s += x86_add("rsp", (long)_.op_zgpop);
    if (site == X86H_JMP && port == X86P_OMEGA && _.op_wpop > 0) s += x86_add("rsp", (long)_.op_wpop);
    if (site == X86H_DEF && port == X86P_ALPHA && _.op_zls2_bytes > 0 && _.op_zls2_ops == 0 && x86_port_mode() == ZC_PORT_ALLOC)
        s += x86_sub(x86_zr(), _.op_zls2_bytes);
    if (site == X86H_DEF && port == X86P_ALPHA) {
        static int on = -1;
        if (on < 0) { const char *e = getenv("SCRIP_RBX_FIELD_TRACE"); on = (e && *e == '1') ? 1 : 0; }
        if (on) fprintf(stderr, "[RBX-FIELD] port=%d zls2_bytes=%ld zls2_ops=%ld fc_bytes=%ld fc_base=%ld\n",
                         x86_port_mode(), (long)_.op_zls2_bytes, (long)_.op_zls2_ops, (long)_.op_fc_bytes, (long)_.op_fc_base);
    }
    { long hk = (_.op_zls2_bytes > 0 && _.op_zls2_ops == 0) ? _.op_zls2_bytes : (_.op_fc_bytes > 0 ? (long)_.op_fc_bytes : 0L);
    if (site == X86H_DEF && port == X86P_ALPHA && hk > 0 && x86_port_mode() == ZC_PORT_HEAP)
        s += x86("mov", "rax", "rbx")
           + x86("add", "rbx", hk)
           + x86("cmp", "rbx", ABSQ(RT_WS_LIMIT))
           + x86("ja",  L(60))
           + x86("jmp", L(61))
           + x86("def", L(60))
           + x86_align_enter()
           + x86("mov", "edi", hk)
           + x86("call", "rt_zh_bump_slow", (uint64_t)(uintptr_t)(void *)rt_zh_bump_slow_addr())
           + x86_align_leave()
           + x86("mov", "rbx", ABSQ(RT_WS_TOP))
           + x86("def", L(61)); }
    if (x86_port_mode() == ZC_PORT_OWNED && (site == X86H_DEF || site == X86H_DEF_PAIR) && (port == X86P_ALPHA || port == X86P_BETA) && _.op_own_mark >= 0 && _.op_own_ci > 0)
        s += x86_zls2_cur_lea("rdi")
           + x86("mov", "rax", RDQ("rdi", 0))
           + x86("mov", "rcx", FRQ(_.op_own_mark))
           + x86_sub("rcx", (long)_.op_own_ci)
           + x86_own_floor_store();
    if (x86_diag_regs_on() && (site == X86H_DEF || site == X86H_DEF_PAIR) && (port == X86P_ALPHA || port == X86P_BETA) && !emit_diag_regs_suppress())
        s += x86("mov", "r11", (long)_.nid);
    if (x86_selfload_mode() == ZC_SELFLOAD_ALLOC && _.op_selfload) {
        if (site == X86H_DEF && port == X86P_ALPHA && _.op_selfload == 1)
            s += x86_align_enter()
               + x86("mov",  "rdi", 4096L)
               + x86("call", "rt_zls_alloc", (uint64_t)(uintptr_t)(void *)(void * (*)(long))rt_zls_alloc)
               + x86("mov",  "rdi", "rax")
               + x86("call", "rt_zls_arbno_step1_store", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_arbno_step1_store)
               + x86_align_leave();
        if (site == X86H_JMP && port == X86P_OMEGA && _.op_selfload == 2)
            s += x86("call", "rt_zls_arbno_step1_load", (uint64_t)(uintptr_t)(void *)(void * (*)(void))rt_zls_arbno_step1_load)
               + x86_align_enter()
               + x86("mov",  "rdi", "rax")
               + x86("call", "rt_zls_release", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_release)
               + x86_align_leave();
    }
    return s;
}
extern "C" std::string bb_zdp_probe(long op, long node, long port, long expect, long want_rbp);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_zdp_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZDP_TEARDOWN"); v = (e && *e == '1') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zdp_probe_at(int port) {
    if (!x86_zdp_on()) return std::string();
    if (port != X86P_ALPHA && port != X86P_BETA) return std::string();
    long expect = (port == X86P_ALPHA) ? (long)_.op_zdp_ad : (long)_.op_zdp_bd;
    long want_rbp = _.op_zdp_rbp ? ((port == X86P_BETA) ? 1L : 2L) : 0L;
    return bb_zdp_probe((long)_.op_node_kind, (long)_.nid, (port == X86P_ALPHA) ? 1L : 2L, expect, want_rbp);
}
extern "C" void rt_zdp_ev(void);
extern "C" int emit_match_rbp(void);
extern "C" int emit_defer_rbp(void);
extern "C" int emit_defer_carve_rbp(void);
extern "C" void rt_zdp_sm_init(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int icn_genframe2() {   /* N-2 (hq_P s271): the generator ACTIVATION-FRAME protocol. Lives HERE, not in emit.cpp, because the alpha/gamma/res/omega arms are emitted from the driver while the caller-side landing is emitted from a bb_* template into libscrip_rt.so -- one switch, two link units, so it has to sit in the header both sides already include. ⛔ DEFAULT OFF until all five slices land and the D2-suspend witness set is green: alpha carve, gamma resume-record, res landing, omega retire and the caller landing are ONE protocol, and a half-built one crashes differently rather than better. SCRIP_ICN_GENFRAME2=1 arms it. */
    static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ICN_GENFRAME2"); v = (e && *e == '1') ? 1 : 0; } return v;
}
inline int x86_zdp_rbp_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM"); v = (e && *e == '1') ? 1 : 0; if (v) rt_zdp_sm_init(); } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_zdp_rbp_frames() {
    if (!_.op_zdp_rbp) return 0;
    if (_.op_node_kind == (int)IR_MATCH_BEGIN) return emit_match_rbp();
    if (_.op_node_kind == (int)IR_MATCH_DEFER) return (((_.op_seal == 1) || emit_defer_carve_rbp()) && x86_port_cstack() && emit_defer_rbp()) ? 1 : 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zsm_ev(int kind) {
    return x86("comment", "ZSM")
         + x86("push", "rax") + x86("push", "rax")
         + x86("push", "rdi") + x86("push", "rsi") + x86("push", "rdx") + x86("push", "rcx")
         + x86("push", "r8")  + x86("push", "r9")  + x86("push", "r10") + x86("push", "r11")
         + x86("mov",  "rdx", "rsp")
         + x86("add",  "rdx", 80L)
         + x86("mov",  "rdi", (long)_.nid)
         + x86("mov",  "rsi", "rbp")
         + x86("mov",  "rcx", (long)(kind | ((long)_.op_node_kind << 8)))
         + x86("mov",  "r8", (x86_rsp_slide_known() && !_.flat_jmp_entry) ? (long)_.op_zdepth : -1L)
         + x86("call_bare", "rt_zdp_ev", (uint64_t)(uintptr_t)(void *)rt_zdp_ev)
         + x86("pop",  "r11") + x86("pop", "r10") + x86("pop", "r9")  + x86("pop", "r8")
         + x86("pop",  "rcx") + x86("pop", "rdx") + x86("pop", "rsi") + x86("pop", "rdi")
         + x86("pop",  "rax") + x86("pop", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_zsm_all() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ZSM_ALL"); v = (e && *e == '1') ? 1 : 0; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zdp_rbp_at(int port) {
    if (!x86_zdp_rbp_on()) return std::string();
    if (x86_zdp_rbp_frames()) { if (port == X86P_ALPHA) return x86_zsm_ev(1); if (port == X86P_BETA) return x86_zsm_ev(2); return std::string(); }
    if (!x86_zsm_all()) return std::string();
    if (port == X86P_ALPHA) return x86_zsm_ev(5);
    if (port == X86P_BETA)  return x86_zsm_ev(6);
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zdp_rbp_omega_at(int port) {
    if (!x86_zdp_rbp_on()) return std::string();
    if (port != X86P_OMEGA) return std::string();
    if (x86_zdp_rbp_frames()) return x86_zsm_ev(3);
    if (x86_zsm_all()) return x86_zsm_ev(7);
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zdp_rbp_gamma_at(int port) {
    if (!x86_zdp_rbp_on()) return std::string();
    if (port != X86P_GAMMA) return std::string();
    if (x86_zdp_rbp_frames()) return x86_zsm_ev(4);
    if (x86_zsm_all()) return x86_zsm_ev(8);
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zeta_mark_call(int off) {
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string();
    return x86("push", "rsi")
         + x86_align_enter()
         + x86("call", "rt_zls_mark", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_zls_mark)
         + x86_align_leave()
         + x86("mov",  FRQ(off), "rax")
         + x86("pop",  "rsi");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zeta_release_to_call(int off) {
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string();
    return x86_align_enter()
         + x86("mov",  "rdi", FRQ(off))
         + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_release_to)
         + x86_align_leave();
}
extern "C" void rt_bomb(const char * msg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_bomb(const char * msg) {
    const char * m   = msg ? msg : "(unimplemented box)";
    const char * lbl = emit_intern_str(m);
    char lblbuf[24];
    if (!MEDIUM_BINARY && (!lbl || !lbl[0])) { strtab_label(lblbuf, sizeof lblbuf, m); lbl = lblbuf; }
    uint64_t     fp  = (uint64_t)(uintptr_t)(void *)rt_bomb;
    return x86_load_ro("rdi", lbl, (uint64_t)(uintptr_t)(const void *)m)
         + x86_call_ro("rt_bomb", fp)
         + (MEDIUM_BINARY ? x86_Lrec(x86_b2(0x0F, 0x0B)) : x86_recn("ud2") + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_fn_body(const char * label, uint64_t fp) {
    if (MEDIUM_BINARY) return fp ? x86_jmpfn(label, fp) : x86_bomb("bb_define_activate: proc fn not registered for binary body-jmp");
    return x86_jmp_lblptr(emit_label_intern(label), label);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_through_fn_cell(const char * label, uint64_t cell) {
    return (MEDIUM_BINARY ? x86("movabs", "rax", cell) : x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)0, label))
         + x86("mov", "rax", RDQ("rax", 0)) + x86("jmp", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline bb_label_t * x86_pair_tgt(int idx) { return bb_label_fold(g_emit.xa_bb_emit_pair_jmp[idx] ? g_emit.xa_bb_emit_pair_jmp[idx] : g_emit.xa_bb_emit_pair_define[idx]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_pair_jmp(int idx) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += (char)(unsigned char)idx; return r; }
    return x86_rec("jmp") + (x86_pair_tgt(idx) ? x86_pair_tgt(idx)->name : "??") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_pair_loop() {
    std::string r;
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
        if (MEDIUM_BINARY) {
            if (g_emit.xa_bb_emit_pair_define[i]) { r += (char)'E'; r += (char)(unsigned char)i; r += x86_port_canary(); r += x86_port_hook(X86H_DEF, X86P_BETA); }
            if (g_emit.xa_bb_emit_pair_jmp[i])    { r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += (char)(unsigned char)i; }
        } else {
            if (g_emit.xa_bb_emit_pair_define[i]) { r += emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name); r += x86_port_canary(); r += x86_port_hook(X86H_DEF, X86P_BETA); }
            if (g_emit.xa_bb_emit_pair_jmp[i])    r += x86_rec("jmp") + g_emit.xa_bb_emit_pair_jmp[i]->name + "\n";
        }
    }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_deflabel_pair(int idx) {
    if (MEDIUM_BINARY) { std::string r; r += (char)'E'; r += (char)(unsigned char)idx; r += x86_port_canary(); r += x86_port_hook(X86H_DEF_PAIR, X86P_BETA); return r; }
    return emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[idx] ? g_emit.xa_bb_emit_pair_define[idx]->name : "??") + x86_port_canary() + x86_port_hook(X86H_DEF_PAIR, X86P_BETA);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_pair(int idx) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += (char)(unsigned char)idx; return r; }
    return x86_rec("jmp") + (x86_pair_tgt(idx) ? x86_pair_tgt(idx)->name : "??") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jcc_pair(const char * mnem, int idx) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))); r += (char)'F'; r += (char)(unsigned char)idx; return r; }
    return x86_rec(mnem) + (x86_pair_tgt(idx) ? x86_pair_tgt(idx)->name : "??") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_lit_bytes(const std::string & b) {
    if (!MEDIUM_BINARY) return b;
    std::string r; size_t i = 0;
    while (i < b.size()) { size_t k = b.size()-i; if (k > 255) k = 255; r += x86_Lrec(b.substr(i,k)); i += k; }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline struct bb_label_t * x86_label_for(int id, bb_label_t * internal) {
    if (id == X86T_TGT0) return _.lbl_t0_p;
    if (id == X86T_TGT1) return _.lbl_t1_p;
    if (id < X86_INTERNAL_BASE) return x86_portlbl(id);
    if (id - X86_INTERNAL_BASE >= X86_INTERNAL_MAX) { fprintf(stderr, "FATAL bb_emit_x86: record label id %d exceeds internal[%d] -- refusing the out-of-bounds stack write\n", id, X86_INTERNAL_MAX); abort(); }
    { bb_label_t * l = &internal[id - X86_INTERNAL_BASE]; if (l->name[0] == '\0') snprintf(l->name, BB_LABEL_NAME_MAX, ".Lxi%d", id - X86_INTERNAL_BASE); return l; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rtcc_call_descr(const char * sym, uint64_t ptr, int slot) {
    unsigned m = x86_rtcc_clob(sym);
    if (m == 0) return x86_call_ro(sym, ptr) + x86("mov", FRQ(slot), "rax") + x86("mov", FRQ(slot + 8), "rdx");
    uint64_t block = (uint64_t)(uintptr_t)rtccb;
    std::string cap = x86("mov", FRQ(slot), "rax") + x86("mov", FRQ(slot + 8), "rdx");
    if (MEDIUM_BINARY) {
        std::string call_b;
        call_b += (char)0x48; call_b += (char)0xB8; call_b += u64le(ptr);
        call_b += (char)0xFF; call_b += (char)0xD0;
        return x86_align_assert() + x86_Lrec(x86_rtcc_wb_bin(block, m)) + x86_Lrec(call_b) + cap + x86_Lrec(x86_rtcc_rl_bin(block, m));
    }
    return x86_align_assert() + x86_rtcc_wb_text(m) + x86_rec("call") + sym + "@PLT\n" + cap + x86_rtcc_rl_text(m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rtcc_call_descr_ops(const char * sym, uint64_t ptr, const std::string & r0, const std::string & r8) {
    std::string cap = x86("mov", r0.c_str(), "rax") + x86("mov", r8.c_str(), "rdx");
    unsigned m = x86_rtcc_clob(sym);
    if (m == 0) return x86_call_ro(sym, ptr) + cap;
    uint64_t block = (uint64_t)(uintptr_t)rtccb;
    if (MEDIUM_BINARY) {
        std::string call_b;
        call_b += (char)0x48; call_b += (char)0xB8; call_b += u64le(ptr);
        call_b += (char)0xFF; call_b += (char)0xD0;
        return x86_align_assert() + x86_Lrec(x86_rtcc_wb_bin(block, m)) + x86_Lrec(call_b) + cap + x86_Lrec(x86_rtcc_rl_bin(block, m));
    }
    return x86_align_assert() + x86_rtcc_wb_text(m) + x86_rec("call") + sym + "@PLT\n" + cap + x86_rtcc_rl_text(m);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⛔⛔ THE PORT MARKER IS IN-BAND, AND \x01 IS NOT A RESERVED BYTE IN EMITTED TEXT.  a01fe9f6 prepends
   '\x01' + ('0'+port) to a port-label line and x86_internal_resolve() consumed ANY '\x01' it met, plus the
   byte after it.  Emitted `.string` DATA legitimately contains \x01 -- Pascal record layout descriptors do --
   so the resolver silently DELETED TWO BYTES OF THE PROGRAM'S OWN DATA.  Measured: rec1.pas emitted
   `.string "0\x010"` before the commit and `.string "0"` after it, and the program printed 4 instead of 7.
   That is why the damage is m4-only (this resolver runs in TEXT medium only, never BINARY), why the
   record/variant-record/nested-proc families were hit hardest (they are the ones emitting \x01 in data), and
   why it reads as `output mismatch` rather than a crash: nothing is malformed, the data is just wrong.
   ⭐ THE DISCRIMINATOR IS WHERE THE MARKER CAN OCCUR, NOT WHAT IT LOOKS LIKE.  A marker is only ever emitted
   at the START of a port-label line, so it can only appear at offset 0 or immediately after a newline; a data
   \x01 always sits mid-line, inside a quoted operand.  Requiring line-start AND a valid port digit separates
   them exactly, and a \x01 that fails the test now passes through VERBATIM instead of being eaten.
   ⛔ This is a narrowing, not a redesign: in-band signalling over a channel that does not reserve the signal
   byte stays fragile by construction, and the durable fix is to carry port transitions OUT of band. Named for
   whoever owns the label-prefix design; this restores correctness without rewriting their commit. */
#define X86_IS_PORTMARK(s_, i_, n_)  ((s_)[i_] == '\x01' && (i_) + 1 < (n_) && ((i_) == 0 || (s_)[(i_) - 1] == '\n') && (s_)[(i_) + 1] >= '0' && (s_)[(i_) + 1] <= '3')
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_ir_digit(char c) { return c >= '0' && c <= '9'; }
inline long x86_ir_num(const std::string & s, size_t & i) { long v = 0; size_t n = s.size(); while (i < n && x86_ir_digit(s[i])) v = v * 10 + (s[i++] - '0'); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_internal_resolve(const std::string & s) {
    static const char * const x86_greek[4] = { "\xce\xb1", "\xce\xb2", "\xce\xb3", "\xcf\x89" };
    const char * fam = _.flat_fam ? _.flat_fam : "anon";
    int portof[X86_INTERNAL_MAX]; for (int k = 0; k < X86_INTERNAL_MAX; k++) portof[k] = -1;
    int cur = X86P_ALPHA; size_t n = s.size();
    for (size_t i = 0; i < n; ) {
        if (X86_IS_PORTMARK(s, i, n)) { cur = s[i + 1] - '0'; i += 2; continue; }
        if (i + 3 < n && s[i] == '.' && s[i + 1] == 'L' && s[i + 2] == 'x' && x86_ir_digit(s[i + 3])) {
            size_t j = i + 3; x86_ir_num(s, j);
            if (j < n && s[j] == '_' && j + 1 < n && x86_ir_digit(s[j + 1])) {
                size_t k = j + 1; long ln = x86_ir_num(s, k); if (ln < X86_INTERNAL_MAX && portof[ln] < 0) portof[ln] = cur; i = k; continue;
            }
        }
        i++;
    }
    std::string out; out.reserve(n + 32); cur = X86P_ALPHA;
    for (size_t i = 0; i < n; ) {
        if (X86_IS_PORTMARK(s, i, n)) { cur = s[i + 1] - '0'; i += 2; continue; }
        if (i + 3 < n && s[i] == '.' && s[i + 1] == 'L' && s[i + 2] == 'x' && x86_ir_digit(s[i + 3])) {
            size_t j = i + 3; long uid = x86_ir_num(s, j);
            if (j < n && s[j] == '_' && j + 1 < n && x86_ir_digit(s[j + 1])) {
                size_t k = j + 1; long ln = x86_ir_num(s, k); int p = (ln < X86_INTERNAL_MAX && portof[ln] >= 0) ? portof[ln] : X86P_ALPHA;
                out += ".L"; out += fam; out += "_"; out += x86_greek[p]; out += "_"; out += std::to_string(uid); out += "_"; out += std::to_string(ln); i = k; continue;
            }
        }
        out += s[i++];
    }
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline void bb_emit_x86(const std::string & s) {
    if (!MEDIUM_BINARY) { if (!s.empty()) { std::string r = x86_internal_resolve(s); emit_text_n(r.data(), r.size()); } return; }
    bb_label_t internal[X86_INTERNAL_MAX];
    for (int k = 0; k < X86_INTERNAL_MAX; k++) { internal[k].offset = BB_LABEL_UNRESOLVED; internal[k].name[0] = '\0'; }
    size_t i = 0, n = s.size();
    while (i < n) {
        char tag = s[i++];
        if (tag == 'L') { int k = (unsigned char)s[i++]; for (int j = 0; j < k; j++) bb_emit_byte((uint8_t)(unsigned char)s[i++]); }
        else if (tag == 'J') { int id = (unsigned char)s[i++]; bb_emit_patch_rel32(x86_label_for(id, internal)); }
        else if (tag == 'D') { int id = (unsigned char)s[i++]; bb_label_define(x86_label_for(id, internal)); }
        else if (tag == 'Q') { int id = (unsigned char)s[i++]; bb_emit_patch_abs64(x86_label_for(id, internal)); }
        else if (tag == 'E') { int idx = (unsigned char)s[i++]; if (g_emit.xa_bb_emit_pair_define[idx]) bb_label_define(g_emit.xa_bb_emit_pair_define[idx]); }
        else if (tag == 'F') { int idx = (unsigned char)s[i++]; bb_label_t * _t = x86_pair_tgt(idx); if (_t) bb_emit_patch_rel32(_t); }
        else if (tag == 'X') { uint64_t v = 0; for (int j = 0; j < 8; j++) v |= ((uint64_t)(unsigned char)s[i++]) << (8 * j); bb_emit_patch_rel32((bb_label_t *)(uintptr_t)v); }
        else if (tag == 'Y') { uint64_t v = 0; for (int j = 0; j < 8; j++) v |= ((uint64_t)(unsigned char)s[i++]) << (8 * j); bb_label_define((bb_label_t *)(uintptr_t)v); }
        else break;
    }
}
#endif
