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
extern uint64_t g_rtcc_block[32];   /* RC-2: RTCC block base; slot layout per rtcc.h (R8=5,R9=6,R10=7,R11=8) */
extern unsigned char g_rtcc_on;     /* RC-2: killswitch gate — 1=ON(default, s13), 0=OFF(SCRIP_RTCC=0, emergency bisect only) */
long *rt_anchor_ptr(void);         /* RC-5: C linkage declared here so the local use in rtcc_anchor_cmp gets C linkage */
}
/* RC-5 (s11): RTCC_SLOT_R8/R9, RTCC_GLOBAL_R8_ANCHOR, RTCC_GLOBAL_R9_GVA and RTCC_GVA_REG were DUPLICATED here
   and in rtx/rtcc.h, both unguarded, so no -D could override either and gcc's "redefined" warning was eaten by
   the tree-wide -w.  That silently VOIDED two graded rungs: RC-5-GVA was RETAINED on a 1.036x "rail" and
   RC-5-ANCHOR was REVERTED on a 1.000x "rail", when in both cases the two arms were the SAME BINARY.  One
   source of truth now; a killswitch flip in rtcc.h reaches the emitter, which is what makes those rungs
   re-gradeable at all.  Values were verified identical across both copies before deletion. */
#include "rtx/rtcc.h"
#ifndef _
#define _ g_emit
#endif
extern "C" {
}
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
inline std::string x86_alu_rr(const char * mnem, uint8_t op, const char * rm, const char * reg) {
    int m = x86_rnum(rm), g = x86_rnum(reg);
    uint8_t rex = 0x40; if (x86_is64(rm) || x86_is64(reg)) rex |= 0x08; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; if (rex != 0x40) code += (char)rex; code += (char)op; code += (char)(0xC0 | ((g & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" ") + mnem + " " + rm + ", " + reg + "\n");
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
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" imul ") + dst + ", " + src + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_cqo() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b2(0x48, 0x99)) : std::string(" cqo\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rep_stosb() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b2(0xF3, 0xAA)) : std::string(" rep stosb\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_xorps_xmm0() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b3(0x0F, 0x57, 0xC0)) : std::string(" xorps xmm0, xmm0\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movabs_r64(const char * dst, uint64_t imm) {
    int m = x86_rnum(dst);
    if (MEDIUM_BINARY) { std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le(imm); return x86_Lrec(code); }
    return std::string(" movabs ") + dst + ", " + std::to_string((unsigned long long)imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movq_xmm0_r64(const char * src) {
    int m = x86_rnum(src); uint8_t rex = 0x48; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)0x66; code += (char)rex; code += (char)0x0F; code += (char)0x6E; code += (char)(0xC0 | (0 << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" movq xmm0, ") + src + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movq_xmm_r64(const char * dst, const char * src) {
    int xn = (dst && !strncmp(dst, "xmm", 3)) ? atoi(dst + 3) : 0; int m = x86_rnum(src); uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; if (xn >= 8) rex |= 0x04;
    std::string code; code += (char)0x66; code += (char)rex; code += (char)0x0F; code += (char)0x6E; code += (char)(0xC0 | ((xn & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" movq ") + dst + ", " + src + "\n");
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
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" idiv ") + reg + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_neg(const char * reg) {
    int m = x86_rnum(reg); uint8_t rex = 0x48; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0xF7; code += (char)(0xC0 | (3 << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" neg ") + reg + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_add(const char * reg, long imm) {
    int m = x86_rnum(reg); bool w = x86_is64(reg);
    std::string code;
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (m >= 8) rex |= 0x01;
    if (imm >= -128 && imm <= 127) { if (rex != 0x40) code += (char)rex; code += (char)0x83; code += (char)(0xC0 | (0 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else if (m == 0 && !w)         { code += (char)0x05; code += u32le((uint32_t)imm); }
    else                           { if (rex != 0x40) code += (char)rex; code += (char)0x81; code += (char)(0xC0 | (0 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" add ") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_sub(const char * reg, long imm) {
    int m = x86_rnum(reg); bool w = x86_is64(reg);
    std::string code;
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (m >= 8) rex |= 0x01; if (rex != 0x40) code += (char)rex;
    if (imm >= -128 && imm <= 127) { code += (char)0x83; code += (char)(0xC0 | (5 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else                           { code += (char)0x81; code += (char)(0xC0 | (5 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" sub ") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movsxd(const char * dst64, const char * src32) {
    int g = x86_rnum(dst64), m = x86_rnum(src32);
    uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0x63; code += (char)(0xC0 | ((g & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" movsxd ") + dst64 + ", " + src32 + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_lea_subj_cursor(const char * dst) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x48 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x8D; code += (char)modrm; code += (char)sib; code += (char)0x00;
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" lea ") + dst + ", [r13 + rcx]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movzx_subj_byte(const char * dst, int disp) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x40 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x0F; code += (char)0xB6; code += (char)modrm; code += (char)sib; code += (char)(disp & 0xFF);
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" movzx ") + dst + ", byte ptr [r13+rcx" + (disp ? std::string("+") + std::to_string(disp) : std::string()) + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movzx_bir(const char * dst, const char * base, const char * idx) {
    int g = x86_rnum(dst), bn = x86_rnum(base), in = x86_rnum(idx);
    uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (in >= 8) rex |= 0x02; if (bn >= 8) rex |= 0x01;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | ((in & 7) << 3) | (bn & 7));
    std::string code; code += (char)rex; code += (char)0x0F; code += (char)0xB6; code += (char)modrm; code += (char)sib; code += (char)0x00;
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" movzx ") + dst + ", byte ptr [" + base + " + " + idx + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_mov_subj_q(const char * dst, int disp) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x48 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x8B; code += (char)modrm; code += (char)sib; code += (char)(disp & 0xFF);
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" mov ") + dst + ", qword ptr [r13+rcx" + (disp ? std::string("+") + std::to_string(disp) : std::string()) + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_mov_subj_d(const char * dst, int disp) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x41; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x8B; code += (char)modrm; code += (char)sib; code += (char)(disp & 0xFF);
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" mov ") + dst + ", dword ptr [r13+rcx" + (disp ? std::string("+") + std::to_string(disp) : std::string()) + "]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_cset_probe() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b4(0x80, 0x3C, 0x37, 0x00)) : std::string(" cmp byte ptr [rdi+rsi], 0\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_push(const char * r) {
    int m = x86_rnum(r); std::string code; if (m >= 8) code += (char)0x41; code += (char)(0x50 | (m & 7)); return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" push ") + r + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_pop (const char * r) {
    int m = x86_rnum(r); std::string code; if (m >= 8) code += (char)0x41; code += (char)(0x58 | (m & 7)); return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" pop ")  + r + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movimm(const char * dst, long imm) {
    int m = x86_rnum(dst);
    if (MEDIUM_BINARY) { std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le((uint64_t)imm); return x86_Lrec(code); }
    return std::string(" mov ") + dst + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_movimm32(const char * dst, long imm) {
    int m = x86_rnum(dst);
    if (MEDIUM_BINARY) { std::string code; if (m >= 8) code += (char)0x41; code += (char)(0xB8 | (m & 7)); code += u32le((uint32_t)imm); return x86_Lrec(code); }
    return std::string(" mov ") + dst + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_load_ro(const char * dst, const char * label, uint64_t ptr) {
    if (MEDIUM_BINARY) {
        int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le(ptr); return x86_Lrec(code);
    }
    return std::string(" lea ") + dst + ", [rip + " + (label ? label : "??") + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_load_got(const char * dst, const char * label, uint64_t ptr) {
    if (MEDIUM_BINARY) {
        int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le(ptr); return x86_Lrec(code);
    }
    return std::string(" mov ") + dst + ", qword ptr [rip + " + (label ? label : "??") + "@GOTPCREL]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_align_assert();
inline std::string x86_call_ro(const char * sym, uint64_t ptr) {
    if (MEDIUM_BINARY) { std::string code; code += (char)0x48; code += (char)0xB8; code += u64le(ptr); code += (char)0xFF; code += (char)0xD0; return x86_align_assert() + x86_Lrec(code); }
    return x86_align_assert() + std::string(" call ") + sym + "@PLT\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_rtcc_writeback / x86_rtcc_reload — RC-4 FULL 9-GPR block I/O helpers (BINARY + TEXT).                                                                                                        */
/* WRITEBACK order: push r11 (saves it to stack), movabs r11,block, store RAX/RCX/RDX/RSI/RDI/R8/R9/R10        */
/* via r11, then pop [r11+64] to capture old r11 into its slot — net cost: 1 push + 1 movabs + 8 stores.        */
/* RELOAD order: movabs r11,block, restore RAX/RCX/RDX/RSI/RDI/R8/R9/R10 via r11, restore r11 last — so r11    */
/* is valid as the block base for all restores.  r11 restored LAST (its slot = offset 64).                       */
/* SLOT OFFSETS (8B each, matches rtcc.h): RAX=0, RCX=8, RDX=16, RSI=24, RDI=32, R8=40, R9=48, R10=56, R11=64 */
/* BINARY call stub: movabs r10, ptr; call r10 — R10 is already written-back so it is free at this point.        */
/* TEXT call stub: call sym@PLT (gas handles PLT indirection).                                                    */
/* RETURN-BEFORE-RELOAD LAW (RC-4, per DESIGN OF RECORD): for DESCR_t-returning calls, the caller captures      */
/* RAX:RDX to the destination frame slot BEFORE the reload — otherwise reload overwrites RAX:RDX with block      */
/* values (which are stale VM globals, not the return value).  x86_rtcc_call_descr handles this case.            */
/* x86_rtcc_call handles void/int/ptr-returning calls where RAX need not be captured before reload.              */
/* ⛔ H2 — THE R9/GVA SLOT IS NOT WRITTEN BACK (s8 2026-08-10).  rtcc_init.c seeds slot 6 with RT_GVA_VA once and    */
/* documents it as a BLOCK-CANONICAL EXCEPTION needing "no companion writes anywhere".  This writeback WAS a companion */
/* write: it stored r9 on EVERY crossing, so the exception silently held only while nothing ever clobbered r9.  When   */
/* something did (bb_func_activate movzx r9,cl) the clobbered value entered the canonical slot and the reload spread   */
/* it -- RT_GVA_VA dead process-wide, every [r9+k*16] near-null.  PROVEN: AB=1 RTCC=1 fibonacci SIGSEGV.  Skipping the */
/* store makes the documented exception TRUE: r9 is a read-only cache of a constant, the reload always restores the    */
/* pristine seed, and a template clobber of r9 becomes SELF-HEALING at the next crossing instead of fatal.             */
/* x86_rtcc_wb_bin — BINARY: writeback all 9 GPRs to g_rtcc_block WITHOUT touching RSP.                         */
/* RSP-SAFETY LAW: the veneer fires inside templates that may have live ζ cells on RSP.  NO push/pop allowed.    */
/* APPROACH: use the REX.W MOV-moffs-rax encoding (48 A3 addr64) to store RAX directly to its slot by absolute  */
/* address — no base register needed.  Then use RAX as the block pointer for the remaining 8 stores.             */
/* After all stores, RAX is left holding the block address (its slot already holds the correct original value).   */
/* The reload restores RAX from slot 0 last — but reload uses R11 as base, so RAX can be restored anytime.       */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⛔ RTCC ABI SEAL (s16) — THE SLOT CONSTANTS ARE ABI, NOT KNOBS.  The encoders below spell every slot as a RAW LITERAL (8,16,24,32,40,48,56,64); rtcc.h spells the same slots as RTCC_SLOT_* for the  */
/* C runtime (rtcc_init seed, keywords.c ANCHOR companion, rtcc_load_scratch).  NOTHING joined the two halves but these assertions.  PROVEN s16 by probe: #ifndef-guarding RTCC_SLOT_R9 makes            */
/* -DRTCC_SLOT_R9=7 move the C seed to block[7] while these literals keep addressing block+48 — C and generated code then disagree about where RT_GVA_VA lives, which is exactly the H2 SIGSEGV class     */
/* documented above, MANUFACTURED by the guard that was meant to prevent a silent -D.  ⛔ DO NOT #ifndef-GUARD RTCC_SLOT_* OR RTCC_GVA_REG.  Guarding is correct ONLY for the two KILLSWITCHES            */
/* (RTCC_GLOBAL_R8_ANCHOR / RTCC_GLOBAL_R9_GVA): those are read by BOTH halves, so a -D reaches the emitted bytes and the flip stays coherent — which is why they are guarded and these are sealed.       */
/* A macro edit that drifts from the literals is now a BUILD ERROR, never a silent miscompile.  static_assert emits ZERO bytes: the killswitch md5 is unchanged by this block, by construction.           */
constexpr bool x86_rtcc_streq(const char * a, const char * b) { return *a == *b && (*a == '\0' ? true : x86_rtcc_streq(a + 1, b + 1)); }
static_assert(RTCC_SLOT_RAX * 8 ==  0, "RTCC ABI drift: RTCC_SLOT_RAX no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RCX * 8 ==  8, "RTCC ABI drift: RTCC_SLOT_RCX no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RDX * 8 == 16, "RTCC ABI drift: RTCC_SLOT_RDX no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RSI * 8 == 24, "RTCC ABI drift: RTCC_SLOT_RSI no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RDI * 8 == 32, "RTCC ABI drift: RTCC_SLOT_RDI no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_R8  * 8 == 40, "RTCC ABI drift: RTCC_SLOT_R8 no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin (and keywords.c ANCHOR companion write)");
static_assert(RTCC_SLOT_R9  * 8 == 48, "RTCC ABI drift: RTCC_SLOT_R9 no longer matches the literal offset in x86_rtcc_rl_bin (and the rtcc_init RT_GVA_VA seed) — this is the H2 SIGSEGV class");
static_assert(RTCC_SLOT_R10 * 8 == 56, "RTCC ABI drift: RTCC_SLOT_R10 no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_R11 * 8 == 64, "RTCC ABI drift: RTCC_SLOT_R11 no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(x86_rtcc_streq(RTCC_GVA_REG, "r9"), "RTCC ABI drift: RTCC_GVA_REG no longer names the register the reload encoders load from slot 6 (mov r9,[r11+48]) — GVARQ would address a register the veneer never seeds");
static_assert(RTCC_GPR_COUNT == 9 && RTCC_GPR_BYTES == 72, "RTCC ABI drift: GPR tier width no longer matches the 9 slots the encoders write back and reload");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline std::string x86_rtcc_wb_bin(uint64_t block) {
    std::string wb;
    /* RC-8b (this session, FINDING pending): the ARG TIER {RAX,RCX,RDX,RSI,RDI} writeback is confirmed dead --
     * exhaustive codebase search (x86_asm.h's own reload, rtcc_load_scratch, keywords.c's ANCHOR companion,
     * and the hand-written inline-asm reload sites in rt.c/runtime_eval.c) shows every single reader of
     * g_rtcc_block touches ONLY offsets 40+ (the scratch tier R8/R9/R10/R11); offsets 0/8/16/24/32 are never
     * read back by anything.  Dropped the 5 arg-tier stores (RAX's own slot-0 save included -- nothing reads
     * slot 0 either).  RAX still loads the block address: it remains the base register for the scratch-tier
     * stores below, that role is independent of whether RAX's OWN pre-call value gets snapshotted anywhere. */
    wb += (char)0x48; wb += (char)0xB8; wb += u64le(block);                      /* movabs rax, block */
    wb += (char)0x4C; wb += (char)0x89; wb += (char)0x40; wb += (char)40;         /* mov [rax+40], r8   (R8  slot 5) */
    if (!RTCC_GLOBAL_R9_GVA) { wb += (char)0x4C; wb += (char)0x89; wb += (char)0x48; wb += (char)48; }   /* mov [rax+48], r9 (R9 slot 6) -- SKIPPED under the GVA claim: see H2 note above x86_rtcc_wb_bin */
    wb += (char)0x4C; wb += (char)0x89; wb += (char)0x50; wb += (char)56;         /* mov [rax+56], r10  (R10 slot 7) */
    wb += (char)0x4C; wb += (char)0x89; wb += (char)0x58; wb += (char)64;         /* mov [rax+64], r11  (R11 slot 8) */
    /* RAX left = block pointer.  Caller uses RAX for indirect call stub. */
    return wb;
}
/* x86_rtcc_rl_bin — BINARY: reload GPRs from g_rtcc_block.                                                      */
/* RC-4 PARTIAL RELOAD: only the SCRATCH TIER {R8, R9, R10, R11} is restored from the block.                   */
/* The ARG TIER {RAX, RCX, RDX, RSI, RDI} reload is DEFERRED to RC-5: until a VM global is assigned to those  */
/* slots, the block values are zero (BSS) and restoring zero would corrupt the call return value in RAX/RDX.   */
/* Consequence: templates that read RAX/RDX after a call still see the call's return value — correct.          */
/* R11 is restored last (it is the block pointer during the reload; overwritten when we load r11 from slot 8). */
static inline std::string x86_rtcc_rl_bin(uint64_t block) {
    std::string rl;
    rl += (char)0x49; rl += (char)0xBB; rl += u64le(block);                     /* movabs r11, block */
    rl += (char)0x4D; rl += (char)0x8B; rl += (char)0x43; rl += (char)40;        /* mov r8,  [r11+40]  */
    rl += (char)0x4D; rl += (char)0x8B; rl += (char)0x4B; rl += (char)48;        /* mov r9,  [r11+48]  */
    rl += (char)0x4D; rl += (char)0x8B; rl += (char)0x53; rl += (char)56;        /* mov r10, [r11+56]  */
    rl += (char)0x4D; rl += (char)0x8B; rl += (char)0x5B; rl += (char)64;        /* mov r11, [r11+64]  */
    return rl;
}
static inline std::string x86_rtcc_wb_text(void) {
    std::string wb;
    /* RC-8b (this session): arg tier {RAX,RCX,RDX,RSI,RDI} writeback dropped -- confirmed dead by exhaustive
     * codebase search, see x86_rtcc_wb_bin's comment for the full account.  RAX still loads the block address
     * as the base register for the scratch-tier stores that remain; its own pre-call value is no longer saved
     * anywhere since nothing ever read slot 0 back either. */
    wb += " mov rax, qword ptr [rip + g_rtcc_block@GOTPCREL]\n";
    wb += " mov qword ptr [rax + 40], r8\n";
    if (!RTCC_GLOBAL_R9_GVA) wb += " mov qword ptr [rax + 48], r9\n";   /* SKIPPED under the GVA claim -- H2 */
    wb += " mov qword ptr [rax + 56], r10\n";
    wb += " mov qword ptr [rax + 64], r11\n";
    /* rax left = block ptr; call stub follows directly */
    return wb;
}
static inline std::string x86_rtcc_rl_text(void) {
    std::string rl;
    /* RC-4 PARTIAL RELOAD: scratch tier only {R8 R9 R10 R11}; arg tier reload deferred to RC-5.            */
    /* Use r11 as block base; restore r8/r9/r10; restore r11 last from its slot.                             */
    rl += " mov r11, qword ptr [rip + g_rtcc_block@GOTPCREL]\n";
    rl += " mov r8,   qword ptr [r11 + 40]\n";
    rl += " mov r9,   qword ptr [r11 + 48]\n";
    rl += " mov r10,  qword ptr [r11 + 56]\n";
    rl += " mov r11,  qword ptr [r11 + 64]\n";
    return rl;
}
/* x86_rtcc_call — RC-4 RTCC veneer for void/int/ptr-returning calls (no DESCR_t capture needed).               */
/* KILLSWITCH: gate OFF → byte-identical to pre-RTCC (x86_call_ro).                                             */
inline std::string x86_rtcc_call(const char * sym, uint64_t ptr) {
    if (!g_rtcc_on) return x86_call_ro(sym, ptr);   /* KILLSWITCH: gate OFF → byte-identical to pre-RTCC */
    uint64_t block = (uint64_t)(uintptr_t)g_rtcc_block;
    if (MEDIUM_BINARY) {
        /* Call stub: movabs r10,ptr; call r10 — R10 already written-back, free as indirect-call scratch */
        std::string call_b;
        call_b += (char)0x49; call_b += (char)0xBA; call_b += u64le(ptr); /* movabs r10, ptr */
        call_b += (char)0x41; call_b += (char)0xFF; call_b += (char)0xD2;  /* call r10 */
        return x86_align_assert() + x86_Lrec(x86_rtcc_wb_bin(block)) + x86_Lrec(call_b) + x86_Lrec(x86_rtcc_rl_bin(block));
    }
    return x86_align_assert() + x86_rtcc_wb_text() + " call " + sym + "@PLT\n" + x86_rtcc_rl_text();
}
/* x86_rtcc_call_descr — RC-4 RTCC veneer for DESCR_t-returning calls — declared here, defined after FRQ/x86. */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum { X86H_DEF = 0, X86H_JMP = 1, X86H_JCC = 2, X86H_DEF_PAIR = 3 };   /* X86H_DEF_PAIR (s22j) -- an N-ary box's PAIR label define is an INTERNAL REJOIN POINT, not an alternative box entry.  x86_deflabel_pair used to report itself as (X86H_DEF, X86P_BETA), which made every one of a box's 2N+3 pair defs look like a beta ENTRY to the carve-only bracket below; a single traversal of IR_MATCH_ALTERNATE then carved at alpha, again at the arm's sigma stub and again at the merge while freeing only once at gamma (measured -64 per pass on 'aaaxx' ? ('aaa'|'bbb')), after which bb_match_end read bb_match_begin's saved-rsp unwind base at the wrong depth and segvd.  The carve-only discipline is rsp-neutral for a box entered through exactly ONE of {alpha,beta} and exited through exactly ONE of {gamma,omega} -- pair rejoins are neither, so they carry this distinct site and the enter arm ignores them.  Every other hook arm is alpha-keyed; the sole beta-keyed arm besides the carve is the ZC_PORT_OWNED mark, which accepts both spellings explicitly so OWNED-mode behavior is unchanged. */
inline std::string x86_port_hook(int site, int port);
inline int x86_fc_on();
inline std::string x86_fc_jcc_omega(const char * mnem);
inline std::string x86_fc_jcc_gamma(const char * mnem);
inline std::string x86_jcc(const char * mnem, int port) {
    /* ZB-FC-0: under the FORTH cell a conditional omega must ALSO pop the box's own cell, and a bare jcc can
     * carry no pop (add rsp,K clobbers flags and a jcc is a single branch).  The synth inverts the condition
     * over a 2-instruction skip: jcc' L(synth); [hook pops via the one X86H_JMP/OMEGA arm] jmp omega;
     * L(synth):  -- S10b's G3 as a visible per-path pop, zero template edits, ONE pop arm serving every exit. */
    if (port == X86P_OMEGA && (x86_fc_on() || _.op_wpop > 0)) return x86_fc_jcc_omega(mnem);   /* BP-9 (ii): a pending ΣK chain-pop needs the same invert+pop+jmp synth a box's own fc cell does */
    if (port == X86P_GAMMA && ((x86_fc_on() && _.op_fc_base < 0) || _.op_zgpop > 0)) return x86_fc_jcc_gamma(mnem);   /* ZW-1: a conditional gamma in the carve-only class must free the bracket too -- same invert+pop+jmp synth, gamma flavor, so the ONE X86H_JMP/GAMMA hook arm serves every success path.  ZD-1: a pending statement-terminal release (op_zgpop) needs the same synth for the same reason -- add rsp clobbers flags, so the release rides the synth's inner jmp-gamma where the ONE hook arm emits it. */
    return x86_port_hook(X86H_JCC, port)
         + (MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(port))
                          : (std::string(" ") + mnem + " " + x86_portname(port) + "\n"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZC_PORT accessor — runtime-selectable (Lon 2026-07-10, --zeta-port flag) via the ONE variable in
 * zeta_alloc.c (flag > SCRIP_ZETA_PORT env > ZC_PORT default), exactly the x86_zeta_mode() pattern below.
 * Emit-side seams read THIS, never getenv, never argv. */
extern "C" int rt_zeta_port_mode(void);
extern "C" int rt_zc_frame_live(void);
extern "C" int rt_zeta_storage_get(void);   /* ZOP-1: the four-config selector, read emit-side by x86_zstorage below (sibling of rt_zc_frame_live above -- same one-authority pattern). */
inline int x86_zc_frame() { return rt_zc_frame_live(); }   /* Z4-7 slice 1: THE live frame value.  Every former compile-time `ZC_FRAME ==/!= ZC_FRAME_RSP` comparison in templates/emitter routes here; rt_zc_frame_live (zeta_alloc.c) is the ONE authority (the Z4-6 law applied to the frame axis) and returns ZC_FRAME_ISLE iff the four-config selector says ZC_STORAGE_FRAME_R12, else the build constant -- so every non-island selection folds to the identical truth table and default emission is byte-identical by construction. */
/* ZW-0 stage 2: x86_isle() deleted -- ZC_FRAME_ISLE unreachable at ZC_FRAME_RSP default; all island arms removed */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_zstorage() { return rt_zeta_storage_get(); }   /* ZOP-1 (Lon s21x-n "parameterize the access to operands via the FOUR modes"): the EMIT-side reader of the four-config selector, twin of x86_zc_frame above.  Until this rung ZC_STORAGE had TWELVE references and not ONE of them was an operand-address decision -- the selector merely DERIVED the legacy port/frame tuple (zeta_alloc.c rt_zeta_storage_set), so the mixed-regime states its own design comment calls "UNREPRESENTABLE" stayed fully representable and a graph kept addressing half its operands one way and half another.  This is the reader that makes the enum load-bearing at the one place that matters. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void *rt_zh_bump_slow(long bytes);
inline void *rt_zh_bump_slow_addr() { return (void *)rt_zh_bump_slow; }
inline int x86_port_mode() { return rt_zeta_port_mode(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZC_PORT_FORTH (RUNG ZB-FC-0, Lon 2026-07-12; design zeta_choices.h ZC_PORT_FORTH + ARCH-ZETA S10).
 * x86_port_cstack(): FORTH is a strict CSTACK SUPERSET -- every seam that fires under CSTACK (statement
 * bracket, dynamic ARB/ARBNO rsp blocks, the mark/release rsp forms) fires identically under FORTH; this
 * predicate replaces the raw ==ZC_PORT_CSTACK comparisons at those seams.  x86_fc_on(): the per-box fixed
 * FORTH cell is live for THIS node (flavor selected AND fc_geom granted).  x86_fc_hit(off): a flat-frame
 * offset falls inside this box's own granted window -- FR/FRQ rebase it to [rsp + off - op_fc_base], valid
 * exactly while control is inside the box (the S10c port invariant: rsp at the box's frontier at every
 * port).  x86_jcc_invert(): condition inversion for the conditional-omega pop synth (see x86_jcc). */
inline int x86_port_cstack() { int m = x86_port_mode(); return m == ZC_PORT_CSTACK || m == ZC_PORT_FORTH; }
inline int x86_fc_on()       { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_bytes > 0; }
inline int x86_fc_miss(int bump) { static int n = 0; if (bump) n++; return n; }
inline int x86_fc_hit(int off) { int w = _.op_fc_bytes > 0 ? (int)_.op_fc_bytes : (int)_.op_fc_wbytes; int granted = x86_port_mode() == ZC_PORT_FORTH && w > 0 && _.op_fc_base >= 0; int hit = granted && off >= _.op_fc_base && off < _.op_fc_base + w; if (granted && !hit) { int own = _.op_own_ci > 0 && off < (int)_.op_own_ci; int fullcell = _.op_fc_bytes > 0; int defect = own && fullcell; if (defect) x86_fc_miss(1); static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_FC_AUDIT"); on = (e && *e == '1') ? 1 : 0; } if (on) fprintf(stderr, "[FC-%s] granted box falls back to [off %d]: window=[%d,%d) w=%d ci=%ld\n", defect ? "MISS" : (own ? "FLAT-BYDESIGN" : "CROSS"), off, _.op_fc_base, _.op_fc_base + w, w, (long)_.op_own_ci); } return hit; }   /* ZB-VAL-8b GATE (s182, closes s181 HEADLINE 6): the fallback is SILENT BY CONSTRUCTION -- an undersized window does not crash and does not emit a WRONG address, it just leaves the box on ___, so "I converted it" and "it converted" were indistinguishable in the build.  A GRANTED box (w>0, base>=0) whose offset misses its own window is exactly that event; count it always, narrate it under SCRIP_FC_AUDIT=1.  test_gate_fc_no_residual____.sh asserts the count is ZERO across the corpus, which is what makes conversion progress falsifiable */
inline std::string x86_fc_jcc_omega(const char * mnem);
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
inline const char * x86_jcc_invert(const char * m) { return x86_jcc_canon((uint8_t)(x86_jcc_op(m) ^ 1)); }   /* ICN-JCC (re-landed: s204's fix lived only in unpushed local commits; HEAD still carried the 4-pair table).  x86_jcc_op and x86_jcc_invert are ONE vocabulary spelled twice, and that is exactly how they drifted -- jcc_op encodes 12 conditions plus aliases, jcc_invert knew 4 pairs and ABORTED on the rest, which silently killed x86_fc_jcc_omega (the ZB-FC-0 conditional-omega pop synth) for every x86_omega("jz") speller: bb_binop_relop, bb_case_arm, bb_to, bb_match_arbno/defer/value -- i.e. Icon's whole relop + generator surface.  THE FIX IS NOT A BIGGER SECOND TABLE: in the x86 encoding the LOW BIT OF THE JCC OPCODE IS THE NEGATION BIT (0x84^1=0x85 je/jne, 0x8C^1=0x8D jl/jge, ...), so the inverse is DERIVED from the one authority and is total over jcc_op's vocabulary by construction.  A new condition added to jcc_op can never again leave invert behind; x86_jcc_canon's abort fires only if jcc_op grows an opcode this switch does not spell, which is a different and louder failure than a missing pair. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* BB-OWNED-ζ STEP 1 (Lon pivot, this session).  x86_selfload_mode() mirrors x86_port_mode()'s env-override
 * pattern but reads ZC_SELFLOAD (the α/β self-load axis), not ZC_PORT.  SCRIP_ZETA_SELFLOAD env var overrides
 * the compile-time default for quick A/B without a rebuild, exactly as SCRIP_ZETA_PORT already does. */
inline int x86_selfload_mode() {
    static int m = -1;
    if (m < 0) { const char *e = getenv("SCRIP_ZETA_SELFLOAD"); m = e ? atoi(e) : (int)ZC_SELFLOAD; }
    return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* FRAME REGISTER SWITCH (Lon directive, 2026-07-08 session 2) — WHICH register is the ζ frame base.  Default =
 * r12 (the ratified GZ3 contract).  ZC_FRAME____ is the C-frame-pointer EXPERIMENT from the C-STACK analysis
 * (GOAL-SNOBOL4-BB.md SESSION STATE: when the proc trampoline retires, rsp becomes the frame and "r12 AND ___
 * free up" — this switch lets the ___ half of that end-state be exercised NOW, ahead of the trampoline work).
 * COMPILE-TIME ONLY (Lon directive 2026-07-09: "We will never flip the R12 to RSP or ___ at runtime") — the
 * SCRIP_ZETA_FRAME env override is DELETED, enforcement by deletion; ZC_FRAME is a BUILD CONSTANT
 * (zeta_choices.h default, or -DZC_FRAME=…) and every selector below constant-folds to a literal.
 * EVERYTHING frame-relative flows through the four accessors
 * below — the text form (x86_zr), the encoding number (x86_zr_num), the modrm/REX producers further down
 * (x86_frame_modrm + the conditional-REX helpers), the FR/FRQ operand spellings AND the x86_parse arm that
 * classifies them back (kept in lockstep via x86_fr32_prefix/x86_fr64_prefix), the port-hook canary/alloc
 * instructions, and the alignment-dance save register.  KNOWN EXPERIMENT LIMITS under ZC_FRAME____, stated
 * honestly: (a) any template still hand-spelling a push-___ alignment dance instead of x86_align_enter/leave
 * clobbers the frame — the live match-family dances are swept this session, the inert rbx-dance holdouts
 * (bb_gvar_assign_concat, bb_pattern_break/len, bb_ref_invariant) are pre-existing and unaffected either way;
 * (b) C code compiled -O0 uses ___ as ITS frame pointer, so a callee's own prologue saving/restoring ___ is
 * fine (callee-saved either way, same as r12) but gdb frame-walking of emitted code gets weirder; (c) the
 * six-register coexpr save contract (bb_create.cpp) already saves BOTH r12 and ___, so it covers either
 * choice unchanged. */
inline const char * x86_zr()         { return "rsp"; }   /* ZW-0 stage 2: island "r12" arm deleted; REG-7 U5 SEAL: ONE stream — under RSP zr IS rsp */
inline int          x86_zr_num()     { return x86_zc_frame() == ZC_FRAME_RSP ? 4 : 5; }   /* ZW-0 stage 2: island 12 arm deleted; ζ basis set CLOSED at rsp(4)/___(5) */
/* FLAT FRAME (s189) — ___ IS ERADICATED FROM FRAME ADDRESSING.  The frame base IS rsp, unconditionally.
 * Every frame reference adds the box's STATIC depth D = g_emit.op_flat_disp: the running prefix sum over the
 * known BB sequence that LOWER's fc_leaf_walk computes (allocation order = flow order on a linear spine;
 * granted ALTERNATE arms pad to fpmax so the post-ALT depth is uniform).  D = 0 is the common case and it is
 * EXACT, not a fallback — a statement that pushes no FORTH cell leaves rsp at the activation seed, which IS
 * the frame base by construction (xa_flat `sub rsp, K_total`).
 * NO SWITCH (Lon directive s189: "we will not be changing back").  ZC_FLATDISP and the ___/r12 arms of these
 * five accessors are DELETED — enforcement by deletion, the same discipline that retired the SCRIP_ZETA_FRAME
 * env override.  There is no A/B control and no way to spell an ___ frame reference any more.
 * ONE function owns the arithmetic; every other site calls it. */
inline int x86_fb_pinned() { return 0; }   /* ⭐ DEL-T1 D-2 (measured mechanism at emit_rec_rsp_arm, emit.h): the orphaned PAT$ blob class establishes NO ___, so its FR() δ refs take the unpinned depth-static rsp arm with op_flat_disp compensation — the arm this selector already provides.  Reading the SAME predicate as the two record sites is the point: base selection for this class is ONE decision.   ZETA-FB-2 (s160): ONE PREDICATE.  Was emit_jmp_pin____(), which named only ONE of the TWO prologue arms that establish ___ as the frame base: xa_flat's jmp-entry hdr (save+seed, gated emit_jmp_pin____) AND xa_flat.cpp:281's gen-proc/resumable HEAP-FRAME ADOPT (`push ___; mov ___,rdi`, gated g_gen_proc_active||g_resumable_callable_active, carving no rsp frame at all).  emit_rec_pin() IS that disjunction, so the base a DATA ref names, the base the RECORD protocol names, and the base the PROLOGUE establishes are now one decision and cannot drift.  THE GAP WAS REACHABLE, NOT THEORETICAL: flat_gen = is_generator && emit_graph_has_suspend(g) (emit.cpp:2399) while g_gen_proc_active = is_generator, so a SUSPEND-FREE generator graph took the adopt prologue (___ = heap frame) while every data ref still spelled rsp — the exact s158 land mine, latent.  MEASURED BYTE-NEUTRAL: the ZETA-FB-1 divergence gate reports 0 disagreements over 592 corpus programs across four frontends, so today this widens nothing and the .s output is byte-identical; it closes the seam before a graph reaches it.  NOT the blind widening s158 measured as a regression — that rebased fb while op_flat_disp still double-counted; FLATDISP-8 gated the compensation (x86_frame_off below), which is what makes the union safe now.  FLATDISP-8 (s197) provenance: THE FRAME-BASE SELECTOR — reads the SAME predicate that decides whether the prologue saves+seeds ___ (emit.h:599, xa_flat's hdr arms), so the base a reference NAMES and the base the prologue ESTABLISHES are one decision and cannot drift.  Seeded `mov ___,rsp` at the activation flat base => ___ IS the frame base and is DEPTH-IMMUNE; unpinned graphs never touch ___ (free GPR) and keep the s189 rsp regime with its op_flat_disp compensation.  This restores the arm s189 deleted, now per-graph instead of a build constant: s188/s189 made fb unconditionally rsp while xa_flat kept seeding ___ for the pinned classes, and a SUSPENDED generator resumes with rsp at the deep frontier -- no static displacement exists (the FLATDISP-5 wall), which is why the residual 14 were ONE class. */
static inline int x86_fb_stmt_on() { static int m = -1; if (m < 0) { const char * e = getenv("SCRIP_FB_STMT"); m = (e && *e == '0') ? 0 : 1; } return m; }   /* FB-STMT-FLIP (s22c): DEFAULT-ON, killswitch SCRIP_FB_STMT=0.  Lon directive 2026-07-29 "Change every ___ to RSP that can be. I want only the housekeeping data indexed by ___" — this is that directive shipped.  MEASURED AT HEAD s22c: ___ instructions over 318 crosscheck programs 15459 -> 10310; split by shape, CEREMONY (___/rsp as a whole-register operand: prologue seed, epilogue restore, frame-chain walk, rsp rebalance — law 4's sanctioned housekeeping) is EXACTLY UNCHANGED at 3762, while DATA REFS ([___+N] as a memory operand only — the value spine) fall 11697 -> 6548 (-5149, -44%).  That the ceremony count does not move by even one instruction is the proof the refinement touches only the class the directive names.  GATES, all green: (a) crosscheck m3 232/85 m4 229/86/2 DIV=1 {W04_arbno_basic} both regimes — IDENTICAL BY SET in BOTH modes, zero broke zero fixed; (b) DECLINED-GRAPH SWEEP (the s21x-r instrument) — 214 programs byte-identical, 103 differ and ALL 103 have ___ REDUCED, ZERO programs changed bytes without a corresponding ___ change, so the null is non-vacuous and there is no process-scope leak.  ⛔ THE s21x FALSIFICATION IS STALE, DO NOT RESTORE IT FROM THE OLD PROSE: it read "default-on regressed 7 programs" on the premise that statement exits do not rebalance rsp, witnessed by (a) 180_pat_arbno_defer_nonrecursive and (b) 072_pat_star_var_alt_backtrack.  BOTH are now red in BOTH regimes at HEAD independent of this flag (they are members of the 85/86 match-family debt set), so neither can discriminate any longer, and the rsp-imbalance class they witnessed is what s22b's WPOP-1 fixed — the fail edge was over-freeing by 32 with the whole-graph carve absorbing it.  The premise died with that bug; this flip is the falsification being retired on measurement, not overridden. */
inline int x86_fb_data() { return 0; }   /* FB-STMT (Lon 2026-07-29 "only housekeeping via ___"): the DATA-REF base predicate — under a statement-bracketed pin, nodes outside deep match statements speak rsp+op_flat_disp; ceremony/record protocol/epilogues keep reading emit_jmp_pin____/emit_rec_pin untouched, so the base the prologue ESTABLISHES is unchanged and mixed spellings address identical bytes (___==base; static reader rsp==base−live, x86_frame_off compensates). */
inline int x86_frame_off_rsp(int off) { return off + _.op_zdepth; }   /* PB-2 (2026-08-07c): RSP-ONLY variant of x86_frame_off — always takes the rsp/zvo arm, never the pinned-___ arm.  Required for reading a PRE-CLAIM UCLAIM producer (a spine VAR above the match-begin claim) from inside a PINNED graph (deep_arrival=1 because of the IR_MATCH_VALUE node).  The pinned arm of x86_frame_off returns [___+off] = [stmt_base+128] — wrong because ___ is the STATEMENT base, not the MATCH_BEGIN claim base.  The rsp arm resolves via zvo_resolve(128, udout, uhead) → 96 + zdepth(0) = 96 = [rsp+96] = the actual machine address the VAR wrote.  ZLS counts MATCH_BEGIN's own 32B region (offset 0..31) before the VAR at 32..127, so zls_off(VAR)=128 while the machine offset from claim_base is 96; zvo_resolve accounts for this 32B region in the owner table and subtracts it.  Only safe to force the rsp arm when the consumer is a non-ZD spine box (zdepth=0 and no sub rsp between claim and consumer), which is exactly the UCLAIM match run. */
inline int x86_rsp_slide_known() { return 1; }   /* SLIDING-RSP AUTHORITY, s53 RSP-ONLY-EVERYWHERE (Lon directive): ALWAYS knowable as off + op_zdepth -- there is no other base.  The old 0-arm (emit_rec_pin || op_stmt_dyn) poisoned every operand in the pinned classes with the [rsp + -1] sentinel, which is not a fallback but a wrong byte; depth-RELATIVE distance (own carve + static neighbor delta) is exact even where ABSOLUTE depth is dynamic, so the static spelling is right for every intra-frame read.  Classes whose depth truly diverges across a dynamic edge (unbounded ARBNO instance growth, suspend re-entry at foreign depth) now fail HONESTLY at run time and go on the failure list, per the s53 ruling: no whack-free without a base is expected, not worked around. */
inline int x86_frame_off(int off) { return x86_rsp_slide_known() ? off + _.op_zdepth : -1; }   /* THE ONE OFFSET FUNCTION, RSP-ONLY: sliding offset = off + own carve depth; unknown depth = -1 */   /* ZW-0 stage 2: x86_isle() guards deleted; THE ONE OFFSET FUNCTION: flat-frame offset -> frame-base-relative displacement; pinned ___ = depth-immune (off raw); rsp = off + op_zdepth */   /* ⭐ W-1b MECH-2 FR-FIX: under mech-2 (op_zw2=1) ___=α-8 (push ___; mov ___,rsp, no +8 adjustment). ZW-12 does lea ___,[___+8] so ___=α=claim_base, and FR(off)=[α+off] is depth-immune and correct. Mech-2 without this fix: FR(off)=[α-8+off], 8B low. Fix: add 8 when op_zw2 and pinned. Gate-OFF (op_zw2=0): returns off unchanged — byte-identical. Does NOT affect the header RDQ("___",-N) calls in bb_match_begin/end which bypass x86_frame_off entirely. */   /* ⭐ PIN-REBASE (s23a, the m4-SEGV-six root): under the s22z HEAD-PIN the pinned ___ IS the STATEMENT CLAIM BASE (rsp==base at the pin by the PATCTX invariant), NOT the whole-graph flat base the raw spelling was baked against -- CARVE-KILL deleted that base, and a raw [___+off] with off in flat coordinates reaches (off - umin) bytes ABOVE the claim: m4's main has ~56B of slack before argv/envp, so 066-class deep statements (claim [256,432), head quartet at 320..344, assign_save cap slot at 384 against a 176B claim) handed rt_cap_push a slot INSIDE environ -- the s22r envp-corruption class, caught by software watchpoint as a dword cursor store zeroing environ[0]'s high half.  m3 survived the identical overshoot only by rt_chain_enter frame slack (LUCK, the same word s22z used for m4's old CRT territory).  THE REBASE IS NOT DEPTH COMPENSATION: zvo's dout terms are K-K==0 on the UCLAIM path by the planner's own staging ("members' claims resolve to (off-min)+(K-K)"), and op_flat_disp/op_zdepth stay OFF this arm -- ZTOS-2's depth-immune law is upheld, only the COORDINATE base moves.  Gated op_stmt_pin>0 so blob/gen-adopt pinned classes (whose ___ is the activation seed, not a claim base, and whose graphs decline uclaims at the jmp-entry gate anyway) are untouched; a resolve MISS (foreign-statement flat ref) keeps today's raw spelling.  Side win: mixed ___/rsp spellings inside one claimed statement now name the SAME byte (rsp arm already resolved through zvo; the ___ arm naming base+raw_off was a latent aliasing split). */   /* UCLAIM (wholesale flip): on the rsp arm a flat off whose range the current run CLAIMED resolves through the owner table -- (off - claim_base) + (delta_out(reader) - delta_out(owner)) -- with the legacy op_flat_disp/op_zdepth intra-claim compensation added on top unchanged; this is the ONE execution-order offset function converting every FR/FRQ/FRQB reader at once, zero template edits.  Negative resolutions (a forward slot no claim backs on this path) fall back to the legacy spelling rather than address below TOS.  Pinned ___ / island arms stay depth-immune and untouched, per the ZTOS-2 law. */   /* ⛔ CARVE-ERAD step 2 -- THE PLAN-OF-RECORD CLAIM IS FALSIFIED, MEASURED A/B THIS SESSION.  The claim was: "op_flat_disp is LOWER\'s STATIC prefix sum -- the carve\'s ADDRESS side.  Delete the static term and every reader converts for free."  It is NOT only the carve\'s address side.  With the whole-graph carve DELETED, keeping this term scores m3 229/88 m4 227/88; deleting it scores m3 226/91 m4 224/91 -- three programs WORSE in both modes.  MECHANISM: op_flat_disp is the prefix sum over fc_geom-GRANTED suspended cells (S10c: every passed box is gamma-suspended) plus the 32B HEAD constant.  Those cells are NOT the whole-graph carve; they survive its deletion and still need compensating.  So step 2 is not a deletion, it is a NARROWING: the term must keep its fc_geom prefix and lose only whatever part described the deleted region.  The 24 sites in the manifest CANNOT simply be cut.  Term retained deliberately.  op_flat_disp was LOWER's prefix-sum distance from rsp to the WHOLE-GRAPH CARVE, i.e. the address side of a frame that no longer gets emitted; with the carve deleted there is no such base to be distant from, so every FR/FRQ/FRQB reader converts to the live per-BB authority by this one deletion rather than by 1065 hand edits.  op_zdepth (the box's own carve) is the only compensation left, which is the FORTH-cell model: a box compensates for exactly what IT claimed. */   /* ZTOS-2 (s21x-o): the rsp arm gains op_zdepth alongside op_flat_disp, for exactly the reason the TOS arm did.  op_flat_disp is LOWER's STATIC distance from rsp to the flat frame base; the box's own alpha carve moves rsp DOWN by K after that distance was computed, so the live distance is disp+K and a reference spelled with disp alone reads K bytes too low.  Arms 1 and 3 (island r12, pinned ___) are depth-IMMUNE bases and must NOT receive it -- adding depth compensation to a base that does not move is the FRQ/FRQB double-add class.  This is the same law as ZTOS-1 applied to the frame arm: a box compensates for exactly what IT carved. */   /* Z4-7: the island base is depth-immune -- compensation identically zero, same argument as the pinned ___. */   /* THE ONE OFFSET FUNCTION: flat-frame offset -> frame-base-relative displacement.  Sole consumers: x86_frame_modrm (BINARY modrm), x86_frame_text_mem (TEXT spelling), FR/FRQ (operand spellings).  Nothing else may add a frame displacement.  Under a PINNED ___ the compensation is IDENTICALLY ZERO by construction -- op_flat_disp is the running rsp-depth prefix sum and ___ does not move -- so adding it would double-count the very depth the pin exists to neutralize. */
inline const char * x86_fb()         { return "rsp"; }   /* ZW-0 stage 2: island r12 arm deleted; FRAME BASE: ___ when pinned, rsp otherwise (REG-7 U3, FLATDISP-8) */
inline int          x86_fb_num()     { return x86_fb_data() ? 5 : 4; }   /* ZW-0 stage 2: island 12 arm deleted; rsp(4)/___(5) only */
inline const char * x86_fr32_prefix() { return "dword ptr [rsp + "; }   /* ZW-0 stage 2: island r12 arm deleted */
inline const char * x86_fr64_prefix() { return "qword ptr [rsp + "; }   /* ZW-0 stage 2: island r12 arm deleted */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZETA SUBSYSTEM accessor — runtime-selectable BY DESIGN (Lon 2026-07-09, contrast the ZC_FRAME build
 * constant above); see zeta_choices.h ZC_ZETA block for the rung map.  RUNG-1 seams read THIS, never getenv,
 * never argv. */
extern "C" int rt_zeta_mode(void);
inline int x86_zeta_mode() { return rt_zeta_mode(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* THE PORT HOOK — the ONE seam where per-port emission flavors plug in (Lon directive, 2026-07-08 session 2:
 * "encapsulate all x86 emission for this through x86 referencing alpha, beta, gamma, and omega ... flexibility
 * to emit tracing, debugging, assertions, varying garbage collectors, varying stacks").  EVERY port label
 * define and EVERY jump/conditional-jump to a port already funnels through exactly three functions —
 * x86_deflabel / x86_jmp / x86_jcc (verified by the XK_PORT dispatch arms in x86(): "def"→deflabel,
 * "jmp"→jmp, jcc-mnemonics→jcc; audit this session found ZERO templates emitting a port label or port jump
 * any other way) — plus the DRIVE_PAIR β-define flush in x86_pair_loop().  All four call THIS hook, so a new
 * flavor (a GC safepoint, a per-port trace, a stack-discipline probe, an allocator) is ONE arm here, zero
 * template edits, both mediums for free.  Sites:
 *   X86H_DEF — a port label was just DEFINED here (code at the hook runs on EVERY arrival at that port).
 *   X86H_JMP — an unconditional jmp to a port is about to be emitted (hook output runs BEFORE the jump).
 *   X86H_JCC — a CONDITIONAL jump to a port is about to be emitted.
 * ⛔ THE FLAGS CONTRACT (load-bearing, do not relax): X86H_JCC hook output MUST PRESERVE CPU FLAGS — the jcc
 * consumes flags set by the caller's own preceding cmp/test, so a flavor that clobbers flags (the canary's
 * test; the ALLOC sub) silently corrupts every conditional port edge it decorates.  Flavors that need flags
 * freedom fire at X86H_DEF and X86H_JMP only (arrival at a label kills flag liveness by this codebase's own
 * convention — the pre-existing α ALLOC sub already relies on that).  This is exactly why the canary was
 * historically wired at x86_jmp and never x86_jcc; the constraint is now stated instead of implicit.
 * ZLS2 FRAME PROTOCOL (2026-07-08 session 3, Lon: "code injection of stack frame bump, restore on backtrack,
 * and release on exit ... the templates stay untouched for this entire operation"): the protocol rides the
 * THREE EXISTING sites — no new site, no template marker.  BUMP at the α define, RESTORE at the β define
 * (backtrack arrival = the fail-direction bulk release), RELEASE before a jmp-ω — gated per node by the
 * zls2_geom grant (zeta_storage.c), which awards RELEASE only to roles whose ω-jumps are STATICALLY all
 * activation-death (role knowledge; the op_omega_is_death chain classifier is recorded broken and is NOT
 * consulted — see the definition's ⛔ note below x86_zls2_pop_call).  Same seam, same reach, for future
 * trace/assert/GC flavors.
 * Current flavors (canary/trace relocated verbatim session 2; frame protocol added session 3):
 *   INSTRUMENTED canary (X86H_JMP)          — test ZR,ZR / jnz +2 / ud2: the R12-wiring assert, D13's sibling.
 *   ω compile-time trace (X86H_JMP, env)    — SCRIP_ZETA_OMEGA_TRACE: which ω sites exist + death class.
 *   ZLS2 FRAME PROTOCOL (ALLOC, ops-gated)  — per-node op_zls2_{ops,slot,bytes} from zls2_geom: BUMP at the
 *                                             α define, RESTORE at the β define, RELEASE at grant-marked
 *                                             jmp-ω.  See the definition below x86_zls2_pop_call for the
 *                                             instruction sequences and their contracts.
 *   ZLS2 α direct-sub (X86H_DEF, ALLOC)     — DORMANT (bytes>0 && ops==0): sub ZR,K, the recorded future
 *                                             full-discipline design for boxes that never touch the graph
 *                                             frame; its free twin is x86_zls2_free() at a template exit.
 * DEFINITION ORDER NOTE: x86_port_hook's BODY moved below x86_zls2_push_call/pop_call (which it now calls),
 * the same forward-decl-then-define-after-x86() pattern x86_zeta_free_call uses; the decl at the site enum
 * keeps x86_jmp/x86_jcc/x86_deflabel compiling here. */
inline std::string x86_sub(const char * reg, long imm);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_align_assert() — ALIGN-INV-0 (Lon directive 2026-07-13: alignment BY CONSTRUCTION — pad to 16-multiples,
 * waste 8 bytes to free ___ for good; the dance dies by deletion once this assert is clean).  Env-gated
 * instrument (SCRIP_ALIGN_ASSERT=1), prepended to EVERY emitted call form at the one "call" dispatch arm:
 * test spl,15 / jz +2 / ud2 — SIGILL at the exact offending call site (PC identifies it; run with
 * SCRIP_NO_SEGV_HANDLER=1 for a clean backtrace).  Same shape as x86_port_canary (D13's sibling); fixed
 * 8-byte Lrec in BINARY (byte-verified vs as: 40 F6 C4 0F / 74 02 / 0F 0B), local-label form in TEXT.
 * Default (env unset) emits NOTHING — byte-identical builds.  x86_align_enter's `and rsp,-16` masks
 * misalignment by design, so the true offender census needs a probe build with the dance no-op'd. */
inline int x86_align_assert_on(void) {
    static int v = -1;
    if (v < 0) { const char * e = getenv("SCRIP_ALIGN_ASSERT"); v = (e && *e && *e != '0') ? 1 : 0; }
    return v;
}
inline std::string x86_align_assert() {
    if (!x86_align_assert_on()) return std::string();
    if (MEDIUM_BINARY) return x86_Lrec(x86_b2(0x40, 0xF6) + x86_b2(0xC4, 0x0F) + x86_b2(0x74, 0x02) + x86_b2(0x0F, 0x0B));
    return std::string(" test spl, 15\n jz 1f\n ud2\n1:\n");
}
inline std::string x86_port_canary() {
    if (x86_port_mode() != ZC_PORT_INSTRUMENTED) return std::string();
    if (MEDIUM_BINARY) {
        int z = x86_zr_num(), lo = z & 7;
        uint8_t rex = (uint8_t)(0x48 | (z >= 8 ? 0x05 : 0x00)), modrm = (uint8_t)(0xC0 | (lo << 3) | lo);
        return x86_Lrec(x86_b3(rex, 0x85, modrm) + x86_b2(0x75, 0x02) + x86_b2(0x0F, 0x0B));
    }
    return std::string(" test ") + x86_zr() + ", " + x86_zr() + "\n jnz 1f\n ud2\n1:\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_zeta_free_call() — emits `call rt_zls_release(r12)`, alignment-safe (push ___/and rsp,-16/restore, the
 * SAME convention bb_match_capture.cpp already uses for its rt_cap_push/rt_cap_pop calls) because this call
 * is spliced into x86_jmp() at an arbitrary jmp-ω site whose surrounding stack parity is not guaranteed the
 * way a template's own fixed-position call site is.  r12 is passed as the sole argument and is NOT clobbered
 * afterward (rt_zls_release takes the block pointer by value; the caller's r12 register is unaffected by the
 * call itself, only by whatever the callee does with the copy in rdi) — the jmp that follows still uses
 * whatever r12 held going in, which is correct: the block is freed, not the register zeroed, matching
 * "flag/pointer-snap on exit" rather than "invalidate the pointer we're about to jump past."
 * FORWARD-DECLARED here (defined after x86() below, since its body calls x86() which isn't defined until
 * later in this file, while x86_jmp — which calls this — must stay BEFORE x86() because x86() itself calls
 * x86_jmp for XK_PORT jump operands; see the ordering note by the definition). */
extern "C" void rt_zls_release(void *);
/* x86_own_floor_store() -- RUNG ZB-OWN-0 tail: rdi=&cell, rax=current cell, rcx=target(mark-C_i).  Stores
 * the target ONLY when it is strictly BELOW the current cursor (floor-only positioning): `cmp rcx,rax /
 * jae +3 / mov [rdi],rcx`.  NEVER-HOIST COMPOSITION RULE (found live this rung, =5 first cut: 25 ARB/ARBNO/
 * capture reds): the universal static shadow and the granted DYNAMIC activation blocks share ONE cursor, and
 * an absolute store that hoists the cursor above a live dynamic block hands the block's bytes to the next
 * bump.  Floor-only composes: statics position exactly when no dynamic block is live below, and the dynamic
 * arm's own RELEASE restores the ceiling.  Canary-precedent raw dual-medium (private to this file). */
inline std::string x86_own_floor_store() {
    if (MEDIUM_BINARY)
        return x86_Lrec(x86_b3(0x48, 0x39, 0xC1) + x86_b2(0x73, 0x03) + x86_b3(0x48, 0x89, 0x0F));
    return std::string(" cmp rcx, rax\n jae 1f\n mov qword ptr [rdi], rcx\n1:\n");
}
inline std::string x86_zeta_free_call();
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_jmp's ALLOC-mode free-call fires ONLY when op_omega_is_death is set (computed once per node in
 * codegen_flat_chain_body, BEFORE this template ran — see the field comment in emit.h).  Verified this
 * session, precisely, against IR_MATCH_ARBNO: role 0's and role 1's `jmp "ω"` are BOTH cases where the
 * port-name "ω" is reused for an internal handoff (role 0's β-aliases into role 2's β, which then reads
 * role-0's OWN shared slot at FR(_.op_off) one instruction before ITS OWN true exit) — freeing on either of
 * those would be a premature free one instruction before the last legitimate read, the exact silent-
 * corruption trap this rung exists to avoid.  Only role 2/5's `jmp "ω"` (built via sno_ω_to(F, fail) in
 * lower_snobol4.c, landing on the pattern's own outer fail continuation, outside the local chain) sets
 * op_omega_is_death=1.  This is NOT a guess or a discriminator inferred from op_phase/op_node_kind (that
 * approach was tried and abandoned this session as unsafe) — it reads a flag the wiring layer already
 * computed with full graph knowledge, at the one point that knowledge was available. */
/* BB-OWNED-ζ STEP 1 STATUS (Lon pivot, this session): op_omega_is_death IS computed correctly (see emit.cpp)
 * and IS a sound, reusable signal — but the free-call this hook WOULD make is x86_zeta_free_call(), which
 * frees r12 itself.  STEP 1's actual implementation deliberately does NOT repoint r12 to the allocated block
 * (repointing the shared r12 would corrupt every sibling box interleaved with this ARBNO activation, since
 * r12 is one register for the whole function — see the design note in bb_match_arbno.cpp's
 * x86_arbno_role0_alloc).  The real pointer lives in a runtime-side single carrier
 * (rt_zls_arbno_step1_store/load in zeta_alloc.c), read back and freed at the TEMPLATE level
 * (x86_arbno_role2_free in bb_match_arbno.cpp), not here.  Firing x86_zeta_free_call() unconditionally at
 * this central hook would free whatever r12 happens to hold for the entire function — catastrophically
 * wrong, not merely a double-free.  DISABLED until a real central-hook design exists that knows how to reach
 * the correct per-construct carrier generically (a later rung, not this one).  op_omega_is_death stays
 * computed (it costs nothing when unused and is exactly the signal a future correct hook would need) but is
 * deliberately NOT read here for now. */
/* x86_jmp / x86_jcc / x86_deflabel — the three port-emission functions.  ALL per-port flavor code (canary,
 * traces, allocs, future GC/stack experiments) lives in x86_port_hook above, NOT inline here — one seam, three
 * callers (+ x86_pair_loop's β-define flush).  The former inline canary/ω-trace/ALLOC-sub bodies moved there
 * verbatim 2026-07-08 session 2; the STEP-1 op_omega_is_death history comment block that used to sit here is
 * preserved in git (this file, HEAD^) — its load-bearing conclusions (central ω free stays DISABLED; the
 * six-decoy-ω finding; op_omega_is_death computed-but-unread) are restated at the hook. */
inline std::string x86_jmp(int port) {
    return x86_port_hook(X86H_JMP, port)
         + (MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(port))
                          : (std::string(" jmp ") + x86_portname(port) + "\n"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_deflabel(int port) {
    std::string s = MEDIUM_BINARY ? x86_Drec(port) : (std::string(" ") + x86_portname(port) + ":\n");
    return s + x86_port_hook(X86H_DEF, port);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* THE FOUR PORT FUNCTIONS (Lon pivot 2026-07-08 session 5: "create a unique x86() function call for EACH of
 * the 4 port processing ... All the code HIDES inside these functions and the templates just call and
 * concat").  This is the ONLY port surface a template may use: the two INPUT ports α (fresh entry) and β
 * (backtrack resume) are label DEFINES; the two OUTPUT ports γ (success) and ω (failure) are TRANSFERS —
 * unconditional, or conditional via the mnemonic overload (jcc; the FLAGS CONTRACT at x86_port_hook governs
 * what a flavor may emit there).  Everything that varies — medium, ZLS2 arena bump/restore/release per
 * zls2_geom grant, poison, canary, ω-trace, the ARBNO selfload carrier, future assert/GC/stack flavors —
 * lives inside these calls (they inherit the whole x86_port_hook seam through the int-level internals they
 * wrap), so a template body is port-uniform, flavor-invisible, and NEVER edited again for flavor work.  The
 * former string forms x86("def","α") / x86("jmp","ω") / x86("jcc","ω") are RETIRED and abort at emit time in
 * x86_parse (the XK_PORT string arm) — the bb_bin_t enforcement-by-deletion pattern; parked (non-Makefile)
 * templates still carrying the old strings trip that lock the moment they are ever re-enabled, loudly. */
inline std::string x86_alpha()                    { return x86_deflabel(X86P_ALPHA); }
inline std::string x86_beta()                     { return x86_deflabel(X86P_BETA); }
inline std::string x86_beta_trampoline()          { return _.op_beta_dead ? std::string() : (x86_deflabel(X86P_BETA) + x86_jmp(X86P_OMEGA)); }   /* BP-9: det-leaf β define + jmp ω, elided when the driver proved zero inbound β edges — one body, both media (R2) */
inline std::string x86_gamma()                    { return x86_jmp(X86P_GAMMA); }
inline std::string x86_gamma(const char * mnem)   { return x86_jcc(mnem, X86P_GAMMA); }
inline std::string x86_omega()                    { return x86_jmp(X86P_OMEGA); }
inline std::string x86_omega(const char * mnem)   { return x86_jcc(mnem, X86P_OMEGA); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZLS2 ω-side free: the box's own-constant up-bump, placed by the TEMPLATE at its single true-exit label,
 * immediately before the final jmp "ω".  Reads the same per-node grant as the hook's α arm; same dormancy. */
inline std::string x86_zls2_free() {
    if (_.op_zls2_bytes > 0 && _.op_zls2_ops == 0 && x86_port_mode() == ZC_PORT_ALLOC) return x86_add(x86_zr(), _.op_zls2_bytes);
    return std::string();
}
enum { X86T_TGT0 = 4, X86T_TGT1 = 5 };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_tgt(int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    return MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(t))
                         : (std::string(" jmp ") + nm + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jcc_tgt(const char * mnem, int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    return MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(t))
                         : (std::string(" ") + mnem + " " + nm + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_call_tgt(int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    return MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE8)) + x86_Jrec(t))
                         : (std::string(" call ") + nm + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_lea_tgt(const char * dst, int t) {
    const char * nm = (t == X86T_TGT0) ? _.lbl_t0 : _.lbl_t1;
    if (MEDIUM_BINARY) {
        int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x04; code += (char)rex; code += (char)0x8D; code += (char)(0x05 | ((m & 7) << 3));
        return x86_Lrec(code) + x86_Jrec(t);
    }
    return std::string(" lea ") + dst + ", [rip + " + nm + "]\n";
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
                         : (std::string(" jmp ") + x86_internal_name(n) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jcc_id(const char * mnem, int n) {
    int id = x86_internal_id(n);
    return MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(id))
                         : (std::string(" ") + mnem + " " + x86_internal_name(n) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_deflabel_id(int n) {
    int id = x86_internal_id(n);
    return MEDIUM_BINARY ? x86_Drec(id) : (x86_internal_name(n) + ":\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZS-2 jmp-port transfer vocabulary (s58, design of record FINDING-2026-07-14-CLAUDE-SN4-ZS2-JMP-TOPOLOGY-DESIGN-AND-R12-LADDER.md §5/§8): materialize an internal label's address (the lea_tgt sibling,
 * same R9 patch primitive), indirect jmp through a register, indirect jmp through [base+disp] with the SIB byte rsp/r12 (low3=100) require — the s52 lea-rsp silent-emit class handled explicitly.     */
inline std::string x86_lea_id(const char * dst, int n) {
    int id = x86_internal_id(n);
    if (MEDIUM_BINARY) { int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x04; code += (char)rex; code += (char)0x8D; code += (char)(0x05 | ((m & 7) << 3)); return x86_Lrec(code) + x86_Jrec(id); }
    return std::string(" lea ") + dst + ", [rip + " + x86_internal_name(n) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_reg(const char * r) {
    if (MEDIUM_BINARY) { int m = x86_rnum(r); std::string code; if (m >= 8) code += (char)0x41; code += (char)0xFF; code += (char)(0xE0 | (m & 7)); return x86_Lrec(code); }
    return std::string(" jmp ") + r + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-DC (REGAIN-1C s108) — the direct det call's site transfer.  Intentional medium split of the R10 sanctioned class (the RO-load/call precedent): TEXT names the callee's dc stub label directly
 * (cross-graph legal, one .s); BINARY rides the fixed dc-fn slot (`movabs r11, &slot; call qword [r11]` — per-graph slabs make cross-graph rel32 unrepresentable, and the slot is filled at graph
 * seal, strictly before any runtime transfer).  r11 is caller-saved and dead at every site. */
inline std::string x86_call_dc(const char * dcname, uint64_t slot) {
    if (MEDIUM_BINARY) { std::string r = x86_movabs_r64("r11", slot); std::string c; c += (char)0x41; c += (char)0xFF; c += (char)0x13; r += x86_Lrec(c); return r; }
    return std::string(" call ") + dcname + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-DC — jmp to an EXTERNALLY-OWNED bb_label_t by pointer: the 'X' record (raw label pointer, rel32-discovered by the walker — the existing record, produced here for the first time from a
 * template-side helper).  Same-emission-session only (the pointer must outlive resolution); the dc stub's jmp to its own graph's α_body is exactly that.  TEXT twin takes the label's NAME. */
inline std::string x86_jmp_lblptr(bb_label_t * l, const char * txt) {
    if (MEDIUM_BINARY) { std::string r = x86_Lrec(x86_b1(0xE9)); r += (char)'X'; uint64_t v = (uint64_t)(uintptr_t)l; for (int j = 0; j < 8; j++) r += (char)(unsigned char)(v >> (8 * j)); return r; }
    return std::string(" jmp ") + txt + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PL-DC — tail-transfer to a C runtime leaf from a stub-local shim: TEXT ` jmp sym@PLT`; BINARY `movabs rax, fp; jmp rax` (rax dead at both shims: γ carries the result in rdi:rsi, ω carries
 * nothing).  The leaf's own `ret` then returns to the emitted site through the retaddr the shim just re-pushed. */
inline std::string x86_jmpfn(const char * sym, uint64_t fp) {
    if (MEDIUM_BINARY) { std::string r = x86_movabs_r64("rax", fp); std::string c; c += (char)0xFF; c += (char)0xE0; r += x86_Lrec(c); return r; }
    return std::string(" jmp ") + sym + "@PLT\n";
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
    char b2[96]; if (disp) snprintf(b2, sizeof b2, " jmp qword ptr [%s + %d]\n", base, disp); else snprintf(b2, sizeof b2, " jmp qword ptr [%s]\n", base);
    return std::string(b2);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-FC-0 conditional-omega pop synth (see x86_jcc): descending per-box id pool _.x86_fc_synth (reset 240 at
 * DRIVE_FILL -- below X86_INTERNAL_MAX, far above any template's own L(n)); the interior x86_jmp(OMEGA) fires
 * the X86H_JMP hook, which is where the single add-rsp,K pop arm lives -- the synth itself never touches rsp. */
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
/* x86_jmp_ext / x86_jcc_ext — jump (unconditional / conditional) to an EXTERNALLY-supplied bb_label_t*, e.g.
 * g_emit.flat_β_p, a label allocated by the surrounding chain machinery in emit.cpp rather than one this
 * template defines itself. The existing 'J' tag only carries a one-byte INDEX into bb_emit_x86's own local
 * internal[] array (see its decoder loop) — it has no way to reference a caller-supplied pointer. This adds
 * a new 'X' tag carrying the raw pointer bytes (8 bytes, this platform's pointer width), decoded by
 * bb_emit_x86 as a direct call to bb_emit_patch_rel32(that pointer) — the SAME function xa_emit_one already
 * calls successfully for external labels, just triggered from the tag-decode loop instead of from
 * xa_emit_one's raw-offset splicing. TEXT mode needs no tag machinery at all — GAS resolves a plain label
 * name at assemble time, so this is just the label's ->name string, exactly as xa_flat.cpp's existing TEXT
 * branches already do it directly. */
inline std::string x86_ext_ptr_bytes(const void * p) {
    uint64_t v = (uint64_t)(uintptr_t)p; std::string s; for (int i = 0; i < 8; i++) { s += (char)(unsigned char)(v & 0xFF); v >>= 8; } return s;
}
inline std::string x86_def_ext(const struct bb_label_t * lbl) {
    if (MEDIUM_BINARY) { std::string r; r += (char)'Y'; r += x86_ext_ptr_bytes(lbl); return r; }
    return std::string(lbl && lbl->name ? lbl->name : "?") + ":\n";
}
inline std::string x86_jmp_ext(const struct bb_label_t * lbl) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b1(0xE9)); r += (char)'X'; r += x86_ext_ptr_bytes(lbl); return r; }
    return std::string(" jmp ") + (lbl ? lbl->name : "?") + "\n";
}
inline std::string x86_jcc_ext(const char * mnem, const struct bb_label_t * lbl) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))); r += (char)'X'; r += x86_ext_ptr_bytes(lbl); return r; }
    return std::string(" ") + mnem + " " + (lbl ? lbl->name : "?") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_lea_rip_id(const char * reg, int n) {
    /* lea r64, [rip + L(n)] — materialize an internal label's code address (ALT-RESUME continuation cells).  REX.W 8D /r mod=00 rm=101; the rel32 is the instruction's last 4 bytes, so the same
     * J-record fixup that resolves jmp/jcc rel32 resolves this (rel32 is relative to next-instruction in both). */
    int g = x86_rnum(reg);
    int id = x86_internal_id(n);
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8D; c += (char)(0x05 | ((g & 7) << 3)); return x86_Lrec(c) + x86_Jrec(id); }
    return std::string(" lea ") + reg + ", [rip + " + x86_internal_name(n) + "]\n";
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
    return std::string(" mov ") + reg + ", qword ptr [rip + " + x86_internal_name(n) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_ro_seal_q(int n, uint64_t val) {
    if (MEDIUM_BINARY) return x86_Drec(X86_INTERNAL_BASE + n) + x86_Lrec(u64le(val));
    return x86_internal_name(n) + ":\n" + std::string(" .quad ") + std::to_string((unsigned long long)val) + "\n";
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
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" and ") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_cmp_imm(const char * reg, long imm) {
    int m = x86_rnum(reg);
    std::string code;
    if (imm >= -128 && imm <= 127) { if (m >= 8) code += (char)0x41; code += (char)0x83; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else if (m == 0)               { code += (char)0x3D; code += u32le((uint32_t)imm); }
    else                           { if (m >= 8) code += (char)0x41; code += (char)0x81; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" cmp ") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_modrm(int regfield, int off) {
    /* Literal-RSP modrm (s68): the x86_rsp_* family encodes base=rsp ALWAYS, independent of the zeta frame register.  Pre-s68 these delegated to x86_frame_modrm and free-rode on the r12/rsp low-3-bit pun
     * (12&7 == 4&7 == 4, REX.B never emitted) — the pun broke the day zr_num() could be 11.  rm=100 + SIB 0x24, mod by disp width, off==0 legal at mod=00. */
    std::string s; int rf = regfield & 7;
    int mod = (off == 0) ? 0 : (off >= -128 && off <= 127) ? 1 : 2;
    s += (char)((mod << 6) | (rf << 3) | 4); s += (char)0x24;
    if (mod == 1) s += (char)(int8_t)off;
    else if (mod == 2) s += u32le((uint32_t)off);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_modrm(int regfield, int off) {
    /* Frame-base modrm, generalized 2026-07-08 session 2.  RENAMED from x86_r12_modrm at ZR-RSP___-2: the old name
     * was kept "for grep continuity" while r12 was still a selectable ζ basis, and its own comment had to carry a
     * correction saying "r12" really meant x86_zr_num().  With ZC_FRAME_R12 deleted (ZR-RSP___-1) that continuity is
     * a liability — the name pointed at a register this encoder no longer has any relationship with — so it now
     * matches its siblings x86_frame_rex / x86_frame_off / x86_frame_text_mem.  Base is x86_fb_num(), NOT zr.
     * rsp (low3=100): SIB byte 0x24 mandatory, mod=00 legal at off 0.  ___ (low3=101): no SIB, and mod=00 is
     * UNAVAILABLE ([___] with mod=00 encodes disp32/RIP-relative), so off==0 must take the disp8 form — one extra
     * byte, matching exactly what `as` emits for [___ + 0] (the R10 BINARY-agrees-with-TEXT law). */
    off = x86_frame_off(off);   /* FLATDISP s188: compensate BEFORE the mod/disp-width choice, or a depth-shifted ref silently picks the wrong encoding length */
    std::string s; int rf = regfield & 7; int b = x86_fb_num() & 7; int sib = (b == 4);   /* REG-7 U3: base = x86_fb_num() (frame base), no longer zr — lockstep with the fr prefixes */
    int mod = (off == 0 && b != 5) ? 0 : (off >= -128 && off <= 127) ? 1 : 2;
    s += (char)((mod << 6) | (rf << 3) | (sib ? 4 : b));
    if (sib) s += (char)0x24;
    if (mod == 1) s += (char)(int8_t)off;
    else if (mod == 2) s += u32le((uint32_t)off);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_frame_rex(w, regfield) — the REX byte for a frame-relative access, emitted ONLY when nonempty (a bare
 * 0x40 is what `as` omits, so R10 forbids emitting it): W from the caller, R from a high regfield, B from the
 * frame register itself.  Under r12 the B bit is always set, so every call site's byte is identical to the
 * old hard-coded 0x49/0x41 forms; under ___ B drops and low-regfield 32-bit ops emit no REX at all. */
inline std::string x86_frame_rex(int w, int regfield) {
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (regfield >= 8) rex |= 0x04; if (x86_fb_num() >= 8) rex |= 0x01;
    std::string s; if (rex != 0x40) s += (char)rex;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_text_mem(int off) { return std::string("[") + x86_fb() + " + " + std::to_string(x86_frame_off(off)) + "]"; }   /* FLATDISP s188: TEXT twin of the x86_frame_modrm compensation — both mediums read the ONE function, so R10 (BINARY agrees with TEXT) holds by construction */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_lea(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x8D; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return std::string(" lea ") + reg + ", " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_movsxd_frame(dst64, off) — movsxd dst64, dword ptr [ζ+off]: the FRAME-SOURCE form of movsxd, mirroring
 * x86_frame_lea exactly (REX.W + opcode + x86_frame_modrm) but with opcode 0x63.  The register-source encoder
 * x86_movsxd builds a mod=11 reg/reg byte and reads x86_rnum(src) — for a memory operand string that yields
 * reg 0 (eax), so the register encoder emits `movsxd dst, eax` in BINARY while TEXT string-concats the correct
 * memory form: the exact mode-3/mode-4 divergence that corrupted scan-nary saved_δ (2026-07-13).  Source is
 * 32-bit by movsxd definition, so the text form carries `dword ptr`. */
inline std::string x86_movsxd_frame(const char * dst64, int off) {
    int g = x86_rnum(dst64);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x63; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return std::string(" movsxd ") + dst64 + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_mov_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, 0); c += (char)0xC7; c += x86_frame_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return std::string(" mov dword ptr ") + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_store(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x89; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov dword ptr ") + x86_frame_text_mem(off) + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_load(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x8B; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov ") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_add_imm(int off, long imm) {
    if (MEDIUM_BINARY) {
        /* REX via x86_frame_rex (2026-07-08 session 2): this helper's former unconditional 0x41 was the ONE
         * hard-coded frame REX the generalization sweep missed — under ZC_FRAME____ it turned the base-___
         * modrm (low3=101) into base-R13 (same low3, REX.B set), so IR_MATCH_BEGIN's β cursor bump wrote
         * [r13+off] while every reader read [___+off]: the anchor never advanced and the simplest literal
         * match spun forever in mode-3 (found by disassembling the live JIT stream; mode-4 was immune because
         * the TEXT arm below was always spelled from x86_frame_text_mem). */
        std::string c; c += x86_frame_rex(0, 0);
        if (imm >= -128 && imm <= 127) { c += (char)0x83; c += x86_frame_modrm(0, off); c += (char)(uint8_t)(int8_t)imm; }
        else                           { c += (char)0x81; c += x86_frame_modrm(0, off); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return std::string(" add dword ptr ") + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_add_to_reg(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x03; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return std::string(" add ") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_sub_from_reg(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x2B; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return std::string(" sub ") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_zop_regime(int off) { if (x86_zstorage() == ZC_STORAGE_FRAME_R12) return 1; if (x86_fc_hit(off)) return 2; return x86_fb_data() ? 3 : 4; }   /* ZOP-1 (ZW-0 stage 2: x86_isle() conjunct deleted -- arm 1 reachable only via ZC_STORAGE_FRAME_R12 now); regime: 1=r12 island, 2=own per-BB cell window on rsp, 3=pinned ___, 4=rsp+op_flat_disp */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline void x86_zop_note(int r) { if (r < 1 || r > 5) return; _.zop_seen |= (1 << r); }   /* ZOP-1 MIX AUDIT: the per-graph regime bitmask.  ZTOS-1 widened the range to admit arm 5 (the value spine).  Arm 5, like arm 2, is NOT a whole-graph frame regime and is deliberately NOT counted by the mixed-frame sum in zop_audit_graph_close -- it is the TOS value stack, which composes with whichever frame regime the graph is in, exactly as the per-BB cell window does.  A graph addressing its operands through more than one whole-graph regime (arms 1/3/4 -- arm 2 is the per-BB cell and composes with exactly one of them BY DESIGN) is the s188 mixed-regime failure shape that zeta_choices.h says must be unrepresentable.  Counting it is what makes it falsifiable; test_gate_zop_one_regime.sh asserts the count. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * x86_zop(int off, int q, int bump) {
    static char b[16][48]; static int i; i = (i + 1) & 15; int r = x86_zop_regime(off); x86_zop_note(r);
    if (r == 2) { if (getenv("SCRIP_ZOP_DIAG")) fprintf(stderr, "[ZOP] off=%d op_fc_base=%d bump=%d computed=%d\n", off, _.op_fc_base, bump, off - _.op_fc_base + bump); snprintf(b[i], 48, "%s ptr [rsp# + %d]", q ? "qword" : "dword", off - _.op_fc_base + bump); }   /* L-3b ARB RESIDUAL ROOT CAUSE + FIX (this session): the ARGREAD land mine (see the neighboring bump-arm comment) also applied HERE, unbumped -- regime 2 resolves the per-BB cell window ONCE, correctly (measured via SCRIP_ZOP_DIAG: off=520192 base=520192 -> computed=0), but the plain "[rsp + N]" spelling this line used to emit is BYTE-IDENTICAL to x86_fr32_prefix()/x86_fr64_prefix() on an unpinned graph, so x86_parse's XK_FR32/XK_FR64 classifier (which runs BEFORE the dead XK_RSP32 plain-rsp fallback at line ~1432) re-parsed the ALREADY-RESOLVED "0" as a FLAT ZLS COORDINATE and re-ran it through x86_frame_off(), adding op_zdepth (16 for a match-primitive with one un-popped sub rsp,16 predecessor) a SECOND time -- confirmed disassembling l3_spl_arb_nonterm.sno: the diagnostic computed 0 but the emitted instruction read "[rsp + 16]".  ARB's own scratch cell (x86_scratch_off, x86_scratch_off+4 -- its retry-extension-counter and saved-start-cursor, bb_match_arb.cpp) landed 16 bytes past its own carve into whatever the PRECEDING un-popped primitive owned there, and bb_match_end.cpp's writer (RDD("rsp", op_fc_disp), which independently and correctly targets that same displaced address by design -- see FINDING-2026-08-12j) then read ARB's retry counter (0,1,2,...) instead of a cursor.  SPAN's identical FR(x86_scratch_off) shape happened to PASS only because its sp_gu() guts variant never writes x86_scratch_off itself (only +4), leaving whatever was at the double-resolved address undisturbed -- a coincidence, not evidence the address was right; l3_spl_span_span_double (two chained SPANs) or any ARB-touching witness was always going to falsify it, which is exactly what happened.  FIX: spell the SAME "[rsp# + N]" raw-machine escape the bump arm already uses (x86_parse line ~1426), so XK_RSP32/XK_RSP64 claims the string first and x86_frame_off() never runs on an already-resolved regime-2 address.  Byte-identical for every OTHER regime (1/3/4 untouched) and for bump!=0 (already escaped). GATE WITNESSES: l3_spl_arb_nonterm/break_nonterm (expected to flip PASS -- BREAK shares bb_match_break.cpp's identical FR(x86_scratch_off) shape per grep) vs every other l3 probe + the broad SNOBOL4/Icon/Prolog corpora (expected byte-identical: regime 2 is FORTH-port match-primitive scratch cells ONLY, per x86_fc_hit's op_fc_base>=0 gate). */
    else if (bump && !x86_fb_data() && !_.op_stmt_dyn) snprintf(b[i], 48, "%s ptr [rsp# + %d]", q ? "qword" : "dword", x86_frame_off(off) + bump);   /* ZW-0 stage 2: !x86_isle() guard deleted -- isle never reached at ZC_FRAME_RSP default */   /* ARGREAD (s22w): the LIVE TEMPLATE BUMP rides AFTER the one-authority resolution, never inside the flat spelling.  The old arm baked off+bump into a plain [rsp+N], which the operand parser routes back through x86_frame_off at encode time -- so the UCLAIM owner table was asked to resolve a FICTITIOUS flat offset (slot+bump) no claim backs, declined, and the legacy fallback left the read exactly bump-high relative to the correctly-resolved bump-free twin (witness 083: slim FRQB(slot=128,32) emitted [rsp+160] while classic FRQ(128) resolved to [rsp+112]; producer wrote 112 -- the slim install read one cell high and every DEFINE formal arrived as garbage, m3 'Illegal data type').  Resolve NOW (same _ the encoder would use), add the bump, spell RAW (the sanctioned [rsp#] escape, x86_parse 1208/1209) so nothing re-resolves.  bump==0 (FR/FRQ) is byte-identical through the old arm; pinned-___ keeps its bump suppression there; isle never carries a bump (scc is ZC_FRAME_RSP-gated). */
    else { snprintf(b[i], 48, "%s%d]", q ? x86_fr64_prefix() : x86_fr32_prefix(), off + ((x86_fb_data() || _.op_stmt_dyn) ? 0 : bump)); }   /* ⭐ C-4-FIX (FINDING-08-07h, s6): mech2 blob non-fc_on nodes: push____ shifts rsp by -8 before sub rsp,Kc, so blob slots designed at [rsp+8+off]=[α_base-Kc+off] need +8. Gate: regime-4 (rsp base, !fb_data), mech2 active (op_mech2_kc>0), no own FORTH cell (!x86_fc_on — own-cell nodes use regime-2 window or TOS, not this arm). Byte-identical when op_mech2_kc==0 (_m2=0). */
    return b[i];
}   /* ZOP-1 -- THE ONE OPERAND ADDRESS AUTHORITY (Lon s21x-n: "Parameterize the access to operands via the FOUR modes" + "Do not put RSP references directly into the templates").  Every zeta operand address in the tree now resolves HERE, through ONE switch over the named regimes, instead of through three near-duplicate accessors each re-deriving the same five booleans in a slightly different order (FRQB differed from FRQ only in where it added the live bump -- a difference that was load-bearing and undocumented until the 083 define cluster).  BYTE-IDENTICAL BY CONSTRUCTION at this rung: the arms reproduce the legacy expressions exactly (arm 2 = the fc window rebase, arms 1/3/4 = x86_fr{32,64}_prefix, whose isle/___/rsp ternary already encodes them), so the regime SELECTION is centralized and named without any regime CHANGING.  That ordering is deliberate -- the seam first, then the per-family migration crawls through one function instead of five call sites. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZTOS-1 -- THE VALUE-SPINE ARM OF THE ONE OPERAND-ADDRESS AUTHORITY (Lon s21x-o: "Do not put RSP references directly into the templates" + "Keep track of sliding offsets and index operands from RSP").
 * A TOS reference names a cell the PRODUCER pushed, read by a CONSUMER that has since carved its own storage.  The address is therefore the producer's offset PLUS whatever the reader allocated beneath it
 * -- `off + _.op_zdepth` -- and that sum is the whole of the "sliding offset" bookkeeping.  IT IS LOCAL BY CONSTRUCTION: a box compensates for exactly what IT carved and knows nothing about its neighbours,
 * so no graph-wide prefix pass computes it (contrast op_flat_disp, LOWER's static prefix sum, which was the right answer only while boxes did not allocate for themselves) and arming one more BB cannot
 * displace another.  WHY THIS IS THE FIX FOR THE s21x-n NEGATIVE RESULT: with op_zdepth==0 the spelling is byte-for-byte the raw [rsp+off] the templates used to spell privately, so an unarmed box is
 * unchanged; with the box armed it slides by precisely its own K, which is the displacement the carve introduced.  The two motions cancel, which is why 028_arith_unary_minus can stop printing 5 for -5
 * without any box needing to know the depth of any other box.  Templates get ZTOS/ZTOSD and never spell "rsp" -- the private rspq/rspd helpers each template grew are exactly the duplication this retires. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * x86_ztos(int off, int q) {
    static char b[16][48]; static int i; i = (i + 1) & 15; x86_zop_note(5);
    snprintf(b[i], 48, "%s ptr [rsp + %d]", q ? "qword" : "dword", off + _.op_zdepth);
    return b[i];
}
inline const char * ZTOS(int off)  { return x86_ztos(off, 1); }
inline const char * ZTOSD(int off) { return x86_ztos(off, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZD-1 — THE FOUR MODES (Lon s21x-v: "Parameterize the access to operands via the FOUR modes" + "Do not put RSP references directly into the templates").  A ZD box's every data reference is one of exactly
 * four parameterized shapes, and each has ONE accessor: (1) OWN RESULT — the 16B DESCR cell the box's own alpha carved, at [rsp+0..15] for the whole body, ZRES/ZRESD(w); (2) OWN LOCALS — box-private RW
 * scratch carved in the same single alpha sub above the result cell, ZLOC/ZLOCD(o) = [rsp+16+o]; (3) OPERAND k — the producer's suspended result cell, at the DRIVER-STAGED difference of two depths
 * op_zread[k] (zd_plan's execution-order walk; s21x-u's law: a single depth cannot express this), ZOPQ/ZOPD(k,w); (4) LEGACY FRAME — FR/FRQ/FRQB, the whole-graph flat authority for unconverted families,
 * untouched.  Modes 1-3 spell the RAW-CELL MARKER [rsp# + N] (x86_parse 1188): the offset the driver computed IS the machine offset, no encoder-side op_flat_disp/op_zdepth compensation may touch it in
 * either medium — one authority, zero hidden additions, which is precisely the property every prior single-depth attempt lacked. */
   /* ⭐ ARB-LON-K16: RAW machine-___ qword operand, runtime offset -- the sanctioned [___# + N] escape (x86_parse s23o rule), NEVER the plain [___ + N] spelling which the fr-prefix classifier would capture and re-canonicalize under op_stmt_dyn.  Consumers: the mechanism-2 dyn whack + old____ restore at the γ-jmp zgpop hook. */
inline const char * x86_zref(int off, int q) {
    static char b[16][48]; static int i; i = (i + 1) & 15;
    snprintf(b[i], 48, "%s ptr [rsp# + %d]", q ? "qword" : "dword", off);
    return b[i];
}
inline const char * ZRES(int w)        { return x86_zref(w, 1); }
inline const char * ZRESD(int w)       { return x86_zref(w, 0); }
inline const char * ZLOC(int o)        { return x86_zref(16 + o, 1); }
inline const char * ZLOCD(int o)       { return x86_zref(16 + o, 0); }
inline const char * ZLOC_B(int o)      { return x86_zref(_.op_ztail + 16 + o, 1); }   /* ⭐ ZK-2 IR_TO BETA-RESUME DEPTH FIX (this rung): β-time twin of ZLOC -- addresses IR_TO's own counter/limit cells from current RSP AFTER downstream run members have further decremented it.  ZLOC(o)=[rsp+16+o] is correct only at α (immediately after IR_TO's sub rsp,32, before any downstream K carves).  At β resume, each downstream node has fired its own sub rsp,K, pushing RSP down by op_ztail bytes total.  IR_TO's cells remain at their ORIGINAL RSP positions (the non-popping law: cells are never moved).  ZLOC_B(o)=[rsp+op_ztail+16+o] reaches them from the deeper current RSP.  When op_ztail=0 (IR_TO is the run terminal, no downstream members), ZLOC_B(o)==ZLOC(o) -- byte-identical.  ONE AUTHORITY: op_ztail is staged once at the emit_drive choke from g_zd_ztail computed in codegen_flat_chain_body.  Use ZLOC for α-only writes (initialization before L(0)); use ZLOC_B for all reads at/after L(0) (the loop body, which may execute at β-resume depth). */
inline const char * ZOPQ(int k, int w) { return x86_zref(_.op_zread[k] + w, 1); }
inline const char * ZOPD(int k, int w) { return x86_zref(_.op_zread[k] + w, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * FR(int off)            { return x86_zop(off, 0, 0); }      /* ZOP-1: was the dword arm of the five-boolean cascade; the decision moved into x86_zop, this is now pure width+bump binding. */
inline const char * PAIR(int idx) { static char b[8][16]; static int i; i = (i + 1) & 7; snprintf(b[i], 16, "P%d", idx); return b[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_load64(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x8B; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov ") + reg + ", qword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_store64(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x89; c += x86_frame_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov qword ptr ") + x86_frame_text_mem(off) + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_frame64(int off) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, 0); c += (char)0xFF; c += x86_frame_modrm(4, off); return x86_Lrec(c); }
    return std::string(" jmp qword ptr ") + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_mov_imm64(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, 0); c += (char)0xC7; c += x86_frame_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return std::string(" mov qword ptr ") + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * FRQ(int off)           { return x86_zop(off, 1, 0); }      /* ZOP-1: qword twin of FR. */
inline const char * FRQB(int off, int bump) { return x86_zop(off, 1, bump); }   /* ZOP-1: qword + LIVE TEMPLATE BUMP (the scc save-block case).  The bump suppression under a pinned ___ (FLATDISP-8) now lives once, inside x86_zop, instead of being spelled only here -- which is why FRQB and FRQ could previously disagree about depth without anything naming the disagreement. */
inline const char * ROQ(int n)   { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rip + %d]", n); return b[i]; }
inline const char * RDQ(const char * base, int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [%s + %d]", base, off); return b[i]; }
inline const char * ABSQ(unsigned long va) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [%lu]", va); return b[i]; }   /* REG-1: pinned-island absolute (SIB no-base, va < 0x7FFFFFFF, identical bytes both mediums) */
/* GVARQ — RC-5-GVA: GVA-base-register displacement form.  When RTCC_GLOBAL_R9_GVA=1 and g_rtcc_on=1,        */
/* R9 holds RT_GVA_VA (seeded once in rtcc_init).  Returns "qword ptr [r9 + off]" where off = k*16+w.        */
/* For k≤7 the offset ≤112+8=120 < 128 → disp8 encoding (4B vs 7B for ABSQ) in the emitted binary.           */
/* When gate is OFF: caller uses ABSQ(RT_GVA_VA + k*16 + w) — byte-identical to pre-RTCC.                    */
inline const char * GVARQ(int k, int w) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [" RTCC_GVA_REG " + %d]", k * 16 + w); return b[i]; }
extern "C" const char * gva_name(int k);
extern "C" const char * bb_kind_name(int op);   /* OBJ-NOTE name sources: GVA slot k -> variable name (gva_collect.c registry); IR op -> the same lowercase kind spelling the n<uid>_<kind> labels use (emit.cpp flat_label_kind wrapper). */
inline const char * ZOPAN() { if (_.op_a_node_kind < 0) return ""; static char b[8][48]; static int i; i = (i + 1) & 7; const char * n = bb_kind_name(_.op_a_node_kind); snprintf(b[i], 48, "%s", n ? n : ""); return b[i]; }   /* OBJ-NOTE ON-2 INTERIM (Lon s23c): operand-a's PRODUCER name -- a ZOPQ(0,.)/ZOPD(0,.) read names the box whose cell it is reading, so a consumer's '[rsp+96]  # binop_arith' says WHOSE result arrived rather than leaving the reader to walk the op_zread differences by hand.  This is the ladder's sanctioned interim for ON-1: operand-a ONLY, via the op_a_node_kind that emit.cpp:867 already stages at the single dispatch point -- operands b..n still await the op_zkind[] ruling on the SHARED params struct.  Guarded on the -1 no-operand sentinel (bb_kind_name would otherwise spell "op-1"); rotating buffer for the same single-static reason as ZRESN. */
inline const char * ZRESN() { return "result"; }   /* ⛔⭐⭐ OBJ-NOTE (Lon s23f, verbatim: "You put a comment with the same name as the BB box you are in ... that is not an operand and the label should not imply such. That should say result."): this accessor USED to render bb_kind_name(_.op_node_kind) -- the CURRENT node's kind -- so a store to the box's own result cell inside `n0_lit_integer_α:` printed `# lit_integer`.  TWO DEFECTS IN ONE: (a) it merely restated the label the reader is already standing in, carrying ZERO information; (b) it was TYPOGRAPHICALLY IDENTICAL to an operand note, which names a DIFFERENT box -- so the one distinction this column exists to draw, MY cell vs SOMEONE ELSE'S cell, was precisely the distinction it erased.  ⭐ THE RULE THE COLUMN OBEYS: name the object AT THAT INSTRUCTION -- a load from a variable's storage names the VARIABLE, a store into this box's own cell is "result", an operand read names the PRODUCER.  Kept as an accessor rather than inlined at 39 sites so the term stays ONE authority. */
inline const char * ZOPN(int k) { if (k < 0 || k >= 6) return ""; int kk = _.op_zkind[k]; if (kk < 0 && k == 0) kk = _.op_a_node_kind; if (kk < 0) return ""; static char b[8][48]; static int i; i = (i + 1) & 7; const char * n = bb_kind_name(kk); snprintf(b[i], 48, "%s", n ? n : ""); return b[i]; }   /* OBJ-NOTE ON-1 (s23e): operand k's PRODUCER name -- the general form ZOPAN could only do for operand a.  Pairs with ZOPQ(k,·)/ZOPD(k,·): the read prints '[rsp+96]  # binop_arith', naming the box whose cell arrived instead of leaving a reader to walk op_zread differences by hand.  ⭐ STRICT SUPERSET OF ZOPAN, deliberately: op_zkind[] is staged ONLY on the ZD-armed arm (emit.cpp's `if (g_zd_arm)`), whereas op_a_node_kind is staged at the single dispatch point for EVERY node -- so a bare swap of ZOPAN()->ZOPN(0) would have silently DROPPED operand-a names on every unarmed node.  The k==0 fallback keeps them, making this accessor safe to use everywhere ZOPAN was and 5 operands wider.  Guarded on the -1 no-producer sentinel zd_plan's reset stages, so a slot with no producer prints NOTHING rather than inheriting the previous node's kind -- a WRONG name is worse than none, and that sentinel is the only thing standing between the two.  Rotating buffer for the same single-static reason as ZRESN/ZOPAN. */
inline const char * HKN(int k) { static const char * n[6] = { "old____", "outer_Σ", "outer_δ", "outer_Δ", "cap_gen", "zls2_mark" }; return (k >= 0 && k < 6) ? n[k] : ""; }   /* OBJ-NOTE ON-3 (s23e): the ONE naming authority for the match-construct housekeeping quintet, keyed by the SAME k the HKQ/FRQ slot accessors take (0=deep-___ 1=r13 2=r14 3=r15 4=capgen).  ⭐ WHY IT LIVES HERE AND NOT IN A TEMPLATE: the save side (bb_match_begin alpha) and the RESTORE sides (bb_match_begin's own exit, bb_match_end, bb_match_replace) are FOUR sites in THREE files reading five slots whose meaning is fixed by the k map -- s23c/s23d annotated the saves with inline string literals and left every restore bare, which is exactly how a name and its twin drift apart.  One table, k-keyed, shared by all four: a reader who renames a term renames it everywhere or not at all.  VOCABULARY IS SPITBOL'S OWN (manual pp.67-68): the subject string and the CURSOR into it are the two things a nested match must save and give back, so Sigma/delta/Delta keep the emitter's established subject-base / cursor / subject-length spelling and "outer_" marks them as the ENCLOSING match's, not this one's. */
inline const char * RDD(const char * base, int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "dword ptr [%s + %d]", base, off); return b[i]; }
inline const char * LIDX(long k)  { static char b[8][24]; static int i; i = (i + 1) & 7; if (k) snprintf(b[i], 24, "[r13+rcx+%ld]", k); else snprintf(b[i], 24, "[r13+rcx]"); return b[i]; }
inline long LITQ(long k) { uint64_t w; memcpy(&w, _.op_sval + k, 8); return (long) w; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_rd32_modrm — the reg_disp32 family's ModRM(+SIB) tail (REG-7 s80).  Bases with low3==100 (rsp, r12)
 * REQUIRE a SIB byte at every mod — the s68 pun class; until now the family either mis-encoded them silently
 * (the disp's first byte would be eaten as a SIB) or, in store_imm32 alone, aborted loudly (the 2026-07-08 s2
 * guard, retired by this helper).  SIB 0x24 = scale 00, index 100 (none), base 100 — REX.B distinguishes rsp
 * from r12.  mod10+disp32 stays unconditional, the family's uniform-shape convention (see store_imm32's note).
 * First customer: the REG-7 pend shapes — capture COND writes its 24B entry at [r12+0/8/16] directly. */
inline void x86_rd32_modrm(std::string & c, int g, int b) { c += (char)(0x80 | ((g & 7) << 3) | (b & 7)); if ((b & 7) == 4) c += (char)0x24; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_load64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8B; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return std::string(" mov ") + dst + ", qword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_store64(const char * base, int disp, const char * src) {
    int g = x86_rnum(src), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x89; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return std::string(" mov qword ptr [") + base + " + " + std::to_string(disp) + "], " + src + "\n";
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
    return std::string(" cmp qword ptr [") + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_inc_r(const char * reg) {
    int m = x86_rnum(reg); int w64 = !(reg && (reg[0] == 'e' || (reg[0] && reg[strlen(reg) - 1] == 'd')));
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = (uint8_t)((w64 ? 0x48 : 0x40) | (m >= 8 ? 0x01 : 0)); if (rex != 0x40) c += (char)rex; c += (char)0xFF; c += (char)(0xC0 | (m & 7)); return x86_Lrec(c);
    }
    return std::string(" inc ") + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_abs_disp32_load64(const char * dst, long va) {
    int g = x86_rnum(dst);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8B; c += (char)(0x04 | ((g & 7) << 3)); c += (char)0x25;
        c += u32le((uint32_t)va); return x86_Lrec(c);
    }
    return std::string(" mov ") + dst + ", qword ptr [" + std::to_string(va) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_abs_disp32_store64(long va, const char * src) {
    int g = x86_rnum(src);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x89; c += (char)(0x04 | ((g & 7) << 3)); c += (char)0x25;
        c += u32le((uint32_t)va); return x86_Lrec(c);
    }
    return std::string(" mov qword ptr [") + std::to_string(va) + "], " + src + "\n";
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
    return std::string(" cmp ") + reg + ", qword ptr [" + std::to_string(va) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_load32(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0x8B; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return std::string(" mov ") + dst + ", dword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_store32(const char * base, int disp, const char * src) {
    int g = x86_rnum(src), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0x89; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return std::string(" mov dword ptr [") + base + " + " + std::to_string(disp) + "], " + src + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_store_imm64(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0xC7; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm);
        return x86_Lrec(c);
    }
    return std::string(" mov qword ptr [") + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_reg_disp32_store_imm32 — the dword sibling of store_imm64 above (Claude Sonnet 5, 2026-07-08 session 2).
 * Added because x86("mov", RDD(base, off), imm) previously had NO dispatch arm and the mov dispatcher's
 * fall-through SILENTLY emitted nothing — found the hard way: bb_match_arb.cpp's ZLS2 α wrote its len=0 field
 * with exactly this shape, the instruction vanished from the emitted stream, and the first activation still
 * worked only because a virgin MAP_NORESERVE page reads as zero (every later activation inherited stale len —
 * the word2 PORT=2 regression, proven by gdb disassembly of the live stream, not inferred).  The mov
 * fall-through is now a loud bomb (see the dispatch block) so this class of silent drop cannot recur.
 * Encoding: optional REX.B (no REX.W — 32-bit op), C7 /0, mod10 rm=base, disp32, imm32 — mod10+disp32
 * unconditionally, matching every sibling in the reg_disp32 family (they trade a shorter disp8 form for one
 * uniform shape; `as` agreement is irrelevant here because RDD text spells the same disp the binary encodes).
 * The former (b&7)==4 abort is RETIRED — x86_rd32_modrm now emits the required SIB family-wide (REG-7 s80). */
inline std::string x86_reg_disp32_store_imm32(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0xC7; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm);
        return x86_Lrec(c);
    }
    return std::string(" mov dword ptr [") + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_reg_disp32_add_imm32 — add dword ptr [base+disp], imm (R7: mech-2 start_δ increment via RDD("___",-48)).
 * RDD("___",N) when x86_fb_data()=false parses XK_REGDISP32; x86("add",XK_REGDISP32,XK_IMM) had no dispatch
 * arm and silently emitted nothing (ZB-FC-1 drop class) — the mech-2 β start_δ increment vanished, causing
 * an infinite retry loop on every blob-armed pattern statement. */
inline std::string x86_reg_disp32_add_imm32(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex;
        if (imm >= -128 && imm <= 127) { c += (char)0x83; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += (char)(uint8_t)(int8_t)imm; }
        else                           { c += (char)0x81; x86_rd32_modrm(c, 0, b); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return std::string(" add dword ptr [") + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_lea64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8D; x86_rd32_modrm(c, g, b);
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return std::string(" lea ") + dst + ", [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_store64(int off, const char * reg) {
    int g = x86_rnum(reg); uint8_t rex = 0x48; if (g >= 8) rex |= 0x04;
    if (MEDIUM_BINARY) { std::string c; c += (char)rex; c += (char)0x89; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov qword ptr [rsp + ") + std::to_string(off) + "], " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_store32_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0xC7; c += x86_rsp_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return std::string(" mov dword ptr [rsp + ") + std::to_string(off) + "], " + std::to_string((uint32_t)imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_load64(const char * reg, int off) {
    int g = x86_rnum(reg); uint8_t rex = 0x48; if (g >= 8) rex |= 0x04;
    if (MEDIUM_BINARY) { std::string c; c += (char)rex; c += (char)0x8B; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov ") + reg + ", qword ptr [rsp + " + std::to_string(off) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_store64_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0x48; c += (char)0xC7; c += x86_rsp_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return std::string(" mov qword ptr [rsp + ") + std::to_string(off) + "], " + std::to_string(imm) + "\n";
}   /* s150: the missing QWORD-immediate rsp store.  Before this, "qword ptr [rsp + N]" + an immediate dispatched to x86_rsp_store32_imm and SILENTLY EMITTED A 4-BYTE STORE (no REX.W), zeroing only the low half of the slot -- and because that encoder's TEXT arm also spells "dword ptr", the two media AGREED with each other, so even a cross-medium byte check could not see it.  imm32 sign-extends to 64 bits, matching the hand-encoded 48 C7 /0 the xa_flat raw-byte family uses; x86_rsp_modrm already picks mod=0/1/2 so the disp width is as-matching. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_store32(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x89; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov dword ptr [rsp + ") + std::to_string(off) + "], " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_load32(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x8B; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov ") + reg + ", dword ptr [rsp + " + std::to_string(off) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_add_imm32(int off, long imm) {
    if (MEDIUM_BINARY) {
        std::string c;
        if (imm >= -128 && imm <= 127) { c += (char)0x83; c += x86_rsp_modrm(0, off); c += (char)(uint8_t)(int8_t)imm; }
        else                           { c += (char)0x81; c += x86_rsp_modrm(0, off); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return std::string(" add dword ptr [rsp + ") + std::to_string(off) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_add_to_reg32(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x03; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return std::string(" add ") + reg + ", dword ptr [rsp + " + std::to_string(off) + "]\n";
}
inline std::string x86_rsp_sub_from_reg32(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; if (g >= 8) c += (char)0x44; c += (char)0x2B; c += x86_rsp_modrm(g, off); return x86_Lrec(c); }
    return std::string(" sub ") + reg + ", dword ptr [rsp + " + std::to_string(off) + "]\n";
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
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" cmp ") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_load_indexed8(const char * dst, const char * base, const char * idx) {
    int g = x86_rnum(dst), b = x86_rnum(base), x = x86_rnum(idx);
    uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (x >= 8) rex |= 0x02; if (b >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0x8B;
    code += (char)(0x00 | ((g & 7) << 3) | 0x04);
    code += (char)((3 << 6) | ((x & 7) << 3) | (b & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" mov ") + dst + ", [" + base + " + " + idx + "*8]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_inc64(int off) {
    std::string code; code += x86_frame_rex(1, 0); code += (char)0xFF; code += x86_frame_modrm(0, off);
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" inc qword ptr ") + x86_frame_text_mem(off) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_load_mem64(const char * dst, const char * basebr) {
    char rb[8]; int k = 0; for (const char * p = basebr; p && *p && k < 7; ++p) if (*p != '[' && *p != ']' && *p != ' ') rb[k++] = *p; rb[k] = 0;
    int g = x86_rnum(dst), m = x86_rnum(rb);
    uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0x8B; code += (char)((0 << 6) | ((g & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" mov ") + dst + ", qword ptr [" + rb + "]\n");
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
enum { XK_NONE = 0, XK_REG, XK_IMM, XK_PORT, XK_ILBL, XK_FR32, XK_FR64, XK_RSP64, XK_RSP32, XK_MEMIND, XK_MEMIDX8, XK_R13RCX, XK_RIPSEAL, XK_REGDISP, XK_REGDISP32, XK_SYM, XK_ROSLOT, XK_EXTLBL, XK_PAIR, XK_ABS64, XK_MEMBI, XK_RIPGOT };
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
                                   "xmm0","xmm1" };
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
    if (!strncmp(s, "dword ptr [rsp# + ", 18)) { o.kind = XK_RSP32; o.off = atoi(s + 18); return; }   /* Z4 s8 RAW-CELL MARKER: on an UNPINNED graph x86_fr32_prefix() is the plain "[rsp + " spelling, so the fr-prefix check below captures EVERY rsp operand -- raw FORTH-cell spellings included -- and the frame encoders then add op_flat_disp.  Per-box that displacement self-cancels (store and load at the same depth reach the same displaced slot), which is why the window boxes survive; a CROSS-DEPTH cell read (capture COND -> SAVE, ZB-FC-3c) double-counts the depth difference its raw offset already encodes and reads a neighboring flat slot -- the measured capture-start defect (d79a427a..cca948c5).  The '#' marker is an escape valve: producers that mean the RAW machine rsp spell [rsp# + N]; both encoders re-canonicalize, so emitted TEXT and BINARY stay standard (R10). */
    if (!strncmp(s, "qword ptr [rsp# + ", 18)) { o.kind = XK_RSP64; o.off = atoi(s + 18); return; }
   /* s23o ___ TWIN of the rsp# escape: on a PINNED graph the fr prefixes are the plain "[___ + " spellings, so a RAW machine-___ slot (the SPD-2 retry protocol's kt-32/kt-40) parses XK_FR* and routes the frame encoders -- and a mnemonic WITHOUT an FR arm (cmp) then silently emitted NOTHING, the ZB-FC-1 drop class, caught only by the .s region diff.  [___# + N] names the raw register; both encoders re-canonicalize to standard [___ + N] in both media (R10). */

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
/* ARG-NOTE (OBJ-NOTE ON-3, Lon s23d) — name the ROLE each argument register carries at a `call rt_*` site.  The 189 argument loads were the biggest opaque family left in the .s: `mov rdi, [rsp+96]` says   */
/* WHERE the operand came from and nothing about WHAT it is.  ⭐ THE CHOKE POINT (the ON-3 lesson applied a second time): the role cannot be known when the mov is emitted — the callee is not named until the  */
/* `call` several instructions later, and the templates' `+` chains evaluate in UNSPECIFIED ORDER, so no stateful lookahead is legal.  But bb_emit_x86 hands emit_text_n the WHOLE template body in one call,  */
/* so by the time x86_4col runs over that chunk the arg loads and their `call` are both present — one BACKWARD walk names all of them, with ZERO edits to the 163 template files.  Walk stops at any label,    */
/* jump, other call, or non-arg-load instruction, so a role is never attributed across a control-flow edge; a line already carrying a `#` keeps it, which makes this idempotent under the sink's second pass    */
/* and lets the more specific hand-written notes (bb_match_begin's housekeeping vocabulary) win.  TEXT-only: x86_4col returns early for BINARY, so mode-3 bytes are untouched BY CONSTRUCTION.                   */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int x86_argreg_slot(const char * s, size_t n) {
    static const char * const r[6][4] = { { "rdi", "edi", "di", "dil" }, { "rsi", "esi", "si", "sil" }, { "rdx", "edx", "dx", "dl" }, { "rcx", "ecx", "cx", "cl" }, { "r8", "r8d", "r8w", "r8b" }, { "r9", "r9d", "r9w", "r9b" } };
    for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) { size_t l = strlen(r[i][j]); if (l == n && !strncmp(s, r[i][j], l)) return i; }
    return -1;
}
static inline const x86_argrole_t * x86_argrole_find(const char * s, size_t n) {
    for (size_t i = 0; i < sizeof(x86_argroles) / sizeof(x86_argroles[0]); i++) { const char * c = x86_argroles[i].callee; if (strlen(c) == n && !strncmp(s, c, n)) return &x86_argroles[i]; }
    return 0;
}
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
        if (ann[L]) { int w = x86_disp_w(o.data() + beg[L], end[L] - beg[L]); int pd = 88 - w; if (pd < 1) pd = 1; out.append((size_t)pd, ' '); out.append("# "); out.append(ann[L]); }
        if (end[L] < o.size()) out.append(1, '\n');
    }
    o.swap(out);
}
static inline void x86_4col_to(std::string & o, size_t ls, int col) { int w = x86_disp_w(o.data() + ls, o.size() - ls); int pd = col - w; if (pd < 1) pd = 1; o.append((size_t)pd, ' '); }
static inline int x86_4col_joinon(void) { static int j = -1; if (j < 0) { const char * e = getenv("SCRIP_ASM_JOIN"); j = (e && *e == '0') ? 0 : 1; } return j; }
static inline int x86_4col_kind(const char * p, size_t len, int * hasl) {
    *hasl = 0; size_t b = 0; while (b < len && (p[b] == ' ' || p[b] == '\t')) b++;
    const char * t = p + b; size_t tl = len - b;
    if (tl == 0) return 0;
    if (t[0] == '#') return 4;
    size_t k = 0; while (k < tl && t[k] != ' ' && t[k] != '\t') k++;
    if (k > 0 && t[k - 1] == ':') { *hasl = 1; size_t r = k; while (r < tl && (t[r] == ' ' || t[r] == '\t')) r++; if (r >= tl) return 1; t += r; tl -= r; }
    return (t[0] == 'j') ? 3 : 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_4col (2026-07-26, Lon directive; corrected same day: BBs have ALWAYS been a FOUR-column format — LABEL / OPERATOR / OPERANDS / GOTO): render every TEXT-medium assembly line in the four-column BB
 * shape — label field 24, operator field 17, operands at col 41, GOTO column at col 68.  Every jump — mnemonic 'j*': jmp + the whole jcc family + jecxz/jrcxz, an exact class in x86 — renders in the GOTO
 * column, mnemonic padded to 6.  LINE JOIN (2026-08-13, Lon directive "hold back the NEW-LINE until the next instruction comes in"): the newline is DEFERRED, never emitted with its own line, and the
 * pending line is resolved only once the FOLLOWING line's kind is known — a label-only line absorbs the instruction (or jump) beneath it, and an instruction absorbs a following jump behind a ';' at the
 * GOTO column.  The walk is FORWARD over the assembled chunk, so source order is exact; the templates' '+' chains evaluate LAST-TO-FIRST under g++ (measured -O0 and -O2), which is why this cannot be a
 * state machine consulted at x86_*() call time and why no global carries it.  '#' comment lines and empty lines pass through at the margin and never join.  x86() applies this per dispatched line (single
 * lines, nothing to join); the emit_text_n sink applies it over the whole assembled body, where the joining actually happens, and so also catches the legacy producers that do not speak x86().  Plain x86
 * TEXT only: BINARY records, MACRO_DEF, and non-x86 platforms pass untouched, so mode-3 bytes and MODE34 identity are unaffected by construction.  SCRIP_ASM_COLUMNS=0 restores verbatim; SCRIP_ASM_JOIN=0
 * restores the pre-join one-line-per-instruction shape with the GOTO column back at 88. */
inline std::string x86_4col(const std::string & s) {
    if (MEDIUM_BINARY || MEDIUM_MACRO_DEF || !PLATFORM_X86) return s;
    { static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_ASM_COLUMNS"); on = (e && *e == '0') ? 0 : 1; } if (!on) return s; }
    const int jn = x86_4col_joinon(); const int CJ = jn ? 68 : 88;
    std::string o; o.reserve(s.size() + s.size() / 2);
    std::string note, prevnote;   /* RUN-DEDUP (s23f): prevnote = the name standing on the previous INSTRUCTION line; both reset per x86_4col call (per chunk) so a run cannot leak across a bb_emit_x86 boundary. */
    size_t i = 0, n = s.size();
    int pend = 0; size_t pls = 0;   /* pend = kind of the line sitting in o[pls..] whose newline is still HELD BACK; 0 = none pending */
    while (i < n) {
        size_t e = s.find('\n', i); size_t len = (e == std::string::npos ? n : e) - i;
        const char * p = s.data() + i; size_t inext = (e == std::string::npos) ? n : e + 1;
        size_t b = 0; while (b < len && (p[b] == ' ' || p[b] == '\t')) b++;
        const char * t = p + b; size_t tl = len - b;
        if (tl >= 2 && t[0] == '#' && t[1] == '@') { note.assign(t + 2, tl - 2); i = inext; continue; }   /* OBJ-NOTE (Lon s23b): x86("note",name) rides in-band as '#@name' -- stateless across the unspecified-order '+' chains -- and folds onto the NEXT instruction line. */
        int hasl = 0; int ck = x86_4col_kind(p, len, &hasl);
        int nk = 0, nhasl = 0;
        for (size_t j2 = inext; j2 < n; ) { size_t e2 = s.find('\n', j2); size_t l2 = (e2 == std::string::npos ? n : e2) - j2; const char * p2 = s.data() + j2; size_t b2 = 0; while (b2 < l2 && (p2[b2] == ' ' || p2[b2] == '\t')) b2++;
            if (l2 - b2 >= 2 && p2[b2] == '#' && p2[b2 + 1] == '@') { j2 = (e2 == std::string::npos) ? n : e2 + 1; continue; } nk = x86_4col_kind(p2, l2, &nhasl); break; }
        int join = jn && !hasl && ((pend == 1 && (ck == 2 || ck == 3)) || (pend == 2 && ck == 3));
        int willjoin = jn && !nhasl && ck == 2 && nk == 3;   /* a jump will land on THIS line at the GOTO column, so its note must not occupy that space */
        size_t ls;
        if (join) { if (pend == 1) x86_4col_to(o, pls, 24); else o.append(1, ';'); if (ck == 3) x86_4col_to(o, pls, CJ); ls = pls; }
        else { if (pend) o.append(1, '\n'); ls = o.size(); }
        int inst = 0, isj = 0;
        if (tl == 0) { }
        else if (t[0] == '#') { o.append(t, tl); }
        else {
            size_t k = 0; while (k < tl && t[k] != ' ' && t[k] != '\t') k++;
            const char * q = t; size_t ql = tl;
            if (hasl) { size_t r = k; while (r < tl && (t[r] == ' ' || t[r] == '\t')) r++;
                if (r >= tl) { o.append(t, k); q = 0; } else { x86_4col_pad(o, t, k, 24); q = t + r; ql = tl - r; } }
            else if (!join) o.append((size_t)24, ' ');
            if (q) {
                size_t m = 0; while (m < ql && q[m] != ' ' && q[m] != '\t') m++;
                std::string op(q, m); size_t r2 = m;
                if ((m == 3 && !strncmp(q, "rep", 3)) || (m == 4 && (!strncmp(q, "repe", 4) || !strncmp(q, "repz", 4) || !strncmp(q, "lock", 4))) || (m == 5 && (!strncmp(q, "repne", 5) || !strncmp(q, "repnz", 5)))) {
                    size_t w = m; while (w < ql && (q[w] == ' ' || q[w] == '\t')) w++;
                    if (w < ql) { size_t m2 = w; while (m2 < ql && q[m2] != ' ' && q[m2] != '\t') m2++; op += ' '; op.append(q + w, m2 - w); r2 = m2; } }
                while (r2 < ql && (q[r2] == ' ' || q[r2] == '\t')) r2++;
                if (op[0] == 'j') { if (!join) x86_4col_to(o, ls, CJ); if (r2 >= ql) o.append(op); else { x86_4col_pad(o, op.data(), op.size(), 6); o.append(q + r2, ql - r2); } inst = 1; isj = 1; }
                else if (r2 >= ql) { o.append(op); inst = 1; }
                else { x86_4col_pad(o, op.data(), op.size(), 17); o.append(q + r2, ql - r2); inst = 1; }
            }
        }
        if (inst && !note.empty()) { int drop = isj || willjoin; if (!drop && note != prevnote && o.find('#', ls) == std::string::npos) { x86_4col_to(o, ls, 88); o.append("# "); o.append(note); prevnote = note; } if (!drop) { if (note != prevnote) prevnote.clear(); } note.clear(); }   /* ⛔ RUN-DEDUP (Lon s23f, verbatim: "do not repeat that comment. Just one will do."): a DESCR_t is TWO 8-byte halves and every template annotates both, so one object reference printed its name twice in a row.  The name belongs to the OBJECT, not to each half, so a note identical to the one on the previous INSTRUCTION line is suppressed and the run reads once at its head.  A jump takes no note by the drop-on-jump rule, and an instruction about to ABSORB a jump takes none either -- the GOTO column at 68 sits left of the note column at 88, so a note there would be overrun by its own line's jump. */
        pend = (ck == 0 || ck == 4) ? 4 : ck; pls = ls;
        i = inext;
    }
    if (pend) o.append(1, '\n');
    if (!note.empty()) { o.append("#@"); o.append(note); o.append(1, '\n'); }
    x86_argnote(o);   /* ARG-NOTE (ON-3, s23d): the backward walk runs on the FORMATTED lines, so it sees the same GOTO column the note fold uses and cannot disturb it — an already-noted line keeps its term. */
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_core_(const char * mnem, xop xa, xop xb, xop xc, xop xd);
inline std::string x86(const char * mnem, xop xa = xop(), xop xb = xop(), xop xc = xop(), xop xd = xop()) { return x86_4col(x86_core_(mnem, xa, xb, xc, xd)); }
inline std::string x86_core_(const char * mnem, xop xa, xop xb, xop xc, xop xd) {
    opnd a, b, c; x86_parse(xa, a); x86_parse(xb, b); x86_parse(xc, c);
    if (!strcmp(mnem, "label"))     return (MEDIUM_BINARY || MEDIUM_MACRO_DEF) ? std::string() : (std::string(xa.s ? xa.s : "") + ":\n");
    if (!strcmp(mnem, "comment"))   return std::string();   /* SN4-ASM-CRIT (Lon s173): BB emissions are COMMENT-FREE — the IR kind now lives in the node label (n<uid>_<kind>_α); statement source echo rides "srccomment", separators ride "commentrule".  All 245 template x86("comment",...) calls become pure empty strings; call-site removal is a named hygiene follow-up. */
    if (!strcmp(mnem, "note")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF || !xa.s || !xa.s[0]) ? std::string() : (std::string("#@") + xa.s + "\n");   /* OBJ-NOTE (Lon s23b): one-term object name for the NEXT instruction line, rendered '# name' in the GOTO column by x86_4col's fold; jump lines never take it (the GOTO column is theirs); BINARY = empty by construction. */
    if (!strcmp(mnem, "srccomment")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF) ? std::string() : (std::string("# ") + (xa.s ? xa.s : "") + "\n");
    if (!strcmp(mnem, "commentrule")) return (MEDIUM_BINARY || MEDIUM_MACRO_DEF) ? std::string() : (std::string("#") + (xa.s ? xa.s : "") + "\n");
    if (!strcmp(mnem, "directive")) return MEDIUM_BINARY ? std::string() : (std::string("  ") + (xa.s ? xa.s : "") + "\n");
    if (!strcmp(mnem, "raw"))       return MEDIUM_BINARY ? std::string() : (std::string(" ") + (xa.s ? xa.s : "") + "\n");
    if (!strcmp(mnem, ".quad")) {
        if (xa.tag == 2) return MEDIUM_BINARY ? x86_Lrec(u64le(xa.u)) : (std::string(" .quad ") + std::to_string((unsigned long long)xa.u) + "\n");
        if (xa.tag == 1 && xb.tag == 1) return MEDIUM_BINARY ? x86_Lrec(u64le((uint64_t)(uintptr_t)(xb.s ? xb.s : ""))) : (std::string(" .quad ") + (xa.s ? xa.s : "") + "\n");
        return std::string();
    }
    if (!strcmp(mnem, ".string")) return MEDIUM_BINARY ? std::string() : (std::string(" .string \"") + x86_asm_str_escape(xa.s ? xa.s : "") + "\"\n");
    if (!strcmp(mnem, "ret")) return MEDIUM_BINARY ? x86_Lrec(std::string(1, (char)0xC3)) : std::string(" ret\n");
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
        if (a.kind == XK_REG) {   /* SN4-FLAT-PROC (s176): jmp through a register — FF /4 (modrm 0xE0|r), REX.B for r8+; the floater's wire transfer.  Mirror of the call XK_REG arm below. */
            int m = x86_rnum(a.txt); uint8_t modrm = (uint8_t)(0xE0 | (m & 7)); uint8_t rex = (m >= 8) ? 0x41 : 0x40;
            return MEDIUM_BINARY ? x86_Lrec(std::string((char)rex == 0x40 ? "" : std::string(1, (char)rex)) + (char)0xFF + (char)modrm) : (std::string(" jmp ") + a.txt + "\n");
        }
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return std::string(" jmp ") + a.sym + "\n";
        return std::string();
    }
    if (mnem[0] == 'j') {
        if (a.kind == XK_PORT) return x86_jcc(mnem, a.port);
        if (a.kind == XK_ILBL) return x86_jcc_id(mnem, a.lbl);
        if (a.kind == XK_PAIR) return x86_jcc_pair(mnem, a.lbl);
        if (a.kind == XK_EXTLBL && xb.tag == 2) return x86_jcc_ext(mnem, (const struct bb_label_t *)(uintptr_t)xb.u);
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return std::string(" ") + mnem + " " + a.sym + "\n";
        return std::string();
    }
    if (!strcmp(mnem, "call")) {
        if (a.kind == XK_PORT) return x86_align_assert() + (MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE8)) + x86_Jrec(a.port))
                                                    : (std::string(" call ") + x86_portname(a.port) + "\n"));
        if (a.kind == XK_SYM && xb.tag == 2) return x86_rtcc_call(a.sym, xb.u);   /* RC-4: RTCC veneer choke for void/int/ptr-returning calls */
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return x86_align_assert() + std::string(" call ") + a.sym + "\n";
        if (a.kind == XK_REG) {
            int m = x86_rnum(a.txt); uint8_t modrm = (uint8_t)(0xD0 | (m & 7)); uint8_t rex = (m >= 8) ? 0x41 : 0x40;
            return x86_align_assert() + (MEDIUM_BINARY ? x86_Lrec(std::string((char)rex == 0x40 ? "" : std::string(1,(char)rex)) + (char)0xFF + (char)modrm) : (std::string(" call ") + a.txt + "\n"));
        }
        return std::string();
    }
    if (!strcmp(mnem, "call_rt")) {
        /* RC-4: RTCC veneer for DESCR_t-returning calls — RETURN-BEFORE-RELOAD law (captures rax:rdx to   */
        /* FRQ(slot):FRQ(slot+8) BEFORE the reload overwrites them).  Template form:                        */
        /*   x86("call_rt", sym, (long)slot, ptr)  where b=slot (XK_IMM) and c=ptr (XK_IMM/XK_SYM tag2)  */
        /* KILLSWITCH: gate OFF → bare call + two post-call moves (byte-identical to pre-RTCC template).   */
        if (a.kind == XK_SYM && b.kind == XK_IMM && c.kind == XK_IMM)
            return x86_rtcc_call_descr(a.sym, (uint64_t)c.imm, (int)b.imm);
        return std::string();
    }
    if (!strcmp(mnem, "call_bare")) {
        /* RC-4: emit the call instruction only, NO RTCC writeback/reload.  Used inside explicit rtcc_wb/rtcc_rl */
        /* brackets where the wb and rl are emitted separately by the template.  BOTH gates: always a bare call. */
        /* KILLSWITCH: same behaviour regardless of g_rtcc_on — this is intentionally veneer-free.               */
        if (a.kind == XK_SYM && xb.tag == 2) return x86_call_ro(a.sym, xb.u);
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return x86_align_assert() + std::string(" call ") + a.sym + "\n";
        return std::string();
    }
    if (!strcmp(mnem, "rtcc_wb")) {
        /* RC-4: emit the writeback half only (all 9 GPRs → block).  For chained post-call sequences where  */
        /* cmp/je/capture all happen between the call and the reload.  KILLSWITCH: no-op when gate OFF.      */
        if (!g_rtcc_on) return std::string();
        uint64_t block = (uint64_t)(uintptr_t)g_rtcc_block;
        return MEDIUM_BINARY ? x86_Lrec(x86_rtcc_wb_bin(block)) : x86_rtcc_wb_text();
    }
    if (!strcmp(mnem, "rtcc_rl")) {
        /* RC-4: emit the reload half only (block → all 9 GPRs).  Paired with rtcc_wb above.               */
        /* KILLSWITCH: no-op when gate OFF.                                                                  */
        if (!g_rtcc_on) return std::string();
        uint64_t block = (uint64_t)(uintptr_t)g_rtcc_block;
        return MEDIUM_BINARY ? x86_Lrec(x86_rtcc_rl_bin(block)) : x86_rtcc_rl_text();
    }
    if (!strcmp(mnem, "rtcc_anchor_cmp")) {
        /* RC-5: test r8, r8 — replaces the 3-insn [rip+rt_anchor_g@GOTPCREL] load+deref+cmp sequence.    */
        /* When gate OFF (RTCC_GLOBAL_R8_ANCHOR=0): emit the original GOT-deref sequence byte-identical.    */
        /* When gate ON: emit 'test r8, r8' (3 bytes: REX.R=1 TEST rm64,r64 with both operands r8).        */
        /* The ZF flag semantics are identical: ZF=1 iff anchor==0 (unanchored), ZF=0 iff anchored.        */
        /* The conditional branch that follows uses 'jne' (branch if anchor != 0 = anchored), unchanged.   */
        if (!g_rtcc_on || !RTCC_GLOBAL_R8_ANCHOR) {
            /* KILLSWITCH path: emit the original 3-insn sequence (byte-identical to pre-RC-5) */
            uint64_t anchor_addr = (uint64_t)(uintptr_t)(const void *)rt_anchor_ptr();
            if (MEDIUM_BINARY) {
                std::string s;
                /* movabs r11, anchor_addr  (REX.WB + B8+3 = 0x49 0xBB + 8-byte addr) */
                s += (char)0x49; s += (char)0xBB; s += u64le(anchor_addr);
                /* mov rax, [r11]  (REX.WR + 8B /0 mod=00 rm=011 = 0x4D 0x8B 0x03) */
                s += (char)0x4D; s += (char)0x8B; s += (char)0x03;
                /* cmp rax, 0  (REX.W + 83 /7 imm8 = 0x48 0x83 0xF8 0x00) */
                s += (char)0x48; s += (char)0x83; s += (char)0xF8; s += (char)0x00;
                return x86_Lrec(s);
            }
            /* TEXT: emit the canonical 3-line sequence bb_match_begin uses */
            return std::string(" mov rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]\n mov rax, qword ptr [rcx]\n cmp rax, 0\n");
        }
        /* RC-5 ON path: test r8, r8 (3 bytes: 4D 85 C0) */
        if (MEDIUM_BINARY) { std::string s; s += (char)0x4D; s += (char)0x85; s += (char)0xC0; return x86_Lrec(s); }
        return std::string(" test r8, r8\n");
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
        if (a.kind == XK_RSP64 && b.kind == XK_IMM)    return x86_rsp_store64_imm(a.off, b.imm);   /* s150 width fix: XK_RSP64 is parsed from the literal "qword ptr [rsp + N]", so it MUST reach a REX.W store; it previously shared XK_RSP32's dword encoder and narrowed the write to 4 bytes without bombing. */
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
        /* No silent fall-through (2026-07-08 session 2): an unhandled mov operand pair previously returned the
         * EMPTY STRING — the instruction vanished from the emitted stream with zero diagnostics.  That exact
         * failure shipped: bb_match_arb.cpp's ZLS2 α len=0 store (REGDISP32 dest, IMM src — the arm two lines
         * up, which did not exist) was dropped, and the program still PASSED its first activation because a
         * virgin arena page reads as zero; only re-entry exposed it (the word2 PORT=2 regression, root-caused
         * by gdb disassembly).  A mov that emits nothing is never legitimate; die with both operands named. */
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
        if (a.kind == XK_REG && (b.kind == XK_FR32 || b.kind == XK_FR64)) return x86_frame_lea(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_REGDISP)              return x86_reg_disp32_lea64(a.txt, b.base, b.off);
        if (a.kind == XK_REG && b.kind == XK_R13RCX)                return x86_lea_subj_cursor(a.txt);
        if (a.kind == XK_REG && b.kind == XK_REG)                   return x86_lea_subj_cursor(a.txt);
        if (a.kind == XK_REG && (b.kind == XK_RSP32 || b.kind == XK_RSP64)) return x86_reg_disp32_lea64(a.txt, "rsp", b.off);   /* ZB-VAL-5 (R7): lea into a FORTH cell -- x86_rd32_modrm already emits the mandatory SIB for the rsp base; mod10+disp32 stays the family's uniform-shape convention */
        if (b.txt && strstr(b.txt, "rip"))                          return x86_bomb("lea: unsealed [rip + label] operand — use the [rip + __] sealed form with (ptr,label) args");
        if (b.kind == XK_RSP32 || b.kind == XK_RSP64 || b.kind == XK_FR32 || b.kind == XK_FR64) {
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
        if (a.kind == XK_REGDISP32 && b.kind == XK_IMM) return x86_reg_disp32_add_imm32(a.base, a.off, b.imm);   /* ⭐ W-1 R7: mech-2 RDD("___",N) add — ZK_REGDISP32 add had no arm, silently dropped start_δ increment → infinite retry loop */
        if (a.kind == XK_REG && b.kind == XK_RSP32) return x86_rsp_add_to_reg32(a.txt, b.off);
        if (a.kind == XK_RSP32 && b.kind == XK_IMM) return x86_rsp_add_imm32(a.off, b.imm);
        if (a.kind == XK_FR32 || a.kind == XK_FR64 || a.kind == XK_RSP32 || a.kind == XK_RSP64 || b.kind == XK_FR32 || b.kind == XK_FR64 || b.kind == XK_RSP32 || b.kind == XK_RSP64) {
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
    if (!strcmp(mnem, "and"))    { if (b.kind == XK_IMM) return x86_and(a.txt, b.imm); return std::string(); }
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
        if (b.kind == XK_REG) {   /* movzx dst64, src8 — REX.W + 0F B6 /r; R7: new encoder for byte-reg source */
            int g = x86_rnum(a.txt), m = x86_rnum(b.txt);
            uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
            uint8_t modrm = (uint8_t)(0xC0 | ((g & 7) << 3) | (m & 7));
            if (MEDIUM_BINARY) return x86_Lrec(x86_b4(rex, 0x0F, 0xB6, modrm));
            return std::string(" movzx ") + a.txt + std::string(", ") + b.txt + std::string("\n");
        }
        return x86_movzx_subj_byte(a.txt, b.kind == XK_R13RCX ? b.off : 0);
    }
    if (!strcmp(mnem, "cmpb0"))  { (void)a; (void)b; return x86_cset_probe(); }
    if (!strcmp(mnem, "xorps"))  { return x86_xorps_xmm0(); }
    if (!strcmp(mnem, "movsd"))  {
        if (b.txt && !strncmp(b.txt, "f64:", 4)) { uint64_t bits = strtoull(b.txt + 4, 0, 10); double d; memcpy(&d, &bits, 8); return x86_set_xmm0_double(d); }
        return std::string();
    }
    if (!strcmp(mnem, "movq")) {
        if (a.kind == XK_REG && b.kind == XK_REG && a.txt && !strncmp(a.txt, "xmm", 3)) return x86_movq_xmm_r64(a.txt, b.txt);
        return std::string();
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_align_enter() / x86_align_leave() — THE alignment dance for C calls from emitted code, centralized
 * (2026-07-08 session 2).  ⭐ CORRECTED s202 — the paragraph that stood here described a mechanism that no
 * longer exists.  It read: "Saves rsp in a callee-saved register … The save register is ALWAYS the other
 * member of the {r12, ___} pair (x86_align_save): ___ when the frame is r12 … and r12 when the frame is ___,
 * which is precisely what makes the dance FRAME-SAFE under ZC_FRAME____."  EVERY clause of that is now false:
 * x86_align_save() has ZERO definitions in the tree (grep: comments only), r12 is not a zeta basis (ZC_FRAME_R12
 * deleted, ZR-RSP___-1 s201), and the ZC_FRAME____ config the sentence certified as frame-safe is #error-guarded
 * as non-running (measured s202: 13 corpus crashes).  What the pair ACTUALLY does today is a push-based dance —
 * push rsp / push [rsp] / and rsp,-16, undone by mov rsp,[rsp+8] — which touches no callee-saved register at
 * all.  Under the RSP default both halves are a no-op (see the early return below).  The reason the pair exists
 * and templates must not hand-spell the dance is unchanged.  The pair leaves flags meaningless across it
 * (push/and both touch or depend on rsp) — callers already treat a C call as a full clobber, so nothing new. */
inline std::string x86_align_enter() {
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string();   /* R12-ERAD s65 (ZB-OWN-1a G1): base ≡ 0 mod 16 (K=65544 phase pad) and every rsp motion in the body is a 16-multiple (32B HEAD cell, 16B leaf cells, 32B xfer, 16-rounded zls blocks) ⇒ rsp ≡ 0 mod 16 at every C-call site ⇒ the dance is a no-op — and MUST be one: its own pushes are what displaced every flat ref inside it.  Pat blobs (r12-island, rsp sinks) keep the dance. */
    if (MEDIUM_BINARY) return x86_Lrec(x86_b1(0x54) + x86_b3(0xFF, 0x34, 0x24) + x86_b2(0x48, 0x83) + x86_b2(0xE4, 0xF0));
    return std::string(" push rsp\n push qword ptr [rsp]\n and rsp, -16\n");
}
inline std::string x86_align_leave() {
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string();   /* R12-ERAD s65: paired with the enter no-op above */
    if (MEDIUM_BINARY) return x86_Lrec(x86_b3(0x48, 0x8B, 0x64) + x86_b2(0x24, 0x08));
    return std::string(" mov rsp, [rsp + 8]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_anchor_enter/leave — ALIGN-INV-1 (Lon 2026-07-13): the DEEP-WINDOW flavor.  A window whose interior
 * SINKS rsp and never re-raises it (the dynamic proc-frame arms: sub rsp,rax then rely on the leave to
 * release) cannot use the register-free align pair — mov rsp,[rsp+8] only works at balanced arrival, and
 * recursion makes the per-activation push of the anchor load-bearing.  This keeps the old register dance
 * VERBATIM for exactly those windows.  ___ here is a TRANSIENT push/pop-preserved scratch, NOT a resident:
 * any future resident meaning of ___ (the dcap cursor) survives the window via the push/pop pair, with the
 * global-mirror spill at these windows when that lands.  Every other window uses x86_align_enter/leave. */
inline std::string x86_anchor_enter() { return x86_align_enter(); }
inline std::string x86_anchor_leave() { return x86_align_leave(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_frame_sink/base/unsink — ___ EVICTION (Lon directive 2026-07-13: "we wanted to not use ___ at all anywhere
 * but for ASSIGNMENT STACK").  THE DYNAMIC PROC-FRAME ANCHOR, REGISTER-FREE.  The four deep windows (capture,
 * defer, release, call_proc_staged) all sink rsp by a RUNTIME fbytes (rax) to carve a callee frame, so the
 * register-free x86_align_leave (mov rsp,[rsp+8], valid only at balanced arrival) could not release them — which
 * is the ONLY reason the old anchor kept the ___ register dance, and the only thing that held ___ hostage.  The
 * fix is to make each sink carry its OWN anchor in the stack it just carved: park old-rsp in a scratch qword
 * BELOW the callee frame, hand the callee a frame base of rsp+16, and release with `mov rsp,[rsp]`.  The callee
 * (xa_flat) is ret-terminated and restores its own entry rsp, so on return rsp is exactly the parked word's
 * address — the release needs no register and no live value.  Consequences beyond freeing ___: (a) each transfer
 * now balances, so the two PUMP LOOPS (release's *VAR commit chain, defer's callout chain) no longer accumulate
 * one un-released frame per iteration — their stack use is now BOUNDED, previously it grew with the pend count;
 * (b) the window itself is balanced at leave, so anchor_enter/leave collapse into the plain register-free align
 * pair above.  rcx is a transient caller-saved scratch, dead at every sink site (rax=fbytes is the only live
 * value).  16-safe: rsp is 16-aligned on entry, sub rax + sub 16 + and -16 leaves it 16-aligned, and the parked
 * word sits AT rsp so the callee's own pushes (which go below rsp) cannot reach it. */
inline std::string x86_frame_sink() {
    return x86("mov", "rcx", "rsp")
         + x86("sub", "rsp", "rax")
         + x86("sub", "rsp", 16L)
         + x86("and", "rsp", -16L)
         + x86("mov", "qword ptr [rsp + 0]", "rcx");
}
inline std::string x86_frame_base(const char * r) {
    return x86("mov", r, "rsp") + x86("add", r, 16L);
}
inline std::string x86_frame_unsink() {
    return x86("mov", "rsp", "qword ptr [rsp + 0]");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_xfer_enter/leave (NCB-1c, 2026-07-11) — THE MATCHER-REGISTER SAVE FOR AN EMITTED BB→BB TRANSFER.
 * An xa_flat callee pushes only its frame register (+ optional display reg): it does NOT honor the SysV
 * callee-saved contract for r13/r14/r15.  The matcher holds its CURSOR in r14d and scratch in r15d, so any
 * transfer taken while a match is in progress must save them ITSELF or the callee's own nested match shoots the
 * cursor out from under it — a SILENT WRONG ANSWER, not a crash (ablation: 'AABZ' ? 'A' *F() 'Z' prints fail
 * where the oracle matches; the full crosscheck does not notice — corpus 161 exists to notice).  The old C
 * trampolines hid this by accident of GCC's own register allocation, never by design.
 * USE IT AROUND EVERY TRANSFER WINDOW EMITTED INSIDE A MATCHER BOX.  Goes OUTSIDE x86_align_enter (it pushes;
 * align_enter then 16-aligns rsp regardless, so the odd push count is harmless).  Not needed at statement-level
 * call sites (no live cursor) — that is why bcps_det_arm is safe today, and it stops being safe the moment a
 * deterministic call is reached with r14 live. */
inline std::string x86_xfer_enter() {
    if (x86_zc_frame() == ZC_FRAME_RSP) return x86("push", "r14") + x86("push", "r15") + x86("push", "r13") + x86("sub", "rsp", 8L);   /* R12-ERAD s65: 32B keeps the G1 16-align invariant; the ONE interior flat ref (RELEASE's mark read) is hand-compensated +32 at the template */
    return x86("push", "r14") + x86("push", "r15") + x86("push", "r13");
}
inline std::string x86_xfer_leave() {
    if (x86_zc_frame() == ZC_FRAME_RSP) return x86("add", "rsp", 8L) + x86("pop", "r13") + x86("pop", "r15") + x86("pop", "r14");
    return x86("pop", "r13") + x86("pop", "r15") + x86("pop", "r14");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Scan-blob choice-cell port encoders (Lon 2026-07-28).  ___ = CP (newest CHOICE cell; callee-saved, survives every C runtime call), rsp = ζ frontier, cells 16B-granular so SysV alignment holds.
 * CHOICE CELL, 32B, ascending from CP: [+0]=resume_addr · [+8]=low32 saved_δ / high32 TAG (0=CHOICE 1=BASE 2=FENCE 3=ARBNO_GUARD) · [+16]=prev_CP · [+24]=saved_MH.
 * BACKTRACKABLE-BOX LAW: carve LOCALS first, THEN push the header; the unwind tail consumes the header, so every β-resume entry sees rsp == own LOCALS base, r14d == saved δ, ___ == enclosing CP.
 * Composed ENTIRELY of pre-verified encoders: zero new byte encodings.  x86_cell_push clobbers rax and FLAGS. */
/* GLUE PROMOTED TO TEMPLATES (Lon s21x-n "Would those two glue snippets qualify for each having their own template" -- yes): the four x86_glue_* inline helpers that sat here from s21x-m are now
 * src/templates/bb_glue_flat.cpp and bb_glue_framed.cpp, one file each.  They were compositions of x86(...) calls, not encodings, which is a template's job by this tree's own taxonomy; and the FOUR-MODE
 * variance they now carry wants R6's IF(...) combinator inside one concatenation, which is a template rule.  They had ZERO callers here, so the move is byte-neutral by construction.  K is read from `_`
 * (op_fc_bytes, the ONE K AUTHORITY) rather than passed as an argument -- the allocator and x86_zop's accessor now read the SAME field and cannot hold two opinions about one box's size. */
inline std::string x86_alpha_carve(long K)          { return K > 0 ? x86("sub", "rsp", K) : std::string(); }
inline std::string x86_gamma_free(long K)           { return K > 0 ? x86("add", "rsp", K) : std::string(); }
inline std::string x86_gamma_result(long K, long n) {
    long drop = K + 16 * (n - 1);
    return x86("mov", "rax", RDQ("rsp", 0)) + x86("mov", "rdx", RDQ("rsp", 8))
         + (drop > 0 ? x86("add", "rsp", drop) : std::string())
         + x86("mov", RDQ("rsp", 0), "rax") + x86("mov", RDQ("rsp", 8), "rdx");
}
inline std::string x86_cell_push(int tag, int resume_ilbl) {
    return x86("xor", "rax", "rax") + x86("push", "rax")
         + std::string("")
         + x86("push", "r14")
         + (tag ? x86_rsp_store32_imm(4, (long)tag) : std::string())
         + x86_lea_rip_id("rax", resume_ilbl)
         + x86("push", "rax")
         + std::string("");
}
inline std::string x86_main_prologue()             { return std::string(); }   /* THE ONLY TWO. main alone keeps the C-ABI pair. */
inline std::string x86_cell_unwind_tail()           { return x86("pop", "rax") + x86("pop", "r14") + std::string("") + x86("add", "rsp", 8L) + x86_jmp_reg("rax"); }
inline std::string x86_cell_fail_body()             { return std::string("") + x86_cell_unwind_tail(); }
inline std::string x86_cell_cut_keep(const char * base) { return x86_reg_disp32_load64("rsp", base, 16) + x86_reg_disp32_lea64("rsp", base, 32); }
inline std::string x86_chain_prev(const char * dst, const char * src) { return x86_reg_disp32_load64(dst, src, 16); }
inline std::string x86_chain_tag_load(const char * dst32, const char * cell) { return x86_reg_disp32_load32(dst32, cell, 12); }
/* ZREL-1 (s21x-p, Lon: "Do not put RSP references directly into the templates"): the spine-cell CLAIM/RELEASE verbs, siblings of the ZTOS/ZTOSD address verbs (defined beside x86_ztos above; the verbs
 * live HERE because they speak x86(...), whose overloads are not yet declared at that point -- the line-419 definition-order convention).  A template that consumes an operand cell (pops it after
 * reading), carves its own mid-body locals, or releases them, speaks x86_zclaim/x86_zrelease and never spells "rsp" -- same retirement ZTOS performed on the address side.  These are MODE-INVARIANT BY
 * MEASUREMENT, not oversight: spine cells ride rsp under every current mode (s21x-m law 2, the spine IS already the cell machine -- the killswitch baseline pushes the same cells), so gating them on
 * ZC_STORAGE would CHANGE behavior where the raw spelling fired unconditionally.  When the CELL_HEAP arm lands (HZ-1), the redirect happens HERE, in one place, loudly -- not in forty template lines.
 * DISTINCT FROM THE GLUE PAIR by role: bb_glue_flat/framed_enter/leave bracket the box's OWN GRANTED cell (K = _.op_fc_bytes, fired at the ports by x86_port_hook / the drive loop); zclaim/zrelease are
 * the box's MID-BODY verbs for cells it consumes from producers or carves for itself beyond the grant.  NOT for: C-ABI alignment dances, pcall records, or the CSTACK-mode x86_zr() swap arms -- those
 * are not ζ cells and naming them so would lie. */
inline std::string x86_zclaim(long b)   { return x86("sub", "rsp", b); }   /* UNCONDITIONAL BY DESIGN: an exact retirement of the raw spelling, byte-for-byte at every value; a site that wants conditional emission wraps in IF(...) at the template level (R6's job), exactly as the sites already do. */
inline std::string x86_zrelease(long b) { return x86("add", "rsp", b); }
/* STATEMENT-FRAME FAMILY (s21x-c, design of record: ___/RSP FRAMES + FORTH-STYLE VARIABLE-LENGTH ζ CELLS — the LON DIRECTIVE block atop GOAL-SNOBOL4-BB.md; hand embodiments oracle-green in
 * SCRIP/seed/test_sno_stmt_frame_1.s and _2.s, incl. recursion + FRETURN).  GLUE-4 (s21x-p): x86_stmt_enter/leave -- the STATEMENT bracket -- are DELETED; the bracket lives as bb_glue_framed_enter/
 * leave at K=0 (bb_glue_framed.cpp), and the emit.cpp head stubs + chain-exit cuts call the glue directly, so the FOUR ___ CONSTRUCTS (STATEMENT/FUNCTION/ARBNO/FENCE1, s21x-c law 4) parameterize ONE
 * shape instead of a fourth spelling of the same three instructions.  x86_call_frame_enter = BB IR_CALL: the 32B header {[___+0] pad · [___+8] caller ___ · [___+16] γ wire · [___+24] ω wire}, wires
 * as internal labels; the jmp into the body and the IR_SAVE_RESTORE slot carve (x86_alpha_carve, above) stay template business.  x86_return/freturn_floater = IR_SAVE_RESTORE roles 1/2: cut to the
 * frame from ANY statement/BB depth, restore caller ___, jmp the wire.  Composed ENTIRELY of pre-verified encoders (push/pop/mov/add/sub dispatch arms + the named disp32 family + x86_lea_rip_id +
 * x86_jmp_reg): ZERO new byte encodings by design, so the keystone byte-verify obligation is discharged by construction.  x86_call_frame_enter clobbers rcx/rdx; the floaters clobber rcx and
 * FLAGS (add). */
/* ZW-1 TWO GLUE CODES (Lon s21x-m directive, the s21x-f dynamic-box companion made concrete): the closed pair every four-port graph invocation composes from.  FLAT = pure rsp-cell discipline, zero frame
 * -- enter carves K, leave releases K, everything else is branches to alpha/beta from gamma/omega (the port verbs).  FRAMED = the same PLUS the ___ dance forward/backward: enter saves caller ___, pins
 * ___ = the box's depth-immune base, and carves K below it with C-call 16-parity preserved (pad = ceil16(K+8)-8; K=0 IS the statement bracket -- WIRED s21x-p GLUE-4: the emit.cpp head stubs and
 * chain-exit cuts call bb_glue_framed_enter/leave directly, x86_stmt_enter/leave deleted -- and ARBNO/FUNCTION/FENCE1 conversions parameterize the same pair instead of minting new shapes).
 * Both media by construction: composed entirely of pre-verified encoder dispatch arms. */
inline std::string x86_call_frame_enter(int gamma_ilbl, int omega_ilbl) {
    return x86_lea_rip_id("rcx", gamma_ilbl)
         + x86_lea_rip_id("rdx", omega_ilbl)
         + x86("push", "rdx")
         + x86("push", "rcx")
         + std::string("")
         + x86("sub", "rsp", 8L)
         + std::string("");
}
inline std::string x86_srf_floater(int wire_disp) {
    return std::string("")
         + x86_reg_disp32_load64("rcx", "rsp", wire_disp)
         + x86_reg_disp32_load64("rsp", "rsp", 8)
         + x86("add", "rsp", 32L)
         + x86_jmp_reg("rcx");
}
inline std::string x86_return_floater()  { return x86_srf_floater(16); }
inline std::string x86_freturn_floater() { return x86_srf_floater(24); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_scan_sync_out/in (ICN-SCAN-CALL-SYNC, 2026-07-11) — THE ICON SCAN-STATE HANDOFF ACROSS A CALL.  Canonical
 * Icon (interp.r k_pos/k_subject; JCON iKeyword) holds &pos/&subject as PROGRAM-GLOBAL dynamic state: a procedure
 * call neither saves nor restores them — a callee's tab/match advance IS the caller's advance.  SCRIP caches the
 * scan env in r13/r14/r15 inside a scan sequence, while callee bodies (by-name match/tab/pos et al.) run against
 * the C globals scan_subj/scan_pos.  These two combinators are the sync at the world boundary, and the OPPOSITE of
 * x86_xfer_enter/leave above (which PRESERVES the SNOBOL4 matcher regs across a callout): sync-out publishes the
 * caller's live δ (r14, 0-based) to scan_pos before any transfer that may run scan code; sync-in re-reads scan_pos
 * into r14 after it returns, both success and failure paths.  Emit-time gated on g_scan_regs_live (the
 * bb_keyword_icon precedent) so non-scan call sites emit nothing.  _rr variant brackets the reload with rax/rdx
 * push/pop so a just-returned DESCR_t survives; two pushes keep 16-byte call alignment. */
extern "C" void rt_scan_sync_out(uint64_t delta);
extern "C" uint64_t rt_scan_sync_in(void);
extern "C" int g_scan_regs_live;
inline std::string x86_scan_sync_out() {
    if (!g_scan_regs_live) return std::string();
    return x86("mov", "rdi", "r14") + x86("call", "rt_scan_sync_out", (uint64_t)(uintptr_t)(void *)rt_scan_sync_out);
}
inline std::string x86_scan_sync_in_rr() {
    if (!g_scan_regs_live) return std::string();
    return x86("push", "rax") + x86("push", "rdx")
         + x86("call", "rt_scan_sync_in", (uint64_t)(uintptr_t)(void *)rt_scan_sync_in)
         + x86("mov", "r14", "rax")
         + x86("pop", "rdx") + x86("pop", "rax");
}
/* ICN-SCAN-CALL-SYNC (?-less callee, 2026-07-22): UNCONDITIONAL variants of the two above, NOT gated on
 * g_scan_regs_live. A by-name scan builtin (tab/move/upto/many/any/find/match/pos/bal via rt_call_arr) ALWAYS
 * reads/writes the scan_pos/scan_subj globals, so it must hand off the register-world cursor (r14) even when the
 * emitter judged the box "not in scan" — the classic case being a ?-less scanning callee (JTRAN's preproc_scan_text
 * / lex_yylex0), whose scan ops are structurally in_scan=0 because the governing ? lives in the *caller*. Without
 * the handoff the inline scan primitives (register-world r14) and the by-name dispatch (global scan_pos) desync,
 * and a later sync_out publishes a stale r14=0, resetting &pos to 1 -> the co-expression re-scans the same line
 * forever -> coexpr-stack overflow. Same doctrine as the unconditional &pos:=/&subject:= reload in bb_keyword_assign;
 * a stray sync outside any real scan is harmless (r14 is dedicated; scan_pos is re-established at the next ? enter). */
inline std::string x86_scan_sync_out_force() {
    return x86("mov", "rdi", "r14") + x86("call", "rt_scan_sync_out", (uint64_t)(uintptr_t)(void *)rt_scan_sync_out);
}
inline std::string x86_scan_sync_in_rr_force() {
    return x86("push", "rax") + x86("push", "rdx")
         + x86("call", "rt_scan_sync_in", (uint64_t)(uintptr_t)(void *)rt_scan_sync_in)
         + x86("mov", "r14", "rax")
         + x86("pop", "rdx") + x86("pop", "rax");
}
/* Names whose by-name dispatch touches the scan globals; used to decide when the forced sync above is required. */
inline bool x86_is_scan_builtin_name(const char *fn) {
    if (!fn) return false;
    return !strcmp(fn, "tab")  || !strcmp(fn, "move") || !strcmp(fn, "upto") || !strcmp(fn, "many")
        || !strcmp(fn, "any")  || !strcmp(fn, "find") || !strcmp(fn, "match")|| !strcmp(fn, "pos")
        || !strcmp(fn, "bal");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_zeta_free_call() definition (forward-declared above x86_jmp; see that declaration's comment for why
 * this must live here, after x86() itself is defined). */
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_zls2_mark_save(off) / x86_zls2_release_to_call(off) — the ZLS2 statement-scope backstop pair (Claude,
 * 2026-07-08 continuation session), the ZC_PORT_ALLOC twins of x86_zeta_mark_call/x86_zeta_release_to_call
 * below.  Both return the EMPTY string unless the port mode is ZC_PORT_ALLOC (SCRIP_ZETA_PORT=2), so every
 * PLAIN/INSTRUMENTED compile is byte-identical with these spliced into a template — the same gated-helper
 * idiom as x86_arbno_role0_alloc (bb_match_arbno.cpp).  mark_save: bare call (rax = cursor) + store to the
 * caller's granted frame quad — placed where alignment is already the caller's problem solved (the head α
 * makes the sibling rt_zls_mark call bare at the same point).  release_to_call: rdi load + bare call — the
 * caller splices it INSIDE its own existing push-___/and-rsp aligned window, immediately after its sibling
 * rt_zls_release_to call, so no second alignment dance is emitted. */
extern "C" char *g_zls2_cur;
/* x86_zls2_cur_lea(reg) — address of the exported ZLS2 cursor cell into reg, the ZC_PORT_INLINE primitive.
 * Medium split is the sanctioned RO-load exception (R10): TEXT `lea reg,[rip+g_zls2_cur]` (resolved by the
 * -no-pie link against libscrip_rt), BINARY movabs of the in-process address. */
inline std::string x86_zls2_cur_lea(const char * reg) {
    return x86_load_ro(reg, "g_zls2_cur", (uint64_t)(uintptr_t)(void *)&g_zls2_cur);
}
inline std::string x86_zls2_mark_save(const char * slot) {   /* ⭐ SLACK-RIDER RE-HOME (this session): the parameter is now the SLOT SPELLING, not an int offset.  The helper never knew which home was correct -- only the caller knows whether its graph took the stfh negative-home arm -- so it baked slot and every caller inherited the positive slack home whether or not that home was backed by anything.  Passing the spelling keeps ONE authority (the caller decides once, both media, every port arm) and makes the mark/release pair incapable of disagreeing about where the mark lives, which is the split-ends failure the HKN(k) table was introduced to prevent on the quartet. */
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
inline std::string x86_zls2_release_to_call(const char * slot) {
    /* CSTACK: this helper is spliced INSIDE the caller's open alignment-dance window (both consumers:
     * bb_match_begin fail arm, bb_match_end success arm), with another C call between this fragment and
     * the caller's dance-leave.  A bare `mov rsp, slot` here is (a) clobbered by the caller's leave and
     * (b) worse, leaves the intervening C call pushing its return address just below the MARK — on top of
     * live stack (the first CSTACK cut segfaulted exactly there, 038_pat_literal m4).  So: CLOSE the
     * caller's window, restore rsp to the mark, RE-OPEN a fresh window at the new position — the caller's
     * remaining call runs safely below the re-based window, and the caller's own dance-leave then lands rsp
     * exactly at the mark. */
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
/* x86_zls2_release_to_rspd -- ZB-FC-3d: the CROSS-BOX variant of the cstack arm above, for a consumer (RELEASE) standing at rsp = frontier - fp(pattern) reading the statement mark out of HEAD's
 * self-pushed rsp CELL at [rsp + disp] (disp = fp(pattern) + 16).  Same close/restore/re-open choreography for the same reason; the mark is the PRE-PUSH rsp, so the one mov releases HEAD's cell and
 * every suspended pattern cell together (S10e's UNWIND, now cell-resident).  FORTH-grant-only by construction: call sites gate on the fc_head registrar, which only fills under the eligibility walk. */
inline std::string x86_zls2_release_to_rspd(int disp) {
    if (!x86_port_cstack()) return x86_bomb("x86_zls2_release_to_rspd: FORTH-grant-only helper reached on a non-cstack port");
    static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp + %d]", disp);
    return x86_align_leave()
         + x86("mov", "rsp", b[i])
         + x86_align_enter();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void *rt_zls2_push(long k);
extern "C" void  rt_zls2_pop(long k);
extern "C" void *rt_zls_alloc(long bytes);
extern "C" void  rt_zls_release(void *fb);
extern "C" void  rt_zls_arbno_step1_store(void *p);
extern "C" void *rt_zls_arbno_step1_load(void);
/* x86_zls2_push_call(k) / x86_zls2_pop_call(k) — the ZLS2 arena's per-ACTIVATION bump pair (promoted from
 * bb_match_arbno.cpp's private statics of the same name to here, 2nd consumer session, so bb_match_arb.cpp can
 * share them without duplicating the alignment-dance idiom — RULES.md's NO-DUPLICATED-LOGIC rule; zero
 * behavior change for ARBNO, confirmed by the byte-identical rebuild this same edit's commit records). Bare
 * alignment-safe call (push ___/and rsp,-16/restore, the x86_zeta_free_call convention) wrapping rt_zls2_push/
 * pop (zeta_alloc.c): push returns the new block pointer in rax; pop needs no return value, no size lookup, no
 * header — k is always the CALLER's own emit-time frame constant. Not gated on x86_port_mode() itself (unlike
 * x86_zls2_mark_save/release_to_call above) because every caller already lives inside its own
 * `if (x86_port_mode() == ZC_PORT_ALLOC)` branch — gating here too would be a redundant, silently-correct-but-
 * confusing second check. */
inline std::string x86_zls2_push_call(long k) {
    return x86_align_enter()
         + x86("mov",  "rdi", k)
         + x86("call", "rt_zls2_push", (uint64_t)(uintptr_t)(void *)(void * (*)(long))rt_zls2_push)
         + x86_align_leave();
}
inline std::string x86_zls2_pop_call(long k) {
    return x86_align_enter()
         + x86("mov",  "rdi", k)
         + x86("call", "rt_zls2_pop", (uint64_t)(uintptr_t)(void *)(void (*)(long))rt_zls2_pop)
         + x86_align_leave();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_port_hook DEFINITION (declared at the site enum; the seam doc lives there).  ZLS2 FRAME PROTOCOL (Lon
 * directive 2026-07-08 session 3: "code injection of stack frame bump, restore on backtrack, and release on
 * exit ... the templates stay untouched") — the whole protocol is injected at the templates' EXISTING port
 * calls; no template names any of this.  Keyed on the per-node zls2_geom grant (op_zls2_{ops,slot,bytes},
 * zeta_storage.c), fully inert at ZC_PORT_PLAIN and for ungranted nodes in every mode.  The arms:
 *   BUMP (DEF+α, ops&ZLS2_BUMP)     rt_zls2_push(K); block->prev = save-slot; save-slot = block.  A fresh
 *                                   per-activation arena frame, prev-chained for same-node nesting (DP-7).
 *   RESTORE (DEF+β, ops&RESTORE)    rt_zls2_release_to(save-slot block): a β define IS the backtrack
 *                                   arrival, and everything the failed successors allocated below this
 *                                   frame is dead — one cursor reset reclaims it all.  This is the
 *                                   FAIL-DIRECTION release: a mid-pattern node's exhaust ω lands on its
 *                                   predecessor's β, and it is THAT restore which frees the exhausted
 *                                   frame (the stack discipline: reclamation belongs to the frame you fail
 *                                   INTO).  Idempotent (release_to at the cursor is a no-op), and
 *                                   rt_zls2_release_to hard-aborts on a LIFO violation — a free assert.
 *   RELEASE (JMP+ω, ops&RELEASE)    unchain (save-slot = block->prev) + rt_zls2_release_to(block+K): the
 *                                   frame itself dies when failure leaves the construct for good.  ⛔ Gated
 *                                   ONLY by the grant, NEVER by op_omega_is_death — the chain-window
 *                                   classifier is recorded BROKEN (bb_match_arbno.cpp's L(9) note: an
 *                                   IR_MATCH_BEGIN sharing the window makes role 2 look resolved).  zls2_geom
 *                                   grants RELEASE only to roles whose jmp-ω sites are STATICALLY all
 *                                   activation-death (ARB's single exhaust ω; ARBNO role 2's single outer-
 *                                   fail ω) — role knowledge is the classifier, so the six-decoy-ω trap
 *                                   (ARBNO roles 0/1's ω = body-entry/exhaust ALIASES) cannot fire it.
 * Register contract: rax/rcx/rdi + C-call caller-saved clobbered at DEF sites and before a port jmp — dead
 * at both by this codebase's convention (r12-r15/rbx/___ callee-saved through the rt_zls2 calls).  The
 * X86H_JCC flags contract is untouched: no frame arm fires at JCC.  γ carries NO frame arm (success hands
 * the live frame down); the seam still fires there for future trace/assert flavors.  The dormant direct-sub
 * α arm (bytes>0 && ops==0) is the recorded future full-discipline design — preserved, disjoint from the
 * C-call scheme (zls2_geom grants always set ops). */
extern "C" void rt_zls2_release_to(void *);
inline std::string x86_zls2_release_to_reg(const char * reg, long disp) {
    return x86_align_enter()
         + (disp ? (x86("mov", "rdi", reg) + x86("add", "rdi", disp)) : x86("mov", "rdi", reg))
         + x86("call", "rt_zls2_release_to", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls2_release_to)
         + x86_align_leave();
}
/* GLUE-3 (Lon s21x-o): the two glue codes, forward-declared so x86_port_hook can route the per-BB allocation through them.  Defined in src/templates/bb_glue_flat.cpp and bb_glue_framed.cpp -- templates, not
 * encoders, because the four-mode variance they carry wants R6's IF() combinator inside one concatenation.  The FRAMED pair is declared beside the FLAT pair deliberately even though only FLAT is wired at this
 * rung: the framed glue is the ___ half of "finish the RSP/___ once and for all", its customers are closed by the s21x-c design of record (STATEMENT/FUNCTION/ARBNO/FENCE1 -- the four constructs whose extent is
 * not knowable at emit time), and wiring it is a LAYOUT CONTRACT change (it establishes ___, so the graph's prologue must save/seed ___ in the same breath -- the s21x-m measured SEGV), not a spelling switch. */
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
inline std::string x86_port_hook(int site, int port) {
    std::string s;
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
    /* ZC_PORT_INLINE (ZETA-INLINE first slice, 2026-07-08 s7) — the IDENTICAL grant-keyed protocol, zero C
     * calls: raw arithmetic on the exported g_zls2_cur cell (see zeta_choices.h for what the flavor drops).
     * Same sites, same order (BUMP saves AFTER the decrement — the slot holds THIS activation's base), same
     * register contract (rax/rcx/rdi only, dead at DEF sites and before a port jmp; no arm at JCC — the
     * FLAGS CONTRACT is untouched by construction since these movs are jmp/def-site only).  ⛔ RULING (Lon,
     * 2026-07-08 s7, same session): ONLY α AND ω PARTICIPATE — no β arm in either flavor; the ω that lands
     * on a β has already put the cursor right (its RELEASE fired at death), so a β restore is a no-op by
     * construction.  ZLS2_RESTORE is never granted (zls2_geom); no hook site keys on it. */
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
    /* ZC_PORT_CSTACK (C-STACK rung, Lon 2026-07-09) — the INLINE protocol with the cursor cell RETIRED: the
     * cursor IS rsp.  Same grants, same sites, same order (BUMP saves AFTER the decrement), same register
     * contract (rax/rcx only, dead at DEF sites and before a port jmp; no arm at JCC — FLAGS CONTRACT holds
     * by construction).  Bumps round to 16 (k16) so ambient call-site alignment mod 16 survives for the
     * bare-call idiom; RELEASE pops with lea rsp,[block+k16] — the same wholesale semantics as the arena's
     * release_to (everything at and below block dies).  See zeta_choices.h ZC_PORT_CSTACK for the design
     * record and the inherited suspended-frame limit. */
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
    /* ZC_PORT_FORTH FIXED CELL (RUNG ZB-FC-0, Lon 2026-07-12; ARCH-ZETA S10a/S10c): the granted box's own
     * fixed 16-multiple cell rides rsp -- alpha pushes (sub rsp,K; flags dead at a DEF site), EVERY omega pops
     * (add rsp,K before the jmp; flags dead before an unconditional transfer; conditional omegas arrive here
     * through the x86_jcc invert+pop+jmp synth, so this ONE arm is every exit path's pop -- S10b G3 visible).
     * gamma SUSPENDS the cell (S10c law: live through all gamma/beta cycling); beta emits NOTHING (pure LIFO
     * put rsp back at the frontier -- the s7 alpha/omega-only ruling, re-derived for cells).  16-multiple K
     * preserves ambient call alignment (S10a item 6), same argument as the CSTACK k16 above. */
    /* GLUE-3 WIRED (Lon s21x-o "The actual allocation is always tied through x86_alpha() and x86_beta()" + "Do not put RSP references directly into the templates").  The four lines that used to spell
     * x86_sub/x86_add("rsp", K) inline HERE now route to the two glue templates promoted at s21x-n, which had been declared, built and left with zero callers.  Three things change and one deliberately does not:
     * (a) the allocation is expressed ONCE, in a template, where R6's IF() combinator can carry the four-mode variance -- x86_asm.h goes back to owning ENCODINGS only, which is the taxonomy the project already
     * states; (b) ZC_STORAGE becomes LOAD-BEARING AT THE ALLOCATION SITE -- until now the hook allocated identically under all four modes, so FRAME_R12/FRAME_RSP would have carved a per-BB cell on top of a
     * whole-graph frame they had already carved (the roman anti-pattern from the other direction) and CELL_HEAP would have silently taken the rsp arm instead of its rbx frontier; the glue answers each mode
     * explicitly and BOMBS on the unimplemented one rather than emitting plausible-but-wrong code, which is the x86_fc_hit silent-fallback lesson; (c) the carve is now reached through x86_alpha/x86_beta exactly
     * as Lon specifies.  WHAT DOES NOT CHANGE: under the compiled default (CELL_STACK) bb_glue_flat_enter emits precisely `sub rsp, op_fc_bytes` and _leave precisely `add rsp, op_fc_bytes`, so this rung is
     * BYTE-IDENTICAL BY CONSTRUCTION -- the seam moves, no bytes move, which is what makes it separable from the ZTOS measurement above.  The carve-only/windowed asymmetry (zwco) stays HERE and is not pushed
     * into the glue: it is a property of THIS NODE's grant, not of the allocation mechanism, and the glue's own header comment is explicit that the leave side is not the mirror of the enter side. */
    if (x86_fc_on()) {
        int zwco = _.op_fc_base < 0 && !_.op_zres;   /* ZW-1 CARVE-ONLY class (Lon s21x-m "across the board, then crawl"): bytes granted, NO window (base -1) -- the cell is UNREFERENCED, so gamma suspension (S10c) buys nothing and costs depth: every flat [rsp+off] read below a suspended carve-only cell is displaced (the armed-run 023_arith_add witness).  The carve-only discipline is the full BRACKET: both entries allocate (alpha AND beta), both exits free (gamma AND omega) -- rsp-neutral at every box boundary, which is what makes across-the-board arming survivable while window migration crawls per family.  WINDOWED cells (base >= 0) keep S10c suspension verbatim: their gamma-live state is the whole point.  ZD-1 (s21x-v): an op_zres box is the THIRD class -- its cell IS referenced (its consumers' op_zread differences name it), so it takes the SUSPENDED discipline regardless of window: alpha carves, beta does NOT re-carve (the cell survived suspension, pure LIFO), gamma does NOT release (consumers read it), omega releases own K + op_wpop restores to statement entry.  The !op_zres conjunct is that whole discipline in one term. */
        if (site == X86H_DEF && port == X86P_ALPHA) s += bb_glue_flat_enter();
        if (zwco && site == X86H_DEF && port == X86P_BETA) s += bb_glue_flat_enter();
        if (zwco && site == X86H_DEF_PAIR && port == X86P_BETA && !_.op_pair_rejoin) s += bb_glue_flat_enter();   /* ALT-NARY REJOIN (s22j): a PAIR define keeps the carve-only beta claim by DEFAULT -- ARBNO's per-iteration pair entries are real claims (the rsp linked-frame chain) and lose nothing here, so this stays byte-identical for every kind except the one that opts out.  op_pair_rejoin (ALTERNATE only, set in emit.cpp's choke) suppresses it, because ALT's pair defines are rejoins inside ONE traversal and each spurious claim drifts rsp down with no matching free. */
        if (zwco && site == X86H_JMP && port == X86P_GAMMA) s += bb_glue_flat_leave();
        if (site == X86H_JMP && port == X86P_OMEGA && !_.op_wsteal) s += bb_glue_flat_leave();   /* END-JMP STEAL (HQ 2026-08-03): a stolen ω lands on IR_STATEMENT_END, the SOLE release authority -- its zgpop frees own K + wp in ONE add, so the per-edge flat_leave here must stand down or the extent double-frees.  The wpop arm below needs no guard: the steal zeroes it at staging, the ZW-5 spelling. */
    }
    if (site == X86H_JMP && port == X86P_GAMMA && _.op_zgpop > 0) s += x86_add("rsp", (long)_.op_zgpop);   /* ⭐ ARB-LON-K16 (op_stmt_dyn): a statement whose extent is indeterminable (FRAMELESS_K ARBNO growth) releases via mechanism-2 off the stmt_base -- lea rsp,[___+K] == pre-claim frontier at ANY depth (___==claim base, the [___+pin] old____ reads are the standing proof); old____ restore ORDERED AFTER the lea (lea reads ___, does not clobber it).  Legacy add arm verbatim otherwise -- killswitch-inert byte identity. */   /* ZD-1 STATEMENT-TERMINAL GAMMA RELEASE (Lon s21x-v "ONLY statement level scoping"): the success-path twin of the op_wpop arm below -- when this gamma edge crosses a statement boundary, every ZD cell the statement suspended is dead, and this ONE add returns rsp to statement entry before the jmp lands on the next head.  Fires only at jmp-gamma (flags dead before an unconditional transfer; conditional gammas that need it arrive through the x86_jcc invert synth exactly as conditional omegas do), staged by zd_plan, zero when the edge stays inside the statement.  ⭐ HEAD-PIN (s22z): when the statement's match head pinned ___ to the region base (op_stmt_pin = the old-___ slot's region offset), the restore rides HERE -- the sole release authority -- read depth-free through the still-pinned ___ BEFORE the pop; the exhaust/release tails' own +40 restores are gated off under the pin so exactly one authority restores on every exit path, including the direct af/β cuts that bypass those tails entirely (the 061 m3 spin witness). */
    /* BP-9 (ii) ΣK ζ-POP FOLD (the rung's accumulate mechanism): op_wpop = the summed fc-cell pops of every
     * whitelisted trivial-β trampoline the driver's ω-wire chase inlined past (flat_trivial_beta, emit.cpp)
     * -- node_ω already retargeted to the chain's final label, so this ONE add IS the collapsed trampoline
     * bodies.  Fires ONLY at jmp-ω (conditional ω arrives through the x86_jcc invert synth, whose inner
     * x86_jmp lands here -- the FLAGS CONTRACT is honored by construction).  Ordered AFTER the box's own fc
     * pop: own cell first, then the chased cells, exactly the walking order the trampolines performed. */
    if (site == X86H_JMP && port == X86P_OMEGA && _.op_wpop > 0) s += x86_add("rsp", (long)_.op_wpop);   /* ⭐ HEAD-PIN (s22z): restore gated on op_wterm -- ONLY the planner's statement-terminal wpop (zd_wp>0 at the choke, "restores to statement entry" by ZD-1's definition) carries the pin restore; a trampoline-ΣK-only fold is mid-statement and restoring there would corrupt the live pin for every later [___+off] reader of the same statement. */
    if (site == X86H_DEF && port == X86P_ALPHA && _.op_zls2_bytes > 0 && _.op_zls2_ops == 0 && x86_port_mode() == ZC_PORT_ALLOC)
        s += x86_sub(x86_zr(), _.op_zls2_bytes);
    /* HOME-RBX X-3 s40 FIELD-ATTRIBUTION TRACE (env-gated, inert at HEAD -- s39/s37 cursor instruction:
     * "do NOT guess-patch REG-4b" until op_fc_bytes vs op_zls2_bytes is TRACED, not inferred from a
     * --dump-zeta table read by eye).  Fires once per X86H_DEF/ALPHA dispatch, before REG-4b's own hk
     * selection reads the same fields two lines below, so this prints exactly what hk is about to see. */
    if (site == X86H_DEF && port == X86P_ALPHA) {
        static int on = -1;
        if (on < 0) { const char *e = getenv("SCRIP_RBX_FIELD_TRACE"); on = (e && *e == '1') ? 1 : 0; }
        if (on) fprintf(stderr, "[RBX-FIELD] port=%d zls2_bytes=%ld zls2_ops=%ld fc_bytes=%ld fc_base=%ld\n",
                         x86_port_mode(), (long)_.op_zls2_bytes, (long)_.op_zls2_ops, (long)_.op_fc_bytes, (long)_.op_fc_base);
    }
    /* REG-4b (s78) -- HEAP-ZETA alpha, rbx PROMOTED (C2's second flavor; the pend park->promote pattern,
     * REG-2 -> REG-6 proven): rbx IS the live bump frontier (the s73 map's GC-TOP tenant).  rax = rbx (the
     * box's base), bump = add rbx,K (register arithmetic, no cell store on the fast path), guard ja ->
     * rt_zh_bump_slow refill, which publishes the fresh frontier into [RT_WS_TOP] -- the slow path RELOADS
     * rbx from that cell, so the cell is the SYNC POINT, live only across the refill (and at the outer-graph
     * seed, xa_flat).  Cold/garbage rbx is NOT self-healing (garbage+K vs limit is unsigned luck, not law):
     * the outer flat prologue SEEDS rbx from [RT_WS_TOP] (zero page -> rbx=0 -> first guard trips -> lazy
     * init preserved); inner entries (jmp-entry procs, EVAL fragments) inherit rbx live through C's
     * callee-saved contract and seed NOTHING.  Omega emits NOTHING by C3 design (LIVE/DEAD lifecycle owns
     * reclamation).  Frame plumbing for heap residence = HZ-1's census slice.  Internal labels 60/61 sit far
     * above any box's own L(n) usage.  HZ-1 SLICE 1 (s78): the fc_geom grant class IS the heap arm's client
     * under ZC_PORT_HEAP -- C2 verbatim: the SAME static-K grant, flavor selected at the port layer (FORTH
     * spends it as sub rsp,K; HEAP spends it as the rbx bump).  fc_geom is port-blind (geometry authority);
     * every fc consumer is FORTH-gated (x86_fc_on / x86_fc_hit), so under HEAP the pops stay silent and the
     * locals stay FLAT-FRAME -- the box's heap block is allocated-but-unread, the PROVING configuration:
     * frontier arithmetic + slow refills exercised corpus-wide, semantics untouched.  Heap RESIDENCE (FR
     * translation to the block base) is HZ-1 slice 2.  The zls2 bytes>0/ops==0 key stays first in the
     * selector -- the recorded-future direct-sub discipline keeps its lane. */
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
    /* RUNG ZB-OWN-0 (Lon 2026-07-11) -- UNIVERSAL BB-owned shadow, absolute cell positioning: cell =
     * statement-mark - C_i at every alpha AND beta define of an entry-holding node.  DEF sites only (flags
     * clean); beta idempotent; no omega code by design.  rax/rcx/rdi, the stated DEF-site register contract. */
    if (x86_port_mode() == ZC_PORT_OWNED && (site == X86H_DEF || site == X86H_DEF_PAIR) && (port == X86P_ALPHA || port == X86P_BETA) && _.op_own_mark >= 0 && _.op_own_ci > 0)
        s += x86_zls2_cur_lea("rdi")
           + x86("mov", "rax", RDQ("rdi", 0))
           + x86("mov", "rcx", FRQ(_.op_own_mark))
           + x86_sub("rcx", (long)_.op_own_ci)
           + x86_own_floor_store();
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_zeta_mark_call(off) / x86_zeta_release_to_call(off) — graph-scope BB-OWNED-zeta mark/release_to calls,
 * pure x86() concatenation, same as x86_zeta_free_call above and every other encoder in this file: zero
 * manual MEDIUM_* branching in these two functions themselves. rt_zls_mark's return (rax) is stashed at
 * [r12+off]; rt_zls_release_to reads it back from there. Callers MUST emit the returned string via
 * bb_emit_x86() (the tag-framed L/J/D/E/F/X decoder below), never via a raw byte-copy loop — x86()'s
 * MEDIUM_BINARY output is a tagged record stream, not literal bytes, and only bb_emit_x86() understands the
 * framing. (This was gotten wrong once already this session: an earlier version's raw output was fed to
 * xa_flat.cpp's own xa_emit_one(), a literal-byte-copy loop with no decoder for this framing — the resulting
 * corrupted stream reproduced this project's prior 108-test regression, 023_arith_add.sno segfaulting on a
 * plain assignment. Confirmed via hex dump of the corrupted bytes, not assumed; then confirmed the fix by
 * finding bb_emit_x86's actual tag loop, the same consumer every ordinary bb_*.cpp template already uses.) */
inline std::string x86_zeta_mark_call(int off) {
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string(); /* R12-ERAD: no heap in the BB equation — the FORTH frame IS the zeta; anchor slot already holds the rsp snapshot */
    return x86("push", "rsi")
         + x86_align_enter()
         + x86("call", "rt_zls_mark", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_zls_mark)
         + x86_align_leave()
         + x86("mov",  FRQ(off), "rax")
         + x86("pop",  "rsi");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zeta_release_to_call(int off) {
    if (x86_zc_frame() == ZC_FRAME_RSP) return std::string(); /* R12-ERAD: no heap release — the FORTH frame unwinds via add rsp,K at ω */
    return x86_align_enter()
         + x86("mov",  "rdi", FRQ(off))
         + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_release_to)
         + x86_align_leave();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void rt_bomb(const char * msg);
inline std::string x86_bomb(const char * msg) {
    const char * m   = msg ? msg : "(unimplemented box)";
    const char * lbl = emit_intern_str(m);
    char lblbuf[24];
    if (!MEDIUM_BINARY && (!lbl || !lbl[0])) { strtab_label(lblbuf, sizeof lblbuf, m); lbl = lblbuf; }
    uint64_t     fp  = (uint64_t)(uintptr_t)(void *)rt_bomb;
    return x86_load_ro("rdi", lbl, (uint64_t)(uintptr_t)(const void *)m)
         + x86_call_ro("rt_bomb", fp)
         + (MEDIUM_BINARY ? x86_Lrec(x86_b2(0x0F, 0x0B)) : std::string(" ud2\n"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline bb_label_t * x86_pair_tgt(int idx) { return bb_label_fold(g_emit.xa_bb_emit_pair_jmp[idx] ? g_emit.xa_bb_emit_pair_jmp[idx] : g_emit.xa_bb_emit_pair_define[idx]); }   /* ZB-FC-3a: a jmp to a DEFINE-only pair (the sigma pad stubs' jmp na_s) targets the define label -- previously '??'/skipped patch = silent SEGV class */
inline std::string x86_pair_jmp(int idx) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += (char)(unsigned char)idx; return r; }
    return std::string(" jmp ") + (x86_pair_tgt(idx) ? x86_pair_tgt(idx)->name : "??") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* x86_pair_loop — the DRIVE_PAIR define/jmp flush, the FOURTH port-emission site (the other three are
 * x86_deflabel/x86_jmp/x86_jcc).  Each flushed define is a β label: the hook fires with (X86H_DEF, β) so
 * future DEF-site flavors reach pair-flushed β defines with zero extra wiring.  The explicit x86_port_canary
 * line predates the hook (D13's β-arrival assert, INSTRUMENTED mode) and is kept as this site's own flavor —
 * folding it into the hook's DEF arm would ADD the canary to every α/γ/ω define under INSTRUMENTED, a
 * behavior expansion deliberately not made today. */
inline std::string x86_pair_loop() {
    std::string r;
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
        if (MEDIUM_BINARY) {
            if (g_emit.xa_bb_emit_pair_define[i]) { r += (char)'E'; r += (char)(unsigned char)i; r += x86_port_canary(); r += x86_port_hook(X86H_DEF, X86P_BETA); }
            if (g_emit.xa_bb_emit_pair_jmp[i])    { r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += (char)(unsigned char)i; }
        } else {
            if (g_emit.xa_bb_emit_pair_define[i]) { r += emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name); r += x86_port_canary(); r += x86_port_hook(X86H_DEF, X86P_BETA); }
            if (g_emit.xa_bb_emit_pair_jmp[i])    r += std::string(" jmp ") + g_emit.xa_bb_emit_pair_jmp[i]->name + "\n";
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
    return std::string(" jmp ") + (x86_pair_tgt(idx) ? x86_pair_tgt(idx)->name : "??") + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jcc_pair(const char * mnem, int idx) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))); r += (char)'F'; r += (char)(unsigned char)idx; return r; }
    return std::string(" ") + mnem + " " + (x86_pair_tgt(idx) ? x86_pair_tgt(idx)->name : "??") + "\n";
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
/* x86_rtcc_call_descr — RC-4 RTCC veneer for DESCR_t-returning calls (RAX:RDX pair).                           */
/* RETURN-BEFORE-RELOAD LAW: capture rax→FRQ(slot) and rdx→FRQ(slot+8) BEFORE the reload clobbers them.        */
/* KILLSWITCH: gate OFF → bare x86_call_ro + the two post-call moves (byte-identical to pre-RTCC template seq). */
/* Defined here (after FRQ, x86_frame_off, x86() — all needed by FRQ) not at the earlier forward-decl site.    */
inline std::string x86_rtcc_call_descr(const char * sym, uint64_t ptr, int slot) {
    if (!g_rtcc_on) {
        return x86_call_ro(sym, ptr) + x86("mov", FRQ(slot), "rax") + x86("mov", FRQ(slot + 8), "rdx");
    }
    uint64_t block = (uint64_t)(uintptr_t)g_rtcc_block;
    std::string cap = x86("mov", FRQ(slot), "rax") + x86("mov", FRQ(slot + 8), "rdx");
    if (MEDIUM_BINARY) {
        std::string call_b;
        call_b += (char)0x49; call_b += (char)0xBA; call_b += u64le(ptr); /* movabs r10, ptr */
        call_b += (char)0x41; call_b += (char)0xFF; call_b += (char)0xD2;  /* call r10 */
        return x86_align_assert() + x86_Lrec(x86_rtcc_wb_bin(block)) + x86_Lrec(call_b) + cap + x86_Lrec(x86_rtcc_rl_bin(block));
    }
    return x86_align_assert() + x86_rtcc_wb_text() + " call " + sym + "@PLT\n" + cap + x86_rtcc_rl_text();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline void bb_emit_x86(const std::string & s) {
    if (!MEDIUM_BINARY) { if (!s.empty()) emit_text_n(s.data(), s.size()); return; }
    bb_label_t internal[X86_INTERNAL_MAX];
    for (int k = 0; k < X86_INTERNAL_MAX; k++) { internal[k].offset = BB_LABEL_UNRESOLVED; internal[k].name[0] = '\0'; }
    size_t i = 0, n = s.size();
    while (i < n) {
        char tag = s[i++];
        if (tag == 'L') { int k = (unsigned char)s[i++]; for (int j = 0; j < k; j++) bb_emit_byte((uint8_t)(unsigned char)s[i++]); }
        else if (tag == 'J') { int id = (unsigned char)s[i++]; bb_emit_patch_rel32(x86_label_for(id, internal)); }
        else if (tag == 'D') { int id = (unsigned char)s[i++]; bb_label_define(x86_label_for(id, internal)); }
        else if (tag == 'E') { int idx = (unsigned char)s[i++]; if (g_emit.xa_bb_emit_pair_define[idx]) bb_label_define(g_emit.xa_bb_emit_pair_define[idx]); }
        else if (tag == 'F') { int idx = (unsigned char)s[i++]; bb_label_t * _t = x86_pair_tgt(idx); if (_t) bb_emit_patch_rel32(_t); }
        else if (tag == 'X') { uint64_t v = 0; for (int j = 0; j < 8; j++) v |= ((uint64_t)(unsigned char)s[i++]) << (8 * j); bb_emit_patch_rel32((bb_label_t *)(uintptr_t)v); }
        else if (tag == 'Y') { uint64_t v = 0; for (int j = 0; j < 8; j++) v |= ((uint64_t)(unsigned char)s[i++]) << (8 * j); bb_label_define((bb_label_t *)(uintptr_t)v); }   /* TINY-SITE s57: DEFINE an externally-owned label at the current cursor — the 'X' record's define twin (role-3 shim entry, jumped by x86_jmp_ext from tiny call sites) */
        else break;
    }
}
#endif

