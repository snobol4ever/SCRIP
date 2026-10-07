#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "stage2.h"
#include "ab_abi.h"
#include "bb_templates.h"
long    rt_proc_call_open(const char *name, int nargs);
void   *rt_proc_fn(const char *name); const char *bb_ab_sym_name(const char *nm);
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
DESCR_t rt_faildescr(void);
void    rt_ab_undef_fn_stub(void);
void    rt_ab_undef_fn_fail(void);
void    rt_pl_iso_throw_existence_key(const char *key);
DESCR_t rt_proc_call_gen_h(const char *name, int nargs, void **act_slot, const uint64_t *regs);
DESCR_t rt_proc_resume_frame_h(void **hslot);
DESCR_t rt_gen_spine_pass_γ(DESCR_t v);
DESCR_t rt_gen_spine_pass_ω(void);
void rt_gen_spine_resume_enter(void);
int     zls_g_resume_by_name(const char *name);
int     zls_g_block_args(const IR_graph_t * g);
int  rt_proc_is_generator(const char *name);
int rt_define_tiny_ok(const char *, int);
int rt_define_returns_by_frame(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_wire_pair_consumed(const char *fname) {
    return (fname && rt_define_returns_by_frame(fname)) ? 0 : 1;
}
int  rt_proc_dyn_scope(const char *name);
void rt_arg_stage(int idx, DESCR_t v);
extern "C" struct gv_s g_call_args;
extern "C" int g_gc_pending;
int  rt_proc_is_registered(const char *name); int  rt_proc_is_redefined(const char *name);
int  rt_proc_nformals(const char *name);
int  rt_pl_dc_ok(const char *name, int nargs);
void **rt_pl_dc_slot(long idx);
DESCR_t rt_nret_fix(DESCR_t r, int wn);
DESCR_t rt_nret_fix_tiny(DESCR_t r, int unused_edx);
int  rt_proc_nparams(const char *name);
const char *rt_proc_pname(const char *name, int k);
const char *rt_proc_result_name_get(const char *name);
int  scc_program_ok(void);
int  gva_index_of(const char *name);
extern int g_gva_active;
extern int g_monitor_bin;
extern long g_trace_budget;
int  bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
int  bb_proc_target_zframe_graph(const char *fname);
int  bb_proc_target_pinned_graph(const char *fname); int bb_proc_target_det_block(const char *fname);
int  rt_proc_pinned(const char *name);
void *bb_ab_fn_cell_ptr(const char *fname);
int  bb_proc_target_icn_block(const char *fname, int *np, int *vari);
DESCR_t rt_make_list(DESCR_t *args, int nargs);
void rt_trace_call_hook_f(const char *fname, int np, void *base);
int  zls_g_block_args(const IR_graph_t * g);
}
#include "x86_asm.h"
#define RO_SEAL_STR(n, s) \
    (x86("def", L(n)) \
   + x86(".quad", LS(n), (s)) \
   + x86("label", LS(n)) \
   + x86(".string", (s)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_wire_stack_on(void) {
    static int _v = -1;
    if (_v < 0) {
        const char *e = getenv("SCRIP_ICN_WIRE_STACK");
        _v = (e && *e == (char)48) ? 0 : 1;
    } return _v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_wire_stack_for(const char *fname) { return icn_wire_stack_on() && !bb_proc_target_zframe_graph(fname); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_wire_cross(int gid, int wid, const char *fname) { return icn_wire_stack_for(fname) ? bb_glue_pass_wires_blob_act(gid, wid) : bb_glue_pass_wires(gid, wid); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_wire_cross_gen(int gid, int wid) {
    if (!icn_wire_stack_on() || x86_fb_pinned()) return IF(icn_wire_stack_on(), x86_sub("rsp", 16)) + bb_glue_pass_wires(gid, wid);
    return x86_lea_id("rcx", wid) + x86("push", "rcx")
         + x86_lea_id("rcx", gid) + x86("push", "rcx")
         + x86_lea_id("rdx", wid)
         + x86_jmp_reg("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_wire_land(const char *fname) { return icn_wire_stack_for(fname) ? IF(!bcps_wire_pair_consumed(fname), x86("add", "rsp", 16L)) : std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_retfix(void) {
    static int v = -1;
    if (v < 0) {
        const char *e = getenv("SCRIP_RET_FIX");
        v = (e && *e == '0') ? 0 : 1;
    } return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_epi_named(int is_omega, uint64_t bare_fp)
{
    if (!bcps_retfix()) return x86("call", is_omega ? "rt_proc_call_epilogue_ω" : "rt_proc_call_epilogue_γ", bare_fp);
    return x86("mov", "rdi", ROQ(0))
         + x86("call", is_omega ? "rt_proc_call_epilogue_named_ω" : "rt_proc_call_epilogue_named_γ",
        TEMPLATE_FN_ADDR(is_omega ? rt_proc_call_epilogue_named_ω : rt_proc_call_epilogue_named_γ))
         + x86_rt_gc_poll_res();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define SAI_L0 200
static std::string stage_arg_inline(int i, int slot, uint64_t stage_fp) {
    std::string slow = x86("mov32", "edi", (long)i)
                     + x86("mov", "rsi", FRQ(slot))
                     + x86("mov", "rdx", FRQ(slot + 8))
                     + x86("call", "rt_arg_stage", stage_fp)
                        + x86_rt_gc_poll();
    if (i < 0 || i >= 8 || getenv("SCRIP_NO_SINK")) return slow;
    return x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)&g_gc_pending, "g_gc_pending")
         + x86("mov", "eax", "dword ptr [r8 + 0]")
         + x86("test", "eax", "eax")
         + x86("jne", L(SAI_L0 + i * 2))
         + x86("mov", "rax", FRQ(slot))
         + x86("mov", "rdx", FRQ(slot + 8))
         + x86("lea", "r8", "[rip + __]", (uint64_t)(uintptr_t)&g_call_args, "g_call_args")
         + x86("mov", "r8", "qword ptr [r8 + 0]")
         + x86("test", "r8", "r8")
         + x86("je", L(SAI_L0 + i * 2))
         + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16) + "]").c_str(), "rax")
         + x86("mov", (std::string("[r8 + ") + std::to_string(i * 16 + 8) + "]").c_str(), "rdx")
         + x86("xor", "r8d", "r8d")
         + x86("jmp", L(SAI_L0 + 1 + i * 2))
         + x86("def", L(SAI_L0 + i * 2))
         + slow
         + x86("def", L(SAI_L0 + 1 + i * 2));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * bb_chain_terminal_staged(IR_t * entry) {
    IR_t * n = entry; int guard = 0;
    while (n && n->γ.node && n->γ.node->op != IR_SUCCEED && n->γ.node->op != IR_FAIL && guard++ < 4096) n = n->γ.node;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_beta_pair_idx() {
    for (int i = 0; i < g_emit.xa_bb_emit_pair_n; i++)
        if (XA_PAIR(i).define == _.lbl_β_p && XA_PAIR(i).jmp) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcps_arg_slot(IR_t * call, IR_graph_t ** argblks, int i) {
    IR_t * a = ir_call_arg(call, i);
    if (a) {
    int s = bb_slot_get(a);
    if (s < 0) s = zls_off(a);
    if (s >= 0) return s;
}
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
static inline int bcps_pl() { return x86_fb_pinned(); }
static int bcps_fnsig(void) {
    static int v = -1;
    if (v < 0) {
        const char * e = getenv("SCRIP_FN_SIG");
        v = (e && *e == '0') ? 0 : 1;
    } return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_nret_consult(const std::string & r0, const std::string & r8) {
    extern int rt_g_ret_by_name;
    return x86("note", std::string("NRETURN by-name consult (live wn, consumed)"))
         + x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_g_ret_by_name, "rt_g_ret_by_name")
         + x86("mov", "ecx", RDD("rcx", 0))
         + x86("cmp", "ecx", (long)0)
         + x86("je", L(29))
         + x86("mov", "rdi", "rax")
         + x86("mov", "rsi", "rdx")
         + x86("mov32", "edx", 0L)
         + x86_rtcc_call_descr_ops("rt_nret_fix_tiny", TEMPLATE_FN_ADDR(rt_nret_fix_tiny), r0, r8)
         + x86("mov", "rax", r0.c_str())
         + x86("mov", "rdx", r8.c_str())
         + x86("def", L(29));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long bcps_parse_rsp(const char * t) {
    const char * p = strstr(t, "[rsp");
    if (!p) return -1;
    p += 4; if (*p == '#' || *p == '$') p++;
    while (*p == ' ') p++;
    if (*p == ']') return 0;
    if (*p != '+') return -1;
    p++; while (*p == ' ') p++;
    if (*p < '0' || *p > '9') return -1;
    long v = 0; while (*p >= '0' && *p <= '9') v = v * 10 + (*p++ - '0');
    return (*p == ']') ? v : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long bcps_sig_disp(int slot) { return bcps_parse_rsp(FRQB(slot, 0)); }
static long bcps_zref_disp(int zoff) { return bcps_parse_rsp(x86_zref(zoff, 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void bcps_sig_tally(const char * arm, const char * fn, long n, int ok, const char * why, const char * opnd) {
    static int _sd = -1; if (_sd < 0) {
    const char * _e = getenv("SCRIP_SIG_DIAG");
    _sd = (_e && *_e == '1') ? 1 : 0;
}
    if (!_sd) return;
    fprintf(stderr, "[SIG] arm=%s fn=%s nargs=%ld verdict=%s why=%s opnd=%s\n", arm, fn ? fn : "?", n, ok ? "SIG" : "DECLINE", why, (opnd && *opnd) ? opnd : "-");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int bb_proc_multi_proto(const char *fname) { if (!fname) return 0; for (int i = 0; i < g_stage2.proc_count; i++) { if (!g_stage2.proc_table[i].name
    || strcmp(g_stage2.proc_table[i].name, fname)) continue; int bi = g_stage2.proc_table[i].bb_idx; return (bi >= 0 && bi < g_stage2.bbp.count
        && g_stage2.bbp.table[bi]) ? g_stage2.bbp.table[bi]->multi_proto : 0; } return 0; }
extern "C" int bb_proc_target_pinned_graph(const char *fname) { if (!fname) return 0; for (int i = 0; i < g_stage2.proc_count; i++) { if (!g_stage2.proc_table[i].name
    || strcmp(g_stage2.proc_table[i].name, fname)) continue; int bi = g_stage2.proc_table[i].bb_idx; IR_graph_t * cg = (bi >= 0
        && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0; if (cg) return (cg->zframe_pinned_base && cg->zframe_graph
            && !cg->icn_cells_graph) ? 1 : 0; break; } return rt_proc_pinned(fname); }
extern "C" int bb_proc_target_block_graph(const char *fname) { if (!fname) return 0; for (int i = 0; i < g_stage2.proc_count; i++) { if (!g_stage2.proc_table[i].name
    || strcmp(g_stage2.proc_table[i].name, fname)) continue; int bi = g_stage2.proc_table[i].bb_idx; IR_graph_t * cg = (bi >= 0
        && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0; return (cg && !cg->zframe_pinned_base && zls_g_block_args(cg)) ? 1 : 0; } return 0; }
extern "C" int bb_proc_target_icn_block(const char *fname, int *np, int *vari) {
    int i = 0;
    while (fname && i < g_stage2.proc_count && (!g_stage2.proc_table[i].name || strcmp(g_stage2.proc_table[i].name, fname))) i++;
    IR_graph_t * cg = (fname && i < g_stage2.proc_count && g_stage2.proc_table[i].bb_idx >= 0 && g_stage2.proc_table[i].bb_idx < g_stage2.bbp.count)
                    ? g_stage2.bbp.table[g_stage2.proc_table[i].bb_idx] : (IR_graph_t *)0;
    if (cg && cg->icn_cells_graph && zls_g_block_args(cg)) { if (np) *np = cg->nparams; if (vari) *vari = g_stage2.proc_table[i].is_variadic; }
    return (cg && cg->icn_cells_graph && zls_g_block_args(cg)) ? 1 : 0;
}
extern "C" int bb_proc_target_zframe_graph(const char *fname) { if (!fname) return 0; for (int i = 0; i < g_stage2.proc_count; i++) { if (!g_stage2.proc_table[i].name
    || strcmp(g_stage2.proc_table[i].name, fname)) continue; int bi = g_stage2.proc_table[i].bb_idx; return (bi >= 0 && bi < g_stage2.bbp.count
        && g_stage2.bbp.table[bi]) ? g_stage2.bbp.table[bi]->zframe_graph : 0; } return 0; }
extern "C" int bb_scc_probe(const char *fname, int nargs, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out) {
    int np = 0, nsave = 0, res_gk = -1, scc = 0;
    if (fname && rt_proc_dyn_scope(fname) && !rt_proc_is_generator(fname) && !bb_proc_multi_proto(fname) && !getenv("SCRIP_SCC_OFF") && g_gva_active && scc_program_ok()
        && rt_proc_is_registered(fname) && !rt_proc_is_redefined(fname)) {
        np = rt_proc_nparams(fname);
        if (np >= 0 && np <= BB_SCC_NP_MAX && nargs <= rt_proc_nformals(fname)) {
            const char *rn = rt_proc_result_name_get(fname); int ok = rn ? 1 : 0, sh = 0;
            for (int k = 0; ok && k < np; k++) {
    const char *nm = rt_proc_pname(fname, k);
    int gk = nm ? gva_index_of(nm) : -1;
    if (gk < 0) ok = 0;
    else { gk_out[nsave++] = gk; if (!strcmp(nm, rn)) sh = 1; }
}
            if (ok) { res_gk = gva_index_of(rn); if (res_gk < 0) ok = 0; else if (!sh) gk_out[nsave++] = res_gk; }
            if (ok) scc = 1;
        }
    }
    if (np_out) *np_out = np; if (nsave_out) *nsave_out = nsave; if (res_gk_out) *res_gk_out = res_gk; return scc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int bb_tiny_shim_ok(const char *fname, int nargs) {
    static int _nt = -1; if (_nt < 0) {
    const char *e = getenv("SCRIP_NO_TINY");
    _nt = (e && *e == '1') ? 1 : 0;
}
    if (_nt || !fname) return 0;
    if (!rt_define_tiny_ok(fname, nargs)) return 0;
    int np = 0, ns = 0, rg = -1; int gk[BB_SCC_NP_MAX + 1];
    if (!bb_scc_probe(fname, 0, &np, &ns, gk, &rg)) return 0;
    int nf = rt_proc_nformals(fname);
    if (!(nf >= 0 && nf <= np)) return 0;
    return 1;
}
static std::string bcps_undef_fallback(uint64_t) {
    return x86("call", "rt_ab_undef_fn_fail", (uint64_t)(uintptr_t)(void *)rt_ab_undef_fn_fail)
         + x86_rt_gc_poll() + x86_omega(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_block_build(IR_graph_t ** argblks, int n);
static std::string bcps_icn_block_arm(int is_gen, int off, int act, IR_graph_t ** argblks, long idx, int np, int vari, int n2_ftc);
static std::string bcps_block_build(IR_graph_t ** argblks, int n); static std::string bcps_det_block_arm(int zres, int off, int bidx, IR_graph_t ** argblks, long idx);
static std::string bcps_pinned_byname_road(const std::string & blk, int fncell, int lγ, int lω, int lskip);
static std::string bcps_det_arm() {
    x86_begin();
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    { int ibnp = 0, ibva = 0; long ibidx = (_.op_sval && !rt_proc_dyn_scope(_.op_sval)) ? (long)rt_proc_index_of(_.op_sval) : -1L;
      if (ibidx >= 0 && bb_proc_target_icn_block(_.op_sval, &ibnp, &ibva)) return bcps_icn_block_arm(0, off, 0, (IR_graph_t **)(intptr_t)_.op_counter, ibidx, ibnp, ibva, 0); }
    if (_.op_zres) {
        int bidx_z = bcps_beta_pair_idx(); IR_graph_t ** argblks_z = (IR_graph_t **)(intptr_t)_.op_counter;
        int is_dyn_z = _.op_sval && rt_proc_dyn_scope(_.op_sval);
        long det_idx_z = (!is_dyn_z && _.op_sval) ? (long)rt_proc_index_of(_.op_sval) : -1L;
        int det_nA_z = (int)_.op_ival; { int _bnp = (det_idx_z >= 0 && _.op_sval) ? bb_proc_target_det_block(_.op_sval) : -1; if (_bnp >= 0) { if (_bnp != det_nA_z) return x86_alpha()
            + x86_bomb("bb_call_proc_staged: a block-protocol callee is called with the wrong argument count"); return bcps_det_block_arm(1, off, bidx_z, argblks_z, det_idx_z); } }
        int det_fuse_z = (det_idx_z >= 0 && det_nA_z >= 0 && det_nA_z <= 4);
        int dc_z = 0; uint64_t dc_slot_z = 0; char dc_name_z[_.op_sval ? fmt_len("%s_dc\xce\xb1", bb_ab_sym_name(_.op_sval)) : 1]; dc_name_z[0] = 0;
        if (det_fuse_z && _.op_sval && rt_pl_dc_ok(_.op_sval, det_nA_z)) {
            void **sl = rt_pl_dc_slot(det_idx_z); if (sl) { dc_z = 1; dc_slot_z = (uint64_t)(uintptr_t)sl;
                snprintf(dc_name_z, sizeof dc_name_z, "%s_dc\xce\xb1", bb_ab_sym_name(_.op_sval)); } }
        static const char * const detN_argreg_z[4] = { "rsi", "rdx", "rcx", "r8" };
        uint64_t detN_fp_z[5];
        { void *(*f0)(long) = rt_proc_call_open_det0; detN_fp_z[0] = (uint64_t)(uintptr_t)(void*)f0; }
        { void *(*f1)(long, DESCR_t*) = rt_proc_call_open_det1; detN_fp_z[1] = (uint64_t)(uintptr_t)(void*)f1; }
        { void *(*f2)(long, DESCR_t*, DESCR_t*) = rt_proc_call_open_det2; detN_fp_z[2] = (uint64_t)(uintptr_t)(void*)f2; }
        { void *(*f3)(long, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det3; detN_fp_z[3] = (uint64_t)(uintptr_t)(void*)f3; }
        { void *(*f4)(long, DESCR_t*, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det4; detN_fp_z[4] = (uint64_t)(uintptr_t)(void*)f4; }
        static const char * const detN_nm_z[5] = { "rt_proc_call_open_det0","rt_proc_call_open_det1","rt_proc_call_open_det2","rt_proc_call_open_det3","rt_proc_call_open_det4" };
        int scc_z = 0, scc_np_z = 0, scc_nsave_z = 0, scc_res_gk_z = -1; int scc_gk_z[BB_SCC_NP_MAX + 1];
        scc_z = bb_scc_probe(_.op_sval, (int)_.op_ival, &scc_np_z, &scc_nsave_z, scc_gk_z, &scc_res_gk_z);
        uint64_t dc_slot_fp_z = dc_slot_z;
        return x86_alpha()
             + x86_scan_sync_out()
             + x86_anchor_enter()
             + ((scc_z || (_.op_sval && bb_tiny_shim_ok(_.op_sval, (int)_.op_ival)))
                ? [&]() -> std::string {
                    static int _ntz = -1; if (_ntz < 0) {
    const char * _e = getenv("SCRIP_NO_TINY");
    _ntz = (_e && *_e == '1') ? 1 : 0;
}
                    static int _b1cz = -1; if (_b1cz < 0) {
    const char * _e = getenv("SCRIP_B1C_PARITY");
    _b1cz = (_e && *_e == '0') ? 0 : 1;
}
                    if (!_ntz && ({ extern int g_rt_fragment_emit; !g_rt_fragment_emit || _b1cz; }) && _.op_sval && bb_tiny_shim_ok(_.op_sval, (int)_.op_ival)) {
                        std::string laz = std::string(_.op_sval) + "_\xce\xb1";
                        if (bcps_fnsig()) {
                            std::vector<long> soffz((size_t)_.op_ival); const char * whyz = "eligible"; std::string badz; int sigokz = 1;
                            for (int i = 0; sigokz && i < (int)_.op_ival; i++) {
                                int zs = _.op_zread[i];
                                if (x86_fc_hit(zs) || x86_fc_hit(zs + 8)) { sigokz = 0; whyz = "fc-hit"; break; }
                                long dlo = bcps_zref_disp(zs), dhi = bcps_zref_disp(zs + 8);
                                if (dlo < 0) { sigokz = 0; whyz = "unparsed-operand"; badz = x86_zref(zs, 1); break; }
                                if (dhi != dlo + 8) { sigokz = 0; whyz = "hi-lo-not-adjacent"; badz = x86_zref(zs + 8, 1); break; }
                                soffz[i] = dlo; }
                            bcps_sig_tally("zref", _.op_sval, (long)_.op_ival, sigokz, whyz, badz.c_str());
                            if (sigokz) {
                                std::string snmz = std::string(".L") + x86_boxkind() + "_sig" + std::to_string((long)_.x86_uid) + "z";
                                const struct bb_label_t * sigl_z = emit_label_intern(snmz.c_str());
                                std::string sz = x86_rsp_mark_save()
                                     + x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)sigl_z)
                                     + x86("jmp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()), laz.c_str())
                                     + x86_def_ext(sigl_z)
                                     + x86(".quad", (uint64_t)_.op_ival)
                                     + x86(".quad", L(2))
                                     + x86(".quad", L(2));
                                for (int i = 0; i < (int)_.op_ival; i++) sz += x86(".quad", (uint64_t)soffz[i]);
                                return sz + x86_rsp_land(0L);
                            }
                            return x86_bomb("bcps signature arm DECLINED and there is no safe fallthrough: the callee's role-4 SIG shim (bb_define.cpp, "
                                "fnsig()) reads its formals from [rcx + 24 + 8i], but a site that cannot compute the staged offsets falls "
                                    "through to the SCC/open_slim convention, which puts the gamma continuation in rcx instead -- one entry, two "
                                        "calling conventions, chosen independently by caller and callee, and the callee dereferences whatever it is "
                                            "handed. MEASURED hq_S 2026-09-06, incremental make, RT_OPT=-O0, m3 and m4 agreeing: 2507 shipped programs "
                                                "compiled, 228 of them carry a signature site, 3639 live sites in total, DECLINES = 0. The 42 frame-arm "
                                                    "declines that existed before bcps_parse_rsp learned x86_fr64_prefix's '$' spelling are all SIG now (ADDON/1 "
                                                        "x18, CUT/1 x9, PR/1 x6, PR/2 x6, RET/1 x3, in aisnobol, gimpel and snoflake_suite). So this refusal has a "
                                                            "measured hit rate of ZERO, and reaching it means a NEW operand spelling or frame shape got here: run "
                                                                "SCRIP_SIG_DIAG=1, which prints arm/fn/nargs/why/opnd for every site and names the operand that would not "
                                                                    "parse. One theoretical false positive, unmeasured because it has zero instances today: the callee tests "
                                                                        "bb_tiny_shim_ok(fn, 0) while this site tests it at the real nargs, so a proc tiny-ok at N but not at 0 would "
                                                                            "bomb here where slim-to-slim was in fact correct. The standing cure for that shape is two entry points at the "
                                                                                "callee, not a silent decline here. [arm=zref]");
                        } else {
                        long Kbz = 16L * (long)_.op_ival + 32;
                        auto ZOPQT = [&](int i, int w) { return x86_zref(_.op_zread[i] + w + (int)Kbz, 1); };
                        return x86("sub", "rsp", Kbz)
                             + FOR(0, (int)_.op_ival, [&](int i) {
                                   return x86("note", ZOPN(i))
                                        + x86("mov", "rax", ZOPQT(i, 0)) + x86_rsp_store64(32 + 16 * i, "rax")
                                        + x86("note", ZOPN(i))
                                        + x86("mov", "rax", ZOPQT(i, 8)) + x86_rsp_store64(32 + 16 * i + 8, "rax"); })
                             + x86("mov32", "eax", (long)_.op_ival) + x86_rsp_store64(0, "rax")
                             + x86("lea", "rax", L(2)) + x86_rsp_store64(16, "rax") + x86_rsp_store64(24, "rax")
                             + x86("jmp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()), laz.c_str());
                        }
                    }
                    if (!scc_z) return std::string();
                    return x86_bomb("bb_call_proc_staged: the CALL2BB slice-2 slim road (zref arm) retired 2026-09-27 on a plant proof over the 4659 corpus sources, "
                                    "the 5027 rungs entries compiled and the SNOBOL4, Snocone and Rebus rungs entries run in mode 3 (0 reached it once the role-4 shim "
                                    "took 30 to 60 formals, scripts/gc_rungs_entry_plant_receipt.tsv): the SCC probe held but no tiny or signature arm took the call");
                }()
                : std::string(""))
             + (!scc_z && dc_z
                ? FOR(0, det_nA_z, [&](int i) { return x86("note", ZOPN(i))
                + x86("lea", detN_argreg_z[i], ZOPQ(i, 0)); })
                + x86_call_dc(dc_name_z, dc_slot_fp_z)
                + x86("jmp", L(2))
                : std::string(""))
             + (!scc_z && !dc_z
                ? ((det_fuse_z
                    ? x86("mov32", "edi", det_idx_z)
                    + FOR(0, det_nA_z, [&](int i) { return x86("note", ZOPN(i))
                    + x86("lea", detN_argreg_z[i], ZOPQ(i, 0)); })
                    + x86("call", detN_nm_z[det_nA_z], detN_fp_z[det_nA_z])
                    : ((det_idx_z >= 0
                        ? FOR(0, (int)_.op_ival, [&](int i) { return x86("mov32", "edi", (long)i)
                        + x86("note", ZOPN(i))
                        + x86("mov", "rsi", ZOPQ(i, 0))
                        + x86("note", ZOPN(i))
                        + x86("mov", "rdx", ZOPQ(i, 8))
                        + x86("call", "rt_arg_stage", TEMPLATE_FN_ADDR(rt_arg_stage))
                        + x86_rt_gc_poll(); })
                        + x86("mov32", "edi", (long)det_idx_z)
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open_det", (uint64_t)TEMPLATE_FN_ADDR(rt_proc_call_open_det))
                        : FOR(0, (int)_.op_ival, [&](int i) {return x86("mov32", "edi", (long)i)
                        + x86("note", ZOPN(i))
                        + x86("mov", "rsi", ZOPQ(i, 0))
                        + x86("note", ZOPN(i))
                        + x86("mov", "rdx", ZOPQ(i, 8))
                        + x86("call", "rt_arg_stage", TEMPLATE_FN_ADDR(rt_arg_stage))
                        + x86_rt_gc_poll(); })
                        + x86("mov", "rdi", ROQ(0))
                        + x86("mov32", "esi", (long)_.op_ival)
                        + x86("call", "rt_proc_call_open", TEMPLATE_FN_ADDR(rt_proc_call_open)))))
                   + x86_rt_gc_poll()
                   + x86("test", "rax", "rax")
                   + x86("je", L(1))
                   + (det_idx_z >= 0 && det_fuse_z ? std::string("") : x86("mov", "rdi", ROQ(0))
                   + x86("call", "rt_proc_fn", TEMPLATE_FN_ADDR(rt_proc_fn)))
                   + IF(det_idx_z < 0 && bcps_pl(), bcps_pinned_byname_road(x86("sub", "rsp", (long)(16 * det_nA_z)) + FOR(0, det_nA_z, [&](int i) { return
                   x86("note", ZOPN(i))
                   + x86("mov", "rax", ZOPQ(i, 0)) + x86_rsp_store64(16 * i, "rax")
                   + x86("mov", "rax", ZOPQ(i, 8)) + x86_rsp_store64(16 * i + 8, "rax"); }), off, 61, 62, 60))
                   + [&]{
                       static int _sp4 = -1;
                       if (_sp4 < 0) {
                           const char *e = getenv("SCRIP_SLIM_PAIR");
                           _sp4 = (!e || *e != (char)48) ? 1 : 0;
                       }
                       return (!icn_wire_stack_for(_.op_sval) && _sp4 && bcps_wire_pair_consumed(_.op_sval)) ? x86("note",
                           "s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces "
                               "n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open "
                                   "here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 "
                                       "floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_ICN_WIRE_STACK=0 restores prior bytes.")
                                                                                                             + x86("lea", "rcx", L(4))
                                                                                                             + x86("push", "rcx")
                                                                                                             + x86("lea", "rcx", L(3))
                                                                                                             + x86("push", "rcx") : std::string("");
                   }
                   ()
                   + bcps_wire_cross(3, 4, _.op_sval)
                   + x86("def", L(3))
                   + bcps_wire_land(_.op_sval)
                   + (det_idx_z >= 0 ? x86("call", "rt_proc_call_epilogue_γ", TEMPLATE_FN_ADDR(rt_proc_call_epilogue_γ)) : bcps_epi_named(0, TEMPLATE_FN_ADDR(rt_proc_call_epilogue_γ)))
                   + x86("jmp", L(2))
                   + x86("def", L(4))
                   + bcps_wire_land(_.op_sval)
                   + (det_idx_z >= 0 ? x86("call", "rt_proc_call_epilogue_ω", TEMPLATE_FN_ADDR(rt_proc_call_epilogue_ω)) : bcps_epi_named(1, TEMPLATE_FN_ADDR(rt_proc_call_epilogue_ω)))
                   + x86("jmp", L(2))
                   + x86("def", L(1))
                   + bcps_undef_fallback(TEMPLATE_FN_ADDR(rt_ab_undef_fn_stub)))
                : std::string(""))
             + x86("def", L(2))
             + x86_gc_site(X86_SITE_LANDING)
             + x86_anchor_leave()
             + x86_scan_sync_in_rr()
             + IF(!bb_proc_target_block_graph(_.op_sval), bcps_nret_consult(std::string(ZRES(0)), std::string(ZRES(8))))
             + x86("note", ZRESN())
             + x86("mov", ZRES(0), "rax")
             + x86("note", ZRESN())
             + x86("mov", ZRES(8), "rdx")
             + x86("cmp", "al", (long)DT_FAIL)
             + x86_omega("je")
             + x86_gamma()
             + x86_beta()
             + (bidx_z < 0 ? x86_omega() : x86_pair_jmp(bidx_z))
             + RO_SEAL_STR(0, _.op_sval ? _.op_sval : "");
    }
    int bidx = bcps_beta_pair_idx(); IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    int is_dyn = _.op_sval && rt_proc_dyn_scope(_.op_sval);
    long det_idx = (!is_dyn && _.op_sval) ? (long)rt_proc_index_of(_.op_sval) : -1L;
    uint64_t detN_fp[5]; static const char *detN_nm[5] = { "rt_proc_call_open_det0", "rt_proc_call_open_det1", "rt_proc_call_open_det2", "rt_proc_call_open_det3", "rt_proc_call_open_det4" };
    { void *(*f0)(long) = rt_proc_call_open_det0; detN_fp[0] = (uint64_t)(uintptr_t)(void*)f0; }
    { void *(*f1)(long, DESCR_t*) = rt_proc_call_open_det1; detN_fp[1] = (uint64_t)(uintptr_t)(void*)f1; }
    { void *(*f2)(long, DESCR_t*, DESCR_t*) = rt_proc_call_open_det2; detN_fp[2] = (uint64_t)(uintptr_t)(void*)f2; }
    { void *(*f3)(long, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det3; detN_fp[3] = (uint64_t)(uintptr_t)(void*)f3; }
    { void *(*f4)(long, DESCR_t*, DESCR_t*, DESCR_t*, DESCR_t*) = rt_proc_call_open_det4; detN_fp[4] = (uint64_t)(uintptr_t)(void*)f4; }
    static const char * const detN_argreg[4] = { "rsi", "rdx", "rcx", "r8" };
{ int _bnp = (det_idx >= 0 && _.op_sval) ? bb_proc_target_det_block(_.op_sval) : -1; if (_bnp >= 0) { if (_bnp != (int)_.op_ival) return x86_alpha()
    + x86_bomb("bb_call_proc_staged: a block-protocol callee is called with the wrong argument count"); return bcps_det_block_arm(0, off, bidx, argblks,
        det_idx); } } int det_nA = (int)_.op_ival; int det_fuse = (det_idx >= 0 && det_nA >= 0 && det_nA <= 4);
    int dc = (det_fuse && _.op_sval && rt_pl_dc_ok(_.op_sval, det_nA));
    uint64_t dc_slot = 0; char dc_name[_.op_sval ? fmt_len("%s_dc\xce\xb1", bb_ab_sym_name(_.op_sval)) : 1]; dc_name[0] = 0;
    if (dc) { void **sl = rt_pl_dc_slot(det_idx); if (!sl) dc = 0; else { dc_slot = (uint64_t)(uintptr_t)sl;
        snprintf(dc_name, sizeof dc_name, "%s_dc\xce\xb1", bb_ab_sym_name(_.op_sval)); } }
    int scc = 0, scc_np = 0, scc_nsave = 0, scc_res_gk = -1; int scc_gk[BB_SCC_NP_MAX + 1];
    scc = bb_scc_probe(_.op_sval, (int)_.op_ival, &scc_np, &scc_nsave, scc_gk, &scc_res_gk);
    { static int _td=-1; if(_td<0)_td=getenv("SCRIP_TINY_DIAG")?1:0; if(_td) fprintf(stderr,"[TINYX] fn=%s nargs=%ld scc=%d\n", _.op_sval?_.op_sval:"?",(long)_.op_ival,scc); }
    if (c2farm() && (!scc || (int)_.op_ival != 1)) return x86_alpha()
        + x86_bomb("bb_call_proc_staged: fc-armed call without SCC 1-arg shape (CALL2BB 3b v1) — the flat fallback does not exist "
            "as storage on an armed statement; registration and the probe disagreed");
    return x86_alpha()
         + x86_scan_sync_out()
         + x86_anchor_enter()
         + (scc || (_.op_sval && bb_tiny_shim_ok(_.op_sval, (int)_.op_ival))
            ? [&]() -> std::string {
                static int _ntiny = -1; if (_ntiny < 0) {
    const char * _e = getenv("SCRIP_NO_TINY");
    _ntiny = (_e && *_e == '1') ? 1 : 0;
}
                { static int _td=-1; if(_td<0)_td=getenv("SCRIP_TINY_DIAG")?1:0; if(_td) fprintf(stderr,"[TINY] fn=%s nargs=%ld ok=%d scc=%d\n",
                    _.op_sval?_.op_sval:"?",(long)_.op_ival,_.op_sval?rt_define_tiny_ok(_.op_sval,(int)_.op_ival):-1,scc); }
                static int _b1ct = -1; if (_b1ct < 0) {
    const char * _e = getenv("SCRIP_B1C_PARITY");
    _b1ct = (_e && *_e == '0') ? 0 : 1;
}
                if (!_ntiny && ({ extern int g_rt_fragment_emit; !g_rt_fragment_emit || _b1ct; }) && _.op_sval && bb_tiny_shim_ok(_.op_sval, (int)_.op_ival)) {
                    std::string la = std::string(_.op_sval) + "_\xce\xb1";
                    if (bcps_fnsig()) {
                        std::vector<long> soff((size_t)_.op_ival); const char * why = "eligible"; std::string bad; int sigok = 1;
                        for (int i = 0; sigok && i < (int)_.op_ival; i++) {
                            int slot = bcps_arg_slot(_.node, argblks, i);
                            if (x86_fc_hit(slot) || x86_fc_hit(slot + 8)) { sigok = 0; why = "fc-hit"; break; }
                            long dlo = bcps_sig_disp(slot), dhi = bcps_sig_disp(slot + 8);
                            if (dlo < 0) { sigok = 0; why = "unparsed-operand"; bad = FRQB(slot, 0); break; }
                            if (dhi != dlo + 8) { sigok = 0; why = "hi-lo-not-adjacent"; bad = FRQB(slot + 8, 0); break; }
                            soff[i] = dlo; }
                        bcps_sig_tally("frame", _.op_sval, (long)_.op_ival, sigok, why, bad.c_str());
                        if (sigok) {
                            std::string snm = std::string(".L") + x86_boxkind() + "_sig" + std::to_string((long)_.x86_uid);
                            const struct bb_label_t * sigl = emit_label_intern(snm.c_str());
                            std::string s = x86_rsp_mark_save()
                                 + x86("lea", "rcx", "extlbl", (uint64_t)(uintptr_t)sigl)
                                 + x86("jmp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()), la.c_str())
                                 + x86_def_ext(sigl)
                                 + x86(".quad", (uint64_t)_.op_ival)
                                 + x86(".quad", L(2))
                                 + x86(".quad", L(2));
                            for (int i = 0; i < (int)_.op_ival; i++) s += x86(".quad", (uint64_t)soff[i]);
                            return s + x86_rsp_land(0L);
                        }
                        return x86_bomb("bcps signature arm DECLINED and there is no safe fallthrough: the callee's role-4 SIG shim (bb_define.cpp, "
                            "fnsig()) reads its formals from [rcx + 24 + 8i], but a site that cannot compute the staged offsets falls "
                                "through to the SCC/open_slim convention, which puts the gamma continuation in rcx instead -- one entry, two "
                                    "calling conventions, chosen independently by caller and callee, and the callee dereferences whatever it is "
                                        "handed. MEASURED hq_S 2026-09-06, incremental make, RT_OPT=-O0, m3 and m4 agreeing: 2507 shipped programs "
                                            "compiled, 228 of them carry a signature site, 3639 live sites in total, DECLINES = 0. The 42 frame-arm "
                                                "declines that existed before bcps_parse_rsp learned x86_fr64_prefix's '$' spelling are all SIG now (ADDON/1 "
                                                    "x18, CUT/1 x9, PR/1 x6, PR/2 x6, RET/1 x3, in aisnobol, gimpel and snoflake_suite). So this refusal has a "
                                                        "measured hit rate of ZERO, and reaching it means a NEW operand spelling or frame shape got here: run "
                                                            "SCRIP_SIG_DIAG=1, which prints arm/fn/nargs/why/opnd for every site and names the operand that would not "
                                                                "parse. One theoretical false positive, unmeasured because it has zero instances today: the callee tests "
                                                                    "bb_tiny_shim_ok(fn, 0) while this site tests it at the real nargs, so a proc tiny-ok at N but not at 0 would "
                                                                        "bomb here where slim-to-slim was in fact correct. The standing cure for that shape is two entry points at the "
                                                                            "callee, not a silent decline here. [arm=frame]");
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
                return x86_bomb("bb_call_proc_staged: the CALL2BB slice-2 slim road (frame arm) retired 2026-09-27 on a plant proof over the 4659 corpus sources, "
                                "the 5027 rungs entries compiled and the SNOBOL4, Snocone and Rebus rungs entries run in mode 3 (0 reached it once the role-4 shim "
                                "took 30 to 60 formals, scripts/gc_rungs_entry_plant_receipt.tsv): the SCC probe held but no tiny or signature arm took the call");
              }()
            : std::string(""))
         + (dc
            ? FOR(0, det_nA, [&](int i) { int slot = bcps_arg_slot(_.node, argblks, i); return x86("lea", detN_argreg[i], FRQ(slot)); })
            + x86_call_dc(dc_name, dc_slot)
            + x86("jmp", L(2))
            : std::string(""))
         + (det_fuse || dc ? std::string("") : FOR(0, (int)_.op_ival, [&](int i) {
        int slot = bcps_arg_slot(_.node, argblks, i);
        return stage_arg_inline(i, slot, TEMPLATE_FN_ADDR(rt_arg_stage));
    }))
         + (dc ? std::string("")
            : det_fuse
            ? x86("mov32", "edi", det_idx)
            + FOR(0, det_nA, [&](int i) { int slot = bcps_arg_slot(_.node, argblks, i); return x86("lea", detN_argreg[i], FRQ(slot)); })
            + x86("call", detN_nm[det_nA], detN_fp[det_nA])
            : det_idx >= 0
            ? x86("mov32", "edi", (long)det_idx)
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open_det", (uint64_t)TEMPLATE_FN_ADDR(rt_proc_call_open_det))
            : x86("mov", "rdi", ROQ(0))
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open", TEMPLATE_FN_ADDR(rt_proc_call_open)))
         + IF(!dc, x86_rt_gc_poll())
         + IF(!dc, x86("test", "rax", "rax")
         + x86("je", L(1)))
         + (dc ? std::string("")
            : (det_idx >= 0 ? std::string("") : x86("mov", "rdi", ROQ(0))
            + x86("call", "rt_proc_fn", TEMPLATE_FN_ADDR(rt_proc_fn)))
            + IF(det_idx < 0 && bcps_pl(), bcps_pinned_byname_road(bcps_block_build(argblks, (int)_.op_ival), off, 61, 62, 60))
            + [&]{
                static int _sp3 = -1;
                if (_sp3 < 0) {
                    const char *e = getenv("SCRIP_SLIM_PAIR");
                    _sp3 = (!e || *e != (char)48) ? 1 : 0;
                }
                return (!icn_wire_stack_for(_.op_sval) && _sp3 && bcps_wire_pair_consumed(_.op_sval)) ? x86("note",
                    "s111 floater pair (LEGACY flat-glue arm): the THIRD non-TINY arm, the one GVA-off actually takes (MONITOR_BIN "
                        "forces n_gva_m3=0 so the SCC gate and the role-4 TINY shim both refuse and the site falls HERE, to "
                            "rt_proc_call_open + flat rcx/rdx wires).  s110 patched only the two open_slim tails, so this arm still pushed "
                                "NOTHING and :(RETURN) popped enclosing-frame bytes.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the "
                                    "fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_ICN_WIRE_STACK=0 restores prior bytes.")
                                                                                                      + x86("lea", "rcx", L(4))
                                                                                                      + x86("push", "rcx")
                                                                                                      + x86("lea", "rcx", L(3))
                                                                                                      + x86("push", "rcx") : std::string("");
            }
            ()
            + bcps_wire_cross(3, 4, _.op_sval)
            + x86("def", L(3))
            + bcps_wire_land(_.op_sval)
            + (det_idx >= 0 ? x86("call", "rt_proc_call_epilogue_γ", TEMPLATE_FN_ADDR(rt_proc_call_epilogue_γ)) : bcps_epi_named(0, TEMPLATE_FN_ADDR(rt_proc_call_epilogue_γ)))
            + x86("jmp", L(2))
            + x86("def", L(4))
            + bcps_wire_land(_.op_sval)
            + (det_idx >= 0 ? x86("call", "rt_proc_call_epilogue_ω", TEMPLATE_FN_ADDR(rt_proc_call_epilogue_ω)) : bcps_epi_named(1, TEMPLATE_FN_ADDR(rt_proc_call_epilogue_ω)))
            + x86("jmp", L(2)))
         + IF(!dc, x86("def", L(1))
         + bcps_undef_fallback(TEMPLATE_FN_ADDR(rt_ab_undef_fn_stub)))
         + x86("def", L(2))
         + x86_gc_site(X86_SITE_LANDING)
         + x86_anchor_leave()
         + x86_scan_sync_in_rr()
         + IF(!bb_proc_target_block_graph(_.op_sval), bcps_nret_consult(std::string(FRQ(off)), std::string(FRQ(off + 8))))
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + (bidx < 0 ? x86_omega() : x86_pair_jmp(bidx))
         + RO_SEAL_STR(0, _.op_sval ? _.op_sval : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_jmp_proc_fn(long idx) {
    extern void *g_rt_gen_procs;
    return x86("note",
        "the registry jump: the record's fn word (PROC_FN at +8, the 128-byte stride both asserted in rt.c), so a fn "
            "re-sealed at run time -- a dynamic predicate's enumerator, a redefinition -- is followed in both media")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_rt_gen_procs, "g_rt_gen_procs")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("mov", "rax", RDQ("rax", (int)(8 + 128 * idx)))
         + x86_jmp_reg("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_jmp_callee(long idx) {
    if (_.node && (_.node->strict & (1 << 30)) && _.op_sval) return x86("note",
        "a static callee (ARCH-PROLOG-C-OUT-OF-THE-BOX 9.3): a direct jump to its entry label in the text, through its "
            "alpha cell in the slab (x86_jmp_via_cell, both media; the cell is sealed by rt_proc_seal_alpha when the "
                "record's fn is set and pinned, under no knob); the registry record is for a fn re-sealed at run time, which a static predicate never is (assertz on it raises permission_error)")
                                                               + x86("jmp", "[rip@cell + __]", (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("alpha$") + _.op_sval).c_str()),
                                                                   (std::string("FN__") + bb_ab_sym_name(_.op_sval)).c_str());
    return bcps_jmp_proc_fn(idx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_lvl_add(long d);
static std::string bcps_pinned_byname_road(const std::string & blk, int fncell, int lγ, int lω, int lskip) {
    extern int rt_proc_pinned(const char *);
    return x86("note",
        "the by-name road into a callee whose record reads PINNED at run time (a dynamic predicate's enumerator, a runtime-compiled graph): the block protocol, decided by the record, never by a name")
         + x86("mov", FRQ(fncell), "rax")
         + x86("mov", "rdi", ROQ(0))
         + x86("call", "rt_proc_pinned", TEMPLATE_FN_ADDR(rt_proc_pinned))
         + x86("test", "eax", "eax")
         + x86("je", L(lskip))
         + blk
         + x86_lea_id("rcx", lγ) + x86_lea_id("rdx", lω)
         + x86("mov", "rax", FRQ(fncell))
         + x86_jmp_reg("rax")
         + x86("def", L(lγ))
         + bcps_lvl_add(-1L) + x86("mov", "rax", "rdi")
         + x86("mov", "rdx", "rsi")
         + x86("jmp", L(2))
         + x86("def", L(lω))
         + bcps_lvl_add(-1L) + x86("mov32", "eax", (long)DT_FAIL)
         + x86("xor", "edx", "edx")
         + x86("jmp", L(2))
         + x86("def", L(lskip))
         + x86("mov", "rax", FRQ(fncell));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_lvl_add(long d) {
    extern int * const rt_k_level_p;
    return x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("add", RDD("rax", 0), d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_block_build(IR_graph_t ** argblks, int n) {
    return x86("sub", "rsp", (long)(16 * n))
         + FOR(0, n, [&](int i) { int slot = bcps_arg_slot(_.node, argblks, i);
               return x86("note", std::string("block A") + std::to_string(i))
                    + x86("mov", "rax", FRQ(slot)) + x86_rsp_store64(16 * i, "rax")
                    + x86("mov", "rax", FRQ(slot + 8)) + x86_rsp_store64(16 * i + 8, "rax"); });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_block_tail_arm(int n, int kt, int lsk, long idx) {
    int ns = g_emit_cfg ? g_emit_cfg->nparams : 0;
    auto var_slot = [&](int i) -> int { IR_t * a = ir_call_arg(_.node, i); if (!a || a->op != IR_VAR_REF || !a->sval) return -1; return bb_varslot_peek(a->sval); };
    auto arms = [&](int lb) -> std::string {
        return x86("def", L(lb + 3))
             + x86("mov", "r8", RDQ("rdi", 8))
             + x86("test", "r8", "r8")
             + x86("je", L(lb + 8))
             + x86("mov", "ecx", RDD("rdi", 4))
             + x86("cmp", "ecx", 2L)
             + x86("jne", L(lb + 6))
             + x86("mov", "r8", RDQ("r8", 0))
             + x86("test", "r8", "r8")
             + x86("je", L(lb + 8))
             + x86("jmp", L(lb + 6))
             + x86("def", L(lb + 5))
             + x86("mov", "r8", RDQ("rdi", 8))
             + x86("cmp", "r8", x86_fb())
             + x86("jb", L(lb + 9))
             + x86("mov", "rax", "r8")
             + x86("sub", "rax", x86_fb())
             + x86("cmp", "rax", (long)(kt + 16 * ns))
             + x86("jae", L(lb + 9))
             + x86("def", L(lb + 6))
             + x86("mov", "rdi", "r8")
             + x86("sub", "r9d", 1L)
             + x86("jne", L(lb + 2))
             + x86("jmp", L(lsk + 99))
             + x86("def", L(lb + 8))
             + x86("mov", "rax", "rdi")
             + x86("sub", "rax", x86_fb())
             + x86("cmp", "rax", (long)(kt + 16 * ns))
             + x86("jb", L(lsk + 99))
             + x86("mov", "rax", (long)((1L << 32) | (long)DT_N))
             + x86("mov", RDQ("rsi", 0), "rax")
             + x86("mov", RDQ("rsi", 8), "rdi")
             + x86("jmp", L(lb + 7)); };
    auto body = [&](int lb) -> std::string {
        return x86("def", L(lb + 2))
             + x86("mov", "eax", RDD("rdi", 0))
             + x86("cmp", "al", (long)DT_N)
             + x86("je", L(lb + 3))
             + x86("test", "al", "al")
             + x86("je", L(lb + 8))
             + x86("cmp", "al", (long)DT_PLVAR)
             + x86("je", L(lb + 5))
             + x86("def", L(lb + 9))
             + x86("mov", "rax", RDQ("rdi", 0))
             + x86("mov", RDQ("rsi", 0), "rax")
             + x86("mov", "rax", RDQ("rdi", 8))
             + x86("mov", RDQ("rsi", 8), "rax")
             + x86("def", L(lb + 7)); };
    auto cell_entry = [&](int lb) -> std::string {
        return x86("mov", "eax", RDD("rsi", 0))
             + x86("cmp", "al", (long)DT_N)
             + x86("je", L(lb + 4))
             + x86("cmp", "al", (long)DT_PLVAR)
             + x86("jne", L(lb + 7))
             + x86("def", L(lb + 4))
             + x86("mov", "rdi", RDQ("rsi", 8))
             + x86("mov32", "r9d", 4L) + body(lb); };
    std::string chk = x86("note",
        "tail-safe, per cell (ARCH-PROLOG-C-OUT-OF-THE-BOX 9.3): a variable argument is inspected from its own slot "
            "(one hop fewer than through its name cell), any other cell from the block; ONE check body, its cold arms out "
                "of line after the callee jump: a name (slen 1 the cell, slen 2 the VCELL's) is followed to its cell; a PLVAR "
                    "link INTO THIS FRAME (A = B between two locals) is followed too, a link elsewhere is a value (its pointer is "
                        "absolute and its target outlives the frame); at most four hops, a longer chain or a self link of this frame "
                            "DECLINES the tail arm; a bound cell's VALUE is copied in; an unbound cell (the zero DESCR, a null name) of this frame declines, elsewhere it becomes a one-hop name");
    std::string ool;
    int wide = n > 8;
    if (wide) { chk += x86("mov", "rsi", "rsp")
                     + x86("lea", "rdx", RDQ("rsp", 16 * n))
         + x86("def", L(lsk + 1))
         + x86("cmp", "rsi", "rdx")
         + x86("jae", L(lsk + 10))
         + cell_entry(lsk)
         + x86("add", "rsi", 16L)
         + x86("jmp", L(lsk + 1))
         + x86("def", L(lsk + 10)); ool = arms(lsk); }
    else for (int i = 0; i < n; i++) {
    int vs = var_slot(i); int lb = lsk + 10 + 10 * i;
        chk += x86("lea", "rsi", RDQ("rsp", 16 * i));
        chk += vs < 0 ? cell_entry(lb) : x86("lea", "rdi", FRQ(vs))
                                       + x86("mov32", "r9d", 4L) + body(lb); ool += arms(lb); }
    int nb = kt + 16 * (ns - n);
    std::string mv = x86("note",
        "the new block's TOP is the old block's top (rbp+kt+16*nparams): a longer block grows DOWN over this frame's "
            "dead header, never up into the caller's spine; copied descending so a long block never overruns its own source");
    for (int i = n - 1; i >= 0; i--) mv += x86("mov", "rax", RDQ("rsp", 16 * i + 8))
                                         + x86("mov", RDQ(x86_fb(), nb + 16 * i + 8), "rax")
                                         + x86("mov", "rax", RDQ("rsp", 16 * i))
                                         + x86("mov", RDQ(x86_fb(), nb + 16 * i), "rax");
    return x86("note", "last call (ARCH-PROLOG-C-OUT-OF-THE-BOX 1.4): the block is tested in place, then copied over this frame's own block and the frame released")
         + chk
         + x86("mov", "rax", RDQ(x86_fb(), kt - 40))
         + x86("cmp", "r13", "rax")
         + x86("jne", L(lsk + 99))
         + x86("lea", "rax", RDQ("rsp", 16 * n))
         + x86("cmp", "rax", x86_fb())
         + x86("jne", L(lsk + 99))
         + x86("mov", "rcx", RDQ(x86_fb(), kt - 24))
         + x86("mov", "rdx", RDQ(x86_fb(), kt - 16))
         + x86("mov", "r8", RDQ(x86_fb(), kt - 8))
         + mv
         + x86("lea", "rsp", RDQ(x86_fb(), nb))
         + x86("mov", x86_fb(), "r8")
         + bcps_jmp_callee(idx)
         + ool
         + x86("def", L(lsk + 99));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_block_arm(int off, int act, IR_graph_t ** argblks, long idx) {
    int n = (int)_.op_ival; int kt = g_emit.flat_frame_bytes;
    int pl_lco_armed = _.node && _.node->seal == 1 && !(g_emit_cfg && g_emit_cfg->root_graph);
    return x86_alpha()
         + x86("note", "the block protocol (ARCH-PROLOG-C-OUT-OF-THE-BOX 1.2): the argument block on this spine, the wires, a jump; no staged medium, no registry call, no C prologue")
         + x86("mov", FRQ(act), 0L)
         + bcps_block_build(argblks, n)
         + IF(pl_lco_armed, bcps_block_tail_arm(n, kt, 100, idx))
         + x86_lea_id("rcx", 3) + x86_lea_id("rdx", 4)
         + bcps_jmp_callee(idx)
         + x86("def", L(3))
         + x86("mov", FRQ(act), "rax")
         + x86("mov", FRQ(act + 8), "rdx")
         + bcps_lvl_add(-1L)
         + x86("mov", "rax", "rdi")
         + x86("mov", "rdx", "rsi")
         + x86("jmp", L(2))
         + x86("def", L(4))
         + x86("mov", FRQ(act), 0L)
         + bcps_lvl_add(-1L)
         + x86("mov32", "eax", (long)DT_FAIL)
         + x86("xor", "edx", "edx")
         + x86("def", L(2))
         + x86("mov", FRQ(off), "rax")
         + x86("mov", FRQ(off + 8), "rdx")
         + x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + x86("test", "r15", "r15")
         + x86("jne", L(22))
         + x86("mov", "rax", FRQ(act))
         + x86("test", "rax", "rax")
         + x86("je", L(22))
         + x86("mov", "rcx", FRQ(act + 8))
         + x86("mov", x86_fb(), "rax")
         + bcps_lvl_add(1L)
         + x86_jmp_reg("rcx")
         + x86("def", L(22))
         + x86_omega()
         + RO_SEAL_STR(0, _.op_sval ? _.op_sval : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_icn_lvl(long d) {
    extern int * const rt_k_level_p; extern int64_t kw_fnclevel;
    return x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("add", RDD("rax", 0), d)
         + x86("mov", "ecx", RDD("rax", 0))
         + x86("movsxd", "rcx", "ecx")
         + x86("sub", "rcx", 1L)
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&kw_fnclevel, "kw_fnclevel")
         + x86("mov", RDQ("rax", 0), "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_icn_opnd(IR_graph_t ** argblks, int i, int w, long d) {
    std::string u = _.op_zres ? x86_zref((i < _.op_zcap ? _.op_zread[i] : 0) + w, 1) : FRQ(bcps_arg_slot(_.node, argblks, i) + w);
    return (u.find("rbp") != std::string::npos || (u.find("rsp$") != std::string::npos && x86_fb_num() == 5)) ? u
         : _.op_zres ? std::string(x86_zref((i < _.op_zcap ? _.op_zread[i] : 0) + w + (int)d, 1))
         : std::string(FRQB(bcps_arg_slot(_.node, argblks, i) + w, (int)d));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_icn_src(IR_graph_t ** argblks, int i, int w, long d) {
    return (_.op_zres && i < _.op_zcap && _.op_zread_xf[i] != -1) ? x86("mov", "rcx", RDQ("rbp", _.op_zread_xf[i] + w))
         : (!_.op_zres && x86_fc_hit(bcps_arg_slot(_.node, argblks, i) + w))
         ? x86_rsp_load64("rcx", bcps_arg_slot(_.node, argblks, i) + w - _.op_fc_base + (int)d)
         : x86("mov", "rcx", bcps_icn_opnd(argblks, i, w, d).c_str());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_icn_block_arm(int is_gen, int off, int act, IR_graph_t ** argblks, long idx, int np, int vari, int n2_ftc) {
    extern std::string xa_icn_trace_tap(const char * pname, int kind, int np, int r11d);
    int nargs = (int)_.op_ival, nfix = vari ? np - 1 : np, ncp = nargs < nfix ? nargs : nfix, rest = (vari && nargs > nfix) ? nargs - nfix : 0;
    long K = (is_gen ? 40L : 16L) + 16L * np, pad = is_gen ? 8L : 0L;
    return x86_alpha()
         + x86("note", "the block protocol (ARCH-PROLOG-C-OUT-OF-THE-BOX 1.2, Icon): the wires and entry words first, then the argument block at the callee's entry rsp, "
                       "then a jump through the registry -- no staged medium, no C open, no C epilogue, no NRETURN consult")
         + x86_scan_sync_out()
         + x86_anchor_enter()
         + IF(is_gen, x86("mov", FRQ(act), (long)DT_RAW)
                    + bcps_icn_lvl(1L)
                    + bb_glue_lvl_slot_rcx()
                    + x86("mov", RDQ("rcx", SNO_LVL_ACT_RSP), 0L)
                    + x86("sub", "rsp", 24L)
                    + x86_rsp_store64_imm(16, 0L)
                    + x86_rsp_store64_imm(8, 0L)
                    + x86_rsp_store64_imm(0, 0L))
         + x86_lea_id("rcx", 4)
         + x86("push", "rcx")
         + x86_lea_id("rcx", 3)
         + x86("push", "rcx")
         + IF(!is_gen, bb_glue_act_record(0))
         + x86("sub", "rsp", 16L * np)
         + FOR(0, ncp, [&](int i) { return bcps_icn_src(argblks, i, 0, K)
                                         + x86_rsp_store64(16 * i, "rcx")
                                         + bcps_icn_src(argblks, i, 8, K)
                                         + x86_rsp_store64(16 * i + 8, "rcx"); })
         + FOR(ncp, np, [&](int i) { return x86_rsp_store64_imm(16 * i, 0L)
                                          + x86_rsp_store64_imm(16 * i + 8, 0L); })
         + IF(vari, x86("sub", "rsp", 16L * rest + pad)
                  + FOR(0, rest, [&](int k) { return bcps_icn_src(argblks, nfix + k, 0, K + 16 * rest + pad)
                                                   + x86_rsp_store64(16 * k, "rcx")
                                                   + bcps_icn_src(argblks, nfix + k, 8, K + 16 * rest + pad)
                                                   + x86_rsp_store64(16 * k + 8, "rcx"); })
                  + x86("mov", "rdi", "rsp")
                  + x86("mov32", "esi", (long)rest)
                  + x86("call", "rt_make_list", (uint64_t)(uintptr_t)(void *)rt_make_list)
                  + x86_rt_gc_poll_res()
                  + x86("add", "rsp", 16L * rest + pad)
                  + x86_rsp_store64(16 * (np - 1), "rax")
                  + x86_rsp_store64(16 * (np - 1) + 8, "rdx"))
         + IF(is_gen, xa_icn_trace_tap(_.op_sval, 1, np, 0))
         + IF(is_gen, x86_lea_id("rdx", 4))
         + bcps_jmp_proc_fn(idx)
         + x86("def", L(3))
         + (is_gen ? x86("cmp", "al", (long)DT_FAIL)
                   + x86("je", L(8))
                   + x86("mov", "rdi", RDQ("rdx", 0 - n2_ftc))
                   + x86("mov", "rsi", RDQ("rdx", 8 - n2_ftc))
                   + x86("mov", FRQ(act + 8), "rdx")
                   + x86("jmp", L(9))
                   + x86("def", L(8))
                   + x86("mov32", "edi", (long)DT_FAIL)
                   + x86("mov32", "esi", 0L)
                   + x86("mov", FRQ(act + 8), "rsp")
                   + x86("def", L(9))
                   + x86("mov", "rax", FRQ(act))
                   + x86_raw_unpack("rax")
                   + x86("test", "rax", "rax")
                   + x86("jne", L(5))
                   + x86("mov", FRQ(act), (long)(0x100 | DT_RAW))
                   + bcps_icn_lvl(-1L)
                   + x86("mov", "rax", "rdi")
                   + x86("mov", "rdx", "rsi")
                   + x86("jmp", L(2))
                   + x86("def", L(5))
                   + x86("call", "rt_gen_spine_pass_γ", (uint64_t)(uintptr_t)(void *)rt_gen_spine_pass_γ)
                   + x86("jmp", L(2))
                 : bcps_wire_land(_.op_sval)
                   + x86("mov", "rax", "rdi")
                   + x86("mov", "rdx", "rsi")
                   + x86("jmp", L(2)))
         + x86("def", L(4))
         + bcps_wire_land(_.op_sval)
         + (is_gen ? x86("add", "rsp", 8L)
                   + x86("mov", "rax", FRQ(act))
                   + x86_raw_unpack("rax")
                   + x86("test", "rax", "rax")
                   + x86("jne", L(6))
                   + x86("mov", FRQ(act), (long)(0x100 | DT_RAW))
                   + bcps_icn_lvl(-1L)
                   + x86("mov32", "eax", (long)DT_FAIL)
                   + x86("xor", "edx", "edx")
                   + x86("jmp", L(2))
                   + x86("def", L(6))
                   + x86("call", "rt_gen_spine_pass_ω", (uint64_t)(uintptr_t)(void *)rt_gen_spine_pass_ω)
                   + x86("jmp", L(2))
                 : x86("mov32", "eax", (long)DT_FAIL)
                   + x86("xor", "edx", "edx"))
         + x86("def", L(2))
         + x86_anchor_leave()
         + x86_scan_sync_in_rr()
         + x86("mov", _.op_zres ? ZRES(0) : FRQ(off), "rax")
         + x86("mov", _.op_zres ? ZRES(8) : FRQ(off + 8), "rdx")
         + x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + (is_gen ? x86_scan_sync_out()
                   + x86("call", "rt_gen_spine_resume_enter", (uint64_t)(uintptr_t)(void *)rt_gen_spine_resume_enter)
                   + x86("mov", "rax", FRQ(act + 8))
                   + x86("mov", "rsp", RDQ("rax", 40))
                   + x86_jmp_mem("rax", 32)
                 : (bcps_beta_pair_idx() < 0 ? x86_omega() : x86_pair_jmp(bcps_beta_pair_idx())))
         + RO_SEAL_STR(0, _.op_sval ? _.op_sval : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcps_spine_gen_arm() {
    x86_begin();
    int off = bcps_result_slot(); if (off < 0) return x86_bomb("bb_call_proc_staged: no LOWER slot grant (TMP-ERADICATE)");
    int act = zls_act_off(_.node); if (act < 0) act = off + 16 * (1 + (int)_.op_ival);
    IR_graph_t ** argblks = (IR_graph_t **)(intptr_t)_.op_counter;
    int   gi_off; {
    static int c = -1;
    if (c < 0) {
        const char *e = getenv("SCRIP_NO_GENIDX");
        c = (e && *e == '1') ? 1 : 0;
    } gi_off = c;
}
    int   gi_dyn = _.op_sval && rt_proc_dyn_scope(_.op_sval);
    long  gi_idx = (!gi_off && !gi_dyn && _.op_sval) ? (long)rt_proc_index_of(_.op_sval) : -1L;
    if (bcps_pl() && gi_idx >= 0 && bb_proc_target_pinned_graph(_.op_sval)) return bcps_block_arm(off, act, argblks, gi_idx);
    int n2_fb = -1;
    if (icn_gen_regime() && _.op_sval) emit_patzeta_frame_reserve(_.op_sval, &n2_fb);
    if (icn_gen_regime() && n2_fb <= 0) return x86_alpha()
        + x86_bomb("N-3: generator call site cannot size the callee's result slot (the callee's frame bytes are not registered: forward reference) -- refusing loudly") + x86_beta()
            + x86_bomb("N-3: beta re-entry into a refused generator call site");
    int n2_ftc = (n2_fb > 0) ? ((n2_fb + 15) & ~15) : 0;
    { int ibnp = 0, ibva = 0; if (gi_idx >= 0 && n2_ftc > 0 && bb_proc_target_icn_block(_.op_sval, &ibnp, &ibva)) return bcps_icn_block_arm(1, off, act, argblks, gi_idx, ibnp, ibva, n2_ftc); }
    return x86_alpha()
         + x86_scan_sync_out()
         + x86_anchor_enter()
         + x86("mov", FRQ(act), 0L)
         + FOR(0, (int)_.op_ival, [&](int i) {
        int slot = bcps_arg_slot(_.node, argblks, i);
        return stage_arg_inline(i, slot, TEMPLATE_FN_ADDR(rt_arg_stage));
    })
         + (gi_idx >= 0
            ? x86("mov32", "edi", (long)gi_idx)
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open_det", (uint64_t)TEMPLATE_FN_ADDR(rt_proc_call_open_det))
            : x86("mov", "rdi", ROQ(0))
            + x86("mov32", "esi", (long)_.op_ival)
            + x86("call", "rt_proc_call_open", TEMPLATE_FN_ADDR(rt_proc_call_open)))
         + x86_rt_gc_poll()
         + IF(icn_gen_regime(), x86("sub", "rsp", 8L) + x86_rsp_store64_imm(0, 0)
         + x86("note",
             "N-2 ABI WORD (row icon-generator-call-path-enters-every-runtime-helper-8-bytes-off-the-sysv-abi, hq_I "
                 "root-caused, hq_B authored): the REGION HAND-OFF push below is a LONE 8B word and therefore PARITY-FLIPPING, "
                     "so the armed call site pushed 40 bytes (pad+L7+region+wire pair) where the unarmed one pushes 32. The callee "
                         "body then ran at rsp0-40 = 8 mod 16 and EVERY call it made entered a helper at 0 mod 16 -- latent until some "
                             "callee reached an aligned SSE store, which is why it read as a record bug (suspend a list, nothing; suspend a "
                                 "RECORD and dat_construct -> rt_fire_buildplan_tweak -> snprintf -> movaps -> dead). This word is pushed "
                                     "FIRST, above the pad, ON PURPOSE: every documented entry offset ([rsp+0]=gamma [rsp+8]=omega [rsp+16]="
                                         "REGION [rsp+24]=L7 [rsp+32]=pad) is UNCHANGED, and only the caller pre-pad rsp0 moves from [rsp+40] to [rsp+48] -- "
                                             "one constant in the alpha's ANCHOR lea and one in the beta re-creation. Placing it between L7 and the region "
                                                 "instead would keep the region at +16 and silently move the pad, which is the slot the selfrec depth is read from at [entry rsp+32]."))
         + (x86("note",
             "CFO-36 (cfo 2026-09-09): the landing words (N-2 word, landing cell) are pushed AFTER the prologue call, not "
                 "before it -- in the generator regime they are an ODD count, so rt_proc_call_open_det ran at rsp 8-mod-16 and "
                     "everything it reached did too (rt_trace_event_args -> image -> vsnprintf movaps: SIGSEGV on every traced "
                         "generator call with an argument; Arizona coexpr and errors, rungs rung03). The entry layout the callee sees "
                             "is byte-identical; only the prologue call moved above the words,") + IF(!icn_gen_regime(), x86_sub("rsp", 16)
         + x86("note",
             "LANDING UNIT (cto, ARCH-GC section 13.2): the 16 bytes under the wire pair are a raw carve nothing reads; "
                 "nothing ever jumped to the L(7) they used to hold (plant over 1,800 programs, both modes).")) + IF(icn_gen_regime(),
         x86("note",
             "CEO-483 (hq_U): NO PAD IN THE GENERATOR REGIME. The pad above is caller-side transient bookkeeping that had "
                 "drifted into the callee ENTRY FRAME as a sixth word, and hq_U FINDING-2026-09-09 measured that NOTHING READS "
                     "IT -- an injected 0x5EEDFACE store into [entry rsp+32] left parse byte-identical while the same store into "
                         "[entry rsp+0] SIGSEGVd, so the experiment had a positive control and the slot is padding. The comment that "
                             "used to sit here named a `selfrec depth` reader at [entry rsp+32]; `selfrec` occurred exactly once in the "
                                 "whole tree -- in that sentence. The 8 bytes are NOT deleted, they MOVE ACROSS THE CALL into the callee`s own "
                                     "carve (emit.cpp: carve gains 8, ANCHOR lea rsp+48 -> rsp+40), so the callee body still lands 0 mod 16. "
                                         "Dropping the pad WITHOUT that move was measured on 2026-09-10 and SIGSEGVs patchu -- the crash is parity, "
                                             "never a lost datum. Entry frame in the generator regime is now FIVE words: [rsp+0]=gamma [rsp+8]="
                                                 "omega [rsp+16]=REGION [rsp+24]=0, the landing word nobody reads [rsp+32]=N-2 ABI word, ANCHOR=[rsp+40]. "
                                                     "rt_genp_spine_enter_n2 (rt.c) is the hand-written twin of this block and was shrunk by the same word in the same landing."))
                                                         + IF(icn_gen_regime(),
         x86("sub", "rsp", 8L) + x86_rsp_store64_imm(0, 0L)))
         + x86("test", "rax", "rax")
         + x86("je", L(1))
         + (gi_idx >= 0 ? std::string("") : x86("mov", "rdi", ROQ(0))
         + x86("call", "rt_proc_fn", TEMPLATE_FN_ADDR(rt_proc_fn)))
         + IF(gi_idx < 0 && bcps_pl(), bcps_pinned_byname_road(bcps_block_build(argblks, (int)_.op_ival), act + 8, 61, 62, 60))
         + IF(icn_gen_regime(),  x86("sub", "rsp", 8L) + x86_rsp_store64_imm(0, 0))
         + bcps_wire_cross_gen(3, 4)
         + x86("def", L(3))
         + (bcps_pl()
            ?  x86("mov", FRQ(act), "rax")
              + x86("mov", FRQ(act + 8), "rdx")
              + x86("test", "rax", "rax")
              + x86("jne", L(21))
              + x86("add", "rsp", 32L)
              + x86("def", L(21))
              + x86("call", "rt_gen_spine_pass_γ", TEMPLATE_FN_ADDR(rt_gen_spine_pass_γ))
              + x86("jmp", L(2))
            : icn_gen_regime()
            ?
               x86("cmp", "al", (long)DT_FAIL)
              + x86("je", L(8))
              + x86("mov", "rdi", RDQ("rdx", 0 - n2_ftc))
              + x86("mov", "rsi", RDQ("rdx", 8 - n2_ftc))
              + x86("mov", FRQ(act + 8), "rdx")
              + x86("jmp", L(9))
              + x86("def", L(8))
              + x86("mov32", "edi", (long)DT_FAIL)
              + x86("mov32", "esi", 0L)
              + x86("mov", FRQ(act + 8), "rsp")
              + x86("def", L(9))
            : bcps_wire_land(_.op_sval)
              + (x86("mov", FRQ(act + 8), "rsp")
                 + x86("add", "rsp", 16L)))
         + IF(!bcps_pl(),
           x86("mov", "rax", FRQ(act))
         + x86("test", "rax", "rax")
         + x86("jne", L(5))
         + x86("mov", FRQ(act), 1L)
         + x86("call", "rt_proc_call_epilogue_γ", TEMPLATE_FN_ADDR(rt_proc_call_epilogue_γ))
         + x86("jmp", L(2))
         + x86("def", L(5))
         + x86("call", "rt_gen_spine_pass_γ", TEMPLATE_FN_ADDR(rt_gen_spine_pass_γ))
         + x86("jmp", L(2)))
         + x86("def", L(4))
         + (bcps_pl()
            ?  x86("add", "rsp", 32L)
              + x86("mov", FRQ(act), 0L)
              + x86("call", "rt_gen_spine_pass_ω", TEMPLATE_FN_ADDR(rt_gen_spine_pass_ω))
              + x86("jmp", L(2))
            : bcps_wire_land(_.op_sval)
         + x86("add", "rsp", icn_gen_regime() ? 8L : 16L)
         + x86("mov", "rax", FRQ(act))
         + x86("test", "rax", "rax")
         + x86("jne", L(6))
         + x86("mov", FRQ(act), 1L)
         + x86("call", "rt_proc_call_epilogue_ω", TEMPLATE_FN_ADDR(rt_proc_call_epilogue_ω))
         + x86("jmp", L(2))
         + x86("def", L(6))
         + x86("call", "rt_gen_spine_pass_ω", TEMPLATE_FN_ADDR(rt_gen_spine_pass_ω))
         + x86("jmp", L(2)))
         + x86("def", L(1))
         + bcps_undef_fallback(TEMPLATE_FN_ADDR(rt_ab_undef_fn_stub))
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
         + IF(!bcps_pl(), x86("call", "rt_gen_spine_resume_enter", TEMPLATE_FN_ADDR(rt_gen_spine_resume_enter)))
         + ((bcps_pl()
               ?  x86("test", "r15", "r15")
                 + x86("jne", L(22))
                 + x86("mov", "rax", FRQ(act))
                 + x86("test", "rax", "rax")
                 + x86("je", L(22))
                 + x86("mov", "rcx", FRQ(act + 8))
                 + x86("mov", x86_fb(), "rax")
                 + x86("call", "rt_gen_spine_resume_enter", TEMPLATE_FN_ADDR(rt_gen_spine_resume_enter))
                 + x86_jmp_reg("rcx")
                 + x86("def", L(22))
                 + x86_omega()
               : icn_gen_regime()
               ?  x86("mov", "rax", FRQ(act + 8))
                 + x86("mov", "rsp", RDQ("rax", 40))
                 + x86_jmp_mem("rax", 32)
               : x86("mov", "rsp", FRQ(act + 8))
                 + x86_jmp_mem("rsp", 0)))
         + RO_SEAL_STR(0, _.op_sval ? _.op_sval : "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_proc_staged_str(IR_t * pBB) {
    int is_gen = _.op_sval && rt_proc_is_generator(_.op_sval);
    if (is_gen && _.op_node_kind != (int)IR_PROC_GEN && _.op_node_kind != (int)IR_CALL_PROC_STAGED) return x86_alpha()
        + x86_bomb("bb_call_proc_staged: generator call on an op kind without a callgen.act RSP-carve handle grant (zeta_storage.c widens only IR_PROC_GEN / IR_CALL_PROC_STAGED)");
    if (is_gen) return bcps_spine_gen_arm();
    return bcps_det_arm();
}
extern "C" int zls_g_det_block(const IR_graph_t * g);
extern "C" int bb_proc_target_det_block(const char *fname) { if (!fname) return -1; for (int i = 0; i < g_stage2.proc_count; i++) { if (!g_stage2.proc_table[i].name
    || strcmp(g_stage2.proc_table[i].name, fname)) continue; int bi = g_stage2.proc_table[i].bb_idx; IR_graph_t * cg = (bi >= 0
        && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0; return (cg && zls_g_det_block(cg)) ? cg->nparams : -1; } return -1; }
static std::string bcps_det_block_arm(int zres, int off, int bidx, IR_graph_t ** argblks, long idx) {
    int n = (int)_.op_ival; long bias = 16L + 16L * n;
    auto zop = [&](int i, int w) -> std::string { int in = i >= 0 && i < _.op_zcap; if (in && _.op_zread_xf[i] != -1) return RDQ("rbp", _.op_zread_xf[i]
        + w); return x86_zref((in ? _.op_zread[i] : 0) + w + (int)bias, 1); };
    std::string s = x86_alpha() + x86_scan_sync_out() + x86_anchor_enter()
         + x86("note",
             "the block protocol for a deterministic Pascal callee (CEO-1491): the two wire cells, then the argument block "
                 "on this spine, then a jump; no staged medium, no registry call, no C prologue or epilogue")
         + x86_lea_id("rcx", 4) + x86("push", "rcx") + x86_lea_id("rcx", 3)
         + x86("push", "rcx")
         + x86("sub", "rsp", (long)(16 * n))
         + FOR(0, n, [&](int i) {
               if (zres) return x86("note", ZOPN(i))
                              + x86("mov", "rax", zop(i, 0)) + x86_rsp_store64(16 * i, "rax")
                              + x86("note", ZOPN(i))
                              + x86("mov", "rax", zop(i, 8)) + x86_rsp_store64(16 * i + 8, "rax");
               int slot = bcps_arg_slot(_.node, argblks, i);
               return x86("note", std::string("block A") + std::to_string(i))
                    + x86("mov", "rax", FRQB(slot, (int)bias)) + x86_rsp_store64(16 * i, "rax")
                    + x86("mov", "rax", FRQB(slot + 8, (int)bias)) + x86_rsp_store64(16 * i + 8, "rax"); })
         + x86_lea_id("rcx", 3) + x86_lea_id("rdx", 4)
         + bcps_jmp_proc_fn(idx)
         + x86("def", L(3))
         + x86("mov", "rax", "rdi")
         + x86("mov", "rdx", "rsi")
         + x86("jmp", L(2))
         + x86("def", L(4))
         + x86("mov32", "eax", (long)DT_FAIL)
         + x86("xor", "edx", "edx")
         + x86("def", L(2))
         + x86_anchor_leave()
         + x86_scan_sync_in_rr()
         + (zres ? x86("note", ZRESN())
         + x86("mov", ZRES(0), "rax")
         + x86("note", ZRESN())
         + x86("mov", ZRES(8), "rdx")
                 : x86("mov", FRQ(off), "rax")
                 + x86("mov", FRQ(off + 8), "rdx"))
         + x86("cmp", "al", (long)DT_FAIL)
         + x86_omega("je")
         + x86_gamma()
         + x86_beta()
         + (bidx < 0 ? x86_omega() : x86_pair_jmp(bidx))
         + RO_SEAL_STR(0, _.op_sval ? _.op_sval : "");
    return s;
}
