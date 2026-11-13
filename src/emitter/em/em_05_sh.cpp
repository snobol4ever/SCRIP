#include "emitter/em/em_head.h"
#include "emitter/emit_state.h"
#include "emitter/emit_gc_site.h"
#include "runtime/core/g_runtime_eval.h"
extern "C" {
#include "templates/x86/x86_frame.h"
}
extern "C" {
#include "templates/x86/x86_gc.h"
}
extern "C" {
#include "templates/x86/x86_rsp_tracker.h"
}
#include "emitter/em/em_01_sh.h"
#include "emitter/em/em_03_sh.h"
#include "emitter/em/em_04_sh.h"
#include "emitter/em/em_05_sh.h"
extern "C" {
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int cap_ok_at(IR_t * base, int bi) { for (int _j = 0; _j < g_emit_cfg->n; _j++) { IR_t * _o = g_emit_cfg->all[_j]; if (_o && _o->op == IR_MATCH_ARBNO && _o->n_operands >= 3) { int _a = -1,
    _b = -1; for (int _k = 0; _k < g_emit_cfg->n; _k++) { if (g_emit_cfg->all[_k] == _o->operands[1]) _a = _k; if (g_emit_cfg->all[_k] == _o->operands[2]) _b = _k; } if (_a > _b) { int _t = _a;
    _a = _b; _b = _t; } if (_a >= 0 && bi >= _a && bi <= _b) return 0; } } (void)base; return 1; }
const char * sr3_gamma_label(const IR_t * g) { if (g_emitter.sr3_gamma_label__off < 0) { const char * e = getenv("SCRIP_SR3_FIX"); g_emitter.sr3_gamma_label__off = (e && *e == '0') ? 1 : 0;
    } if (getenv("SCRIP_SR3_CENSUS")) fprintf(stderr, "SR3 gamma op=%d\n", (int)g->op); if (!g_emitter.sr3_gamma_label__off && g->op != IR_GOTO_DEFERRED) return (const char *)0; return IR_LIT(g).sval;
        }
int gc_sites_report_on(void) { if (g_emitter.gc_sites_report_on_v < 0) { const char * e = getenv("SCRIP_GC_SITES_REPORT"); g_emitter.gc_sites_report_on_v = (e && *e == '1') ? 1 : 0;
    } return g_emitter.gc_sites_report_on_v; }
static int gc_site_rule_pick(int adj, int * depth) {
    if (g_emit.flat_pat && blob_frame_bytes() > 0) { *depth = 0; return 3; }
    if (x86_hb_on()) { *depth = 0; return 1; }
    if (g_emit.flat_gen_rbp > 0) { *depth = g_emit.flat_gen_rbp; return 2; }
    if (adj == X86_ADJ_ALIGNED) { *depth = g_emit.op_zdepth + g_emit.op_vdj_live; return 4; }
    *depth = g_emit.op_zrun + g_emit.op_zxb + adj + g_emit.op_vdj_live;
    return 0;
}
}
extern "C++" int emit_gc_site_rsp_based(void) { int d; return !g_emit.site_shim9 && gc_site_rule_pick(0, &d) == 0; }
extern "C++" int emit_gc_site_new(int kind, int adj) {
    if (!g_emitter.g_gc_sites_armed) return -1;
    int rule, depth;
    rule = gc_site_rule_pick(adj, &depth);
    if (g_emit.site_shim9) { rule = 9; depth = (int)(g_emit.site_shim9 - 1); }
    gc_site_t * e = &CV_PUSH(g_emitter.g_gc_sites_v, gc_site_t); e->pc = 0; e->depth = depth; e->kind = (uint16_t)kind; e->rule = (uint16_t)rule; e->zdepth = g_emit.op_zdepth;
        e->fcb = (int32_t)g_emit.op_fc_bytes; e->fcbase = (int32_t)g_emit.op_fc_base; e->demit = depth; e->bk = x86_boxkind(); e->mn = 0; e->nd = g_emit.node;
    e->wk_off = -1; e->wk_n = 0; e->wk_e = 0; e->wk_pre = 0; e->dk = -1;
    if (rule == 1 || rule == 2) e->dk = g_emit.op_zrun + g_emit.op_zxb + adj + g_emit.op_vdj_live;
    if ((rule == 0 || rule == 4 || rule == 1 || rule == 2) && g_emit.op_wk_off >= 0 && g_emit.op_wk_pool && !(g_emit.op_wk_pre & 7)) {
        const uint64_t * blk = g_emit.op_wk_pool + g_emit.op_wk_off;
        int sh = g_emit.op_wk_pre / 8;
        auto it = g_emitter.g_gc_wk_blk.find(blk);
        if (it == g_emitter.g_gc_wk_blk.end()) {
            int nw = (int)g_emit.op_wk_pool[g_emit.op_wk_off - 1], wo = sh / 64, lo = sh % 64, tw = nw + wo + (lo ? 1 : 0);
            CV_PUSH(g_emitter.g_gc_sites_wk, uint64_t) = (uint64_t)tw;
            int base = (int)g_emitter.g_gc_sites_wk.len;
            for (int q = 0; q < tw; q++) CV_PUSH(g_emitter.g_gc_sites_wk, uint64_t) = 0;
            for (int q = 0; q < nw; q++) {
                CV_AT(g_emitter.g_gc_sites_wk, uint64_t, base + q + wo) |= blk[q] << lo;
                if (lo) CV_AT(g_emitter.g_gc_sites_wk, uint64_t, base + q + wo + 1) |= blk[q] >> (64 - lo);
            }
            it = g_emitter.g_gc_wk_blk.emplace(blk, base).first;
        }
        e->wk_off = (int32_t)it->second; e->wk_n = g_emit.op_wk_n + sh; e->wk_e = g_emit.op_wk_e; e->wk_pre = g_emit.op_wk_pre; }
    return (int)g_emitter.g_gc_sites_v.len - 1;
}
extern "C" {
void emit_gc_rsp_reset(void) { x86_rsp_reset_all(); }
}
extern "C++" int emit_gc_site_raw(int kind, int rule, int depth) {
    if (!g_emitter.g_gc_sites_armed) return -1;
    gc_site_t * e = &CV_PUSH(g_emitter.g_gc_sites_v, gc_site_t); e->pc = 0; e->depth = depth; e->kind = (uint16_t)kind; e->rule = (uint16_t)rule; e->zdepth = 0; e->fcb = 0; e->fcbase = 0;
        e->demit = depth;
        e->bk = x86_boxkind(); e->mn = 0; e->nd = g_emit.node; e->wk_off = -1; e->wk_n = 0; e->wk_e = 0; e->wk_pre = 0; e->dk = -1;
    return (int)g_emitter.g_gc_sites_v.len - 1;
}
extern "C" {
static int emit_gc_match_reaches(const IR_t * b, const IR_t * nd) {
    int cap = (g_emit_cfg && g_emit_cfg->n > 0) ? g_emit_cfg->n : 0, n = 0, h = 0;
    if (!cap || !b || !nd || !b->γ.node) return 0;
    const IR_t * q[cap + 1];
    q[n++] = b->γ.node;
    while (h < n) {
        const IR_t * x = q[h++];
        if (x == nd) return 1;
        if (x == b || x->op == IR_MATCH_END || !ir_is_matcher(x->op)) continue;
        const IR_t * nx[2] = { x->γ.node, x->ω.node };
        for (int k = 0; k < 2 + x->n_operands; k++) {
            const IR_t * y = k < 2 ? nx[k] : x->operands[k - 2];
            int seen = 0;
            if (!y) continue;
            for (int i = 0; i < n && !seen; i++) seen = (q[i] == y);
            if (!seen && n < cap) q[n++] = y;
        }
    }
    return 0;
}
static const gc_site_t * emit_gc_match_owner(const IR_t * nd, int * levels) {
    const gc_site_t * outer = (const gc_site_t *)0;
    *levels = 0;
    for (uint32_t i = 0; i < g_emitter.g_gc_sites_v.len; i++) {
        const gc_site_t * m = &CV_AT(g_emitter.g_gc_sites_v, gc_site_t, i);
        if (m->kind != X86_SITE_MATCH_ENTER || !m->mn || !m->nd || !emit_gc_match_reaches(m->nd, nd)) continue;
        (*levels)++;
        if (!outer) outer = m;
    }
    return outer;
}
}
extern "C++" void emit_gc_site_play(int idx, const void * pc, int delta, int aligned, int unknown) {
    if (idx < 0 || (uint32_t)idx >= g_emitter.g_gc_sites_v.len) return;
    gc_site_t * e = &CV_AT(g_emitter.g_gc_sites_v, gc_site_t, idx);
    if (pc) e->pc = (uint64_t)(uintptr_t)pc;
    else e->pc = ++g_emitter.g_gc_site_play_seq;
    if (e->kind == X86_SITE_MATCH_ENTER || e->kind == X86_SITE_MATCH_LEAVE) e->dk = -1;
    if (e->kind == X86_SITE_MATCH_ENTER) { int ek = e->rule == 3 ? 3 : (e->rule == 1 ? 1 : (e->rule == 0 && !aligned && !unknown ? 0 : 5)); int ed = e->rule == 0 ? e->depth + delta : 0;
        if (g_emitter.g_gc_match_n == 0) { g_emitter.g_gc_match_out.enc_kind = ek; g_emitter.g_gc_match_out.enc_depth = ed; g_emitter.g_gc_match_out.nd = e->nd; } g_emitter.g_gc_match_n++;
            e->depth = ed; e->mn = ek + 1; return; }
    if (e->kind == X86_SITE_MATCH_LEAVE) { if (g_emitter.g_gc_match_n > 0) g_emitter.g_gc_match_n--; return; }
    if (e->rule == 9) {
        const x86_rsp_st_t & K = x86_rsp_S();
        int m9 = K.mark9_set ? K.mark9 : K.mark, m9u = K.mark9_set ? K.mark9_unknown : K.mark_unknown;
        int d = delta - m9 - 16;
        if (aligned || unknown || m9u || d < 0 || (d & 7) || d / 8 > 255 || (e->kind & 0xFF00)) {
            fprintf(stderr,
                "FATAL emit_gc_site_play: a rule-9 SHIM-SELF site in %s needs a known distance from R to the shim's base, a multiple of 8 of at most 255 slots (delta=%d mark=%d d=%d al=%d unk=%d mar"
                "k_unk=%d)\n",
                g_emit.flat_fam ? g_emit.flat_fam : "chain", delta, K.mark, d, aligned, unknown, K.mark_unknown);
            abort();
        }
        e->kind = (uint16_t)((e->kind & 255) | ((d / 8) << 8));
    }
    if (e->rule == 0) { e->depth += delta; if (aligned) e->rule = 4; if (unknown) e->rule = 5; }
    else if (e->rule == 1 || e->rule == 2) e->dk = (aligned || unknown || e->dk < 0) ? -1 : e->dk + delta;
    if (g_emitter.g_gc_match_n > 0 && (e->rule == 0 || e->rule == 1 || e->rule == 3 || e->rule == 5) &&
        (ir_is_matcher(e->nd ? (int)e->nd->op : 0) || emit_gc_match_reaches(g_emitter.g_gc_match_out.nd, e->nd))) {
        const gc_match_lvl_t & o = g_emitter.g_gc_match_out;
        if (o.enc_kind == 5) e->rule = 5; else { e->rule = 6; e->kind = (uint16_t)((e->kind & 255) | ((g_emitter.g_gc_match_n & 15) << 8) | ((o.enc_kind & 15) << 12)); e->depth = o.enc_depth; } }
    else if (g_emitter.g_gc_match_n == 0 && e->nd && ir_is_matcher(e->nd->op) && e->nd->op != IR_MATCH_BEGIN && e->nd->op != IR_MATCH_END
        && (e->rule == 0 || e->rule == 1 || e->rule == 3 || e->rule == 5)) {
        int lv = 0;
        const gc_site_t * o = emit_gc_match_owner(e->nd, &lv);
        if (o) {
            int ek = o->mn - 1;
            if (ek == 5) e->rule = 5;
            else { e->rule = 6; e->kind = (uint16_t)((e->kind & 255) | ((lv & 15) << 8) | ((ek & 15) << 12)); e->depth = o->depth; }
        }
    }
    if (gc_sites_report_on())
        fprintf(stderr, "[GC-SITE] %s #%d kind=%d rule=%d zdepth=%d fc=%d/%d zrun=%d delta=%d al=%d unk=%d -> depth=%d off=%ld buf=%p box=%s matchn=%d pc=%p nd_op=%d nd_idx=%d \n",
        g_emit.flat_fam ? g_emit.flat_fam : "chain", idx, (int)e->kind, (int)e->rule, (int)e->zdepth, (int)e->fcb, (int)e->fcbase, (int)e->demit, delta, aligned, unknown, (int)e->depth,
        pc ? (long)((const char *)pc - (const char *)bb_emit_buf) : -1L, (const void *)bb_emit_buf, e->bk ? e->bk : "?", e->mn, pc, e->nd ? (int)e->nd->op : -1,
        [&]() { if (!g_emit_cfg || !e->nd) return -1; for (int q = 0; q < g_emit_cfg->n; q++) if (g_emit_cfg->all[q] == e->nd) return q; return -2; }());
}
extern "C++" std::string emit_gc_site_label(int idx) { return std::string(".Lgcsite_") + (g_emit.flat_fam ? g_emit.flat_fam : "chain") + "_" + std::to_string(idx); }
extern "C" {
extern "C" int emit_gc_sites_last_off(void) { return g_emitter.g_gc_sites_last_off; }
extern "C" int emit_gc_sites_offs_n(void) { return (int)g_emitter.g_gc_sites_offs_v.len; }
extern "C" int emit_gc_sites_off(int k) { return (k >= 0 && (uint32_t)k < g_emitter.g_gc_sites_offs_v.len) ? CV_AT(g_emitter.g_gc_sites_offs_v, int, k) : -1; }
extern "C" int emit_gc_sites_names_n(void) { return g_emitter.g_gc_sites_names_n; }
extern "C" const char * emit_gc_sites_name(int k) { return (k >= 0 && k < g_emitter.g_gc_sites_names_n) ? CV_AT(g_emitter.g_gc_sites_names_v, char *, k) : "?"; }
static int emit_gc_site_nw(const gc_site_t * e) {
    if (e->wk_off < 0) return 0;
    int D = (e->rule == 0 || e->rule == 4) ? e->depth : ((e->rule == 1 || e->rule == 2) ? e->dk : -1);
    if (D < 0 && e->rule != 0 && e->rule != 4) return 0;
    int delta = D - e->wk_pre - e->wk_e;
    if (D <= 0 || (D & 7) || delta < 0 || (delta & 7) || (e->wk_pre & 7)) return 0;
    return e->wk_n < D / 8 ? e->wk_n : D / 8;
}
static uint64_t emit_gc_site_anchor(const gc_site_t * e) {
    if ((e->rule == 1 || e->rule == 2) && (e->kind & 255) != 1) return 0u;
    int nw = emit_gc_site_nw(e);
    if (nw <= 0) return 0u;
    return (uint64_t)(((e->rule == 0 || e->rule == 4) ? e->depth : e->dk) / 8);
}
static uint64_t emit_gc_site_wkword(const gc_site_t * e, size_t q, int nw) {
    if ((int)(q * 64) >= nw) return 0ull;
    uint64_t v = CV_AT(g_emitter.g_gc_sites_wk, uint64_t, (size_t)e->wk_off + q);
    return (int)(q * 64 + 64) > nw ? v & ((1ull << (nw - (int)(q * 64))) - 1ull) : v;
}
static uint64_t emit_gc_site_third(const gc_site_t * e, int n, std::vector<uint64_t> & pool, std::unordered_map<int, uint64_t> & blkpool, const std::unordered_map<int, int> & maxnw) {
    if ((e->rule == 1 || e->rule == 2) && (e->kind & 255) != 1) return 0;
    int nw = emit_gc_site_nw(e);
    if (e->rule == 0 || e->rule == 4) {
        if (nw <= 0) return 0;
        if (e->depth <= 512) return emit_gc_site_wkword(e, 0, nw);
        uint64_t t = 32u + 32u * (uint64_t)n + 8u * (uint64_t)pool.size();
        for (int q = 0; q < (e->depth / 8 + 63) / 64; q++) pool.push_back(emit_gc_site_wkword(e, (size_t)q, nw));
        return t;
    }
    if (nw <= 0) return 0;
    auto it = blkpool.find(e->wk_off);
    if (it == blkpool.end()) {
        int mx = maxnw.find(e->wk_off)->second;
        uint64_t t = 32u + 32u * (uint64_t)n + 8u * (uint64_t)pool.size();
        for (int q = 0; q < (mx + 63) / 64; q++) pool.push_back(emit_gc_site_wkword(e, (size_t)q, mx));
        it = blkpool.emplace(e->wk_off, t).first;
    }
    return it->second | ((uint64_t)nw << 32);
}
static void emit_gc_site_thirds(int n, const int * ord, std::vector<uint64_t> & pool, std::vector<uint64_t> & thirds) {
    std::unordered_map<int, uint64_t> blkpool;
    std::unordered_map<int, int> maxnw;
    thirds.assign((size_t)(n > 0 ? n : 0), 0u);
    for (int k = 0; k < n; k++) { const gc_site_t * e = &CV_AT(g_emitter.g_gc_sites_v, gc_site_t, ord[k]);
        if ((e->rule == 1 || e->rule == 2) && (e->kind & 255) == 1) { int nw = emit_gc_site_nw(e); if (nw > 0 && nw > maxnw[e->wk_off]) maxnw[e->wk_off] = nw; } }
    for (int k = 0; k < n; k++) { int i = ord[k]; gc_site_t * e = &CV_AT(g_emitter.g_gc_sites_v, gc_site_t, i);
        thirds[(size_t)i] = emit_gc_site_third(e, n, pool, blkpool, maxnw);
        if (thirds[(size_t)i] && gc_sites_report_on())
            fprintf(stderr, "[GC-MASK] %s #%d op=%s rule=%d depth=%d known=%d arrival=%d pre=%d words=%d anchor=%llu third=%llx\n", g_emit.flat_fam ? g_emit.flat_fam : "chain", i,
                e->nd ? bb_op_name(e->nd->op) : "?",
                (int)e->rule, (int)e->depth, (int)e->dk, e->wk_e, e->wk_pre, emit_gc_site_nw(e), (unsigned long long)emit_gc_site_anchor(e), (unsigned long long)thirds[(size_t)i]); }
}
void emit_gc_sites_data(const char * fam, int frameless) {
     int n = (int)g_emitter.g_gc_sites_v.len; bb_label_t tl; emit_label_initf(&tl, ".Lgcsites_%s_%d", g_emit.flat_fam ? g_emit.flat_fam : "chain", g_emitter.emit_gc_sites_data_seq++);
    if (frameless && n == 0) { g_emitter.g_gc_sites_last_off = -1; return; }
    const uint64_t sig = g_rt_eval.rt_fragment_emit ? 0u : (uint64_t)1 << 32;
    const uint64_t ckt = frameless ? ((xa_flat_class_c_pred()) ? ((uint64_t)g_emit.flat_frame_bytes | sig) : 0u) : g_emitter.g_gc_map_link;
    if (g_is_text) {
        std::string t =
            std::string(".pushsection .data.rel.ro,\"aw\",@progbits\n .p2align 3\n") + tl.name + ":\n .quad " + std::to_string(n) + "\n .quad " +
                (frameless ? std::string("0") : std::string(g_emitter.g_gc_map_lbl.name)) + "\n .quad " + std::to_string(ckt) +
                "\n .quad "
            + (g_emit.gc_code_lbl.name ? std::string(g_emit.gc_code_lbl.name) : std::string("0")) + "\n";
            emit_text_n(t.data(), t.size());
        int ord[n > 0 ? n : 1];
        for (int i = 0; i < n; i++) ord[i] = i;
        std::stable_sort(ord, ord + n, [](int a, int b) { return CV_AT(g_emitter.g_gc_sites_v, gc_site_t, a).pc < CV_AT(g_emitter.g_gc_sites_v, gc_site_t, b).pc; });
        std::vector<uint64_t> pool, thirds;
        emit_gc_site_thirds(n, ord, pool, thirds);
        for (int k = 0; k < n; k++) { int i = ord[k]; gc_site_t * e = &CV_AT(g_emitter.g_gc_sites_v, gc_site_t, i);
            uint64_t w = (uint64_t)e->kind | ((uint64_t)e->rule << 16) | ((uint64_t)(uint32_t)e->depth << 32), third = thirds[(size_t)i];
            t = " .quad " + emit_gc_site_label(i) + (g_emit.gc_code_lbl.name ? std::string("-") + g_emit.gc_code_lbl.name : std::string()) + "\n .quad "
                + std::to_string((unsigned long long)w) + "\n .quad " + std::to_string((unsigned long long)third) + "\n .quad " + std::to_string((unsigned long long)emit_gc_site_anchor(e)) + "\n"; emit_text_n(t.data(), t.size()); }
        for (size_t q = 0; q < pool.size(); q++) { t = " .quad " + std::to_string((unsigned long long)pool[q]) + "\n"; emit_text_n(t.data(), t.size()); }
        emit_textf(".popsection\n");
        g_emitter.g_gc_sites_last_off = 0;
    } else {
        emit_label_define_bb(&tl); g_emitter.g_gc_sites_last_off = tl.offset; CV_PUSH(g_emitter.g_gc_sites_offs_v, int) = tl.offset;
        bb_emit_u64((uint64_t)n); bb_emit_u64(frameless ? (uint64_t)0 : (uint64_t)(uintptr_t)(bb_emit_buf + g_emitter.g_gc_map_lbl.offset)); bb_emit_u64(ckt);
        bb_emit_u64(g_emit.gc_code_lbl.name ? (uint64_t)(uintptr_t)(bb_emit_buf + g_emit.gc_code_lbl.offset) : (uint64_t)0);
        if (n > 1)
            qsort(&CV_AT(g_emitter.g_gc_sites_v, gc_site_t, 0), (size_t)n, sizeof(gc_site_t), [](const void * a, const void * b) -> int { uint64_t x = ((const gc_site_t *)a)->pc,
                y = ((const gc_site_t *)b)->pc;
            return (x > y) - (x < y); });
        std::vector<uint64_t> pool, thirds;
        int idn[n > 0 ? n : 1];
        for (int i = 0; i < n; i++) idn[i] = i;
        emit_gc_site_thirds(n, idn, pool, thirds);
        for (int i = 0; i < n; i++) { gc_site_t * e = &CV_AT(g_emitter.g_gc_sites_v, gc_site_t, i); uint64_t w = (uint64_t)e->kind | ((uint64_t)e->rule << 16) | ((uint64_t)(uint32_t)e->depth << 32),
            third = thirds[(size_t)i];
            bb_emit_u64(e->pc && g_emit.gc_code_lbl.name ? e->pc - (uint64_t)(uintptr_t)(bb_emit_buf + g_emit.gc_code_lbl.offset) : e->pc); bb_emit_u64(w); bb_emit_u64(third); bb_emit_u64(emit_gc_site_anchor(e)); }
        for (size_t q = 0; q < pool.size(); q++) bb_emit_u64(pool[q]);
    }
    CV_PUSH(g_emitter.g_gc_sites_names_v, char *) = ct_strdup(tl.name); g_emitter.g_gc_sites_names_n++;
    g_emitter.g_gc_sites_v.len = 0;
    g_emitter.g_gc_sites_wk.len = 0;
    g_emitter.g_gc_wk_blk.clear();
    (void)fam;
}
}
int xa_flat_class_c(void) {
    if (!g_emit.flat_jmp_entry) { if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[CLASS-C] nid=%d bail=no_jmp_entry\n", g_emit.nid); return 0; }
    if (g_emit.flat_pat || g_emit.flat_gen || g_emit.flat_lcl_proc || g_emit.zframe_graph) {
        if (getenv("SCRIP_FLOOR_DIAG"))
            fprintf(stderr, "[CLASS-C] nid=%d bail=pat/gen/lcl/zframe pat=%d gen=%d lcl=%d zframe=%d\n", g_emit.nid, g_emit.flat_pat, g_emit.flat_gen, g_emit.flat_lcl_proc, g_emit.zframe_graph);
        return 0; }
    { extern int g_flat_frame_floor; if (g_flat_frame_floor > 0) {
        if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[CLASS-C] nid=%d bail=floor floor=%d\n", g_emit.nid, g_flat_frame_floor);
        return 0; } }
    { int _r = (g_emit.flat_frame_bytes >= 48) ? 1 : 0;
      if (getenv("SCRIP_FLOOR_DIAG")) fprintf(stderr, "[CLASS-C] nid=%d PASS frame_bytes=%d -> %d\n", g_emit.nid, g_emit.flat_frame_bytes, _r);
      return _r; }
}
extern "C" {
extern "C" int xa_flat_class_c_pred(void) { return xa_flat_class_c(); }
}
