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
typedef struct { void *fn; long aux; } rt_defer_pr_t;
extern "C" rt_defer_pr_t rt_defer_probe_run(const char *varname, int cur_delta, long site);
extern "C" int rt_patv_defer_run_all(void *hv, long i, const char *fb, int cur_delta);
extern "C" void *dtp_fn_of(void *headv);
extern "C" void *rt_defer_xpat_dtp(const char *nm);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int defer_xpat_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_DEFER_XPAT"); v = (e && *e == '0') ? 0 : 1; } return v; }
extern "C" uint64_t g_sno_defer_cells[4096];
extern uint64_t g_scan_hit_start;
extern int g_gva_active;
extern "C" uint64_t g_rspd_save, g_rspd_g4, g_rspd_g5, g_rspd_s2, g_rspd_g6, g_rspd_beta;
#include "x86_asm.h"
extern "C" int sn4_alt_carrier(void);
#define dswap() (x86_zc_frame() == ZC_FRAME_RSP)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dw_cell(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DEFER_CELL"); v = e ? (atoi(e) != 0) : 1; } return v; }
static int one_defer(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ONE_DEFER"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int defer_inline(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DEFER_INLINE"); v = (e && *e == '0') ? 0 : 1; } return v; }
extern "C" int emit_defer_carve_rbp(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dfrm(void) { return (_.op_seal == 1) || emit_defer_carve_rbp(); }
#define rspd()  (getenv("SCRIP_RSPDIFF") ? 1 : 0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⛔⭐⭐ PROMOTED TO DEFAULT-ON (hq_P, Lon's tier-1 demo campaign; the arm itself is UNCHANGED and is s168 PT-3, commit 72f9c772, 2026-08-19).  ⛔ THE POLARITY WAS THE DEFECT, NOT THE ARM.  This read
   v = (e && *e && *e != CHAR0) ? 1 : 0 -- OPT-IN, so an unset variable meant the collapse was NOT EMITTED.  Every other switch in this same file is opt-OUT (defer_xpat_on, defer_ic_on, and rt_defer_merge_on
   next door): v = (e && *e == CHAR0) ? 0 : 1.  So a cure that was written, reviewed, measured and committed sat DARK for eight days, and every demo profile taken in that window measured the code path this arm
   exists to delete -- which is exactly why the tier-1 dig still found rt_patv_defer_get_pat_dtp at 16% of calculator-1-match and an RTCC-wrapped PLT call per deferred grammar-node visit in the emitted boxes.
   ⭐ THE CLASS, AND IT IS WORTH MORE THAN THIS ONE FLAG: a default-OFF killswitch on a CURE is not a killswitch, it is a deletion with a comment explaining what it used to do.  It is invisible in every
   profile, it cannot be distinguished from unwritten code, and it survives review precisely because the file still reads as though the cure shipped.  A staging flag needs an owner and a promotion date, or it
   needs to default ON with an escape hatch -- which is what it now has.  ⛔ ITS ORIGINAL EVIDENCE STANDS AND WAS NOT TAKEN ON TRUST: 72f9c772 graded 1034 programs across both media with ZERO arm-caused movers
   (four apparent m3 movers all disproven by a hold-the-arm-fixed control), oracle-graded the 112 programs that emit the collapse at PASS 74 / arm-caused FAIL 0, and measured treebank-match 1.41x and
   treebank-match-fence 1.63x on disjoint windows.  RE-VERIFIED ON TODAY'S TREE before this flip -- see the commit message for the board and the A/B, because that tree is eight days and hundreds of commits old.
   SCRIP_PATV_FAST=0 is now the control arm and reproduces the pre-flip binary exactly. */
static int patv_fast_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PATV_FAST"); v = (e && *e == '0') ? 0 : 1; } return v; }
/* ⭐ KILLSWITCH + CONTROL ARM for the merged-defer inline cache below (RULES.md: every perf claim ships one).  SCRIP_DEFER_IC=0 removes the inline arm entirely and every site falls back to the unmodified
   rt_defer_probe_run call, so the cure can be A/B'd in one binary without a rebuild.  Default ON, same polarity and same spelling as defer_xpat_on/rt_defer_merge_on. */
static int defer_ic_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_DEFER_IC"); v = (e && *e == '0') ? 0 : 1; } return v; }
#define rspd_snap(cell, nm) IF(rspd(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)(cell),nm) \
                                     + x86("mov",RDQ("rcx",0),"rsp"))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_defer() {
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    int vslot = -1; { const char *sv = _.op_sval; if (sv) { const char *d = strstr(sv, "$V"); if (d && d[2] >= '0' && d[2] <= '9') { char *e = 0; long k = strtol(d + 2, &e, 10); if (e && !*e) vslot = (int)k; } } }
    int ci = (vslot < 0 && dw_cell() && g_gva_active && _.op_gva_k >= 0 && _.op_seal == 2 && g_emit.sn4_defer_cell_n < 2048) ? g_emit.sn4_defer_cell_n++ : -1;   /* ⛔ 2048 not 4096: the UPPER half of g_sno_defer_cells holds the merged arm's self-validating (key,cell) PAIRS, and a DTP written into one of those words would be read back as a cell address */
    static char cl[8][48]; static int cln; if (ci >= 0) { cln = (cln + 1) & 7; snprintf(cl[cln], sizeof cl[cln], "g_sno_defer_cells+%d", ci * 8); }
    const char * clbl = ci >= 0 ? cl[cln] : "";
    int merged = (vslot < 0 && one_defer() && !(g_gva_active && _.op_gva_k >= 0));
    /* ⭐ per-site cell slot for the merged arm, taken from the UPPER half of g_sno_defer_cells so it can never overlap the DTP cache the ci arm writes into the lower half (a shared slot would let one arm read the
       other's word as its own -- the runtime's slot[0]==varname check turns a collision into a harmless miss for US, but our write would corrupt a DTP for THEM, so the ranges are kept disjoint by construction). */
    static int g_defer_site_n; int msite = merged ? (g_defer_site_n < 1024 ? g_defer_site_n++ : -1) : -1;
    static char pl[8][48]; static int pln; if (msite >= 0) { pln = (pln + 1) & 7; snprintf(pl[pln], sizeof pl[pln], "g_sno_defer_cells+%d", (2048 + msite * 2) * 8); }
    const char * pairlbl = msite >= 0 ? pl[pln] : "";
    uint64_t pairadr = msite >= 0 ? (uint64_t)(uintptr_t)(const void *)&g_sno_defer_cells[2048 + msite * 2] : 0;
    uint64_t cadr = ci >= 0 ? (uint64_t)(uintptr_t)(const void *)&g_sno_defer_cells[ci] : 0;
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
         + IF(vslot >= 0 && patv_fast_on(),
               x86("comment", "⭐ PT-3 ROUND-TRIP COLLAPSE (SCRIP_PATV_FAST): the 3-call C round trip rt_patv_defer_get_pat_dtp -> patv_slot -> dtp_fn_of, measured at 59.4% of ALL treebank-match cycles (FINDING s168 PT-0/1/2), is spent ONLY on the arms that need it.  WHAT MAKES THE SKIP PROVABLE, not merely plausible: dtp_fn_of is `if (!h->fn && h->rcp) { lazy-compile }; return h->fn;` -- so once fn is MATERIALIZED it is a PURE function returning [dtp+0], which is the very word the cold arm below re-loads by hand two lines later.  Reproduce patv_slot's snap arm inline (h=[rbp-24], snap=[h+32], nsnap=[h+40] -- the offsets DTP_t's own definition publishes for asm consumers), take the DT_P/payload test in the GVA arm's already-verified spelling, and if fn is non-null we are DONE with zero calls.  EVERY other case -- no DTP, no snap, index past nsnap, not DT_P, null payload, fn not yet compiled -- falls through to the UNCHANGED cold path, so the arm cannot answer differently, only sooner.")
             + x86("mov",  "rdi", RDQ("rbp", -24))
             + x86("test", "rdi", "rdi")
             + x86("je",   L(17))
             + x86("mov",  "rsi", RDQ("rdi", 32))
             + x86("test", "rsi", "rsi")
             + x86("je",   L(17))
             + x86("cmp",  RDQ("rdi", 40), (long)vslot + 1)
             + x86("jl",   L(17))
             + x86("mov",  "rax", RDQ("rsi", vslot * 16))
             + x86("cmp",  "al", (long)DT_P)
             + x86("jne",  L(17))
             + x86("mov",  "rdx", RDQ("rsi", vslot * 16 + 8))
             + x86("test", "rdx", "rdx")
             + x86("je",   L(17))
             + x86("mov",  "rax", RDQ("rdx", 0))
             + x86("test", "rax", "rax")
             + x86("jne",  L(18))
             + x86("def",  L(17)))
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
             + x86("def",  L(16))
             + IF(patv_fast_on(), x86("def", L(18))))
         + IF(vslot < 0 && g_gva_active && _.op_gva_k >= 0,
               x86("note", gva_name(_.op_gva_k)) + x86("mov",  "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16))
             + x86("note", gva_name(_.op_gva_k)) + x86("mov",  "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))
             + x86("cmp",  "al", (long)DT_P)
             + x86("jne",  L(9))
             + x86("mov",  "rax", RDQ("rdx", 0))
             + x86("test", "rax", "rax")
             + x86("jne",  L(10))
             + x86("mov",  "rdi", "rdx")
             + x86_align_enter()
             + x86("call", "dtp_fn_of", (uint64_t)(uintptr_t)(void *)(void *(*)(void *))dtp_fn_of)
             + x86_align_leave()
             + x86("note", gva_name(_.op_gva_k)) + x86("mov",  "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))
             + x86("jmp",  L(10))
             + x86("def",  L(9))
             + IF(defer_xpat_on(),
                  x86("cmp",  "al", (long)DT_X)
                + x86("jne",  L(21))
                + x86("mov",  "rdi", "rdx")
                + x86_align_enter()
                + x86("call", "rt_defer_xpat_dtp", (uint64_t)(uintptr_t)(void *)(void *(*)(const char *))rt_defer_xpat_dtp)
                + x86_align_leave()
                + x86("mov",  "rdx", "rax")
                + x86("test", "rax", "rax")
                + x86("je",   L(21))
                + x86("mov",  "rax", RDQ("rdx", 0))
                + x86("jmp",  L(10))
                + x86("def",  L(21)))
             + x86("xor",  "eax", "eax")
             + x86("def",  L(10)))
         + IF(ci >= 0,
               x86("test", "rax", "rax")
             + x86("je",   L(15))
             + x86("lea",  "rsi", "[rip + __]", cadr, clbl)
             + x86("mov",  RDQ("rsi", 0), "rdx")
             + x86("def",  L(15))
             + x86("def",  L(11)))
         + IF(merged && msite >= 0 && defer_inline() && defer_ic_on(),
               x86("comment", "⭐⭐ FUSED PROBE HEAD (ceo s283, row perf-pattern-defer-capture-layer-cure): the one-char-string arm (defer_inline, s261) and the resolved-fn inline cache (defer_ic, slice a) each loaded the SAME (key,cell) pair and compared the SAME baked varname back to back -- and the one-char arm's DT_S/len==1 preconditions ran FIRST on every position even when the deferred value is a PATTERN (this kernel: tag DT_P=8, so the one-char probe was a structurally-guaranteed miss costing ~9 insns per anchor position, the ARM-A class of a probe that cannot fire).  MEASURED before this cure (pattern_bt m4, 2M iter, perf annotate): n58_match_defer_bx 28.9%% of the kernel with the duplicated head executed per position.  THIS ARM: ONE pair load, ONE key compare, ONE cell load + null test (⛔ the standalone one-char arm dereferenced cell WITHOUT a null test -- latent, inherited from s261, fixed here for free), ONE tag read, then a TAG BRANCH routes to the one-char tail (DT_S) or the fn-cache tail (DT_P); every miss lands on the UNCHANGED cold call at L22.  ⛔ IT CANNOT ANSWER DIFFERENTLY, ONLY SOONER -- both tails are byte-for-byte the originals' tails, every fall-through reaches the same cold path, and a multi-char DT_S goes cold exactly as before (old route: one-char len-miss -> fn-cache tag-miss -> cold; new route: len-miss -> cold, same destination two compares earlier).  ⭐ KILLSWITCH MATRIX PRESERVED: this fused arm emits only when BOTH SCRIP_DEFER_INLINE and SCRIP_DEFER_IC are on (the default); either switch off restores that arm's ORIGINAL standalone emission verbatim below, so the A/B story of each original cure survives.")
             + x86("lea",   "rcx", "[rip + __]", pairadr, pairlbl)
             + x86("mov",   "rax", RDQ("rcx", 0))
             + x86("lea",   "rdx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("cmp",   "rax", "rdx")
             + x86("jne",   L(22))
             + x86("mov",   "rax", RDQ("rcx", 8))
             + x86("test",  "rax", "rax")
             + x86("je",    L(22))
             + x86("mov",   "edx", RDD("rax", 0))
             + x86("and",   "edx", 255L)
             + x86("cmp",   "edx", (long)DT_S)
             + x86("je",    L(32))
             + x86("cmp",   "edx", (long)DT_P)
             + x86("jne",   L(22))
             + x86("mov",   "rdx", RDQ("rax", 8))
             + x86("test",  "rdx", "rdx")
             + x86("je",    L(22))
             + x86("mov",   "rax", RDQ("rdx", 0))
             + x86("test",  "rax", "rax")
             + x86("jne",   L(23))
             + x86("jmp",   L(22))
             + x86("def",   L(32))
             + x86("mov",   "edx", RDD("rax", 4))
             + x86("cmp",   "edx", 1L)
             + x86("jne",   L(22))
             + x86("movsxd","rcx", "r14d")
             + x86("cmp",   "ecx", "r15d")
             + x86("jge",   L(31))
             + x86("movzx", "esi", "[r13+rcx]")
             + x86("mov",   "rdi", RDQ("rax", 8))
             + x86("xor",   "edx", "edx")
             + x86("movzx", "edi", "[rdi+rdx]")
             + x86("cmp",   "esi", "edi")
             + x86("jne",   L(31))
             + x86("mov",   "edx", "r14d")
             + x86("add",   "edx", 1L)
             + x86("jmp",   "L0")
             + x86("def",   L(31))
             + x86("mov",   "edx", -1L)
             + x86("jmp",   "L0")
             + x86("def",   L(22)))
         + IF(merged && msite >= 0 && defer_inline() && !defer_ic_on(),
               x86("comment", "⭐⭐⭐ THE DEFERRED READ, INLINE, WITH NO CALL AT ALL (hq_P s261).  After the cell cache the remaining cost was not the lookup -- it was the CEREMONY of reaching it: an xfer_enter/leave push-pop of r13/r14/r15, an rtccb save/restore of r8/r9, a PLT call, and a branch chain, ~120 Ir per execution for what is finally 'read a cell, compare one byte'.  This arm does the whole common case in ~18 instructions: confirm the site's cached (key,cell) pair still names THIS baked literal, confirm the value is a one-character string, and compare that character against the subject at the cursor.  ⛔ WHY IT CANNOT ANSWER DIFFERENTLY, ONLY SOONER -- the same argument the PT-3 arm above already relies on: EVERY other case falls through to L30 and the UNCHANGED call.  A cold slot, a different name at a shared slot, a non-string value, a multi-character string, a pattern, an unevaluated expression -- all take the old path untouched.  ⭐ THE REGISTER MAP IS NOT ASSUMED, IT IS VERIFIED: bb_match_break's emitted scan reads 'movzx esi, byte ptr [r13+rcx]' after 'movsxd rcx, r14d' and 'cmp ecx, r15d', so r13 is the subject base, r14d the cursor and r15d the length.  This arm reuses that exact idiom rather than a second guess about which register holds what.")
             + x86("lea",   "rcx", "[rip + __]", pairadr, pairlbl)
             + x86("mov",   "rax", RDQ("rcx", 0))
             + x86("lea",   "rdx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("cmp",   "rax", "rdx")
             + x86("jne",   L(30))
             + x86("mov",   "rax", RDQ("rcx", 8))
             + x86("mov",   "edx", RDD("rax", 0))
             + x86("and",   "edx", 255L)
             + x86("cmp",   "edx", (long)DT_S)
             + x86("jne",   L(30))
             + x86("mov",   "edx", RDD("rax", 4))
             + x86("cmp",   "edx", 1L)
             + x86("jne",   L(30))
             + x86("movsxd","rcx", "r14d")
             + x86("cmp",   "ecx", "r15d")
             + x86("jge",   L(31))
             + x86("movzx", "esi", "[r13+rcx]")
             + x86("mov",   "rdi", RDQ("rax", 8))
             + x86("xor",   "edx", "edx")
             + x86("movzx", "edi", "[rdi+rdx]")
             + x86("cmp",   "esi", "edi")
             + x86("jne",   L(31))
             + x86("mov",   "edx", "r14d")
             + x86("add",   "edx", 1L)
             + x86("jmp",   "L0")
             + x86("def",   L(31))
             + x86("mov",   "edx", -1L)
             + x86("jmp",   "L0")
             + x86("def",   L(30)))
         + IF(merged && msite >= 0 && defer_ic_on() && !defer_inline(),
               x86("comment", "⭐⭐ THE RESOLVED-fn INLINE CACHE (hq_P, slice (a) of perf-pattern-defer-capture-layer-cure).  The merged arm below already resolves ONCE instead of twice, and the runtime already remembers the site's NV cell in the g_sno_defer_cells pair -- but EVERY iteration still paid a full C call to be told a thing the pair could answer here: xfer_enter's three wire pushes, the argument marshalling, call/ret, rt_defer_probe_run's own prologue, the (now always_inline) slot compare, and the load of [dtp+0].  MEASURED at fixed work on pattern_bt, m4 -O0: rt_defer_probe_run 15.1% + rt_defer_cell_read 10.5% + dtp_fn_of 5.1% of the whole program, with the call ceremony itself inside n58_match_defer_a's 25.7%.  This arm answers the DT_P-with-materialised-fn case with ZERO calls and zero stack traffic, in the same register contract the call publishes: rax=fn, rdx=dtp.  ⛔ IT CANNOT ANSWER DIFFERENTLY, ONLY SOONER -- exactly the PT-3 argument the $V arm above already relies on.  Six conditions must ALL hold or we fall through to the unmodified call: the pair's key still equals THIS site's baked varname (pointer identity, so a collision or a re-resolution MISSES rather than lying), a non-null cell, cell->v == DT_P, a non-null payload, and a non-null [dtp+0].  fn is read out of the LIVE DTP every iteration and is never memoised beside the pair, so reassigning the deferred variable is seen immediately -- a new DTP has a different pointer and a different fn, and a non-pattern value fails the DT_P test.  ⭐ NO NEW GLOBAL: the pair is the EXISTING slot the runtime already writes; this arm only READS it, and it is the runtime's own write that arms it on the first iteration.  ⛔ The killswitch SCRIP_DEFER_MERGE=0 also disarms this for free -- with merging off the runtime never writes the pair, the key stays 0, and every probe misses.")
             + x86("lea",  "rsi", "[rip + __]", pairadr, pairlbl)
             + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("mov",  "rcx", RDQ("rsi", 0))
             + x86("cmp",  "rcx", "rdi")
             + x86("jne",  L(22))
             + x86("mov",  "rcx", RDQ("rsi", 8))
             + x86("test", "rcx", "rcx")
             + x86("je",   L(22))
             + x86("mov",  "rax", RDQ("rcx", 0))
             + x86("cmp",  "al", (long)DT_P)
             + x86("jne",  L(22))
             + x86("mov",  "rdx", RDQ("rcx", 8))
             + x86("test", "rdx", "rdx")
             + x86("je",   L(22))
             + x86("mov",  "rax", RDQ("rdx", 0))
             + x86("test", "rax", "rax")
             + x86("jne",  L(23))
             + x86("def",  L(22)))
         + IF(merged,
               x86("comment", "⭐ ONE RESOLUTION, NOT TWO (hq_P s260): this site used to call rt_defer_get_pat_dtp and then, on the not-a-pattern fall-through, rt_defer_run_all -- and BOTH opened by resolving the SAME baked literal through the global name table, back to back, with only a test and a jz between them.  Measured on roman.sno: rt_defer_nv_read'rt_defer_get_pat_dtp and rt_defer_nv_read'rt_defer_run_all at 594,060 Ir EACH -- identical counts -- pushing NV_GET_fn to 19.35% of the whole program with another 5.91% of __strcmp_avx2 under it.  rt_defer_probe_run resolves once and answers both questions in registers: rax=fn (0 => not a pattern, take L0), rdx=dtp when it IS a pattern and the new cursor when it is not.  The string half therefore needs no call at all -- L0 just moves edx into eax.  esi now carries cur_delta where it used to carry a constant-zero ival_flag, which is sound because this is the only arm that reaches here and it always passed 0.")
             + x86_xfer_enter()
             + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("mov",  "esi", "r14d")
             + x86("mov",  "rdx", (long)msite)
             + x86_anchor_enter()
             + x86("call", "rt_defer_probe_run", (uint64_t)(uintptr_t)(void *)(rt_defer_pr_t (*)(const char *, int, long))rt_defer_probe_run)
             + x86_anchor_leave()
             + x86_xfer_leave())
         + IF(merged && msite >= 0 && defer_ic_on(), x86("comment", "the inline cache's fast exit lands here: rax=fn, rdx=dtp, identical to what the call leaves") + x86("def", L(23)))
         + x86("test", "rax", "rax")
         + x86("jz",   "L0")
         + rspd_snap(&g_rspd_save, "g_rspd_save")
         + x86("mov",  "r8d", (long)(_.op_scan ? 1 : 0))
         + bb_glue_pass_wires_blob(4, 5)
         + x86("def",  L(4))
         + bb_glue_wire_land()
         + IF(dfrm() && x86_port_cstack() && emit_defer_rbp(),
               x86("mov", "rsp", "rbp") + x86("pop", "rbp"))
         + IF(dfrm() && x86_port_cstack() && !emit_defer_rbp(),
               x86("mov",  "rsp", FRQ(_.op_off)))
         + IF(_.op_scan && _.op_scan_head_off >= 0 && !emit_match_owns_startd(),
               x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_scan_hit_start, "g_scan_hit_start")
             + x86("mov",  "rax", "[rcx]")
             + x86("mov",  emit_match_begin_stfh_k() > 0 ? "dword ptr [rsp# + 0]" : FR(_.op_scan_head_off), "eax"))
         + rspd_snap(&g_rspd_g4, "g_rspd_g4")
         + x86_gamma()
         + x86("def",  L(5))
         + bb_glue_wire_land()
         + IF(dfrm() && x86_port_cstack() && emit_defer_rbp(),
               x86("mov", "rsp", "rbp") + x86("pop", "rbp"))
         + IF(dfrm() && x86_port_cstack() && !emit_defer_rbp(),
               x86("mov",  "rsp", FRQ(_.op_off)))
         + rspd_snap(&g_rspd_g5, "g_rspd_g5")
         + x86_omega()
         + (one_defer()
             ? x86("def",  "L0")
             + IF(merged, x86("comment", "⭐ the string half of the merged probe: rt_defer_probe_run already ran it and left the new cursor in rdx, so there is no second call and no second name lookup")
                        + x86("mov",  "eax", "edx"))
             + IF(!merged, x86_xfer_enter())
             + IF(!merged && vslot < 0,
                   x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
                 + x86("mov",  "esi", "r14d"))
             + IF(vslot >= 0,
                   x86("mov",  "rdi", RDQ("rbp", -24))
                 + x86("mov",  "esi", (long)vslot)
                 + x86("lea",  "rdx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
                 + x86("mov",  "ecx", "r14d"))
             + IF(!merged, x86_anchor_enter())
             + IF(!merged && vslot < 0,  x86("call", "rt_defer_run_all", (uint64_t)(uintptr_t)(void *)(int (*)(const char *, int))rt_defer_run_all))
             + IF(vslot >= 0, x86("call", "rt_patv_defer_run_all", (uint64_t)(uintptr_t)(void *)(int (*)(void *, long, const char *, int))rt_patv_defer_run_all))
             + IF(!merged, x86_anchor_leave())
             + IF(!merged, x86_xfer_leave())
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
         + bb_glue_pass_wires(7, 8)
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
              : (IF(dfrm() && _.op_seal != 1 && x86_port_cstack() && emit_defer_rbp(),
                      x86("comment", "s139 UNSEALED CARVE-DEFER beta: RESTORE THE FRAME, THEN RESUME THE RECORD")
                    + x86("mov", "rsp", "rbp")
                    + x86("pop", "rbp"))
                 + (_.op_defer_leaf_susp > 0
                   ? (rspd_snap(&g_rspd_beta, "g_rspd_beta")
                      + x86("mov",  "rax", RDQ("rsp", 0))
                      + x86("test", "rax", "rax")
                      + x86("jne",  L(12))
                      + x86_zrelease((long)_.op_defer_leaf_susp)
                      + x86_omega()
                      + x86("def",  L(12))
                      + x86_jmp_reg("rax"))
                   : (rspd_snap(&g_rspd_beta, "g_rspd_beta")
                      + (({ static int _bg = -1; if (_bg < 0) { const char * e = getenv("SCRIP_DEFER_BETA_GUARD"); _bg = (e && *e == '0') ? 0 : 1; } _bg; })
                          ? (x86_reg_disp32_cmp_imm("rsp", 0, 0L)
                           + x86("jne",  L(12))
                           + x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)&rtccb[0], "rtccb")
                           + x86("mov", "rax", RDQ("rcx", 248))
                           + x86("test", "rax", "rax")
                           + x86("je",   L(12))
                           + x86_jmp_reg("rax")
                           + x86("def",  L(12))
                           + x86_jmp_mem("rsp", 0))
                          : x86_jmp_mem("rsp", 0))))));
}
