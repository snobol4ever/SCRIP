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
extern "C" int emit_defer_carve_rbp(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dfrm(void) { return (_.op_seal == 1) || emit_defer_carve_rbp(); }
#define rspd()  (getenv("SCRIP_RSPDIFF") ? 1 : 0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int patv_fast_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PATV_FAST"); v = (e && *e && *e != '0') ? 1 : 0; } return v; }
#define rspd_snap(cell, nm) IF(rspd(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)(cell),nm) \
                                     + x86("mov",RDQ("rcx",0),"rsp"))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_defer() {
    if (!PLATFORM_X86) return std::string();
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    int vslot = -1; { const char *sv = _.op_sval; if (sv) { const char *d = strstr(sv, "$V"); if (d && d[2] >= '0' && d[2] <= '9') { char *e = 0; long k = strtol(d + 2, &e, 10); if (e && !*e) vslot = (int)k; } } }
    int ci = (vslot < 0 && dw_cell() && g_gva_active && _.op_gva_k >= 0 && _.op_seal == 2 && g_emit.sn4_defer_cell_n < 4096) ? g_emit.sn4_defer_cell_n++ : -1;
    static char cl[8][48]; static int cln; if (ci >= 0) { cln = (cln + 1) & 7; snprintf(cl[cln], sizeof cl[cln], "g_sno_defer_cells+%d", ci * 8); }
    const char * clbl = ci >= 0 ? cl[cln] : "";
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
         + IF(vslot < 0 && !(g_gva_active && _.op_gva_k >= 0),
               x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("xor",  "esi", "esi")
             + x86_align_enter()
             + x86("call", "rt_defer_get_pat_dtp", (uint64_t)(uintptr_t)(void *)(void *(*)(const char *, int))rt_defer_get_pat_dtp)
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
