#include "lower/lower_raku/lrk_head.h"
#include <string.h>
#include <stdlib.h>
#include "ir/ir_node.h"
#include "ir/ir_graph.h"
#include "parsers/snobol4/scrip_cc.h"
#include "lower/lower_common.h"
#include "lower/lower_raku/lrk_01_sh.h"
#include "lower/lower_raku/lrk_03_rk.h"
#include "lower/lower_raku/lrk_04_rk.h"
#include "lower/lower_raku/lrk_05_rk.h"
#include "lower/lower_raku/lrk_07_rk.h"
#include "lower/lower_raku/lrk_09_sh.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast.h"
#include "ir/ast_node_new.h"
#include "runtime/builtins/gen_expr_t_defined.h"
#include "ir/ct_arena.h"
static rkrx_t rk_rx_lower(rcx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t * land, const rkrx_groups_t * gr);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_destr_count(const char * pn) { if (!pn || (pn[0] != '[' && pn[0] != '(')) return 0; int n = 1; for (const char * q = pn; *q; q++) if (*q == ',') n++; return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_destr_plan(const char * pn, const tree_t ** plan, int nplan) {
    if (!pn || (pn[0] != '[' && pn[0] != '(')) return nplan;
    const char * q = pn + 1;
    int idx = 0;
    for (;;) {
        while (*q == ' ' || *q == '\t' || *q == '\n') q++;
        if (!*q || *q == ']' || *q == ')') break;
        int slurp = 0;
        if (*q == '*') { slurp = 1; q++; }
        const char * sg = q;
        tree_t * rhs = NULL, * nm = NULL;
        if (*q == '$' || *q == '@' || *q == '%') {
            const char * e = q + 1;
            while (isalnum((unsigned char) *e) || *e == '_' || ((*e == '-' || *e == '\'') && isalpha((unsigned char) e[1]))) e++;
            if (e > q + 1) {
                char nb[(size_t) (e - q) + 1];
                memcpy(nb, q, (size_t) (e - q));
                nb[e - q] = 0;
                nm = leaf_sval2(TT_VAR, *q == '$' ? lp_strdup(nb + 1) : lp_strdup(nb));
                if (*q == '$' && !slurp) {
                    rhs = ast_node_new(TT_ARR_GET);
                    ast_push(rhs, leaf_sval2(TT_VAR, pn));
                    tree_t * ix = ast_node_new(TT_ILIT);
                    ix->v.ival = idx;
                    ast_push(rhs, ix);
                } else {
                    rhs = ast_node_new(TT_METHCALL);
                    ast_push(rhs, leaf_sval2(TT_VAR, pn));
                    ast_push(rhs, leaf_sval2(TT_QLIT, "skip"));
                    tree_t * ix = ast_node_new(TT_ILIT);
                    ix->v.ival = idx;
                    ast_push(rhs, ix);
                }
            }
            q = e;
        } else if (*q == ':' && isalpha((unsigned char) q[1])) {
            const char * w = q + 1, * e = w;
            while (isalnum((unsigned char) *e) || *e == '_' || *e == '-') e++;
            if (*e == '(' && e[1] == '$') {
                const char * v = e + 2, * ve = v;
                while (isalnum((unsigned char) *ve) || *ve == '_' || *ve == '-') ve++;
                if (ve > v && *ve == ')') {
                    char wb[(size_t) (e - w) + 1], vb[(size_t) (ve - v) + 1];
                    memcpy(wb, w, (size_t) (e - w));
                    wb[e - w] = 0;
                    memcpy(vb, v, (size_t) (ve - v));
                    vb[ve - v] = 0;
                    nm = leaf_sval2(TT_VAR, lp_strdup(vb));
                    rhs = ast_node_new(TT_METHCALL);
                    ast_push(rhs, leaf_sval2(TT_VAR, pn));
                    ast_push(rhs, leaf_sval2(TT_QLIT, lp_strdup(wb)));
                    q = ve + 1;
                    idx--;
                }
            }
        }
        (void) sg;
        if (nm && rhs) { tree_t * as = ast_node_new(TT_ASSIGN); ast_push(as, nm); ast_push(as, rhs); plan[nplan++] = as; }
        idx++;
        int depth = 0;
        while (*q && !((*q == ',' || *q == ']' || *q == ')') && depth == 0)) { if (*q == '[' || *q == '(') depth++; else if (*q == ']' || *q == ')') depth--; q++; }
        if (*q == ',') q++;
    }
    return nplan;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_subsig_count(const tree_t * pv) {
    int n = 0;
    for (int j = 0; pv && j < pv->n; j++) {
        const tree_t * sq = pv->c[j];
        if (!sq || sq->t != TT_SEQ_EXPR) continue;
        for (int i = 0; i < sq->n; i++) { const tree_t * sp = sq->c[i]; n += 3 + ((sp && sp->t == TT_ASSIGN && sp->n) ? rk_subsig_count(sp->c[0]) : rk_subsig_count(sp)); }
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_subsig_plan(const tree_t * pv, const char * src, const tree_t ** plan, int nplan) {
    for (int j = 0; pv && j < pv->n; j++) {
        const tree_t * sq = pv->c[j];
        if (!sq || sq->t != TT_SEQ_EXPR) continue;
        int ix = 0;
        for (int i = 0; i < sq->n; i++) {
            const tree_t * sp = sq->c[i];
            const tree_t * v = (sp && sp->t == TT_ASSIGN && sp->n) ? sp->c[0] : sp;
            if (!v || v->t != TT_VAR || !v->v.sval) continue;
            const char * key = v->v.sval, * var = key;
            for (int a = 0; a < v->n; a++) if (v->c[a] && v->c[a]->t == TT_VAR && (v->c[a]->slen & RK_PV_ALIAS)) var = v->c[a]->v.sval;
            if (!var || !*var) continue;
            int slurp = v->n > 0 && v->c[0] && v->c[0]->t == TT_QLIT && v->c[0]->v.sval && v->c[0]->v.sval[0] == '*';
            tree_t * rhs = NULL;
            if (v->slen & RK_PV_NAMED) {
                rhs = ast_node_new(TT_FNC);
                rhs->v.sval = (char *) "__rk_unpack_named";
                ast_push(rhs, leaf_sval2(TT_VAR, "__rk_unpack_named"));
                ast_push(rhs, leaf_sval2(TT_VAR, src));
                ast_push(rhs, leaf_sval2(TT_QLIT, (key[0] == '@' || key[0] == '%') ? key + 1 : key));
            } else if (slurp) {
                if (var[0] == '%') continue;
                rhs = ast_node_new(TT_METHCALL);
                ast_push(rhs, leaf_sval2(TT_VAR, src));
                ast_push(rhs, leaf_sval2(TT_QLIT, "skip"));
                tree_t * ixn = ast_node_new(TT_ILIT);
                ixn->v.ival = ix;
                ast_push(rhs, ixn);
            } else {
                rhs = ast_node_new(TT_ARR_GET);
                ast_push(rhs, leaf_sval2(TT_VAR, src));
                tree_t * ixn = ast_node_new(TT_ILIT);
                ixn->v.ival = ix++;
                ast_push(rhs, ixn);
            }
            tree_t * as = ast_node_new(TT_ASSIGN), * dv = leaf_sval2(TT_VAR, var);
            dv->slen |= 4;
            ast_push(as, dv);
            ast_push(as, rhs);
            plan[nplan++] = as;
            if (sp != v) {
                tree_t * df = ast_node_new(TT_METHCALL), * un = ast_node_new(TT_UNLESS), * bd = ast_node_new(TT_SEQ_EXPR), * da = ast_node_new(TT_ASSIGN);
                ast_push(df, leaf_sval2(TT_VAR, var));
                ast_push(df, leaf_sval2(TT_QLIT, "defined"));
                ast_push(da, leaf_sval2(TT_VAR, var));
                ast_push(da, sp->c[1]);
                ast_push(bd, da);
                ast_push(un, df);
                ast_push(un, bd);
                plan[nplan++] = un;
            }
            nplan = rk_subsig_plan(v, var, plan, nplan);
        }
    }
    return nplan;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * rk_coerce_base(const char * ty) {
    if (!ty) return NULL;
    const char * lp = strchr(ty, '(');
    size_t bl = lp ? (size_t) (lp - ty) : 0;
    if (!bl || ty[strlen(ty) - 1] != ')') return NULL;
    static const char * const k[] = { "Int", "Str", "Num", "Rat", "Bool", 0 };
    for (int i = 0; k[i]; i++) if (strlen(k[i]) == bl && !strncmp(ty, k[i], bl)) return k[i];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static rk_loopfr_t * rk_loop_find(rcx_t * cx, const char * label) { rk_loopfr_t * lf = cx->loops; while (lf && label && (!lf->label || strcmp(lf->label, label))) lf = lf->up; return lf; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_loop_node(const tree_t * t) { return t && (t->t == TT_WHILE || t->t == TT_UNTIL || t->t == TT_REPEAT || t->t == TT_CLOOP || t->t == TT_FOR_RANGE || t->t == TT_EVERY); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_multi_for(const tree_t * t) {
    return t && t->t == TT_SEQ && t->n == 2 && t->c[0] && t->c[0]->t == TT_ASSIGN && t->c[0]->n == 2 && t->c[0]->c[0] && t->c[0]->c[0]->v.sval && !strncmp(t->c[0]->c[0]->v.sval, "__fm_a_", 7) &&
        rk_is_loop_node(t->c[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t * rk_loop_ctl(rcx_t * cx, const tree_t * t, const char * nm, IR_t * ω, IR_t ** res) {
    const char * lb = (t->n == 2 && t->c[1] && t->c[1]->t == TT_VAR) ? t->c[1]->v.sval : NULL;
    rk_loopfr_t * lf = rk_loop_find(cx, lb);
    if (t->n > 2 || (t->n == 2 && (!lb || !lf))) return NULL;
    IR_t * tgt = ω;
    if (lf && !strcmp(nm, "last")) tgt = lf->exit;
    if (lf && !strcmp(nm, "next")) tgt = lf->next;
    if (lf && !strcmp(nm, "redo") && !lf->redo) lf->redo = lrk_build(cx, IR_GOTO, NULL, ω);
    if (lf && !strcmp(nm, "redo")) tgt = lf->redo;
    IR_t * j = lrk_build(cx, IR_GOTO, tgt, tgt);
    *res = NULL;
    return j;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_str_subform(const char * nm) {
    static const char * const names[] = { "lc", "uc", "samecase", "tc", "tclc", "fc", "chomp", "chop", "flip", "wordcase", "trim-leading", "trim-trailing", "samemark", "substr", "substr-rw", "index",
        "rindex", NULL };
    for (int i = 0; names[i]; i++) if (!strcmp(nm, names[i])) return 1;
    return 0;
}
static long long rk_rx_fl(const tree_t * t) { return (t && t->n > 0 && t->c[0] && t->c[0]->t == TT_ILIT) ? t->c[0]->v.ival : 0; }
static void rk_rx_tag(IR_ref_t * r, const char * tg) { memcpy(r->sz, tg, 3); r->sz[3] = 0; }
static int rk_rx_tagged(const IR_ref_t * r, unsigned char b1) { return (unsigned char) r->sz[0] == 0xcf && (unsigned char) r->sz[1] == b1; }
static int rk_rx_atomic(const tree_t * a) {
    if (!a) return 0;
    if (a->t == TT_RX_ANY) return 1;
    if (a->t == TT_RX_LIT) return (rk_rx_fl(a) & 33) == 0 && a->slen > 0 && a->v.sval && (int) strlen(a->v.sval) == a->slen;
    if (a->t == TT_RX_CLASS) { if (a->v.ival & 1) return 0; for (int i = 0; i < a->n; i++) if (!a->c[i] || (a->c[i]->t != TT_RX_CRANGE && a->c[i]->t != TT_RX_CNAME)) return 0; return 1; }
    if (a->t == TT_RX_GROUP && a->n == 1) return rk_rx_atomic(a->c[0]);
    return 0;
}
static int rk_rx_has_cap(const tree_t * t) { if (!t) return 0; if (t->t == TT_RX_CAP) return 1; for (int i = 0; i < t->n; i++) if (rk_rx_has_cap(t->c[i])) return 1; return 0; }
int rk_rx_core(const tree_t * t) {
    if (!t) return 0;
    switch (t->t) {
        case TT_RX_SEQ:
        case TT_RX_GROUP:
        case TT_RX_ALT:
        if ((t->t == TT_RX_SEQ && t->v.ival) || (t->t == TT_RX_ALT && t->v.ival == 2)) return 0;
        for (int i = 0; i < t->n; i++) if (!rk_rx_core(t->c[i])) return 0;
        return 1;
        case TT_RX_LIT:
        case TT_RX_ANY:
        case TT_RX_CLASS:
        return rk_rx_atomic(t);
        case TT_RX_QUANT:
        return t->n == 3 && rk_rx_atomic(t->c[0]) && t->c[1] && t->c[1]->t == TT_ILIT && t->c[2] && t->c[2]->t == TT_ILIT;
        case TT_RX_CAP:
        return t->n == 1 && !rk_rx_has_cap(t->c[0]) && rk_rx_core(t->c[0]);
        case TT_RX_ANCHOR:
        return t->v.ival == 0 || t->v.ival == 1;
        case TT_RX_NULL:
        return 1;
        default:
        return 0;
    }
}
static int rk_rx_count_caps(const tree_t * t) { int n = 0; if (!t) return 0; if (t->t == TT_RX_CAP) n = 1; for (int i = 0; i < t->n; i++) n += rk_rx_count_caps(t->c[i]); return n; }
static void rk_rx_scan_groups(const tree_t * t, rkrx_groups_t * gr) {
    if (!t) return;
    if (t->t == TT_RX_CAP) {
        if (t->slen > 0 && t->v.sval) {
            int seen = 0;
            for (int i = 0; i < gr->nnamed; i++) if (!strcmp(gr->named[i], t->v.sval)) seen = 1;
            if (!seen) gr->named[gr->nnamed++] = t->v.sval;
        } else if ((int) t->v.ival > gr->maxp) gr->maxp = (int) t->v.ival;
    }
    for (int i = 0; i < t->n; i++) rk_rx_scan_groups(t->c[i], gr);
}
static int rk_rx_slot(const tree_t * cap, const rkrx_groups_t * gr) {
    if (cap->slen <= 0) return (int) cap->v.ival;
    for (int i = 0; i < gr->nnamed; i++) if (!strcmp(gr->named[i], cap->v.sval)) return gr->maxp + 1 + i;
    return 0;
}
static char * rk_rx_blob(const tree_t * a, long mn, long mx, long flags) {
    while (a->t == TT_RX_GROUP && a->n == 1) a = a->c[0];
    {
        size_t cap = 80 + (a->t == TT_RX_LIT ? (size_t) a->slen : (size_t) a->n * 48);
        char * b = (char *) ct_alloc(cap);
        int k = snprintf(b, cap, "%c%ld,%ld,%ld;", a->t == TT_RX_LIT ? 'L' : a->t == TT_RX_ANY ? 'A' : 'C', mn, mx, flags);
        if (a->t == TT_RX_LIT) { memcpy(b + k, a->v.sval, (size_t) a->slen); b[k + a->slen] = 0; return b; }
        for (int i = 0; a->t == TT_RX_CLASS && i < a->n; i++) {
            const tree_t * it = a->c[i];
            if (it->t == TT_RX_CRANGE) k += snprintf(b + k, cap - (size_t) k, "%sr%lld,%lld;", it->v.ival ? "-" : "", it->c[0]->v.ival, it->c[1]->v.ival);
            else { long long f = rk_rx_fl(it); k += snprintf(b + k, cap - (size_t) k, "%s%c%s;", (f & 2) ? "-" : "", (f & 1) ? 'N' : 'n', it->v.sval); }
        }
        return b;
    }
}
static void rk_rx_retag_alpha(IR_graph_t * g, int from, IR_t * target) {
    for (int k = from; k < g->n; k++) { IR_t * x = g->all[k]; if (x && x->γ.node == target && (unsigned char) x->γ.sz[0] == 0xce && (unsigned char) x->γ.sz[1] == 0xb2) lc_γ_to(x, target); }
}
static rkrx_t rk_rx_leaf(rcx_t * cx, IR_e op, IR_t * succ, IR_t * fail) { IR_t * nd = lc_build(cx->g, op, succ, NULL); lc_ω_to_β(nd, fail); return (rkrx_t) { nd, nd }; }
static rkrx_t rk_rx_seq(rcx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t * land, const rkrx_groups_t * gr) {
    IR_graph_t * g = cx->g;
    int ne = t->n;
    IR_t * S = lc_build(g, IR_GOTO, succ, NULL);
    IR_t * ent[ne > 0 ? ne : 1], * res[ne > 0 ? ne : 1];
    int lo[ne > 0 ? ne : 1];
    lc_ω_to_β(S, fail);
    for (int i = 0; i < ne; i++) {
        int before = g->n, rb = before;
        rkrx_t r = rk_rx_lower(cx, t->c[i], S, S, land, gr);
        IR_t * ti = NULL;
        while (rb < g->n && g->all[rb] && g->all[rb]->op == IR_GOTO && g->all[rb]->γ.node == S && g->all[rb]->ω.node == S && g->all[rb]->n_operands == 0) rb++;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == S) rk_rx_tag(&x->ω, "φ");
            if (x->γ.node == S) { if (x->op == IR_GOTO && x->ω.node == S) rk_rx_tag(&x->γ, "φ"); else { rk_rx_tag(&x->γ, "σ"); ti = x; } }
        }
        ent[i] = r.entry;
        res[i] = ti ? ti : ((rb < g->n) ? g->all[rb] : r.entry);
        lo[i] = before;
    }
    for (int i = 0; i < ne; i++) {
        IR_t * nxt = (i + 1 < ne) ? ent[i + 1] : succ;
        IR_t * prv = (i > 0) ? res[i - 1] : fail;
        int hi = (i + 1 < ne) ? lo[i + 1] : g->n;
        for (int k = lo[i]; k < g->n && k < hi; k++) {
            IR_t * x = g->all[k];
            if (!x || x == S) continue;
            if (x->ω.node == S && rk_rx_tagged(&x->ω, 0x86)) { x->ω.node = prv; rk_rx_tag(&x->ω, "β"); }
            if (x->γ.node == S && rk_rx_tagged(&x->γ, 0x86)) { x->γ.node = prv; rk_rx_tag(&x->γ, "β"); }
            if (x->γ.node == S && rk_rx_tagged(&x->γ, 0x83)) { x->γ.node = nxt; x->γ.sz[0] = 0; }
        }
    }
    for (int k = 0; k < g->n; k++) {
        IR_t * x = g->all[k];
        if (!x || x == S) continue;
        if (x->ω.node == S && rk_rx_tagged(&x->ω, 0x86)) { x->ω.node = fail; rk_rx_tag(&x->ω, "β"); }
        if (x->γ.node == S && rk_rx_tagged(&x->γ, 0x86)) { x->γ.node = fail; rk_rx_tag(&x->γ, "β"); }
        if (x->γ.node == S && rk_rx_tagged(&x->γ, 0x83)) { x->γ.node = succ; x->γ.sz[0] = 0; }
    }
    S->γ.node = succ;
    S->γ.sz[0] = 0;
    return (rkrx_t) { ne > 0 ? ent[0] : succ, ne > 0 ? res[ne - 1] : succ };
}
static rkrx_t rk_rx_alt(rcx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t * land, const rkrx_groups_t * gr) {
    IR_graph_t * g = cx->g;
    IR_t * A = lc_build(g, IR_MATCH_ALTERNATE, succ, NULL);
    lc_ω_to_β(A, fail);
    for (int i = 0; i < t->n; i++) {
        int before = g->n;
        rkrx_t r = rk_rx_lower(cx, t->c[i], A, A, land, gr);
        IR_t * ri = (before < g->n) ? g->all[before] : r.entry, * ti = NULL;
        for (int k = before; k < g->n; k++) {
            IR_t * x = g->all[k];
            if (!x) continue;
            if (x->ω.node == A) rk_rx_tag(&x->ω, "φ");
            if (x->γ.node == A) { if (x->op == IR_GOTO && x->ω.node == A) rk_rx_tag(&x->γ, "φ"); else { rk_rx_tag(&x->γ, "σ"); ti = x; } }
        }
        if (ti) ri = ti;
        ir_operand_push(A, r.entry);
        ir_operand_push(A, ri);
    }
    IR_LIT(A).ival = (long) t->n;
    return (rkrx_t) { A, A };
}
static rkrx_t rk_rx_lower(rcx_t * cx, const tree_t * t, IR_t * succ, IR_t * fail, IR_t * land, const rkrx_groups_t * gr) {
    IR_graph_t * g = cx->g;
    switch (t->t) {
        case TT_RX_SEQ:
        return rk_rx_seq(cx, t, succ, fail, land, gr);
        case TT_RX_ALT:
        return rk_rx_alt(cx, t, succ, fail, land, gr);
        case TT_RX_GROUP:
        return t->n == 1 ? rk_rx_lower(cx, t->c[0], succ, fail, land, gr) : rk_rx_seq(cx, t, succ, fail, land, gr);
        case TT_RX_LIT:
        { rkrx_t r = rk_rx_leaf(cx, IR_MATCH_LIT, succ, fail); IR_LIT(r.entry).sval = t->v.sval; return r; }
        case TT_RX_ANY:
        case TT_RX_CLASS:
        { rkrx_t r = rk_rx_leaf(cx, IR_MATCH_RKRUN, succ, fail); IR_LIT(r.entry).sval = rk_rx_blob(t, 1, 1, 0); return r; }
        case TT_RX_QUANT:
        {
            long mn = (long) t->c[1]->v.ival, mx = (long) t->c[2]->v.ival;
            rkrx_t r = rk_rx_leaf(cx, IR_MATCH_RKRUN, succ, fail);
            IR_LIT(r.entry).sval = rk_rx_blob(t->c[0], mn, mx < 0 ? 65535 : mx, (long) (t->v.ival & 3));
            return r;
        }
        case TT_RX_ANCHOR:
        { rkrx_t r = rk_rx_leaf(cx, t->v.ival == 0 ? IR_MATCH_POS : IR_MATCH_RPOS, succ, fail); IR_LIT(r.entry).ival = 0; return r; }
        case TT_RX_CAP:
        {
            int slot = rk_rx_slot(t, gr);
            IR_t * cape = lc_build(g, IR_MATCH_RKCAPE, succ, NULL);
            IR_t * caps = lc_build(g, IR_MATCH_RKCAPS, NULL, NULL);
            rkrx_t body;
            ir_operand_push(cape, land);
            ir_operand_push(caps, land);
            IR_LIT(cape).ival = slot;
            IR_LIT(caps).ival = slot;
            lc_ω_to_β(caps, fail);
            body = rk_rx_lower(cx, t->c[0], cape, caps, land, gr);
            lc_γ_to(caps, body.entry);
            lc_ω_to_β(cape, body.tail);
            return (rkrx_t) { caps, cape };
        }
        case TT_RX_VAR:
        {
            IR_t * mv = lc_build(g, IR_MATCH_VALUE, succ, NULL), * vr = NULL, * ve;
            tree_t * vt = ast_node_new(TT_VAR);
            vt->v.sval = t->v.sval;
            lc_ω_to_β(mv, fail);
            { int before = g->n; ve = lower_rv(cx, vt, mv, fail, &vr); rk_rx_retag_alpha(g, before, mv); }
            if (vr) ir_operand_push(mv, vr);
            return (rkrx_t) { ve ? ve : mv, mv };
        }
        default:
        return rk_rx_leaf(cx, IR_GOTO, succ, fail);
    }
}
IR_t * rk_lower_smatch_boxes(rcx_t * cx, const tree_t * t, IR_t * γ, IR_t * ω, IR_t ** res) {
    IR_graph_t * g = cx->g;
    const tree_t * rx = t->c[1]->c[1];
    int ncap = rk_rx_count_caps(rx);
    const char * named[ncap > 0 ? ncap : 1];
    rkrx_groups_t gr = { -1, 0, named };
    IR_t * last, * clr, * head, * release, * land, * sv = NULL, * se;
    rkrx_t p;
    rk_rx_scan_groups(rx, &gr);
    {
        int n = gr.maxp + 1 + gr.nnamed;
        size_t cap = 24;
        char * names;
        for (int i = 0; i < gr.nnamed; i++) cap += strlen(gr.named[i]) + 1;
        cap += (size_t) (gr.maxp + 1) + 1;
        names = (char *) ct_alloc(cap);
        {
            int k = snprintf(names, cap, "%d\n", n);
            for (int i = 0; i <= gr.maxp; i++) names[k++] = '\n';
            for (int i = 0; i < gr.nnamed; i++) k += snprintf(names + k, cap - (size_t) k, "%s\n", gr.named[i]);
            names[k] = 0;
        }
        last = lrk_build(cx, IR_CALL, γ, ω);
        IR_LIT(last).sval = (char *) "__rk_match_last";
        clr = lrk_build(cx, IR_CALL, last, ω);
        IR_LIT(clr).sval = (char *) "__rk_match_clear";
        head = lc_build(g, IR_MATCH_BEGIN, NULL, clr);
        release = lc_build(g, IR_MATCH_END, last, NULL);
        ir_operand_push(release, head);
        land = lc_build(g, IR_MATCH_RKLAND, release, NULL);
        IR_LIT(land).sval = names;
    }
    p = rk_rx_lower(cx, rx, land, head, land, &gr);
    lc_ω_to_β(land, p.tail);
    lc_γ_to(head, p.entry);
    { int before = g->n; se = lower_rv(cx, t->c[0], head, ω, &sv); rk_rx_retag_alpha(g, before, head); }
    if (sv) ir_operand_push(head, sv);
    *res = last;
    return se ? se : head;
}
int rk_rx_boxes_on(void) { const char * e = getenv("RK_RX_BOXES"); return !(e && e[0] == '0'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_refuse(const tree_t * t, const char * what) { fprintf(stderr, "scrip: raku: line %d: REFUSED -- %s\n", t ? t->line : 0, what); exit(2); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_listops_to_methcalls(tree_t * t, int is_iter_src) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_listops_to_methcalls(t->c[i], t->t == TT_ITERATE && i == 0);
    if (!is_iter_src && (t->t == TT_SORT || t->t == TT_REVERSE) && t->n == 1 && t->c[0]) {
        tree_t * m = ast_node_new(TT_METHCALL);
        m->line = t->line;
        ast_push(m, t->c[0]);
        ast_push(m, leaf_sval2(TT_QLIT, t->t == TT_SORT ? "sort" : "reverse"));
        *t = *m;
        return;
    }
    if (!is_iter_src && (t->t == TT_MAP || t->t == TT_GREP) && t->n == 1 && t->c[0] && t->c[0]->t == TT_FNC && t->c[0]->v.sval && !strcmp(t->c[0]->v.sval, "__rk_arr") && t->c[0]->n >= 3) {
        tree_t * a = t->c[0];
        t->n = 0;
        for (int i = 1; i < a->n; i++) ast_push(t, a->c[i]);
    }
    if ((t->t != TT_MAP && t->t != TT_GREP) || t->n < 2 || !t->c[0] || !t->c[1]) return;
    if (is_iter_src && (t->n > 2 || t->c[1]->t == TT_TO || t->c[1]->t == TT_FNC)) return;
    tree_t * blk = t->c[0];
    int tyobj = t->t == TT_GREP && blk->t == TT_FNC && blk->v.sval && !strcmp(blk->v.sval, "__rk_typeobj");
    if (blk->t != TT_ANON_BLOCK && !tyobj) { tree_t * body = ast_node_new(TT_SEQ_EXPR); ast_push(body, t->c[0]); blk = ast_node_new(TT_ANON_BLOCK); ast_push(blk, body); }
    int npos = 0;
    for (int i = 1; i < t->n; i++) if (!(t->c[i] && t->c[i]->t == TT_FNC && t->c[i]->v.sval && !strcmp(t->c[i]->v.sval, "__rk_pair"))) npos++;
    tree_t * src = NULL;
    if (npos == 1) {
        for (int i = 1; i < t->n && !src; i++) if (!(t->c[i] && t->c[i]->t == TT_FNC && t->c[i]->v.sval && !strcmp(t->c[i]->v.sval, "__rk_pair"))) src = t->c[i];
    } else {
        src = ast_node_new(TT_FNC);
        src->v.sval = (char *) "__rk_arr";
        ast_push(src, leaf_sval2(TT_VAR, "__rk_arr"));
        for (int i = 1; i < t->n; i++) if (!(t->c[i] && t->c[i]->t == TT_FNC && t->c[i]->v.sval && !strcmp(t->c[i]->v.sval, "__rk_pair"))) ast_push(src, t->c[i]);
    }
    tree_t * m = ast_node_new(TT_METHCALL);
    m->line = t->line;
    ast_push(m, src);
    ast_push(m, leaf_sval2(TT_QLIT, t->t == TT_MAP ? "map" : "grep"));
    ast_push(m, blk);
    for (int i = 1; i < t->n; i++) if (t->c[i] && t->c[i]->t == TT_FNC && t->c[i]->v.sval && !strcmp(t->c[i]->v.sval, "__rk_pair")) ast_push(m, t->c[i]);
    *t = *m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_alias_walk(tree_t * t, const rk_ns_t * attrs, const rk_ns_t * locals) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t * c = t->c[i];
        if (c && c->t == TT_VAR && !c->n && c->v.sval && rk_ns_has(attrs, c->v.sval) && !rk_ns_has(locals, c->v.sval) && !(t->t == TT_FNC && i == 0) && t->t != TT_SUB_DECL) {
            tree_t * f = ast_node_new(TT_TWIGIL_FIELD);
            f->line = c->line;
            f->v.sval = c->v.sval;
            t->c[i] = f;
            continue;
        }
        rk_alias_walk(c, attrs, locals);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_local_names(const tree_t * t, rk_ns_t * out) {
    if (!t) return;
    if ((t->t == TT_DECL && t->n > 1 && t->c[1] && t->c[1]->t == TT_VAR) || t->t == TT_ASSIGN) {
        const tree_t * v = t->t == TT_DECL ? t->c[1] : (t->n > 0 ? t->c[0] : NULL);
        if (v && v->t == TT_VAR && v->v.sval && (v->slen & 4)) rk_ns_add(out, v->v.sval);
    }
    for (int i = 0; i < t->n; i++) rk_local_names(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_attr_aliases(tree_t * t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_attr_aliases(t->c[i]);
    if (t->t != TT_CLASS_DECL && t->t != TT_ROLE_DECL) return;
    rk_ns_t attrs = { 0 };
    for (int j = 1; j < t->n; j++) {
        const tree_t * m = t->c[j];
        if (m && m->t == TT_VAR && !m->n && m->v.sval && (isalpha((unsigned char) m->v.sval[0]) || m->v.sval[0] == '_')) rk_ns_add(&attrs, m->v.sval);
    }
    if (!attrs.n) return;
    for (int j = 1; j < t->n; j++) {
        tree_t * m = t->c[j];
        if (!m || m->t != TT_SUB_DECL) continue;
        rk_ns_t locals = { 0 };
        for (int k = 1; k <= (int) m->v.ival && k < m->n; k++) if (m->c[k] && m->c[k]->t == TT_VAR && m->c[k]->v.sval) rk_ns_add(&locals, m->c[k]->v.sval);
        rk_local_names(m, &locals);
        rk_alias_walk(m, &attrs, &locals);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_name_anon_subsig(tree_t * t, int * ctr) {
    if (!t) return;
    if (t->t == TT_SUB_DECL) for (int i = 1; i < t->n && t->c[i] && t->c[i]->t == TT_VAR; i++) {
        tree_t * pv = t->c[i];
        int sub = 0;
        for (int j = 0; j < pv->n; j++) if (pv->c[j] && pv->c[j]->t == TT_SEQ_EXPR) sub = 1;
        if (sub && (!pv->v.sval || !*pv->v.sval)) { char nb[32]; snprintf(nb, sizeof nb, "__anonsig%d", (*ctr)++); pv->v.sval = (char *) intern(nb); }
    }
    for (int i = 0; i < t->n; i++) rk_name_anon_subsig(t->c[i], ctr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_shd_subsig(const tree_t * pv, rk_ns_t * out) {
    for (int j = 0; pv && j < pv->n; j++) {
        const tree_t * sq = pv->c[j];
        if (!sq || sq->t != TT_SEQ_EXPR) continue;
        for (int i = 0; i < sq->n; i++) {
            const tree_t * sp = sq->c[i];
            const tree_t * v = (sp && sp->t == TT_ASSIGN && sp->n) ? sp->c[0] : sp;
            if (!v || v->t != TT_VAR) continue;
            int aliased = 0;
            for (int a = 0; a < v->n; a++) if (v->c[a] && v->c[a]->t == TT_VAR && (v->c[a]->slen & RK_PV_ALIAS)) { rk_ns_add(out, v->c[a]->v.sval); aliased = 1; }
            if (v->v.sval && !aliased) rk_ns_add(out, v->v.sval);
            rk_shd_subsig(v, out);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_subsig_prepend(tree_t * body, const tree_t * pv, const char * src) {
    int cnt = rk_subsig_count(pv);
    const tree_t * plan[cnt + 1];
    int nd = rk_subsig_plan(pv, src, plan, 0);
    if (!nd) return body;
    tree_t * sq = ast_node_new(TT_SEQ_EXPR);
    for (int i = 0; i < nd; i++) ast_push(sq, (tree_t *) plan[i]);
    if (body && body->t == TT_SEQ_EXPR) for (int i = 0; i < body->n; i++) ast_push(sq, body->c[i]);
    else if (body) ast_push(sq, body);
    return sq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_subsig_blocks(tree_t * t, int * ctr) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) rk_subsig_blocks(t->c[i], ctr);
    if (t->t == TT_EVERY && t->n == 2 && t->c[0] && t->c[0]->t == TT_ITERATE && t->c[0]->n == 2 && t->c[0]->c[1] && t->c[0]->c[1]->t == TT_SEQ_EXPR) {
        tree_t * gen = t->c[0], * sg = gen->c[1];
        char nb[32];
        snprintf(nb, sizeof nb, "__anonsig%d", (*ctr)++);
        const char * nm = (gen->v.sval && *gen->v.sval) ? gen->v.sval : intern(nb);
        gen->v.sval = (char *) nm;
        tree_t * pv = leaf_sval2(TT_VAR, nm);
        ast_push(pv, sg);
        t->c[1] = rk_subsig_prepend(t->c[1], pv, nm);
        gen->n = 1;
    }
    if (t->t == TT_ANON_BLOCK) for (int i = 1; i < t->n && t->c[i] && t->c[i]->t == TT_VAR; i++) {
        tree_t * pv = t->c[i];
        int sub = 0;
        for (int j = 0; j < pv->n; j++) if (pv->c[j] && pv->c[j]->t == TT_SEQ_EXPR) sub = 1;
        if (!sub) continue;
        char nb[32];
        snprintf(nb, sizeof nb, "__anonsig%d", (*ctr)++);
        if (!pv->v.sval || !*pv->v.sval) pv->v.sval = (char *) intern(nb);
        t->c[0] = rk_subsig_prepend(t->c[0], pv, pv->v.sval);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t * rk_pl_store(tree_t * target, tree_t * rhs) { tree_t * a = rk_t2(TT_ASSIGN, target, rhs); a->v.ival = 1; return a; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_pl_destructure(tree_t * t, int * uid) {
    char nb[40];
    int nt = t->n - 2, slurped = 0;
    tree_t * rhs = t->c[1], * tg[nt + 1], * tv;
    int blank = rhs && rhs->t == TT_FNC && rhs->v.sval && !strcmp(rhs->v.sval, "__rk_arr") && rhs->n <= 1;
    snprintf(nb, sizeof nb, "__destr_%d", (*uid)++);
    tv = leaf_sval2(TT_VAR, intern(nb));
    for (int i = 0; i < nt; i++) tg[i] = t->c[2 + i];
    if (nt && tg[0] && tg[0]->t == TT_VAR) tv->slen |= tg[0]->slen & (4 | RK_VAR_OUR);
    t->t = TT_SEQ_EXPR;
    t->v.sval = NULL;
    t->n = 0;
    ast_push(t, rk_pl_store(tv, rhs));
    for (int i = 0; i < nt; i++) {
        tree_t * get = rk_fnc2("__rk_arr_at", leaf_sval2(TT_VAR, tv->v.sval), rk_ti(i));
        if (slurped) get = rk_fnc2("__rk_undef", NULL, NULL);
        else if (tg[i] && tg[i]->t == TT_VAR && tg[i]->v.sval && tg[i]->v.sval[0] == '@') {
            if (blank) get = rk_fnc2("__rk_undef", NULL, NULL);
            else { get = rk_t2(TT_METHCALL, leaf_sval2(TT_VAR, tv->v.sval), leaf_sval2(TT_QLIT, "skip")); ast_push(get, rk_ti(i)); }
            slurped = 1;
        }
        ast_push(t, rk_pl_store(tg[i], get));
    }
}
