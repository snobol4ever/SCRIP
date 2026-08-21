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
 * THE LAW (s81 ruling, for the CROSSING case, REVISED s88): RSP owns ZETA-SPINE for ordinary captures (either
 * home above); a capture whose SAVE and IMM/COND straddle unbounded stack growth (a non-static DEFER or
 * MATCH_VALUE between them, per op_frame_need -- EARN-1, and its s83 SAVE-propagation fix in emit.cpp's
 * frame_need_of) needs storage that survives that crossing. gdb-measured root cause (s82): ZRESD/ZOPD and
 * FR(op_off) are BOTH ultimately RSP-relative ([rsp#+N]) -- sound only when SAVE and its reader see the SAME
 * rsp, which a non-self-releasing DEFER call breaks.
 *
 * ⛔ s88 REVERT (Lon in-chat: "I tried creating BBs each with their own transient RBP frame for ARBNO and
 * *P $ VAR and *P . VAR, but it failed when a crossover landed where an RBP was the nested one not the outer
 * one -- CAPTURE can never work like that"): CAPTURE NEVER PUSHES ITS OWN RBP FRAME. THE ONLY TWO operator-BBs
 * permitted to push rbp, product-wide, are IR_MATCH_BEGIN (ζ-STANDING, S ? P) and IR_MATCH_DEFER (ζ-FRAME, *P)
 * -- see ARCH-SCRIP.md's ζ-tier table. Every other construct is ζ-SPINE by default (RSP, this file's havehome()
 * arm) and, when it needs a cell that survives a DEFER crossing, REGISTERS A SLOT in the closest already-live
 * ζ-STANDING/ζ-FRAME activation (op_cap_frame_off, capture_frame_slot() in emit.cpp) rather than establishing
 * a frame of its own. The s81/s83 arm this comment used to describe (SAVE does push rbp; mov rbp,rsp and writes
 * [rbp-8]; IMM/COND read it back and mov rsp,rbp; pop rbp) is DELETED: measured (D11 probe, THREE ZETAS s86/s87)
 * to collide with an ARBNO textually nested inside its own SAVE..IMM span -- ARBNO's alpha, assuming "the rbp
 * live right now IS ζ-STANDING" (a correct assumption once ARBNO itself stopped pushing frames, s79/s80), wrote
 * its cell relative to CAPTURE's tiny transient frame instead; CAPTURE's own IMM then popped that frame out
 * from under ARBNO before ARBNO's own retries were done, corrupting the yield cursor on backtrack. When
 * op_cap_frame_off == -1 (no enclosing MATCH_BEGIN in scope -- the PAT$N stored-pattern-blob boundary; or
 * SCRIP_MATCH_RBP=0), CAPTURE now BOMBS honestly instead of reaching for its own frame -- the identical refuse
 * bb_match_arbno.cpp's own ARBNO-FRAME arm already takes at the same boundary (arbno_frame_off == -1).
 *
 * COND (`.` conditional assign) parks {varname, start, len} on the r12 CAS pend-array -- pure asm, no C call
 * (CAS-R12-UNIFY: r12 IS the live pend top on every arm). IMM (`$` immediate assign) calls rt_cap_open for the
 * plain-name construct-and-store step -- rt_cap_open's ARM A (rtx_match.S) is hand-written asm, not C, and
 * already proven; this rung fixes WHAT delta reaches it, not how it builds the DESCR. The `*VAR` computed-name
 * path (rt_cap_open ARM B / c_rt_cap_open, g_capx) is UNTOUCHED and still C -- Lon separately directed its
 * deletion in favor of ordinary BB RESULT/operand dataflow, but that is BLOCKED on the :(NRETURN) lowering bug
 * (s82: does not compile at all today) and is not attempted here; a computed-name capture bombs honestly. */
static inline int havehome(void) { return _.op_zres || _.op_cap_anchor; }
static inline int nret_cap_live(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_NRET_CAP"); v = e ? (e[0] != '0') : 1; } return v; }   /* s108 (Lon ruling in-chat): DEFAULT ON -- pairs with SCRIP_DYN_ALPHA; semantic_driver measured green with both on.  Killswitch SCRIP_NRET_CAP=0 restores the pre-s108 default. */
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
         : (int)_.op_phase == 0 && _.op_frame_need && _.op_cap_frame_off != -1
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE activation-frame SLOT (THREE ZETAS s87: registered in the CURRENT live frame, no push of its own -- see op_cap_frame_off)")
           + x86_alpha()
           + x86("mov",  CFC(0), "r14d")
           + x86_gamma()
           + x86_beta_trampoline() )
         : (int)_.op_phase == 0 && _.op_frame_need
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_SAVE: hazard crosses a DEFER-unsafe boundary but op_cap_frame_off is unavailable (no enclosing MATCH_BEGIN in scope, or SCRIP_MATCH_RBP=0) -- CAPTURE never pushes its own activation frame (s88 revert: the s81/s83 own-transient-rbp arm crossed over with an ARBNO nested in its own span and corrupted the yield cursor, D11). Honest refuse, matching bb_match_arbno.cpp's identical boundary bomb.")
           + x86_beta_trampoline() )   /* dead code after the bomb's abort/ud2 -- beta is unreachable at runtime (alpha always bombs first) but the label must still be DEFINED for any sibling box's forward jump to it to resolve at emit-end, same discipline as bb_match_arbno.cpp's bomb arm defining PAIR(2)/PAIR(3). */
         : (int)_.op_phase == 0 && havehome()
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE spine (ordinary, unchanged mechanism)")
           + x86_alpha()
           + x86("mov",  writehome(), "r14d")
           + x86_gamma()
           + x86_beta_trampoline() )
         : (int)_.op_phase == 0
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_SAVE: no home -- neither a ζ-SPINE cell (op_zres) nor a ζ-STANDING slot (frame_need_of: DEFER-hazard / ALT-arm classes); classifier and ZD plan disagree on this node -- the legacy C rt_cap_push fallback is deliberately not rebuilt (s83)")
           + x86_beta_trampoline() )   /* R-0 (s93): dead code after the bomb, but β is DEFINED -- a port-less bomb whose sibling references β is a LINK BUG (m1_alt_* witnesses), never a refuse */
         : (int)_.op_phase == 1 && _.op_sval && _.op_sval[0] == '*' && !nret_cap_live()
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_COND: computed-name (*VAR/NRETURN) target not yet rebuilt -- blocked on the :(NRETURN) lowering bug (s82), see this file's header comment")
           + x86_beta() )   /* s104: WIP arm, DEFAULT OFF (opt-in SCRIP_NRET_CAP=1) pending the dyn-entry protocol fix (see s104 cursor): '*' targets fall through to the ordinary frame/spine pend-push arms -- rt_dcap_pump's '*' transfer (pattern_match.c NCB-1c) opens the proc want_name=1 and rt_dcap_step assigns through the returned NAME. */
         : (int)_.op_phase == 1 && _.op_frame_need && _.op_cap_frame_off != -1
         ? ( x86("comment", "IR_MATCH_CAPTURE_COND activation-frame SLOT (THREE ZETAS s87: reads the registered slot, no pop of its own -- the frame it lives in is released by whoever established it, MATCH_BEGIN/DEFER)")
           + x86_alpha()
           + x86("mov",  "eax", CFC(0))
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
         : (int)_.op_phase == 1 && _.op_frame_need
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_COND: hazard crosses a DEFER-unsafe boundary but op_cap_frame_off is unavailable -- CAPTURE never pushes its own activation frame (s88 revert), see IR_MATCH_CAPTURE_SAVE's bomb for the full rationale.")
           + x86_beta() )   /* dead code after the bomb -- see IR_MATCH_CAPTURE_SAVE's identical note; COND's live arms use the bare x86_beta() define (not the trampoline), so that is what's mirrored here. */
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
           + x86_bomb("IR_MATCH_CAPTURE_COND: no home -- neither a ζ-SPINE cell (op_zres) nor a ζ-STANDING slot (frame_need_of: DEFER-hazard / ALT-arm classes); classifier and ZD plan disagree on this node -- the legacy C rt_cap_top fallback is deliberately not rebuilt (s83)")
           + x86_beta() )   /* R-0 (s93): dead code after the bomb, but β is DEFINED -- ALT's β does jmp [resume_ptr] into this box's β (n8_match_assign_cond_β was the M1 link failure), never leave it undefined */
         : (_.op_sval && _.op_sval[0] == '*' && !nret_cap_live())
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_IMM: computed-name (*VAR/NRETURN) target not yet rebuilt -- blocked on the :(NRETURN) lowering bug (s82), see this file's header comment") )
         : (int)_.op_phase == 2 && _.op_frame_need && _.op_cap_frame_off != -1
         ? ( x86("comment", "IR_MATCH_CAPTURE_IMM activation-frame SLOT (THREE ZETAS s87: reads the registered slot, no pop of its own)")
           + x86_alpha()
           + x86("mov",  "eax", CFC(0))
           + x86_anchor_enter()
           + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
           + x86("mov",  "esi", "eax")
           + x86("mov",  "edx", "r14d")
           + x86("mov",  "ecx", (long)1)
           + x86("call", "rt_cap_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int, int, int))rt_cap_open)
           + (cap_fail_retreat() ? (x86("cmp", "rax", (long)-1) + x86("je", L(4))) : std::string())   /* ⭐⭐⭐ s193 THE FAILURE EDGE, and it MUST precede the transfer test: rt_cap_open resolves a `*target` itself and answers -1 when that target FAILED, but the next instruction treats every non-zero rax as a TRANSFER ADDRESS to jmp -- so the sentinel is read first or it becomes a jump to address -1.  0 still means fully-handled and a positive fbytes still means a transfer is owed; both fall through untouched. */
           + x86("test", "rax", "rax")
           + x86("je",   L(1))
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
           + (cap_fail_retreat() ? (x86_anchor_leave() + x86_jmp(X86P_OMEGA)) : std::string())   /* ⭐⭐⭐ s193 THE FAILURE EDGE THE IMM CAPTURE ROAD NEVER HAD: this is the ω landing (the target proc FAILED), and without this line it FALLS THROUGH into L(1) -- the label the γ arm jumps to -- so a failing deferred target matched exactly like a succeeding one.  anchor_leave FIRST because x86_anchor_enter above is an alignment pair and the concede path must balance it exactly once; then CONCEDE on the box's own ω wire, which is where x86_beta_trampoline already routes β, so no new edge is invented.  Disarmed emits the empty string = the fall-through VERBATIM. */
           + x86("def",  L(1))
           + x86_anchor_leave()
           + x86_gamma()
           + x86_beta_trampoline()
           + (cap_fail_retreat() ? (x86("def", L(4)) + x86_anchor_leave() + x86_omega()) : std::string()) )   /* ⭐⭐⭐ s193 the -1 landing: leave the alignment anchor EXACTLY ONCE (x86_anchor_enter above is a pair and this path never reached L(1)'s leave) and CONCEDE on the box's own ω wire -- the same wire x86_beta_trampoline routes β through, so no new edge is invented.  ⛔ CONCEDE, NOT ABORT, ORACLE-RULED: `(SPAN('ABC') $ *chk('NO') | 'ABC')` prints one CHK and then MATCHES on live sbl -bf, so a FAILING target retreats and the next alternative runs -- unlike a NOT-A-NAME target, which aborts the statement through IR_MATCH_END's ω (s170 SN4-CAP-NAME-STRICT).  Two different verdicts on one road; do not merge them.  Disarmed emits the empty string. */
         : (int)_.op_phase == 2 && _.op_frame_need
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_IMM: hazard crosses a DEFER-unsafe boundary but op_cap_frame_off is unavailable -- CAPTURE never pushes its own activation frame (s88 revert), see IR_MATCH_CAPTURE_SAVE's bomb for the full rationale.")
           + x86_beta_trampoline() )   /* dead code after the bomb -- see IR_MATCH_CAPTURE_SAVE's identical note; IMM's live arms end x86_beta_trampoline() same as SAVE. */
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
           + (cap_fail_retreat() ? (x86("cmp", "rax", (long)-1) + x86("je", L(4))) : std::string())   /* ⭐⭐⭐ s193 THE FAILURE EDGE, and it MUST precede the transfer test: rt_cap_open resolves a `*target` itself and answers -1 when that target FAILED, but the next instruction treats every non-zero rax as a TRANSFER ADDRESS to jmp -- so the sentinel is read first or it becomes a jump to address -1.  0 still means fully-handled and a positive fbytes still means a transfer is owed; both fall through untouched. */
           + x86("test", "rax", "rax")
           + x86("je",   L(1))
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
           + (cap_fail_retreat() ? (x86_anchor_leave() + x86_jmp(X86P_OMEGA)) : std::string())   /* ⭐⭐⭐ s193 THE FAILURE EDGE THE IMM CAPTURE ROAD NEVER HAD: this is the ω landing (the target proc FAILED), and without this line it FALLS THROUGH into L(1) -- the label the γ arm jumps to -- so a failing deferred target matched exactly like a succeeding one.  anchor_leave FIRST because x86_anchor_enter above is an alignment pair and the concede path must balance it exactly once; then CONCEDE on the box's own ω wire, which is where x86_beta_trampoline already routes β, so no new edge is invented.  Disarmed emits the empty string = the fall-through VERBATIM. */
           + x86("def",  L(1))
           + x86_anchor_leave()
           + x86_gamma()
           + x86_beta_trampoline()
           + (cap_fail_retreat() ? (x86("def", L(4)) + x86_anchor_leave() + x86_omega()) : std::string()) )   /* ⭐⭐⭐ s193 the -1 landing: leave the alignment anchor EXACTLY ONCE (x86_anchor_enter above is a pair and this path never reached L(1)'s leave) and CONCEDE on the box's own ω wire -- the same wire x86_beta_trampoline routes β through, so no new edge is invented.  ⛔ CONCEDE, NOT ABORT, ORACLE-RULED: `(SPAN('ABC') $ *chk('NO') | 'ABC')` prints one CHK and then MATCHES on live sbl -bf, so a FAILING target retreats and the next alternative runs -- unlike a NOT-A-NAME target, which aborts the statement through IR_MATCH_END's ω (s170 SN4-CAP-NAME-STRICT).  Two different verdicts on one road; do not merge them.  Disarmed emits the empty string. */
         : ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_IMM: no home -- neither a ζ-SPINE cell (op_zres) nor a ζ-STANDING slot (frame_need_of: DEFER-hazard / ALT-arm classes); classifier and ZD plan disagree on this node -- the legacy C rt_cap_top fallback is deliberately not rebuilt (s83)")
           + x86_beta_trampoline() );   /* R-0 (s93): dead code after the bomb, but β is DEFINED, same discipline as the SAVE/COND fallbacks */
}
