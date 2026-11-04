#ifndef X86_RTCC_H
#define X86_RTCC_H
#include <cstdint>
#include <cstring>
#include <string>
#include "runtime/rtx/rtcc.h"
extern "C++" {
#define RTX_VERDICT_AL 0
#define RTX_VERDICT_INT 1
#define RTX_VERDICT_PTR 2
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_cset_probe();
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_cset_bt();
std::string x86_call_text(const char * sym, const char * sfx);
std::string x86_rtx_verdict_adj(const char * sym, int adj);
std::string x86_call_ro(const char * sym, uint64_t ptr);
#define RTCC_C_R8 1u
#define RTCC_C_R9 2u
#define RTCC_C_R10 4u
#define RTCC_C_R11 8u
#define RTCC_C_ALL (RTCC_C_R8 | RTCC_C_R9 | RTCC_C_R10 | RTCC_C_R11)
unsigned x86_rtcc_live_mask(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned x86_rtcc_clob_raw(const char * sym);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
constexpr bool x86_rtcc_streq(const char * a, const char * b) { return *a == *b && (*a == '\0' ? true : x86_rtcc_streq(a + 1, b + 1)); }
static_assert(RTCC_SLOT_RAX * 8 == 0, "RTCC ABI drift: RTCC_SLOT_RAX no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RCX * 8 == 8, "RTCC ABI drift: RTCC_SLOT_RCX no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RDX * 8 == 16, "RTCC ABI drift: RTCC_SLOT_RDX no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RSI * 8 == 24, "RTCC ABI drift: RTCC_SLOT_RSI no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_RDI * 8 == 32, "RTCC ABI drift: RTCC_SLOT_RDI no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin");
static_assert(RTCC_SLOT_R8 * 8 == 40, "RTCC ABI drift: RTCC_SLOT_R8 no longer matches the literal offset in x86_rtcc_wb_bin/x86_rtcc_rl_bin (and keywords.c ANCHOR companion write)");
static_assert(RTCC_SLOT_R9 * 8 == 48, "RTCC ABI drift: RTCC_SLOT_R9 no longer matches the literal offset in x86_rtcc_rl_bin (and the rtcc_init RT_GVA_VA seed) — this is the H2 SIGSEGV class");
static_assert(x86_rtcc_streq(RTCC_GVA_REG, "r9"),
    "RTCC ABI drift: RTCC_GVA_REG no longer names the register the reload encoders load from slot 6 (mov r9,[rcx+48]) — GVARQ would address a register the veneer never seeds");
static_assert(RTCC_GPR_COUNT == 9 && RTCC_GPR_BYTES == 72, "RTCC ABI drift: GPR tier width no longer matches the 9 slots the encoders write back and reload");
#define PIN_FRONTIER_REG "rbx"
#define PIN_CAS_TOP_REG "r12"
#define PIN_SIGMA_REG "r13"
#define PIN_CURSOR_REG "r14"
#define PIN_SUBJLEN_REG "r15"
static_assert(x86_rtcc_streq(PIN_FRONTIER_REG, "rbx"),
    "BLOB PIN DRIFT (ARCH-SNOBOL4-RTX section 2 REGISTER CONTRACT, which ARCH-ICON-RTX names as THE single source): rbx is the arena heap top, the DESCR mint pointer. This assert exists because that"
    " plane DID drift -- the frontier lived in the HEAP arm of the port selector the no-modes law deleted (that selector's identifiers are themselves forbidden under src/ by test_gate_no_zeta_frame_"
    "switches.sh, R-448, so it is described here and never spelled), and rbx, preserved by SysV and therefore free-looking, was picked up as scratch by four sites for months. Nothing failed, becau"
    "se a register plane written in a table and enforced nowhere cannot fail. r9 never drifted in the same years, and the only difference is the RTCC_GVA_REG assert above. One assert per pin is the "
    "cure (cto 2026-09-20, row gc-rbx-is-the-bump-frontier step 3; R-959).");
static_assert(x86_rtcc_streq(PIN_CAS_TOP_REG, "r12"),
    "BLOB PIN DRIFT: r12 is the CONDITIONAL-ASSIGNMENT STACK TOP (Lon 2026-08-02, reinstated s23k; the CAS BASE is a MATCH_BEGIN frame slot and not a register). The C-to-BB trampolines seed it from "
    "the RT_DCAP_TOP cell and the match templates advance it by 24 per pended capture.");
static_assert(x86_rtcc_streq(PIN_SIGMA_REG, "r13"),
    "BLOB PIN DRIFT: r13 is the subject base pointer, the one Lon named in-chat 2026-09-19 -- r13 for SNOBOL4 and Icon points to the subject string. rt.c's generator entry loads it from 24(%rdi) and"
    " rt_coexpr.c's package from 16(%0); the chain trampolines seed it from the parked subject.");
static_assert(x86_rtcc_streq(PIN_CURSOR_REG, "r14"), "BLOB PIN DRIFT: r14 is the subject cursor, the delta the match spine advances and every capture records.");
static_assert(x86_rtcc_streq(PIN_SUBJLEN_REG, "r15"), "BLOB PIN DRIFT: r15 is the subject length, which the defer road's close compares the capture end against before it reads a byte.");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_wb_bin(uint64_t block, unsigned m = RTCC_C_ALL);
static_assert(RTCC_SLOT_R11 * 8 == 64,
    "RTCC ABI drift: the binary write-back pops r11's pre-call value straight into slot 8 (pop qword ptr [r11+64]) -- slot 8 must stay r11's, or the pop lands in another register's slot");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_rl_bin(uint64_t block, unsigned m = RTCC_C_ALL);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_call_b(uint64_t ptr);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_wb_text(unsigned m = RTCC_C_ALL);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_rl_text(unsigned m = RTCC_C_ALL);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtx_reestablish(unsigned m);
std::string x86_rtcc_call_adj(const char * sym, uint64_t ptr, int adj);
std::string x86_rtcc_call(const char * sym, uint64_t ptr);
std::string x86_rtcc_call_descr(const char * sym, uint64_t ptr, int slot);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_call_dc(const char * dcname, uint64_t slot);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string x86_rtcc_call_descr_ops(const char * sym, uint64_t ptr, const std::string & r0, const std::string & r8);
}
#endif
