#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "stage2.h"
#include "ab_abi.h"
#include "bb_templates.h"
int zls_g_resumable_frame(const IR_graph_t * g);
int zls_g_block_args(const IR_graph_t * g);
int zls_g_det_block(const IR_graph_t * g);
const char *bb_ab_sym_name(const char *nm);
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
#include "x86_asm.h"
static unsigned bb_proc_hash(const char *s) { unsigned h = 2166136261u; for (; *s; s++) h = (h ^ (unsigned char)*s) * 16777619u; return h; }
static void bb_proc_index_build(stage2_t *s) {
    int cap = 16, nul = 0;
    while (cap < 2 * s->proc_count + 2) cap <<= 1;
    ct_drop(s->proc_ix);
    s->proc_ix = (int *)ct_alloc((size_t)cap * sizeof(int));
    for (int k = 0; k < cap; k++) s->proc_ix[k] = -1;
    s->proc_ix_mask = (unsigned)(cap - 1);
    for (int i = 0; i < s->proc_count; i++) {
        const char *nm = s->proc_table[i].name;
        unsigned h;
        if (!nm) { nul++; continue; }
        h = bb_proc_hash(nm) & s->proc_ix_mask;
        while (s->proc_ix[h] >= 0 && strcmp(s->proc_table[s->proc_ix[h]].name, nm)) h = (h + 1) & s->proc_ix_mask;
        if (s->proc_ix[h] < 0) s->proc_ix[h] = i;
    }
    s->proc_ix_n = s->proc_count;
    s->proc_ix_tbl = s->proc_table;
    s->proc_ix_nul = nul;
}
static int bb_proc_find(const char *fname) {
    stage2_t *s = &g_stage2;
    if (!fname || s->proc_count <= 0) return -1;
    for (int pass = 0; pass < 2; pass++) {
        int i = -1;
        unsigned h;
        if (pass || !s->proc_ix || s->proc_ix_n != s->proc_count || s->proc_ix_tbl != s->proc_table) bb_proc_index_build(s);
        h = bb_proc_hash(fname) & s->proc_ix_mask;
        while (s->proc_ix[h] >= 0) { if (!strcmp(s->proc_table[s->proc_ix[h]].name, fname)) { i = s->proc_ix[h]; break; } h = (h + 1) & s->proc_ix_mask; }
        if (i >= 0 ? (s->proc_table[i].name && !strcmp(s->proc_table[i].name, fname)) : !s->proc_ix_nul) return i;
    }
    for (int i = 0; i < s->proc_count; i++) if (s->proc_table[i].name && !strcmp(s->proc_table[i].name, fname)) return i;
    return -1;
}
extern "C" int bb_proc_multi_proto(const char *fname) { if (!fname) return 0; { int i = bb_proc_find(fname); if (i >= 0) { int bi = g_stage2.proc_table[i].bb_idx;
    return (bi >= 0 && bi < g_stage2.bbp.count
        && g_stage2.bbp.table[bi]) ? g_stage2.bbp.table[bi]->multi_proto : 0; } } return 0; }
extern "C" int bb_proc_target_pinned_graph(const char *fname) { if (!fname) return 0; { int i = bb_proc_find(fname); if (i >= 0) { int bi = g_stage2.proc_table[i].bb_idx; IR_graph_t * cg = (bi >= 0
        && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0; if (cg) return zls_g_resumable_frame(cg); } } return rt_proc_pinned(fname); }
extern "C" int bb_proc_target_retains(const char *fname) {
    if (!fname) return 0;
    {
        int i = bb_proc_find(fname);
        if (i >= 0) {
            int bi = g_stage2.proc_table[i].bb_idx;
            IR_graph_t * cg = (bi >= 0 && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0;
            return (!cg || zls_g_resumable_frame(cg)) ? 1 : 0;
        }
    }
    return 1;
}
extern "C" int bb_proc_target_block_graph(const char *fname) { if (!fname) return 0; { int i = bb_proc_find(fname); if (i >= 0) { int bi = g_stage2.proc_table[i].bb_idx; IR_graph_t * cg = (bi >= 0
        && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0; return (cg && cg->cell_frame && zls_g_block_args(cg)) ? 1 : 0; } } return 0; }
extern "C" int bb_proc_target_icn_block(const char *fname, int *np, int *vari) {
    int i = bb_proc_find(fname);
    if (i < 0) i = g_stage2.proc_count;
    IR_graph_t * cg = (fname && i < g_stage2.proc_count && g_stage2.proc_table[i].bb_idx >= 0 && g_stage2.proc_table[i].bb_idx < g_stage2.bbp.count)
                    ? g_stage2.bbp.table[g_stage2.proc_table[i].bb_idx] : (IR_graph_t *)0;
    if (cg && cg->cell_frame && zls_g_block_args(cg)) { if (np) *np = cg->nparams; if (vari) *vari = g_stage2.proc_table[i].is_variadic; }
    return (cg && cg->cell_frame && zls_g_block_args(cg)) ? 1 : 0;
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
extern "C" int zls_g_det_block(const IR_graph_t * g);
extern "C" int bb_proc_target_det_block(const char *fname) { if (!fname) return -1; { int i = bb_proc_find(fname); if (i >= 0) { int bi = g_stage2.proc_table[i].bb_idx; IR_graph_t * cg = (bi >= 0
        && bi < g_stage2.bbp.count) ? g_stage2.bbp.table[bi] : (IR_graph_t *)0; return (cg && zls_g_det_block(cg)) ? cg->nparams : -1; } } return -1; }
