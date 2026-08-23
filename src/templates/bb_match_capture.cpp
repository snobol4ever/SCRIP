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
static inline int havehome(void) { return _.op_zres || _.op_cap_anchor || _.op_off >= 0; }
static inline int nret_cap_live(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_NRET_CAP"); v = e ? (e[0] != '0') : 1; } return v; }
static inline const char * writehome(void) { return _.op_zres ? ZRESD(0) : FR(_.op_off); }
static inline const char * readhome(void)  { return _.op_zres ? ZOPD(1, 0) : FR(_.op_off); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
           + x86_beta_trampoline() )
         : (int)_.op_phase == 0 && havehome()
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE spine (ordinary, unchanged mechanism)")
           + x86_alpha()
           + x86("mov",  writehome(), "r14d")
           + x86_gamma()
           + x86_beta_trampoline() )
         : (int)_.op_phase == 0
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_SAVE: no home -- neither a ζ-SPINE cell (op_zres) nor a ζ-STANDING slot (frame_need_of: DEFER-hazard / ALT-arm classes); classifier and ZD plan disagree on this node -- the legacy C rt_cap_push fallback is deliberately not rebuilt (s83)")
           + x86_beta_trampoline() )
         : (int)_.op_phase == 1 && _.op_sval && _.op_sval[0] == '*' && !nret_cap_live()
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_COND: computed-name (*VAR/NRETURN) target not yet rebuilt -- blocked on the :(NRETURN) lowering bug (s82), see this file's header comment")
           + x86_beta() )
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
           + x86_beta() )
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
           + x86_beta() )
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
           + (cap_fail_retreat() ? (x86("cmp", "rax", (long)-1) + x86("je", L(4))) : std::string())
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
           + (cap_fail_retreat() ? (x86_anchor_leave() + x86_jmp(X86P_OMEGA)) : std::string())
           + x86("def",  L(1))
           + x86_anchor_leave()
           + x86_gamma()
           + x86_beta_trampoline()
           + (cap_fail_retreat() ? (x86("def", L(4)) + x86_anchor_leave() + x86_omega()) : std::string()) )
         : (int)_.op_phase == 2 && _.op_frame_need
         ? ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_IMM: hazard crosses a DEFER-unsafe boundary but op_cap_frame_off is unavailable -- CAPTURE never pushes its own activation frame (s88 revert), see IR_MATCH_CAPTURE_SAVE's bomb for the full rationale.")
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
           + (cap_fail_retreat() ? (x86("cmp", "rax", (long)-1) + x86("je", L(4))) : std::string())
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
           + (cap_fail_retreat() ? (x86_anchor_leave() + x86_jmp(X86P_OMEGA)) : std::string())
           + x86("def",  L(1))
           + x86_anchor_leave()
           + x86_gamma()
           + x86_beta_trampoline()
           + (cap_fail_retreat() ? (x86("def", L(4)) + x86_anchor_leave() + x86_omega()) : std::string()) )
         : ( x86_alpha()
           + x86_bomb("IR_MATCH_CAPTURE_IMM: no home -- neither a ζ-SPINE cell (op_zres) nor a ζ-STANDING slot (frame_need_of: DEFER-hazard / ALT-arm classes); classifier and ZD plan disagree on this node -- the legacy C rt_cap_top fallback is deliberately not rebuilt (s83)")
           + x86_beta_trampoline() );
}
