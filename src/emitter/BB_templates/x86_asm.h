#ifndef X86_ASM_H
#define X86_ASM_H
/* x86_asm.h — UNIFIED x86 emission for BB templates (pivot 2026-06-01, Lon directive).
 *
 * GOAL: kill the SPIN/CHURN of maintaining two divergent arms (MEDIUM_BINARY hand-byte-map +
 * MEDIUM_TEXT GAS) per box plus a hand-counted bb_bin_t offset table.  A template becomes ONE
 * return per PLATFORM_*, pure string concatenation, no locals — only g_emit (_) , functions of
 * it, and literals — with all variance inline via IF(...)/FOR(...).  Each x86_* function below
 * is SIDE-EFFECT-FREE and returns either GAS text (MEDIUM_TEXT) or an in-band RECORD STREAM
 * (MEDIUM_BINARY); the medium switch is invisible to the template.
 *
 * PATCH SITES IN-BAND (replaces bb_bin_t).  A jump/label-def is encoded as a tagged record in the
 * returned string; bb_emit_x86 DISCOVERS the byte position as it copies, so there are NO
 * hand-counted offsets to drift.  Records (MEDIUM_BINARY only):
 *   'L' <len:1> <len bytes>   literal code bytes
 *   'J' <port:1>              rel32 patch to a port label  (opcode bytes precede via an 'L')
 *   'D' <port:1>              define a port label here (0 bytes)
 * Ports: 0=alpha 1=beta 2=gamma 3=omega (resolved against g_emit.lbl_*_p / .lbl_* at walk time).
 *
 * This is the FIRST test case (bb_lit).  Two genuinely-divergent instructions — the RO load
 * (lea[rip] vs movabs) and the runtime call (call@PLT vs movabs+call rax) — are ENCAPSULATED in
 * x86_load_ro / x86_call_ro, both arms adjacent so they cannot silently drift.  Making both arms
 * position-independent [rip+disp] (REG-RO) is then a one-line change inside those two functions.
 */
#include <string>
#include <cstdint>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "emit_core.h"
#include "emit_globals.h"
#include "emit_io.h"
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum x86_port { PORT_ALPHA = 0, PORT_BETA = 1, PORT_GAMMA = 2, PORT_OMEGA = 3 };
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
inline const char * x86_portname(int p) {
    switch (p) { case PORT_ALPHA: return _.lbl_α; case PORT_BETA: return _.lbl_β;
                 case PORT_GAMMA: return _.lbl_γ; default: return _.lbl_ω; }
}
inline struct bb_label_t * x86_portlbl(int p) {
    switch (p) { case PORT_ALPHA: return _.lbl_α_p; case PORT_BETA: return _.lbl_β_p;
                 case PORT_GAMMA: return _.lbl_γ_p; default: return _.lbl_ω_p; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_Lrec(const std::string & b) { std::string r; r += (char)'L'; r += (char)(unsigned char)b.size(); r += b; return r; }
inline std::string x86_Jrec(int port)              { std::string r; r += (char)'J'; r += (char)(unsigned char)port; return r; }
inline std::string x86_Drec(int port)              { std::string r; r += (char)'D'; r += (char)(unsigned char)port; return r; }
inline std::string x86_b1(uint8_t a)                          { std::string s; s += (char)a; return s; }
inline std::string x86_b2(uint8_t a, uint8_t b)               { std::string s; s += (char)a; s += (char)b; return s; }
inline std::string x86_b3(uint8_t a, uint8_t b, uint8_t c)    { std::string s; s += (char)a; s += (char)b; s += (char)c; return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* 64-bit register? rax..rdi / rsp / rbp / rsi / rdi / r8..r15 (NO trailing 'd'). 32-bit: eax.. / rNNd.   */
inline bool x86_is64(const char * r) {
    if (!r) return false;
    if (r[0] != 'r') return false;
    size_t n = strlen(r);
    if (r[n - 1] == 'd') return false;
    return true;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ALU r/m, r (op /r stores reg into rm): mov 0x89, cmp 0x39, test 0x85. REX.W when the operands are 64-bit*/
/* (e.g. test rax,rax after a pointer-returning call); 32-bit otherwise. Operands share a width in x86.    */
inline std::string x86_alu_rr(const char * mnem, uint8_t op, const char * rm, const char * reg) {
    int m = x86_rnum(rm), g = x86_rnum(reg);
    uint8_t rex = 0x40; if (x86_is64(rm) || x86_is64(reg)) rex |= 0x08; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; if (rex != 0x40) code += (char)rex; code += (char)op; code += (char)(0xC0 | ((g & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" ") + mnem + " " + rm + ", " + reg + "\n");
}
inline std::string x86_mov (const char * rm, const char * reg) { return x86_alu_rr("mov",  0x89, rm, reg); }
inline std::string x86_cmp (const char * rm, const char * reg) { return x86_alu_rr("cmp",  0x39, rm, reg); }
inline std::string x86_test(const char * rm, const char * reg) { return x86_alu_rr("test", 0x85, rm, reg); }
inline std::string x86_add_rr(const char * rm, const char * reg) { return x86_alu_rr("add", 0x01, rm, reg); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* add r32, imm — imm8 short form (0x83 /0) when it fits in int8 (matches `as`); else eax→0x05, others 0x81*/
inline std::string x86_add(const char * reg, long imm) {
    int m = x86_rnum(reg);
    std::string code;
    if (imm >= -128 && imm <= 127) { if (m >= 8) code += (char)0x41; code += (char)0x83; code += (char)(0xC0 | (0 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else if (m == 0)               { code += (char)0x05; code += u32le((uint32_t)imm); }
    else                           { if (m >= 8) code += (char)0x41; code += (char)0x81; code += (char)(0xC0 | (0 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" add ") + reg + ", " + std::to_string(imm) + "\n");
}
/* sub r32, imm — imm8 short form (0x83 /5) when it fits in int8 (matches `as`); else 0x81 /5.             */
inline std::string x86_sub(const char * reg, long imm) {
    int m = x86_rnum(reg);
    std::string code;
    if (imm >= -128 && imm <= 127) { if (m >= 8) code += (char)0x41; code += (char)0x83; code += (char)(0xC0 | (5 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else                           { if (m >= 8) code += (char)0x41; code += (char)0x81; code += (char)(0xC0 | (5 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" sub ") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* movsxd r64, r/m32 — 0x63 /r, REX.W.                                                                    */
inline std::string x86_movsxd(const char * dst64, const char * src32) {
    int g = x86_rnum(dst64), m = x86_rnum(src32);
    uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; if (m >= 8) rex |= 0x01;
    std::string code; code += (char)rex; code += (char)0x63; code += (char)(0xC0 | ((g & 7) << 3) | (m & 7));
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" movsxd ") + dst64 + ", " + src32 + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* lea dst64, [r13 + rcx] — subject-base(Σ=r13) + cursor-index(rcx). 8D /r, REX.W+B, mod=01 disp8=0, SIB. */
inline std::string x86_lea_subj_cursor(const char * dst) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x48 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x8D; code += (char)modrm; code += (char)sib; code += (char)0x00;
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" lea ") + dst + ", [r13 + rcx]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* movzx dst32, byte [r13 + rcx] — load one subject byte at Σ=r13 + cursor-index rcx (0F B6 /r, REX.B, SIB).*/
/* dst is the 32-bit dest (esi etc.); reg field = dst&7 (+REX.R if dst>=8). 6 bytes for low dests.          */
inline std::string x86_movzx_subj_byte(const char * dst) {
    int g = x86_rnum(dst);
    uint8_t rex = 0x40 | 0x01; if (g >= 8) rex |= 0x04;
    uint8_t modrm = (uint8_t)((1 << 6) | ((g & 7) << 3) | 0x04);
    uint8_t sib   = (uint8_t)((0 << 6) | (1 << 3) | 5);
    std::string code; code += (char)rex; code += (char)0x0F; code += (char)0xB6; code += (char)modrm; code += (char)sib; code += (char)0x00;
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" movzx ") + dst + ", byte ptr [r13+rcx]\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* mov [r10], r14d — the legacy cursor mirror (45 89 32). Dies at REG-RO once r10 is retired.             */
inline std::string x86_store_cursor_mirror() {
    return MEDIUM_BINARY ? x86_Lrec(x86_b3(0x45, 0x89, 0x32)) : std::string(" mov [r10], r14d\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_push(const char * r) { int m = x86_rnum(r); std::string code; if (m >= 8) code += (char)0x41; code += (char)(0x50 | (m & 7)); return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" push ") + r + "\n"); }
inline std::string x86_pop (const char * r) { int m = x86_rnum(r); std::string code; if (m >= 8) code += (char)0x41; code += (char)(0x58 | (m & 7)); return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" pop ")  + r + "\n"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* mov dst64, imm — TEXT `mov dst, imm` ; BINARY movabs dst, imm64 (the small-int operand load, e.g. len).*/
inline std::string x86_movimm(const char * dst, long imm) {
    int m = x86_rnum(dst);
    if (MEDIUM_BINARY) { std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le((uint64_t)(uint32_t)imm); return x86_Lrec(code); }
    return std::string(" mov ") + dst + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* RO pointer load. TEXT: lea dst, [rip + label] (position-independent). BINARY: movabs dst, ptr (v1).    */
/* REG-RO target: make the BINARY arm position-independent too (lea dst,[rip+disp] into a sealed RO       */
/* trailer + a rip-rel patch record) — a one-function change, no template edit.                           */
inline std::string x86_load_ro(const char * dst, const char * label, uint64_t ptr) {
    if (MEDIUM_BINARY) { int m = x86_rnum(dst); std::string code; uint8_t rex = 0x48; if (m >= 8) rex |= 0x01; code += (char)rex; code += (char)(0xB8 | (m & 7)); code += u64le(ptr); return x86_Lrec(code); }
    return std::string(" lea ") + dst + ", [rip + " + (label ? label : "??") + "]\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Runtime-helper call. TEXT: call sym@PLT. BINARY: movabs rax, ptr ; call rax (v1).                      */
/* REG-RO target: call qword [rip+disp] through an in-blob pointer slot (position-independent).           */
inline std::string x86_call_ro(const char * sym, uint64_t ptr) {
    if (MEDIUM_BINARY) { std::string code; code += (char)0x48; code += (char)0xB8; code += u64le(ptr); code += (char)0xFF; code += (char)0xD0; return x86_Lrec(code); }
    return std::string(" call ") + sym + "@PLT\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline uint8_t x86_jcc_op(const char * mnem) {
    if (!strcmp(mnem, "je"))  return 0x84;
    if (!strcmp(mnem, "jne")) return 0x85;
    if (!strcmp(mnem, "jl"))  return 0x8C;
    if (!strcmp(mnem, "jge")) return 0x8D;
    if (!strcmp(mnem, "jle")) return 0x8E;
    if (!strcmp(mnem, "jg"))  return 0x8F;
    return 0x85;
}
inline std::string x86_jcc(const char * mnem, int port) {
    return MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(port))
                         : (std::string(" ") + mnem + " " + x86_portname(port) + "\n");
}
inline std::string x86_jmp(int port) {
    return MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(port))
                         : (std::string(" jmp ") + x86_portname(port) + "\n");
}
inline std::string x86_deflabel(int port) {
    return MEDIUM_BINARY ? x86_Drec(port) : (std::string(" ") + x86_portname(port) + ":\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* INTERNAL (box-local) LABELS.  Ports 0..3 are α/β/γ/ω.  A box with an internal loop (SPAN/BREAK) or a join  */
/* ("done") jumps to targets that are NOT ports — only reachable from within the box.  These use record ids   */
/* >= X86_INTERNAL_BASE (id = base + n, n>=0).  BINARY: the walker (bb_emit_x86) maps id>=base to a fresh      */
/* box-local bb_label_t, defined/patched with the SAME bb_label_define / bb_emit_patch_rel32 primitives as     */
/* ports (forward + backward refs already handled by the patch list).  TEXT: the name is .Lx<uid>_<n> where    */
/* uid = _.x86_uid (set per-box by x86_begin() BEFORE the string is built), so two instances of the same box   */
/* never collide.  Template-facing: x86("jmp"/"jge"/.../"def", L(n)).                                          */
#define X86_INTERNAL_BASE 4
#define X86_INTERNAL_MAX  16
struct x86_lbl { int n; };
inline x86_lbl L(int n) { return x86_lbl{ n }; }
inline std::string x86_internal_name(int n) { return std::string(".Lx") + std::to_string(_.x86_uid) + "_" + std::to_string(n); }
inline std::string x86_jmp_id(int n) {
    return MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(X86_INTERNAL_BASE + n))
                         : (std::string(" jmp ") + x86_internal_name(n) + "\n");
}
inline std::string x86_jcc_id(const char * mnem, int n) {
    return MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(X86_INTERNAL_BASE + n))
                         : (std::string(" ") + mnem + " " + x86_internal_name(n) + "\n");
}
inline std::string x86_deflabel_id(int n) {
    return MEDIUM_BINARY ? x86_Drec(X86_INTERNAL_BASE + n) : (x86_internal_name(n) + ":\n");
}
/* Set the per-box internal-label uid (TEXT only).  Called by a looping box's extern BEFORE building the      */
/* string.  Mirrors the bb_cs_id idiom; BINARY needs no uid (records carry ids).                              */
inline void x86_begin() { if (!MEDIUM_BINARY) _.x86_uid = g_flat_node_id++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* cmp r32, imm — imm8 short form (0x83 /7) when it fits int8 (matches `as`); else eax→0x3D, others 0x81 /7.  */
inline std::string x86_cmp_imm(const char * reg, long imm) {
    int m = x86_rnum(reg);
    std::string code;
    if (imm >= -128 && imm <= 127) { if (m >= 8) code += (char)0x41; code += (char)0x83; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += (char)(uint8_t)(int8_t)imm; }
    else if (m == 0)               { code += (char)0x3D; code += u32le((uint32_t)imm); }
    else                           { if (m >= 8) code += (char)0x41; code += (char)0x81; code += (char)(0xC0 | (7 << 3) | (m & 7)); code += u32le((uint32_t)imm); }
    return MEDIUM_BINARY ? x86_Lrec(code) : (std::string(" cmp ") + reg + ", " + std::to_string(imm) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ζ-FRAME MEMORY OPERAND [r12 + off].  RW box-local state (PER-BOX LOCAL STORAGE FACT RULE) lives in the     */
/* one-register frame ζ=r12 and is reached register-relative — IDENTICAL bytes in BINARY and TEXT (no movabs  */
/* to a process address, no rip-rel .data), so there is no medium divergence to drift.  off==0 → mod00 (no    */
/* disp); int8 → mod01 disp8; else mod10 disp32 — matching how `as` encodes an r12 (SIB-base) memory operand. */
inline std::string x86_r12_modrm(int regfield, int off) {
    std::string s; int rf = regfield & 7;
    if (off == 0)                       { s += (char)((0 << 6) | (rf << 3) | 4); s += (char)0x24; }
    else if (off >= -128 && off <= 127) { s += (char)((1 << 6) | (rf << 3) | 4); s += (char)0x24; s += (char)(int8_t)off; }
    else                                { s += (char)((2 << 6) | (rf << 3) | 4); s += (char)0x24; s += u32le((uint32_t)off); }
    return s;
}
inline std::string x86_frame_text_mem(int off) { return std::string("[r12 + ") + std::to_string(off) + "]"; }
/* mov dword [r12+off], imm32 — C7 /0.                                                                        */
inline std::string x86_frame_mov_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0x41; c += (char)0xC7; c += x86_r12_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return std::string(" mov dword ptr ") + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/* mov [r12+off], reg32 — 89 /r (store reg into frame slot).                                                  */
inline std::string x86_frame_store(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x41; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x89; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov dword ptr ") + x86_frame_text_mem(off) + ", " + reg + "\n";
}
/* mov reg32, [r12+off] — 8B /r (load frame slot into reg).                                                   */
inline std::string x86_frame_load(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x41; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8B; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov ") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/* add dword [r12+off], imm — 83 /0 (imm8) or 81 /0 (imm32).                                                  */
inline std::string x86_frame_add_imm(int off, long imm) {
    if (MEDIUM_BINARY) {
        std::string c; c += (char)0x41;
        if (imm >= -128 && imm <= 127) { c += (char)0x83; c += x86_r12_modrm(0, off); c += (char)(uint8_t)(int8_t)imm; }
        else                           { c += (char)0x81; c += x86_r12_modrm(0, off); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return std::string(" add dword ptr ") + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/* add reg32, dword [r12+off] — 03 /r (accumulate a frame slot into a reg).                                  */
inline std::string x86_frame_add_to_reg(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x41; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x03; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" add ") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
struct x86_frame { int off; };
inline x86_frame FR(int off) { return x86_frame{ off }; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* UNIFIED FRONT-END (Lon eureka 2026-06-01).  ONE x86(...) keyed on the mnemonic (1st arg); the remaining   */
/* args' cardinality + type select the form via overloading.  This is the template-facing API — the typed   */
/* x86_* encoders above are the internal implementation it dispatches to.  Add a case here as vocabulary     */
/* grows; the medium (BINARY/TEXT) stays invisible because the encoders handle it.                           */
inline std::string x86(const char * mnem, const char * op1) {                                  /* push / pop                */
    if (!strcmp(mnem, "push")) return x86_push(op1);
    if (!strcmp(mnem, "pop"))  return x86_pop(op1);
    return std::string();
}
inline std::string x86(const char * mnem, x86_port port) {                                     /* jmp / jcc / def(label)    */
    if (!strcmp(mnem, "jmp")) return x86_jmp(port);
    if (!strcmp(mnem, "def")) return x86_deflabel(port);
    return x86_jcc(mnem, port);
}
inline std::string x86(const char * mnem, x86_lbl lab) {                                       /* jmp / jcc / def to internal*/
    if (!strcmp(mnem, "jmp")) return x86_jmp_id(lab.n);
    if (!strcmp(mnem, "def")) return x86_deflabel_id(lab.n);
    return x86_jcc_id(mnem, lab.n);
}
inline std::string x86(const char * mnem, x86_frame f, const char * reg) {                     /* mov [r12+off], reg        */
    (void)mnem; return x86_frame_store(f.off, reg);
}
inline std::string x86(const char * mnem, const char * reg, x86_frame f) {                     /* mov/add reg, [r12+off]    */
    if (!strcmp(mnem, "add")) return x86_frame_add_to_reg(reg, f.off);
    return x86_frame_load(reg, f.off);
}
inline std::string x86(const char * mnem, x86_frame f, long imm) {                             /* mov/add dword[r12+off],imm*/
    if (!strcmp(mnem, "add")) return x86_frame_add_imm(f.off, imm);
    return x86_frame_mov_imm(f.off, imm);
}
inline std::string x86(const char * mnem, const char * a, const char * b) {                    /* reg/mem 2-operand         */
    if (!strcmp(mnem, "mov"))    return (a[0] == '[') ? x86_store_cursor_mirror() : x86_mov(a, b);
    if (!strcmp(mnem, "add"))    return x86_add_rr(a, b);
    if (!strcmp(mnem, "cmp"))    return x86_cmp(a, b);
    if (!strcmp(mnem, "test"))   return x86_test(a, b);
    if (!strcmp(mnem, "movsxd")) return x86_movsxd(a, b);
    if (!strcmp(mnem, "movzx"))  return x86_movzx_subj_byte(a);                                /* movzx a, byte[r13+rcx]    */
    if (!strcmp(mnem, "lea"))    return x86_lea_subj_cursor(a);                                 /* lea a, [r13 + rcx]        */
    return std::string();
}
inline std::string x86(const char * mnem, const char * reg, long imm) {                        /* reg, imm32                */
    if (!strcmp(mnem, "add")) return x86_add(reg, imm);
    if (!strcmp(mnem, "sub")) return x86_sub(reg, imm);
    if (!strcmp(mnem, "cmp")) return x86_cmp_imm(reg, imm);
    if (!strcmp(mnem, "mov")) return x86_movimm(reg, imm);
    return std::string();
}
inline std::string x86(const char * mnem, const char * sym, uint64_t ptr) {                    /* call sym (RO ptr)         */
    (void)mnem;
    return x86_call_ro(sym, ptr);
}
inline std::string x86(const char * mnem, const char * dst, const char * mem, uint64_t val, const char * label) { /* lea dst,[rip+RO] */
    (void)mnem; (void)mem;
    return x86_load_ro(dst, label, val);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* The consumer.  TEXT: passthrough.  BINARY: walk records, DISCOVERING byte positions (no bb_bin_t).      */
/* id < X86_INTERNAL_BASE → port label (g_emit.lbl_*_p); id >= base → a fresh box-local internal label,    */
/* defined/patched with the same primitives (the global patch list resolves forward refs within the walk).*/
inline struct bb_label_t * x86_label_for(int id, bb_label_t * internal) {
    return id < X86_INTERNAL_BASE ? x86_portlbl(id) : &internal[id - X86_INTERNAL_BASE];
}
inline void bb_emit_x86(const std::string & s) {
    if (!MEDIUM_BINARY) { if (!s.empty()) emit_text_n(s.data(), s.size()); return; }
    bb_label_t internal[X86_INTERNAL_MAX];
    for (int k = 0; k < X86_INTERNAL_MAX; k++) { internal[k].offset = BB_LABEL_UNRESOLVED; snprintf(internal[k].name, BB_LABEL_NAME_MAX, ".Lxi%d", k); }
    size_t i = 0, n = s.size();
    while (i < n) {
        char tag = s[i++];
        if (tag == 'L') { int k = (unsigned char)s[i++]; for (int j = 0; j < k; j++) bb_emit_byte((uint8_t)(unsigned char)s[i++]); }
        else if (tag == 'J') { int id = (unsigned char)s[i++]; bb_emit_patch_rel32(x86_label_for(id, internal)); }
        else if (tag == 'D') { int id = (unsigned char)s[i++]; bb_label_define(x86_label_for(id, internal)); }
        else break;
    }
}
#endif
