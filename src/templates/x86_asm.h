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
inline std::string x86_jcc(const char * mnem, int port) {
    return MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(port))
                         : (std::string(" ") + mnem + " " + x86_portname(port) + "\n");
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
inline std::string x86_port_canary() {
    if (x86_port_mode() != ZC_PORT_INSTRUMENTED) return std::string();
    if (MEDIUM_BINARY) return x86_Lrec(x86_b3(0x4D, 0x85, 0xE4) + x86_b2(0x75, 0x02) + x86_b2(0x0F, 0x0B));
    return std::string(" test r12, r12\n jnz 1f\n ud2\n1:\n");
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
inline std::string x86_jmp(int port) {
    std::string pre = x86_port_canary();
    if (port == X86P_OMEGA && getenv("SCRIP_ZETA_OMEGA_TRACE")) {
        /* Central-hook observability only (Claude, this session, per Lon's ask: trace both enters and both
         * exits from ONE place, without touching any template).  This fires once PER JMP "ω" SITE COMPILED
         * (i.e. at emit/codegen time, same as every other x86() call -- NOT a runtime per-execution trace;
         * this reports which ω sites exist and how each was classified, not how often each one runs). Fires
         * for EVERY jmp "ω", true-death or internal-alias alike -- op_omega_is_death distinguishes them,
         * deliberately NOT filtered out of the trace, so a person auditing it sees both classes side by side,
         * exactly the fact this session's own ARBNO investigation needed and had to reconstruct by hand from
         * source reading. The free-call this hook COULD make (x86_zeta_free_call) stays OFF regardless of
         * op_omega_is_death's value, exactly per the STEP 1 STATUS comment immediately above this function --
         * it frees r12 itself, wrong for ARBNO's carrier-based design; this trace does not change that. */
        fprintf(stderr, "[OMEGA-TRACE] x86_uid=%d op_omega_is_death=%s\n", _.x86_uid, _.op_omega_is_death ? "TRUE-DEATH" : "internal-alias");
    }
    return pre + (MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(port))
                                : (std::string(" jmp ") + x86_portname(port) + "\n"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_deflabel(int port) {
    std::string s = MEDIUM_BINARY ? x86_Drec(port) : (std::string(" ") + x86_portname(port) + ":\n");
    /* ZLS2 ARENA (Lon 2026-07-08): ALLOC flavor of the α port — the box self-allocates its frame with its
     * OWN emit-time constant on the DOWN-growing arena.  Fires only under SCRIP_ZETA_PORT=2 AND a nonzero
     * per-node grant (g_emit.op_zls2_bytes, zeroed at every DRIVE_FILL) — dormant everywhere until a
     * construct's driver arm opts in.  The matching free is NEVER hooked here or at x86_jmp(ω) (six-decoy-ω
     * finding): a construct places x86_zls2_free() at its own single true-exit label. */
    if (port == X86P_ALPHA && _.op_zls2_bytes > 0 && x86_port_mode() == ZC_PORT_ALLOC)
        s += x86_sub("r12", _.op_zls2_bytes);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZLS2 ω-side free: the box's own-constant up-bump, placed by the TEMPLATE at its single true-exit label,
 * immediately before the final jmp "ω".  Reads the same per-node grant as the α hook; same dormancy rules. */
inline std::string x86_zls2_free() {
    if (_.op_zls2_bytes > 0 && x86_port_mode() == ZC_PORT_ALLOC) return x86_add("r12", _.op_zls2_bytes);
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
#define X86_INTERNAL_MAX  16
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * L(int n) { static char b[8][8]; static int i; i = (i + 1) & 7; snprintf(b[i], 8, "L%d", n); return b[i]; }
inline std::string x86_internal_name(int n) { return std::string(".Lx") + std::to_string(_.x86_uid) + "_" + std::to_string(n); }
inline std::string LS(int n) { return x86_internal_name(n) + "_s"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_id(int n) {
    return MEDIUM_BINARY ? (x86_Lrec(x86_b1(0xE9)) + x86_Jrec(X86_INTERNAL_BASE + n))
                         : (std::string(" jmp ") + x86_internal_name(n) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jcc_id(const char * mnem, int n) {
    return MEDIUM_BINARY ? (x86_Lrec(x86_b2(0x0F, x86_jcc_op(mnem))) + x86_Jrec(X86_INTERNAL_BASE + n))
                         : (std::string(" ") + mnem + " " + x86_internal_name(n) + "\n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_deflabel_id(int n) {
    return MEDIUM_BINARY ? x86_Drec(X86_INTERNAL_BASE + n) : (x86_internal_name(n) + ":\n");
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
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x48; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8D; c += (char)(0x05 | ((g & 7) << 3)); return x86_Lrec(c) + x86_Jrec(X86_INTERNAL_BASE + n); }
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
    std::string s; int rf = regfield & 7;
    if (off == 0)                       { s += (char)((0 << 6) | (rf << 3) | 4); s += (char)0x24; }
    else if (off >= -128 && off <= 127) { s += (char)((1 << 6) | (rf << 3) | 4); s += (char)0x24; s += (char)(int8_t)off; }
    else                                { s += (char)((2 << 6) | (rf << 3) | 4); s += (char)0x24; s += u32le((uint32_t)off); }
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_text_mem(int off) { return std::string("[r12 + ") + std::to_string(off) + "]"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_lea(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x49; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8D; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" lea ") + reg + ", " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_mov_imm(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0x41; c += (char)0xC7; c += x86_r12_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return std::string(" mov dword ptr ") + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_store(int off, const char * reg) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x41; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x89; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov dword ptr ") + x86_frame_text_mem(off) + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_load(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x41; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x8B; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov ") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_add_imm(int off, long imm) {
    if (MEDIUM_BINARY) {
        std::string c; c += (char)0x41;
        if (imm >= -128 && imm <= 127) { c += (char)0x83; c += x86_r12_modrm(0, off); c += (char)(uint8_t)(int8_t)imm; }
        else                           { c += (char)0x81; c += x86_r12_modrm(0, off); c += u32le((uint32_t)imm); }
        return x86_Lrec(c);
    }
    return std::string(" add dword ptr ") + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_add_to_reg(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x41; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x03; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" add ") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_sub_from_reg(const char * reg, int off) {
    int g = x86_rnum(reg);
    if (MEDIUM_BINARY) { std::string c; uint8_t rex = 0x41; if (g >= 8) rex |= 0x04; c += (char)rex; c += (char)0x2B; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" sub ") + reg + ", dword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * FR(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "dword ptr [r12 + %d]", off); return b[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_load64(const char * reg, int off) {
    int g = x86_rnum(reg); uint8_t rex = 0x49; if (g >= 8) rex |= 0x04;
    if (MEDIUM_BINARY) { std::string c; c += (char)rex; c += (char)0x8B; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov ") + reg + ", qword ptr " + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_store64(int off, const char * reg) {
    int g = x86_rnum(reg); uint8_t rex = 0x49; if (g >= 8) rex |= 0x04;
    if (MEDIUM_BINARY) { std::string c; c += (char)rex; c += (char)0x89; c += x86_r12_modrm(g, off); return x86_Lrec(c); }
    return std::string(" mov qword ptr ") + x86_frame_text_mem(off) + ", " + reg + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_jmp_frame64(int off) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0x49; c += (char)0xFF; c += x86_r12_modrm(4, off); return x86_Lrec(c); }
    return std::string(" jmp qword ptr ") + x86_frame_text_mem(off) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_frame_mov_imm64(int off, long imm) {
    if (MEDIUM_BINARY) { std::string c; c += (char)0x49; c += (char)0xC7; c += x86_r12_modrm(0, off); c += u32le((uint32_t)imm); return x86_Lrec(c); }
    return std::string(" mov qword ptr ") + x86_frame_text_mem(off) + ", " + std::to_string(imm) + "\n";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline const char * FRQ(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [r12 + %d]", off); return b[i]; }
inline const char * ROQ(int n)   { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rip + %d]", n); return b[i]; }
inline const char * RDQ(const char * base, int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [%s + %d]", base, off); return b[i]; }
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
inline std::string x86_reg_disp32_store_imm64(const char * base, int disp, long imm) {
    int b = x86_rnum(base);
    if (MEDIUM_BINARY) {
        std::string c; uint8_t rex = 0x48; if (b >= 8) rex |= 0x01; c += (char)rex; c += (char)0xC7; c += (char)(0x80 | (b & 7)); c += u32le((uint32_t)disp); c += u32le((uint32_t)imm);
        return x86_Lrec(c);
    }
    return std::string(" mov qword ptr [") + base + " + " + std::to_string(disp) + "], " + std::to_string(imm) + "\n";
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
    std::string code; code += (char)0x49; code += (char)0xFF; code += x86_r12_modrm(0, off);
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
enum { XK_NONE = 0, XK_REG, XK_IMM, XK_PORT, XK_ILBL, XK_FR32, XK_FR64, XK_RSP64, XK_MEMIND, XK_MEMIDX8, XK_R13RCX, XK_R10MIR, XK_RIPSEAL, XK_REGDISP, XK_SYM, XK_ROSLOT, XK_EXTLBL };
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
    if (p >= 0 && (s[2] == 0)) { o.kind = XK_PORT; o.port = p; return; }
    if (s[0] == 'L' && s[1] >= '0' && s[1] <= '9') { int n = atoi(s + 1); o.kind = XK_ILBL; o.lbl = n; return; }
    if (!strcmp(s, "extlbl")) { o.kind = XK_EXTLBL; return; }
    if (!strncmp(s, "dword ptr [r12 + ", 17)) { o.kind = XK_FR32;  o.off = atoi(s + 17); return; }
    if (!strncmp(s, "qword ptr [r12 + ", 17)) { o.kind = XK_FR64;  o.off = atoi(s + 17); return; }
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
        if (a.kind == XK_REG && b.kind == XK_MEMIDX8)  return x86_load_indexed8(a.txt, b.base, b.idx);
        if (a.kind == XK_REG && b.kind == XK_MEMIND)   return x86_load_mem64(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_RIPSEAL)  return x86_load_ro(a.txt, xd.s, xc.u);
        if (a.kind == XK_REG && b.kind == XK_REG)      return x86_mov(a.txt, b.txt);
        if (a.kind == XK_REG && b.kind == XK_IMM)      return x86_movimm(a.txt, b.imm);
        return std::string();
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
/* x86_zeta_free_call() definition (forward-declared above x86_jmp; see that declaration's comment for why
 * this must live here, after x86() itself is defined). */
inline std::string x86_zeta_free_call() {
    return x86("push", "rbp")
         + x86("mov",  "rbp", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("mov",  "rdi", "r12")
         + x86("call", "rt_zls_release", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_release)
         + x86("mov",  "rsp", "rbp")
         + x86("pop",  "rbp");
}
extern "C" void *rt_zls_mark(void);
extern "C" void  rt_zls_release_to(void *);
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
         + x86("push", "rbp")
         + x86("mov",  "rbp", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("call", "rt_zls_mark", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_zls_mark)
         + x86("mov",  "rsp", "rbp")
         + x86("pop",  "rbp")
         + x86("mov",  FRQ(off), "rax")
         + x86("pop",  "rsi");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
inline std::string x86_zeta_release_to_call(int off) {
    return x86("push", "rbp")
         + x86("mov",  "rbp", "rsp")
         + x86("and",  "rsp", -16L)
         + x86("mov",  "rdi", FRQ(off))
         + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_zls_release_to)
         + x86("mov",  "rsp", "rbp")
         + x86("pop",  "rbp");
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
inline std::string x86_pair_loop() {
    std::string r;
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++) {
        if (MEDIUM_BINARY) {
            if (g_emit.xa_bb_emit_pair_define[i]) { r += (char)'E'; r += (char)(unsigned char)i; r += x86_port_canary(); }
            if (g_emit.xa_bb_emit_pair_jmp[i])    { r += x86_Lrec(x86_b1(0xE9)); r += (char)'F'; r += (char)(unsigned char)i; }
        } else {
            if (g_emit.xa_bb_emit_pair_define[i]) { r += emit_fmt("%s:\n", g_emit.xa_bb_emit_pair_define[i]->name); r += x86_port_canary(); }
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
    return id < X86_INTERNAL_BASE ? x86_portlbl(id) : &internal[id - X86_INTERNAL_BASE];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
        else if (tag == 'E') { int idx = (unsigned char)s[i++]; if (g_emit.xa_bb_emit_pair_define[idx]) bb_label_define(g_emit.xa_bb_emit_pair_define[idx]); }
        else if (tag == 'F') { int idx = (unsigned char)s[i++]; if (g_emit.xa_bb_emit_pair_jmp[idx]) bb_emit_patch_rel32(g_emit.xa_bb_emit_pair_jmp[idx]); }
        else if (tag == 'X') { uint64_t v = 0; for (int j = 0; j < 8; j++) v |= ((uint64_t)(unsigned char)s[i++]) << (8 * j); bb_emit_patch_rel32((bb_label_t *)(uintptr_t)v); }
        else break;
    }
}
#endif
