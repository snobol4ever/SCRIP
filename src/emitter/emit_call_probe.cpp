#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include "ir/ir_graph.h"
#include "planner/frame_layout.h"
#include "runtime/core/g_runtime.h"
#include "emitter/emit_knobs.h"
#include "emitter/bb_label.h"
#include "emitter/emit_walk.h"
extern "C" {
#include "emitter/emit_state.h"
#include "ir/ct_arena.h"
const char *rt_define_query(const char *, int *, int *, int *, void **);
#include "ir/stage2.h"
#include "ir/ab_abi.h"
#include "templates/bb/bb_templates.h"
int zls_g_resumable_frame(const IR_graph_t * g);
int zls_g_block_args(const IR_graph_t * g);
int zls_g_det_block(const IR_graph_t * g);
const char *bb_ab_sym_name(const char *nm);
int xa_flat_class_c_pred(void);
void *bb_ab_fn_cell_ptr(const char *fname);
int rt_proc_dyn_scope(const char *name);
int rt_proc_is_generator(const char *name);
int rt_proc_is_registered(const char *name);
int rt_proc_is_redefined(const char *name);
int rt_proc_is_variadic(const char *name);
int rt_proc_nformals(const char *name);
int rt_proc_nparams(const char *name);
int rt_proc_pinned(const char *name);
const char *rt_proc_pname(const char *name, int k);
const char *rt_proc_result_name_get(const char *name);
int rt_define_tiny_ok(const char *, int);
int gva_index_of(const char *name);
int rt_gva_add_var(const char *name);
int gva_trace_demoted(void);
void rt_ab_undef_fn_stub(void);
void rt_ab_undef_sig_stub(void);
}
#include "templates/x86/x86_ops.h"
extern "C" int bb_proc_multi_proto(const char *fname) { if (!fname) return 0; { int i = bb_proc_find(fname); if (i >= 0) { int bi = g_stage2.proc_table[i].bb_idx;
    return (bi >= 0 && bi < g_stage2.bbp.count
        && g_stage2.bbp.table[bi]) ? g_stage2.bbp.table[bi]->multi_proto : 0; } } return 0; }
extern "C" int bb_proc_target_pinned_graph(const char *fname) { if (!fname) return 0; { int i = bb_proc_find(fname); if (i >= 0) { int bi = g_stage2.proc_table[i].bb_idx; IR_graph_t * cg = (bi >= 0
        && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0; if (cg) return cg->pf.resumable_frame; } } return rt_proc_pinned(fname); }
extern "C" int bb_proc_target_block_graph(const char *fname) { if (!fname) return 0; { int i = bb_proc_find(fname); if (i >= 0) { int bi = g_stage2.proc_table[i].bb_idx; IR_graph_t * cg = (bi >= 0
        && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0; return (cg && cg->cell_frame && cg->pf.block_args) ? 1 : 0; } } return 0; }
extern "C" int bb_proc_target_icn_block(const char *fname, int *np, int *vari) {
    int i = bb_proc_find(fname);
    if (i < 0) i = g_stage2.proc_count;
    IR_graph_t * cg = (fname && i < g_stage2.proc_count && g_stage2.proc_table[i].bb_idx >= 0 && g_stage2.proc_table[i].bb_idx < g_stage2.bbp.count)
                    ? g_stage2.bbp.table[g_stage2.proc_table[i].bb_idx] : (IR_graph_t *)0;
    if (cg && cg->cell_frame && cg->pf.block_args) { if (np) *np = cg->nparams; if (vari) *vari = g_stage2.proc_table[i].is_variadic; }
    return (cg && cg->cell_frame && cg->pf.block_args) ? 1 : 0;
}
extern "C" int bb_proc_target_zframe_graph(const char *fname) { if (!fname) return 0; { int i = bb_proc_find(fname); if (i >= 0) { int bi = g_stage2.proc_table[i].bb_idx;
    return (bi >= 0 && bi < g_stage2.bbp.count
        && g_stage2.bbp.table[bi]) ? g_stage2.bbp.table[bi]->zframe_graph : 0; } } return 0; }
extern "C" int bb_scc_gk_len(const char *fname) { int np = fname ? rt_proc_nparams(fname) : 0; return (np > 0 ? np : 0) + 1; }
extern "C" int bb_scc_probe(const char *fname, int nargs, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out) {
    int np = 0, nsave = 0, res_gk = -1, scc = 0;
    if (fname && rt_proc_dyn_scope(fname) && !rt_proc_is_generator(fname) && !bb_proc_multi_proto(fname) && !getenv("SCRIP_SCC_OFF")
        && rt_proc_is_registered(fname) && !rt_proc_is_redefined(fname)) {
        np = rt_proc_nparams(fname);
        if (np >= 0 && nargs <= rt_proc_nformals(fname)) {
            const char *rn = rt_proc_result_name_get(fname); int ok = rn ? 1 : 0, sh = 0;
            for (int k = 0; ok && k < np; k++) {
    const char *nm = rt_proc_pname(fname, k);
    if (!nm) { ok = 0; break; }
    { int gk = gva_trace_demoted() ? -1 : gva_index_of(nm); if (gk < 0) gk = rt_gva_add_var(nm); gk_out[nsave++] = gk; if (!strcmp(nm, rn)) sh = 1; }
}
            if (ok) { res_gk = gva_trace_demoted() ? -1 : gva_index_of(rn); if (res_gk < 0) res_gk = rt_gva_add_var(rn); if (!sh) gk_out[nsave++] = res_gk; }
            if (ok) scc = 1;
        }
    }
    if (np_out) *np_out = np; if (nsave_out) *nsave_out = nsave; if (res_gk_out) *res_gk_out = res_gk; return scc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int bb_rt_shim_probe(const char *fname, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out) {
    int np = 0, nsave = 0, res_gk = -1, ok = 0;
    if (fname && rt_proc_is_registered(fname) && rt_proc_dyn_scope(fname) && !rt_proc_is_generator(fname)) {
        np = rt_proc_nparams(fname);
        if (np >= 0) {
            const char *rn = rt_proc_result_name_get(fname); int sh = 0; ok = rn ? 1 : 0;
            for (int k = 0; ok && k < np; k++) {
                const char *nm = rt_proc_pname(fname, k); if (!nm) { ok = 0; break; }
                { int gk = gva_trace_demoted() ? -1 : gva_index_of(nm); if (gk < 0) gk = rt_gva_add_var(nm); gk_out[nsave++] = gk; if (!strcmp(nm, rn)) sh = 1; } }
            if (ok) { res_gk = gva_trace_demoted() ? -1 : gva_index_of(rn); if (res_gk < 0) res_gk = rt_gva_add_var(rn); if (!sh) gk_out[nsave++] = res_gk; }
        }
    }
    if (np_out) *np_out = np; if (nsave_out) *nsave_out = nsave; if (res_gk_out) *res_gk_out = res_gk; return ok;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int bb_tiny_shim_ok(const char *fname, int nargs) {
    static int _nt = -1; if (_nt < 0) {
    const char *e = getenv("SCRIP_NO_TINY");
    _nt = (e && *e == '1') ? 1 : 0;
}
    if (_nt || !fname) return 0;
    if (!rt_define_tiny_ok(fname, nargs)) return 0;
    int np = 0, ns = 0, rg = -1; int gk[bb_scc_gk_len(fname)];
    if (!bb_scc_probe(fname, 0, &np, &ns, gk, &rg)) return 0;
    int nf = rt_proc_nformals(fname);
    if (!(nf >= 0 && nf <= np)) return 0;
    return 1;
}
extern "C" int bb_sig_site_ok(const char *fname, int nargs) {
    static int _nt = -1; if (_nt < 0) {
    const char *e = getenv("SCRIP_NO_TINY");
    _nt = (e && *e == '1') ? 1 : 0;
}
    (void)nargs;
    if (_nt || !fname || !rt_proc_is_registered(fname) || !rt_proc_dyn_scope(fname) || rt_proc_is_generator(fname) || rt_proc_is_variadic(fname)) return 0;
    { int np = rt_proc_nparams(fname); return (np >= 0) ? 1 : 0; }
}
static std::vector<std::string> g_alpha_cellp_seen, g_alpha_cellp_names, g_alpha_shim_emitted;
std::string bcps_alpha_cellp_label(const char * fname) { return std::string("alpha_cellp$") + bb_ab_sym_name(fname); }
std::string bcps_alpha_cellp_data(const char * fname) {
    if (!fname) return std::string();
    { void ** c = (void **) bb_ab_fn_cell_ptr((std::string("alpha$") + fname).c_str()); if (c && (!*c || *c == (void *)(uintptr_t) rt_ab_undef_fn_stub)) *c = (void *)(uintptr_t) rt_ab_undef_sig_stub;
        }
    std::string lbl = bcps_alpha_cellp_label(fname);
    if (!std::count(g_alpha_cellp_seen.begin(), g_alpha_cellp_seen.end(), lbl)) { g_alpha_cellp_seen.push_back(lbl); g_alpha_cellp_names.push_back(std::string(fname)); }
    return std::string();
}
extern "C" void bcps_alpha_shim_note(const char * fname) { if (fname) g_alpha_shim_emitted.push_back(std::string(fname)); }
extern "C" const char * bcps_alpha_cellp_table_c(void) {
    static std::string out; out.clear();
    out += x86("directive", std::string(".section .data"))
         + x86("directive", std::string(".align 8"));
    for (size_t i = 0; i < g_alpha_cellp_names.size(); i++) {
        const std::string & nm = g_alpha_cellp_names[i]; const std::string & lbl = g_alpha_cellp_seen[i];
        std::string init = std::count(g_alpha_shim_emitted.begin(), g_alpha_shim_emitted.end(), nm) ? (nm + "_\xce\xb1") : std::string("rt_ab_undef_sig_stub");
        out += x86("directive", lbl + std::string(":"))
             + x86("directive", std::string(".quad ") + lbl + std::string(" + 8"))
             + x86("directive", std::string(".quad ") + init);
    }
    out += x86("directive", std::string("__alpha_cellp_tab:"));
    for (size_t i = 0; i < g_alpha_cellp_names.size(); i++) out += x86("directive", std::string(".quad .Lalphan") + std::to_string((long)i) + std::string(", ") + g_alpha_cellp_seen[i]);
    out += x86("directive", std::string(".quad 0"));
    out += x86("directive", std::string(".section .rodata"));
    for (size_t i = 0; i < g_alpha_cellp_names.size(); i++) {
    std::string esc; for (char c : g_alpha_cellp_names[i]) { if (c == '"' || c == '\\') esc += '\\'; esc += c; }
        out += x86("directive", std::string(".Lalphan") + std::to_string((long)i) + std::string(": .string \"") + esc + std::string("\"")); }
    out += x86("directive", std::string(".section .text"))
         + x86("directive", std::string(".intel_syntax noprefix"));
    return out.c_str();
}
extern "C" int bb_proc_target_det_block(const char *fname) { if (!fname) return -1; { int i = bb_proc_find(fname); if (i >= 0) { int bi = g_stage2.proc_table[i].bb_idx; IR_graph_t * cg = (bi >= 0
        && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0; return (cg && cg->pf.det_block) ? cg->nparams : -1; } } return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int xa_flat_sig_names(const char * fname, int * nf_out, int * nsave_out, int * gk_out, int * res_gk_out);
static int * sig_gk_room(int ** gk, int * cap, int n) { if (n > *cap) { *gk = (int *)ct_grow(*gk, (size_t)n * sizeof(int)); *cap = n; } return *gk; }
extern "C" void sig_prepare(const char * fname, int nargs) {
    int * gk = sig_gk_room(&g_emit.op_sig_gk, &g_emit.op_sig_gk_cap, bb_scc_gk_len(fname));
    g_emit.op_sig_np = 0; g_emit.op_sig_ns = 0; g_emit.op_sig_rg = -1; g_emit.op_sig_nf = 0; g_emit.op_sig_xt = 0; g_emit.op_sig_nnv = 0; g_emit.op_sig_T = 0; g_emit.op_sig_F = 0;
    g_emit.op_sig_ok = (fname && fname[0]) ? bb_scc_probe(fname, nargs, &g_emit.op_sig_np, &g_emit.op_sig_ns, gk, &g_emit.op_sig_rg) : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void sig_prepare_floater(void) { g_emit.op_frag_release = (g_runtime.eval.rt_fragment_emit && xa_flat_class_c_pred()) ? 1 : 0; }
static void sig_prepare_define_columns(const char * fname, const char * entry, int role) {
    const char * fn6 = fname ? fname : "?"; int cell = 0, tiny = 0;
    if (role == 1 || role == 2) sig_prepare_floater();
    if ((role == 4 || role == 5 || role == 7) && g_emit.op_sig_ok) {
        g_emit.op_sig_lbl_a = emit_label_intern((std::string(fname) + "_\xce\xb1").c_str());
        g_emit.op_sig_lbl_b = emit_label_intern((std::string(fname) + "_\xce\xb3").c_str());
        g_emit.op_sig_lbl_o = emit_label_intern((std::string(fname) + "_\xcf\x89").c_str());
        g_emit.op_sig_blb = ct_strdup(role == 5 ? std::string(entry).c_str() : (std::string("LBL__") + entry).c_str());
        g_emit.op_sig_entry_cell = (uint64_t)(uintptr_t)bb_ab_fn_cell_ptr((std::string("entry$") + fname).c_str());
        g_emit.op_sig_bcell = ct_strdup((std::string("entry_cell$") + std::string(bb_ab_sym_name(fname))).c_str());
    }
    if ((role == 4 || role == 5) && g_emit.op_sig_ok) bcps_alpha_shim_note(fname);
    if (role == 6) {
        cell = bb_ab_cell_addr(fn6) ? 1 : 0; tiny = bb_tiny_shim_ok(fn6, 0);
        g_emit.op_dq_fname = fn6;
        g_emit.op_dq_blbl = g_emit.lbl_t0 ? g_emit.lbl_t0 : "rt_ab_undef_fn_stub";
        g_emit.op_dq_seal_alpha = (emit_knob_unless_zero("SCRIP_M4_ALPHA_SEAL") && !cell && tiny) ? 1 : 0;
        g_emit.op_dq_seal_entry = (g_emit.lbl_t0 && !cell && tiny) ? 1 : 0;
        g_emit.op_dq_bind = (g_emit.lbl_t0 && cell) ? 1 : 0;
        g_emit.op_dq_ent = g_emit.op_dq_bind ? ct_strdup((strncmp(g_emit.lbl_t0, "LBL__", 5) == 0) ? g_emit.lbl_t0 + 5 : g_emit.lbl_t0) : (const char *)0;
        g_emit.op_dq_site_entry = (g_emit.op_proto && strchr(g_emit.op_proto, '|') && g_emit.op_entry && *g_emit.op_entry) ? 1 : 0;
        g_emit.op_dq_alpha_sym = ct_strdup((std::string(fn6) + "_\xce\xb1").c_str());
        g_emit.op_dq_entry_cell_sym = ct_strdup((std::string("entry_cell$") + std::string(bb_ab_sym_name(fn6))).c_str());
    }
}
extern "C" void sig_prepare_define(const char * fname, const char * entry, int role) {
    int rt = role == 7, ok = 0, nf = 0, nnv = 0, np = 0, nf2 = 0, fb = 0; void * fn = 0; const char * csv;
    int * gk = sig_gk_room(&g_emit.op_sig_gk, &g_emit.op_sig_gk_cap, bb_scc_gk_len(fname));
    g_emit.op_sig_np = 0; g_emit.op_sig_ns = 0; g_emit.op_sig_rg = -1;
    if (!entry) entry = fname;
    ok = (fname && entry) ?
        (rt ? bb_rt_shim_probe(fname, &g_emit.op_sig_np, &g_emit.op_sig_ns, gk, &g_emit.op_sig_rg) :
        (bb_tiny_shim_ok(fname, 0) ? bb_scc_probe(fname, 0, &g_emit.op_sig_np, &g_emit.op_sig_ns, gk, &g_emit.op_sig_rg) : 0)) : 0;
    nf = ok ? rt_proc_nformals(fname) : 0;
    g_emit.op_sig_ok = (ok && nf >= 0 && nf <= g_emit.op_sig_np) ? 1 : 0;
    g_emit.op_sig_nf = nf; g_emit.op_sig_xt = g_emit.op_sig_ns - nf; g_emit.op_sig_T = 16L * g_emit.op_sig_xt + 32;
    for (int j = 0; j < g_emit.op_sig_ns; j++) if (gk[j] < 0) nnv++;
    g_emit.op_sig_nnv = nnv; g_emit.op_sig_F = g_emit.op_sig_T + 16L * nf + 16L * nnv;
    csv = rt_define_query(fname ? fname : "?", &np, &nf2, &fb, &fn);
    if (g_emit.op_proto && strchr(g_emit.op_proto, '|')) { nf2 = atoi(g_emit.op_proto); csv = strchr(g_emit.op_proto, '|') + 1; np = *csv ? 1 : 0;
        for (const char * c = csv; *c; c++) if (*c == ',') np++; fn = 0; }
    g_emit.op_dq_np = np; g_emit.op_dq_nf = nf2; g_emit.op_dq_fb = fb; g_emit.op_dq_fn = fn; g_emit.op_dq_csv = csv;
    sig_prepare_define_columns(fname, entry, role);
}
extern "C" void sig_prepare_chain(const char * fname) {
    int * gk = sig_gk_room(&g_emit.op_csig_gk, &g_emit.op_csig_gk_cap, bb_scc_gk_len(fname));
    g_emit.op_csig_nf = 0; g_emit.op_csig_ns = 0; g_emit.op_csig_rg = -1;
    g_emit.op_csig_ok = xa_flat_sig_names(fname, &g_emit.op_csig_nf, &g_emit.op_csig_ns, gk, &g_emit.op_csig_rg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_entry_cell_first(const char * lbl) {
    static std::vector<std::string> seen;
    if (std::count(seen.begin(), seen.end(), std::string(lbl)) > 0) return 0;
    seen.push_back(std::string(lbl));
    return 1;
}
void emit_shim9_arm(long loff, long carve) {
    if (loff > 0xFFFF || carve > 0xFFFF) {
        fprintf(stderr, "FATAL bb_define role 4: a SIG shim frame of %ld bytes cannot be named by a rule-9 SHIM-SELF site (loff and carve must fit 16 bits)\n", carve);
        abort();
    }
    g_emit.site_shim9_saved = g_emit.site_shim9;
    g_emit.site_shim9 = (loff | (carve << 16)) + 1L;
}
std::string emit_shim9_after(std::string s) {
    g_emit.site_shim9 = g_emit.site_shim9_saved;
    return s;
}
