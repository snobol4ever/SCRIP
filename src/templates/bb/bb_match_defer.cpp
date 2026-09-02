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
#define dswap() (1)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dw_cell(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DEFER_CELL"); v = e ? (atoi(e) != 0) : 1; } return v; }
static int one_defer(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ONE_DEFER"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int defer_inline(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_DEFER_INLINE"); v = (e && *e == '0') ? 0 : 1; } return v; }
extern "C" int emit_defer_carve_rbp(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dfrm(void) { return (_.op_seal == 1) || emit_defer_carve_rbp(); }
#define rspd()  (getenv("SCRIP_RSPDIFF") ? 1 : 0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int patv_fast_on() { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_PATV_FAST"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int defer_ic_on(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_DEFER_IC"); v = (e && *e == '0') ? 0 : 1; } return v; }
#define rspd_snap(cell, nm) IF(rspd(), x86("lea","rcx","[rip + __]",(uint64_t)(uintptr_t)(const void*)(cell),nm) \
                                     + x86("mov",RDQ("rcx",0),"rsp"))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_defer() {
    static char b[24];
    strtab_label(b, sizeof b, _.op_sval ? _.op_sval : "");
    int vslot = -1;
    { const char *sv = _.op_sval, *d = sv ? strstr(sv, "$V") : 0;
      if (d && d[2] >= '0' && d[2] <= '9')
      { char *e = 0; long k = strtol(d + 2, &e, 10); if (e && !*e) vslot = (int)k; } }
    int ci = (vslot < 0 && dw_cell() && g_gva_active && _.op_gva_k >= 0 && _.op_seal == 2 && g_emit.sn4_defer_cell_n < 2048) ? g_emit.sn4_defer_cell_n++ : -1;
    static char cl[8][48]; static int cln; if (ci >= 0) { cln = (cln + 1) & 7; snprintf(cl[cln], sizeof cl[cln], "g_sno_defer_cells+%d", ci * 8); }
    const char * clbl = ci >= 0 ? cl[cln] : "";
    int merged = (vslot < 0 && one_defer() && !(g_gva_active && _.op_gva_k >= 0));
    static int g_defer_site_n; int msite = merged ? (g_defer_site_n < 1024 ? g_defer_site_n++ : -1) : -1;
    static char pl[8][48]; static int pln; if (msite >= 0) { pln = (pln + 1) & 7; snprintf(pl[pln], sizeof pl[pln], "g_sno_defer_cells+%d", (2048 + msite * 2) * 8); }
    const char * pairlbl = msite >= 0 ? pl[pln] : "";
    uint64_t pairadr = msite >= 0 ? (uint64_t)(uintptr_t)(const void *)&g_sno_defer_cells[2048 + msite * 2] : 0;
    uint64_t cadr = ci >= 0 ? (uint64_t)(uintptr_t)(const void *)&g_sno_defer_cells[ci] : 0;
    return x86("comment", "IR_MATCH_DEFER (ZS-2 jmp-entry)")
         + x86_alpha()
         + IF(dfrm() && emit_defer_rbp(),
               x86("comment", "IR_MATCH_DEFER ζ-frame")
             + x86("push", "rbp")
             + x86("mov",  "rbp", "rsp"))
         + IF(dfrm() && !emit_defer_rbp(),
               x86("comment", "IR_MATCH_DEFER sealed")
             + x86("mov",  FRQ(_.op_off), "rsp"))
         + IF(ci >= 0,
               x86("comment", "IR_MATCH_DEFER cell")
             + x86("lea",  "rsi", "[rip + __]", cadr, clbl)
             + x86("mov",  "rdx", RDQ("rsi", 0))
             + x86("test", "rdx", "rdx")
             + x86("je",   L(13))
             + x86("mov",  "rax", RDQ("rdx", 0))
             + x86("jmp",  L(11))
             + x86("def",  L(13)))
         + IF(vslot >= 0 && patv_fast_on(),
               x86("comment", "IR_MATCH_DEFER patv-fast")
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
               x86("comment", "IR_MATCH_DEFER $V-slot")
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
               x86("note", gva_name(_.op_gva_k))
             + x86("mov",  "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 0) : ABSQ(RT_GVA_VA + _.op_gva_k * 16))
             + x86("note", gva_name(_.op_gva_k))
             + x86("mov",  "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))
             + x86("cmp",  "al", (long)DT_P)
             + x86("jne",  L(9))
             + x86("mov",  "rax", RDQ("rdx", 0))
             + x86("test", "rax", "rax")
             + x86("jne",  L(10))
             + x86("mov",  "rdi", "rdx")
             + x86_align_enter()
             + x86("call", "dtp_fn_of", (uint64_t)(uintptr_t)(void *)(void *(*)(void *))dtp_fn_of)
             + x86_align_leave()
             + x86("note", gva_name(_.op_gva_k))
             + x86("mov",  "rdx", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(_.op_gva_k, 8) : ABSQ(RT_GVA_VA + _.op_gva_k * 16 + 8))
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
               x86("comment", "IR_MATCH_DEFER probe-head")
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
               x86("comment", "IR_MATCH_DEFER inline-read")
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
               x86("comment", "IR_MATCH_DEFER ic")
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
               x86("comment", "IR_MATCH_DEFER resolve")
             + x86_xfer_enter()
             + x86("lea",  "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)(_.op_sval ? _.op_sval : ""), b)
             + x86("mov",  "esi", "r14d")
             + x86("mov",  "rdx", (long)msite)
             + x86_anchor_enter()
             + x86("call", "rt_defer_probe_run", (uint64_t)(uintptr_t)(void *)(rt_defer_pr_t (*)(const char *, int, long))rt_defer_probe_run)
             + x86_anchor_leave()
             + x86_xfer_leave())
         + IF(merged && msite >= 0 && defer_ic_on(), x86("comment", "IR_MATCH_DEFER ic-hit")
                                                   + x86("def", L(23)))
         + x86("test", "rax", "rax")
         + x86("jz",   "L0")
         + rspd_snap(&g_rspd_save, "g_rspd_save")
         + x86("mov",  "r8d", (long)(_.op_scan ? 1 : 0))
         + bb_glue_pass_wires_blob(4, 5)
         + x86("def",  L(4))
         + bb_glue_wire_land()
         + IF(dfrm() && emit_defer_rbp(),
               x86("mov", "rsp", "rbp")
             + x86("pop", "rbp"))
         + IF(dfrm() && !emit_defer_rbp(),
               x86("mov",  "rsp", FRQ(_.op_off)))
         + IF(_.op_scan && _.op_scan_head_off >= 0 && !emit_match_owns_startd(),
               x86("lea",  "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_scan_hit_start, "g_scan_hit_start")
             + x86("mov",  "rax", "[rcx]")
             + x86("mov",  emit_match_begin_stfh_k() > 0 ? "dword ptr [rsp# + 0]" : FR(_.op_scan_head_off), "eax"))
         + rspd_snap(&g_rspd_g4, "g_rspd_g4")
         + x86_gamma()
         + x86("def",  L(5))
         + bb_glue_wire_land()
         + IF(dfrm() && emit_defer_rbp(),
               x86("mov", "rsp", "rbp")
             + x86("pop", "rbp"))
         + IF(dfrm() && !emit_defer_rbp(),
               x86("mov",  "rsp", FRQ(_.op_off)))
         + rspd_snap(&g_rspd_g5, "g_rspd_g5")
         + x86_omega()
         + (one_defer()
             ? x86("def",  "L0")
             + IF(merged, x86("comment", "IR_MATCH_DEFER probe-str")
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
               x86("comment", "IR_MATCH_DEFER $V-scalar")
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
               x86("comment", "IR_MATCH_DEFER pseudo-record")
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
         + ((_.op_seal == 1)
              ? ((emit_defer_rbp() ? (x86("mov", "rsp", "rbp")
                                    + x86("pop", "rbp"))
                                   : x86("mov", "rsp", FRQ(_.op_off)))
                 + x86_omega())
              : (IF(dfrm() && _.op_seal != 1 && emit_defer_rbp(),
                      x86("comment", "IR_MATCH_DEFER β unsealed-carve")
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
