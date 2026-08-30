#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
long    rt_proc_call_open(const char *name, int nargs);
void   *rt_proc_fn(const char *name);
void   *rt_proc_call_open_det(long idx, int nargs);
void   *rt_proc_call_open_det0(long idx);
void   *rt_proc_call_open_det1(long idx, DESCR_t *a0);
void   *rt_proc_call_open_det2(long idx, DESCR_t *a0, DESCR_t *a1);
void   *rt_proc_call_open_det3(long idx, DESCR_t *a0, DESCR_t *a1, DESCR_t *a2);
void   *rt_proc_call_open_det4(long idx, DESCR_t *a0, DESCR_t *a1, DESCR_t *a2, DESCR_t *a3);
int     rt_proc_index_of(const char *name);
void   *rt_proc_open_fn(void);
void   *rt_frame_prep(void *fb, long fbytes);
DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
DESCR_t rt_proc_call_epilogue_ω(void);
DESCR_t rt_proc_call_epilogue_named_γ(const char *name);
DESCR_t rt_proc_call_epilogue_named_ω(const char *name);
DESCR_t rt_proc_call_epilogue_ret(DESCR_t fret);
DESCR_t rt_faildescr(void);
void    rt_ab_undef_fn_stub(void);
void    rt_pl_iso_throw_existence_key(const char *key);
DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **act_slot);
DESCR_t rt_proc_resume_frame(void *act);
DESCR_t rt_proc_resume_frame_h(void **hslot);
DESCR_t rt_gen_spine_pass_γ(DESCR_t v);
DESCR_t rt_gen_spine_pass_ω(void);
void rt_gen_spine_resume_enter(void);
int     zls_g_resume_by_name(const char *name);
int     zls_g_pl_zf_trail_mark_off_by_name(const char *name);
int     zls_g_pl_zf_trail_mark_off_by_name(const char *name);
int  rt_proc_is_generator(const char *name);
int rt_define_tiny_ok(const char *, int);
int rt_define_returns_by_frame(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_wire_pair_consumed(const char *fname) {
    static int _wpf = -1; if (_wpf < 0) { const char *e = getenv("SCRIP_WIRE_PAIR_FRAME"); _wpf = (e && *e == (char)48) ? 0 : 1; }
    if (!_wpf) return 1;
    return (fname && rt_define_returns_by_frame(fname)) ? 0 : 1;
}
void *rt_pl_cp_pop3(long *tm_lo, long *tm_hi);
void rt_pl_zf_resume_set(void *cursor, long tm_lo, long tm_hi, int tm_off, int cursor_off);
int  rt_proc_dyn_scope(const char *name);
void rt_arg_stage(int idx, DESCR_t v);
extern "C" DESCR_t g_call_args[];
extern "C" int g_gc_pending;
int  rt_proc_is_registered(const char *name);
long rt_proc_call_open_slim(const char *name, int np, int nargs);
int  rt_proc_nformals(const char *name);
void rt_c2b_arm_trap(void);
int  rt_pl_dc_ok(const char *name, int nargs);
void **rt_pl_dc_slot(long idx);
DESCR_t rt_proc_call_epilogue_slim_γ(DESCR_t result);
DESCR_t rt_proc_call_epilogue_slim_ω(void);
DESCR_t rt_nret_fix(DESCR_t r, int wn);
DESCR_t rt_nret_fix_tiny(DESCR_t r, int unused_edx);
int  rt_proc_nparams(const char *name);
const char *rt_proc_pname(const char *name, int k);
const char *rt_proc_result_name_get(const char *name);
int  scc_program_ok(void);
int  gva_index_of(const char *name);
extern int g_gva_active;
extern int g_monitor_bin;
int  bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_wire_stack_on(void) { static int _v = -1; if (_v < 0) { const char *e = getenv("SCRIP_ICN_WIRE_STACK"); _v = (e && *e == (char)48) ? 0 : 1; } return _v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_wire_cross(int gid, int wid) { return icn_wire_stack_on() ? bb_glue_pass_wires_blob(gid, wid) : bb_glue_pass_wires(gid, wid); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* bcps_spine_gen_arm's callee is always a zframe_graph prologue (xa_flat_zframe_prologue_str), which
 * captures its wires UNCONDITIONALLY from rcx/rdx registers -- it has no stack-blob landing of its own.
 * bb_glue_pass_wires_blob leaves rcx correctly loaded only as a side effect of using rcx as its lea
 * scratch register for the LAST push (gid); it never touches rdx at all, so the omega wire arrives as
 * whatever rt_proc_call_open_det/rt_arg_stage last left there. Gamma is masked by that coincidence;
 * omega is not -- confirmed via gdb: a multi-clause predicate that runs its clause chain to exhaustion
 * (any backtracking query) lands at its own omega port with [kt-16]=garbage and jmp's it -- measured as
 * fact$2F1_ω reading 0 at [rsp+496] and jmp'ing to address 0 (fact(a).fact(b).fact(c). main :- fact(X),
 * write(X), nl, fail ; true.). This is independent of SCRIP_PL_GAMMA_RETAIN -- both arms reach the same
 * omega port. Icon's icn_cells_graph generators are unaffected: their own epilogue consumes the pushed
 * pair directly off the stack (bb_glue_wire_γ/ω) rather than through this prologue's register capture,
 * so adding the register load here is inert for that case and only fills in the previously-missing wire
 * for the zframe_graph (Prolog) case that actually reads it. */
static std::string bcps_wire_cross_gen(int gid, int wid) {
    if (!icn_wire_stack_on()) return bb_glue_pass_wires(gid, wid);
    return x86_lea_id("rcx", wid) + x86("push", "rcx")
         + x86_lea_id("rcx", gid) + x86("push", "rcx")
         + x86_lea_id("rdx", wid)
         + x86_jmp_reg("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_wire_land(const char *fname) { return icn_wire_stack_on() ? IF(!bcps_wire_pair_consumed(fname), x86("add", "rsp", 16L)) : std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_retfix(void) { static int v = -1; if (v < 0) { const char *e = getenv("SCRIP_RET_FIX"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_epi_named(int is_omega, uint64_t bare_fp)
{
    uint64_t nm_fp; if (is_omega) { DESCR_t (*fp)(const char *) = rt_proc_call_epilogue_named_ω; nm_fp = (uint64_t)(uintptr_t)(void *)fp; }
    else               { DESCR_t (*fp)(const char *) = rt_proc_call_epilogue_named_γ; nm_fp = (uint64_t)(uintptr_t)(void *)fp; }
    if (!bcps_retfix()) return x86("call", is_omega ? "rt_proc_call_epilogue_ω" : "rt_proc_call_epilogue_γ", bare_fp);
    return x86_ro_load_q("rdi", 0) + x86("call", is_omega ? "rt_proc_call_epilogue_named_ω" : "rt_proc_call_epilogue_named_γ", nm_fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SAI_L0 200   /* stage_arg_inline owns internal labels 200..215 (i in [0,8)); the old base 20 put i==4 on L(29), which bcps_nret_consult also defines -- a duplicate .Lx<uid>_29 in one box */
static std::string stage_arg_inline(int i, int slot, uint64_t stage_fp) {
    bool plc = g_emit_cfg && g_emit_cfg->pl_cells_graph;
    std::string slow = x86("mov32", "edi", (long)i) + x86("mov", "rsi", FRQ(slot)) + x86("mov", "rdx", FRQ(slot + 8)) + x86("call", "rt_arg_stage", stage_fp);
    if (i < 0 || i >= 8 || getenv("SCRIP_NO_SINK")) return slow;
    return x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)&g_gc_pending, "g_gc_pending")
         + x86("mov", "eax", "dword ptr [r8 + 0]")
         + x86("test", "eax", "eax")
         + x86("jne", L(SAI_L0 + i * 2))
         + x86("mov", "rax", FRQ(slot))
         + x86("mov", "rdx", FRQ(slot + 8))
         + x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)g_call_args, "g_call_args")
         + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16) + "]").c_str(), "rax")
         + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16 + 8) + "]").c_str(), "rdx")
         + x86("jmp", L(SAI_L0 + 1 + i * 2))
         + x86("def", L(SAI_L0 + i * 2))
         + slow
         + x86("def", L(SAI_L0 + 1 + i * 2));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * bb_chain_terminal_staged(IR_t * entry) { IR_t * n = entry; int guard = 0;
    while (n && n->γ.node && n->γ.node->op != IR_SUCCEED && n->γ.node->op != IR_FAIL && guard++ < 4096) n = n->γ.node;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bb_label_t * bb_call_staged_beta_target() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i])
            return g_emit.xa_bb_emit_pair_jmp[i];
    return _.lbl_ω_p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_beta_pair_idx() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (g_emit.xa_bb_emit_pair_define[i] == _.lbl_β_p && g_emit.xa_bb_emit_pair_jmp[i]) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_arg_slot(IR_t * call, IR_graph_t ** argblks, int i) {
    IR_t * a = ir_call_arg(call, i);
    if (a) { int s = bb_slot_get(a); if (s < 0) s = zls_off(a); if (s >= 0) return s; }
    IR_t * prod = bb_chain_terminal_staged(argblks && argblks[i] ? argblks[i]->entry : NULL); int s = prod ? bb_slot_get(prod) : -1; return s < 0 ? 0 : s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_result_slot() {
    IR_t * nd = _.node;
    { int _s = nd ? zls_off(nd) : -1; if (_s >= 0) { if (bb_slot_get(nd) < 0) bb_slot_register(nd, _s); return _s; } }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline int c2farm() { return _.op_fc_wbytes > 0; }
static int bcps_fnsig(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_FN_SIG"); v = (e && *e == '0') ? 0 : 1; } return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_nret_consult(const std::string & r0, const std::string & r8) {
    extern int rt_g_ret_by_name;
    uint64_t fix_fp; { DESCR_t (*fp)(DESCR_t, int) = rt_nret_fix_tiny; fix_fp = (uint64_t)(uintptr_t)(void*)fp; }
    return x86("note", std::string("NRETURN by-name consult (live wn, consumed)"))
         + x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", "ecx", RDD("rcx", 0))
         + x86("cmp", "ecx", (long)0)
         + x86("je", L(29))
         + x86("mov", "rdi", "rax")
         + x86("mov", "rsi", "rdx")
         + x86("mov32", "edx", 0L)
         + x86_rtcc_call_descr_ops("rt_nret_fix_tiny", fix_fp, r0, r8)
         + x86("mov", "rax", r0.c_str())
         + x86("mov", "rdx", r8.c_str())
         + x86("def", L(29));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long bcps_sig_disp(int slot) {
    const char * t = FRQB(slot, 0); const char * p = strstr(t, "[rsp");
    if (!p) return -1;
    p += 4; if (*p == '#') p++;
    while (*p == ' ') p++;
    if (*p == ']') return 0;
    if (*p != '+') return -1;
    p++; while (*p == ' ') p++;
    if (*p < '0' || *p > '9') return -1;
    long v = 0; while (*p >= '0' && *p <= '9') v = v * 10 + (*p++ - '0');
    return (*p == ']') ? v : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long bcps_parse_rsp(const char * t) {
    const char * p = strstr(t, "[rsp");
    if (!p) return -1;
    p += 4; if (*p == '#') p++;
    while (*p == ' ') p++;
    if (*p == ']') return 0;
    if (*p != '+') return -1;
    p++; while (*p == ' ') p++;
    if (*p < '0' || *p > '9') return -1;
    long v = 0; while (*p >= '0' && *p <= '9') v = v * 10 + (*p++ - '0');
    return (*p == ']') ? v : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long bcps_zref_disp(int zoff) { return bcps_parse_rsp(x86_zref(zoff, 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int bb_scc_probe(const char *fname, int nargs, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out) {
    int np = 0, nsave = 0, res_gk = -1, scc = 0;
    if (fname && rt_proc_dyn_scope(fname) && !rt_proc_is_generator(fname) && !getenv("SCRIP_SCC_OFF") && (!g_monitor_bin || getenv("SCRIP_MON_SCC")) && g_gva_active && scc_program_ok() && rt_proc_is_registered(fname)) {
        np = rt_proc_nparams(fname);
        if (np >= 0 && np <= 60 && nargs <= rt_proc_nformals(fname)) {
            const char *rn = rt_proc_result_name_get(fname); int ok = rn ? 1 : 0, sh = 0;
            for (int k = 0; ok && k < np; k++) { const char *nm = rt_proc_pname(fname, k); int gk = nm ? gva_index_of(nm) : -1; if (gk < 0) ok = 0; else { gk_out[nsave++] = gk; if (!strcmp(nm, rn)) sh = 1; } }
            if (ok) { res_gk = gva_index_of(rn); if (res_gk < 0) ok = 0; else if (!sh) gk_out[nsave++] = res_gk; }
            if (ok) scc = 1;
        }
    }
    if (np_out) *np_out = np; if (nsave_out) *nsave_out = nsave; if (res_gk_out) *res_gk_out = res_gk; return scc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int bb_tiny_shim_ok(const char *fname, int nargs) {
    static int _nt = -1; if (_nt < 0) { const char *e = getenv("SCRIP_NO_TINY"); _nt = (e && *e == '1') ? 1 : 0; }
    if (_nt || !fname) return 0;
    if (!rt_define_tiny_ok(fname, nargs)) return 0;
    int np = 0, ns = 0, rg = -1; int gk[64];
    if (!bb_scc_probe(fname, 0, &np, &ns, gk, &rg)) return 0;
    int nf = rt_proc_nformals(fname);
    if (!(nf >= 0 && nf <= np && nf <= 29)) return 0;
    return 1;
}
#define BB_C2H_MAX 64
static struct { const void *call; char name[256]; } g_c2h_tab[BB_C2H_MAX]; static int g_c2h_n;
static struct { const void *call; const char *name; int live; } g_c2h_pend;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_scc_handoff_pending_set(const void *call_nd, const char *fname) { g_c2h_pend.call = call_nd; g_c2h_pend.name = fname; g_c2h_pend.live = call_nd && fname ? 1 : 0; }
extern "C" void bb_scc_handoff_pending_clear(void) { g_c2h_pend.live = 0; }
extern "C" int  bb_scc_handoff_arm(void) { if (!g_c2h_pend.live || g_c2h_n >= BB_C2H_MAX) return 0; g_c2h_tab[g_c2h_n].call = g_c2h_pend.call; snprintf(g_c2h_tab[g_c2h_n].name, sizeof g_c2h_tab[g_c2h_n].name, "%s", g_c2h_pend.name); g_c2h_n++; g_c2h_pend.live = 0; return 1; }
extern "C" int  bb_scc_handoff_room(void) { return g_c2h_pend.live && g_c2h_n < BB_C2H_MAX; }
extern "C" int  bb_scc_handoff_consume(const void *call_nd, const char *fname) { for (int i = 0; i < g_c2h_n; i++) if (g_c2h_tab[i].call == call_nd) { int ok = fname && !strcmp(g_c2h_tab[i].name, fname) ? 1 : 0; g_c2h_tab[i] = g_c2h_tab[--g_c2h_n]; return ok ? 1 : -1; } return 0; }
extern "C" void bb_scc_handoff_reset(void) { g_c2h_n = 0; g_c2h_pend.live = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static bool bcps_is_pl_pi(const char * s) {
    if (!s || s[0] == '$') return false;
    const char * sl = strrchr(s, '/');
    if (!sl || sl == s || !sl[1]) return false;
    for (const char * d = sl + 1; *d; d++) if (*d < '0' || *d > '9') return false;
    return true;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_undef_fallback(uint64_t undef_fp) {
    if (bcps_is_pl_pi(_.op_sval)) {
        uint64_t pl_fp; { void (*fp)(const char *) = rt_pl_iso_throw_existence_key; pl_fp = (uint64_t)(uintptr_t)(void*)fp; }
        return x86_ro_load_q("rdi", 0) + x86("call", "rt_pl_iso_throw_existence_key", pl_fp) + x86_omega();
    }
    return x86("call", "rt_ab_undef_fn_stub", undef_fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_det_arm() {
    x86_begin();
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    if (_.op_zres) {
        int bidx_z = bcps_beta_pair_idx(); IR_graph_t ** argblks_z = (IR_graph_t **)(intptr_t)_.op_counter;
        int is_dyn_z = _.op_sval && rt_proc_dyn_scope(_.op_sval);
        long det_idx_z = (!is_dyn_z && _.op_sval) ? (long)rt_proc_index_of(_.op_sval) : -1L;
        int det_nA_z = (int)_.op_ival;
        int det_fuse_z = (det_idx_z >= 0 && det_nA_z >= 0 && det_nA_z <= 4);
        int dc_z = 0; uint64_t dc_slot_z = 0; char dc_name_z[280]; dc_name_z[0] = 0;
        if (det_fuse_z && _.op_sval && rt_pl_dc_ok(_.op_sval, det_nA_z)) {
            void **sl = rt_pl_dc_slot(det_idx_z); if (sl) { dc_z = 1; dc_slot_z = (uint64_t)(uintptr_t)sl;
                { char mang[256]; int mi = 0; const char *nm = _.op_sval; for (; *nm && mi < 250; nm++) { unsigned char u = (unsigned char)*nm; if ((u>='A'&&u<='Z')||(u>='a'&&u<='z')||(u>='0'&&u<='9')||u=='_'||u=='$'||u=='.') mang[mi++]=(char)u; else mi+=snprintf(mang+mi,(size_t)(256-mi),"$%02X",u); } mang[mi]=0; snprintf(dc_name_z,sizeof dc_name_z,"%s_dc\xce\xb1",mang); } } }
        static const char *detN_argreg_z[4] = { "rsi", "rdx", "rcx", "r8" };
        uint64_t detN_fp_z[5];
        { void *(*f0)(long) = rt_proc_call_open_det0; detN_fp_z[0] = (uint64_t)(uintptr_t)(void*)f0; }
        { void *(*f1)(long, DESCR_t*) = rt_proc_call_open_det1; detN_fp_z[1] = (uint64_t)(uintptr_t)(void*)f1; }
        { void *(*f2)(long, DESCR_t*, DESCR_t*) = rt_proc_call_open_det2; detN_fp_z[2] = (uint64_t)(uintptr_t)(void*)f2; }
        { void *(*f3)(long, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det3; detN_fp_z[3] = (uint64_t)(uintptr_t)(void*)f3; }
        { void *(*f4)(long, DESCR_t*, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det4; detN_fp_z[4] = (uint64_t)(uintptr_t)(void*)f4; }
        static const char *detN_nm_z[5] = { "rt_proc_call_open_det0","rt_proc_call_open_det1","rt_proc_call_open_det2","rt_proc_call_open_det3","rt_proc_call_open_det4" };
        uint64_t open_fp_z;  { long (*fp)(const char *, int) = rt_proc_call_open; open_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t openfn_fp_z; { void * (*fp)(void) = rt_proc_open_fn; openfn_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t procfn_fp_z; { void * (*fp)(const char *) = rt_proc_fn; procfn_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t epig_fp_z;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_γ; epig_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t epiw_fp_z;  { DESCR_t (*fp)(void) = rt_proc_call_epilogue_ω; epiw_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t undef_fp_z;  { void (*fp)(void) = rt_ab_undef_fn_stub; undef_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        uint64_t det_fp_z; { void * (*fp)(long, int) = rt_proc_call_open_det; det_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        int scc_z = 0, scc_np_z = 0, scc_nsave_z = 0, scc_res_gk_z = -1; int scc_gk_z[64];
        scc_z = bb_scc_probe(_.op_sval, (int)_.op_ival, &scc_np_z, &scc_nsave_z, scc_gk_z, &scc_res_gk_z);
        long scc_sb_z = 16L * (long)scc_nsave_z;
        uint64_t trap_fp_z; { void (*fp)(void) = rt_c2b_arm_trap; trap_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        long scc_fp_oz; { long (*fp)(const char *, int, int) = rt_proc_call_open_slim; scc_fp_oz = (long)(uint64_t)(uintptr_t)(void*)fp; }
        long scc_fp_gz; { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_slim_γ; scc_fp_gz = (long)(uint64_t)(uintptr_t)(void*)fp; }
        long scc_fp_wz; { DESCR_t (*fp)(void) = rt_proc_call_epilogue_slim_ω; scc_fp_wz = (long)(uint64_t)(uintptr_t)(void*)fp; }
        uint64_t dc_slot_fp_z = dc_slot_z;
        uint64_t stage_fp_z; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp_z = (uint64_t)(uintptr_t)(void*)fp; }
        return x86_alpha()
             + x86_scan_sync_out()
             + x86_anchor_enter()
             + ((scc_z || (_.op_sval && bb_tiny_shim_ok(_.op_sval, (int)_.op_ival)))
                ? [&]() -> std::string {
                    static int _ntz = -1; if (_ntz < 0) { const char * _e = getenv("SCRIP_NO_TINY"); _ntz = (_e && *_e == '1') ? 1 : 0; }
                    static int _b1cz = -1; if (_b1cz < 0) { const char * _e = getenv("SCRIP_B1C_PARITY"); _b1cz = (_e && *_e == '0') ? 0 : 1; }
                    if (!_ntz && ({ extern int g_rt_fragment_emit; !g_rt_fragment_emit || _b1cz; }) && _.op_sval && bb_tiny_shim_ok(_.op_sval, (int)_.op_ival)) {
                        std::string laz = std::string(_.op_sval) + "_\xce\xb1";
                        if (bcps_fnsig()) {
                            long soffz[29]; int sigokz = ((long)_.op_ival <= 29);
                            for (int i = 0; sigokz && i < (int)_.op_ival; i++) {
                                int zs = _.op_zread[i];
                                if (x86_fc_hit(zs) || x86_fc_hit(zs + 8)) { sigokz = 0; break; }
                                long dlo = bcps_zref_disp(zs), dhi = bcps_zref_disp(zs + 8);
                                if (dlo < 0 || dhi != dlo + 8) { sigokz = 0; break; }
                                soffz[i] = dlo; }
                            if (sigokz) {
                                std::string snmz = std::string(".L") + x86_boxkind() + "_sig" + std::to_string((long)_.x86_uid) + "z";
                                const struct bb_label_t * sigl_z = emit_label_intern(snmz.c_str());
                                std::string sz = x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)sigl_z)
                                     + x86("jmp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()), laz.c_str())
                                     + x86_def_ext(sigl_z)
                                     + x86(".quad", (uint64_t)_.op_ival)
                                     + x86(".quad", L(2))
                                     + x86(".quad", L(2));
                                for (int i = 0; i < (int)_.op_ival; i++) sz += x86(".quad", (uint64_t)soffz[i]);
                                return sz;
                            }
                        } else {
                        long Kbz = 16L * (long)_.op_ival + 32;
                        auto ZOPQT = [&](int i, int w) { return x86_zref(_.op_zread[i] + w + (int)Kbz, 1); };
                        return x86("sub", "rsp", Kbz)
                             + FOR(0, (int)_.op_ival, [&](int i) {
                                   return x86("note", ZOPN(i)) + x86("mov", "rax", ZOPQT(i, 0)) + x86_rsp_store64(32 + 16 * i, "rax")
                                        + x86("note", ZOPN(i)) + x86("mov", "rax", ZOPQT(i, 8)) + x86_rsp_store64(32 + 16 * i + 8, "rax"); })
                             + x86("mov32", "eax", (long)_.op_ival) + x86_rsp_store64(0, "rax")
                             + x86("lea", "rax", L(2)) + x86_rsp_store64(16, "rax") + x86_rsp_store64(24, "rax")
                             + x86("jmp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()), laz.c_str());
                        }
                    }
                    if (!scc_z) return std::string();
                    _tiny_fallback_z: ;
                    bool ab3b_z = (g_emit_cfg && g_emit_cfg->ab_n > 0 && _.op_sval);
                    if (ab3b_z) {
                        std::string fn_cell_lbl_z = std::string("fn_cell$") + _.op_sval;
                        void * fn_cell_bin_z = bb_ab_fn_cell_ptr(_.op_sval);
                        auto ZOPQC = [&](int i, int w) { return x86_zref(_.op_zread[i] + w + (int)scc_sb_z, 1); };
                        auto ab_formals_restore = [&]() { return FOR(0, scc_np_z, [&](int i) {
                              return x86("note", gva_name(scc_gk_z[i]))
                                   + x86_rsp_load64("rcx", 16 * i)     + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[i] * 16),     "rcx")
                                   + x86_rsp_load64("rcx", 16 * i + 8) + x86("mov", ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[i] * 16 + 8), "rcx"); }); };
                        return x86("sub", "rsp", scc_sb_z)
                            + FOR(0, scc_nsave_z, [&](int k) {
                                  return x86("note", gva_name(scc_gk_z[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                                       + x86("note", gva_name(scc_gk_z[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
                            + FOR(0, (int)_.op_ival, [&](int i) {
                                  int gk_iz = scc_gk_z[i];
                                  return x86("note", ZOPN(i)) + x86("mov", "rax", ZOPQC(i, 0))
                                       + x86("note", gva_name(gk_iz)) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk_iz, 0) : ABSQ(RT_GVA_VA + (unsigned long)gk_iz * 16), "rax")
                                       + x86("note", ZOPN(i)) + x86("mov", "rax", ZOPQC(i, 8))
                                       + x86("note", gva_name(gk_iz)) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk_iz, 8) : ABSQ(RT_GVA_VA + (unsigned long)gk_iz * 16 + 8), "rax"); })
                            + x86("lea", "rcx", L(10))
                            + x86("lea", "rdx", L(11))
                            + x86("jmp_fn_cell", fn_cell_lbl_z.c_str(), (uint64_t)(uintptr_t)fn_cell_bin_z)
                            + x86("def", L(10)) + ab_formals_restore() + x86("add", "rsp", scc_sb_z) + x86("jmp", L(2))
                            + x86("def", L(11)) + ab_formals_restore() + x86("add", "rsp", scc_sb_z) + x86("jmp", L(2));
                    }
                    return FOR(0, (int)_.op_ival, [&](int i) {
                               return x86("mov32", "edi", (long)i) + x86("note", ZOPN(i)) + x86("mov", "rsi", ZOPQ(i, 0)) + x86("note", ZOPN(i)) + x86("mov", "rdx", ZOPQ(i, 8)) + x86("call", "rt_arg_stage", stage_fp_z); })
                        + x86("sub", "rsp", scc_sb_z)
                        + FOR(0, scc_nsave_z, [&](int k) {
                              return x86("note", gva_name(scc_gk_z[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                                   + x86("note", gva_name(scc_gk_z[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
                        + x86_ro_load_q("rdi", 0)
                        + x86("mov32", "esi", (long)scc_np_z)
                        + x86("mov32", "edx", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open_slim", (uint64_t)scc_fp_oz)
                        + x86("test", "rax", "rax")
                        + x86("je", L(5))
                        + x86("mov", "rdx", "rax")
                        + FOR(0, (int)_.op_ival, [&](int i) {
                              return x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)g_call_args, "g_call_args")
                                   + x86("mov", "rax", (std::string("[r8 + ") + std::to_string(i * 16) + "]").c_str())
                                   + x86("note", gva_name(scc_gk_z[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[i], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[i] * 16), "rax")
                                   + x86("mov", "rax", (std::string("[r8 + ") + std::to_string(i * 16 + 8) + "]").c_str())
                                   + x86("note", gva_name(scc_gk_z[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[i], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[i] * 16 + 8), "rax"); })
                        + x86("mov", "rax", "rdx")
                        + [&]{ static int _spz = -1; if (_spz < 0) { const char *e = getenv("SCRIP_SLIM_PAIR"); _spz = (!e || *e != (char)48) ? 1 : 0; } return _spz ? x86("note", "s110 floater pair (ZD twin): push omega then gamma so the fnrbp2 RETURN/FRETURN floaters find {gamma,omega} AT TOS; floater consumes 16 so L(6)/L(7) arrive at today's depth; witness probe/mon/mon_define_call_min; SCRIP_SLIM_PAIR=0 restores prior bytes") + x86("lea", "rcx", L(7)) + x86("push", "rcx") + x86("lea", "rcx", L(6)) + x86("push", "rcx") : std::string(""); }()
                        + bb_glue_pass_wires_blob(6, 7)
                        + x86("def", L(6))
                        + bb_glue_wire_land()
                        + x86("note", gva_name((scc_res_gk_z < 0 ? 0 : scc_res_gk_z))) + x86("mov", "rdi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk_z < 0 ? 0 : scc_res_gk_z), 0) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk_z < 0 ? 0 : scc_res_gk_z) * 16))
                        + x86("note", gva_name((scc_res_gk_z < 0 ? 0 : scc_res_gk_z))) + x86("mov", "rsi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk_z < 0 ? 0 : scc_res_gk_z), 8) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk_z < 0 ? 0 : scc_res_gk_z) * 16 + 8))
                        + FOR(0, scc_nsave_z, [&](int j) { int k = scc_nsave_z - 1 - j;
                              return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk_z[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16), "rax")
                                   + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk_z[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16 + 8), "rax"); })
                        + x86("add", "rsp", scc_sb_z)
                        + x86("call", "rt_proc_call_epilogue_slim_γ", (uint64_t)scc_fp_gz)
                        + x86("jmp", L(2))
                        + x86("def", L(7))
                        + bb_glue_wire_land()
                        + FOR(0, scc_nsave_z, [&](int j) { int k = scc_nsave_z - 1 - j;
                              return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk_z[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16), "rax")
                                   + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk_z[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk_z[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk_z[k] * 16 + 8), "rax"); })
                        + x86("add", "rsp", scc_sb_z)
                        + x86("call", "rt_proc_call_epilogue_slim_ω", (uint64_t)scc_fp_wz)
                        + x86("jmp", L(2))
                        + x86("def", L(5))
                        + x86("add", "rsp", scc_sb_z);
                }()
                : std::string(""))
             + (!scc_z && dc_z
                ? FOR(0, det_nA_z, [&](int i) { return x86("note", ZOPN(i)) + x86("lea", detN_argreg_z[i], ZOPQ(i, 0)); })
                + x86_call_dc(dc_name_z, dc_slot_fp_z)
                + x86("jmp", L(2))
                : std::string(""))
             + (!scc_z && !dc_z
                ? ((det_fuse_z
                    ? x86("mov32", "edi", det_idx_z)
                    + FOR(0, det_nA_z, [&](int i) { return x86("note", ZOPN(i)) + x86("lea", detN_argreg_z[i], ZOPQ(i, 0)); })
                    + x86("call", detN_nm_z[det_nA_z], detN_fp_z[det_nA_z])
                    : ((det_idx_z >= 0
                        ? x86("mov32", "edi", (long)det_idx_z)
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open_det", (uint64_t)det_fp_z)
                        : FOR(0, (int)_.op_ival, [&](int i) { uint64_t stage_fp_z; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp_z = (uint64_t)(uintptr_t)(void*)fp; } return x86("mov32", "edi", (long)i) + x86("note", ZOPN(i)) + x86("mov", "rsi", ZOPQ(i, 0)) + x86("note", ZOPN(i)) + x86("mov", "rdx", ZOPQ(i, 8)) + x86("call", "rt_arg_stage", stage_fp_z); })
                        + x86_ro_load_q("rdi", 0)
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open", open_fp_z))))
                   + x86("test", "rax", "rax")
                   + x86("je", L(1))
                   + (det_idx_z >= 0 && det_fuse_z ? std::string("") : x86_ro_load_q("rdi", 0) + x86("call", "rt_proc_fn", procfn_fp_z))
                   + [&]{ static int _sp4 = -1; if (_sp4 < 0) { const char *e = getenv("SCRIP_SLIM_PAIR"); _sp4 = (!e || *e != (char)48) ? 1 : 0; } return (!icn_wire_stack_on() && _sp4 && bcps_wire_pair_consumed(_.op_sval)) ? x86("note", "s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_ICN_WIRE_STACK=0 restores prior bytes.") + x86("lea", "rcx", L(4)) + x86("push", "rcx") + x86("lea", "rcx", L(3)) + x86("push", "rcx") : std::string(""); }()
                   + bcps_wire_cross(3, 4)
                   + x86("def", L(3))
                   + bcps_wire_land(_.op_sval)
                   + bcps_epi_named(0, epig_fp_z)
                   + x86("jmp", L(2))
                   + x86("def", L(4))
                   + bcps_wire_land(_.op_sval)
                   + bcps_epi_named(1, epiw_fp_z)
                   + x86("jmp", L(2))
                   + x86("def", L(1))
                   + bcps_undef_fallback(undef_fp_z))
                : std::string(""))
             + x86("def", L(2))
             + x86_anchor_leave()
             + x86_scan_sync_in_rr()
             + bcps_nret_consult(std::string(ZRES(0)), std::string(ZRES(8)))
             + x86("note", ZRESN()) + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx")
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86_gamma()
             + x86_beta()
             + (bidx_z < 0 ? x86_omega() : x86_pair_jmp(bidx_z))
             + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
    }
    int bidx = bcps_beta_pair_idx(); IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    uint64_t stage_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t open_fp;  { long (*fp)(const char *, int) = rt_proc_call_open; open_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t openfn_fp; { void * (*fp)(void) = rt_proc_open_fn; openfn_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t procfn_fp; { void * (*fp)(const char *) = rt_proc_fn; procfn_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t prep_fp;  { void * (*fp)(void *, long) = rt_frame_prep; prep_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epig_fp;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_γ; epig_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epiw_fp;  { DESCR_t (*fp)(void) = rt_proc_call_epilogue_ω; epiw_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epir_fp;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_ret; epir_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t undef_fp;  { void (*fp)(void) = rt_ab_undef_fn_stub; undef_fp = (uint64_t)(uintptr_t)(void*)fp; }
    int is_dyn = _.op_sval && rt_proc_dyn_scope(_.op_sval);
    uint64_t det_fp; { void * (*fp)(long, int) = rt_proc_call_open_det; det_fp = (uint64_t)(uintptr_t)(void*)fp; }
    long det_idx = (!is_dyn && _.op_sval) ? (long)rt_proc_index_of(_.op_sval) : -1L;
    uint64_t detN_fp[5]; static const char *detN_nm[5] = { "rt_proc_call_open_det0", "rt_proc_call_open_det1", "rt_proc_call_open_det2", "rt_proc_call_open_det3", "rt_proc_call_open_det4" };
    { void *(*f0)(long) = rt_proc_call_open_det0; detN_fp[0] = (uint64_t)(uintptr_t)(void*)f0; }
    { void *(*f1)(long, DESCR_t*) = rt_proc_call_open_det1; detN_fp[1] = (uint64_t)(uintptr_t)(void*)f1; }
    { void *(*f2)(long, DESCR_t*, DESCR_t*) = rt_proc_call_open_det2; detN_fp[2] = (uint64_t)(uintptr_t)(void*)f2; }
    { void *(*f3)(long, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det3; detN_fp[3] = (uint64_t)(uintptr_t)(void*)f3; }
    { void *(*f4)(long, DESCR_t*, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det4; detN_fp[4] = (uint64_t)(uintptr_t)(void*)f4; }
    static const char *detN_argreg[4] = { "rsi", "rdx", "rcx", "r8" };
    int det_nA = (int)_.op_ival; int det_fuse = (det_idx >= 0 && det_nA >= 0 && det_nA <= 4);
    int dc = (det_fuse && _.op_sval && rt_pl_dc_ok(_.op_sval, det_nA));
    uint64_t dc_slot = 0; char dc_name[280]; dc_name[0] = 0;
    if (dc) { void **sl = rt_pl_dc_slot(det_idx); if (!sl) dc = 0; else { dc_slot = (uint64_t)(uintptr_t)sl;
        { char mang[256]; int mi = 0; const char *nm = _.op_sval; for (; *nm && mi < 250; nm++) { unsigned char u = (unsigned char) *nm; if ((u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z') || (u >= '0' && u <= '9') || u == '_' || u == '$' || u == '.') mang[mi++] = (char) u; else mi += snprintf(mang + mi, (size_t) (256 - mi), "$%02X", u); } mang[mi] = 0; snprintf(dc_name, sizeof dc_name, "%s_dc\xce\xb1", mang); } } }
    long scc_fp_o; { long (*fp)(const char *, int, int) = rt_proc_call_open_slim; scc_fp_o = (long)(uint64_t)(uintptr_t)(void*)fp; }
    uint64_t trap_fp; { void (*fp)(void) = rt_c2b_arm_trap; trap_fp = (uint64_t)(uintptr_t)(void*)fp; }
    long scc_fp_g; { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_slim_γ; scc_fp_g = (long)(uint64_t)(uintptr_t)(void*)fp; }
    long scc_fp_w; { DESCR_t (*fp)(void) = rt_proc_call_epilogue_slim_ω; scc_fp_w = (long)(uint64_t)(uintptr_t)(void*)fp; }
    int scc = 0, scc_np = 0, scc_nsave = 0, scc_res_gk = -1; int scc_gk[64];
    scc = bb_scc_probe(_.op_sval, (int)_.op_ival, &scc_np, &scc_nsave, scc_gk, &scc_res_gk);
    int c2 = _.op_c2 == 1 ? 1 : 0;
    { static int _td=-1; if(_td<0)_td=getenv("SCRIP_TINY_DIAG")?1:0; if(_td) fprintf(stderr,"[TINYX] fn=%s nargs=%ld scc=%d c2=%d\n", _.op_sval?_.op_sval:"?",(long)_.op_ival,scc,c2); }
    if (_.op_c2 < 0) return x86_alpha() + x86_bomb("bb_call_proc_staged: CALL2BB handoff callee-name mismatch for this exact call node (producer/consumer drift)");
    if (c2 && !scc)  return x86_alpha() + x86_bomb("bb_call_proc_staged: CALL2BB consumer probe disagrees with the role-0 producer that armed for this node (structural drift — bb_scc_probe is supposed to make this impossible)");
    if (c2farm() && (!scc || (int)_.op_ival != 1)) return x86_alpha() + x86_bomb("bb_call_proc_staged: fc-armed call without SCC 1-arg shape (CALL2BB 3b v1) — the flat fallback does not exist as storage on an armed statement; registration and the probe disagreed");
    long scc_sb = 16L * (long)scc_nsave;
    return x86_alpha()
         + (c2 ? IF(g_scan_regs_live, x86("push", "rax") + x86("push", "rax")) + x86_scan_sync_out() + IF(g_scan_regs_live, x86("pop", "rax") + x86("pop", "rax")) : x86_scan_sync_out())
         + x86_anchor_enter()
         + (c2
            ? x86("test", "rax", "rax")
            + x86("je", L(5))
            + bb_glue_pass_wires_blob(6, 7)
            + x86("def", L(6))
            + bb_glue_wire_land()
            + x86("note", gva_name((scc_res_gk < 0 ? 0 : scc_res_gk))) + x86("mov", "rdi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk < 0 ? 0 : scc_res_gk), 0) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk < 0 ? 0 : scc_res_gk) * 16))
            + x86("note", gva_name((scc_res_gk < 0 ? 0 : scc_res_gk))) + x86("mov", "rsi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk < 0 ? 0 : scc_res_gk), 8) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk < 0 ? 0 : scc_res_gk) * 16 + 8))
            + FOR(0, scc_nsave, [&](int j) { int k = scc_nsave - 1 - j;
                  return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16), "rax")
                       + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8), "rax"); })
            + x86("add", "rsp", scc_sb)
            + x86("call", "rt_proc_call_epilogue_slim_γ", (uint64_t)scc_fp_g)
            + x86("jmp", L(2))
            + x86("def", L(7))
            + bb_glue_wire_land()
            + FOR(0, scc_nsave, [&](int j) { int k = scc_nsave - 1 - j;
                  return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16), "rax")
                       + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8), "rax"); })
            + x86("add", "rsp", scc_sb)
            + x86("call", "rt_proc_call_epilogue_slim_ω", (uint64_t)scc_fp_w)
            + x86("jmp", L(2))
            + x86("def", L(5))
            + IF(c2farm(), x86("call", "rt_c2b_arm_trap", trap_fp))
            : std::string(""))
         + ((scc || (_.op_sval && bb_tiny_shim_ok(_.op_sval, (int)_.op_ival))) && !c2
            ? [&]() -> std::string {
                static int _ntiny = -1; if (_ntiny < 0) { const char * _e = getenv("SCRIP_NO_TINY"); _ntiny = (_e && *_e == '1') ? 1 : 0; }
                { static int _td=-1; if(_td<0)_td=getenv("SCRIP_TINY_DIAG")?1:0; if(_td) fprintf(stderr,"[TINY] fn=%s nargs=%ld ok=%d scc=%d c2=%d\n", _.op_sval?_.op_sval:"?",(long)_.op_ival,_.op_sval?rt_define_tiny_ok(_.op_sval,(int)_.op_ival):-1,scc,c2); }
                static int _b1ct = -1; if (_b1ct < 0) { const char * _e = getenv("SCRIP_B1C_PARITY"); _b1ct = (_e && *_e == '0') ? 0 : 1; }
                if (!_ntiny && ({ extern int g_rt_fragment_emit; !g_rt_fragment_emit || _b1ct; }) && _.op_sval && bb_tiny_shim_ok(_.op_sval, (int)_.op_ival)) {
                    std::string la = std::string(_.op_sval) + "_\xce\xb1";
                    if (bcps_fnsig()) {
                        long soff[29]; int sigok = ((long)_.op_ival <= 29);
                        for (int i = 0; sigok && i < (int)_.op_ival; i++) {
                            int slot = bcps_arg_slot(_.node, argblks, i);
                            if (x86_fc_hit(slot) || x86_fc_hit(slot + 8)) { sigok = 0; break; }
                            long dlo = bcps_sig_disp(slot), dhi = bcps_sig_disp(slot + 8);
                            if (dlo < 0 || dhi != dlo + 8) { sigok = 0; break; }
                            soff[i] = dlo; }
                        if (sigok) {
                            std::string snm = std::string(".L") + x86_boxkind() + "_sig" + std::to_string((long)_.x86_uid);
                            const struct bb_label_t * sigl = emit_label_intern(snm.c_str());
                            std::string s = x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)sigl)
                                 + x86("jmp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()), la.c_str())
                                 + x86_def_ext(sigl)
                                 + x86(".quad", (uint64_t)_.op_ival)
                                 + x86(".quad", L(2))
                                 + x86(".quad", L(2));
                            for (int i = 0; i < (int)_.op_ival; i++) s += x86(".quad", (uint64_t)soff[i]);
                            return s;
                        }
                    } else {
                    long Kb = 16L * (long)_.op_ival + 32;
                    return x86("sub", "rsp", Kb)
                         + FOR(0, (int)_.op_ival, [&](int i) { int slot = bcps_arg_slot(_.node, argblks, i);
                               return (x86_fc_hit(slot) ? x86_rsp_load64("rax", slot - _.op_fc_base + (int)Kb) : x86("mov", "rax", FRQB(slot, (int)Kb)))
                                    + x86_rsp_store64(32 + 16 * i, "rax")
                                    + (x86_fc_hit(slot + 8) ? x86_rsp_load64("rax", slot + 8 - _.op_fc_base + (int)Kb) : x86("mov", "rax", FRQB(slot + 8, (int)Kb)))
                                    + x86_rsp_store64(32 + 16 * i + 8, "rax"); })
                         + x86("mov32", "eax", (long)_.op_ival) + x86_rsp_store64(0, "rax")
                         + x86("lea", "rax", L(2)) + x86_rsp_store64(16, "rax") + x86_rsp_store64(24, "rax")
                         + x86("jmp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()), la.c_str());
                    }
                }
                if (!scc) return std::string();
                bool ab3b = (g_emit_cfg && g_emit_cfg->ab_n > 0 && _.op_sval);
                if (ab3b) {
                    std::string fn_cell_lbl = std::string("fn_cell$") + _.op_sval;
                    void * fn_cell_bin = bb_ab_fn_cell_ptr(_.op_sval);
                    return x86("sub", "rsp", scc_sb)
                        + FOR(0, scc_nsave, [&](int k) {
                              return x86("note", gva_name(scc_gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                                   + x86("note", gva_name(scc_gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
                        + FOR(0, (int)_.op_ival, [&](int i) {
                              int slot = bcps_arg_slot(_.node, argblks, i);
                              int gk_i = scc_gk[i < scc_np ? i + 1 : i];
                              return (x86_fc_hit(slot) ? x86_rsp_load64("rax", slot - _.op_fc_base + (int)scc_sb) : x86("mov", "rax", FRQB(slot, (int)scc_sb)))
                                   + x86("note", gva_name(gk_i)) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk_i, 0) : ABSQ(RT_GVA_VA + (unsigned long)gk_i * 16), "rax")
                                   + (x86_fc_hit(slot + 8) ? x86_rsp_load64("rax", slot + 8 - _.op_fc_base + (int)scc_sb) : x86("mov", "rax", FRQB(slot + 8, (int)scc_sb)))
                                   + x86("note", gva_name(gk_i)) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(gk_i, 8) : ABSQ(RT_GVA_VA + (unsigned long)gk_i * 16 + 8), "rax"); })
                        + x86("lea", "rcx", L(8))
                        + x86("lea", "rdx", L(9))
                        + x86("jmp_fn_cell", fn_cell_lbl.c_str(), (uint64_t)(uintptr_t)fn_cell_bin)
                        + x86("def", L(8)) + x86("add", "rsp", scc_sb) + x86("jmp", L(2))
                        + x86("def", L(9)) + x86("add", "rsp", scc_sb) + x86("jmp", L(2));
                }
                return x86("sub", "rsp", scc_sb)
                    + FOR(0, scc_nsave, [&](int k) {
                          return x86("note", gva_name(scc_gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                               + x86("note", gva_name(scc_gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
                    + x86_ro_load_q("rdi", 0)
                    + x86("mov32", "esi", (long)scc_np)
                    + x86("mov32", "edx", (long)_.op_ival)
                    + x86("call", "rt_proc_call_open_slim", (uint64_t)scc_fp_o)
                    + x86("test", "rax", "rax")
                    + x86("je", L(5))
                    + x86("mov", "rdx", "rax")
                    + FOR(0, (int)_.op_ival, [&](int i) {
                          int slot = bcps_arg_slot(_.node, argblks, i);
                          return (c2farm() ? x86_rsp_load64("rax", (int)scc_sb) : x86_fc_hit(slot) ? x86_rsp_load64("rax", slot - _.op_fc_base + (int)scc_sb) : x86("mov", "rax", FRQB(slot, (int)scc_sb)))
                               + x86("note", gva_name(scc_gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[i], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[i] * 16), "rax")
                               + (c2farm() ? x86_rsp_load64("rax", (int)scc_sb + 8) : x86_fc_hit(slot + 8) ? x86_rsp_load64("rax", slot + 8 - _.op_fc_base + (int)scc_sb) : x86("mov", "rax", FRQB(slot + 8, (int)scc_sb)))
                               + x86("note", gva_name(scc_gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[i], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[i] * 16 + 8), "rax"); })
                    + x86("mov", "rax", "rdx")
                    + [&]{ static int _sp = -1; if (_sp < 0) { const char *e = getenv("SCRIP_SLIM_PAIR"); _sp = (!e || *e != (char)48) ? 1 : 0; } return _sp ? x86("note", "s110 floater pair: fnrbp2 RETURN/FRETURN floaters pop {gamma,omega} AT TOS (bb_define role-1/2 s64 arm); this non-TINY site pushed NOTHING, so :(RETURN) popped enclosing-frame bytes and jumped junk (rip=_rtld_global, the omega_driver signature; witness probe/mon/mon_define_call_min).  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the floater consumes 16 so L(6)/L(7) arrive at today's post-carve depth unchanged.  Wires below stay seated for blob-exit spellings.  SCRIP_SLIM_PAIR=0 restores prior bytes.") + x86("lea", "rcx", L(7)) + x86("push", "rcx") + x86("lea", "rcx", L(6)) + x86("push", "rcx") : std::string(""); }()
                    + bb_glue_pass_wires_blob(6, 7)
                    + x86("def", L(6))
                    + bb_glue_wire_land()
                    + x86("note", gva_name((scc_res_gk < 0 ? 0 : scc_res_gk))) + x86("mov", "rdi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk < 0 ? 0 : scc_res_gk), 0) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk < 0 ? 0 : scc_res_gk) * 16))
                    + x86("note", gva_name((scc_res_gk < 0 ? 0 : scc_res_gk))) + x86("mov", "rsi", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ((scc_res_gk < 0 ? 0 : scc_res_gk), 8) : ABSQ(RT_GVA_VA + (unsigned long)(scc_res_gk < 0 ? 0 : scc_res_gk) * 16 + 8))
                    + FOR(0, scc_nsave, [&](int j) { int k = scc_nsave - 1 - j;
                          return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16), "rax")
                               + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8), "rax"); })
                    + x86("add", "rsp", scc_sb)
                    + x86("call", "rt_proc_call_epilogue_slim_γ", (uint64_t)scc_fp_g)
                    + x86("jmp", L(2))
                    + x86("def", L(7))
                    + bb_glue_wire_land()
                    + FOR(0, scc_nsave, [&](int j) { int k = scc_nsave - 1 - j;
                          return x86_rsp_load64("rax", 16 * k) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16), "rax")
                               + x86_rsp_load64("rax", 16 * k + 8) + x86("note", gva_name(scc_gk[k])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(scc_gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)scc_gk[k] * 16 + 8), "rax"); })
                    + x86("add", "rsp", scc_sb)
                    + x86("call", "rt_proc_call_epilogue_slim_ω", (uint64_t)scc_fp_w)
                    + x86("jmp", L(2))
                    + x86("def", L(5))
                    + x86("add", "rsp", scc_sb)
                    + IF(c2farm(), x86("call", "rt_c2b_arm_trap", trap_fp));
              }()
            : std::string(""))
         + (dc
            ? FOR(0, det_nA, [&](int i) { int slot = bcps_arg_slot(_.node, argblks, i); return x86("lea", detN_argreg[i], FRQ(slot)); })
            + x86_call_dc(dc_name, dc_slot)
            + x86("jmp", L(2))
            : std::string(""))
         + (det_fuse || dc ? std::string("") : FOR(0, (int)_.op_ival, [&](int i) {
        int slot = bcps_arg_slot(_.node, argblks, i);
        return stage_arg_inline(i, slot, stage_fp);
    }))
         + (dc ? std::string("")
            : det_fuse
            ? x86("mov32", "edi", det_idx)
            + FOR(0, det_nA, [&](int i) { int slot = bcps_arg_slot(_.node, argblks, i); return x86("lea", detN_argreg[i], FRQ(slot)); })
            + x86("call", detN_nm[det_nA], detN_fp[det_nA])
            : det_idx >= 0
            ? x86("mov32", "edi", (long)det_idx)
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open_det", (uint64_t)det_fp)
            : x86_ro_load_q("rdi", 0)
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open", open_fp))
         + IF(!dc, x86("test", "rax", "rax")
         + x86("je", L(1)))
         + (dc ? std::string("")
            : (det_idx >= 0 ? std::string("") : x86_ro_load_q("rdi", 0) + x86("call", "rt_proc_fn", procfn_fp))
            + [&]{ static int _sp3 = -1; if (_sp3 < 0) { const char *e = getenv("SCRIP_SLIM_PAIR"); _sp3 = (!e || *e != (char)48) ? 1 : 0; } return (!icn_wire_stack_on() && _sp3 && bcps_wire_pair_consumed(_.op_sval)) ? x86("note", "s111 floater pair (LEGACY flat-glue arm): the THIRD non-TINY arm, the one GVA-off actually takes (MONITOR_BIN forces n_gva_m3=0 so the SCC gate and the role-4 TINY shim both refuse and the site falls HERE, to rt_proc_call_open + flat rcx/rdx wires).  s110 patched only the two open_slim tails, so this arm still pushed NOTHING and :(RETURN) popped enclosing-frame bytes.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_ICN_WIRE_STACK=0 restores prior bytes.") + x86("lea", "rcx", L(4)) + x86("push", "rcx") + x86("lea", "rcx", L(3)) + x86("push", "rcx") : std::string(""); }()
            + bcps_wire_cross(3, 4)
            + x86("def", L(3))
            + bcps_wire_land(_.op_sval)
            + x86("call", "rt_proc_call_epilogue_γ", epig_fp)
            + x86("jmp", L(2))
            + x86("def", L(4))
            + bcps_wire_land(_.op_sval)
            + x86("call", "rt_proc_call_epilogue_ω", epiw_fp)
            + x86("jmp", L(2)))
         + IF(!dc, x86("def", L(1))
         + bcps_undef_fallback(undef_fp))
         + x86("def", L(2))
         + x86_anchor_leave()
         + x86_scan_sync_in_rr()
         + bcps_nret_consult(std::string(FRQ(off)), std::string(FRQ(off + 8)))
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + (bidx < 0 ? x86_omega() : x86_pair_jmp(bidx))
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_spine_gen_arm() {
    x86_begin();
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    int act = off + 16 * (1 + (int)_.op_ival);
    IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    uint64_t stage_fp; { void (*fp)(int, DESCR_t) = rt_arg_stage; stage_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t open_fp;  { long (*fp)(const char *, int) = rt_proc_call_open; open_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t openfn_fp; { void * (*fp)(void) = rt_proc_open_fn; openfn_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t procfn_fp; { void * (*fp)(const char *) = rt_proc_fn; procfn_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epig_fp;  { DESCR_t (*fp)(DESCR_t) = rt_proc_call_epilogue_γ; epig_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t epiw_fp;  { DESCR_t (*fp)(void) = rt_proc_call_epilogue_ω; epiw_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t undef_fp;  { void (*fp)(void) = rt_ab_undef_fn_stub; undef_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t pasg_fp;  { DESCR_t (*fp)(DESCR_t) = rt_gen_spine_pass_γ; pasg_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t pasw_fp;  { DESCR_t (*fp)(void) = rt_gen_spine_pass_ω; pasw_fp = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t rsen_fp;  { void (*fp)(void) = rt_gen_spine_resume_enter; rsen_fp = (uint64_t)(uintptr_t)(void*)fp; }
    int  zf_cont_off = (g_emit.zframe_graph && _.op_sval) ? ([]() { extern int zls_g_resume_by_name(const char *); return zls_g_resume_by_name(_.op_sval); })() : -1;
    bool pl_zf_resume = g_emit.zframe_graph && (zf_cont_off >= 0);
    int  pl_tm_off = pl_zf_resume ? zls_g_pl_zf_trail_mark_off_by_name(_.op_sval) : 0;
    int   gi_off; { static int c = -1; if (c < 0) { const char *e = getenv("SCRIP_NO_GENIDX"); c = (e && *e == '1') ? 1 : 0; } gi_off = c; }
    int   gi_dyn = _.op_sval && rt_proc_dyn_scope(_.op_sval);
    long  gi_idx = (!gi_off && !gi_dyn && _.op_sval) ? (long)rt_proc_index_of(_.op_sval) : -1L;
    uint64_t gidet_fp; { void * (*fp)(long, int) = rt_proc_call_open_det; gidet_fp = (uint64_t)(uintptr_t)(void*)fp; }
    /* N2-STEP3-DBG (seat01, 2026-08-28): getenv-gated, stderr only, zero emission effect. VERIFIES two premises step 3's
     * call-site wiring depends on before any real code is written against them: (1) _.node -- the pointer this template
     * already uses for arg-slot lookups -- is pointer-IDENTICAL to what icn_gen_host_reserve_offset()'s scan of
     * g_emit_cfg->all[] finds, so the offset/base it returns actually describes THIS call. MEASURED true on
     * suspend_single (off=0 base=144) and same_gen_twice (off=0/96, base=240 both calls, matching the selftest exactly).
     * (2) g_emit.flat_lcl_proc is true for every host in the four flat_lcl_proc-hosted D2 shapes -- but FALSE for
     * suspend_nested's outer()->inner() call, because outer() is itself flat_gen and icn_gen_regime()'s flat_gen prologue
     * arm (emit.cpp ~2832) never calls icn_gen_host_reserve() -- confirmed by grep, zero hits in that arm's span.
     * ⛔ THIS IS A REAL GAP, NOT AN IMPLEMENTATION DETAIL: a generator calling another generator has no reservation
     * mechanism at all yet. Extending icn_gen_host_reserve() to the flat_gen arm is its own piece of work, not step 3's.
     * See .github/FINDING-2026-08-28-seat01-n2-step3-flat-gen-host-has-no-reservation-mechanism.md before scoping step 3. */
    if (getenv("SEAT01_N2_STEP3_DBG")) { int _dbgbase = -1; int _dbgoff = icn_gen_host_reserve_offset(0, _.node, &_dbgbase); fprintf(stderr, "[N2-STEP3-DBG] node=%p callee=%s host_flat_lcl_proc=%d off=%d base=%d\n", (void*)_.node, _.op_sval ? _.op_sval : "?", g_emit.flat_lcl_proc, _dbgoff, _dbgbase); }
    /* N-2 STEP 3 (ceo s283): the call site owns the region hand-off. n2_off/n2_base come from the ONE function the carve used; n2_fb is THIS callee's registered ft (needed for the landing's value load at
     * [H-ft]); n2_res>0 proves the host's carve actually grew -- offset() can answer off=0 for the FIRST callee while a LATER forward reference zeroed the whole reservation (plausible-zero #5 of this rung,
     * caught at design time), so the guard is the SUM, not the offset. ⛔ A site that cannot supply a region under arming BOMBS loudly (rt_bomb at run time) rather than letting the shared prologue read
     * garbage at [rsp+16] -- that silent-wild-rbp is exactly the shape s282 ruled UNSOUND in option (a). flat_gen-hosted calls (suspend_nested) land here until the transitive-reserve follow-on row. */
    int n2_base = -1, n2_off = -1, n2_res = 0, n2_fb = -1;
    if (icn_gen_regime()) { n2_off = icn_gen_host_reserve_offset(0, _.node, &n2_base); n2_res = icn_gen_host_reserve(0); if (_.op_sval) emit_patzeta_frame_reserve(_.op_sval, &n2_fb); }
    if (icn_gen_regime() && (n2_off < 0 || n2_res <= 0 || n2_fb <= 0)) return x86_alpha() + x86_bomb("N-2 armed: generator call site has no reserved region (flat_gen host or forward reference) -- transitive reserve is the follow-on row; refusing loudly instead of emitting a wild-rbp protocol") + x86_beta() + x86_bomb("N-2 armed: beta re-entry into a refused generator call site");   /* the beta label must still be DEFINED -- a later box's beta chain jumps to it (suspend_nested: outer's suspend beta resumes the inner call), and an early return without it dies as an unresolved forward reference at bb_emit_end instead of as this bomb */
    int n2_ftc = (n2_fb > 0) ? ((n2_fb + 15) & ~15) : 0;
    return x86_alpha()
         + x86_scan_sync_out()
         + x86_anchor_enter()
         + x86("mov", FRQ(act), 0L)
         + FOR(0, (int)_.op_ival, [&](int i) {
        int slot = bcps_arg_slot(_.node, argblks, i);
        return stage_arg_inline(i, slot, stage_fp);
    })
         + ((icn_gen_regime() && icn_genframe2_selfrec() && _.op_sval && icn_gen_is_selfrec(_.op_sval))
              ? x86("comment", "row icon-n2-recursive-generator-per-activation-storage: repurpose the pad slot (same 8 bytes, same rsp math as the PL-CALL-ALIGN pad below) to carry the bounded-self-recursion depth instead of leaving it uninitialized. Only reached when the CALLEE is a direct-self-recursive generator and SCRIP_ICN_N2_SELFREC=1 additionally arms it.")
                + ((g_emit.flat_gen && g_emit.flat_fam && !strcmp(_.op_sval, g_emit.flat_fam))
                   ? x86("comment", "I am the recursive call: read MY OWN depth from the free header slot H+40 (rbp==H for a flat_gen host), bound it against N2_SELFREC_SLOTS, refuse LOUDLY (never silently reuse/corrupt an in-use slot) rather than pass an out-of-range depth forward.")
                     + x86("mov", "rax", RDQ("rbp", 40))
                     + x86("add", "rax", 1L)
                     + x86("cmp", "rax", (long)N2_SELFREC_SLOTS)
                     + x86("jl", L(30))
                     + x86_bomb("N-2 bounded self-recursion: depth exceeds the reserved N2_SELFREC_SLOTS table -- refusing loudly rather than silently reusing an in-use activation slot (row icon-n2-recursive-generator-per-activation-storage; N is a fixed bound sized over one measured workload, see N2_SELFREC_SLOTS' own comment before widening it)")
                     + x86("def", L(30))
                     + x86("push", "rax")
                   : x86("comment", "first (non-recursive) call into a bounded-self-recursive generator: seed depth 0 -- a real zero, not a label address: x86_lea_id loads a PER-CALL-SITE LABEL'S ADDRESS, which is never small, so the very first recursive self-call's depth check (rax=seed+1, cmp rax,N2_SELFREC_SLOTS) always bombed the instant this path was ever actually reached (row icon-n2-recursive-generator-per-activation-storage, FINDING seat_fork-2026-08-30: masked until the 5th-word fix below let execution get this far at all).")
                     + x86("mov32", "eax", 0L) + x86("push", "rax"))
                + x86("sub", "rsp", 8L) + x86("note", "N-2 SELFREC 5TH WORD (row icon-n2-recursive-generator-per-activation-storage, FINDING seat_fork-2026-08-30): the self-rec branches above repurpose ONLY the pad slot (per their own comment) but historically omitted L7's own slot entirely, silently shrinking the callee's documented 5-word/40-byte entry stack (emit.cpp ~2887: [rsp+0]=gamma [rsp+8]=omega [rsp+16]=REGION [rsp+24]=L7 [rsp+32]=pad) to 4 words/32 bytes -- an 8-byte restore-RSP miscount (FINDING 2026-08-30-seat07) that corrupted every RSP-relative caller local after the first resume, and, once THAT was masking a second bug, fed the depth-bank read at emit.cpp:2895 one slot off too (root cause 1, seat12's genqueen garbage-depth finding -- same missing word, read from a different angle). L7's own VALUE is never consumed on this armed path (no consumer reads [rsp+24]/L7 for icn_gen_regime() calls -- L7 is a real jump-back label only on the non-N2/Prolog branch below, defined once, shared, unconditionally, later in this same function), so an anonymous reservation restores the correct BYTE COUNT without needing a label this branch never defines.")
              : x86("sub", "rsp", 8L) + x86("note", "PL-CALL-ALIGN: pad the lone L(7) push to a 16B unit -- one bare 8B push here left rsp 8-mod-16 into rt_proc_call_open_det and the callee jmp, a real ABI violation (SIGSEGV in a later vsnprintf movaps; witness prolog-call-n-user-predicate-segfault). L(7) stays at [rsp+0]; the matching add-rsp-8 landings become 16.") + x86_lea_id("rax", 7) + x86("push", "rax"))
         + (gi_idx >= 0
            ? x86("mov32", "edi", (long)gi_idx)
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open_det", (uint64_t)gidet_fp)
            : x86_ro_load_q("rdi", 0)
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open", open_fp))
         + x86("test", "rax", "rax")
         + x86("je", L(1))
         + (gi_idx >= 0 ? std::string("") : x86_ro_load_q("rdi", 0) + x86("call", "rt_proc_fn", procfn_fp))
         + IF(icn_gen_regime(), x86("comment", "N-2 STEP 3 REGION HAND-OFF (ceo s283): push the callee's region slice address as the third word above the wire pair -- it lands at [entry rsp+16] where the new alpha reads it. Depth arithmetic: pad+L7 = 16 bytes are already down since the carve, so the slice [carve + base + off] is [rsp + base + off + 16] HERE; this constant is verifiable in the .s and breaks loudly (bomb'd alpha reads garbage) if a push is ever added between L7 and this point. Alignment: one extra 8B word flips nothing that matters -- the ABI-sensitive calls (open_det above, args_install inside the callee) keep their old mod-16 classes because the callee no longer subs its ft (alpha carves nothing).")
              + x86("comment", "N-2 TRANSITIVE (seat06 2026-08-29): a FLAT_GEN caller has no stack carve to be RSP-relative INTO -- its own storage is RBP-relative (rbp=H, item 1 rebase + STEP 3 region-resident alpha), fixed regardless of the pad/L7 pushes just above, so no +16 depth-tracking applies the way it does for a flat_lcl_proc caller's RSP-relative slice.")
              + (g_emit.flat_gen ? x86("lea", "rcx", RDQ("rbp", n2_base + n2_off)) : x86("lea", "rcx", RDQ("rsp", n2_base + n2_off + 16))) + x86("push", "rcx"))
         + bcps_wire_cross_gen(3, 4)
         + x86("def", L(3))
         + (icn_gen_regime()
            ? /* N-2 STEP 3 (ceo s283): L(3) is still the SHARED landing and al still tells a RETIRE (DT_FAIL, arrives with rsp ALREADY restored to the anchor by the retire arm) from a SUSPEND (arrives on the
               * generator's scratch rsp with rdx = the region header H). Both arms now run every FRQ at TRUE depth (rsp = carve), which buries the old trio of depth defects this landing carried: the suspend
               * arm joined 8 low so FRQ(act) read [carve+56] (garbage), both banks wrote [carve+64] -- the act FLAG -- while beta read [carve+72], and the epilogue calls ran 8-misaligned on the suspend path
               * (the intermittent armed m4 SIGSEGV, measured in the .s 2026-08-29). The suspend arm also loads rdi:rsi from the region's return slot [H-ft] -- rt_proc_call_epilogue_γ and rt_gen_spine_pass_γ
               * are PASS-THROUGH (rt.c:1305, rtx_icngen.s), so the yielded descriptor must be in the argument registers HERE or the caller stores garbage: that was s273's missing value path. */
              x86("comment", "N-2 STEP 3 LANDING: restore rsp from the ANCHOR in the region header ([rdx+24], = caller pre-pad rsp0), load the yielded descriptor from the frame's return slot, bank the header as the resume token at TRUE depth so beta's FRQ(act+8) read finds it.")
              + x86("cmp", "al", (long)DT_FAIL)
              + x86("je", L(8))
              + x86("mov", "rsp", RDQ("rdx", 24))
              + x86("mov", "rdi", RDQ("rdx", 0 - n2_ftc))
              + x86("mov", "rsi", RDQ("rdx", 8 - n2_ftc))
              + x86("mov", FRQ(act + 8), "rdx")
              + x86("jmp", L(9))
              + x86("def", L(8))
              + x86("mov32", "edi", (long)DT_FAIL) + x86("mov32", "esi", 0L)
              + x86("mov", FRQ(act + 8), "rsp")
              + x86("def", L(9))
            : ((pl_zf_resume && emit_pl_gamma_retain())
               ? x86_bomb("bb_call_proc_staged: PZ-4 clause (c) LANDING is scaffolded and still refuses to run -- BUT NOT FOR THE REASON THIS BOMB CARRIED UNTIL 2026-08-30. The ARITHMETIC blocker seat05 named here is CURED and the cure is measured: emit_patzeta_frame_reserve() now returns a Prolog zframe callee's TRUE frame_total on BOTH registration windows (per-emission SCRIP 3fe34608; the forward-reference PRE-PASS in scrip.c, which kept registering the Icon-shaped (np+nl)*16 after that cure and disagreed with it, fixed in the same commit family by hq_B). MEASURED on fact/2 (np=2 nl=4): pre-pass 1328 -> 1232, per-emission 1232, callee alpha carve `sub rsp, 1232` -- all three now agree, so `rsp = rax + emit_patzeta_frame_reserve(callee)` IS the correct restore and the byte count is no longer unknown. ⛔ A SECOND, INDEPENDENT BLOCKER STANDS, AND IT IS THE ONE THAT MATTERS: THE HOST IS STILL RSP-RELATIVE. Clause (b) hands rax = the callee base with NO unwind (xa_flat_zframe_epilogue_γ_str under pl_gamma_retain_on), so the retained frame lives BELOW the caller landing. Restoring rsp to rax+ft puts rsp back ABOVE that frame, leaving it below rsp and unprotected against the next call -- which is not a hypothesis: hq_P measured that exact class on 18 of 21 van Roy kernels, first invalid access at pl_trail_unwind (src/parsers/prolog/pl_cell.h:81), dead-stack writes 1672/1680 bytes below rsp, .github FINDING 3b349119. ⭐ ceo's own ruling on this row names the missing half: promotion is the ENABLING step -- 'once host ζ is rbp-relative, rsp may sit below the retained frame and the landing protects it'. MEASURED at HEAD, not assumed: a --compile of a two-clause fact/2 under SCRIP_PL_GAMMA_RETAIN=1 emits ZERO `[rbp`-relative frame references and six `qword ptr [rsp + 320]` -- the caller's whole ζ is rsp-relative, so there is no base to re-anchor off and every FRQ() would move under a protective rsp. Land the host RBP promotion for Prolog FIRST; then this landing is the two instructions above, not a redesign. Do NOT 'fix' this by restoring rsp anyway -- that trades a loud refusal for hq_P's silent dead-stack write.")

               : bcps_wire_land(_.op_sval)
                 + (x86("mov", FRQ(act + 8), "rsp")
                    + x86("add", "rsp", 16L))))
         + x86("mov", "rax", FRQ(act))
         + x86("test", "rax", "rax")
         + x86("jne", L(5))
         + x86("mov", FRQ(act), 1L)
         + x86("call", "rt_proc_call_epilogue_γ", epig_fp)
         + x86("jmp", L(2))
         + x86("def", L(5))
         + x86("call", "rt_gen_spine_pass_γ", pasg_fp)
         + x86("jmp", L(2))
         + x86("def", L(4))
         + bcps_wire_land(_.op_sval)
         + x86("add", "rsp", 16L)
         + x86("mov", "rax", FRQ(act))
         + x86("test", "rax", "rax")
         + x86("jne", L(6))
         + x86("mov", FRQ(act), 1L)
         + x86("call", "rt_proc_call_epilogue_ω", epiw_fp)
         + x86("jmp", L(2))
         + x86("def", L(6))
         + x86("call", "rt_gen_spine_pass_ω", pasw_fp)
         + x86("jmp", L(2))
         + x86("def", L(1))
         + bcps_undef_fallback(undef_fp)
         + x86("def", L(2))
         + x86_anchor_leave()
         + x86_scan_sync_in_rr()
         + bcps_nret_consult(std::string(FRQ(off)), std::string(FRQ(off + 8)))
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + x86_scan_sync_out()
         + x86("call", "rt_gen_spine_resume_enter", rsen_fp)
         + (pl_zf_resume
            ? ( [&]() -> std::string {
                /* PZ-4 clause (c) RETRY (seat05 2026-08-30): NOT landed this pass, deliberately. The intended
                 * shape (restore rsp from FRQ(act+8), jmp [rax+zf_cont_off] straight into the callee's own
                 * resume continuation -- no re-invocation, no mailbox -- mirroring icn_gen_regime()'s own
                 * restore+jmp roughly fifty lines below) is BLOCKED for two independent reasons, both real
                 * and both diagnosed, neither resolved: (1) it depends on the landing-side byte-count fix,
                 * which is itself refused-with-a-bomb above (see the landing site a few lines up in this
                 * function) -- so even a working retry would run on a poisoned base; (2) inserting ANY new
                 * early-return branch at this exact position -- verified even with a bare x86_bomb() call,
                 * with and without an added x86_gamma() port-bookkeeping call -- produces
                 * "bb_emit_end: 1 unresolved forward reference(s), label=''" at final emission, a SCRIP-
                 * internal box/port bookkeeping issue not yet root-caused (the landing-side bomb, at a
                 * DIFFERENT position in the same function, does NOT trigger it -- isolated by direct A/B,
                 * not assumed). Left as the pre-existing mailbox code, UNCHANGED, so the flag stays
                 * COMPILABLE end to end; the landing-side bomb (confirmed working, see above) makes this
                 * branch unreachable at runtime for any program that ever suspends, so nothing is lost by
                 * leaving it as-is for this pass. See the FINDING for the full isolation trail. */
                uint64_t _pop3_fp; { void *(*_f)(long *, long *) = rt_pl_cp_pop3; _pop3_fp = (uint64_t)(uintptr_t)(void *)_f; }
                uint64_t _set_fp; { void (*_f)(void *, long, long, int, int) = rt_pl_zf_resume_set; _set_fp = (uint64_t)(uintptr_t)(void *)_f; }
                return x86("comment", "PL-FR-4 zframe β: pop triple, set pending resume, re-enter callee α")
                     + x86("mov", FRQ(act), 0L)
                     + x86("lea", "rdi", FRQ(act + 16))
                     + x86("lea", "rsi", FRQ(act + 24))
                     + x86("call", "rt_pl_cp_pop3", _pop3_fp)
                     + x86("test", "rax", "rax")
                     + x86_omega("je")
                     + x86("mov", "rdi", "rax")
                     + x86("mov", "rsi", FRQ(act + 16))
                     + x86("mov", "rdx", FRQ(act + 24))
                     + x86("mov32", "ecx", (long)pl_tm_off)
                     + x86("mov32", "r8d", (long)zf_cont_off)
                     + x86("call", "rt_pl_zf_resume_set", _set_fp)
                     + FOR(0, (int)_.op_ival, [&](int i) {
                         int slot = bcps_arg_slot(_.node, argblks, i);
                         return x86("mov32", "edi", (long)i)
                              + x86("mov", "rsi", FRQ(slot))
                              + x86("mov", "rdx", FRQ(slot + 8))
                              + x86("call", "rt_arg_stage", stage_fp);
                     })
                     + (gi_idx >= 0
                        ? x86("mov32", "edi", (long)gi_idx)
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open_det", (uint64_t)gidet_fp)
                        : x86_ro_load_q("rdi", 0)
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open", open_fp))
                     + x86("test", "rax", "rax")
                     + x86_omega("je")
                     + (gi_idx >= 0 ? std::string("") : x86_ro_load_q("rdi", 0) + x86("call", "rt_proc_fn", procfn_fp))
                     + x86("sub", "rsp", 8L)
                     + x86_lea_id("r8", 7)
                     + x86("push", "r8")
                     + x86("comment", "PZ-4: the RETRY entry must use the SAME wire spelling as the first-call entry at :615.  It hardcoded the FLAT register pair (lea rcx,L3; lea rdx,L4) while the first-call path goes through bcps_wire_cross, which under icn_wire_stack_on() PUSHES the pair instead.  The callee reads its continuation from a FIXED [rsp+k] and the shared landings pop what the first-call entry pushed -- so with stack wires armed the retry entry arrived 16 bytes shallower, the landing over-popped, and the next continuation was read one slot off: a small integer (measured 17) used as a jump target, rip=0x11.  Route through the shared helper so the two entries CANNOT disagree; it emits its own jmp rax and is byte-identical to the old spelling when stack wires are OFF.")
                     + x86("note", "PL-CALL-ALIGN: same 16B-unit pad as the first-call entry above -- this retry entry pushed L(7) unpadded too, 8-mod-16 into open_det/the callee jmp; matching add-rsp-8 below becomes 16.")
                     + bcps_wire_cross_gen(3, 4)
                     + x86("def", L(7))
                     + x86("add", "rsp", 16L)
                     + x86_anchor_leave()
                     + x86_scan_sync_in_rr()
                     + x86("mov", FRQ(off), "rax")
                     + x86("mov", FRQ(off + 8), "rdx")
                     + x86("cmp", "al", (long)DT_FAIL)
                     + x86_omega("je")
                     + x86_gamma();
              })()
            : (icn_gen_regime()
               ? x86("comment", "N-2 STEP 3 BETA (ceo s283): the banked token is the region header H, not a stack record. Re-create the generator's body rsp from the ANCHOR -- [H+24] is the caller's pre-pad rsp0 and the body ran 40 below it at first entry (pad+L7+region+wire pair), so anchor-40 IS first-entry depth and parity, per activation, per call site -- then jump the resume label stored at [H+32] with the token in rax for the resume landing's one-instruction rbp repoint. The old form (mov rsp,[record]; jmp [rsp]) read a stack record the caller's own calls had already scribbled over.")
                 + x86("mov", "rax", FRQ(act + 8))
                 + x86("mov", "rsp", RDQ("rax", 24))
                 + x86("sub", "rsp", 40L)
                 + x86_jmp_mem("rax", 32)
               : x86("mov", "rsp", FRQ(act + 8))
                 + x86_jmp_mem("rsp", 0))
            + x86("def", L(7))
            + x86("add", "rsp", 8L)
            + x86_anchor_leave()
            + x86_scan_sync_in_rr()
            + x86("mov", FRQ(off), "rax")
            + x86("mov", FRQ(off + 8), "rdx")
            + x86("cmp", "al", (long)DT_FAIL)
            + x86_omega("je")
            + x86_gamma())
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_proc_staged_str(IR_t * pBB) {
    int is_gen = _.op_sval && rt_proc_is_generator(_.op_sval);
    if (is_gen && _.op_node_kind != (int)IR_PROC_GEN && _.op_node_kind != (int)IR_CALL_PROC_STAGED) return x86_alpha() + x86_bomb("bb_call_proc_staged: generator call on an op kind without a callgen.act ZLS2 handle grant (zeta_storage.c widens only IR_PROC_GEN / IR_CALL_PROC_STAGED)");
    if (is_gen) return bcps_spine_gen_arm();
    return bcps_det_arm();
}
