#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include "ir/ir_node.h"
#include "ir/stage2_sm_prog_api.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "lower/lower_raku/lrk_01_sh.h"
#include "lower/lower_raku/lrk_02_rk.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_04_rk.h"
#include "lower/lower_raku/lrk_06_rk.h"
#include "lower/lower_raku/lrk_07_rk.h"
#include "lower/lower_raku/lrk_09_sh.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ast_node_new.h"
#include "runtime/builtins/gen_expr_t_defined.h"
#include "ir/ct_arena.h"
static tree_t * rk_fnc_cl(const char * nm, const tree_t * cl, const tree_t * a, const tree_t * b);
static int rk_proc_nparams(const char * name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_meth_is_bool(const char * m) {
    return m &&
        (!strcmp(m, "defined") || !strcmp(m, "Bool") || !strcmp(m, "so") || !strcmp(m, "not") || !strcmp(m, "does") || !strcmp(m, "isa") || !strcmp(m, "starts-with") || !strcmp(m, "ends-with") ||
        !strcmp(m, "contains"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_rcall_bool(rcx_t * cx, const tree_t * t, const char * nm, int from, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * bl = lrk_build(cx, IR_CALL, γ, ω);
    IR_LIT(bl).sval = "__rk_mkbool";
    IR_t * inner = NULL;
    IR_t * e = lower_rcall(cx, t, nm, from, bl, ω, &inner);
    if (inner) ir_operand_push(bl, inner);
    if (res) *res = bl;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * lower_rcall1(rcx_t * cx, const tree_t * recv, const char * nm, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = lrk_build(cx, IR_CALL, γ, ω);
    IR_LIT(nd).sval = nm;
    IR_t * ar = NULL;
    IR_t * ae = lower_rv(cx, recv, nd, ω, &ar);
    if (ar) ir_operand_push(nd, ar);
    if (res) *res = nd;
    return ae;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * rk_lower_str_operand(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    if (!t || t->t == TT_QLIT || t->t == TT_ILIT || t->t == TT_CAT) return lower_rv(cx, t, γ, ω, res);
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "!")) {
        tree_t * mc = ast_node_new(TT_METHCALL);
        ast_push(mc, (tree_t *) t);
        ast_push(mc, leaf_sval2(TT_QLIT, "Str"));
        return lower_rv(cx, mc, γ, ω, res);
    }
    return lower_rcall1(cx, t, "__rk_str", γ, ω, res);
}
IR_t * lower_rcall_skip1(rcx_t * cx, const tree_t * t, const char * nm, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_t * nd = lrk_build(cx, IR_CALL, γ, ω);
    IR_LIT(nd).sval = nm;
    int total = t->n - 1;
    IR_t * prev = NULL;
    IR_t * entry = nd;
    int k = 0;
    for (int ci = 0; ci < t->n; ci++) {
        if (ci == 1) continue;
        IR_t * ar = NULL;
        IR_t * ae = lower_rv(cx, t->c[ci], (k == total - 1) ? nd : NULL, ω, &ar);
        if (k == 0) entry = ae;
        if (prev && ae) lc_γ_to(prev, ae);
        prev = ar;
        if (ar) ir_operand_push(nd, ar);
        k++;
    }
    if (res) *res = nd;
    return entry;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_divis_desugar(const tree_t * t) {
    tree_t * md = ast_node_new(TT_MOD);
    ast_push(md, (tree_t *) t->c[0]);
    ast_push(md, (tree_t *) t->c[1]);
    tree_t * z = ast_node_new(TT_ILIT);
    z->v.ival = 0;
    tree_t * eq = ast_node_new(TT_EQ);
    ast_push(eq, md);
    ast_push(eq, z);
    return eq;
}
static int rk_is_code_var(const tree_t * t) { size_t n = (t && t->t == TT_VAR && t->v.sval) ? strlen(t->v.sval) : 0; return n > 6 && !strcmp(t->v.sval + n - 6, "__code"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_code_call1(const tree_t * prog, const tree_t * code, const tree_t * topic, int * ok) {
    const tree_t * cl = rk_closure_block(code) ? code : NULL, * B = cl ? cl->c[1] : code;
    *ok = 0;
    if (rk_is_code_var(code)) { tree_t * iv = ast_node_new(TT_INVOKE); ast_push(iv, (tree_t *) code); if (topic) ast_push(iv, (tree_t *) topic); *ok = 1; return iv; }
    if (!B || B->t != TT_ANON_BLOCK || !B->v.sval) return NULL;
    const tree_t * bd = prog ? rk_block_decl(prog, B->v.sval) : NULL;
    int np = prog ? (bd ? (int) bd->v.ival : -1) : rk_proc_nparams(B->v.sval), nref = cl ? cl->n - 2 : 0;
    if (np < 0 || np - nref > 1) return NULL;
    *ok = 1;
    return rk_fnc_cl(B->v.sval, cl, np - nref == 1 ? topic : NULL, NULL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_case_match(const tree_t * subj, const tree_t * cond) {
    if (cond->t == TT_FNC && cond->n > 1 && cond->c[0] && cond->c[0]->v.sval && !strcmp(cond->c[0]->v.sval, "__rk_regex")) {
        tree_t * m = ast_node_new(TT_SMATCH);
        ast_push(m, (tree_t *) subj);
        ast_push(m, cond->c[1]);
        ast_push(m, leaf_sval2(TT_QLIT, "match"));
        return m;
    }
    if (cond->t == TT_TO && cond->n > 1) {
        tree_t * ge = ast_node_new(TT_GE);
        ast_push(ge, (tree_t *) subj);
        ast_push(ge, (tree_t *) cond->c[0]);
        tree_t * le = ast_node_new(TT_LE);
        ast_push(le, (tree_t *) subj);
        ast_push(le, (tree_t *) cond->c[1]);
        tree_t * an = ast_node_new(TT_SEQ);
        ast_push(an, ge);
        ast_push(an, le);
        return an;
    }
    { int ok = 0; tree_t * bc = rk_code_call1(NULL, cond, subj, &ok); if (ok) return rk_fnc2("__rk_mkbool", bc, NULL); }
    if (cond->t != TT_VAR && cond->t != TT_ILIT && cond->t != TT_FLIT) return rk_fnc2("__rk_smartmatch", subj, cond);
    if (cond->t == TT_VAR) {
        tree_t * mc = ast_node_new(TT_FNC);
        mc->v.sval = (char *) intern("__rk_when_match");
        tree_t * nmv = ast_node_new(TT_VAR);
        nmv->v.sval = (char *) intern("__rk_when_match");
        ast_push(mc, nmv);
        ast_push(mc, (tree_t *) subj);
        ast_push(mc, (tree_t *) cond);
        return mc;
    }
    tree_t * eq = ast_node_new(cond->t == TT_QLIT ? TT_LEQ : TT_EQ);
    ast_push(eq, (tree_t *) subj);
    ast_push(eq, (tree_t *) cond);
    return eq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_proc_nparams(const char * name) {
    for (int i = 0; name && i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, name))
        return (g_stage2.proc_table[i].is_variadic || g_stage2.proc_table[i].named_rest) ? -1 : g_stage2.proc_table[i].nparams;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_user_meth_has_named(const char * m) {
    size_t L = strlen(m);
    for (int i = 0; i < g_stage2.proc_count; i++) {
        const char * n = g_stage2.proc_table[i].name;
        const tree_t * pr = g_stage2.proc_table[i].proc;
        size_t nl = n ? strlen(n) : 0;
        if (!pr || nl <= L + 2 || !strncmp(n, "__", 2) || strcmp(n + nl - L, m) || n[nl - L - 1] != '_' || n[nl - L - 2] != '_') continue;
        for (int k = 1; k < pr->n && k <= g_stage2.proc_table[i].nparams; k++) {
            const tree_t * pv = pr->c[k];
            if (pv && ((pv->slen & RK_PV_NAMED) || (pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT && pv->c[0]->v.sval && !strcmp(pv->c[0]->v.sval, "*%")))) return 1;
        }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_user_meth_named(const char * m) {
    size_t L = strlen(m);
    for (int i = 0; i < g_stage2.proc_count; i++) {
        const char * n = g_stage2.proc_table[i].name;
        size_t nl = n ? strlen(n) : 0;
        if (nl > L + 2 && strncmp(n, "__", 2) && !strcmp(n + nl - L, m) && n[nl - L - 1] == '_' && n[nl - L - 2] == '_') return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const tree_t * rk_closure_block(const tree_t * t) {
    if (t && t->t == TT_FNC && t->v.sval && !strcmp(t->v.sval, "__blk_close") && t->n > 1 && t->c[1] && t->c[1]->t == TT_ANON_BLOCK && t->c[1]->v.sval) return t->c[1];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_fnc_cl(const char * nm, const tree_t * cl, const tree_t * a, const tree_t * b) {
    tree_t * f = rk_fnc2(nm, NULL, NULL);
    for (int i = 2; cl && i < cl->n; i++) ast_push(f, cl->c[i]);
    if (a) ast_push(f, (tree_t *) a);
    if (b) ast_push(f, (tree_t *) b);
    return f;
}
static tree_t * rk_iter_call(const char * bn, const tree_t * cl, const tree_t * cv, const tree_t * a, const tree_t * b) {
    if (!cv) return rk_fnc_cl(bn, cl, a, b);
    tree_t * iv = ast_node_new(TT_INVOKE);
    ast_push(iv, (tree_t *) cv);
    if (a) ast_push(iv, (tree_t *) a);
    if (b) ast_push(iv, (tree_t *) b);
    return iv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_first_adverb(const tree_t * a, int * fend, int * fmode) {
    if (!a || a->t != TT_FNC || !a->v.sval || strcmp(a->v.sval, "__rk_pair") || a->n != 3 || !a->c[1] || a->c[1]->t != TT_QLIT || !a->c[1]->v.sval) return 0;
    const tree_t * b = a->c[2];
    if (!b || b->t != TT_FNC || !b->v.sval || strcmp(b->v.sval, "__rk_mkbool") || b->n != 2 || !b->c[1] || b->c[1]->t != TT_ILIT) return 0;
    const char * k = a->c[1]->v.sval;
    int m = !strcmp(k, "k") ? 1 : !strcmp(k, "kv") ? 2 : !strcmp(k, "p") ? 3 : !strcmp(k, "v") ? 0 : !strcmp(k, "end") ? -2 : -1;
    if (m == -1) return 0;
    if (b->c[1]->v.ival) { if (m == -2) *fend = 1; else *fmode = m; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t * rk_throw_desugar(rcx_t * cx, const tree_t * e) {
    char xn[40];
    snprintf(xn, sizeof xn, "$?xe%d", cx->g->n);
    const char * xv = lp_strdup(xn);
    tree_t * as = ast_node_new(TT_ASSIGN);
    ast_push(as, leaf_sval2(TT_VAR, xv));
    ast_push(as, (tree_t *) e);
    tree_t * mc = ast_node_new(TT_METHCALL);
    ast_push(mc, leaf_sval2(TT_VAR, xv));
    ast_push(mc, leaf_sval2(TT_QLIT, "message"));
    tree_t * tn = ast_node_new(TT_TERNARY);
    ast_push(tn, rk_fnc2("__rk_exc_umsg", leaf_sval2(TT_VAR, xv), NULL));
    ast_push(tn, mc);
    ast_push(tn, rk_fnc2("__rk_undef", NULL, NULL));
    tree_t * sq = ast_node_new(TT_SEQ_EXPR);
    ast_push(sq, as);
    ast_push(sq, rk_fnc2("__rk_die_m", leaf_sval2(TT_VAR, xv), tn));
    return sq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_first_ix(const char * src, const char * idx, int fend) {
    if (!fend) return leaf_sval2(TT_VAR, idx);
    tree_t * e1 = ast_node_new(TT_SUB);
    ast_push(e1, rk_fnc2("elems", leaf_sval2(TT_VAR, src), NULL));
    ast_push(e1, rk_ti(1));
    tree_t * rv = ast_node_new(TT_SUB);
    ast_push(rv, e1);
    ast_push(rv, leaf_sval2(TT_VAR, idx));
    return rv;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * rk_lower_iter_meth(rcx_t * cx, const tree_t * t, const char * mname, IR_t * γ, IR_t * ω, IR_t ** res) {
    int keyed = t->n == 4 && t->c[2] && t->c[2]->t == TT_QLIT && t->c[2]->v.sval && !strcmp(t->c[2]->v.sval, "as") &&
        (!strcmp(mname, "unique") || !strcmp(mname, "squish") || !strcmp(mname, "repeated"));
    int k = !strcmp(mname, "map") ? 1 : !strcmp(mname, "grep") ? 2 : !strcmp(mname, "first") ? 3 : !strcmp(mname, "reduce") ? 4 : !strcmp(mname, "sort") ? 5 : 0;
    if (!k) k = !strcmp(mname, "produce") ? 6 : keyed ? 7 : 0;
    int fend = 0, fmode = 0, fadv = k == 3 && t->n > 3;
    for (int i = 3; fadv && i < t->n; i++) if (!rk_first_adverb(t->c[i], &fend, &fmode)) fadv = 0;
    if (!fadv) fend = fmode = 0;
    const tree_t * fa = keyed ? t->c[3] : (t->n == 3 || fadv) ? t->c[2] : NULL;
    if ((k == 2 || k == 3) && t->slen != 79 && fa && fa->t == TT_VAR && fa->v.sval && fa->v.sval[0] && !strchr("&@%$", fa->v.sval[0]) && !rk_is_code_var(fa) && !rk_proc_known(fa->v.sval)) {
        char cn[48];
        snprintf(cn, sizeof cn, "$?fc%d__code", cx->g->n);
        const char * cvn = lp_strdup(cn);
        tree_t * mc = ast_node_new(TT_METHCALL);
        mc->line = t->line;
        for (int i = 0; i < t->n; i++) ast_push(mc, i == 2 ? leaf_sval2(TT_VAR, cvn) : t->c[i]);
        tree_t * as = ast_node_new(TT_ASSIGN);
        ast_push(as, leaf_sval2(TT_VAR, cvn));
        ast_push(as, (tree_t *) fa);
        tree_t * sq = ast_node_new(TT_SEQ_EXPR);
        ast_push(sq, as);
        ast_push(sq, mc);
        tree_t * mm = ast_node_new(TT_METHCALL);
        mm->line = t->line;
        mm->slen = 79;
        for (int i = 0; i < t->n; i++) ast_push(mm, t->c[i]);
        tree_t * tn = ast_node_new(TT_TERNARY);
        ast_push(tn, rk_fnc2("__rk_is_code", fa, NULL));
        ast_push(tn, sq);
        ast_push(tn, mm);
        return lower_rv(cx, tn, γ, ω, res);
    }
    const tree_t * cl = rk_closure_block(fa) ? fa : NULL;
    const char * bn = NULL;
    int scalar_code = k == 1 && fa && fa->t == TT_VAR && fa->v.sval && fa->v.sval[0] && !strchr("&@%$", fa->v.sval[0]) && !rk_proc_known(fa->v.sval);
    const tree_t * cv = (rk_is_code_var(fa) || scalar_code) ? fa : NULL;
    if (cl) fa = cl->c[1];
    if (fa && fa->t == TT_ANON_BLOCK && fa->v.sval) bn = fa->v.sval;
    else if (fa && fa->t == TT_VAR && fa->v.sval && fa->v.sval[0] == '&' && rk_proc_known(fa->v.sval + 1) && !rk_is_multi_name(fa->v.sval + 1)) bn = fa->v.sval + 1;
    if (!k || !(bn || (cv && k != 5)) || !t->c[0]) return NULL;
    int np = cv ? (k == 4 || k == 6 ? 2 : 1) : rk_proc_nparams(bn), nref = cl ? cl->n - 2 : 0;
    if (np < 0 && !cv && k >= 1 && k <= 3 && rk_proc_slurpy_only(bn, nref)) np = 1 + nref;
    if (np >= 0 && (np -= nref) < 0) return NULL;
    int step = (k == 1 && np > 1) ? np : (k == 4 && np > 2) ? np - 1 : 1;
    if (k == 4 ? np < 2 : k == 6 ? np != 2 : k == 5 ? np != 1 : (np < 0 || (k != 1 && np > 1))) return NULL;
    if (rk_user_meth_named(mname) || (k > 3 && t->c[0]->t == TT_VAR && t->c[0]->v.sval && t->c[0]->v.sval[0] == '%')) return NULL;
    char b1[40], b2[40], b3[40], b4[40];
    int id = cx->g->n;
    snprintf(b1, sizeof b1, "$?isrc%d", id);
    snprintf(b2, sizeof b2, "$?ires%d", id);
    snprintf(b3, sizeof b3, "$?iidx%d", id);
    snprintf(b4, sizeof b4, "$?iel%d", id);
    const char * src = lp_strdup(b1), * acc = lp_strdup(b2), * idx = lp_strdup(b3), * el = lp_strdup(b4);
    IR_t * fin, * fr = NULL;
    if (k <= 2) fin = lower_rcall(cx, rk_fnc2("__rk_iter_done", leaf_sval2(TT_VAR, acc), NULL), "__rk_iter_done", 1, γ, ω, &fr);
    else if (k == 5) fin = lower_rcall(cx, rk_fnc2("__rk_sort_by_keys", leaf_sval2(TT_VAR, src), leaf_sval2(TT_VAR, acc)), "__rk_sort_by_keys", 1, γ, ω, &fr);
    else if (k >= 6) {
        const char * dn = k == 6 ? "__rk_produce_done" : "__rk_keyed_done";
        fin = lower_rcall(cx, rk_fnc2(dn, leaf_sval2(TT_VAR, acc), NULL), dn, 1, γ, ω, &fr);
    } else {
        fin = lrk_build(cx, IR_VAR, γ, ω);
        IR_LIT(fin).sval = acc;
        fr = fin;
    }
    IR_t * va = lrk_build(cx, IR_ASSIGN, NULL, ω);
    IR_LIT(va).sval = idx;
    IR_t * to = lrk_build(cx, IR_TO, va, fin);
    IR_LIT(to).sval = "ag";
    tree_t * lo = ast_node_new(TT_ILIT);
    lo->v.ival = (k == 4 && step == 1) || k == 6 ? 1 : 0;
    tree_t * one = ast_node_new(TT_ILIT);
    one->v.ival = 1;
    tree_t * nel = rk_fnc2("elems", leaf_sval2(TT_VAR, src), NULL);
    if (k == 4 && step > 1) { tree_t * o1 = ast_node_new(TT_ILIT); o1->v.ival = 1; tree_t * sb = ast_node_new(TT_SUB); ast_push(sb, nel); ast_push(sb, o1); nel = sb; }
    if (step > 1) { tree_t * sn = ast_node_new(TT_ILIT); sn->v.ival = step; nel = rk_fnc2("__rk_intdiv", nel, sn); }
    tree_t * hi = ast_node_new(TT_SUB);
    ast_push(hi, nel);
    ast_push(hi, one);
    IR_t * rlo = NULL, * rhi = NULL;
    IR_t * elo = lower_rv(cx, lo, NULL, ω, &rlo);
    IR_t * ehi = lower_rv(cx, hi, to, ω, &rhi);
    lrk_γ_to(rlo, ehi);
    ir_operand_push(to, rlo);
    ir_operand_push(to, rhi);
    if (rhi && ir_is_generator_kind(to->op)) lc_γ_to(rhi, to);
    ir_operand_push(va, to);
    tree_t * at = rk_fnc2("__rk_arr_at", leaf_sval2(TT_VAR, src), rk_first_ix(src, idx, fend));
    IR_t * bentry, * r = NULL;
    if (k == 1 || k == 4 || k == 5 || k == 6 || k == 7) {
        IR_t * as = lrk_build(cx, IR_ASSIGN, to, to);
        IR_LIT(as).sval = acc;
        tree_t * call = k == 4 ? rk_iter_call(bn, cl, cv, leaf_sval2(TT_VAR, acc), at) : rk_iter_call(bn, cl, cv, np ? at : NULL, NULL);
        if (step > 1) {
            call = rk_iter_call(bn, cl, cv, k == 4 ? leaf_sval2(TT_VAR, acc) : NULL, NULL);
            for (int j = 0; j < step; j++) {
                tree_t * sn = ast_node_new(TT_ILIT);
                sn->v.ival = step;
                tree_t * jn = ast_node_new(TT_ILIT);
                jn->v.ival = k == 4 ? j + 1 : j;
                tree_t * mu = ast_node_new(TT_MUL);
                ast_push(mu, leaf_sval2(TT_VAR, idx));
                ast_push(mu, sn);
                tree_t * ad = ast_node_new(TT_ADD);
                ast_push(ad, mu);
                ast_push(ad, jn);
                ast_push(call, rk_fnc2("__rk_arr_at", leaf_sval2(TT_VAR, src), ad));
            }
        }
        if (k == 1 || k == 5) call = rk_fnc2(k == 1 ? "__rk_map_append" : "__rk_grep_append", leaf_sval2(TT_VAR, acc), call);
        if (k == 6) call = rk_fnc2("__rk_produce_step", leaf_sval2(TT_VAR, acc), rk_iter_call(bn, cl, cv, rk_fnc2("__rk_produce_last", leaf_sval2(TT_VAR, acc), NULL), at));
        if (k == 7) { tree_t * kc = rk_iter_call(bn, cl, cv, np ? at : NULL, NULL); call = rk_fnc2("__rk_keyed_step", leaf_sval2(TT_VAR, acc), at); ast_push(call, kc); }
        bentry = call->t == TT_INVOKE ? lower_rv(cx, call, as, to, &r) : lower_rcall(cx, call, call->v.sval, 1, as, to, &r);
        if (r) ir_operand_push(as, r);
    } else {
        IR_t * hit;
        if (k == 2) {
            IR_t * as = lrk_build(cx, IR_ASSIGN, to, to);
            IR_LIT(as).sval = acc;
            hit = lower_rcall(cx, rk_fnc2("__rk_grep_append", leaf_sval2(TT_VAR, acc), leaf_sval2(TT_VAR, el)), "__rk_grep_append", 1, as, to, &r);
            if (r) ir_operand_push(as, r);
        } else if (fmode) {
            IR_t * as = lrk_build(cx, IR_ASSIGN, fin, to);
            IR_LIT(as).sval = acc;
            tree_t * sh = rk_fnc2("__rk_first_shape", leaf_sval2(TT_VAR, src), rk_first_ix(src, idx, fend));
            ast_push(sh, rk_ti(fmode));
            IR_t * sr = NULL;
            hit = lower_rcall(cx, sh, "__rk_first_shape", 1, as, to, &sr);
            if (sr) ir_operand_push(as, sr);
        } else {
            IR_t * as = lrk_build(cx, IR_ASSIGN, fin, to);
            IR_LIT(as).sval = acc;
            hit = lrk_build(cx, IR_VAR, as, to);
            IR_LIT(hit).sval = el;
            ir_operand_push(as, hit);
        }
        IR_t * ce = lower_cond(cx, rk_iter_call(bn, cl, cv, np ? leaf_sval2(TT_VAR, el) : NULL, NULL), hit, to);
        IR_t * ael = lrk_build(cx, IR_ASSIGN, ce, to);
        IR_LIT(ael).sval = el;
        bentry = lower_rcall(cx, at, "__rk_arr_at", 1, ael, to, &r);
        if (r) ir_operand_push(ael, r);
    }
    lrk_γ_to(va, bentry);
    IR_t * ares = lrk_build(cx, IR_ASSIGN, elo, ω);
    IR_LIT(ares).sval = acc;
    tree_t * init = (k <= 2 || k == 5) ? rk_fnc2("__rk_iter_src", NULL, NULL) : k == 3 ? rk_fnc2("__rk_undef", NULL, NULL) : k == 6 ? rk_fnc2("__rk_produce_init", leaf_sval2(TT_VAR, src), NULL) :
        k == 7 ? rk_fnc2("__rk_keyed_init", rk_ti(mname[0] == 'u' ? 0 : mname[0] == 's' ? 1 : 2), NULL) : rk_fnc2("__rk_arr_at", leaf_sval2(TT_VAR, src), ast_node_new(TT_ILIT));
    IR_t * eini = lower_rcall(cx, init, init->v.sval, 1, ares, ω, &r);
    if (r) ir_operand_push(ares, r);
    IR_t * asrc = lrk_build(cx, IR_ASSIGN, eini, ω);
    IR_LIT(asrc).sval = src;
    IR_t * esi = lower_rcall(cx, rk_fnc2("__rk_iter_src", t->c[0], NULL), "__rk_iter_src", 1, asrc, ω, &r);
    if (r) ir_operand_push(asrc, r);
    *res = fr;
    return esi;
}
const char * rk_ph_id(const char * v) { return v[0] == '^' ? v + 1 : ((v[0] == '@' || v[0] == '%' || v[0] == '&') && v[1] == '^') ? v + 2 : NULL; }
int rk_ph_cmp(const char * a, const char * b) { const char * x = rk_ph_id(a), * y = rk_ph_id(b); return strcmp(x ? x : a, y ? y : b); }
void rk_scan_implicit_params(const tree_t * t, int * topic, const char ** ph, int * nph, int max) {
    if (!t) return;
    if (t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_ARM) *topic |= 2;
    if (t->t == TT_VAR && t->v.sval) {
        const char * v = t->v.sval;
        if (!strcmp(v, "_")) *topic |= 1;
        else if (rk_ph_id(v)) { int seen = 0; for (int i = 0; i < *nph; i++) if (!strcmp(ph[i], v)) seen = 1; if (!seen && *nph < max) ph[(*nph)++] = v; }
    }
    for (int i = 0; i < t->n; i++) rk_scan_implicit_params(t->c[i], topic, ph, nph, max);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_topic_default(tree_t * seq) {
    tree_t * def = ast_node_new(TT_METHCALL);
    ast_push(def, leaf_sval2(TT_VAR, "_"));
    ast_push(def, leaf_sval2(TT_QLIT, "defined"));
    tree_t * set = ast_node_new(TT_ASSIGN);
    ast_push(set, leaf_sval2(TT_VAR, "_"));
    ast_push(set, rk_fnc2("__rk_undef", NULL, NULL));
    tree_t * arm = ast_node_new(TT_SEQ_EXPR);
    ast_push(arm, set);
    tree_t * un = ast_node_new(TT_UNLESS);
    ast_push(un, def);
    ast_push(un, arm);
    ast_push(seq, un);
    for (int i = seq->n - 1; i > 0; i--) { tree_t * x = seq->c[i]; seq->c[i] = seq->c[i - 1]; seq->c[i - 1] = x; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ph_rename(tree_t * t, const char * from, const char * to) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, from)) t->v.sval = (char *) to;
    for (int i = 0; i < t->n; i++) rk_ph_rename(t->c[i], from, to);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_named_ph_scan(const tree_t * t, rk_ns_t * out) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && t->v.sval && t->v.sval[0] == ':' && ((t->v.sval[1] >= 'a' && t->v.sval[1] <= 'z') || (t->v.sval[1] >= 'A' && t->v.sval[1] <= 'Z') || t->v.sval[1] == '_'))
        rk_ns_add(out, t->v.sval);
    for (int i = 0; i < t->n; i++) rk_named_ph_scan(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_named_ph_rename(tree_t * t, const char * from, const char * to) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, from)) t->v.sval = (char *) to;
    for (int i = 0; i < t->n; i++) rk_named_ph_rename(t->c[i], from, to);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_named_placeholders(tree_t * t) {
    rk_ns_t np = { 0 };
    for (int i = 1 + (int) t->v.ival; i < t->n; i++) rk_named_ph_scan(t->c[i], &np);
    if (!np.n) return;
    int at = 1 + (int) t->v.ival;
    for (int k = 0; k < np.n; k++) {
        const char * nm = intern(np.v[k] + 1);
        for (int i = at; i < t->n; i++) rk_named_ph_rename(t->c[i], np.v[k], nm);
        ast_push(t, NULL);
        for (int j = t->n - 1; j > at; j--) t->c[j] = t->c[j - 1];
        t->c[at++] = leaf_sval2(TT_VAR, nm);
        t->v.ival++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_sub_placeholders(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_sub_placeholders(t->c[i]);
    if (t->t != TT_SUB_DECL || t->v.ival != 0 || t->n < 1) return;
    int topic = 0, nph = 0, phmax = rk_tree_size(t) + 1;
    const char ** ph = (const char **) ct_alloc(sizeof(const char *) * (size_t) phmax);
    for (int i = 1; i < t->n; i++) rk_scan_implicit_params(t->c[i], &topic, ph, &nph, phmax);
    if (nph == 0) { rk_named_placeholders(t); return; }
    for (int a = 1; a < nph; a++) { const char * key = ph[a]; int b = a - 1; while (b >= 0 && rk_ph_cmp(ph[b], key) > 0) { ph[b + 1] = ph[b]; b--; } ph[b + 1] = key; }
    int nbody = t->n - 1;
    tree_t ** body = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) nbody);
    for (int i = 0; i < nbody; i++) body[i] = t->c[i + 1];
    t->n = 1;
    for (int k = 0; k < nph; k++) ast_push(t, leaf_sval2(TT_VAR, ph[k]));
    for (int i = 0; i < nbody; i++) { for (int k = 0; k < nph; k++) rk_ph_rename(body[i], intern(ph[k] + 1), ph[k]); ast_push(t, body[i]); }
    t->v.ival = nph;
    rk_named_placeholders(t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_ph_alias(tree_t * t) {
    if (!t) return;
    if (t->t == TT_ANON_BLOCK && t->n == 1 && t->c[0]) {
        int topic = 0, nph = 0, phmax = rk_tree_size(t->c[0]) + 1;
        const char ** ph = (const char **) ct_alloc(sizeof(const char *) * (size_t) phmax);
        rk_scan_implicit_params(t->c[0], &topic, ph, &nph, phmax);
        for (int k = 0; k < nph; k++) rk_ph_rename(t->c[0], intern(ph[k] + 1), ph[k]);
    }
    for (int i = 0; i < t->n; i++) rk_ph_alias(t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_subst_code_walk(const tree_t * prog, tree_t * t, int * seq) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_subst_code_walk(prog, t->c[i], seq);
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "re_subst") || t->n < 8) return;
    const tree_t * cl = rk_closure_block(t->c[3]) ? t->c[3] : NULL, * B = cl ? cl->c[1] : t->c[3];
    if (!B || B->t != TT_ANON_BLOCK || !B->v.sval) return;
    char sv[32];
    snprintf(sv, sizeof sv, "__rk_sst%d", (*seq)++);
    const char * st = lp_strdup(sv);
    const tree_t * bd = rk_block_decl(prog, B->v.sval);
    tree_t * op = rk_fnc2("__rk_subst_open", t->c[1], t->c[2]);
    for (int i = 4; i < t->n; i++) ast_push(op, t->c[i]);
    tree_t * wh = ast_node_new(TT_WHILE);
    ast_push(wh, rk_fnc2("__rk_subst_next", rk_tv(st), NULL));
    ast_push(wh, rk_fnc2("__rk_subst_put", rk_tv(st), rk_fnc_cl(B->v.sval, cl, (bd ? (int) bd->v.ival : -1) - (cl ? cl->n - 2 : 0) == 1 ? rk_tv("/") : NULL, NULL)));
    tree_t * fin = rk_fnc2("__rk_subst_close", rk_tv(st), NULL);
    int line = t->line;
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    t->line = line;
    ast_push(t, rk_tset(st, op));
    ast_push(t, wh);
    ast_push(t, fin);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_make_walk(tree_t * t, const char * slash) {
    if (!t) return;
    if (t->t == TT_SUB_DECL) {
        slash = "/";
        for (int k = 0; k < (int) t->v.ival && 1 + k < t->n; k++) { const tree_t * pv = t->c[1 + k]; if (pv && pv->t == TT_VAR && pv->v.sval && !strcmp(pv->v.sval, "__slash")) slash = "__slash"; }
    }
    for (int i = 0; i < t->n; i++) rk_make_walk(t->c[i], slash);
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "make") || t->n != 2 || rk_proc_known("make")) return;
    tree_t * arg = t->c[1];
    t->v.sval = (char *) "__rk_make";
    t->c[0] = leaf_sval2(TT_VAR, "__rk_make");
    t->c[1] = leaf_sval2(TT_VAR, slash);
    ast_push(t, arg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_nsub_decl_name(const tree_t * d) {
    return (d && d->t == TT_SUB_DECL && d->n > 0 && d->c[0] && d->c[0]->t == TT_VAR && d->c[0]->v.sval && d->c[0]->v.sval[0] && !strchr(d->c[0]->v.sval, '$')) ? d->c[0]->v.sval : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_nsub_count(const tree_t * t, int in_pkg, int nest, rk_ns_t * seen, rk_ns_t * dup) {
    extern int rk_is_core_routine(const char *);
    if (!t) return;
    const char * nm = in_pkg ? NULL : rk_nsub_decl_name(t);
    if (nm) { if (rk_ns_has(seen, nm) || (nest > 0 && rk_is_core_routine(nm))) rk_ns_add(dup, nm); else rk_ns_add(seen, nm); }
    int cn = nest < 0 ? 0 : (nest || t->t == TT_SUB_DECL || t->t == TT_SEQ_EXPR || t->t == TT_ANON_BLOCK);
    for (int i = 0; i < t->n; i++) rk_nsub_count(t->c[i], t->t == TT_CLASS_DECL || t->t == TT_ROLE_DECL || t->t == TT_GRAMMAR_DECL, cn, seen, dup);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_nsub_find(const rk_nsb_t * e, const char * nm) { for (; e; e = e->up) if (!strcmp(e->from, nm)) return e->to; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * rk_nsub_amp(const rk_nsb_t * e, const char * nm) {
    const char * to = (e && nm && nm[0] == '&') ? rk_nsub_find(e, nm + 1) : NULL;
    if (!to) return NULL;
    char b[strlen(to) + 2];
    b[0] = '&';
    strcpy(b + 1, to);
    return intern(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_nsub_walk(tree_t * t, const rk_nsb_t * env, const rk_ns_t * dup, int * ctr, int root) {
    if (!t) return;
    const rk_nsb_t * e = env;
    int nk = (!root && (t->t == TT_SUB_DECL || t->t == TT_SEQ_EXPR)) ? t->n : 0;
    rk_nsb_t nb[nk + 1];
    int nn = 0;
    for (int i = (t->t == TT_SUB_DECL) ? 1 : 0; i < nk; i++) {
        tree_t * d = (t->c[i] && t->c[i]->t == TT_STMT) ? (tree_t *) lrk_stmt_subj(t->c[i]) : t->c[i];
        const char * nm = rk_nsub_decl_name(d);
        if (!nm || !rk_ns_has(dup, nm)) continue;
        char b[strlen(nm) + 24];
        snprintf(b, sizeof b, "%s-ns%d", nm, ++*ctr);
        nb[nn] = (rk_nsb_t) { e, nm, intern(b) };
        e = &nb[nn++];
        d->c[0]->v.sval = (char *) e->to;
    }
    if (e && t->t == TT_FNC && t->v.sval) {
        const char * to = rk_nsub_find(e, t->v.sval);
        if (to && t->n > 0 && t->c[0] && t->c[0]->t == TT_VAR && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, t->v.sval)) t->c[0]->v.sval = (char *) to;
        if (to) t->v.sval = (char *) to;
    }
    if (e && t->t == TT_VAR) { const char * to = rk_nsub_amp(e, t->v.sval); if (to) t->v.sval = (char *) to; }
    if (e && t->t == TT_VAR && (t->slen & 1) && t->v.sval) { const char * to = rk_nsub_find(e, t->v.sval); if (to) t->v.sval = (char *) to; }
    for (int i = 0; i < t->n; i++) rk_nsub_walk(t->c[i], e, dup, ctr, 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_nested_sub_names(tree_t * prog) { rk_ns_t seen = { 0 }, dup = { 0 }; int ctr = 0; rk_nsub_count(prog, 0, -1, &seen, &dup); if (dup.n) rk_nsub_walk(prog, NULL, &dup, &ctr, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tree_has_sub(const tree_t * t, const char * nm) {
    if (!t) return 0;
    size_t l = strlen(nm);
    const char * dn = (t->t == TT_SUB_DECL && t->n > 0 && t->c[0]) ? t->c[0]->v.sval : NULL;
    if (dn && !strncmp(dn, nm, l) && (dn[l] == 0 || dn[l] == '$')) return 1;
    for (int i = 0; i < t->n; i++) if (rk_tree_has_sub(t->c[i], nm)) return 1;
    return 0;
}
static int rk_tree_has_var(const tree_t * t, const char * nm) {
    if (!t) return 0;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, nm)) return 1;
    for (int i = 0; i < t->n; i++) if (rk_tree_has_var(t->c[i], nm)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_amp_methcalls(const tree_t * prog, tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_amp_methcalls(prog, t->c[i]);
    const char * mn = (t->t == TT_METHCALL && t->n > 1 && t->c[1] && t->c[1]->t == TT_QLIT) ? t->c[1]->v.sval : NULL;
    if (!mn || mn[0] != '&' || !mn[1]) return;
    int isv = mn[1] == '$';
    const char * nm = lp_strdup(mn + 1 + isv);
    if (!nm[0]) return;
    for (const char * q = nm; *q; q++) if (!isalnum((unsigned char) *q) && *q != '_' && *q != '-') return;
    char cn[strlen(nm) + 8];
    snprintf(cn, sizeof cn, "%s__code", nm);
    int code = !isv && rk_tree_has_var(prog, cn);
    if (!isv && !code && !rk_tree_has_sub(prog, nm)) { t->c[1] = leaf_sval2(TT_QLIT, nm); return; }
    tree_t * f = ast_node_new(isv || code ? TT_INVOKE : TT_FNC);
    f->line = t->line;
    if (!isv && !code) f->v.sval = (char *) nm;
    ast_push(f, leaf_sval2(TT_VAR, code ? lp_strdup(cn) : nm));
    ast_push(f, t->c[0]);
    for (int i = 2; i < t->n; i++) ast_push(f, t->c[i]);
    *t = *f;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_bool_rx(tree_t ** sp) {
    tree_t * t = *sp;
    if (!t) return;
    if (t->t == TT_FNC && t->n > 1 && t->c[0] && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__rk_regex")) { *sp = rk_case_match(leaf_sval2(TT_VAR, "_"), t); return; }
    if (t->t == TT_SEQ || t->t == TT_ALT || t->t == TT_NOT) for (int i = 0; i < t->n; i++) rk_bool_rx(&t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_bool_regexes(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_bool_regexes(t->c[i]);
    if ((t->t == TT_IF || t->t == TT_UNLESS || t->t == TT_WHILE || t->t == TT_UNTIL || t->t == TT_NOT || t->t == TT_TERNARY) && t->n > 0) rk_bool_rx(&t->c[0]);
    else if (t->t == TT_REPEAT && t->n > 1) rk_bool_rx(&t->c[1]);
    else if (t->t == TT_GREP && t->n > 0)
        rk_bool_rx((t->n == 1 && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__rk_arr") && t->c[0]->n >= 3) ? &t->c[0]->c[1] : &t->c[0]);
    else if (t->t == TT_METHCALL && t->n > 2 && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval && !strcmp(t->c[1]->v.sval, "grep") && t->c[2] && t->c[2]->t == TT_ANON_BLOCK && t->c[2]->n > 0 &&
        t->c[2]->c[0] && t->c[2]->c[0]->t == TT_SEQ_EXPR && t->c[2]->c[0]->n > 0) rk_bool_rx(&t->c[2]->c[0]->c[t->c[2]->c[0]->n - 1]);
    else if (t->t == TT_FNC && t->n == 2 && t->v.sval && !strcmp(t->v.sval, "__rk_mkbool")) rk_bool_rx(&t->c[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_relaxed_collect(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if (t->t == TT_SUB_DECL && t->n > 0 && t->c[0] && t->c[0]->v.sval) {
        for (int k = 1; k < t->n && k <= (int) t->v.ival; k++) {
            const tree_t * pv = t->c[k];
            if (pv && ((pv->slen & RK_PV_NAMED) || (pv->n > 0 && pv->c[0] && pv->c[0]->t == TT_QLIT && pv->c[0]->v.sval && !strcmp(pv->c[0]->v.sval, "*%")))) {
                const char * nm = t->c[0]->v.sval;
                const char * dl = strchr(nm, '$');
                char b[strlen(nm) + 1];
                snprintf(b, sizeof b, "%.*s", dl ? (int) (dl - nm) : (int) strlen(nm), nm);
                rk_ns_add(out, lp_strdup(b));
                break;
            }
        }
    }
    for (int i = 0; i < t->n; i++) rk_relaxed_collect(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_relaxed_walk(tree_t * t, const rk_ns_t * set) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_relaxed_walk(t->c[i], set);
    if (t->t != TT_METHCALL || t->n < 3 || !t->c[1] || t->c[1]->t != TT_QLIT || !t->c[1]->v.sval) return;
    int first = -1;
    for (int i = 2; i < t->n && first < 0; i++) if (t->c[i] && t->c[i]->t == TT_QLIT && (t->c[i]->slen == 0x4E41 || t->c[i]->slen == 0x4E42)) first = i;
    if (first < 0) return;
    int n = t->n;
    tree_t ** old = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) n);
    for (int i = 0; i < n; i++) old[i] = t->c[i];
    const char * mn = old[1]->v.sval;
    if (rk_ns_has(set, mn) || rk_user_meth_has_named(mn)) {
        int nold = 0;
        for (int i = first; i < n; i += 2) if (old[i] && old[i]->t == TT_QLIT && old[i]->slen == 0x4E41) nold++;
        t->t = TT_FNC;
        t->v.sval = (char *) "__rk_meth_nc";
        t->n = 0;
        ast_push(t, leaf_sval2(TT_VAR, "__rk_meth_nc"));
        ast_push(t, old[0]);
        ast_push(t, old[1]);
        ast_push(t, rk_ti(first - 2));
        ast_push(t, rk_ti(nold));
        for (int i = 2; i < n; i++) ast_push(t, old[i]);
        return;
    }
    t->n = 0;
    for (int i = 0; i < n; i++) {
        int mk = (i >= 2 && old[i] && old[i]->t == TT_QLIT) ? old[i]->slen : 0;
        if (mk == 0x4E42) { if (++i < n) ast_push(t, old[i]); continue; }
        if (mk == 0x4E41) old[i]->slen = 0;
        ast_push(t, old[i]);
    }
}
