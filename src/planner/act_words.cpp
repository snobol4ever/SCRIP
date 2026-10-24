#include <stdlib.h>
#include <string.h>
#include <vector>
#include "../emitter/emit_head.h"
#include "../emitter/emit_decls.h"
#include "planner.h"
}
extern "C" int fc_pair_extent(const IR_t * nd);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int act_unbounded_kind(int o) {
    return o == IR_MATCH_DEFER || o == IR_MATCH_VALUE || o == IR_CALL || o == IR_CALL_VALUE || o == IR_MATCH_CALLOUT || o == IR_MATCH_ARBNO || o == IR_DISJUNCTION || o == IR_MATCH_ABORT;
}
static int act_idx(const IR_graph_t * g, const IR_t * nd) { if (!g || !nd) return -1; for (int j = 0; j < g->n; j++) if (g->all[j] == nd) return j; return -1; }
static int act_match_bare(const IR_t * nd) { return nd->op == IR_MATCH_FENCE1 && IR_LIT(nd).ival == SNO_FENCE_LIT_BARE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" const char * planner_act_cause_name(int cause) {
    static const char * const nm[10] = { "-", "deferred_pattern", "static_pattern", "pattern_function", "arbno_iteration", "resume_road", "arm_variance", "bounded_other", "home", "regex_kit" };
    return (cause >= 0 && cause < 10) ? nm[cause] : "?";
}
extern "C" const char * planner_act_class_name(int cls) {
    static const char * const nm[9] = { "-", "arbno", "capture", "fence", "leaf", "choice", "value", "home", "regex" };
    return (cls >= 0 && cls < 9) ? nm[cls] : "?";
}
extern "C" int planner_act_cause(const IR_graph_t * g, const IR_t * nd, int cls, int lo, int hi, int in_arm, int in_repeat, int is_root) {
    if (!nd || cls == ACT_CLASS_NONE) return ACT_NONE;
    if (cls == ACT_CLASS_HOME) return ACT_HOME;
    if (cls == ACT_CLASS_REGEX) return ACT_REGEX_KIT;
    int d = 0, sd = 0, pf = 0, ar = 0, al = 0;
    if (lo > hi) { int t = lo; lo = hi; hi = t; }
    if (g && lo >= 0) for (int j = lo; j <= hi && j < g->n; j++) { const IR_t * m = g->all[j]; if (!m || m == nd) continue; int o = (int)ir_kind0(m);
        if (o == IR_MATCH_DEFER) { if (m->pat_static) sd++; else d++; }
        else if (o == IR_MATCH_VALUE || o == IR_CALL || o == IR_CALL_VALUE || o == IR_MATCH_CALLOUT) pf++;
        else if (o == IR_MATCH_ARBNO) ar++;
        else if (o == IR_MATCH_ALTERNATE || o == IR_DISJUNCTION) al++; }
    if (d) return ACT_DEFERRED_PATTERN;
    if (pf) return ACT_PATTERN_FUNCTION;
    if (ar || in_repeat) return ACT_ARBNO_ITERATION;
    if (sd) return ACT_STATIC_PATTERN;
    if (cls == ACT_CLASS_CHOICE || (cls == ACT_CLASS_FENCE && is_root)) return ACT_RESUME_ROAD;
    if (al || in_arm) return ACT_BOUNDED_ARM;
    return ACT_BOUNDED_OTHER;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int planner_alt_exit(const IR_graph_t * g, const IR_t * alt, int * M);
static int act_depth = 0;
extern "C" int planner_body_live(const IR_graph_t * g, const IR_t * first, const IR_t * last, const IR_t * self) {
    if (!g || !first || !last || act_depth > 64) return -1;
    IR_t * cur = zd_chase((IR_t *)first); int live = 0, guard = 0; act_depth++;
    while (cur && guard++ <= g->n) {
        if (cur == self) { act_depth--; return live; }
        { int o = (int)ir_kind0(cur);
            if (act_unbounded_kind(o)) { act_depth--; return -1; }
            if (o == IR_MATCH_ALTERNATE) { int M = 0; if (planner_alt_exit(g, cur, &M) != 0) { act_depth--; return -1; } }
            live += zd_k(cur); }
        if (cur == last) { act_depth--; return live; }
        cur = zd_chase(cur->γ.node); }
    act_depth--; return -1;
}
extern "C" int planner_blob_exit_live(const IR_graph_t * g) {
    if (!g || !(g->entry || g->body_root) || act_depth > 64) return -1;
    IR_t * cur = zd_chase(g->entry ? g->entry : g->body_root); int live = 0, guard = 0; act_depth++;
    while (cur && guard++ <= g->n) { int o = (int)ir_kind0(cur);
        if (o == IR_SUCCEED) { act_depth--; return live; }
        if (o == IR_FAIL) break;
        if (act_unbounded_kind(o)) break;
        if (o == IR_MATCH_ALTERNATE) { int M = 0; if (planner_alt_exit(g, cur, &M) != 0) break; }
        live += zd_k(cur); cur = zd_chase(cur->γ.node); }
    act_depth--; return -1;
}
extern "C" int planner_alt_k(const IR_graph_t * g, const IR_t * nd) { int M = 0; return planner_alt_exit(g, nd, &M) == 0 ? 32 + M : 48; }
extern "C" int planner_alt_arm_live(const IR_graph_t * g, const IR_t * alt, int j) {
    if (!alt || 2 * j + 1 >= alt->n_operands) return 0;
    return planner_body_live(g, alt->operands[2 * j], alt->operands[2 * j + 1], alt);
}
extern "C" int planner_alt_exit(const IR_graph_t * g, const IR_t * alt, int * M) {
    *M = 0; if (!alt || alt->op != IR_MATCH_ALTERNATE) return -1;
    for (int j = 0; 2 * j + 1 < alt->n_operands; j++) { int l = planner_alt_arm_live(g, alt, j); if (l < 0) return -1; if (l > *M) *M = l; }
    return 0;
}
extern "C" int planner_fence_k(const IR_graph_t * g, const IR_t * nd) {
    if (!nd || nd->op != IR_MATCH_FENCE1 || act_match_bare(nd) || nd->n_operands < 2) return 0;
    return planner_body_live(g, nd->operands[0], nd->operands[1], nd) < 0 ? 0 : 16;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int act_slot_off(int blob, int head, int k) { return -((blob ? head + 8 : 64) + 16 * k); }
static int act_walk_crosses(const IR_graph_t * g, const IR_t * from, const IR_t * to) {
    IR_t * t = zd_chase(from->γ.node); int haz = 0;
    for (int s = 0; t && s < g->n + 4; s++) { if (t == to) return haz; if (act_unbounded_kind((int)ir_kind0(t)) || (int)t->op == IR_MATCH_ARBNO) haz = 1; t = zd_chase(t->γ.node); }
    return -1;
}
static int act_mark_depth = 0;
static void act_mark_chain(const IR_graph_t * g, const IR_t * first, const IR_t * last, const IR_t * self, std::vector<int> & tag, int v) {
    IR_t * cur = zd_chase((IR_t *)first); int guard = 0; if (act_mark_depth > 64) return; act_mark_depth++;
    while (cur && guard++ <= g->n) { if (cur == self) break; int k = act_idx(g, cur); if (k >= 0 && tag[(size_t)k] < 0) tag[(size_t)k] = v;
        { int o = (int)cur->op;
            if (o == IR_MATCH_ALTERNATE) for (int q = 0; 2 * q + 1 < cur->n_operands; q++) act_mark_chain(g, cur->operands[2 * q], cur->operands[2 * q + 1], cur, tag, v);
            else if (o == IR_MATCH_FENCE1 && cur->n_operands >= 2) act_mark_chain(g, cur->operands[0], cur->operands[1], cur, tag, v);
            else if (o == IR_MATCH_ARBNO && cur->n_operands >= 3) act_mark_chain(g, cur->operands[1], cur->operands[2], cur, tag, v); }
        if (cur == last) break; cur = zd_chase(cur->γ.node); }
    act_mark_depth--;
}
extern "C" void planner_act_build(const IR_graph_t * g, int blob, int head) {
    if (!g) return;
    if (g_emitter.g_act_g == g && g_emitter.g_act_all == g->all && g_emitter.g_act_n == g->n && g_emitter.g_act_blob == blob && g_emitter.g_act_head == head) return;
    int n = g->n;
    g_emitter.g_act_g = g; g_emitter.g_act_all = g->all; g_emitter.g_act_n = n; g_emitter.g_act_blob = blob; g_emitter.g_act_head = head; g_emitter.g_act_blob_units = 0;
    g_emitter.g_act_cls.assign((size_t)n, 0); g_emitter.g_act_cause.assign((size_t)n, 0); g_emitter.g_act_slot.assign((size_t)n, -1); g_emitter.g_act_units.assign((size_t)n, 0);
    g_emitter.g_act_extra.assign((size_t)n, 32); g_emitter.g_act_owner.assign((size_t)n, -1);
    std::vector<int> arm((size_t)n, -1), body((size_t)n, -1);
    for (int j = 0; j < n; j++) { IR_t * a = g->all[j]; if (!a) continue;
        if (a->op == IR_MATCH_ALTERNATE) for (int q = 0; 2 * q + 1 < a->n_operands; q++) act_mark_chain(g, a->operands[2 * q], a->operands[2 * q + 1], a, arm, j * 64 + q);
        if (a->op == IR_MATCH_ARBNO && a->n_operands >= 3) act_mark_chain(g, a->operands[1], a->operands[2], a, body, j); }
    for (int j = 0; j < n; j++) { IR_t * nd = g->all[j]; if (!nd) continue; int o = (int)nd->op;
        if (o == IR_MATCH_ARBNO && nd->n_operands >= 3) { if (planner_body_live(g, nd->operands[1], nd->operands[2], nd) < 0) { int lo = act_idx(g, nd->operands[1]), hi = act_idx(g, nd->operands[2]);
            g_emitter.g_act_cls[(size_t)j] = ACT_CLASS_ARBNO; g_emitter.g_act_units[(size_t)j] = 1; g_emitter.g_act_cause[(size_t)j] = planner_act_cause(g, nd, ACT_CLASS_ARBNO, lo, hi, 0, 0, 0);
                } continue; }
        if (o == IR_MATCH_FENCE1 && !act_match_bare(nd) && nd->n_operands >= 2) { if (planner_body_live(g, nd->operands[0], nd->operands[1], nd) < 0) { int lo = act_idx(g, nd->operands[0]),
            hi = act_idx(g, nd->operands[1]);
            g_emitter.g_act_cls[(size_t)j] = ACT_CLASS_FENCE; g_emitter.g_act_units[(size_t)j] = 2;
                g_emitter.g_act_cause[(size_t)j] = planner_act_cause(g, nd, ACT_CLASS_FENCE, lo, hi, 0, 0, g->body_root == nd); } continue; }
        if ((o == IR_MATCH_ASSIGN_COND || o == IR_MATCH_ASSIGN_IMM) && nd->n_operands > 1 && nd->operands[1] && nd->operands[1]->op == IR_MATCH_ASSIGN_SAVE) {
            int si = act_idx(g, nd->operands[1]), hi = j; { int ext = fc_pair_extent(nd); if (ext > 0 && ext - 1 > hi) hi = ext - 1; }
            int lo = si < 0 ? j : si, unb = 0;
            for (int q = (lo < hi ? lo : hi); q <= (lo < hi ? hi : lo) && q < n; q++) { IR_t * m = g->all[q];
                if (m && m != nd && m != nd->operands[1] && act_unbounded_kind((int)ir_kind0(m))) { unb = 1; break; } }
            if (unb && si >= 0) { int c = planner_act_cause(g, nd, ACT_CLASS_CAPTURE, lo, hi, 0, 0, 0);
                g_emitter.g_act_cls[(size_t)si] = ACT_CLASS_CAPTURE; g_emitter.g_act_units[(size_t)si] = 1; g_emitter.g_act_cause[(size_t)si] = c;
                g_emitter.g_act_cls[(size_t)j] = ACT_CLASS_CAPTURE; g_emitter.g_act_cause[(size_t)j] = c; g_emitter.g_act_owner[(size_t)j] = si; }
            continue; }
        if (o == IR_MATCH_RKRUN || o == IR_MATCH_RKLAND) { int ng = (o == IR_MATCH_RKLAND && IR_LIT(nd).sval) ? atoi(IR_LIT(nd).sval) : 0;
            g_emitter.g_act_cls[(size_t)j] = ACT_CLASS_REGEX; g_emitter.g_act_units[(size_t)j] = (o == IR_MATCH_RKLAND) ? (8 + 8 * ng + 15) / 16 : 1; g_emitter.g_act_cause[(size_t)j] = ACT_REGEX_KIT;
                continue; }
        if (o == IR_MATCH_RKCAPS || o == IR_MATCH_RKCAPE) { g_emitter.g_act_cls[(size_t)j] = ACT_CLASS_REGEX; g_emitter.g_act_cause[(size_t)j] = ACT_REGEX_KIT;
            g_emitter.g_act_owner[(size_t)j] = nd->n_operands > 0 ? act_idx(g, nd->operands[0]) : -1; continue; }
        if (!blob || o == IR_MATCH_ASSIGN_SAVE || o == IR_MATCH_ALTERNATE || o == IR_MATCH_FENCE0 || !ir_node_produces_value(nd->op) || zd_k_own(nd) != 16) continue;
        for (int c = 0; c < n; c++) { IR_t * cn = g->all[c]; if (!cn || cn == nd) continue; int uses = 0; for (int a = 0; a < cn->n_operands; a++) if (cn->operands[a] == nd) uses = 1;
            if (!uses) continue;
            int cross = act_walk_crosses(g, nd, cn), cause = ACT_NONE;
            if (cross > 0) cause = planner_act_cause(g, nd, ACT_CLASS_VALUE, j, c, 0, 0, 0);
            else if (body[(size_t)c] >= 0 && body[(size_t)c] != body[(size_t)j] && body[(size_t)c] != j) cause = ACT_ARBNO_ITERATION;
            else if (arm[(size_t)j] >= 0 && arm[(size_t)c] != arm[(size_t)j]) cause = ACT_BOUNDED_ARM;
            if (cause != ACT_NONE) { g_emitter.g_act_cls[(size_t)j] = ACT_CLASS_VALUE; g_emitter.g_act_units[(size_t)j] = 1; g_emitter.g_act_cause[(size_t)j] = cause; break; } } }
    int mb = -1, cum = 0;
    for (int j = 0; j <= n; j++) {
        IR_t * nd = j < n ? g->all[j] : (IR_t *)0;
        if (j == n || (nd && nd->op == IR_MATCH_BEGIN)) { if (mb >= 0) g_emitter.g_act_extra[(size_t)mb] = 32 + 16 * cum; else if (blob) g_emitter.g_act_blob_units = cum; if (j == n) break; mb = j;
            cum = 0; continue; }
        if (!nd || g_emitter.g_act_units[(size_t)j] <= 0) continue;
        if (mb < 0 && !blob) continue;
        int u = g_emitter.g_act_units[(size_t)j], cls = g_emitter.g_act_cls[(size_t)j];
        g_emitter.g_act_slot[(size_t)j] = act_slot_off(mb < 0, head, cum + (cls == ACT_CLASS_FENCE ? 1 : (cls == ACT_CLASS_REGEX ? u - 1 : 0)));
        cum += u; }
    for (int j = 0; j < n; j++) { int ow = g_emitter.g_act_owner[(size_t)j]; if (ow >= 0 && g_emitter.g_act_slot[(size_t)j] == -1) g_emitter.g_act_slot[(size_t)j] = g_emitter.g_act_slot[(size_t)ow]; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int act_nidx(IR_t ** nodes, int n, const IR_t * x) { for (int q = 0; q < n; q++) if (nodes[q] == x) return q; return -1; }
static int act_path_bytes(IR_t ** nodes, int n, IR_t * start, int inclusive, int to, int * ok) {
    int bytes = 0, g = 0; IR_t * cur = inclusive ? start : (start ? zd_chase(start->γ.node) : (IR_t *)0); *ok = 0;
    while (cur && g++ <= n) {
        if (cur == nodes[to]) { *ok = 1; return bytes; }
        int o = (int)ir_kind0(cur);
        if (o == IR_MATCH_DEFER) bytes += 16;
        else if (o == IR_MATCH_BEGIN) { IR_t * t = zd_chase(cur->γ.node); int h = 0, closed = 0;
            while (t && h++ <= n) { if (t == nodes[to]) break; if ((int)ir_kind0(t) == IR_MATCH_END) { closed = 1; cur = t; break; } t = zd_chase(t->γ.node); }
            if (!closed) bytes += 64 + planner_act_extra(cur); }
        cur = zd_chase(cur->γ.node); }
    return bytes;
}
static int act_env_arm_first(IR_t ** nodes, int n, const int * zarm, int e, int x) {
    IR_t * env = nodes[e]; int lo = env->op == IR_MATCH_ARBNO ? 1 : 0;
    for (int a = lo; a + 1 < env->n_operands; a += 2) { IR_t * c = env->operands[a]; int g = 0;
        while (c && g++ <= n) { int ci = act_nidx(nodes, n, c); if (ci < 0) break;
            if (ci == x || planner_env_encloses(zarm, x, ci)) return a;
            if (c == env->operands[a + 1]) break;
            c = zd_chase(c->γ.node); } }
    return -1;
}
extern "C" int planner_read_extra(IR_t ** nodes, int n, const int * zarm, int i, int k) {
    if (!nodes || !zarm || i < 0 || k < 0 || i >= n || k >= n || i == k) return -1;
    int chain[64], m = 0, extra = 0;
    for (int e = zarm[i], g = 0; e >= 0 && g++ < 4096 && m < 64; e = zarm[e]) { if (planner_env_encloses(zarm, k, e)) break; chain[m++] = e; }
    for (int t = 0; t < m; t++) { int o = (int)nodes[chain[t]]->op;
        if (o == IR_MATCH_ALTERNATE) extra += 32;
        else if (o == IR_MATCH_FENCE1 && planner_act_slot(nodes[chain[t]]) == -1 && planner_fence_k(g_emit_cfg, nodes[chain[t]]) == 16) extra += 16; }
    int ok = 0, target = m > 0 ? chain[m - 1] : i;
    extra += act_path_bytes(nodes, n, nodes[k], 0, target, &ok); if (!ok) return -1;
    for (int t = m - 1; t >= 0; t--) { int inner = t > 0 ? chain[t - 1] : i, a = act_env_arm_first(nodes, n, zarm, chain[t], inner); if (a < 0) return -1;
        extra += act_path_bytes(nodes, n, nodes[chain[t]]->operands[a], 1, inner, &ok); if (!ok) return -1; }
    return extra;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int act_ready(const IR_t * nd, int * j) { if (!nd || !g_emitter.g_act_g || g_emitter.g_act_all != g_emitter.g_act_g->all || g_emitter.g_act_n != g_emitter.g_act_g->n) return 0;
    *j = act_idx(g_emitter.g_act_g, nd); return *j >= 0; }
extern "C" int planner_act_slot(const IR_t * nd) { int j; return act_ready(nd, &j) ? g_emitter.g_act_slot[(size_t)j] : -1; }
extern "C" int planner_act_value_slot(const IR_t * nd) { int j; return (act_ready(nd, &j) && g_emitter.g_act_cls[(size_t)j] == ACT_CLASS_VALUE) ? g_emitter.g_act_slot[(size_t)j] : -1; }
extern "C" int planner_act_class_of(const IR_t * nd) { int j; return act_ready(nd, &j) ? g_emitter.g_act_cls[(size_t)j] : ACT_CLASS_NONE; }
extern "C" int planner_act_cause_of(const IR_t * nd) { int j; return act_ready(nd, &j) ? g_emitter.g_act_cause[(size_t)j] : ACT_NONE; }
extern "C" int planner_act_extra(const IR_t * mb) { int j; return (act_ready(mb, &j) && mb->op == IR_MATCH_BEGIN) ? g_emitter.g_act_extra[(size_t)j] : 32; }
extern "C" int planner_blob_slot_bytes(void) { return g_emitter.g_act_g ? 16 * g_emitter.g_act_blob_units : 0; }
extern "C" int planner_act_window(const IR_t * arbno, int * lo, int * bytes) {
    *lo = 0; *bytes = 0; int j; if (!act_ready(arbno, &j) || arbno->n_operands < 3) return 0;
    const IR_graph_t * g = g_emitter.g_act_g; int s0 = act_idx(g, arbno->operands[1]), s1 = act_idx(g, arbno->operands[2]); if (s0 > s1) { int t = s0; s0 = s1; s1 = t; }
    int any = 0, mn = 0, mx = 0;
    for (int q = s0; q >= 0 && q <= s1; q++) { if (g->all[q] == arbno || g_emitter.g_act_units[(size_t)q] <= 0 || g_emitter.g_act_slot[(size_t)q] == -1) continue;
        int a = g_emitter.g_act_slot[(size_t)q] - (g_emitter.g_act_cls[(size_t)q] == ACT_CLASS_FENCE ? 0 : 0), b = g_emitter.g_act_slot[(size_t)q] + 16 * g_emitter.g_act_units[(size_t)q];
        if (!any || a < mn) mn = a; if (!any || b > mx) mx = b; any = 1; }
    if (any) { *lo = mn; *bytes = mx - mn; } return any;
}
extern "C" void planner_act_visit(void (*fn)(const IR_t *, int, int, int)) {
    const IR_graph_t * g = g_emitter.g_act_g; if (!g || g_emitter.g_act_all != g->all) return;
    for (int j = 0; j < g->n; j++) if (g_emitter.g_act_units[(size_t)j] > 0 && g_emitter.g_act_slot[(size_t)j] != -1)
        fn(g->all[j], g_emitter.g_act_slot[(size_t)j], g_emitter.g_act_units[(size_t)j], g_emitter.g_act_cls[(size_t)j]);
}
