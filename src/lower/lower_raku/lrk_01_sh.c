#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include "ir/ir_node.h"
#include "ir/ir_graph.h"
#include "ir/stage2.h"
#include "lower/lower_raku.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "lower/lower_raku/lrk_01_sh.h"
#include "lower/lower_raku/lrk_02_rk.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_04_rk.h"
#include "lower/lower_raku/lrk_05_rk.h"
#include "lower/lower_raku/lrk_06_rk.h"
#include "lower/lower_raku/lrk_08_rk.h"
#include "lower/lower_raku/lrk_09_sh.h"
#include "lower/lower_raku/lrk_10_rk.h"
#include "runtime/core/g_runtime.h"
#include "lower/g_lower_rk.h"
static tree_t * rk_arm_chain(const tree_t * const * st, int n, const char * subj);
static void rk_arm_body(tree_t * seq, int from);
static int rk_block_has_arm(const tree_t * t);
static tree_t * rk_tail_return(tree_t * st);
static tree_t * rk_tail_case(tree_t * st);
static int rk_block_has_arm(const tree_t * t);
static tree_t * rk_arm_chain(const tree_t * const * st, int n, const char * subj);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lrk_rk_seq_is_logical_and(const tree_t * t) { return t && t->t == TT_SEQ && t->n == 2 && t->v.ival == 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_multi_name(const char * nm) {
    if (!nm) return 0;
    for (uint32_t i = 0; i < g_lower_rk.multi_names.len; i++) if (!strcmp(CV_AT(g_lower_rk.multi_names, const char *, i), nm)) return 1;
    return 0;
}
void rk_multi_name_add(const char * base) { if (!base || rk_is_multi_name(base)) return; { const char * nm = ct_strdup(base); CV_PUSH(g_lower_rk.multi_names, const char *) = nm; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_name_is_byref(rcx_t * cx, const char * name) {
    if (!cx || !cx->cur_proc || !cx->cur_byref_mask || !name) return 0;
    for (int k = 0; k < cx->cur_nparams && k < 64 && (1 + k) < cx->cur_proc->n; k++) {
        if (!((cx->cur_byref_mask >> k) & 1ULL)) continue;
        const tree_t * pv = cx->cur_proc->c[1 + k];
        if (pv && pv->v.sval && !strcmp(pv->v.sval, name)) return 1;
    }
    return 0;
}
int rk_var_byref(rcx_t * cx, const tree_t * v) { return v && v->v.sval && ((v->slen & RK_VF_BOXED) || rk_name_is_byref(cx, v->v.sval)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint64_t rk_callee_byref_mask(const char * nm) {
    if (!nm) return 0;
    for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, nm)) return g_stage2.proc_table[i].byref_mask;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * lrk_stmt_subj(const tree_t * s) { return lc_stmt_subj(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lrk_build(rcx_t * cx, IR_e op, IR_t * γ, IR_t * ω) {
    IR_t * nd = lc_build(cx->g, op, γ, ω);
    if (γ && ir_is_generator_kind(γ->op)) lc_γ_to_β(nd, γ);
    if (ω && ir_is_generator_kind(ω->op)) lc_ω_to_β(nd, ω);
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * rk_excise_at(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = lrk_build(cx, IR_EXCISED, γ, ω);
    char kb[48];
    snprintf(kb, sizeof kb, "tree kind %d with %d children", t ? (int) t->t : -1, t ? t->n : 0);
    IR_LIT(nd).sval = lp_strdup(kb);
    if (res) *res = nd;
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int trace_wanted(void) { return g_runtime.abi.trace_budget != 0; }
static int rk_use_is_version(const tree_t * s) { return s && s->t == TT_USE_DECL && s->v.sval && s->v.sval[0] == 'v' && s->v.sval[1] >= '0' && s->v.sval[1] <= '9'; }
long trace_stmt_line(const tree_t * s) { return (s && !rk_use_is_version(s)) ? s->line : 0; }
IR_t * trace_stmt_wrap(rcx_t * cx, long line, IR_t * stmt_entry, IR_t * ω) {
    if (line <= 0 || !trace_wanted()) return stmt_entry;
    IR_t * call = lrk_build(cx, IR_CALL, stmt_entry, ω);
    IR_LIT(call).sval = "__trace_stmt";
    IR_t * lit = lrk_build(cx, IR_LIT_INTEGER, call, ω);
    IR_LIT(lit).ival = line;
    ir_operand_push(call, lit);
    return lit;
}
int rk_trace_is_block(const char * name) { return name && !strncmp(name, "__blk_", 6); }
const char * rk_trace_name(const char * name) { return (name && !strcmp(name, "&main")) ? "main" : name; }
IR_t * trace_call_wrap(rcx_t * cx, const char * name, IR_t * body_entry, IR_t * ω) {
    if (!name || !*name || !trace_wanted() || rk_trace_is_block(name)) return body_entry;
    name = rk_trace_name(name);
    IR_t * call = lrk_build(cx, IR_CALL, body_entry, ω);
    IR_LIT(call).sval = "__trace_call";
    IR_t * nm = lrk_build(cx, IR_LIT_STRING, call, ω);
    IR_LIT(nm).sval = name;
    ir_operand_push(call, nm);
    return nm;
}
IR_t * trace_value_prep(rcx_t * cx, const char * name, IR_t * γ, IR_t * ω, IR_t ** call_out) {
    if (!name || !*name || !trace_wanted() || !strncmp(name, "__", 2) || !strncmp(name, "$?", 2)) { *call_out = NULL; return NULL; }
    IR_t * call = lrk_build(cx, IR_CALL, γ, ω);
    IR_LIT(call).sval = "__trace_value";
    IR_t * nm = lrk_build(cx, IR_LIT_STRING, call, ω);
    IR_LIT(nm).sval = name;
    ir_operand_push(call, nm);
    *call_out = call;
    return nm;
}
static const tree_t * rk_catch_of(const tree_t * t) {
    for (int i = 0; i < t->n; i++) { const tree_t * s = t->c[i]; if (s && s->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(s); if (sub) s = sub; } if (s && s->t == TT_CATCH && s->n > 0) return s; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * rk_catch_handler(const tree_t * c) {
    const tree_t * h = c->c[0];
    if (!h || (h->t != TT_SEQ && h->t != TT_SEQ_EXPR)) return h;
    int ci = -1;
    for (int i = h->n - 1; i >= 0; i--) { const tree_t * s = h->c[i]; if (s && s->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(s); if (sub) s = sub; } if (s && s->t == TT_CASE) { ci = i; break; } }
    tree_t * rt = ast_node_new(TT_FNC);
    rt->v.sval = (char *) "__rk_rethrow";
    ast_push(rt, leaf_sval2(TT_VAR, "__rk_rethrow"));
    tree_t * nh = ast_node_new(h->t);
    for (int i = 0; i < h->n; i++) {
        const tree_t * s = h->c[i];
        const tree_t * u = s;
        if (u && u->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(u); if (sub) u = sub; }
        if (i != ci) { ast_push(nh, (tree_t *) s); continue; }
        int hasdef = 0;
        for (int k = 1; k < u->n; k++) if (u->c[k] && u->c[k]->t == TT_ARM && u->c[k]->n == 1) hasdef = 1;
        if (hasdef) { ast_push(nh, (tree_t *) s); continue; }
        tree_t * nc = ast_node_new(TT_CASE);
        nc->line = u->line;
        for (int k = 0; k < u->n; k++) ast_push(nc, u->c[k]);
        tree_t * rb = ast_node_new(TT_SEQ_EXPR);
        ast_push(rb, rt);
        tree_t * da = ast_node_new(TT_ARM);
        ast_push(da, rb);
        ast_push(nc, da);
        ast_push(nh, nc);
    }
    if (ci < 0) ast_push(nh, rt);
    return nh;
}
int rk_empty_body(const tree_t * t) { return !t || ((t->t == TT_SEQ || t->t == TT_SEQ_EXPR || t->t == TT_PROGRAM) && t->n == 0); }
IR_t * lower_rblock(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** last_res) {
    if (t && (t->t == TT_SEQ || t->t == TT_SEQ_EXPR || t->t == TT_PROGRAM) && t->slen != 78) {
        const tree_t * ct = rk_catch_of(t);
        if (ct) {
            tree_t * nb = ast_node_new(t->t);
            nb->line = t->line;
            nb->slen = 78;
            for (int i = 0; i < t->n; i++) ast_push(nb, t->c[i]);
            tree_t * nt = ast_node_new(TT_TRY);
            nt->line = t->line;
            nt->slen = 77;
            ast_push(nt, nb);
            ast_push(nt, (tree_t *) rk_catch_handler(ct));
            IR_t * r = NULL;
            IR_t * e = lower_rv(cx, nt, γ, ω, &r);
            if (last_res) *last_res = r;
            return e;
        }
    }
    if (t && (t->t == TT_SEQ || t->t == TT_SEQ_EXPR || t->t == TT_PROGRAM) && rk_block_has_arm(t)) {
        tree_t * ch = rk_arm_chain((const tree_t * const *) t->c, t->n, "_");
        return lower_rblock(cx, ch, γ, ω, last_res);
    }
    if (!t) return (γ && ir_is_generator_kind(γ->op)) ? γ : lrk_build(cx, IR_SUCCEED, γ, ω);
    if (t->t != TT_SEQ && t->t != TT_PROGRAM && t->t != TT_SEQ_EXPR) { IR_t * r = NULL; IR_t * e = lower_rv(cx, t, γ, ω, &r); if (last_res) *last_res = r; return e; }
    if (t->n == 0) return (γ && ir_is_generator_kind(γ->op)) ? γ : lrk_build(cx, IR_SUCCEED, γ, ω);
    IR_t * succ = γ;
    IR_t * entry = γ;
    int got_last = 0;
    for (int i = t->n - 1; i >= 0; i--) {
        const tree_t * s = t->c[i];
        if (s && s->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(s); if (!sub) continue; s = sub; }
        long line = trace_stmt_line(s);
        if (s && s->t == TT_CATCH) continue;
        if (s && (s->t == TT_SEQ || s->t == TT_SEQ_EXPR || s->t == TT_PROGRAM) && s->n == 0) continue;
        IR_t * gs = succ, * gw = ω;
        if (cx->try_catch) {
            IR_t * pγ = lrk_build(cx, IR_CALL, cx->try_catch, succ);
            IR_LIT(pγ).sval = "exc_check";
            IR_t * pω = lrk_build(cx, IR_CALL, cx->try_catch, ω);
            IR_LIT(pω).sval = "exc_check";
            gs = pγ;
            gw = pω;
        }
        IR_t * r = NULL;
        IR_t * e = lower_rv(cx, s, gs, gw, &r);
        if (last_res && !got_last) { *last_res = r; got_last = 1; }
        if (e && ir_is_generator_kind(e->op)) { IR_t * tramp = lrk_build(cx, IR_GOTO, NULL, NULL); lc_γ_to(tramp, e); lc_ω_to(tramp, e); e = tramp; }
        if (e) e = trace_stmt_wrap(cx, line, e, gw);
        if (e) { entry = e; succ = e; }
    }
    return entry;
}
static int rk_param_is_raw(const tree_t * pv) { if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0]; return pv && pv->t == TT_VAR && (pv->slen & RK_PV_RAW); }
static int rk_callee_param_rw(const char * nm, int k) {
    for (int i = 0; nm && i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, nm)) {
        const tree_t * pr = (const tree_t *) g_stage2.proc_table[i].proc;
        return pr && pr->t == TT_SUB_DECL && (1 + k) < pr->n && rk_param_is_rw(pr->c[1 + k]) && !rk_param_is_raw(pr->c[1 + k]);
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int lrk_is_slip(const tree_t * a) { return a && a->t == TT_FNC && a->v.sval && !strcmp(a->v.sval, "__rk_arr_slip"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t * lrk_spread_call(rcx_t * cx, const tree_t * t, int from, int nargs, IR_t * nd, IR_t * ω, IR_t ** res) {
    IR_t * entry = NULL, * prev = NULL;
    IR_t * val[nargs > 0 ? nargs : 1];
    for (int k = 0; k < nargs; k++) {
        IR_t * ar = NULL;
        IR_t * ae = lower_rv(cx, t->c[from + k], NULL, ω, &ar);
        if (!entry) entry = ae;
        if (prev && ae) lc_γ_to(prev, ae);
        prev = ar;
        val[k] = ar;
    }
    IR_t * cnt = lrk_build(cx, IR_LIT_INTEGER, NULL, ω);
    IR_LIT(cnt).ival = 0;
    if (!entry) entry = cnt;
    if (prev) lc_γ_to(prev, cnt);
    prev = cnt;
    for (int k = 0; k < nargs; k++) {
        IR_t * st = lrk_build(cx, IR_ARG_STAGE, NULL, ω);
        IR_LIT(st).ival = lrk_is_slip(t->c[from + k]) ? 1 : 0;
        ir_operand_push(st, prev);
        if (val[k]) ir_operand_push(st, val[k]);
        lc_γ_to(prev, st);
        prev = st;
    }
    lc_γ_to(prev, nd);
    ir_operand_push(nd, prev);
    if (res) *res = nd;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_rcall(rcx_t * cx, const tree_t * t, const char * nm, int from, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = lrk_build(cx, rk_proc_is_generator(nm) ? IR_PROC_GEN : IR_CALL, γ, ω);
    IR_LIT(nd).sval = nm;
    int nargs = t->n - from;
    for (int k = 0; nm && nm[0] != '_' && k < nargs; k++) {
        const tree_t * a0 = t->c[from + k];
        if (a0 && (a0->t == TT_ILIT || a0->t == TT_FLIT || a0->t == TT_QLIT) && rk_callee_param_rw(nm, k)) {
            tree_t * rf = ast_node_new(TT_FNC);
            rf->v.sval = (char *) "__rk_rw_refuse";
            ast_push(rf, leaf_sval2(TT_VAR, "__rk_rw_refuse"));
            ast_push(rf, leaf_sval2(TT_QLIT, nm));
            ast_push(rf, rk_ti(k));
            ast_push(rf, (tree_t *) a0);
            return lower_rv(cx, rf, γ, ω, res);
        }
    }
    { int sp = 0; for (int k = 0; k < nargs; k++) if (lrk_is_slip(t->c[from + k])) sp = 1; if (sp && nm && (nm[0] != '_' || !strcmp(nm, "__multi_call")) && nd->op == IR_CALL) return lrk_spread_call(cx, t, from, nargs, nd, ω, res); }
    IR_t * prev = NULL;
    IR_t * tgt = nd;
    if (nd->op == IR_PROC_GEN) { tgt = lrk_build(cx, IR_GOTO, NULL, NULL); lc_γ_to(tgt, nd); lc_ω_to(tgt, nd); }
    IR_t * entry = tgt;
    uint64_t brm = rk_callee_byref_mask(nm);
    if (!strcmp(nm, "__blk_close")) brm = ~1ULL;
    if (!strcmp(nm, "__rk_byref_assign")) brm = 1ULL;
    for (int k = 0; k < nargs; k++) {
        const tree_t * argt = t->c[from + k];
        IR_t * ar = NULL;
        IR_t * ae;
        if (((brm >> k) & 1ULL) && argt && argt->t == TT_VAR && argt->v.sval && !rk_var_byref(cx, argt)) {
            IR_t * vr = lrk_build(cx, IR_VAR_REF, (k == nargs - 1) ? tgt : NULL, ω);
            IR_LIT(vr).sval = argt->v.sval;
            ae = vr;
            ar = vr;
        } else if (((brm >> k) & 1ULL) && argt && argt->t == TT_VAR && argt->v.sval) {
            IR_t * vr = lrk_build(cx, IR_VAR, (k == nargs - 1) ? tgt : NULL, ω);
            IR_LIT(vr).sval = argt->v.sval;
            ae = vr;
            ar = vr;
        } else if (argt && argt->t == TT_FNC && argt->n == 2 && argt->v.sval && !strcmp(argt->v.sval, "__rk_byval")) {
            ae = lower_rv(cx, argt->c[1], (k == nargs - 1) ? tgt : NULL, ω, &ar);
        } else {
            ae = lower_rv(cx, argt, (k == nargs - 1) ? tgt : NULL, ω, &ar);
        }
        if (k == 0) entry = ae;
        if (prev && ae) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) ir_operand_push(nd, ar);
    }
    if (res) *res = nd;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_arm_chain(const tree_t * const * st, int n, const char * subj) {
    tree_t * rest = NULL;
    const tree_t * run[n > 0 ? n : 1];
    int nr = 0;
    for (int i = n - 1; i >= -1; i--) {
        const tree_t * u = i >= 0 ? st[i] : NULL;
        if (u && u->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(u); if (sub) u = sub; }
        int arm = u && u->t == TT_ARM && u->n >= 1;
        if (i >= 0 && !arm) { run[nr++] = st[i]; continue; }
        if (nr) { tree_t * sq = ast_node_new(TT_SEQ_EXPR); for (int k = nr - 1; k >= 0; k--) ast_push(sq, (tree_t *) run[k]); if (rest) ast_push(sq, rest); rest = sq; nr = 0; }
        if (i < 0) break;
        if (u->n == 1) { rest = (tree_t *) u->c[0]; continue; }
        tree_t * gate = ast_node_new(TT_IF);
        ast_push(gate, rk_case_match(leaf_sval2(TT_VAR, subj), u->c[0]));
        if (u->c[1] && u->c[1]->t == TT_SEQ_EXPR) rk_arm_body((tree_t *) u->c[1], 0);
        ast_push(gate, (tree_t *) u->c[1]);
        if (rest) ast_push(gate, rest);
        rest = gate;
    }
    return rest;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_block_has_arm(const tree_t * t) {
    for (int i = 0; t && i < t->n; i++) { const tree_t * u = t->c[i]; if (u && u->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(u); if (sub) u = sub; } if (u && u->t == TT_ARM) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_case_desugar(const tree_t * t) {
    int cid = g_lower_rk.rk_case_desugar_cid++;
    char nb[48], sb[48];
    snprintf(nb, sizeof nb, "__case_%d", cid);
    snprintf(sb, sizeof sb, "__casesv_%d", cid);
    const char * vn = intern(nb), * sv = intern(sb);
    tree_t * chain = rk_arm_chain((const tree_t * const *) t->c + 1, t->n - 1, vn);
    tree_t * seq = ast_node_new(TT_SEQ);
    tree_t * as = ast_node_new(TT_ASSIGN);
    ast_push(as, leaf_sval2(TT_VAR, vn));
    ast_push(as, (tree_t *) t->c[0]);
    ast_push(seq, as);
    tree_t * sa = ast_node_new(TT_ASSIGN);
    ast_push(sa, leaf_sval2(TT_VAR, sv));
    ast_push(sa, leaf_sval2(TT_VAR, "_"));
    ast_push(seq, sa);
    tree_t * ta = ast_node_new(TT_ASSIGN);
    ast_push(ta, leaf_sval2(TT_VAR, "_"));
    ast_push(ta, leaf_sval2(TT_VAR, vn));
    ast_push(seq, ta);
    if (chain) ast_push(seq, chain);
    tree_t * ra = ast_node_new(TT_ASSIGN);
    ast_push(ra, leaf_sval2(TT_VAR, "_"));
    ast_push(ra, leaf_sval2(TT_VAR, sv));
    ast_push(seq, ra);
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t * lower_raku_proc(const tree_t * prog, const tree_t * pd) {
    IR_graph_t * g = IR_alloc(8192);
    rcx_t cx;
    cx.cond_om = NULL;
    cx.try_depth = 0;
    cx.g = g;
    cx.try_catch = NULL;
    cx.loops = NULL;
    cx.loop_label = NULL;
    cx.cur_proc = pd;
    cx.cur_byref_mask = 0;
    cx.cur_nparams = 0;
    const char * rk_proc_name = NULL;
    for (int _pbi = 0; pd && _pbi < g_stage2.proc_count; _pbi++) if (g_stage2.proc_table[_pbi].proc == pd) {
        cx.cur_byref_mask = g_stage2.proc_table[_pbi].byref_mask;
        cx.cur_nparams = g_stage2.proc_table[_pbi].nparams;
        rk_proc_name = g_stage2.proc_table[_pbi].name;
        break;
    }
    cx.cur_proc_name = rk_proc_name;
    if (pd && pd->t == TT_SUB_DECL && rk_catch_of(pd)) {
        const tree_t * ct = rk_catch_of(pd);
        tree_t * p2 = ast_node_new(pd->t);
        p2->v = pd->v;
        p2->slen = pd->slen;
        p2->line = pd->line;
        int k = 0;
        while (k < pd->n && (k == 0 || (pd->c[k] && pd->c[k]->t == TT_VAR))) ast_push(p2, pd->c[k++]);
        tree_t * nb = ast_node_new(TT_SEQ_EXPR);
        nb->slen = 78;
        for (; k < pd->n; k++) ast_push(nb, pd->c[k]);
        tree_t * nt = ast_node_new(TT_TRY);
        nt->slen = 77;
        ast_push(nt, nb);
        ast_push(nt, (tree_t *) rk_catch_handler(ct));
        ast_push(p2, nt);
        pd = p2;
    }
    IR_t * succ = IR_node_alloc(g, IR_SUCCEED);
    IR_t * fail = IR_node_alloc(g, IR_FAIL);
    int rk_gen = rk_proc_is_generator(rk_proc_name);
    IR_t * fall = rk_gen ? lrk_build(&cx, IR_GOTO, fail, fail) : lrk_build(&cx, IR_RETURN, succ, fail);
    if (rk_gen) { g->cell_frame = 1; g->spine_cells = 1; }
    IR_t * sentry = fall;
    IR_t * entry = fall;
    cx.proc_exit = succ;
    int is_multi = (pd && pd->n > 0 && pd->c[0] && pd->c[0]->v.sval) && strchr(pd->c[0]->v.sval, '$');
    int rk_np = 0;
    if (pd && !is_multi) for (int k = 1; k < pd->n && pd->c[k] && pd->c[k]->t == TT_VAR; k++) rk_np++;
    int rk_bstart = rk_np + 1, rk_bn = (pd && pd->n > rk_bstart) ? pd->n - rk_bstart : 0;
    int rk_destr_extra = 0;
    for (int k = 1; k <= rk_np; k++) if (pd->c[k] && pd->c[k]->t == TT_VAR) rk_destr_extra += rk_destr_count(pd->c[k]->v.sval) + pd->c[k]->n + rk_subsig_count(pd->c[k]);
    const tree_t ** rk_plan = (rk_np > 0) ? (const tree_t **) ct_zalloc((size_t) (rk_bn + 2 * rk_np + rk_destr_extra), sizeof(const tree_t *)) : NULL;
    if (rk_plan) {
        int nplan = 0, bi = 0;
        for (int k = 1; k <= rk_np; k++) {
            const tree_t * pv = pd->c[k];
            const char * pn = (pv && pv->t == TT_VAR) ? pv->v.sval : NULL;
            const char * tgt = (bi < rk_bn) ? rk_prologue_target(pd->c[rk_bstart + bi]) : NULL;
            if (pn && tgt && !strcmp(tgt, pn)) rk_plan[nplan++] = pd->c[rk_bstart + bi++];
            if (pn && (pv->slen & RK_PV_COPY) && (pn[0] == '@' || pn[0] == '%')) {
                tree_t * cp = ast_node_new(TT_ASSIGN);
                ast_push(cp, leaf_sval2(TT_VAR, pn));
                ast_push(cp, leaf_sval2(TT_VAR, pn));
                rk_plan[nplan++] = cp;
            }
            if (pn) nplan = rk_destr_plan(pn, rk_plan, nplan);
            for (int j = 0; pn && j < pv->n; j++) if (pv->c[j] && pv->c[j]->t == TT_VAR && (pv->c[j]->slen & RK_PV_ALIAS)) {
                tree_t * as = ast_node_new(TT_ASSIGN), * dv = leaf_sval2(TT_VAR, pv->c[j]->v.sval);
                dv->slen |= 4;
                ast_push(as, dv);
                ast_push(as, leaf_sval2(TT_VAR, pn));
                rk_plan[nplan++] = as;
            }
            if (pn) nplan = rk_subsig_plan(pv, pn, rk_plan, nplan);
            if (pn && pv->n >= 1 && pv->c[0] && pv->c[0]->v.sval) {
                const char * ty = pv->c[0]->v.sval;
                const char * cbase = rk_coerce_base(ty);
                if (cbase) {
                    tree_t * as = ast_node_new(TT_ASSIGN);
                    ast_push(as, leaf_sval2(TT_VAR, pn));
                    ast_push(as, rk_coerce_val(leaf_sval2(TT_VAR, pn), cbase));
                    rk_plan[nplan++] = as;
                } else if (strstr(ty, ":D") || strstr(ty, ":U") || rk_is_modeled_type(ty)) {
                    tree_t * mc = ast_node_new(TT_FNC);
                    mc->v.sval = (char *)"__param_check";
                    tree_t * nmv = ast_node_new(TT_VAR);
                    nmv->v.sval = (char *)"__param_check";
                    ast_push(mc, nmv);
                    tree_t * tyq = ast_node_new(TT_QLIT);
                    tyq->v.sval = (char *)ty;
                    ast_push(mc, tyq);
                    tree_t * pvr = ast_node_new(TT_VAR);
                    pvr->v.sval = (char *)pn;
                    ast_push(mc, pvr);
                    tree_t * pnq = ast_node_new(TT_QLIT);
                    pnq->v.sval = (char *)pn;
                    ast_push(mc, pnq);
                    rk_plan[nplan++] = mc;
                } else if (ty[0] == ':' && ty[1] == ':' && ty[2]) {
                    tree_t * as = ast_node_new(TT_ASSIGN);
                    ast_push(as, leaf_sval2(TT_VAR, ty + 2));
                    ast_push(as, rk_coerce_val(leaf_sval2(TT_VAR, pn), "WHAT"));
                    rk_plan[nplan++] = as;
                }
            }
        }
        for (; bi < rk_bn; bi++) rk_plan[nplan++] = pd->c[rk_bstart + bi];
        for (int i = nplan - 1; i >= 0; i--) {
            const tree_t * s = rk_plan[i];
            if (!s) continue;
            if (s->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(s); if (!sub) continue; s = sub; }
            if (s->t == TT_VAR) continue;
            IR_t * r = NULL;
            IR_t * e = lower_rv(&cx, s, sentry, fail, &r);
            if (e) e = trace_stmt_wrap(&cx, trace_stmt_line(s), e, fail);
            if (e) { entry = e; sentry = e; }
        }
        ct_drop(rk_plan);
        g->entry = trace_call_wrap(&cx, rk_proc_name, entry, fail);
        return g;
    }
    for (int i = (pd ? pd->n : 0) - 1; i >= 1; i--) {
        const tree_t * s = pd->c[i];
        if (!s) continue;
        if (s->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(s); if (!sub) continue; s = sub; }
        if (s->t == TT_VAR && is_multi && rk_destr_count(s->v.sval) > 0) {
            const tree_t * dp[rk_destr_count(s->v.sval)];
            int nd = rk_destr_plan(s->v.sval, dp, 0);
            for (int di = nd - 1; di >= 0; di--) { IR_t * r = NULL; IR_t * e = lower_rv(&cx, dp[di], sentry, fail, &r); if (e) { entry = e; sentry = e; } }
            continue;
        }
        if (s->t == TT_VAR && is_multi && s->v.sval && rk_subsig_count(s) > 0) {
            const tree_t * sp_plan[rk_subsig_count(s) + 1];
            int nd = rk_subsig_plan(s, s->v.sval, sp_plan, 0);
            for (int di = nd - 1; di >= 0; di--) { IR_t * r = NULL; IR_t * e = lower_rv(&cx, sp_plan[di], sentry, fail, &r); if (e) { entry = e; sentry = e; } }
        }
        if (s->t == TT_VAR && is_multi && s->v.sval) {
            for (int aj = s->n - 1; aj >= 0; aj--) if (s->c[aj] && s->c[aj]->t == TT_VAR && (s->c[aj]->slen & RK_PV_ALIAS)) {
                tree_t * as = ast_node_new(TT_ASSIGN), * dv = leaf_sval2(TT_VAR, s->c[aj]->v.sval);
                dv->slen |= 4;
                ast_push(as, dv);
                ast_push(as, leaf_sval2(TT_VAR, s->v.sval));
                IR_t * r = NULL;
                IR_t * e = lower_rv(&cx, as, sentry, fail, &r);
                if (e) { entry = e; sentry = e; }
            }
        }
        if (s->t == TT_VAR) continue;
        IR_t * r = NULL;
        IR_t * e = lower_rv(&cx, s, sentry, fail, &r);
        if (e) e = trace_stmt_wrap(&cx, trace_stmt_line(s), e, fail);
        if (e) { entry = e; sentry = e; }
    }
    if (pd && !is_multi) {
        for (int k = pd->n - 1; k >= 1; k--) {
            const tree_t * pv = pd->c[k];
            if (!pv || pv->t != TT_VAR || !pv->v.sval) continue;
            if (pv->n < 1 || !pv->c[0] || !pv->c[0]->v.sval) continue;
            const char * ty = pv->c[0]->v.sval;
            if (!strstr(ty, ":D") && !strstr(ty, ":U") && !rk_is_modeled_type(ty)) continue;
            const char * pn = pv->v.sval;
            tree_t * mc = ast_node_new(TT_FNC);
            mc->v.sval = (char *) "__param_check";
            tree_t * nmv = ast_node_new(TT_VAR);
            nmv->v.sval = (char *) "__param_check";
            ast_push(mc, nmv);
            tree_t * tyq = ast_node_new(TT_QLIT);
            tyq->v.sval = (char *) ty;
            ast_push(mc, tyq);
            tree_t * pvr = ast_node_new(TT_VAR);
            pvr->v.sval = (char *) pn;
            ast_push(mc, pvr);
            tree_t * pnq = ast_node_new(TT_QLIT);
            pnq->v.sval = (char *) pn;
            ast_push(mc, pnq);
            IR_t * r = NULL;
            IR_t * e = lower_rcall(&cx, mc, "__param_check", 1, sentry, fail, &r);
            if (e) { entry = e; sentry = e; }
        }
    }
    g->entry = trace_call_wrap(&cx, rk_proc_name, entry, fail);
    return g;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int lower_raku_body(const tree_t *prog, const tree_t *proc) { IR_graph_t * ng = lower_raku_proc(prog, proc); if (!ng || !ng->entry) return -1; return bb_program_add(&g_stage2.bbp, ng); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_collect_blocks(const tree_t * t, tree_t ** out, int * n, int max) {
    if (!t || *n >= max || t->t == TT_HAS_DECL) return;
    if (t->t == TT_ANON_BLOCK && !t->v.sval) out[(*n)++] = (tree_t *) t;
    for (int i = 0; i < t->n; i++) rk_collect_blocks(t->c[i], out, n, max);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_count_blocks(const tree_t * t) {
    int n;
    if (!t || t->t == TT_HAS_DECL) return 0;
    n = (t->t == TT_ANON_BLOCK && !t->v.sval);
    for (int i = 0; i < t->n; i++) n += rk_count_blocks(t->c[i]);
    return n;
}
static int rk_var_nodes(const tree_t * t) { int n; if (!t) return 0; n = (t->t == TT_VAR); for (int i = 0; i < t->n; i++) n += rk_var_nodes(t->c[i]); return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_block_tail_is_value(const tree_t * s) {
    if (!s) return 0;
    if (lrk_rk_seq_is_logical_and(s)) return 1;
    switch (s->t) {
        case TT_RETURN:
        case TT_NRETURN:
        case TT_PROC_FAIL:
        case TT_SAY:
        case TT_SAY_FH:
        case TT_PRINT:
        case TT_PRINT_FH:
        case TT_IF:
        case TT_UNLESS:
        case TT_CASE:
        case TT_WHILE:
        case TT_UNTIL:
        case TT_REPEAT:
        case TT_FOR:
        case TT_EVERY:
        case TT_DO_WHILE:
        case TT_CLOOP:
        case TT_SEQ:
        case TT_SEQ_EXPR:
        case TT_STMT:
        case TT_SUB_DECL:
        return 0;
        default:
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_param_is_rw(const tree_t * pv) { if (pv && pv->t == TT_ASSIGN && pv->n) pv = pv->c[0]; return pv && pv->t == TT_VAR && (pv->slen & RK_PV_RW); }
static tree_t * rk_tail_if(tree_t * st) {
    if (!st || (st->t != TT_IF && st->t != TT_UNLESS) || st->n < 2) return st;
    tree_t * nd = ast_node_new(st->t);
    nd->line = st->line;
    nd->v = st->v;
    nd->slen = st->slen;
    for (int i = 0; i < st->n; i++) {
        tree_t * ch = st->c[i];
        ast_push(nd, (i >= 1 && ch && (ch->t == TT_SEQ_EXPR || ch->t == TT_IF || ch->t == TT_UNLESS || ch->t == TT_CASE)) ? rk_tail_return(ch) : ch);
    }
    return nd;
}
static void rk_arm_body(tree_t * seq, int from) {
    if (!seq || from >= seq->n) return;
    tree_t tmp = *seq;
    tmp.c = seq->c + from;
    tmp.n = seq->n - from;
    if (!rk_block_has_arm(&tmp)) return;
    tree_t * ch = rk_arm_chain((const tree_t * const *) seq->c + from, seq->n - from, "_");
    seq->n = from;
    while (ch && ch->t == TT_SEQ_EXPR && ch->n > 0) { for (int i = 0; i < ch->n - 1; i++) ast_push(seq, ch->c[i]); ch = ch->c[ch->n - 1]; }
    if (ch) ast_push(seq, ch);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_tail_ifs(tree_t * t, int in_pkg) {
    if (!t) return;
    if (t->t == TT_SUB_DECL && t->n > 1) {
        rk_arm_body(t, in_pkg ? (int) t->v.ival : (int) t->v.ival + 1);
        int li = t->n - 1;
        tree_t * ls = t->c[li];
        if (ls && ls->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(ls); if (sub) ls = (tree_t *) sub; }
        if (ls && (ls->t == TT_IF || ls->t == TT_UNLESS)) t->c[li] = rk_tail_if(ls);
        else if (ls && ls->t == TT_CASE && ls->n > 1) t->c[li] = rk_tail_case(ls);
    }
    for (int i = 0; i < t->n; i++) rk_tail_ifs(t->c[i], t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_GRAMMAR_DECL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_tail_case(tree_t * st) {
    tree_t * nd = ast_node_new(TT_CASE);
    nd->line = st->line;
    nd->v = st->v;
    nd->slen = st->slen;
    for (int i = 0; i < st->n; i++) {
        tree_t * ch = st->c[i];
        if (i >= 1 && ch && ch->t == TT_ARM && ch->n >= 1) {
            tree_t * arm = ast_node_new(TT_ARM);
            arm->line = ch->line;
            arm->v = ch->v;
            arm->slen = ch->slen;
            for (int k = 0; k < ch->n; k++) ast_push(arm, k == ch->n - 1 ? rk_tail_return(ch->c[k]) : ch->c[k]);
            ch = arm;
        }
        ast_push(nd, ch);
    }
    return nd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_tail_return(tree_t * st) {
    if (st && st->t == TT_CASE && st->n > 1) return rk_tail_case(st);
    if (st && (st->t == TT_IF || st->t == TT_UNLESS)) return rk_tail_if(st);
    if (st && st->t == TT_SEQ_EXPR && st->n > 0) { st->c[st->n - 1] = rk_tail_return(st->c[st->n - 1]); return st; }
    if (!rk_block_tail_is_value(st)) return st;
    tree_t * r = ast_node_new(TT_RETURN);
    r->line = st->line;
    ast_push(r, st);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_hoist_anon_blocks(tree_t * prog) {
    if (!prog) return;
    tree_t * blks[rk_count_blocks(prog) + 1];
    int nb = 0;
    rk_collect_blocks(prog, blks, &nb, (int) (sizeof blks / sizeof blks[0]));
    for (int i = 0; i < nb; i++) {
        extern int rt_proc_is_registered(const char *);
        tree_t * blk = blks[i];
        char nm[64];
        do snprintf(nm, sizeof nm, "__blk_%d", ++g_lower_rk.rk_hoist_anon_blocks_blk_ctr);
        while (rt_proc_is_registered(nm));
        char * pn = lp_strdup(nm);
        blk->v.sval = pn;
        tree_t * sd = ast_node_new(TT_SUB_DECL);
        sd->v.ival = 0;
        tree_t * nn = ast_node_new(TT_VAR);
        nn->v.sval = pn;
        ast_push(sd, nn);
        const tree_t * body = (blk->n > 0) ? blk->c[0] : NULL;
        if (blk->n > 1) {
            for (int k = 1; k < blk->n; k++) ast_push(sd, blk->c[k]);
            sd->v.ival = blk->n - 1;
        } else {
            int topic = 0, nph = 0;
            const char * ph[rk_var_nodes(body) + 1];
            rk_scan_implicit_params(body, &topic, ph, &nph, (int) (sizeof ph / sizeof ph[0]));
            for (int a = 1; a < nph; a++) { const char * key = ph[a]; int b = a - 1; while (b >= 0 && rk_ph_cmp(ph[b], key) > 0) { ph[b + 1] = ph[b]; b--; } ph[b + 1] = key; }
            if (nph > 0) {
                for (int k = 0; k < nph; k++) { tree_t * pv = ast_node_new(TT_VAR); pv->v.sval = (char *) ph[k]; ast_push(sd, pv); }
                sd->v.ival = nph;
            } else if (topic) {
                tree_t * pv = ast_node_new(TT_VAR);
                pv->v.sval = (char *) intern("_");
                ast_push(sd, pv);
                sd->v.ival = 1;
                if (body && (topic & 2)) rk_topic_default((tree_t *) body);
            }
        }
        if (body) rk_arm_body((tree_t *) body, 0);
        if (body) { for (int k = 0; k < body->n; k++) { tree_t * st = body->c[k]; if (k == body->n - 1 && !(blk->slen & 64)) st = rk_tail_return(st); ast_push(sd, st); } ((tree_t *) body)->n = 0; }
        if (blk->slen & 64) sd->slen |= 64;
        blk->n = 0;
        ast_push(prog, sd);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rename_main_refs(tree_t * t) {
    if (!t) return;
    if (t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "main")) {
        t->v.sval = (char *) "&main";
        if (t->n > 0 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "main")) t->c[0]->v.sval = (char *) "&main";
    } else if (t->t == TT_VAR && (t->slen & 1) && t->v.sval && !strcmp(t->v.sval, "main")) t->v.sval = (char *) "&main";
    for (int i = 0; i < t->n; i++) rk_rename_main_refs(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_rename_user_main(tree_t * prog) {
    int found = 0;
    for (int i = 0; prog && i < prog->n; i++) {
        tree_t * d = prog->c[i];
        if (d && d->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(d); d = (tree_t *) sub; }
        if (!d || d->t != TT_SUB_DECL || d->n < 1 || !d->c[0] || !d->c[0]->v.sval || strcmp(d->c[0]->v.sval, "main")) continue;
        d->c[0]->v.sval = (char *) "&main";
        if (d->v.sval && !strcmp(d->v.sval, "main")) d->v.sval = (char *) "&main";
        found = 1;
    }
    if (found) rk_rename_main_refs(prog);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_top_subj(tree_t * st) { return st && st->t == TT_STMT ? (tree_t *) lrk_stmt_subj(st) : st; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
stage2_t *lower_raku_stage2(const tree_t *prog) { return rk_stage2_core(prog, 1, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_eval_proc_name(int n) { char nb[48]; snprintf(nb, sizeof nb, "EVAL$%d", n); return lp_strdup(nb); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_eval_is_declaration(const tree_t * st) {
    const tree_t * u = st;
    if (u->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(u); if (sub) u = sub; }
    return u->t == TT_SUB_DECL || u->t == TT_CLASS_DECL || u->t == TT_ROLE_DECL || u->t == TT_GRAMMAR_DECL || u->t == TT_USE_DECL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * lower_raku_eval_stage2(const tree_t * prog) {
    int pc0 = g_stage2.proc_count;
    const char * nm = rk_eval_proc_name(pc0);
    tree_t * p2 = ast_node_new(TT_PROGRAM), * sd = ast_node_new(TT_SUB_DECL), * nv = ast_node_new(TT_VAR);
    nv->v.sval = (char *) nm;
    sd->v.ival = 0;
    ast_push(sd, nv);
    int lastb = -1;
    for (int i = prog->n - 1; i >= 0 && lastb < 0; i--) {
        tree_t * st = prog->c[i];
        if (!st) continue;
        const tree_t * u = st;
        if (u->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(u); if (sub) u = sub; }
        if (!rk_eval_is_declaration(st) && !(u->t == TT_SEQ_EXPR && u->n == 0)) lastb = i;
    }
    for (int i = 0; i < prog->n; i++) {
        tree_t * st = prog->c[i];
        if (!st) continue;
        const tree_t * u = st;
        if (u->t == TT_STMT) { const tree_t * sub = lrk_stmt_subj(u); if (sub) u = sub; }
        if (rk_eval_is_declaration(st)) ast_push(p2, st);
        else { rk_wrap_bodies(st); ast_push(sd, rk_ph_of(u) ? (tree_t *) u : i == lastb ? rk_tail_return((tree_t *) u) : st); }
    }
    ast_push(p2, sd);
    g_lower_rk.eval_road = 1;
    rk_stage2_core(p2, 0, 0);
    g_lower_rk.eval_road = 0;
    lower_raku_eval_reads_are_globals(pc0);
    return nm;
}
