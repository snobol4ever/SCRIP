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
extern "C" void *dtp_fn_of(void *headv);
extern "C" uint64_t g_sno_defer_cells[4096];
extern uint64_t g_scan_hit_start;
extern int g_gva_active;
extern "C" uint64_t g_rspd_save, g_rspd_g4, g_rspd_g5, g_rspd_s2, g_rspd_g6, g_rspd_beta;
#include "x86_asm.h"
#define dswap() (x86_zc_frame() == ZC_FRAME_RSP)
static int dw_cell(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DEFER_CELL"); v = e ? (atoi(e) != 0) : 1; } return v; }   /* s142 DEFER-SITE DIET kill-switch (NOFILL precedent): =0 restores the uncached GVA dance for A/B and emergencies */
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
     * scratch: dead at α on this path (the non-GVA arm's xor esi,esi is the only prior user), clobbered by the dtp_fn_of C call, hence the re-lea before the store.  Index claim = bb_slot_claim precedent
     * (emit-time staging at template top); ≥4096 falls back to the uncached path; counter monotonic per process (uniqueness is the only requirement). */
    int ci = (dw_cell() && g_gva_active && _.op_gva_k >= 0 && _.op_seal == 2 && g_emit.sn4_defer_cell_n < 4096) ? g_emit.sn4_defer_cell_n++ : -1;
    static char cl[8][48]; static int cln; if (ci >= 0) { cln = (cln + 1) & 7; snprintf(cl[cln], sizeof cl[cln], "g_sno_defer_cells+%d", ci * 8); }
    const char * clbl = ci >= 0 ? cl[cln] : "";
    uint64_t cadr = ci >= 0 ? (uint64_t)(uintptr_t)(const void *)&g_sno_defer_cells[ci] : 0;
    /* s137 OVER-SEAL (Lon ruling: a fence clearly demarks a point OUTSIDE a γ where entire chunks of ζ can be whacked, since no backtracking is guaranteed): when the defer's target is a STATICALLY
     * right-sealed stored pattern (IR_t.seal → op_seal), this element is that demarked sync point in ITS OWN activation — α stamps rsp into the defer.pad quad (FRQ(op_off), rbp-relative → recursion-
     * safe), the L(4)/L(5) glues restore it (bulk-freeing the callee's ENTIRE retained subtree: frame, suspend record, every transitive carve — the resume surface is already dead by NCB-2/SZ-1
     * body_root=NULL, so nothing the whack destroys is ever read), and β restores-then-ωs instead of `jmp [rsp+0]` (the fast-path record is whacked; the SLOW-path L(6) exhaust record is discarded by
     * the same restore, keeping the frontier LIFO exact for the left neighbour).  CSTACK/FORTH only — other ports keep the untouched original body. */
    return x86("comment", "IR_MATCH_DEFER (ZS-2 jmp-entry)")
         + x86_alpha()
         + IF(_.op_seal == 1 && x86_port_cstack(),
               x86("comment", "s137 SEALED defer: fence-demarked sync point (watermark in defer.pad)")
             + x86("mov",  FRQ(_.op_off), "rsp"))
         + IF(ci >= 0,
               x86("lea",  "rsi", "[rip + __]", cadr, clbl)
             + x86("mov",  "rax", RDQ("rsi", 0))
             + x86("test", "rax", "rax")
             + x86("jne",  L(11)))
         + IF(g_gva_active && _.op_gva_k >= 0,
               x86("note", gva_name(_.op_gva_k)) + x86("mov",  "rax", ABSQ(RT_GVA_VA + _.op_gva_k * 16))
             + x86("note", gva_name(_.op_gva_k)) + x86("mov",  "rdx", ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))
             + x86("cmp",  "eax", (long)DT_P)
             + x86("jne",  L(9))
             + x86("mov",  "rax", RDQ("rdx", 0))
             + x86("test", "rax", "rax")
             + x86("jne",  L(10))
             + x86("mov",  "rdi", "rdx")
             + x86_align_enter()
             + x86("call", "dtp_fn_of", (uint64_t)(uintptr_t)(void *)(void *(*)(void *))dtp_fn_of)
             + x86_align_leave()
             + x86("jmp",  L(10))
             + x86("def",  L(9))
             + x86("xor",  "eax", "eax")
             + x86("def",  L(10)))
         + IF(ci >= 0,
               x86("lea",  "rsi", "[rip + __]", cadr, clbl)
             + x86("mov",  RDQ("rsi", 0), "rax")
             + x86("def",  L(11)))
         + IF(!(g_gva_active && _.op_gva_k >= 0),
               x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("xor",  "esi", "esi")
             + x86_align_enter()
             + x86("call", "rt_defer_get_pat_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(const char *, int))rt_defer_get_pat_fn)
             + x86_align_leave())
         + x86("test", "rax", "rax")
         + x86("jz",   "L0")
         + rspd_snap(&g_rspd_save, "g_rspd_save")
         + x86("mov",  "r8d", (long)(_.op_scan ? 1 : 0))
         + bb_glue_pass_wires(4, 5)   /* PASS-THROUGH GLUE (s22v): the canonical consumer -- blob entry with this box's L(4)/L(5) as the ride-through γ/ω wires; byte-identical to the hand-rolled trio it replaces */
         + x86("def",  L(4))
         + IF(_.op_seal == 1 && x86_port_cstack(),
               x86("mov",  "rsp", FRQ(_.op_off)))
         + IF(_.op_scan && _.op_scan_head_off >= 0,
               x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_scan_hit_start, "g_scan_hit_start")
             + x86("mov",  "rax", "[rcx]")
             + x86("mov",  FR(_.op_scan_head_off), "eax"))

         + rspd_snap(&g_rspd_g4, "g_rspd_g4")
         + x86_gamma()
         + x86("def",  L(5))
         + IF(_.op_seal == 1 && x86_port_cstack(),
               x86("mov",  "rsp", FRQ(_.op_off)))
         + rspd_snap(&g_rspd_g5, "g_rspd_g5")
         + x86_omega()
         + x86("def",  "L0")
         + x86_xfer_enter()
         + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
         + x86("xor",  "esi", "esi")
         + x86_anchor_enter()
         + x86("call", "rt_defer_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, int))rt_defer_open)
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
         + x86("test", "eax", "eax")
         + x86_omega("js")
         + x86("mov",  "r14d", "eax")
         + x86_lea_id("rax", 6)
         + rspd_snap(&g_rspd_s2, "g_rspd_s2")
         + x86_sub("rsp", 8)
         + x86("push", "rax")
         + x86_gamma()
         + x86("def",  L(6))
         + x86_add("rsp", 16)
         + rspd_snap(&g_rspd_g6, "g_rspd_g6")
         + x86_omega()
         + x86_beta()
         + ((_.op_seal == 1 && x86_port_cstack())
              ? (x86("mov", "rsp", FRQ(_.op_off)) + x86_omega())
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
