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
extern "C" void rt_cap_push(void *slot, int delta);
extern "C" void rt_cap_pop(void *slot);
extern "C" int rt_cap_top(void *slot);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ZB-FC-3c (ARCH-ZETA S13 Tier C, CAPTURES; plan of record = the s47 COND-CROSS-BOX-READ finding): when
 * LOWER registered the capture (linear inner, no ARBNO either direction), SAVE owns a 16-byte rsp cell --
 * alpha stores delta at cell+0 (the hook's sub rsp,16 has already run at the alpha define), beta emits
 * NOTHING (omega's hook pop is the release, S10b FORTH law), the rt_cap software array never called -- and
 * COND/IMM read that delta CROSS-BOX at [rsp + fp(inner)]: by S10c (cells pop at omega not gamma) the whole
 * inner subtree is still suspended at the yield, so the displacement is a compile-time constant, exactly
 * ALTERNATE's rspd(op_fc_fpmax+4) mechanism.  COND/IMM are ZERO-CELL (no hook motion); their read sits
 * OUTSIDE any align/anchor window (align_enter pushes 16 and masks rsp -- an rspd read inside it would be
 * wrong; eax survives the window's pushes into the later mov esi,eax).  sfc/cfc off = the flat rt_cap array
 * path VERBATIM (degrade never die); the pend/rbp-dcap machinery is identical on both paths (F2: by-value). */
static inline int  cap_sym()  { static int on = -1; if (on < 0) { const char * e = getenv("SCRIP_CAP_SYM"); on = (e && *e == '0') ? 0 : 1; } return on; }   /* CAP-SYM killswitch (kill-switch law): =0 restores the pre-fix bare x86_fc_on() producer predicate byte-for-byte, so the A/B is one binary two envs.  PROCESS-SCOPE IS SOUND HERE and is NOT the s21x-r defect: this latch gates a per-BOX template predicate that is re-evaluated per node against per-node state (op_fc_base), never a graph-scope regime verdict that must agree with a planner decision -- the banned shape is `static getenv` inside a function taking IR_graph_t * g. */
static inline int  sfc()      { return x86_fc_on() && (!cap_sym() || _.op_fc_base >= 0); }   /* CAP-SYM (s22m): op_fc_bytes has TWO writers meaning DIFFERENT things -- emit.cpp's ZB-FC-3c capture grant (which also sets op_fc_base = op_off >= 0) and the ZD zeta-spine planner (emit.cpp:832, op_fc_bytes = g_zd_k with op_fc_base = -1).  Bare x86_fc_on() cannot tell them apart, so a SAVE armed by the ZD spine took the 3c CELL arm and stored its delta into the zeta RESULT cell, while its COND partner -- which correctly consults its OWN per-node grant (fc_cond_fp -> op_fc_disp) -- declined to the rt_cap software array that nobody had pushed.  Producer wrote one place, consumer read another: capture start came back 0, so every capture preceded by a matched element swallowed the preceding characters (measured: 'abcd' 'a' (w . tag) bound "abc" where SPITBOL binds "bc").  op_fc_base >= 0 is ALREADY the established discriminator for exactly this split -- x86_asm.h:282 uses `x86_fc_on() && _.op_fc_base < 0` to name the carve-only ZD class -- so this reuses the existing authority rather than minting a second one.  The two ends are now keyed to the SAME grant: both on the cell, or both on the array. */
static inline int  cfc()      { return x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0; }
static inline const char * rspd(int off) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "dword ptr [rsp# + %d]", off); return b[i]; }   /* Z4 s8 (capture-start fix): RAW rsp marker -- the plain spelling collides with the unpinned fr32 prefix in x86_parse and gains op_flat_disp, which displaced SAVE's delta store AND lifted COND's cross-depth read 16 past it (double-counted depth difference).  The '#' routes XK_RSP32 raw, restoring this family's designed cell addressing on both the writer (alpha delta store) and the readers (COND/IMM op_fc_disp) in one move -- the pair cannot desync. */
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
         : (int)_.op_phase == 0 && _.op_zres
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE ZD cell")   /* ZD-5b (s27 cross-front OMEGA): SAVE under ZD -- K=16 own cell; ZRESD(0) stores r14d (cursor at alpha); beta trampoline retries; omega is caller's wire (cell released by op_zgpop at statement end, not here -- K=16 under law-4). */
           + x86_alpha()
           + x86("mov",  ZRESD(0), "r14d")
           + x86_gamma()
           + x86_beta_trampoline() )
         : (int)_.op_phase == 1 && _.op_zres
         ? ( x86("comment", "IR_MATCH_CAPTURE_COND ZD")   /* ZD-5b (s27 cross-front OMEGA): COND under ZD -- K=0; SAVE cursor delta from ZOPD(1,0) (staging loop stages operands[1]=SAVE at slot 1, skipping slot 0 which is the blob inner-pattern outside the run); r14d-ZOPD(1,0)=match length; pend/CAS push unchanged. */
           + x86_alpha()
           + x86("mov",  "eax", ZOPD(1, 0))
           + x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
           + x86("mov",  RDQ("r12", 0), "rcx")   /* CAS-R12-UNIFY (Lon mandate, this session): r12 IS the live pend top on every arm -- the op_zw fork collapsed; cell [RT_CAS_TOP] is boot-seed only (rt_outer_call thunk / m4 wrapper), rt_chain_enter inherits r12 callee-saved so EVAL/nested graphs read the true top. */
           + x86("mov",  "esi", "eax")
           + x86("mov",  RDQ("r12", 8), "rsi")
           + x86("mov",  "edx", "r14d")
           + x86("sub",  "edx", "eax")
           + x86("mov",  RDQ("r12", 16), "rdx")
           + x86("add",  "r12", (long)24)
           + x86_gamma()
           + x86_beta()
           + x86("sub",  "r12", (long)24)   /* CAS-R12-UNIFY: unguarded per-entry pop, sound by the LIFO cascade (REG-7 note below) */
           + x86_omega() )
         : (int)_.op_phase == 2 && _.op_zres
         ? ( x86("comment", "IR_MATCH_CAPTURE_IMM ZD")   /* ZD-5b (s27 cross-front OMEGA): IMM under ZD -- K=0; same ZOPD(1,0) delta read; immediate capture via rt_cap_open + proc epilogue (identical to legacy IMM arm except the delta source). */
           + x86_alpha()
           + x86("mov",  "eax", ZOPD(1, 0))
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
         : (int)_.op_phase == 0 && sfc()
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE fc cell")
           + x86_alpha()
           + x86("mov",  rspd(0), "r14d")
           + x86_gamma()
           + x86_beta_trampoline() )
         : (int)_.op_phase == 0
         ? ( x86("comment", "IR_MATCH_CAPTURE_SAVE push")
           + x86_alpha()
           + x86("lea",  "rdi", FR(_.op_off))
           + x86("mov",  "esi", "r14d")
           + x86_align_enter()
           + x86("call", "rt_cap_push", (uint64_t)(uintptr_t)(void *)(void (*)(void *, int))rt_cap_push)
           + x86_align_leave()
           + x86_gamma()
           + x86_beta()
           + x86("lea",  "rdi", FR(_.op_off))
           + x86_align_enter()
           + x86("call", "rt_cap_pop", (uint64_t)(uintptr_t)(void *)(void (*)(void *))rt_cap_pop)
           + x86_align_leave()
           + x86_omega() )
         : (int)_.op_phase == 1
         /* REG-7 pend shapes (s80; was REG-6 rdi-staged, was REG-2 cell, was rbp-dcap).  γ: the pend top IS
          * r12 and the 24B entry {varname@0, saved_delta@8, len@16} is written AT it directly — three
          * [r12+0/8/16] stores through the family's new SIB arm (x86_rd32_modrm; the rdi staging mov is
          * deleted, rdi now untouched here), then bump r12 by 24 (add-reg imm8).  β: sub r12,24, UNGUARDED — sound
          * because within-alternative failure cascades transit the boxes (balanced, the Python LIFO theorem)
          * and the alternative-SWITCH bypass is bulk-restored by bb_match_alternate's own mark (SZ-2c gap,
          * ported inline).  r12 is ALWAYS live (outer seed + callee-saved inheritance), so nested heads read
          * the register directly.  R12-FREE-1 (Lon 2026-07-29, GOAL-ZETA-FOUR): the pend top is now CELL-RESIDENT at [RT_CAS_TOP] -- the same cell the C side already aliases as g_dcap_top (pattern_match.c:644) -- so C-side and emitted-side pushes share ONE authority and r12 is vacated for config 1 (FRAME_R12); r10 stages the entry stores (no C call intervenes), rax the beta pop.  Post-RTX the top returns to a register (r12) per the same directive.  The residual rt_cap_top call is the SAVE-stack array read — ZB-FC-3c's
          * named kill, NOT this rung's. */
         ? ( x86("comment", "IR_MATCH_CAPTURE_COND (pend-park inline pend)")
           + x86_alpha()
           + IF(cfc(),  x86("mov", "eax", rspd((int)_.op_fc_disp)))
           + IF(!cfc(), x86_align_enter()
                      + x86("lea",  "rdi", FR(_.op_off))
                      + x86("call", "rt_cap_top", (uint64_t)(uintptr_t)(void *)(int (*)(void *))rt_cap_top)
                      + x86_align_leave())
           + x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
           + x86("mov",  RDQ("r12", 0), "rcx")   /* CAS-R12-UNIFY (Lon mandate, this session): the op_zw fork collapsed -- r12 IS the live CAS top on every arm (the s23l ruling made universal); the REG-7 design below returns as shipped, cell demoted to boot-seed. */
           + x86("mov",  "esi", "eax")
           + x86("mov",  RDQ("r12", 8), "rsi")
           + x86("mov",  "edx", "r14d")
           + x86("sub",  "edx", "eax")
           + x86("mov",  RDQ("r12", 16), "rdx")
           + x86("add",  "r12", (long)24)
           + x86_gamma()
           + x86_beta()
           + x86("sub",  "r12", (long)24)   /* unguarded per-entry pop: within-alternative failure cascades transit the boxes (balanced LIFO); terminal exits scan-to-sentinel wholesale */
           + x86_omega() )
         : ( x86("comment", "IR_MATCH_CAPTURE_IMM")
           + x86_alpha()
           + IF(cfc(),  x86("mov", "eax", rspd((int)_.op_fc_disp)))
           + x86_anchor_enter()
           + IF(!cfc(), x86("lea",  "rdi", FR(_.op_off))
                      + x86("call", "rt_cap_top", (uint64_t)(uintptr_t)(void *)(int (*)(void *))rt_cap_top))
           + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), (strtab_label(b, sizeof b, (_.op_sval ? _.op_sval : "")), b))
           + x86("mov",  "esi", "eax")
           + x86("mov",  "edx", "r14d")
           + x86("mov",  "ecx", (long)1)
           + x86("call", "rt_cap_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int, int, int))rt_cap_open)
           + x86("test", "rax", "rax")
           + x86("je",   L(1))
           /* REG-6 (was REG-2): no mirror-out — r12 IS the live top, inherited callee-saved by any nested
            * match a *VAR body runs, so its head reads the true top directly. */
           + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
           /* ZW-0 stage 2: island push arm deleted -- unreachable under ZC_FRAME_RSP default */
           + bb_glue_pass_wires(2, 3)   /* GLUE-SYM (s22x): dormant legacy anchor hoisted above the glue (lea rcx/rdx touch neither rsp nor zr); byte-identical at the ZC_FRAME_RSP default */
           + x86("def",  L(2))
           /* ZW-0 stage 2: island rsp/zr dance deleted */
           + x86("call", "rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
           + x86("mov",  "rdi", "rax")
           + x86("mov",  "rsi", "rdx")
           + x86("call", "rt_cap_finish", (uint64_t)(uintptr_t)(void *)(void (*)(DESCR_t))rt_cap_finish)
           + x86("jmp",  L(1))
           + x86("def",  L(3))
           /* ZW-0 stage 2: island rsp/zr dance deleted */
           + x86("call", "rt_proc_call_epilogue_ω", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(void))rt_proc_call_epilogue_ω)
           + x86("mov",  "rdi", "rax")
           + x86("mov",  "rsi", "rdx")
           + x86("call", "rt_cap_finish", (uint64_t)(uintptr_t)(void *)(void (*)(DESCR_t))rt_cap_finish)
           + x86("def",  L(1))
           + x86_anchor_leave()
           + x86_gamma()
           + x86_beta_trampoline() );
}
