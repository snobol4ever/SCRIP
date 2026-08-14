#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" long rt_cap_open(const char *varname, int saved_delta, int cur_delta, int is_imm);
extern "C" void *rt_proc_open_fn(void);
extern "C" DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
extern "C" DESCR_t rt_proc_call_epilogue_ω(void);
extern "C" void rt_cap_finish(DESCR_t fret);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* s83 CAPTURE REBUILD (Lon in-chat, this session): one mechanism for the CROSSING case, not seven -- but the
 * ORDINARY (non-crossing) case genuinely has TWO live, mutually exclusive home regimes staged by emit.cpp,
 * discovered the hard way (s83b) after a first-pass rebuild silently collapsed them into one and reproduced the
 * exact CAP-SYM disease this file's history already named ("producer wrote one place, consumer read another").
 * op_cap_anchor (cap_anchor_of()) and op_zres (g_zd_arm) are SEPARATE compile-time verdicts, never both set on
 * the same node in what's been exercised so far: anchor => FR(op_off), the node's own ZLS slot; zres => ZRESD(0)
 * write / ZOPD(1,0) cross-box read, the ZD-armed spine cell. Neither set => the old file's third, legacy fallback
 * (a C rt_cap_push/rt_cap_top array) which is NOT rebuilt here -- BOMBS explicitly rather than guessing which
 * of the two homes might coincidentally work, or reintroducing the C array Lon separately directed deleted.
 *
 * THE LAW (s81 ruling, for the CROSSING case): RSP owns ZETA-SPINE for ordinary captures (either home above);
 * a capture whose SAVE and IMM/COND straddle unbounded stack growth (a non-static DEFER or MATCH_VALUE between
 * them, per op_frame_need -- EARN-1, and its s83 SAVE-propagation fix in emit.cpp's frame_need_of) moves into a
 * ZETA-ACTIVATION-FRAME on RBP instead, one per syntactic instance. SAVE establishes the frame (push rbp; mov
 * rbp,rsp) and writes its delta at [rbp-8]; IMM/COND read it back and tear the frame down (mov rsp,rbp; pop rbp)
 * before doing anything else. gdb-measured root cause (s82): ZRESD/ZOPD and FR(op_off) are BOTH ultimately
 * RSP-relative ([rsp#+N]) -- sound only when SAVE and its reader see the SAME rsp, which a non-self-releasing
 * DEFER call breaks. RBP does not move across that call, so it is immune by construction.
 *
 * COND (`.` conditional assign) parks {varname, start, len} on the r12 CAS pend-array -- pure asm, no C call
 * (CAS-R12-UNIFY: r12 IS the live pend top on every arm). IMM (`$` immediate assign) calls rt_cap_open for the
 * plain-name construct-and-store step -- rt_cap_open's ARM A (rtx_match.S) is hand-written asm, not C, and
 * already proven; this rung fixes WHAT delta reaches it, not how it builds the DESCR. The `*VAR` computed-name
 * path (rt_cap_open ARM B / c_rt_cap_open, g_capx) is UNTOUCHED and still C -- Lon separately directed its
 * deletion in favor of ordinary BB RESULT/operand dataflow, but that is BLOCKED on the :(NRETURN) lowering bug
 * (s82: does not compile at all today) and is not attempted here; a computed-name capture bombs honestly. */
static inline int havehome(void) { return _.op_zres || _.op_cap_anchor; }
static inline const char * writehome(void) { return _.op_zres ? ZRESD(0) : FR(_.op_off); }
static inline const char * readhome(void)  { return _.op_zres ? ZOPD(1, 0) : FR(_.op_off); }
std::string bb_match_capture() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    return (_.op_off < 0)
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_ASSIGN: capture stack slot not promoted (flat_drive_capture)") )
         : !(_.op_sval ? _.op_sval : "")[0]
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_ASSIGN: empty capture variable name") )
         : (int)_.op_phase == 0 && _.op_frame_need
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE activation frame (s81/s83: crosses unbounded stack growth)")
           + x86_alpha()
           + x86("push", "rbp")
           + x86("mov",  "rbp", "rsp")
           + x86("sub",  "rsp", 8L)
           + x86("mov",  RDD("rbp", -8), "r14d")
           + x86_gamma()
           + x86_beta_trampoline() )
         : (int)_.op_phase == 0 && havehome()
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE spine (ordinary, unchanged mechanism)")
           + x86_alpha()
           + x86("mov",  writehome(), "r14d")
           + x86_gamma()
           + x86_beta_trampoline() )
         : (int)_.op_phase == 0
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_SAVE: neither op_zres nor op_cap_anchor granted a home -- the legacy C rt_cap_push array this used to fall back to is deliberately not rebuilt (s83)") )
         : (int)_.op_phase == 1 && _.op_frame_need
         ? ( x86("comment", "IR_MATCH_CAPTURE_COND activation frame (s81/s83)")
           + x86_alpha()
           + x86("mov",  "eax", RDD("rbp", -8))
           + x86("mov",  "rsp", "rbp")
           + x86("pop",  "rbp")
           + x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
           + x86("mov",  RDQ("r12", 0), "rcx")
           + x86("mov",  "esi", "eax")
           + x86("mov",  RDQ("r12", 8), "rsi")
           + x86("mov",  "edx", "r14d")
           + x86("sub",  "edx", "eax")
           + x86("mov",  RDQ("r12", 16), "rdx")
           + x86("add",  "r12", (long)24)
           + x86_gamma()
           + x86_beta()
           + x86("sub",  "r12", (long)24)
           + x86_omega() )
         : (int)_.op_phase == 1 && havehome()
         ? ( x86("comment", "IR_MATCH_CAPTURE_COND spine (ordinary, unchanged mechanism: r12 CAS pend-push, pure asm)")
           + x86_alpha()
           + x86("mov",  "eax", readhome())
           + x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
           + x86("mov",  RDQ("r12", 0), "rcx")
           + x86("mov",  "esi", "eax")
           + x86("mov",  RDQ("r12", 8), "rsi")
           + x86("mov",  "edx", "r14d")
           + x86("sub",  "edx", "eax")
           + x86("mov",  RDQ("r12", 16), "rdx")
           + x86("add",  "r12", (long)24)
           + x86_gamma()
           + x86_beta()
           + x86("sub",  "r12", (long)24)
           + x86_omega() )
         : (int)_.op_phase == 1
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_COND: neither op_zres nor op_cap_anchor granted a home -- the legacy C rt_cap_top array this used to fall back to is deliberately not rebuilt (s83)") )
         : (_.op_sval && _.op_sval[0] == '*')
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_IMM: computed-name (*VAR/NRETURN) target not yet rebuilt -- blocked on the :(NRETURN) lowering bug (s82), see this file's header comment") )
         : (int)_.op_phase == 2 && _.op_frame_need
         ? ( x86("comment", "IR_MATCH_CAPTURE_IMM activation frame (s81/s83)")
           + x86_alpha()
           + x86("mov",  "eax", RDD("rbp", -8))
           + x86("mov",  "rsp", "rbp")
           + x86("pop",  "rbp")
           + x86_anchor_enter()
           + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
           + x86("mov",  "esi", "eax")
           + x86("mov",  "edx", "r14d")
           + x86("mov",  "ecx", (long)1)
           + x86("call", "rt_cap_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int, int, int))rt_cap_open)
           + x86("test", "rax", "rax")
           + x86("je",   L(1))
           + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
           + bb_glue_pass_wires(2, 3)
           + x86("def",  L(2))
           + x86("call", "rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
           + x86("mov",  "rdi", "rax")
           + x86("mov",  "rsi", "rdx")
           + x86("call", "rt_cap_finish", (uint64_t)(uintptr_t)(void *)(void (*)(DESCR_t))rt_cap_finish)
           + x86("jmp",  L(1))
           + x86("def",  L(3))
           + x86("call", "rt_proc_call_epilogue_ω", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(void))rt_proc_call_epilogue_ω)
           + x86("mov",  "rdi", "rax")
           + x86("mov",  "rsi", "rdx")
           + x86("call", "rt_cap_finish", (uint64_t)(uintptr_t)(void *)(void (*)(DESCR_t))rt_cap_finish)
           + x86("def",  L(1))
           + x86_anchor_leave()
           + x86_gamma()
           + x86_beta_trampoline() )
         : havehome()
         ? ( x86("comment", "IR_MATCH_CAPTURE_IMM spine (ordinary, unchanged mechanism)")
           + x86_alpha()
           + x86("mov",  "eax", readhome())
           + x86_anchor_enter()
           + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
           + x86("mov",  "esi", "eax")
           + x86("mov",  "edx", "r14d")
           + x86("mov",  "ecx", (long)1)
           + x86("call", "rt_cap_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int, int, int))rt_cap_open)
           + x86("test", "rax", "rax")
           + x86("je",   L(1))
           + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
           + bb_glue_pass_wires(2, 3)
           + x86("def",  L(2))
           + x86("call", "rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
           + x86("mov",  "rdi", "rax")
           + x86("mov",  "rsi", "rdx")
           + x86("call", "rt_cap_finish", (uint64_t)(uintptr_t)(void *)(void (*)(DESCR_t))rt_cap_finish)
           + x86("jmp",  L(1))
           + x86("def",  L(3))
           + x86("call", "rt_proc_call_epilogue_ω", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(void))rt_proc_call_epilogue_ω)
           + x86("mov",  "rdi", "rax")
           + x86("mov",  "rsi", "rdx")
           + x86("call", "rt_cap_finish", (uint64_t)(uintptr_t)(void *)(void (*)(DESCR_t))rt_cap_finish)
           + x86("def",  L(1))
           + x86_anchor_leave()
           + x86_gamma()
           + x86_beta_trampoline() )
         : ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_IMM: neither op_zres nor op_cap_anchor granted a home -- the legacy C rt_cap_top array this used to fall back to is deliberately not rebuilt (s83)") );
}
