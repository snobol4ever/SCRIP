#ifndef X86_ASM_H
#define X86_ASM_H
#include <string>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "emit.h"
#include "zeta_choices.h"
extern "C" {
}
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
    if (!strcmp(r, "ebp") || !strcmp(r, "rbp")) return 5;
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
inline std::string x86_movzx_subj_byte(const char * dst) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x40 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x0F; code += (char)0xB6; code += (char)modrm; code += (char)sib; code += (char)0x00;
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" movzx ") + dst + ", byte ptr [r13+rcx]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_store_cursor_mirror() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b3(0x45, 0x89, 0x32)) : std::string(" mov [r10], r14d\n");
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
inline std::string x86_call_ro(const char * sym, uint64_t ptr) {
    if (MEDIUM_BINARY) { std::string code; code += (char)0x48; code += (char)0xB8; code += u64le(ptr); code += (char)0xFF; code += (char)0xD0; return x86_Lrec(code); }
    return std::string(" call ") + sym + "@PLT\n";
}
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
enum { X86H_DEF = 0, X86H_JMP = 1, X86H_JCC = 2 };
inline std::string x86_port_hook(int site, int port);
inline std::string x86_jcc(const char * mnem, int port) {
    return x86_port_hook(X86H_JCC, port)
         + (MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(port))
                          : (std::string(" ") + mnem + " " + x86_portname(port) + "\n"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline int x86_port_mode() {
    static int m = -1;
    if (m < 0) { const char *e = getenv("SCRIP_ZETA_PORT"); m = e ? atoi(e) : (int)ZC_PORT; }
    return m;
}
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
 * r12 (the ratified GZ3 contract).  ZC_FRAME_RBP is the C-frame-pointer EXPERIMENT from the C-STACK analysis
 * (GOAL-SNOBOL4-BB.md SESSION STATE: when the proc trampoline retires, rsp becomes the frame and "r12 AND rbp
 * free up" — this switch lets the rbp half of that end-state be exercised NOW, ahead of the trampoline work).
 * COMPILE-TIME ONLY (Lon directive 2026-07-09: "We will never flip the R12 to RSP or RBP at runtime") — the
 * SCRIP_ZETA_FRAME env override is DELETED, enforcement by deletion; ZC_FRAME is a BUILD CONSTANT
 * (zeta_choices.h default, or -DZC_FRAME=…) and every selector below constant-folds to a literal.
 * EVERYTHING frame-relative flows through the four accessors
 * below — the text form (x86_zr), the encoding number (x86_zr_num), the modrm/REX producers further down
 * (x86_frame_modrm + the conditional-REX helpers), the FR/FRQ operand spellings AND the x86_parse arm that
 * classifies them back (kept in lockstep via x86_fr32_prefix/x86_fr64_prefix), the port-hook canary/alloc
 * instructions, and the alignment-dance save register.  KNOWN EXPERIMENT LIMITS under ZC_FRAME_RBP, stated
 * honestly: (a) any template still hand-spelling a push-rbp alignment dance instead of x86_align_enter/leave
 * clobbers the frame — the live match-family dances are swept this session, the inert rbx-dance holdouts
 * (bb_gvar_assign_concat, bb_pattern_break/len, bb_ref_invariant) are pre-existing and unaffected either way;
 * (b) C code compiled -O0 uses rbp as ITS frame pointer, so a callee's own prologue saving/restoring rbp is
 * fine (callee-saved either way, same as r12) but gdb frame-walking of emitted code gets weirder; (c) the
 * six-register coexpr save contract (bb_create.cpp) already saves BOTH r12 and rbp, so it covers either
 * choice unchanged. */
inline const char * x86_zr()         { return ZC_FRAME == ZC_FRAME_RSP ? "rsp" : ZC_FRAME == ZC_FRAME_RBP ? "rbp" : "r12"; }
inline int          x86_zr_num()     { return ZC_FRAME == ZC_FRAME_RSP ? 4 : ZC_FRAME == ZC_FRAME_RBP ? 5 : 12; }
inline const char * x86_align_save() { return ZC_FRAME == ZC_FRAME_RBP ? "r12" : "rbp"; }
inline const char * x86_fr32_prefix() { return ZC_FRAME == ZC_FRAME_RSP ? "dword ptr [rsp + " : ZC_FRAME == ZC_FRAME_RBP ? "dword ptr [rbp + " : "dword ptr [r12 + "; }
inline const char * x86_fr64_prefix() { return ZC_FRAME == ZC_FRAME_RSP ? "qword ptr [rsp + " : ZC_FRAME == ZC_FRAME_RBP ? "qword ptr [rbp + " : "qword ptr [r12 + "; }
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
/* x86_zeta_free_call() — emits `call rt_zls_release(r12)`, alignment-safe (push rbp/and rsp,-16/restore, the
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
inline std::string x86_r12_modrm(int regfield, int off) {
    /* Frame-base modrm, generalized 2026-07-08 session 2 (name kept for grep continuity with its 17 call
     * sites; "r12" in the name now means "the ζ frame register", x86_zr_num()).  r12 (low3=100): SIB byte
     * 0x24 mandatory, mod=00 legal at off 0 — byte-identical to the pre-generalization encoder.  rbp
     * (low3=101): no SIB, and mod=00 is UNAVAILABLE ([rbp] with mod=00 encodes disp32/RIP-relative), so
     * off==0 must take the disp8 form — one extra byte vs r12, matching exactly what `as` emits for
     * [rbp + 0] (the R10 BINARY-agrees-with-TEXT law). */
    std::string s; int rf = regfield & 7; int b = x86_zr_num() & 7; int sib = (b == 4);
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
 * old hard-coded 0x49/0x41 forms; under rbp B drops and low-regfield 32-bit ops emit no REX at all. */
inline std::string x86_frame_rex(int w, int regfield) {
    uint8_t rex = 0x40; if (w) rex |= 0x08; if (regfield >= 8) rex |= 0x04; if (x86_zr_num() >= 8) rex |= 0x01;
    std::string s; if (rex != 0x40) s += (char)rex;
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_text_mem(int off) { return std::string("[") + x86_zr() + " + " + std::to_string(off) + "]"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_lea(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x8D; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" lea ") + reg + ", " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_mov_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, 0); c += (char)0xC7; c += x86_r12_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return std::string(" mov dword ptr ") + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_store(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x89; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov dword ptr ") + x86_frame_text_mem(off) + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_load(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x8B; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov ") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_add_imm(int off, long imm) {
    if (MEDIUM_BINARY) {
        /* REX via x86_frame_rex (2026-07-08 session 2): this helper's former unconditional 0x41 was the ONE
         * hard-coded frame REX the generalization sweep missed — under ZC_FRAME_RBP it turned the base-rbp
         * modrm (low3=101) into base-R13 (same low3, REX.B set), so IR_MATCH_HEAD's β cursor bump wrote
         * [r13+off] while every reader read [rbp+off]: the anchor never advanced and the simplest literal
         * match spun forever in mode-3 (found by disassembling the live JIT stream; mode-4 was immune because
         * the TEXT arm below was always spelled from x86_frame_text_mem). */
        std::string c; c += x86_frame_rex(0, 0);
        if (imm >= -128 && imm <= 127) { c += (char)0x83; c += x86_r12_modrm(0, off); c += (char)(uint8_t)(int8_t)imm; }
        else                           { c += (char)0x81; c += x86_r12_modrm(0, off); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return std::string(" add dword ptr ") + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_add_to_reg(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x03; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" add ") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_sub_from_reg(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(0, g); c += (char)0x2B; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" sub ") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * FR(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "%s%d]", x86_fr32_prefix(), off); return b[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_load64(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x8B; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov ") + reg + ", qword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_store64(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, g); c += (char)0x89; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov qword ptr ") + x86_frame_text_mem(off) + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_frame64(int off) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, 0); c += (char)0xFF; c += x86_r12_modrm(4, off); return x86_Lrec(c); }
    return std::string(" jmp qword ptr ") + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_mov_imm64(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += x86_frame_rex(1, 0); c += (char)0xC7; c += x86_r12_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return std::string(" mov qword ptr ") + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * FRQ(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "%s%d]", x86_fr64_prefix(), off); return b[i]; }
inline const char * ROQ(int n)   { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rip + %d]", n); return b[i]; }
inline const char * RDQ(const char * base, int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [%s + %d]", base, off); return b[i]; }
inline const char * RDD(const char * base, int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "dword ptr [%s + %d]", base, off); return b[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_load64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8B; c += (char)(0x80 | ((g & 7) << 3) | (b & 7));
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return std::string(" mov ") + dst + ", qword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_store64(const char * base, int disp, const char * src) {
    int g = x86_rnum(src), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x89; c += (char)(0x80 | ((g & 7) << 3) | (b & 7));
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return std::string(" mov qword ptr [") + base + " + " + std::to_string(disp) + "], " + src + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_load32(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0x8B; c += (char)(0x80 | ((g & 7) << 3) | (b & 7));
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return std::string(" mov ") + dst + ", dword ptr [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_store32(const char * base, int disp, const char * src) {
    int g = x86_rnum(src), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0x89; c += (char)(0x80 | ((g & 7) << 3) | (b & 7));
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return std::string(" mov dword ptr [") + base + " + " + std::to_string(disp) + "], " + src + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_store_imm64(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0xC7; c += (char)(0x80 | (b & 7)); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm);
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
 * uniform shape; `as` agreement is irrelevant here because RDD text spells the same disp the binary encodes). */
inline std::string x86_reg_disp32_store_imm32(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if ((b & 7) == 4) { fprintf(stderr, "FATAL x86_reg_disp32_store_imm32: base '%s' needs a SIB byte this encoder does not produce (whole reg_disp32 family limitation) — use a different base register\n", base); abort(); }
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x40; if (b >= 8) rex |= 0x01; if (rex != 0x40) c += (char)rex; c += (char)0xC7; c += (char)(0x80 | (b & 7)); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm);
        return x86_Lrec(c);
    }
    return std::string(" mov dword ptr [") + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_reg_disp32_lea64(const char * dst, const char * base, int disp) {
    int g = x86_rnum(dst), b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0x8D; c += (char)(0x80 | ((g & 7) << 3) | (b & 7));
        c += u32le((uint32_t)disp); return x86_Lrec(c);
    }
    return std::string(" lea ") + dst + ", [" + base + " + " + std::to_string(disp) + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_store64(int off, const char * reg) {
    int g = x86_rnum(reg); uint8_t rex = 0x48; if (g >= 8) rex |= 0x04;
    if (MEDIUM_BINARY) { std::string c; c += (char)rex; c += (char)0x89; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov qword ptr [rsp + ") + std::to_string(off) + "], " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_store32_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0xC7; c += x86_r12_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return std::string(" mov dword ptr [rsp + ") + std::to_string(off) + "], " + std::to_string((uint32_t)imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_rsp_load64(const char * reg, int off) {
    int g = x86_rnum(reg); uint8_t rex = 0x48; if (g >= 8) rex |= 0x04;
    if (MEDIUM_BINARY) { std::string c; c += (char)rex; c += (char)0x8B; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov ") + reg + ", qword ptr [rsp + " + std::to_string(off) + "]\n";
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
    std::string code; code += x86_frame_rex(1, 0); code += (char)0xFF; code += x86_r12_modrm(0, off);
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
enum { XK_NONE = 0, XK_REG, XK_IMM, XK_PORT, XK_ILBL, XK_FR32, XK_FR64, XK_RSP64, XK_MEMIND, XK_MEMIDX8, XK_R13RCX, XK_R10MIR, XK_RIPSEAL, XK_REGDISP, XK_REGDISP32, XK_SYM, XK_ROSLOT, XK_EXTLBL };
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
    static const char * regs[] = { "rax","rbx","rcx","rdx","rsi","rdi","rsp","rbp","r8","r9","r10","r11","r12","r13","r14","r15",
                                   "eax","ebx","ecx","edx","esi","edi","esp","ebp","r8d","r9d","r10d","r11d","r12d","r13d","r14d","r15d",
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
    if (!strcmp(s, "extlbl")) { o.kind = XK_EXTLBL; return; }
    if (!strncmp(s, x86_fr32_prefix(), strlen(x86_fr32_prefix()))) { o.kind = XK_FR32;  o.off = atoi(s + strlen(x86_fr32_prefix())); return; }
    if (!strncmp(s, x86_fr64_prefix(), strlen(x86_fr64_prefix()))) { o.kind = XK_FR64;  o.off = atoi(s + strlen(x86_fr64_prefix())); return; }
    if (!strncmp(s, "dword ptr [", 11)) { const char * lb = s + 10; const char * pl = strstr(lb, " + ");
      if (pl) { size_t bl = (size_t)(pl - (lb + 1)); if (bl > 7) bl = 7; memcpy(o.base, lb + 1, bl); o.base[bl] = 0;
        char * ep = 0; long d = strtol(pl + 3, &ep, 10); if (x86_is_reg(o.base) && ep && *ep == ']') { o.kind = XK_REGDISP32; o.off = (int)d; return; } } }
    if (!strncmp(s, "qword ptr [rip + ", 17)) { o.kind = XK_ROSLOT; o.off = atoi(s + 17); return; }
    if (!strncmp(s, "qword ptr [rsp + ", 17)) { o.kind = XK_RSP64; o.off = atoi(s + 17); return; }
    if (!strncmp(s, "qword ptr [", 11)) { const char * lb = s + 10; const char * pl = strstr(lb, " + ");
      if (pl) { size_t bl = (size_t)(pl - (lb + 1)); if (bl > 7) bl = 7; memcpy(o.base, lb + 1, bl); o.base[bl] = 0;
        char * ep = 0; long d = strtol(pl + 3, &ep, 10); if (x86_is_reg(o.base) && ep && *ep == ']') { o.kind = XK_REGDISP; o.off = (int)d; return; } } }
    { char ns[32]; int k = 0; for (const char * q = s; *q && k < 31; q++) if (*q != ' ') ns[k++] = *q; ns[k] = 0;
      if (!strcmp(ns, "[r13+rcx]")) { o.kind = XK_R13RCX; return; }
      if (!strcmp(ns, "[r10]"))     { o.kind = XK_R10MIR; return; } }
    if (!strcmp(s, "[rip + __]"))              { o.kind = XK_RIPSEAL; return; }
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
            if (!x86_is_reg(o.base)) {
                fprintf(stderr, "FATAL x86_parse: memory operand '%s' has non-register base '%s' — no parse arm owns this form; "
                        "templates must speak a canonical operand (FRQ/ROQ/RDQ/RSP), never a hand-built bracket string\n", s, o.base);
                abort();
            }
            o.kind = XK_MEMIND; return;
        }
    }
    if (x86_is_reg(s)) { o.kind = XK_REG; o.reg = x86_rnum(s); return; }
    char * endp = 0; long v = strtol(s, &endp, 0);
    if (endp && endp != s && *endp == 0) { o.kind = XK_IMM; o.imm = v; return; }
    o.kind = XK_SYM; size_t sl = strlen(s); if (sl > 95) sl = 95; memcpy(o.sym, s, sl); o.sym[sl] = 0;
}
inline std::string x86_bomb(const char * msg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86(const char * mnem, xop xa = xop(), xop xb = xop(), xop xc = xop(), xop xd = xop()) {
    opnd a, b; x86_parse(xa, a); x86_parse(xb, b);
    if (!strcmp(mnem, "label"))     return (MEDIUM_BINARY || MEDIUM_MACRO_DEF) ? std::string() : (std::string(xa.s ? xa.s : "") + ":\n");
    if (!strcmp(mnem, "comment"))   return (MEDIUM_BINARY || MEDIUM_MACRO_DEF) ? std::string() : (std::string("# ") + (xa.s ? xa.s : "") + "\n");
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
    if (!strcmp(mnem, "def")) {
        if (a.kind == XK_PORT) return x86_deflabel(a.port);
        if (a.kind == XK_ILBL) return x86_deflabel_id(a.lbl);
        return std::string();
    }
    if (!strcmp(mnem, "jmp")) {
        if (a.kind == XK_PORT) return x86_jmp(a.port);
        if (a.kind == XK_ILBL) return x86_jmp_id(a.lbl);
        if (a.kind == XK_FR64) return x86_jmp_frame64(a.off);
        if (a.kind == XK_EXTLBL && xb.tag == 2) return x86_jmp_ext((const struct bb_label_t *)(uintptr_t)xb.u);
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return std::string(" jmp ") + a.sym + "\n";
        return std::string();
    }
    if (mnem[0] == 'j') {
        if (a.kind == XK_PORT) return x86_jcc(mnem, a.port);
        if (a.kind == XK_ILBL) return x86_jcc_id(mnem, a.lbl);
        if (a.kind == XK_EXTLBL && xb.tag == 2) return x86_jcc_ext(mnem, (const struct bb_label_t *)(uintptr_t)xb.u);
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return std::string(" ") + mnem + " " + a.sym + "\n";
        return std::string();
    }
    if (!strcmp(mnem, "call")) {
        if (a.kind == XK_PORT) return MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE8)) + x86_Jrec(a.port))
                                                    : (std::string(" call ") + x86_portname(a.port) + "\n");
        if (a.kind == XK_SYM && xb.tag == 2) return x86_call_ro(a.sym, xb.u);
        if (a.kind == XK_SYM && !MEDIUM_BINARY) return std::string(" call ") + a.sym + "\n";
        if (a.kind == XK_REG) {
            int m = x86_rnum(a.txt); uint8_t modrm = (uint8_t)(0xD0 | (m & 7)); uint8_t rex = (m >= 8) ? 0x41 : 0x40;
            return MEDIUM_BINARY ? x86_Lrec(std::string((char)rex == 0x40 ? "" : std::string(1,(char)rex)) + (char)0xFF + (char)modrm) : (std::string(" call ") + a.txt + "\n");
        }
        return std::string();
    }
    if (!strcmp(mnem, "push")) return x86_push(a.txt);
    if (!strcmp(mnem, "pop"))  return x86_pop(a.txt);
    if (!strcmp(mnem, "idiv")) return x86_idiv(a.txt);
    if (!strcmp(mnem, "neg"))  return x86_neg(a.txt);
    if (!strcmp(mnem, "inc")) {
        if (a.kind == XK_FR64) return x86_frame_inc64(a.off);
        return std::string();
    }
    if (!strcmp(mnem, "mov")) {
        if (a.kind == XK_R10MIR)                       return x86_store_cursor_mirror();
        if (a.kind == XK_FR32 && b.kind == XK_REG)     return x86_frame_store(a.off, b.txt);
        if (a.kind == XK_FR32 && b.kind == XK_IMM)     return x86_frame_mov_imm(a.off, b.imm);
        if (a.kind == XK_FR64 && b.kind == XK_REG)     return x86_frame_store64(a.off, b.txt);
        if (a.kind == XK_FR64 && b.kind == XK_IMM)     return x86_frame_mov_imm64(a.off, b.imm);
        if (a.kind == XK_RSP64 && b.kind == XK_REG)    return x86_rsp_store64(a.off, b.txt);
        if (a.kind == XK_RSP64 && b.kind == XK_IMM)    return x86_rsp_store32_imm(a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_FR32)     return x86_frame_load(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_FR64)     return x86_frame_load64(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_ROSLOT)   return x86_ro_load_q(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_RSP64)    return x86_rsp_load64(a.txt, b.off);
        if (a.kind == XK_REGDISP && b.kind == XK_REG)  return x86_reg_disp32_store64(a.base, a.off, b.txt);
        if (a.kind == XK_REGDISP && b.kind == XK_IMM)  return x86_reg_disp32_store_imm64(a.base, a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_REGDISP)  return x86_reg_disp32_load64(a.txt, b.base, b.off);
        if (a.kind == XK_REGDISP32 && b.kind == XK_REG)  return x86_reg_disp32_store32(a.base, a.off, b.txt);
        if (a.kind == XK_REGDISP32 && b.kind == XK_IMM)  return x86_reg_disp32_store_imm32(a.base, a.off, b.imm);
        if (a.kind == XK_REG && b.kind == XK_REGDISP32)  return x86_reg_disp32_load32(a.txt, b.base, b.off);
        if (a.kind == XK_REG && b.kind == XK_MEMIDX8)  return x86_load_indexed8(a.txt, b.base, b.idx);
        if (a.kind == XK_REG && b.kind == XK_MEMIND)   return x86_load_mem64(a.txt, b.txt);
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
        if (a.kind == XK_REG && (b.kind == XK_FR32 || b.kind == XK_FR64)) return x86_frame_lea(a.txt, b.off);
        if (a.kind == XK_REG && b.kind == XK_REGDISP)              return x86_reg_disp32_lea64(a.txt, b.base, b.off);
        if (a.kind == XK_REG && b.kind == XK_R13RCX)                return x86_lea_subj_cursor(a.txt);
        if (a.kind == XK_REG && b.kind == XK_REG)                   return x86_lea_subj_cursor(a.txt);
        if (b.txt && strstr(b.txt, "rip"))                          return x86_bomb("lea: unsealed [rip + label] operand — use the [rip + __] sealed form with (ptr,label) args");
        return std::string();
    }
    if (!strcmp(mnem, "add")) {
        if (a.kind == XK_REG && b.kind == XK_REG)  return x86_add_rr(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM)  return x86_add(a.txt, b.imm);
        if (a.kind == XK_REG && b.kind == XK_FR32) return x86_frame_add_to_reg(a.txt, b.off);
        if (a.kind == XK_FR32 && b.kind == XK_IMM) return x86_frame_add_imm(a.off, b.imm);
        return std::string();
    }
    if (!strcmp(mnem, "sub")) {
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_sub_rr(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM) return x86_sub(a.txt, b.imm);
        if (a.kind == XK_REG && b.kind == XK_FR32) return x86_frame_sub_from_reg(a.txt, b.off);
        return std::string();
    }
    if (!strcmp(mnem, "imul"))   { return x86_imul_rr(a.txt, b.txt); }
    if (!strcmp(mnem, "and"))    { if (b.kind == XK_IMM) return x86_and(a.txt, b.imm); return std::string(); }
    if (!strcmp(mnem, "cmp")) {
        if (a.kind == XK_REG && b.kind == XK_REG) return x86_cmp(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM) return x86_cmp_imm(a.txt, b.imm);
        return std::string();
    }
    if (!strcmp(mnem, "cmp64"))  { if (b.kind == XK_IMM) return x86_cmp_imm64(a.txt, b.imm); return std::string(); }
    if (!strcmp(mnem, "test"))   { return x86_test(a.txt, b.txt); }
    if (!strcmp(mnem, "movsxd")) { return x86_movsxd(a.txt, b.txt); }
    if (!strcmp(mnem, "movzx"))  { (void)b; return x86_movzx_subj_byte(a.txt); }
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
 * (2026-07-08 session 2).  Saves rsp in a callee-saved register, 16-aligns, restores.  The save register is
 * ALWAYS the other member of the {r12, rbp} pair (x86_align_save): rbp when the frame is r12 — today's exact
 * bytes at every existing dance site — and r12 when the frame is rbp, which is precisely what makes the dance
 * FRAME-SAFE under ZC_FRAME_RBP (a hand-spelled push-rbp dance would clobber an rbp frame; that hazard is why
 * this pair exists and why templates must use it instead of spelling the dance).  The pair leaves flags
 * meaningless across it (push/and both touch or depend on rsp) — callers already treat a C call as a full
 * clobber, so nothing new. */
inline std::string x86_align_enter() {
    const char * sv = x86_align_save();
    return x86("push", sv) + x86("mov", sv, "rsp") + x86("and", "rsp", -16L);
}
inline std::string x86_align_leave() {
    const char * sv = x86_align_save();
    return x86("mov", "rsp", sv) + x86("pop", sv);
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
 * caller splices it INSIDE its own existing push-rbp/and-rsp aligned window, immediately after its sibling
 * rt_zls_release_to call, so no second alignment dance is emitted. */
extern "C" char *g_zls2_cur;
/* x86_zls2_cur_lea(reg) — address of the exported ZLS2 cursor cell into reg, the ZC_PORT_INLINE primitive.
 * Medium split is the sanctioned RO-load exception (R10): TEXT `lea reg,[rip+g_zls2_cur]` (resolved by the
 * -no-pie link against libscrip_rt), BINARY movabs of the in-process address. */
inline std::string x86_zls2_cur_lea(const char * reg) {
    return x86_load_ro(reg, "g_zls2_cur", (uint64_t)(uintptr_t)(void *)&g_zls2_cur);
}
inline std::string x86_zls2_mark_save(int off) {
    if (x86_port_mode() == ZC_PORT_CSTACK)
        return x86("mov", FRQ(off), "rsp");
    if (x86_port_mode() == ZC_PORT_INLINE)
        return x86_zls2_cur_lea("rdi")
             + x86("mov", "rax", RDQ("rdi", 0))
             + x86("mov", FRQ(off), "rax");
    if (x86_port_mode() != ZC_PORT_ALLOC) return std::string();
    return x86("call", "rt_zls2_mark", (uint64_t)(uintptr_t)(void *)rt_zls2_mark)
         + x86("mov", FRQ(off), "rax");
}
inline std::string x86_zls2_release_to_call(int off) {
    /* CSTACK: this helper is spliced INSIDE the caller's open alignment-dance window (both consumers:
     * bb_match_head fail arm, bb_match_release success arm), with another C call between this fragment and
     * the caller's dance-leave.  A bare `mov rsp, FRQ(off)` here is (a) clobbered by the caller's leave and
     * (b) worse, leaves the intervening C call pushing its return address just below the MARK — on top of
     * live stack (the first CSTACK cut segfaulted exactly there, 038_pat_literal m4).  So: CLOSE the
     * caller's window, restore rsp to the mark, RE-OPEN a fresh window at the new position — the caller's
     * remaining call runs safely below the re-based window, and the caller's own dance-leave then lands rsp
     * exactly at the mark. */
    if (x86_port_mode() == ZC_PORT_CSTACK)
        return x86_align_leave()
             + x86("mov", "rsp", FRQ(off))
             + x86_align_enter();
    if (x86_port_mode() == ZC_PORT_INLINE)
        return x86_zls2_cur_lea("rdi")
             + x86("mov", "rax", FRQ(off))
             + x86("mov", RDQ("rdi", 0), "rax");
    if (x86_port_mode() != ZC_PORT_ALLOC) return std::string();
    return x86("mov",  "rdi", FRQ(off))
         + x86("call", "rt_zls2_release_to", (uint64_t)(uintptr_t)(void *)rt_zls2_release_to);
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
 * alignment-safe call (push rbp/and rsp,-16/restore, the x86_zeta_free_call convention) wrapping rt_zls2_push/
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
 *                                   IR_MATCH_HEAD sharing the window makes role 2 look resolved).  zls2_geom
 *                                   grants RELEASE only to roles whose jmp-ω sites are STATICALLY all
 *                                   activation-death (ARB's single exhaust ω; ARBNO role 2's single outer-
 *                                   fail ω) — role knowledge is the classifier, so the six-decoy-ω trap
 *                                   (ARBNO roles 0/1's ω = body-entry/exhaust ALIASES) cannot fire it.
 * Register contract: rax/rcx/rdi + C-call caller-saved clobbered at DEF sites and before a port jmp — dead
 * at both by this codebase's convention (r12-r15/rbx/rbp callee-saved through the rt_zls2 calls).  The
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
    if (x86_port_mode() == ZC_PORT_INLINE && _.op_zls2_ops && _.op_zls2_slot >= 0) {
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
    if (x86_port_mode() == ZC_PORT_CSTACK && _.op_zls2_ops && _.op_zls2_slot >= 0) {
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
    if (site == X86H_DEF && port == X86P_ALPHA && _.op_zls2_bytes > 0 && _.op_zls2_ops == 0 && x86_port_mode() == ZC_PORT_ALLOC)
        s += x86_sub(x86_zr(), _.op_zls2_bytes);
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
    return x86("push", "rsi")
         + x86_align_enter()
         + x86("call", "rt_zls_mark", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_zls_mark)
         + x86_align_leave()
         + x86("mov",  FRQ(off), "rax")
         + x86("pop",  "rsi");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zeta_release_to_call(int off) {
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
inline std::string x86_pair_jmp(int idx) {
    if (MEDIUM_BINARY) { std::string r; r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += (char)(unsigned char)idx; return r; }
    return std::string(" jmp ") + (g_emit.xa_bb_emit_pair_jmp[idx] ? g_emit.xa_bb_emit_pair_jmp[idx]->name : "??") + "\n";
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
        else if (tag == 'F') { int idx = (unsigned char)s[i++]; if (g_emit.xa_bb_emit_pair_jmp[idx]) bb_emit_patch_rel32(g_emit.xa_bb_emit_pair_jmp[idx]); }
        else if (tag == 'X') { uint64_t v = 0; for (int j = 0; j < 8; j++) v |= ((uint64_t)(unsigned char)s[i++]) << (8 * j); bb_emit_patch_rel32((bb_label_t *)(uintptr_t)v); }
        else break;
    }
}
#endif
