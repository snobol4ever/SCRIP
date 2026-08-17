#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" long  rt_defer_open     (const char *varname, int ival_flag);
extern "C" long  rt_defer_step     (DESCR_t fret);
extern "C" int   rt_defer_close    (int cur_delta);
extern "C" void *rt_proc_open_fn   (void);
extern "C" DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
extern "C" DESCR_t rt_proc_call_epilogue_ω(void);
extern "C" void *rt_defer_get_pat_fn(const char *varname, int ival_flag);
extern "C" void *rt_defer_get_pat_dtp(const char *varname, int ival_flag);
extern "C" void *rt_patv_defer_get_pat_dtp(void *hv, long i, const char *fb);
extern "C" long  rt_patv_defer_open(void *hv, long i, const char *fb, int ival_flag);
extern "C" int rt_defer_run_all(const char *varname, int cur_delta);
extern "C" int rt_patv_defer_run_all(void *hv, long i, const char *fb, int cur_delta);
extern "C" void *dtp_fn_of(void *headv);
extern "C" uint64_t g_sno_defer_cells[4096];
extern uint64_t g_scan_hit_start;
extern int g_gva_active;
extern "C" uint64_t g_rspd_save, g_rspd_g4, g_rspd_g5, g_rspd_s2, g_rspd_g6, g_rspd_beta;
#include "x86_asm.h"
extern "C" int sn4_alt_carrier(void);   /* s127: ONE AUTHORITY in emit.cpp -- reader (3) of the SN4-ALT-CARRIER switch: the C-path pseudo-record grows its cursor pad in lockstep with the af edge the emit-side readers admit (one switch, all readers flip together, the s124 law) */
#define dswap() (x86_zc_frame() == ZC_FRAME_RSP)
static int dw_cell(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DEFER_CELL"); v = e ? (atoi(e) != 0) : 1; } return v; }   /* s142 DEFER-SITE DIET kill-switch (NOFILL precedent): =0 restores the uncached GVA dance for A/B and emergencies */
static int one_defer(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ONE_DEFER"); v = (e && *e == '0') ? 0 : 1; } return v; }   /* ONE-DEFER (Lon s119): =0 restores the pre-s119 open/open_fn/epilogue-loop/step/close arm byte-for-byte */
extern "C" int emit_defer_carve_rbp(void);
static int dfrm(void) { return (_.op_seal == 1) || emit_defer_carve_rbp(); }   /* ⭐⭐⭐⭐⭐ s137 — "DOES THIS DEFER MANAGE ITS OWN FRAME?", asked ONCE for all seven paired sites (α push · α watermark · γ · ω · β), so the exits can never disagree with the entry about whether a frame exists.  THE WHOLE POINT OF ONE HELPER: the previous spelling repeated `_.op_seal == 1` seven times, which is the s68/s70 spelled-twice disease with six chances to drift -- and an α that pushes with a β that does not pop is an unbalanced stack, i.e. the failure mode is silent corruption rather than a compile error.  OFF (default) this is EXACTLY `_.op_seal == 1` and the emission is byte-identical by construction; ON it admits the UNSEALED defer, which is what an ARBNO body defer always is.  Rationale, measured chain and the flip protocol: emit_defer_carve_rbp() in emit.cpp. */
#define rspd()  (getenv("SCRIP_RSPDIFF") ? 1 : 0)
#define rspd_snap(cell, nm) IF(rspd(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)(cell),nm) \
                                     + x86("mov",RDQ("rcx",0),"rsp"))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_defer() {
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    /* s142 DEFER-SITE DIET: per-site WRITE-ONCE entry cell (seal==2, GVA arm only this slice).  Steady state replaces the 8-Ir GVA/DT_P/memo dance with lea+load+test+jne; the cold path is the UNCHANGED
     * dance plus one store into the cell (a 0 store on the not-yet-DT_P arm is a no-op — the cell arms itself only when the fn first resolves, and write-once makes it permanently valid).  rsi is the
     * scratch: dead at α on this path (the non-GVA arm's xor esi,esi is the only prior user), clobbered by the dtp_fn_of C call, hence the re-lea before the store.  Index claim = drive_value_slot precedent (fact fix 2026-08-12: bb_slot_claim was deleted 2026-07-02)
     * (emit-time staging at template top); ≥4096 falls back to the uncached path; counter monotonic per process (uniqueness is the only requirement). */
    int vslot = -1; { const char *sv = _.op_sval; if (sv) { const char *d = strstr(sv, "$V"); if (d && d[2] >= '0' && d[2] <= '9') { char *e = 0; long k = strtol(d + 2, &e, 10); if (e && !*e) vslot = (int)k; } } }   /* PB-1s (s108): a compiler-minted PAT$n$V<i> value-leaf ('$' cannot occur in user names) reads slot i of the DTP this activation runs under, NOT the per-site global -- manual p.85-86 per-construction freeze; the case_driver cycle/stale class */
    int ci = (vslot < 0 && dw_cell() && g_gva_active && _.op_gva_k >= 0 && _.op_seal == 2 && g_emit.sn4_defer_cell_n < 4096) ? g_emit.sn4_defer_cell_n++ : -1;   /* s108: $V leaves are per-construction by definition -- no per-site cell may cache them */
    static char cl[8][48]; static int cln; if (ci >= 0) { cln = (cln + 1) & 7; snprintf(cl[cln], sizeof cl[cln], "g_sno_defer_cells+%d", ci * 8); }
    const char * clbl = ci >= 0 ? cl[cln] : "";
    uint64_t cadr = ci >= 0 ? (uint64_t)(uintptr_t)(const void *)&g_sno_defer_cells[ci] : 0;
    /* s137 OVER-SEAL (Lon ruling: a fence clearly demarks a point OUTSIDE a γ where entire chunks of ζ can be whacked, since no backtracking is guaranteed): when the defer's target is a STATICALLY
     * right-sealed stored pattern (IR_t.seal → op_seal), this element is that demarked sync point in ITS OWN activation — α stamps rsp into the defer.pad quad (FRQ(op_off), ___-relative → recursion-
     * safe), the L(4)/L(5) glues restore it (bulk-freeing the callee's ENTIRE retained subtree: frame, suspend record, every transitive carve — the resume surface is already dead by NCB-2/SZ-1
     * body_root=NULL, so nothing the whack destroys is ever read), and β restores-then-ωs instead of `jmp [rsp+0]` (the fast-path record is whacked; the SLOW-path L(6) exhaust record is discarded by
     * the same restore, keeping the frontier LIFO exact for the left neighbour).  CSTACK/FORTH only — other ports keep the untouched original body. */
    return x86("comment", "IR_MATCH_DEFER (ZS-2 jmp-entry)")
         + x86_alpha()
         + IF(dfrm() && x86_port_cstack() && emit_defer_rbp(),
               x86("comment", "THREE ZETAS ζ-FRAME (s85): *P DEFER establishes its own RBP activation frame at alpha -- the SECOND and LAST operator-BB (with MATCH_BEGIN/ζ-STANDING) permitted to push rbp.  Replaces the s137 rsp-watermark save (FRQ(op_off)=rsp, restored rsp-relatively at every exit): that save/restore pair is Defect C -- both ends compute [rsp#+op_off] against WHATEVER rsp happens to be AT THAT POINT, sound only if the deferred target's own body never carves stack without self-releasing before jumping back through the wire, which the non-popping ζ-SPINE law (committed growth released only by bracket whacks) guarantees it does NOT.  rbp does not move across the jmp-entry wire transfer (the callee's own carves are rsp-relative, never touch our rbp), so a push here is immune by construction -- the exact argument bb_match_capture.cpp's s81/s83 activation-frame arm already uses for the SAVE/IMM-or-COND capture-family crossing.  No slot registration for spine-only BBs is added here: the frame exists so THEY can register into it (ARBNO's chained-K0/K16-defer bodies, per bb_match_arbno.cpp's own op_frame_need consultation), not so this box owns extra state of its own -- op_off is unused on this arm; the WHOLE FRAME is the watermark.")
             + x86("push", "rbp")
             + x86("mov",  "rbp", "rsp"))
         + IF(dfrm() && x86_port_cstack() && !emit_defer_rbp(),
               x86("comment", "s137 SEALED defer: fence-demarked sync point (watermark in defer.pad)")
             + x86("mov",  FRQ(_.op_off), "rsp"))
         + IF(ci >= 0,
               x86("comment", "s142 cell reworked s108: the cell holds the DTP (write-once => the DTP is as fixed as the fn once armed), fn loaded at [dtp+0] (the pinned offset-0 assert), so the cached fast path still carries the DTP into the blob in rdx")
             + x86("lea",  "rsi", "[rip + __]", cadr, clbl)
             + x86("mov",  "rdx", RDQ("rsi", 0))
             + x86("test", "rdx", "rdx")
             + x86("je",   L(13))
             + x86("mov",  "rax", RDQ("rdx", 0))
             + x86("jmp",  L(11))
             + x86("def",  L(13)))
         + IF(vslot >= 0,
               x86("comment", "PB-1s (s108) $V PER-CONSTRUCTION SLOT: value = snap[i] of the DTP this activation runs under ([rbp-24], preamble-stored from entry rdx); the PAT$n$V global is stage-2 marshalling whose lifetime ends at MKPAT -- per-site cell reads were the case_driver cycle/stale class")
             + x86("mov",  "rdi", RDQ("rbp", -24))
             + x86("mov",  "esi", (long)vslot)
             + x86("lea",  "rdx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86_align_enter()
             + x86("call", "rt_patv_defer_get_pat_dtp", (uint64_t)(uintptr_t)(void *)(void *(*)(void *, long, const char *))rt_patv_defer_get_pat_dtp)
             + x86_align_leave()
             + x86("mov",  "rdx", "rax")
             + x86("test", "rax", "rax")
             + x86("je",   L(16))
             + x86("mov",  "rax", RDQ("rdx", 0))
             + x86("def",  L(16)))
         + IF(vslot < 0 && g_gva_active && _.op_gva_k >= 0,
               x86("note", gva_name(_.op_gva_k)) + x86("mov",  "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16))
             + x86("note", gva_name(_.op_gva_k)) + x86("mov",  "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))
             + x86("cmp",  "eax", (long)DT_P)
             + x86("jne",  L(9))
             + x86("mov",  "rax", RDQ("rdx", 0))
             + x86("test", "rax", "rax")
             + x86("jne",  L(10))
             + x86("mov",  "rdi", "rdx")
             + x86_align_enter()
             + x86("call", "dtp_fn_of", (uint64_t)(uintptr_t)(void *)(void *(*)(void *))dtp_fn_of)
             + x86_align_leave()
             + x86("note", gva_name(_.op_gva_k)) + x86("mov",  "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))   /* s108: the C call clobbered rdx -- re-derive the DTP from the same GVA payload spelling so blob entry carries it */
             + x86("jmp",  L(10))
             + x86("def",  L(9))
             + x86("xor",  "eax", "eax")
             + x86("def",  L(10)))
         + IF(ci >= 0,
               x86("test", "rax", "rax")
             + x86("je",   L(15))
             + x86("lea",  "rsi", "[rip + __]", cadr, clbl)
             + x86("mov",  RDQ("rsi", 0), "rdx")
             + x86("def",  L(15))
             + x86("def",  L(11)))
         + IF(vslot < 0 && !(g_gva_active && _.op_gva_k >= 0),
               x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("xor",  "esi", "esi")
             + x86_align_enter()
             + x86("call", "rt_defer_get_pat_dtp", (uint64_t)(uintptr_t)(void *)(void *(*)(const char *, int))rt_defer_get_pat_dtp)   /* s108: DTP twin of rt_defer_get_pat_fn -- fn at [dtp+0], rdx carries the DTP into the blob */
             + x86_align_leave()
             + x86("mov",  "rdx", "rax")
             + x86("test", "rax", "rax")
             + x86("je",   L(14))
             + x86("mov",  "rax", RDQ("rdx", 0))
             + x86("def",  L(14)))
         + x86("test", "rax", "rax")
         + x86("jz",   "L0")
         + rspd_snap(&g_rspd_save, "g_rspd_save")
         + x86("mov",  "r8d", (long)(_.op_scan ? 1 : 0))
         + IF(_.op_seal != 1 && x86_fb_pinned(),
               x86("comment", "s44 WIRE-SAVE: stash caller's true r10/r11 before the blob-entry glue overwrites them with this node's private L(4)/L(5)")
             + x86("mov",  FRQ(_.op_off),     "r10")
             + x86("mov",  FRQ(_.op_off + 8), "r11"))
         + bb_glue_pass_wires_blob(4, 5)   /* PASS-THROUGH GLUE (s22v): the canonical consumer -- blob entry with this box's L(4)/L(5) as the ride-through γ/ω wires; byte-identical to the hand-rolled trio it replaces */   /* ⭐ LADDER WREG (s15): THE blob-entry site.  rax here is rt_defer_get_pat_fn's PAT$ blob pointer, so this call site is blob-only BY CONSTRUCTION -- which is why the wire spelling can convert here without touching the DEFINE'd-proc/one-shot kinds that share bb_glue_pass_wires.  Under WREG the wires ride r10/r11 and the blob needs ZERO receiving code -- UNCONDITIONALLY: the SCRIP_WREG killswitch was DELETED with the PAT$ frame arm (DEL-T1 D-1, 855a12a5); revert = git revert, never an env flag (fact fix 2026-08-12). */
         + x86("def",  L(4))
         + IF(_.op_seal != 1 && x86_fb_pinned(),
               x86("comment", "s44 WIRE-RESTORE (success fallthrough): the rest of THIS box's own enclosing pattern reads r10/r11 as its live γ/ω under WREG -- restore before falling into it")
             + x86("mov",  "r10", FRQ(_.op_off))
             + x86("mov",  "r11", FRQ(_.op_off + 8)))
         + IF(dfrm() && x86_port_cstack() && emit_defer_rbp(),
               x86("mov", "rsp", "rbp") + x86("pop", "rbp"))
         + IF(dfrm() && x86_port_cstack() && !emit_defer_rbp(),
               x86("mov",  "rsp", FRQ(_.op_off)))
         + IF(_.op_scan && _.op_scan_head_off >= 0 && !emit_match_owns_startd(),
               x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_scan_hit_start, "g_scan_hit_start")
             + x86("mov",  "rax", "[rcx]")
             + x86("mov",  emit_match_begin_stfh_k() > 0 ? "dword ptr [rsp# + 0]" : FR(_.op_scan_head_off), "eax"))   /* ⭐ ZD-8·STFH-DEFER-FIX (FINDING-2026-08-09): start_δ home matches MATCH_BEGIN.  When stfh fires (emit_match_begin_stfh_k()>0), MATCH_BEGIN writes start_δ to HKD=[___-64]; the γ-continuation here must write the scan hit position to the SAME slot.  Old FR(_.op_scan_head_off)=[___+416] was the pre-stfh positive layout — landing in CRT territory above the outer frame's RSP, corrupting environ, SEGV in getenv() on first iteration.  emit_match_begin_stfh_k() is the ONE AUTHORITY (same call MATCH_BEGIN's template uses at line 48); when it returns 0, FR(_.op_scan_head_off) is byte-identical to the historic path.  Killswitch: SCRIP_OS_CAP=0 forces emit_match_begin_stfh_k()==0 → legacy path byte-identical. */   /* ⭐ START-δ HOME WIRED (s69, closing s68's half-landed rung): the GUARD now reads emit_match_owns_startd(), not emit_match_begin_stfh_k().  THE TWO PREDICATES ANSWER DIFFERENT QUESTIONS and MATCH-RBP is where they diverge: stfh_k() is the PLANNER's byte-accounting question and mrbp deliberately zeroes it (the rbp frame releases itself), while THIS site asks the HOME question — "does MATCH_BEGIN already own start_δ?" — which is TRUE for both head-frame flavors (legacy carve → HKD, mrbp frame → [rbp-40]).  Reading the planner's answer here let the suppression lift under mrbp and emitted a stray 4-byte write to FR(op_scan_head_off) that NOTHING reads.  MEASURED s70 over 302 programs (corpus/probe + programs/snobol4/feat), re-derived rather than inherited: PRE-FIX, spot-checked mrbp customers each emitted exactly ONE stray site under SCRIP_MATCH_RBP=1 and ZERO under =0 (ab_defer_call, dc_recur, pb_stitch_defer, mv_valheld_cap; on ab_defer_call MATCH_BEGIN writes start_δ to [rbp-40] while this γ wrote `mov dword ptr [rsp+208],eax`).  POST-FIX the full 302-program census reads ZERO writeback sites under BOTH arms with ON==OFF on every row, and legacy is byte-identical 181/181 against the pre-fix artifacts — as it must be BY CONSTRUCTION, since with mrbp off emit_match_owns_startd() ≡ (emit_match_begin_stfh_k()>0) for every graph.  ⛔ AN EARLIER DRAFT OF THIS COMMENT CLAIMED "28 of 312": that number did not reproduce and is STRUCK — the codebase's own law (s50 FACT RULE: a claim about a code path is admissible only with a printed value beside it) applies to a comment as much as to a cursor.  ⛔ CONSEQUENCE WORTH KNOWING: the writeback now fires for NO program in this corpus, because every graph reaching it owns a head frame — so the carve-less class this arm exists to serve is currently WITNESS-FREE, and the start_δ divergence named below is open but unwitnessed.  ⛔ The ternary below is PROVABLY DEAD and always has been — the guard admits only !owns_startd, which implies raw stfh==0, which implies emit_match_begin_stfh_k()==0, so the "[rsp# + 0]" arm cannot be selected from here; it is left in place rather than deleted because its removal is a byte-inert cosmetic edit that would obscure this rung's one-line diff, and it is named HERE so the next reader does not trust it. */

         + rspd_snap(&g_rspd_g4, "g_rspd_g4")
         + x86_gamma()
         + x86("def",  L(5))
         + IF(_.op_seal != 1 && x86_fb_pinned(),
               x86("comment", "s44 WIRE-RESTORE (exhaust): without this, x86_omega() below reads r11 == this node's own dead L(5) -- the s43a closed loop")
             + x86("mov",  "r10", FRQ(_.op_off))
             + x86("mov",  "r11", FRQ(_.op_off + 8)))
         + IF(dfrm() && x86_port_cstack() && emit_defer_rbp(),
               x86("mov", "rsp", "rbp") + x86("pop", "rbp"))
         + IF(dfrm() && x86_port_cstack() && !emit_defer_rbp(),
               x86("mov",  "rsp", FRQ(_.op_off)))
         + rspd_snap(&g_rspd_g5, "g_rspd_g5")
         + x86_omega()
         + (one_defer()
             ? x86("def",  "L0")   /* ONE-DEFER arm: resolution + thunk-eval + literal-match close in ONE C call; the emitted open/open_fn/epilogue/step round-trips fold into rt_defer_run_all\047s rt_call_proc_descr (s117 by-name entry).  δ rides an ARG (r14d is untouched between α and here on this path), result comes back in eax with close\047s exact contract; the success tail below is shared with the legacy arm verbatim. */
             + x86_xfer_enter()
             + IF(vslot < 0,
                   x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
                 + x86("mov",  "esi", "r14d"))
             + IF(vslot >= 0,
                   x86("mov",  "rdi", RDQ("rbp", -24))
                 + x86("mov",  "esi", (long)vslot)
                 + x86("lea",  "rdx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
                 + x86("mov",  "ecx", "r14d"))
             + x86_anchor_enter()
             + IF(vslot < 0,  x86("call", "rt_defer_run_all", (uint64_t)(uintptr_t)(void *)(int (*)(const char *, int))rt_defer_run_all))
             + IF(vslot >= 0, x86("call", "rt_patv_defer_run_all", (uint64_t)(uintptr_t)(void *)(int (*)(void *, long, const char *, int))rt_patv_defer_run_all))
             + x86_anchor_leave()
             + x86_xfer_leave()
             : x86("def",  "L0")
         + x86_xfer_enter()
         + IF(vslot < 0,
               x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("xor",  "esi", "esi"))
         + IF(vslot >= 0,
               x86("comment", "PB-1s (s108): scalar half of the $V slot read -- open with the FROZEN value (dtp->snap[i]), not the per-site global; this is the pb_stale_snapshot_value silent-wrong-answer half")
             + x86("mov",  "rdi", RDQ("rbp", -24))
             + x86("mov",  "esi", (long)vslot)
             + x86("lea",  "rdx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("xor",  "ecx", "ecx"))
         + x86_anchor_enter()
         + IF(vslot < 0,
               x86("call", "rt_defer_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int))rt_defer_open))
         + IF(vslot >= 0,
               x86("call", "rt_patv_defer_open", (uint64_t)(uintptr_t)(void *)(long (*)(void *, long, const char *, int))rt_patv_defer_open))
         + x86("def",  "L2")
         + x86("test", "rax", "rax")
         + x86("je",   "L3")
         + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
         + IF(!dswap(), x86("push",x86_zr())
                      + x86("sub","rsp",8L)
                      + x86("mov",x86_zr(),"rsp"))
         + bb_glue_pass_wires(7, 8)   /* GLUE-SYM (s22x): dormant legacy anchor hoisted above the glue; byte-identical at the dswap() default */
         + x86("def",  L(7))
         + IF(!dswap(), x86("mov","rax","rsp")
                      + x86("mov","rax",RDQ("rax",8))
                      + x86("mov","rdi",RDQ("rax",0))
                      + x86("mov","rsi",RDQ("rax",8))
                      + x86("mov","rsp",x86_zr())
                      + x86("add","rsp",8L)
                      + x86("pop",x86_zr()))
         + x86("call","rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_defer_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_defer_step)
         + x86("jmp",  "L2")
         + x86("def",  L(8))
         + IF(!dswap(), x86("mov","rsp",x86_zr())
                      + x86("add","rsp",8L)
                      + x86("pop",x86_zr()))
         + x86("call","rt_proc_call_epilogue_ω", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(void))rt_proc_call_epilogue_ω)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_defer_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_defer_step)
         + x86("jmp",  "L2")
         + x86("def",  "L3")
         + x86_anchor_leave()
         + x86_xfer_leave()
         + x86("mov",  "edi", "r14d")
         + x86_align_enter()
         + x86("call", "rt_defer_close", (uint64_t)(uintptr_t)(void *)(int (*)(int))rt_defer_close)
         + x86_align_leave()
         )
         + x86("test", "eax", "eax")
         + x86_omega("js")
         + IF(sn4_alt_carrier(),
               x86("comment", "s127 R-4(a) CURSOR-BEARING PSEUDO-RECORD: the C-path 16B record's pad now carries the instance's ENTRY cursor and the exhaust stub RESTORES it before af.  The old stub (add rsp,16 only) was built for a world where the af retry edge was suppressed for defer bodies -- it was only ever consumed by the as null-progress je, where r14d's value no longer mattered at the pop.  With the sn4_alt_carrier af->PAIR(1) edge admitted, af cascades `jne -> jmp [rsp]` per placed instance and each pop MUST rewind r14d to that instance's entry, or the Δ0 compare never terminates and the walk descends past every record into the statement spine -- measured 165_pat_arbno_defer_var_body stmt 2 (Q='aa' on 'aaa'): rip=0 on a zeroed pad with r14d stuck at 2.  Spelled push/pop-only (mov ecx,r14d zero-extends; rcx is dead post-call) so no [rsp+N] text spelling can pick up FR64 depth compensation (the s97 x86_rsp_load64 lesson).  The blob path needs nothing: a target blob's interior (ALT af) restores r14d from ITS OWN record by construction, and the record layout/size here is unchanged -- {stub@0, cursor-pad@8}, same 16B, same parity.")
             + rspd_snap(&g_rspd_s2, "g_rspd_s2")
             + x86("mov",  "ecx", "r14d")
             + x86("mov",  "r14d", "eax")
             + x86_lea_id("rax", 6)
             + x86("push", "rcx")
             + x86("push", "rax")
             + x86_gamma()
             + x86("def",  L(6))
             + x86_add("rsp", 8)
             + x86("pop",  "rax")
             + x86("mov",  "r14d", "eax")
             + rspd_snap(&g_rspd_g6, "g_rspd_g6")
             + x86_omega())
         + IF(!sn4_alt_carrier(),
               x86("mov",  "r14d", "eax")
             + x86_lea_id("rax", 6)
             + rspd_snap(&g_rspd_s2, "g_rspd_s2")
             + x86_sub("rsp", 8)
             + x86("push", "rax")
             + x86_gamma()
             + x86("def",  L(6))
             + x86_add("rsp", 16)
             + rspd_snap(&g_rspd_g6, "g_rspd_g6")
             + x86_omega())
         + x86_beta()
         + ((_.op_seal == 1 && x86_port_cstack())
              ? ((emit_defer_rbp() ? (x86("mov", "rsp", "rbp") + x86("pop", "rbp")) : x86("mov", "rsp", FRQ(_.op_off))) + x86_omega())
              : (_.op_defer_leaf_susp > 0
                   ? (rspd_snap(&g_rspd_beta, "g_rspd_beta")   /* PS-3 s153 ZERO-GUARDED β (priced tail-candidate leaf only): the ε-resume cascade re-enters every body box's β on the PHANTOM FPB pad,
                                                                * which is zeros -- granted leaves read a zero cell and fail benignly, but the raw `jmp [rsp+0]` is a jump through NULL (t3 rip=0).
                                                                * Guarded: a real γ-record resumes the blob as ever; zero = the phantom share -> pop this leaf's SUSP and ω-transit (exhausted-leaf
                                                                * behavior), consuming the pad exactly as granted leaves consume theirs -- the fail glue then reads the ε header at the exact depth. */
                      + x86("mov",  "rax", RDQ("rsp", 0))
                      + x86("test", "rax", "rax")
                      + x86("jne",  L(12))
                      + x86_zrelease((long)_.op_defer_leaf_susp)
                      + x86_omega()
                      + x86("def",  L(12))
                      + x86_jmp_reg("rax"))
                   : (rspd_snap(&g_rspd_beta, "g_rspd_beta") + x86_jmp_mem("rsp", 0))));
}
